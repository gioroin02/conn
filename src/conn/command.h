#ifndef CONN_COMMAND_H
#define CONN_COMMAND_H

#include "import.h"

typedef enum ConnCommandKind
{
    ConnCommand_None,
    ConnCommand_MoveLeft,
    ConnCommand_MoveRight,
    ConnCommand_Place,
    ConnCommand_Quit,
}
ConnCommandKind;

typedef struct ConnCommand
{
    ConnCommandKind kind;
}
ConnCommand;

ConnCommand connCommandDecode(PxConsoleEvent event);

ssize connCommandToString(ConnCommand command, u8* pntr, ssize size);

#endif // CONN_COMMAND_H
