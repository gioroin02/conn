#ifndef RN_SYSTEM_MEMORY_COMMON_C
#define RN_SYSTEM_MEMORY_COMMON_C

#include "./common.h"

#if RN_SYSTEM == RN_SYSTEM_WINDOWS

    #include "../../win32/memory/export.c"

    #define __rnSystemMemoryPageSize__ rnWin32MemoryPageSize
    #define __rnSystemMemoryReserve__  rnWin32MemoryReserve
    #define __rnSystemMemoryRelease__  rnWin32MemoryRelease

#elif RN_SYSTEM == RN_SYSTEM_LINUX

    #include "../../linux/memory/export.c"

    #define __rnSystemMemoryPageSize__ rnLinuxMemoryPageSize
    #define __rnSystemMemoryReserve__  rnLinuxMemoryReserve
    #define __rnSystemMemoryRelease__  rnLinuxMemoryRelease

#else

    #error "Unknown platform"

#endif

ssize
rnSystemMemoryPageSize()
{
    return __rnSystemMemoryPageSize__();
}

RnMemoryArena
rnSystemMemoryReserve(ssize size)
{
    return __rnSystemMemoryReserve__(size);
}

void
rnSystemMemoryRelease(RnMemoryArena value)
{
    __rnSystemMemoryRelease__(value);
}

#endif // RN_SYSTEM_MEMORY_COMMON_C
