#ifndef PX_BASE_MEMORY_ARENA_C
#define PX_BASE_MEMORY_ARENA_C

#include "arena.h"

PxMemoryArena
pxMemoryArenaMake(void* pntr, ssize size)
{
    PxMemoryArena result;

    pxMemorySet(&result, sizeof result, 0xAB);

    if (pntr == PX_NULL || size <= 0) return result;

    result.pntr_base = (u8*) pntr;
    result.pntr_next = result.pntr_base,
    result.size      = size;

    pxMemorySet(result.pntr_base, result.size, 0xAB);

    return result;
}

void*
pxMemoryArenaPntr(PxMemoryArena* self)
{
    return self->pntr_base;
}

ssize
pxMemoryArenaSize(PxMemoryArena* self)
{
    return self->size;
}

void
pxMemoryArenaClear(PxMemoryArena* self)
{
    pxMemorySet(self->pntr_base, self->size, 0xAB);

    self->pntr_next = self->pntr_base;
}

void*
pxMemoryArenaReserve(PxMemoryArena* self, ssize count, ssize size)
{
    ssize bytes  = count * size;
    u8*   result = self->pntr_next;
    u8*   next   = self->pntr_next + bytes;

    if (count <= 0 || size <= 0 || count > PX_MAX_SSIZE / size)
        return PX_NULL;

    if (next < self->pntr_base || next > self->pntr_base + self->size)
        return PX_NULL;

    self->pntr_next = pxMemoryAlignPntrForw(next,
        PX_MEMORY_DEFAULT_ALIGNMENT);

    pxMemorySet(result, self->pntr_next - result, 0xAB);

    return result;
}

b32
pxMemoryArenaRelease(PxMemoryArena* self, void* pntr)
{
    if (pntr == PX_NULL) return 0;

    u8*   head = ((u8*) pntr);
    ssize dist = head - self->pntr_base;

    if (head < self->pntr_base || head >= self->pntr_next)
        return 0;

    if (dist % PX_MEMORY_DEFAULT_ALIGNMENT != 0) return 0;

    self->pntr_next = self->pntr_base + dist;

    pxMemorySet(self->pntr_base + dist, self->size - dist, 0xAB);

    return 1;
}

void*
pxMemoryArenaTell(PxMemoryArena* self)
{
    return self->pntr_next;
}

#endif // PX_BASE_MEMORY_ARENA_C
