#ifndef PX_SYSTEM_ASYNC_STORAGE_FILE_H
#define PX_SYSTEM_ASYNC_STORAGE_FILE_H

#include "import.h"

typedef enum PxFileEventKind
{
    PxFileEvent_None,
    PxFileEvent_Error,
    PxFileEvent_Write,
    PxFileEvent_Read,
    PxFileEvent_Close,
}
PxFileEventKind;

typedef struct PxFileEventWrite
{
    PxFile* file;
    u8*     values;
    ssize   start;
    ssize   stop;
}
PxFileEventWrite;

typedef struct PxFileEventRead
{
    PxFile* file;
    u8*     values;
    ssize   start;
    ssize   stop;
}
PxFileEventRead;

typedef struct PxFileEventClose
{
    PxFile* file;
}
PxFileEventClose;

typedef struct PxFileEvent
{
    PxFileEventKind kind;

    union
    {
        PxFileEventWrite write;
        PxFileEventRead  read;
        PxFileEventClose close;
    };
}
PxFileEvent;

PxFileEvent
pxFileEventWrite(PxFile* file, u8* values, ssize start, ssize stop);

PxFileEvent
pxFileEventRead(PxFile* file, u8* values, ssize start, ssize stop);

PxFileEvent
pxFileEventClose(PxFile* file);

b32
pxFileWriteAsync(PxAsync* async, void* tag, PxFile* self, u8* values, ssize start, ssize stop);

b32
pxFileReadAsync(PxAsync* async, void* tag, PxFile* self, u8* values, ssize start, ssize stop);

#endif // PX_SYSTEM_ASYNC_STORAGE_FILE_H
