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
connCommandShow(ConnCommand command)
{
    u8 buffer[1024];

    pxMemorySet(buffer, sizeof buffer, 0x00);

    ssize count = connCommandToString(command, buffer, 1024);

    if (count > 0)
        printf("%.*s", ((int) count), buffer);
}

static void
connBoardShow(ConnBoard* self, u16 column)
{
    ssize index = 0;
    ssize row   = 0;
    ssize col   = 0;

    char* colors[] = {"\x1b[0m", "\x1b[44m", "\x1b[41m"};

    for (index = 0; index < self->width; index += 1)
        printf("%s", index == 0 ? "+-----+" : "-----+");

    printf("\r\n");

    for (row = 0; row < self->height; row += 1) {
        printf("| ");

        for (col = 0; col < self->width; col += 1) {
            u16 value = connBoardGet(self, col, row);

            if (value > 0 && value < 3)
                printf("%s %u %s | ", colors[value], value, colors[0]);
            else
                printf("    | ");
        }

        printf("\r\n");

        for (index = 0; index < self->width; index += 1)
            printf("%s", index == 0 ? "+-----+" : "-----+");

        printf("\r\n");
    }

    printf(" ");

    for (index = 0; index < self->width; index += 1)
        printf("%s", index == column ? " ^^^ " : "      ");

    printf("\r\n");
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
    while (state != self->state_curr && state != ConnClientState_None) {
        self->state_prev = self->state_curr;
        self->state_curr = state;

        state = connClientOnStateChange(self);
    }
}

void
connClientStateSetError(ConnClient* self)
{
    connClientStateSet(self, ConnClientState_Error);
}

b32
connClientCreate(ConnClient* self, PxMemoryArena* arena)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    self->async        = pxAsyncReserve(arena);
    self->async2       = pxAsyncReserve(arena);
    self->socket       = pxSocketTcpReserve(arena);
    // self->consin       = pxFileReserve(arena);
    self->console      = pxConsoleReserve(arena);
    self->client       = 0;
    self->column       = 0;
    self->player_count = 0;

    pxMemorySet(self->buff_tcp_write, sizeof self->buff_tcp_write, 0x00);
    pxMemorySet(self->buff_tcp_read, sizeof self->buff_tcp_read, 0x00);
    pxMemorySet(self->buff_term_read, sizeof self->buff_term_read, 0x00);

    if (pxAsyncCreate(self->async, arena, pxMemoryKiB(64)) == 0)
        return 0;

    if (pxAsyncCreate(self->async2, arena, pxMemoryKiB(64)) == 0)
        return 0;

    PxAddressIp address = pxAddressIp4Empty();

    if (pxSocketTcpCreate(self->socket, address, 0) == 0) return 0;

    if (pxConsoleCreate(self->console) == 0) return 0;

    if (pxConsoleModeSet(self->console, PxConsoleMode_Raw) == 0)
        return 0;

    if (pxArrayCreate(&self->messages, arena, 16) == 0) return 0;
    if (pxArrayCreate(&self->players, arena, 2) == 0)   return 0;
    if (pxArrayCreate(&self->commands, arena, 32) == 0) return 0;

    ssize index = 0;

    for (index = 0; index < pxArraySize(&self->players); index += 1)
        pxArrayAddBack(&self->players);

    if (connBoardCreate(&self->board, arena, 9, 6) == 0) return 0;

    self->state_curr = ConnClientState_None;
    self->state_prev = ConnClientState_None;

    connClientStateSet(self, ConnClientState_Starting);

    return 1;
}

void
connClientDestroy(ConnClient* self)
{
    pxConsoleDestroy(self->console);
    // pxFileDestroy(self->consin);

    pxSocketTcpDestroy(self->socket);

    pxAsyncDestroy(self->async);
}

void
connClientStart(ConnClient* self)
{
    connClientTcpConnect(self, pxAddressIp4Local(), 50000);
    connClientFileRead(self);
}

void
connClientStop(ConnClient* self) {}

void
connClientUpdate(ConnClient* self)
{
    ConnClientState state;

    state = connClientOnUpdate(self);

    connClientStateSet(self, state);
    connClientPollEvents(self);
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
    u8*          values = self->buff_tcp_write;
    ssize        start  = 0;
    ssize        stop   = 0;

    stop = connMessageEncode(message,
        self->buff_tcp_write, sizeof self->buff_tcp_write);

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
    u8*          values = self->buff_tcp_read;
    ssize        start  = 0;
    ssize        stop   = sizeof self->buff_tcp_read;

    b32 status = pxSocketTcpReadAsync(
        self->async, PX_NULL, socket, values, start, stop);

    if (status == 0) connClientStateSetError(self);
}

void
connClientFileRead(ConnClient* self)
{
    PxFile* file   = PX_NULL; // self->consin;
    u8*     values = self->buff_term_read;
    ssize   start  = 0;
    ssize   stop   = sizeof self->buff_term_read;

    b32 status = pxFileReadAsync(
        self->async2, PX_NULL, file, values, start, stop);

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

                if (status != 0)
                    connClientOnTcpEvent(self, tcp);
                else
                    connClientStateSetError(self);
            } break;

            default: break;
        }
    }

    while (connClientStateIsActive(self) != 0) {
        void* event = PX_NULL;
        void* tag   = PX_NULL;

        family = pxAsyncPoll(self->async2, &tag, &event, 10);

        if (family == PxAsyncEventFamily_None) break;

        switch (family) {
            case PxAsyncEventFamily_File: {
                PxFileEvent file;

                pxMemoryCopy(&file, sizeof file, event);

                b32 status = pxAsyncReturn(self->async2, event);

                if (status != 0) {
                    connClientOnFileEvent(self, file);
                } else
                    connClientStateSetError(self);
            } break;

            default: break;
        }
    }
}

void
connClientOnTcpEvent(ConnClient* self, PxSocketTcpEvent event)
{
    switch (event.kind) {
        case PxSocketTcpEvent_Error: connClientStateSetError(self); break;

        case PxSocketTcpEvent_Connect: {
            if (event.connect.status != 0)
                connClientOnTcpConnect(self);
            else
                connClientStateSetError(self);
        } break;

        case PxSocketTcpEvent_Write: {
            ConnMessage message;

            if (pxArrayRemoveFront(&self->messages, &message) == 0) break;

            PxSocketTcp* socket = self->socket;
            u8*          values = self->buff_tcp_write;
            ssize        start  = 0;
            ssize        stop   = 0;

            stop = connMessageEncode(message,
                self->buff_tcp_write, sizeof self->buff_tcp_write);

            b32 status = pxSocketTcpWriteAsync(
                self->async, PX_NULL, socket, values, start, stop);

            if (status != 0)
                connClientOnTcpWrite(self, message);
            else
                connClientStateSetError(self);
        } break;

        case PxSocketTcpEvent_Read: {
            PxSocketTcp* socket = event.read.socket;
            u8*          values = event.read.values;
            ssize        start  = event.read.stop;
            ssize        stop   = sizeof self->buff_tcp_read;

            ConnMessage message = connMessageDecode(event.read.values, event.read.stop);

            if (message.length <= 0 || event.read.stop < message.length) {
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

    connClientTcpWrite(self, connMessageJoin(ConnClient_Player));
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
        case ConnMessage_Quit:
            connClientStateSet(self, ConnClientState_Stopping);
        break;

        case ConnMessage_Data: {
            if (connClientStateIsEqual(self, ConnClientState_Starting) == 0) break;

            if (message.data.client > 0) {
                ConnPlayer  player = connPlayerMake(message.data.flag, message.data.client);
                ConnPlayer* value  = pxArrayGetPntr(&self->players, player.client - 1);

                if (value == PX_NULL)
                    connClientStateSetError(self);
                else
                    *value = player;

                if (self->client == 0) self->client = player.client;

                self->player_count += 1;
            }

            if (self->player_count == pxArraySize(&self->players))
                connClientStateSet(self, ConnClientState_WaitingTurn);

            connClientTcpRead(self);
        } break;

        case ConnMessage_Turn: {
            if (connClientStateIsEqual(self, ConnClientState_WaitingTurn) == 0) break;

            u16 client = message.turn.client;

            if (self->client != client) {
                connClientStateSet(self, ConnClientState_WaitingMove);
                connClientTcpRead(self);
            }
            else connClientStateSet(self, ConnClientState_ChoosingMove);
        } break;

        case ConnMessage_Move: {
            if (connClientStateIsEqual(self, ConnClientState_WaitingMove) == 0) break;

            u16 client = message.move.client;
            u16 column = message.move.column;

            connBoardInsert(&self->board, column, client);
            connBoardShow(&self->board, self->column);

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
connClientOnFileEvent(ConnClient* self, PxFileEvent event)
{
    switch (event.kind) {
        case PxFileEvent_Read: {
            // if (event.read.file != self->consin) break;

            PxFile* file   = event.read.file;
            u8*     values = event.read.values;
            ssize   start  = event.read.start;
            ssize   stop   = event.read.stop;
            ssize   index  = 0;

            for (index = start; index < stop; index += 1) {
                ConnCommand command = connCommandDecode(values[index]);

                if (pxArrayInsertBack(&self->commands, command) == 0)
                    connClientStateSetError(self);
            }

            if (stop < sizeof self->buff_term_read) {
                start = stop;
                stop  = sizeof self->buff_term_read;

                b32 status = pxFileReadAsync(
                    self->async2, PX_NULL, file, values, start, stop);

                if (status == 0) connClientStateSetError(self);
            }
            else connClientFileRead(self);
        } break;

        default: break;
    }
}

void
connClientOnCommand(ConnClient* self, ConnCommand command)
{
    printf("[DEBUG] Read command: ");
        connCommandShow(command);
    printf("\n");

    switch (command.kind) {
        case ConnCommand_MoveLeft: {
            self->column =
                pxClamp(self->column - 1, 0, self->board.width - 1);

            connBoardShow(&self->board, self->column);
        } break;

        case ConnCommand_MoveRight: {
            self->column =
                pxClamp(self->column + 1, 0, self->board.width - 1);

            connBoardShow(&self->board, self->column);
        } break;

        case ConnCommand_Place: {
            u16 client = self->client;
            u16 column = self->column;

            if (connClientStateIsEqual(self, ConnClientState_ChoosingMove) != 0) {
                b32 status = connBoardInsert(&self->board, column, client);

                if (status != 0) {
                    connClientStateSet(self, ConnClientState_SendingMove);

                    connBoardShow(&self->board, self->column);
                }
            }
        } break;

        case ConnCommand_Quit: {
            connClientTcpWrite(self, connMessageQuit(self->client));

            connClientStateSet(self, ConnClientState_Stopping);
        } break;

        default: break;
    }
}

ConnClientState
connClientOnUpdate(ConnClient* self)
{
    ConnCommand command;

    while (pxArrayRemoveFront(&self->commands, &command) != 0)
        connClientOnCommand(self, command);

    return ConnClientState_None;
}

ConnClientState
connClientOnStateChange(ConnClient* self)
{
    static const char* const states[] = {
        "None",
        "Error",
        "Starting",
        "Stopping",
        "WaitingTurn",
        "ChoosingMove",
        "SendingMove",
        "WaitingMove",
    };

    printf("[DEBUG] State changed from <%s> to <%s>\n",
        states[self->state_prev], states[self->state_curr]);

    if (connClientStateIsEqual(self, ConnClientState_ChoosingMove) != 0)
        connBoardShow(&self->board, self->column);

    if (connClientStateIsEqual(self, ConnClientState_SendingMove) != 0) {
        u16 client = self->client;
        u16 column = self->column;

        connClientTcpWrite(self, connMessageMove(client, column));
        connClientTcpRead(self);

        return ConnClientState_WaitingTurn;
    }

    if (connClientStateIsEqual(self, ConnClientState_Error) != 0)
        connClientTcpWrite(self, connMessageQuit(self->client));

    return ConnClientState_None;
}

#endif // CONN_CLIENT_C
