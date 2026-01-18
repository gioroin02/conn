#ifndef CONN_COMMAND_C
#define CONN_COMMAND_C

#include "command.h"

#include <stdio.h>

ConnCommand connCommandDecode(PConsoleEvent event)
{
    ConnCommand result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.kind = ConnCommand_None;

    if (event.kind == PConsoleEvent_KeyboardKey) {
        switch (event.keyboard_key.key) {
            case PConsoleKeyboard_A:      result.kind = ConnCommand_MoveLeft;  break;
            case PConsoleKeyboard_D:      result.kind = ConnCommand_MoveRight; break;
            case PConsoleKeyboard_Enter:  result.kind = ConnCommand_Place;     break;
            case PConsoleKeyboard_Escape: result.kind = ConnCommand_Quit;      break;

            default: break;
        }
    }

    return result;
}

Int connCommandToString(ConnCommand command, U8* pntr, Int size)
{
    Int result = 0;

    switch (command.kind) {
        case ConnCommand_MoveLeft:
            result = snprintf((I8*) pntr, size, "(MoveLeft) {}");
        break;

        case ConnCommand_MoveRight:
            result = snprintf((I8*) pntr, size, "(MoveRight) {}");
        break;

        case ConnCommand_Place:
            result = snprintf((I8*) pntr, size, "(Place) {}");
        break;

        case ConnCommand_Quit:
            result = snprintf((I8*) pntr, size, "(Quit) {}");
        break;

        default:
            result = snprintf((I8*) pntr, size, "(None) {}");
        break;
    }

    if (result >= 0 && result < size) return result;

    return 0;
}

#endif // CONN_COMMAND_C
