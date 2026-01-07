#ifndef PX_WIN32_NETWORK_ADDRESS_C
#define PX_WIN32_NETWORK_ADDRESS_C

#include "address.h"

PxWin32SockAddrStorage
pxWin32SockAddrStorageMake(PxAddressIp address, u16 port, ssize* size)
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

PxWin32SockAddrStorage
pxWin32SockAddrStorageMakeAny(PxAddressIpKind kind, u16 port, ssize* size)
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

PxAddressIp
pxWin32SockAddrStorageGetAddress(PxWin32SockAddrStorage* self)
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

u16
pxWin32SockAddrStorageGetPort(PxWin32SockAddrStorage* self)
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

#endif // PX_WIN32_NETWORK_ADDRESS_C
