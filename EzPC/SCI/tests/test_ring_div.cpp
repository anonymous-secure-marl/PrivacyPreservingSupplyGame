#include "Math/math-functions.h"
#include <fstream>
#include <iostream>
#include <thread>

using namespace sci;
using namespace std;

#define MAX_THREADS 4

int party, port = 32000;
int num_threads = 1;
string address;
bool localhost = true;

/*
int l = 32;
int s = 28;
int t = 28;
*/
int dim = 256;
int bw_nm = 32;
int bw_dn = 32;
int bw_out = 32;
int s_nm = 26; // s;
int s_dn = 26;
int s_out = 26; // t;
bool signed_nm = true;
bool compute_msnzb = false;

uint64_t mask_nm = (bw_nm == 64 ? -1 : ((1ULL << bw_nm) - 1));
uint64_t mask_dn = (bw_dn == 64 ? -1 : ((1ULL << bw_dn) - 1));
uint64_t mask_out = (bw_out == 64 ? -1 : ((1ULL << bw_out) - 1));

IOPack *iopackArr[MAX_THREADS];
OTPack *otpackArr[MAX_THREADS];

uint64_t computeULPErr(double calc, double actual, int SCALE) {
  int64_t calc_fixed = (double(calc) * (1ULL << SCALE));
  int64_t actual_fixed = (double(actual) * (1ULL << SCALE));
  uint64_t ulp_err = (calc_fixed - actual_fixed) > 0
                         ? (calc_fixed - actual_fixed)
                         : (actual_fixed - calc_fixed);
  return ulp_err;
}

void div_thread(int tid, uint64_t *nm, uint64_t *dn, uint64_t *out,
                int num_div) {
  MathFunctions *math;
  if (tid & 1) {
    math = new MathFunctions(3 - party, iopackArr[tid], otpackArr[tid]);
  } else {
    math = new MathFunctions(party, iopackArr[tid], otpackArr[tid]);
  }
  math->div(num_div, nm, dn, out, bw_nm, bw_dn, bw_out, s_nm, s_dn, s_out, signed_nm, compute_msnzb);

  delete math;
}

int main(int argc, char **argv) {
  /************* Argument Parsing  ************/
  /********************************************/
  ArgMapping amap;
  amap.arg("r", party, "Role of party: ALICE = 1; BOB = 2");
  amap.arg("p", port, "Port Number");
  amap.arg("N", dim, "Number of divisions");
  amap.arg("lo", localhost, "Localhost Run?");
  amap.arg("nt", num_threads, "Number of threads");
  /*
  amap.arg("l", l, "Bitlength of inputs");
  amap.arg("s", s, "Input scale");
  amap.arg("t", t, "Output scale");
  */

  amap.parse(argc, argv);

  if (not localhost)
    address = "40.118.124.169";
  else
    address = "127.0.0.1";

  assert(num_threads <= MAX_THREADS);

  /*
  cout << "========================================================" << endl;
  cout << "Role: " << party << " - Bitlength: " << l << " - Input Scale: " << s
      << "\nOutput Scale: " << t << " - # Divisions: "
      << dim << " - # Threads: " << num_threads << endl;
  cout << "========================================================" << endl;
  */

  /********** Setup IO and Base OTs ***********/
  /********************************************/
  for (int i = 0; i < num_threads; i++) {
    iopackArr[i] = new sci::IOPack(party, port + i, address);
    if (i & 1) {
      otpackArr[i] = new OTPack(iopackArr[i], 3 - party);
    } else {
      otpackArr[i] = new OTPack(iopackArr[i], party);
    }
  }
  std::cout << "All Base OTs Done" << std::endl;

  /************ Generate Test Data ************/
  /********************************************/
  PRG128 prg(fix_key);

  uint64_t *nm = new uint64_t[dim];
  uint64_t *dn = new uint64_t[dim];
  uint64_t *out = new uint64_t[dim];

  prg.random_data(nm, dim * sizeof(uint64_t));
  prg.random_data(dn, dim * sizeof(uint64_t));

  if (party == ALICE) {
    iopackArr[0]->io->send_data(dn, dim * sizeof(uint64_t));
  } else {
    uint64_t *dn0 = new uint64_t[dim];
    iopackArr[0]->io->recv_data(dn0, dim * sizeof(uint64_t));
    uint64_t mask_s_dn = (s_dn == 64 ? -1 : ((1ULL << s_dn) - 1));
    for (int i = 0; i < dim; i++) {
      nm[i] &= mask_nm;
      if (compute_msnzb) {
        // dn is always positive
        dn[i] = (dn[i] & (mask_dn >> 1)) - dn0[i];
      } else {
        // dn is always of the form 1.xxxx
        dn[i] = ((1ULL << s_dn) + (dn[i] & mask_s_dn)) - dn0[i];
      }
    }
    delete[] dn0;
  }
  for (int i = 0; i < dim; i++) {
    nm[i] &= mask_nm;
    dn[i] &= mask_dn;
  }

  /************** Fork Threads ****************/
  /********************************************/
  uint64_t total_comm = 0;
  uint64_t thread_comm[num_threads];
  for (int i = 0; i < num_threads; i++) {
    thread_comm[i] = iopackArr[i]->get_comm();
  }

  auto start = clock_start();
  std::thread div_threads[num_threads];
  int chunk_size = dim / num_threads;
  for (int i = 0; i < num_threads; ++i) {
    int offset = i * chunk_size;
    int lnum_div;
    if (i == (num_threads - 1)) {
      lnum_div = dim - offset;
    } else {
      lnum_div = chunk_size;
    }
    div_threads[i] = std::thread(div_thread, i, nm + offset, dn + offset,
                                 out + offset, lnum_div);
  }
  for (int i = 0; i < num_threads; ++i) {
    div_threads[i].join();
  }
  long long t = time_from(start);

  for (int i = 0; i < num_threads; i++) {
    thread_comm[i] = iopackArr[i]->get_comm() - thread_comm[i];
    total_comm += thread_comm[i];
  }

  /************** Verification ****************/
  /********************************************/
  cout.precision(15);
  if (party == ALICE) {
    iopackArr[0]->io->send_data(nm, dim * sizeof(uint64_t));
    iopackArr[0]->io->send_data(dn, dim * sizeof(uint64_t));
    iopackArr[0]->io->send_data(out, dim * sizeof(uint64_t));
  } else { // party == BOB
    uint64_t *nm0 = new uint64_t[dim];
    uint64_t *dn0 = new uint64_t[dim];
    uint64_t *out0 = new uint64_t[dim];
    iopackArr[0]->io->recv_data(nm0, dim * sizeof(uint64_t));
    iopackArr[0]->io->recv_data(dn0, dim * sizeof(uint64_t));
    iopackArr[0]->io->recv_data(out0, dim * sizeof(uint64_t));

    uint64_t total_err = 0;
    uint64_t max_ULP_err = 0;
    for (int i = 0; i < dim; i++) {
      double a, d;
      if (signed_nm) {
        a = (signed_val(nm0[i] + nm[i], bw_nm)) / double(1LL << s_nm);
      } else {
        a = (unsigned_val(nm0[i] + nm[i], bw_nm)) / double(1ULL << s_nm);
      }
      double b = (unsigned_val(dn0[i] + dn[i], bw_dn)) / double(1ULL << s_dn);
      if (signed_nm) {
        d = (signed_val(out[i] + out0[i], bw_out) / double(1LL << s_out));
      } else {
        d = (unsigned_val(out[i] + out0[i], bw_out) / double(1ULL << s_out));
      }
      double a_by_b;
      if (signed_nm) {
        a_by_b =
            signed_val((a / b) * (1LL << s_out), bw_out) / double(1LL << s_out);
      } else {
        a_by_b = unsigned_val((a / b) * (1LL << s_out), bw_out) /
                 double(1LL << s_out);
      }
      uint64_t err = computeULPErr(d, a_by_b, s_out);
      cout << "ULP Error: " << a << "," << b << ","
          << d << "," << a_by_b << "," << a/b << "," << err << endl;
      if (abs(a / b) <= (1ULL << (bw_out - s_out - int(signed_nm)))) {
        total_err += err;
        max_ULP_err = std::max(max_ULP_err, err);
      }
    }

    cerr << "Average ULP error: " << total_err / dim << endl;
    cerr << "Max ULP error: " << max_ULP_err << endl;
    cerr << "Number of tests: " << dim << endl;

    delete[] nm0;
    delete[] dn0;
    delete[] out0;
  }

  /**** Process & Write Benchmarking Data *****/
  /********************************************/
  cout << "Number of Divisions/s:\t" << (double(dim) / t) * 1e6 << std::endl;
  cout << "Division Time\t" << t / (1000.0) << " ms" << endl;
  cout << "Division Bytes Sent\t" << total_comm << " bytes" << endl;

  /******************* Cleanup ****************/
  /********************************************/
  delete[] nm;
  delete[] dn;
  delete[] out;
  for (int i = 0; i < num_threads; i++) {
    delete iopackArr[i];
    delete otpackArr[i];
  }
}
