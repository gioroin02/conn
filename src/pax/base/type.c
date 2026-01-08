#ifndef PX_BASE_TYPE_C
#define PX_BASE_TYPE_C

#include "type.h"

ssize pxKilo(ssize value)
{
    return value < 0 || value > PX_SSIZE_MAX / PX_SSIZE_KILO ? 0 : value * PX_SSIZE_KILO;
}

ssize pxMega(ssize value)
{
    return value < 0 || value > PX_SSIZE_MAX / PX_SSIZE_MEGA ? 0 : value * PX_SSIZE_MEGA;
}

ssize pxGiga(ssize value)
{
    return value < 0 || value > PX_SSIZE_MAX / PX_SSIZE_GIGA ? 0 : value * PX_SSIZE_GIGA;
}

ssize pxKibi(ssize value)
{
    return value < 0 || value > PX_SSIZE_MAX / PX_SSIZE_KIBI ? 0 : value * PX_SSIZE_KIBI;
}

ssize pxMebi(ssize value)
{
    return value < 0 || value > PX_SSIZE_MAX / PX_SSIZE_MEBI ? 0 : value * PX_SSIZE_MEBI;
}

ssize pxGibi(ssize value)
{
    return value < 0 || value > PX_SSIZE_MAX / PX_SSIZE_GIBI ? 0 : value * PX_SSIZE_GIBI;
}

#endif // PX_BASE_TYPE_C
