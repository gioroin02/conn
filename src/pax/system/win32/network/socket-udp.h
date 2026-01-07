#ifndef PX_WIN32_NETWORK_SOCKET_UDP_H
#define PX_WIN32_NETWORK_SOCKET_UDP_H

#include "address.h"

typedef struct PxWin32SocketUdp
{
    SOCKET                 handle;
    PxWin32SockAddrStorage storage;
}
PxWin32SocketUdp;

PxWin32SocketUdp*
pxWin32SocketUdpReserve(PxMemoryArena* arena);

b32
pxWin32SocketUdpCreate(PxWin32SocketUdp* self, PxAddressIp address, u16 port);

void
pxWin32SocketUdpDestroy(PxWin32SocketUdp* self);

b32
pxWin32SocketUdpBind(PxWin32SocketUdp* self);

b32
pxWin32SocketUdpBindTo(PxWin32SocketUdp* self, PxAddressIp address, u16 port);

ssize
pxWin32SocketUdpWrite(PxWin32SocketUdp* self, u8* values, ssize start, ssize stop, PxAddressIp address, u16 port);

ssize
pxWin32SocketUdpRead(PxWin32SocketUdp* self, u8* values, ssize start, ssize stop, PxAddressIp* address, u16* port);

PxAddressIp
pxWin32SocketUdpGetAddress(PxWin32SocketUdp* self);

u16
pxWin32SocketUdpGetPort(PxWin32SocketUdp* self);

#endif // PX_WIN32_NETWORK_SOCKET_UDP_H
