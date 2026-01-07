#ifndef PX_SYSTEM_CONSOLE_COMMON_H
#define PX_SYSTEM_CONSOLE_COMMON_H

#include "import.h"

typedef enum PxConsoleMode
{
    PxConsoleMode_None,
    PxConsoleMode_Cooked,
    PxConsoleMode_Raw,
}
PxConsoleMode;

typedef void PxConsole;

PxConsole*
pxConsoleReserve(PxMemoryArena* arena);

b32
pxConsoleCreate(PxConsole* self);

void
pxConsoleDestroy(PxConsole* self);

b32
pxConsoleModeSet(PxConsole* self, PxConsoleMode mode);

PxConsoleMode
pxConsoleModeGet(PxConsole* self);

#endif // PX_SYSTEM_CONSOLE_COMMON_H
