#ifndef RN_WIN32_NETWORK_ADDRESS_C
#define RN_WIN32_NETWORK_ADDRESS_C

#include "./address.h"

RnSockAddrStorage
rnSockAddrStorageMake(RnAddressIP address, u16 port, ssize* size)
{
    RnSockAddrStorage result = {0};

    switch (address.kind) {
        case RnAddressIP_IPv4: {
            RnSockAddrIn4* ipv4 = ((RnSockAddrIn4*) &result);

            ipv4->sin_family = AF_INET;
            ipv4->sin_port   = htons(port);

            for (ssize i = 0; i < RN_ADDRESS_IPV4_SIZE; i += 1)
                ((u8*) &ipv4->sin_addr.s_addr)[i] = ((u8*) address.ipv4.values)[i];

            if (size != 0) *size = sizeof(RnSockAddrIn4);
        } break;

        case RnAddressIP_IPv6: {
            RnSockAddrIn6* ipv6 = ((RnSockAddrIn6*) &result);

            ipv6->sin6_family = AF_INET6;
            ipv6->sin6_port   = htons(port);

            for (ssize i = 0; i < RN_ADDRESS_IPV6_SIZE; i += 1)
                ((u16*) &ipv6->sin6_addr.s6_addr)[i] = ((u16*) address.ipv6.values)[i];

            if (size != 0) *size = sizeof(RnSockAddrIn6);
        } break;

        default: break;
    }

    return result;
}

RnSockAddrStorage
rnSockAddrStorageMakeAny(RnAddressIPKind kind, u16 port, ssize* size)
{
    RnSockAddrStorage result = {0};

    switch (kind) {
        case RnAddressIP_IPv4: {
            u32 in4addr_any = INADDR_ANY;

            RnSockAddrIn4* ipv4 = ((RnSockAddrIn4*) &result);

            ipv4->sin_family = AF_INET;
            ipv4->sin_port   = htons(port);

            for (ssize i = 0; i < RN_ADDRESS_IPV4_SIZE; i += 1)
                ((u8*) &ipv4->sin_addr.s_addr)[i] = ((u8*) &in4addr_any)[i];

            if (size != 0) *size = sizeof(RnSockAddrIn4);
        } break;

        case RnAddressIP_IPv6: {
            RnSockAddrIn6* ipv6 = ((RnSockAddrIn6*) &result);

            ipv6->sin6_family = AF_INET6;
            ipv6->sin6_port   = htons(port);

            for (ssize i = 0; i < RN_ADDRESS_IPV6_SIZE; i += 1)
                ((u16*) &ipv6->sin6_addr.s6_addr)[i] = ((u16*) &in6addr_any)[i];

            if (size != 0) *size = sizeof(RnSockAddrIn6);
        } break;

        default: break;
    }

    return result;
}

RnAddressIP
rnSockAddrStorageGetAddress(RnSockAddrStorage* self)
{
    RnAddressIP result = {0};

    switch (self->ss_family) {
        case AF_INET: {
            result.kind = RnAddressIP_IPv4;

            RnSockAddrIn4* ipv4 = ((RnSockAddrIn4*) self);

            for (ssize i = 0; i < RN_ADDRESS_IPV4_SIZE; i += 1)
                ((u8*) result.ipv4.values)[i] = ((u8*) &ipv4->sin_addr.s_addr)[i];
        } break;

        case AF_INET6: {
            result.kind = RnAddressIP_IPv6;

            RnSockAddrIn6* ipv6 = ((RnSockAddrIn6*) self);

            for (ssize i = 0; i < RN_ADDRESS_IPV6_SIZE; i += 1)
                ((u16*) result.ipv6.values)[i] = ((u16*) &ipv6->sin6_addr.s6_addr)[i];
        } break;

        default: return result;
    }

    return result;
}

u16
rnSockAddrStorageGetPort(RnSockAddrStorage* self)
{
    switch (self->ss_family) {
        case AF_INET:
            return ntohs(((RnSockAddrIn4*) self)->sin_port);

        case AF_INET6:
            return ntohs(((RnSockAddrIn6*) self)->sin6_port);

        default: break;
    }

    return 0;
}

#endif // RN_WIN32_NETWORK_ADDRESS_C
