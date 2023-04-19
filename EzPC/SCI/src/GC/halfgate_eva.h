#ifndef EMP_HALFGATE_EVA_H__
#define EMP_HALFGATE_EVA_H__
#include "GC/circuit_execution.h"
#include "GC/mitccrh.h"
#include "utils/utils.h"
#include <iostream>
namespace sci {

block128 halfgates_eval(block128 A, block128 B, const block128 *table,
                        MITCCRH<8> *mitccrh);

template <typename T> class HalfGateEva : public CircuitExecution {
public:
  T *io;
  block128 constant[2];
  MITCCRH<8> mitccrh;
  HalfGateEva(T *io) : io(io) {
    set_delta();
    block128 tmp;
    io->recv_block(&tmp, 1);
    mitccrh.setS(tmp);
  }
  void set_delta() { io->recv_block(constant, 2); }
  block128 public_label(bool b) override { return constant[b]; }
  block128 and_gate(const block128 &a, const block128 &b) override {
    block128 table[2];
    io->recv_block(table, 2);
    return halfgates_eval(a, b, table, &mitccrh);
  }
  block128 xor_gate(const block128 &a, const block128 &b) override {
    return a ^ b;
  }
  block128 not_gate(const block128 &a) override {
    return xor_gate(a, public_label(true));
  }
  size_t num_and() override { return mitccrh.gid; }
};
} // namespace sci
#endif // HALFGATE_EVA_H__
