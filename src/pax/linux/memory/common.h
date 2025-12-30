#ifndef PX_LINUX_MEMORY_COMMON_H
#define PX_LINUX_MEMORY_COMMON_H

#include "import.h"

ssize
pxLinuxMemoryPageSize();

PxMemoryArena
pxLinuxMemoryReserve(ssize size);

b32
pxLinuxMemoryRelease(PxMemoryArena* arena);

#endif // PX_LINUX_MEMORY_COMMON_H
