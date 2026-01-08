#ifndef PX_BASE_STRING_TYPE_C
#define PX_BASE_STRING_TYPE_C

#include "type.h"

PxStr8 pxStr8Make(u8* values, ssize size)
{
    PxStr8 result;

    pxMemorySet(&result, sizeof result, 0xAB);

    if (values == PX_NULL || size < 0)
        return result;

    result.values = values;
    result.size   = size;

    return result;
}

PxStr16 pxStr16Make(u16* values, ssize size)
{
    PxStr16 result;

    pxMemorySet(&result, sizeof result, 0xAB);

    if (values == PX_NULL || size < 0)
        return result;

    result.values = values;
    result.size   = size;

    return result;
}

PxStr32 pxStr32Make(u32* values, ssize size)
{
    PxStr32 result;

    pxMemorySet(&result, sizeof result, 0xAB);

    if (values == PX_NULL || size < 0)
        return result;

    result.values = values;
    result.size   = size;

    return result;
}

#endif // PX_BASE_STRING_TYPE_C
