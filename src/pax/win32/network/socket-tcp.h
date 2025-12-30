#ifndef PX_WIN32_NETWORK_SOCKET_TCP_H
#define PX_WIN32_NETWORK_SOCKET_TCP_H

#include "address.h"

typedef struct PxWin32SocketTcp
{
    SOCKET                 handle;
    PxWin32SockAddrStorage storage;
}
PxWin32SocketTcp;

PxWin32SocketTcp*
pxWin32SocketTcpReserve(PxMemoryArena* arena);

b32
pxWin32SocketTcpCreate(PxWin32SocketTcp* self, PxAddressIp address, u16 port);

b32
pxWin32SocketTcpAccept(PxWin32SocketTcp* self, PxWin32SocketTcp* value);

void
pxWin32SocketTcpDestroy(PxWin32SocketTcp* self);

b32
pxWin32SocketTcpBind(PxWin32SocketTcp* self);

b32
pxWin32SocketTcpBindTo(PxWin32SocketTcp* self, PxAddressIp address, u16 port);

b32
pxWin32SocketTcpListen(PxWin32SocketTcp* self);

b32
pxWin32SocketTcpConnect(PxWin32SocketTcp* self, PxAddressIp address, u16 port);

ssize
pxWin32SocketTcpWrite(PxWin32SocketTcp* self, u8* values, ssize size);

ssize
pxWin32SocketTcpRead(PxWin32SocketTcp* self, u8* values, ssize size);

PxAddressIp
pxWin32SocketTcpGetAddress(PxWin32SocketTcp* self);

u16
pxWin32SocketTcpGetPort(PxWin32SocketTcp* self);

#endif // PX_WIN32_NETWORK_SOCKET_TCP_H
