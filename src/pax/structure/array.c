#ifndef PX_STRUCTURE_ARRAY_C
#define PX_STRUCTURE_ARRAY_C

#include "array.h"

b32
__pxArrayCreate__(PxArrayTag* self, void** pntr, ssize step, PxMemoryArena* arena, ssize size)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    u8* values = pxMemoryArenaReserve(arena, size, step);

    if (values == PX_NULL) return 0;

    *pntr = values;

    self->array_size  = size;
    self->array_count = 0;
    self->array_step  = step;

    return 1;
}

ssize
__pxArraySize__(PxArrayTag* self)
{
    return self->array_size;
}

ssize
__pxArrayCount__(PxArrayTag* self)
{
    return self->array_count;
}

ssize
__pxArrayFront__(PxArrayTag* self)
{
    return 0;
}

ssize
__pxArrayBack__(PxArrayTag* self)
{
    return self->array_count > 0 ? self->array_count - 1 : 0;
}

b32
__pxArrayIsEmpty__(PxArrayTag* self)
{
    return self->array_count == 0 ? 1 : 0;
}

b32
__pxArrayIsFull__(PxArrayTag* self)
{
    return self->array_count == self->array_size ? 1 : 0;
}

b32
__pxArrayIsIndex__(PxArrayTag* self, ssize index)
{
    if (index < 0 || index >= self->array_count)
        return 0;

    return 1;
}

void
__pxArrayClear__(PxArrayTag* self)
{
    self->array_count = 0;
}

b32
__pxArrayCopy__(PxArrayTag* self, void* values, ssize index, void* value)
{
    ssize start = self->array_step * index;

    if (index < 0 || index >= self->array_count) return 0;

    pxMemoryCopy(value, self->array_step,
        &((u8*) values)[start]);

    return 1;
}

b32
__pxArraySlotOpen__(PxArrayTag* self, void* values, ssize index)
{
    ssize start = self->array_step * index;
    ssize stop  = self->array_step * self->array_size;

    if (index < 0 || index > self->array_count) return 0;

    pxMemoryShiftForw(&((u8*) values)[start],
        stop, self->array_step, 0xAB);

    return 1;
}

b32
__pxArraySlotClose__(PxArrayTag* self, void* values, ssize index)
{
    ssize start = self->array_step * index;
    ssize stop  = self->array_step * self->array_size;

    if (index < 0 || index >= self->array_count) return 0;

    pxMemoryShiftBack(&((u8*) values)[start],
        stop, self->array_step, 0xAB);

    return 1;
}

#endif // PX_STRUCTURE_ARRAY_C
