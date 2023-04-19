#ifndef EMP_PROTOCOL_EXECUTION_H__
#define EMP_PROTOCOL_EXECUTION_H__
#include "utils/block.h"
#include "utils/constants.h"
#include <pthread.h>

namespace sci {
class ProtocolExecution {
public:
  int cur_party;
  /*
  #ifndef THREADING
          // static ProtocolExecution * prot_exec;
  #else
          static __thread ProtocolExecution * prot_exec;
  #endif
  */

  ProtocolExecution(int party = PUBLIC) : cur_party(party) {}
  virtual ~ProtocolExecution() {}
  virtual void feed(block128 *lbls, int party, const bool *b, int nel) = 0;
  virtual void reveal(bool *out, int party, const block128 *lbls, int nel) = 0;
  virtual void finalize() {}
};
} // namespace sci
// extern sci::ProtocolExecution* prot_exec;
thread_local extern sci::ProtocolExecution *prot_exec;
#endif
