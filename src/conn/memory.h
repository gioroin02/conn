#ifndef CONN_MEMORY_H
#define CONN_MEMORY_H

#include "./import.h"

u8
connMemoryReadU8(u8* values, ssize index, ssize size);

u16
connMemoryReadU16(u8* values, ssize index, ssize size);

u32
connMemoryReadU32(u8* values, ssize index, ssize size);

u64
connMemoryReadU64(u8* values, ssize index, ssize size);

ssize
connMemoryWriteU8(u8 value, u8* values, ssize index, ssize size);

ssize
connMemoryWriteU16(u16 value, u8* values, ssize index, ssize size);

ssize
connMemoryWriteU32(u32 value, u8* values, ssize index, ssize size);

ssize
connMemoryWriteU64(u64 value, u8* values, ssize index, ssize size);

#endif // CONN_MEMORY_H
