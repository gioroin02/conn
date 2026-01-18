#ifndef P_SYSTEM_NETWORK_ADDRESS_IP_H
#define P_SYSTEM_NETWORK_ADDRESS_IP_H

#include "import.h"

#define P_ADDRESS_IP4_SIZE ((Int) 4)
#define P_ADDRESS_IP6_SIZE ((Int) 8)

#define pAddressIp4Any()  pAddressIp4Make(0x00, 0x00, 0x00, 0x00)
#define pAddressIp4Self() pAddressIp4Make(0x7f, 0x00, 0x00, 0x01)

#define pAddressIp6Any()  pAddressIp6Make(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)
#define pAddressIp6Self() pAddressIp6Make(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01)

typedef enum PAddressIpKind
{
    PAddressIp_None,
    PAddressIp_Ver4,
    PAddressIp_Ver6,
}
PAddressIpKind;

typedef union PAddressIp4
{
    U8 values[P_ADDRESS_IP4_SIZE];

    struct
    {
        U8 v0, v1, v2, v3;
    };
}
PAddressIp4;

typedef union PAddressIp6
{
    U16 values[P_ADDRESS_IP6_SIZE];

    struct
    {
        U16 v0, v1, v2, v3, v4, v5, v6, v7;
    };
}
PAddressIp6;

typedef struct PAddressIp
{
    PAddressIpKind kind;

    union
    {
        PAddressIp4 ip4;
        PAddressIp6 ip6;
    };
}
PAddressIp;

typedef struct PHostIp
{
    PAddressIp address;
    U16        port;
}
PHostIp;

PAddressIp pAddressIp4Make(U8 v0, U8 v1, U8 v2, U8 v3);

PAddressIp pAddressIp6Make(U16 v0, U16 v1, U16 v2, U16 v3, U16 v4, U16 v5, U16 v6, U16 v7);

PAddressIp pAddressIpNone();

PAddressIp pAddressIpAny(PAddressIpKind kind);

PAddressIp pAddressIpSelf(PAddressIpKind kind);

Bool pAddressIpIsEqual(PAddressIp self, PAddressIp value);

PHostIp pHostIpMake(PAddressIp address, U16 port);

#endif // P_SYSTEM_NETWORK_ADDRESS_IP_H
