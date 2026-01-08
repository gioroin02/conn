#ifndef CONN_SERVER_H
#define CONN_SERVER_H

#include "message.h"

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
    ConnServerState_Spinning,
}
ConnServerState;

typedef PxArray(ConnMessage) ConnMessageArray;

typedef struct ConnSession
{
    PxSocketTcp*     socket;
    ConnMessageArray messages;
    ConnPlayer       player;

    u8 buff_tcp_write[CONN_MESSAGE_SIZE];
    u8 buff_tcp_read[CONN_MESSAGE_SIZE];
}
ConnSession;

typedef PxArray(ConnSession) ConnSessionArray;

typedef struct ConnServer
{
    PxAsync*         async;
    PxSocketTcp*     listener;
    ConnSessionArray sessions;

    ConnBoard board;

    ssize spin_count;
    ssize client_count;
    ssize player_count;
    ssize player_turn;

    ConnServerState state_curr;
    ConnServerState state_prev;
}
ConnServer;

b32
connServerStateIsEqual(ConnServer* self, ConnServerState state);

b32
connServerStateIsActive(ConnServer* self);

void
connServerStateSet(ConnServer* self, ConnServerState state);

void
connServerStateSetError(ConnServer* self);

b32
connServerCreate(ConnServer* self, PxMemoryArena* arena);

void
connServerDestroy(ConnServer* self);

void
connServerStart(ConnServer* self);

void
connServerStop(ConnServer* self);

void
connServerUpdate(ConnServer* self);

void
connServerTcpAccept(ConnServer* self, ConnSession* session);

void
connServerTcpBroadcast(ConnServer* self, ConnSession* session, ConnMessage message);

void
connServerTcpWrite(ConnServer* self, ConnSession* session, ConnMessage message);

void
connServerTcpRead(ConnServer* self, ConnSession* session);

void
connServerPollEvents(ConnServer* self);

void
connServerOnTcpEvent(ConnServer* self, PxSocketTcpEvent event);

void
connServerOnTcpAccept(ConnServer* self, ConnSession* session);

void
connServerOnTcpWrite(ConnServer* self, ConnSession* session, ConnMessage message);

void
connServerOnTcpRead(ConnServer* self, ConnSession* session, ConnMessage message);

ConnServerState
connServerOnUpdate(ConnServer* self);

ConnServerState
connServerOnStateChange(ConnServer* self);

b32
connSessionCreate(ConnSession* self, PxMemoryArena* arena);

void
connSessionDestroy(ConnSession* self);

#endif // CONN_SERVER_H
