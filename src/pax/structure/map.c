#ifndef PX_STRUCTURE_MAP_C
#define PX_STRUCTURE_MAP_C

#include "map.h"

static ssize
__pxMapHash__(PxMapTag* self, void* key)
{
    return ((PxMapProcHash*) self->map_proc_hash)(key);
}

static b32
__pxMapIsEqual__(PxMapTag* self, void* key, void* other)
{
    return ((PxMapProcIsEqual*) self->map_proc_is_equal)(key, other);
}

static ssize
__pxMapDistance__(PxMapTag* self, void* key, ssize index)
{
    ssize size  = self->map_size;
    ssize probe = __pxMapHash__(self, key) % size;

    if (index < probe)
        return size + index - probe;

    return index - probe;
}

static ssize
__pxMapIndexForKey__(PxMapTag* self, void* keys, void* key)
{
    ssize size  = self->map_size;
    ssize probe = __pxMapHash__(self, key) % size;
    ssize dist  = 0;
    ssize index = self->map_indices[probe];

    if (key == PX_NULL || self->map_count == 0) return -1;

    for (dist = 0; dist < size && index >= 0; dist += 1) {
        void* other_key = &((u8*) keys)[index * self->map_step_key];

        if (__pxMapIsEqual__(self, key, other_key) != 0) return probe;

        if (dist > __pxMapDistance__(self, other_key, index))
            break;

        probe = (probe + 1) % size;
        index = self->map_indices[probe];
    }

    return -1;
}

b32
__pxMapCreate__(PxMapTag* self, void** pntr_keys, ssize step_key, void** pntr_values, ssize step_value,
    PxMemoryArena* arena, ssize size, void* proc_hash, void* proc_is_equal)
{
    void* mark = pxMemoryArenaTell(arena);

    pxMemorySet(self, sizeof *self, 0xAB);

    ssize* indices = pxMemoryArenaReserveManyOf(arena, ssize, size);
    u8*    keys    = pxMemoryArenaReserve(arena, size, step_key);
    u8*    values  = pxMemoryArenaReserve(arena, size, step_value);

    if (indices != PX_NULL && keys != PX_NULL && values != PX_NULL) {
        *pntr_keys   = keys;
        *pntr_values = values;

        self->map_size          = size;
        self->map_count         = 0;
        self->map_step_key      = step_key;
        self->map_step_value    = step_value;
        self->map_indices       = indices;
        self->map_proc_hash     = proc_hash;
        self->map_proc_is_equal = proc_is_equal;

        __pxMapClear__(self);

        return 1;
    }

    pxMemoryArenaRelease(arena, mark);

    return 0;
}

ssize
__pxMapSize__(PxMapTag* self)
{
    return self->map_size;
}

ssize
__pxMapCount__(PxMapTag* self)
{
    return self->map_count;
}

b32
__pxMapIsEmpty__(PxMapTag* self)
{
    return self->map_count == 0 ? 1 : 0;
}

b32
__pxMapIsFull__(PxMapTag* self)
{
    return self->map_count == self->map_size ? 1 : 0;
}

b32
__pxMapIsKey__(PxMapTag* self, void* keys, void* key)
{
    ssize index = __pxMapIndexForKey__(self, keys, key);

    if (index < 0) return 0;

    self->map_index = self->map_indices[index];

    return 1;
}

void
__pxMapClear__(PxMapTag* self)
{
    ssize index = 0;

    for (index = 0; index < self->map_size; index += 1)
        self->map_indices[index] = -1;

    self->map_count = 0;
}

b32
__pxMapSlotOpen__(PxMapTag* self, void* keys, void* key)
{
    if (self->map_count == self->map_size) return 0;

    ssize size  = self->map_size;
    ssize probe = __pxMapHash__(self, key) % size;
    ssize dist  = 0;
    ssize index = self->map_indices[probe];

    for (dist = 0; dist < size && index >= 0; dist += 1) {
        void* other_key  = &((u8*) keys)[index * self->map_step_key];
        ssize other_dist = 0;

        if (__pxMapIsEqual__(self, key, other_key) != 0) break;

        other_dist = __pxMapDistance__(self, other_key, index);

        if (dist > other_dist) {
            self->map_indices[probe] = index;
            dist                     = other_dist;
        }

        probe = (probe + 1) % size;
        index = self->map_indices[probe];
    }

    self->map_index          = probe;
    self->map_indices[probe] = -1;

    return 1;
}

#endif // PX_STRUCTURE_MAP_C
