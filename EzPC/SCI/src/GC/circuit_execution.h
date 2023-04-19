#ifndef EMP_CIRCUIT_EXECUTION_H__
#define EMP_CIRCUIT_EXECUTION_H__
#include "utils/block.h"
#include "utils/constants.h"

namespace sci {

/* Circuit Pipelining
 * [REF] Implementation of "Faster Secure Two-Party Computation Using Garbled
 * Circuit" https://www.usenix.org/legacy/event/sec11/tech/full_papers/Huang.pdf
 */
class CircuitExecution {
public:
  /*
  #ifndef THREADING
          // static CircuitExecution * circ_exec;
  #else
          static __thread CircuitExecution * circ_exec;
  #endif
  */
  virtual block128 and_gate(const block128 &in1, const block128 &in2) = 0;
  virtual block128 xor_gate(const block128 &in1, const block128 &in2) = 0;
  virtual block128 not_gate(const block128 &in1) = 0;
  virtual block128 public_label(bool b) = 0;
  virtual size_t num_and() { return -1; }
  virtual ~CircuitExecution() {}
};
enum RTCktOpt { on, off };
} // namespace sci

// extern sci::CircuitExecution* circ_exec;
thread_local extern sci::CircuitExecution *circ_exec;
#endif
