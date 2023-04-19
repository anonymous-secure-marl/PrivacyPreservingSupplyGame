
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
int32_t __tac_var69 = (i1 * s2) ;

int32_t __tac_var70 = (__tac_var69 + i2) ;

arr1Flat[__tac_var70] = arr1[i1][i2] ;

int32_t __tac_var71 = __tac_var69 ;

int32_t __tac_var72 = __tac_var70 ;

arr2Flat[__tac_var72] = arr2[i1][i2] ;

}
}
ElemWiseAdd(sz, arr1Flat, arr2Flat, outArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var73 = (i1 * s2) ;

int32_t __tac_var74 = (__tac_var73 + i2) ;

outArr[i1][i2] = outArrFlat[__tac_var74] ;

}
}
}

void scalarMultiplication2(int32_t s1, int32_t s2, float scalar, vector < vector < FPArray > >& inArr, vector < vector < FPArray > >& outArr){
int32_t sz = (s1 * s2) ;

vector < FPArray > arr1Flat = make_vector_float(ALICE, sz) ;

vector < FPArray > outArrFlat = make_vector_float(ALICE, sz) ;

for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var75 = (i1 * s2) ;

int32_t __tac_var76 = (__tac_var75 + i2) ;

arr1Flat[__tac_var76] = inArr[i1][i2] ;

}
}
scalarMultiplication(sz, scalar, arr1Flat, outArrFlat);
for (uint32_t i1 = 0; i1 < s1; i1++){
for (uint32_t i2 = 0; i2 < s2; i2++){
int32_t __tac_var77 = (i1 * s2) ;

int32_t __tac_var78 = (__tac_var77 + i2) ;

outArr[i1][i2] = outArrFlat[__tac_var78] ;

}
}
}

const int32_t BATCH = 128;

const int32_t HIDDENSZ = 128;

void forward(vector < vector < FPArray > >& layer1W, vector < FPArray >& layer1b, vector < vector < FPArray > >& layer2W, vector < FPArray >& layer2b, vector < vector < FPArray > >& layer3W, vector < FPArray >& layer3b, vector < vector < FPArray > >& layer1In, vector < vector < BoolArray > >& layer1ReluHot, vector < vector < FPArray > >& layer1Out, vector < vector < FPArray > >& layer2In, vector < vector < BoolArray > >& layer2ReluHot, vector < vector < FPArray > >& layer2Out, vector < vector < FPArray > >& layer3In, vector < vector < FPArray > >& fwdOut){
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


int main (int __argc, char **__argv) {
__init(__argc, __argv) ;

vector < vector < FPArray > > inpCLIENT = make_vector_float(ALICE, 128, 14) ;

if ((__party == BOB)) {
cout << ("Input inpCLIENT:") << endl ;

}
float *__tmp_in_inpCLIENT = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 14; i1++){
if ((__party == BOB)) {
cin >> __tmp_in_inpCLIENT[0];
}
inpCLIENT[i0][i1] = __fp_op->input(BOB, 1, __tmp_in_inpCLIENT, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_inpCLIENT ;

vector < vector < FPArray > > inpSERVER = make_vector_float(ALICE, 128, 14) ;

if ((__party == ALICE)) {
cout << ("Input inpSERVER:") << endl ;

}
float *__tmp_in_inpSERVER = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 14; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_inpSERVER[0];
}
inpSERVER[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_inpSERVER, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_inpSERVER ;

vector < vector < FPArray > > layer1W = make_vector_float(ALICE, 128, 14) ;

if ((__party == ALICE)) {
cout << ("Input layer1W:") << endl ;

}
float *__tmp_in_layer1W = new float[1] ;

for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 14; i1++){
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

vector < vector < FPArray > > layer3W = make_vector_float(ALICE, 1, 128) ;

if ((__party == ALICE)) {
cout << ("Input layer3W:") << endl ;

}
float *__tmp_in_layer3W = new float[1] ;

for (uint32_t i0 = 0; i0 < 1; i0++){
for (uint32_t i1 = 0; i1 < 128; i1++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer3W[0];
}
layer3W[i0][i1] = __fp_op->input(ALICE, 1, __tmp_in_layer3W, __m_bits, __e_bits) ;

}
}
delete[] __tmp_in_layer3W ;

vector < FPArray > layer3b = make_vector_float(ALICE, 1) ;

if ((__party == ALICE)) {
cout << ("Input layer3b:") << endl ;

}
float *__tmp_in_layer3b = new float[1] ;

for (uint32_t i0 = 0; i0 < 1; i0++){
if ((__party == ALICE)) {
cin >> __tmp_in_layer3b[0];
}
layer3b[i0] = __fp_op->input(ALICE, 1, __tmp_in_layer3b, __m_bits, __e_bits) ;

}
delete[] __tmp_in_layer3b ;

vector < vector < FPArray > > inp = make_vector_float(ALICE, 128, 14) ;

ElemWiseAdd2(128, 14, inpCLIENT, inpSERVER, inp);
int32_t iters = 1 ;

for (uint32_t i = 0; i < 1; i++){
vector < vector < BoolArray > > layer1ReluHot = make_vector_bool(ALICE, 128, 128) ;

vector < vector < FPArray > > layer1Out = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer2In = make_vector_float(ALICE, 128, 128) ;

vector < vector < BoolArray > > layer2ReluHot = make_vector_bool(ALICE, 128, 128) ;

vector < vector < FPArray > > layer2Out = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > layer3In = make_vector_float(ALICE, 128, 128) ;

vector < vector < FPArray > > fwdOut = make_vector_float(ALICE, 128, 1) ;

vector < FPArray > loss = make_vector_float(ALICE, 1) ;

forward(layer1W, layer1b, layer2W, layer2b, layer3W, layer3b, inp, layer1ReluHot, layer1Out, layer2In, layer2ReluHot, layer2Out, layer3In, fwdOut);
if ((__party == ALICE)) {
cout << "Value of fwdOut : " ;

}
for (uint32_t i0 = 0; i0 < 128; i0++){
for (uint32_t i1 = 0; i1 < 1; i1++){
__fp_pub = __fp_op->output(PUBLIC, fwdOut[i0][i1]) ;

if ((__party == ALICE)) {
cout << (__fp_pub.get_native_type<float>()[0]) << endl ;

}
}
}
}
return 0;
}

