#ifndef PX_SYSTEM_MEMORY_COMMON_H
#define PX_SYSTEM_MEMORY_COMMON_H

#include "import.h"

ssize pxSystemMemoryPageSize();

PxMemoryArena pxSystemMemoryReserve(ssize size);

b32 pxSystemMemoryRelease(PxMemoryArena* arena);

#endif // PX_SYSTEM_MEMORY_COMMON_H
