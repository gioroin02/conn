#ifndef P_BASE_MEMORY_ARENA_H
#define P_BASE_MEMORY_ARENA_H

#include "common.h"

#define pMemoryArenaReserveManyOf(self, type, count) \
    ((type*) pMemoryArenaReserve(self, count, sizeof (type)))

#define pMemoryArenaReserveOneOf(self, type) \
    ((type*) pMemoryArenaReserve(self, 1, sizeof (type)))

typedef struct PMemoryArena
{
    U8* pntr_base;
    U8* pntr_next;
    Int size;
}
PMemoryArena;

PMemoryArena pMemoryArenaMake(void* pntr, Int size);

void* pMemoryArenaPntr(PMemoryArena* self);

Int pMemoryArenaSize(PMemoryArena* self);

void pMemoryArenaClear(PMemoryArena* self);

void* pMemoryArenaReserve(PMemoryArena* self, Int count, Int size);

Bool pMemoryArenaRelease(PMemoryArena* self, void* pntr);

Bool pMemoryArenaRewind(PMemoryArena* self, void* pntr);

void* pMemoryArenaTell(PMemoryArena* self);

#endif // P_BASE_MEMORY_ARENA_H
