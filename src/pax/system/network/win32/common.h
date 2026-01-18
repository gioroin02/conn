#ifndef P_SYSTEM_WIN32_NETWORK_COMMON_H
#define P_SYSTEM_WIN32_NETWORK_COMMON_H

#include "import.h"

#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

typedef struct sockaddr_storage PWin32AddrStorage;
typedef struct sockaddr         PWin32Addr;
typedef struct sockaddr_in      PWin32AddrIp4;
typedef struct sockaddr_in6     PWin32AddrIp6;

Bool pWin32NetworkStart();

Bool pWin32NetworkStartImpl();

void pWin32NetworkStop();

void pWin32NetworkStopImpl();

PWin32AddrStorage pWin32AddrStorageMake(PAddressIp address, U16 port, Int* size);

PWin32AddrStorage pWin32AddrStorageMakeAny(PAddressIpKind kind, U16 port, Int* size);

Int pWin32AddrStorageGetSize(PWin32AddrStorage* self);

PAddressIp pWin32AddrStorageGetAddress(PWin32AddrStorage* self);

U16 pWin32AddrStorageGetPort(PWin32AddrStorage* self);

#endif // P_SYSTEM_WIN32_NETWORK_COMMON_H
