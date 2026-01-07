#ifndef PX_WIN32_NETWORK_COMMON_C
#define PX_WIN32_NETWORK_COMMON_C

#include "common.h"

LPFN_CONNECTEX pxWin32ConnectEx = 0;
LPFN_ACCEPTEX  pxWin32AcceptEx  = 0;

static volatile LONG px_win32_winsock_refs = 0;

b32
pxWin32NetworkStart()
{
    if (InterlockedIncrement(&px_win32_winsock_refs) == 1)
        return pxWin32NetworkStartImpl();

    return 1;
}

b32
pxWin32NetworkStartImpl()
{
    WSADATA data;

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return 0;

    SOCKET handle = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

    if (handle == INVALID_SOCKET) return 0;

    DWORD bytes = 0;

    GUID guid_connect_ex = WSAID_CONNECTEX;
    GUID guid_accept_ex  = WSAID_ACCEPTEX;

    WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid_connect_ex, sizeof guid_connect_ex,
        &pxWin32ConnectEx, sizeof pxWin32ConnectEx, &bytes, 0, 0);

    WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid_accept_ex, sizeof guid_accept_ex,
        &pxWin32AcceptEx, sizeof pxWin32AcceptEx, &bytes, 0, 0);

    closesocket(handle);

    if (pxWin32ConnectEx != PX_NULL && pxWin32AcceptEx != PX_NULL) return 1;

    pxWin32NetworkStop();

    return 0;
}

void
pxWin32NetworkStop()
{
    if (InterlockedDecrement(&px_win32_winsock_refs) == 0)
        pxWin32NetworkStopImpl();
}

void
pxWin32NetworkStopImpl()
{
    pxWin32ConnectEx = PX_NULL;
    pxWin32AcceptEx  = PX_NULL;

    WSACleanup();
}

#endif // PX_WIN32_NETWORK_COMMON_C
