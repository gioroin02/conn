#ifndef PX_SYSTEM_STORAGE_FILE_C
#define PX_SYSTEM_STORAGE_FILE_C

#include "file.h"

#if PX_SYSTEM == PX_SYSTEM_WINDOWS

    #include "../win32/storage/export.c"

    #define __pxFileReserve__       pxWin32FileReserve
    #define __pxFileConsoleInput__  pxWin32FileConsoleInput
    #define __pxFileConsoleOutput__ pxWin32FileConsoleOutput
    #define __pxFileConsoleError__  pxWin32FileConsoleError
    #define __pxFileDestroy__       pxWin32FileDestroy
    #define __pxFileWrite__         pxWin32FileWrite
    #define __pxFileRead__          pxWin32FileRead

#else

    #error "Unknown platform"

#endif

PxFile*
pxFileReserve(PxMemoryArena* arena)
{
    return __pxFileReserve__(arena);
}

b32 pxFileConsoleInput(PxFile* self)
{
    return __pxFileConsoleInput__(self);
}

b32 pxFileConsoleOutput(PxFile* self)
{
    return __pxFileConsoleOutput__(self);
}

b32 pxFileConsoleError(PxFile* self)
{
    return __pxFileConsoleError__(self);
}

void pxFileDestroy(PxFile* self)
{
    return __pxFileDestroy__(self);
}

ssize pxFileWrite(PxFile* self, u8* pntr, ssize start, ssize stop)
{
    return __pxFileWrite__(self, pntr, start, stop);
}

ssize pxFileRead(PxFile* self, u8* pntr, ssize start, ssize stop)
{
    return __pxFileRead__(self, pntr, start, stop);
}

#endif // PX_SYSTEM_STORAGE_FILE_C
