#ifndef RN_WIN32_NETWORK_SOCKET_TCP_H
#define RN_WIN32_NETWORK_SOCKET_TCP_H

#include "./address.h"

typedef struct RnWin32SocketTCP
{
    SOCKET            handle;
    RnSockAddrStorage storage;
}
RnWin32SocketTCP;

RnWin32SocketTCP*
rnWin32SocketTCPReserve(RnMemoryArena* arena);

b32
rnWin32SocketTCPCreate(RnWin32SocketTCP* self, RnAddressIP address, u16 port);

b32
rnWin32SocketTCPAccept(RnWin32SocketTCP* self, RnWin32SocketTCP* value);

void
rnWin32SocketTCPDestroy(RnWin32SocketTCP* self);

b32
rnWin32SocketTCPBind(RnWin32SocketTCP* self);

b32
rnWin32SocketTCPBindTo(RnWin32SocketTCP* self, RnAddressIP address, u16 port);

b32
rnWin32SocketTCPListen(RnWin32SocketTCP* self);

b32
rnWin32SocketTCPConnect(RnWin32SocketTCP* self, RnAddressIP address, u16 port);

ssize
rnWin32SocketTCPWrite(RnWin32SocketTCP* self, u8* values, ssize size);

ssize
rnWin32SocketTCPRead(RnWin32SocketTCP* self, u8* values, ssize size);

#endif // RN_WIN32_NETWORK_SOCKET_TCP_H
