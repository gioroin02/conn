#ifndef PX_WIN32_ASYNC_STORAGE_FILE_H
#define PX_WIN32_ASYNC_STORAGE_FILE_H

#include "import.h"

typedef struct PxWin32FileTaskWrite
{
    PxWin32File* file;
    u8*          values;
    ssize        start;
    ssize        stop;
}
PxWin32FileTaskWrite;

typedef struct PxWin32FileTaskRead
{
    PxWin32File* file;
    u8*          values;
    ssize        start;
    ssize        stop;
}
PxWin32FileTaskRead;

typedef struct PxWin32FileTask
{
    PxFileEventKind kind;

    union
    {
        PxWin32FileTaskWrite write;
        PxWin32FileTaskRead  read;
    };
}
PxWin32FileTask;

b32
pxWin32FileWriteAsync(PxWin32Async* async, void* tag, PxWin32File* self, u8* values, ssize start, ssize stop);

b32
pxWin32FileReadAsync(PxWin32Async* async, void* tag, PxWin32File* self, u8* values, ssize start, ssize stop);

#endif // PX_WIN32_ASYNC_STORAGE_FILE_H
