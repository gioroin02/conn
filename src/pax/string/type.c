#ifndef P_BASE_STRING_TYPE_C
#define P_BASE_STRING_TYPE_C

#include "type.h"

PString8 pString8Make(U8* values, Int size)
{
    PString8 result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.values = NULL;
    result.size   = 0;

    if (values != NULL && size >= 0) {
        result.values = values;
        result.size   = size;
    }

    return result;
}

PString16 pString16Make(U16* values, Int size)
{
    PString16 result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.values = NULL;
    result.size   = 0;

    if (values != NULL && size >= 0) {
        result.values = values;
        result.size   = size;
    }

    return result;
}

PString32 pString32Make(U32* values, Int size)
{
    PString32 result;

    pMemorySet(&result, sizeof result, 0xAB);

    result.values = NULL;
    result.size   = 0;

    if (values != NULL && size >= 0) {
        result.values = values;
        result.size   = size;
    }

    return result;
}

#endif // P_BASE_STRING_TYPE_C
