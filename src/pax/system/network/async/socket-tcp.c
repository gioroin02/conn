#ifndef P_SYSTEM_NETWORK_ASYNC_SOCKET_TCP_C
#define P_SYSTEM_NETWORK_ASYNC_SOCKET_TCP_C

#include "socket-tcp.h"

#if P_SYSTEM == P_SYSTEM_WINDOWS

    #include "win32/export.c"

    #define __PAsyncIoQueue__ PWin32AsyncIoQueue
    #define __PSocketTcp__    PWin32SocketTcp

    #define __pSocketTcpAcceptAsync__  pWin32SocketTcpAcceptAsync
    #define __pSocketTcpConnectAsync__ pWin32SocketTcpConnectAsync
    #define __pSocketTcpWriteAsync__   pWin32SocketTcpWriteAsync
    #define __pSocketTcpReadAsync__    pWin32SocketTcpReadAsync

#else

    #error "Unknown platform"

#endif

PSocketTcpEvent pSocketTcpEventAccept(PSocketTcp* self, PSocketTcp* value, void* ctxt)
{
    PSocketTcpEvent result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind          = PSocketTcpEvent_Accept;
    result.ctxt          = ctxt;
    result.accept.socket = self;
    result.accept.value  = value;

    return result;
}

PSocketTcpEvent pSocketTcpEventConnect(PSocketTcp* self, PHostIp host, Bool status, void* ctxt)
{
    PSocketTcpEvent result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind           = PSocketTcpEvent_Connect;
    result.ctxt           = ctxt;
    result.connect.socket = self;
    result.connect.host   = host;
    result.connect.status = status;

    return result;
}

PSocketTcpEvent pSocketTcpEventWrite(PSocketTcp* self, U8* pntr, Int start, Int stop, Int bytes, void* ctxt)
{
    PSocketTcpEvent result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind         = PSocketTcpEvent_Write;
    result.ctxt         = ctxt;
    result.write.socket = self;
    result.write.pntr   = pntr;
    result.write.start  = start;
    result.write.stop   = stop;
    result.write.bytes  = bytes;

    return result;
}

PSocketTcpEvent pSocketTcpEventRead(PSocketTcp* self, U8* pntr, Int start, Int stop, Int bytes, void* ctxt)
{
    PSocketTcpEvent result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind        = PSocketTcpEvent_Read;
    result.ctxt        = ctxt;
    result.read.socket = self;
    result.read.pntr   = pntr;
    result.read.start  = start;
    result.read.stop   = stop;
    result.read.bytes  = bytes;

    return result;
}

Bool pSocketTcpAcceptAsync(PSocketTcp* self, PSocketTcp* value, PAsyncIoQueue* queue, void* ctxt)
{
    return __pSocketTcpAcceptAsync__((__PSocketTcp__*) self, (__PSocketTcp__*) value, (__PAsyncIoQueue__*) queue, ctxt);
}

Bool pSocketTcpConnectAsync(PSocketTcp* self, PHostIp host, PAsyncIoQueue* queue, void* ctxt)
{
    return __pSocketTcpConnectAsync__((__PSocketTcp__*) self, host, (__PAsyncIoQueue__*) queue, ctxt);
}

Bool pSocketTcpWriteAsync(PSocketTcp* self, U8* pntr, Int start, Int stop, PAsyncIoQueue* queue, void* ctxt)
{
    return __pSocketTcpWriteAsync__((__PSocketTcp__*) self, pntr, start, stop, (__PAsyncIoQueue__*) queue, ctxt);
}

Bool pSocketTcpReadAsync(PSocketTcp* self, U8* pntr, Int start, Int stop, PAsyncIoQueue* queue, void* ctxt)
{
    return __pSocketTcpReadAsync__((__PSocketTcp__*) self, pntr, start, stop, (__PAsyncIoQueue__*) queue, ctxt);
}

#endif // P_SYSTEM_NETWORK_ASYNC_SOCKET_TCP_C
