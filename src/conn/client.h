#ifndef CONN_CLIENT_H
#define CONN_CLIENT_H

#include "message.h"

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
    ConnClientState_ChoosingMove,
    ConnClientState_SendingMove,
    ConnClientState_WaitingMove,
}
ConnClientState;

typedef PxArray(ConnMessage) ConnMessageArray;
typedef PxArray(ConnPlayer)  ConnPlayerArray;

typedef struct ConnClient
{
    PxAsync*         async;
    PxSocketTcp*     socket;
    ConnMessageArray messages;
    ConnPlayerArray  players;

    u32 client;
    u32 column;

    ConnBoard board;

    u8 writing[CONN_MESSAGE_SIZE];
    u8 reading[CONN_MESSAGE_SIZE];

    ConnClientState state_curr;
    ConnClientState state_prev;
}
ConnClient;

ConnClient
connClientMake();

b32
connClientStateIsEqual(ConnClient* self, ConnClientState state);

b32
connClientStateIsActive(ConnClient* self);

void
connClientStateSet(ConnClient* self, ConnClientState state);

void
connClientStateSetError(ConnClient* self);

b32
connClientCreate(ConnClient* self, PxMemoryArena* arena);

void
connClientDestroy(ConnClient* self);

void
connClientStart(ConnClient* self);

void
connClientStop(ConnClient* self);

void
connClientUpdate(ConnClient* self);

void
connClientTcpConnect(ConnClient* self, PxAddressIp address, u16 port);

void
connClientTcpWrite(ConnClient* self, ConnMessage message);

void
connClientTcpRead(ConnClient* self);

void
connClientPollEvents(ConnClient* self);

void
connClientOnTcpEvent(ConnClient* self, PxSocketTcpEvent* event);

void
connClientOnTcpConnect(ConnClient* self);

void
connClientOnTcpWrite(ConnClient* self, ConnMessage message);

void
connClientOnTcpRead(ConnClient* self, ConnMessage message);

void
connClientOnStateChange(ConnClient* self, ConnClientState previous, ConnClientState current);

#endif // CONN_CLIENT_H
