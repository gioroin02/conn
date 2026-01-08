#ifndef PX_WIN32_ASYNC_ASYNC_C
#define PX_WIN32_ASYNC_ASYNC_C

#include "async.h"

static b32 pxWin32AsyncTaskProc(PxWin32AsyncTask* self, ssize bytes)
{
    return ((PxWin32AsyncTaskProc*) self->proc)(self, bytes);
}

static b32 pxWin32AsyncInsertBack(PxWin32Async* self, PxWin32AsyncTask* value)
{
    if (self->list_front != PX_NULL)
        self->list_back->list_next = value;
    else
        self->list_front = value;

    self->list_back = value;

    return 1;
}

static PxWin32AsyncTask* pxWin32AsyncRemoveFront(PxWin32Async* self)
{
    PxWin32AsyncTask* result = self->list_front;

    if (result == PX_NULL) return result;

    self->list_front = result->list_next;

    if (self->list_front == PX_NULL)
        self->list_back = PX_NULL;

    return result;
}

static PxWin32AsyncTask* pxWin32AsyncRemove(PxWin32Async* self, PxWin32AsyncTask* prev)
{
    if (prev == PX_NULL) return pxWin32AsyncRemoveFront(self);

    PxWin32AsyncTask* result = prev->list_next;

    if (result == PX_NULL) return PX_NULL;

    prev->list_next = result->list_next;

    return result;
}

static PxWin32AsyncTask* pxWin32AsyncRemoveByOverlapped(PxWin32Async* self, OVERLAPPED* overlap)
{
    PxWin32AsyncTask* prev = PX_NULL;
    PxWin32AsyncTask* task = self->list_front;

    while (task != 0) {
        if (&task->overlap == overlap)
            return pxWin32AsyncRemove(self, prev);

        prev = task;
        task = task->list_next;
    }

    return PX_NULL;
}

PxWin32Async* pxWin32AsyncReserve(PxMemoryArena* arena)
{
    return pxMemoryArenaReserveOneOf(arena, PxWin32Async);
}

b32 pxWin32AsyncCreate(PxWin32Async* self, PxMemoryArena* arena, ssize size)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    HANDLE handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    if (handle == PX_NULL) return 0;

    void* memory = pxMemoryArenaReserve(arena, size / 300, 300);

    if (memory != PX_NULL) {
        self->handle     = handle;
        self->pool       = pxMemoryPoolMake(memory, (size / 300) * 300, 300);
        self->list_front = PX_NULL;
        self->list_back  = PX_NULL;

        return 1;
    }

    CloseHandle(handle);

    return 0;
}

void pxWin32AsyncDestroy(PxWin32Async* self)
{
    if (self->handle != PX_NULL)
        CloseHandle(self->handle);

    pxMemorySet(self, sizeof *self, 0xAB);
}

b32 pxWin32AsyncSubmit(PxWin32Async* self, PxWin32AsyncTask* task)
{
    if (task != PX_NULL)
        return pxWin32AsyncInsertBack(self, task);

    return 0;
}

PxAsyncEventFamily pxWin32AsyncPoll(PxWin32Async* self, ssize timeout, PxWin32AsyncEvent* event, ssize size)
{
    PxAsyncEventFamily family = PxAsyncEventFamily_None;

    DWORD       time    = timeout >= 0 ? timeout : INFINITE;
    DWORD       bytes   = 0;
    ULONG_PTR   key     = 0;
    OVERLAPPED* overlap = PX_NULL;

    BOOL status = GetQueuedCompletionStatus(self->handle, &bytes, &key, &overlap, time);

    if (status == 0 && GetLastError() != WAIT_TIMEOUT)
        return PxAsyncEventFamily_Error;

    if (status != 0) {
        PxWin32AsyncTask* task = pxWin32AsyncRemoveByOverlapped(self, overlap);

        if (task == PX_NULL) return family;

        if (pxWin32AsyncTaskProc(task, bytes) != 0) {
            family = task->family;

            if (event != PX_NULL && task->size_event <= size)
                pxMemoryCopy(event, task->size_event, task->pntr_event);
        }

        pxMemoryPoolRelease(&self->pool, task);
        pxMemoryPoolRelease(&self->pool, task->pntr_body);
        pxMemoryPoolRelease(&self->pool, task->pntr_event);
    }

    return family;
}

#endif // PX_WIN32_ASYNC_ASYNC_C
