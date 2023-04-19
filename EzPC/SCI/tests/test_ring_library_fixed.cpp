#include "library_fixed.h"
#include <fstream>
#include <iostream>
#include <random>
#include <thread>

using namespace std;

#define MAX_THREADS 4

int party = 1;
string address = "127.0.0.1";
int port = 8000;
int num_threads = 4;
int bitlength = 32;

int64_t get_mask(int bw) {
    int64_t mask = (1ULL << bw) - 1;
    if (bw < 64) return mask;
    else return -1;
}

void test_matmul() {
    std::random_device rand_div;
    std::mt19937 generator(rand_div());
    int bwA = 16;
    int bwB = 8;
    int bwTemp = 32;
    int bwC = 8;
    int64_t maskA = get_mask(bwA);
    int64_t maskB = get_mask(bwB);
    int64_t maskC = get_mask(bwC);
    int I = 1;
    int K = 100;
    int J = 35;
    int shrA = 1;
    int shrB = 1;
    int H1 = 6;
    int H2 = 1;
    int demote = 256;
    int64_t* A = new int64_t[I*K];
    for (int i = 0; i < I*K; i++) {
        // A[i] = generator() & maskA;
        cin >> A[i];
    }
    int64_t* B = new int64_t[K*J];
    for (int i = 0; i < K*J; i++) {
        // B[i] = (party == 1 ? generator() & maskB : 0);
        cin >> B[i];
    }
    int64_t* C = new int64_t[I*J];
    initialize();

    MatMul(I, K, J, shrA, shrB, H1, H2, demote, bwA, bwB, bwTemp, bwC, A, B, C, nullptr);

    finalize();
}

int main(int argc, char **argv) {
  ArgMapping amap;
  amap.arg("r", party, "Role of party: ALICE = 1; BOB = 2");
  amap.arg("p", port, "Port Number");
  amap.arg("nt", num_threads, "Num Threads");
  amap.arg("ip", address, "Server IP address");
  amap.parse(argc, argv);

  test_matmul();
}
