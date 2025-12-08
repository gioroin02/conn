#ifndef RN_WIN32_NETWORK_COMMON_C
#define RN_WIN32_NETWORK_COMMON_C

#include "./common.h"

static volatile LONG gWinsockRefs = 0;

LPFN_CONNECTEX connectEx = 0;
LPFN_ACCEPTEX  acceptEx  = 0;

b32
rnWin32NetworkStart()
{
    if (InterlockedIncrement(&gWinsockRefs) == 1)
        return rnWin32NetworkStartImpl();

    return 1;
}

b32
rnWin32NetworkStartImpl()
{
    WSADATA data = {0};

    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return 0;

    SOCKET handle = WSASocket(AF_INET,
        SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

    if (handle == INVALID_SOCKET) return 0;

    DWORD bytes = 0;

    GUID guidConnectEx = WSAID_CONNECTEX;
    GUID guidAcceptEx  = WSAID_ACCEPTEX;

    WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER,
        &guidConnectEx, sizeof(guidConnectEx), &connectEx, sizeof(connectEx), &bytes, 0, 0);

    WSAIoctl(handle, SIO_GET_EXTENSION_FUNCTION_POINTER,
        &guidAcceptEx, sizeof(guidAcceptEx), &acceptEx, sizeof(acceptEx), &bytes, 0, 0);

    closesocket(handle);

    if (connectEx != 0 && acceptEx != 0) return 1;

    rnWin32NetworkStop();

    return 0;
}

void
rnWin32NetworkStop()
{
    if (InterlockedDecrement(&gWinsockRefs) == 0)
        rnWin32NetworkStopImpl();
}

void
rnWin32NetworkStopImpl()
{
    WSACleanup();
}

#endif // RN_WIN32_NETWORK_COMMON_C
