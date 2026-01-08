#ifndef PX_SYSTEM_ASYNC_ASYNC_C
#define PX_SYSTEM_ASYNC_ASYNC_C

#include "async.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../win32/async/export.c"

    #define __pxAsyncReserve__ pxWin32AsyncReserve
    #define __pxAsyncCreate__  pxWin32AsyncCreate
    #define __pxAsyncDestroy__ pxWin32AsyncDestroy
    #define __pxAsyncSubmit__  pxWin32AsyncSubmit
    #define __pxAsyncPoll__    pxWin32AsyncPoll

#else

    #error "Unknown platform"

#endif

PxAsync* pxAsyncReserve(PxMemoryArena* arena)
{
    return __pxAsyncReserve__(arena);
}

b32 pxAsyncCreate(PxAsync* self, PxMemoryArena* arena, ssize size)
{
    return __pxAsyncCreate__(self, arena, size);
}

void pxAsyncDestroy(PxAsync* self)
{
    return __pxAsyncDestroy__(self);
}

b32 pxAsyncSubmit(PxAsync* self, PxAsyncTask* task)
{
    return __pxAsyncSubmit__(self, task);
}

PxAsyncEventFamily pxAsyncPoll(PxAsync* self, ssize timeout, PxAsyncEvent* event, ssize size)
{
    return __pxAsyncPoll__(self, timeout, event, size);
}

#endif // PX_SYSTEM_ASYNC_ASYNC_C
