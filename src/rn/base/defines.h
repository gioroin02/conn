#ifndef RN_BASE_DEFINES_H
#define RN_BASE_DEFINES_H

#define rnMin(a, b) ((a) < (b) ? (a) : (b))
#define rnMax(a, b) ((a) < (b) ? (b) : (a))

#define rnClampTop(x, max) rnMin(x, max)
#define rnClampBot(x, min) rnMax(x, min)

#define rnClamp(x, min, max) rnMax(min, rnMin(x, max))

#define __rnText__(expr) # expr
#define __rnGlue__(a, b) a ## b
#define __rnEval__(expr) (expr)

#define rnText(expr) __rnText__(expr)
#define rnGlue(a, b) __rnGlue__(a, b)
#define rnEval(expr) __rnEval__(expr)

#endif // RN_BASE_DEFINES_H
