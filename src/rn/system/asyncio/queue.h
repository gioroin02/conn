#ifndef RN_SYSTEM_ASYNCIO_QUEUE_H
#define RN_SYSTEM_ASYNCIO_QUEUE_H

#include "./event.h"
#include "./task.h"

typedef void RnAsyncIOQueue;

RnAsyncIOQueue*
rnAsyncIOQueueReserve(RnMemoryArena* arena);

b32
rnAsyncIOQueueCreate(RnAsyncIOQueue* self);

void
rnAsyncIOQueueDestroy(RnAsyncIOQueue* self);

b32
rnAsyncIOQueueSubmit(RnAsyncIOQueue* self, RnAsyncIOTask* task);

b32
rnAsyncIOQueuePoll(RnAsyncIOQueue* self, RnAsyncIOEvent* event, ssize timeout);

#endif // RN_SYSTEM_ASYNCIO_QUEUE_H
