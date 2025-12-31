#include "client.h"

int
main(int argc, char** argv)
{
    srand(time(0));

    PxMemoryArena arena = pxSystemMemoryReserve(pxMemoryMiB(4));

    ConnClient client = connClientMake();

    if (connClientCreate(&client, &arena) == 0) return 1;

    connClientStart(&client);

    while (connClientStateIsActive(&client) != 0)
        connClientUpdate(&client);

    connClientStop(&client);
    connClientDestroy(&client);
}
