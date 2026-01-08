#ifndef PX_BASE_DEFINE_H
#define PX_BASE_DEFINE_H

#define pxMin(v1, v2) ((v1) < (v2) ? (v1) : (v2))
#define pxMax(v1, v2) ((v1) < (v2) ? (v2) : (v1))

#define pxClampTop(value, max) pxMin(value, max)
#define pxClampBot(value, min) pxMax(value, min)

#define pxClamp(value, min, max) pxMax(min, pxMin(value, max))

#endif // PX_BASE_DEFINE_H
