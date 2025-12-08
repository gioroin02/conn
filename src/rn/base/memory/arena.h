#ifndef RN_BASE_MEMORY_ARENA_H
#define RN_BASE_MEMORY_ARENA_H

#include "./slice.h"

#define rnMemoryArenaReserveOneOf(self, type) \
    ((type*) rnMemoryArenaReserve(self, 1, sizeof(type), 0))

#define rnMemoryArenaReserveManyOf(self, type, count) \
    ((type*) rnMemoryArenaReserve(self, count, sizeof(type), 0))

typedef struct RnMemoryArena
{
    u8*   values;
    ssize size;
    ssize count;
}
RnMemoryArena;

RnMemoryArena
rnMemoryArenaMake(void* memory, ssize size);

void
rnMemoryArenaClear(RnMemoryArena* self);

void*
rnMemoryArenaTell(RnMemoryArena* self);

void*
rnMemoryArenaReserve(RnMemoryArena* self, ssize size, ssize step, ssize* result);

b32
rnMemoryArenaRelease(RnMemoryArena* self, void* memory);

#endif // RN_BASE_MEMORY_ARENA_H
