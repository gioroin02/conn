#ifndef PX_SYSTEM_ASYNC_COMMON_H
#define PX_SYSTEM_ASYNC_COMMON_H

#include "import.h"

typedef enum PxAsyncEventFamily
{
    PxAsyncEventFamily_None,
    PxAsyncEventFamily_Tcp,
    PxAsyncEventFamily_Udp,
    PxAsyncEventFamily_File,
    PxAsyncEventFamily_Dir,
    PxAsyncEventFamily_Term,
}
PxAsyncEventFamily;

typedef void PxAsync;
typedef void PxAsyncTask;

PxAsync*
pxAsyncReserve(PxMemoryArena* arena);

b32
pxAsyncCreate(PxAsync* self, PxMemoryArena* arena, ssize size);

void
pxAsyncDestroy(PxAsync* self);

b32
pxAsyncSubmit(PxAsync* self, PxAsyncTask* task);

PxAsyncEventFamily
pxAsyncPoll(PxAsync* self, void** tag, void** event, ssize timeout);

b32
pxAsyncReturn(PxAsync* self, void* event, void* pntr, ssize size);

#endif // PX_SYSTEM_ASYNC_COMMON_H
