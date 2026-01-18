#ifndef P_SYSTEM_WIN32_MEMORY_COMMON_H
#define P_SYSTEM_WIN32_MEMORY_COMMON_H

#include "import.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI

#include <windows.h>

Int pWin32MemoryPageSize();

PMemoryArena pWin32MemoryReserve(Int size);

Bool pWin32MemoryRelease(PMemoryArena* arena);

#endif // P_SYSTEM_WIN32_MEMORY_COMMON_H
