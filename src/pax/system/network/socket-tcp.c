#ifndef P_SYSTEM_NETWORK_SOCKET_TCP_C
#define P_SYSTEM_NETWORK_SOCKET_TCP_C

#include "socket-tcp.h"

#if P_SYSTEM == P_SYSTEM_WINDOWS

    #include "win32/export.c"

    #define __PSocketTcp__ PWin32SocketTcp

    #define __pSocketTcpReserve__ pWin32SocketTcpReserve
    #define __pSocketTcpCreate__  pWin32SocketTcpCreate
    #define __pSocketTcpAccept__  pWin32SocketTcpAccept
    #define __pSocketTcpDestroy__ pWin32SocketTcpDestroy
    #define __pSocketTcpBind__    pWin32SocketTcpBind
    #define __pSocketTcpBindAs__  pWin32SocketTcpBindAs
    #define __pSocketTcpListen__  pWin32SocketTcpListen
    #define __pSocketTcpConnect__ pWin32SocketTcpConnect
    #define __pSocketTcpWrite__   pWin32SocketTcpWrite
    #define __pSocketTcpRead__    pWin32SocketTcpRead
    #define __pSocketTcpGetHost__ pWin32SocketTcpGetHost

#else

    #error "Unknown platform"

#endif

PSocketTcp* pSocketTcpReserve(PMemoryArena* arena)
{
    return (PSocketTcp*)  __pSocketTcpReserve__(arena);
}

Bool pSocketTcpCreate(PSocketTcp* self, PHostIp host)
{
    return __pSocketTcpCreate__((__PSocketTcp__*) self, host);
}

Bool pSocketTcpAccept(PSocketTcp* self, PSocketTcp* value)
{
    return __pSocketTcpAccept__((__PSocketTcp__*) self, (__PSocketTcp__*) value);
}

void pSocketTcpDestroy(PSocketTcp* self)
{
    return __pSocketTcpDestroy__((__PSocketTcp__*) self);
}

Bool pSocketTcpBind(PSocketTcp* self)
{
    return __pSocketTcpBind__((__PSocketTcp__*) self);
}

Bool pSocketTcpBindAs(PSocketTcp* self, PHostIp host)
{
    return __pSocketTcpBindAs__((__PSocketTcp__*) self, host);
}

Bool pSocketTcpListen(PSocketTcp* self)
{
    return __pSocketTcpListen__((__PSocketTcp__*) self);
}

Bool pSocketTcpConnect(PSocketTcp* self, PHostIp host)
{
    return __pSocketTcpConnect__((__PSocketTcp__*) self, host);
}

Int
pSocketTcpWrite(PSocketTcp* self, U8* pntr, Int start, Int stop)
{
    return __pSocketTcpWrite__((__PSocketTcp__*) self, pntr, start, stop);
}

Int pSocketTcpRead(PSocketTcp* self, U8* pntr, Int start, Int stop)
{
    return __pSocketTcpRead__((__PSocketTcp__*) self, pntr, start, stop);
}

PHostIp pSocketTcpGetHost(PSocketTcp* self)
{
    return __pSocketTcpGetHost__((__PSocketTcp__*) self);
}

#endif // P_SYSTEM_NETWORK_SOCKET_TCP_C
