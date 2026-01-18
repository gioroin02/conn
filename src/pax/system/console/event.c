#ifndef P_SYSTEM_CONSOLE_EVENT_C
#define P_SYSTEM_CONSOLE_EVENT_C

#include "event.h"

PConsoleEvent
pConsoleEventKeyboardKey(PConsoleKeyboardKey key)
{
    PConsoleEvent result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind             = PConsoleEvent_KeyboardKey;
    result.keyboard_key.key = key;

    return result;
}

#endif // P_SYSTEM_CONSOLE_EVENT_C
