#ifndef RN_SYSTEM_NETWORK_ADDRESS_IP_H
#define RN_SYSTEM_NETWORK_ADDRESS_IP_H

#include "./import.h"

#define RN_ADDRESS_IPV4_SIZE ((ssize) 4)

#define rnAddressIPv4Empty() \
    ((RnAddressIP) {.kind = RnAddressIP_IPv4, .ipv4.values = {0}})

#define rnAddressIPv4Local() \
    ((RnAddressIP) {.kind = RnAddressIP_IPv4, .ipv4.values = {[0] = 127, [3] = 1}})

#define RN_ADDRESS_IPV6_SIZE ((ssize) 8)

#define rnAddressIPv6Empty() \
    ((RnAddressIP) {.kind = RnAddressIP_IPv6, .ipv6.values = {0}})

#define rnAddressIPv6Local() \
    ((RnAddressIP) {.kind = RnAddressIP_IPv6, .ipv6.values = {[7] = 1}})

typedef enum RnAddressIPKind
{
    RnAddressIP_None,
    RnAddressIP_IPv4,
    RnAddressIP_IPv6,
}
RnAddressIPKind;

typedef union RnAddressIPv4
{
    u8 values[RN_ADDRESS_IPV4_SIZE];

    struct
    {
        u8 v0, v1;
        u8 v2, v3;
    };
}
RnAddressIPv4;

typedef union RnAddressIPv6
{
    u8 values[RN_ADDRESS_IPV6_SIZE];

    struct
    {
        u16 v0, v1, v2, v3;
        u16 v4, v5, v6, v7;
    };
}
RnAddressIPv6;

typedef struct RnAddressIP
{
    RnAddressIPKind kind;

    union
    {
        RnAddressIPv4 ipv4;
        RnAddressIPv6 ipv6;
    };
}
RnAddressIP;

RnAddressIP
rnAddressIPv4Make(u8 v0, u8 v1, u8 v2, u8 v3);

RnAddressIP
rnAddressIPv4FromArray(u8* values, ssize size);

RnAddressIP
rnAddressIPv6Make(u16 v0, u16 v1, u16 v2, u16 v3, u16 v4, u16 v5, u16 v6, u16 v7);

RnAddressIP
rnAddressIPv6FromArray(u16* values, ssize size);

RnAddressIP
rnAddressIPEmpty(RnAddressIPKind kind);

RnAddressIP
rnAddressIPLocal(RnAddressIPKind kind);

b32
rnAddressIPIsEqual(RnAddressIP self, RnAddressIP value);

#endif // RN_SYSTEM_NETWORK_ADDRESS_IP_H
