#ifndef P_BASE_MEMORY_COMMON_H
#define P_BASE_MEMORY_COMMON_H

#include "import.h"

#define P_MEMORY_DEFAULT_ALIGNMENT ((Int) 16)

#define P_MEMORY_KB ((Int) 1000ll)
#define P_MEMORY_MB ((Int) 1000000ll)
#define P_MEMORY_GB ((Int) 1000000000ll)

#define pMemoryKB(value) (value < 0 || value > P_INT_MAX / P_MEMORY_KB ? 0 : value * P_MEMORY_KB)
#define pMemoryMB(value) (value < 0 || value > P_INT_MAX / P_MEMORY_MB ? 0 : value * P_MEMORY_MB)
#define pMemoryGB(value) (value < 0 || value > P_INT_MAX / P_MEMORY_GB ? 0 : value * P_MEMORY_GB)

#define P_MEMORY_KIB ((Int) 1024ll)
#define P_MEMORY_MIB ((Int) 1048576ll)
#define P_MEMORY_GIB ((Int) 1073741824ll)

#define pMemoryKIB(value) (value < 0 || value > P_INT_MAX / P_MEMORY_KIB ? 0 : value * P_MEMORY_KIB)
#define pMemoryMIB(value) (value < 0 || value > P_INT_MAX / P_MEMORY_MIB ? 0 : value * P_MEMORY_MIB)
#define pMemoryGIB(value) (value < 0 || value > P_INT_MAX / P_MEMORY_GIB ? 0 : value * P_MEMORY_GIB)

#define pHostByteOrderIsNetwork() (pGetHostByteOrder() == PByteOrder_Network ? 1 : 0)
#define pHostByteOrderIsReverse() (pGetHostByteOrder() == PByteOrder_Reverse ? 1 : 0)

typedef enum PByteOrder
{
    PByteOrder_None,
    PByteOrder_Network,
    PByteOrder_Reverse,
}
PByteOrder;

PByteOrder pGetHostByteOrder();

void* pMemorySet(void* pntr, Int size, U8 value);

void* pMemoryCopy(void* pntr, Int size, void* value);

void* pMemoryCopyOrSet(void* pntr, Int size, void* value, U8 other);

void* pMemoryShiftForw(void* pntr, Int size, Int about, U8 value);

void* pMemoryShiftBack(void* pntr, Int size, Int about, U8 value);

void* pMemoryReverse(void* pntr, Int size);

void* pMemoryAlignPntr(void* pntr, Uint align);

Int pMemoryAlignSize(Int size, Uint align);

#endif // P_BASE_MEMORY_COMMON_H
