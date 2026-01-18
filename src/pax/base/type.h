#ifndef P_BASE_TYPE_H
#define P_BASE_TYPE_H

#include "define.h"
#include "system.h"

typedef char      I8;
typedef short     I16;
typedef long      I32;
typedef long long I64;

#define P_I8_MAX  ((I8)  0x7f)
#define P_I16_MAX ((I16) 0x7fff)
#define P_I32_MAX ((I32) 0x7fffffffl)
#define P_I64_MAX ((I64) 0x7fffffffffffffffll)

#define P_I8_MIN  ((I8)  0x80)
#define P_I16_MIN ((I16) 0x8000)
#define P_I32_MIN ((I32) 0x80000000l)
#define P_I64_MIN ((I64) 0x8000000000000000ll)

typedef unsigned char      U8;
typedef unsigned short     U16;
typedef unsigned long      U32;
typedef unsigned long long U64;

#define P_U8_MAX  ((U8)  0xffu)
#define P_U16_MAX ((U16) 0xffffu)
#define P_U32_MAX ((U32) 0xfffffffflu)
#define P_U64_MAX ((U64) 0xffffffffffffffffllu)

#define P_U8_MIN  ((U8)  0x0u)
#define P_U16_MIN ((U16) 0x0u)
#define P_U32_MIN ((U32) 0x0lu)
#define P_U64_MIN ((U64) 0x0llu)

typedef float  F32;
typedef double F64;

typedef U8  B8;
typedef U16 B16;
typedef U32 B32;
typedef U64 B64;

#if P_WORD == P_WORD_32

    typedef I32 Int;
    typedef U32 Uint;
    typedef F32 Float;
    typedef B32 Bool;

    #define P_INT_MAX P_I32_MAX
    #define P_INT_MIN P_I32_MIN

    #define P_UINT_MAX P_U32_MAX
    #define P_UINT_MIN P_U32_MIN

#elif P_WORD == P_WORD_64

    typedef I64 Int;
    typedef U64 Uint;
    typedef F64 Float;
    typedef B64 Bool;

    #define P_INT_MAX P_I64_MAX
    #define P_INT_MIN P_I64_MIN

    #define P_UINT_MAX P_U64_MAX
    #define P_UINT_MIN P_U64_MIN

#else

    typedef I16  Int;
    typedef U16 Uint;
    typedef B16 Bool;

    #define P_INT_MAX P_I16_MAX
    #define P_INT_MIN P_I16_MIN

    #define P_UINT_MAX P_U16_MAX
    #define P_UINT_MIN P_U16_MIN

#endif

#endif // P_BASE_TYPE_H
