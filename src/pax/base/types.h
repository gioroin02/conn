#ifndef PX_BASE_NUMBERS_H
#define PX_BASE_NUMBERS_H

#include "platform.h"
#include "defines.h"

/* Signed numbers */

typedef char      s8;
typedef short     s16;
typedef long      s32;
typedef long long s64;

#define PX_MAX_S8  ((s8)  0x7f)
#define PX_MAX_S16 ((s16) 0x7fff)
#define PX_MAX_S32 ((s32) 0x7fffffffl)
#define PX_MAX_S64 ((s64) 0x7fffffffffffffffll)

#define PX_MIN_S8  ((s8)  0x80)
#define PX_MIN_S16 ((s16) 0x8000)
#define PX_MIN_S32 ((s32) 0x80000000l)
#define PX_MIN_S64 ((s64) 0x8000000000000000ll)

/* Unsigned numbers */

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned long      u32;
typedef unsigned long long u64;

#define PX_MAX_U8  ((u8)  0xffu)
#define PX_MAX_U16 ((u16) 0xffffu)
#define PX_MAX_U32 ((u32) 0xfffffffflu)
#define PX_MAX_U64 ((u64) 0xffffffffffffffffllu)

#define PX_MIN_U8  ((u8)  0x0u)
#define PX_MIN_U16 ((u16) 0x0u)
#define PX_MIN_U32 ((u32) 0x0lu)
#define PX_MIN_U64 ((u64) 0x0llu)

/* Floating numbers */

typedef float  f32;
typedef double f64;

/* Boolean values */

typedef u8  b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

#if PX_WORD == PX_WORD_32

    typedef s32 ssize;
    typedef u32 usize;
    typedef f32 fsize;
    typedef b32 bsize;

    #define PX_MAX_SSIZE PX_MAX_S32
    #define PX_MIN_SSIZE PX_MIN_S32

    #define PX_MAX_USIZE PX_MAX_U32
    #define PX_MIN_USIZE PX_MIN_U32

#elif PX_WORD == PX_WORD_64

    typedef s64 ssize;
    typedef u64 usize;
    typedef f64 fsize;
    typedef b64 bsize;

    #define PX_MAX_SSIZE PX_MAX_S64
    #define PX_MIN_SSIZE PX_MIN_S64

    #define PX_MAX_USIZE PX_MAX_U64
    #define PX_MIN_USIZE PX_MIN_U64

#else

    #error "Unknown word size"

#endif

#endif // PX_BASE_NUMBERS_H
