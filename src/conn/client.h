#ifndef CONN_CLIENT_H
#define CONN_CLIENT_H

#include "message.h"
#include "command.h"

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
typedef PxArray(ConnCommand) ConnCommandArray;

typedef struct ConnClient
{
    PxAsync*         async;
    PxAsync*         async2;
    PxSocketTcp*     socket;
    PxConsole*       console;
    ConnMessageArray messages;
    ConnPlayerArray  players;
    ConnCommandArray commands;

    ConnBoard board;

    u16   client;
    u16   column;
    ssize player_count;

    u8 buff_tcp_write[CONN_MESSAGE_SIZE];
    u8 buff_tcp_read[CONN_MESSAGE_SIZE];
    u8 buff_term_read[16];

    ConnClientState state_curr;
    ConnClientState state_prev;
}
ConnClient;

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
connClientFileRead(ConnClient* self);

void
connClientPollEvents(ConnClient* self);

void
connClientOnTcpEvent(ConnClient* self, PxSocketTcpEvent event);

void
connClientOnTcpConnect(ConnClient* self);

void
connClientOnTcpWrite(ConnClient* self, ConnMessage message);

void
connClientOnTcpRead(ConnClient* self, ConnMessage message);

void
connClientOnFileEvent(ConnClient* self, PxFileEvent event);

void
connClientOnCommand(ConnClient* self, ConnCommand command);

ConnClientState
connClientOnUpdate(ConnClient* self);

ConnClientState
connClientOnStateChange(ConnClient* self);

#endif // CONN_CLIENT_H
