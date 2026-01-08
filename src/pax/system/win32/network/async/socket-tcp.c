#ifndef PX_WIN32_ASYNC_NETWORK_SOCKET_TCP_C
#define PX_WIN32_ASYNC_NETWORK_SOCKET_TCP_C

#include "socket-tcp.h"

extern LPFN_CONNECTEX pxWin32ConnectEx;
extern LPFN_ACCEPTEX  pxWin32AcceptEx;

static b32 pxWin32AsyncBindSocketTcp(PxWin32Async* async, PxWin32SocketTcp* socket)
{
    HANDLE handle = ((HANDLE) socket->handle);
    HANDLE result = CreateIoCompletionPort(handle, async->handle, 0, 0);

    return result != PX_NULL ? 1 : 0;
}

static b32 pxWin32SocketTcpTaskStart(PxWin32AsyncTask* task, PxWin32Async* async)
{
    PxWin32SocketTcpTask* body = ((PxWin32SocketTcpTask*) task->pntr_body);

    switch (body->kind) {
        case PxSocketTcpEvent_Accept: {
            PxWin32SocketTcp* self  = body->self;
            PxWin32SocketTcp* value = body->accept.value;
            u8*               buff  = body->accept.buff;
            DWORD             bytes = 0;
            DWORD             size  = sizeof (body->accept.buff) / 2;

            if (pxWin32AsyncBindSocketTcp(async, value) == 0) return 0;

            pxWin32AsyncBindSocketTcp(async, self);

            BOOL status = pxWin32AcceptEx(self->handle, value->handle,
                buff, 0, size, size, &bytes, &task->overlap);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case PxSocketTcpEvent_Connect: {
            PxWin32SocketTcp* self    = body->self;
            PxAddressIp       address = body->connect.address;
            u16               port    = body->connect.port;
            DWORD             bytes   = 0;

            PxWin32SockAddrStorage storage;
            PxWin32SockAddr*       sockaddr = (PxWin32SockAddr*) &storage;
            ssize                  length  = 0;

            storage = pxWin32SockAddrStorageMake(address, port, &length);

            pxWin32AsyncBindSocketTcp(async, self);

            BOOL status = pxWin32ConnectEx(self->handle, sockaddr, length,
                0, 0, &bytes, &task->overlap);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case PxSocketTcpEvent_Write: {
            PxWin32SocketTcp* self   = body->self;
            u8*               pntr   = body->write.pntr;
            ssize             start  = body->write.start;
            ssize             stop   = body->write.stop;
            WSABUF*           buffer = &body->write.buffer;
            DWORD             flag   = 0;
            DWORD             bytes  = 0;

            pxWin32AsyncBindSocketTcp(async, self);

            buffer->buf = ((char*) pntr + start);
            buffer->len = stop - start;

            int status = WSASend(self->handle, buffer, 1, &bytes,
                flag, &task->overlap, PX_NULL);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case PxSocketTcpEvent_Read: {
            PxWin32SocketTcp* self   = body->self;
            u8*               pntr   = body->read.pntr;
            ssize             start  = body->read.start;
            ssize             stop   = body->read.stop;
            WSABUF*           buffer = &body->read.buffer;
            DWORD             flag   = 0;
            DWORD             bytes  = 0;

            pxWin32AsyncBindSocketTcp(async, self);

            buffer->buf = ((char*) pntr + start);
            buffer->len = stop - start;

            int status = WSARecv(self->handle, buffer, 1, &bytes,
                &flag, &task->overlap, PX_NULL);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        default: return 0;
    }

    return 1;
}

static b32 pxWin32SocketTcpTaskProc(PxWin32AsyncTask* task, ssize bytes)
{
    PxWin32SocketTcpTask* body  = (PxWin32SocketTcpTask*) task->pntr_body;
    PxSocketTcpEvent*     event = (PxSocketTcpEvent*) task->pntr_event;

    switch (body->kind) {
        case PxSocketTcpEvent_Accept: {
            void*             ctxt  = body->ctxt;
            PxWin32SocketTcp* self  = body->self;
            PxWin32SocketTcp* value = body->accept.value;
            u8*               buff  = body->accept.buff;

            setsockopt(value->handle, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
                (char*) &self->handle, sizeof self->handle);

            PxWin32SockAddrStorage storage;
            PxWin32SockAddr*       sockaddr = (PxWin32SockAddr*) &storage;
            int                    length   = sizeof storage;

            getsockname(value->handle, sockaddr, &length);

            value->storage = storage;

            *event = pxSocketTcpEventAccept(ctxt, self, value);
        } break;

        case PxSocketTcpEvent_Connect: {
            void*             ctxt    = body->ctxt;
            PxWin32SocketTcp* self    = body->self;
            PxAddressIp       address = body->connect.address;
            u16               port    = body->connect.port;
            DWORD             bytes   = 0;
            DWORD             flag    = 0;

            setsockopt(self->handle, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, PX_NULL, 0);

            BOOL status = WSAGetOverlappedResult(self->handle,
                &task->overlap, &bytes, 0, &flag);

            *event = pxSocketTcpEventConnect(ctxt, self, status);
        } break;

        case PxSocketTcpEvent_Write: {
            void*             ctxt  = body->ctxt;
            PxWin32SocketTcp* self  = body->self;
            u8*               pntr  = body->write.pntr;
            ssize             start = body->write.start;
            ssize             stop  = start + bytes;

            *event = pxSocketTcpEventWrite(ctxt, self, pntr, start, stop);
        } break;

        case PxSocketTcpEvent_Read: {
            void*             ctxt  = body->ctxt;
            PxWin32SocketTcp* self  = body->self;
            u8*               pntr  = body->read.pntr;
            ssize             start = body->read.start;
            ssize             stop  = start + bytes;

            *event = pxSocketTcpEventClose(ctxt, self);

            if (bytes > 0)
                *event = pxSocketTcpEventRead(ctxt, self, pntr, start, stop);
        } break;

        default: return 0;
    }

    return 1;
}

static PxWin32AsyncTask* pxWin32AsyncTaskSocketTcp(PxMemoryPool* pool,
    ssize size_body, void** pntr_body, ssize size_event, void** pntr_event)
{
    if (pntr_body == PX_NULL || size_body < 0 || size_body > pool->step)    return 0;
    if (pntr_event == PX_NULL || size_event < 0 || size_event > pool->step) return 0;

    PxWin32AsyncTask* result = (PxWin32AsyncTask*) pxMemoryPoolReserve(pool, 1, pool->step);
    void*             body   = pxMemoryPoolReserve(pool, 1, pool->step);
    void*             event  = pxMemoryPoolReserve(pool, 1, pool->step);

    if (result != PX_NULL && body != PX_NULL && event != PX_NULL) {
        result->family     = PxAsyncEventFamily_Tcp;
        result->pntr_body  = body;
        result->pntr_event = event;
        result->size_body  = size_body;
        result->size_event = size_event;
        result->proc       = pxWin32SocketTcpTaskProc;
        result->list_next  = PX_NULL;

        pxMemorySet(&result->overlap, sizeof result->overlap, 0x00);

        *pntr_body  = body;
        *pntr_event = event;

        return result;
    }

    pxMemoryPoolRelease(pool, result);
    pxMemoryPoolRelease(pool, body);
    pxMemoryPoolRelease(pool, event);

    return PX_NULL;
}

static PxWin32AsyncTask* pxWin32SocketTcpTaskAccept(PxWin32Async* async, void* ctxt,
    PxWin32SocketTcp* self, PxWin32SocketTcp* value)
{
    PxAddressIp address = pxWin32SocketTcpGetAddress(self);

    if (pxWin32SocketTcpCreate(value, pxAddressIpEmpty(address.kind), 0) == 0)
        return PX_NULL;

    PxWin32SocketTcpTask* body  = PX_NULL;
    PxSocketTcpEvent*     event = PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskSocketTcp(&async->pool,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind         = PxSocketTcpEvent_Accept;
    body->ctxt         = ctxt;
    body->self         = self;
    body->accept.value = value;

    if (pxWin32SocketTcpTaskStart(result, async) == 0) {
        pxMemoryPoolRelease(&async->pool, result);
        pxMemoryPoolRelease(&async->pool, body);
        pxMemoryPoolRelease(&async->pool, event);

        return PX_NULL;
    }

    return result;
}

static PxWin32AsyncTask* pxWin32SocketTcpTaskConnect(PxWin32Async* async, void* ctxt,
    PxWin32SocketTcp* self, PxAddressIp address, u16 port)
{
    if (pxWin32SocketTcpBind(self) == 0) return PX_NULL;

    PxWin32SocketTcpTask* body  = PX_NULL;
    PxSocketTcpEvent*     event = PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskSocketTcp(&async->pool,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind            = PxSocketTcpEvent_Connect;
    body->ctxt            = ctxt;
    body->self            = self;
    body->connect.address = address;
    body->connect.port    = port;

    if (pxWin32SocketTcpTaskStart(result, async) == 0) {
        pxMemoryPoolRelease(&async->pool, result);
        pxMemoryPoolRelease(&async->pool, body);
        pxMemoryPoolRelease(&async->pool, event);

        return PX_NULL;
    }

    return result;
}

static PxWin32AsyncTask* pxWin32SocketTcpTaskWrite(PxWin32Async* async, void* ctxt,
    PxWin32SocketTcp* self, u8* pntr, ssize start, ssize stop)
{
    PxWin32SocketTcpTask* body  = PX_NULL;
    PxSocketTcpEvent*     event = PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskSocketTcp(&async->pool,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind        = PxSocketTcpEvent_Write;
    body->ctxt        = ctxt;
    body->self        = self;
    body->write.pntr  = pntr;
    body->write.start = start;
    body->write.stop  = stop;

    if (pxWin32SocketTcpTaskStart(result, async) == 0) {
        pxMemoryPoolRelease(&async->pool, result);
        pxMemoryPoolRelease(&async->pool, body);
        pxMemoryPoolRelease(&async->pool, event);

        return PX_NULL;
    }

    return result;
}

static PxWin32AsyncTask* pxWin32SocketTcpTaskRead(PxWin32Async* async, void* ctxt,
    PxWin32SocketTcp* self, u8* pntr, ssize start, ssize stop)
{
    PxWin32SocketTcpTask* body  = PX_NULL;
    PxSocketTcpEvent*     event = PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskSocketTcp(&async->pool,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind       = PxSocketTcpEvent_Read;
    body->ctxt       = ctxt;
    body->self       = self;
    body->read.pntr  = pntr;
    body->read.start = start;
    body->read.stop  = stop;

    if (pxWin32SocketTcpTaskStart(result, async) == 0) {
        pxMemoryPoolRelease(&async->pool, result);
        pxMemoryPoolRelease(&async->pool, body);
        pxMemoryPoolRelease(&async->pool, event);

        return PX_NULL;
    }

    return result;
}

b32 pxWin32SocketTcpAcceptAsync(PxWin32Async* async, void* ctxt,
    PxWin32SocketTcp* self, PxWin32SocketTcp* value)
{
    PxWin32AsyncTask* task =
        pxWin32SocketTcpTaskAccept(async, ctxt, self, value);

    return pxAsyncSubmit(async, task);
}

b32 pxWin32SocketTcpConnectAsync(PxWin32Async* async, void* ctxt,
    PxWin32SocketTcp* self, PxAddressIp address, u16 port)
{
    PxWin32AsyncTask* task =
        pxWin32SocketTcpTaskConnect(async, ctxt, self, address, port);

    return pxAsyncSubmit(async, task);
}

b32 pxWin32SocketTcpWriteAsync(PxWin32Async* async, void* ctxt,
    PxWin32SocketTcp* self, u8* pntr, ssize start, ssize stop)
{
    PxWin32AsyncTask* task =
        pxWin32SocketTcpTaskWrite(async, ctxt, self, pntr, start, stop);

    return pxAsyncSubmit(async, task);
}

b32 pxWin32SocketTcpReadAsync(PxWin32Async* async, void* ctxt,
    PxWin32SocketTcp* self, u8* pntr, ssize start, ssize stop)
{
    PxWin32AsyncTask* task =
        pxWin32SocketTcpTaskRead(async, ctxt, self, pntr, start, stop);

    return pxAsyncSubmit(async, task);
}

#endif // PX_WIN32_ASYNC_NETWORK_SOCKET_TCP_C
