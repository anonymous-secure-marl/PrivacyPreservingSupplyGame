#include "LinearOT/linear-ot.h"
#include "LinearOT/linear-uniform.h"
#include "utils/emp-tool.h"
#include <iostream>

using namespace sci;
using namespace std;

int dim1 = 100;
int dim2 = 1000;
int dim3 = 100;
int bwA = 26;
int bwB = 26;
int bwC = 26;
bool uniform = false;

uint64_t maskA = (bwA == 64 ? -1 : ((1ULL << bwA) - 1));
uint64_t maskB = (bwB == 64 ? -1 : ((1ULL << bwB) - 1));
uint64_t maskC = (bwC == 64 ? -1 : ((1ULL << bwC) - 1));

int main(int argc, char **argv) {
  int party, port = 32000;
  party = atoi(argv[1]);
  if (argc > 2) {
    uniform = atoi(argv[2]);
  }

  IOPack *iopack = new IOPack(party, port, "127.0.0.1");
  OTPack *otpack = new OTPack(iopack, party);

  LinearOT prod(party, iopack, otpack);
  IKNP<NetIO> *iknpOT = new IKNP<NetIO>(iopack->io);
  IKNP<NetIO> *iknpOTRoleReversed = new IKNP<NetIO>(iopack->io);
  MatMulUniform<NetIO, uint64_t, IKNP<NetIO>> mult(party, bwA, iopack->io,
                                                   iknpOT, iknpOTRoleReversed);

  PRG128 prg;

  uint64_t *inA = new uint64_t[dim1 * dim2];
  uint64_t *inB = new uint64_t[dim2 * dim3];
  uint64_t *outC = new uint64_t[dim1 * dim3];

  prg.random_data(inA, dim1 * dim2 * sizeof(uint64_t));
  prg.random_data(inB, dim2 * dim3 * sizeof(uint64_t));

  for (int i = 0; i < dim1 * dim2; i++) {
    inA[i] &= maskA;
  }
  for (int i = 0; i < dim2 * dim3; i++) {
    inB[i] &= maskB;
  }
  for (int i = 0; i < dim1 * dim3; i++) {
    outC[i] = 0;
  }

  uint64_t comm_start = iopack->get_comm();

  INIT_TIMER;
  START_TIMER;
  cout << "Started Timer" << endl;
  if (!uniform) {
    prod.matmul_cross_terms(dim1, dim2, dim3, inA, inB, outC, bwA, bwB, bwC,
                            true, MultMode::Alice_has_A);
  } else {
    if (party == ALICE) {
      mult.funcOTSenderInputA(dim1, dim2, dim3, inA, outC, iknpOT);
    } else {
      mult.funcOTReceiverInputB(dim1, dim2, dim3, inB, outC, iknpOT);
    }
  }
  STOP_TIMER(string("Total time for matmul ") +
             (uniform ? "(uniform): " : "(ot): "));

  uint64_t comm_end = iopack->get_comm();
  cout << "Bytes Sent: " << (comm_end - comm_start) << endl;

  if (party == ALICE) {
    iopack->io->send_data(inA, dim1 * dim2 * sizeof(uint64_t));
    iopack->io->send_data(inB, dim2 * dim3 * sizeof(uint64_t));
    iopack->io->send_data(outC, dim1 * dim3 * sizeof(uint64_t));
  } else { // party == BOB
    uint64_t *inA0 = new uint64_t[dim1 * dim2];
    uint64_t *inB0 = new uint64_t[dim2 * dim3];
    uint64_t *outC0 = new uint64_t[dim1 * dim3];
    iopack->io->recv_data(inA0, dim1 * dim2 * sizeof(uint64_t));
    iopack->io->recv_data(inB0, dim2 * dim3 * sizeof(uint64_t));
    iopack->io->recv_data(outC0, dim1 * dim3 * sizeof(uint64_t));

    uint64_t *res0 = new uint64_t[dim1 * dim3];
    uint64_t *res1 = new uint64_t[dim1 * dim3];
    prod.matmul_cleartext(dim1, dim2, dim3, inA, inB0, res0);
    prod.matmul_cleartext(dim1, dim2, dim3, inA0, inB, res1);

    for (int i = 0; i < dim1 * dim3; i++) {
      // cout << ((res0[i] + res1[i]) & maskC)
      //     << "\t" << ((outC[i] + outC0[i]) & maskC) << endl;
      // assert(((res0[i] + res1[i]) & maskC) == ((outC[i] + outC0[i]) &
      // maskC));
      assert(((res1[i]) & maskC) == ((outC[i] + outC0[i]) & maskC));
    }
    cout << "Tests Passed" << endl;

    delete[] inA0;
    delete[] inB0;
    delete[] outC0;
    delete[] res0;
    delete[] res1;
  }
}
