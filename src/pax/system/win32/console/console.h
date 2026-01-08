#ifndef PX_WIN32_CONSOLE_CONSOLE_H
#define PX_WIN32_CONSOLE_CONSOLE_H

#include "import.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

typedef struct PxWin32Console
{
    PxConsoleMode mode;

    DWORD conf_in;
    DWORD conf_out;
}
PxWin32Console;

PxWin32Console* pxWin32ConsoleReserve(PxMemoryArena* arena);

b32 pxWin32ConsoleCreate(PxWin32Console* self);

void pxWin32ConsoleDestroy(PxWin32Console* self);

b32 pxWin32ConsoleModeSet(PxWin32Console* self, PxConsoleMode mode);

PxConsoleMode pxWin32ConsoleModeGet(PxWin32Console* self);

ssize pxWin32ConsoleWrite(PxWin32Console* self, u8* pntr, ssize start, ssize stop);

ssize pxWin32ConsoleRead(PxWin32Console* self, u8* pntr, ssize start, ssize stop);

b32 pxWin32ConsolePollEvent(PxWin32Console* self, PxConsoleEvent* event);

#endif // PX_WIN32_CONSOLE_CONSOLE_H
