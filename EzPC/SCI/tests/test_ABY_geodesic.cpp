#include "FloatingPoint/floating-point.h"
#include "FloatingPoint/fp-math.h"
#include <cfenv>
#include <fstream>
#include <random>
#include <thread>

using namespace sci;
using namespace std;

#define MAX_THREADS 4
#define DEGREE_DECIMAL_PLACES 5
#define RADIUS 6371000 // 6378137

bool verbose = false;
IOPack *iopackArr[MAX_THREADS];
OTPack *otpackArr[MAX_THREADS];
int dim = 1000;
int party = 1;
string address = "127.0.0.1";
int port = 8000;
int num_threads = 1;
bool trig_ops = true;
std::random_device rand_div;
// std::mt19937 generator(rand_div());
std::mt19937 generator(1);
double max_deviation = 0;
double max_ulp_err_a = 0;

int32_t get_exponent(float x) {
  int32_t x_int = *((int32_t *)&x);
  return ((x_int >> 23) & 255) - 127;
}

double ULP_error(float actual, float expected) {
  double abs_err = abs(double(actual) - double(expected));
  int32_t expected_exp = get_exponent(expected);
  double ulp;
  if (expected_exp < -126) {
    ulp = exp2(-126.0 - 23.0);
  } else {
    ulp = exp2(expected_exp - 23.0);
  }
  return abs_err / ulp;
}

void benchmark_1(int tid, int sz) {
  if (sz == 0)
    return;
  int lparty = (tid & 1 ? 3 - party : party);
  FPOp *fp_op;
  fp_op = new FPOp(lparty, iopackArr[tid], otpackArr[tid]);

  float *latA = new float[sz];
  float *lonA = new float[sz];
  float *latB = new float[sz];
  float *lonB = new float[sz];
  float *alpha = new float[sz];
  float *beta = new float[sz];
  float *gamma = new float[sz];
  float *delta = new float[sz];
  float *zeta = new float[sz];
  float *eta = new float[sz];
  float *theta = new float[sz];
  float *lambda = new float[sz];
  float *mu = new float[sz];
  float *nu = new float[sz];
  float *a_eps = new float[sz];
  float *tmp = new float[sz];
  for (int i = 0; i < sz; i++) {
    latA[i] = M_PI * (generator() % 90);
    lonA[i] = M_PI * (generator() % 90);
    latB[i] = M_PI * (generator() % 90);
    lonB[i] = M_PI * (generator() % 90);
    a_eps[i] = generator();
    alpha[i] = cos(latA[i] / 2);
    beta[i] = sin(latB[i] / 2);
    gamma[i] = sin(latA[i] / 2);
    delta[i] = cos(latB[i] / 2);
    zeta[i] = cos(latA[i]);
    eta[i] = cos(latB[i]);
    theta[i] = sin(lonA[i] / 2);
    delta[i] = cos(lonB[i] / 2);
    mu[i] = cos(lonA[i] / 2);
    nu[i] = sin(lonB[i] / 2);
  }

  FPArray LHS;
  for (int i = 0; i < sz; i++) {
    tmp[i] = alpha[i] * alpha[i];
  }
  FPArray alpha_sq = fp_op->input<float>(ALICE, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = 2 * alpha[i] * gamma[i];
  }
  FPArray two_alpha_gamma = fp_op->input<float>(ALICE, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = gamma[i] * gamma[i];
  }
  FPArray gamma_sq = fp_op->input<float>(ALICE, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = zeta[i] * theta[i] * theta[i];
  }
  FPArray zeta_theta_sq = fp_op->input<float>(ALICE, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = 2 * zeta[i] * theta[i] * mu[i];
  }
  FPArray two_zeta_theta_mu = fp_op->input<float>(ALICE, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = zeta[i] * mu[i] * mu[i];
  }
  FPArray zeta_mu_sq = fp_op->input<float>(ALICE, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = beta[i] * beta[i];
  }
  FPArray beta_sq = fp_op->input<float>(BOB, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = beta[i] * delta[i];
  }
  FPArray beta_delta = fp_op->input<float>(BOB, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = delta[i] * delta[i];
  }
  FPArray delta_sq = fp_op->input<float>(BOB, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = eta[i] * lambda[i] * lambda[i];
  }
  FPArray eta_lambda_sq = fp_op->input<float>(BOB, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = eta[i] * lambda[i] * nu[i];
  }
  FPArray eta_lambda_nu = fp_op->input<float>(BOB, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = eta[i] * nu[i] * nu[i];
  }
  FPArray eta_nu_sq = fp_op->input<float>(BOB, sz, tmp);
  for (int i = 0; i < sz; i++) {
    tmp[i] = a_eps[i];
  }
  FPArray RHS = fp_op->input<float>(BOB, sz, tmp);

  LHS = fp_op->mul(alpha_sq, beta_sq);
  LHS = fp_op->sub(LHS, fp_op->mul(two_alpha_gamma, beta_delta));
  LHS = fp_op->add(LHS, fp_op->mul(gamma_sq, delta_sq));
  LHS = fp_op->add(LHS, fp_op->mul(zeta_theta_sq, eta_lambda_sq));
  LHS = fp_op->sub(LHS, fp_op->mul(two_zeta_theta_mu, eta_lambda_nu));
  LHS = fp_op->add(LHS, fp_op->mul(zeta_mu_sq, eta_nu_sq));

  BoolArray out = fp_op->LT(LHS, RHS);

  delete[] latA;
  delete[] lonA;
  delete[] latB;
  delete[] lonB;
  delete[] alpha;
  delete[] beta;
  delete[] gamma;
  delete[] delta;
  delete[] zeta;
  delete[] eta;
  delete[] theta;
  delete[] lambda;
  delete[] mu;
  delete[] nu;
  delete[] a_eps;
  delete[] tmp;
}

float geodesic_ideal(float latA, float lonA, float latB, float lonB) {
  double lat_sub = (double(latA) - double(latB)) / 2.0;
  double lon_sub = (double(lonA) - double(lonB)) / 2.0;
  double sin_lat_sub = sin(lat_sub * M_PI);
  double hav_lat = sin_lat_sub * sin_lat_sub;
  double sin_lon_sub = sin(lon_sub * M_PI);
  double hav_lon = sin_lon_sub * sin_lon_sub;
  double cos_latA = cos(latA * M_PI);
  double cos_latB = cos(latB * M_PI);
  double cos_latA_cos_latB = cos_latA * cos_latB;
  double hav_lon_cos_latA_cos_latB = cos_latA_cos_latB * hav_lon;
  double a = hav_lat + hav_lon_cos_latA_cos_latB;
  /*
  cout << "lat_sub: " << lat_sub << endl;
  cout << "lon_sub: " << lon_sub << endl;
  cout << "sin_lat_sub: " << sin_lat_sub << endl;
  cout << "hav_lat: " << hav_lat << endl;
  cout << "sin_lon_sub: " << sin_lon_sub << endl;
  cout << "hav_lon: " << hav_lon << endl;
  cout << "cos_latA: " << cos_latA << endl;
  cout << "cos_latB: " << cos_latB << endl;
  cout << "cos_latA_cos_latB: " << cos_latA_cos_latB << endl;
  cout << "hav_lon_cos_latA_cos_latB: " << hav_lon_cos_latA_cos_latB << endl;
  cout << "a: " << a << endl;
  // */
  return float(a);
}

double compute_distance(float a) {
  return 2 * RADIUS * atan(sqrt(double(a) / (1.0 - double(a))));
}

float degree_to_radian(float deg) { return double(deg) / 180.0; }

uint32_t flt_to_int(float x) {
  uint32_t x_int = *((uint32_t *)&x);
  return x_int;
}

float int_to_flt(uint32_t x) {
  float y = *((float *)&x);
  return y;
}

void benchmark_2(int tid, int sz) {
  if (sz == 0)
    return;
  int lparty = (tid & 1 ? 3 - party : party);
  FPOp *fp_op;
  fp_op = new FPOp(lparty, iopackArr[tid], otpackArr[tid]);
  FPMath *fp_math;
  fp_math = new FPMath(lparty, iopackArr[tid], otpackArr[tid]);

  float *latA = new float[sz];
  float *lonA = new float[sz];
  float *latB = new float[sz];
  float *lonB = new float[sz];
  float *a_eps = new float[sz];
  float *tmp = new float[sz];
  float smallest_flt_gt_zero = (pow(10.0, -DEGREE_DECIMAL_PLACES)) / 180.0;
  // int smallest_exp = ceil(log2(smallest_flt_gt_zero));
  int res = int(pow(10, DEGREE_DECIMAL_PLACES));
  for (int i = 0; i < sz; i++) {
    uint32_t rand_int;
    float rand_flt;
    {
      do {
        rand_int = generator();
        rand_flt = int_to_flt(rand_int);
      } while (
          !((abs(rand_flt) > smallest_flt_gt_zero && abs(rand_flt) <= 0.5) ||
            rand_flt == 0.0));
      latA[i] = rand_flt;
    }
    {
      do {
        rand_int = generator();
        rand_flt = int_to_flt(rand_int);
      } while (
          !((abs(rand_flt) > smallest_flt_gt_zero && abs(rand_flt) <= 1.0) ||
            rand_flt == 0.0));
      lonA[i] = rand_flt;
    }
    {
      do {
        rand_int = generator();
        rand_flt = int_to_flt(rand_int);
      } while (
          !((abs(rand_flt) > smallest_flt_gt_zero && abs(rand_flt) <= 0.5) ||
            rand_flt == 0.0));
      latB[i] = rand_flt;
    }
    {
      do {
        rand_int = generator();
        rand_flt = int_to_flt(rand_int);
      } while (
          !((abs(rand_flt) > smallest_flt_gt_zero && abs(rand_flt) <= 1.0) ||
            rand_flt == 0.0));
      lonB[i] = rand_flt;
    }
    /*
    float rand_flt;
    rand_flt = ((1 - 2.0*(generator() & 1)) * (generator() % (res *
    90)))/double(res); latA[i] = degree_to_radian(rand_flt); rand_flt = ((1
    - 2.0*(generator() & 1)) * (generator() % (res * 180)))/double(res); lonA[i]
    = degree_to_radian(rand_flt); rand_flt = ((1 - 2.0*(generator() & 1)) *
    (generator() % (res * 90)))/double(res); latB[i] =
    degree_to_radian(rand_flt); rand_flt = ((1 - 2.0*(generator() & 1)) *
    (generator() % (res * 180)))/double(res); lonB[i] =
    degree_to_radian(rand_flt);
    */
    // cout << latA[i] << "\t" << latB[i] << "\t" << lonA[i] << "\t" << lonB[i]
    // << endl;
    assert(abs(latA[i]) <= 0.5);
    assert(abs(latB[i]) <= 0.5);
    assert(abs(lonA[i]) <= 1.0);
    assert(abs(lonB[i]) <= 1.0);
    a_eps[i] = generator();
  }

  FPArray latA_fp = fp_op->input<float>(ALICE, sz, latA);
  FPArray lonA_fp = fp_op->input<float>(ALICE, sz, lonA);
  FPArray latB_fp = fp_op->input<float>(BOB, sz, latB);
  FPArray lonB_fp = fp_op->input<float>(BOB, sz, lonB);
  FPArray a_eps_fp = fp_op->input<float>(BOB, sz, a_eps);
  FPArray two = fp_op->input<float>(ALICE, sz, 2.0);

  FPArray lat_sub = fp_op->mulpow2(fp_op->sub(latA_fp, latB_fp), -1);
  FPArray lon_sub = fp_op->mulpow2(fp_op->sub(lonA_fp, lonB_fp), -1);
  FPArray sin_lat_sub = fp_math->sinpi(lat_sub);
  FPArray hav_lat = fp_op->mul(sin_lat_sub, sin_lat_sub);
  FPArray sin_lon_sub = fp_math->sinpi(lon_sub);
  FPArray hav_lon = fp_op->mul(sin_lon_sub, sin_lon_sub);
  FPArray cos_latA = fp_math->cospi(latA_fp);
  FPArray cos_latB = fp_math->cospi(latB_fp);
  FPArray cos_latA_cos_latB = fp_op->mul(cos_latA, cos_latB);
  FPArray hav_lon_cos_latA_cos_latB = fp_op->mul(cos_latA_cos_latB, hav_lon);
  FPArray a = fp_op->add(hav_lat, hav_lon_cos_latA_cos_latB);

  BoolArray out = fp_op->LT(a, a_eps_fp);
  /*
  print_fp(lat_sub);
  print_fp(lon_sub);
  print_fp(sin_lat_sub);
  print_fp(hav_lat);
  print_fp(sin_lon_sub);
  print_fp(hav_lon);
  print_fp(cos_latA);
  print_fp(cos_latB);
  print_fp(cos_latA_cos_latB);
  print_fp(hav_lon_cos_latA_cos_latB);
  print_fp(a);
  // */

  cout.precision(10);
  FPArray a_pub = fp_op->output(PUBLIC, a);
  vector<float> f_ = a_pub.get_native_type<float>();
  for (int i = 0; i < sz; i++) {
    float expected = geodesic_ideal(latA[i], lonA[i], latB[i], lonB[i]);
    double actual_distance = compute_distance(f_[i]);
    double expected_distance = compute_distance(expected);
    double ulp_err_a = ULP_error(f_[i], expected);
    if (verbose) {
      cout << i << "\t" << latA[i] << "\t" << latB[i] << "\t" << lonA[i] << "\t"
           << lonB[i] << "\t" << f_[i] << "\t" << expected << "\t" << ulp_err_a
           << "\t" << actual_distance << " m\t" << expected_distance << " m\n";
    }
    max_deviation =
        max(max_deviation, abs(actual_distance - expected_distance));
    max_ulp_err_a = max(max_ulp_err_a, abs(ulp_err_a));
  }

  cout << "Max Deviation: " << max_deviation << endl;
  cout << "Max ULP error a: " << max_ulp_err_a << endl;

  delete[] latA;
  delete[] lonA;
  delete[] latB;
  delete[] lonB;
  delete[] a_eps;
}

int main(int argc, char **argv) {
  ArgMapping amap;

  amap.arg("r", party, "Role of party: ALICE/SERVER = 1; BOB/CLIENT = 2");
  amap.arg("p", port, "Port Number");
  amap.arg("ip", address, "IP Address of server (ALICE)");
  amap.arg("nt", num_threads, "Number of threads");
  amap.arg("t", trig_ops, "Trig Ops?");
  amap.arg("N", dim, "Batch sz");
  amap.parse(argc, argv);

  assert(num_threads <= MAX_THREADS);

  for (int i = 0; i < num_threads; i++) {
    iopackArr[i] = new IOPack(party, port + i, address);
    if (i & 1) {
      otpackArr[i] = new OTPack(iopackArr[i], 3 - party);
    } else {
      otpackArr[i] = new OTPack(iopackArr[i], party);
    }
  }
  std::cout << "All Base OTs Done" << std::endl;

  uint64_t num_rounds = iopackArr[0]->get_rounds();
  uint64_t total_comm = 0;
  uint64_t thread_comm[num_threads];
  for (int i = 0; i < num_threads; i++) {
    thread_comm[i] = iopackArr[i]->get_comm();
  }
  auto start = clock_start();
  std::thread bench_threads[num_threads];
  int chunk_sz = dim / num_threads;
  for (int i = 0; i < num_threads; ++i) {
    int offset = i * chunk_sz;
    int lsz;
    if (i == (num_threads - 1)) {
      lsz = dim - offset;
    } else {
      lsz = chunk_sz;
    }
    if (trig_ops) {
      bench_threads[i] = std::thread(benchmark_2, i, lsz);
    } else {
      bench_threads[i] = std::thread(benchmark_1, i, lsz);
    }
  }
  for (int i = 0; i < num_threads; ++i) {
    bench_threads[i].join();
  }
  long long t = time_from(start);

  for (int i = 0; i < num_threads; i++) {
    thread_comm[i] = iopackArr[i]->get_comm() - thread_comm[i];
    total_comm += thread_comm[i];
  }
  num_rounds = iopackArr[0]->get_rounds() - num_rounds;

  if (party == BOB) {
    uint64_t total_comm_ALICE = 0;
    iopackArr[0]->io->recv_data(&total_comm_ALICE, sizeof(uint64_t));
    total_comm += total_comm_ALICE;
    cout << "Total Communication (ALICE + BOB)\t" << total_comm << " bytes"
         << endl;
    string file_addr;
    file_addr = "FP-geodesic.csv";
    bool write_title = true;
    {
      fstream result(file_addr.c_str(), fstream::in);
      if (result.is_open())
        write_title = false;
      result.close();
    }
    fstream result(file_addr.c_str(), fstream::out | fstream::app);
    if (write_title) {
      result << "Trig Ops?,Variant,#Operations,#Threads,Time "
                "(ms),Communication (Bytes),Total Rounds"
             << endl;
    }
    result << trig_ops << "," << dim << "," << num_threads << ","
           << t / (1000.0) << "," << total_comm << "," << num_rounds << endl;
    result.close();
  } else {
    iopackArr[0]->io->send_data(&total_comm, sizeof(uint64_t));
  }
  cout << "Ops/s:\t" << (double(dim) / t) * 1e6 << std::endl;
  cout << "Total Time\t" << t / (1000.0) << " ms" << endl;

  for (int i = 0; i < num_threads; i++) {
    delete iopackArr[i];
    delete otpackArr[i];
  }
}
