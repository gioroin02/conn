#ifndef PX_WIN32_NETWORK_SOCKET_TCP_C
#define PX_WIN32_NETWORK_SOCKET_TCP_C

#include "socket-tcp.h"

PxWin32SocketTcp*
pxWin32SocketTcpReserve(PxMemoryArena* arena)
{
    return pxMemoryArenaReserveOneOf(arena, PxWin32SocketTcp);
}

b32
pxWin32SocketTcpCreate(PxWin32SocketTcp* self, PxAddressIp address, u16 port)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    ssize length = 0;

    PxWin32SockAddrStorage storage =
        pxWin32SockAddrStorageMake(address, port, &length);

    if (pxWin32NetworkStart() == 0 || length == 0) return 0;

    SOCKET handle = WSASocketA(storage.ss_family, SOCK_STREAM,
        IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

    if (handle == INVALID_SOCKET) return 0;

    self->handle  = handle;
    self->storage = storage;

    return 1;
}

b32
pxWin32SocketTcpAccept(PxWin32SocketTcp* self, PxWin32SocketTcp* value)
{
    pxMemorySet(value, sizeof *value, 0xAB);

    PxWin32SockAddrStorage storage;

    pxMemorySet(&storage, sizeof storage, 0xAB);

    int length = sizeof (PxWin32SockAddrStorage);

    if (pxWin32NetworkStart() == 0) return 0;

    SOCKET handle = accept(self->handle,
        (PxWin32SockAddr*) &storage, &length);

    if (handle == INVALID_SOCKET) return 0;

    value->handle  = handle;
    value->storage = storage;

    return 1;
}

void
pxWin32SocketTcpDestroy(PxWin32SocketTcp* self)
{
    if (self->handle != INVALID_SOCKET)
        closesocket(self->handle);

    pxMemorySet(self, sizeof *self, 0xAB);

    pxWin32NetworkStop();
}

b32
pxWin32SocketTcpBind(PxWin32SocketTcp* self)
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
pxWin32SocketTcpBindTo(PxWin32SocketTcp* self, PxAddressIp address, u16 port)
{
    PxWin32SockAddr* sockaddr = (PxWin32SockAddr*) &self->storage;
    ssize            length   = 0;

    PxWin32SockAddrStorage storage =
        pxWin32SockAddrStorageMake(address, port, &length);

    if (storage.ss_family != self->storage.ss_family) return 0;

    if (bind(self->handle, sockaddr, length) == SOCKET_ERROR)
        return 0;

    self->storage = storage;

    return 1;
}

b32
pxWin32SocketTcpListen(PxWin32SocketTcp* self)
{
    if (listen(self->handle, SOMAXCONN) == SOCKET_ERROR)
        return 0;

    return 1;
}

b32
pxWin32SocketTcpConnect(PxWin32SocketTcp* self, PxAddressIp address, u16 port)
{
    ssize length = 0;

    PxWin32SockAddrStorage storage =
        pxWin32SockAddrStorageMake(address, port, &length);

    PxWin32SockAddr* sockaddr = (PxWin32SockAddr*) &storage;

    if (port == 0 || length == 0) return 0;

    if (connect(self->handle, sockaddr, length) == SOCKET_ERROR)
        return 0;

    return 1;
}

ssize
pxWin32SocketTcpWrite(PxWin32SocketTcp* self, u8* values, ssize start, ssize stop)
{
    if (values == PX_NULL || stop <= start || start < 0) return 0;

    char* pntr   = ((char*) values + start);
    ssize size   = stop - start;
    ssize result = 0;

    while (result < size) {
        DWORD count = send(self->handle, pntr + result,
            ((int) size - result), 0);

        if (count > 0 && count <= size - result)
            result += count;
        else
            break;
    }

    return result;
}

ssize
pxWin32SocketTcpRead(PxWin32SocketTcp* self, u8* values, ssize start, ssize stop)
{
    if (values == PX_NULL || stop <= start || start < 0) return 0;

    char* pntr  = ((char*) values + start);
    ssize size  = stop - start;
    ssize count = recv(self->handle, pntr, (int) size, 0);

    if (count > 0 && count <= size) return count;

    return 0;
}

PxAddressIp
pxWin32SocketTcpGetAddress(PxWin32SocketTcp* self)
{
    return pxWin32SockAddrStorageGetAddress(&self->storage);
}

u16
pxWin32SocketTcpGetPort(PxWin32SocketTcp* self)
{
    return pxWin32SockAddrStorageGetPort(&self->storage);
}

#endif // PX_WIN32_NETWORK_SOCKET_TCP_C
