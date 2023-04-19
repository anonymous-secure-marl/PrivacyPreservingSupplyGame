
#include <vector>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "library_float.h"

using namespace std ;
using namespace sci ;

extern float intToFloat(int32_t m);
extern void Softmax2(int32_t s1, int32_t s2, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& outArr);
extern void Ln(int32_t s1, vector < FPArray >& inArr, vector < FPArray >& outArr);
extern void Sigmoid(int32_t s1, vector < FPArray >& inArr, vector < FPArray >& outArr);
extern void Tanh(int32_t s1, vector < FPArray >& inArr, vector < FPArray >& outArr);
extern void Relu(int32_t s1, vector < FPArray >& inArr, vector < FPArray >& outArr, vector < BoolArray >& hotArr);
extern void ElemWiseSub(int32_t s1, vector < FPArray >& inArr1, vector < FPArray >& inArr2, vector < FPArray >& outArr);
extern void ElemWiseMul(int32_t s1, vector < FPArray >& inArr1, vector < FPArray >& inArr2, vector < FPArray >& outArr);
extern void ElemWiseDiv(int32_t s1, vector < FPArray >& inArr1, vector < FPArray >& inArr2, vector < FPArray >& outArr);
extern void ElemWiseAdd(int32_t s1, vector < FPArray >& inArr1, vector < FPArray >& inArr2, vector < FPArray >& outArr);
extern void IfElse(int32_t s1, vector < FPArray >& dat, vector < BoolArray >& hot, vector < FPArray >& out, bool flip);
extern void updateWeights(int32_t s, float lr, vector < FPArray >& wt, vector < FPArray >& der);
extern void scalarMultiplication(int32_t s, float scalar, vector < FPArray >& inArr, vector < FPArray >& outArr);
extern void AllOneDividedBySizeArray(int32_t s, vector < FPArray >& inArr);
extern void updateWeightsAdam(int32_t sz, vector < FPArray >& t, float lr, float beta1, float beta2, float eps, vector < FPArray >& inArr, vector < FPArray >& derArr, vector < FPArray >& mArr, vector < FPArray >& vArr);
extern void MatMul(int32_t s1, int32_t s2, int32_t s3, vector < vector < FPArray > >& mat1, vector < vector < FPArray > >& mat2, vector < vector < FPArray > >& mat3);
extern void GemmAdd(int32_t s1, int32_t s2, vector < vector < FPArray > >& prod, vector < FPArray >& bias, vector < vector < FPArray > >& out);
extern void SubtractOne(int32_t s1, vector < FPArray >& inArr, vector < FPArray >& outArr);
extern void getOutDer(int32_t s1, int32_t s2, vector < vector < FPArray > >& batchSoft, vector < vector < FPArray > >& lab, vector < vector < FPArray > >& der);
extern void getBiasDer(int32_t s1, int32_t s2, vector < vector < FPArray > >& der, vector < FPArray >& biasDer);
extern void dotProduct2(int32_t s1, int32_t s2, vector < vector < FPArray > >& arr1, vector < vector < FPArray > >& arr2, vector < FPArray >& outArr);
extern void getLoss(int32_t m, vector < FPArray >& lossTerms, vector < FPArray >& loss);
void Reassign2(int32_t s1, int32_t s2, vector < vector < FPArray > >& arr1, vector < vector < FPArray > >& arr2){
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
arr2[i1][i2] = arr1[i1][i2] ;

}
}
}

void Reassign3(int32_t s1, int32_t s2, int32_t s3, vector < vector < vector < FPArray > > >& arr1, vector < vector < vector < FPArray > > >& arr2){
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
for (uint32_t i3 = 0; i3 < s3; i3++){
arr2[i1][i2][i3] = arr1[i1][i2][i3] ;

}
}
}
}

void Reassign4(int32_t s1, int32_t s2, int32_t s3, int32_t s4, vector < vector < vector < vector < FPArray > > > >& arr1, vector < vector < vector < vector < FPArray > > > >& arr2){
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
for (uint32_t i3 = 0; i3 < s3; i3++){
for (uint32_t i4 = 0; i4 < s4; i4++){
arr2[i1][i2][i3][i4] = arr1[i1][i2][i3][i4] ;

}
}
}
}
}

void Transpose(int32_t s1, int32_t s2, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& outArr){
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
outArr[i1][i2] = inArr[i2][i1] ;

}
}
}

void Ln2(int32_t s1, int32_t s2, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& outArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > inArrFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > outArrFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var1 = (i1 * s2) ;

int32_t __tac_var2 = (__tac_var1 + i2) ;

inArrFlat[__tac_var2] = inArr[i1][i2] ;

}
}
Ln(sz, inArrFlat, outArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var3 = (i1 * s2) ;

int32_t __tac_var4 = (__tac_var3 + i2) ;

outArr[i1][i2] = outArrFlat[__tac_var4] ;

}
}
}

void Sigmoid2(int32_t s1, int32_t s2, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& outArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > inArrFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > outArrFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var5 = (i1 * s2) ;

int32_t __tac_var6 = (__tac_var5 + i2) ;

inArrFlat[__tac_var6] = inArr[i1][i2] ;

}
}
Sigmoid(sz, inArrFlat, outArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var7 = (i1 * s2) ;

int32_t __tac_var8 = (__tac_var7 + i2) ;

outArr[i1][i2] = outArrFlat[__tac_var8] ;

}
}
}

void Tanh2(int32_t s1, int32_t s2, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& outArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > inArrFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > outArrFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var9 = (i1 * s2) ;

int32_t __tac_var10 = (__tac_var9 + i2) ;

inArrFlat[__tac_var10] = inArr[i1][i2] ;

}
}
Tanh(sz, inArrFlat, outArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var11 = (i1 * s2) ;

int32_t __tac_var12 = (__tac_var11 + i2) ;

outArr[i1][i2] = outArrFlat[__tac_var12] ;

}
}
}

void Relu2(int32_t s1, int32_t s2, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& outArr, vector < vector < BoolArray > >& hotArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > inArrFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > outArrFlat = make_vector_float(ALICE, sz) ;

vector < BoolArray > hotArrFlat = make_vector_bool(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var13 = (i1 * s2) ;

int32_t __tac_var14 = (__tac_var13 + i2) ;

inArrFlat[__tac_var14] = inArr[i1][i2] ;

}
}
Relu(sz, inArrFlat, outArrFlat, hotArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var15 = (i1 * s2) ;

int32_t __tac_var16 = (__tac_var15 + i2) ;

outArr[i1][i2] = outArrFlat[__tac_var16] ;

int32_t __tac_var17 = __tac_var15 ;

int32_t __tac_var18 = __tac_var16 ;

hotArr[i1][i2] = hotArrFlat[__tac_var16] ;

}
}
}

void Relu3(int32_t s1, int32_t s2, int32_t s3, vector < vector < vector < FPArray > > >& inArr, vector < vector < vector < FPArray > > >& outArr, vector < vector < vector < BoolArray > > >& hotArr){
int32_t __tac_var19 = (s1 * s2) ;

int32_t sz = (__tac_var19 * s3) ;

vector < FPArray > inArrFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > outArrFlat = make_vector_float(ALICE, sz) ;

vector < BoolArray > hotArrFlat = make_vector_bool(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
for (uint32_t i3 = 0; i3 < s3; i3++){
int32_t __tac_var20 = (i1 * s2) ;

int32_t __tac_var21 = (__tac_var20 * s3) ;

int32_t __tac_var22 = (i2 * s3) ;

int32_t __tac_var23 = (__tac_var21 + __tac_var22) ;

int32_t __tac_var24 = (__tac_var23 + i3) ;

inArrFlat[__tac_var24] = inArr[i1][i2][i3] ;

}
}
}
Relu(sz, inArrFlat, outArrFlat, hotArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
for (uint32_t i3 = 0; i3 < s3; i3++){
int32_t __tac_var25 = (i1 * s2) ;

int32_t __tac_var26 = (__tac_var25 * s3) ;

int32_t __tac_var27 = (i2 * s3) ;

int32_t __tac_var28 = (__tac_var26 + __tac_var27) ;

int32_t __tac_var29 = (__tac_var28 + i3) ;

outArr[i1][i2][i3] = outArrFlat[__tac_var29] ;

int32_t __tac_var30 = __tac_var25 ;

int32_t __tac_var31 = __tac_var26 ;

int32_t __tac_var32 = __tac_var27 ;

int32_t __tac_var33 = __tac_var28 ;

int32_t __tac_var34 = __tac_var29 ;

hotArr[i1][i2][i3] = hotArrFlat[__tac_var29] ;

}
}
}
}

void ElemWiseMul2(int32_t s1, int32_t s2, vector < vector < FPArray > >& arr1, vector < vector < FPArray > >& arr2, vector < vector < FPArray > >& outArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > arr1Flat = make_vector_float(ALICE, sz) ;

vector < FPArray > arr2Flat = make_vector_float(ALICE, sz) ;

vector < FPArray > outArrFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var35 = (i1 * s2) ;

int32_t __tac_var36 = (__tac_var35 + i2) ;

arr1Flat[__tac_var36] = arr1[i1][i2] ;

int32_t __tac_var37 = __tac_var35 ;

int32_t __tac_var38 = __tac_var36 ;

arr2Flat[__tac_var38] = arr2[i1][i2] ;

}
}
ElemWiseMul(sz, arr1Flat, arr2Flat, outArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var39 = (i1 * s2) ;

int32_t __tac_var40 = (__tac_var39 + i2) ;

outArr[i1][i2] = outArrFlat[__tac_var40] ;

}
}
}

void ElemWiseMul3(int32_t s1, int32_t s2, int32_t s3, vector < vector < vector < FPArray > > >& arr1, vector < vector < vector < FPArray > > >& arr2, vector < vector < vector < FPArray > > >& outArr){
int32_t __tac_var41 = (s1 * s2) ;

int32_t sz = (__tac_var41 * s3) ;

vector < FPArray > arr1Flat = make_vector_float(ALICE, sz) ;

vector < FPArray > arr2Flat = make_vector_float(ALICE, sz) ;

vector < FPArray > outArrFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
for (uint32_t i3 = 0; i3 < s3; i3++){
int32_t __tac_var42 = (i1 * s2) ;

int32_t __tac_var43 = (__tac_var42 * s3) ;

int32_t __tac_var44 = (i2 * s3) ;

int32_t __tac_var45 = (__tac_var43 + __tac_var44) ;

int32_t __tac_var46 = (__tac_var45 + i3) ;

arr1Flat[__tac_var46] = arr1[i1][i2][i3] ;

int32_t __tac_var47 = __tac_var42 ;

int32_t __tac_var48 = __tac_var43 ;

int32_t __tac_var49 = __tac_var44 ;

int32_t __tac_var50 = __tac_var45 ;

int32_t __tac_var51 = __tac_var46 ;

arr2Flat[__tac_var51] = arr2[i1][i2][i3] ;

}
}
}
ElemWiseMul(sz, arr1Flat, arr2Flat, outArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
for (uint32_t i3 = 0; i3 < s3; i3++){
int32_t __tac_var52 = (i1 * s2) ;

int32_t __tac_var53 = (__tac_var52 * s3) ;

int32_t __tac_var54 = (i2 * s3) ;

int32_t __tac_var55 = (__tac_var53 + __tac_var54) ;

int32_t __tac_var56 = (__tac_var55 + i3) ;

outArr[i1][i2][i3] = outArrFlat[__tac_var56] ;

}
}
}
}

void IfElse2(int32_t s1, int32_t s2, vector < vector < FPArray > >& dat, vector < vector < BoolArray > >& hot, vector < vector < FPArray > >& out, bool flip){
int32_t sz = (s1 * s2) ;

vector < FPArray > datFlat = make_vector_float(ALICE, sz) ;

vector < BoolArray > hotFlat = make_vector_bool(ALICE, sz) ;

vector < FPArray > outFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var57 = (i1 * s2) ;

int32_t __tac_var58 = (__tac_var57 + i2) ;

datFlat[__tac_var58] = dat[i1][i2] ;

int32_t __tac_var59 = __tac_var57 ;

int32_t __tac_var60 = __tac_var58 ;

hotFlat[__tac_var60] = hot[i1][i2] ;

}
}
IfElse(sz, datFlat, hotFlat, outFlat, flip);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var61 = (i1 * s2) ;

int32_t __tac_var62 = (__tac_var61 + i2) ;

out[i1][i2] = outFlat[__tac_var62] ;

}
}
}

void updateWeights2(int32_t s1, int32_t s2, float lr, vector < vector < FPArray > >& wt, vector < vector < FPArray > >& der){
int32_t sz = (s1 * s2) ;

vector < FPArray > wtFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > derFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var63 = (i1 * s2) ;

int32_t __tac_var64 = (__tac_var63 + i2) ;

wtFlat[__tac_var64] = wt[i1][i2] ;

int32_t __tac_var65 = __tac_var63 ;

int32_t __tac_var66 = __tac_var64 ;

derFlat[__tac_var66] = der[i1][i2] ;

}
}
updateWeights(sz, lr, wtFlat, derFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var67 = (i1 * s2) ;

int32_t __tac_var68 = (__tac_var67 + i2) ;

wt[i1][i2] = wtFlat[__tac_var68] ;

}
}
}

void updateWeightsAdam2(int32_t s1, int32_t s2, vector < vector < FPArray > >& t, float lr, float beta1, float beta2, float eps, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& derArr, vector < vector < FPArray > >& mArr, vector < vector < FPArray > >& vArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > inFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > derFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > mFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > vFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > tFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var69 = (i1 * s2) ;

int32_t __tac_var70 = (__tac_var69 + i2) ;

inFlat[__tac_var70] = inArr[i1][i2] ;

int32_t __tac_var71 = __tac_var69 ;

int32_t __tac_var72 = __tac_var70 ;

derFlat[__tac_var72] = derArr[i1][i2] ;

int32_t __tac_var73 = __tac_var69 ;

int32_t __tac_var74 = __tac_var70 ;

mFlat[__tac_var74] = mArr[i1][i2] ;

int32_t __tac_var75 = __tac_var69 ;

int32_t __tac_var76 = __tac_var70 ;

vFlat[__tac_var76] = vArr[i1][i2] ;

int32_t __tac_var77 = __tac_var69 ;

int32_t __tac_var78 = __tac_var70 ;

tFlat[__tac_var78] = t[i1][i2] ;

}
}
updateWeightsAdam(sz, tFlat, lr, beta1, beta2, eps, inFlat, derFlat, mFlat, vFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var79 = (i1 * s2) ;

int32_t __tac_var80 = (__tac_var79 + i2) ;

inArr[i1][i2] = inFlat[__tac_var80] ;

int32_t __tac_var81 = __tac_var79 ;

int32_t __tac_var82 = __tac_var80 ;

mArr[i1][i2] = mFlat[__tac_var80] ;

int32_t __tac_var83 = __tac_var79 ;

int32_t __tac_var84 = __tac_var80 ;

vArr[i1][i2] = vFlat[__tac_var80] ;

int32_t __tac_var85 = __tac_var79 ;

int32_t __tac_var86 = __tac_var80 ;

t[i1][i2] = tFlat[__tac_var80] ;

}
}
}

void computeMSELoss(int32_t m, int32_t s, vector < vector < FPArray > >& target, vector < vector < FPArray > >& fwdOut, vector < FPArray >& loss){
vector < FPArray > targetFlat = make_vector_float(ALICE, m) ;

vector < FPArray > outFlat = make_vector_float(ALICE, m) ;

for (uint32_t i = 0; i < m; i++){
targetFlat[i] = target[i][0] ;

outFlat[i] = fwdOut[i][0] ;

}
vector < FPArray > subbed = make_vector_float(ALICE, m) ;

vector < FPArray > lossTerms = make_vector_float(ALICE, m) ;

ElemWiseSub(m, outFlat, targetFlat, subbed);
ElemWiseMul(m, subbed, subbed, lossTerms);
getLoss(m, lossTerms, loss);
}

void computeCELoss(int32_t m, int32_t s, vector < vector < FPArray > >& labels, vector < vector < FPArray > >& batchSoft, vector < FPArray >& loss){
vector < vector < FPArray > > batchLn = make_vector_float(ALICE, m, s) ;

vector < FPArray > lossTerms = make_vector_float(ALICE, m) ;

Ln2(m, s, batchSoft, batchLn);
dotProduct2(m, s, batchLn, labels, lossTerms);
getLoss(m, lossTerms, loss);
}

void ElemWiseAdd2(int32_t s1, int32_t s2, vector < vector < FPArray > >& arr1, vector < vector < FPArray > >& arr2, vector < vector < FPArray > >& outArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > arr1Flat = make_vector_float(ALICE, sz) ;

vector < FPArray > arr2Flat = make_vector_float(ALICE, sz) ;

vector < FPArray > outArrFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var87 = (i1 * s2) ;

int32_t __tac_var88 = (__tac_var87 + i2) ;

arr1Flat[__tac_var88] = arr1[i1][i2] ;

int32_t __tac_var89 = __tac_var87 ;

int32_t __tac_var90 = __tac_var88 ;

arr2Flat[__tac_var90] = arr2[i1][i2] ;

}
}
ElemWiseAdd(sz, arr1Flat, arr2Flat, outArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var91 = (i1 * s2) ;

int32_t __tac_var92 = (__tac_var91 + i2) ;

outArr[i1][i2] = outArrFlat[__tac_var92] ;

}
}
}

void scalarMultiplication2(int32_t s1, int32_t s2, float scalar, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& outArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > arr1Flat = make_vector_float(ALICE, sz) ;

vector < FPArray > outArrFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var93 = (i1 * s2) ;

int32_t __tac_var94 = (__tac_var93 + i2) ;

arr1Flat[__tac_var94] = inArr[i1][i2] ;

}
}
scalarMultiplication(sz, scalar, arr1Flat, outArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var95 = (i1 * s2) ;

int32_t __tac_var96 = (__tac_var95 + i2) ;

outArr[i1][i2] = outArrFlat[__tac_var96] ;

}
}
}

void AllOneDividedBySizeArray2(int32_t s1, int32_t s2, vector < vector < FPArray > >& inArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > arr1Flat = make_vector_float(ALICE, sz) ;

AllOneDividedBySizeArray(sz, arr1Flat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var97 = (i1 * s2) ;

int32_t __tac_var98 = (__tac_var97 + i2) ;

inArr[i1][i2] = arr1Flat[__tac_var98] ;

}
}
}

void SubtractOne2(int32_t s1, int32_t s2, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& outArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > inFlat = make_vector_float(ALICE, sz) ;

vector < FPArray > outFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var99 = (i1 * s2) ;

int32_t __tac_var100 = (__tac_var99 + i2) ;

inFlat[__tac_var100] = inArr[i1][i2] ;

}
}
SubtractOne(sz, inFlat, outFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var101 = (i1 * s2) ;

int32_t __tac_var102 = (__tac_var101 + i2) ;

outArr[i1][i2] = outFlat[__tac_var102] ;

}
}
}

void SigmoidDer2(int32_t s1, int32_t s2, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& outArr){
vector < vector < FPArray > > temp1 = make_vector_float(ALICE, s1, s2) ;

vector < vector < FPArray > > SigmoidVal = make_vector_float(ALICE, s1, s2) ;

Sigmoid2(s1, s2, inArr, SigmoidVal);
SubtractOne2(s1, s2, SigmoidVal, temp1);
ElemWiseMul2(s1, s2, SigmoidVal, temp1, outArr);
}

const int32_t BATCH = 128;

const int32_t HIDDENSZ = 128;

void CriticForward(vector < vector < FPArray > >& layer1W, vector < FPArray >& layer1b, vector < vector < FPArray > >& layer2W, vector < FPArray >& layer2b, vector < vector < FPArray > >& layer3W, vector < FPArray >& layer3b, vector < vector < FPArray > >& layer1In, vector < vector < BoolArray > >& layer1ReluHot, vector < vector < FPArray > >& layer1Out, vector < vector < FPArray > >& layer2In, vector < vector < BoolArray > >& layer2ReluHot, vector < vector < FPArray > >& layer2Out, vector < vector < FPArray > >& layer3In, vector < vector < FPArray > >& fwdOut){
vector < vector < FPArray > > layer1WReshaped = make_vector_float(ALICE, 14, 128) ;

vector < vector < FPArray > > layer1Temp = make_vector_float(ALICE, 128, 128) ;

Transpose(14, 128, layer1W, layer1WReshaped);
MatMul(128, 14, 128, layer1In, layer1WReshaped, layer1Temp);
GemmAdd(128, 128, layer1Temp, layer1b, layer1Out);
Relu2(128, 128, layer1Out, layer2In, layer1ReluHot);
vector < vector < FPArray > > layer2WReshaped = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer2Temp = make_vector_float(ALICE, 128, 128) ;

Transpose(128, 128, layer2W, layer2WReshaped);
MatMul(128, 128, 128, layer2In, layer2WReshaped, layer2Temp);
GemmAdd(128, 128, layer2Temp, layer2b, layer2Out);
Relu2(128, 128, layer2Out, layer3In, layer2ReluHot);
vector < vector < FPArray > > layer3WReshaped = make_vector_float(ALICE, 128, 1) ;

vector < vector < FPArray > > layer3Temp = make_vector_float(ALICE, 128, 1) ;

Transpose(128, 1, layer3W, layer3WReshaped);
MatMul(128, 128, 1, layer3In, layer3WReshaped, layer3Temp);
GemmAdd(128, 1, layer3Temp, layer3b, layer3Temp);
Reassign2(128, 1, layer3Temp, fwdOut);
}

void CriticBackward(vector < vector < FPArray > >& layer1WSpecialStrip, vector < vector < FPArray > >& layer2W, vector < vector < FPArray > >& layer3W, vector < vector < BoolArray > >& layer1ReluHot, vector < vector < BoolArray > >& layer2ReluHot, vector < vector < FPArray > >& OutDerWrtInp){
vector < vector < FPArray > > layer3Der = make_vector_float(ALICE, 128, 1) ;

vector < vector < FPArray > > layer2ActDer = make_vector_float(ALICE, 128, 128) ;

AllOneDividedBySizeArray2(128, 1, layer3Der);
MatMul(128, 1, 128, layer3Der, layer3W, layer2ActDer);
vector < vector < FPArray > > layer2Der = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer1ActDer = make_vector_float(ALICE, 128, 128) ;

IfElse2(128, 128, layer2ActDer, layer2ReluHot, layer2Der, 1);
MatMul(128, 128, 128, layer2Der, layer2W, layer1ActDer);
vector < vector < FPArray > > layer1Der = make_vector_float(ALICE, 128, 128) ;

IfElse2(128, 128, layer1ActDer, layer1ReluHot, layer1Der, 1);
MatMul(128, 128, 2, layer1Der, layer1WSpecialStrip, OutDerWrtInp);
}

void ActorForward(vector < vector < FPArray > >& layer1W, vector < FPArray >& layer1b, vector < vector < FPArray > >& layer2W, vector < FPArray >& layer2b, vector < vector < FPArray > >& layer3W, vector < FPArray >& layer3b, vector < vector < FPArray > >& layer1In, vector < vector < BoolArray > >& layer1ReluHot, vector < vector < FPArray > >& layer1Out, vector < vector < FPArray > >& layer2In, vector < vector < BoolArray > >& layer2ReluHot, vector < vector < FPArray > >& layer2Out, vector < vector < FPArray > >& layer3In, vector < vector < FPArray > >& fwdOut, vector < vector < FPArray > >& actionLimHigh, vector < vector < FPArray > >& layer3Out){
vector < vector < FPArray > > layer1WReshaped = make_vector_float(ALICE, 10, 128) ;

vector < vector < FPArray > > layer1Temp = make_vector_float(ALICE, 128, 128) ;

Transpose(10, 128, layer1W, layer1WReshaped);
MatMul(128, 10, 128, layer1In, layer1WReshaped, layer1Temp);
GemmAdd(128, 128, layer1Temp, layer1b, layer1Out);
Relu2(128, 128, layer1Out, layer2In, layer1ReluHot);
vector < vector < FPArray > > layer2WReshaped = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer2Temp = make_vector_float(ALICE, 128, 128) ;

Transpose(128, 128, layer2W, layer2WReshaped);
MatMul(128, 128, 128, layer2In, layer2WReshaped, layer2Temp);
GemmAdd(128, 128, layer2Temp, layer2b, layer2Out);
Relu2(128, 128, layer2Out, layer3In, layer2ReluHot);
vector < vector < FPArray > > layer3WReshaped = make_vector_float(ALICE, 128, 2) ;

vector < vector < FPArray > > layer3Temp = make_vector_float(ALICE, 128, 2) ;

Transpose(128, 2, layer3W, layer3WReshaped);
MatMul(128, 128, 2, layer3In, layer3WReshaped, layer3Temp);
GemmAdd(128, 2, layer3Temp, layer3b, layer3Temp);
Reassign2(128, 2, layer3Temp, layer3Out);
vector < vector < FPArray > > SigmoidVal = make_vector_float(ALICE, 128, 2) ;

Sigmoid2(128, 2, layer3Temp, SigmoidVal);
ElemWiseMul2(128, 2, SigmoidVal, actionLimHigh, fwdOut);
}

void ActorBackward(vector < vector < FPArray > >& layer1WT, vector < FPArray >& layer1bT, vector < vector < FPArray > >& layer2WT, vector < FPArray >& layer2bT, vector < vector < FPArray > >& layer3WT, vector < FPArray >& layer3bT, vector < vector < FPArray > >& AllZero, vector < vector < FPArray > >& OutDerWrtInpCritic, vector < vector < FPArray > >& actionLimHigh, vector < vector < FPArray > >& layer3Out, vector < vector < FPArray > >& layer1W, vector < FPArray >& layer1b, vector < vector < FPArray > >& layer2W, vector < FPArray >& layer2b, vector < vector < FPArray > >& layer3W, vector < FPArray >& layer3b, vector < vector < FPArray > >& layer1WM, vector < FPArray >& layer1bM, vector < vector < FPArray > >& layer2WM, vector < FPArray >& layer2bM, vector < vector < FPArray > >& layer3WM, vector < FPArray >& layer3bM, vector < vector < FPArray > >& layer1WV, vector < FPArray >& layer1bV, vector < vector < FPArray > >& layer2WV, vector < FPArray >& layer2bV, vector < vector < FPArray > >& layer3WV, vector < FPArray >& layer3bV, vector < vector < FPArray > >& layer1In, vector < vector < BoolArray > >& layer1ReluHot, vector < vector < FPArray > >& layer1Out, vector < vector < FPArray > >& layer2In, vector < vector < BoolArray > >& layer2ReluHot, vector < vector < FPArray > >& layer2Out, vector < vector < FPArray > >& layer3In){
vector < vector < FPArray > > layer3Der = make_vector_float(ALICE, 128, 2) ;

vector < vector < FPArray > > layer3Der1Temp = make_vector_float(ALICE, 128, 2) ;

vector < vector < FPArray > > layer3Der1 = make_vector_float(ALICE, 128, 2) ;

vector < vector < FPArray > > layer3Der2Temp = make_vector_float(ALICE, 128, 2) ;

vector < vector < FPArray > > layer3Der2 = make_vector_float(ALICE, 128, 2) ;

vector < vector < FPArray > > layer3DerTemp = make_vector_float(ALICE, 128, 2) ;

vector < vector < FPArray > > layer3SigmoidDer = make_vector_float(ALICE, 128, 2) ;

vector < vector < FPArray > > layer3InReshaped = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer3WDerReshaped = make_vector_float(ALICE, 128, 2) ;

vector < FPArray > layer3bDer = make_vector_float(ALICE, 2) ;

vector < vector < FPArray > > layer2ActDer = make_vector_float(ALICE, 128, 128) ;

ElemWiseMul2(128, 2, OutDerWrtInpCritic, actionLimHigh, layer3DerTemp);
SigmoidDer2(128, 2, layer3Out, layer3SigmoidDer);
ElemWiseMul2(128, 2, layer3DerTemp, layer3SigmoidDer, layer3Der1Temp);
scalarMultiplication2(128, 2, -1., layer3Der1Temp, layer3Der1);
getOutDer(128, 2, layer3Out, AllZero, layer3Der2Temp);
scalarMultiplication2(128, 2, 0.001, layer3Der2Temp, layer3Der2);
ElemWiseAdd2(128, 2, layer3Der1, layer3Der2, layer3Der);
Transpose(128, 128, layer3In, layer3InReshaped);
MatMul(128, 128, 2, layer3InReshaped, layer3Der, layer3WDerReshaped);
getBiasDer(128, 2, layer3Der, layer3bDer);
MatMul(128, 2, 128, layer3Der, layer3W, layer2ActDer);
vector < vector < FPArray > > layer2Der = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer2InReshaped = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer2WDerReshaped = make_vector_float(ALICE, 128, 128) ;

vector < FPArray > layer2bDer = make_vector_float(ALICE, 128) ;

vector < vector < FPArray > > layer1ActDer = make_vector_float(ALICE, 128, 128) ;

IfElse2(128, 128, layer2ActDer, layer2ReluHot, layer2Der, 1);
Transpose(128, 128, layer2In, layer2InReshaped);
MatMul(128, 128, 128, layer2InReshaped, layer2Der, layer2WDerReshaped);
getBiasDer(128, 128, layer2Der, layer2bDer);
MatMul(128, 128, 128, layer2Der, layer2W, layer1ActDer);
vector < vector < FPArray > > layer1Der = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer1InReshaped = make_vector_float(ALICE, 10, 128) ;

vector < vector < FPArray > > layer1WDerReshaped = make_vector_float(ALICE, 10, 128) ;

vector < FPArray > layer1bDer = make_vector_float(ALICE, 128) ;

IfElse2(128, 128, layer1ActDer, layer1ReluHot, layer1Der, 1);
Transpose(10, 128, layer1In, layer1InReshaped);
MatMul(10, 128, 128, layer1InReshaped, layer1Der, layer1WDerReshaped);
getBiasDer(128, 128, layer1Der, layer1bDer);
vector < vector < FPArray > > layer1WDer = make_vector_float(ALICE, 128, 10) ;

vector < vector < FPArray > > layer2WDer = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer3WDer = make_vector_float(ALICE, 2, 128) ;

Transpose(128, 10, layer1WDerReshaped, layer1WDer);
Transpose(128, 128, layer2WDerReshaped, layer2WDer);
Transpose(2, 128, layer3WDerReshaped, layer3WDer);
updateWeightsAdam2(128, 10, layer1WT, 0.0001, 0.9, 0.999, 1e-08, layer1W, layer1WDer, layer1WM, layer1WV);
updateWeightsAdam(128, layer1bT, 0.0001, 0.9, 0.999, 1e-08, layer1b, layer1bDer, layer1bM, layer1bV);
updateWeightsAdam2(128, 128, layer2WT, 0.0001, 0.9, 0.999, 1e-08, layer2W, layer2WDer, layer2WM, layer2WV);
updateWeightsAdam(128, layer2bT, 0.0001, 0.9, 0.999, 1e-08, layer2b, layer2bDer, layer2bM, layer2bV);
updateWeightsAdam2(2, 128, layer3WT, 0.0001, 0.9, 0.999, 1e-08, layer3W, layer3WDer, layer3WM, layer3WV);
updateWeightsAdam(2, layer3bT, 0.0001, 0.9, 0.999, 1e-08, layer3b, layer3bDer, layer3bM, layer3bV);
}


int main (int __argc, char **__argv) {
__init(__argc, __argv) ;

vector < vector < FPArray > > inpCLIENT = make_vector_float(ALICE, 128, 10) ;

if ((__party == BOB)) {
cout << ("Input inpCLIENT:") << endl ;

}
float *__tmp_in_inpCLIENT = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 10; i1++){
if ((__party == BOB)) {
cin >> __tmp_in_inpCLIENT[0];
}
inpCLIENT[i0][i1] = __fp_op->input(BOB, 1, __tmp_in_inpCLIENT, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_inpCLIENT ;

vector < vector < FPArray > > inpSERVER = make_vector_float(ALICE, 128, 10) ;

if ((__party == ALICE)) {
cout << ("Input inpSERVER:") << endl ;

}
float *__tmp_in_inpSERVER = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 10; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_inpSERVER[0];
}
inpSERVER[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_inpSERVER, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_inpSERVER ;

vector < vector < FPArray > > AllZero = make_vector_float(ALICE, 128, 2) ;

if ((__party == ALICE)) {
cout << ("Input AllZero:") << endl ;

}
float *__tmp_in_AllZero = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 2; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_AllZero[0];
}
AllZero[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_AllZero, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_AllZero ;

vector < vector < FPArray > > layer1W = make_vector_float(ALICE, 128, 10) ;

if ((__party == ALICE)) {
cout << ("Input layer1W:") << endl ;

}
float *__tmp_in_layer1W = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 10; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer1W[0];
}
layer1W[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer1W, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer1W ;

vector < FPArray > layer1b = make_vector_float(ALICE, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer1b:") << endl ;

}
float *__tmp_in_layer1b = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer1b[0];
}
layer1b[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer1b, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer1b ;

vector < vector < FPArray > > layer2W = make_vector_float(ALICE, 128, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer2W:") << endl ;

}
float *__tmp_in_layer2W = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer2W[0];
}
layer2W[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer2W, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer2W ;

vector < FPArray > layer2b = make_vector_float(ALICE, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer2b:") << endl ;

}
float *__tmp_in_layer2b = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer2b[0];
}
layer2b[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer2b, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer2b ;

vector < vector < FPArray > > layer3W = make_vector_float(ALICE, 2, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer3W:") << endl ;

}
float *__tmp_in_layer3W = new float[1] ;

for (uint32_t i0 = 0; i0 < 2; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer3W[0];
}
layer3W[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer3W, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer3W ;

vector < FPArray > layer3b = make_vector_float(ALICE, 2) ;

if ((__party == ALICE)) {
cout << ("Input layer3b:") << endl ;

}
float *__tmp_in_layer3b = new float[1] ;

for (uint32_t i0 = 0; i0 < 2; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer3b[0];
}
layer3b[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer3b, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer3b ;

vector < vector < FPArray > > layer1WM = make_vector_float(ALICE, 128, 10) ;

if ((__party == ALICE)) {
cout << ("Input layer1WM:") << endl ;

}
float *__tmp_in_layer1WM = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 10; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer1WM[0];
}
layer1WM[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer1WM, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer1WM ;

vector < FPArray > layer1bM = make_vector_float(ALICE, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer1bM:") << endl ;

}
float *__tmp_in_layer1bM = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer1bM[0];
}
layer1bM[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer1bM, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer1bM ;

vector < vector < FPArray > > layer2WM = make_vector_float(ALICE, 128, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer2WM:") << endl ;

}
float *__tmp_in_layer2WM = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer2WM[0];
}
layer2WM[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer2WM, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer2WM ;

vector < FPArray > layer2bM = make_vector_float(ALICE, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer2bM:") << endl ;

}
float *__tmp_in_layer2bM = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer2bM[0];
}
layer2bM[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer2bM, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer2bM ;

vector < vector < FPArray > > layer3WM = make_vector_float(ALICE, 2, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer3WM:") << endl ;

}
float *__tmp_in_layer3WM = new float[1] ;

for (uint32_t i0 = 0; i0 < 2; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer3WM[0];
}
layer3WM[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer3WM, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer3WM ;

vector < FPArray > layer3bM = make_vector_float(ALICE, 2) ;

if ((__party == ALICE)) {
cout << ("Input layer3bM:") << endl ;

}
float *__tmp_in_layer3bM = new float[1] ;

for (uint32_t i0 = 0; i0 < 2; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer3bM[0];
}
layer3bM[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer3bM, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer3bM ;

vector < vector < FPArray > > layer1WV = make_vector_float(ALICE, 128, 10) ;

if ((__party == ALICE)) {
cout << ("Input layer1WV:") << endl ;

}
float *__tmp_in_layer1WV = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 10; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer1WV[0];
}
layer1WV[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer1WV, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer1WV ;

vector < FPArray > layer1bV = make_vector_float(ALICE, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer1bV:") << endl ;

}
float *__tmp_in_layer1bV = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer1bV[0];
}
layer1bV[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer1bV, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer1bV ;

vector < vector < FPArray > > layer2WV = make_vector_float(ALICE, 128, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer2WV:") << endl ;

}
float *__tmp_in_layer2WV = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer2WV[0];
}
layer2WV[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer2WV, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer2WV ;

vector < FPArray > layer2bV = make_vector_float(ALICE, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer2bV:") << endl ;

}
float *__tmp_in_layer2bV = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer2bV[0];
}
layer2bV[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer2bV, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer2bV ;

vector < vector < FPArray > > layer3WV = make_vector_float(ALICE, 2, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer3WV:") << endl ;

}
float *__tmp_in_layer3WV = new float[1] ;

for (uint32_t i0 = 0; i0 < 2; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer3WV[0];
}
layer3WV[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer3WV, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer3WV ;

vector < FPArray > layer3bV = make_vector_float(ALICE, 2) ;

if ((__party == ALICE)) {
cout << ("Input layer3bV:") << endl ;

}
float *__tmp_in_layer3bV = new float[1] ;

for (uint32_t i0 = 0; i0 < 2; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer3bV[0];
}
layer3bV[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer3bV, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer3bV ;

vector < vector < FPArray > > layer1WT = make_vector_float(ALICE, 128, 10) ;

if ((__party == ALICE)) {
cout << ("Input layer1WT:") << endl ;

}
float *__tmp_in_layer1WT = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 10; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer1WT[0];
}
layer1WT[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer1WT, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer1WT ;

vector < FPArray > layer1bT = make_vector_float(ALICE, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer1bT:") << endl ;

}
float *__tmp_in_layer1bT = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer1bT[0];
}
layer1bT[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer1bT, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer1bT ;

vector < vector < FPArray > > layer2WT = make_vector_float(ALICE, 128, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer2WT:") << endl ;

}
float *__tmp_in_layer2WT = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer2WT[0];
}
layer2WT[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer2WT, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer2WT ;

vector < FPArray > layer2bT = make_vector_float(ALICE, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer2bT:") << endl ;

}
float *__tmp_in_layer2bT = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer2bT[0];
}
layer2bT[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer2bT, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer2bT ;

vector < vector < FPArray > > layer3WT = make_vector_float(ALICE, 2, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer3WT:") << endl ;

}
float *__tmp_in_layer3WT = new float[1] ;

for (uint32_t i0 = 0; i0 < 2; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer3WT[0];
}
layer3WT[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer3WT, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer3WT ;

vector < FPArray > layer3bT = make_vector_float(ALICE, 2) ;

if ((__party == ALICE)) {
cout << ("Input layer3bT:") << endl ;

}
float *__tmp_in_layer3bT = new float[1] ;

for (uint32_t i0 = 0; i0 < 2; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer3bT[0];
}
layer3bT[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer3bT, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer3bT ;

vector < vector < FPArray > > actionLimHigh = make_vector_float(ALICE, 128, 2) ;

if ((__party == ALICE)) {
cout << ("Input actionLimHigh:") << endl ;

}
float *__tmp_in_actionLimHigh = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 2; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_actionLimHigh[0];
}
actionLimHigh[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_actionLimHigh, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_actionLimHigh ;

vector < vector < FPArray > > layer1WSpecialStripCritic = make_vector_float(ALICE, 128, 2) ;

if ((__party == ALICE)) {
cout << ("Input layer1WSpecialStripCritic:") << endl ;

}
float *__tmp_in_layer1WSpecialStripCritic = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 2; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer1WSpecialStripCritic[0];
}
layer1WSpecialStripCritic[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer1WSpecialStripCritic, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer1WSpecialStripCritic ;

vector < vector < FPArray > > inpCLIENTCritic = make_vector_float(ALICE, 128, 14) ;

if ((__party == BOB)) {
cout << ("Input inpCLIENTCritic:") << endl ;

}
float *__tmp_in_inpCLIENTCritic = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 14; i1++){
if ((__party == BOB)) {
cin >> __tmp_in_inpCLIENTCritic[0];
}
inpCLIENTCritic[i0][i1] = __fp_op->input(BOB, 1, __tmp_in_inpCLIENTCritic, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_inpCLIENTCritic ;

vector < vector < FPArray > > inpSERVERCritic = make_vector_float(ALICE, 128, 14) ;

if ((__party == ALICE)) {
cout << ("Input inpSERVERCritic:") << endl ;

}
float *__tmp_in_inpSERVERCritic = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 14; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_inpSERVERCritic[0];
}
inpSERVERCritic[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_inpSERVERCritic, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_inpSERVERCritic ;

vector < vector < FPArray > > Criticlayer1W = make_vector_float(ALICE, 128, 14) ;

if ((__party == ALICE)) {
cout << ("Input Criticlayer1W:") << endl ;

}
float *__tmp_in_Criticlayer1W = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 14; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_Criticlayer1W[0];
}
Criticlayer1W[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_Criticlayer1W, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_Criticlayer1W ;

vector < FPArray > Criticlayer1b = make_vector_float(ALICE, 128) ;

if ((__party == ALICE)) {
cout << ("Input Criticlayer1b:") << endl ;

}
float *__tmp_in_Criticlayer1b = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_Criticlayer1b[0];
}
Criticlayer1b[i0] = __fp_op->input(ALICE, 1, __tmp_in_Criticlayer1b, __m_bits, __e_bits) ;

}
delete[] __tmp_in_Criticlayer1b ;

vector < vector < FPArray > > Criticlayer2W = make_vector_float(ALICE, 128, 128) ;

if ((__party == ALICE)) {
cout << ("Input Criticlayer2W:") << endl ;

}
float *__tmp_in_Criticlayer2W = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_Criticlayer2W[0];
}
Criticlayer2W[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_Criticlayer2W, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_Criticlayer2W ;

vector < FPArray > Criticlayer2b = make_vector_float(ALICE, 128) ;

if ((__party == ALICE)) {
cout << ("Input Criticlayer2b:") << endl ;

}
float *__tmp_in_Criticlayer2b = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_Criticlayer2b[0];
}
Criticlayer2b[i0] = __fp_op->input(ALICE, 1, __tmp_in_Criticlayer2b, __m_bits, __e_bits) ;

}
delete[] __tmp_in_Criticlayer2b ;

vector < vector < FPArray > > Criticlayer3W = make_vector_float(ALICE, 1, 128) ;

if ((__party == ALICE)) {
cout << ("Input Criticlayer3W:") << endl ;

}
float *__tmp_in_Criticlayer3W = new float[1] ;

for (uint32_t i0 = 0; i0 < 1; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_Criticlayer3W[0];
}
Criticlayer3W[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_Criticlayer3W, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_Criticlayer3W ;

vector < FPArray > Criticlayer3b = make_vector_float(ALICE, 1) ;

if ((__party == ALICE)) {
cout << ("Input Criticlayer3b:") << endl ;

}
float *__tmp_in_Criticlayer3b = new float[1] ;

for (uint32_t i0 = 0; i0 < 1; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_Criticlayer3b[0];
}
Criticlayer3b[i0] = __fp_op->input(ALICE, 1, __tmp_in_Criticlayer3b, __m_bits, __e_bits) ;

}
delete[] __tmp_in_Criticlayer3b ;

vector < vector < FPArray > > inp = make_vector_float(ALICE, 128, 10) ;

ElemWiseAdd2(128, 10, inpCLIENT, inpSERVER, inp);
vector < vector < FPArray > > inpCritic = make_vector_float(ALICE, 128, 14) ;

ElemWiseAdd2(128, 14, inpCLIENTCritic, inpSERVERCritic, inpCritic);
int32_t iters = 1 ;

for (uint32_t i = 0; i < 1; i++){
vector < vector < BoolArray > > layer1ReluHotCritic = make_vector_bool(ALICE, 128, 128) ;

vector < vector < FPArray > > layer1OutCritic = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer2InCritic = make_vector_float(ALICE, 128, 128) ;

vector < vector < BoolArray > > layer2ReluHotCritic = make_vector_bool(ALICE, 128, 128) ;

vector < vector < FPArray > > layer2OutCritic = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer3InCritic = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > fwdOutCritic = make_vector_float(ALICE, 128, 1) ;

CriticForward(Criticlayer1W, Criticlayer1b, Criticlayer2W, Criticlayer2b, Criticlayer3W, Criticlayer3b, inpCritic, layer1ReluHotCritic, layer1OutCritic, layer2InCritic, layer2ReluHotCritic, layer2OutCritic, layer3InCritic, fwdOutCritic);
vector < vector < FPArray > > OutDerWrtInpCritic = make_vector_float(ALICE, 128, 2) ;

CriticBackward(layer1WSpecialStripCritic, Criticlayer2W, Criticlayer3W, layer1ReluHotCritic, layer2ReluHotCritic, OutDerWrtInpCritic);
vector < vector < BoolArray > > layer1ReluHot = make_vector_bool(ALICE, 128, 128) ;

vector < vector < FPArray > > layer1Out = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer2In = make_vector_float(ALICE, 128, 128) ;

vector < vector < BoolArray > > layer2ReluHot = make_vector_bool(ALICE, 128, 128) ;

vector < vector < FPArray > > layer2Out = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer3In = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > fwdOut = make_vector_float(ALICE, 128, 2) ;

vector < vector < FPArray > > layer3Out = make_vector_float(ALICE, 128, 2) ;

ActorForward(layer1W, layer1b, layer2W, layer2b, layer3W, layer3b, inp, layer1ReluHot, layer1Out, layer2In, layer2ReluHot, layer2Out, layer3In, fwdOut, actionLimHigh, layer3Out);
ActorBackward(layer1WT, layer1bT, layer2WT, layer2bT, layer3WT, layer3bT, AllZero, OutDerWrtInpCritic, actionLimHigh, layer3Out, layer1W, layer1b, layer2W, layer2b, layer3W, layer3b, layer1WM, layer1bM, layer2WM, layer2bM, layer3WM, layer3bM, layer1WV, layer1bV, layer2WV, layer2bV, layer3WV, layer3bV, inp, layer1ReluHot, layer1Out, layer2In, layer2ReluHot, layer2Out, layer3In);
if ((__party == ALICE)) {
cout << "Value of layer1W : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 10; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer1W[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer1b : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer1b[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer2W : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer2W[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer2b : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer2b[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer3W : " ;

}
for (uint32_t i0 = 0; i0 < 2; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer3W[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer3b : " ;

}
for (uint32_t i0 = 0; i0 < 2; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer3b[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer1WM : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 10; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer1WM[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer1bM : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer1bM[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer2WM : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer2WM[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer2bM : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer2bM[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer3WM : " ;

}
for (uint32_t i0 = 0; i0 < 2; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer3WM[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer3bM : " ;

}
for (uint32_t i0 = 0; i0 < 2; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer3bM[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer1WV : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 10; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer1WV[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer1bV : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer1bV[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer2WV : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer2WV[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer2bV : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer2bV[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer3WV : " ;

}
for (uint32_t i0 = 0; i0 < 2; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer3WV[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer3bV : " ;

}
for (uint32_t i0 = 0; i0 < 2; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer3bV[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer1WT : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 10; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer1WT[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer1bT : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer1bT[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer2WT : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer2WT[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer2bT : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer2bT[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
if ((__party == ALICE)) {
cout << "Value of layer3WT : " ;

}
for (uint32_t i0 = 0; i0 < 2; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
__fp_pub = __fp_op->output(PUBLIC, layer3WT[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
if ((__party == ALICE)) {
cout << "Value of layer3bT : " ;

}
for (uint32_t i0 = 0; i0 < 2; i0++){
__fp_pub = __fp_op->output(PUBLIC, layer3bT[i0]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
return 0;
}

