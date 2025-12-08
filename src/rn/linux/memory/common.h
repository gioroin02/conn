#ifndef RN_LINUX_MEMORY_COMMON_H
#define RN_LINUX_MEMORY_COMMON_H

#include "./import.h"

ssize
rnLinuxMemoryPageSize();

RnMemoryArena
rnLinuxMemoryReserve(ssize size);

void
rnLinuxMemoryRelease(RnMemoryArena value);

#endif // RN_LINUX_MEMORY_COMMON_H
