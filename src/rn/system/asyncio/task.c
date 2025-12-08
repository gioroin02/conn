#ifndef RN_SYSTEM_ASYNCIO_TASK_C
#define RN_SYSTEM_ASYNCIO_TASK_C

#include "./task.h"

#if RN_SYSTEM == RN_SYSTEM_WINDOWS

    #include "../../win32/asyncio/export.c"

    #define __rnAsyncIOTaskAccept__  rnWin32AsyncIOTaskAccept
    #define __rnAsyncIOTaskConnect__ rnWin32AsyncIOTaskConnect
    #define __rnAsyncIOTaskWrite__   rnWin32AsyncIOTaskWrite
    #define __rnAsyncIOTaskRead__    rnWin32AsyncIOTaskRead

#else

    #error "Unknown platform"

#endif

RnAsyncIOTask*
rnAsyncIOTaskAccept(RnMemoryArena* arena, void* ctxt, RnSocketTCP* listener, RnSocketTCP* socket)
{
    return __rnAsyncIOTaskAccept__(arena, ctxt, listener, socket);
}

RnAsyncIOTask*
rnAsyncIOTaskConnect(RnMemoryArena* arena, void* ctxt, RnSocketTCP* socket, RnAddressIP address, u16 port)
{
    return __rnAsyncIOTaskConnect__(arena, ctxt, socket, address, port);
}

RnAsyncIOTask*
rnAsyncIOTaskWrite(RnMemoryArena* arena, void* ctxt, RnSocketTCP* socket, u8* values, ssize start, ssize stop)
{
    return __rnAsyncIOTaskWrite__(arena, ctxt, socket, values, start, stop);
}

RnAsyncIOTask*
rnAsyncIOTaskRead(RnMemoryArena* arena, void* ctxt, RnSocketTCP* socket, u8* values, ssize start, ssize stop)
{
    return __rnAsyncIOTaskRead__(arena, ctxt, socket, values, start, stop);
}

#endif // RN_SYSTEM_ASYNCIO_TASK_C
