#ifndef PX_SYSTEM_MEMORY_COMMON_C
#define PX_SYSTEM_MEMORY_COMMON_C

#include "common.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../win32/memory/export.c"

    #define __pxSystemMemoryPageSize__ pxWin32MemoryPageSize
    #define __pxSystemMemoryReserve__  pxWin32MemoryReserve
    #define __pxSystemMemoryRelease__  pxWin32MemoryRelease

#elif PX_SYSTEM == PX_SYSTEM_LINUX

    #include "../linux/memory/export.c"

    #define __pxSystemMemoryPageSize__ pxLinuxMemoryPageSize
    #define __pxSystemMemoryReserve__  pxLinuxMemoryReserve
    #define __pxSystemMemoryRelease__  pxLinuxMemoryRelease

#else

    #error "Unknown platform"

#endif

ssize
pxSystemMemoryPageSize()
{
    return __pxSystemMemoryPageSize__();
}

PxMemoryArena
pxSystemMemoryReserve(ssize size)
{
    return __pxSystemMemoryReserve__(size);
}

b32
pxSystemMemoryRelease(PxMemoryArena* arena)
{
    return __pxSystemMemoryRelease__(arena);
}

#endif // PX_SYSTEM_MEMORY_COMMON_C
