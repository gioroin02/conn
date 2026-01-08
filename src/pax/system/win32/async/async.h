#ifndef PX_WIN32_ASYNC_ASYNC_H
#define PX_WIN32_ASYNC_ASYNC_H

#include "import.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

typedef struct PxWin32AsyncTask PxWin32AsyncTask;
typedef void                    PxWin32AsyncEvent;

struct PxWin32AsyncTask
{
    PxAsyncEventFamily family;
    OVERLAPPED         overlap;
    void*              pntr_body;
    void*              pntr_event;
    ssize              size_body;
    ssize              size_event;
    void*              proc;
    PxWin32AsyncTask*  list_next;
};

typedef struct PxWin32Async
{
    HANDLE handle;

    PxMemoryPool pool;

    PxWin32AsyncTask* list_front;
    PxWin32AsyncTask* list_back;
}
PxWin32Async;

typedef b32 (PxWin32AsyncTaskProc) (PxWin32AsyncTask*, ssize);

PxWin32Async* pxWin32AsyncReserve(PxMemoryArena* arena);

b32 pxWin32AsyncCreate(PxWin32Async* self, PxMemoryArena* arena, ssize size);

void pxWin32AsyncDestroy(PxWin32Async* self);

b32 pxWin32AsyncSubmit(PxWin32Async* self, PxWin32AsyncTask* task);

PxAsyncEventFamily pxWin32AsyncPoll(PxWin32Async* self, ssize timeout, PxAsyncEvent* event, ssize size);

#endif // PX_WIN32_ASYNC_ASYNC_H
