#ifndef RN_WIN32_MEMORY_COMMON_H
#define RN_WIN32_MEMORY_COMMON_H

#include "./import.h"

ssize
rnWin32MemoryPageSize();

RnMemoryArena
rnWin32MemoryReserve(ssize size);

void
rnWin32MemoryRelease(RnMemoryArena value);

#endif // RN_WIN32_MEMORY_COMMON_H
