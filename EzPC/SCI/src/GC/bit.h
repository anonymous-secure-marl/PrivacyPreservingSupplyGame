#ifndef EMP_BIT_H__
#define EMP_BIT_H__
#include "GC/circuit_execution.h"
#include "GC/protocol_execution.h"
#include "GC/swappable.h"
#include "utils/block.h"
#include "utils/utils.h"

namespace sci {
class Bit : public Swappable<Bit> {
public:
  block128 bit;

  Bit(bool _b = false, int party = PUBLIC);
  Bit(const block128 &a) { memcpy(&bit, &a, sizeof(block128)); }

  template <typename O = bool> O reveal(int party = PUBLIC) const;

  Bit operator!=(const Bit &rhs) const;
  Bit operator==(const Bit &rhs) const;
  Bit operator&(const Bit &rhs) const;
  Bit operator|(const Bit &rhs) const;
  Bit operator!() const;

  // swappable
  Bit select(const Bit &select, const Bit &new_v) const;
  Bit operator^(const Bit &rhs) const;

  // batcher
  template <typename... Args> static size_t bool_size(Args &&...args) {
    return 1;
  }

  static void bool_data(bool *b, bool data) { b[0] = data; }

  Bit(size_t size, const block128 *a) { memcpy(&bit, a, sizeof(block128)); }
};

#include "GC/bit.hpp"
} // namespace sci
#endif
