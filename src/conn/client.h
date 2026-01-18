#ifndef CONN_CLIENT_H
#define CONN_CLIENT_H

#include "message.h"
#include "command.h"

#include <stdio.h>

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

typedef PArray(ConnMessage) ConnMessageArray;
typedef PArray(ConnPlayer)  ConnPlayerArray;
typedef PArray(ConnCommand) ConnCommandArray;

typedef struct ConnClient
{
    PAsyncIoQueue*   queue;
    PSocketTcp*      socket;
    PConsole*        console;
    ConnMessageArray messages;
    ConnPlayerArray  players;
    ConnCommandArray commands;

    ConnBoard board;

    U16 client;
    U16 column;
    Int player_count;

    U8 buff_tcp_write[CONN_MESSAGE_SIZE];
    U8 buff_tcp_read[CONN_MESSAGE_SIZE];

    ConnClientState state_curr;
    ConnClientState state_prev;
}
ConnClient;

Bool connClientCreate(ConnClient* self, PMemoryArena* arena);

void connClientDestroy(ConnClient* self);

void connClientStart(ConnClient* self);

void connClientStop(ConnClient* self);

void connClientUpdate(ConnClient* self, PMemoryArena* arena);

void connClientConnect(ConnClient* self, PHostIp host);

void connClientMessageWrite(ConnClient* self, ConnMessage message);

void connClientMessageRead(ConnClient* self);

void connClientOnConnect(ConnClient* self, PHostIp host);

void connClientOnMessageWrite(ConnClient* self, ConnMessage message);

void connClientOnMessageRead(ConnClient* self, ConnMessage message);

void connClientOnCommandRead(ConnClient* self, ConnCommand command);

ConnClientState connClientOnUpdate(ConnClient* self);

ConnClientState connClientOnStateChange(ConnClient* self);

Bool connClientStateIsEqual(ConnClient* self, ConnClientState state);

Bool connClientStateIsActive(ConnClient* self);

void connClientStateSet(ConnClient* self, ConnClientState state);

void connClientStateSetError(ConnClient* self);

#endif // CONN_CLIENT_H
