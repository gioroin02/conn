#ifndef P_STRUCTURE_MAP_C
#define P_STRUCTURE_MAP_C

#include "map.h"

static Int __pMapHash__(PMapTag* self, void* key)
{
    return ((PMapProcHash*) self->map_proc_hash)(key);
}

static Bool __pMapIsEqual__(PMapTag* self, void* key, void* other)
{
    return ((PMapProcIsEqual*) self->map_proc_is_equal)(key, other);
}

static Int __pMapDistance__(PMapTag* self, void* key, Int index)
{
    Int size  = self->map_size;
    Int probe = __pMapHash__(self, key) % size;

    if (index < probe)
        return size + index - probe;

    return index - probe;
}

static Int __pMapIndexForKey__(PMapTag* self, void* keys, void* key)
{
    Int size  = self->map_size;
    Int probe = __pMapHash__(self, key) % size;
    Int dist  = 0;
    Int index = self->map_indices[probe];

    if (key == NULL || self->map_count == 0) return -1;

    for (dist = 0; dist < size && index >= 0; dist += 1) {
        void* other_key = &((U8*) keys)[index * self->map_step_key];

        if (__pMapIsEqual__(self, key, other_key) != 0) return probe;

        if (dist > __pMapDistance__(self, other_key, index))
            break;

        probe = (probe + 1) % size;
        index = self->map_indices[probe];
    }

    return -1;
}

Bool __pMapCreate__(PMapTag* self, void** pntr_keys, Int step_key, void** pntr_values, Int step_value,
    PMemoryArena* arena, Int size, void* proc_hash, void* proc_is_equal)
{
    pMemorySet(self, sizeof *self, 0xAB);

    if (pntr_keys == NULL || pntr_values == NULL || size < 0 || step_key <= 0 || step_value <= 0)
        return 0;

    void* mark = pMemoryArenaTell(arena);

    Int* indices = pMemoryArenaReserveManyOf(arena, Int, size);
    U8*  keys    = pMemoryArenaReserve(arena, size, step_key);
    U8*  values  = pMemoryArenaReserve(arena, size, step_value);

    if (indices != NULL && keys != NULL && values != NULL) {
        self->map_size          = size;
        self->map_count         = 0;
        self->map_step_key      = step_key;
        self->map_step_value    = step_value;
        self->map_indices       = indices;
        self->map_proc_hash     = proc_hash;
        self->map_proc_is_equal = proc_is_equal;

        *pntr_keys   = keys;
        *pntr_values = values;

        __pMapClear__(self);

        return 1;
    }

    pMemoryArenaRewind(arena, mark);

    return 0;
}

Int __pMapSize__(PMapTag* self)
{
    return self->map_size;
}

Int __pMapCount__(PMapTag* self)
{
    return self->map_count;
}

Bool __pMapIsEmpty__(PMapTag* self)
{
    return self->map_count == 0 ? 1 : 0;
}

Bool __pMapIsFull__(PMapTag* self)
{
    return self->map_count == self->map_size ? 1 : 0;
}

Bool __pMapIsKey__(PMapTag* self, void* keys, void* key)
{
    Int index = __pMapIndexForKey__(self, keys, key);

    if (index < 0) return 0;

    self->map_index = self->map_indices[index];

    return 1;
}

void __pMapClear__(PMapTag* self)
{
    Int index = 0;

    for (index = 0; index < self->map_size; index += 1)
        self->map_indices[index] = -1;

    self->map_count = 0;
}

Bool __pMapSlotOpen__(PMapTag* self, void* keys, void* key)
{
    if (self->map_count == self->map_size) return 0;

    Int size  = self->map_size;
    Int probe = __pMapHash__(self, key) % size;
    Int dist  = 0;
    Int index = self->map_indices[probe];

    for (dist = 0; dist < size && index >= 0; dist += 1) {
        void* other_key  = &((U8*) keys)[index * self->map_step_key];
        Int   other_dist = 0;

        if (__pMapIsEqual__(self, key, other_key) != 0) break;

        other_dist = __pMapDistance__(self, other_key, index);

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

#endif // P_STRUCTURE_MAP_C
