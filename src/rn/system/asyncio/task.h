#ifndef RN_SYSTEM_ASYNCIO_TASK_H
#define RN_SYSTEM_ASYNCIO_TASK_H

#include "./import.h"

typedef void RnAsyncIOTask;

RnAsyncIOTask*
rnAsyncIOTaskAccept(RnMemoryArena* arena, void* ctxt, RnSocketTCP* listener, RnSocketTCP* socket);

RnAsyncIOTask*
rnAsyncIOTaskConnect(RnMemoryArena* arena, void* ctxt, RnSocketTCP* socket, RnAddressIP address, u16 port);

RnAsyncIOTask*
rnAsyncIOTaskWrite(RnMemoryArena* arena, void* ctxt, RnSocketTCP* socket, u8* values, ssize start, ssize stop);

RnAsyncIOTask*
rnAsyncIOTaskRead(RnMemoryArena* arena, void* ctxt, RnSocketTCP* socket, u8* values, ssize start, ssize stop);

#endif // RN_SYSTEM_ASYNCIO_TASK_H
