#include "mpfr.h"
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

mpfr_t mval;

enum class Op { SINPI, COSPI, TANPI, EXP2, LOG2, EXP, LN, ERF, NEG_EXP64, NEG_EXP3 };

Op op = Op::NEG_EXP3;

string op_name;
string poly[] = {"NOP", "NOP", "NOP", "NOP", "NOP"};
// bool closed_right_interval[] = {false, false, false, false};
int32_t LUT_size_N = -1;
int32_t exp_bits[] = {0, 0, 0, 0, 0};
int32_t man_bits[] = {0, 0, 0, 0, 0};
uint32_t e_offset[] = {0, 0, 0, 0, 0};

#define FP_INTMD_M_BITS 27
#define FP_INTMD_E_BITS 8
#define FRAC_RANGE 9
/*
#define REDUCED_RANGE_LB -14
#define REDUCED_RANGE_UB -9
#define FRAC_RANGE -1*REDUCED_RANGE_UB
#define FP_LUT_E_BITS 3
#define FP_LUT_M_BITS 2
#define TAN_LUT 7
#define SIN_LUT 8
#define LUT_SIZE SIN_LUT
*/

string bits_float(float x) {
  uint32_t x_int = *((uint32_t *)&x);
  string tmp_x = bitset<32>(x_int).to_string();
  string ret =
      tmp_x.substr(0, 1) + " " + tmp_x.substr(1, 8) + " " + tmp_x.substr(9, 23);
  return ret;
}

string bits_custom_float(uint64_t x) {
  string tmp_x = bitset<64>(x).to_string();
  int float_len = FP_INTMD_M_BITS + FP_INTMD_E_BITS + 5;
  tmp_x = tmp_x.substr(64 - float_len, float_len);
  string ret = tmp_x.substr(0, 1) + " " + tmp_x.substr(1, 1) + " " +
               tmp_x.substr(2, FP_INTMD_E_BITS + 2) + " " +
               tmp_x.substr(FP_INTMD_E_BITS + 4, FP_INTMD_M_BITS + 1);
  return ret;
}

uint64_t RoundTiesToEven(uint64_t x, int32_t shift_amt) {
  assert(shift_amt > 0);
  bool a = (x >> (shift_amt - 1)) & 1;
  bool b = (x & ((1ULL << (shift_amt - 1)) - 1)) != 0;
  bool c = (x >> shift_amt) & 1;
  uint64_t y = x >> shift_amt;
  if (a && (b || c)) {
    y += 1;
  }
  return y;
}

class Poly {
public:
  int termSize;
  double xlb;
  double xub;
  int lowIndex;
  int highIndex;
  vector<int> power;
  vector<double> coeffs;

  Poly(int _termSize, std::vector<int> &_power) {
    termSize = _termSize;
    power = _power;
  }
  void PrintPolyInfo() {
    printf("For inputs in the range of: \n");
    printf("%.20lf ~ %.20lf\n", xlb, xub);

    if (termSize == 0) {
      printf("How can a polynomial have no terms?\n");
      exit(0);
    }

    printf("\ty = %.70e x^(%d)\n", coeffs[0], power[0]);
    for (int j = 1; j < termSize; j++) {
      printf("\t  + %.70e x^(%d)\n", coeffs[j], power[j]);
    }
    printf("\n");
  }
};

void deserialize_piecewise_poly(string file_name, vector<Poly> &piecewise) {
  ifstream fin;
  fin.open(file_name.c_str());

  if (fin) {
    int num_pieces;
    fin >> num_pieces;
    for (int p = 0; p < num_pieces; p++) {
      int termSize;
      uint64_t xlb, xub;
      fin >> termSize;
      fin >> xlb;
      fin >> xub;
      vector<uint64_t> coeffs(termSize);
      vector<int> power(termSize);
      for (int i = 0; i < termSize; i++) {
        fin >> coeffs[i];
        fin >> power[i];
      }
      Poly piece(termSize, power);
      piece.xlb = *((double *)&xlb);
      piece.xub = *((double *)&xub);
      for (int i = 0; i < termSize; i++) {
        piece.coeffs.push_back(*((double *)&coeffs[i]));
      }
      piecewise.push_back(piece);
    }
  } else {
    cout << "ERROR File does not exist" << endl;
    abort();
  }
}

// x <= 2^-14, tanpi(x) = pi*x in float representation
double precise_tan(double x) {
  mpfr_const_pi(mval, MPFR_RNDN);
  mpfr_mul_d(mval, mval, x, MPFR_RNDN);
  mpfr_tan(mval, mval, MPFR_RNDN);
  return mpfr_get_d(mval, MPFR_RNDN);
}

// x <= 2^-14, sinpi(x) = pi*x in float representation
double precise_sin(double x) {
  mpfr_const_pi(mval, MPFR_RNDN);
  mpfr_mul_d(mval, mval, x, MPFR_RNDN);
  mpfr_sin(mval, mval, MPFR_RNDN);
  return mpfr_get_d(mval, MPFR_RNDN);
}
double precise_cos(double x) {
  mpfr_const_pi(mval, MPFR_RNDN);
  mpfr_mul_d(mval, mval, x, MPFR_RNDN);
  mpfr_cos(mval, mval, MPFR_RNDN);
  return mpfr_get_d(mval, MPFR_RNDN);
}

int main(int argc, char **argv) {
  switch (op) {
  case Op::SINPI:
    op_name = "Sinpi";
    poly[0] = "Sin_27-1.poly";
    poly[1] = "Sin_27-2.poly";
    exp_bits[0] = 4;
    man_bits[0] = 0;
    exp_bits[1] = 2;
    man_bits[1] = 5;
    e_offset[0] = 127 - 14;
    e_offset[1] = 127 - 5;
    break;
  case Op::COSPI:
    op_name = "Cospi";
    poly[0] = "Cos_27-1.poly";
    poly[1] = "Cos_27-2.poly";
    exp_bits[0] = 4;
    man_bits[0] = 0;
    exp_bits[1] = 2;
    man_bits[1] = 5;
    e_offset[0] = 127 - 14;
    e_offset[1] = 127 - 5;
    break;
  case Op::TANPI:
    op_name = "Tanpi";
    poly[0] = "Tan_27.poly";
    exp_bits[0] = 3;
    man_bits[0] = 2;
    e_offset[0] = 127 - 14;
    e_offset[1] = 127 - 14;
    LUT_size_N = 7;
    break;
  case Op::EXP2:
    op_name = "Exp2";
    poly[0] = "Exp2_27.poly";
    exp_bits[0] = 3;
    man_bits[0] = 5;
    e_offset[0] = 127 - 7;
    break;
  case Op::LOG2:
    op_name = "Log2";
    poly[0] = "Log2b_27-1.poly";
    poly[1] = "Log2b_27-2.poly";
    poly[2] = "Log2a_27-1.poly";
    poly[3] = "Log2a_27-2.poly";
    exp_bits[0] = 5;
    man_bits[0] = 0;
    exp_bits[1] = 3;
    man_bits[1] = 4;
    exp_bits[2] = 5;
    man_bits[2] = 0;
    exp_bits[3] = 3;
    man_bits[3] = 4;
    e_offset[0] = 127 - 24;
    e_offset[1] = 127 - 5;
    e_offset[2] = 127 - 24;
    e_offset[3] = 127 - 5;
    break;
  case Op::LN:
    op_name = "LN";
    poly[0] = "Lnb_27-1.poly";
    poly[1] = "Lnb_27-2.poly";
    poly[2] = "Lna_27-1.poly";
    poly[3] = "Lna_27-2.poly";
    exp_bits[0] = 5;
    man_bits[0] = 0;
    exp_bits[1] = 3;
    man_bits[1] = 4;
    exp_bits[2] = 5;
    man_bits[2] = 0;
    exp_bits[3] = 3;
    man_bits[3] = 4;
    e_offset[0] = 127 - 24;
    e_offset[1] = 127 - 5;
    e_offset[2] = 127 - 24;
    e_offset[3] = 127 - 5;
    break;
  case Op::EXP:
    op_name = "EXP";
    poly[0] = "Expa_27-1.poly";
    poly[1] = "Expa_27-2.poly";
    poly[2] = "Expb_27-1.poly";
    poly[3] = "Expb_27-2.poly";
    exp_bits[0] = 5;
    man_bits[0] = 0;
    exp_bits[1] = 3;
    man_bits[1] = 5;
    exp_bits[2] = 5;
    man_bits[2] = 0;
    exp_bits[3] = 3;
    man_bits[3] = 5;
    e_offset[0] = 127 - 24;
    e_offset[1] = 127 - 6;
    e_offset[2] = 127 - 24;
    e_offset[3] = 127 - 6;
    break;
  case Op::ERF:
    op_name = "ERF";
    poly[0] = "Erf_27-1.poly";
    poly[1] = "Erf_27-2.poly";
    exp_bits[0] = 3;
    man_bits[0] = 3;
    exp_bits[1] = 1;
    man_bits[1] = 5;
    e_offset[0] = 127 - 6;
    e_offset[1] = 127;
    break;
  case Op::NEG_EXP64:
    op_name = "NEG_EXP64";
    poly[0] = "neg_Exp_64-1.poly";
    poly[1] = "neg_Exp_64-2.poly";
    poly[2] = "neg_Exp_64-3.poly";
    exp_bits[0] = 5;
    man_bits[0] = 0;
    exp_bits[1] = 2;
    man_bits[1] = 4;
    exp_bits[2] = 1;
    man_bits[2] = 6;
    e_offset[0] = 127 - 24;
    e_offset[1] = 127 - 6;
    e_offset[2] = 127 - 2;
    break;
  case Op::NEG_EXP3:
    op_name = "NEG_EXP3";
    poly[0] = "neg_Exp-3-1.poly";
    poly[1] = "neg_Exp-3-2.poly";
    poly[2] = "neg_Exp-3-3.poly";
    poly[3] = "neg_Exp-3-4.poly";
    poly[4] = "neg_Exp-3-5.poly";
    exp_bits[0] = 5;
    exp_bits[1] = 2;
    exp_bits[2] = 1;
    exp_bits[3] = 0;
    exp_bits[4] = 0;
    man_bits[0] = 3;
    man_bits[1] = 6;
    man_bits[2] = 7;
    man_bits[3] = 8;
    man_bits[4] = 9;
    e_offset[0] = 127 - 24;
    e_offset[1] = 127 - 7;
    e_offset[2] = 127 - 3;
    e_offset[3] = 127 - 2;
    e_offset[4] = 127 - 1;
    break;
  default:
    abort();
  }
  cout.precision(15);
  mpfr_init2(mval, 300);

  vector<Poly> piecewise[5];
  uint64_t e_bias = (1ULL << (FP_INTMD_E_BITS - 1)) - 1;

  uint64_t m32_mask = (1ULL << 23) - 1;
  uint64_t e32_mask = (1ULL << 8) - 1;
  for (int k = 0; k < 5; k++) {
    if (poly[k] == "NOP")
      continue;
    deserialize_piecewise_poly(poly[k], piecewise[k]);
    cout << "========== POLY " << k + 1 << " (" << op_name
         << ") ===========" << endl;
    int num_pieces = piecewise[k].size();
    /*
    if (closed_right_interval[k]) {
        num_pieces += 1;
    }
    */
    int termSize = piecewise[k][0].termSize;
    vector<vector<uint64_t>> c_vec(termSize, vector<uint64_t>(num_pieces, 0));
    vector<vector<double>> d_vec(termSize, vector<double>(num_pieces, 0));
    vector<uint64_t> knots_bits(piecewise[k].size() - 1, 0);
    for (int i = 0; i < piecewise[k].size(); i++) {
      float xlb_flt = piecewise[k][i].xlb, xub_flt = piecewise[k][i].xub;
      cout << "Piece " << i + 1 << ": " << xlb_flt << "\t("
           << bits_float(xlb_flt) << ") --->" << xub_flt << "\t("
           << bits_float(xub_flt) << ")" << endl;
      if (i < piecewise[k].size() - 1) {
        float xlb_next = piecewise[k][i + 1].xlb;
        uint32_t xlb_next_int = *((uint32_t *)&xlb_next);
        knots_bits[i] = (xlb_next_int & m32_mask) >> (23 - man_bits[k]) |
                        (((((xlb_next_int >> 23) & e32_mask) - e_offset[k]) &
                          ((1ULL << exp_bits[k]) - 1))
                         << man_bits[k]);
      }
      for (int j = 0; j < termSize; j++) {
        double c = piecewise[k][i].coeffs[j];
        uint64_t c_int = *((uint64_t *)&c);
        // uint64_t c_m = (c_int & ((1ULL << 52) - 1)) >> (52 -
        // FP_INTMD_M_BITS); c_m |= (1ULL << FP_INTMD_M_BITS);
        uint64_t c_m = (c_int & ((1ULL << 52) - 1)) | (1ULL << 52);
        c_m = RoundTiesToEven(c_m, 52 - FP_INTMD_M_BITS);
        bool c_z;
        uint64_t c_e = ((c_int >> 52) & ((1ULL << 11) - 1));
        if (c_e != 0) {
          c_e = c_e - 1023 + e_bias;
          c_z = 0;
        } else {
          c_z = 1;
          c_m = 0;
        }
        if (c_m == (1ULL << (FP_INTMD_M_BITS + 1))) {
          c_e = c_e + 1;
          c_m >>= 1;
        }
        bool c_s = c_int >> 63;
        assert(c_e <= 254 && c_e >= 0);

        c_vec[j][i] =
            ((uint64_t(c_z) & 1) << (FP_INTMD_M_BITS + 4 + FP_INTMD_E_BITS)) |
            ((uint64_t(c_s) & 1) << (FP_INTMD_M_BITS + 3 + FP_INTMD_E_BITS)) |
            (uint64_t(c_e) << (FP_INTMD_M_BITS + 1)) | uint64_t(c_m);
        d_vec[j][i] = c;
      }
    }
    cout << "Knots: {";
    for (int i = 0; i < piecewise[k].size() - 1; i++) {
      cout << hex << "0x" << knots_bits[i] << dec << ", ";
    }
    cout << "}" << endl;
    for (int j = 0; j < termSize; j++) {
      cout << "Coeff " << piecewise[k][0].power[j] << ":" << endl;
      cout << "LUT_" << k + 1 << ": {";
      for (int i = 0; i < num_pieces; i++) {
        cout << hex << "0x" << c_vec[j][i] << dec << ", ";
      }
      cout << "}" << endl;
      // /*
      cout << "LUT_" << k + 1 << ": {";
      for (int i = 0; i < num_pieces; i++) {
        cout << d_vec[j][i] << ", ";
      }
      cout << "}" << endl;
      // */
    }
  }

  if (LUT_size_N != -1) {
    cout << "========== POLY-N (" << op_name << ") ===========" << endl;
    vector<uint64_t> N_vec(1 << LUT_size_N, 0);
    vector<double> Nd_vec(1 << LUT_size_N, 0);
    for (int i = 0; i < (1 << LUT_size_N); i++) {
      double x = i / double(1 << FRAC_RANGE);
      double N;
      switch (op) {
      case Op::SINPI:
        N = precise_sin(x);
        break;
      case Op::COSPI:
        N = precise_cos(x);
        break;
      case Op::TANPI:
        N = precise_tan(x);
        break;
      }
      uint64_t N_int = *((uint64_t *)&N);
      uint64_t N_m = (N_int & ((1ULL << 52) - 1)) | (1ULL << 52);
      N_m = RoundTiesToEven(N_m, 52 - FP_INTMD_M_BITS);
      bool N_z;
      uint64_t N_e = ((N_int >> 52) & ((1ULL << 11) - 1));
      if (N_e != 0) {
        N_e = N_e - 1023 + e_bias;
        N_z = 0;
      } else {
        N_z = 1;
        N_m = 0;
      }
      if (N_m == (1ULL << (FP_INTMD_M_BITS + 1))) {
        N_e = N_e + 1;
        N_m >>= 1;
      }
      bool N_s = N_int >> 63;
      assert(N_e <= 254 && N_e >= 0);

      N_vec[i] =
          ((uint64_t(N_z) & 1) << (FP_INTMD_M_BITS + 4 + FP_INTMD_E_BITS)) |
          ((uint64_t(N_s) & 1) << (FP_INTMD_M_BITS + 3 + FP_INTMD_E_BITS)) |
          (uint64_t(N_e) << (FP_INTMD_M_BITS + 1)) | uint64_t(N_m);
      Nd_vec[i] = N;
    }
    cout << "LUT_N: {";
    for (int i = 0; i < (1 << LUT_size_N); i++) {
      cout << hex << "0x" << N_vec[i] << dec << ", ";
    }
    cout << "}" << endl;
    /*
    cout << "LUT_N: {";
    for (int i = 0; i < (1 << LUT_size_N); i++) {
        cout << Nd_vec[i] << ", ";
    }
    cout << "}" << endl;
    // */
  }

  /*
  cout << "========== Log2-Int2Float ===========" << endl;
  vector<uint64_t> x_vec(1 << 8, 0);
  vector<float> xd_vec(1 << 8, 0);
  for (int i = 0; i < (1 << 8); i++) {
      float x = i - 127;
      uint32_t x_int = *((uint32_t*) &x);
      uint32_t x_m = (x_int & ((1U << 23) - 1)) | (1U << 23);
      bool x_z = 0;
      uint32_t x_e = ((x_int >> 23) & ((1U << 8) - 1));
      if (x_e == 0) {
          x_z = 1;
          x_m = 0;
      }
      bool x_s = x_int >> 31;
      assert(x_e <= 254 && x_e >= 0);
      x_vec[i] = ((uint32_t(x_z) & 1) << 18)
                 | ((uint32_t(x_s) & 1) << 17)
                 | (uint32_t(x_e) << 7)
                 | uint32_t(x_m >> 17);
      xd_vec[i] = x;
  }
  cout << "LUT_x: {";
  for (int i = 0; i < (1 << 8); i++) {
      cout << hex << "0x" << x_vec[i] << dec << ", ";
  }
  cout << "}" << endl;
  cout << "LUT_x: {";
  for (int i = 0; i < (1 << 8); i++) {
      cout << xd_vec[i] << ", ";
  }
  cout << "}" << endl;
  */
  /*
  cout << "========== LN-Int2Float ===========" << endl;
  vector<uint64_t> x_vec(1 << 8, 0);
  vector<float> xd_vec(1 << 8, 0);
  for (int i = 0; i < (1 << 8); i++) {
      mpfr_set_flt(mval, 2.0, MPFR_RNDN);
      mpfr_log(mval, mval, MPFR_RNDN);
      mpfr_mul_d(mval, mval, double(i)-127.0, MPFR_RNDN);
      double x = mpfr_get_d(mval, MPFR_RNDN);
      uint64_t x_int = *((uint64_t*) &x);
      // uint64_t x_m = (x_int & ((1ULL << 52) - 1)) >> (52 - FP_INTMD_M_BITS);
      // x_m |= (1ULL << FP_INTMD_M_BITS);
      uint64_t x_m = (x_int & ((1ULL << 52) - 1)) | (1ULL << 52);
      x_m = RoundTiesToEven(x_m, 52 - FP_INTMD_M_BITS);
      bool x_z;
      uint64_t x_e = ((x_int >> 52) & ((1ULL << 11) - 1));
      if (x_e != 0) {
          x_e = x_e - 1023 + e_bias;
          x_z = 0;
      } else {
          x_z = 1;
          x_m = 0;
      }
      if (x_m == (1ULL << (FP_INTMD_M_BITS + 1))) {
          x_e = x_e + 1;
          x_m >>= 1;
      }
      bool x_s = x_int >> 63;
      assert(x_e <= 254 && x_e >= 0);

      x_vec[i] = ((uint64_t(x_z) & 1) << (FP_INTMD_M_BITS + 4 +
  FP_INTMD_E_BITS)) | ((uint64_t(x_s) & 1) << (FP_INTMD_M_BITS + 3 +
  FP_INTMD_E_BITS)) | (uint64_t(x_e) << (FP_INTMD_M_BITS + 1)) | uint64_t(x_m);
      xd_vec[i] = x;
  }
  cout << "LUT_x: {";
  for (int i = 0; i < (1 << 8); i++) {
      cout << hex << "0x" << x_vec[i] << dec << ", ";
  }
  cout << "}" << endl;
  cout << "LUT_x: {";
  for (int i = 0; i < (1 << 8); i++) {
      cout << xd_vec[i] << ", ";
  }
  cout << "}" << endl;
  */

  mpfr_clear(mval);
}
