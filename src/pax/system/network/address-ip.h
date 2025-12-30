#ifndef PX_SYSTEM_NETWORK_ADDRESS_IP_H
#define PX_SYSTEM_NETWORK_ADDRESS_IP_H

#include "import.h"

#define PX_ADDRESS_IP4_SIZE ((ssize) 4)
#define PX_ADDRESS_IP6_SIZE ((ssize) 8)

#define pxAddressIp4Empty() pxAddressIp4Make(  0, 0, 0, 0)
#define pxAddressIp4Local() pxAddressIp4Make(127, 0, 0, 1)

#define pxAddressIp6Empty() pxAddressIp6Make(0, 0, 0, 0, 0, 0, 0, 0)
#define pxAddressIp6Local() pxAddressIp6Make(0, 0, 0, 0, 0, 0, 0, 1)

typedef enum PxAddressIpKind
{
    PxAddressIp_None,
    PxAddressIp_Ver4,
    PxAddressIp_Ver6,
}
PxAddressIpKind;

typedef union PxAddressIp4
{
    u8 values[PX_ADDRESS_IP4_SIZE];

    struct
    {
        u8 v0, v1;
        u8 v2, v3;
    };
}
PxAddressIp4;

typedef union PxAddressIp6
{
    u16 values[PX_ADDRESS_IP6_SIZE];

    struct
    {
        u16 v0, v1, v2, v3;
        u16 v4, v5, v6, v7;
    };
}
PxAddressIp6;

typedef struct PxAddressIp
{
    PxAddressIpKind kind;

    union
    {
        PxAddressIp4 ip4;
        PxAddressIp6 ip6;
    };
}
PxAddressIp;

PxAddressIp
pxAddressIp4Make(u8 v0, u8 v1, u8 v2, u8 v3);

PxAddressIp
pxAddressIp6Make(u16 v0, u16 v1, u16 v2, u16 v3, u16 v4, u16 v5, u16 v6, u16 v7);

PxAddressIp
pxAddressIpNone();

PxAddressIp
pxAddressIpEmpty(PxAddressIpKind kind);

PxAddressIp
pxAddressIpLocal(PxAddressIpKind kind);

b32
pxAddressIpIsEqual(PxAddressIp self, PxAddressIp value);

#endif // PX_SYSTEM_NETWORK_ADDRESS_IP_H
