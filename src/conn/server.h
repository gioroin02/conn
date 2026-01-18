#ifndef CONN_SERVER_H
#define CONN_SERVER_H

#include "message.h"

#include <stdio.h>

typedef enum ConnServerState
{
    ConnServerState_None,

    ConnServerState_Error,

    ConnServerState_Starting,
    ConnServerState_Stopping,

    ConnServerState_SendingTurn,
    ConnServerState_WaitingMove,
    ConnServerState_Spinning,
}
ConnServerState;

typedef PArray(ConnMessage) ConnMessageArray;

typedef struct ConnSession
{
    PSocketTcp*      socket;
    ConnMessageArray messages;
    ConnPlayer       player;

    U8 buff_tcp_write[CONN_MESSAGE_SIZE];
    U8 buff_tcp_read[CONN_MESSAGE_SIZE];
}
ConnSession;

typedef PArray(ConnSession) ConnSessionArray;

typedef struct ConnServer
{
    PAsyncIoQueue*   queue;
    PSocketTcp*      listener;
    ConnSessionArray sessions;

    ConnBoard board;

    Int spin_count;
    Int client_count;
    Int player_count;
    Int player_turn;

    ConnServerState state_curr;
    ConnServerState state_prev;
}
ConnServer;

Bool connServerCreate(ConnServer* self, PMemoryArena* arena);

void connServerDestroy(ConnServer* self);

void connServerStart(ConnServer* self);

void connServerStop(ConnServer* self);

void connServerUpdate(ConnServer* self, PMemoryArena* arena);

void connServerAccept(ConnServer* self, ConnSession* session);

void connServerMessageBroad(ConnServer* self, ConnMessage message, ConnSession* session);

void connServerMessageWrite(ConnServer* self, ConnSession* session, ConnMessage message);

void connServerTcpMessageRead(ConnServer* self, ConnSession* session);

void connServerOnAccept(ConnServer* self, ConnSession* session);

void connServerOnMessageWrite(ConnServer* self, ConnSession* session, ConnMessage message);

void connServerOnMessageRead(ConnServer* self, ConnSession* session, ConnMessage message);

ConnServerState connServerOnUpdate(ConnServer* self);

ConnServerState connServerOnStateChange(ConnServer* self);

Bool connSessionCreate(ConnSession* self, PMemoryArena* arena);

void connSessionDestroy(ConnSession* self);

Bool connServerStateIsEqual(ConnServer* self, ConnServerState state);

Bool connServerStateIsActive(ConnServer* self);

void connServerStateSet(ConnServer* self, ConnServerState state);

void connServerStateSetError(ConnServer* self);

#endif // CONN_SERVER_H
