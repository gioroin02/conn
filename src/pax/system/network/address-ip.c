#ifndef P_SYSTEM_NETWORK_ADDRESS_IP_C
#define P_SYSTEM_NETWORK_ADDRESS_IP_C

#include "address-ip.h"

static Bool pAddressIp4IsEqual(PAddressIp4 self, PAddressIp4 value)
{
    Int index = 0;
    Int size  = P_ADDRESS_IP4_SIZE;

    for (index = 0; index < size; index += 1) {
        if (self.values[index] != value.values[index])
            return 0;
    }

    return 1;
}

static Bool pAddressIp6IsEqual(PAddressIp6 self, PAddressIp6 value)
{
    Int index = 0;
    Int size  = P_ADDRESS_IP6_SIZE;

    for (index = 0; index < size; index += 1) {
        if (self.values[index] != value.values[index])
            return 0;
    }

    return 1;
}

PAddressIp pAddressIp4Make(U8 v0, U8 v1, U8 v2, U8 v3)
{
    PAddressIp result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind   = PAddressIp_Ver4;
    result.ip4.v0 = v0;
    result.ip4.v1 = v1;
    result.ip4.v2 = v2;
    result.ip4.v3 = v3;

    return result;
}

PAddressIp pAddressIp6Make(U16 v0, U16 v1, U16 v2, U16 v3, U16 v4, U16 v5, U16 v6, U16 v7)
{
    PAddressIp result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind   = PAddressIp_Ver6;
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

PAddressIp pAddressIpNone()
{
    PAddressIp result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind = PAddressIp_None;

    return result;
}

PAddressIp pAddressIpAny(PAddressIpKind kind)
{
    PAddressIp result;

    pMemorySet(&result, sizeof result, 0xAB);

    switch (kind) {
        case PAddressIp_Ver4: return pAddressIp4Any();
        case PAddressIp_Ver6: return pAddressIp6Any();

        default: break;
    }

    return result;
}

PAddressIp pAddressIpSelf(PAddressIpKind kind)
{
    PAddressIp result;

    pMemorySet(&result, sizeof result, 0xAB);

    switch (kind) {
        case PAddressIp_Ver4: return pAddressIp4Self();
        case PAddressIp_Ver6: return pAddressIp6Self();

        default: break;
    }

    return result;
}

Bool pAddressIpIsEqual(PAddressIp self, PAddressIp value)
{
    if (self.kind != value.kind) return 0;

    switch (self.kind) {
        case PAddressIp_Ver4:
            return pAddressIp4IsEqual(self.ip4, value.ip4);

        case PAddressIp_Ver6:
            return pAddressIp6IsEqual(self.ip6, value.ip6);

        default: break;
    }

    return 0;
}

PHostIp pHostIpMake(PAddressIp address, U16 port)
{
    PHostIp result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.address = address;
    result.port    = port;

    return result;
}

#endif // P_SYSTEM_NETWORK_ADDRESS_IP_C
