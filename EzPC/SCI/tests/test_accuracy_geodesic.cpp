#include "FloatingPoint/floating-point.h"
#include "FloatingPoint/fp-math.h"
#include "mpfr.h"
#include <cfenv>
#include <fstream>
#include <random>
#include <thread>

#define MPFR_PREC 300

using namespace sci;
using namespace std;

mpfr_t mval;

#define MAX_THREADS 4
#define DEGREE_DECIMAL_PLACES 5
#define FIX_SCALE 32
#define FIXED_RUN
#define RADIUS 6371000//6378137

bool verbose = true;
IOPack *iopackArr[MAX_THREADS];
OTPack *otpackArr[MAX_THREADS];
int dim = 100000;
int party = 1;
string address = "127.0.0.1";
int port = 8000;
int num_threads = 1;
int seed = 0;
int g_iter = 0;
int global_iters = 1;
std::random_device rand_div;
// std::mt19937 generator(rand_div());
std::mt19937 generator(0);
double max_ulp_err_a = 0.0;
double avg_ulp_err_a = 0.0;
double max_deviation = 0.0;
double avg_deviation = 0.0;

float degree_to_radian(float deg) { return double(deg) / 180.0; }

uint32_t flt_to_int(float x) {
  uint32_t x_int = *((uint32_t *)&x);
  return x_int;
}

float int_to_flt(uint32_t x) {
  float y = *((float *)&x);
  return y;
}

int64_t get_exponent(double x) {
  int64_t x_int = *((int64_t *)&x);
  return ((x_int >> 52) & 2047) - 1023;
}

double ULP_error(float actual, double expected) {
  if (actual == expected)
    return 0.0;
  double abs_err = abs(double(actual) - expected);
  // std::cout << actual << "\t" << expected << "\t" << abs_err << std::endl;
  int64_t expected_exp = get_exponent(expected);
  // std::cout << expected_exp << std::endl;
  double ulp;
  ulp = exp2(expected_exp - 23.0);
  // std::cout << expected_exp << std::endl;
  return abs_err / ulp;
}

double precise_sinpi(float x) {
  mpfr_const_pi(mval, MPFR_RNDN);
  mpfr_mul_d(mval, mval, (double)x, MPFR_RNDN);
  mpfr_sin(mval, mval, MPFR_RNDN);
  return mpfr_get_d(mval, MPFR_RNDN);
}

double precise_cospi(float x) {
  mpfr_const_pi(mval, MPFR_RNDN);
  mpfr_mul_d(mval, mval, (double)x, MPFR_RNDN);
  mpfr_cos(mval, mval, MPFR_RNDN);
  return mpfr_get_d(mval, MPFR_RNDN);
}

double geodesic_ideal(float latA, float lonA, float latB, float lonB) {
  double lat_sub = (double(latA) - double(latB)) / 2.0;
  double lon_sub = (double(lonA) - double(lonB)) / 2.0;
  double sin_lat_sub = precise_sinpi(lat_sub);
  double hav_lat = sin_lat_sub * sin_lat_sub;
  double sin_lon_sub = precise_sinpi(lon_sub);
  double hav_lon = sin_lon_sub * sin_lon_sub;
  double cos_latA = precise_cospi(latA);
  double cos_latB = precise_cospi(latB);
  double cos_latA_cos_latB = cos_latA * cos_latB;
  double hav_lon_cos_latA_cos_latB = cos_latA_cos_latB * hav_lon;
  double a = hav_lat + hav_lon_cos_latA_cos_latB;
  return a;
}

template <typename type>
void benchmark_1_ideal(int sz, float *latA, float *lonA, float *latB,
                       float *lonB, float *a_eps, type *f) {
  double *alpha = new double[sz];
  double *beta = new double[sz];
  double *gamma = new double[sz];
  double *delta = new double[sz];
  double *zeta = new double[sz];
  double *eta = new double[sz];
  double *theta = new double[sz];
  double *lambda = new double[sz];
  double *mu = new double[sz];
  double *nu = new double[sz];
  for (int i = 0; i < sz; i++) {
    alpha[i] = precise_cospi(latA[i] / 2);
    beta[i] = precise_sinpi(latB[i] / 2);
    gamma[i] = precise_sinpi(latA[i] / 2);
    delta[i] = precise_cospi(latB[i] / 2);
    zeta[i] = precise_cospi(latA[i]);
    eta[i] = precise_cospi(latB[i]);
    theta[i] = precise_sinpi(lonA[i] / 2);
    lambda[i] = precise_cospi(lonB[i] / 2);
    mu[i] = precise_cospi(lonA[i] / 2);
    nu[i] = precise_sinpi(lonB[i] / 2);
  }

  vector<type> LHS(sz, 0.0);
  vector<type> alpha_sq = LHS, two_alpha_gamma = LHS, gamma_sq = LHS,
               zeta_theta_sq = LHS, two_zeta_theta_mu = LHS, zeta_mu_sq = LHS,
               beta_sq = LHS, beta_delta = LHS, delta_sq = LHS,
               eta_lambda_sq = LHS, eta_lambda_nu = LHS, eta_nu_sq = LHS,
               RHS = LHS;
  for (int i = 0; i < sz; i++) {
    alpha_sq[i] = alpha[i] * alpha[i];
    two_alpha_gamma[i] = 2 * alpha[i] * gamma[i];
    gamma_sq[i] = gamma[i] * gamma[i];
    zeta_theta_sq[i] = zeta[i] * theta[i] * theta[i];
    two_zeta_theta_mu[i] = 2 * zeta[i] * theta[i] * mu[i];
    zeta_mu_sq[i] = zeta[i] * mu[i] * mu[i];
  }
  for (int i = 0; i < sz; i++) {
    beta_sq[i] = beta[i] * beta[i];
    beta_delta[i] = beta[i] * delta[i];
    delta_sq[i] = delta[i] * delta[i];
    eta_lambda_sq[i] = eta[i] * lambda[i] * lambda[i];
    eta_lambda_nu[i] = eta[i] * lambda[i] * nu[i];
    eta_nu_sq[i] = eta[i] * nu[i] * nu[i];
    RHS[i] = a_eps[i];
  }

  for (int i = 0; i < sz; i++) {
    LHS[i] = alpha_sq[i] * beta_sq[i];
    LHS[i] = LHS[i] - (two_alpha_gamma[i] * beta_delta[i]);
    LHS[i] = LHS[i] + (gamma_sq[i] * delta_sq[i]);
    LHS[i] = LHS[i] + (zeta_theta_sq[i] * eta_lambda_sq[i]);
    LHS[i] = LHS[i] - (two_zeta_theta_mu[i] * eta_lambda_nu[i]);
    f[i] = LHS[i] + (zeta_mu_sq[i] * eta_nu_sq[i]);
  }

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
}

float compute_distance (float a) {
    return 2*RADIUS*atan(sqrt(a/(1.0-a)));
}

double compute_distance_dbl (double a) {
    return 2*RADIUS*atan(sqrt(a/(1.0-a)));
}

void benchmark_1_cleartext(int tid, int sz, float *latA, float *lonA, float *latB,
                 float *lonB, float *a_eps, float *f_) {
  if (sz == 0)
    return;
  int lparty = (tid & 1 ? 3 - party : party);

  double *alpha = new double[sz];
  double *beta = new double[sz];
  double *gamma = new double[sz];
  double *delta = new double[sz];
  double *zeta = new double[sz];
  double *eta = new double[sz];
  double *theta = new double[sz];
  double *lambda = new double[sz];
  double *mu = new double[sz];
  double *nu = new double[sz];
  float *tmp = new float[sz];
  for (int i = 0; i < sz; i++) {
    alpha[i] = precise_cospi(latA[i] / 2);
    beta[i] = precise_sinpi(latB[i] / 2);
    gamma[i] = precise_sinpi(latA[i] / 2);
    delta[i] = precise_cospi(latB[i] / 2);
    zeta[i] = precise_cospi(latA[i]);
    eta[i] = precise_cospi(latB[i]);
    theta[i] = precise_sinpi(lonA[i] / 2);
    lambda[i] = precise_cospi(lonB[i] / 2);
    mu[i] = precise_cospi(lonA[i] / 2);
    nu[i] = precise_sinpi(lonB[i] / 2);
  }

  int64_t* alpha_sq = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    alpha_sq[i] = (alpha[i] * alpha[i]) * (1LL << FIX_SCALE);
  }
  int64_t* two_alpha_gamma = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    two_alpha_gamma[i] = 2 * alpha[i] * gamma[i] * (1LL << FIX_SCALE);
  }
  int64_t* gamma_sq = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    gamma_sq[i] = gamma[i] * gamma[i] * (1LL << FIX_SCALE);
  }
  int64_t* zeta_theta_sq = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    zeta_theta_sq[i] = zeta[i] * theta[i] * theta[i] * (1LL << FIX_SCALE);
  }
  int64_t* two_zeta_theta_mu = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    two_zeta_theta_mu[i] = 2 * zeta[i] * theta[i] * mu[i] * (1LL << FIX_SCALE);
  }
  int64_t* zeta_mu_sq = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    zeta_mu_sq[i] = zeta[i] * mu[i] * mu[i] * (1LL << FIX_SCALE);
  }
  int64_t* beta_sq = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    beta_sq[i] = beta[i] * beta[i] * (1LL << FIX_SCALE);
  }
  int64_t* beta_delta = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    beta_delta[i] = beta[i] * delta[i] * (1LL << FIX_SCALE);
  }
  int64_t* delta_sq = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    delta_sq[i] = delta[i] * delta[i] * (1LL << FIX_SCALE);
  }
  int64_t* eta_lambda_sq = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    eta_lambda_sq[i] = eta[i] * lambda[i] * lambda[i] * (1LL << FIX_SCALE);
  }
  int64_t* eta_lambda_nu = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    eta_lambda_nu[i] = eta[i] * lambda[i] * nu[i] * (1LL << FIX_SCALE);
  }
  int64_t* eta_nu_sq = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    eta_nu_sq[i] = eta[i] * nu[i] * nu[i] * (1LL << FIX_SCALE);
  }
  int64_t* RHS = new int64_t[sz];
  for (int i = 0; i < sz; i++) {
    RHS[i] = a_eps[i] * (1LL << FIX_SCALE);
  }

  int64_t* LHS = new int64_t[sz];
  int64_t* a = new int64_t[sz];
  vector<float> f(sz);
  for (int i = 0; i < sz; i++) {
    LHS[i] = (alpha_sq[i] * beta_sq[i]) >> FIX_SCALE;
    LHS[i] = LHS[i] - ((two_alpha_gamma[i]*beta_delta[i]) >> FIX_SCALE);
    LHS[i] = LHS[i] + ((gamma_sq[i] * delta_sq[i]) >> FIX_SCALE);
    LHS[i] = LHS[i] + ((zeta_theta_sq[i] * eta_lambda_sq[i]) >> FIX_SCALE);
    LHS[i] = LHS[i] - ((two_zeta_theta_mu[i] * eta_lambda_nu[i]) >> FIX_SCALE);
    a[i] = LHS[i] + ((zeta_mu_sq[i] * eta_nu_sq[i]) >> FIX_SCALE);
    f[i] = a[i]/double(1LL << FIX_SCALE);
  }

  memcpy(f_, f.data(), sizeof(float)*sz);

  double *expected_dbl = new double[sz];
  float *expected_flt = new float[sz];
  benchmark_1_ideal<double>(sz, latA, lonA, latB, lonB, a_eps, expected_dbl);
  benchmark_1_ideal<float>(sz, latA, lonA, latB, lonB, a_eps, expected_flt);

  for (int i = 0; i < sz; i++) {
    // float expected = geodesic_ideal(latA[i], lonA[i], latB[i], lonB[i]);
    double expected = geodesic_ideal(latA[i], lonA[i], latB[i], lonB[i]);
    double ulp_err_a = ULP_error(f_[i], expected);
    double distance_expected = compute_distance_dbl(expected);
    double distance_fixed = compute_distance_dbl(f_[i]);
    double deviation = abs(distance_expected - distance_fixed);
    max_deviation = max(deviation, max_deviation);
    if (isnormal(deviation)) avg_deviation += deviation;
    max_ulp_err_a = max(max_ulp_err_a, ulp_err_a);
    avg_ulp_err_a += ulp_err_a;
    // double ulp_err_a = ULP_error(f_[i], expected_[i]);
    if (deviation >= max_deviation) {
      cout.precision(20);
      cout << i << "\t" << latA[i] << "\t" << latB[i] << "\t" << lonA[i] << "\t"
           << lonB[i] << "\t" << f_[i] << "\t" << expected << "\t"
           << expected_flt[i] << "\t" << expected_dbl[i] << "\t" << ulp_err_a
           << "\t" << avg_ulp_err_a / (i + 1) << "\t" << distance_fixed << "\t"
           << distance_expected << "\t" << deviation << "\t" << avg_deviation / (i + 1) << endl;
    }
  }
  cout << "Max ULP error (a): " << max_ulp_err_a << endl;
  cout << "Avg ULP error (a): " << avg_ulp_err_a / ((g_iter + 1) * sz) << endl;
  cout << "Max Deviation: " << max_deviation << endl;
  cout << "Avg Deviation: " << avg_deviation / ((g_iter + 1) * sz) << endl;

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
  delete[] tmp;
}

void benchmark_1(int tid, int sz, float *latA, float *lonA, float *latB,
                 float *lonB, float *a_eps, float *f_) {
  if (sz == 0)
    return;
  int lparty = (tid & 1 ? 3 - party : party);
  FPOp *fp_op;
  fp_op = new FPOp(lparty, iopackArr[tid], otpackArr[tid]);

  double *alpha = new double[sz];
  double *beta = new double[sz];
  double *gamma = new double[sz];
  double *delta = new double[sz];
  double *zeta = new double[sz];
  double *eta = new double[sz];
  double *theta = new double[sz];
  double *lambda = new double[sz];
  double *mu = new double[sz];
  double *nu = new double[sz];
  float *tmp = new float[sz];
  for (int i = 0; i < sz; i++) {
    alpha[i] = precise_cospi(latA[i] / 2);
    beta[i] = precise_sinpi(latB[i] / 2);
    gamma[i] = precise_sinpi(latA[i] / 2);
    delta[i] = precise_cospi(latB[i] / 2);
    zeta[i] = precise_cospi(latA[i]);
    eta[i] = precise_cospi(latB[i]);
    theta[i] = precise_sinpi(lonA[i] / 2);
    lambda[i] = precise_cospi(lonB[i] / 2);
    mu[i] = precise_cospi(lonA[i] / 2);
    nu[i] = precise_sinpi(lonB[i] / 2);
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
  FPArray a = fp_op->add(LHS, fp_op->mul(zeta_mu_sq, eta_nu_sq));

  BoolArray out = fp_op->LT(a, RHS);

  FPArray a_pub = fp_op->output(PUBLIC, a);
  vector<float> f = a_pub.get_native_type<float>();
  memcpy(f_, f.data(), sizeof(float)*sz);

  double *expected_dbl = new double[sz];
  float *expected_flt = new float[sz];
  benchmark_1_ideal<double>(sz, latA, lonA, latB, lonB, a_eps, expected_dbl);
  benchmark_1_ideal<float>(sz, latA, lonA, latB, lonB, a_eps, expected_flt);

  for (int i = 0; i < sz; i++) {
    // float expected = geodesic_ideal(latA[i], lonA[i], latB[i], lonB[i]);
    double expected = geodesic_ideal(latA[i], lonA[i], latB[i], lonB[i]);
    double ulp_err_a = ULP_error(f_[i], expected);
    max_ulp_err_a = max(max_ulp_err_a, ulp_err_a);
    avg_ulp_err_a += ulp_err_a;
    // double ulp_err_a = ULP_error(f_[i], expected_[i]);
    if (ulp_err_a >= max_ulp_err_a) {
      cout.precision(20);
      cout << i << "\t" << latA[i] << "\t" << latB[i] << "\t" << lonA[i] << "\t"
           << lonB[i] << "\t" << f_[i] << "\t" << expected << "\t"
           << expected_flt[i] << "\t" << expected_dbl[i] << "\t" << ulp_err_a
           << "\t" << avg_ulp_err_a / (i + 1) << endl;
    }
  }
  cout << "Max ULP error (a): " << max_ulp_err_a << endl;
  cout << "Avg ULP error (a): " << avg_ulp_err_a / ((g_iter + 1) * sz) << endl;

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
  delete[] tmp;
}

void benchmark_2(int tid, int sz, float *latA, float *lonA, float *latB,
                 float *lonB, float *a_eps, float *f_) {
  if (sz == 0)
    return;
  int lparty = (tid & 1 ? 3 - party : party);
  FPOp *fp_op;
  fp_op = new FPOp(lparty, iopackArr[tid], otpackArr[tid]);
  FPMath *fp_math;
  fp_math = new FPMath(lparty, iopackArr[tid], otpackArr[tid]);

  FPArray latA_fp = fp_op->input<float>(ALICE, sz, latA);
  FPArray lonA_fp = fp_op->input<float>(ALICE, sz, lonA);
  FPArray latB_fp = fp_op->input<float>(BOB, sz, latB);
  FPArray lonB_fp = fp_op->input<float>(BOB, sz, lonB);
  FPArray a_eps_fp = fp_op->input<float>(BOB, sz, a_eps);
  FPArray two = fp_op->input<float>(ALICE, sz, 2.0);

  FPArray lat_sub = fp_op->div(fp_op->sub(latA_fp, latB_fp), two);
  FPArray lon_sub = fp_op->div(fp_op->sub(lonA_fp, lonB_fp), two);
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

  FPArray a_pub = fp_op->output(PUBLIC, a);
  vector<float> f = a_pub.get_native_type<float>();
  memcpy(f_, f.data(), sizeof(float)*sz);
}

int main(int argc, char **argv) {
  mpfr_init2(mval, MPFR_PREC);
  ArgMapping amap;

  amap.arg("r", party, "Role of party: ALICE/SERVER = 1; BOB/CLIENT = 2");
  amap.arg("p", port, "Port Number");
  amap.arg("ip", address, "IP Address of server (ALICE)");
  amap.arg("nt", num_threads, "Number of threads");
  amap.arg("N", dim, "Batch sz");
  amap.arg("s", seed, "Seed");
  amap.arg("o", seed, "Seed");
  amap.parse(argc, argv);
  generator.seed(seed);

  assert(num_threads <= MAX_THREADS);

  for (int i = 0; i < num_threads; i++) {
    iopackArr[i] = new sci::IOPack(party, port + i, address);
    if (i & 1) {
      otpackArr[i] = new OTPack(iopackArr[i], 3 - party);
    } else {
      otpackArr[i] = new OTPack(iopackArr[i], party);
    }
  }
  std::cout << "All Base OTs Done" << std::endl;

  float *latA = new float[dim];
  float *lonA = new float[dim];
  float *latB = new float[dim];
  float *lonB = new float[dim];
  float *a_eps = new float[dim];
  float *f_ = new float[dim];
  float smallest_flt_gt_zero = (pow(10.0, -DEGREE_DECIMAL_PLACES)) / 180.0;
  int res = int(pow(10, DEGREE_DECIMAL_PLACES));
  for (int j = 0; j < global_iters; j++) {
    cout << "============== Seed " << j << "===========" << endl;
    g_iter = j;
    seed = j;
    generator.seed(seed);
    for (int i = 0; i < dim; i++) {
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
      /* latA[i] = -0.033836532384157180786; */
      /* latB[i] = -0.033865682780742645264; */
      /* lonA[i] = 1.0093440749869841966e-07; */
      /* lonB[i] = 7.398289199045393616e-07; */

      assert(abs(latA[i]) <= 0.5);
      assert(abs(latB[i]) <= 0.5);
      assert(abs(lonA[i]) <= 1.0);
      assert(abs(lonB[i]) <= 1.0);
      a_eps[i] = generator();
    }

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
      // bench_threads[i] =
      // std::thread(benchmark_2,i,lsz,latA+offset,lonA+offset,latB+offset,lonB+offset,a_eps+offset,f_+offset);
      /*
      bench_threads[i] = std::thread(
          benchmark_1, i, lsz, latA + offset, lonA + offset, latB + offset,
          lonB + offset, a_eps + offset, f_ + offset);
      */
      bench_threads[i] = std::thread(
          benchmark_1_cleartext, i, lsz, latA + offset, lonA + offset, latB + offset,
          lonB + offset, a_eps + offset, f_ + offset);
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
    // cout <<"Ops/s:\t" <<(double(dim)/t)*1e6<< std::endl;
    // cout <<"Total Time\t" << t/(1000.0) <<" ms"<< endl;
  }

  // /*
  if (party == BOB) {
      string file_addr;
      /* file_addr = "geodesic-out-no-trig.csv"; */
      file_addr = "geodesic-fixed-no-trig.csv";
      fstream result(file_addr.c_str(), fstream::out|fstream::app);
      for (int i = 0; i < dim; i++) {
          result << seed << "\t" << i << "\t" << flt_to_int(latA[i]) << "\t"
              << flt_to_int(latB[i]) << "\t" << flt_to_int(lonA[i]) << "\t"
              << flt_to_int(lonB[i]) << "\t" << flt_to_int(f_[i]) << endl;
      }
      result.close();
  }
  // */

  for (int i = 0; i < num_threads; i++) {
    delete iopackArr[i];
    delete otpackArr[i];
  }
  delete[] latA;
  delete[] lonA;
  delete[] latB;
  delete[] lonB;
  delete[] a_eps;
  delete[] f_;
  mpfr_clear(mval);
}
