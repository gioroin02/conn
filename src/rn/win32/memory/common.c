#ifndef RN_WIN32_MEMORY_COMMON_C
#define RN_WIN32_MEMORY_COMMON_C

#include "./common.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

ssize
rnWin32MemoryPageSize()
{
    SYSTEM_INFO info = {0};

    GetSystemInfo(&info);

    return ((ssize) info.dwPageSize);
}

RnMemoryArena
rnWin32MemoryReserve(ssize size)
{
    RnMemoryArena result = {0};

    ssize page   = rnWin32MemoryPageSize();
    void* memory = 0;

    size = rnMemoryAlignForward(size, page);

    if (size <= 0) return result;

    memory = VirtualAlloc(0, size,
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (memory != 0)
        result = rnMemoryArenaMake(memory, size);

    return result;
}

void
rnWin32MemoryRelease(RnMemoryArena value)
{
    if (value.values != 0)
        VirtualFree(value.values, 0, MEM_RELEASE);
}

#endif // RN_WIN32_MEMORY_COMMON_C
