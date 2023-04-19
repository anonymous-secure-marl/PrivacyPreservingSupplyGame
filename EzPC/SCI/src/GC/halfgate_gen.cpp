#include "GC/halfgate_gen.h"

using namespace sci;

block128 sci::halfgates_garble(block128 LA0, block128 A1, block128 LB0,
                               block128 B1, block128 delta, block128 *table,
                               MITCCRH<8> *mitccrh) {
  bool pa = getLSB(LA0);
  bool pb = getLSB(LB0);
  block128 HLA0, HA1, HLB0, HB1;
  block128 tmp, W0;

  block128 H[4];
  H[0] = LA0;
  H[1] = A1;
  H[2] = LB0;
  H[3] = B1;
  mitccrh->hash<2, 2>(H);
  HLA0 = H[0];
  HA1 = H[1];
  HLB0 = H[2];
  HB1 = H[3];

  table[0] = HLA0 ^ HA1;
  table[0] = table[0] ^ (select_mask[pb] & delta);
  W0 = HLA0;
  W0 = W0 ^ (select_mask[pa] & table[0]);
  tmp = HLB0 ^ HB1;
  table[1] = tmp ^ LA0;
  W0 = W0 ^ HLB0;
  W0 = W0 ^ (select_mask[pb] & tmp);

  return W0;
}
