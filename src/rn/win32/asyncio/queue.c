#ifndef RN_WIN32_ASYNCIO_QUEUE_C
#define RN_WIN32_ASYNCIO_QUEUE_C

#include "./queue.h"

extern LPFN_CONNECTEX            connectEx;
extern LPFN_ACCEPTEX             acceptEx;
extern LPFN_GETACCEPTEXSOCKADDRS getAcceptExSockAddrs;

static b32
rnWin32AsyncIOQueueBindSocketTCP(RnWin32AsyncIOQueue* self, RnWin32SocketTCP* value)
{
    if (self == 0 || value == 0 || value->storage.ss_family == 0)
        return 0;

    HANDLE handle = ((HANDLE) value->handle);

    if (CreateIoCompletionPort(handle, self->handle, 0, 0) == 0)
        return 0;

    return 1;
}

static RnWin32AsyncIOTask*
rnWin32AsyncIOQueueRemoveTaskByOverlapped(RnWin32AsyncIOQueue* self, OVERLAPPED* overlap)
{
    if (self == 0 || overlap == 0) return 0;

    for (RnWin32AsyncIOTask* task = self->head; task != 0; task = task->next) {
        if (&task->overlap == overlap) {
            if (task->prev)
                task->prev->next = task->next;
            else
                self->head = task->next;

            if (task->next)
                task->next->prev = task->prev;
            else
                self->tail = task->prev;

            task->prev = task->next = 0;

            return task;
        }
    }

    return 0;
}

RnWin32AsyncIOQueue*
rnWin32AsyncIOQueueReserve(RnMemoryArena* arena)
{
    return rnMemoryArenaReserveOneOf(arena, RnWin32AsyncIOQueue);
}

b32
rnWin32AsyncIOQueueCreate(RnWin32AsyncIOQueue* self)
{
    if (self == 0) return 0;

    HANDLE handle = CreateIoCompletionPort(
        INVALID_HANDLE_VALUE, 0, 0, 0);

    if (handle == 0) return 0;

    self->handle = handle;

    return 1;
}

void
rnWin32AsyncIOQueueDestroy(RnWin32AsyncIOQueue* self)
{
    if (self == 0) return;

    if (self->handle != 0)
        CloseHandle(self->handle);

    *self = (RnWin32AsyncIOQueue) {0};
}

b32
rnWin32AsyncIOQueueSubmit(RnWin32AsyncIOQueue* self, RnWin32AsyncIOTask* task)
{
    if (self == 0 || task == 0) return 0;

    switch (task->kind) {
        case RnAsyncIOEvent_Accept: {
            // TODO(gio): Work on an explicit "interest group"

            RnWin32AsyncIOTaskAccept accept = task->accept;

            rnWin32AsyncIOQueueBindSocketTCP(self, accept.listener);

            if (rnWin32AsyncIOQueueBindSocketTCP(self, accept.socket) == 0)
                return 0;

            DWORD bytes = 0;

            BOOL status = acceptEx(accept.listener->handle, accept.socket->handle,
                accept.buffer, 0, accept.size, accept.size, &bytes, &task->overlap);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case RnAsyncIOEvent_Connect: {
            RnWin32AsyncIOTaskConnect connect = task->connect;

            rnWin32AsyncIOQueueBindSocketTCP(self, connect.socket);

            DWORD bytes = 0;
            ssize type  = 0;

            RnSockAddrStorage storage = rnSockAddrStorageMake(connect.address, connect.port, &type);

            BOOL status = connectEx(connect.socket->handle,
                ((RnSockAddr*) &storage), type, 0, 0, &bytes, &task->overlap);

            int error = WSAGetLastError();

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case RnAsyncIOEvent_Write: {
            RnWin32AsyncIOTaskWrite write = task->write;

            int status = WSASend(write.socket->handle,
                &write.buffer, 1, 0, write.flags, &task->overlap, 0);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case RnAsyncIOEvent_Read: {
            RnWin32AsyncIOTaskRead read = task->read;

            int status = WSARecv(read.socket->handle,
                &read.buffer, 1, 0, &read.flags, &task->overlap, 0);

            if (status == SOCKET_ERROR && WSAGetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        default: return 0;
    }

    // TODO(gio): Make this a binary tree to speed up search time

    if (self->head == 0 && self->tail == 0) {
        self->head = task;
        self->tail = self->head;
    } else {
        if (self->head == self->tail) {
            self->tail = task;

            self->head->next = self->tail;
            self->tail->prev = self->head;
        } else {
            self->tail->next = task;
            task->prev       = self->tail;

            self->tail = task;
        }
    }

    return 1;
}

b32
rnWin32AsyncIOQueuePoll(RnWin32AsyncIOQueue* self, RnAsyncIOEvent* event, ssize timeout)
{
    DWORD time = timeout >= 0 ? timeout : INFINITE;

    if (self == 0) return 0;

    DWORD       bytes   = 0;
    ULONG_PTR   key     = 0;
    OVERLAPPED* overlap = 0;

    BOOL status = GetQueuedCompletionStatus(self->handle, &bytes, &key, &overlap, time);

    if (status == 0) {
        if (GetLastError() != WAIT_TIMEOUT) {
            if (event != 0)
                *event = rnAsyncIOEventError();

            return 1;
        }

        return 0;
    }

    RnWin32AsyncIOTask* task = rnWin32AsyncIOQueueRemoveTaskByOverlapped(self, overlap);

    if (task == 0) return 0;

    switch (task->kind) {
        case RnAsyncIOEvent_Accept: {
            RnWin32AsyncIOTaskAccept accept = task->accept;

            setsockopt(accept.socket->handle, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
                ((char*) &accept.listener->handle), sizeof(accept.listener->handle));

            RnSockAddrStorage storage = {0};
            INT               size    = accept.size;

            getsockname(accept.socket->handle, ((RnSockAddr*) &storage), &size);

            if (event != 0) {
                accept.socket->storage = storage;

                *event = rnAsyncIOEventAccept(task->ctxt,
                    accept.listener, accept.socket);
            }
        } break;

        case RnAsyncIOEvent_Connect: {
            RnWin32AsyncIOTaskConnect connect = task->connect;

            setsockopt(connect.socket->handle,
                SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, 0, 0);

            DWORD bytes = 0;
            DWORD flags = 0;

            BOOL status = WSAGetOverlappedResult(
                connect.socket->handle, &task->overlap, &bytes, 0, &flags);

            if (event != 0) {
                *event = rnAsyncIOEventConnect(task->ctxt,
                    connect.socket, status != 0 ? 1 : 0);
            }
        } break;

        case RnAsyncIOEvent_Write: {
            RnWin32AsyncIOTaskWrite write = task->write;

            if (event != 0) {
                *event = rnAsyncIOEventWrite(task->ctxt, write.socket,
                    write.values, write.start, write.start + bytes);
            }
        } break;

        case RnAsyncIOEvent_Read: {
            RnWin32AsyncIOTaskRead read = task->read;

            if (event != 0) {
                if (bytes > 0) {
                    *event = rnAsyncIOEventRead(task->ctxt, read.socket,
                        read.values, read.start, read.start + bytes);
                } else
                    *event = rnAsyncIOEventClose(task->ctxt, read.socket);
            }
        } break;

        default: return 0;
    }

    return 1;
}

#endif // RN_WIN32_ASYNCIO_QUEUE_C
