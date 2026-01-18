#ifndef P_SYSTEM_MEMORY_COMMON_C
#define P_SYSTEM_MEMORY_COMMON_C

#include "common.h"

#if P_SYSTEM == P_SYSTEM_WINDOWS

    #include "win32/export.c"

    #define __pSystemMemoryPageSize__ pWin32MemoryPageSize
    #define __pSystemMemoryReserve__  pWin32MemoryReserve
    #define __pSystemMemoryRelease__  pWin32MemoryRelease

#elif P_SYSTEM == P_SYSTEM_LINUX

    #include "linux/export.c"

    #define __pSystemMemoryPageSize__ pLinuxMemoryPageSize
    #define __pSystemMemoryReserve__  pLinuxMemoryReserve
    #define __pSystemMemoryRelease__  pLinuxMemoryRelease

#else

    #error "Unknown platform"

#endif

Int pSystemMemoryPageSize()
{
    return __pSystemMemoryPageSize__();
}

PMemoryArena pSystemMemoryReserve(Int size)
{
    return __pSystemMemoryReserve__(size);
}

Bool pSystemMemoryRelease(PMemoryArena* arena)
{
    return __pSystemMemoryRelease__(arena);
}

#endif // P_SYSTEM_MEMORY_COMMON_C
