#ifndef RN_SYSTEM_NETWORK_SOCKET_UDP_C
#define RN_SYSTEM_NETWORK_SOCKET_UDP_C

#include "./socket-udp.h"

#if RN_SYSTEM == RN_SYSTEM_WINDOWS

    #include "../../win32/network/export.c"

    #define __rnSocketUDPReserve__ rnWin32SocketUDPReserve
    #define __rnSocketUDPCreate__  rnWin32SocketUDPCreate
    #define __rnSocketUDPDestroy__ rnWin32SocketUDPDestroy
    #define __rnSocketUDPBind__    rnWin32SocketUDPBind
    #define __rnSocketUDPBindTo__  rnWin32SocketUDPBindTo
    #define __rnSocketUDPWrite__   rnWin32SocketUDPWrite
    #define __rnSocketUDPRead__    rnWin32SocketUDPRead

#else

    #error "Unknown platform"

#endif

RnSocketUDP*
rnSocketUDPReserve(RnMemoryArena* arena)
{
    return __rnSocketUDPReserve__(arena);
}

b32
rnSocketUDPCreate(RnSocketUDP* self, RnAddressIP address, u16 port)
{
    return __rnSocketUDPCreate__(self, address, port);
}

void
rnSocketUDPDestroy(RnSocketUDP* self)
{
    return __rnSocketUDPDestroy__(self);
}

b32
rnSocketUDPBind(RnSocketUDP* self)
{
    return __rnSocketUDPBind__(self);
}

b32
rnSocketUDPBindTo(RnSocketUDP* self, RnAddressIP address, u16 port)
{
    return __rnSocketUDPBindTo__(self, address, port);
}

ssize
rnSocketUDPWrite(RnSocketUDP* self, u8* values, ssize size, RnAddressIP address, u16 port)
{
    return __rnSocketUDPWrite__(self, values, size, address, port);
}

ssize
rnSocketUDPRead(RnSocketUDP* self, u8* values, ssize size, RnAddressIP* address, u16* port)
{
    return __rnSocketUDPRead__(self, values, size, address, port);
}

#endif // RN_SYSTEM_NETWORK_SOCKET_UDP_C
