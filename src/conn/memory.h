#ifndef CONN_MEMORY_H
#define CONN_MEMORY_H

#include "import.h"

u8*
connMemoryReadU8Net(u8* values, ssize* size, u8* value);

u8*
connMemoryReadU16Net(u8* values, ssize* size, u16* value);

u8*
connMemoryReadU32Net(u8* values, ssize* size, u32* value);

u8*
connMemoryReadU64Net(u8* values, ssize* size, u64* value);

u8*
connMemoryWriteU8Net(u8* values, ssize* size, u8 value);

u8*
connMemoryWriteU16Net(u8* values, ssize* size, u16 value);

u8*
connMemoryWriteU32Net(u8* values, ssize* size, u32 value);

u8*
connMemoryWriteU64Net(u8* values, ssize* size, u64 value);

#endif // CONN_MEMORY_H
