#ifndef CONN_CLIENT_C
#define CONN_CLIENT_C

#include "./message.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum ConnClientState
{
    ConnClientState_None,

    ConnClientState_Error,

    ConnClientState_Starting,
    ConnClientState_Stopping,

    ConnClientState_WaitingTurn,
    ConnClientState_SendingMove,
    ConnClientState_WaitingMove,
}
ConnClientState;

typedef RnArray(ConnMessage) ArrayConnMessage;
typedef RnArray(ConnPlayer)  ArrayConnPlayer;

typedef struct ConnClient
{
    ConnClientState  state;
    RnAsyncIOQueue*  queue;
    RnSocketTCP*     socket;
    ArrayConnMessage messages;
    ArrayConnPlayer  players;

    u32 clientCode;
    u32 column;

    u8 writing[CONN_MESSAGE_SIZE];
    u8 reading[CONN_MESSAGE_SIZE];
}
ConnClient;

void
connClientConnect(ConnClient* self, RnMemoryArena* arena)
{
    RnAddressIP address = rnAddressIPv4Local();
    u16         port    = 50000;

    b32 status = rnAsyncIOQueueSubmit(self->queue,
        rnAsyncIOTaskConnect(arena, 0, self->socket, address, port));

    if (status == 0) self->state = ConnClientState_Error;
}

void
connClientWrite(ConnClient* self, RnMemoryArena* arena, ConnMessage message)
{
    if (rnArrayIsFull(&self->messages) != 0) self->state = ConnClientState_Error;

    if (rnArrayIsEmpty(&self->messages) != 0) {
        ssize stop = connMessageEncode(message, self->writing, CONN_MESSAGE_SIZE);

        RnSocketTCP* socket = self->socket;
        u8*          values = self->writing;
        ssize        start  = 0;

        b32 status = rnAsyncIOQueueSubmit(self->queue,
            rnAsyncIOTaskWrite(arena, 0, socket, values, start, stop));

        if (status == 0) self->state = ConnClientState_Error;
    }
    else rnArrayPushBack(&self->messages, message);
}

void
connClientRead(ConnClient* self, RnMemoryArena* arena)
{
    RnSocketTCP* socket = self->socket;
    u8*          values = self->reading;
    ssize        start  = 0;
    ssize        stop   = CONN_MESSAGE_SIZE;

    b32 status = rnAsyncIOQueueSubmit(self->queue,
        rnAsyncIOTaskRead(arena, 0, socket, values, start, stop));

    if (status == 0) self->state = ConnClientState_Error;
}

void
connClientCreate(ConnClient* self, RnMemoryArena* arena)
{
    self->queue  = rnAsyncIOQueueReserve(arena);
    self->socket = rnSocketTCPReserve(arena);

    rnArrayCreate(&self->messages, arena, 16);

    // TODO(gio): move to accept/reject message
    rnArrayCreate(&self->players, arena, 2);

    rnAsyncIOQueueCreate(self->queue);

    self->state = ConnClientState_Starting;
}

void
connClientStart(ConnClient* self, RnMemoryArena* arena)
{
    rnSocketTCPCreate(self->socket, rnAddressIPv4Empty(), 0);

    connClientConnect(self, arena);
}

void
connClientStop(ConnClient* self)
{
    rnSocketTCPDestroy(self->socket);
}

void
connClientDestroy(ConnClient* self)
{
    rnAsyncIOQueueDestroy(self->queue);
}

b32
connClientIsActive(ConnClient* self)
{
    if (self->state == ConnClientState_Stopping) return 0;
    if (self->state == ConnClientState_Error)    return 0;

    return 1;
}

void
connClientOnMessage(ConnClient* self, RnMemoryArena* arena, ConnMessage message)
{
    switch (message.kind) {
        case ConnMessage_Data: {
            printf("(Data) { clientFlags = %u, clientCode = %lu, clientSymbol = '%c' }\n",
                message.data.clientFlags, message.data.clientCode, message.data.clientSymbol);

            if (message.data.clientCode > 0) {
                ConnPlayer value = {
                    .flags  = message.data.clientFlags,
                    .code   = message.data.clientCode,
                    .symbol = message.data.clientSymbol,
                };

                rnArrayInsert(&self->players, value.code - 1, value);

                if (self->clientCode == 0)
                    self->clientCode = value.code;
            }

            connClientRead(self, arena);

            self->state = ConnClientState_WaitingTurn;
        } break;

        case ConnMessage_Turn: {
            printf("(Turn) { clientCode = %lu }\n",
                message.turn.clientCode);

            if (self->state != ConnClientState_WaitingTurn) break;

            if (self->clientCode != message.turn.clientCode) {
                self->state = ConnClientState_WaitingMove;

                connClientRead(self, arena);
            } else
                self->state = ConnClientState_SendingMove;
        } break;

        case ConnMessage_Move: {
            printf("(Move) { clientCode = %lu, column = %lu }\n",
                message.move.clientCode, message.move.column);

            if (self->state != ConnClientState_WaitingMove) break;

            self->state = ConnClientState_WaitingTurn;

            connClientRead(self, arena);
        } break;

        case ConnMessage_Result: {
            printf("(Result) { clientCode = %lu }\n",
                message.result.clientCode);

            if (message.result.clientCode != 0) {
                if (message.result.clientCode == self->clientCode)
                    printf("Vittoria!\n");
                else
                    printf("Sconfitta...\n");
            }
            else printf("Pareggio.\n");

            self->state = ConnClientState_Stopping;
        } break;

        default: break;
    }
}

void
connClientUpdateMessage(ConnClient* self, RnMemoryArena* arena)
{
    RnAsyncIOEvent event = {0};

    while (rnAsyncIOQueuePoll(self->queue, &event, 10) != 0) {
        switch (event.kind) {
            case RnAsyncIOEvent_Error: self->state = ConnClientState_Error; break;

            case RnAsyncIOEvent_Connect: {
                b32 status = 0;

                if (event.connect.status != 0) {
                    printf("[INFO] Connected!\n");

                    // TODO(gio): Maybe add an "onConnect" function?
                    connClientWrite(self, arena,
                        connMessageJoin(ConnClient_Player));

                    connClientRead(self, arena);

                    status = 1;
                }

                if (status == 0) self->state = ConnClientState_Error;
            } break;

            case RnAsyncIOEvent_Write: {
                if (rnArrayIsEmpty(&self->messages) != 0) break;

                ConnMessage message = {0};

                rnArrayPopFront(&self->messages, &message);

                ssize stop = connMessageEncode(message, self->writing, CONN_MESSAGE_SIZE);

                RnSocketTCP* socket = self->socket;
                u8*          values = self->writing;
                ssize        start  = 0;

                b32 status = rnAsyncIOQueueSubmit(self->queue,
                    rnAsyncIOTaskWrite(arena, 0, socket, values, start, stop));

                if (status == 0) self->state = ConnClientState_Error;
            } break;

            case RnAsyncIOEvent_Read: {
                ConnMessage message = connMessageDecode(event.read.values, event.read.stop);

                if (message.length <= 0 || event.read.stop < message.length) {
                    RnSocketTCP* socket = event.read.socket;
                    u8*          values = event.read.values;
                    ssize        start  = event.read.stop;
                    ssize        stop   = CONN_MESSAGE_SIZE;

                    b32 status = rnAsyncIOQueueSubmit(self->queue,
                        rnAsyncIOTaskRead(arena, event.ctxt, socket, values, start, stop));

                    if (status == 0) self->state = ConnClientState_Error;
                }
                else connClientOnMessage(self, arena, message);
            } break;

            default: break;
        }

        if (connClientIsActive(self) == 0) break;
    }
}

void
connClientUpdate(ConnClient* self, RnMemoryArena* arena)
{
    connClientUpdateMessage(self, arena);

    self->column = (rand() % 10) + 1;

    if (self->state == ConnClientState_SendingMove) {
        u32 clientCode = self->clientCode;
        u32 column     = self->column;

        connClientWrite(self, arena,
            connMessageMove(clientCode, column));

        self->state = ConnClientState_WaitingTurn;

        connClientRead(self, arena);
    }
}

int
main(int argc, char** argv)
{
    srand(time(0));

    RnMemoryArena arena = rnSystemMemoryReserve(rnMemoryMiB(2));

    ConnClient client = {0};

    connClientCreate(&client, &arena);
    connClientStart(&client, &arena);

    while (connClientIsActive(&client) != 0)
        connClientUpdate(&client, &arena);

    connClientStop(&client);
    connClientDestroy(&client);
}

#endif // CONN_CLIENT_C
