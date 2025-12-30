#ifndef PX_WIN32_ASYNC_NETWORK_SOCKET_TCP_H
#define PX_WIN32_ASYNC_NETWORK_SOCKET_TCP_H

#include "import.h"

typedef struct PxWin32SocketTcpTaskAccept
{
    PxWin32SocketTcp* listener;
    PxWin32SocketTcp* socket;

    u8 buffer[256];
}
PxWin32SocketTcpTaskAccept;

typedef struct PxWin32SocketTcpTaskConnect
{
    PxWin32SocketTcp* socket;
    PxAddressIp       address;
    u16               port;
}
PxWin32SocketTcpTaskConnect;

typedef struct PxWin32SocketTcpTaskWrite
{
    PxWin32SocketTcp* socket;
    u8*               values;
    ssize             start;
    ssize             stop;
    WSABUF            wsabuf;
    DWORD             wsaflags;
}
PxWin32SocketTcpTaskWrite;

typedef struct PxWin32SocketTcpTaskRead
{
    PxWin32SocketTcp* socket;
    u8*               values;
    ssize             start;
    ssize             stop;
    WSABUF            wsabuf;
    DWORD             wsaflags;
}
PxWin32SocketTcpTaskRead;

typedef struct PxWin32SocketTcpTask
{
    PxSocketTcpEventKind kind;

    union
    {
        PxWin32SocketTcpTaskAccept  accept;
        PxWin32SocketTcpTaskConnect connect;
        PxWin32SocketTcpTaskWrite   write;
        PxWin32SocketTcpTaskRead    read;
    };
}
PxWin32SocketTcpTask;

b32
pxWin32SocketTcpAcceptAsync(PxWin32Async* async, void* tag, PxWin32SocketTcp* self, PxWin32SocketTcp* value);

b32
pxWin32SocketTcpConnectAsync(PxWin32Async* async, void* tag, PxWin32SocketTcp* self, PxAddressIp address, u16 port);

b32
pxWin32SocketTcpWriteAsync(PxWin32Async* async, void* tag, PxWin32SocketTcp* self, u8* values, ssize start, ssize stop);

b32
pxWin32SocketTcpReadAsync(PxWin32Async* async, void* tag, PxWin32SocketTcp* self, u8* values, ssize start, ssize stop);

#endif // PX_WIN32_ASYNC_NETWORK_SOCKET_TCP_H
