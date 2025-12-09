#ifndef RN_STRUCTURE_ARRAY_H
#define RN_STRUCTURE_ARRAY_H

#include "./import.h"

#define RN_ARRAY_TAG struct { ssize size; ssize count; }

#define RnArray(type) struct { RN_ARRAY_TAG arrayTag; type* values; }

#define rnArrayCreate(self, arena, size) ( \
    (self)->values = __rnArrayTagCreate__( \
        &(self)->arrayTag,                 \
        (self)->values,                    \
        arena,                             \
        size,                              \
        sizeof(*(self)->values)),          \
    rnArraySize(self)                      \
)

#define rnArraySize(self)           __rnArrayTagSize__(&(self)->arrayTag)
#define rnArrayCount(self)          __rnArrayTagCount__(&(self)->arrayTag)
#define rnArrayFront(self)          __rnArrayTagFront__(&(self)->arrayTag)
#define rnArrayBack(self)           __rnArrayTagBack__(&(self)->arrayTag)
#define rnArrayIsEmpty(self)        __rnArrayTagIsEmpty__(&(self)->arrayTag)
#define rnArrayIsFull(self)         __rnArrayTagIsFull__(&(self)->arrayTag)
#define rnArrayIsIndex(self, index) __rnArrayTagIsIndex__(&(self)->arrayTag, index)
#define rnArrayClear(self)          __rnArrayClear__(&(self)->arrayTag)

#define rnArrayInsert(self, index, value) ( \
    __rnArrayTagMoveRight__(                \
        &(self)->arrayTag,                  \
        (self)->values,                     \
        index,                              \
        sizeof(*(self)->values)             \
    ) != 0 ? (                              \
        (self)->values[index]   = value,    \
        (self)->arrayTag.count += 1         \
    ), 1                                    \
    :  0                                    \
)

#define rnArrayPushFront(self, value) rnArrayInsert(self, 0,                  value)
#define rnArrayPushBack(self, value)  rnArrayInsert(self, rnArrayCount(self), value)

#define rnArrayRemove(self, index, value) \
    (__rnArrayTagRemove__(&(self)->arrayTag, (self)->values, index, value, sizeof(*(self)->values)))

#define rnArrayPopFront(self, value) rnArrayRemove(self, 0,                 value)
#define rnArrayPopBack(self, value)  rnArrayRemove(self, rnArrayBack(self), value)

#define rnArrayGet(self, index, other) \
    (rnArrayIsIndex(self, index) != 0 ? (self)->values[index] : other)

#define rnArrayGetRef(self, index) \
    (rnArrayIsIndex(self, index) != 0 ? &(self)->values[index] : 0)

void*
__rnArrayTagCreate__(void* tag, void* values, RnMemoryArena* arena, ssize size, ssize step);

ssize
__rnArrayTagSize__(void* tag);

ssize
__rnArrayTagCount__(void* tag);

ssize
__rnArrayTagFront__(void* tag);

ssize
__rnArrayTagBack__(void* tag);

b32
__rnArrayTagIsEmpty__(void* tag);

b32
__rnArrayTagIsFull__(void* tag);

b32
__rnArrayTagIsIndex__(void* tag, ssize index);

void
__rnArrayClear__(void* tag);

b32
__rnArrayTagMoveRight__(void* tag, void* values, ssize index, ssize step);

b32
__rnArrayTagRemove__(void* tag, void* values, ssize index, void* value, ssize step);

#endif // RN_STRUCTURE_ARRAY_H
