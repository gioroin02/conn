#ifndef PX_SYSTEM_NETWORK_ADDRESS_IP_C
#define PX_SYSTEM_NETWORK_ADDRESS_IP_C

#include "address-ip.h"

static b32
pxAddressIp4IsEqual(PxAddressIp4 self, PxAddressIp4 value)
{
    ssize index = 0;
    ssize size  = PX_ADDRESS_IP4_SIZE;

    for (index = 0; index < size; index += 1) {
        if (self.values[index] != value.values[index])
            return 0;
    }

    return 1;
}

static b32
pxAddressIp6IsEqual(PxAddressIp6 self, PxAddressIp6 value)
{
    ssize index = 0;
    ssize size  = PX_ADDRESS_IP6_SIZE;

    for (index = 0; index < size; index += 1) {
        if (self.values[index] != value.values[index])
            return 0;
    }

    return 1;
}

PxAddressIp
pxAddressIp4Make(u8 v0, u8 v1, u8 v2, u8 v3)
{
    PxAddressIp result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind   = PxAddressIp_Ver4;
    result.ip4.v0 = v0;
    result.ip4.v1 = v1;
    result.ip4.v2 = v2;
    result.ip4.v3 = v3;

    return result;
}

PxAddressIp
pxAddressIp6Make(u16 v0, u16 v1, u16 v2, u16 v3, u16 v4, u16 v5, u16 v6, u16 v7)
{
    PxAddressIp result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind   = PxAddressIp_Ver6;
    result.ip6.v0 = v0;
    result.ip6.v1 = v1;
    result.ip6.v2 = v2;
    result.ip6.v3 = v3;
    result.ip6.v4 = v4;
    result.ip6.v5 = v5;
    result.ip6.v6 = v6;
    result.ip6.v7 = v7;

    return result;
}

PxAddressIp
pxAddressIpNone()
{
    PxAddressIp result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind = PxAddressIp_None;

    return result;
}

PxAddressIp
pxAddressIpEmpty(PxAddressIpKind kind)
{
    PxAddressIp result;

    pxMemorySet(&result, sizeof result, 0xAB);

    switch (kind) {
        case PxAddressIp_Ver4: return pxAddressIp4Empty();
        case PxAddressIp_Ver6: return pxAddressIp6Empty();

        default: break;
    }

    return result;
}

PxAddressIp
pxAddressIpLocal(PxAddressIpKind kind)
{
    PxAddressIp result;

    pxMemorySet(&result, sizeof result, 0xAB);

    switch (kind) {
        case PxAddressIp_Ver4: return pxAddressIp4Local();
        case PxAddressIp_Ver6: return pxAddressIp6Local();

        default: break;
    }

    return result;
}

b32
pxAddressIpIsEqual(PxAddressIp self, PxAddressIp value)
{
    if (self.kind != value.kind) return 0;

    switch (self.kind) {
        case PxAddressIp_Ver4:
            return pxAddressIp4IsEqual(self.ip4, value.ip4);

        case PxAddressIp_Ver6:
            return pxAddressIp6IsEqual(self.ip6, value.ip6);

        default: break;
    }

    return 0;
}

#endif // PX_SYSTEM_NETWORK_ADDRESS_IP_C
