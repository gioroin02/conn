#ifndef CONN_MEMORY_C
#define CONN_MEMORY_C

#include "./memory.h"

u8
connMemoryReadU8(u8* values, ssize index, ssize size)
{
    u8 result = 0;

    if (values == 0 || index < 0 || index >= size)
        return result;

    for (ssize i = 0; i < sizeof(result); i += 1)
        ((u8*) &result)[i] = values[index + i];

    return result;
}

u16
connMemoryReadU16(u8* values, ssize index, ssize size)
{
    u16 result = 0;

    if (values == 0 || index < 0 || index >= size)
        return result;

    for (ssize i = 0; i < sizeof(result); i += 1)
        ((u8*) &result)[i] = values[index + i];

    return result;
}

u32
connMemoryReadU32(u8* values, ssize index, ssize size)
{
    u32 result = 0;

    if (values == 0 || index < 0 || index >= size)
        return result;

    for (ssize i = 0; i < sizeof(result); i += 1)
        ((u8*) &result)[i] = values[index + i];

    return result;
}

u64
connMemoryReadU64(u8* values, ssize index, ssize size)
{
    u32 result = 0;

    if (values == 0 || index < 0 || index >= size)
        return result;

    for (ssize i = 0; i < sizeof(result); i += 1)
        ((u8*) &result)[i] = values[index + i];

    return result;
}

ssize
connMemoryWriteU8(u8 value, u8* values, ssize index, ssize size)
{
    if (values == 0 || index < 0 || index >= size)
        return 0;

    for (ssize i = 0; i < sizeof(value); i += 1)
        values[index + i] = ((u8*) &value)[i];

    return sizeof(value);
}

ssize
connMemoryWriteU16(u16 value, u8* values, ssize index, ssize size)
{
    if (values == 0 || index < 0 || index >= size)
        return 0;

    for (ssize i = 0; i < sizeof(value); i += 1)
        values[index + i] = ((u8*) &value)[i];

    return sizeof(value);
}

ssize
connMemoryWriteU32(u32 value, u8* values, ssize index, ssize size)
{
    if (values == 0 || index < 0 || index >= size)
        return 0;

    for (ssize i = 0; i < sizeof(value); i += 1)
        values[index + i] = ((u8*) &value)[i];

    return sizeof(value);
}

ssize
connMemoryWriteU64(u64 value, u8* values, ssize index, ssize size)
{
    if (values == 0 || index < 0 || index >= size)
        return 0;

    for (ssize i = 0; i < sizeof(value); i += 1)
        values[index + i] = ((u8*) &value)[i];

    return sizeof(value);
}

#endif // CONN_MEMORY_C
