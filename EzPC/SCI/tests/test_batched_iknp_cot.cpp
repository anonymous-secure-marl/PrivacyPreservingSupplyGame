#include "OT/emp-ot.h"
#include "utils/emp-tool.h"
#include <iostream>

using namespace sci;
using namespace std;

int length = 4;
int hashes_per_ot = 2;
vector<int> l = {4, 16};

double test_batched_cot(IOPack *iopack, int party, int length, vector<int> l) {
  uint64_t *data = new uint64_t[length * hashes_per_ot];
  uint64_t *corr = new uint64_t[length * hashes_per_ot];
  uint64_t *r = new uint64_t[length * hashes_per_ot];
  PRG128 prg(fix_key);
  prg.random_data(corr, length * hashes_per_ot * 8);
  uint8_t *b = new uint8_t[length];
  prg.random_data(b, length);
  uint64_t mask[l.size()];
  for (int i = 0; i < l.size(); i++) {
    mask[i] = (l[i] == 64 ? -1 : ((1ULL << l[i]) - 1));
  }
  int dim = length / l.size();
  for (int i = 0; i < length; i += dim) {
    int bit_idx = i / dim;
    for (int j = i; j < i + dim and j < length; j++) {
      for (int k = 0; k < hashes_per_ot; k++) {
        corr[j * hashes_per_ot + k] =
            corr[j * hashes_per_ot + k] & mask[bit_idx];
      }
      b[j] = b[j] & 1;
    }
  }

  iopack->io->sync();
  auto start = clock_start();
  OTPack *otpack = new OTPack(iopack, party);
  if (party == ALICE)
    otpack->iknp_straight->send_batched_cot(data, corr, l, length,
                                            hashes_per_ot);
  else
    otpack->iknp_straight->recv_batched_cot(r, (bool *)b, l, length,
                                            hashes_per_ot);

  iopack->io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    iopack->io->recv_data(data, length * hashes_per_ot * sizeof(uint64_t));
    for (int i = 0; i < length; i += dim) {
      int bit_idx = i / dim;
      for (int j = i; j < i + dim and j < length; j++) {
        for (int k = 0; k < hashes_per_ot; k++) {
          int idx = j * hashes_per_ot + k;
          // cout << int(b[j]) << "\t" << data[idx] << "\t" << corr[idx] << "\t"
          // << r[idx] << "\t" << ((data[idx] + b[j]*corr[idx]) & mask[bit_idx])
          // << endl;
          assert(r[idx] == ((data[idx] + b[j] * corr[idx]) & mask[bit_idx]));
        }
      }
    }
    std::cout << "Tests passed.\n";
  } else {
    iopack->io->send_data(data, length * hashes_per_ot * sizeof(uint64_t));
  }
  delete otpack;
  delete[] data;
  delete[] corr;
  delete[] r;
  delete[] b;
  return t;
}

int main(int argc, char **argv) {
  int port = 32000, party = 1;
  string address;
  bool localhost = true;
  assert(argc > 1 &&
         "ERROR: argc = 1, need two argsm party ID {1,2} and port.");
  if (argc >= 2)
    party = atoi(argv[1]);
  if (argc >= 3) {
    port = atoi(argv[2]);
  }
  if (argc >= 4) {
    localhost = atoi(argv[3]);
  }
  address = "127.0.0.1";

  IOPack *iopack = new IOPack(party, port, address);

  cout << "Batched COT Total (l={";
  for (int i = 0; i < l.size(); i++)
    cout << l.at(i) << ",";
  cout << "})\t"
       << double(length) / test_batched_cot(iopack, party, length, l) * 1e6
       << " OTps" << endl;
  return 0;
}
