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

/*
typedef int16_t TypeA;
typedef int16_t TypeB;
typedef int32_t TypeC;
typedef int16_t TypeTemp;

void test_matmul() {
    std::random_device rand_div;
    std::mt19937 generator(rand_div());
    // std::mt19937 generator(0);
    int I = 1024;
    int K = 4;
    int J = 16;
    int shrA = 4;
    int shrB = 8;
    int H1 = 2;
    int H2 = 0;
    int demote = 1;
    TypeA* A = new TypeA[I*K];
    for (int i = 0; i < I*K; i++) {
        A[i] = generator();
    }
    TypeB* B = new TypeB[K*J];
    for (int i = 0; i < K*J; i++) {
        B[i] = (party == 1 ? generator() : 0);
    }
    TypeC* C = new TypeC[I*J];
    initialize();

    MatMul<TypeA,TypeB,TypeTemp,TypeC>(A,B,C,nullptr,I,K,J,shrA,shrB,H1,H2,demote);;

    finalize();
}
// */

/*
typedef int16_t TypeA;
typedef int16_t TypeB;
typedef int32_t TypeC;
typedef int32_t TypeTemp;

void test_convolution() {
    std::random_device rand_div;
    std::mt19937 generator(rand_div());
    // std::mt19937 generator(0);
    int N = 1;
    int H = 230;
    int W = 230;
    int CIN = 3;
    int HF = 7;
    int WF = 7;
    int CINF = 3;
    int COUTF = 64;
    int HOUT = 112;
    int WOUT = 112;
    int HPADL = 0;
    int HPADR = 0;
    int WPADL = 0;
    int WPADR = 0;
    int HSTR = 2;
    int WSTR = 2;
    int HDL = 1;
    int WDL = 1;
    int G = 1;
    int shrA = 1;
    int shrB = 1;
    int H1 = 18;
    int H2 = 0;
    int demote = 1;
    TypeA* A = new TypeA[N*H*W*CIN];
    for (int i = 0; i < N*H*W*CIN; i++) {
        A[i] = generator();
    }
    TypeB* B = new TypeB[G*HF*WF*CINF*COUTF];
    for (int i = 0; i < G*HF*WF*CINF*COUTF; i++) {
        B[i] = generator();
    }
    TypeC* C = new TypeC[N*HOUT*WOUT*COUTF*G];
    for (int i = 0; i < N*HOUT*WOUT*COUTF*G; i++) {
        C[i] = generator();
    }
    initialize();

    Convolution<TypeA, TypeB, TypeTemp, TypeC>(A, B, C, nullptr, N, H, W, CIN,
HF, WF, CINF, COUTF, HOUT, WOUT, HPADL, HPADR, WPADL, WPADR, HSTR, WSTR, HDL,
WDL, G, shrA, shrB, H1, H2, demote);

    finalize();
}
// */

/*
typedef int32_t TypeA;
typedef int16_t TypeB;
typedef int16_t TypeBNW;
typedef int16_t TypeBNB;
typedef int32_t TypeTemp;

void test_BNorm() {
    std::random_device rand_div;
    std::mt19937 generator(rand_div());
    // std::mt19937 generator(0);
    int32_t I = 1*30*40;
    int32_t J = 128;
    int32_t shA = 1;
    int32_t shBNB = 0;
    int32_t shB = 14;
    TypeA* A = new TypeA[I*J];
    for (int i = 0; i < I*J; i++) {
        A[i] = generator();
    }
    TypeBNW* BNW = new TypeBNW[J];
    TypeBNB* BNB = new TypeBNB[J];
    for (int i = 0; i < J; i++) {
        BNW[i] = generator();
        BNB[i] = generator();
    }
    TypeB* B = new TypeB[I*J];

    initialize();

    BNorm<TypeA, TypeBNW, TypeBNB, TypeTemp, TypeB>(A, BNW, BNB, B, I, J, shA,
shBNB, shB);

    finalize();

    delete[] A;
    delete[] BNW;
    delete[] BNB;
    delete[] B;
}
*/

// MBConv<int16_t, int8_t, int8_t, int8_t, int8_t, int8_t, int8_t, int8_t,
// int8_t, int8_t, int16_t, int16_t, int16_t, int32_t, int32_t, int32_t,
// int32_t>(&tmp378_16[0][0][0][0], &L9F1[0][0][0][0][0], &L9W1[0], &L9B1[0],
// &L9F2[0][0][0][0][0], &L9W2[0], &L9B2[0], &L9F3[0][0][0][0][0], &L9W3[0],
// &L9B3[0], &tmp391_16[0][0][0][0], &tmp392_16[0][0][0], &tmp393_16[0],
// &tmp390_32[0], 1, 15, 20, 96, 192, 3, 3, 96, 15, 20, 1, 1, 1, 1, 1, 1, 7, 4,
// 8, 1572864L, 393216L, 1, 1, 64, 8, 1, 16, 1, 1, 64, 1, 8, 1, 1, 16, 1, 2, 64,
// 1);

/*
typedef int16_t TypeA;
typedef int8_t TypeF1;
typedef int8_t TypeB1W;
typedef int8_t TypeB1B;
typedef int8_t TypeF2;
typedef int8_t TypeB2W;
typedef int8_t TypeB2B;
typedef int8_t TypeF3;
typedef int8_t TypeB3W;
typedef int8_t TypeB3B;
typedef int16_t TypeC;
typedef int16_t TypeX;
typedef int16_t TypeT;
typedef int32_t TypeU;
typedef int32_t TypeUB1W;
typedef int32_t TypeUB2W;
typedef int32_t TypeUB3W;

void test_MBConv() {
    std::random_device rand_div;
    std::mt19937 generator(rand_div());
    // std::mt19937 generator(0);
    int N = 1;
    int H = 15;
    int W = 20;
    int Cin = 96;
    int Ct = 192;
    int HF = 3;
    int WF = 3;
    int Cout = 96;
    int Hout = 15;
    int Wout = 20;
    int HPADL = 1;
    int HPADR = 1;
    int WPADL = 1;
    int WPADR = 1;
    int HSTR = 1;
    int WSTR = 1;
    int D1 = 7;
    int D2 = 4;
    int D3 = 8;
    TypeUB1W SIX_1 = 1572864L;
    TypeUB2W SIX_2 = 393216L;
    int shr1 = 1;
    int shr2 = 1;
    int shr3 = 64;
    int shr4 = 8;
    int shr5 = 1;
    int shr6 = 16;
    int shr7 = 1;
    int shr8 = 1;
    int shr9 = 64;
    int shl1 = 1;
    int shl2 = 8;
    int shl3 = 1;
    int shl4 = 1;
    int shl5 = 16;
    int shl6 = 1;
    int shl7 = 2;
    int shl8 = 64;
    int shl9 = 1;

    TypeA* A = new TypeA[N*H*W*Cin];
    TypeF1* F1 = new TypeF1[Cin*Ct];
    TypeB1W* BN1W = new TypeB1W[Ct];
    TypeB1B* BN1B = new TypeB1B[Ct];
    TypeF2* F2 = new TypeF2[Ct*HF*WF];
    TypeB2W* BN2W = new TypeB2W[Ct];
    TypeB2B* BN2B = new TypeB2B[Ct];
    TypeF3* F3 = new TypeF3[Ct*Cout];
    TypeB3W* BN3W = new TypeB3W[Cout];
    TypeB3B* BN3B = new TypeB3B[Cout];

    for (int i = 0; i < N*H*W*Cin; i++) {
        A[i] = generator();
    }
    for (int i = 0; i < Cin*Ct; i++) {
        F1[i] = (party == 1 ? generator() : 0);
    }
    for (int i = 0; i < Ct*HF*WF; i++) {
        F2[i] = (party == 1 ? generator() : 0);
    }
    for (int i = 0; i < Ct*Cout; i++) {
        F3[i] = (party == 1 ? generator() : 0);
    }
    for (int i = 0; i < Ct; i++) {
        BN1W[i] = (party == 1 ? generator() : 0);
        BN1B[i] = (party == 1 ? generator() : 0);
        BN2W[i] = (party == 1 ? generator() : 0);
        BN2B[i] = (party == 1 ? generator() : 0);
    }
    for (int i = 0; i < Cout; i++) {
        BN3W[i] = (party == 1 ? generator() : 0);
        BN3B[i] = (party == 1 ? generator() : 0);
    }

    TypeC* C = new TypeC[N*Hout*Wout*Cout];
    // TypeC* C_layerwise = new TypeC[N*Hout*Wout*Cout];
    // TypeC* C_seedot = new TypeC[N*Hout*Wout*Cout];

    initialize();

    MBConv<TypeA, TypeF1, TypeB1W, TypeB1B, TypeF2, TypeB2W, TypeB2B, TypeF3,
TypeB3W, TypeB3B, TypeC, TypeX, TypeT, TypeU, TypeUB1W, TypeUB2W, TypeUB3W>(A,
F1, BN1W, BN1B, F2, BN2W, BN2B, F3, BN3W, BN3B, C, nullptr, nullptr, nullptr, N,
H, W, Cin, Ct, HF, WF, Cout, Hout, Wout, HPADL, HPADR, WPADL, WPADR, HSTR, WSTR,
D1, D2, D3, SIX_1, SIX_2, shr1, shr2, shr3, shr4, shr5, shr6, shr7, shr8, shr9,
shl1, shl2, shl3, shl4, shl5, shl6, shl7, shl8, shl9);
    // cleartext_MBConv<TypeA, TypeF1, TypeB1W, TypeB1B, TypeF2, TypeB2W,
TypeB2B, TypeF3, TypeB3W, TypeB3B, TypeC, TypeX, TypeT, TypeU, TypeUB1W,
TypeUB2W, TypeUB3W>(A, F1, BN1W, BN1B, F2, BN2W, BN2B, F3, BN3W, BN3B, C,
nullptr, nullptr, nullptr, N, H, W, Cin, Ct, HF, WF, Cout, Hout, Wout, HPADL,
HPADR, WPADL, WPADR, HSTR, WSTR, D1, D2, D3, SIX_1, SIX_2, shr1, shr2, shr3,
shr4, shr5, shr6, shr7, shr8, shr9, shl1, shl2, shl3, shl4, shl5, shl6, shl7,
shl8, shl9);
    // cleartext_MBConv_layerwise<TypeA, TypeF1, TypeB1W, TypeB1B, TypeF2,
TypeB2W, TypeB2B, TypeF3, TypeB3W, TypeB3B, TypeC, TypeX, TypeT, TypeU,
TypeUB1W, TypeUB2W, TypeUB3W>(A, F1, BN1W, BN1B, F2, BN2W, BN2B, F3, BN3W, BN3B,
C_layerwise, nullptr, nullptr, nullptr, N, H, W, Cin, Ct, HF, WF, Cout, Hout,
Wout, HPADL, HPADR, WPADL, WPADR, HSTR, WSTR, D1, D2, D3, SIX_1, SIX_2, shr1,
shr2, shr3, shr4, shr5, shr6, shr7, shr8, shr9, shl1, shl2, shl3, shl4, shl5,
shl6, shl7, shl8, shl9);
    // cleartext_MBConv_seedot<TypeA, TypeF1, TypeB1W, TypeB1B, TypeF2, TypeB2W,
TypeB2B, TypeF3, TypeB3W, TypeB3B, TypeC, TypeX, TypeT, TypeU, TypeUB1W,
TypeUB2W, TypeUB3W>(A, F1, BN1W, BN1B, F2, BN2W, BN2B, F3, BN3W, BN3B, C_seedot,
nullptr, nullptr, nullptr, N, H, W, Cin, Ct, HF, WF, Cout, Hout, Wout, HPADL,
HPADR, WPADL, WPADR, HSTR, WSTR, D1, D2, D3, SIX_1, SIX_2, shr1, shr2, shr3,
shr4, shr5, shr6, shr7, shr8, shr9, shl1, shl2, shl3, shl4, shl5, shl6, shl7,
shl8, shl9);

    finalize();

    delete[] A;
    delete[] F1;
    delete[] BN1W;
    delete[] BN1B;
    delete[] F2;
    delete[] BN2W;
    delete[] BN2B;
    delete[] F3;
    delete[] BN3W;
    delete[] BN3B;

    delete[] C;
    // delete[] C_layerwise;
    // delete[] C_seedot;
}
// */

// /*
typedef int16_t TypeA;

void test_NormaliseL2() {
  std::random_device rand_div;
  std::mt19937 generator(rand_div());
  // std::mt19937 generator(0);
  int32_t N = 1;
  int32_t H = 30;
  int32_t W = 40;
  int32_t C = 32;
  int32_t scaleA = -12;
  int32_t shrA = 8;
  int32_t bwA = 16;
  TypeA *A = new TypeA[N * H * W * C];
  TypeA *calcB = new TypeA[N * H * W * C];
  TypeA *calcCorrectB = new TypeA[N * H * W * C];
  ifstream infile("norminfile");
  for (int i = 0; i < N * H * W * C; i++) {
    cin >> A[i];
    infile >> calcB[i];
    infile >> calcB[i];
    infile >> calcCorrectB[i];
    // A[i] = generator();
  }
  TypeA *B = new TypeA[N * H * W * C];

  initialize();

  NormaliseL2<TypeA>(A, B, N, H, W, C, scaleA, shrA);

  finalize();

  delete[] A;
  delete[] B;
}
// */

/*
typedef int32_t TypeA;

void test_exp() {
    std::random_device rand_div;
    std::mt19937 generator(rand_div());
    // std::mt19937 generator(0);
    int32_t I = 1;
    int32_t J = 16;
    int32_t scale_in = 1LL << 26;
    int32_t scale_out = 1LL << 26;
    int32_t bwA = sizeof(TypeA)*8;
    TypeA* A = new TypeA[I*J];
    for (int i = 0; i < I*J; i++) {
        A[i] = (party == 1 ? (generator() | (1LL << (bwA-1))) : 0);
    }
    TypeA* B = new TypeA[I*J];

    initialize();

    Exp<TypeA>(A, I, J, scale_in, scale_out, B);

    finalize();

    delete[] A;
    delete[] B;
}
// */

/*
typedef int32_t TypeA;

void test_div() {
    std::random_device rand_div;
    std::mt19937 generator(rand_div());
    // std::mt19937 generator(0);
    int32_t I = 1;
    int32_t J = 16;
    int32_t scale_in = 1LL << 26;
    int32_t scale_out = 1LL << 26;
    int32_t bwA = sizeof(TypeA)*8;
    TypeA* A = new TypeA[I*J];
    TypeA* B = new TypeA[I*J];
    for (int i = 0; i < I*J; i++) {
        A[i] = generator();
        B[i] = (party == 1 ? ((generator() & (scale_in - 1)) | scale_in) : 0);
    }
    TypeA* C = new TypeA[I*J];

    initialize();

    Div<TypeA>(A, B, I, J, scale_in, scale_in, scale_out, C);

    finalize();

    delete[] A;
    delete[] B;
    delete[] C;
}
// */

/*
typedef int16_t TypeA;

void test_sigmoid() {
    std::random_device rand_div;
    std::mt19937 generator(rand_div());
    // std::mt19937 generator(0);
    int32_t I = 1131;
    int32_t J = 16;
    int32_t scale_in = 1LL << 11;
    int32_t scale_out = 1LL << 14;
    TypeA* A = new TypeA[I*J];
    for (int i = 0; i < I*J; i++) {
        A[i] = generator();
    }
    TypeA* B = new TypeA[I*J];

    initialize();

    Sigmoid<TypeA>(A, I, J, 0, 0, 0, scale_in, scale_out, B);

    finalize();

    delete[] A;
    delete[] B;
}
// */

int main(int argc, char **argv) {
  if (argc >= 2) {
    party = atoi(argv[1]);
  }
  if (argc >= 3) {
    address = argv[2];
  }
  if (argc >= 4) {
    port = atoi(argv[3]);
  }
  if (argc >= 5) {
    num_threads = atoi(argv[4]);
  }
  assert(party == 1 || party == 2);

  cout << "Party: " << party << endl;

  // test_matmul();
  // test_convolution();
  // test_BNorm();
  // test_MBConv();
  test_NormaliseL2();
  // test_sigmoid();
  // test_exp();
  // test_div();
}
