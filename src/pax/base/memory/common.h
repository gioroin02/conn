#ifndef PX_BASE_MEMORY_COMMON_H
#define PX_BASE_MEMORY_COMMON_H

#include "import.h"

#define PX_MEMORY_DEFAULT_ALIGNMENT ((ssize) 16)

#define PX_MEMORY_KB ((ssize) 1000ll)
#define PX_MEMORY_MB (PX_MEMORY_KB * PX_MEMORY_KB)
#define PX_MEMORY_GB (PX_MEMORY_MB * PX_MEMORY_KB)

#define PX_MEMORY_KIB ((ssize) 1024ll)
#define PX_MEMORY_MIB (PX_MEMORY_KIB * PX_MEMORY_KIB)
#define PX_MEMORY_GIB (PX_MEMORY_MIB * PX_MEMORY_KIB)

#define pxMemoryKB(expr) (((ssize) expr) * PX_MEMORY_KB)
#define pxMemoryMB(expr) (((ssize) expr) * PX_MEMORY_MB)
#define pxMemoryGB(expr) (((ssize) expr) * PX_MEMORY_GB)

#define pxMemoryKiB(expr) (((ssize) expr) * PX_MEMORY_KIB)
#define pxMemoryMiB(expr) (((ssize) expr) * PX_MEMORY_MIB)
#define pxMemoryGiB(expr) (((ssize) expr) * PX_MEMORY_GIB)

#define pxHostByteOrderIsNetwork() \
    (pxGetHostByteOrder() == PxByteOrder_Network ? 1 : 0)

#define pxHostByteOrderIsReverse() \
    (pxGetHostByteOrder() == PxByteOrder_Reverse ? 1 : 0)

typedef enum PxByteOrder
{
    PxByteOrder_None,
    PxByteOrder_Network,
    PxByteOrder_Reverse,
}
PxByteOrder;

void*
pxMemorySet(void* pntr, ssize size, u8 value);

void*
pxMemoryCopy(void* pntr, ssize size, void* value);

void*
pxMemoryCopyOrSet(void* pntr, ssize size, void* value, u8 other);

void*
pxMemoryShiftForw(void* pntr, ssize size, ssize about, u8 value);

void*
pxMemoryShiftBack(void* pntr, ssize size, ssize about, u8 value);

void*
pxMemoryAlignPntrForw(void* pntr, usize align);

void*
pxMemoryAlignPntrBack(void* pntr, usize align);

ssize
pxMemoryAlignSizeForw(ssize size, usize align);

ssize
pxMemoryAlignSizeBack(ssize size, usize align);

PxByteOrder
pxGetHostByteOrder();

#endif // PX_BASE_MEMORY_COMMON_H
