#ifndef EMP_SEMIHONEST_H__
#define EMP_SEMIHONEST_H__
#include "GC/sh_eva.h"
#include "GC/sh_gen.h"

namespace sci {

template <typename IO = NetIO>
inline SemiHonestParty<IO> *setup_semi_honest(IO *io, int party,
                                              int batch_size = 1024 * 16) {
  if (party == ALICE) {
    HalfGateGen<IO> *t = new HalfGateGen<IO>(io);
    circ_exec = t;
    prot_exec = new SemiHonestGen<IO>(io, t);
  } else {
    HalfGateEva<IO> *t = new HalfGateEva<IO>(io);
    circ_exec = t;
    prot_exec = new SemiHonestEva<IO>(io, t);
  }
  static_cast<SemiHonestParty<IO> *>(prot_exec)->set_batch_size(batch_size);
  return (SemiHonestParty<IO> *)prot_exec;
}
} // namespace sci
#endif
