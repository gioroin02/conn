#include "server.h"

int
main(int argc, char** argv)
{
    srand(time(0));

    PxMemoryArena arena = pxSystemMemoryReserve(pxMemoryMiB(2));

    ConnServer server = connServerMake();

    if (connServerCreate(&server, &arena) == 0) return 1;

    connServerStart(&server);

    while (connServerStateIsActive(&server) != 0)
        connServerUpdate(&server);

    connServerStop(&server);
    connServerDestroy(&server);
}
