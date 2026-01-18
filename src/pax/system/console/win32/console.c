#ifndef P_SYSTEM_WIN32_CONSOLE_CONSOLE_C
#define P_SYSTEM_WIN32_CONSOLE_CONSOLE_C

#include "console.h"

static PConsoleKeyboardKey pWin32ConsoleConvertKey(WORD key_code, CHAR key_char)
{
    switch (key_char) {
        case 'a': return PConsoleKeyboard_A; break;
        case 'b': return PConsoleKeyboard_B; break;
        case 'c': return PConsoleKeyboard_C; break;
        case 'd': return PConsoleKeyboard_D; break;
        case 'e': return PConsoleKeyboard_E; break;
        case 'f': return PConsoleKeyboard_F; break;
        case 'g': return PConsoleKeyboard_G; break;
        case 'h': return PConsoleKeyboard_H; break;
        case 'i': return PConsoleKeyboard_I; break;
        case 'j': return PConsoleKeyboard_J; break;
        case 'k': return PConsoleKeyboard_K; break;
        case 'l': return PConsoleKeyboard_L; break;
        case 'm': return PConsoleKeyboard_M; break;
        case 'n': return PConsoleKeyboard_N; break;
        case 'o': return PConsoleKeyboard_O; break;
        case 'p': return PConsoleKeyboard_P; break;
        case 'q': return PConsoleKeyboard_Q; break;
        case 'r': return PConsoleKeyboard_R; break;
        case 's': return PConsoleKeyboard_S; break;
        case 't': return PConsoleKeyboard_T; break;
        case 'u': return PConsoleKeyboard_U; break;
        case 'v': return PConsoleKeyboard_V; break;
        case 'w': return PConsoleKeyboard_W; break;
        case 'x': return PConsoleKeyboard_X; break;
        case 'y': return PConsoleKeyboard_Y; break;
        case 'z': return PConsoleKeyboard_Z; break;
        case '0': return PConsoleKeyboard_0; break;
        case '1': return PConsoleKeyboard_1; break;
        case '2': return PConsoleKeyboard_2; break;
        case '3': return PConsoleKeyboard_3; break;
        case '4': return PConsoleKeyboard_4; break;
        case '5': return PConsoleKeyboard_5; break;
        case '6': return PConsoleKeyboard_6; break;
        case '7': return PConsoleKeyboard_7; break;
        case '8': return PConsoleKeyboard_8; break;
        case '9': return PConsoleKeyboard_9; break;

        default: break;
    }

    switch (key_code) {
        case VK_SPACE:       return PConsoleKeyboard_Space;        break;
        case VK_OEM_MINUS:   return PConsoleKeyboard_Minus;        break;
        case VK_OEM_PLUS:    return PConsoleKeyboard_Equal;        break;
        case VK_OEM_4:       return PConsoleKeyboard_SquareLeft;   break;
        case VK_OEM_6:       return PConsoleKeyboard_SquareRight;  break;
        case VK_OEM_5:       return PConsoleKeyboard_BackSlash;    break;
        case VK_OEM_1:       return PConsoleKeyboard_SemiColon;    break;
        case VK_OEM_7:       return PConsoleKeyboard_Apostrophe;   break;
        case VK_OEM_3:       return PConsoleKeyboard_Grave;        break;
        case VK_OEM_COMMA:   return PConsoleKeyboard_Comma;        break;
        case VK_OEM_PERIOD:  return PConsoleKeyboard_Period;       break;
        case VK_OEM_2:       return PConsoleKeyboard_Slash;        break;
        case VK_UP:          return PConsoleKeyboard_Up;           break;
        case VK_DOWN:        return PConsoleKeyboard_Down;         break;
        case VK_LEFT:        return PConsoleKeyboard_Left;         break;
        case VK_RIGHT:       return PConsoleKeyboard_Right;        break;
        case VK_HOME:        return PConsoleKeyboard_Home;         break;
        case VK_END:         return PConsoleKeyboard_End;          break;
        case VK_PRIOR:       return PConsoleKeyboard_PageUp;       break;
        case VK_NEXT:        return PConsoleKeyboard_PageDown;     break;
        case VK_INSERT:      return PConsoleKeyboard_Insert;       break;
        case VK_DELETE:      return PConsoleKeyboard_Delete;       break;
        case VK_F1:          return PConsoleKeyboard_F1;           break;
        case VK_F2:          return PConsoleKeyboard_F2;           break;
        case VK_F3:          return PConsoleKeyboard_F3;           break;
        case VK_F4:          return PConsoleKeyboard_F4;           break;
        case VK_F5:          return PConsoleKeyboard_F5;           break;
        case VK_F6:          return PConsoleKeyboard_F6;           break;
        case VK_F7:          return PConsoleKeyboard_F7;           break;
        case VK_F8:          return PConsoleKeyboard_F8;           break;
        case VK_F9:          return PConsoleKeyboard_F9;           break;
        case VK_F10:         return PConsoleKeyboard_F10;          break;
        case VK_F11:         return PConsoleKeyboard_F11;          break;
        case VK_F12:         return PConsoleKeyboard_F12;          break;
        case VK_RETURN:      return PConsoleKeyboard_Enter;        break;
        case VK_ESCAPE:      return PConsoleKeyboard_Escape;       break;
        case VK_BACK:        return PConsoleKeyboard_BackSpace;    break;
        case VK_TAB:         return PConsoleKeyboard_Tab;          break;

        default: break;
    }

    return PConsoleKeyboard_None;
}

static Bool pWin32ConsoleModeSetCooked(PWin32Console* self)
{
    if (self->mode != PConsoleMode_Cooked) {
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), self->conf_in);
        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), self->conf_out);

        self->mode = PConsoleMode_Cooked;
    }

    return 1;
}

static Bool pWin32ConsoleModeSetRaw(PWin32Console* self)
{
    Int conf_in = self->conf_in & ~(ENABLE_PROCESSED_INPUT |
        ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);

    Int conf_out = self->conf_out | (ENABLE_PROCESSED_OUTPUT |
        ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN);

    if (self->mode != PConsoleMode_Raw) {
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), conf_in);
        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), conf_out);

        self->mode = PConsoleMode_Raw;
    }

    return 1;
}

PWin32Console* pWin32ConsoleReserve(PMemoryArena* arena)
{
    return pMemoryArenaReserveOneOf(arena, PWin32Console);
}

Bool pWin32ConsoleCreate(PWin32Console* self)
{
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), (DWORD*) &self->conf_in);
    GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), (DWORD*) &self->conf_out);

    self->mode = PConsoleMode_Cooked;

    return 1;
}

void pWin32ConsoleDestroy(PWin32Console* self)
{
    if (self->mode != PConsoleMode_Cooked)
        pWin32ConsoleModeSetCooked(self);

    pMemorySet(self, sizeof *self, 0xAB);
}

Bool pWin32ConsoleModeSet(PWin32Console* self, PConsoleMode mode)
{
    switch (mode) {
        case PConsoleMode_Cooked: return pWin32ConsoleModeSetCooked(self);
        case PConsoleMode_Raw:    return pWin32ConsoleModeSetRaw(self);

        default: break;
    }

    return 0;
}

PConsoleMode pWin32ConsoleModeGet(PWin32Console* self)
{
    return self->mode;
}

Int pWin32ConsoleWrite(PWin32Console* self, U8* pntr, Int start, Int stop)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    I8* memory = ((I8*) pntr + start);
    Int size   = stop - start;
    Int result = 0;

    while (result < size) {
        Int count = 0;

        Bool status = WriteFile(handle, memory + result,
            size - result, (DWORD*) &count, NULL);

        if (status != 0 && count > 0 && count <= size - result)
            result += count;
        else
            break;
    }

    return result;
}

Int pWin32ConsoleRead(PWin32Console* self, U8* pntr, Int start, Int stop)
{
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);

    I8* memory = ((I8*) pntr + start);
    Int size   = stop - start;
    Int count  = 0;

    Bool status = ReadFile(handle, memory, size, (DWORD*) &count, NULL);

    if (status != 0 && count > 0 && count <= size) return count;

    return 0;
}

Bool pWin32ConsolePollEvent(PWin32Console* self, PConsoleEvent* event)
{
    INPUT_RECORD record;

    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD  count  = 0;

    if (GetNumberOfConsoleInputEvents(handle, &count) == 0 || count <= 0)
        return 0;

    if (ReadConsoleInput(handle, &record, 1, &count) == 0 || count <= 0)
        return 0;

    if (record.EventType == KEY_EVENT) {
        WORD key_code  = record.Event.KeyEvent.wVirtualKeyCode;
        CHAR key_char  = record.Event.KeyEvent.uChar.AsciiChar;

        PConsoleKeyboardKey key = pWin32ConsoleConvertKey(key_code, key_char);

        if (record.Event.KeyEvent.bKeyDown != 0) {
            if (event != NULL)
                *event = pConsoleEventKeyboardKey(key);

            return 1;
        }
    }

    return 0;
}

#endif // P_CONSOLE_CONSOLE_C
