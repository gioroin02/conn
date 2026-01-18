#ifndef P_STRUCTURE_ARRAY_H
#define P_STRUCTURE_ARRAY_H

#include "import.h"

#define __PArrayTag__() struct { \
    Int array_size;              \
    Int array_count;             \
    Int array_step;              \
    Int array_index;             \
}

typedef struct
{
    Int array_size;
    Int array_count;
    Int array_step;
    Int array_index;
}
PArrayTag;

#define PArray(type) struct { __PArrayTag__(); type* values; }

#define pArraySize(self)    __pArraySize__(((PArrayTag*) self))
#define pArrayCount(self)   __pArrayCount__(((PArrayTag*) self))
#define pArrayFront(self)   __pArrayFront__(((PArrayTag*) self))
#define pArrayBack(self)    __pArrayBack__(((PArrayTag*) self))
#define pArrayIsEmpty(self) __pArrayIsEmpty__(((PArrayTag*) self))
#define pArrayIsFull(self)  __pArrayIsFull__(((PArrayTag*) self))

#define pArrayIsIndex(self, index) ( \
    (self)->array_index = (index),   \
    __pArrayIsIndex__(               \
        ((PArrayTag*) self),         \
        (self)->array_index)         \
)

#define pArrayClear(self) __pArrayClear__(((PArrayTag*) self))

#define pArrayCreate(self, arena, size) ( \
    __pArrayCreate__(                     \
        ((PArrayTag*) self),              \
        ((void**) &(self)->values),       \
        sizeof *(self)->values,           \
        arena, size)                      \
)

#define pArrayCopy(self, index, value)  \
(                                       \
    __pArrayCopy__(((PArrayTag*) self), \
        (self)->values, index, value)   \
)

#define pArrayInsert(self, index, value)                \
(                                                       \
    (self)->array_index = (index),                      \
    __pArraySlotOpen__(                                 \
        ((PArrayTag*) self),                            \
        (self)->values,                                 \
        (self)->array_index) != 0 ?                     \
    (                                                   \
        (self)->values[(self)->array_index]  = (value), \
        (self)->array_count                 += 1        \
    ), 1 : 0                                            \
)

#define pArrayAdd(self, index)      \
(                                   \
    (self)->array_index = (index),  \
    __pArraySlotOpen__(             \
        ((PArrayTag*) self),        \
        (self)->values,             \
        (self)->array_index) != 0 ? \
    (                               \
        (self)->array_count += 1    \
    ), 1 : 0                        \
)

#define pArrayRemove(self, index, value)  \
(                                         \
    (self)->array_index = (index),        \
    pArrayCopy(self, index, value) != 0 ? \
    (                                     \
        __pArraySlotClose__(              \
            ((PArrayTag*) self),          \
            (self)->values,               \
            (self)->array_index),         \
        (self)->array_count -= 1          \
    ), 1 : 0                              \
)

#define pArrayDrop(self, index)     \
(                                   \
    (self)->array_index = (index),  \
    __pArraySlotClose__(            \
        ((PArrayTag*) self),        \
        (self)->values,             \
        (self)->array_index) != 0 ? \
    (                               \
        (self)->array_count -= 1    \
    ), 1 : 0                        \
)

#define pArrayInsertFront(self, value) pArrayInsert(self, 0,                 value)
#define pArrayInsertBack(self, value)  pArrayInsert(self, pArrayCount(self), value)

#define pArrayAddFront(self) pArrayAdd(self, 0)
#define pArrayAddBack(self)  pArrayAdd(self, pArrayCount(self))

#define pArrayRemoveFront(self, value) pArrayRemove(self, 0,                value)
#define pArrayRemoveBack(self, value)  pArrayRemove(self, pArrayBack(self), value)

#define pArrayDropFront(self) pArrayDrop(self, 0)
#define pArrayDropBack(self)  pArrayDrop(self, pArrayBack(self))

#define pArrayGet(self, index, other) \
    (pArrayIsIndex(self, index) != 0 ? (self)->values[(self)->array_index] : (other))

#define pArrayGetPntr(self, index) \
    (pArrayIsIndex(self, index) != 0 ? &(self)->values[(self)->array_index] : NULL)

Bool __pArrayCreate__(PArrayTag* self, void** pntr, Int step, PMemoryArena* arena, Int size);

Int __pArraySize__(PArrayTag* self);

Int __pArrayCount__(PArrayTag* self);

Int __pArrayFront__(PArrayTag* self);

Int __pArrayBack__(PArrayTag* self);

Bool __pArrayIsEmpty__(PArrayTag* self);

Bool __pArrayIsFull__(PArrayTag* self);

Bool __pArrayIsIndex__(PArrayTag* self, Int index);

void __pArrayClear__(PArrayTag* self);

Bool __pArrayCopy__(PArrayTag* self, void* values, Int index, void* value);

Bool __pArraySlotOpen__(PArrayTag* self, void* values, Int index);

Bool __pArraySlotClose__(PArrayTag* self, void* values, Int index);

#endif // P_STRUCTURE_ARRAY_H
