#ifndef PX_WIN32_ASYNC_STORAGE_FILE_C
#define PX_WIN32_ASYNC_STORAGE_FILE_C

#include "file.h"

static b32
pxWin32AsyncBindFile(PxWin32Async* async, PxWin32File* file)
{
    HANDLE handle = file->handle;

    if (CreateIoCompletionPort(handle, async->handle, 0, 0) == 0)
        return 0;

    return 1;
}

static b32
pxWin32FileTaskStart(PxWin32AsyncTask* task, PxWin32Async* async)
{
    PxWin32FileTask* self = ((PxWin32FileTask*) task->pntr_body);

    switch (self->kind) {
        case PxFileEvent_Write: {
            PxWin32FileTaskWrite write = self->write;
            DWORD                bytes = 0;

            if (pxWin32AsyncBindFile(async, write.file) == 0) return 0;

            int status = WriteFile(write.file->handle, write.values + write.start,
                write.stop - write.start, &bytes, &task->overlap);

            if (status == 0 && GetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case PxFileEvent_Read: {
            PxWin32FileTaskRead read  = self->read;
            DWORD               bytes = 0;

            if (pxWin32AsyncBindFile(async, read.file) == 0) return 0;

            int status = ReadFile(read.file->handle, read.values + read.start,
                read.stop - read.start, &bytes, &task->overlap);

            if (status == 0 && GetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        default: return 0;
    }

    return 1;
}

static b32
pxWin32FileTaskProc(PxWin32AsyncTask* task, ssize bytes)
{
    PxWin32FileTask* self  = (PxWin32FileTask*) task->pntr_body;
    PxFileEvent*     event = (PxFileEvent*) task->pntr_event;

    switch (self->kind) {
        case PxFileEvent_Write: {
            PxWin32FileTaskWrite write = self->write;

            *event = pxFileEventWrite(write.file,
                write.values, write.start, write.start + bytes);
        } break;

        case PxFileEvent_Read: {
            PxWin32FileTaskRead read = self->read;

            if (bytes > 0) {
                *event = pxFileEventRead(read.file,
                    read.values, read.start, read.start + bytes);
            }
            else *event = pxFileEventClose(read.file);
        } break;

        default: return 0;
    }

    return 1;
}

static PxWin32AsyncTask*
pxWin32AsyncTaskFile(PxMemoryPool* pool, void* tag, ssize size_body, void** pntr_body, ssize size_event, void** pntr_event)
{
    if (pntr_body == 0 || size_body < 0 || size_body > pool->step)    return 0;
    if (pntr_event == 0 || size_event < 0 || size_event > pool->step) return 0;

    PxWin32AsyncTask* result = (PxWin32AsyncTask*) pxMemoryPoolReserve(pool, 1, pool->step);
    void*             body   = pxMemoryPoolReserve(pool, 1, pool->step);
    void*             event  = pxMemoryPoolReserve(pool, 1, pool->step);

    if (result != PX_NULL && body != PX_NULL && event != PX_NULL) {
        result->family       = PxAsyncEventFamily_File;
        result->pntr_body    = body;
        result->pntr_event   = event;
        result->pntr_tag     = tag;
        result->proc         = pxWin32FileTaskProc;
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
pxWin32FileTaskWrite(PxWin32Async* async, void* tag, PxWin32File* file, u8* values, ssize start, ssize stop)
{
    PxWin32FileTask* body  = PX_NULL;
    PxFileEvent*     event = PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskFile(&async->pool, tag,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind         = PxFileEvent_Write;
    body->write.file   = file;
    body->write.values = values;
    body->write.start  = start;
    body->write.stop   = stop;

    if (pxWin32FileTaskStart(result, async) == 0) {
        pxMemoryPoolRelease(&async->pool, result);
        pxMemoryPoolRelease(&async->pool, body);
        pxMemoryPoolRelease(&async->pool, event);

        return PX_NULL;
    }

    return result;
}

static PxWin32AsyncTask*
pxWin32FileTaskRead(PxWin32Async* async, void* tag, PxWin32File* file, u8* values, ssize start, ssize stop)
{
    PxWin32FileTask* body  = PX_NULL;
    PxFileEvent*     event = PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskFile(&async->pool, tag,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind        = PxFileEvent_Read;
    body->read.file   = file;
    body->read.values = values;
    body->read.start  = start;
    body->read.stop   = stop;

    if (pxWin32FileTaskStart(result, async) == 0) {
        pxMemoryPoolRelease(&async->pool, result);
        pxMemoryPoolRelease(&async->pool, body);
        pxMemoryPoolRelease(&async->pool, event);

        return PX_NULL;
    }

    return result;
}

b32
pxWin32FileWriteAsync(PxWin32Async* async, void* tag, PxWin32File* self, u8* values, ssize start, ssize stop)
{
    PxWin32AsyncTask* task = pxWin32FileTaskWrite(
        async, tag, self, values, start, stop);

    if (task != PX_NULL) return pxAsyncSubmit(async, task);

    return 0;
}

b32
pxWin32FileReadAsync(PxWin32Async* async, void* tag, PxWin32File* self, u8* values, ssize start, ssize stop)
{
    PxWin32AsyncTask* task = pxWin32FileTaskRead(
        async, tag, self, values, start, stop);

    if (task != PX_NULL) return pxAsyncSubmit(async, task);

    return 0;
}

#endif // PX_WIN32_ASYNC_STORAGE_FILE_C
