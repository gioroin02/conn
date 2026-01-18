#ifndef P_SYSTEM_ASYNCIO_QUEUE_C
#define P_SYSTEM_ASYNCIO_QUEUE_C

#include "queue.h"

#if P_SYSTEM == P_SYSTEM_WINDOWS

    #include "win32/export.c"

    #define __PAsyncIoQueue__ PWin32AsyncIoQueue

    #define __pAsyncIoQueueReserve__   pWin32AsyncIoQueueReserve
    #define __pAsyncIoQueueCreate__    pWin32AsyncIoQueueCreate
    #define __pAsyncIoQueueDestroy__   pWin32AsyncIoQueueDestroy
    #define __pAsyncIoQueuePollEvent__ pWin32AsyncIoQueuePollEvent

#else

    #error "Unknown platform"

#endif

PAsyncIoQueue* pAsyncIoQueueReserve(PMemoryArena* arena)
{
    return (PAsyncIoQueue*) __pAsyncIoQueueReserve__(arena);
}

Bool pAsyncIoQueueCreate(PAsyncIoQueue* self, PMemoryPool pool)
{
    return __pAsyncIoQueueCreate__((__PAsyncIoQueue__*) self, pool);
}

void pAsyncIoQueueDestroy(PAsyncIoQueue* self)
{
    return __pAsyncIoQueueDestroy__((__PAsyncIoQueue__*) self);
}

PAsyncIoEventKind pAsyncIoQueuePollEvent(PAsyncIoQueue* self, Int timeout, PMemoryArena* arena, PAsyncIoEvent** event)
{
    return __pAsyncIoQueuePollEvent__((__PAsyncIoQueue__*) self, timeout, arena, event);
}

#endif // P_SYSTEM_ASYNCIO_QUEUE_C
