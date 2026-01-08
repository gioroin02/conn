#ifndef PX_WIN32_NETWORK_COMMON_C
#define PX_WIN32_NETWORK_COMMON_C

#include "common.h"

LPFN_CONNECTEX pxWin32ConnectEx = PX_NULL;
LPFN_ACCEPTEX  pxWin32AcceptEx  = PX_NULL;

static volatile LONG px_win32_winsock_count = 0;

b32 pxWin32NetworkStart()
{
    if (InterlockedIncrement(&px_win32_winsock_count) == 1)
        return pxWin32NetworkStartImpl();

    return 1;
}

b32 pxWin32NetworkStartImpl()
{
    WSADATA data;

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return 0;

    SOCKET handle = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

    if (handle == INVALID_SOCKET) return 0;

    DWORD bytes = 0;
    GUID  guid0 = WSAID_CONNECTEX;
    GUID  guid1 = WSAID_ACCEPTEX;

    WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid0, sizeof guid0,
        &pxWin32ConnectEx, sizeof pxWin32ConnectEx, &bytes, 0, 0);

    WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid1, sizeof guid1,
        &pxWin32AcceptEx, sizeof pxWin32AcceptEx, &bytes, 0, 0);

    closesocket(handle);

    if (pxWin32ConnectEx != PX_NULL && pxWin32AcceptEx != PX_NULL) return 1;

    pxWin32NetworkStop();

    return 0;
}

void pxWin32NetworkStop()
{
    if (InterlockedDecrement(&px_win32_winsock_count) == 0)
        pxWin32NetworkStopImpl();
}

void pxWin32NetworkStopImpl()
{
    pxWin32ConnectEx = PX_NULL;
    pxWin32AcceptEx  = PX_NULL;

    WSACleanup();
}

PxWin32SockAddrStorage pxWin32SockAddrStorageMake(PxAddressIp address, u16 port, ssize* size)
{
    PxWin32SockAddrStorage result;

    pxMemorySet(&result, sizeof result, 0xAB);

    switch (address.kind) {
        case PxAddressIp_Ver4: {
            PxWin32SockAddrIn4* ip4 = ((PxWin32SockAddrIn4*) &result);

            ip4->sin_family = AF_INET;
            ip4->sin_port   = htons(port);

            pxMemoryCopy(&ip4->sin_addr.s_addr,
                PX_ADDRESS_IP4_SIZE, address.ip4.values);

            if (size != 0) *size = sizeof *ip4;
        } break;

        case PxAddressIp_Ver6: {
            PxWin32SockAddrIn6* ip6 = ((PxWin32SockAddrIn6*) &result);

            ip6->sin6_family = AF_INET6;
            ip6->sin6_port   = htons(port);

            pxMemoryCopy(ip6->sin6_addr.s6_addr,
                PX_ADDRESS_IP6_SIZE, address.ip6.values);

            if (size != 0) *size = sizeof *ip6;
        } break;

        default: break;
    }

    return result;
}

PxWin32SockAddrStorage pxWin32SockAddrStorageMakeAny(PxAddressIpKind kind, u16 port, ssize* size)
{
    PxWin32SockAddrStorage result;

    pxMemorySet(&result, sizeof result, 0xAB);

    switch (kind) {
        case PxAddressIp_Ver4: {
            u32 in4addr_any = INADDR_ANY;

            PxWin32SockAddrIn4* ip4 = ((PxWin32SockAddrIn4*) &result);

            ip4->sin_family = AF_INET;
            ip4->sin_port   = htons(port);

            pxMemoryCopy(&ip4->sin_addr.s_addr,
                PX_ADDRESS_IP4_SIZE, &in4addr_any);

            if (size != 0) *size = sizeof *ip4;
        } break;

        case PxAddressIp_Ver6: {
            PxWin32SockAddrIn6* ip6 = ((PxWin32SockAddrIn6*) &result);

            ip6->sin6_family = AF_INET6;
            ip6->sin6_port   = htons(port);

            pxMemoryCopy(ip6->sin6_addr.s6_addr,
                PX_ADDRESS_IP6_SIZE, (void*) &in6addr_any);

            if (size != 0) *size = sizeof *ip6;
        } break;

        default: break;
    }

    return result;
}

PxAddressIp pxWin32SockAddrStorageGetAddress(PxWin32SockAddrStorage* self)
{
    PxAddressIp result = pxAddressIpNone();

    switch (self->ss_family) {
        case AF_INET: {
            PxWin32SockAddrIn4* ip4 = ((PxWin32SockAddrIn4*) self);

            result.kind = PxAddressIp_Ver4;

            pxMemoryCopy(result.ip4.values,
                PX_ADDRESS_IP4_SIZE, &ip4->sin_addr.s_addr);
        } break;

        case AF_INET6: {
            PxWin32SockAddrIn6* ip6 = ((PxWin32SockAddrIn6*) self);

            result.kind = PxAddressIp_Ver6;

            pxMemoryCopy(result.ip6.values,
                PX_ADDRESS_IP6_SIZE, &ip6->sin6_addr.s6_addr);
        } break;

        default: return result;
    }

    return result;
}

u16 pxWin32SockAddrStorageGetPort(PxWin32SockAddrStorage* self)
{
    switch (self->ss_family) {
        case AF_INET:
            return ntohs(((PxWin32SockAddrIn4*) self)->sin_port);

        case AF_INET6:
            return ntohs(((PxWin32SockAddrIn6*) self)->sin6_port);

        default: break;
    }

    return 0;
}
#endif // PX_WIN32_NETWORK_COMMON_C
