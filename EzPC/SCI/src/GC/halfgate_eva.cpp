#include "GC/halfgate_eva.h"

using namespace sci;

block128 sci::halfgates_eval(block128 A, block128 B, const block128 *table,
                             MITCCRH<8> *mitccrh) {
  block128 HA, HB, W;
  int sa, sb;

  sa = getLSB(A);
  sb = getLSB(B);

  block128 H[2];
  H[0] = A;
  H[1] = B;
  mitccrh->hash<2, 1>(H);
  HA = H[0];
  HB = H[1];

  W = HA ^ HB;
  W = W ^ (select_mask[sa] & table[0]);
  W = W ^ (select_mask[sb] & table[1]);
  W = W ^ (select_mask[sb] & A);
  return W;
}
