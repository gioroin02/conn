#ifndef PX_SYSTEM_NETWORK_SOCKET_TCP_H
#define PX_SYSTEM_NETWORK_SOCKET_TCP_H

#include "address-ip.h"

typedef void PxSocketTcp;

PxSocketTcp*
pxSocketTcpReserve(PxMemoryArena* arena);

b32
pxSocketTcpCreate(PxSocketTcp* self, PxAddressIp address, u16 port);

b32
pxSocketTcpAccept(PxSocketTcp* self, PxSocketTcp* value);

void
pxSocketTcpDestroy(PxSocketTcp* self);

b32
pxSocketTcpBind(PxSocketTcp* self);

b32
pxSocketTcpBindTo(PxSocketTcp* self, PxAddressIp address, u16 port);

b32
pxSocketTcpListen(PxSocketTcp* self);

b32
pxSocketTcpConnect(PxSocketTcp* self, PxAddressIp address, u16 port);

ssize
pxSocketTcpWrite(PxSocketTcp* self, u8* values, ssize start, ssize stop);

ssize
pxSocketTcpRead(PxSocketTcp* self, u8* values, ssize start, ssize stop);

PxAddressIp
pxSocketTcpGetAddress(PxSocketTcp* self);

u16
pxSocketTcpGetPort(PxSocketTcp* self);

#endif // PX_SYSTEM_NETWORK_SOCKET_TCP_H
