#ifndef P_BASE_MEMORY_COMMON_C
#define P_BASE_MEMORY_COMMON_C

#include "common.h"

PByteOrder pGetHostByteOrder()
{
    static U16 value = 1;

    U8 bytes[sizeof value];

    pMemoryCopy(bytes, sizeof value, &value);

    if (bytes[0] == 1)
        return PByteOrder_Reverse;

    return PByteOrder_Network;
}

void* pMemorySet(void* pntr, Int size, U8 value)
{
    if (pntr == NULL) return NULL;

    Int index = 0;

    for (index = 0; index < size; index += 1)
        ((U8*) pntr)[index] = value;

    return pntr;
}

void* pMemoryCopy(void* pntr, Int size, void* value)
{
    if (pntr == NULL || value == NULL) return NULL;

    Int index = 0;

    for (index = 0; index < size; index += 1)
        ((U8*) pntr)[index] = ((U8*) value)[index];

    return pntr;
}

void* pMemoryCopyOrSet(void* pntr, Int size, void* value, U8 other)
{
    if (pntr == NULL) return NULL;

    if (value == NULL)
        return pMemorySet(pntr, size, other);

    return pMemoryCopy(pntr, size, value);
}

void* pMemoryShiftForw(void* pntr, Int size, Int about, U8 value)
{
    if (pntr == NULL || size < 0 || about < 0 || about >= size)
        return pntr;

    Int index = 0;

    for (index = size; index > about; index -= 1)
        ((U8*) pntr)[index - 1] = ((U8*) pntr)[index - about - 1];

    for (index = about; index > 0; index -= 1)
        ((U8*) pntr)[index - 1] = value;

    return pntr;
}

void* pMemoryShiftBack(void* pntr, Int size, Int about, U8 value)
{
    if (pntr == NULL || size < 0 || about < 0 || about >= size)
        return pntr;

    Int index = 0;

    for (index = 0; index < size - about; index += 1)
        ((U8*) pntr)[index] = ((U8*) pntr)[index + about];

    for (index = size - about; index < size; index += 1)
        ((U8*) pntr)[index] = value;

    return pntr;
}

void* pMemoryReverse(void* pntr, Int size)
{
    if (pntr == NULL || size <= 0) return NULL;

    Int index = 0;
    Int other = 0;

    for (other = size - 1; index < other; index += 1, other -= 1) {
        U8 temp = ((U8*) pntr)[index];

        ((U8*) pntr)[index] = ((U8*) pntr)[other];
        ((U8*) pntr)[other] = temp;
    }

    return pntr;
}

void* pMemoryAlignPntr(void* pntr, Uint align)
{
    if (pntr == NULL || (align & (align - 1)) != 0)
        return NULL;

    Uint dist = ((Uint) pntr) % align;

    if (dist != 0)
        return ((U8*) pntr) + (align - dist);

    return pntr;
}

Int pMemoryAlignSize(Int size, Uint align)
{
    if (size <= 0 || (align & (align - 1)) != 0)
        return 0;

    Uint dist = ((Uint) size) % align;

    if (dist != 0)
        return size + (align - dist);

    return size;
}

#endif // P_BASE_MEMORY_COMMON_C
