#ifndef PX_BASE_MEMORY_COMMON_C
#define PX_BASE_MEMORY_COMMON_C

#include "common.h"

void*
pxMemorySet(void* pntr, ssize size, u8 value)
{
    ssize index = 0;

    if (pntr == PX_NULL) return PX_NULL;

    for (index = 0; index < size; index += 1)
        ((u8*) pntr)[index] = value;

    return pntr;
}

void*
pxMemoryCopy(void* pntr, ssize size, void* value)
{
    ssize index = 0;

    if (pntr == PX_NULL || value == PX_NULL) return PX_NULL;

    for (index = 0; index < size; index += 1)
        ((u8*) pntr)[index] = ((u8*) value)[index];

    return pntr;
}

void*
pxMemoryCopyOrSet(void* pntr, ssize size, void* value, u8 other)
{
    if (pntr == PX_NULL) return PX_NULL;

    if (value == PX_NULL)
        return pxMemorySet(pntr, size, other);

    return pxMemoryCopy(pntr, size, value);
}

void*
pxMemoryShiftForw(void* pntr, ssize size, ssize about, u8 value)
{
    ssize index = 0;

    if (pntr == PX_NULL || size < 0 || about < 0 || about >= size)
        return pntr;

    for (index = size; index > about; index -= 1)
        ((u8*) pntr)[index - 1] = ((u8*) pntr)[index - about - 1];

    for (index = about; index > 0; index -= 1)
        ((u8*) pntr)[index - 1] = value;

    return pntr;
}

void*
pxMemoryShiftBack(void* pntr, ssize size, ssize about, u8 value)
{
    ssize index = 0;

    if (pntr == PX_NULL || size < 0 || about < 0 || about >= size)
        return pntr;

    for (index = 0; index < size - about; index += 1)
        ((u8*) pntr)[index] = ((u8*) pntr)[index + about];

    for (index = size - about; index < size; index += 1)
        ((u8*) pntr)[index] = value;

    return pntr;
}

void*
pxMemoryAlignPntrForw(void* pntr, usize align)
{
    usize dist = ((usize) pntr) % align;

    if (pntr == PX_NULL) return PX_NULL;

    if (dist != 0)
        return ((u8*) pntr) + (align - dist);

    return pntr;
}

void*
pxMemoryAlignPntrBack(void* pntr, usize align)
{
    usize dist = ((usize) pntr) % align;

    if (pntr == PX_NULL) return PX_NULL;

    if (dist != 0)
        return ((u8*) pntr) - dist;

    return pntr;
}

ssize
pxMemoryAlignSizeForw(ssize size, usize align)
{
    usize dist = ((usize) size) % align;

    if (size <= 0) return 0;

    if (dist != 0)
        return size + (align - dist);

    return size;
}

ssize
pxMemoryAlignSizeBack(ssize size, usize align)
{
    usize dist = ((usize) size) % align;

    if (size <= 0) return 0;

    if (dist != 0)
        return size - dist;

    return size;
}

PxByteOrder
pxGetHostByteOrder()
{
    u32 value = 1;

    u8 bytes[sizeof value];

    pxMemoryCopy(bytes, sizeof value, &value);

    if (bytes[0] == 1)
        return PxByteOrder_Reverse;

    return PxByteOrder_Network;
}

#endif // PX_BASE_MEMORY_COMMON_C
