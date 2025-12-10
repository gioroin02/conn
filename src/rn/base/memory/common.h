#ifndef RN_BASE_MEMORY_COMMON_H
#define RN_BASE_MEMORY_COMMON_H

#include "./import.h"

#define RN_MEMORY_DEFAULT_ALIGNMENT ((ssize) 16)

#define RN_MEMORY_KB ((ssize) 1000ll)
#define RN_MEMORY_MB ((ssize) 1000ll * 1000ll)
#define RN_MEMORY_GB ((ssize) 1000ll * 1000ll * 1000ll)

#define RN_MEMORY_KIB ((ssize) 1024ll)
#define RN_MEMORY_MIB ((ssize) 1024ll * 1024ll)
#define RN_MEMORY_GIB ((ssize) 1024ll * 1024ll * 1024ll)

#define rnMemoryKB(expr) (((ssize) expr) * RN_MEMORY_KB)
#define rnMemoryMB(expr) (((ssize) expr) * RN_MEMORY_MB)
#define rnMemoryGB(expr) (((ssize) expr) * RN_MEMORY_GB)

#define rnMemoryKiB(expr) (((ssize) expr) * RN_MEMORY_KIB)
#define rnMemoryMiB(expr) (((ssize) expr) * RN_MEMORY_MIB)
#define rnMemoryGiB(expr) (((ssize) expr) * RN_MEMORY_GIB)

#define rnHostByteOrderIsNetwork() \
    (rnGetHostByteOrder() == RnByteOrder_Network ? 1 : 0)

#define rnHostByteOrderIsReverse() \
    (rnGetHostByteOrder() == RnByteOrder_Reverse ? 1 : 0)

typedef enum RnByteOrder
{
    RnByteOrder_None,
    RnByteOrder_Network,
    RnByteOrder_Reverse,
}
RnByteOrder;

ssize
rnMemoryAlignForward(ssize value, ssize align);

RnByteOrder
rnGetHostByteOrder();

#endif // RN_BASE_MEMORY_COMMON_H
