#ifndef PX_BASE_MEMORY_COMMON_C
#define PX_BASE_MEMORY_COMMON_C

#include "common.h"

ssize
pxMemoryKiB(ssize size)
{
    if (size < 0 || size > PX_MAX_SSIZE / PX_MEMORY_KIB)
        return 0;

    return size * PX_MEMORY_KIB;
}

ssize
pxMemoryMiB(ssize size)
{
    if (size < 0 || size > PX_MAX_SSIZE / PX_MEMORY_MIB)
        return 0;

    return size * PX_MEMORY_MIB;
}

ssize
pxMemoryGiB(ssize size)
{
    if (size < 0 || size > PX_MAX_SSIZE / PX_MEMORY_GIB)
        return 0;

    return size * PX_MEMORY_GIB;
}

PxByteOrder
pxGetHostByteOrder()
{
    u8 bytes[sizeof (u16)];

    u16 value = 1;

    pxMemoryCopy(bytes, sizeof bytes, &value);

    if (bytes[0] == 1)
        return PxByteOrder_Reverse;

    return PxByteOrder_Network;
}

void*
pxMemorySet(void* pntr, ssize size, u8 value)
{
    if (pntr == PX_NULL) return PX_NULL;

    ssize index = 0;

    for (index = 0; index < size; index += 1)
        ((u8*) pntr)[index] = value;

    return pntr;
}

void*
pxMemoryCopy(void* pntr, ssize size, void* value)
{
    if (pntr == PX_NULL || value == PX_NULL) return PX_NULL;

    ssize index = 0;

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
    if (pntr == PX_NULL || size < 0 || about < 0 || about >= size)
        return pntr;

    ssize index = 0;

    for (index = size; index > about; index -= 1)
        ((u8*) pntr)[index - 1] = ((u8*) pntr)[index - about - 1];

    for (index = about; index > 0; index -= 1)
        ((u8*) pntr)[index - 1] = value;

    return pntr;
}

void*
pxMemoryShiftBack(void* pntr, ssize size, ssize about, u8 value)
{
    if (pntr == PX_NULL || size < 0 || about < 0 || about >= size)
        return pntr;

    ssize index = 0;

    for (index = 0; index < size - about; index += 1)
        ((u8*) pntr)[index] = ((u8*) pntr)[index + about];

    for (index = size - about; index < size; index += 1)
        ((u8*) pntr)[index] = value;

    return pntr;
}

void*
pxMemoryReverse(void* pntr, ssize size)
{
    if (pntr == PX_NULL || size <= 0) return PX_NULL;

    ssize index = 0;
    ssize other = 0;

    for (other = size - 1; index < other; index += 1, other -= 1) {
        u8 temp = ((u8*) pntr)[index];

        ((u8*) pntr)[index] = ((u8*) pntr)[other];
        ((u8*) pntr)[other] = temp;
    }

    return pntr;
}

void*
pxMemoryAlignPntr(void* pntr, usize align)
{
    if (pntr == PX_NULL || (align & (align - 1)) != 0)
        return PX_NULL;

    usize dist = ((usize) pntr) % align;

    if (dist != 0)
        return ((u8*) pntr) + (align - dist);

    return pntr;
}

ssize
pxMemoryAlignSize(ssize size, usize align)
{
    if (size <= 0 || (align & (align - 1)) != 0)
        return 0;

    usize dist = ((usize) size) % align;

    if (dist != 0)
        return size + (align - dist);

    return size;
}

#endif // PX_BASE_MEMORY_COMMON_C
