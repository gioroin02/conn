#ifndef PX_BASE_DEFINE_H
#define PX_BASE_DEFINE_H

#define PX_NULL ((void*) 0)

#define pxMin(a, b) ((a) < (b) ? (a) : (b))
#define pxMax(a, b) ((a) < (b) ? (b) : (a))

#define pxClampTop(x, max) pxMin(x, max)
#define pxClampBot(x, min) pxMax(x, min)

#define pxClamp(x, min, max) pxMax(min, pxMin(x, max))

#define __pxText__(expr) # expr
#define __pxGlue__(a, b) a ## b

#define pxText(expr) __pxText__(expr)
#define pxGlue(a, b) __pxGlue__(a, b)

#endif // PX_BASE_DEFINE_H
