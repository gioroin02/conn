#ifndef PX_WIN32_CONSOLE_COMMON_C
#define PX_WIN32_CONSOLE_COMMON_C

#include "common.h"

static b32
pxWin32ConsoleModeSetCooked(PxWin32Console* self)
{
    if (self->mode != PxConsoleMode_Cooked) {
        SetConsoleMode(self->handle_in, self->conf_in);
        SetConsoleMode(self->handle_out, self->conf_out);

        self->mode = PxConsoleMode_Cooked;
    }

    return 1;
}

static b32
pxWin32ConsoleModeSetRaw(PxWin32Console* self)
{
    DWORD conf_in = (self->conf_in & ~(ENABLE_PROCESSED_INPUT |
        ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT)) | ENABLE_VIRTUAL_TERMINAL_INPUT;

    DWORD conf_out = self->conf_out | (ENABLE_PROCESSED_OUTPUT |
        ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);

    if (self->mode != PxConsoleMode_Raw) {
        SetConsoleMode(self->handle_in, conf_in);
        SetConsoleMode(self->handle_out, conf_out);

        self->mode = PxConsoleMode_Raw;
    }

    return 1;
}

PxWin32Console*
pxWin32ConsoleReserve(PxMemoryArena* arena)
{
    return pxMemoryArenaReserveOneOf(arena, PxWin32Console);
}

b32
pxWin32ConsoleCreate(PxWin32Console* self)
{
    HANDLE handle_in  = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);

    if (handle_in != INVALID_HANDLE_VALUE && handle_out != INVALID_HANDLE_VALUE) {
        GetConsoleMode(handle_in, &self->conf_in);
        GetConsoleMode(handle_out, &self->conf_out);

        self->mode       = PxConsoleMode_Cooked;
        self->handle_in  = handle_in;
        self->handle_out = handle_out;

        return 1;
    }

    CloseHandle(handle_out);
    CloseHandle(handle_in);

    return 0;
}

void
pxWin32ConsoleDestroy(PxWin32Console* self)
{
    if (self->mode != PxConsoleMode_Cooked)
        pxWin32ConsoleModeSetCooked(self);

    pxMemorySet(self, sizeof *self, 0xAB);
}

b32
pxWin32ConsoleModeSet(PxWin32Console* self, PxConsoleMode mode)
{
    switch (mode) {
        case PxConsoleMode_Cooked: return pxWin32ConsoleModeSetCooked(self);
        case PxConsoleMode_Raw:    return pxWin32ConsoleModeSetRaw(self);

        default: break;
    }

    return 0;
}

PxConsoleMode
pxWin32ConsoleModeGet(PxWin32Console* self)
{
    return self->mode;
}

#endif // PX_CONSOLE_COMMON_C
