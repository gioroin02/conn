#ifndef PX_WIN32_ASYNC_NETWORK_SOCKET_TCP_C
#define PX_WIN32_ASYNC_NETWORK_SOCKET_TCP_C

#include "socket-tcp.h"

extern LPFN_CONNECTEX pxWin32ConnectEx;
extern LPFN_ACCEPTEX  pxWin32AcceptEx;

static b32
pxWin32AsyncBindSocketTcp(PxWin32Async* async, PxWin32SocketTcp* socket)
{
    HANDLE handle = ((HANDLE) socket->handle);

    if (CreateIoCompletionPort(handle, async->handle, 0, 0) == 0)
        return 0;

    return 1;
}

static b32
pxWin32SocketTcpTaskPrepare(PxWin32AsyncTask* task, PxWin32Async* async)
{
    PxWin32SocketTcpTask* self = ((PxWin32SocketTcpTask*) task->pntr_body);

    switch (self->kind) {
        case PxSocketTcpEvent_Accept: {
            PxWin32SocketTcpTaskAccept accept = self->accept;
            DWORD                      bytes  = 0;
            DWORD                      size   = sizeof (PxWin32SockAddrStorage);

            pxWin32AsyncBindSocketTcp(async, accept.listener);
            pxWin32AsyncBindSocketTcp(async, accept.socket);

            BOOL status = pxWin32AcceptEx(accept.listener->handle, accept.socket->handle,
                accept.buffer, 0, size, size, &bytes, &task->overlap);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case PxSocketTcpEvent_Connect: {
            PxWin32SocketTcpTaskConnect connect = self->connect;
            DWORD                       bytes   = 0;
            ssize                       length  = 0;

            pxWin32AsyncBindSocketTcp(async, connect.socket);

            PxWin32SockAddrStorage storage =
                pxWin32SockAddrStorageMake(connect.address, connect.port, &length);

            PxWin32SockAddr* sockaddr = (PxWin32SockAddr*) &storage;

            BOOL status = pxWin32ConnectEx(connect.socket->handle, sockaddr,
                length, 0, 0, &bytes, &task->overlap);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case PxSocketTcpEvent_Write: {
            PxWin32SocketTcpTaskWrite write = self->write;
            DWORD                     bytes = 0;

            int status = WSASend(write.socket->handle, &write.wsabuf, 1,
                &bytes, write.wsaflags, &task->overlap, PX_NULL);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case PxSocketTcpEvent_Read: {
            PxWin32SocketTcpTaskRead read  = self->read;
            DWORD                    bytes = 0;

            int status = WSARecv(read.socket->handle, &read.wsabuf, 1,
                &bytes, &read.wsaflags, &task->overlap, PX_NULL);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        default: return 0;
    }

    return 1;
}

static b32
pxWin32SocketTcpTaskComplete(PxWin32AsyncTask* task, ssize bytes)
{
    PxWin32SocketTcpTask* self  = (PxWin32SocketTcpTask*) task->pntr_body;
    PxSocketTcpEvent*     event = (PxSocketTcpEvent*) task->pntr_event;

    switch (self->kind) {
        case PxSocketTcpEvent_Accept: {
            PxWin32SocketTcpTaskAccept accept = self->accept;
            PxWin32SockAddrStorage     storage;

            setsockopt(accept.socket->handle, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
                ((char*) &accept.listener->handle), sizeof(accept.listener->handle));

            INT              length   = sizeof (PxWin32SockAddrStorage);
            PxWin32SockAddr* sockaddr = (PxWin32SockAddr*) &storage;

            getsockname(accept.socket->handle, sockaddr, &length);

            accept.socket->storage = storage;

            *event = pxSocketTcpEventAccept(accept.listener, accept.socket);
        } break;

        case PxSocketTcpEvent_Connect: {
            PxWin32SocketTcpTaskConnect connect = self->connect;
            DWORD                       bytes   = 0;
            DWORD                       flags   = 0;

            setsockopt(connect.socket->handle, SOL_SOCKET,
                SO_UPDATE_CONNECT_CONTEXT, 0, 0);

            BOOL status = WSAGetOverlappedResult(connect.socket->handle,
                &task->overlap, &bytes, 0, &flags);

            *event = pxSocketTcpEventConnect(connect.socket, status);
        } break;

        case PxSocketTcpEvent_Write: {
            PxWin32SocketTcpTaskWrite write = self->write;

            *event = pxSocketTcpEventWrite(write.socket,
                write.values, write.start, write.start + bytes);
        } break;

        case PxSocketTcpEvent_Read: {
            PxWin32SocketTcpTaskRead read = self->read;

            if (bytes > 0) {
                *event = pxSocketTcpEventRead(read.socket,
                    read.values, read.start, read.start + bytes);
            }
            else *event = pxSocketTcpEventClose(read.socket);
        } break;

        default: return 0;
    }

    return 1;
}

static PxWin32AsyncTask*
pxWin32AsyncTaskSocketTcp(PxMemoryPool* pool, void* tag, ssize size_body, void** pntr_body, ssize size_event, void** pntr_event)
{
    if (pntr_body == 0 || size_body < 0 || size_body > pool->step)    return 0;
    if (pntr_event == 0 || size_event < 0 || size_event > pool->step) return 0;

    PxWin32AsyncTask* result = (PxWin32AsyncTask*) pxMemoryPoolReserve(pool, 1, pool->step);
    void*             body   = pxMemoryPoolReserve(pool, 1, pool->step);
    void*             event  = pxMemoryPoolReserve(pool, 1, pool->step);

    if (result != PX_NULL && body != PX_NULL && event != PX_NULL) {
        result->family       = PxAsyncEventFamily_Tcp;
        result->pntr_body    = body;
        result->pntr_event   = event;
        result->pntr_tag     = tag;
        result->proc         = pxWin32SocketTcpTaskComplete;
        result->pending_next = PX_NULL;

        pxMemorySet(&result->overlap, sizeof result->overlap, 0x00);

        if (pntr_body != 0)  *pntr_body  = body;
        if (pntr_event != 0) *pntr_event = event;

        return result;
    }

    pxMemoryPoolRelease(pool, result);
    pxMemoryPoolRelease(pool, body);
    pxMemoryPoolRelease(pool, event);

    return PX_NULL;
}

static PxWin32AsyncTask*
pxWin32SocketTcpTaskAccept(PxMemoryPool* pool, void* tag, PxWin32SocketTcp* listener, PxWin32SocketTcp* socket)
{
    PxWin32SocketTcpTask* body  = PX_NULL;
    PxSocketTcpEvent*     event = PX_NULL;

    PxAddressIpKind kind    = pxWin32SocketTcpGetAddress(listener).kind;
    PxAddressIp     address = pxAddressIpEmpty(kind);

    if (pxWin32SocketTcpCreate(socket, address, 0) == 0) return PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskSocketTcp(pool, tag,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind            = PxSocketTcpEvent_Accept;
    body->accept.listener = listener;
    body->accept.socket   = socket;

    return result;
}

static PxWin32AsyncTask*
pxWin32SocketTcpTaskConnect(PxMemoryPool* pool, void* tag, PxWin32SocketTcp* socket, PxAddressIp address, u16 port)
{
    PxWin32SocketTcpTask* body  = PX_NULL;
    PxSocketTcpEvent*     event = PX_NULL;

    if (pxWin32SocketTcpBind(socket) == 0) return PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskSocketTcp(pool, tag,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind            = PxSocketTcpEvent_Connect;
    body->connect.socket  = socket;
    body->connect.address = address;
    body->connect.port    = port;

    return result;
}

static PxWin32AsyncTask*
pxWin32SocketTcpTaskWrite(PxMemoryPool* pool, void* tag, PxWin32SocketTcp* socket, u8* values, ssize start, ssize stop)
{
    PxWin32SocketTcpTask* body  = PX_NULL;
    PxSocketTcpEvent*     event = PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskSocketTcp(pool, tag,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind             = PxSocketTcpEvent_Write;
    body->write.socket     = socket;
    body->write.values     = values;
    body->write.start      = start;
    body->write.stop       = stop;
    body->write.wsabuf.buf = ((char*) values) + start;
    body->write.wsabuf.len = stop - start;
    body->write.wsaflags   = 0;

    return result;
}

static PxWin32AsyncTask*
pxWin32SocketTcpTaskRead(PxMemoryPool* pool, void* tag, PxWin32SocketTcp* socket, u8* values, ssize start, ssize stop)
{
    PxWin32SocketTcpTask* body  = PX_NULL;
    PxSocketTcpEvent*     event = PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskSocketTcp(pool, tag,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind            = PxSocketTcpEvent_Read;
    body->read.socket     = socket;
    body->read.values     = values;
    body->read.start      = start;
    body->read.stop       = stop;
    body->read.wsabuf.buf = ((char*) values) + start;
    body->read.wsabuf.len = stop - start;
    body->read.wsaflags   = 0;

    return result;
}

b32
pxWin32SocketTcpAcceptAsync(PxWin32Async* async, void* tag, PxWin32SocketTcp* self, PxWin32SocketTcp* value)
{
    PxWin32AsyncTask* task = pxWin32SocketTcpTaskAccept(
        &async->pool, tag, self, value);

    if (pxWin32SocketTcpTaskPrepare(task, async) == 0)
        return 0;

    return pxAsyncSubmit(async, task);
}

b32
pxWin32SocketTcpConnectAsync(PxWin32Async* async, void* tag, PxWin32SocketTcp* self, PxAddressIp address, u16 port)
{
    PxWin32AsyncTask* task = pxWin32SocketTcpTaskConnect(
        &async->pool, tag, self, address, port);

    if (pxWin32SocketTcpTaskPrepare(task, async) == 0)
        return 0;

    return pxAsyncSubmit(async, task);
}

b32
pxWin32SocketTcpWriteAsync(PxWin32Async* async, void* tag, PxWin32SocketTcp* self, u8* values, ssize start, ssize stop)
{
    PxWin32AsyncTask* task = pxWin32SocketTcpTaskWrite(
        &async->pool, tag, self, values, start, stop);

    if (pxWin32SocketTcpTaskPrepare(task, async) == 0)
        return 0;

    return pxAsyncSubmit(async, task);
}

b32
pxWin32SocketTcpReadAsync(PxWin32Async* async, void* tag, PxWin32SocketTcp* self, u8* values, ssize start, ssize stop)
{
    PxWin32AsyncTask* task = pxWin32SocketTcpTaskRead(
        &async->pool, tag, self, values, start, stop);

    if (pxWin32SocketTcpTaskPrepare(task, async) == 0)
        return 0;

    return pxAsyncSubmit(async, task);
}

#endif // PX_WIN32_ASYNC_NETWORK_SOCKET_TCP_C
