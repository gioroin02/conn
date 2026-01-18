#ifndef P_SYSTEM_MEMORY_COMMON_H
#define P_SYSTEM_MEMORY_COMMON_H

#include "import.h"

Int pSystemMemoryPageSize();

PMemoryArena pSystemMemoryReserve(Int size);

Bool pSystemMemoryRelease(PMemoryArena* arena);

#endif // P_SYSTEM_MEMORY_COMMON_H
