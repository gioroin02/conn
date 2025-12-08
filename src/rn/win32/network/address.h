#ifndef RN_WIN32_NETWORK_ADDRESS_H
#define RN_WIN32_NETWORK_ADDRESS_H

#include "./common.h"

RnSockAddrStorage
rnSockAddrStorageMake(RnAddressIP address, u16 port, ssize* size);

RnSockAddrStorage
rnSockAddrStorageMakeAny(RnAddressIPKind kind, u16 port, ssize* size);

RnAddressIP
rnSockAddrStorageGetAddress(RnSockAddrStorage* self);

u16
rnSockAddrStorageGetPort(RnSockAddrStorage* self);

#endif // RN_WIN32_NETWORK_ADDRESS_H
