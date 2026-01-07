#ifndef PX_WIN32_ASYNC_COMMON_C
#define PX_WIN32_ASYNC_COMMON_C

#include "common.h"

static b32
pxWin32AsyncTaskProc(PxWin32AsyncTask* self, ssize bytes)
{
    return ((PxWin32AsyncTaskProc*) self->proc)(self, bytes);
}

void
pxWin32AsyncInsertBack(PxWin32Async* self, PxWin32AsyncTask* value)
{
    if (self->pending_front != PX_NULL)
        self->pending_back->pending_next = value;
    else
        self->pending_front = value;

    self->pending_back = value;
}

PxWin32AsyncTask*
pxWin32AsyncRemoveFront(PxWin32Async* self)
{
    PxWin32AsyncTask* result = self->pending_front;

    if (result == PX_NULL) return result;

    self->pending_front = result->pending_next;

    if (self->pending_front == PX_NULL)
        self->pending_back = PX_NULL;

    return result;
}

PxWin32AsyncTask*
pxWin32AsyncRemove(PxWin32Async* self, PxWin32AsyncTask* prev)
{
    if (prev == PX_NULL) return pxWin32AsyncRemoveFront(self);

    PxWin32AsyncTask* result = prev->pending_next;

    if (result == PX_NULL) return PX_NULL;

    prev->pending_next = result->pending_next;

    return result;
}

PxWin32AsyncTask*
pxWin32AsyncRemoveByOverlapped(PxWin32Async* self, OVERLAPPED* overlap)
{
    PxWin32AsyncTask* prev = PX_NULL;
    PxWin32AsyncTask* task = self->pending_front;

    while (task != 0) {
        if (&task->overlap == overlap)
            return pxWin32AsyncRemove(self, prev);

        prev = task;
        task = task->pending_next;
    }

    return PX_NULL;
}

PxWin32Async*
pxWin32AsyncReserve(PxMemoryArena* arena)
{
    return pxMemoryArenaReserveOneOf(arena, PxWin32Async);
}

b32
pxWin32AsyncCreate(PxWin32Async* self, PxMemoryArena* arena, ssize size)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    HANDLE handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

    if (handle == 0) return 0;

    void* memory = pxMemoryArenaReserve(arena, size / 384, 384);

    if (memory != 0) {
        self->handle        = handle;
        self->pool          = pxMemoryPoolMake(memory, (size / 384) * 384, 384);
        self->pending_front = PX_NULL;
        self->pending_back  = PX_NULL;

        return 1;
    }

    CloseHandle(handle);

    return 0;
}

void
pxWin32AsyncDestroy(PxWin32Async* self)
{
    if (self->handle != 0)
        CloseHandle(self->handle);

    pxMemorySet(self, sizeof *self, 0xAB);
}

b32
pxWin32AsyncSubmit(PxWin32Async* self, PxWin32AsyncTask* task)
{
    if (task == PX_NULL) return 0;

    pxWin32AsyncInsertBack(self, task);

    return 1;
}

PxAsyncEventFamily
pxWin32AsyncPoll(PxWin32Async* self, void** tag, void** event, ssize timeout)
{
    DWORD       time    = timeout >= 0 ? timeout : INFINITE;
    DWORD       bytes   = 0;
    ULONG_PTR   key     = 0;
    OVERLAPPED* overlap = PX_NULL;

    BOOL status = GetQueuedCompletionStatus(self->handle, &bytes, &key, &overlap, time);

    if (status == 0 && GetLastError() != WAIT_TIMEOUT)
        return PxAsyncEventFamily_None;

    if (status != 0) {
        PxWin32AsyncTask*  temp   = pxWin32AsyncRemoveByOverlapped(self, overlap);
        PxAsyncEventFamily family = PxAsyncEventFamily_None;;

        if (temp != PX_NULL) {
            if (pxWin32AsyncTaskProc(temp, bytes) != 0) {
                family = temp->family;

                if (event != PX_NULL) *event = temp->pntr_event;
                if (tag != PX_NULL)   *tag   = temp->pntr_tag;
            }
            else pxMemoryPoolRelease(&self->pool, temp->pntr_event);

            pxMemoryPoolRelease(&self->pool, temp->pntr_body);
            pxMemoryPoolRelease(&self->pool, temp);
        }

        return family;
    }

    return PxAsyncEventFamily_None;
}

b32
pxWin32AsyncReturn(PxWin32Async* self, void* event)
{
    return pxMemoryPoolRelease(&self->pool, event);
}

#endif // PX_WIN32_ASYNC_COMMON_C
