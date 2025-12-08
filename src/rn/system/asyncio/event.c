#ifndef RN_SYSTEM_ASYNCIO_EVENT_C
#define RN_SYSTEM_ASYNCIO_EVENT_C

#include "./event.h"

RnAsyncIOEvent
rnAsyncIOEventAccept(void* ctxt, RnSocketTCP* listener, RnSocketTCP* socket)
{
    return (RnAsyncIOEvent) {
        .kind = RnAsyncIOEvent_Accept,
        .ctxt = ctxt,

        .accept = {
            .listener = listener,
            .socket   = socket,
        },
    };
}

RnAsyncIOEvent
rnAsyncIOEventConnect(void* ctxt, RnSocketTCP* socket, b32 status)
{
    return (RnAsyncIOEvent) {
        .kind = RnAsyncIOEvent_Connect,
        .ctxt = ctxt,

        .connect = {
            .socket = socket,
            .status = status != 0 ? 1 : 0,
        },
    };
}

RnAsyncIOEvent
rnAsyncIOEventWrite(void* ctxt, RnSocketTCP* socket, u8* values, ssize start, ssize stop)
{
    return (RnAsyncIOEvent) {
        .kind = RnAsyncIOEvent_Write,
        .ctxt = ctxt,

        .write = {
            .socket = socket,
            .values = values,
            .start  = start,
            .stop   = stop,
        },
    };
}

RnAsyncIOEvent
rnAsyncIOEventRead(void* ctxt, RnSocketTCP* socket, u8* values, ssize start, ssize stop)
{
    return (RnAsyncIOEvent) {
        .kind = RnAsyncIOEvent_Read,
        .ctxt = ctxt,

        .read = {
            .socket = socket,
            .values = values,
            .start  = start,
            .stop   = stop,
        },
    };
}

RnAsyncIOEvent
rnAsyncIOEventClose(void* ctxt, RnSocketTCP* socket)
{
    return (RnAsyncIOEvent) {
        .kind = RnAsyncIOEvent_Close,
        .ctxt = ctxt,

        .close = {
            .socket = socket,
        },
    };
}

#endif // RN_SYSTEM_ASYNCIO_EVENT_C
