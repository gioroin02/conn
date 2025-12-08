#ifndef RN_WIN32_ASYNCIO_QUEUE_H
#define RN_WIN32_ASYNCIO_QUEUE_H

#include "./task.h"

typedef struct RnWin32AsyncIOQueue
{
    HANDLE handle;

    RnWin32AsyncIOTask* head;
    RnWin32AsyncIOTask* tail;
}
RnWin32AsyncIOQueue;

RnWin32AsyncIOQueue*
rnWin32AsyncIOQueueReserve(RnMemoryArena* arena);

b32
rnWin32AsyncIOQueueCreate(RnWin32AsyncIOQueue* self);

void
rnWin32AsyncIOQueueDestroy(RnWin32AsyncIOQueue* self);

b32
rnWin32AsyncIOQueueSubmit(RnWin32AsyncIOQueue* self, RnWin32AsyncIOTask* task);

b32
rnWin32AsyncIOQueuePoll(RnWin32AsyncIOQueue* self, RnAsyncIOEvent* event, ssize timeout);

#endif // RN_WIN32_ASYNCIO_QUEUE_H
