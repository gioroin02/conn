#ifndef CONN_COMMAND_C
#define CONN_COMMAND_C

#include "command.h"

#include <stdio.h>

ConnCommand
connCommandDecode(u8 value)
{
    ConnCommand result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind = ConnCommand_None;

    switch (value) {
        case 'a': case 'A':
            result.kind = ConnCommand_MoveLeft;
        break;

        case 'd': case 'D':
            result.kind = ConnCommand_MoveRight;
        break;

        case '\r':
            result.kind = ConnCommand_Place;
        break;

        default:
            result.kind = ConnCommand_Quit;
        break;
    }

    return result;
}

ssize
connCommandToString(ConnCommand command, u8* values, ssize size)
{
    ssize result = 0;

    switch (command.kind) {
        case ConnCommand_MoveLeft: {
            result = snprintf(((char*) values), size, "(MoveLeft) {}");
        } break;

        case ConnCommand_MoveRight: {
            result = snprintf(((char*) values), size, "(MoveRight) {}");
        } break;

        case ConnCommand_Place: {
            result = snprintf(((char*) values), size, "(Place) {}");
        } break;

        case ConnCommand_Quit: {
            result = snprintf(((char*) values), size, "(Quit) {}");
        } break;

        default: result = snprintf(((char*) values), size, "(None) {}"); break;
    }

    if (result >= 0 && result < size) return result;

    return 0;
}

#endif // CONN_COMMAND_C
