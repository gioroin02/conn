#ifndef PX_SYSTEM_CONSOLE_CONSOLE_C
#define PX_SYSTEM_CONSOLE_CONSOLE_C

#include "console.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../win32/console/export.c"

    #define __pxConsoleReserve__   pxWin32ConsoleReserve
    #define __pxConsoleCreate__    pxWin32ConsoleCreate
    #define __pxConsoleDestroy__   pxWin32ConsoleDestroy
    #define __pxConsoleModeSet__   pxWin32ConsoleModeSet
    #define __pxConsoleModeGet__   pxWin32ConsoleModeGet
    #define __pxConsoleWrite__     pxWin32ConsoleWrite
    #define __pxConsoleRead__      pxWin32ConsoleRead
    #define __pxConsolePollEvent__ pxWin32ConsolePollEvent

#else

    #error "Unknown platform"

#endif

PxConsole* pxConsoleReserve(PxMemoryArena* arena)
{
    return __pxConsoleReserve__(arena);
}

b32 pxConsoleCreate(PxConsole* self)
{
    return __pxConsoleCreate__(self);
}

void pxConsoleDestroy(PxConsole* self)
{
    return __pxConsoleDestroy__(self);
}

b32 pxConsoleModeSet(PxConsole* self, PxConsoleMode mode)
{
    return __pxConsoleModeSet__(self, mode);
}

PxConsoleMode pxConsoleModeGet(PxConsole* self)
{
    return __pxConsoleModeGet__(self);
}

ssize pxConsoleWrite(PxConsole* self, u8* pntr, ssize start, ssize stop)
{
    return __pxConsoleWrite__(self, pntr, start, stop);
}

ssize pxConsoleRead(PxConsole* self, u8* pntr, ssize start, ssize stop)
{
    return __pxConsoleRead__(self, pntr, start, stop);
}

b32 pxConsolePollEvent(PxConsole* self, PxConsoleEvent* event)
{
    return __pxConsolePollEvent__(self, event);
}

#endif // PX_SYSTEM_CONSOLE_CONSOLE_C
