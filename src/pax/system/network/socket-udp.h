#ifndef PX_SYSTEM_NETWORK_SOCKET_UDP_H
#define PX_SYSTEM_NETWORK_SOCKET_UDP_H

#include "address-ip.h"

typedef void PxSocketUdp;

PxSocketUdp*
pxSocketUdpReserve(PxMemoryArena* arena);

b32
pxSocketUdpCreate(PxSocketUdp* self, PxAddressIp address, u16 port);

void
pxSocketUdpDestroy(PxSocketUdp* self);

b32
pxSocketUdpBind(PxSocketUdp* self);

b32
pxSocketUdpBindTo(PxSocketUdp* self, PxAddressIp address, u16 port);

ssize
pxSocketUdpWrite(PxSocketUdp* self, u8* values, ssize size, PxAddressIp address, u16 port);

ssize
pxSocketUdpRead(PxSocketUdp* self, u8* values, ssize size, PxAddressIp* address, u16* port);

PxAddressIp
pxSocketUdpGetAddress(PxSocketUdp* self);

u16
pxSocketUdpGetPort(PxSocketUdp* self);

#endif // PX_SYSTEM_NETWORK_SOCKET_UDP_H
