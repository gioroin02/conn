#ifndef RN_STRUCTURE_ARRAY_C
#define RN_STRUCTURE_ARRAY_C

#include "./array.h"

typedef struct RnArrayTag
{
    ssize size;
    ssize count;
}
RnArrayTag;

void*
__rnArrayTagCreate__(void* tag, void* values, RnMemoryArena* arena, ssize size, ssize step)
{
    RnArrayTag* self = ((RnArrayTag*) tag);

    if (self == 0 || values != 0 || step <= 0 || size <= 0)
        return 0;

    u8* memory = rnMemoryArenaReserve(arena, size, step, &size);

    if (memory != 0) self->size = size / step;

    self->count = 0;

    return memory;
}

ssize
__rnArrayTagSize__(void* tag)
{
    RnArrayTag* self = ((RnArrayTag*) tag);

    if (self != 0)
        return self->size;

    return 0;
}

ssize
__rnArrayTagCount__(void* tag)
{
    RnArrayTag* self = ((RnArrayTag*) tag);

    if (self != 0)
        return self->count;

    return 0;
}

ssize
__rnArrayTagFront__(void* tag)
{
    return 0;
}

ssize
__rnArrayTagBack__(void* tag)
{
    RnArrayTag* self = ((RnArrayTag*) tag);

    if (self != 0 && self->count > 0)
        return self->count - 1;

    return 0;
}

b32
__rnArrayTagIsEmpty__(void* tag)
{
    RnArrayTag* self = ((RnArrayTag*) tag);

    if (self != 0 && self->count == 0)
        return 1;

    return 0;
}

b32
__rnArrayTagIsFull__(void* tag)
{
    RnArrayTag* self = ((RnArrayTag*) tag);

    if (self != 0 && self->count == self->size)
        return 1;

    return 0;
}

b32
__rnArrayTagIsIndex__(void* tag, ssize index)
{
    RnArrayTag* self = ((RnArrayTag*) tag);

    if (self == 0 || index < 0 || index >= self->count)
        return 0;

    return 1;
}

void
__rnArrayClear__(void* tag)
{
    ((RnArrayTag*) tag)->count = 0;
}

b32
__rnArrayTagExtend__(void* tag, void* values, ssize index, ssize step)
{
    RnArrayTag* self = ((RnArrayTag*) tag);

    if (self == 0 || values == 0 || step <= 0 || index < 0 || index > self->count)
        return 0;

    if (index == self->count) return 1;

    ssize start = step * self->count;
    ssize stop  = step * index;

    for (ssize i = start; i > stop; i -= 1)
        ((u8*) values)[i + step - 1] = ((u8*) values)[i - 1];

    return 1;
}

b32
__rnArrayTagRemove__(void* tag, void* values, ssize index, void* value, ssize step)
{
    RnArrayTag* self = ((RnArrayTag*) tag);

    if (self == 0 || values == 0 || step <= 0 || index < 0 || index >= self->count)
        return 0;

    self->count -= 1;

    ssize start = step * index;
    ssize stop  = step * self->count;

    if (value != 0) {
        for (ssize i = 0; i < step; i += 1)
            ((u8*) value)[i] = ((u8*) values)[i + index];
    }

    for (ssize i = start; i < stop; i += 1)
        ((u8*) values)[i] = ((u8*) values)[i + step];

    return 1;
}

#endif // RN_STRUCTURE_ARRAY_C
