#ifndef PX_WIN32_MEMORY_COMMON_H
#define PX_WIN32_MEMORY_COMMON_H

#include "import.h"

ssize
pxWin32MemoryPageSize();

PxMemoryArena
pxWin32MemoryReserve(ssize size);

b32
pxWin32MemoryRelease(PxMemoryArena* arena);

#endif // PX_WIN32_MEMORY_COMMON_H
