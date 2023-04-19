#include "LinearOT/linear-ot.h"
#include "utils/emp-tool.h"
#include <iostream>

using namespace sci;
using namespace std;

int dim1 = 100;
int dim2 = 1000;
int dim3 = 100;
int bwA = 32;
int bwB = 1;
int bwC = 32;

uint64_t maskA = (bwA == 64 ? -1 : ((1ULL << bwA) - 1));
uint64_t maskB = (bwB == 64 ? -1 : ((1ULL << bwB) - 1));
uint64_t maskC = (bwC == 64 ? -1 : ((1ULL << bwC) - 1));

int main(int argc, char **argv) {
  int party, port = 32000;
  party = atoi(argv[1]);

  IOPack *iopack = new IOPack(party, port, "127.0.0.1");
  OTPack *otpack = new OTPack(iopack, party);

  LinearOT prod(party, iopack, otpack);

  PRG128 prg;

  uint64_t dim = dim1 * dim3;

  uint64_t *inA = new uint64_t[dim1 * dim2];
  uint64_t *inB = new uint64_t[dim2 * dim3];
  uint64_t *outC = new uint64_t[dim];

  prg.random_data(inA, dim1 * dim2 * sizeof(uint64_t));
  prg.random_data(inB, dim2 * dim3 * sizeof(uint64_t));

  for (int i = 0; i < dim1 * dim2; i++) {
    inA[i] &= maskA;
  }
  for (int i = 0; i < dim2 * dim3; i++) {
    inB[i] &= maskB;
  }
  for (int i = 0; i < dim; i++) {
    outC[i] = 0;
  }

  uint64_t comm_start = iopack->get_comm();

  prod.matmul_multiplexer(dim1, dim2, dim3, inA, inB, outC, bwA, bwB, bwC, true,
                          MultMode::None);

  uint64_t comm_end = iopack->get_comm();
  cout << "Bytes Sent: " << (comm_end - comm_start) << endl;

  if (party == ALICE) {
    iopack->io->send_data(inA, dim1 * dim2 * sizeof(uint64_t));
    iopack->io->send_data(inB, dim2 * dim3 * sizeof(uint64_t));
    iopack->io->send_data(outC, dim * sizeof(uint64_t));
  } else { // party == BOB
    uint64_t *inA0 = new uint64_t[dim1 * dim2];
    uint64_t *inB0 = new uint64_t[dim2 * dim3];
    uint64_t *outC0 = new uint64_t[dim];
    iopack->io->recv_data(inA0, dim1 * dim2 * sizeof(uint64_t));
    iopack->io->recv_data(inB0, dim2 * dim3 * sizeof(uint64_t));
    iopack->io->recv_data(outC0, dim * sizeof(uint64_t));

    for (int i = 0; i < dim1 * dim2; i++) {
      inA0[i] = (inA0[i] + inA[i]) & maskA;
      // cout << "inA[" << i << "]: " << inA0[i] << endl;
    }
    for (int i = 0; i < dim2 * dim3; i++) {
      inB0[i] = (inB0[i] + inB[i]) & maskB;
      // cout << "inB[" << i << "]: " << inB0[i] << endl;
    }

    uint64_t *res = new uint64_t[dim];
    prod.matmul_cleartext(dim1, dim2, dim3, inA0, inB0, res);

    for (int i = 0; i < dim; i++) {
      // cout << (res[i] & maskC) << "\t" << ((outC[i] + outC0[i]) & maskC) <<
      // endl;
      assert((res[i] & maskC) == ((outC[i] + outC0[i]) & maskC));
    }
    cout << "Tests Passed" << endl;

    delete[] inA0;
    delete[] inB0;
    delete[] outC0;
    delete[] res;
  }
}
