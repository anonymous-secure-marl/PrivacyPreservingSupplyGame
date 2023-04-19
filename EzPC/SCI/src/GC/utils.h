#ifndef GC_UTILS_H__
#define GC_UTILS_H__

#include "utils/emp-tool.h"

namespace sci {

inline block128 bool_to_block(const bool *data) {
  return makeBlock128(bool_to_int<uint64_t>(data + 64),
                      bool_to_int<uint64_t>(data));
}

inline void block_to_bool(bool *data, block128 b) {
  uint64_t *ptr = (uint64_t *)(&b);
  int_to_bool<uint64_t>(data, ptr[0], 64);
  int_to_bool<uint64_t>(data + 64, ptr[1], 64);
}

} // namespace sci
#endif // GC_UTILS_H__
