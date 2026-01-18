#ifndef CONN_MEMORY_C
#define CONN_MEMORY_C

#include "memory.h"

U8* connMemoryWriteU8Net(U8* values, Int* size, U8 value)
{
    if (values == NULL || size == NULL || *size < sizeof value)
        return NULL;

    pMemoryCopy(values, sizeof value, &value);

    if (pHostByteOrderIsReverse() != 0)
        pMemoryReverse(values, sizeof value);

    *size  -= sizeof value;
    values += sizeof value;

    return values;
}

U8* connMemoryWriteU16Net(U8* values, Int* size, U16 value)
{
    if (values == NULL || size == NULL || *size < sizeof value)
        return NULL;

    pMemoryCopy(values, sizeof value, &value);

    if (pHostByteOrderIsReverse() != 0)
        pMemoryReverse(values, sizeof value);

    *size  -= sizeof value;
    values += sizeof value;

    return values;
}

U8* connMemoryWriteU32Net(U8* values, Int* size, U32 value)
{
    if (values == NULL || size == NULL || *size < sizeof value)
        return NULL;

    pMemoryCopy(values, sizeof value, &value);

    if (pHostByteOrderIsReverse() != 0)
        pMemoryReverse(values, sizeof value);

    *size  -= sizeof value;
    values += sizeof value;

    return values;
}

U8* connMemoryWriteU64Net(U8* values, Int* size, U64 value)
{
    if (values == NULL || size == NULL || *size < sizeof value)
        return NULL;

    pMemoryCopy(values, sizeof value, &value);

    if (pHostByteOrderIsReverse() != 0)
        pMemoryReverse(values, sizeof value);

    *size  -= sizeof value;
    values += sizeof value;

    return values;
}

U8* connMemoryReadU8Net(U8* values, Int* size, U8* value)
{
    if (values == NULL || size == NULL || *size < sizeof *value)
        return NULL;

    pMemoryCopy(value, sizeof *value, values);

    if (pHostByteOrderIsReverse() != 0)
        pMemoryReverse(value, sizeof * value);

    *size  -= sizeof *value;
    values += sizeof *value;

    return values;
}

U8* connMemoryReadU16Net(U8* values, Int* size, U16* value)
{
    if (values == NULL || size == NULL || *size < sizeof *value)
        return NULL;

    pMemoryCopy(value, sizeof *value, values);

    if (pHostByteOrderIsReverse() != 0)
        pMemoryReverse(value, sizeof * value);

    *size  -= sizeof *value;
    values += sizeof *value;

    return values;
}

U8* connMemoryReadU32Net(U8* values, Int* size, U32* value)
{
    if (values == NULL || size == NULL || *size < sizeof *value)
        return NULL;

    pMemoryCopy(value, sizeof *value, values);

    if (pHostByteOrderIsReverse() != 0)
        pMemoryReverse(value, sizeof * value);

    *size  -= sizeof *value;
    values += sizeof *value;

    return values;
}

U8* connMemoryReadU64Net(U8* values, Int* size, U64* value)
{
    if (values == NULL || size == NULL || *size < sizeof *value)
        return NULL;

    pMemoryCopy(value, sizeof *value, values);

    if (pHostByteOrderIsReverse() != 0)
        pMemoryReverse(value, sizeof * value);

    *size  -= sizeof *value;
    values += sizeof *value;

    return values;
}

#endif // CONN_MEMORY_C
