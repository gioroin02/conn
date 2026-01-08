#ifndef PX_BASE_TYPE_H
#define PX_BASE_TYPE_H

#include "define.h"
#include "system.h"

/* Signed numbers */

typedef char      s8;
typedef short     s16;
typedef long      s32;
typedef long long s64;

#define PX_S8_MAX  ((s8)  0x7f)
#define PX_S16_MAX ((s16) 0x7fff)
#define PX_S32_MAX ((s32) 0x7fffffffl)
#define PX_S64_MAX ((s64) 0x7fffffffffffffffll)

#define PX_S8_MIN  ((s8)  0x80)
#define PX_S16_MIN ((s16) 0x8000)
#define PX_S32_MIN ((s32) 0x80000000l)
#define PX_S64_MIN ((s64) 0x8000000000000000ll)

/* Unsigned numbers */

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned long      u32;
typedef unsigned long long u64;

#define PX_U8_MAX  ((u8)  0xffu)
#define PX_U16_MAX ((u16) 0xffffu)
#define PX_U32_MAX ((u32) 0xfffffffflu)
#define PX_U64_MAX ((u64) 0xffffffffffffffffllu)

#define PX_U8_MIN  ((u8)  0x0u)
#define PX_U16_MIN ((u16) 0x0u)
#define PX_U32_MIN ((u32) 0x0lu)
#define PX_U64_MIN ((u64) 0x0llu)

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

    #define PX_SSIZE_MAX PX_S32_MAX
    #define PX_SSIZE_MIN PX_S32_MIN

    #define PX_USIZE_MAX PX_U32_MAX
    #define PX_USIZE_MIN PX_U32_MIN

#elif PX_WORD == PX_WORD_64

    typedef s64 ssize;
    typedef u64 usize;
    typedef f64 fsize;
    typedef b64 bsize;

    #define PX_SSIZE_MAX PX_S64_MAX
    #define PX_SSIZE_MIN PX_S64_MIN

    #define PX_USIZE_MAX PX_U64_MAX
    #define PX_USIZE_MIN PX_U64_MIN

#else

    #error "Unknown word size"

#endif

#define PX_SSIZE_KILO ((ssize) 1000ll)
#define PX_SSIZE_MEGA ((ssize) 1000000ll)
#define PX_SSIZE_GIGA ((ssize) 1000000000ll)

#define PX_SSIZE_KIBI ((ssize) 1024ll)
#define PX_SSIZE_MEBI ((ssize) 1048576ll)
#define PX_SSIZE_GIBI ((ssize) 1073741824ll)

ssize pxKilo(ssize value);
ssize pxMega(ssize value);
ssize pxGiga(ssize value);

ssize pxKibi(ssize value);
ssize pxMebi(ssize value);
ssize pxGibi(ssize value);

#endif // PX_BASE_TYPE_H
