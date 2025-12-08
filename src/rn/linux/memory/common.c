#ifndef RN_LINUX_MEMORY_COMMON_C
#define RN_LINUX_MEMORY_COMMON_C

#include "./common.h"

#define _DEFAULT_SOURCE

#include <errno.h>
#include <unistd.h>

#include <sys/mman.h>

ssize
rnLinuxMemoryPageSize()
{
    return ((ssize) sysconf(_SC_PAGESIZE));
}

RnMemoryArena
rnLinuxMemoryReserve(ssize size)
{
    RnMemoryArena result = {0};

    ssize page   = rnLinuxMemoryPageSize();
    void* memory = 0;

    size = rnMemoryAlignForward(size, page);

    if (size <= 0) return result;

    do {
        memory = mmap(0, size, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
    while (memory == MAP_FAILED && errno == EINTR);

    if (memory != MAP_FAILED)
        result = rnMemoryArenaMake(memory, size);

    return result;
}

void
rnLinuxMemoryRelease(RnMemoryArena value)
{
    if (value.values == 0) return;

    int status = 0;

    do {
        status = munmap(value.values, value.size);
    }
    while (status == -1 && errno == EINTR);
}

#endif // RN_LINUX_MEMORY_COMMON_C
