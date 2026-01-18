#ifndef P_SYSTEM_NETWORK_SOCKET_TCP_H
#define P_SYSTEM_NETWORK_SOCKET_TCP_H

#include "address-ip.h"

typedef struct PSocketTcp { U8 __private__; } PSocketTcp;

PSocketTcp* pSocketTcpReserve(PMemoryArena* arena);

Bool pSocketTcpCreate(PSocketTcp* self, PHostIp host);

Bool pSocketTcpAccept(PSocketTcp* self, PSocketTcp* value);

void pSocketTcpDestroy(PSocketTcp* self);

Bool pSocketTcpBind(PSocketTcp* self);

Bool pSocketTcpBindAs(PSocketTcp* self, PHostIp host);

Bool pSocketTcpListen(PSocketTcp* self);

Bool pSocketTcpConnect(PSocketTcp* self, PHostIp host);

Int pSocketTcpWrite(PSocketTcp* self, U8* pntr, Int start, Int stop);

Int pSocketTcpRead(PSocketTcp* self, U8* pntr, Int start, Int stop);

PHostIp pSocketTcpGetHost(PSocketTcp* self);

#endif // P_SYSTEM_NETWORK_SOCKET_TCP_H
