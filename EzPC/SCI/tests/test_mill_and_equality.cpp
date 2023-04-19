#include "Millionaire/equality.h"
#include "Millionaire/millionaire_with_equality.h"
#include <iostream>

using namespace sci;
using namespace std;

int num_cmps = 1 << 8;
int l = 6;
int b = 4;
bool do_gt = true;

uint64_t mask = (l == 64 ? -1 : ((1ULL << l) - 1));

// vars
int party, port = 32000;
IOPack *iopack;
OTPack *otpack;
MillionaireProtocol *mill;
MillionaireWithEquality *mill_and_eq;
Equality *eq;
PRG128 prg;

void test_eq() {
  uint64_t *in = new uint64_t[num_cmps];
  uint8_t *out_eq = new uint8_t[num_cmps];

  prg.random_data(in, num_cmps * sizeof(uint64_t));
  prg.random_data(out_eq, num_cmps * sizeof(uint8_t));

  for (int i = 0; i < num_cmps; i++) {
    in[i] &= mask;
    out_eq[i] = 0;
  }

  eq->check_equality(out_eq, in, num_cmps, l, b);

  if (party == ALICE) {
    uint64_t *in_bob = new uint64_t[num_cmps];
    uint8_t *out_bob_eq = new uint8_t[num_cmps];
    uint8_t *actual_eq = new uint8_t[num_cmps];

    iopack->io->recv_data(in_bob, sizeof(uint64_t) * num_cmps);
    iopack->io->recv_data(out_bob_eq, sizeof(uint8_t) * num_cmps);

    for (int i = 0; i < num_cmps; i++) {
      out_eq[i] = (out_eq[i] ^ out_bob_eq[i]);
    }
    cout << "Testing for correctness..." << endl;
    cout << "Testing Equality..." << endl;
    for (int i = 0; i < num_cmps; i++) {
      actual_eq[i] = ((in[i] & mask) == (in_bob[i] & mask) ? 1 : 0);
      // cout<<+actual[i]<<" "<<+out[i]<<endl;
      assert(actual_eq[i] == out_eq[i]);
    }
    cout << "Correct!" << endl;
  } else { // BOB
    iopack->io->send_data(in, sizeof(uint64_t) * num_cmps);
    iopack->io->send_data(out_eq, sizeof(uint8_t) * num_cmps);
  }
}

void mill_eq() {
  uint64_t *in = new uint64_t[num_cmps];
  uint8_t *out_cmp = new uint8_t[num_cmps];
  uint8_t *out_eq = new uint8_t[num_cmps];

  prg.random_data(in, num_cmps * sizeof(uint64_t));
  prg.random_data(out_cmp, num_cmps * sizeof(uint8_t));
  prg.random_data(out_eq, num_cmps * sizeof(uint8_t));

  for (int i = 0; i < num_cmps; i++) {
    in[i] &= mask;
    out_cmp[i] = 0;
    out_eq[i] = 0;
  }

  mill_and_eq->compare_with_eq(out_cmp, out_eq, in, num_cmps, l, do_gt, b);

  if (party == ALICE) {
    uint64_t *in_bob = new uint64_t[num_cmps];
    uint8_t *out_bob_cmp = new uint8_t[num_cmps];
    uint8_t *out_bob_eq = new uint8_t[num_cmps];
    uint8_t *actual_cmp = new uint8_t[num_cmps];
    uint8_t *actual_eq = new uint8_t[num_cmps];

    iopack->io->recv_data(in_bob, sizeof(uint64_t) * num_cmps);
    iopack->io->recv_data(out_bob_cmp, sizeof(uint8_t) * num_cmps);
    iopack->io->recv_data(out_bob_eq, sizeof(uint8_t) * num_cmps);

    for (int i = 0; i < num_cmps; i++) {
      out_cmp[i] = (out_cmp[i] ^ out_bob_cmp[i]);
      out_eq[i] = (out_eq[i] ^ out_bob_eq[i]);
    }
    cout << "Testing for correctness..." << endl;
    cout << "Testing Comparison..." << endl;
    for (int i = 0; i < num_cmps; i++) {
      if (do_gt) {
        actual_cmp[i] = ((in[i] & mask) > (in_bob[i] & mask) ? 1 : 0);
      } else {
        actual_cmp[i] = ((in[i] & mask) < (in_bob[i] & mask) ? 1 : 0);
      }
      assert(actual_cmp[i] == out_cmp[i]);
    }
    cout << "Correct!" << endl;

    cout << "Testing Equality..." << endl;
    for (int i = 0; i < num_cmps; i++) {
      actual_eq[i] = ((in[i] & mask) == (in_bob[i] & mask) ? 1 : 0);
      // cout<<+actual[i]<<" "<<+out[i]<<endl;
      assert(actual_eq[i] == out_eq[i]);
    }
    cout << "Correct!" << endl;
  } else { // BOB
    iopack->io->send_data(in, sizeof(uint64_t) * num_cmps);
    iopack->io->send_data(out_cmp, sizeof(uint8_t) * num_cmps);
    iopack->io->send_data(out_eq, sizeof(uint8_t) * num_cmps);
  }
}

void mill_test() {
  uint64_t *in = new uint64_t[num_cmps];
  uint8_t *out_cmp = new uint8_t[num_cmps];

  prg.random_data(in, num_cmps * sizeof(uint64_t));
  prg.random_data(out_cmp, num_cmps * sizeof(uint8_t));

  for (int i = 0; i < num_cmps; i++) {
    in[i] &= mask;
    out_cmp[i] = 0;
  }

  mill->compare(out_cmp, in, num_cmps, l, do_gt, false, b);

  if (party == ALICE) {
    uint64_t *in_bob = new uint64_t[num_cmps];
    uint8_t *out_bob_cmp = new uint8_t[num_cmps];
    uint8_t *actual_cmp = new uint8_t[num_cmps];

    iopack->io->recv_data(in_bob, sizeof(uint64_t) * num_cmps);
    iopack->io->recv_data(out_bob_cmp, sizeof(uint8_t) * num_cmps);

    for (int i = 0; i < num_cmps; i++) {
      out_cmp[i] = (out_cmp[i] ^ out_bob_cmp[i]);
    }
    cout << "Testing for correctness..." << endl;
    for (int i = 0; i < num_cmps; i++) {
      if (do_gt) {
        actual_cmp[i] = ((in[i] & mask) > (in_bob[i] & mask) ? 1 : 0);
      } else {
        actual_cmp[i] = ((in[i] & mask) < (in_bob[i] & mask) ? 1 : 0);
      }
      assert(actual_cmp[i] == out_cmp[i]);
    }
    cout << "Correct!" << endl;
  } else { // BOB
    iopack->io->send_data(in, sizeof(uint64_t) * num_cmps);
    iopack->io->send_data(out_cmp, sizeof(uint8_t) * num_cmps);
  }
}

int main(int argc, char **argv) {
  ArgMapping amap;
  amap.arg("r", party, "Role of party: ALICE = 1; BOB = 2");
  amap.arg("p", port, "Port Number");
  amap.arg("l", l, "Bitlength of inputs");
  amap.arg("N", num_cmps, "Number of comparisons");
  amap.arg("b", b, "Radix base");

  amap.parse(argc, argv);

  iopack = new IOPack(party, port, "127.0.0.1");
  otpack = new OTPack(iopack, party);
  mill = new MillionaireProtocol(party, iopack, otpack, l, b);
  mill_and_eq = new MillionaireWithEquality(party, iopack, otpack, l, b);
  eq = new Equality(party, iopack, otpack, l, b);
  uint64_t num_rounds;

  num_rounds = iopack->get_rounds();
  INIT_TIMER;
  START_TIMER;
  cout << "<><><><> Millionaire Only <><><><>" << endl;
  mill_test();
  STOP_TIMER("Millionaire runtime");
  num_rounds = iopack->get_rounds() - num_rounds;
  cout << "Num rounds: " << num_rounds << endl;

  num_rounds = iopack->get_rounds();
  START_TIMER;
  cout << "<><><><> Millionaire & Equality <><><><>" << endl;
  mill_eq();
  STOP_TIMER("Millionaire & Equality runtime");
  num_rounds = iopack->get_rounds() - num_rounds;
  cout << "Num rounds: " << num_rounds << endl;

  num_rounds = iopack->get_rounds();
  START_TIMER;
  cout << "<><><><> Equality <><><><>" << endl;
  test_eq();
  STOP_TIMER("Equality runtime");
  num_rounds = iopack->get_rounds() - num_rounds;
  cout << "Num rounds: " << num_rounds << endl;
}
