#ifndef RN_BASE_MEMORY_BUFFER8_H
#define RN_BASE_MEMORY_BUFFER8_H

#include "./arena.h"

typedef struct RnBuffer8
{
    u8*   values;
    ssize size;
    ssize count;
}
RnBuffer8;

RnBuffer8
rnBuffer8Make(u8* values, ssize size);

RnBuffer8
rnBuffer8Cut(RnBuffer8* self, ssize index, ssize size);

ssize
rnBuffer8Size(RnBuffer8* self);

ssize
rnBuffer8Count(RnBuffer8* self);

void
rnBuffer8Clear(RnBuffer8* self);

ssize
rnBuffer8DropHead(RnBuffer8* self, ssize size);

ssize
rnBuffer8InsertHead(RnBuffer8* self, u8* values, ssize size);

ssize
rnBuffer8RemoveHead(RnBuffer8* self, u8* values, ssize size);

ssize
rnBuffer8DropTail(RnBuffer8* self, ssize size);

ssize
rnBuffer8InsertTail(RnBuffer8* self, u8* values, ssize size);

ssize
rnBuffer8RemoveTail(RnBuffer8* self, u8* values, ssize size);

#endif // RN_BASE_MEMORY_BUFFER8_H
