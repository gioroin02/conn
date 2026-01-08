#ifndef PX_WIN32_NETWORK_COMMON_H
#define PX_WIN32_NETWORK_COMMON_H

#include "import.h"

#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

typedef struct sockaddr_storage PxWin32SockAddrStorage;
typedef struct sockaddr         PxWin32SockAddr;
typedef struct sockaddr_in      PxWin32SockAddrIn4;
typedef struct sockaddr_in6     PxWin32SockAddrIn6;

b32 pxWin32NetworkStart();

b32 pxWin32NetworkStartImpl();

void pxWin32NetworkStop();

void pxWin32NetworkStopImpl();

PxWin32SockAddrStorage pxWin32SockAddrStorageMake(PxAddressIp address, u16 port, ssize* size);

PxWin32SockAddrStorage pxWin32SockAddrStorageMakeAny(PxAddressIpKind kind, u16 port, ssize* size);

PxAddressIp pxWin32SockAddrStorageGetAddress(PxWin32SockAddrStorage* self);

u16 pxWin32SockAddrStorageGetPort(PxWin32SockAddrStorage* self);

#endif // PX_WIN32_NETWORK_COMMON_H
