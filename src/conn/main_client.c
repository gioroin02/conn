#include "client.h"

int
main(int argc, char** argv)
{
    srand(time(0));

    PxMemoryArena arena = pxSystemMemoryReserve(pxMebi(4));

    ConnClient client;

    if (connClientCreate(&client, &arena) == 0) return 1;

    connClientStart(&client);

    while (connClientStateIsActive(&client) != 0)
        connClientUpdate(&client);

    connClientStop(&client);
    connClientDestroy(&client);
}
