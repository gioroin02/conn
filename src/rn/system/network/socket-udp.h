#ifndef RN_SYSTEM_NETWORK_SOCKET_UDP_H
#define RN_SYSTEM_NETWORK_SOCKET_UDP_H

#include "./address-ip.h"

typedef void RnSocketUDP;

RnSocketUDP*
rnSocketUDPReserve(RnMemoryArena* arena);

b32
rnSocketUDPCreate(RnSocketUDP* self, RnAddressIP address, u16 port);

void
rnSocketUDPDestroy(RnSocketUDP* self);

b32
rnSocketUDPBind(RnSocketUDP* self);

b32
rnSocketUDPBindTo(RnSocketUDP* self, RnAddressIP address, u16 port);

ssize
rnSocketUDPWrite(RnSocketUDP* self, u8* values, ssize size, RnAddressIP address, u16 port);

ssize
rnSocketUDPRead(RnSocketUDP* self, u8* values, ssize size, RnAddressIP* address, u16* port);

#endif // RN_SYSTEM_NETWORK_SOCKET_UDP_H
