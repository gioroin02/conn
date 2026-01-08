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
    u8*   pntr;
    ssize start;
    ssize stop;
}
PxFileEventWrite;

typedef struct PxFileEventRead
{
    u8*   pntr;
    ssize start;
    ssize stop;
}
PxFileEventRead;

typedef struct PxFileEvent
{
    PxFileEventKind kind;

    void*   ctxt;
    PxFile* self;

    union
    {
        PxFileEventWrite write;
        PxFileEventRead  read;
    };
}
PxFileEvent;

PxFileEvent pxFileEventWrite(void* ctxt, PxFile* self, u8* pntr, ssize start, ssize stop);

PxFileEvent pxFileEventRead(void* ctxt, PxFile* self, u8* pntr, ssize start, ssize stop);

PxFileEvent pxFileEventClose(void* ctxt, PxFile* self);

b32 pxFileWriteAsync(PxAsync* async, void* ctxt, PxFile* self, u8* pntr, ssize start, ssize stop);

b32 pxFileReadAsync(PxAsync* async, void* ctxt, PxFile* self, u8* pntr, ssize start, ssize stop);

#endif // PX_SYSTEM_ASYNC_STORAGE_FILE_H
