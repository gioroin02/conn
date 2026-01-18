#ifndef P_SYSTEM_CONSOLE_CONSOLE_C
#define P_SYSTEM_CONSOLE_CONSOLE_C

#include "console.h"

#if P_SYSTEM == P_SYSTEM_WINDOWS

    #include "win32/export.c"

    #define __PConsole__ PWin32Console

    #define __pConsoleReserve__   pWin32ConsoleReserve
    #define __pConsoleCreate__    pWin32ConsoleCreate
    #define __pConsoleDestroy__   pWin32ConsoleDestroy
    #define __pConsoleModeSet__   pWin32ConsoleModeSet
    #define __pConsoleModeGet__   pWin32ConsoleModeGet
    #define __pConsoleWrite__     pWin32ConsoleWrite
    #define __pConsoleRead__      pWin32ConsoleRead
    #define __pConsolePollEvent__ pWin32ConsolePollEvent

#else

    #error "Unknown platform"

#endif

PConsole* pConsoleReserve(PMemoryArena* arena)
{
    return (PConsole*) __pConsoleReserve__(arena);
}

Bool pConsoleCreate(PConsole* self)
{
    return __pConsoleCreate__((__PConsole__*) self);
}

void pConsoleDestroy(PConsole* self)
{
    return __pConsoleDestroy__((__PConsole__*) self);
}

Bool pConsoleModeSet(PConsole* self, PConsoleMode mode)
{
    return __pConsoleModeSet__((__PConsole__*) self, mode);
}

PConsoleMode pConsoleModeGet(PConsole* self)
{
    return __pConsoleModeGet__((__PConsole__*) self);
}

Int pConsoleWrite(PConsole* self, U8* pntr, Int start, Int stop)
{
    return __pConsoleWrite__((__PConsole__*) self, pntr, start, stop);
}

Int pConsoleRead(PConsole* self, U8* pntr, Int start, Int stop)
{
    return __pConsoleRead__((__PConsole__*) self, pntr, start, stop);
}

Bool pConsolePollEvent(PConsole* self, PConsoleEvent* event)
{
    return __pConsolePollEvent__((__PConsole__*) self, event);
}

#endif // P_SYSTEM_CONSOLE_CONSOLE_C
