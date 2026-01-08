#ifndef PX_WIN32_ASYNC_STORAGE_FILE_H
#define PX_WIN32_ASYNC_STORAGE_FILE_H

#include "import.h"

typedef struct PxWin32FileTaskWrite
{
    u8*   pntr;
    ssize start;
    ssize stop;
}
PxWin32FileTaskWrite;

typedef struct PxWin32FileTaskRead
{
    u8*   pntr;
    ssize start;
    ssize stop;
}
PxWin32FileTaskRead;

typedef struct PxWin32FileTask
{
    PxFileEventKind kind;

    void*        ctxt;
    PxWin32File* self;

    union
    {
        PxWin32FileTaskWrite write;
        PxWin32FileTaskRead  read;
    };
}
PxWin32FileTask;

b32
pxWin32FileWriteAsync(PxWin32Async* async, void* ctxt,
    PxWin32File* self, u8* pntr, ssize start, ssize stop);

b32
pxWin32FileReadAsync(PxWin32Async* async, void* ctxt,
    PxWin32File* self, u8* pntr, ssize start, ssize stop);

#endif // PX_WIN32_ASYNC_STORAGE_FILE_H
