#ifndef RN_BASE_PLATFORM_H
#define RN_BASE_PLATFORM_H

#define RN_WORD_NONE  0
#define RN_WORD_32    1
#define RN_WORD_64    2
#define RN_WORD_COUNT 3

#ifndef RN_WORD

    #if defined(_WIN64)     || \
        defined(__PL64__)   || \
        defined(__x86_64__) || \
        defined(__amd64__)  || \
        defined(__aarch64__)

        #define RN_WORD RN_WORD_64

    #elif defined(_WIN32)    || \
          defined(__ILP32__) || \
          defined(_i386_)

        #define RN_WORD RN_WORD_32

    #else

        #define RN_WORD RN_WORD_NONE

    #endif

#endif

#define RN_SYSTEM_NONE    0
#define RN_SYSTEM_WINDOWS 1
#define RN_SYSTEM_LINUX   2
#define RN_SYSTEM_MAX     3

#ifndef RN_SYSTEM

    #if defined(_WIN32) || \
        defined(_WIN64)

        #define RN_SYSTEM RN_SYSTEM_WINDOWS

    #elif defined(__linux__)

        #define RN_SYSTEM RN_SYSTEM_LINUX

    #else

        #define RN_SYSTEM RN_SYSTEM_NONE

    #endif

#endif

#endif // RN_BASE_PLATFORM_H
