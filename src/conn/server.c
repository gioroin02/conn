#ifndef CONN_SERVER_C
#define CONN_SERVER_C

#include "server.h"

static void
connMessageShow(ConnMessage message)
{
    u8 buffer[1024];

    pxMemorySet(buffer, sizeof buffer, 0x00);

    ssize count = connMessageToString(message, buffer, 1024);

    if (count > 0)
        printf("%.*s", ((int) count), buffer);
}

static void
connBoardShow(ConnBoard* self)
{
    ssize index = 0;
    ssize row   = 0;
    ssize col   = 0;

    char* colors[] = {"\x1b[0m", "\x1b[44m", "\x1b[41m"};

    for (index = 0; index < self->width; index += 1)
        printf("%s", index == 0 ? "+-----+" : "-----+");

    printf("\n");

    for (row = 0; row < self->height; row += 1) {
        printf("| ");

        for (col = 0; col < self->width; col += 1) {
            u32 value = connBoardGet(self, col, row);

            if (value > 0 && value < 3)
                printf("%s %lu %s | ", colors[value], value, colors[0]);
            else
                printf("    | ");
        }

        printf("\n");

        for (index = 0; index < self->width; index += 1)
            printf("%s", index == 0 ? "+-----+" : "-----+");

        printf("\n");
    }
}

b32
connServerStateIsEqual(ConnServer* self, ConnServerState state)
{
    return self->state_curr == state ? 1 : 0;
}

b32
connServerStateIsActive(ConnServer* self)
{
    if (connServerStateIsEqual(self, ConnServerState_Stopping) != 0) return 0;
    if (connServerStateIsEqual(self, ConnServerState_Error) != 0)    return 0;

    return 1;
}

void
connServerStateSet(ConnServer* self, ConnServerState state)
{
    self->state_prev = self->state_curr;
    self->state_curr = state;

    connServerOnStateChange(self, self->state_prev, self->state_curr);
}

void
connServerStateSetError(ConnServer* self)
{
    connServerStateSet(self, ConnServerState_Error);
}

b32
connServerCreate(ConnServer* self, PxMemoryArena* arena)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    self->async        = pxAsyncReserve(arena);
    self->listener     = pxSocketTcpReserve(arena);
    self->player_count = 0;
    self->player_turn  = 0;

    if (pxAsyncCreate(self->async, arena, pxMemoryKiB(64)) == 0)
        return 0;

    PxAddressIp address = pxAddressIp4Empty();
    u16         port    = 50000;

    if (pxSocketTcpCreate(self->listener, address, port) == 0)
        return 0;

    pxSocketTcpBind(self->listener);
    pxSocketTcpListen(self->listener);

    if (pxArrayCreate(&self->sessions, arena, 2) == 0) return 0;

    ssize index = 0;

    for (index = 0; index < pxArraySize(&self->sessions); index += 1) {
        if (pxArrayAddBack(&self->sessions) == 0) return 0;

        ssize        back  = pxArrayBack(&self->sessions);
        ConnSession* value = pxArrayGetPntr(&self->sessions, back);

        if (value == PX_NULL) return 0;

        if (connSessionCreate(value, arena) == 0) return 0;
    }

    if (connBoardCreate(&self->board, arena, 7, 5) == 0)
        return 0;

    self->state_curr = ConnServerState_None;
    self->state_prev = ConnServerState_None;

    connServerStateSet(self, ConnServerState_Starting);

    return 1;
}

void
connServerDestroy(ConnServer* self)
{
    pxSocketTcpDestroy(self->listener);
    pxAsyncDestroy(self->async);
}

void
connServerStart(ConnServer* self)
{
    ConnSession* value = pxArrayGetPntr(&self->sessions, 0);

    if (value != 0)
        connServerTcpAccept(self, value);
    else
        connServerStateSetError(self);
}

void
connServerStop(ConnServer* self)
{
    ConnSession value;

    while (pxArrayRemoveBack(&self->sessions, &value) != 0)
        connSessionDestroy(&value);
}

void
connServerUpdate(ConnServer* self)
{
    connServerPollEvents(self);

    if (connServerStateIsEqual(self, ConnServerState_SendingTurn) != 0) {
        self->player_turn = (self->player_turn % self->player_count) + 1;

        ConnSession* session = pxArrayGetPntr(&self->sessions, self->player_turn - 1);

        connServerTcpBroadcast(self, 0,
            connMessageTurn(self->player_turn));

        connServerStateSet(self, ConnServerState_WaitingMove);

        connServerTcpRead(self, session);
    }
}

void
connServerTcpAccept(ConnServer* self, ConnSession* session)
{
    b32 status = pxSocketTcpAcceptAsync(
        self->async, session, self->listener, session->socket);

    if (status == 0) connServerStateSetError(self);
}

void
connServerTcpBroadcast(ConnServer* self, ConnSession* session, ConnMessage message)
{
    ssize index = 0;

    for (index = 0; index < self->player_count; index += 1) {
        ConnSession* value = pxArrayGetPntr(&self->sessions, index);

        if (value != PX_NULL && value != session)
            connServerTcpWrite(self, value, message);
    }
}

void
connServerTcpWrite(ConnServer* self, ConnSession* session, ConnMessage message)
{
    PxSocketTcp* socket = session->socket;
    u8*          values = session->writing;
    ssize        start  = 0;
    ssize        stop   = 0;

    stop = connMessageEncode(message, session->writing, CONN_MESSAGE_SIZE);

    if (pxArrayIsFull(&session->messages) != 0) connServerStateSetError(self);

    if (pxArrayIsEmpty(&session->messages) != 0) {
        b32 status = pxSocketTcpWriteAsync(
            self->async, session, socket, values, start, stop);

        if (status != 0)
            connServerOnTcpWrite(self, session, message);
        else
            connServerStateSetError(self);
    }
    else pxArrayInsertBack(&session->messages, message);
}

void
connServerTcpRead(ConnServer* self, ConnSession* session)
{
    PxSocketTcp* socket = session->socket;
    u8*          values = session->reading;
    ssize        start  = 0;
    ssize        stop   = CONN_MESSAGE_SIZE;

    b32 status = pxSocketTcpReadAsync(
        self->async, session, socket, values, start, stop);

    if (status == 0) connServerStateSetError(self);
}

void
connServerPollEvents(ConnServer* self)
{
    PxAsyncEventFamily family = PxAsyncEventFamily_None;

    while (connServerStateIsActive(self) != 0) {
        void* event = PX_NULL;
        void* tag   = PX_NULL;

        family = pxAsyncPoll(self->async, &tag, &event, 10);

        if (family == PxAsyncEventFamily_None) break;

        switch (family) {
            case PxAsyncEventFamily_Tcp: {
                PxSocketTcpEvent tcp;

                b32 status = pxAsyncReturn(self->async, event, &tcp, sizeof tcp);

                if (status == 0) connServerStateSetError(self);

                connServerOnTcpEvent(self, tag, &tcp);
            } break;

            default: break;
        }
    }
}

void
connServerOnTcpEvent(ConnServer* self, ConnSession* session, PxSocketTcpEvent* event)
{
    switch (event->kind) {
        case PxSocketTcpEvent_Error: connServerStateSetError(self); break;

        case PxSocketTcpEvent_Accept: {
            if (event->accept.socket != 0) {
                connServerOnTcpAccept(self, session);

                ssize        index = self->player_count + 1;
                ConnSession* value = pxArrayGetPntr(&self->sessions, index);

                if (value != 0) connServerTcpAccept(self, value);
            }
            else connServerStateSetError(self);
        } break;

        case PxSocketTcpEvent_Write: {
            PxSocketTcp* socket = session->socket;
            u8*          values = session->writing;
            ssize        start  = 0;
            ssize        stop   = 0;

            ConnMessage message;

            if (pxArrayRemoveFront(&session->messages, &message) == 0) break;

            stop = connMessageEncode(message, session->writing, CONN_MESSAGE_SIZE);

            b32 status = pxSocketTcpWriteAsync(
                self->async, session, socket, values, start, stop);

            if (status != 0)
                connServerOnTcpWrite(self, session, message);
            else
                connServerStateSetError(self);
        } break;

        case PxSocketTcpEvent_Read: {
            PxSocketTcp* socket = event->read.socket;
            u8*          values = event->read.values;
            ssize        start  = event->read.stop;
            ssize        stop   = CONN_MESSAGE_SIZE;

            ConnMessage  message = connMessageDecode(event->read.values, event->read.stop);

            if (message.length <= 0 || event->read.stop < message.length) {
                b32 status = pxSocketTcpReadAsync(
                    self->async, session, socket, values, start, stop);

                if (status == 0) connServerStateSetError(self);
            }
            else connServerOnTcpRead(self, session, message);
        } break;

        default: break;
    }
}

void
connServerOnTcpAccept(ConnServer* self, ConnSession* session)
{
    printf("[DEBUG] Accepted 0x%016llx!\n", (usize) session);

    connServerTcpRead(self, session);
}

void
connServerOnTcpWrite(ConnServer* self, ConnSession* session, ConnMessage message)
{
    printf("[DEBUG] Wrote message to 0x%016llx: ", (usize) session);
        connMessageShow(message);
    printf("\n");
}

void
connServerOnTcpRead(ConnServer* self, ConnSession* session, ConnMessage message)
{
    printf("[DEBUG] Read message from 0x%016llx: ", (usize) session);
        connMessageShow(message);
    printf("\n");

    switch (message.kind) {
        case ConnMessage_Join: {
            if (connServerStateIsEqual(self, ConnServerState_Starting) == 0) break;

            if (session->player.client == 0) {
                // TODO(gio): Add accept message

                session->player.flag    = message.join.flag;
                session->player.client = ((u32) self->player_count + 1);

                self->player_count += 1;

                connServerTcpBroadcast(self, PX_NULL,
                    connMessageData(session->player.flag, session->player.client));

                ssize index = 0;

                for (index = 0; index < self->player_count; index += 1) {
                    ConnSession* value = pxArrayGetPntr(&self->sessions, index);

                    if (value == session) continue;

                    connServerTcpWrite(self, session,
                        connMessageData(value->player.flag, value->player.client));
                }

                // TODO(gio): Maybe move elsewhere?
                if (self->player_count == pxArraySize(&self->sessions))
                    connServerStateSet(self, ConnServerState_SendingTurn);
            }
            else {
                // TODO(gio): Add reject message
            }
        } break;

        case ConnMessage_Move: {
            if (connServerStateIsEqual(self, ConnServerState_WaitingMove) == 0) break;

            u32 client = message.move.client;
            u32 column = message.move.column;

            connBoardInsert(&self->board, column, client);
            connBoardShow(&self->board);

            connServerTcpBroadcast(self, session, message);

            u32 winner = 0;
            b32 full   = connBoardIsFull(&self->board);

            if (connBoardIsWinner(&self->board, column, client) != 0)
                winner = client;

            if (winner != 0 || full != 0) {
                connServerStateSet(self, ConnServerState_Stopping);

                connServerTcpBroadcast(self, PX_NULL,
                    connMessageResult(winner));
            }
            else connServerStateSet(self, ConnServerState_SendingTurn);
        } break;

        case ConnMessage_Quit: {
            connServerStateSet(self, ConnServerState_Stopping);

            connServerTcpBroadcast(self, PX_NULL,
                connMessageResult(0));
        } break;

        default: break;
    }
}

void
connServerOnStateChange(ConnServer* self, ConnServerState previous, ConnServerState current)
{

}

b32
connSessionCreate(ConnSession* self, PxMemoryArena* arena)
{
    self->socket = pxSocketTcpReserve(arena);
    self->player = connPlayerMake(0, 0);

    if (self->socket == PX_NULL) return 0;

    pxMemorySet(self->reading, sizeof self->reading, 0x00);
    pxMemorySet(self->writing, sizeof self->writing, 0x00);

    if (pxArrayCreate(&self->messages, arena, 16) == 0) return 0;

    return 1;
}

void
connSessionDestroy(ConnSession* self)
{
    pxSocketTcpDestroy(self->socket);
}

#endif // CONN_SERVER_C
