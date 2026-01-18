#ifndef P_SYSTEM_ASYNCIO_QUEUE_H
#define P_SYSTEM_ASYNCIO_QUEUE_H

#include "import.h"

typedef enum PAsyncIoEventKind
{
    PAsyncIoEvent_None,
    PAsyncIoEvent_File,
    PAsyncIoEvent_Tcp,
    PAsyncIoEvent_Udp,
}
PAsyncIoEventKind;

typedef struct PAsyncIoEvent { U8 __private__; } PAsyncIoEvent;
typedef struct PAsyncIoQueue { U8 __private__; } PAsyncIoQueue;

PAsyncIoQueue* pAsyncIoQueueReserve(PMemoryArena* arena);

Bool pAsyncIoQueueCreate(PAsyncIoQueue* self, PMemoryPool pool);

void pAsyncIoQueueDestroy(PAsyncIoQueue* self);

PAsyncIoEventKind pAsyncIoQueuePollEvent(PAsyncIoQueue* self, Int timeout, PMemoryArena* arena, PAsyncIoEvent** event);

#endif // P_SYSTEM_ASYNCIO_QUEUE_H
