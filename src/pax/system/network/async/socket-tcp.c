#ifndef PX_SYSTEM_ASYNC_NETWORK_SOCKET_TCP_C
#define PX_SYSTEM_ASYNC_NETWORK_SOCKET_TCP_C

#include "socket-tcp.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../../win32/network/async/export.c"

    #define __pxSocketTcpAcceptAsync__  pxWin32SocketTcpAcceptAsync
    #define __pxSocketTcpConnectAsync__ pxWin32SocketTcpConnectAsync
    #define __pxSocketTcpWriteAsync__   pxWin32SocketTcpWriteAsync
    #define __pxSocketTcpReadAsync__    pxWin32SocketTcpReadAsync

#else

    #error "Unknown platform"

#endif

PxSocketTcpEvent pxSocketTcpEventAccept(void* ctxt, PxSocketTcp* self, PxSocketTcp* value)
{
    PxSocketTcpEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind         = PxSocketTcpEvent_Accept;
    result.ctxt         = ctxt;
    result.self         = self;
    result.accept.value = value;

    return result;
}

PxSocketTcpEvent pxSocketTcpEventConnect(void* ctxt, PxSocketTcp* self, b32 status)
{
    PxSocketTcpEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind           = PxSocketTcpEvent_Connect;
    result.ctxt           = ctxt;
    result.self           = self;
    result.connect.status = status != 0 ? 1 : 0;

    return result;
}

PxSocketTcpEvent pxSocketTcpEventWrite(void* ctxt, PxSocketTcp* self, u8* pntr, ssize start, ssize stop)
{
    PxSocketTcpEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind        = PxSocketTcpEvent_Write;
    result.ctxt        = ctxt;
    result.self        = self;
    result.write.pntr  = pntr;
    result.write.start = start,
    result.write.stop  = stop;

    return result;
}

PxSocketTcpEvent pxSocketTcpEventRead(void* ctxt, PxSocketTcp* self, u8* pntr, ssize start, ssize stop)
{
    PxSocketTcpEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind       = PxSocketTcpEvent_Read;
    result.ctxt       = ctxt;
    result.self       = self;
    result.read.pntr  = pntr;
    result.read.start = start,
    result.read.stop  = stop;

    return result;
}

PxSocketTcpEvent pxSocketTcpEventClose(void* ctxt, PxSocketTcp* self)
{
    PxSocketTcpEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind = PxSocketTcpEvent_Close;
    result.ctxt = ctxt;
    result.self = self;

    return result;
}

b32 pxSocketTcpAcceptAsync(PxAsync* async, void* ctxt, PxSocketTcp* self, PxSocketTcp* value)
{
    return __pxSocketTcpAcceptAsync__(async, ctxt, self, value);
}

b32 pxSocketTcpConnectAsync(PxAsync* async, void* ctxt, PxSocketTcp* self, PxAddressIp address, u16 port)
{
    return __pxSocketTcpConnectAsync__(async, ctxt, self, address, port);
}

b32 pxSocketTcpWriteAsync(PxAsync* async, void* ctxt, PxSocketTcp* self, u8* pntr, ssize start, ssize stop)
{
    return __pxSocketTcpWriteAsync__(async, ctxt, self, pntr, start, stop);
}

b32 pxSocketTcpReadAsync(PxAsync* async, void* ctxt, PxSocketTcp* self, u8* pntr, ssize start, ssize stop)
{
    return __pxSocketTcpReadAsync__(async, ctxt, self, pntr, start, stop);
}

#endif // PX_SYSTEM_ASYNC_NETWORK_SOCKET_TCP_C
