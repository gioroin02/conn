#ifndef PX_SYSTEM_ASYNC_ASYNC_H
#define PX_SYSTEM_ASYNC_ASYNC_H

#include "import.h"

typedef enum PxAsyncEventFamily
{
    PxAsyncEventFamily_None,
    PxAsyncEventFamily_Error,
    PxAsyncEventFamily_Tcp,
    PxAsyncEventFamily_Udp,
    PxAsyncEventFamily_File,
    PxAsyncEventFamily_Dir,
    PxAsyncEventFamily_Pipe,
}
PxAsyncEventFamily;

typedef void PxAsync;
typedef void PxAsyncTask;
typedef void PxAsyncEvent;

PxAsync* pxAsyncReserve(PxMemoryArena* arena);

b32 pxAsyncCreate(PxAsync* self, PxMemoryArena* arena, ssize size);

void pxAsyncDestroy(PxAsync* self);

b32 pxAsyncSubmit(PxAsync* self, PxAsyncTask* task);

PxAsyncEventFamily pxAsyncPoll(PxAsync* self, ssize timeout, PxAsyncEvent* event, ssize size);

#endif // PX_SYSTEM_ASYNC_ASYNC_H
