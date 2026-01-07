#ifndef PX_SYSTEM_CONSOLE_COMMON_C
#define PX_SYSTEM_CONSOLE_COMMON_C

#include "common.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../win32/console/export.c"

    #define __pxConsoleReserve__ pxWin32ConsoleReserve
    #define __pxConsoleCreate__  pxWin32ConsoleCreate
    #define __pxConsoleDestroy__ pxWin32ConsoleDestroy
    #define __pxConsoleModeSet__ pxWin32ConsoleModeSet
    #define __pxConsoleModeGet__ pxWin32ConsoleModeGet

#else

    #error "Unknown platform"

#endif

PxConsole*
pxConsoleReserve(PxMemoryArena* arena)
{
    return __pxConsoleReserve__(arena);
}

b32
pxConsoleCreate(PxConsole* self)
{
    return __pxConsoleCreate__(self);
}

void
pxConsoleDestroy(PxConsole* self)
{
    return __pxConsoleDestroy__(self);
}

b32
pxConsoleModeSet(PxConsole* self, PxConsoleMode mode)
{
    return __pxConsoleModeSet__(self, mode);
}

PxConsoleMode
pxConsoleModeGet(PxConsole* self)
{
    return __pxConsoleModeGet__(self);
}

#endif // PX_SYSTEM_CONSOLE_COMMON_C
