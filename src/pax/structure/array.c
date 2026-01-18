#ifndef P_STRUCTURE_ARRAY_C
#define P_STRUCTURE_ARRAY_C

#include "array.h"

Bool __pArrayCreate__(PArrayTag* self, void** pntr, Int step, PMemoryArena* arena, Int size)
{
    pMemorySet(self, sizeof *self, 0xAB);

    if (pntr == NULL || size < 0 || step <= 0) return 0;

    U8* values = pMemoryArenaReserve(arena, size, step);

    if (values != NULL) {
        self->array_size  = size;
        self->array_count = 0;
        self->array_step  = step;

        *pntr = values;

        return 1;
    }

    return 0;
}

Int __pArraySize__(PArrayTag* self)
{
    return self->array_size;
}

Int __pArrayCount__(PArrayTag* self)
{
    return self->array_count;
}

Int __pArrayFront__(PArrayTag* self)
{
    return 0;
}

Int __pArrayBack__(PArrayTag* self)
{
    return self->array_count > 0 ? self->array_count - 1 : 0;
}

Bool __pArrayIsEmpty__(PArrayTag* self)
{
    return self->array_count == 0 ? 1 : 0;
}

Bool __pArrayIsFull__(PArrayTag* self)
{
    return self->array_count == self->array_size ? 1 : 0;
}

Bool __pArrayIsIndex__(PArrayTag* self, Int index)
{
    if (index < 0 || index >= self->array_count)
        return 0;

    return 1;
}

void __pArrayClear__(PArrayTag* self)
{
    self->array_count = 0;
}

Bool __pArrayCopy__(PArrayTag* self, void* values, Int index, void* value)
{
    Int start = self->array_step * index;

    if (index < 0 || index >= self->array_count) return 0;

    pMemoryCopy(value, self->array_step,
        &((U8*) values)[start]);

    return 1;
}

Bool __pArraySlotOpen__(PArrayTag* self, void* values, Int index)
{
    Int start = self->array_step * index;
    Int stop  = self->array_step * self->array_size;

    if (index < 0 || index > self->array_count) return 0;

    pMemoryShiftForw(&((U8*) values)[start],
        stop, self->array_step, 0xAB);

    return 1;
}

Bool __pArraySlotClose__(PArrayTag* self, void* values, Int index)
{
    Int start = self->array_step * index;
    Int stop  = self->array_step * self->array_size;

    if (index < 0 || index >= self->array_count) return 0;

    pMemoryShiftBack(&((U8*) values)[start],
        stop, self->array_step, 0xAB);

    return 1;
}

#endif // P_STRUCTURE_ARRAY_C
