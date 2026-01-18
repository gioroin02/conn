#ifndef P_SYSTEM_CONSOLE_EVENT_H
#define P_SYSTEM_CONSOLE_EVENT_H

#include "import.h"

typedef enum PConsoleKeyboardKey
{
    PConsoleKeyboard_None,
    PConsoleKeyboard_A,
    PConsoleKeyboard_B,
    PConsoleKeyboard_C,
    PConsoleKeyboard_D,
    PConsoleKeyboard_E,
    PConsoleKeyboard_F,
    PConsoleKeyboard_G,
    PConsoleKeyboard_H,
    PConsoleKeyboard_I,
    PConsoleKeyboard_J,
    PConsoleKeyboard_K,
    PConsoleKeyboard_L,
    PConsoleKeyboard_M,
    PConsoleKeyboard_N,
    PConsoleKeyboard_O,
    PConsoleKeyboard_P,
    PConsoleKeyboard_Q,
    PConsoleKeyboard_R,
    PConsoleKeyboard_S,
    PConsoleKeyboard_T,
    PConsoleKeyboard_U,
    PConsoleKeyboard_V,
    PConsoleKeyboard_W,
    PConsoleKeyboard_X,
    PConsoleKeyboard_Y,
    PConsoleKeyboard_Z,
    PConsoleKeyboard_0,
    PConsoleKeyboard_1,
    PConsoleKeyboard_2,
    PConsoleKeyboard_3,
    PConsoleKeyboard_4,
    PConsoleKeyboard_5,
    PConsoleKeyboard_6,
    PConsoleKeyboard_7,
    PConsoleKeyboard_8,
    PConsoleKeyboard_9,
    PConsoleKeyboard_Space,
    PConsoleKeyboard_Minus,
    PConsoleKeyboard_Equal,
    PConsoleKeyboard_SquareLeft,
    PConsoleKeyboard_SquareRight,
    PConsoleKeyboard_BackSlash,
    PConsoleKeyboard_SemiColon,
    PConsoleKeyboard_Apostrophe,
    PConsoleKeyboard_Grave,
    PConsoleKeyboard_Comma,
    PConsoleKeyboard_Period,
    PConsoleKeyboard_Slash,
    PConsoleKeyboard_Up,
    PConsoleKeyboard_Down,
    PConsoleKeyboard_Left,
    PConsoleKeyboard_Right,
    PConsoleKeyboard_Home,
    PConsoleKeyboard_End,
    PConsoleKeyboard_PageUp,
    PConsoleKeyboard_PageDown,
    PConsoleKeyboard_Insert,
    PConsoleKeyboard_Delete,
    PConsoleKeyboard_F1,
    PConsoleKeyboard_F2,
    PConsoleKeyboard_F3,
    PConsoleKeyboard_F4,
    PConsoleKeyboard_F5,
    PConsoleKeyboard_F6,
    PConsoleKeyboard_F7,
    PConsoleKeyboard_F8,
    PConsoleKeyboard_F9,
    PConsoleKeyboard_F10,
    PConsoleKeyboard_F11,
    PConsoleKeyboard_F12,
    PConsoleKeyboard_Enter,
    PConsoleKeyboard_Escape,
    PConsoleKeyboard_BackSpace,
    PConsoleKeyboard_Tab,
}
PConsoleKeyboardKey;

typedef enum PConsoleEventKind
{
    PConsoleEvent_None,
    PConsoleEvent_KeyboardKey,
}
PConsoleEventKind;

typedef struct PConsoleEventKeyboardKey
{
    PConsoleKeyboardKey key;
}
PConsoleEventKeyboardKey;

typedef struct PConsoleEvent
{
    PConsoleEventKind kind;

    union
    {
        PConsoleEventKeyboardKey keyboard_key;
    };
}
PConsoleEvent;

PConsoleEvent
pConsoleEventKeyboardKey(PConsoleKeyboardKey key);

#endif // P_SYSTEM_CONSOLE_EVENT_H
