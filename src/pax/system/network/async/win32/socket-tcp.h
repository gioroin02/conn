#ifndef P_SYSTEM_WIN32_NETWORK_ASYNC_SOCKET_TCP_H
#define P_SYSTEM_WIN32_NETWORK_ASYNC_SOCKET_TCP_H

#include "import.h"

typedef struct PWin32SocketTcpAccept
{
    __PWin32AsyncIoTaskTag__;

    PWin32SocketTcp* self;
    void*            ctxt;

    PWin32SocketTcp* value;

    U8 __buff__[sizeof (PWin32AddrStorage) * 2];
}
PWin32SocketTcpAccept;

typedef struct PWin32SocketTcpConnect
{
    __PWin32AsyncIoTaskTag__;

    PWin32SocketTcp* self;
    void*            ctxt;

    PHostIp host;
}
PWin32SocketTcpConnect;

typedef struct PWin32SocketTcpWrite
{
    __PWin32AsyncIoTaskTag__;

    PWin32SocketTcp* self;
    void*            ctxt;

    U8*    pntr;
    Int    start;
    Int    stop;
    WSABUF buffer;
}
PWin32SocketTcpWrite;

typedef struct PWin32SocketTcpRead
{
    __PWin32AsyncIoTaskTag__;

    PWin32SocketTcp* self;
    void*            ctxt;

    U8*    pntr;
    Int    start;
    Int    stop;
    WSABUF buffer;
}
PWin32SocketTcpRead;

Bool pWin32SocketTcpAcceptAsync(PWin32SocketTcp* self, PWin32SocketTcp* value, PWin32AsyncIoQueue* queue, void* ctxt);

Bool pWin32SocketTcpConnectAsync(PWin32SocketTcp* self, PHostIp host, PWin32AsyncIoQueue* queue, void* ctxt);

Bool pWin32SocketTcpWriteAsync(PWin32SocketTcp* self, U8* pntr, Int start, Int stop, PWin32AsyncIoQueue* queue, void* ctxt);

Bool pWin32SocketTcpReadAsync(PWin32SocketTcp* self, U8* pntr, Int start, Int stop, PWin32AsyncIoQueue* queue, void* ctxt);

#endif // P_SYSTEM_WIN32_NETWORK_ASYNC_SOCKET_TCP_H
