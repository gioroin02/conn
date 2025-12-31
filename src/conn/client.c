#ifndef CONN_CLIENT_C
#define CONN_CLIENT_C

#include "client.h"

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

            if (value >= 0 && value < 3)
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

ConnClient
connClientMake()
{
    ConnClient result;

    pxMemorySet(&result, sizeof result, 0xAB);

    return result;
}

b32
connClientStateIsEqual(ConnClient* self, ConnClientState state)
{
    return self->state_curr == state ? 1 : 0;
}

b32
connClientStateIsActive(ConnClient* self)
{
    if (connClientStateIsEqual(self, ConnClientState_Stopping) != 0) return 0;
    if (connClientStateIsEqual(self, ConnClientState_Error) != 0)    return 0;

    return 1;
}

void
connClientStateSet(ConnClient* self, ConnClientState state)
{
    self->state_prev = self->state_curr;
    self->state_curr = state;

    connClientOnStateChange(self, self->state_prev, self->state_curr);
}

void
connClientStateSetError(ConnClient* self)
{
    connClientStateSet(self, ConnClientState_Error);
}

b32
connClientCreate(ConnClient* self, PxMemoryArena* arena)
{
    self->async  = pxAsyncReserve(arena);
    self->socket = pxSocketTcpReserve(arena);
    self->client = 0;
    self->column = 0;

    pxMemorySet(self->writing, sizeof self->writing, 0x00);
    pxMemorySet(self->reading, sizeof self->reading, 0x00);

    if (pxAsyncCreate(self->async, arena, pxMemoryKiB(64)) == 0)
        return 0;

    if (pxSocketTcpCreate(self->socket, pxAddressIp4Empty(), 0) == 0)
        return 0;

    if (pxArrayCreate(&self->messages, arena, 16) == 0) return 0;
    if (pxArrayCreate(&self->players, arena, 2) == 0)   return 0;

    if (connBoardCreate(&self->board, arena, 7, 5) == 0) return 0;

    self->state_curr = ConnClientState_None;
    self->state_prev = ConnClientState_None;

    connClientStateSet(self, ConnClientState_Starting);

    return 1;
}

void
connClientDestroy(ConnClient* self)
{
    pxSocketTcpDestroy(self->socket);
    pxAsyncDestroy(self->async);
}

void
connClientStart(ConnClient* self)
{
    connClientTcpConnect(self, pxAddressIp4Local(), 50000);
}

void
connClientStop(ConnClient* self) {}

void
connClientUpdate(ConnClient* self)
{
    connClientPollEvents(self);

    if (connClientStateIsEqual(self, ConnClientState_ChoosingMove) != 0) {
        b32 status = 0;

        while (status == 0) {
            self->column = rand() % self->board.width;

            status = connBoardInsert(&self->board,
                self->column, self->client);
        }

        connClientStateSet(self, ConnClientState_SendingMove);
    }

    if (connClientStateIsEqual(self, ConnClientState_SendingMove) != 0) {
        u32 client = self->client;
        u32 column = self->column;

        connClientTcpWrite(self,
            connMessageMove(client, column));

        connBoardShow(&self->board);

        connClientStateSet(self, ConnClientState_WaitingTurn);

        connClientTcpRead(self);
    }
}

void
connClientTcpConnect(ConnClient* self, PxAddressIp address, u16 port)
{
    b32 status = pxSocketTcpConnectAsync(
        self->async, PX_NULL, self->socket, address, port);

    if (status == 0) connClientStateSetError(self);
}

void
connClientTcpWrite(ConnClient* self, ConnMessage message)
{
    PxSocketTcp* socket = self->socket;
    u8*          values = self->writing;
    ssize        start  = 0;
    ssize        stop   = 0;

    stop = connMessageEncode(message, self->writing, CONN_MESSAGE_SIZE);

    if (pxArrayIsFull(&self->messages) != 0) connClientStateSetError(self);

    if (pxArrayIsEmpty(&self->messages) != 0) {
        b32 status = pxSocketTcpWriteAsync(
            self->async, PX_NULL, socket, values, start, stop);

        if (status != 0)
            connClientOnTcpWrite(self, message);
        else
            connClientStateSetError(self);
    }
    else pxArrayInsertBack(&self->messages, message);
}

void
connClientTcpRead(ConnClient* self)
{
    PxSocketTcp* socket = self->socket;
    u8*          values = self->reading;
    ssize        start  = 0;
    ssize        stop   = CONN_MESSAGE_SIZE;

    b32 status = pxSocketTcpReadAsync(
        self->async, PX_NULL, socket, values, start, stop);

    if (status == 0) connClientStateSetError(self);
}

void
connClientPollEvents(ConnClient* self)
{
    PxAsyncEventFamily family = PxAsyncEventFamily_None;

    while (connClientStateIsActive(self) != 0) {
        void* event = PX_NULL;
        void* tag   = PX_NULL;

        family = pxAsyncPoll(self->async, &tag, &event, 10);

        if (family == PxAsyncEventFamily_None) break;

        switch (family) {
            case PxAsyncEventFamily_Tcp: {
                PxSocketTcpEvent tcp;

                pxMemoryCopy(&tcp, sizeof tcp, event);

                b32 status = pxAsyncReturn(self->async, event);

                if (status == 0) connClientStateSetError(self);

                connClientOnTcpEvent(self, &tcp);
            } break;

            default: break;
        }
    }
}

void
connClientOnTcpEvent(ConnClient* self, PxSocketTcpEvent* event)
{
    switch (event->kind) {
        case PxSocketTcpEvent_Error: connClientStateSetError(self); break;

        case PxSocketTcpEvent_Connect: {
            if (event->connect.status != 0)
                connClientOnTcpConnect(self);
            else
                connClientStateSetError(self);
        } break;

        case PxSocketTcpEvent_Write: {
            PxSocketTcp* socket = self->socket;
            u8*          values = self->writing;
            ssize        start  = 0;
            ssize        stop   = 0;

            ConnMessage message;

            if (pxArrayRemoveFront(&self->messages, &message) == 0) break;

            stop = connMessageEncode(message, self->writing, CONN_MESSAGE_SIZE);

            b32 status = pxSocketTcpWriteAsync(
                self->async, PX_NULL, socket, values, start, stop);

            if (status != 0)
                connClientOnTcpWrite(self, message);
            else
                connClientStateSetError(self);
        } break;

        case PxSocketTcpEvent_Read: {
            PxSocketTcp* socket = event->read.socket;
            u8*          values = event->read.values;
            ssize        start  = event->read.stop;
            ssize        stop   = CONN_MESSAGE_SIZE;

            ConnMessage message = connMessageDecode(event->read.values, event->read.stop);

            if (message.length <= 0 || event->read.stop < message.length) {
                b32 status = pxSocketTcpReadAsync(
                    self->async, PX_NULL, socket, values, start, stop);

                if (status == 0) connClientStateSetError(self);
            }
            else connClientOnTcpRead(self, message);
        } break;

        default: break;
    }
}

void
connClientOnTcpConnect(ConnClient* self)
{
    printf("[DEBUG] Connected!\n");

    connClientTcpWrite(self,
        connMessageJoin(ConnClient_Player));

    connClientTcpRead(self);
}

void
connClientOnTcpWrite(ConnClient* self, ConnMessage message)
{
    printf("[DEBUG] Wrote message: ");
        connMessageShow(message);
    printf("\n");
}

void
connClientOnTcpRead(ConnClient* self, ConnMessage message)
{
    printf("[DEBUG] Read message: ");
        connMessageShow(message);
    printf("\n");

    switch (message.kind) {
        case ConnMessage_Data: {
            if (message.data.client > 0) {
                ConnPlayer value = connPlayerMake(
                    message.data.flag, message.data.client);

                pxArrayInsert(&self->players, value.client - 1, value);

                if (self->client == 0) self->client = value.client;
            }

            connClientTcpRead(self);

            connClientStateSet(self, ConnClientState_WaitingTurn);
        } break;

        case ConnMessage_Turn: {
            if (connClientStateIsEqual(self, ConnClientState_WaitingTurn) == 0) break;

            if (self->client != message.turn.client) {
                connClientStateSet(self, ConnClientState_WaitingMove);

                connClientTcpRead(self);
            } else
                connClientStateSet(self, ConnClientState_ChoosingMove);
        } break;

        case ConnMessage_Move: {
            if (connClientStateIsEqual(self, ConnClientState_WaitingMove) == 0) break;

            connBoardInsert(&self->board,
                message.move.column, message.move.client);

            connBoardShow(&self->board);

            connClientStateSet(self, ConnClientState_WaitingTurn);

            connClientTcpRead(self);
        } break;

        case ConnMessage_Result: {
            if (message.result.client != 0) {
                if (message.result.client == self->client)
                    printf("Vittoria!\n");
                else
                    printf("Sconfitta...\n");
            }
            else printf("Pareggio.\n");

            connClientStateSet(self, ConnClientState_Stopping);
        } break;

        default: break;
    }
}

void
connClientOnStateChange(ConnClient* self, ConnClientState previous, ConnClientState current)
{

}

#endif // CONN_CLIENT_C
