#ifndef P_SYSTEM_CONSOLE_CONSOLE_H
#define P_SYSTEM_CONSOLE_CONSOLE_H

#include "event.h"

typedef enum PConsoleMode
{
    PConsoleMode_None,
    PConsoleMode_Cooked,
    PConsoleMode_Raw,
}
PConsoleMode;

typedef struct PConsole { U8 __private__; } PConsole;

PConsole* pConsoleReserve(PMemoryArena* arena);

Bool pConsoleCreate(PConsole* self);

void pConsoleDestroy(PConsole* self);

Bool pConsoleModeSet(PConsole* self, PConsoleMode mode);

PConsoleMode pConsoleModeGet(PConsole* self);

Int pConsoleWrite(PConsole* self, U8* pntr, Int start, Int stop);

Int pConsoleRead(PConsole* self, U8* pntr, Int start, Int stop);

Bool pConsolePollEvent(PConsole* self, PConsoleEvent* event);

#endif // P_SYSTEM_CONSOLE_CONSOLE_H
