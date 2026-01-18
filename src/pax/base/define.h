#ifndef P_BASE_DEFINE_H
#define P_BASE_DEFINE_H

#define pMin(v0, v1) ((v0) < (v1) ? (v0) : (v1))
#define pMax(v0, v1) ((v0) < (v1) ? (v1) : (v0))

#define pClampTop(value, max) pMin(value, max)
#define pClampBot(value, min) pMax(value, min)

#define pClamp(value, min, max) pMax(min, pMin(value, max))

#endif // P_BASE_DEFINE_H
