#include "defines_float.h"
#include "FloatingPoint/floating-point.h"
#include "FloatingPoint/fp-math.h"

using namespace sci ;
using namespace std ;

IOPack *iopackArr[MAX_THREADS] ;
OTPack *otpackArr[MAX_THREADS] ;

BoolOp *boolopArr[MAX_THREADS] ;
FixOp *fixopArr[MAX_THREADS] ;
FPOp *fpopArr[MAX_THREADS] ;
FPMath *fpmathArr[MAX_THREADS] ;