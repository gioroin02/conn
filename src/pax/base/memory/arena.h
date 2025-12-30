#ifndef PX_BASE_MEMORY_ARENA_H
#define PX_BASE_MEMORY_ARENA_H

#include "common.h"

#define pxMemoryArenaReserveOneOf(self, type) \
    ((type*) pxMemoryArenaReserve(self, 1, sizeof (type)))

#define pxMemoryArenaReserveManyOf(self, type, count) \
    ((type*) pxMemoryArenaReserve(self, count, sizeof (type)))

typedef struct PxMemoryArena
{
    u8*   pntr_base;
    u8*   pntr_next;
    ssize size;
}
PxMemoryArena;

PxMemoryArena
pxMemoryArenaMake(void* pntr, ssize size);

void*
pxMemoryArenaPntr(PxMemoryArena* self);

ssize
pxMemoryArenaSize(PxMemoryArena* self);

void
pxMemoryArenaClear(PxMemoryArena* self);

void*
pxMemoryArenaReserve(PxMemoryArena* self, ssize count, ssize size);

b32
pxMemoryArenaRelease(PxMemoryArena* self, void* pntr);

void*
pxMemoryArenaTell(PxMemoryArena* self);

#endif // PX_BASE_MEMORY_ARENA_H
