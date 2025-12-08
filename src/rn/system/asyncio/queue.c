#ifndef RN_SYSTEM_ASYNCIO_QUEUE_C
#define RN_SYSTEM_ASYNCIO_QUEUE_C

#include "./queue.h"

#if RN_SYSTEM == RN_SYSTEM_WINDOWS

    #include "../../win32/asyncio/export.c"

    #define __rnAsyncIOQueueReserve__ rnWin32AsyncIOQueueReserve
    #define __rnAsyncIOQueueCreate__  rnWin32AsyncIOQueueCreate
    #define __rnAsyncIOQueueDestroy__ rnWin32AsyncIOQueueDestroy
    #define __rnAsyncIOQueueSubmit__  rnWin32AsyncIOQueueSubmit
    #define __rnAsyncIOQueuePoll__    rnWin32AsyncIOQueuePoll

#else

    #error "Unknown platform"

#endif

RnAsyncIOQueue*
rnAsyncIOQueueReserve(RnMemoryArena* arena)
{
    return __rnAsyncIOQueueReserve__(arena);
}

b32
rnAsyncIOQueueCreate(RnAsyncIOQueue* self)
{
    return __rnAsyncIOQueueCreate__(self);
}

void
rnAsyncIOQueueDestroy(RnAsyncIOQueue* self)
{
    return __rnAsyncIOQueueDestroy__(self);
}

b32
rnAsyncIOQueueSubmit(RnAsyncIOQueue* self, RnAsyncIOTask* task)
{
    return __rnAsyncIOQueueSubmit__(self, task);
}

b32
rnAsyncIOQueuePoll(RnAsyncIOQueue* self, RnAsyncIOEvent* event, ssize timeout)
{
    return __rnAsyncIOQueuePoll__(self, event, timeout);
}

#endif // RN_SYSTEM_ASYNCIO_QUEUE_C
