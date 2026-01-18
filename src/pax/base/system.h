#ifndef P_BASE_SYSTEM_H
#define P_BASE_SYSTEM_H

#ifndef NULL

    #define NULL ((void*) 0)

#endif

#define P_WORD_NONE  0
#define P_WORD_32    1
#define P_WORD_64    2
#define P_WORD_COUNT 3

#ifndef P_WORD

    #if defined(_WIN64)     || \
        defined(__PL64__)   || \
        defined(__x86_64__) || \
        defined(__amd64__)  || \
        defined(__aarch64__)

        #define P_WORD P_WORD_64

    #elif defined(_WIN32)    || \
          defined(__ILP32__) || \
          defined(_i386_)

        #define P_WORD P_WORD_32

    #else

        #define P_WORD P_WORD_NONE

    #endif

#endif

#define P_SYSTEM_NONE    0
#define P_SYSTEM_WINDOWS 1
#define P_SYSTEM_LINUX   2
#define P_SYSTEM_COUNT   3

#ifndef P_SYSTEM

    #if defined(_WIN32) || \
        defined(_WIN64)

        #define P_SYSTEM P_SYSTEM_WINDOWS

    #elif defined(__linux__)

        #define P_SYSTEM P_SYSTEM_LINUX

    #else

        #define P_SYSTEM P_SYSTEM_NONE

    #endif

#endif

#endif // P_BASE_SYSTEM_H
