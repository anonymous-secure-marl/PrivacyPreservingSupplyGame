#include "test_ot.h"
#include <iostream>
using namespace std;

int N = 4, l = 1, l_iknp = 1;
int batch_size = 1;
string address = "127.0.0.1";

int main(int argc, char **argv) {
  int length = 1, port = 32000, party = 1;
  ArgMapping amap;
  amap.arg("r", party, "Role of party: ALICE = 1; BOB = 2");
  amap.arg("p", port, "Port Number");
  amap.arg("N", length, "#OTs");
  amap.arg("ip", address, "Server IP address");
  amap.parse(argc, argv);

  NetIO *io = new NetIO(party == 1 ? nullptr : address.c_str(), port);

  cout << "SplitKKOT Total (N=" << N << ",l=" << l << ")\t"
       << double(length) /
              test_split_KKOT<NetIO>(io, party, length, batch_size, N, l) * 1e6
       << " OTps" << endl;
  // cout <<"SplitKKOT Online (N=" << N << ",l=" << l << ")\t"
  // <<double(length)/test_split_KKOT<NetIO>(io, party, length, batch_size, N,
  // l, true)*1e6
  // <<" OTps"<<endl;
  // cout <<"NPOT\t"<<1000.0/test_ot<block256, NetIO, OTNP>(io, party,
  // 1000)*1e6<<" OTps"<<endl;
  cout << "SplitIKNP [64] (l = " << l_iknp << ")\t"
       << double(length) /
              test_split_iknp<uint64_t, block128, NetIO, IKNP>(
                  io, party, length, l_iknp, batch_size) *
              1e6
       << " OTps" << endl;
  // cout <<"SplitIKNP [64] Online (l =
  // "<<l_iknp<<")\t"<<double(length)/test_split_iknp<uint64_t, block128, NetIO,
  // IKNP>(io, party, length, l_iknp, batch_size, true)*1e6<<" OTps"<<endl;
  cout << "SplitIKNP [8] (l = " << l << ")\t"
       << double(length) /
              test_split_iknp<uint8_t, block128, NetIO, IKNP>(io, party, length,
                                                              l, batch_size) *
              1e6
       << " OTps" << endl;
  // cout <<"SplitIKNP [8] Online (l =
  // "<<l<<")\t"<<double(length)/test_split_iknp<uint8_t, block128, NetIO,
  // IKNP>(io, party, length, l, batch_size, true)*1e6<<" OTps"<<endl; cout
  // <<"IKNP (l=" << l << ")\t"<<double(length)/test_ot_l<block128, NetIO,
  // IKNP>(io, party, length, l)*1e6<<" OTps"<<endl; cout <<"KKOT (N=" << N <<
  // ")\t"<<double(length)/test_1ooNot<block128, NetIO, KKOT>(io, party, length,
  // N)*1e6<<" OTps"<<endl; cout <<"KKOT (N=" << N << ",l=" << l <<
  // ")\t"<<double(length)/test_1ooNot_l<block128, NetIO, KKOT>(io, party,
  // length, N, l)*1e6<<" OTps"<<endl; cout
  // <<"IKNP\t"<<double(length)/test_ot<block128, NetIO, IKNP>(io, party,
  // length)*1e6<<" OTps"<<endl; cout <<"OTIdeal (N=" << N << ",l=" << l <<
  // ")\t"<<double(length)/test_1ooNot_l<block128, NetIO, OTIdeal>(io, party,
  // length, N, l)*1e6<<" OTps"<<endl; cout <<"IKNP
  // COT\t"<<double(length)/test_cot<block128, NetIO, IKNP>(io, party,
  // length)*1e6<<" OTps"<<endl; cout <<"IKNP
  // ROT\t"<<double(length)/test_rot<block128, NetIO, IKNP>(io, party,
  // length)*1e6<<" OTps"<<endl;
  delete io;
}
