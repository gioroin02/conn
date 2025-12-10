#ifndef RN_WIN32_ASYNCIO_TASK_C
#define RN_WIN32_ASYNCIO_TASK_C

#include "./task.h"

#define RN_WIN32_ASYNCIO_ACCEPT_LENGTH ((sizeof(RnSockAddrStorage) + 16) * 2)

RnWin32AsyncIOTask*
rnWin32AsyncIOTaskAccept(RnMemoryArena* arena, void* ctxt, RnWin32SocketTCP* listener, RnWin32SocketTCP* socket)
{
    RnAddressIP address = rnAddressIPEmpty(rnSocketTCPGetAddress(listener).kind);

    RnWin32AsyncIOTask* result =
        rnMemoryArenaReserveOneOf(arena, RnWin32AsyncIOTask);

    if (result == 0 || rnSocketTCPCreate(socket, address, 0) == 0) return 0;

    ssize size = RN_WIN32_ASYNCIO_ACCEPT_LENGTH;

    *result = (RnWin32AsyncIOTask) {
        .kind = RnAsyncIOEvent_Accept,
        .ctxt = ctxt,

        .accept = {
            .listener = listener,
            .socket   = socket,
            .buffer   = rnMemoryArenaReserveManyOf(arena, u8, size),
            .size     = size,
        },
    };

    if (result->accept.buffer != 0) return result;

    rnMemoryArenaRelease(arena, result);

    return 0;
}

RnWin32AsyncIOTask*
rnWin32AsyncIOTaskConnect(RnMemoryArena* arena, void* ctxt, RnWin32SocketTCP* socket, RnAddressIP address, u16 port)
{
    RnWin32AsyncIOTask* result =
        rnMemoryArenaReserveOneOf(arena, RnWin32AsyncIOTask);

    if (result == 0 || rnSocketTCPBind(socket) == 0) return 0;

    *result = (RnWin32AsyncIOTask) {
        .kind = RnAsyncIOEvent_Connect,
        .ctxt = ctxt,

        .connect = {
            .socket  = socket,
            .address = address,
            .port    = port,
        },
    };

    return result;
}

RnWin32AsyncIOTask*
rnWin32AsyncIOTaskWrite(RnMemoryArena* arena, void* ctxt, RnWin32SocketTCP* socket, u8* values, ssize start, ssize stop)
{
    RnWin32AsyncIOTask* result =
        rnMemoryArenaReserveOneOf(arena, RnWin32AsyncIOTask);

    if (result == 0) return 0;

    *result = (RnWin32AsyncIOTask) {
        .kind = RnAsyncIOEvent_Write,
        .ctxt = ctxt,

        .write = {
            .socket = socket,
            .values = values,
            .start  = start,
            .stop   = stop,
            .buffer = (WSABUF) {
                .buf = ((char*) values) + start,
                .len = stop - start,
            },
            .flags  = 0,
        },
    };

    return result;
}

RnWin32AsyncIOTask*
rnWin32AsyncIOTaskRead(RnMemoryArena* arena, void* ctxt, RnWin32SocketTCP* socket, u8* values, ssize start, ssize stop)
{
    RnWin32AsyncIOTask* result =
        rnMemoryArenaReserveOneOf(arena, RnWin32AsyncIOTask);

    if (result == 0) return 0;

    *result = (RnWin32AsyncIOTask) {
        .kind = RnAsyncIOEvent_Read,
        .ctxt = ctxt,

        .write = {
            .socket = socket,
            .values = values,
            .start  = start,
            .stop   = stop,
            .buffer = (WSABUF) {
                .buf = ((char*) values) + start,
                .len = stop - start,
            },
            .flags  = 0,
        },
    };

    return result;
}

#endif // RN_WIN32_ASYNCIO_TASK_C
