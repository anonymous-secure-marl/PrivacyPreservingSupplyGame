#include "OT/emp-ot.h"
#include "utils/emp-tool.h"
#include <iostream>
using namespace sci;
using namespace std;

int num_ot = 1 << 16;
int hashes_per_ot = 4;
int l = 32;

double test_batched_ot(IOPack *iopack, int party) {
  uint64_t *data = new uint64_t[2 * num_ot * hashes_per_ot];
  uint64_t *r = new uint64_t[num_ot * hashes_per_ot];
  PRG128 prg(fix_key);
  uint8_t *b = new uint8_t[num_ot];
  prg.random_data(b, num_ot);
  uint64_t mask = (l == 64 ? -1 : ((1ULL << l) - 1));

  prg.random_data(data, 2 * num_ot * hashes_per_ot * sizeof(uint64_t));
  for (int i = 0; i < num_ot; i++) {
    for (int j = 0; j < hashes_per_ot; j++) {
      for (int k = 0; k < 2; k++) {
        data[2 * (i * hashes_per_ot + j) + k] &= mask;
      }
    }
    b[i] = b[i] & 1;
  }

  iopack->io->sync();
  auto start = clock_start();
  OTPack *otpack = new OTPack(iopack, party);
  if (party == ALICE)
    otpack->iknp_straight->send_batched_got(data, num_ot, l, hashes_per_ot);
  else
    otpack->iknp_straight->recv_batched_got(r, b, num_ot, l, hashes_per_ot);

  iopack->io->flush();
  long long t = time_from(start);
  if (party == ALICE) {
    iopack->io->recv_data(r, num_ot * hashes_per_ot * sizeof(uint64_t));
    for (int i = 0; i < num_ot; i++) {
      for (int j = 0; j < hashes_per_ot; j++) {
        int idx = i * hashes_per_ot + j;
        // cout << int(b[i]) << "\t" << data[idx][0] << "\t" << data[idx][1] <<
        // "\t" << r[idx] << "\t" << (data[idx][b[i]] & mask) << endl;
        assert(r[idx] == (data[2 * idx + b[i]] & mask));
      }
    }
    std::cout << "Tests passed.\n";
  } else {
    iopack->io->send_data(r, num_ot * hashes_per_ot * sizeof(uint64_t));
  }
  delete otpack;
  delete[] data;
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
  address = "127.0.0.1";

  IOPack *iopack = new IOPack(party, port, address);

  cout << "Batched OT Total (l=" << l << ")\t"
       << double(num_ot) / test_batched_ot(iopack, party) * 1e6 << " OTps"
       << endl;
  return 0;
}
