#ifndef GLOBALS_FLOAT_H___
#define GLOBALS_FLOAT_H___

#include "defines_float.h"
#include "FloatingPoint/floating-point.h"
#include "FloatingPoint/fp-math.h"

using namespace sci ;
using namespace std ;

extern IOPack *iopackArr[MAX_THREADS] ;
extern OTPack *otpackArr[MAX_THREADS] ;

extern BoolOp *boolopArr[MAX_THREADS] ;
extern FixOp *fixopArr[MAX_THREADS] ;
extern FPOp *fpopArr[MAX_THREADS] ;
extern FPMath *fpmathArr[MAX_THREADS] ;

#endif