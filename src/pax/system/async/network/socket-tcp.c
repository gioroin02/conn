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
    PxSocketTcpEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind            = PxSocketTcpEvent_Accept;
    result.accept.listener = listener;
    result.accept.socket   = socket;

    return result;
}

PxSocketTcpEvent
pxSocketTcpEventConnect(PxSocketTcp* socket, b32 status)
{
    PxSocketTcpEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind           = PxSocketTcpEvent_Connect;
    result.connect.socket = socket;
    result.connect.status = status != 0 ? 1 : 0;

    return result;
}

PxSocketTcpEvent
pxSocketTcpEventWrite(PxSocketTcp* socket, u8* values, ssize start, ssize stop)
{
    PxSocketTcpEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind         = PxSocketTcpEvent_Write;
    result.write.socket = socket;
    result.write.values = values;
    result.write.start  = start,
    result.write.stop   = stop;

    return result;
}

PxSocketTcpEvent
pxSocketTcpEventRead(PxSocketTcp* socket, u8* values, ssize start, ssize stop)
{
    PxSocketTcpEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind        = PxSocketTcpEvent_Read;
    result.read.socket = socket;
    result.read.values = values;
    result.read.start  = start,
    result.read.stop   = stop;

    return result;
}

PxSocketTcpEvent
pxSocketTcpEventClose(PxSocketTcp* socket)
{
    PxSocketTcpEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind         = PxSocketTcpEvent_Close;
    result.close.socket = socket;

    return result;
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
