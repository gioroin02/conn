#ifndef PX_WIN32_NETWORK_ADDRESS_H
#define PX_WIN32_NETWORK_ADDRESS_H

#include "common.h"

PxWin32SockAddrStorage
pxWin32SockAddrStorageMake(PxAddressIp address, u16 port, ssize* size);

PxWin32SockAddrStorage
pxWin32SockAddrStorageMakeAny(PxAddressIpKind kind, u16 port, ssize* size);

PxAddressIp
pxWin32SockAddrStorageGetAddress(PxWin32SockAddrStorage* self);

u16
pxWin32SockAddrStorageGetPort(PxWin32SockAddrStorage* self);

#endif // PX_WIN32_NETWORK_ADDRESS_H
