#ifndef CONN_SERVER_C
#define CONN_SERVER_C

#include "server.h"

static PString8 connServerStateName(ConnServerState state)
{
    switch (state) {
        case ConnServerState_None:        return pString8("None");
        case ConnServerState_Error:       return pString8("Error");
        case ConnServerState_Starting:    return pString8("Starting");
        case ConnServerState_Stopping:    return pString8("Stopping");
        case ConnServerState_SendingTurn: return pString8("SendingTurn");
        case ConnServerState_WaitingMove: return pString8("WaitingMove");
        case ConnServerState_Spinning:    return pString8("Spinning");

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

static void connBoardShow(ConnBoard* self)
{
    Int index = 0;
    Int row   = 0;
    Int col   = 0;

    char* colors[] = {"\x1b[0m", "\x1b[44m", "\x1b[41m"};

    for (index = 0; index < self->width; index += 1)
        printf("%s", index == 0 ? "+-----+" : "-----+");

    printf("\n");

    for (row = 0; row < self->height; row += 1) {
        printf("| ");

        for (col = 0; col < self->width; col += 1) {
            U16 value = connBoardGet(self, col, row);

            if (value > 0 && value < 3)
                printf("%s %u %s | ", colors[value], value, colors[0]);
            else
                printf("    | ");
        }

        printf("\n");

        for (index = 0; index < self->width; index += 1)
            printf("%s", index == 0 ? "+-----+" : "-----+");

        printf("\n");
    }
}

static void connServerOnTcpAccept(ConnServer* self, PSocketTcpEventAccept event)
{
    ConnSession* session = (ConnSession*) event.ctxt;

    if (event.value != NULL) {
        self->client_count += 1;

        connServerOnAccept(self, session);

        Int          index = self->client_count;
        ConnSession* value = pArrayGetPntr(&self->sessions, index);

        if (value != NULL) connServerAccept(self, value);
    }
    else connServerStateSetError(self);
}

static void connServerOnTcpWrite(ConnServer* self, PSocketTcpEventWrite event)
{
    ConnSession* session = (ConnSession*) event.ctxt;
    ConnMessage  message;

    if (event.start + event.bytes == event.stop) {
        if (pArrayRemoveFront(&session->messages, &message) == 0) return;

        Int count = connMessageEncode(message,
            session->buff_tcp_write, sizeof session->buff_tcp_write);

        event.pntr  = session->buff_tcp_write;
        event.start = 0;
        event.stop  = count;
    }
    else event.start += event.bytes;

    Bool status = pSocketTcpWriteAsync(session->socket, event.pntr,
        event.start, event.stop, self->queue, session);

    if (status != 0)
        connServerOnMessageWrite(self, session, message);
    else
        connServerStateSetError(self);
}

static void connServerOnTcpRead(ConnServer* self, PSocketTcpEventRead event)
{
    ConnSession* session = (ConnSession*) event.ctxt;
    ConnMessage  message;

    Int count = event.start + event.bytes;

    Bool status = connMessageDecode(&message, event.pntr, count);

    if (message.length <= 0 || message.length > count) {
        event.start += event.bytes;

        Bool status = pSocketTcpReadAsync(event.socket, event.pntr,
            event.start, event.stop, self->queue, NULL);

        if (status == 0) connServerStateSetError(self);
    }
    else connServerOnMessageRead(self, session, message);
}

static void connServerPollAsyncIo(ConnServer* self, PMemoryArena* arena)
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
                    case PSocketTcpEvent_Accept: connServerOnTcpAccept(self, event_tcp.accept); break;
                    case PSocketTcpEvent_Write:  connServerOnTcpWrite(self, event_tcp.write); break;
                    case PSocketTcpEvent_Read:   connServerOnTcpRead(self, event_tcp.read); break;

                    default: break;
                }
            } break;

            default: break;
        }
    }
}

Bool connServerCreate(ConnServer* self, PMemoryArena* arena)
{
    pMemorySet(self, sizeof *self, 0xAB);

    PMemoryPool pool = pMemoryPoolMake(
        pMemoryArenaReserveManyOf(arena, U8, pMemoryKIB(64)),
        pMemoryKIB(64), 512);

    self->queue    = pAsyncIoQueueReserve(arena);
    self->listener = pSocketTcpReserve(arena);

    self->spin_count   = 0;
    self->client_count = 0;
    self->player_count = 0;
    self->player_turn  = 0;

    if (pAsyncIoQueueCreate(self->queue, pool) == 0) return 0;

    PHostIp host = pHostIpMake(pAddressIp4Any(), 50000);

    if (pSocketTcpCreate(self->listener, host) == 0) return 0;

    pSocketTcpBind(self->listener);
    pSocketTcpListen(self->listener);

    if (pArrayCreate(&self->sessions, arena, 2) == 0) return 0;

    Int index = 0;

    for (index = 0; index < pArraySize(&self->sessions); index += 1) {
        if (pArrayAddBack(&self->sessions) == 0) return 0;

        Int          back  = pArrayBack(&self->sessions);
        ConnSession* value = pArrayGetPntr(&self->sessions, back);

        if (value == NULL) return 0;

        if (connSessionCreate(value, arena) == 0) return 0;
    }

    if (connBoardCreate(&self->board, arena, 9, 7) == 0) return 0;

    self->state_curr = ConnServerState_None;
    self->state_prev = ConnServerState_None;

    connServerStateSet(self, ConnServerState_Starting);

    return 1;
}

void connServerDestroy(ConnServer* self)
{
    pSocketTcpDestroy(self->listener);
    pAsyncIoQueueDestroy(self->queue);
}

void connServerStart(ConnServer* self)
{
    Int          index = self->client_count;
    ConnSession* value = pArrayGetPntr(&self->sessions, index);

    if (value != NULL)
        connServerAccept(self, value);
    else
        connServerStateSetError(self);
}

void connServerStop(ConnServer* self)
{
    ConnSession value;

    while (pArrayRemoveBack(&self->sessions, &value) != 0)
        connSessionDestroy(&value);
}

void connServerUpdate(ConnServer* self, PMemoryArena* arena)
{
    connServerPollAsyncIo(self, arena);

    ConnServerState state = connServerOnUpdate(self);

    connServerStateSet(self, state);
}

void connServerAccept(ConnServer* self, ConnSession* session)
{
    Bool status = pSocketTcpAcceptAsync(self->listener,
        session->socket, self->queue, session);

    if (status == 0) connServerStateSetError(self);
}

void connServerMessageBroad(ConnServer* self, ConnMessage message, ConnSession* session)
{
    Int index = 0;

    for (index = 0; index < self->player_count; index += 1) {
        ConnSession* value = pArrayGetPntr(&self->sessions, index);

        if (value != NULL && value != session)
            connServerMessageWrite(self, value, message);
    }
}

void connServerMessageWrite(ConnServer* self, ConnSession* session, ConnMessage message)
{
    PSocketTcp* socket = session->socket;
    U8*         pntr   = session->buff_tcp_write;

    if (pArrayIsFull(&session->messages) != 0) connServerStateSetError(self);

    if (pArrayIsEmpty(&session->messages) != 0) {
        Int count = connMessageEncode(message,
            session->buff_tcp_write, sizeof session->buff_tcp_write);

        Bool status = pSocketTcpWriteAsync(
            socket, pntr, 0, count, self->queue, session);

        if (status == 0) connServerStateSetError(self);
    }
    else pArrayInsertBack(&session->messages, message);
}

void connServerMessageRead(ConnServer* self, ConnSession* session)
{
    PSocketTcp* socket = session->socket;
    U8*         pntr   = session->buff_tcp_read;
    Int         count  = sizeof session->buff_tcp_read;

    Bool status = pSocketTcpReadAsync(
        socket, pntr, 0, count, self->queue, session);

    if (status == 0) connServerStateSetError(self);
}

/*
void connServerOnTcpEvent(ConnServer* self, PSocketTcpEvent event)
{
    ConnSession* session = (ConnSession*) event.ctxt;

    switch (event.kind) {
        case PSocketTcpEvent_Error: connServerStateSetError(self); break;

        case PSocketTcpEvent_Accept: {

        } break;

        case PSocketTcpEvent_Write: {
            ConnMessage message;

            if (pArrayRemoveFront(&session->messages, &message) == 0) break;

            PSocketTcp* socket = session->socket;
            U8*          pntr   = session->buff_tcp_write;
            Int        start  = 0;
            Int        stop   = 0;

            stop = connMessageEncode(message,
                session->buff_tcp_write, sizeof session->buff_tcp_write);

            Bool status = pSocketTcpWriteAsync(
                self->queue, session, socket, pntr, start, stop);

            if (status != 0)
                connServerOnTcpWrite(self, session, message);
            else
                connServerStateSetError(self);
        } break;

        case PSocketTcpEvent_Read: {
            PSocketTcp* socket = event.self;
            U8*          pntr   = event.read.pntr;
            Int        start  = event.read.stop;
            Int        stop   = sizeof session->buff_tcp_read;

            ConnMessage message = connMessageDecode(pntr, start);

            if (message.length <= 0 || start < message.length) {
                Bool status = pSocketTcpReadAsync(
                    self->queue, session, socket, pntr, start, stop);

                if (status == 0) connServerStateSetError(self);
            }
            else connServerOnTcpRead(self, session, message);
        } break;

        default: break;
    }
}
*/

void connServerOnAccept(ConnServer* self, ConnSession* session, PSocketTcp* socket)
{
    if (session != NULL) {
        printf("[DEBUG] Accepted (0x%016llx)!\n", (Uint) self);

        connServerMessageRead(self, session);

        if (connServerStateIsEqual(self, ConnServerState_Error) == 0)
            self->client_count += 1;
    }
    else connServerStateSetError(self);
}

void connServerOnMessageWrite(ConnServer* self, ConnSession* session, ConnMessage message)
{
    printf("[DEBUG] Wrote message (0x%016llx): ", (Uint) session);
        connMessageShow(message);
    printf("\n");
}

void connServerOnMessageRead(ConnServer* self, ConnSession* session, ConnMessage message)
{
    printf("[DEBUG] Read message (0x%016llx): ", (Uint) session);
        connMessageShow(message);
    printf("\n");

    switch (message.kind) {
        case ConnMessage_Join: {
            if (connServerStateIsEqual(self, ConnServerState_Starting) == 0) break;

            if (session->player.client == 0) {
                // TODO(gio): Add accept message

                session->player.flag   = message.join.flag;
                session->player.client = ((U16) self->player_count + 1);

                self->player_count += 1;

                connServerMessageBroad(self, connMessageData(
                    session->player.flag, session->player.client), NULL);

                Int index = 0;

                for (index = 0; index < self->player_count; index += 1) {
                    ConnSession* value = pArrayGetPntr(&self->sessions, index);

                    if (value == session) continue;

                    connServerMessageWrite(self, session, connMessageData(
                        value->player.flag, value->player.client));
                }

                if (self->player_count == pArraySize(&self->sessions))
                    connServerStateSet(self, ConnServerState_SendingTurn);
            }
            else {
                // TODO(gio): Add reject message
            }
        } break;

        case ConnMessage_Move: {
            if (connServerStateIsEqual(self, ConnServerState_WaitingMove) == 0) break;

            U16 client = message.move.client;
            U16 column = message.move.column;

            connBoardInsert(&self->board, column, client);
            connBoardShow(&self->board);

            connServerMessageBroad(self, message, session);

            U16 winner = 0;
            Bool full   = connBoardIsFull(&self->board);

            if (connBoardIsWinner(&self->board, column, client) != 0)
                winner = client;

            if (winner != 0 || full != 0) {
                connServerMessageBroad(self, connMessageResult(winner), NULL);

                connServerStateSet(self, ConnServerState_Spinning);
            }
            else connServerStateSet(self, ConnServerState_SendingTurn);
        } break;

        case ConnMessage_Quit: {
            connServerMessageBroad(self, connMessageResult(0), NULL);

            connServerStateSet(self, ConnServerState_Spinning);
        } break;

        default: break;
    }
}

ConnServerState connServerOnUpdate(ConnServer* self)
{
    if (connServerStateIsEqual(self, ConnServerState_Spinning) != 0) {
        self->spin_count += 1;

        if (self->spin_count >= 100)
            return ConnServerState_Stopping;
    }

    return ConnServerState_None;
}

ConnServerState connServerOnStateChange(ConnServer* self)
{
    PString8 prev = connServerStateName(self->state_prev);
    PString8 curr = connServerStateName(self->state_curr);

    printf("[DEBUG] State changed from <%s> to <%s>\n",
        prev.values, curr.values);

    if (connServerStateIsEqual(self, ConnServerState_SendingTurn) != 0) {
        Int        index   = self->player_turn;
        ConnSession* session = pArrayGetPntr(&self->sessions, index);

        connServerMessageBroad(self, connMessageTurn(
            self->player_turn + 1), NULL);

        self->player_turn = (self->player_turn + 1) % self->player_count;

        connServerMessageRead(self, session);

        return ConnServerState_WaitingMove;
    }

    return ConnServerState_None;
}

Bool
connSessionCreate(ConnSession* self, PMemoryArena* arena)
{
    self->socket = pSocketTcpReserve(arena);
    self->player = connPlayerMake(0, 0);

    if (self->socket == NULL) return 0;

    pMemorySet(self->buff_tcp_write, sizeof self->buff_tcp_write, 0x00);
    pMemorySet(self->buff_tcp_read,  sizeof self->buff_tcp_read,  0x00);

    if (pArrayCreate(&self->messages, arena, 16) == 0) return 0;

    return 1;
}

void
connSessionDestroy(ConnSession* self)
{
    pSocketTcpDestroy(self->socket);
}

Bool connServerStateIsEqual(ConnServer* self, ConnServerState state)
{
    return self->state_curr == state ? 1 : 0;
}

Bool connServerStateIsActive(ConnServer* self)
{
    if (connServerStateIsEqual(self, ConnServerState_Stopping) != 0) return 0;
    if (connServerStateIsEqual(self, ConnServerState_Error) != 0)    return 0;

    return 1;
}

void connServerStateSet(ConnServer* self, ConnServerState state)
{
    while (state != self->state_curr && state != ConnServerState_None) {
        self->state_prev = self->state_curr;
        self->state_curr = state;

        state = connServerOnStateChange(self);
    }
}

void connServerStateSetError(ConnServer* self)
{
    connServerStateSet(self, ConnServerState_Error);
}

#endif // CONN_SERVER_C
