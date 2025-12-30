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
    PxSocketTcp* listener;
    PxSocketTcp* socket;
}
PxSocketTcpEventAccept;

typedef struct PxSocketTcpEventConnect
{
    PxSocketTcp* socket;
    b32          status;
}
PxSocketTcpEventConnect;

typedef struct PxSocketTcpEventWrite
{
    PxSocketTcp* socket;
    u8*          values;
    ssize        start;
    ssize        stop;
}
PxSocketTcpEventWrite;

typedef struct PxSocketTcpEventRead
{
    PxSocketTcp* socket;
    u8*          values;
    ssize        start;
    ssize        stop;
}
PxSocketTcpEventRead;

typedef struct PxSocketTcpEventClose
{
    PxSocketTcp* socket;
}
PxSocketTcpEventClose;

typedef struct PxSocketTcpEvent
{
    PxSocketTcpEventKind kind;

    union
    {
        PxSocketTcpEventAccept  accept;
        PxSocketTcpEventConnect connect;
        PxSocketTcpEventWrite   write;
        PxSocketTcpEventRead    read;
        PxSocketTcpEventClose   close;
    };
}
PxSocketTcpEvent;

PxSocketTcpEvent
pxSocketTcpEventAccept(PxSocketTcp* listener, PxSocketTcp* socket);

PxSocketTcpEvent
pxSocketTcpEventConnect(PxSocketTcp* socket, b32 status);

PxSocketTcpEvent
pxSocketTcpEventWrite(PxSocketTcp* socket, u8* values, ssize start, ssize stop);

PxSocketTcpEvent
pxSocketTcpEventRead(PxSocketTcp* socket, u8* values, ssize start, ssize stop);

PxSocketTcpEvent
pxSocketTcpEventClose(PxSocketTcp* socket);

b32
pxSocketTcpAcceptAsync(PxAsync* async, void* tag, PxSocketTcp* self, PxSocketTcp* value);

b32
pxSocketTcpConnectAsync(PxAsync* async, void* tag, PxSocketTcp* self, PxAddressIp address, u16 port);

b32
pxSocketTcpWriteAsync(PxAsync* async, void* tag, PxSocketTcp* self, u8* values, ssize start, ssize stop);

b32
pxSocketTcpReadAsync(PxAsync* async, void* tag, PxSocketTcp* self, u8* values, ssize start, ssize stop);

#endif // PX_SYSTEM_ASYNC_NETWORK_SOCKET_TCP_H
