#ifndef PX_WIN32_CONSOLE_CONSOLE_C
#define PX_WIN32_CONSOLE_CONSOLE_C

#include "console.h"

static PxConsoleEvent pxWin32ConsoleEventConvert(WORD key_code, CHAR key_char)
{
    PxConsoleEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind = PxConsoleEvent_None;

    PxConsoleKey key = PxConsoleKey_None;

    switch (key_char) {
        case 'a': key = PxConsoleKey_A; break;
        case 'b': key = PxConsoleKey_B; break;
        case 'c': key = PxConsoleKey_C; break;
        case 'd': key = PxConsoleKey_D; break;
        case 'e': key = PxConsoleKey_E; break;
        case 'f': key = PxConsoleKey_F; break;
        case 'g': key = PxConsoleKey_G; break;
        case 'h': key = PxConsoleKey_H; break;
        case 'i': key = PxConsoleKey_I; break;
        case 'j': key = PxConsoleKey_J; break;
        case 'k': key = PxConsoleKey_K; break;
        case 'l': key = PxConsoleKey_L; break;
        case 'm': key = PxConsoleKey_M; break;
        case 'n': key = PxConsoleKey_N; break;
        case 'o': key = PxConsoleKey_O; break;
        case 'p': key = PxConsoleKey_P; break;
        case 'q': key = PxConsoleKey_Q; break;
        case 'r': key = PxConsoleKey_R; break;
        case 's': key = PxConsoleKey_S; break;
        case 't': key = PxConsoleKey_T; break;
        case 'u': key = PxConsoleKey_U; break;
        case 'v': key = PxConsoleKey_V; break;
        case 'w': key = PxConsoleKey_W; break;
        case 'x': key = PxConsoleKey_X; break;
        case 'y': key = PxConsoleKey_Y; break;
        case 'z': key = PxConsoleKey_Z; break;
        case '0': key = PxConsoleKey_0; break;
        case '1': key = PxConsoleKey_1; break;
        case '2': key = PxConsoleKey_2; break;
        case '3': key = PxConsoleKey_3; break;
        case '4': key = PxConsoleKey_4; break;
        case '5': key = PxConsoleKey_5; break;
        case '6': key = PxConsoleKey_6; break;
        case '7': key = PxConsoleKey_7; break;
        case '8': key = PxConsoleKey_8; break;
        case '9': key = PxConsoleKey_9; break;

        default: break;
    }

    switch (key_code) {
        case VK_SPACE:       key = PxConsoleKey_Space;        break;
        case VK_OEM_MINUS:   key = PxConsoleKey_Minus;        break;
        case VK_OEM_PLUS:    key = PxConsoleKey_Equal;        break;
        case VK_OEM_4:       key = PxConsoleKey_SquareLeft;   break;
        case VK_OEM_6:       key = PxConsoleKey_SquareRight;  break;
        case VK_OEM_5:       key = PxConsoleKey_BackSlash;    break;
        case VK_OEM_1:       key = PxConsoleKey_SemiColon;    break;
        case VK_OEM_7:       key = PxConsoleKey_Apostrophe;   break;
        case VK_OEM_3:       key = PxConsoleKey_Grave;        break;
        case VK_OEM_COMMA:   key = PxConsoleKey_Comma;        break;
        case VK_OEM_PERIOD:  key = PxConsoleKey_Period;       break;
        case VK_OEM_2:       key = PxConsoleKey_Slash;        break;
        case VK_UP:          key = PxConsoleKey_Up;           break;
        case VK_DOWN:        key = PxConsoleKey_Down;         break;
        case VK_LEFT:        key = PxConsoleKey_Left;         break;
        case VK_RIGHT:       key = PxConsoleKey_Right;        break;
        case VK_HOME:        key = PxConsoleKey_Home;         break;
        case VK_END:         key = PxConsoleKey_End;          break;
        case VK_PRIOR:       key = PxConsoleKey_PageUp;       break;
        case VK_NEXT:        key = PxConsoleKey_PageDown;     break;
        case VK_INSERT:      key = PxConsoleKey_Insert;       break;
        case VK_DELETE:      key = PxConsoleKey_Delete;       break;
        case VK_F1:          key = PxConsoleKey_F1;           break;
        case VK_F2:          key = PxConsoleKey_F2;           break;
        case VK_F3:          key = PxConsoleKey_F3;           break;
        case VK_F4:          key = PxConsoleKey_F4;           break;
        case VK_F5:          key = PxConsoleKey_F5;           break;
        case VK_F6:          key = PxConsoleKey_F6;           break;
        case VK_F7:          key = PxConsoleKey_F7;           break;
        case VK_F8:          key = PxConsoleKey_F8;           break;
        case VK_F9:          key = PxConsoleKey_F9;           break;
        case VK_F10:         key = PxConsoleKey_F10;          break;
        case VK_F11:         key = PxConsoleKey_F11;          break;
        case VK_F12:         key = PxConsoleKey_F12;          break;
        case VK_RETURN:      key = PxConsoleKey_Enter;        break;
        case VK_ESCAPE:      key = PxConsoleKey_Escape;       break;
        case VK_BACK:        key = PxConsoleKey_BackSpace;    break;
        case VK_TAB:         key = PxConsoleKey_Tab;          break;

        default: break;
    }

    if (key != PxConsoleKey_None) {
        result.kind         = PxConsoleEvent_Keyboard;
        result.keyboard.key = key;
    }

    return result;
}

static b32 pxWin32ConsoleModeSetCooked(PxWin32Console* self)
{
    if (self->mode != PxConsoleMode_Cooked) {
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), self->conf_in);
        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), self->conf_out);

        self->mode = PxConsoleMode_Cooked;
    }

    return 1;
}

static b32 pxWin32ConsoleModeSetRaw(PxWin32Console* self)
{
    DWORD conf_in = self->conf_in & ~(ENABLE_PROCESSED_INPUT |
        ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);

    DWORD conf_out = self->conf_out | (ENABLE_PROCESSED_OUTPUT |
        ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);

    if (self->mode != PxConsoleMode_Raw) {
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), conf_in);
        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), conf_out);

        self->mode = PxConsoleMode_Raw;
    }

    return 1;
}

PxWin32Console* pxWin32ConsoleReserve(PxMemoryArena* arena)
{
    return pxMemoryArenaReserveOneOf(arena, PxWin32Console);
}

b32 pxWin32ConsoleCreate(PxWin32Console* self)
{
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &self->conf_in);
    GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &self->conf_out);

    self->mode = PxConsoleMode_Cooked;

    return 1;
}

void pxWin32ConsoleDestroy(PxWin32Console* self)
{
    if (self->mode != PxConsoleMode_Cooked)
        pxWin32ConsoleModeSetCooked(self);

    pxMemorySet(self, sizeof *self, 0xAB);
}

b32 pxWin32ConsoleModeSet(PxWin32Console* self, PxConsoleMode mode)
{
    switch (mode) {
        case PxConsoleMode_Cooked: return pxWin32ConsoleModeSetCooked(self);
        case PxConsoleMode_Raw:    return pxWin32ConsoleModeSetRaw(self);

        default: break;
    }

    return 0;
}

PxConsoleMode pxWin32ConsoleModeGet(PxWin32Console* self)
{
    return self->mode;
}

ssize pxWin32ConsoleWrite(PxWin32Console* self, u8* pntr, ssize start, ssize stop)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    char* memory = ((char*) pntr + start);
    ssize size   = stop - start;
    ssize result = 0;

    while (result < size) {
        DWORD count = 0;

        b32 status = WriteFile(handle, memory + result,
            ((DWORD) size - result), &count, PX_NULL);

        if (status != 0 && count > 0 && count <= size - result)
            result += count;
        else
            break;
    }

    return result;
}

ssize pxWin32ConsoleRead(PxWin32Console* self, u8* pntr, ssize start, ssize stop)
{
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);

    char* memory = ((char*) pntr + start);
    ssize size   = stop - start;
    DWORD count  = 0;

    b32 status = ReadFile(handle, memory, size, &count, PX_NULL);

    if (status != 0 && count > 0 && count <= size) return count;

    return 0;
}

b32 pxWin32ConsolePollEvent(PxWin32Console* self, PxConsoleEvent* event)
{
    INPUT_RECORD record;

    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD  count  = 0;

    if (GetNumberOfConsoleInputEvents(handle, &count) == 0 || count <= 0)
        return 0;

    if (ReadConsoleInput(handle, &record, 1, &count) == 0 || count <= 0)
        return 0;

    if (record.EventType == KEY_EVENT) {
        BOOL key_state = record.Event.KeyEvent.bKeyDown;
        WORD key_code  = record.Event.KeyEvent.wVirtualKeyCode;
        CHAR key_char  = record.Event.KeyEvent.uChar.AsciiChar;

        if (key_state != 0) {
            if (event != PX_NULL)
                *event = pxWin32ConsoleEventConvert(key_code, key_char);

            return 1;
        }
    }

    return 0;
}

#endif // PX_CONSOLE_CONSOLE_C
