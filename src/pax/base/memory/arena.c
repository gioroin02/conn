#ifndef P_BASE_MEMORY_ARENA_C
#define P_BASE_MEMORY_ARENA_C

#include "arena.h"

PMemoryArena pMemoryArenaMake(void* pntr, Int size)
{
    PMemoryArena result;

    pMemorySet(&result, sizeof result, 0xAB);

    if (pntr == NULL || size <= 0) return result;

    result.pntr_base = (U8*) pntr;
    result.pntr_next = result.pntr_base,
    result.size      = size;

    pMemorySet(result.pntr_base, result.size, 0xAB);

    return result;
}

void* pMemoryArenaPntr(PMemoryArena* self)
{
    return self->pntr_base;
}

Int pMemoryArenaSize(PMemoryArena* self)
{
    return self->size;
}

void pMemoryArenaClear(PMemoryArena* self)
{
    pMemorySet(self->pntr_base, self->size, 0xAB);

    self->pntr_next = self->pntr_base;
}

void* pMemoryArenaReserve(PMemoryArena* self, Int count, Int size)
{
    if (count <= 0 || size <= 0 || count > P_INT_MAX / size)
        return NULL;

    Int bytes  = count * size;
    U8* result = self->pntr_next;
    U8* next   = self->pntr_next + bytes;

    if (next < self->pntr_base || next > self->pntr_base + self->size)
        return NULL;

    self->pntr_next = pMemoryAlignPntr(next, P_MEMORY_DEFAULT_ALIGNMENT);

    pMemorySet(result, self->pntr_next - result, 0xAB);

    return result;
}

Bool pMemoryArenaRelease(PMemoryArena* arena, void* pntr)
{
    return 0;
}

Bool pMemoryArenaRewind(PMemoryArena* self, void* pntr)
{
    U8* head = ((U8*) pntr);

    if (pntr == NULL || head < self->pntr_base || head >= self->pntr_next)
        return 0;

    Int dist = head - self->pntr_base;

    if (dist % P_MEMORY_DEFAULT_ALIGNMENT != 0) return 0;

    self->pntr_next = self->pntr_base + dist;

    pMemorySet(self->pntr_base + dist, self->size - dist, 0xAB);

    return 1;
}

void* pMemoryArenaTell(PMemoryArena* self)
{
    return self->pntr_next;
}

#endif // P_BASE_MEMORY_ARENA_C
