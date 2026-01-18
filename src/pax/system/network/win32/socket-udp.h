#ifndef P_SYSTEM_WIN32_NETWORK_SOCKET_UDP_H
#define P_SYSTEM_WIN32_NETWORK_SOCKET_UDP_H

#include "common.h"

typedef struct PWin32SocketUdp
{
    SOCKET            handle;
    PWin32AddrStorage storage;
}
PWin32SocketUdp;

PWin32SocketUdp* pWin32SocketUdpReserve(PMemoryArena* arena);

Bool pWin32SocketUdpCreate(PWin32SocketUdp* self, PHostIp host);

void pWin32SocketUdpDestroy(PWin32SocketUdp* self);

Bool pWin32SocketUdpBind(PWin32SocketUdp* self);

Bool pWin32SocketUdpBindAs(PWin32SocketUdp* self, PHostIp host);

Int pWin32SocketUdpWrite(PWin32SocketUdp* self, U8* pntr, Int start, Int stop, PHostIp host);

Int pWin32SocketUdpRead(PWin32SocketUdp* self, U8* pntr, Int start, Int stop, PHostIp* host);

PHostIp pWin32SocketUdpGetHost(PWin32SocketUdp* self);

#endif // P_SYSTEM_WIN32_NETWORK_SOCKET_UDP_H
