#ifndef PX_SYSTEM_ASYNC_COMMON_C
#define PX_SYSTEM_ASYNC_COMMON_C

#include "common.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../../win32/async/export.c"

    #define __pxAsyncReserve__ pxWin32AsyncReserve
    #define __pxAsyncCreate__  pxWin32AsyncCreate
    #define __pxAsyncDestroy__ pxWin32AsyncDestroy
    #define __pxAsyncSubmit__  pxWin32AsyncSubmit
    #define __pxAsyncPoll__    pxWin32AsyncPoll
    #define __pxAsyncReturn__  pxWin32AsyncReturn

#else

    #error "Unknown platform"

#endif

PxAsync*
pxAsyncReserve(PxMemoryArena* arena)
{
    return __pxAsyncReserve__(arena);
}

b32
pxAsyncCreate(PxAsync* self, PxMemoryArena* arena, ssize size)
{
    return __pxAsyncCreate__(self, arena, size);
}

void
pxAsyncDestroy(PxAsync* self)
{
    return __pxAsyncDestroy__(self);
}

b32
pxAsyncSubmit(PxAsync* self, PxAsyncTask* task)
{
    return __pxAsyncSubmit__(self, task);
}

PxAsyncEventFamily
pxAsyncPoll(PxAsync* self, void** tag, void** event, ssize timeout)
{
    return __pxAsyncPoll__(self, tag, event, timeout);
}

b32
pxAsyncReturn(PxAsync* self, void* event, void* pntr, ssize size)
{
    return __pxAsyncReturn__(self, event, pntr, size);
}

#endif // PX_SYSTEM_ASYNC_COMMON_C
