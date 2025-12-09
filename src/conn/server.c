#ifndef CONN_SERVER_C
#define CONN_SERVER_C

#include "./message.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum ConnServerState
{
    ConnServerState_None,

    ConnServerState_Error,

    ConnServerState_Starting,
    ConnServerState_Stopping,

    ConnServerState_SendingTurn,
    ConnServerState_WaitingMove,
}
ConnServerState;

typedef RnArray(ConnMessage) ArrayConnMessage;

typedef struct ConnSession
{
    RnSocketTCP*     socket;
    ArrayConnMessage messages;
    ConnPlayer       player;

    u8 writing[CONN_MESSAGE_SIZE];
    u8 reading[CONN_MESSAGE_SIZE];
}
ConnSession;

typedef RnArray(ConnSession) ArrayConnSession;

typedef struct ConnServer
{
    ConnServerState  state;
    RnAsyncIOQueue*  queue;
    RnSocketTCP*     listener;
    ArrayConnSession sessions;

    ConnBoard board;

    ssize playerCount;
    ssize playerTurn;
}
ConnServer;

void
connMessageShow(ConnMessage message)
{
    u8    buffer[1024] = {0};
    ssize count        = connMessageToString(message, buffer, 1024);

    if (count != 0)
        printf("%.*s\n", ((int) count), buffer);
}

void
connBoardShow(ConnBoard* self)
{
    char* colors[] = {"\x1b[0m", "\x1b[34m", "\x1b[31m"};

    for (ssize i = 0; i < self->width; i += 1)
        printf("%s", i == 0 ? "+----+" : "----+");

    printf("\n");

    for (ssize r = 0; r < self->height; r += 1) {
        printf("|");

        for (ssize c = 0; c < self->width; c += 1) {
            u32 value = self->values[self->width * r + c];

            if (value != 0)
                printf("%s%3lu%s |", colors[value], value, colors[0]);
            else
                printf("    |");
        }

        printf("\n");

        for (ssize i = 0; i < self->width; i += 1)
            printf("%s", i == 0 ? "+----+" : "----+");

        printf("\n");
    }
}

void
connServerAccept(ConnServer* self, RnMemoryArena* arena)
{
    RnSocketTCP* socket = rnSocketTCPReserve(arena);

    b32 status = rnAsyncIOQueueSubmit(self->queue,
        rnAsyncIOTaskAccept(arena, 0, self->listener, socket));

    if (status == 0) self->state = ConnServerState_Error;
}

void
connServerWrite(ConnServer* self, RnMemoryArena* arena, ConnSession* session, ConnMessage message)
{
    if (rnArrayIsFull(&session->messages) != 0) self->state = ConnServerState_Error;

    if (rnArrayIsEmpty(&session->messages) != 0) {
        ssize stop = connMessageEncode(message, session->writing, CONN_MESSAGE_SIZE);

        printf("0x%016llx ", ((usize) session));
        connMessageShow(message);

        RnSocketTCP* socket = session->socket;
        u8*          values = session->writing;
        ssize        start  = 0;

        b32 status = rnAsyncIOQueueSubmit(self->queue,
            rnAsyncIOTaskWrite(arena, session, socket, values, start, stop));

        if (status == 0) self->state = ConnServerState_Error;
    }
    else rnArrayPushBack(&session->messages, message);
}

void
connServerBroadcast(ConnServer* self, RnMemoryArena* arena, ConnSession* session, ConnMessage message)
{
    for (ssize i = 0; i < rnArrayCount(&self->sessions); i += 1) {
        ConnSession* value = rnArrayGetRef(&self->sessions, i);

        if (value != 0 && value != session)
            connServerWrite(self, arena, value, message);
    }
}

void
connServerRead(ConnServer* self, RnMemoryArena* arena, ConnSession* session)
{
    RnSocketTCP* socket = session->socket;
    u8*          values = session->reading;
    ssize        start  = 0;
    ssize        stop   = CONN_MESSAGE_SIZE;

    b32 status = rnAsyncIOQueueSubmit(self->queue,
        rnAsyncIOTaskRead(arena, session, socket, values, start, stop));

    if (status == 0) self->state = ConnServerState_Error;
}

b32
connSessionCreate(ConnSession* self, RnMemoryArena* arena, RnSocketTCP* socket)
{
    if (self == 0 || socket == 0) return 0;

    rnArrayCreate(&self->messages, arena, 16);

    self->socket = socket;

    if (rnArraySize(&self->messages) == 0)
        return 0;

    return 1;
}

void
connSessionDestroy(ConnSession* self)
{
    rnSocketTCPDestroy(self->socket);
}

void
connServerCreate(ConnServer* self, RnMemoryArena* arena)
{
    RnAddressIP address = rnAddressIPv4Empty();
    u16         port    = 50000;

    self->queue    = rnAsyncIOQueueReserve(arena);
    self->listener = rnSocketTCPReserve(arena);

    rnArrayCreate(&self->sessions, arena, 2);

    connBoardCreate(&self->board, arena, 7, 5);

    rnAsyncIOQueueCreate(self->queue);
    rnSocketTCPCreate(self->listener, address, port);

    rnSocketTCPBind(self->listener);
    rnSocketTCPListen(self->listener);

    self->state = ConnServerState_Starting;
}

void
connServerStart(ConnServer* self, RnMemoryArena* arena)
{
    connServerAccept(self, arena);
}

void
connServerStop(ConnServer* self)
{
    ConnSession value = {0};

    while (rnArrayPopBack(&self->sessions, &value) != 0)
        connSessionDestroy(&value);
}

void
connServerDestroy(ConnServer* self)
{
    rnSocketTCPDestroy(self->listener);
    rnAsyncIOQueueDestroy(self->queue);
}

b32
connServerIsActive(ConnServer* self)
{
    if (self->state == ConnServerState_Stopping) return 0;
    if (self->state == ConnServerState_Error)    return 0;

    return 1;
}

void
connServerOnMessage(ConnServer* self, RnMemoryArena* arena, ConnSession* session, ConnMessage message)
{
    switch (message.kind) {
        case ConnMessage_Join: {
            if (self->state != ConnServerState_Starting) break;

            if (session->player.code == 0) {
                // TODO(gio): Add accept/welcome message

                session->player.flags  = message.join.clientFlags;
                session->player.code   = ((u32) self->playerCount + 1);
                session->player.symbol = ((u8)  self->playerCount + 'a');

                connServerBroadcast(self, arena, 0, connMessageData(session->player));

                for (ssize i = 0; i < self->playerCount; i += 1) {
                    ConnSession* value = rnArrayGetRef(&self->sessions, i);

                    connServerWrite(self, arena, session,
                        connMessageData(value->player));
                }

                self->playerCount += 1;

                // TODO(gio): Maybe move elsewhere?
                if (self->playerCount == 2)
                    self->state = ConnServerState_SendingTurn;
            }
            else {
                // TODO(gio): Add reject message
            }
        } break;

        case ConnMessage_Move: {
            if (self->state != ConnServerState_WaitingMove) break;

            u32 clientCode = message.move.clientCode;
            u32 column     = message.move.column;

            connBoardInsert(&self->board, column, clientCode);
            connBoardShow(&self->board);

            connServerBroadcast(self, arena, session, message);

            u32 winner = 0;
            b32 full   = connBoardIsFull(&self->board);

            if (connBoardIsWinner(&self->board, column, clientCode) != 0)
                winner = clientCode;

            if (winner != 0 || full != 0) {
                connServerBroadcast(self, arena, 0,
                    connMessageResult(winner));

                self->state = ConnServerState_Stopping;
            }
            else self->state = ConnServerState_SendingTurn;
        } break;

        case ConnMessage_Quit: {
            connServerBroadcast(self, arena, 0,
                connMessageResult(0));

            self->state = ConnServerState_Stopping;
        } break;

        default: break;
    }
}

void
connServerUpdateMessage(ConnServer* self, RnMemoryArena* arena)
{
    RnAsyncIOEvent event = {0};

    while (rnAsyncIOQueuePoll(self->queue, &event, 10) != 0) {
        switch (event.kind) {
            case RnAsyncIOEvent_Error: self->state = ConnServerState_Error; break;

            case RnAsyncIOEvent_Accept: {
                b32 status = 0;

                if (event.accept.socket != 0) {
                    if (rnArrayPushBack(&self->sessions, (ConnSession) {0}) != 0) {
                        ssize        index   = rnArrayBack(&self->sessions);
                        ConnSession* session = rnArrayGetRef(&self->sessions, index);

                        if (connSessionCreate(session, arena, event.accept.socket) != 0) {
                            printf("[INFO] Accepted!\n");

                            // TODO(gio): Maybe add an "onAccept" function?
                            connServerRead(self, arena, session);

                            if (rnArrayIsFull(&self->sessions) == 0)
                                connServerAccept(self, arena);

                            status = 1;
                        }
                    }
                }

                if (status == 0) self->state = ConnServerState_Error;
            } break;

            case RnAsyncIOEvent_Write: {
                ConnSession* session = ((ConnSession*) event.ctxt);
                ConnMessage  message = {0};

                if (rnArrayIsEmpty(&session->messages) != 0) break;

                rnArrayPopFront(&session->messages, &message);

                ssize stop = connMessageEncode(message, session->writing, CONN_MESSAGE_SIZE);

                printf("0x%016llx ", ((usize) session));
                connMessageShow(message);

                RnSocketTCP* socket = session->socket;
                u8*          values = session->writing;
                ssize        start  = 0;

                b32 status = rnAsyncIOQueueSubmit(self->queue,
                    rnAsyncIOTaskWrite(arena, session, socket, values, start, stop));

               if (status == 0) self->state = ConnServerState_Error;
            } break;

            case RnAsyncIOEvent_Read: {
                ConnSession* session = ((ConnSession*) event.ctxt);
                ConnMessage  message = connMessageDecode(event.read.values, event.read.stop);

                if (message.length <= 0 || event.read.stop < message.length) {
                    RnSocketTCP* socket = event.read.socket;
                    u8*          values = event.read.values;
                    ssize        start  = event.read.stop;
                    ssize        stop   = CONN_MESSAGE_SIZE;

                    b32 status = rnAsyncIOQueueSubmit(self->queue,
                        rnAsyncIOTaskRead(arena, event.ctxt, socket, values, start, stop));

                    if (status == 0) self->state = ConnServerState_Error;
                }
                else {
                    printf("0x%016llx ", ((usize) session));
                    connMessageShow(message);
                    connServerOnMessage(self, arena, session, message);
                }
            } break;

            default: break;
        }

        if (connServerIsActive(self) == 0) break;
    }
}

void
connServerUpdate(ConnServer* self, RnMemoryArena* arena)
{
    connServerUpdateMessage(self, arena);

    if (self->state == ConnServerState_SendingTurn) {
        self->playerTurn = (self->playerTurn % self->playerCount) + 1;

        ConnSession* session = rnArrayGetRef(&self->sessions, self->playerTurn - 1);

        connServerBroadcast(self, arena, 0,
            connMessageTurn(self->playerTurn));

        connServerRead(self, arena, session);

        self->state = ConnServerState_WaitingMove;
    }
}

int
main(int argc, char** argv)
{
    srand(time(0));

    RnMemoryArena arena = rnSystemMemoryReserve(rnMemoryMiB(2));

    ConnServer server = {0};

    connServerCreate(&server, &arena);
    connServerStart(&server, &arena);

    while (connServerIsActive(&server) != 0)
        connServerUpdate(&server, &arena);

    connServerStop(&server);
    connServerDestroy(&server);
}

#endif // CONN_SERVER_C
