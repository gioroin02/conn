#ifndef P_SYSTEM_WIN32_NETWORK_SOCKET_TCP_H
#define P_SYSTEM_WIN32_NETWORK_SOCKET_TCP_H

#include "common.h"

typedef struct PWin32SocketTcp
{
    SOCKET            handle;
    PWin32AddrStorage storage;
}
PWin32SocketTcp;

PWin32SocketTcp* pWin32SocketTcpReserve(PMemoryArena* arena);

Bool pWin32SocketTcpCreate(PWin32SocketTcp* self, PHostIp host);

Bool pWin32SocketTcpAccept(PWin32SocketTcp* self, PWin32SocketTcp* value);

void pWin32SocketTcpDestroy(PWin32SocketTcp* self);

Bool pWin32SocketTcpBind(PWin32SocketTcp* self);

Bool pWin32SocketTcpBindAs(PWin32SocketTcp* self, PHostIp host);

Bool pWin32SocketTcpListen(PWin32SocketTcp* self);

Bool pWin32SocketTcpConnect(PWin32SocketTcp* self, PHostIp host);

Int pWin32SocketTcpWrite(PWin32SocketTcp* self, U8* pntr, Int start, Int stop);

Int pWin32SocketTcpRead(PWin32SocketTcp* self, U8* pntr, Int start, Int stop);

PHostIp pWin32SocketTcpGetHost(PWin32SocketTcp* self);

#endif // P_SYSTEM_WIN32_NETWORK_SOCKET_TCP_H
