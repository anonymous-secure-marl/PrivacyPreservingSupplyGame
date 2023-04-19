#include "FloatingPoint/floating-point.h"
#include <random>

using namespace sci;
using namespace std;

enum class Op { NOT, XOR, AND, OR };

Op op = Op::OR;
bool verbose = true;
IOPack *iopack = nullptr;
OTPack *otpack = nullptr;
BoolOp *bool_op = nullptr;
int sz = 35;
int party = 1;
string address = "127.0.0.1";
int port = 8000;
std::random_device rand_div;
std::mt19937 generator(0);

void test_op() {
  assert(party == ALICE || party == BOB);
  BoolArray fp;
  uint8_t *f_1 = new uint8_t[sz];
  uint8_t *f_2 = new uint8_t[sz];
  uint8_t *f = new uint8_t[sz];
  uint8_t *f_;
  for (int i = 0; i < sz; i++) {
    uint32_t fint = generator();
    f_1[i] = fint & 1;
    fint = generator();
    f_2[i] = fint & 1;
  }
  BoolArray fp_1 = bool_op->input(ALICE, sz, f_1);
  BoolArray fp_2 = bool_op->input(BOB, sz, f_2);
  switch (op) {
  case Op::NOT:
    for (int i = 0; i < sz; i++) {
      f[i] = ~f_1[i];
    }
    fp = bool_op->NOT(fp_1);
    break;
  case Op::XOR:
    for (int i = 0; i < sz; i++) {
      f[i] = f_1[i] ^ f_2[i];
    }
    fp = bool_op->XOR(fp_1, fp_2);
    break;
  case Op::AND:
    for (int i = 0; i < sz; i++) {
      f[i] = f_1[i] & f_2[i];
    }
    fp = bool_op->AND(fp_1, fp_2);
    break;
  case Op::OR:
    for (int i = 0; i < sz; i++) {
      f[i] = f_1[i] | f_2[i];
    }
    fp = bool_op->OR(fp_1, fp_2);
    break;
  default:
    assert(false);
  }
  BoolArray fp_pub = bool_op->output(PUBLIC, fp);
  f_ = fp_pub.data;
  for (int i = 0; i < sz; i++) {
    if (verbose) {
      BoolArray fp_i = fp_pub.subset(i, i + 1);
      cout << i << "\t" << bool(f_1[i]) << "\t" << bool(f_2[i]) << "\t"
           << bool(f[i]) << "\t" << bool(f_[i]) << "\t" << fp_i << endl;
    }
    assert((f[i] & 1) == f_[i]);
  }
  delete[] f;
  delete[] f_1;
  delete[] f_2;
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

  bool_op = new BoolOp(party, iopack, otpack);

  auto start = clock_start();
  uint64_t comm_start = iopack->get_comm();
  uint64_t initial_rounds = iopack->get_rounds();
  test_op();
  uint64_t comm_end = iopack->get_comm();
  long long t = time_from(start);
  cout << "Comm. per operations: " << 8 * (comm_end - comm_start) / sz
       << " bits" << endl;
  cout << "Number of Bool ops/s:\t" << (double(sz) / t) * 1e6 << std::endl;
  cout << "Total Time:\t" << t / (1000.0) << " ms" << endl;
  cout << "Num_rounds: " << (iopack->get_rounds() - initial_rounds) << endl;

  delete iopack;
  delete otpack;
  delete bool_op;
}
