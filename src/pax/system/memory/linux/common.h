#ifndef P_SYSTEM_LINUX_MEMORY_COMMON_H
#define P_SYSTEM_LINUX_MEMORY_COMMON_H

#include "import.h"

#define _DEFAULT_SOURCE

#include <unistd.h>
#include <errno.h>

#include <sys/mman.h>

Int pLinuxMemoryPageSize();

PMemoryArena pLinuxMemoryReserve(Int size);

Bool pLinuxMemoryRelease(PMemoryArena* arena);

#endif // P_SYSTEM_LINUX_MEMORY_COMMON_H
