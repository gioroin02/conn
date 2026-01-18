#ifndef CONN_CLIENT_C
#define CONN_CLIENT_C

#include "client.h"

static PString8 connClientStateName(ConnClientState state)
{
    switch (state) {
        case ConnClientState_None:         return pString8("None");
        case ConnClientState_Error:        return pString8("Error");
        case ConnClientState_Starting:     return pString8("Starting");
        case ConnClientState_Stopping:     return pString8("Stopping");
        case ConnClientState_WaitingTurn:  return pString8("WaitingTurn");
        case ConnClientState_ChoosingMove: return pString8("ChoosingMove");
        case ConnClientState_SendingMove:  return pString8("SendingMove");
        case ConnClientState_WaitingMove:  return pString8("WaitingMove");

        default: break;
    }

    return pString8("");
}

static void connMessageShow(ConnMessage message)
{
    U8 buffer[1024];

    pMemorySet(buffer, sizeof buffer, 0x00);

    Int count = connMessageToString(message, buffer, sizeof buffer);

    if (count > 0)
        printf("%.*s", (int) count, buffer);
}

static void connCommandShow(ConnCommand command)
{
    U8 buffer[1024];

    pMemorySet(buffer, sizeof buffer, 0x00);

    Int count = connCommandToString(command, buffer, sizeof buffer);

    if (count > 0)
        printf("%.*s", (int) count, buffer);
}

static void connBoardShow(ConnBoard* self, U16 column)
{
    Int index  = 0;
    Int height = 0;
    Int row    = 0;
    Int col    = 0;

    char* colors[] = {"\033[0m", "\033[44m", "\033[41m"};

    connBoardHeight(self, column, &height);

    printf("\033c");

    printf("    ");

    for (index = 0; index < self->width; index += 1)
        printf("%s", index == column ? " vvv " : "      ");

    printf("\r\n   +");

    for (index = 0; index < self->width; index += 1)
        printf("%s", index == 0 ? "-----+" : "-----+");

    printf("\r\n");

    for (row = 0; row < self->height; row += 1) {
        printf(" %s | ", row == height ? ">" : " ");

        for (col = 0; col < self->width; col += 1) {
            U16 value = connBoardGet(self, col, row);

            if (value > 0 && value < 3)
                printf("%s %u %s | ", colors[value], value, colors[0]);
            else
                printf("    | ");
        }

        printf(" %s\r\n   +", row == height ? "<" : " ");

        for (index = 0; index < self->width; index += 1)
            printf("%s", index == 0 ? "-----+" : "-----+");

        printf("\r\n");
    }

    printf("    ");

    for (index = 0; index < self->width; index += 1)
        printf("%s", index == column ? " ^^^ " : "      ");

    printf("\r\n");
}

static void connClientOnTcpConnect(ConnClient* self, PSocketTcpEventConnect event)
{
    if (event.status != 0)
        connClientOnConnect(self, event.host);
    else
        connClientStateSetError(self);
}

static void connClientOnTcpWrite(ConnClient* self, PSocketTcpEventWrite event)
{
    ConnMessage message;

    if (event.start + event.bytes == event.stop) {
        if (pArrayRemoveFront(&self->messages, &message) == 0) return;

        Int count = connMessageEncode(message,
            self->buff_tcp_write, sizeof self->buff_tcp_write);

        event.pntr  = self->buff_tcp_write;
        event.start = 0;
        event.stop  = count;
    }
    else event.start += event.bytes;

    Bool status = pSocketTcpWriteAsync(self->socket, event.pntr,
        event.start, event.stop, self->queue, NULL);

    if (status != 0)
        connClientOnMessageWrite(self, message);
    else
        connClientStateSetError(self);
}

static void connClientOnTcpRead(ConnClient* self, PSocketTcpEventRead event)
{
    ConnMessage message;

    Int count = event.start + event.bytes;

    Bool status = connMessageDecode(&message, event.pntr, count);

    if (message.length <= 0 || message.length > count) {
        event.start += event.bytes;

        Bool status = pSocketTcpReadAsync(event.socket, event.pntr,
            event.start, event.stop, self->queue, NULL);

        if (status == 0) connClientStateSetError(self);
    }
    else connClientOnMessageRead(self, message);
}

static void connClientPollAsyncIo(ConnClient* self, PMemoryArena* arena)
{
    Int index = 0;
    Int limit = 20;

    for (index = 0; index < limit; index += 1) {
        void*          marker = pMemoryArenaTell(arena);
        PAsyncIoEvent* event  = NULL;

        PAsyncIoEventKind kind =
            pAsyncIoQueuePollEvent(self->queue, 10, arena, &event);

        if (kind == PAsyncIoEvent_None) break;

        switch (kind) {
            case PAsyncIoEvent_Tcp: {
                PSocketTcpEvent event_tcp = *(PSocketTcpEvent*) event;

                pMemoryArenaRewind(arena, marker);

                switch (event_tcp.kind) {
                    case PSocketTcpEvent_Connect: connClientOnTcpConnect(self, event_tcp.connect); break;
                    case PSocketTcpEvent_Write:   connClientOnTcpWrite(self, event_tcp.write); break;
                    case PSocketTcpEvent_Read:    connClientOnTcpRead(self, event_tcp.read); break;

                    default: break;
                }
            } break;

            default: break;
        }
    }
}

static void connClientPollConsole(ConnClient* self, PMemoryArena* arena)
{
    PConsoleEvent event;

    while (pConsolePollEvent(self->console, &event) != 0) {
        ConnCommand command = connCommandDecode(event);

        if (pArrayInsertBack(&self->commands, command) == 0)
            connClientStateSetError(self);
    }
}

Bool connClientCreate(ConnClient* self, PMemoryArena* arena)
{
    pMemorySet(self, sizeof *self, 0xAB);

    PMemoryPool pool = pMemoryPoolMake(
        pMemoryArenaReserveManyOf(arena, U8, pMemoryKIB(64)),
        pMemoryKIB(64), 512);

    self->queue   = pAsyncIoQueueReserve(arena);
    self->socket  = pSocketTcpReserve(arena);
    self->console = pConsoleReserve(arena);

    self->client       = 0;
    self->column       = 0;
    self->player_count = 0;

    pMemorySet(self->buff_tcp_write, sizeof self->buff_tcp_write, 0x00);
    pMemorySet(self->buff_tcp_read, sizeof self->buff_tcp_read, 0x00);

    if (pAsyncIoQueueCreate(self->queue, pool) == 0) return 0;

    PHostIp host = pHostIpMake(pAddressIp4Any(), 0);

    if (pSocketTcpCreate(self->socket, host) == 0) return 0;
    if (pConsoleCreate(self->console) == 0)        return 0;

    if (pConsoleModeSet(self->console, PConsoleMode_Raw) == 0)
        return 0;

    if (pArrayCreate(&self->messages, arena, 16) == 0) return 0;
    if (pArrayCreate(&self->players, arena, 2) == 0)   return 0;
    if (pArrayCreate(&self->commands, arena, 32) == 0) return 0;

    Int index = 0;

    for (index = 0; index < pArraySize(&self->players); index += 1)
        pArrayAddBack(&self->players);

    if (connBoardCreate(&self->board, arena, 9, 7) == 0) return 0;

    self->state_curr = ConnClientState_None;
    self->state_prev = ConnClientState_None;

    connClientStateSet(self, ConnClientState_Starting);

    return 1;
}

void connClientDestroy(ConnClient* self)
{
    pConsoleDestroy(self->console);
    pSocketTcpDestroy(self->socket);
    pAsyncIoQueueDestroy(self->queue);
}

void connClientStart(ConnClient* self)
{
    connClientConnect(self, pHostIpMake(pAddressIp4Self(), 50000));
}

void connClientStop(ConnClient* self) {}

void connClientUpdate(ConnClient* self, PMemoryArena* arena)
{
    connClientPollAsyncIo(self, arena);
    connClientPollConsole(self, arena);

    ConnClientState state = connClientOnUpdate(self);

    connClientStateSet(self, state);
}

void connClientConnect(ConnClient* self, PHostIp host)
{
    Bool status = pSocketTcpConnectAsync(
        self->socket, host, self->queue, NULL);

    if (status == 0) connClientStateSetError(self);
}

void connClientMessageWrite(ConnClient* self, ConnMessage message)
{
    PSocketTcp* socket = self->socket;
    U8*         pntr   = self->buff_tcp_write;

    if (pArrayIsFull(&self->messages) != 0) connClientStateSetError(self);

    if (pArrayIsEmpty(&self->messages) != 0) {
        Int count = connMessageEncode(message,
            self->buff_tcp_write, sizeof self->buff_tcp_write);

        Bool status = pSocketTcpWriteAsync(
            socket, pntr, 0, count, self->queue, NULL);

        if (status == 0) connClientStateSetError(self);
    }
    else pArrayInsertBack(&self->messages, message);
}

void connClientMessageRead(ConnClient* self)
{
    PSocketTcp* socket = self->socket;
    U8*         pntr   = self->buff_tcp_read;
    Int         count  = sizeof self->buff_tcp_read;

    Bool status = pSocketTcpReadAsync(
        socket, pntr, 0, count, self->queue, NULL);

    if (status == 0) connClientStateSetError(self);
}

void connClientOnConnect(ConnClient* self, PHostIp host)
{
    printf("[DEBUG] Connected!\n");

    connClientMessageWrite(self, connMessageJoin(ConnClient_Player));
    connClientMessageRead(self);
}

void connClientOnMessageWrite(ConnClient* self, ConnMessage message)
{
    printf("[DEBUG] Writing message: ");
        connMessageShow(message);
    printf("\n");
}

void connClientOnMessageRead(ConnClient* self, ConnMessage message)
{
    printf("[DEBUG] Read message: ");
        connMessageShow(message);
    printf("\n");

    switch (message.kind) {
        case ConnMessage_Quit: connClientStateSet(self, ConnClientState_Stopping); break;

        case ConnMessage_Data: {
            if (connClientStateIsEqual(self, ConnClientState_Starting) == 0) break;

            if (message.data.client > 0) {
                ConnPlayer  player = connPlayerMake(message.data.flag, message.data.client);
                ConnPlayer* value  = pArrayGetPntr(&self->players, player.client - 1);

                if (value == NULL)
                    connClientStateSetError(self);
                else
                    *value = player;

                if (self->client == 0) self->client = player.client;

                self->player_count += 1;
            }

            if (self->player_count == pArraySize(&self->players))
                connClientStateSet(self, ConnClientState_WaitingTurn);

            connClientMessageRead(self);
        } break;

        case ConnMessage_Turn: {
            if (connClientStateIsEqual(self, ConnClientState_WaitingTurn) == 0) break;

            U16 client = message.turn.client;

            if (self->client != client) {
                connClientStateSet(self, ConnClientState_WaitingMove);
                connClientMessageRead(self);
            }
            else connClientStateSet(self, ConnClientState_ChoosingMove);
        } break;

        case ConnMessage_Move: {
            if (connClientStateIsEqual(self, ConnClientState_WaitingMove) == 0) break;

            U16 client = message.move.client;
            U16 column = message.move.column;

            connBoardInsert(&self->board, column, client);
            connBoardShow(&self->board, self->column);

            connClientStateSet(self, ConnClientState_WaitingTurn);
            connClientMessageRead(self);
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

void connClientOnCommandRead(ConnClient* self, ConnCommand command)
{
    switch (command.kind) {
        case ConnCommand_MoveLeft: {
            self->column =
                pClamp(self->column - 1, 0, self->board.width - 1);

            connBoardShow(&self->board, self->column);
        } break;

        case ConnCommand_MoveRight: {
            self->column =
                pClamp(self->column + 1, 0, self->board.width - 1);

            connBoardShow(&self->board, self->column);
        } break;

        case ConnCommand_Place: {
            U16 client = self->client;
            U16 column = self->column;

            if (connClientStateIsEqual(self, ConnClientState_ChoosingMove) != 0) {
                Bool status = connBoardInsert(&self->board, column, client);

                if (status != 0) {
                    connClientStateSet(self, ConnClientState_SendingMove);

                    connBoardShow(&self->board, self->column);
                }
            }
        } break;

        case ConnCommand_Quit: {
            connClientMessageWrite(self, connMessageQuit(self->client));

            connClientStateSet(self, ConnClientState_Stopping);
        } break;

        default: break;
    }
}

ConnClientState connClientOnUpdate(ConnClient* self)
{
    ConnCommand command;

    while (pArrayRemoveFront(&self->commands, &command) != 0)
        connClientOnCommandRead(self, command);

    return ConnClientState_None;
}

ConnClientState connClientOnStateChange(ConnClient* self)
{
    PString8 prev = connClientStateName(self->state_prev);
    PString8 curr = connClientStateName(self->state_curr);

    printf("[DEBUG] State changed from <%s> to <%s>\n",
        prev.values, curr.values);

    if (connClientStateIsEqual(self, ConnClientState_ChoosingMove) != 0)
        connBoardShow(&self->board, self->column);

    if (connClientStateIsEqual(self, ConnClientState_SendingMove) != 0) {
        U16 client = self->client;
        U16 column = self->column;

        connClientMessageWrite(self, connMessageMove(client, column));
        connClientMessageRead(self);

        return ConnClientState_WaitingTurn;
    }

    if (connClientStateIsEqual(self, ConnClientState_Error) != 0)
        connClientMessageWrite(self, connMessageQuit(self->client));

    return ConnClientState_None;
}

Bool connClientStateIsEqual(ConnClient* self, ConnClientState state)
{
    return self->state_curr == state ? 1 : 0;
}

Bool connClientStateIsActive(ConnClient* self)
{
    if (connClientStateIsEqual(self, ConnClientState_Stopping) != 0) return 0;
    if (connClientStateIsEqual(self, ConnClientState_Error) != 0)    return 0;

    return 1;
}

void connClientStateSet(ConnClient* self, ConnClientState state)
{
    while (state != self->state_curr && state != ConnClientState_None) {
        self->state_prev = self->state_curr;
        self->state_curr = state;

        state = connClientOnStateChange(self);
    }
}

void connClientStateSetError(ConnClient* self)
{
    connClientStateSet(self, ConnClientState_Error);
}

#endif // CONN_CLIENT_C
