#ifndef CONN_MEMORY_C
#define CONN_MEMORY_C

#include "memory.h"

u8*
connMemoryReadU8Net(u8* values, ssize* size, u8* value)
{
    if (values == PX_NULL || size == PX_NULL || *size < sizeof *value)
        return PX_NULL;

    pxMemoryCopy(value, sizeof *value, values);

    if (pxHostByteOrderIsReverse() != 0)
        pxMemoryReverse(value, sizeof * value);

    *size  -= sizeof *value;
    values += sizeof *value;

    return values;
}

u8*
connMemoryReadU16Net(u8* values, ssize* size, u16* value)
{
    if (values == PX_NULL || size == PX_NULL || *size < sizeof *value)
        return PX_NULL;

    pxMemoryCopy(value, sizeof *value, values);

    if (pxHostByteOrderIsReverse() != 0)
        pxMemoryReverse(value, sizeof * value);

    *size  -= sizeof *value;
    values += sizeof *value;

    return values;
}

u8*
connMemoryReadU32Net(u8* values, ssize* size, u32* value)
{
    if (values == PX_NULL || size == PX_NULL || *size < sizeof *value)
        return PX_NULL;

    pxMemoryCopy(value, sizeof *value, values);

    if (pxHostByteOrderIsReverse() != 0)
        pxMemoryReverse(value, sizeof * value);

    *size  -= sizeof *value;
    values += sizeof *value;

    return values;
}

u8*
connMemoryReadU64Net(u8* values, ssize* size, u64* value)
{
    if (values == PX_NULL || size == PX_NULL || *size < sizeof *value)
        return PX_NULL;

    pxMemoryCopy(value, sizeof *value, values);

    if (pxHostByteOrderIsReverse() != 0)
        pxMemoryReverse(value, sizeof * value);

    *size  -= sizeof *value;
    values += sizeof *value;

    return values;
}

u8*
connMemoryWriteU8Net(u8* values, ssize* size, u8 value)
{
    if (values == PX_NULL || size == PX_NULL || *size < sizeof value)
        return PX_NULL;

    pxMemoryCopy(values, sizeof value, &value);

    if (pxHostByteOrderIsReverse() != 0)
        pxMemoryReverse(values, sizeof value);

    *size  -= sizeof value;
    values += sizeof value;

    return values;
}

u8*
connMemoryWriteU16Net(u8* values, ssize* size, u16 value)
{
    if (values == PX_NULL || size == PX_NULL || *size < sizeof value)
        return PX_NULL;

    pxMemoryCopy(values, sizeof value, &value);

    if (pxHostByteOrderIsReverse() != 0)
        pxMemoryReverse(values, sizeof value);

    *size  -= sizeof value;
    values += sizeof value;

    return values;
}

u8*
connMemoryWriteU32Net(u8* values, ssize* size, u32 value)
{
    if (values == PX_NULL || size == PX_NULL || *size < sizeof value)
        return PX_NULL;

    pxMemoryCopy(values, sizeof value, &value);

    if (pxHostByteOrderIsReverse() != 0)
        pxMemoryReverse(values, sizeof value);

    *size  -= sizeof value;
    values += sizeof value;

    return values;
}

u8*
connMemoryWriteU64Net(u8* values, ssize* size, u64 value)
{
    if (values == PX_NULL || size == PX_NULL || *size < sizeof value)
        return PX_NULL;

    pxMemoryCopy(values, sizeof value, &value);

    if (pxHostByteOrderIsReverse() != 0)
        pxMemoryReverse(values, sizeof value);

    *size  -= sizeof value;
    values += sizeof value;

    return values;
}

#endif // CONN_MEMORY_C
