#ifndef CONN_MEMORY_H
#define CONN_MEMORY_H

#include "import.h"

U8* connMemoryWriteU8Net(U8* values, Int* size, U8 value);

U8* connMemoryWriteU16Net(U8* values, Int* size, U16 value);

U8* connMemoryWriteU32Net(U8* values, Int* size, U32 value);

U8* connMemoryWriteU64Net(U8* values, Int* size, U64 value);

U8* connMemoryReadU8Net(U8* values, Int* size, U8* value);

U8* connMemoryReadU16Net(U8* values, Int* size, U16* value);

U8* connMemoryReadU32Net(U8* values, Int* size, U32* value);

U8* connMemoryReadU64Net(U8* values, Int* size, U64* value);

#endif // CONN_MEMORY_H
