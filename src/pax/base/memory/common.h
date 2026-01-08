#ifndef PX_BASE_MEMORY_COMMON_H
#define PX_BASE_MEMORY_COMMON_H

#include "import.h"

#define PX_MEMORY_DEFAULT_ALIGNMENT ((ssize) 16)

#define pxHostByteOrderIsNetwork() (pxGetHostByteOrder() == PxByteOrder_Network ? 1 : 0)
#define pxHostByteOrderIsReverse() (pxGetHostByteOrder() == PxByteOrder_Reverse ? 1 : 0)

typedef enum PxByteOrder
{
    PxByteOrder_None,
    PxByteOrder_Network,
    PxByteOrder_Reverse,
}
PxByteOrder;

PxByteOrder pxGetHostByteOrder();

void* pxMemorySet(void* pntr, ssize size, u8 value);

void* pxMemoryCopy(void* pntr, ssize size, void* value);

void* pxMemoryCopyOrSet(void* pntr, ssize size, void* value, u8 other);

void* pxMemoryShiftForw(void* pntr, ssize size, ssize about, u8 value);

void* pxMemoryShiftBack(void* pntr, ssize size, ssize about, u8 value);

void* pxMemoryReverse(void* pntr, ssize size);

void* pxMemoryAlignPntr(void* pntr, usize align);

ssize pxMemoryAlignSize(ssize size, usize align);

#endif // PX_BASE_MEMORY_COMMON_H
