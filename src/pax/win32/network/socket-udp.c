#ifndef PX_WIN32_NETWORK_SOCKET_UDP_C
#define PX_WIN32_NETWORK_SOCKET_UDP_C

#include "socket-udp.h"

PxWin32SocketUdp*
pxWin32SocketUdpReserve(PxMemoryArena* arena)
{
    return pxMemoryArenaReserveOneOf(arena, PxWin32SocketUdp);
}

b32
pxWin32SocketUdpCreate(PxWin32SocketUdp* self, PxAddressIp address, u16 port)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    ssize length = 0;

    PxWin32SockAddrStorage storage =
        pxWin32SockAddrStorageMake(address, port, &length);

    if (pxWin32NetworkStart() == 0 || length == 0) return 0;

    SOCKET handle = WSASocketA(storage.ss_family, SOCK_DGRAM,
        IPPROTO_UDP, 0, 0, WSA_FLAG_OVERLAPPED);

    if (handle == INVALID_SOCKET) return 0;

    self->handle  = handle;
    self->storage = storage;

    return 1;
}

void
pxWin32SocketUdpDestroy(PxWin32SocketUdp* self)
{
    if (self->handle != INVALID_SOCKET)
        closesocket(self->handle);

    pxMemorySet(self, sizeof *self, 0xAB);

    pxWin32NetworkStop();
}

b32
pxWin32SocketUdpBind(PxWin32SocketUdp* self)
{
    PxWin32SockAddr* sockaddr = (PxWin32SockAddr*) &self->storage;

    ssize length = 0;

    switch (self->storage.ss_family) {
        case AF_INET:  length = sizeof (PxWin32SockAddrIn4); break;
        case AF_INET6: length = sizeof (PxWin32SockAddrIn6); break;

        default: break;
    }

    if (length == 0) return 0;

    if (bind(self->handle, sockaddr, length) == SOCKET_ERROR)
        return 0;

    return 1;
}

b32
pxWin32SocketUdpBindTo(PxWin32SocketUdp* self, PxAddressIp address, u16 port)
{
    ssize length = 0;

    PxWin32SockAddrStorage storage =
        pxWin32SockAddrStorageMake(address, port, &length);

    PxWin32SockAddr* sockaddr = (PxWin32SockAddr*) &storage;

    if (storage.ss_family != self->storage.ss_family) return 0;

    if (bind(self->handle, sockaddr, length) == SOCKET_ERROR)
        return 0;

    self->storage = storage;

    return 1;
}

ssize
pxWin32SocketUdpWrite(PxWin32SocketUdp* self, u8* values, ssize size, PxAddressIp address, u16 port)
{
    ssize length = 0;
    ssize count  = 0;
    ssize temp   = 0;

    PxWin32SockAddrStorage storage =
        pxWin32SockAddrStorageMake(address, port, &length);

    PxWin32SockAddr* sockaddr = (PxWin32SockAddr*) &storage;

    if (values == PX_NULL || size <= 0 || length == 0) return 0;

    for (temp = 0; count < size; count += temp) {
        temp = sendto(self->handle, ((char*) values + count),
            ((int) size - count), 0, sockaddr, length);

        if (temp <= 0 || temp > size - count) break;
    }

    return count;
}

ssize
pxWin32SocketUdpRead(PxWin32SocketUdp* self, u8* values, ssize size, PxAddressIp* address, u16* port)
{
    PxWin32SockAddrStorage storage;

    pxMemorySet(&storage, sizeof storage, 0xAB);

    PxWin32SockAddr* sockaddr = (PxWin32SockAddr*) &storage;
    int              length   = sizeof (PxWin32SockAddrStorage);
    ssize            count    = 0;

    if (values == PX_NULL || size <= 0) return 0;

    count = recvfrom(self->handle, ((char*) values),
        ((int) size), 0, sockaddr, &length);

    if (count <= 0 || count > size || length == 0) return 0;

    if (address != PX_NULL)
        *address = pxWin32SockAddrStorageGetAddress(&storage);

    if (port != PX_NULL) *port = pxWin32SockAddrStorageGetPort(&storage);

    return count;
}

PxAddressIp
pxWin32SocketUdpGetAddress(PxWin32SocketUdp* self)
{
    return pxWin32SockAddrStorageGetAddress(&self->storage);
}

u16
pxWin32SocketUdpGetPort(PxWin32SocketUdp* self)
{
    return pxWin32SockAddrStorageGetPort(&self->storage);
}

#endif // PX_WIN32_NETWORK_SOCKET_UDP_C
