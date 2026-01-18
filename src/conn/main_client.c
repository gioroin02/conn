#include "client.h"

int
main(int argc, char** argv)
{
    PMemoryArena arena = pSystemMemoryReserve(pMemoryMIB(4));

    ConnClient client;

    if (connClientCreate(&client, &arena) == 0) return 1;

    connClientStart(&client);

    while (connClientStateIsActive(&client) != 0)
        connClientUpdate(&client, &arena);

    connClientStop(&client);
    connClientDestroy(&client);
}
