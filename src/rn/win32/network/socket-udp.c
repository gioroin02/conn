#ifndef RN_WIN32_NETWORK_SOCKET_UDP_C
#define RN_WIN32_NETWORK_SOCKET_UDP_C

#include "./socket-udp.h"

RnWin32SocketUDP*
rnWin32SocketUDPReserve(RnMemoryArena* arena)
{
    return rnMemoryArenaReserveOneOf(arena, RnWin32SocketUDP);
}

b32
rnWin32SocketUDPCreate(RnWin32SocketUDP* self, RnAddressIP address, u16 port)
{
    RnSockAddrStorage storage = {0};
    ssize             length  = 0;

    if (self == 0 || address.kind == RnAddressIP_None)
        return 0;

    if (rnWin32NetworkStart() == 0) return 0;

    storage = rnSockAddrStorageMake(address, port, &length);

    SOCKET handle = WSASocketA(storage.ss_family,
        SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_OVERLAPPED);

    if (handle == INVALID_SOCKET) return 0;

    self->handle  = handle;
    self->storage = storage;

    return 1;
}

void
rnWin32SocketUDPDestroy(RnWin32SocketUDP* self)
{
    if (self == 0) return;

    if (self->handle != INVALID_SOCKET)
        closesocket(self->handle);

    *self = (RnWin32SocketUDP) {0};

    rnWin32NetworkStop();
}

b32
rnWin32SocketUDPBind(RnWin32SocketUDP* self)
{
    ssize length = 0;

    if (self == 0) return 0;

    switch (self->storage.ss_family) {
        case AF_INET:  length = sizeof(RnSockAddrIn4); break;
        case AF_INET6: length = sizeof(RnSockAddrIn6); break;

        default: break;
    }

    if (length == 0) return 0;

    int status = bind(self->handle,
        ((RnSockAddr*) &self->storage), length);

    if (status == SOCKET_ERROR) return 0;

    return 1;
}

b32
rnWin32SocketUDPBindTo(RnWin32SocketUDP* self, RnAddressIP address, u16 port)
{
    RnSockAddrStorage storage = {0};
    ssize             length  = 0;

    if (self == 0) return 0;

    storage = rnSockAddrStorageMake(address, port, &length);

    if (storage.ss_family != self->storage.ss_family)
        return 0;

    int status = bind(self->handle,
        ((RnSockAddr*) &storage), length);

    if (status == SOCKET_ERROR) return 0;

    self->storage = storage;

    return 1;
}

ssize
rnWin32SocketUDPWrite(RnWin32SocketUDP* self, u8* values, ssize size, RnAddressIP address, u16 port)
{
    if (self == 0 || values == 0 || size <= 0) return 0;

    ssize length = 0;
    ssize count  = 0;

    RnSockAddrStorage storage = rnSockAddrStorageMake(address, port, &length);

    for (ssize temp = 0; count < size; count += temp) {
        temp = sendto(self->handle, ((char*) values + count),
            ((int) size - count), 0, ((RnSockAddr*) &storage), length);

        if (temp <= 0 || temp > size - count) break;
    }

    return count;
}

ssize
rnWin32SocketUDPRead(RnWin32SocketUDP* self, u8* values, ssize size, RnAddressIP* address, u16* port)
{
    if (self == 0 || values == 0 || size <= 0) return 0;

    int   length = sizeof(RnSockAddrStorage);
    ssize count  = 0;

    RnSockAddrStorage storage = {0};

    count = recvfrom(self->handle, ((char*) values),
        ((int) size), 0, ((RnSockAddr*) &storage), &length);

    if (count <= 0 || count > size) return 0;

    if (address != 0)
        *address = rnSockAddrStorageGetAddress(&storage);

    if (port != 0)
        *port = rnSockAddrStorageGetPort(&storage);

    return count;
}

#endif // RN_WIN32_NETWORK_SOCKET_UDP_C
