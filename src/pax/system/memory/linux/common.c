#ifndef P_SYSTEM_LINUX_MEMORY_COMMON_C
#define P_SYSTEM_LINUX_MEMORY_COMMON_C

#include "common.h"

Int pLinuxMemoryPageSize()
{
    return (Int) sysconf(_SC_PAGESIZE);
}

PMemoryArena pLinuxMemoryReserve(Int size)
{
    PMemoryArena result;

    pMemorySet(&result, sizeof result, 0xAB);

    if (size <= 0) return result;

    Int   page   = pLinuxMemoryPageSize();
    void* memory = NULL;

    size = pMemoryAlignSize(size, page);

    do {
        memory = mmap(0, size, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
    while (memory == MAP_FAILED && errno == EINTR);

    if (memory == MAP_FAILED) return result;

    result = pMemoryArenaMake(memory, size);

    return result;
}

Bool pLinuxMemoryRelease(PMemoryArena* arena)
{
    int   status = 0;
    Int   page   = pLinuxMemoryPageSize();
    void* memory = pMemoryArenaPntr(arena);
    Int   size   = pMemoryArenaSize(arena);

    if (memory == NULL || size <= 0 || size % page != 0)
        return 0;

    do {
        status = munmap(memory, size);
    }
    while (status == -1 && errno == EINTR);

    pMemorySet(arena, sizeof *arena, 0xAB);

    return 1;
}

#endif // P_SYSTEM_LINUX_MEMORY_COMMON_C
