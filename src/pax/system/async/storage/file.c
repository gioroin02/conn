#ifndef PX_SYSTEM_ASYNC_STORAGE_FILE_C
#define PX_SYSTEM_ASYNC_STORAGE_FILE_C

#include "file.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../../win32/async/storage/export.c"

    #define __pxFileWriteAsync__ pxWin32FileWriteAsync
    #define __pxFileReadAsync__  pxWin32FileReadAsync

#else

    #error "Unknown platform"

#endif

PxFileEvent
pxFileEventWrite(PxFile* file, u8* values, ssize start, ssize stop)
{
    PxFileEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind         = PxFileEvent_Write;
    result.write.file   = file;
    result.write.values = values;
    result.write.start  = start,
    result.write.stop   = stop;

    return result;
}

PxFileEvent
pxFileEventRead(PxFile* file, u8* values, ssize start, ssize stop)
{
    PxFileEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind        = PxFileEvent_Read;
    result.read.file   = file;
    result.read.values = values;
    result.read.start  = start,
    result.read.stop   = stop;

    return result;
}

PxFileEvent
pxFileEventClose(PxFile* file)
{
    PxFileEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind       = PxFileEvent_Close;
    result.close.file = file;

    return result;
}

b32
pxFileWriteAsync(PxAsync* async, void* tag, PxFile* self, u8* values, ssize start, ssize stop)
{
    return __pxFileWriteAsync__(async, tag, self, values, start, stop);
}

b32
pxFileReadAsync(PxAsync* async, void* tag, PxFile* self, u8* values, ssize start, ssize stop)
{
    return __pxFileReadAsync__(async, tag, self, values, start, stop);
}

#endif // PX_SYSTEM_ASYNC_STORAGE_FILE_C
