#include "Millionaire/bit-triple-generator.h"

using namespace std;
using namespace sci;

// TripleGenMethod method = _16KKOT_to_4OT;
TripleGenMethod method = _8KKOT;

int main(int argc, char **argv) {
  int port = 32000, party = 1, num_triples = 1 << 24;
  int offset = 1 << 20;
  bool packed = true;
  if (argc >= 2) {
    party = atoi(argv[1]);
  }
  if (argc >= 3) {
    port = atoi(argv[2]);
  }
  if (argc >= 4) {
    num_triples = atoi(argv[3]);
  }
  IOPack *iopack = new IOPack(party, port, "127.0.0.1");
  OTPack *otpack = new OTPack(iopack, party);
  cout << "Base OTs done" << endl;
  TripleGenerator triple_gen(party, iopack, otpack);
  Triple triples(num_triples, packed, offset);

  // Triple Generation
  triple_gen.generate(party, &triples, method);
  cout << "Beaver Triple generation done" << endl;

  // Triple Verification
  switch (party) {
  case ALICE: {
    iopack->io->send_data(triples.ai, triples.num_bytes);
    iopack->io->send_data(triples.bi, triples.num_bytes);
    iopack->io->send_data(triples.ci, triples.num_bytes);
    break;
  }
  case BOB: {
    uint8_t *a = new uint8_t[triples.num_bytes];
    uint8_t *b = new uint8_t[triples.num_bytes];
    uint8_t *c = new uint8_t[triples.num_bytes];
    iopack->io->recv_data(a, triples.num_bytes);
    iopack->io->recv_data(b, triples.num_bytes);
    iopack->io->recv_data(c, triples.num_bytes);
    if (method == _8KKOT) {
      int offset_;
      if (triples.packed)
        offset_ = triples.offset / 8;
      else
        offset_ = triples.offset;
      for (int i = 0; i < triples.num_bytes; i += 2 * offset_) {
        for (int k = 0; k < offset_; k++) {
          assert(triples.ai[i + k] == triples.ai[i + k + offset_]);
          assert(((a[i + k] ^ triples.ai[i + k]) &
                  (b[i + k] ^ triples.bi[i + k])) ==
                 (c[i + k] ^ triples.ci[i + k]));
          assert(((a[i + k + offset_] ^ triples.ai[i + k + offset_]) &
                  (b[i + k + offset_] ^ triples.bi[i + k + offset_])) ==
                 (c[i + k + offset_] ^ triples.ci[i + k + offset_]));
        }
      }
    } else {
      for (int i = 0; i < triples.num_bytes; i++) {
        assert(((a[i] ^ triples.ai[i]) & (b[i] ^ triples.bi[i])) ==
               (c[i] ^ triples.ci[i]));
      }
    }
    cout << "Triples Verified" << endl;
    break;
  }
  }
  return 0;
}
