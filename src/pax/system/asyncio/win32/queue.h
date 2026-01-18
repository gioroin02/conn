#ifndef P_SYSTEM_WIN32_ASYNCIO_QUEUE_H
#define P_SYSTEM_WIN32_ASYNCIO_QUEUE_H

#include "import.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI

#include <windows.h>

// TODO(Gio): OBJECT POOL. In the future replace the linked list and the memory pool with an array of blocks of constant size,
// the array would have an implicit free list to get a new block in constant time but still access active ones by index.
// This removes the need for an explicity memory pool (and 16 bytes of header per node) and a linear time search through the list.

#define __PWin32AsyncIoTaskTag__ struct { \
    OVERLAPPED overlap;                   \
    void*      callback;                  \
                                          \
    PWin32AsyncIoTask* list_next;         \
}

typedef struct PWin32AsyncIoTask
{
    OVERLAPPED overlap;
    void*      callback;

    struct PWin32AsyncIoTask* list_next;
}
PWin32AsyncIoTask;

typedef struct PWin32AsyncIoQueue
{
    HANDLE handle;

    PMemoryPool pool;

    PWin32AsyncIoTask* list_front;
    PWin32AsyncIoTask* list_back;
}
PWin32AsyncIoQueue;

typedef PAsyncIoEventKind (PWin32AsyncIoProc) (void*, Int, PMemoryArena*, PAsyncIoEvent** event);

PWin32AsyncIoQueue* pWin32AsyncIoQueueReserve(PMemoryArena* arena);

Bool pWin32AsyncIoQueueCreate(PWin32AsyncIoQueue* self, PMemoryPool pool);

void pWin32AsyncIoQueueDestroy(PWin32AsyncIoQueue* self);

PAsyncIoEventKind pWin32AsyncIoQueuePollEvent(PWin32AsyncIoQueue* self, Int timeout, PMemoryArena* arena, PAsyncIoEvent** event);

Bool pWin32AsyncIoQueueSubmit(PWin32AsyncIoQueue* self, PWin32AsyncIoTask* value);

#endif // P_SYSTEM_WIN32_ASYNCIO_QUEUE_H
