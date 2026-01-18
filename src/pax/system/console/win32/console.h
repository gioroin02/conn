#ifndef P_SYSTEM_WIN32_CONSOLE_CONSOLE_H
#define P_SYSTEM_WIN32_CONSOLE_CONSOLE_H

#include "import.h"

#define WIN32_LEAN_AND_MEAN
#define NOGDI

#include <windows.h>

typedef struct PWin32Console
{
    PConsoleMode mode;

    Int conf_in;
    Int conf_out;
}
PWin32Console;

PWin32Console* pWin32ConsoleReserve(PMemoryArena* arena);

Bool pWin32ConsoleCreate(PWin32Console* self);

void pWin32ConsoleDestroy(PWin32Console* self);

Bool pWin32ConsoleModeSet(PWin32Console* self, PConsoleMode mode);

PConsoleMode pWin32ConsoleModeGet(PWin32Console* self);

Int pWin32ConsoleWrite(PWin32Console* self, U8* pntr, Int start, Int stop);

Int pWin32ConsoleRead(PWin32Console* self, U8* pntr, Int start, Int stop);

Bool pWin32ConsolePollEvent(PWin32Console* self, PConsoleEvent* event);

#endif // P_SYSTEM_WIN32_CONSOLE_CONSOLE_H
