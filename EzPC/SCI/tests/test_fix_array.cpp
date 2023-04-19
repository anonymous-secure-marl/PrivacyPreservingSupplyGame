#include "FloatingPoint/floating-point.h"
#include <random>

using namespace sci;
using namespace std;

enum class Op { Exp, Div, Sigmoid, Tanh };

Op op = Op::Tanh;
bool verbose = true;
bool normalized_dn = true;
IOPack *iopack = nullptr;
OTPack *otpack = nullptr;
FixOp *fix = nullptr;
int sz = 16384;
int ell_x = 16;
int ell_y = 16;
int s_x = 12;
int s_y = 12;
int party = 1;
string address = "127.0.0.1";
int port = 8000;
std::random_device rand_div;
std::mt19937 generator(8);
uint64_t max_ULP_err = 0;

uint64_t computeULPErr(double calc, double actual, int SCALE) {
  int64_t calc_fixed = (double(calc) * (1ULL << SCALE));
  int64_t actual_fixed = (double(actual) * (1ULL << SCALE));
  uint64_t ulp_err = (calc_fixed - actual_fixed) > 0
                         ? (calc_fixed - actual_fixed)
                         : (actual_fixed - calc_fixed);
  return ulp_err;
}

void test_op() {
  assert(party == ALICE || party == BOB);
  FixArray fp;
  uint64_t *f_1 = new uint64_t[sz];
  uint64_t *f_2 = new uint64_t[sz];
  for (int i = 0; i < sz; i++) {
    uint64_t fint = (uint64_t(generator()) << 32) | generator();
    // fint = -20.2006773650646 * (1ULL << s_x);
    // fint = 4118;
    f_1[i] = fint;
    fint = (uint64_t(generator()) << 32) | generator();
    // fint = 1.54963004589081 * (1ULL << s_x);
    f_2[i] = fint;
  }
  if (op == Op::Div) {
    if (normalized_dn) {
      for (int i = 0; i < sz; i++) {
        f_2[i] = (1ULL << s_x) + (f_2[i] & ((1ULL << s_x) - 1));
      }
    }
  }
  FixArray fp_1 = fix->input(ALICE, sz, f_1, true, ell_x, s_x);
  FixArray fp_2 = fix->input(ALICE, sz, f_2, true, ell_x, s_x);
  if (op == Op::Div) {
    fp_2.signed_ = false;
  }
  FixArray fp_1_pub = fix->output(PUBLIC, fp_1);
  FixArray fp_2_pub = fix->output(PUBLIC, fp_2);
  vector<double> dbl_f_1 = fp_1_pub.get_native_type<double>();
  vector<double> dbl_f_2 = fp_2_pub.get_native_type<double>();
  vector<double> f(sz);
  switch (op) {
  case Op::Exp:
    for (int i = 0; i < sz; i++) {
      f[i] = exp(dbl_f_1[i]);
    }
    fp = fix->exp(fp_1, ell_y, s_y);
    break;
  case Op::Div:
    for (int i = 0; i < sz; i++) {
      f[i] = dbl_f_1[i]/dbl_f_2[i];
    }
    fp = fix->div(fp_1, fp_2, ell_y, s_y, normalized_dn);
    break;
  case Op::Sigmoid:
    for (int i = 0; i < sz; i++) {
      f[i] = 1.0/(1.0 + exp(-1.0*dbl_f_1[i]));
    }
    fp = fix->sigmoid(fp_1, ell_y, s_y);
    break;
  case Op::Tanh:
    for (int i = 0; i < sz; i++) {
      f[i] = (1.0 - exp(-2.0*dbl_f_1[i]))/(1.0 + exp(-2.0*dbl_f_1[i]));
    }
    fp = fix->tanh(fp_1, ell_y, s_y);
    break;
  default:
    assert(false);
  }
  FixArray fp_pub = fix->output(PUBLIC, fp);
  vector<double> dbl_f = fp_pub.get_native_type<double>();
  for (int i = 0; i < sz; i++) {
    uint64_t ULP_err = computeULPErr(dbl_f[i], f[i], s_y);
    max_ULP_err = max(max_ULP_err, ULP_err);
    if (verbose) {
      FixArray fp_i = fp_pub.subset(i, i + 1);
      cout << i << "\t" << dbl_f_1[i] << "\t" << dbl_f_2[i] << "\t" << f[i] << "\t"
           << dbl_f[i] << "\t" << fp_i << "\t" << ULP_err << endl;
    }
  }
  delete[] f_1;
}

void test_LUT() {
  ell_x = 12;
  ell_y = 15;
  FixArray yp;
  uint64_t *x = new uint64_t[sz];
  vector<uint64_t> spec(1 << ell_x);
  for (int i = 0; i < sz; i++) {
    uint64_t tmp = (uint64_t(generator()) << 32) | generator();
    x[i] = tmp;
  }
  uint64_t x_mask = (1ULL << ell_x) - 1;
  uint64_t y_mask = (1ULL << ell_y) - 1;
  for (int i = 0; i < (1 << ell_x); i++) {
    uint64_t tmp = (uint64_t(generator()) << 32) | generator();
    spec[i] = tmp & y_mask;
  }
  FixArray xp = fix->input(ALICE, sz, x, false, ell_x, 0);
  FixArray xp_pub = fix->output(PUBLIC, xp);
  vector<double> dbl_x = xp_pub.get_native_type<double>();
  vector<double> y(sz);
  for (int i = 0; i < sz; i++) {
    y[i] = spec[x[i] & x_mask];
  }
  yp = fix->LUT(spec, xp, false, ell_y, 0);
  FixArray yp_pub = fix->output(PUBLIC, yp);
  vector<uint64_t> y_ = yp_pub.get_native_type<uint64_t>();
  for (int i = 0; i < sz; i++) {
    if (verbose) {
      FixArray yp_i = yp_pub.subset(i, i + 1);
      cout << i << "\t" << (x[i] & x_mask) << "\t" << y[i] << "\t" << y_[i] << "\t" << yp_i << endl;
    }
    assert(y_[i] == y[i]);
  }
  delete[] x;
}

int main(int argc, char **argv) {
  cout.precision(15);

  ArgMapping amap;

  int int_op = static_cast<int>(op);
  amap.arg("r", party, "Role of party: ALICE/SERVER = 1; BOB/CLIENT = 2");
  amap.arg("p", port, "Port Number");
  amap.arg("ip", address, "IP Address of server (ALICE)");
  amap.arg("o", int_op, "FP Operation");
  amap.parse(argc, argv);
  op = static_cast<Op>(int_op);

  iopack = new IOPack(party, port, address);
  otpack = new OTPack(iopack, party);

  fix = new FixOp(party, iopack, otpack);

  auto start = clock_start();
  uint64_t comm_start = iopack->get_comm();
  uint64_t initial_rounds = iopack->get_rounds();
  // test_op();
  test_LUT();
  uint64_t comm_end = iopack->get_comm();
  long long t = time_from(start);
  cout << "Comm. per operations: " << 8 * (comm_end - comm_start) / sz
       << " bits" << endl;
  cout << "Number of Fix ops/s:\t" << (double(sz) / t) * 1e6 << std::endl;
  cout << "Total Time:\t" << t / (1000.0) << " ms" << endl;
  cout << "Num_rounds: " << (iopack->get_rounds() - initial_rounds) << endl;
  cout << "Max ULP error: " << max_ULP_err << endl;

  delete iopack;
  delete otpack;
  delete fix;
}
