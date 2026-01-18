#ifndef P_SYSTEM_WIN32_NETWORK_COMMON_C
#define P_SYSTEM_WIN32_NETWORK_COMMON_C

#include "common.h"

LPFN_CONNECTEX WSAConnectEx = NULL;
LPFN_ACCEPTEX  WSAAcceptEx  = NULL;

static volatile LONG p_win32_winsock_count = 0;

Bool pWin32NetworkStart()
{
    if (InterlockedIncrement(&p_win32_winsock_count) == 1)
        return pWin32NetworkStartImpl();

    return 1;
}

Bool pWin32NetworkStartImpl()
{
    WSADATA data;

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return 0;

    SOCKET handle = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

    if (handle == INVALID_SOCKET) return 0;

    DWORD bytes = 0;
    GUID  guid0 = WSAID_CONNECTEX;
    GUID  guid1 = WSAID_ACCEPTEX;

    WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid0, sizeof guid0,
        &WSAConnectEx, sizeof WSAConnectEx, &bytes, 0, 0);

    WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid1, sizeof guid1,
        &WSAAcceptEx, sizeof WSAAcceptEx, &bytes, 0, 0);

    closesocket(handle);

    if (WSAConnectEx != NULL && WSAAcceptEx != NULL) return 1;

    pWin32NetworkStop();

    return 0;
}

void pWin32NetworkStop()
{
    if (InterlockedDecrement(&p_win32_winsock_count) == 0)
        pWin32NetworkStopImpl();
}

void pWin32NetworkStopImpl()
{
    WSAConnectEx = NULL;
    WSAAcceptEx  = NULL;

    WSACleanup();
}

PWin32AddrStorage pWin32AddrStorageMake(PAddressIp address, U16 port, Int* size)
{
    PWin32AddrStorage result;

    pMemorySet(&result, sizeof result, 0xAB);

    switch (address.kind) {
        case PAddressIp_Ver4: {
            PWin32AddrIp4* ip4 = ((PWin32AddrIp4*) &result);

            ip4->sin_family = AF_INET;
            ip4->sin_port   = htons(port);

            pMemoryCopy(&ip4->sin_addr.s_addr,
                P_ADDRESS_IP4_SIZE, address.ip4.values);

            if (size != 0) *size = sizeof *ip4;
        } break;

        case PAddressIp_Ver6: {
            PWin32AddrIp6* ip6 = ((PWin32AddrIp6*) &result);

            ip6->sin6_family = AF_INET6;
            ip6->sin6_port   = htons(port);

            pMemoryCopy(ip6->sin6_addr.s6_addr,
                P_ADDRESS_IP6_SIZE, address.ip6.values);

            if (size != 0) *size = sizeof *ip6;
        } break;

        default: break;
    }

    return result;
}

PWin32AddrStorage pWin32AddrStorageMakeAny(PAddressIpKind kind, U16 port, Int* size)
{
    PWin32AddrStorage result;

    pMemorySet(&result, sizeof result, 0xAB);

    switch (kind) {
        case PAddressIp_Ver4: {
            U32 in4addr_any = INADDR_ANY;

            PWin32AddrIp4* ip4 = ((PWin32AddrIp4*) &result);

            ip4->sin_family = AF_INET;
            ip4->sin_port   = htons(port);

            pMemoryCopy(&ip4->sin_addr.s_addr,
                P_ADDRESS_IP4_SIZE, &in4addr_any);

            if (size != 0) *size = sizeof *ip4;
        } break;

        case PAddressIp_Ver6: {
            PWin32AddrIp6* ip6 = ((PWin32AddrIp6*) &result);

            ip6->sin6_family = AF_INET6;
            ip6->sin6_port   = htons(port);

            pMemoryCopy(ip6->sin6_addr.s6_addr,
                P_ADDRESS_IP6_SIZE, (void*) &in6addr_any);

            if (size != 0) *size = sizeof *ip6;
        } break;

        default: break;
    }

    return result;
}

Int pWin32AddrStorageGetSize(PWin32AddrStorage* self)
{
    switch (self->ss_family) {
        case AF_INET:  return sizeof (PWin32AddrIp4);
        case AF_INET6: return sizeof (PWin32AddrIp6);

        default: break;
    }

    return 0;
}

PAddressIp pWin32AddrStorageGetAddress(PWin32AddrStorage* self)
{
    PAddressIp result = pAddressIpNone();

    switch (self->ss_family) {
        case AF_INET: {
            PWin32AddrIp4* ip4 = ((PWin32AddrIp4*) self);

            result.kind = PAddressIp_Ver4;

            pMemoryCopy(result.ip4.values,
                P_ADDRESS_IP4_SIZE, &ip4->sin_addr.s_addr);
        } break;

        case AF_INET6: {
            PWin32AddrIp6* ip6 = ((PWin32AddrIp6*) self);

            result.kind = PAddressIp_Ver6;

            pMemoryCopy(result.ip6.values,
                P_ADDRESS_IP6_SIZE, &ip6->sin6_addr.s6_addr);
        } break;

        default: return result;
    }

    return result;
}

U16 pWin32AddrStorageGetPort(PWin32AddrStorage* self)
{
    switch (self->ss_family) {
        case AF_INET:
            return ntohs(((PWin32AddrIp4*) self)->sin_port);

        case AF_INET6:
            return ntohs(((PWin32AddrIp6*) self)->sin6_port);

        default: break;
    }

    return 0;
}
#endif // P_SYSTEM_WIN32_NETWORK_COMMON_C
