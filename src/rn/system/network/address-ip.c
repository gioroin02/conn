#ifndef RN_SYSTEM_NETWORK_ADDRESS_IP_C
#define RN_SYSTEM_NETWORK_ADDRESS_IP_C

#include "./address-ip.h"

static b32
rnAddressIPv4IsEqual(RnAddressIPv4 self, RnAddressIPv4 value)
{
    for (ssize i = 0; i < RN_ADDRESS_IPV4_SIZE; i += 1) {
        if (self.values[i] != value.values[i])
            return 0;
    }

    return 1;
}

static b32
rnAddressIPv6IsEqual(RnAddressIPv6 self, RnAddressIPv6 value)
{
    for (ssize i = 0; i < RN_ADDRESS_IPV6_SIZE; i += 1) {
        if (self.values[i] != value.values[i])
            return 0;
    }

    return 1;
}

RnAddressIP
rnAddressIPv4Make(u8 v0, u8 v1, u8 v2, u8 v3)
{
    return (RnAddressIP) {
        .kind = RnAddressIP_IPv4,

        .ipv4.values = {
            [0] = v0, [1] = v1,
            [2] = v2, [3] = v3,
        },
    };
}

RnAddressIP
rnAddressIPv4FromArray(u8* values, ssize size)
{
    RnAddressIP result = {.kind = RnAddressIP_IPv4};

    size = rnClamp(size, 0, RN_ADDRESS_IPV4_SIZE);

    for (ssize i = 0; i < size; i += 1)
        result.ipv4.values[i] = values[i];

    return result;
}

RnAddressIP
rnAddressIPv6Make(u16 v0, u16 v1, u16 v2, u16 v3, u16 v4, u16 v5, u16 v6, u16 v7)
{
    return (RnAddressIP) {
        .kind = RnAddressIP_IPv6,

        .ipv6.values = {
            [0] = v0, [1] = v1, [2] = v2, [3] = v3,
            [4] = v4, [5] = v5, [6] = v6, [7] = v7,
        },
    };
}

RnAddressIP
rnAddressIPv6FromArray(u16* values, ssize size)
{
    RnAddressIP result = {.kind = RnAddressIP_IPv6};

    size = rnClamp(size, 0, RN_ADDRESS_IPV6_SIZE);

    for (ssize i = 0; i < size; i += 1)
        result.ipv6.values[i] = values[i];

    return result;
}

RnAddressIP
rnAddressIPEmpty(RnAddressIPKind kind)
{
    switch (kind) {
        case RnAddressIP_IPv4: return rnAddressIPv4Empty();
        case RnAddressIP_IPv6: return rnAddressIPv6Empty();

        default: break;
    }

    return (RnAddressIP) {0};
}

RnAddressIP
rnAddressIPLocal(RnAddressIPKind kind)
{
    switch (kind) {
        case RnAddressIP_IPv4: return rnAddressIPv4Local();
        case RnAddressIP_IPv6: return rnAddressIPv6Local();

        default: break;
    }

    return (RnAddressIP) {0};
}

b32
rnAddressIPIsEqual(RnAddressIP self, RnAddressIP value)
{
    if (self.kind != value.kind) return 0;

    switch (self.kind) {
        case RnAddressIP_IPv4:
            return rnAddressIPv4IsEqual(self.ipv4, value.ipv4);

        case RnAddressIP_IPv6:
            return rnAddressIPv6IsEqual(self.ipv6, value.ipv6);

        default: break;
    }

    return 0;
}

#endif // RN_SYSTEM_NETWORK_ADDRESS_IP_C
