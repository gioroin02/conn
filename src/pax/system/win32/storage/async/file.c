#ifndef PX_WIN32_ASYNC_STORAGE_FILE_C
#define PX_WIN32_ASYNC_STORAGE_FILE_C

#include "file.h"

static b32 pxWin32AsyncBindFile(PxWin32Async* async, PxWin32File* file)
{
    HANDLE handle = file->handle;
    HANDLE result = CreateIoCompletionPort(handle, async->handle, 0, 0);

    return result != PX_NULL ? 1 : 0;
}

static b32 pxWin32FileTaskStart(PxWin32AsyncTask* task, PxWin32Async* async)
{
    PxWin32FileTask* body = ((PxWin32FileTask*) task->pntr_body);

    switch (body->kind) {
        case PxFileEvent_Write: {
            PxWin32File* self  = body->self;
            u8*          pntr  = body->write.pntr;
            ssize        start = body->write.start;
            ssize        stop  = body->write.stop;
            DWORD        bytes = 0;

            pxWin32AsyncBindFile(async, self);

            char* memory = ((char*) pntr + start);
            ssize size   = stop - start;

            int status = WriteFile(self->handle, memory, size,
                &bytes, &task->overlap);

            if (status == 0 && GetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        case PxFileEvent_Read: {
            PxWin32File* self   = body->self;
            u8*          pntr   = body->read.pntr;
            ssize        start  = body->read.start;
            ssize        stop   = body->read.stop;
            DWORD        bytes = 0;

            pxWin32AsyncBindFile(async, self);

            char* memory = ((char*) pntr + start);
            ssize size   = stop - start;

            int status = ReadFile(self->handle, memory, size,
                &bytes, &task->overlap);

            if (status == 0 && GetLastError() != ERROR_IO_PENDING)
                return 0;
        } break;

        default: return 0;
    }

    return 1;
}

static b32 pxWin32FileTaskProc(PxWin32AsyncTask* task, ssize bytes)
{
    PxWin32FileTask* body  = (PxWin32FileTask*) task->pntr_body;
    PxFileEvent*     event = (PxFileEvent*) task->pntr_event;

    switch (body->kind) {
        case PxFileEvent_Write: {
            void*        ctxt  = body->ctxt;
            PxWin32File* self  = body->self;
            u8*          pntr  = body->write.pntr;
            ssize        start = body->write.start;
            ssize        stop  = start + bytes;

            *event = pxFileEventWrite(ctxt, self, pntr, start, stop);
        } break;

        case PxFileEvent_Read: {
            void*        ctxt  = body->ctxt;
            PxWin32File* self  = body->self;
            u8*          pntr  = body->read.pntr;
            ssize        start = body->read.start;
            ssize        stop  = start + bytes;

            *event = pxFileEventClose(ctxt, self);

            if (bytes > 0)
                *event = pxFileEventRead(ctxt, self, pntr, start, stop);
        } break;

        default: return 0;
    }

    return 1;
}

static PxWin32AsyncTask* pxWin32AsyncTaskFile(PxMemoryPool* pool,
    ssize size_body, void** pntr_body, ssize size_event, void** pntr_event)
{
    if (pntr_body == 0 || size_body < 0 || size_body > pool->step)    return 0;
    if (pntr_event == 0 || size_event < 0 || size_event > pool->step) return 0;

    PxWin32AsyncTask* result = (PxWin32AsyncTask*) pxMemoryPoolReserve(pool, 1, pool->step);
    void*             body   = pxMemoryPoolReserve(pool, 1, pool->step);
    void*             event  = pxMemoryPoolReserve(pool, 1, pool->step);

    if (result != PX_NULL && body != PX_NULL && event != PX_NULL) {
        result->family     = PxAsyncEventFamily_File;
        result->pntr_body  = body;
        result->pntr_event = event;
        result->size_body  = size_body;
        result->size_event = size_event;
        result->proc       = pxWin32FileTaskProc;
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

static PxWin32AsyncTask* pxWin32FileTaskWrite(PxWin32Async* async, void* ctxt, 
    PxWin32File* self, u8* pntr, ssize start, ssize stop)
{
    PxWin32FileTask* body  = PX_NULL;
    PxFileEvent*     event = PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskFile(&async->pool,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind        = PxFileEvent_Write;
    body->ctxt        = ctxt;
    body->self        = self;
    body->write.pntr  = pntr;
    body->write.start = start;
    body->write.stop  = stop;

    if (pxWin32FileTaskStart(result, async) == 0) {
        pxMemoryPoolRelease(&async->pool, result);
        pxMemoryPoolRelease(&async->pool, body);
        pxMemoryPoolRelease(&async->pool, event);

        return PX_NULL;
    }

    return result;
}

static PxWin32AsyncTask* pxWin32FileTaskRead(PxWin32Async* async, void* ctxt,
    PxWin32File* self, u8* pntr, ssize start, ssize stop)
{
    PxWin32FileTask* body  = PX_NULL;
    PxFileEvent*     event = PX_NULL;

    PxWin32AsyncTask* result = pxWin32AsyncTaskFile(&async->pool,
        sizeof *body, (void**) &body, sizeof *event, (void**) &event);

    if (result == PX_NULL) return PX_NULL;

    body->kind       = PxFileEvent_Read;
    body->ctxt       = ctxt;
    body->self       = self;
    body->read.pntr  = pntr;
    body->read.start = start;
    body->read.stop  = stop;

    if (pxWin32FileTaskStart(result, async) == 0) {
        pxMemoryPoolRelease(&async->pool, result);
        pxMemoryPoolRelease(&async->pool, body);
        pxMemoryPoolRelease(&async->pool, event);

        return PX_NULL;
    }

    return result;
}

b32 pxWin32FileWriteAsync(PxWin32Async* async, void* ctxt,
    PxWin32File* self, u8* pntr, ssize start, ssize stop)
{
    PxWin32AsyncTask* task =
        pxWin32FileTaskWrite(async, ctxt, self, pntr, start, stop);

    return pxAsyncSubmit(async, task);
}

b32 pxWin32FileReadAsync(PxWin32Async* async, void* ctxt,
    PxWin32File* self, u8* pntr, ssize start, ssize stop)
{
    PxWin32AsyncTask* task =
        pxWin32FileTaskRead(async, ctxt, self, pntr, start, stop);

    return pxAsyncSubmit(async, task);
}

#endif // PX_WIN32_ASYNC_STORAGE_FILE_C
