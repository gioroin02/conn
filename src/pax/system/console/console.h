#ifndef PX_SYSTEM_CONSOLE_CONSOLE_H
#define PX_SYSTEM_CONSOLE_CONSOLE_H

#include "event.h"

typedef enum PxConsoleMode
{
    PxConsoleMode_None,
    PxConsoleMode_Cooked,
    PxConsoleMode_Raw,
}
PxConsoleMode;

typedef void PxConsole;

PxConsole* pxConsoleReserve(PxMemoryArena* arena);

b32 pxConsoleCreate(PxConsole* self);

void pxConsoleDestroy(PxConsole* self);

b32 pxConsoleModeSet(PxConsole* self, PxConsoleMode mode);

PxConsoleMode pxConsoleModeGet(PxConsole* self);

ssize pxConsoleWrite(PxConsole* self, u8* pntr, ssize start, ssize stop);

ssize pxConsoleRead(PxConsole* self, u8* pntr, ssize start, ssize stop);

b32 pxConsolePollEvent(PxConsole* self, PxConsoleEvent* event);

#endif // PX_SYSTEM_CONSOLE_CONSOLE_H
