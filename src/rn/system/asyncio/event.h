#ifndef RN_WIN32_ASYNCIO_EVENT_H
#define RN_WIN32_ASYNCIO_EVENT_H

#include "./import.h"

#define rnAsyncIOEventError() \
    ((RnAsyncIOEvent) {.kind = RnAsyncIOEvent_Error})

typedef enum RnAsyncIOEventKind
{
    RnAsyncIOEvent_None,

    RnAsyncIOEvent_Error,

    RnAsyncIOEvent_Accept,
    RnAsyncIOEvent_Connect,
    RnAsyncIOEvent_Write,
    RnAsyncIOEvent_Read,
    RnAsyncIOEvent_Close,
}
RnAsyncIOEventKind;

typedef struct RnAsyncIOEventAccept
{
    RnSocketTCP* listener;
    RnSocketTCP* socket;
}
RnAsyncIOEventAccept;

typedef struct RnAsyncIOEventConnect
{
    RnSocketTCP* socket;
    b32          status;
}
RnAsyncIOEventConnect;

typedef struct RnAsyncIOEventWrite
{
    RnSocketTCP* socket;
    u8*          values;
    ssize        start;
    ssize        stop;
}
RnAsyncIOEventWrite;

typedef struct RnAsyncIOEventRead
{
    RnSocketTCP* socket;
    u8*          values;
    ssize        start;
    ssize        stop;
}
RnAsyncIOEventRead;

typedef struct RnAsyncIOEventClose
{
    RnSocketTCP* socket;
}
RnAsyncIOEventClose;

typedef struct RnAsyncIOEvent
{
    RnAsyncIOEventKind kind;
    void*              ctxt;

    union
    {
        RnAsyncIOEventAccept  accept;
        RnAsyncIOEventConnect connect;
        RnAsyncIOEventWrite   write;
        RnAsyncIOEventRead    read;
        RnAsyncIOEventClose   close;
    };
}
RnAsyncIOEvent;

RnAsyncIOEvent
rnAsyncIOEventAccept(void* ctxt, RnSocketTCP* litener, RnSocketTCP* socket);

RnAsyncIOEvent
rnAsyncIOEventConnect(void* ctxt, RnSocketTCP* socket, b32 status);

RnAsyncIOEvent
rnAsyncIOEventWrite(void* ctxt, RnSocketTCP* socket, u8* values, ssize start, ssize stop);

RnAsyncIOEvent
rnAsyncIOEventRead(void* ctxt, RnSocketTCP* socket, u8* values, ssize start, ssize stop);

RnAsyncIOEvent
rnAsyncIOEventClose(void* ctxt, RnSocketTCP* socket);

#endif // RN_WIN32_ASYNCIO_EVENT_H
