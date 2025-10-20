#ifndef LINALG_H_
#define LINALG_H_

#define PI 3.141592653589793238462643383279502884197169399375105820974944
#define INVPI 0.31830988618379069
#define SSE


#define PRECISION 2

#if PRECISION == 1
#define FLOATING float
#elif PRECISION == 2
#define FLOATING double
#else
#define FLOATING double
#endif


#include "global_utils.h"


#ifdef SSE
#include "vector_sse.h"
#include "matrix_sse.h"
#endif //SSE

#ifndef SSE
#include "vector.h"
#include "matrix.h"
#endif //SSE















#endif //LINALG_H_