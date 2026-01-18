#ifndef P_BASE_STRING_TYPE_H
#define P_BASE_STRING_TYPE_H

#include "import.h"

#define pString8(arr)  pString8Make(((U8*) arr),   sizeof (arr) / sizeof *(arr))
#define pString16(arr) pString16Make(((U16*) arr), sizeof (arr) / sizeof *(arr))
#define pString32(arr) pString32Make(((U32*) arr), sizeof (arr) / sizeof *(arr))

typedef struct PString8
{
    U8* values;
    Int size;
}
PString8;

typedef struct PString16
{
    U16* values;
    Int  size;
}
PString16;

typedef struct PString32
{
    U32* values;
    Int  size;
}
PString32;

PString8 pString8Make(U8* values, Int size);

PString16 pString16Make(U16* values, Int size);

PString32 pString32Make(U32* values, Int size);

#endif // P_BASE_STRING_TYPE_H
