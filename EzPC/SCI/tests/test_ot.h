#include "OT/emp-ot.h"
#include <iostream>
#include <utils/emp-tool.h>
using namespace sci;

template <typename block, typename IO, template <typename> class T>
double test_ot(IO *io, int party, int length) {
  size_t align = sizeof(block);
  block *b0 = (block *)_mm_malloc(length * align, align);
  block *b1 = (block *)_mm_malloc(length * align, align);
  block *r = (block *)_mm_malloc(length * align, align);
  PRG128 prg(fix_key);
  prg.random_block(b0, length);
  prg.random_block(b1, length);
  bool *b = new bool[length];
  prg.random_bool(b, length);

  io->sync();
  auto start = clock_start();
  T<IO> *ot = new T<IO>(io);
  // T<IO> *ot = static_cast<T<IO>*>(std::aligned_alloc(align, sizeof(T<IO>)));
  if (party == ALICE) {
    ot->send(b0, b1, length);
  } else {
    ot->recv(r, b, length);
  }
  io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    for (int i = 0; i < length; ++i) {
      // std::cout << b[i] << " ";
      // print(b0[i], " ");
      // print(b1[i], " ");
      // print(r[i]);
      if (b[i])
        assert(block_cmp(&r[i], &b1[i], 1));
      else
        assert(block_cmp(&r[i], &b0[i], 1));
    }
  }
  std::cout << "Tests passed.\n";
  delete ot;
  _mm_free(b0);
  _mm_free(b1);
  _mm_free(r);
  // delete[] b0;
  // delete[] b1;
  // delete[] r;
  delete[] b;
  return t;
}

template <typename block, typename IO, template <typename> class T>
double test_ot_l(IO *io, int party, int length, int l = 1) {
  uint8_t **m = new uint8_t *[length]; // length X 2
  for (int i = 0; i < length; i++)
    m[i] = new uint8_t[2];
  // block *b0 = (block*) _mm_malloc(length * align, align);
  // block *b1 = (block*) _mm_malloc(length * align, align);
  // block *r = (block*) _mm_malloc(length * align, align);
  uint8_t *r = new uint8_t[length];
  PRG128 prg(fix_key);
  for (int i = 0; i < length; i++)
    prg.random_data(m[i], 2);
  uint8_t *b = new uint8_t[length];
  prg.random_data(b, length);
  uint8_t mask;
  if (l == 8)
    mask = -1;
  else
    mask = (1 << l) - 1;
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < 2; j++)
      m[i][j] = m[i][j] & mask;
    b[i] = b[i] & 1;
  }

  io->sync();
  auto start = clock_start();
  T<IO> *ot = new T<IO>(io);
  // T<IO> *ot = static_cast<T<IO>*>(std::aligned_alloc(align, sizeof(T<IO>)));
  if (party == ALICE) {
    ot->send(m, length, l);
  } else {
    ot->recv(r, b, length, l);
  }
  io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    for (int i = 0; i < length; ++i) {
      // std::cout << unsigned(b[i]) << " ";
      // for(int k = 0; k < 2; k++) {
      //     print(m[i][k], " ");
      // }
      // print(r[i]);
      assert(r[i] == m[i][b[i]]);
    }
  }
  std::cout << "Tests passed.\n";
  delete ot;
  for (int i = 0; i < length; i++)
    delete[] m[i];
  delete[] m;
  delete[] r;
  delete[] b;
  return t;
}

template <typename block, typename IO, template <typename> class T>
double test_1ooNot(IO *io, int party, int length, int N = 2) {
  // size_t align = sizeof(block);
  block **m = new block *[length]; // length X N
  for (int i = 0; i < length; i++)
    m[i] = new block[N];
  // block *b0 = (block*) _mm_malloc(length * align, align);
  // block *b1 = (block*) _mm_malloc(length * align, align);
  // block *r = (block*) _mm_malloc(length * align, align);
  block *r = new block[length];
  PRG128 prg(fix_key);
  for (int i = 0; i < length; i++)
    prg.random_block(m[i], N);
  uint8_t *b = new uint8_t[length];
  prg.random_data(b, length);
  for (int i = 0; i < length; i++) {
    b[i] = b[i] & (N - 1); // TODO: Will not work for N = 256
  }

  io->sync();
  auto start = clock_start();
  T<IO> *ot = new T<IO>(io);
  // T<IO> *ot = static_cast<T<IO>*>(std::aligned_alloc(align, sizeof(T<IO>)));
  if (party == ALICE) {
    ot->send(m, length, N);
  } else {
    ot->recv(r, b, length, N);
  }
  io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    for (int i = 0; i < length; ++i) {
      // std::cout << unsigned(b[i]) << " ";
      // for(int k = 0; k < N; k++) {
      //     print(m[i][k], " ");
      // }
      // print(r[i]);
      assert(block_cmp(&r[i], &m[i][b[i]], 1));
    }
  }
  std::cout << "Tests passed.\n";
  delete ot;
  for (int i = 0; i < length; i++)
    delete[] m[i];
  delete[] m;
  delete[] r;
  delete[] b;
  return t;
}

template <typename block, typename IO, template <typename> class T>
double test_1ooNot_l(IO *io, int party, int length, int N = 2, int l = 1) {
  // size_t align = sizeof(block);
  uint8_t **m = new uint8_t *[length]; // length X N
  for (int i = 0; i < length; i++)
    m[i] = new uint8_t[N];
  // block *b0 = (block*) _mm_malloc(length * align, align);
  // block *b1 = (block*) _mm_malloc(length * align, align);
  // block *r = (block*) _mm_malloc(length * align, align);
  uint8_t *r = new uint8_t[length];
  PRG128 prg(fix_key);
  for (int i = 0; i < length; i++)
    prg.random_data(m[i], N);
  uint8_t *b = new uint8_t[length];
  prg.random_data(b, length);
  uint8_t mask;
  if (l == 8)
    mask = -1;
  else
    mask = (1 << l) - 1;
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < N; j++)
      m[i][j] = m[i][j] & mask;
    b[i] = b[i] & (N - 1); // TODO: Will not work for N = 256
  }

  io->sync();
  auto start = clock_start();
  T<IO> *ot = new T<IO>(io);
  // T<IO> *ot = static_cast<T<IO>*>(std::aligned_alloc(align, sizeof(T<IO>)));
  if (party == ALICE) {
    ot->send(m, length, N, l);
  } else {
    ot->recv(r, b, length, N, l);
  }
  io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    for (int i = 0; i < length; ++i) {
      // std::cout << unsigned(b[i]) << " ";
      // for(int k = 0; k < N; k++) {
      //     print(m[i][k], " ");
      // }
      // print(r[i]);
      assert(r[i] == m[i][b[i]]);
    }
  }
  std::cout << "Tests passed.\n";
  delete ot;
  for (int i = 0; i < length; i++)
    delete[] m[i];
  delete[] m;
  delete[] r;
  delete[] b;
  return t;
}

template <typename IO>
double test_KKOT_l_small(IO *io, int party, int length, int batch_size,
                         int N = 2, int l = 1) {
  uint8_t **m = new uint8_t *[length]; // length X N
  for (int i = 0; i < length; i++)
    m[i] = new uint8_t[N];
  uint8_t *r = new uint8_t[length];
  PRG128 prg(fix_key);
  for (int i = 0; i < length; i++)
    prg.random_data(m[i], N);
  uint8_t *b = new uint8_t[length];
  prg.random_data(b, length);
  uint8_t mask;
  if (l == 8)
    mask = -1;
  else
    mask = (1 << l) - 1;
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < N; j++)
      m[i][j] = m[i][j] & mask;
    b[i] = b[i] & (N - 1); // TODO: Will not work for N = 256
  }

  io->sync();
  auto start = clock_start();
  KKOT<IO> *ot = new KKOT<IO>(io);
  for (int i = 0; i < length; i += batch_size) {
    if (party == ALICE) {
      if (batch_size <= length - i) {
        ot->send(m + i, batch_size, N, l);
      } else {
        ot->send(m + i, length - i, N, l);
      }
    } else {
      if (batch_size <= length - i) {
        ot->recv(r + i, b + i, batch_size, N, l);
      } else {
        ot->recv(r + i, b + i, length - i, N, l);
      }
    }
  }
  io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    for (int i = 0; i < length; ++i) {
      // std::cout << unsigned(b[i]) << " ";
      // for(int k = 0; k < N; k++) {
      //     print(m[i][k], " ");
      // }
      // print(r[i]);
      assert(r[i] == m[i][b[i]]);
    }
  }
  // std::cout << "Tests passed.\n";
  delete ot;
  for (int i = 0; i < length; i++)
    delete[] m[i];
  delete[] m;
  delete[] r;
  delete[] b;
  return t;
}

/*************************************************************************
 *                     Offline-Online Split OTs                          *
 *************************************************************************/

template <typename IO>
double test_split_KKOT(IO *io, int party, int length, int batch_size, int N = 2,
                       int l = 1, bool preprocess = false) {
  uint8_t **m = new uint8_t *[length]; // length X N
  for (int i = 0; i < length; i++)
    m[i] = new uint8_t[N];
  uint8_t *r = new uint8_t[length];
  PRG128 prg(fix_key);
  for (int i = 0; i < length; i++)
    prg.random_data(m[i], N);
  uint8_t *b = new uint8_t[length];
  prg.random_data(b, length);
  uint8_t mask;
  if (l == 8)
    mask = -1;
  else
    mask = (1 << l) - 1;
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < N; j++)
      m[i][j] = m[i][j] & mask;
    b[i] = b[i] & (N - 1); // TODO: Will not work for N = 256
  }

  SplitKKOT<IO> *ot = new SplitKKOT<IO>(party, io, N);
  if (preprocess) {
    ot->set_precomp_batch_size(length);
    ot->preprocess();
  }
  io->sync();
  auto start = clock_start();
  for (int i = 0; i < length; i += batch_size) {
    if (party == ALICE) {
      if (batch_size <= length - i) {
        ot->send(m + i, batch_size, l);
      } else {
        ot->send(m + i, length - i, l);
      }
    } else {
      if (batch_size <= length - i) {
        ot->recv(r + i, b + i, batch_size, l);
      } else {
        ot->recv(r + i, b + i, length - i, l);
      }
    }
  }
  io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    for (int i = 0; i < length; ++i) {
      // std::cout << unsigned(b[i]) << " ";
      // for(int k = 0; k < N; k++) {
      //     print(m[i][k], " ");
      // }
      // print(r[i]);
      assert(r[i] == m[i][b[i]]);
    }
  }
  std::cout << "Tests passed.\n";
  delete ot;
  for (int i = 0; i < length; i++)
    delete[] m[i];
  delete[] m;
  delete[] r;
  delete[] b;
  return t;
}

template <typename basetype, typename block, typename IO,
          template <typename> class T>
double test_split_iknp(IO *io, int party, int length, int l, int batch_size,
                       bool preprocess = false) {
  basetype **m = new basetype *[length]; // length X 2
  for (int i = 0; i < length; i++)
    m[i] = new basetype[2];
  basetype *r = new basetype[length];
  PRG128 prg(fix_key);
  for (int i = 0; i < length; i++)
    prg.random_data(m[i], 2 * sizeof(basetype));
  uint8_t *b = new uint8_t[length];
  prg.random_data(b, length);
  basetype mask = (1 << l) - 1;
  if (8 * sizeof(basetype) == 64) {
    mask = (basetype)((1ULL << l) - 1ULL);
  }
  if (8 * sizeof(basetype) == l) {
    if (l == 64) {
      mask = (basetype)(-1ULL);
    } else {
      mask = (basetype)(-1);
    }
  }
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < 2; j++)
      m[i][j] = m[i][j] & mask;
    b[i] = b[i] & 1;
  }

  SplitIKNP<IO> *ot = new SplitIKNP<IO>(party, io);
  if (preprocess) {
    ot->set_precomp_batch_size(length);
    ot->preprocess();
  }

  io->sync();
  auto start = clock_start();
  for (int i = 0; i < length; i += batch_size) {
    if (party == ALICE) {
      if (batch_size <= length - i) {
        ot->send(m + i, batch_size, l);
      } else {
        ot->send(m + i, length - i, l);
      }
    } else {
      if (batch_size <= length - i) {
        ot->recv(r + i, b + i, batch_size, l);
      } else {
        ot->recv(r + i, b + i, length - i, l);
      }
    }
  }
  io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    for (int i = 0; i < length; ++i) {
      // std::cout << unsigned(b[i]) << " ";
      // for(int k = 0; k < 2; k++) {
      //     print(m[i][k], " ");
      // }
      // print(r[i]);
      assert(r[i] == m[i][b[i]]);
    }
  }
  std::cout << "Tests passed.\n";
  delete ot;
  for (int i = 0; i < length; i++)
    delete[] m[i];
  delete[] m;
  delete[] r;
  delete[] b;
  return t;
}
