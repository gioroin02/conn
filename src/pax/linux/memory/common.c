#ifndef PX_LINUX_MEMORY_COMMON_C
#define PX_LINUX_MEMORY_COMMON_C

#include "common.h"

#define _DEFAULT_SOURCE

#include <errno.h>
#include <unistd.h>

#include <sys/mman.h>

ssize
pxLinuxMemoryPageSize()
{
    return ((ssize) sysconf(_SC_PAGESIZE));
}

PxMemoryArena
pxLinuxMemoryReserve(ssize size)
{
    PxMemoryArena result = pxMemoryArenaMake(0, 0);
    ssize         page   = pxLinuxMemoryPageSize();
    void*         memory = PX_NULL;

    if (size <= 0) return result;

    size = pxMemoryAlignSizeForw(size, page);

    do {
        memory = mmap(0, size, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
    while (memory == MAP_FAILED && errno == EINTR);

    if (memory != MAP_FAILED)
        result = pxMemoryArenaMake(memory, size);

    return result;
}

b32
pxLinuxMemoryRelease(PxMemoryArena* arena)
{
    int   status = 0;
    ssize page   = pxLinuxMemoryPageSize();
    void* memory = pxMemoryArenaPntr(arena);
    ssize size   = pxMemoryArenaSize(arena);

    if (memory == PX_NULL || size <= 0 || size % page != 0)
        return 0;

    do
        status = munmap(memory, size);
    while (status == -1 && errno == EINTR);

    *arena = pxMemoryArenaMake(0, 0);

    return 1;
}

#endif // PX_LINUX_MEMORY_COMMON_C
