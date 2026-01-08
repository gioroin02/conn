#ifndef PX_WIN32_STORAGE_FILE_C
#define PX_WIN32_STORAGE_FILE_C

#include "file.h"

PxWin32File* pxWin32FileReserve(PxMemoryArena* arena)
{
    return pxMemoryArenaReserveOneOf(arena, PxWin32File);
}

b32 pxWin32FileConsoleInput(PxWin32File* self)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    HANDLE stdin = GetStdHandle(STD_INPUT_HANDLE);

    if (stdin == INVALID_HANDLE_VALUE) return 0;

    self->handle = stdin;

    return 1;
}

b32 pxWin32FileConsoleOutput(PxWin32File* self)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (stdout == INVALID_HANDLE_VALUE) return 0;

    self->handle = stdout;

    return 1;
}

b32 pxWin32FileConsoleError(PxWin32File* self)
{
    pxMemorySet(self, sizeof *self, 0xAB);

    HANDLE stderr = GetStdHandle(STD_ERROR_HANDLE);

    if (stderr == INVALID_HANDLE_VALUE) return 0;

    self->handle = stderr;

    return 1;
}

void pxWin32FileDestroy(PxWin32File* self)
{
    if (self->handle != INVALID_HANDLE_VALUE)
        CloseHandle(self->handle);

    pxMemorySet(self, sizeof *self, 0xAB);
}

ssize pxWin32FileWrite(PxWin32File* self, u8* pntr, ssize start, ssize stop)
{
    if (pntr == PX_NULL || stop <= start || start < 0) return 0;

    char* memory = ((char*) pntr + start);
    ssize size   = stop - start;
    ssize result = 0;

    pxMemorySet(&self->overlap, sizeof self->overlap, 0x00);

    self->overlap.hEvent = CreateEvent(PX_NULL, 1, 0, PX_NULL);

    if (self->overlap.hEvent == PX_NULL) return 0;

    while (result < size) {
        DWORD count = 0;

        b32 status = WriteFile(self->handle, memory + result,
            ((DWORD) size - result), &count, &self->overlap);

        if (status == 0 && GetLastError() == ERROR_IO_PENDING)
            status = GetOverlappedResult(self->handle, &self->overlap, &count, 1);

        if (status != 0 && count > 0 && count <= size - result)
            result += count;
        else
            break;
    }

    CloseHandle(self->overlap.hEvent);

    return result;
}

ssize pxWin32FileRead(PxWin32File* self, u8* pntr, ssize start, ssize stop)
{
    if (pntr == PX_NULL || stop <= start || start < 0) return 0;

    char* memory = ((char*) pntr + start);
    ssize size   = stop - start;
    DWORD count  = 0;

    pxMemorySet(&self->overlap, sizeof self->overlap, 0x00);

    self->overlap.hEvent = CreateEvent(PX_NULL, 1, 0, PX_NULL);

    if (self->overlap.hEvent == PX_NULL) return 0;

    b32 status = ReadFile(self->handle, memory, (DWORD) size, &count, &self->overlap);

    if (status == 0 && GetLastError() == ERROR_IO_PENDING)
        status = GetOverlappedResult(self->handle, &self->overlap, &count, 1);

    CloseHandle(self->overlap.hEvent);

    if (status != 0 && count > 0 && count <= size) return count;

    return 0;
}

#endif // PX_WIN32_STORAGE_FILE_C
