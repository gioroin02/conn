#ifndef PX_SYSTEM_ASYNC_STORAGE_FILE_C
#define PX_SYSTEM_ASYNC_STORAGE_FILE_C

#include "file.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../../win32/storage/async/export.c"

    #define __pxFileWriteAsync__ pxWin32FileWriteAsync
    #define __pxFileReadAsync__  pxWin32FileReadAsync

#else

    #error "Unknown platform"

#endif

PxFileEvent pxFileEventWrite(void* ctxt, PxFile* self, u8* pntr, ssize start, ssize stop)
{
    PxFileEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind        = PxFileEvent_Write;
    result.ctxt        = ctxt;
    result.self        = self;
    result.write.pntr  = pntr;
    result.write.start = start,
    result.write.stop  = stop;

    return result;
}

PxFileEvent pxFileEventRead(void* ctxt, PxFile* self, u8* pntr, ssize start, ssize stop)
{
    PxFileEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind       = PxFileEvent_Read;
    result.ctxt       = ctxt;
    result.self       = self;
    result.read.pntr  = pntr;
    result.read.start = start,
    result.read.stop  = stop;

    return result;
}

PxFileEvent pxFileEventClose(void* ctxt, PxFile* self)
{
    PxFileEvent result;

    pxMemorySet(&result, sizeof result, 0xAB);

    result.kind = PxFileEvent_Close;
    result.ctxt = ctxt;
    result.self = self;

    return result;
}

b32 pxFileWriteAsync(PxAsync* async, void* ctxt, PxFile* self, u8* pntr, ssize start, ssize stop)
{
    return __pxFileWriteAsync__(async, ctxt, self, pntr, start, stop);
}

b32 pxFileReadAsync(PxAsync* async, void* ctxt, PxFile* self, u8* pntr, ssize start, ssize stop)
{
    return __pxFileReadAsync__(async, ctxt, self, pntr, start, stop);
}

#endif // PX_SYSTEM_ASYNC_STORAGE_FILE_C
