#include "../src/conn/export.h"

#include <stdio.h>

void
testMessage(ConnMessage message)
{
    u8 buffer[1024];
    u8 memory[1024];

    ssize size = connMessageToString(message, memory, 1024);

    printf("%.*s\n", (int) size, memory);

    size = connMessageEncode(message, buffer, 1024);

    ssize index = 0;

    for (index = 0; index < size; index += 1) {
        printf("%03u ", buffer[index]);

        if ((index + 1) % 8 == 0 || index + 1 == size)
            printf("\n");
    }

    message = connMessageDecode(buffer, size);

    size = connMessageToString(message, memory, 1024);

    printf("%.*s\n", (int) size, memory);
}

int
main(int argc, char** argv)
{
    testMessage(connMessageJoin(ConnClient_Player));
    testMessage(connMessageQuit(1));
    testMessage(connMessageData(ConnClient_Player, 1));
    testMessage(connMessageTurn(2));
    testMessage(connMessageMove(2, 65));
    testMessage(connMessageResult(2));
}
