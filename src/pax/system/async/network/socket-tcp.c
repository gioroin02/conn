#ifndef PX_SYSTEM_ASYNC_NETWORK_SOCKET_TCP_C
#define PX_SYSTEM_ASYNC_NETWORK_SOCKET_TCP_C

#include "socket-tcp.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../../../win32/async/network/export.c"

    #define __pxSocketTcpAcceptAsync__  pxWin32SocketTcpAcceptAsync
    #define __pxSocketTcpConnectAsync__ pxWin32SocketTcpConnectAsync
    #define __pxSocketTcpWriteAsync__   pxWin32SocketTcpWriteAsync
    #define __pxSocketTcpReadAsync__    pxWin32SocketTcpReadAsync

#else

    #error "Unknown platform"

#endif

PxSocketTcpEvent
pxSocketTcpEventAccept(PxSocketTcp* listener, PxSocketTcp* socket)
{
    return (PxSocketTcpEvent) {
        .kind = PxSocketTcpEvent_Accept,

        .accept = {
            .listener = listener,
            .socket   = socket,
        },
    };
}

PxSocketTcpEvent
pxSocketTcpEventConnect(PxSocketTcp* socket, b32 status)
{
    return (PxSocketTcpEvent) {
        .kind = PxSocketTcpEvent_Connect,

        .connect = {
            .socket = socket,
            .status = status != 0 ? 1 : 0,
        },
    };
}

PxSocketTcpEvent
pxSocketTcpEventWrite(PxSocketTcp* socket, u8* values, ssize start, ssize stop)
{
    return (PxSocketTcpEvent) {
        .kind = PxSocketTcpEvent_Write,

        .write = {
            .socket = socket,
            .values = values,
            .start  = start,
            .stop   = stop,
        },
    };
}

PxSocketTcpEvent
pxSocketTcpEventRead(PxSocketTcp* socket, u8* values, ssize start, ssize stop)
{
    return (PxSocketTcpEvent) {
        .kind = PxSocketTcpEvent_Read,

        .read = {
            .socket = socket,
            .values = values,
            .start  = start,
            .stop   = stop,
        },
    };
}

PxSocketTcpEvent
pxSocketTcpEventClose(PxSocketTcp* socket)
{
    return (PxSocketTcpEvent) {
        .kind = PxSocketTcpEvent_Close,

        .close = {
            .socket = socket,
        },
    };
}

b32
pxSocketTcpAcceptAsync(PxAsync* async, void* tag, PxSocketTcp* self, PxSocketTcp* value)
{
    return __pxSocketTcpAcceptAsync__(async, tag, self, value);
}

b32
pxSocketTcpConnectAsync(PxAsync* async, void* tag, PxSocketTcp* self, PxAddressIp address, u16 port)
{
    return __pxSocketTcpConnectAsync__(async, tag, self, address, port);
}

b32
pxSocketTcpWriteAsync(PxAsync* async, void* tag, PxSocketTcp* self, u8* values, ssize start, ssize stop)
{
    return __pxSocketTcpWriteAsync__(async, tag, self, values, start, stop);
}

b32
pxSocketTcpReadAsync(PxAsync* async, void* tag, PxSocketTcp* self, u8* values, ssize start, ssize stop)
{
    return __pxSocketTcpReadAsync__(async, tag, self, values, start, stop);
}

#endif // PX_SYSTEM_ASYNC_NETWORK_SOCKET_TCP_C
