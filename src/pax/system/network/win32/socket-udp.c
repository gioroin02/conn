#ifndef P_SYSTEM_WIN32_NETWORK_SOCKET_UDP_C
#define P_SYSTEM_WIN32_NETWORK_SOCKET_UDP_C

#include "socket-udp.h"

PWin32SocketUdp* pWin32SocketUdpReserve(PMemoryArena* arena)
{
    return pMemoryArenaReserveOneOf(arena, PWin32SocketUdp);
}

Bool pWin32SocketUdpCreate(PWin32SocketUdp* self, PHostIp host)
{
    pMemorySet(self, sizeof *self, 0xAB);

    PWin32AddrStorage storage;
    Int               length = 0;

    storage = pWin32AddrStorageMake(host.address, host.port, &length);

    if (pWin32NetworkStart() == 0 || length == 0) return 0;

    SOCKET handle = WSASocketW(storage.ss_family,
        SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_OVERLAPPED);

    if (handle != INVALID_SOCKET) {
        self->handle  = handle;
        self->storage = storage;

        return 1;
    }

    pWin32NetworkStop();

    return 0;
}

void pWin32SocketUdpDestroy(PWin32SocketUdp* self)
{
    if (self->handle != INVALID_SOCKET)
        closesocket(self->handle);

    pMemorySet(self, sizeof *self, 0xAB);

    pWin32NetworkStop();
}

Bool pWin32SocketUdpBind(PWin32SocketUdp* self)
{
    PWin32Addr* sockaddr = (PWin32Addr*) &self->storage;
    Int         length   = pWin32AddrStorageGetSize(&self->storage);

    if (length == 0) return 0;

    if (bind(self->handle, sockaddr, length) == SOCKET_ERROR)
        return 0;

    return 1;
}

Bool pWin32SocketUdpBindAs(PWin32SocketUdp* self, PHostIp host)
{
    PWin32AddrStorage storage;
    PWin32Addr*       sockaddr = (PWin32Addr*) &storage;
    Int               length   = 0;

    storage = pWin32AddrStorageMake(host.address, host.port, &length);

    if (storage.ss_family != self->storage.ss_family) return 0;

    if (bind(self->handle, sockaddr, length) == SOCKET_ERROR)
        return 0;

    self->storage = storage;

    return 1;
}

Int pWin32SocketUdpWrite(PWin32SocketUdp* self, U8* pntr, Int start, Int stop, PHostIp host)
{
    if (pntr == NULL || stop <= start || start < 0) return 0;

    PWin32AddrStorage storage;
    PWin32Addr*       sockaddr = (PWin32Addr*) &storage;
    Int               length   = 0;

    storage = pWin32AddrStorageMake(host.address, host.port, &length);

    I8* memory = ((I8*) pntr + start);
    Int size   = stop - start;
    Int result = 0;

    while (result < size) {
        Int count = sendto(self->handle, memory + result,
            size - result, 0, sockaddr, length);

        if (count > 0 && count <= size - result)
            result += count;
        else
            break;
    }

    return result;
}

Int pWin32SocketUdpRead(PWin32SocketUdp* self, U8* pntr, Int start, Int stop, PHostIp* host)
{
    if (pntr == NULL || stop <= start || start < 0) return 0;

    PWin32AddrStorage storage;
    PWin32Addr*       sockaddr = (PWin32Addr*) &storage;
    int               length   = sizeof storage;

    pMemorySet(&storage, sizeof storage, 0xAB);

    I8* memory = ((I8*) pntr + start);
    Int size   = stop - start;
    Int count  = recvfrom(self->handle, memory, size, 0, sockaddr, &length);

    if (count > 0 && count <= size && length > 0) {
        if (host != NULL) {
            *host = pHostIpMake(
                pWin32AddrStorageGetAddress(&storage),
                pWin32AddrStorageGetPort(&storage));
        }

        return count;
    }

    return 0;
}

PHostIp pWin32SocketUdpGetHost(PWin32SocketUdp* self)
{
    return pHostIpMake(
        pWin32AddrStorageGetAddress(&self->storage),
        pWin32AddrStorageGetPort(&self->storage));
}

#endif // P_SYSTEM_WIN32_NETWORK_SOCKET_UDP_C
