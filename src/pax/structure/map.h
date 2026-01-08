#ifndef PX_STRUCTURE_MAP_H
#define PX_STRUCTURE_MAP_H

#include "import.h"

#define __PxMapTag__(ktype) struct { \
    ssize  map_size;                 \
    ssize  map_count;                \
    ssize  map_step_key;             \
    ssize  map_step_value;           \
    ssize* map_indices;              \
    void*  map_proc_hash;            \
    void*  map_proc_is_equal;        \
    ssize  map_index;                \
    ktype  map_key;                  \
}

typedef struct PxMapTag
{
    ssize  map_size;
    ssize  map_count;
    ssize  map_step_key;
    ssize  map_step_value;
    ssize* map_indices;
    void*  map_proc_hash;
    void*  map_proc_is_equal;
    ssize  map_index;
}
PxMapTag;

typedef ssize (PxMapProcHash)    (void*);
typedef b32   (PxMapProcIsEqual) (void*, void*);

#define PxMap(ktype, vtype) struct {                 \
    __PxMapTag__(ktype); ktype* keys; vtype* values; \
}

#define pxMapSize(self)    __pxMapSize__(((PxMapTag*) self))
#define pxMapCount(self)   __pxMapCount__(((PxMapTag*) self))
#define pxMapIsEmpty(self) __pxMapIsEmpty__(((PxMapTag*) self))
#define pxMapIsFull(self)  __pxMapIsFull__(((PxMapTag*) self))

#define pxMapIsKey(self, key) ( \
    (self)->map_key = (key),    \
    __pxMapIsKey__(             \
        ((PxMapTag*) self),     \
        (self)->keys,           \
        &(self)->map_key)       \
)

#define pxMapClear(self) __pxMapClear__(((PxMapTag*) self))

#define pxMapCreate(self, arena, size, proc_hash, proc_is_equal) ( \
    __pxMapCreate__(                                               \
        ((PxMapTag*) self),                                        \
        ((void**) &(self)->keys),                                  \
        sizeof *(self)->keys,                                      \
        ((void**) &(self)->values),                                \
        sizeof *(self)->values,                                    \
        arena, size, proc_hash, proc_is_equal)                     \
)

#define pxMapInsert(self, key, value) (                              \
    (self)->map_key = (key),                                         \
    __pxMapSlotOpen__(                                               \
        ((PxMapTag*) self),                                          \
        (self)->keys,                                                \
        &(self)->map_key) != 0 ?                                     \
    (                                                                \
        (self)->map_indices[(self)->map_index]  = (self)->map_count, \
        (self)->keys[(self)->map_count]         = (self)->map_key,   \
        (self)->values[(self)->map_count]       = (value),           \
        (self)->map_count                     += 1                   \
    ), 1 : 0                                                         \
)

#define pxMapGet(self, key, other) \
    (pxMapIsKey(self, key) != 0 ? (self)->values[(self)->map_index] : (other))

#define pxMapGetPntr(self, key) \
    (pxMapIsKey(self, key) != 0 ? &(self)->values[(self)->map_index] : PX_NULL)

b32
__pxMapCreate__(PxMapTag* self, void** pntr_keys, ssize step_key, void** pntr_values, ssize step_value,
    PxMemoryArena* arena, ssize size, void* proc_hash, void* proc_is_equal);

ssize __pxMapSize__(PxMapTag* self);

ssize __pxMapCount__(PxMapTag* self);

b32 __pxMapIsEmpty__(PxMapTag* self);

b32 __pxMapIsFull__(PxMapTag* self);

b32 __pxMapIsKey__(PxMapTag* self, void* keys, void* key);

void __pxMapClear__(PxMapTag* self);

b32 __pxMapSlotOpen__(PxMapTag* self, void* keys, void* key);

#endif // PX_STRUCTURE_MAP_H
