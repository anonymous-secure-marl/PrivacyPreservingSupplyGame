#include "Millionaire/millionaire.h"
#include "OT/emp-ot.h"
#include "utils/emp-tool.h"
#include <iostream>
using namespace sci;
using namespace std;

int l = 4;

double test_cot(NetIO *io, int party, int length, int l = 1) {
  uint64_t *data = new uint64_t[length];
  uint64_t *corr = new uint64_t[length];
  uint64_t *r = new uint64_t[length];
  PRG128 prg(fix_key);
  prg.random_data(corr, length * 8);
  uint8_t *b = new uint8_t[length];
  prg.random_data(b, length);
  uint64_t mask;
  if (l == 64)
    mask = -1;
  else
    mask = (1ULL << l) - 1;
  for (int i = 0; i < length; i++) {
    corr[i] = corr[i] & mask;
    b[i] = b[i] & 1;
  }

  io->sync();
  auto start = clock_start();
  SplitIKNP<NetIO> *ot = new SplitIKNP<NetIO>(party, io);
  if (party == ALICE) {
    ot->send_cot(data, corr, length, l);
  } else {
    ot->recv_cot(r, (bool *)b, length, l);
  }
  io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    io->recv_data(data, length * sizeof(uint64_t));
    for (int i = 0; i < length; ++i) {
      // cout << int(b[i]) << "\t" << data[i] << "\t" << corr[i] << "\t" << r[i]
      // << "\t" << ((data[i] + b[i]*corr[i]) & mask) << endl;
      assert(r[i] == ((data[i] + b[i] * corr[i]) & mask));
    }
    std::cout << "Tests passed.\n";
  } else {
    io->send_data(data, length * sizeof(uint64_t));
  }
  delete ot;
  delete[] data;
  delete[] corr;
  delete[] r;
  delete[] b;
  return t;
}

int main(int argc, char **argv) {
  int length = 1 << 10, port = 32000, party = 1;
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

  NetIO *io = new NetIO(party == 1 ? nullptr : address.c_str(), port);

  cout << "SplitIKNP Total (l=" << l << ")\t"
       << double(length) / test_cot(io, party, length, l) * 1e6 << " OTps"
       << endl;
  return 0;
}
