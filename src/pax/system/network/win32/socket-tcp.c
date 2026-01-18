#ifndef P_SYSTEM_WIN32_NETWORK_SOCKET_TCP_C
#define P_SYSTEM_WIN32_NETWORK_SOCKET_TCP_C

#include "socket-tcp.h"

PWin32SocketTcp* pWin32SocketTcpReserve(PMemoryArena* arena)
{
    return pMemoryArenaReserveOneOf(arena, PWin32SocketTcp);
}

Bool pWin32SocketTcpCreate(PWin32SocketTcp* self, PHostIp host)
{
    pMemorySet(self, sizeof *self, 0xAB);

    PWin32AddrStorage storage;
    Int               length = 0;

    storage = pWin32AddrStorageMake(host.address, host.port, &length);

    if (pWin32NetworkStart() == 0 || length == 0) return 0;

    SOCKET handle = WSASocketW(storage.ss_family,
        SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

    if (handle != INVALID_SOCKET) {
        self->handle  = handle;
        self->storage = storage;

        return 1;
    }

    pWin32NetworkStop();

    return 0;
}

Bool pWin32SocketTcpAccept(PWin32SocketTcp* self, PWin32SocketTcp* value)
{
    pMemorySet(value, sizeof *value, 0xAB);

    PWin32AddrStorage storage;
    PWin32Addr*       sockaddr = (PWin32Addr*) &storage;
    int               length   = sizeof storage;

    pMemorySet(&storage, sizeof storage, 0xAB);

    if (pWin32NetworkStart() == 0) return 0;

    SOCKET handle = accept(self->handle, sockaddr, &length);

    if (handle != INVALID_SOCKET) {
        value->handle  = handle;
        value->storage = storage;

        return 1;
    }

    pWin32NetworkStop();

    return 0;
}

void pWin32SocketTcpDestroy(PWin32SocketTcp* self)
{
    if (self->handle != INVALID_SOCKET)
        closesocket(self->handle);

    pMemorySet(self, sizeof *self, 0xAB);

    pWin32NetworkStop();
}

Bool pWin32SocketTcpBind(PWin32SocketTcp* self)
{
    PWin32Addr* sockaddr = (PWin32Addr*) &self->storage;
    Int         length   = pWin32AddrStorageGetSize(&self->storage);

    if (length == 0) return 0;

    if (bind(self->handle, sockaddr, length) == SOCKET_ERROR)
        return 0;

    return 1;
}

Bool pWin32SocketTcpBindAs(PWin32SocketTcp* self, PHostIp host)
{
    PWin32AddrStorage storage;
    PWin32Addr*       sockaddr = (PWin32Addr*) &self->storage;
    Int               length   = 0;

    storage = pWin32AddrStorageMake(host.address, host.port, &length);

    if (storage.ss_family != self->storage.ss_family) return 0;

    if (bind(self->handle, sockaddr, length) == SOCKET_ERROR)
        return 0;

    self->storage = storage;

    return 1;
}

Bool pWin32SocketTcpListen(PWin32SocketTcp* self)
{
    if (listen(self->handle, SOMAXCONN) == SOCKET_ERROR)
        return 0;

    return 1;
}

Bool pWin32SocketTcpConnect(PWin32SocketTcp* self, PHostIp host)
{
    PWin32AddrStorage storage;
    PWin32Addr*       sockaddr = (PWin32Addr*) &storage;
    Int               length   = 0;

    storage = pWin32AddrStorageMake(host.address, host.port, &length);

    if (host.port == 0 || length == 0) return 0;

    if (connect(self->handle, sockaddr, length) == SOCKET_ERROR)
        return 0;

    return 1;
}

Int pWin32SocketTcpWrite(PWin32SocketTcp* self, U8* pntr, Int start, Int stop)
{
    if (pntr == NULL || stop <= start || start < 0) return 0;

    I8* memory = ((I8*) pntr + start);
    Int size   = stop - start;
    Int result = 0;

    while (result < size) {
        Int count = send(self->handle,
            memory + result, size - result, 0);

        if (count > 0 && count <= size - result)
            result += count;
        else
            break;
    }

    return result;
}

Int pWin32SocketTcpRead(PWin32SocketTcp* self, U8* pntr, Int start, Int stop)
{
    if (pntr == NULL || stop <= start || start < 0) return 0;

    I8* memory = ((I8*) pntr + start);
    Int size   = stop - start;
    Int count  = recv(self->handle, memory, size, 0);

    if (count > 0 && count <= size) return count;

    return 0;
}

PHostIp pWin32SocketTcpGetHost(PWin32SocketTcp* self)
{
    return pHostIpMake(
        pWin32AddrStorageGetAddress(&self->storage),
        pWin32AddrStorageGetPort(&self->storage));
}

#endif // P_SYSTEM_WIN32_NETWORK_SOCKET_TCP_C
