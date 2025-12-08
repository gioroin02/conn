#ifndef RN_WIN32_ASYNCIO_TASK_H
#define RN_WIN32_ASYNCIO_TASK_H

#include "./import.h"

typedef struct RnWin32AsyncIOTaskAccept
{
    RnWin32SocketTCP* listener;
    RnWin32SocketTCP* socket;
    u8*               buffer;
    ssize             size;
}
RnWin32AsyncIOTaskAccept;

typedef struct RnWin32AsyncIOTaskConnect
{
    RnWin32SocketTCP* socket;
    RnAddressIP       address;
    u16               port;
}
RnWin32AsyncIOTaskConnect;

typedef struct RnWin32AsyncIOTaskWrite
{
    RnWin32SocketTCP* socket;
    u8*               values;
    ssize             start;
    ssize             stop;
    WSABUF            buffer;
    DWORD             flags;
}
RnWin32AsyncIOTaskWrite;

typedef struct RnWin32AsyncIOTaskRead
{
    RnWin32SocketTCP* socket;
    u8*               values;
    ssize             start;
    ssize             stop;
    WSABUF            buffer;
    DWORD             flags;
}
RnWin32AsyncIOTaskRead;

typedef struct RnWin32AsyncIOTask
{
    RnAsyncIOEventKind kind;

    struct RnWin32AsyncIOTask* next;
    struct RnWin32AsyncIOTask* prev;

    void* ctxt;

    OVERLAPPED overlap;

    union
    {
        RnWin32AsyncIOTaskAccept  accept;
        RnWin32AsyncIOTaskConnect connect;
        RnWin32AsyncIOTaskWrite   write;
        RnWin32AsyncIOTaskRead    read;
    };
}
RnWin32AsyncIOTask;

RnWin32AsyncIOTask*
rnWin32AsyncIOTaskAccept(RnMemoryArena* arena, void* ctxt, RnWin32SocketTCP* listener, RnWin32SocketTCP* socket);

RnWin32AsyncIOTask*
rnWin32AsyncIOTaskConnect(RnMemoryArena* arena, void* ctxt, RnWin32SocketTCP* socket, RnAddressIP address, u16 port);

RnWin32AsyncIOTask*
rnWin32AsyncIOTaskWrite(RnMemoryArena* arena, void* ctxt, RnWin32SocketTCP* socket, u8* values, ssize start, ssize stop);

RnWin32AsyncIOTask*
rnWin32AsyncIOTaskRead(RnMemoryArena* arena, void* ctxt, RnWin32SocketTCP* socket, u8* values, ssize start, ssize stop);

#endif // RN_WIN32_ASYNCIO_TASK_H
