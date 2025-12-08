#ifndef RN_SYSTEM_NETWORK_SOCKET_TCP_C
#define RN_SYSTEM_NETWORK_SOCKET_TCP_C

#include "./socket-tcp.h"

#if RN_SYSTEM == RN_SYSTEM_WINDOWS

    #include "../../win32/network/export.c"

    #define __rnSocketTCPReserve__ rnWin32SocketTCPReserve
    #define __rnSocketTCPCreate__  rnWin32SocketTCPCreate
    #define __rnSocketTCPAccept__  rnWin32SocketTCPAccept
    #define __rnSocketTCPDestroy__ rnWin32SocketTCPDestroy
    #define __rnSocketTCPBind__    rnWin32SocketTCPBind
    #define __rnSocketTCPBindTo__  rnWin32SocketTCPBindTo
    #define __rnSocketTCPListen__  rnWin32SocketTCPListen
    #define __rnSocketTCPConnect__ rnWin32SocketTCPConnect
    #define __rnSocketTCPWrite__   rnWin32SocketTCPWrite
    #define __rnSocketTCPRead__    rnWin32SocketTCPRead

#else

    #error "Unknown platform"

#endif

RnSocketTCP*
rnSocketTCPReserve(RnMemoryArena* arena)
{
    return __rnSocketTCPReserve__(arena);
}

b32
rnSocketTCPCreate(RnSocketTCP* self, RnAddressIP address, u16 port)
{
    return __rnSocketTCPCreate__(self, address, port);
}

b32
rnSocketTCPAccept(RnSocketTCP* self, RnSocketTCP* value)
{
    return __rnSocketTCPAccept__(self, value);
}

void
rnSocketTCPDestroy(RnSocketTCP* self)
{
    return __rnSocketTCPDestroy__(self);
}

b32
rnSocketTCPBind(RnSocketTCP* self)
{
    return __rnSocketTCPBind__(self);
}

b32
rnSocketTCPBindTo(RnSocketTCP* self, RnAddressIP address, u16 port)
{
    return __rnSocketTCPBindTo__(self, address, port);
}

b32
rnSocketTCPListen(RnSocketTCP* self)
{
    return __rnSocketTCPListen__(self);
}

b32
rnSocketTCPConnect(RnSocketTCP* self, RnAddressIP address, u16 port)
{
    return __rnSocketTCPConnect__(self, address, port);
}

ssize
rnSocketTCPWrite(RnSocketTCP* self, u8* values, ssize size)
{
    return __rnSocketTCPWrite__(self, values, size);
}

ssize
rnSocketTCPRead(RnSocketTCP* self, u8* values, ssize size)
{
    return __rnSocketTCPRead__(self, values, size);
}

#endif // RN_SYSTEM_NETWORK_SOCKET_TCP_C
