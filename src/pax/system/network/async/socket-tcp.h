#ifndef PX_SYSTEM_ASYNC_NETWORK_SOCKET_TCP_H
#define PX_SYSTEM_ASYNC_NETWORK_SOCKET_TCP_H

#include "import.h"

typedef enum PxSocketTcpEventKind
{
    PxSocketTcpEvent_None,
    PxSocketTcpEvent_Error,
    PxSocketTcpEvent_Accept,
    PxSocketTcpEvent_Connect,
    PxSocketTcpEvent_Write,
    PxSocketTcpEvent_Read,
    PxSocketTcpEvent_Close,
}
PxSocketTcpEventKind;

typedef struct PxSocketTcpEventAccept
{
    PxSocketTcp* value;
}
PxSocketTcpEventAccept;

typedef struct PxSocketTcpEventConnect
{
    b32 status;
}
PxSocketTcpEventConnect;

typedef struct PxSocketTcpEventWrite
{
    u8*   pntr;
    ssize start;
    ssize stop;
}
PxSocketTcpEventWrite;

typedef struct PxSocketTcpEventRead
{
    u8*   pntr;
    ssize start;
    ssize stop;
}
PxSocketTcpEventRead;

typedef struct PxSocketTcpEvent
{
    PxSocketTcpEventKind kind;

    void*        ctxt;
    PxSocketTcp* self;

    union
    {
        PxSocketTcpEventAccept  accept;
        PxSocketTcpEventConnect connect;
        PxSocketTcpEventWrite   write;
        PxSocketTcpEventRead    read;
    };
}
PxSocketTcpEvent;

PxSocketTcpEvent pxSocketTcpEventAccept(void* ctxt, PxSocketTcp* self, PxSocketTcp* value);

PxSocketTcpEvent pxSocketTcpEventConnect(void* ctxt, PxSocketTcp* self, b32 status);

PxSocketTcpEvent pxSocketTcpEventWrite(void* ctxt, PxSocketTcp* self, u8* pntr, ssize start, ssize stop);

PxSocketTcpEvent pxSocketTcpEventRead(void* ctxt, PxSocketTcp* self, u8* pntr, ssize start, ssize stop);

PxSocketTcpEvent pxSocketTcpEventClose(void* ctxt, PxSocketTcp* self);

b32 pxSocketTcpAcceptAsync(PxAsync* async, void* ctxt, PxSocketTcp* self, PxSocketTcp* value);

b32 pxSocketTcpConnectAsync(PxAsync* async, void* ctxt, PxSocketTcp* self, PxAddressIp address, u16 port);

b32 pxSocketTcpWriteAsync(PxAsync* async, void* ctxt, PxSocketTcp* self, u8* pntr, ssize start, ssize stop);

b32 pxSocketTcpReadAsync(PxAsync* async, void* ctxt, PxSocketTcp* self, u8* pntr, ssize start, ssize stop);

#endif // PX_SYSTEM_ASYNC_NETWORK_SOCKET_TCP_H
