#ifndef PX_WIN32_STORAGE_FILE_H
#define PX_WIN32_STORAGE_FILE_H

#include "import.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

typedef struct PxWin32File
{
    HANDLE     handle;
    OVERLAPPED overlap;
}
PxWin32File;

PxWin32File*
pxWin32FileReserve(PxMemoryArena* arena);

b32
pxWin32FileConsoleInput(PxWin32File* self);

b32
pxWin32FileConsoleOutput(PxWin32File* self);

b32
pxWin32FileConsoleError(PxWin32File* self);

void
pxWin32FileDestroy(PxWin32File* self);

ssize
pxWin32FileWrite(PxWin32File* self, u8* values, ssize start, ssize stop);

ssize
pxWin32FileRead(PxWin32File* self, u8* values, ssize start, ssize stop);

#endif // PX_WIN32_STORAGE_FILE_H
