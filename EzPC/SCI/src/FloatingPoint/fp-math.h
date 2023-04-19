#ifndef FLOATING_POINT_MATH_H__
#define FLOATING_POINT_MATH_H__

#include "FloatingPoint/floating-point.h"
#include "Math/math-functions.h"

class FPMath {
public:
  int party;
  sci::IOPack *iopack;
  sci::OTPack *otpack;
  BoolOp *bool_op;
  FixOp *fix;
  FPOp *fp_op;

  FPMath(int party, sci::IOPack *iopack, sci::OTPack *otpack) {
    this->party = party;
    this->iopack = iopack;
    this->otpack = otpack;
    this->bool_op = new BoolOp(party, iopack, otpack);
    this->fix = new FixOp(party, iopack, otpack);
    this->fp_op = new FPOp(party, iopack, otpack);
  }

  ~FPMath() {
    delete bool_op;
    delete fix;
    delete fp_op;
  }

  // Floating-Point Math Functions: returns OP(x[i]), OP = {sinpi, cospi, tanpi, exp2, log2, exp, ln, erf}
  // x must be secret-shared
  FPArray sinpi(const FPArray &x);
  FPArray cospi(const FPArray &x);
  FPArray tanpi(const FPArray &x);
  FPArray exp2(const FPArray &x);
  FPArray log2(const FPArray &x);
  FPArray exp(const FPArray &x);
  FPArray ln(const FPArray &x);
  FPArray erf(const FPArray &x);

  FPArray sigmoid_bf16(const FPArray &x);
  FPArray sigmoid_fp32(const FPArray &x);
  FPArray tanh_bf16(const FPArray &x);
  FPArray tanh_fp32(const FPArray &x);

  FPArray exp_neg(const FPArray &x, uint8_t max_ULP_error = 3);
  vector<FPArray> softmax(const vector<FPArray>& x, bool impure_bfloat16 = true);
  vector<FPArray> softmax_old(const vector<FPArray>& x);
};

#endif // FLOATING_POINT_MATH_H__
