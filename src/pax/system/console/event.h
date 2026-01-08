#ifndef PX_SYSTEM_CONSOLE_EVENT_H
#define PX_SYSTEM_CONSOLE_EVENT_H

#include "import.h"

typedef enum PxConsoleKey
{
    PxConsoleKey_None,
    PxConsoleKey_A,
    PxConsoleKey_B,
    PxConsoleKey_C,
    PxConsoleKey_D,
    PxConsoleKey_E,
    PxConsoleKey_F,
    PxConsoleKey_G,
    PxConsoleKey_H,
    PxConsoleKey_I,
    PxConsoleKey_J,
    PxConsoleKey_K,
    PxConsoleKey_L,
    PxConsoleKey_M,
    PxConsoleKey_N,
    PxConsoleKey_O,
    PxConsoleKey_P,
    PxConsoleKey_Q,
    PxConsoleKey_R,
    PxConsoleKey_S,
    PxConsoleKey_T,
    PxConsoleKey_U,
    PxConsoleKey_V,
    PxConsoleKey_W,
    PxConsoleKey_X,
    PxConsoleKey_Y,
    PxConsoleKey_Z,
    PxConsoleKey_0,
    PxConsoleKey_1,
    PxConsoleKey_2,
    PxConsoleKey_3,
    PxConsoleKey_4,
    PxConsoleKey_5,
    PxConsoleKey_6,
    PxConsoleKey_7,
    PxConsoleKey_8,
    PxConsoleKey_9,
    PxConsoleKey_Space,
    PxConsoleKey_Minus,
    PxConsoleKey_Equal,
    PxConsoleKey_SquareLeft,
    PxConsoleKey_SquareRight,
    PxConsoleKey_BackSlash,
    PxConsoleKey_SemiColon,
    PxConsoleKey_Apostrophe,
    PxConsoleKey_Grave,
    PxConsoleKey_Comma,
    PxConsoleKey_Period,
    PxConsoleKey_Slash,
    PxConsoleKey_Up,
    PxConsoleKey_Down,
    PxConsoleKey_Left,
    PxConsoleKey_Right,
    PxConsoleKey_Home,
    PxConsoleKey_End,
    PxConsoleKey_PageUp,
    PxConsoleKey_PageDown,
    PxConsoleKey_Insert,
    PxConsoleKey_Delete,
    PxConsoleKey_F1,
    PxConsoleKey_F2,
    PxConsoleKey_F3,
    PxConsoleKey_F4,
    PxConsoleKey_F5,
    PxConsoleKey_F6,
    PxConsoleKey_F7,
    PxConsoleKey_F8,
    PxConsoleKey_F9,
    PxConsoleKey_F10,
    PxConsoleKey_F11,
    PxConsoleKey_F12,
    PxConsoleKey_Enter,
    PxConsoleKey_Escape,
    PxConsoleKey_BackSpace,
    PxConsoleKey_Tab,
}
PxConsoleKey;

typedef enum PxConsoleEventKind
{
    PxConsoleEvent_None,
    PxConsoleEvent_Keyboard,
}
PxConsoleEventKind;

typedef struct PxConsoleEventKeyboard
{
    PxConsoleKey key;
}
PxConsoleEventKeyboard;

typedef struct PxConsoleEvent
{
    PxConsoleEventKind kind;

    union
    {
        PxConsoleEventKeyboard keyboard;
    };
}
PxConsoleEvent;

#endif // PX_SYSTEM_CONSOLE_EVENT_H
