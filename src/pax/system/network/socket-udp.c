#ifndef PX_SYSTEM_NETWORK_SOCKET_UDP_C
#define PX_SYSTEM_NETWORK_SOCKET_UDP_C

#include "socket-udp.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../../win32/network/export.c"

    #define __pxSocketUdpReserve__ pxWin32SocketUdpReserve
    #define __pxSocketUdpCreate__  pxWin32SocketUdpCreate
    #define __pxSocketUdpDestroy__ pxWin32SocketUdpDestroy
    #define __pxSocketUdpBind__    pxWin32SocketUdpBind
    #define __pxSocketUdpBindTo__  pxWin32SocketUdpBindTo
    #define __pxSocketUdpWrite__   pxWin32SocketUdpWrite
    #define __pxSocketUdpRead__    pxWin32SocketUdpRead

    #define __pxSocketUdpGetAddress__ pxWin32SocketUdpGetAddress
    #define __pxSocketUdpGetPort__    pxWin32SocketUdpGetPort

#else

    #error "Unknown platform"

#endif

PxSocketUdp*
pxSocketUdpReserve(PxMemoryArena* arena)
{
    return __pxSocketUdpReserve__(arena);
}

b32
pxSocketUdpCreate(PxSocketUdp* self, PxAddressIp address, u16 port)
{
    return __pxSocketUdpCreate__(self, address, port);
}

void
pxSocketUdpDestroy(PxSocketUdp* self)
{
    return __pxSocketUdpDestroy__(self);
}

b32
pxSocketUdpBind(PxSocketUdp* self)
{
    return __pxSocketUdpBind__(self);
}

b32
pxSocketUdpBindTo(PxSocketUdp* self, PxAddressIp address, u16 port)
{
    return __pxSocketUdpBindTo__(self, address, port);
}

ssize
pxSocketUdpWrite(PxSocketUdp* self, u8* values, ssize size, PxAddressIp address, u16 port)
{
    return __pxSocketUdpWrite__(self, values, size, address, port);
}

ssize
pxSocketUdpRead(PxSocketUdp* self, u8* values, ssize size, PxAddressIp* address, u16* port)
{
    return __pxSocketUdpRead__(self, values, size, address, port);
}

PxAddressIp
pxSocketUdpGetAddress(PxSocketUdp* self)
{
    return __pxSocketUdpGetAddress__(self);
}

u16
pxSocketUdpGetPort(PxSocketUdp* self)
{
    return __pxSocketUdpGetPort__(self);
}

#endif // PX_SYSTEM_NETWORK_SOCKET_UDP_C
