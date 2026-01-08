#ifndef CONN_COMMAND_C
#define CONN_COMMAND_C

#include "command.h"

#include <stdio.h>

ConnCommand connCommandDecode(PxConsoleEvent event)
{
    ConnCommand result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind = ConnCommand_None;

    if (event.kind == PxConsoleEvent_Keyboard) {
        switch (event.keyboard.key) {
            case PxConsoleKey_A:      result.kind = ConnCommand_MoveLeft;  break;
            case PxConsoleKey_D:      result.kind = ConnCommand_MoveRight; break;
            case PxConsoleKey_Enter:  result.kind = ConnCommand_Place;     break;
            case PxConsoleKey_Escape: result.kind = ConnCommand_Quit;      break;

            default: break;
        }
    }

    return result;
}

ssize connCommandToString(ConnCommand command, u8* pntr, ssize size)
{
    ssize result = 0;

    switch (command.kind) {
        case ConnCommand_MoveLeft:
            result = snprintf(((char*) pntr), size, "(MoveLeft) {}");
        break;

        case ConnCommand_MoveRight:
            result = snprintf(((char*) pntr), size, "(MoveRight) {}");
        break;

        case ConnCommand_Place:
            result = snprintf(((char*) pntr), size, "(Place) {}");
        break;

        case ConnCommand_Quit:
            result = snprintf(((char*) pntr), size, "(Quit) {}");
        break;

        default:
            result = snprintf(((char*) pntr), size, "(None) {}");
        break;
    }

    if (result >= 0 && result < size) return result;

    return 0;
}

#endif // CONN_COMMAND_C
