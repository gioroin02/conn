#ifndef PX_BASE_STRING_STRING_C
#define PX_BASE_STRING_STRING_C

#include "string.h"

PxStr8
pxStr8Make(u8* values, ssize size)
{
    PxStr8 result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.values = values;
    result.size   = size;

    return result;
}

PxStr16
pxStr16Make(u16* values, ssize size)
{
    PxStr16 result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.values = values;
    result.size   = size;

    return result;
}

PxStr32
pxStr32Make(u32* values, ssize size)
{
    PxStr32 result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.values = values;
    result.size   = size;

    return result;
}

#endif // PX_BASE_STRING_STRING_C
