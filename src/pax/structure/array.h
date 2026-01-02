#ifndef PX_STRUCTURE_ARRAY_H
#define PX_STRUCTURE_ARRAY_H

#include "import.h"

#define __PxArrayTag__() struct { \
    ssize array_size;             \
    ssize array_count;            \
    ssize array_step;             \
    ssize array_index;            \
}

typedef struct
{
    ssize array_size;
    ssize array_count;
    ssize array_step;
    ssize array_index;
}
PxArrayTag;

#define PxArray(type) struct { __PxArrayTag__(); type* values; }

#define pxArraySize(self)    __pxArraySize__(((PxArrayTag*) self))
#define pxArrayCount(self)   __pxArrayCount__(((PxArrayTag*) self))
#define pxArrayFront(self)   __pxArrayFront__(((PxArrayTag*) self))
#define pxArrayBack(self)    __pxArrayBack__(((PxArrayTag*) self))
#define pxArrayIsEmpty(self) __pxArrayIsEmpty__(((PxArrayTag*) self))
#define pxArrayIsFull(self)  __pxArrayIsFull__(((PxArrayTag*) self))

#define pxArrayIsIndex(self, index) ( \
    (self)->array_index = (index),    \
    __pxArrayIsIndex__(               \
        ((PxArrayTag*) self),         \
        (self)->array_index)          \
)

#define pxArrayClear(self) __pxArrayClear__(((PxArrayTag*) self))

#define pxArrayCreate(self, arena, size) ( \
    __pxArrayCreate__(                     \
        ((PxArrayTag*) self),              \
        ((void**) &(self)->values),        \
        sizeof *(self)->values,            \
        arena, size)                       \
)

#define pxArrayCopy(self, index, value)   \
(                                         \
    __pxArrayCopy__(((PxArrayTag*) self), \
        (self)->values, index, value)     \
)

#define pxArrayInsert(self, index, value)               \
(                                                       \
    (self)->array_index = (index),                      \
    __pxArraySlotOpen__(                                \
        ((PxArrayTag*) self),                           \
        (self)->values,                                 \
        (self)->array_index) != 0 ?                     \
    (                                                   \
        (self)->values[(self)->array_index]  = (value), \
        (self)->array_count                 += 1        \
    ), 1 : 0                                            \
)

#define pxArrayAdd(self, index)     \
(                                   \
    (self)->array_index = (index),  \
    __pxArraySlotOpen__(            \
        ((PxArrayTag*) self),       \
        (self)->values,             \
        (self)->array_index) != 0 ? \
    (                               \
        (self)->array_count += 1    \
    ), 1 : 0                        \
)

#define pxArrayRemove(self, index, value)  \
(                                          \
    pxArrayCopy(self, index, value) != 0 ? \
    (                                      \
        __pxArraySlotClose__(              \
            ((PxArrayTag*) self),          \
            (self)->values,                \
            (self)->array_index),          \
        (self)->array_count -= 1           \
    ), 1 : 0                               \
)

#define pxArrayDrop(self, index) \
(                                \
    __pxArraySlotClose__(        \
        ((PxArrayTag*) self),    \
        (self)->values,          \
        (index)) != 0 ?          \
    1 : 0                        \
)

#define pxArrayInsertFront(self, value) pxArrayInsert(self, 0,                  value)
#define pxArrayInsertBack(self, value)  pxArrayInsert(self, pxArrayCount(self), value)

#define pxArrayAddFront(self) pxArrayAdd(self, 0)
#define pxArrayAddBack(self)  pxArrayAdd(self, pxArrayCount(self))

#define pxArrayRemoveFront(self, value) pxArrayRemove(self, 0,                 value)
#define pxArrayRemoveBack(self, value)  pxArrayRemove(self, pxArrayBack(self), value)

#define pxArrayDropFront(self) pxArrayDrop(self, 0)
#define pxArrayDropBack(self)  pxArrayDrop(self, pxArrayBack(self))

#define pxArrayGet(self, index, other) \
    (pxArrayIsIndex(self, index) != 0 ? (self)->values[(self)->array_index] : (other))

#define pxArrayGetPntr(self, index) \
    (pxArrayIsIndex(self, index) != 0 ? &(self)->values[(self)->array_index] : PX_NULL)

b32
__pxArrayCreate__(PxArrayTag* self, void** pntr, ssize step, PxMemoryArena* arena, ssize size);

ssize
__pxArraySize__(PxArrayTag* self);

ssize
__pxArrayCount__(PxArrayTag* self);

ssize
__pxArrayFront__(PxArrayTag* self);

ssize
__pxArrayBack__(PxArrayTag* self);

b32
__pxArrayIsEmpty__(PxArrayTag* self);

b32
__pxArrayIsFull__(PxArrayTag* self);

b32
__pxArrayIsIndex__(PxArrayTag* self, ssize index);

void
__pxArrayClear__(PxArrayTag* self);

b32
__pxArrayCopy__(PxArrayTag* self, void* values, ssize index, void* value);

b32
__pxArraySlotOpen__(PxArrayTag* self, void* values, ssize index);

b32
__pxArraySlotClose__(PxArrayTag* self, void* values, ssize index);

#endif // PX_STRUCTURE_ARRAY_H
