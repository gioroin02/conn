#ifndef RN_BASE_MEMORY_ARENA_C
#define RN_BASE_MEMORY_ARENA_C

#include "./arena.h"

RnMemoryArena
rnMemoryArenaMake(void* memory, ssize size)
{
    RnMemoryArena result = {0};

    if (memory == 0 || size <= 0)
        return result;

    result.values = ((u8*) memory);
    result.size   = size;

    return result;
}

void
rnMemoryArenaClear(RnMemoryArena* self)
{
    self->count = 0;
}

void*
rnMemoryArenaTell(RnMemoryArena* self)
{
    return self->values + self->count;
}

void*
rnMemoryArenaReserve(RnMemoryArena* self, ssize size, ssize step, ssize* result)
{
    void* memory = rnMemoryArenaTell(self);
    ssize count  = self->count;

    if (size <= 0 || step <= 0 || size > RN_MAX_SSIZE / step)
        return 0;

    ssize bytes = size * step;

    if (self->count < 0 || self->count + bytes > self->size)
        return 0;

    self->count = rnMemoryAlignForward(
        self->count + bytes, RN_MEMORY_DEFAULT_ALIGNMENT);

    for (ssize i = 0; i < self->count - count; i += 1)
        ((u8*) memory)[i] = 0;

    if (result != 0) *result = self->count - count;

    return memory;
}

b32
rnMemoryArenaRelease(RnMemoryArena* self, void* memory)
{
    ssize diff = ((u8*) memory) - self->values;

    if (diff % RN_MEMORY_DEFAULT_ALIGNMENT != 0)
        return 0;

    if (diff < 0 || diff >= self->count) return 0;

    self->count = diff;

    return 1;
}

#endif // RN_BASE_MEMORY_ARENA_C
