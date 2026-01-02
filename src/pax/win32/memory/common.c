#ifndef PX_WIN32_MEMORY_COMMON_C
#define PX_WIN32_MEMORY_COMMON_C

#include "common.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

ssize
pxWin32MemoryPageSize()
{
    SYSTEM_INFO info;

    GetSystemInfo(&info);

    return ((ssize) info.dwPageSize);
}

PxMemoryArena
pxWin32MemoryReserve(ssize size)
{
    PxMemoryArena result;

    pxMemorySet(&result, sizeof result, 0xAB);

    ssize page   = pxWin32MemoryPageSize();
    void* memory = PX_NULL;

    if (size <= 0) return result;

    size = pxMemoryAlignSizeForw(size, page);

    memory = VirtualAlloc(0, size,
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (memory != 0)
        result = pxMemoryArenaMake(memory, size);

    return result;
}

b32
pxWin32MemoryRelease(PxMemoryArena* arena)
{
    ssize page   = pxWin32MemoryPageSize();
    void* memory = pxMemoryArenaPntr(arena);
    ssize size   = pxMemoryArenaSize(arena);

    if (memory == PX_NULL || size <= 0 || size % page != 0)
        return 0;

    VirtualFree(arena, 0, MEM_RELEASE);

    pxMemorySet(arena, sizeof *arena, 0xAB);

    return 1;
}

#endif // PX_WIN32_MEMORY_COMMON_C
