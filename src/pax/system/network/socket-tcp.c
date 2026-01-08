#ifndef PX_SYSTEM_NETWORK_SOCKET_TCP_C
#define PX_SYSTEM_NETWORK_SOCKET_TCP_C

#include "socket-tcp.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../win32/network/export.c"

    #define __pxSocketTcpReserve__ pxWin32SocketTcpReserve
    #define __pxSocketTcpCreate__  pxWin32SocketTcpCreate
    #define __pxSocketTcpAccept__  pxWin32SocketTcpAccept
    #define __pxSocketTcpDestroy__ pxWin32SocketTcpDestroy
    #define __pxSocketTcpBind__    pxWin32SocketTcpBind
    #define __pxSocketTcpBindTo__  pxWin32SocketTcpBindTo
    #define __pxSocketTcpListen__  pxWin32SocketTcpListen
    #define __pxSocketTcpConnect__ pxWin32SocketTcpConnect
    #define __pxSocketTcpWrite__   pxWin32SocketTcpWrite
    #define __pxSocketTcpRead__    pxWin32SocketTcpRead

    #define __pxSocketTcpGetAddress__ pxWin32SocketTcpGetAddress
    #define __pxSocketTcpGetPort__    pxWin32SocketTcpGetPort

#else

    #error "Unknown platform"

#endif

PxSocketTcp* pxSocketTcpReserve(PxMemoryArena* arena)
{
    return __pxSocketTcpReserve__(arena);
}

b32 pxSocketTcpCreate(PxSocketTcp* self, PxAddressIp address, u16 port)
{
    return __pxSocketTcpCreate__(self, address, port);
}

b32 pxSocketTcpAccept(PxSocketTcp* self, PxSocketTcp* value)
{
    return __pxSocketTcpAccept__(self, value);
}

void pxSocketTcpDestroy(PxSocketTcp* self)
{
    return __pxSocketTcpDestroy__(self);
}

b32 pxSocketTcpBind(PxSocketTcp* self)
{
    return __pxSocketTcpBind__(self);
}

b32 pxSocketTcpBindTo(PxSocketTcp* self, PxAddressIp address, u16 port)
{
    return __pxSocketTcpBindTo__(self, address, port);
}

b32 pxSocketTcpListen(PxSocketTcp* self)
{
    return __pxSocketTcpListen__(self);
}

b32 pxSocketTcpConnect(PxSocketTcp* self, PxAddressIp address, u16 port)
{
    return __pxSocketTcpConnect__(self, address, port);
}

ssize
pxSocketTcpWrite(PxSocketTcp* self, u8* pntr, ssize start, ssize stop)
{
    return __pxSocketTcpWrite__(self, pntr, start, stop);
}

ssize pxSocketTcpRead(PxSocketTcp* self, u8* pntr, ssize start, ssize stop)
{
    return __pxSocketTcpRead__(self, pntr, start, stop);
}

PxAddressIp pxSocketTcpGetAddress(PxSocketTcp* self)
{
    return __pxSocketTcpGetAddress__(self);
}

u16 pxSocketTcpGetPort(PxSocketTcp* self)
{
    return __pxSocketTcpGetPort__(self);
}

#endif // PX_SYSTEM_NETWORK_SOCKET_TCP_C
