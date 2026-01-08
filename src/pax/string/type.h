#ifndef PX_BASE_STRING_TYPE_H
#define PX_BASE_STRING_TYPE_H

#include "import.h"

#define pxStr8(arr)  pxStr8Make(((u8*) arr),   sizeof (arr) / sizeof *(arr))
#define pxStr16(arr) pxStr16Make(((u16*) arr), sizeof (arr) / sizeof *(arr))
#define pxStr32(arr) pxStr32Make(((u32*) arr), sizeof (arr) / sizeof *(arr))

typedef struct PxStr8
{
    u8*   values;
    ssize size;
}
PxStr8;

typedef struct PxStr16
{
    u16*  values;
    ssize size;
}
PxStr16;

typedef struct PxStr32
{
    u32*  values;
    ssize size;
}
PxStr32;

PxStr8 pxStr8Make(u8* values, ssize size);

PxStr16 pxStr16Make(u16* values, ssize size);

PxStr32 pxStr32Make(u32* values, ssize size);

#endif // PX_BASE_STRING_TYPE_H
