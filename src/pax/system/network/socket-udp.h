#ifndef P_SYSTEM_NETWORK_SOCKET_UDP_H
#define P_SYSTEM_NETWORK_SOCKET_UDP_H

#include "address-ip.h"

typedef struct PSocketUdp { U8 __private__; } PSocketUdp;

PSocketUdp* pSocketUdpReserve(PMemoryArena* arena);

Bool pSocketUdpCreate(PSocketUdp* self, PHostIp host);

void pSocketUdpDestroy(PSocketUdp* self);

Bool pSocketUdpBind(PSocketUdp* self);

Bool pSocketUdpBindAs(PSocketUdp* self, PHostIp host);

Int pSocketUdpWrite(PSocketUdp* self, U8* pntr, Int start, Int stop, PHostIp host);

Int pSocketUdpRead(PSocketUdp* self, U8* pntr, Int start, Int stop, PHostIp* host);

PHostIp pSocketUdpGetHost(PSocketUdp* self);

#endif // P_SYSTEM_NETWORK_SOCKET_UDP_H
