#ifndef P_STRUCTURE_MAP_H
#define P_STRUCTURE_MAP_H

#include "import.h"

#define __PMapTag__(ktype) struct { \
    Int   map_size;                 \
    Int   map_count;                \
    Int   map_step_key;             \
    Int   map_step_value;           \
    Int*  map_indices;              \
    void* map_proc_hash;            \
    void* map_proc_is_equal;        \
    Int   map_index;                \
    ktype map_key;                  \
}

typedef struct PMapTag
{
    Int   map_size;
    Int   map_count;
    Int   map_step_key;
    Int   map_step_value;
    Int*  map_indices;
    void* map_proc_hash;
    void* map_proc_is_equal;
    Int   map_index;
}
PMapTag;

typedef Int  (PMapProcHash)    (void*);
typedef Bool (PMapProcIsEqual) (void*, void*);

#define PMap(ktype, vtype) struct {                 \
    __PMapTag__(ktype); ktype* keys; vtype* values; \
}

#define pMapSize(self)    __pMapSize__(((PMapTag*) self))
#define pMapCount(self)   __pMapCount__(((PMapTag*) self))
#define pMapIsEmpty(self) __pMapIsEmpty__(((PMapTag*) self))
#define pMapIsFull(self)  __pMapIsFull__(((PMapTag*) self))

#define pMapIsKey(self, key) ( \
    (self)->map_key = (key),   \
    __pMapIsKey__(             \
        ((PMapTag*) self),     \
        (self)->keys,          \
        &(self)->map_key)      \
)

#define pMapClear(self) __pMapClear__(((PMapTag*) self))

#define pMapCreate(self, arena, size, proc_hash, proc_is_equal) ( \
    __pMapCreate__(                                               \
        ((PMapTag*) self),                                        \
        ((void**) &(self)->keys),                                 \
        sizeof *(self)->keys,                                     \
        ((void**) &(self)->values),                               \
        sizeof *(self)->values,                                   \
        arena, size, proc_hash, proc_is_equal)                    \
)

#define pMapInsert(self, key, value) (                               \
    (self)->map_key = (key),                                         \
    __pMapSlotOpen__(                                                \
        ((PMapTag*) self),                                           \
        (self)->keys,                                                \
        &(self)->map_key) != 0 ?                                     \
    (                                                                \
        (self)->map_indices[(self)->map_index]  = (self)->map_count, \
        (self)->keys[(self)->map_count]         = (self)->map_key,   \
        (self)->values[(self)->map_count]       = (value),           \
        (self)->map_count                     += 1                   \
    ), 1 : 0                                                         \
)

#define pMapGet(self, key, other) \
    (pMapIsKey(self, key) != 0 ? (self)->values[(self)->map_index] : (other))

#define pMapGetPntr(self, key) \
    (pMapIsKey(self, key) != 0 ? &(self)->values[(self)->map_index] : NULL)

Bool __pMapCreate__(PMapTag* self, void** pntr_keys, Int step_key, void** pntr_values, Int step_value,
    PMemoryArena* arena, Int size, void* proc_hash, void* proc_is_equal);

Int __pMapSize__(PMapTag* self);

Int __pMapCount__(PMapTag* self);

Bool __pMapIsEmpty__(PMapTag* self);

Bool __pMapIsFull__(PMapTag* self);

Bool __pMapIsKey__(PMapTag* self, void* keys, void* key);

void __pMapClear__(PMapTag* self);

Bool __pMapSlotOpen__(PMapTag* self, void* keys, void* key);

#endif // P_STRUCTURE_MAP_H
