#ifndef FLOATING_POINT_MATH_COEFFS_H__
#define FLOATING_POINT_MATH_COEFFS_H__
#include <vector>

// Coefficients for splines used in fp-math functions (derived from .poly files)
// z || s || 00 || e || {0,1} || m

std::vector<uint64_t> tan_N = {
    0x8000000000, 0x77c910800, 0x78c912702, 0x7996d0406, 0x79c91a312,
    0x79fb6802f,  0x7a96dd575, 0x7ab00996a, 0x7ac9393c5, 0x7ae26cc57,
    0x7afba4af5,  0x7b8a70bbe, 0x7b9711ce6, 0x7ba3b5ce8, 0x7bb05cfbb,
    0x7bbd0795a,  0x7bc9b5dc6, 0x7bd668104, 0x7be31e71c, 0x7befd941e,
    0x7bfc98c1d,  0x7c84ae99b, 0x7c8b136c3, 0x7c917af9b, 0x7c97e563a,
    0x7c9e52cba,  0x7ca4c353a, 0x7cab371dd, 0x7cb1ae4c8, 0x7cb829028,
    0x7cbea762a,  0x7cc529905, 0x7ccbafaf0, 0x7cd239e2b, 0x7cd8c84f8,
    0x7cdf5b1a1,  0x7ce5f2676, 0x7cec8e5ca, 0x7cf32f1f9, 0x7cf9d4d65,
    0x7d803fd3b,  0x7d8397dcd, 0x7d86f29a4, 0x7d8a501fe, 0x7d8db081f,
    0x7d9113d4c,  0x7d947a2cf, 0x7d97e39f9, 0x7d9b5041b, 0x7d9ec028c,
    0x7da2336aa,  0x7da5aa1d4, 0x7da924571, 0x7daca22ea, 0x7db023bb0,
    0x7db3a9138,  0x7db7324fc, 0x7dbabf87d, 0x7dbe50d42, 0x7dc1e64d5,
    0x7dc5800cb,  0x7dc91e2bc, 0x7dccc0c47, 0x7dd067f13, 0x7dd413ccd,
    0x7dd7c4729,  0x7ddb79fe4, 0x7ddf348c1, 0x7de2f438c, 0x7de6b9218,
    0x7dea83641,  0x7dee531ec, 0x7df228707, 0x7df603787, 0x7df9e456d,
    0x7dfdcb2c2,  0x7e80dc0ce, 0x7e82d5a0b, 0x7e84d262d, 0x7e86d264e,
    0x7e88d5b8d,  0x7e8adc70f, 0x7e8ce69ff, 0x7e8ef4590, 0x7e9105af8,
    0x7e931ab75,  0x7e953384d, 0x7e97502cc, 0x7e9970c45, 0x7e9b95611,
    0x7e9dbe194,  0x7e9feb038, 0x7ea21c36e, 0x7ea451cb1, 0x7ea68bd84,
    0x7ea8ca774,  0x7eab0dc15, 0x7ead55d07, 0x7eafa2bf2, 0x7eb1f4a8a,
    0x7eb44ba8a,  0x7eb6a7dbc, 0x7eb9095f3, 0x7ebb7050e, 0x7ebddccf7,
    0x7ec04efa5,  0x7ec2c6f1e, 0x7ec544d71, 0x7ec7c8cbe, 0x7eca52f32,
    0x7ecce3708,  0x7ecf7a68b, 0x7ed218015, 0x7ed4bc612, 0x7ed767afd,
    0x7eda1a162,  0x7edcd3be1, 0x7edf94d2b, 0x7ee25d805, 0x7ee52df49,
    0x7ee8065e4,  0x7eeae6eda, 0x7eedcfd45, 0x7ef0c1456, 0x7ef3bb758,
    0x7ef6be9ac,  0x7ef9caed0, 0x7efce0a5a};

std::vector<uint64_t> trig_knots_bits = {0x1, 0x2,  0x3,  0x4,  0x5, 0x6, 0x7,
                                         0x8, 0x9,  0xa,  0xb,  0xc, 0xd, 0xe,
                                         0xf, 0x10, 0x11, 0x12, 0x13};

std::vector<std::vector<uint64_t>> tan_coeffs = {
    // theta_1 (tan)
    {0x80c90fd8f, 0x80c90fd8c, 0x80c90fd7a, 0x80c90fd71, 0x80c90fd90,
     0x80c90fd8d, 0x80c90fd77, 0x80c90fd71, 0x80c90fd91, 0x80c90fd8e,
     0x80c90fd7a, 0x80c90fd71, 0x80c90fd8e, 0x80c90fd8f, 0x80c90fd7f,
     0x80c90fd75, 0x80c90fd91, 0x80c90fd90, 0x80c90fd83, 0x80c90fd76},
    // theta_3 (tan)
    {0x85bf324af, 0x8598a8bf8, 0x85a7132fa, 0x8595dd606, 0x83f917d57,
     0x83ce65aef, 0x83ec812ce, 0x83d3be6ed, 0x82f34c572, 0x82e0d33bb,
     0x82ef8a3ad, 0x82e5e993c, 0x82bb2caef, 0x82b3f2815, 0x82b608568,
     0x82b44645e, 0x82aa32c8e, 0x82a8d8d22, 0x82a914fe0, 0x82a91ab98}};

std::vector<uint64_t> exp2_knots_bits = {
    0x20, 0x40, 0x50, 0x60, 0x68, 0x70, 0x78, 0x80, 0x84, 0x88, 0x8c,
    0x90, 0x94, 0x98, 0x9c, 0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac,
    0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 0xc0, 0xc1,
    0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc,
    0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf};

std::vector<std::vector<uint64_t>> exp2_coeffs = {
    // theta_0 (2^x)
    {0x7f8000008, 0x7f80000ce, 0x7f8000309, 0x7f8000783, 0x7f8000eb8,
     0x7f80019c0, 0x7f800294d, 0x7f8003dff, 0x7f8005782, 0x7f8007a37,
     0x7f800a3a7, 0x7f800d67f, 0x7f8011024, 0x7f8015816, 0x7f801a65f,
     0x7f802097e, 0x7f802709a, 0x7f802efdf, 0x7f8037c41, 0x7f80417b7,
     0x7f804c52f, 0x7f805894f, 0x7f8065c17, 0x7f80748da, 0x7f8084bb8,
     0x7f8096811, 0x7f80a9e7c, 0x7f80bf007, 0x7f80d5cb3, 0x7f80ee4b3,
     0x7f81092ca, 0x7f8126033, 0x7f8144e40, 0x7f81662d1, 0x7f818a337,
     0x7f81b01c8, 0x7f81d8d4f, 0x7f82046ff, 0x7f823377a, 0x7f826418d,
     0x7f8298453, 0x7f82d0033, 0x7f830b51e, 0x7f8348bd1, 0x7f838c135,
     0x7f83d2015, 0x7f8418cf7, 0x7f8468ed3, 0x7f84baff1, 0x7f850fc33,
     0x7f856bfec, 0x7f85caa12, 0x7f862c585, 0x7f86988e9, 0x7f87034c9,
     0x7f877a48b, 0x7f87f08ba, 0x7f8870f4b, 0x7f88f4fdd, 0x7f897eb41,
     0x7f8a10553, 0x7f8aa7c64, 0x7f8b46c15, 0x7f8be86e6},
    // theta_1 (2^x)
    {0x7eb16fda0, 0x7eb167048, 0x7eb158a95, 0x7eb144556, 0x7eb12b045,
     0x7eb10b328, 0x7eb0e5421, 0x7eb0b984d, 0x7eb089e27, 0x7eb04f90c,
     0x7eb010c2c, 0x7eafca59a, 0x7eaf80e56, 0x7eaf2bc91, 0x7eaed5a58,
     0x7eae6f4bc, 0x7eae0b8b7, 0x7ead971b6, 0x7ead1dd08, 0x7eac9e6e2,
     0x7eac17232, 0x7eab853ea, 0x7eaaef738, 0x7eaa4e38f, 0x7ea9a5250,
     0x7ea8f29be, 0x7ea83713c, 0x7ea7728f7, 0x7ea6a5ad6, 0x7ea5d0d8b,
     0x7ea4eee66, 0x7ea40437e, 0x7ea310a3e, 0x7ea211da3, 0x7ea10610c,
     0x7e9ff43e9, 0x7e9ed61f3, 0x7e9dabdb2, 0x7e9c7286e, 0x7e9b36ce0,
     0x7e99ec4c6, 0x7e9893a56, 0x7e972d963, 0x7e95c3557, 0x7e943ef03,
     0x7e92b4970, 0x7e912ddac, 0x7e8f7cc21, 0x7e8dca7b8, 0x7e8c13035,
     0x7e8a3e182, 0x7e886678a, 0x7e8688a72, 0x7e84813b8, 0x7e828a702,
     0x7e8063dc0, 0x7dfc94fbb, 0x7df81a003, 0x7df392ce0, 0x7deeee28a,
     0x7dea19cc0, 0x7de5290ff, 0x7de00e992, 0x7ddaf2f7f},
    // theta_2 (2^x)
    {0x7cf91ee2d, 0x7cfbc448f, 0x7cfe7be2a, 0x7d80a40a1, 0x7d820110b,
     0x7d836bba4, 0x7d84dad02, 0x7d864a1a4, 0x7d87ac0fc, 0x7d8932bc4,
     0x7d8aae4fa, 0x7d8c335e0, 0x7d8da8ee0, 0x7d8f3b04c, 0x7d90b58de,
     0x7d925bdd1, 0x7d93dd6af, 0x7d9587325, 0x7d972a096, 0x7d98cb3b4,
     0x7d9a71076, 0x7d9c22e14, 0x7d9dcc752, 0x7d9f836db, 0x7da13cf81,
     0x7da2fd340, 0x7da4c2379, 0x7da68bb82, 0x7da858062, 0x7daa261d9,
     0x7dac00cf2, 0x7dadde285, 0x7dafbe6b2, 0x7db1a5e90, 0x7db397777,
     0x7db585d3c, 0x7db77c5d3, 0x7db97a56e, 0x7dbb84266, 0x7dbd8488b,
     0x7dbf8fe62, 0x7dc1a4976, 0x7dc3c0f96, 0x7dc5d7139, 0x7dc80718d,
     0x7dca33023, 0x7dcc4e086, 0x7dce97506, 0x7dd0d5c66, 0x7dd30f590,
     0x7dd563533, 0x7dd7aee4e, 0x7dd9f6fc5, 0x7ddc66455, 0x7ddeb654d,
     0x7de1333bc, 0x7de395a56, 0x7de615f10, 0x7de891fd1, 0x7deb1332d,
     0x7deda335a, 0x7df037541, 0x7df2d6535, 0x7df56b505}};

// input in [2^-24, 1/32)
std::vector<uint64_t> log_knots_bits_1 = {0x1, 0x2, 0x3, 0x4,  0x5,  0x6,
                                          0x7, 0x8, 0x9, 0xa,  0xb,  0xc,
                                          0xd, 0xe, 0xf, 0x10, 0x11, 0x12};

std::vector<std::vector<uint64_t>> log2_coeffs_1 = {
    // theta_0 (exponent = -1)
    {0x67b8aa3b8, 0x454a480000, 0x45ab68c000, 0x45aaed8000, 0x45aff631de,
     0x45bbf3913f, 0x45c9f17880, 0x45da0ff60a, 0x45e9b4dbe7, 0x45f8244835,
     0x45ff7d9ca9, 0x460ff77051, 0x461f803dc0, 0x462ea08164, 0x463e357d1d,
     0x464e51cb78, 0x465f0c019d, 0x46782cc641, 0x468d45504f},
    // theta_1 (exponent = -1)
    {0x8000000000, 0x7fb8aa44d, 0x7fb8ad842, 0x7fb8abc67, 0x7fb8ab52d,
     0x7fb8ab092, 0x7fb8aae6a, 0x7fb8aaea1, 0x7fb8aae15, 0x7fb8aac8b,
     0x7fb8aac24, 0x7fb8aac55, 0x7fb8aac21, 0x7fb8aabae, 0x7fb8aab6f,
     0x7fb8aab7d, 0x7fb8aabf2, 0x7fb8aace2, 0x7fb8ab472},
    // theta_2 (exponent = -1)
    {0x8000000000, 0x8000000000, 0x4879d90000, 0x4858f8f8af, 0x483c24e766,
     0x48284f6d25, 0x480c17f59b, 0x47f9abb50f, 0x47cb85b74d, 0x7daa0befe,
     0x7e88ee318, 0x7ea06fdc5, 0x7eacc5cc3, 0x7eb305d49, 0x7eb5f00fe,
     0x7eb74acb9, 0x7eb7ee42a, 0x7eb83c33f, 0x7eb8319c5},
    // theta_3 (exponent = -1)
    {0x8000000000, 0x8000000000, 0x9b9aaaaaa, 0x988a04e05, 0x95baa0071,
     0x9383e0dac, 0x90d93ecbc, 0x8ee28ec73, 0x8ccfe5ed4, 0x8ab5cb2f8,
     0x88aefbf57, 0x86b0b062c, 0x84afc9cf7, 0x82ace3038, 0x80be0f23d,
     0x7f8de8437, 0x7ea71a1a4, 0x7e89da037, 0x7e8671d2f},
    // theta_0
    {0x8000000000, 0x68b8aa3a8, 0x4548100000, 0x45aea98000, 0x45b9dca492,
     0x45be186e9e, 0x45c9fcd81a, 0x45db6b8e3c, 0x45eb30dca8, 0x45f86d1e79,
     0x4608985af9, 0x460fe2a096, 0x461f98b981, 0x462f29c692, 0x463e55898d,
     0x464ecca0a2, 0x465e6dbdb3, 0x466c69df55, 0x465d4e7349},
    // theta_1
    {0x8000000000, 0x8000000000, 0x7fb8aa3c9, 0x7fb8ac575, 0x7fb8aba10,
     0x7fb8ab329, 0x7fb8aaeaa, 0x7fb8aafe9, 0x7fb8aaf94, 0x7fb8aacd6,
     0x7fb8aad02, 0x7fb8aac61, 0x7fb8aac2e, 0x7fb8aabf3, 0x7fb8aab7d,
     0x7fb8aabbe, 0x7fb8aab83, 0x7fb8aaa35, 0x7fb8aa370},
    // theta_2
    {0x8000000000, 0x8000000000, 0x8000000000, 0x485cca0000, 0x484871d41d,
     0x482bc5ed09, 0x481942d373, 0x480b644e28, 0x47fdfce81b, 0x47f8fd00f2,
     0x47eed0287d, 0x47ed147417, 0x47ec4a2dc9, 0x47ebe7d4f1, 0x47ebb68e32,
     0x47eba14fe5, 0x47eb958f93, 0x47eb8edc42, 0x47eb890fb2},
    // theta_3
    {0x8000000000, 0x8000000000, 0x8000000000, 0x98c800000, 0x95fe2be2c,
     0x93a4587e7, 0x90e765bc2, 0x8ef66dd2c, 0x8cec64b15, 0x8abc03a30,
     0x88beade75, 0x86b529589, 0x84b0ee221, 0x82b1e374f, 0x80bed09d2,
     0x7f9001cea, 0x7ea1e1c2f, 0x7e80b257d, 0x7deafa938}};

std::vector<std::vector<uint64_t>> ln_coeffs_1 = {
    // theta_0 (exponent = -1)
    {0x678000006, 0x453b380000, 0x459aaf8000, 0x459ef90000, 0x45aacf500f,
     0x45b885a2c2, 0x45bfe843e1, 0x45cf8a597b, 0x45ddc59dc0, 0x45ee920a6a,
     0x45fc1e1e10, 0x460c459015, 0x461b0350fc, 0x462b14e7e8, 0x463afee990,
     0x464ab0d034, 0x465a268804, 0x466aff7f0c, 0x468953cf7f},
    // theta_1 (exponent = -1)
    {0x8000000000, 0x7f8000056, 0x7f8001888, 0x7f800109a, 0x7f8000be9,
     0x7f800094c, 0x7f800089c, 0x7f800086e, 0x7f800076d, 0x7f80007dc,
     0x7f8000687, 0x7f800068a, 0x7f80005d9, 0x7f80005db, 0x7f80005d2,
     0x7f80005ae, 0x7f8000566, 0x7f800060e, 0x7f8000ba2},
    // theta_2 (exponent = -1)
    {0x8000000000, 0x8000000000, 0x4869280000, 0x484bd33333, 0x4838446bfc,
     0x481c2c7183, 0x480a199da5, 0x47efbcce98, 0x47c9a65fa4, 0x7c9fd543f,
     0x7db7022f1, 0x7ddbe1a14, 0x7defe5ab3, 0x7df7f9cb5, 0x7dfc01ef4,
     0x7dfe0bd55, 0x7dff10fd9, 0x7dff715d0, 0x7dff59cdb},
    // theta_3 (exponent = -1)
    {0x8000000000, 0x8000000000, 0x9a9000000, 0x97b333333, 0x94fd350fd,
     0x92c221e79, 0x90b0fd0d9, 0x8eadc492b, 0x8c981c397, 0x8aa05ee3e,
     0x8884b7b11, 0x8681fe997, 0x83e928c11, 0x81ef5d3e4, 0x8087377b2,
     0x7ec54fcf7, 0x7de152efe, 0x7dbdb0ae2, 0x7dba42662},
    // theta_0
    {0x8000000000, 0x67ffffffd, 0x451bc00000, 0x459fe37fff, 0x45a824a492,
     0x45af11c37d, 0x45bd4949df, 0x45cafa9d51, 0x45db06f325, 0x45e9d65f57,
     0x45f9bead01, 0x460a8c4c5a, 0x461a28113b, 0x4629d356c5, 0x4639a99321,
     0x46494ab8e4, 0x46589b8ca3, 0x465fee58f3, 0x463e1b1263},
    // theta_1
    {0x8000000000, 0x8000000000, 0x7efffffef, 0x7f8001261, 0x7f800091e,
     0x7f800083c, 0x7f8000724, 0x7f80005eb, 0x7f80005f5, 0x7f8000553,
     0x7f8000546, 0x7f80005b2, 0x7f8000568, 0x7f800053b, 0x7f8000529,
     0x7f80004ee, 0x7f8000493, 0x7f8000419, 0x7effffdf9},
    // theta_2
    {0x8000000000, 0x8000000000, 0x8000000000, 0x484dfbffff, 0x482dbf7df7,
     0x481c9e93e9, 0x480be1cf01, 0x47fc3967a0, 0x47f827aa0a, 0x47ebb9376b,
     0x47e9d7f264, 0x47e8fe8475, 0x47e8772e04, 0x47e839aace, 0x47e81c8560,
     0x47e80d87e4, 0x47e8063df8, 0x47e80293e3, 0x47dffd2ae2},
    // theta_3
    {0x8000000000, 0x8000000000, 0x8000000000, 0x97da00000, 0x94c7df7df,
     0x92a9c2f94, 0x908da0f81, 0x8dec80240, 0x8befd88b0, 0x89d7bcdaa,
     0x87d5b5bc0, 0x85e6d8667, 0x83d7894e7, 0x81d885d94, 0x7ff6ef62f,
     0x7eb5505ee, 0x7dd5b262f, 0x7db0f1a7e, 0x7da23bb13}};

// input in [1/32, 1)
std::vector<uint64_t> log_knots_bits_2 = {
    0x8,  0x10, 0x18, 0x20, 0x24, 0x28, 0x2c, 0x30, 0x32, 0x34, 0x36, 0x38,
    0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
    0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f};

std::vector<std::vector<uint64_t>> log2_coeffs_2 = {
    // theta_0 (exponent = -1)
    {0x46be6a41ea, 0x46e862f26b, 0x46f891aa59, 0x471a2d2e39, 0x472f410b0c,
     0x4749ae4d10, 0x475af22c95, 0x476aceece2, 0x477a096dc5, 0x47890074cd,
     0x478fcd4013, 0x479d04a38f, 0x47aa81816e, 0x47b92524b7, 0x47bb4084e6,
     0x47bdca96d5, 0x47c84fdb85, 0x47ca3d2d5b, 0x47cc9a5d4d, 0x7f8000002,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000},
    // theta_1 (exponent = -1)
    {0x7fb8af6df,  0x7fb8baf51,  0x7fb8c5f05,  0x7fb905041,  0x7fb97cf94,
     0x7fba5bb06,  0x7fbbdccf7,  0x7fbe1e940,  0x7fc19027f,  0x7fc6d6e63,
     0x7fceee0d6,  0x7fda01040,  0x7fe9e7a07,  0x80833fc4c,  0x808a7f2e9,
     0x8092f51ce,  0x809c1b742,  0x80a83ba5b,  0x80b6ab65f,  0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000},
    // theta_2 (exponent = -1)
    {0x7eb74dce7,  0x7eb5a4b30,  0x7eb4898db,  0x7eaf5a57e,  0x7ea7f762e,
     0x7e9cf19c8,  0x7e8d1d58a,  0x7df1797e1,  0x7dbad70d0,  0x7ce085d81,
     0x7998dc93c,  0x47cec92823, 0x47e8f819d3, 0x47f8dc8aca, 0x47faefd620,
     0x47fd48bb3f, 0x47ffbee339, 0x4809764702, 0x480b4c62e8, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000},
    // theta_3 (exponent = -1)
    {0x7e8e70556,  0x7e9877d7c,  0x7e9cf26cb,  0x7eaf38bb7,  0x7ec2aa537,
     0x7ed9f9141,  0x7ef5c1ac1,  0x7f8a37d9c,  0x7f9c48dd5,  0x7fb241222,
     0x7fcd6d46a,  0x7fec2dcbd,  0x8088be406,  0x80a55589e,  0x80b1ff4bc,
     0x80bfe0aa6,  0x80cdff0b6,  0x80dfc3c12,  0x80f3b1b03,  0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000},
    // theta_0
    {0x467fe98efd, 0x46bdceffc4, 0x6e91e9dc2, 0x6fedbf7ca, 0x719c3e00c,
     0x72a3ee83a,  0x72f859bfa,  0x73ca200f3, 0x749c7cf76, 0x74e50da68,
     0x75a487668,  0x75dd4baa8,  0x7693ab45c, 0x769721217, 0x769721217,
     0x7697593a8,  0x7697593a8,  0x76b766181, 0x76b766181, 0x76dc282ce,
     0x76f63c2b6,  0x778c2df8c,  0x779e036b4, 0x77bf79f9b, 0x77c98c2c2,
     0x77f64926f,  0x77f87162f,  0x78868cbd4, 0x789c867a3, 0x78a5fc39b,
     0x78c9a973a,  0x78c4f67ed,  0x78e54fc48, 0x78ee7b1c6, 0x78df50698},
    // theta_1
    {0x7fb8a9e92, 0x7fb8ab86d, 0x7fb89a718, 0x7fb884883, 0x7fb85dfb7,
     0x7fb827a56, 0x7fb7f8279, 0x7fb7acdab, 0x7fb74fd15, 0x7fb6e3daf,
     0x7fb65fffc, 0x7fb5d7a64, 0x7fb53677e, 0x7fb5183c8, 0x7fb5183c8,
     0x7fb4f9b91, 0x7fb4f9b91, 0x7fb470198, 0x7fb470198, 0x7fb3dcd22,
     0x7fb36adb6, 0x7fb2e3966, 0x7fb25a3e9, 0x7fb18951c, 0x7fb128424,
     0x7fb030a32, 0x7faff1386, 0x7faf6367a, 0x7fae7e90e, 0x7fadfbe50,
     0x7facc12bf, 0x7faca35cb, 0x7fab8cfe9, 0x7fab11a08, 0x7fab29e9d},
    // theta_2
    {0x47eb857369, 0x47eb85a6fc, 0x47eb619c06, 0x47eb429519, 0x47eb19b5d2,
     0x47eaebe88a, 0x47eac8bd24, 0x47ea98d542, 0x47ea6557aa, 0x47ea305bd3,
     0x47e9f6da8e, 0x47e9c0b685, 0x47e9869a67, 0x47e977f0d1, 0x47e977f0d1,
     0x47e966b89e, 0x47e966b89e, 0x47e938aba0, 0x47e938aba0, 0x47e90a9863,
     0x47e8e649bb, 0x47e8be62c6, 0x47e897864a, 0x47e86411b4, 0x47e847dcfe,
     0x47e8114e43, 0x47dff9a277, 0x47dfb6a149, 0x47df5a2bfa, 0x47df1f40b4,
     0x47deafe7a1, 0x47de95abfc, 0x47de372940, 0x47de0503f0, 0x47ddf9511e},
    // theta_3
    {0x7de2ec016, 0x7ddf87418, 0x7dc61296d, 0x7db771277, 0x7da903ff2,
     0x7d9c29fbc, 0x7d939220f, 0x7d8982a84, 0x7d80185f5, 0x7cef00ce9,
     0x7cde644b1, 0x7cd028404, 0x7cc244055, 0x7cbe29067, 0x7cbe29067,
     0x7cb942c85, 0x7cb942c85, 0x7caf6931f, 0x7caf6931f, 0x7ca630243,
     0x7c9f00cc7, 0x7c97a1f74, 0x7c90c822f, 0x7c88a150d, 0x7c83d070d,
     0x7bf816a83, 0x7bf1250b0, 0x7be781b20, 0x7bdb97956, 0x7bd3a131e,
     0x7bc6d7df8, 0x7bc2a8805, 0x7bb8564df, 0x7bb258ba7, 0x7bafcba09}};

std::vector<std::vector<uint64_t>> ln_coeffs_2 = {
    // theta_0 (exponent = -1)
    {0x46bb9a1e7b, 0x46d9fb9cf8, 0x46ecb26fa5, 0x470d3c3a35, 0x472b1e3b6b,
     0x473d91b8bf, 0x474e945937, 0x475ee02d1b, 0x476e5a9db5, 0x477ca18e49,
     0x478abc30da, 0x4798d9660e, 0x479e2d5276, 0x47ae0a5cb9, 0x47b8908349,
     0x47ba5a7955, 0x47bcb4b84b, 0x47bf669326, 0x47c96c6b5c, 0x47c96c6b5c,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000},
    // theta_1 (exponent = -1)
    {0x7f8004158,  0x7f800a32a,  0x7f8014387,  0x7f803bdbb,  0x7f8097f85,
     0x7f812ef0d,  0x7f8225ae5,  0x7f83c286a,  0x7f8652e08,  0x7f89eb796,
     0x7f8f2ecec,  0x7f96be35f,  0x7fa16248d,  0x7fbae32ca,  0x7fc557550,
     0x7fd114099,  0x7fe01d4a1,  0x7ff0d08e8,  0x8082cf41a,  0x8082cf41a,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000},
    // theta_2 (exponent = -1)
    {0x7dfdeca5c,  0x7dfc34208,  0x7dfa11f0a,  0x7df389d94,  0x7de8303a7,
     0x7dd95aab5,  0x7dc4ff914,  0x7da7b8d0d,  0x7cfe4c5bb,  0x7c991b1df,
     0x799ae876c,  0x47c9bca86c, 0x47dbf44dc8, 0x47edc67f25, 0x47f85bb796,
     0x47f9f59952, 0x47fbf5830f, 0x47fe1cebea, 0x48085d276d, 0x48085d276d,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000},
    // theta_3 (exponent = -1)
    {0x7dc705d51,  0x7dd1075fa,  0x7dda7901c,  0x7df185ae1,  0x7e87b933f,
     0x7e97494c5,  0x7ea936bbb,  0x7ebf6002e,  0x7eda240dd,  0x7ef7cf905,
     0x7f8d9c8ca,  0x7fa2c0a77,  0x7fbbec881,  0x7feecd82b,  0x80803413a,
     0x8089806ff,  0x8094d5438,  0x80a0ac48d,  0x80aeaf72c,  0x80aeaf72c,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000,
     0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000, 0x8000000000},
    // theta_0
    {0x468b3fff17, 0x468fedfc35, 0x6dc46258e, 0x6fb3f68bf, 0x70d29d1ff,
     0x71ce0e35b,  0x72bc4d051,  0x739d7db71, 0x73f184849, 0x748a00d7f,
     0x74b543494,  0x74f79786d,  0x75a19dd82, 0x75c764d58, 0x75c764d58,
     0x75ff84975,  0x75ff84975,  0x7697f2dc0, 0x7697f2dc0, 0x76bdb0a8a,
     0x76dc9f010,  0x77862c7cb,  0x7799cd708, 0x779f62216, 0x769e3f43a,
     0x76a80c72d,  0x76b597c68,  0x76c70e77b, 0x76f68235b, 0x75eac25a2,
     0x76bb75395,  0x77910ad64,  0x77863f3d8, 0x76c215d06, 0x7787f81a2},
    // theta_1
    {0x7f8000020, 0x7efffe852, 0x7effea8d5, 0x7effc8745, 0x7eff98787,
     0x7eff568f5, 0x7efef9fba, 0x7efe84de0, 0x7efdfcffb, 0x7efdbef7a,
     0x7efd3e86d, 0x7efc91f43, 0x7efbdc60e, 0x7efb2f804, 0x7efb2f804,
     0x7efa4b452, 0x7efa4b452, 0x7ef98a5ba, 0x7ef98a5ba, 0x7ef87f70c,
     0x7ef7a75db, 0x7ef67ddff, 0x7ef58ba08, 0x7ef525244, 0x7ef7b4cd5,
     0x7ef7243cf, 0x7ef6816ef, 0x7ef5cdb1e, 0x7ef4a844e, 0x7ef60b066,
     0x7ef48f793, 0x7ef2b2003, 0x7ef27c825, 0x7ef2e5aeb, 0x7ef16b1e5},
    // theta_2
    {0x47dffa5878, 0x47dff192bf, 0x47dfc7cca8, 0x47df98566c, 0x47df6588db,
     0x47df2c6190, 0x47dee9bf0c, 0x47dea1e738, 0x47de5905c6, 0x47de365d7a,
     0x47ddf92152, 0x47ddb038ca, 0x47dd69960b, 0x47dd2a1df6, 0x47dd2a1df6,
     0x47dcdea9ab, 0x47dcdea9ab, 0x47dca10e07, 0x47dca10e07, 0x47dc54320b,
     0x47dc179c5a, 0x47dbcbf1b0, 0x47db8fb2f8, 0x47db716c11, 0x47dbdf7a7d,
     0x47dbb2af4e, 0x47db83e351, 0x47db539d84, 0x47db12de70, 0x47db36b23d,
     0x47daeb235c, 0x47da94f813, 0x47da7c5c9c, 0x47da78aaf1, 0x47da3612dc},
    // theta_3
    {0x7d9e20c81, 0x7d9814a7e, 0x7d8976f1a, 0x7cfcde8b2, 0x7ceaf7ca2,
     0x7cda875a0, 0x7cca9da75, 0x7cbbfc0fa, 0x7caf01d8c, 0x7ca8d4244,
     0x7c9f58da6, 0x7c95466d3, 0x7c8c4d250, 0x7c84bcb8c, 0x7c84bcb8c,
     0x7bf925a7d, 0x7bf925a7d, 0x7bec62d58, 0x7bec62d58, 0x7bdde2a13,
     0x7bd2da518, 0x7bc6398c9, 0x7bbc788bf, 0x7bb72149e, 0x7bc33a7eb,
     0x7bbb8e109, 0x7bb3fc63c, 0x7bac962ee, 0x7ba3c7fc3, 0x7ba588ebe,
     0x7b9c29785, 0x7b923e57d, 0x7b8e65a4e, 0x7b8c7e905, 0x7b853d05b}};

std::vector<uint64_t> log2_int_to_float = {
    0x242ff, 0x242fe, 0x242fd, 0x242fc, 0x242fb, 0x242fa, 0x242f9, 0x242f8,
    0x242f7, 0x242f6, 0x242f5, 0x242f4, 0x242f3, 0x242f2, 0x242f1, 0x242f0,
    0x242ef, 0x242ee, 0x242ed, 0x242ec, 0x242eb, 0x242ea, 0x242e9, 0x242e8,
    0x242e7, 0x242e6, 0x242e5, 0x242e4, 0x242e3, 0x242e2, 0x242e1, 0x242e0,
    0x242df, 0x242de, 0x242dd, 0x242dc, 0x242db, 0x242da, 0x242d9, 0x242d8,
    0x242d7, 0x242d6, 0x242d5, 0x242d4, 0x242d3, 0x242d2, 0x242d1, 0x242d0,
    0x242cf, 0x242ce, 0x242cd, 0x242cc, 0x242cb, 0x242ca, 0x242c9, 0x242c8,
    0x242c7, 0x242c6, 0x242c5, 0x242c4, 0x242c3, 0x242c2, 0x242c1, 0x242c0,
    0x2427e, 0x2427c, 0x2427a, 0x24278, 0x24276, 0x24274, 0x24272, 0x24270,
    0x2426e, 0x2426c, 0x2426a, 0x24268, 0x24266, 0x24264, 0x24262, 0x24260,
    0x2425e, 0x2425c, 0x2425a, 0x24258, 0x24256, 0x24254, 0x24252, 0x24250,
    0x2424e, 0x2424c, 0x2424a, 0x24248, 0x24246, 0x24244, 0x24242, 0x24240,
    0x241fc, 0x241f8, 0x241f4, 0x241f0, 0x241ec, 0x241e8, 0x241e4, 0x241e0,
    0x241dc, 0x241d8, 0x241d4, 0x241d0, 0x241cc, 0x241c8, 0x241c4, 0x241c0,
    0x24178, 0x24170, 0x24168, 0x24160, 0x24158, 0x24150, 0x24148, 0x24140,
    0x240f0, 0x240e0, 0x240d0, 0x240c0, 0x24060, 0x24040, 0x23fc0, 0x40000,
    0x3fc0,  0x4040,  0x4060,  0x40c0,  0x40d0,  0x40e0,  0x40f0,  0x4140,
    0x4148,  0x4150,  0x4158,  0x4160,  0x4168,  0x4170,  0x4178,  0x41c0,
    0x41c4,  0x41c8,  0x41cc,  0x41d0,  0x41d4,  0x41d8,  0x41dc,  0x41e0,
    0x41e4,  0x41e8,  0x41ec,  0x41f0,  0x41f4,  0x41f8,  0x41fc,  0x4240,
    0x4242,  0x4244,  0x4246,  0x4248,  0x424a,  0x424c,  0x424e,  0x4250,
    0x4252,  0x4254,  0x4256,  0x4258,  0x425a,  0x425c,  0x425e,  0x4260,
    0x4262,  0x4264,  0x4266,  0x4268,  0x426a,  0x426c,  0x426e,  0x4270,
    0x4272,  0x4274,  0x4276,  0x4278,  0x427a,  0x427c,  0x427e,  0x42c0,
    0x42c1,  0x42c2,  0x42c3,  0x42c4,  0x42c5,  0x42c6,  0x42c7,  0x42c8,
    0x42c9,  0x42ca,  0x42cb,  0x42cc,  0x42cd,  0x42ce,  0x42cf,  0x42d0,
    0x42d1,  0x42d2,  0x42d3,  0x42d4,  0x42d5,  0x42d6,  0x42d7,  0x42d8,
    0x42d9,  0x42da,  0x42db,  0x42dc,  0x42dd,  0x42de,  0x42df,  0x42e0,
    0x42e1,  0x42e2,  0x42e3,  0x42e4,  0x42e5,  0x42e6,  0x42e7,  0x42e8,
    0x42e9,  0x42ea,  0x42eb,  0x42ec,  0x42ed,  0x42ee,  0x42ef,  0x42f0,
    0x42f1,  0x42f2,  0x42f3,  0x42f4,  0x42f5,  0x42f6,  0x42f7,  0x42f8,
    0x42f9,  0x42fa,  0x42fb,  0x42fc,  0x42fd,  0x42fe,  0x42ff,  0x4340};

std::vector<uint64_t> ln_int_to_float = {
    0x485b00f33c, 0x485aeac4f9, 0x485ad496b7, 0x485abe6874, 0x485aa83a31,
    0x485a920bee, 0x485a7bddab, 0x485a65af68, 0x485a4f8125, 0x485a3952e2,
    0x485a23249f, 0x485a0cf65c, 0x4859f6c819, 0x4859e099d6, 0x4859ca6b93,
    0x4859b43d50, 0x48599e0f0d, 0x485987e0ca, 0x485971b287, 0x48595b8444,
    0x4859455601, 0x48592f27be, 0x485918f97b, 0x485902cb38, 0x4858ec9cf5,
    0x4858d66eb2, 0x4858c0406f, 0x4858aa122c, 0x485893e3e9, 0x48587db5a6,
    0x4858678763, 0x4858515920, 0x48583b2add, 0x485824fc9a, 0x48580ece57,
    0x484ff14027, 0x484fc4e3a1, 0x484f98871b, 0x484f6c2a95, 0x484f3fce0f,
    0x484f137189, 0x484ee71503, 0x484ebab87d, 0x484e8e5bf7, 0x484e61ff71,
    0x484e35a2eb, 0x484e094665, 0x484ddce9df, 0x484db08d59, 0x484d8430d3,
    0x484d57d44d, 0x484d2b77c7, 0x484cff1b41, 0x484cd2bebb, 0x484ca66235,
    0x484c7a05af, 0x484c4da929, 0x484c214ca3, 0x484bf4f01d, 0x484bc89397,
    0x484b9c3711, 0x484b6fda8b, 0x484b437e05, 0x484b17217f, 0x484aeac4f9,
    0x484abe6874, 0x484a920bee, 0x484a65af68, 0x484a3952e2, 0x484a0cf65c,
    0x4849e099d6, 0x4849b43d50, 0x484987e0ca, 0x48495b8444, 0x48492f27be,
    0x484902cb38, 0x4848d66eb2, 0x4848aa122c, 0x48487db5a6, 0x4848515920,
    0x484824fc9a, 0x483ff14027, 0x483f98871b, 0x483f3fce0f, 0x483ee71503,
    0x483e8e5bf7, 0x483e35a2eb, 0x483ddce9df, 0x483d8430d3, 0x483d2b77c7,
    0x483cd2bebb, 0x483c7a05af, 0x483c214ca3, 0x483bc89397, 0x483b6fda8b,
    0x483b17217f, 0x483abe6874, 0x483a65af68, 0x483a0cf65c, 0x4839b43d50,
    0x48395b8444, 0x483902cb38, 0x4838aa122c, 0x4838515920, 0x482ff14027,
    0x482f3fce0f, 0x482e8e5bf7, 0x482ddce9df, 0x482d2b77c7, 0x482c7a05af,
    0x482bc89397, 0x482b17217f, 0x482a65af68, 0x4829b43d50, 0x482902cb38,
    0x4828515920, 0x481f3fce0f, 0x481ddce9df, 0x481c7a05af, 0x481b17217f,
    0x4819b43d50, 0x4818515920, 0x480ddce9df, 0x480b17217f, 0x4808515920,
    0x47fb17217f, 0x47eb17217f, 0x8000000000, 0x7eb17217f,  0x7fb17217f,
    0x808515920,  0x80b17217f,  0x80ddce9df,  0x818515920,  0x819b43d50,
    0x81b17217f,  0x81c7a05af,  0x81ddce9df,  0x81f3fce0f,  0x828515920,
    0x82902cb38,  0x829b43d50,  0x82a65af68,  0x82b17217f,  0x82bc89397,
    0x82c7a05af,  0x82d2b77c7,  0x82ddce9df,  0x82e8e5bf7,  0x82f3fce0f,
    0x82ff14027,  0x838515920,  0x838aa122c,  0x83902cb38,  0x8395b8444,
    0x839b43d50,  0x83a0cf65c,  0x83a65af68,  0x83abe6874,  0x83b17217f,
    0x83b6fda8b,  0x83bc89397,  0x83c214ca3,  0x83c7a05af,  0x83cd2bebb,
    0x83d2b77c7,  0x83d8430d3,  0x83ddce9df,  0x83e35a2eb,  0x83e8e5bf7,
    0x83ee71503,  0x83f3fce0f,  0x83f98871b,  0x83ff14027,  0x84824fc9a,
    0x848515920,  0x8487db5a6,  0x848aa122c,  0x848d66eb2,  0x84902cb38,
    0x8492f27be,  0x8495b8444,  0x84987e0ca,  0x849b43d50,  0x849e099d6,
    0x84a0cf65c,  0x84a3952e2,  0x84a65af68,  0x84a920bee,  0x84abe6874,
    0x84aeac4f9,  0x84b17217f,  0x84b437e05,  0x84b6fda8b,  0x84b9c3711,
    0x84bc89397,  0x84bf4f01d,  0x84c214ca3,  0x84c4da929,  0x84c7a05af,
    0x84ca66235,  0x84cd2bebb,  0x84cff1b41,  0x84d2b77c7,  0x84d57d44d,
    0x84d8430d3,  0x84db08d59,  0x84ddce9df,  0x84e094665,  0x84e35a2eb,
    0x84e61ff71,  0x84e8e5bf7,  0x84ebab87d,  0x84ee71503,  0x84f137189,
    0x84f3fce0f,  0x84f6c2a95,  0x84f98871b,  0x84fc4e3a1,  0x84ff14027,
    0x8580ece57,  0x85824fc9a,  0x8583b2add,  0x858515920,  0x858678763,
    0x8587db5a6,  0x85893e3e9,  0x858aa122c,  0x858c0406f,  0x858d66eb2,
    0x858ec9cf5,  0x85902cb38,  0x85918f97b,  0x8592f27be,  0x859455601,
    0x8595b8444,  0x85971b287,  0x85987e0ca,  0x8599e0f0d,  0x859b43d50,
    0x859ca6b93,  0x859e099d6,  0x859f6c819,  0x85a0cf65c,  0x85a23249f,
    0x85a3952e2,  0x85a4f8125,  0x85a65af68,  0x85a7bddab,  0x85a920bee,
    0x85aa83a31,  0x85abe6874,  0x85ad496b7,  0x85aeac4f9,  0x85b00f33c,
    0x85b17217f};

// input in [2^-24, 1/32)
std::vector<uint64_t> sin_knots_bits_1 = {0x1, 0x2, 0x3, 0x4,
                                          0x5, 0x6, 0x7, 0x8};

std::vector<std::vector<uint64_t>> sin_coeffs_1 = {
    // theta_0 (x)
    {0x80c90fdcb, 0x80c90fdc9, 0x80c90fdc8, 0x80c90fdcb, 0x80c90fdcd,
     0x80c90fdca, 0x80c90fdc8, 0x80c90fdc8, 0x80c90fdc8},
    // theta_1 (x^3)
    {0x4868a64cdd, 0x484914a934, 0x482cbdf0fd, 0x481e8199b3, 0x481b70f5b4,
     0x481a95452f, 0x481a6507d9, 0x481a59a9c6, 0x481a56d141},
    // theta_2 (x^5)
    {0x9fd5c1706, 0x9bc6f0f5c, 0x97c1ea8b3, 0x93d5ea0d8, 0x8fe266fb5,
     0x8bc8f1f91, 0x87c36e043, 0x83d6c62ff, 0x81822bf6a}};

std::vector<std::vector<uint64_t>> cos_coeffs_1 = {
    // theta_0 (x)
    {0x80c90fdd2, 0x80c90fdcc, 0x80c90fdce, 0x80c90fdcc, 0x80c90fdcc,
     0x80c90fdcc, 0x80c90fdcb, 0x80c90fdcb, 0x80c90fdcd},
    // theta_1 (x^3)
    {0x486a2a90c7, 0x4849f20c6d, 0x482e343a01, 0x481ea4041f, 0x481b6cd07b,
     0x481a99292b, 0x481a662452, 0x481a59ffbf, 0x481a56f965},
    // theta_2 (x^5)
    {0x9fff3e016, 0x9be0d5ef1, 0x97e6f0994, 0x93dd4afb2, 0x8fe199edf,
     0x8bd77b7ba, 0x87cfef880, 0x83e759e8e, 0x818a2e0fd}};

// input in [1/32, 0.5]
std::vector<uint64_t> sin_knots_bits_2 = {
    0x20, 0x30, 0x40, 0x48, 0x50, 0x58, 0x60, 0x64, 0x66, 0x68, 0x69,
    0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74,
    0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f};

std::vector<std::vector<uint64_t>> sin_coeffs_2 = {
    // theta_0 (x)
    {0x80c90fdca, 0x80c90fdef, 0x80c90fe23, 0x80c90fd59, 0x80c90fb69,
     0x80c90f550, 0x80c90e686, 0x80c90c854, 0x80c90b44e, 0x80c90933c,
     0x80c90a91b, 0x80c909392, 0x80c90749c, 0x80c905f25, 0x80c904104,
     0x80c901b8b, 0x80c8fef5b, 0x80c8fc240, 0x80c8f9294, 0x80c8f6146,
     0x80c8f2003, 0x80c8edf2a, 0x80c8e8cde, 0x80c8e47da, 0x80c8de7fe,
     0x80c8d8b1a, 0x80c8d20ae, 0x80c8cab1d, 0x80c8c2d50, 0x80c8ba53a,
     0x80c8b0cc7, 0x80c8a670c, 0x80c89cbf9, 0x80c89175d},
    // theta_1 (x^3)
    {0x481a561e85, 0x481a56097e, 0x481a55fa4e, 0x481a55ae60, 0x481a554d55,
     0x481a548cc6, 0x481a534bb1, 0x481a5166f1, 0x481a50571a, 0x481a4ece64,
     0x481a4f9381, 0x481a4ea17a, 0x481a4d6492, 0x481a4c8a52, 0x481a4b6f09,
     0x481a4a2501, 0x481a48b65e, 0x481a475120, 0x481a45e848, 0x481a448216,
     0x481a42c186, 0x481a411598, 0x481a3f110b, 0x481a3d6d07, 0x481a3b4156,
     0x481a393a6e, 0x481a36ff3b, 0x481a349fc8, 0x481a322d91, 0x481a2fa00c,
     0x481a2cde8c, 0x481a29fa4c, 0x481a275a94, 0x481a2465bc},
    // theta_2 (x^5)
    {0x80a977765, 0x80a4842fa, 0x80a3093a4, 0x80a173d7d, 0x80a04e353,
     0x809ed5d60, 0x809d2299d, 0x809b3b876, 0x809a55fe1, 0x809933182,
     0x8099a0117, 0x8098f9124, 0x8098300fb, 0x8097a7a9d, 0x809702dc5,
     0x80964e652, 0x8095911aa, 0x8094e0fbb, 0x809436e66, 0x809394dce,
     0x8092d4957, 0x809224753, 0x80915a0e6, 0x8090ba945, 0x808ff1714,
     0x808f3c713, 0x808e7cf6d, 0x808db8e9b, 0x808cf62d4, 0x808c321bf,
     0x808b661c2, 0x808a97804, 0x8089e1b2f, 0x80891b84b}};

std::vector<std::vector<uint64_t>> cos_coeffs_2 = {
    // theta_0 (x)
    {0x80c90fdc8, 0x80c90fdf1, 0x80c90fe1b, 0x80c90fd5f, 0x80c90fb79,
     0x80c90f557, 0x80c90e63d, 0x80c90c811, 0x80c90b44e, 0x80c909426,
     0x80c90a3f0, 0x80c9091f8, 0x80c90789c, 0x80c905efb, 0x80c903a1f,
     0x80c9016bc, 0x80c8ff36c, 0x80c8fc252, 0x80c8f90d3, 0x80c8f53fb,
     0x80c8f21b4, 0x80c8edb6d, 0x80c8e930d, 0x80c8e44ed, 0x80c8de0fd,
     0x80c8d871b, 0x80c8d1f33, 0x80c8caf7d, 0x80c8c2c3a, 0x80c8ba675,
     0x80c8b0c2a, 0x80c8a748c, 0x80c89c885, 0x80c891374},
    // theta_1 (x^3)
    {0x481a561a1e, 0x481a560b12, 0x481a55f75c, 0x481a55afc0, 0x481a554f73,
     0x481a548d87, 0x481a53465f, 0x481a51634e, 0x481a50571a, 0x481a4ed84a,
     0x481a4f5fe4, 0x481a4e928e, 0x481a4d8926, 0x481a4c88f1, 0x481a4b3604,
     0x481a49fef1, 0x481a48d515, 0x481a47519c, 0x481a45dc1b, 0x481a442941,
     0x481a42cc73, 0x481a40fe79, 0x481a3f35cf, 0x481a3d5c64, 0x481a3b1b0d,
     0x481a392555, 0x481a36f789, 0x481a34b573, 0x481a322859, 0x481a2fa5c3,
     0x481a2cdbc0, 0x481a2a34c3, 0x481a274c25, 0x481a245593},
    // theta_2 (x^5)
    {0x80a905221, 0x80a49729d, 0x80a2f8a68, 0x80a178bc7, 0x80a0528c4,
     0x809ed704c, 0x809d1c7a7, 0x809b3868f, 0x809a55fe1, 0x809939c73,
     0x80997fda8, 0x8098f0611, 0x809844f56, 0x8097a6f12, 0x8096e566a,
     0x80963b931, 0x80959f9aa, 0x8094e12f3, 0x8094319d0, 0x80936fc5b,
     0x8092d8fdf, 0x80921b852, 0x809167b6a, 0x8090b4ab4, 0x808fe45d7,
     0x808f357b0, 0x808e7a770, 0x808dbf9e2, 0x808cf49bb, 0x808c33c42,
     0x808b654fa, 0x808aa7528, 0x8089ddedc, 0x80891758c}};

std::vector<uint64_t> exp_knots_bits_1 = {0x1, 0x2, 0x3, 0x4,  0x5, 0x6,
                                          0x7, 0x8, 0x9, 0xa,  0xb, 0xc,
                                          0xd, 0xe, 0xf, 0x10, 0x11};

std::vector<std::vector<uint64_t>> exp_coeffs_1 = {
    // theta_0 (e^x)
    {0x7f80000ed, 0x7f8000120, 0x7f80000da, 0x7f80000e2, 0x7f80000ae,
     0x7f8000095, 0x7f8000087, 0x7f8000080, 0x7f800007e, 0x7f800007a,
     0x7f800007a, 0x7f800007b, 0x7f8000079, 0x7f800007a, 0x7f800007a,
     0x7f8000078, 0x7f800007e, 0x7f8000098},
    // theta_1 (e^x)
    {0x484a08009f, 0x483ba000b9, 0x481fd0a694, 0x480e91dc31, 0x47ed46fed6,
     0x7cd003cbf, 0x7ea216816, 0x7ed3111ce, 0x7ee9e2e10, 0x7ef54b16d,
     0x7efaa2b51, 0x7efd4c994, 0x7efeabd23, 0x7eff541a6, 0x7effaa77d,
     0x7effd5a81, 0x7effe98d2, 0x7efff121d},
    // theta_2 (e^x)
    {0x9ad6000f8, 0x98f800120, 0x96b406f5d, 0x94bc13de1, 0x92975ab85,
     0x9085e53b5, 0x8df937671, 0x8bef9c9db, 0x89ebf0c75, 0x87e48c9db,
     0x85e5d28fa, 0x83ea851b0, 0x81f2c31c1, 0x8092ad369, 0x7ef219188,
     0x7e9c6c2bf, 0x7e87dde61, 0x7e8341a52},
    // theta_0 (e^-x)
    {0x7f8000077, 0x7f8000052, 0x7f800005a, 0x7f8000048, 0x7f800003f,
     0x7f800003b, 0x7f8000037, 0x7f8000035, 0x7f8000034, 0x7f8000034,
     0x7f8000031, 0x7f8000038, 0x7f8000034, 0x7f8000036, 0x7f8000031,
     0x7f8000032, 0x7f800002c, 0x7f800000e},
    // theta_1 (e^-x)
    {0x483ba000e7, 0x48289852c5, 0x481a58ed75, 0x480a8f735a, 0x47fd987e3c,
     0x47fa95fa98, 0x47f93773aa, 0x47f89717e6, 0x47f84a148a, 0x47f82515e2,
     0x47f811834a, 0x47f809f36d, 0x47f804a384, 0x47f802663b, 0x47f80115f3,
     0x47f8008e05, 0x47f8003b8f, 0x47effffc8a},
    // theta_2 (e^-x)
    {0x99f800120, 0x97ac06e49, 0x95bc13c7b, 0x939140f5f, 0x90f0f9cac,
     0x8edc8e9f0, 0x8ccfaad40, 0x8ac9987e6, 0x88c5aeb9b, 0x86c635b61,
     0x84bccb91c, 0x82dc3038d, 0x80e61b0d3, 0x7fa665d7b, 0x7eae242e1,
     0x7e8bae8f2, 0x7e8218113, 0x7dfe60c96}};

std::vector<uint64_t> exp_knots_bits_2 = {
    0x20, 0x30, 0x40, 0x48, 0x50, 0x58, 0x60, 0x64, 0x68, 0x6c, 0x70,
    0x74, 0x78, 0x7c, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e,
    0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa1, 0xa2,
    0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac};

std::vector<std::vector<uint64_t>> exp_coeffs_2 = {
    // theta_0 (e^x)
    {0x7f800016c, 0x7f8000648, 0x7f80010f1, 0x7f800234a, 0x7f800405d,
     0x7f80069eb, 0x7f800a32d, 0x7f800eebf, 0x7f8014f6e, 0x7f801c6c6,
     0x7f8025ab2, 0x7f8030b26, 0x7f803dedc, 0x7f804d717, 0x7f805f845,
     0x7f8074622, 0x7f808c41b, 0x7f80a75c3, 0x7f80c6123, 0x7f80e80cf,
     0x7f810dfeb, 0x7f813a654, 0x7f8169bfb, 0x7f819ebb3, 0x7f81d9a30,
     0x7f8218de0, 0x7f825f429, 0x7f82aaa3a, 0x7f82fd5d8, 0x7f8357854,
     0x7f83bae96, 0x7f8425040, 0x7f84956ad, 0x7f8512237, 0x7f8595048,
     0x7f8624042, 0x7f86beff5, 0x7f8760de3, 0x7f8811cae, 0x7f88cc39a,
     0x7f8992b1b, 0x7f8a65490, 0x7f8b454ee, 0x7f8cc753f},
    // theta_1 (e^x)
    {0x7effe9e54, 0x7effc601f, 0x7eff9030d, 0x7eff49d13, 0x7efeefc3e,
     0x7efe83efc, 0x7efe04023, 0x7efd6fd1f, 0x7efcc6ed5, 0x7efc0adf8,
     0x7efb36872, 0x7efa4e057, 0x7ef94c317, 0x7ef8332c3, 0x7ef7015d7,
     0x7ef5b62c8, 0x7ef451500, 0x7ef2d270a, 0x7ef1372fb, 0x7eef869f8,
     0x7eedba0f5, 0x7eebb7916, 0x7ee9aae3f, 0x7ee7780f1, 0x7ee51f6ba,
     0x7ee2b369d, 0x7ee01a6a3, 0x7edd6b9fb, 0x7eda937cd, 0x7ed794337,
     0x7ed46198e, 0x7ed1131e5, 0x7ecdad4de, 0x7eca04432, 0x7ec648e9e,
     0x7ec251cf8, 0x7ebe237ec, 0x7eb9e2f99, 0x7eb55b8a8, 0x7eb0b4551,
     0x7eabdeead, 0x7ea6dccd9, 0x7ea1a83ef, 0x7e98e56a6},
    // theta_2 (e^x)
    {0x7e8369342, 0x7e8577967, 0x7e87953f1, 0x7e89b016a, 0x7e8bdd562,
     0x7e8e0c7d6, 0x7e9047d62, 0x7e928ccd4, 0x7e94dab0c, 0x7e972b348,
     0x7e998cbfe, 0x7e9bf166e, 0x7e9e65514, 0x7ea0e18e8, 0x7ea3684b5,
     0x7ea5f954d, 0x7ea89419b, 0x7eab38253, 0x7eade884c, 0x7eb098d31,
     0x7eb353980, 0x7eb63ccd4, 0x7eb91379c, 0x7ebbfed57, 0x7ebefc62e,
     0x7ec1f44fb, 0x7ec50595c, 0x7ec813c26, 0x7ecb34deb, 0x7ece65236,
     0x7ed1afe9a, 0x7ed4fc0ab, 0x7ed8456a6, 0x7edbb56db, 0x7edf1d1f8,
     0x7ee2a1dd4, 0x7ee63dd4a, 0x7ee9d0d18, 0x7eed8694e, 0x7ef13dfc2,
     0x7ef5025e1, 0x7ef8d2167, 0x7efcb0e6c, 0x7f8186e6a},
    // theta_0 (e^-x)
    {0x7effffe33, 0x7effff6bf, 0x7efffe5ff, 0x7efffc8f4, 0x7efff9bb0,
     0x7efff5b5c, 0x7efff05de, 0x7effe964d, 0x7effe0d15, 0x7effd645a,
     0x7effc9b60, 0x7effbaf2c, 0x7effa9c49, 0x7eff963c9, 0x7eff8091e,
     0x7eff67d14, 0x7eff4c8ef, 0x7eff2e046, 0x7eff0d3ae, 0x7efee87de,
     0x7efec1dad, 0x7efe97234, 0x7efe6997d, 0x7efe38ab5, 0x7efe04e37,
     0x7efdccaf1, 0x7efd919f9, 0x7efd5305b, 0x7efd112d0, 0x7efccb4c4,
     0x7efc82591, 0x7efc353ec, 0x7efbe4fb2, 0x7efb9008a, 0x7efb3a250,
     0x7efadd784, 0x7efa7e217, 0x7efa1be29, 0x7ef9b4aa2, 0x7ef94ae86,
     0x7ef8dd754, 0x7ef86bb35, 0x7ef7f7038, 0x7ef852386},
    // theta_1 (e^-x)
    {0x47effeff3a, 0x47effd184b, 0x47effa4411, 0x47eff69795, 0x47eff2047c,
     0x47efec9ba6, 0x47efe67291, 0x47efdf6b28, 0x47efd7bc53, 0x47efcf3c78,
     0x47efc609a2, 0x47efbc226a, 0x47efb1810f, 0x47efa64a05, 0x47ef9aac15,
     0x47ef8e3b39, 0x47ef815670, 0x47ef73b2ff, 0x47ef65d3c7, 0x47ef5710e8,
     0x47ef484764, 0x47ef38ac62, 0x47ef28c235, 0x47ef18604f, 0x47ef07ba40,
     0x47eef65a64, 0x47eee4c66d, 0x47eed2cf17, 0x47eec08feb, 0x47eeadd7ff,
     0x47ee9aee82, 0x47ee87922a, 0x47ee7406bd, 0x47ee5ff42e, 0x47ee4c3b95,
     0x47ee378bf4, 0x47ee22d868, 0x47ee0e12b6, 0x47edf8cfb7, 0x47ede3928c,
     0x47edce2077, 0x47edb85e58, 0x47eda291ce, 0x47edb2ed12},
    // theta_2 (e^-x)
    {0x7dfa413cc, 0x7df65c7a2, 0x7df288c09, 0x7deed099b, 0x7deb1d72b,
     0x7de779a48, 0x7de3ec678, 0x7de061655, 0x7ddcf0640, 0x7dd9838b3,
     0x7dd6254d7, 0x7dd2d2f5b, 0x7dcf89074, 0x7dcc50bac, 0x7dc93379e,
     0x7dc6131b7, 0x7dc306781, 0x7dbffae81, 0x7dbd0bab3, 0x7dba14825,
     0x7db7401f1, 0x7db46665b, 0x7db19e95f, 0x7daee06c5, 0x7dac3342e,
     0x7da983cae, 0x7da6e625f, 0x7da4522ac, 0x7da1caf68, 0x7d9f4928e,
     0x7d9cd59a3, 0x7d9a67570, 0x7d98062cc, 0x7d95a7146, 0x7d93637ca,
     0x7d911479d, 0x7d8ed5215, 0x7d8ca2f9c, 0x7d8a72669, 0x7d8850712,
     0x7d8636960, 0x7d8421e65, 0x7d8218a41, 0x7d839007c}};

// input in [2^-11, 1)
std::vector<uint64_t> erf_knots_bits_1 = {
    0x8,  0x10, 0x14, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x21, 0x22, 0x23, 0x24,
    0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f};

std::vector<std::vector<uint64_t>> erf_coeffs_1 = {
    // theta_0 (1)
    {0x45be3c7d08, 0x46985d4af5, 0x46cb25c4d0, 0x46ec194c1a, 0x470a9303a2,
     0x471c5d2d46, 0x472cbdbe58, 0x473ce69dc3, 0x474cf3fb0a, 0x475a63702d,
     0x476808c854, 0x476bcb2be9, 0x4778556d1b, 0x477b9beea0, 0x477ffc7988,
     0x478b24e800, 0x478de6a89f, 0x479b293cbd, 0x47a85c37e0, 0x47abd04ad1,
     0x47afef743e, 0x47ba4ce224, 0x47bcc0b5c0, 0x47bf3d7ea9},
    // theta_1 (x)
    {0x7f906ebaf, 0x7f906f56d, 0x7f90728d3, 0x7f907ad17, 0x7f908f5f3,
     0x7f90ae5c6, 0x7f90df5d9, 0x7f913487a, 0x7f91c8046, 0x7f926390c,
     0x7f93311e2, 0x7f9429fc8, 0x7f95545a0, 0x7f96c99cd, 0x7f989a736,
     0x7f9b0ca4b, 0x7f9d04aab, 0x7fa29ed4c, 0x7fa946b6e, 0x7fb0cb8f2,
     0x7fb905775, 0x7fc19bc8d, 0x7fc9fb812, 0x7fd1e6374},
    // theta_2 (x^2)
    {0x46ecbba7a5, 0x473f7e6e17, 0x475e97252b, 0x47787b3c90, 0x4788deefd1,
     0x478e7afee3, 0x479b09cda0, 0x47a867dbf9, 0x47acc01e59, 0x47b860f0a2,
     0x47bacab451, 0x47bd774bd1, 0x47c8377a94, 0x47c9f20cd4, 0x47cbf3cf93,
     0x47ce7b6fe4, 0x47d82d1e67, 0x47daa97eff, 0x47dd51bc64, 0x47e8063707,
     0x47e9648ee9, 0x47eab5d3c0, 0x47ebe6de12, 0x47ecf3ac60},
    // theta_3 (x^3)
    {0x47dc053d72, 0x47dbe646a2, 0x47dbb7c91b, 0x47db7c7233, 0x47db243c09,
     0x47dace6ddb, 0x47da6a7013, 0x47d9e52905, 0x47d936e8a6, 0x47d8aacbda,
     0x47d810ad54, 0x47cee8742a, 0x47cda6bfc9, 0x47cc495adb, 0x47cacf28a0,
     0x47c910ff28, 0x47bfc52546, 0x47b9e25e14, 0x47a872d734, 0x7887d00ab,
     0x7abd6b10c,  0x7ba3b767d,  0x7bdd99a9d,  0x7c869249a}};

// input in [1, 3.875]
std::vector<uint64_t> erf_knots_bits_2 = {
    0x2,  0x4,  0x6,  0x8,  0xa,  0xc,  0xe,  0x10, 0x12, 0x14, 0x16, 0x18,
    0x1a, 0x1c, 0x1e, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d};

std::vector<std::vector<uint64_t>> erf_coeffs_2 = {
    // theta_0 (1)
    {0x47c868ebc9, 0x47c94fbf20, 0x47c9e747bc, 0x47ca1d6071, 0x47c9e875db,
     0x47c927997f, 0x47bfdb43d3, 0x47bc1f6534, 0x47ae68a1b7, 0x47888b466a,
     0x7ab903a6c,  0x7bd536f02,  0x7cae967b1,  0x7cf7716f3,  0x7da22f4e0,
     0x7dc6a1dc8,  0x7de968995,  0x7e876154f,  0x7e98ab08f,  0x7ea8af4cd,
     0x7eb74001e,  0x7ec460bac,  0x7ed1661f7,  0x7edc755fa,  0x7ee6cfa2a,
     0x7eefaf0b3,  0x7ef7011a5,  0x7efd3de88,  0x7f804ef84,  0x7f81f11ea,
     0x7f8523621,  0x7f86557ab,  0x7f88001db,  0x7f893f962,  0x7f8ba2e08,
     0x7f8d88fe1,  0x7f8f55896,  0x7f8fdedea,  0x7f9587246,  0x7f97896c6,
     0x7f95fb467,  0x7f9de111f,  0x7facdffe3,  0x7f9aa4613,  0x7faf4dff6,
     0x7fb4b70e2},
    // theta_1 (x)
    {0x7fd6a5ec7,  0x7fdbcea5d,  0x7fdf08b82,  0x7fe02d869,  0x7fdf43175,
     0x7fdbe4cda,  0x7fd69c8cf,  0x7fcee13ad,  0x7fc51b417,  0x7fb964f94,
     0x7facd3214,  0x7f9f7b409,  0x7f90fb46a,  0x7f81fd706,  0x7ee56bdb5,
     0x7ec9729ed,  0x7eaf97aa4,  0x7e94acfb3,  0x7df91c518,  0x7dcdcd074,
     0x7da79a2e1,  0x7d8631e3e,  0x7ccbfc3aa,  0x7c9709100,  0x7bcde2d91,
     0x7afc6468e,  0x79fb2c971,  0x77aa9b3b4,  0x477f5d007f, 0x479a058703,
     0x47ab44ca75, 0x47ad46561f, 0x47b815cb54, 0x47b9161589, 0x47bb250126,
     0x47bcac863a, 0x47be10ad3c, 0x47be4595e2, 0x47c97d99d0, 0x47ca2f6bc7,
     0x47c9568d9e, 0x47cc788436, 0x47d9349db5, 0x47cabfefe6, 0x47d96210d0,
     0x47da4ae0de},
    // theta_2 (x^2)
    {0x47ed8bf3d0, 0x47ee296117, 0x47ee8729a6, 0x47eea8098e, 0x47ee928cd4,
     0x47ee424409, 0x47edc8f1f5, 0x47ed1e2bd6, 0x47ec4f028f, 0x47eb608fe4,
     0x47ea6a84f5, 0x47e96ec877, 0x47e866dc75, 0x47debf3c32, 0x47dcb94b29,
     0x47daef4a61, 0x47d9553a0c, 0x47cf6ee3e0, 0x47cca10836, 0x47ca308cb2,
     0x47c81a6643, 0x47bcaa4f54, 0x47b95931db, 0x47ad6b6606, 0x47a8c52a09,
     0x479a19265f, 0x4788caa625, 0x72f89708d,  0x778b3c3ca,  0x7882f8695,
     0x79828c60b,  0x7993a613c,  0x79adde30c,  0x79be2a578,  0x79e386f84,
     0x79fd0f0f4,  0x7a89931ea,  0x7a88df30e,  0x7ab27fa62,  0x7abc0ad33,
     0x7aa93fb89,  0x7ade12319,  0x7ba123bee,  0x7ab90ea5c,  0x7b9ed1252,
     0x7bab7b278},
    // theta_3 (x^3)
    {0x7c9349436,  0x7c9fcba89,  0x7ca6e424a,  0x7ca957ab5,  0x7ca80a57f,
     0x7ca30f324,  0x7c9bcdcf5,  0x7c91fb2da,  0x7c868bb97,  0x7bf3ce7b2,
     0x7bdab8f58,  0x7bc1fc77e,  0x7ba8f73bb,  0x7b90e3e93,  0x7af40f03a,
     0x7acd01212,  0x7aab2222a,  0x7a8a01504,  0x79dc61346,  0x79ad7d891,
     0x7986992a7,  0x78cd2a8b0,  0x7892e2def,  0x77cc14bd0,  0x76feead45,
     0x768889896,  0x74c53b50b,  0x4738912c1a, 0x474b0a91b2, 0x47589fed27,
     0x475fa2eebb, 0x476884ce05, 0x4769b759ac, 0x476a59ff67, 0x476c16b54a,
     0x476d27309b, 0x476e038813, 0x476dabd1e3, 0x4778bdec83, 0x47790a5129,
     0x476ff9e4cb, 0x477a4c3ec1, 0x477eb0d766, 0x47784bca01, 0x477e0019d9,
     0x477ee14486}};

std::vector<std::vector<uint64_t>> neg_exp_coeffs_64_1 = {
    // theta_0 (e^-x)
    {0x7effff402, 0x7effff402, 0x7effff402, 0x7effff402, 0x7effff402, 0x7effff402, 0x7effff40a, 0x7effff40a, 0x7effff411, 0x7effff415, 0x7effff416, 0x7effff415, 0x7effff409, 0x7effff3d9, 0x7effff31e, 0x7effff022, 0x7efffe434, 0x7efffbc87},
    // theta_1 (e^-x)
    {0x85fed0020, 0x84fcf0020, 0x83f8f001f, 0x82f0f001e, 0x81e0f001c, 0x80c0f0018, 0x7effc0020, 0x47481001dc, 0x47e808f8f7, 0x47ec065613, 0x47ee0356dc, 0x47ef014a09, 0x47ef7f8c2c, 0x47efbd8848, 0x47efda4fab, 0x47efe42dce, 0x47efe02942, 0x47efd04803}};

std::vector<uint64_t> neg_exp_knots_64_1 = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 0x11};

std::vector<std::vector<uint64_t>> neg_exp_coeffs_64_2 = {
    // theta_0 (e^-x)
    {0x7efff3e34, 0x7effe822a, 0x7effd8956, 0x7effc5488, 0x7effae4f1, 0x7eff93b73, 0x7eff758af, 0x7eff53de1, 0x7eff2ebdb, 0x7eff063bf, 0x7efeda5c8, 0x7efeab3a3, 0x7efe78da4, 0x7efe43494, 0x7efe0a98d, 0x7efdced8c, 0x7efd900c7, 0x7efd4e4d6, 0x7efd0999f, 0x7efcc20e5, 0x7efc77abd, 0x7efc2a8c9, 0x7efbdaaa2, 0x7efb8816f, 0x7efb32e7f, 0x7efadb217, 0x7efa80cbb, 0x7efa23eff, 0x7ef9c4b4e, 0x7ef963047},
    // theta_1 (e^-x)
    {0x47efb0b50b, 0x47ef91648a, 0x47ef72528d, 0x47ef537b11, 0x47ef34e401, 0x47ef168ade, 0x47eef86a67, 0x47eeda879d, 0x47eebce0a0, 0x47ee9f7780, 0x47ee824493, 0x47ee654fea, 0x47ee489388, 0x47ee2c0ec4, 0x47ee0fc378, 0x47edf3b240, 0x47edd7d630, 0x47edbc3465, 0x47eda0c66b, 0x47ed85920e, 0x47ed6a9282, 0x47ed4fcc3b, 0x47ed353845, 0x47ed1ad898, 0x47ed00af94, 0x47ece6ba68, 0x47ecccf7c6, 0x47ecb366ca, 0x47ec9a0e61, 0x47ec80e527}};

std::vector<uint64_t> neg_exp_knots_64_2 = {0x8, 0x10, 0x14, 0x18, 0x1c, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f};

std::vector<std::vector<uint64_t>> neg_exp_coeffs_64_3 = {
    // theta_0 (e^-x)
    {0x7ef8fef18, 0x7ef898960, 0x7ef82fe69, 0x7ef7c503d, 0x7ef757ee4, 0x7ef6e8af0, 0x7ef677475, 0x7ef603d8e, 0x7ef58e56c, 0x7ef516df6, 0x7ef49d701, 0x7ef422115, 0x7ef3a4cff, 0x7ef325abb, 0x7ef2a4bd4, 0x7ef22213b, 0x7ef19da5f, 0x7ef11778e, 0x7ef08fac1, 0x7ef006350, 0x7eef7b282, 0x7eeeee7a9, 0x7eee6052e, 0x7eedd0aa9, 0x7eed3f877, 0x7eecaced3, 0x7eec18edd, 0x7eeb83921, 0x7eeaecd69, 0x7eea54c94, 0x7ee9bb7a7, 0x7ee920db6, 0x7ee885090, 0x7ee7e7fa5, 0x7ee749cc0, 0x7ee6aa746, 0x7ee609ec8, 0x7ee568623, 0x7ee4c5cc2, 0x7ee4221f5, 0x7ee37d6d1, 0x7ee2d7c4c, 0x7ee231211, 0x7ee1898c9, 0x7ee0e114d, 0x7ee037b22, 0x7edf8d837, 0x7edee2749, 0x7ede36852, 0x7edd89d6b, 0x7edcdc691, 0x7edc2e3c7, 0x7edb7f45b, 0x7edacfaf4, 0x7eda1f62e, 0x7ed96e673, 0x7ed8cbf36},
    // theta_1 (e^-x)
    {0x47ec67ecfc, 0x47ec4f291e, 0x47ec3693d3, 0x47ec1e31e6, 0x47ec06008d, 0x47ebedff27, 0x47ebd62b78, 0x47ebbe8a09, 0x47eba715bf, 0x47eb8fd257, 0x47eb78bcf4, 0x47eb61d508, 0x47eb4b1ad8, 0x47eb348c53, 0x47eb1e2be1, 0x47eb07fa0f, 0x47eaf1f36d, 0x47eadc1704, 0x47eac66830, 0x47eab0e381, 0x47ea9b8a7e, 0x47ea8659dd, 0x47ea7155d5, 0x47ea5c7bd2, 0x47ea47cb23, 0x47ea3342e1, 0x47ea1ee41e, 0x47ea0aaea1, 0x47e9f6a096, 0x47e9e2ba88, 0x47e9cefd44, 0x47e9bb65a0, 0x47e9a7f60e, 0x47e994ac37, 0x47e9818a6f, 0x47e96e8e59, 0x47e95bb60e, 0x47e94905d1, 0x47e9367b66, 0x47e9241484, 0x47e911d21b, 0x47e8ffb4d8, 0x47e8edbb29, 0x47e8dbe53a, 0x47e8ca3389, 0x47e8b8a456, 0x47e8a739eb, 0x47e895f14b, 0x47e884c999, 0x47e873c555, 0x47e862e39a, 0x47e8522397, 0x47e8418371, 0x47e8310665, 0x47e820a990, 0x47e8106cd0, 0x47e801b187}};

std::vector<uint64_t> neg_exp_knots_64_3 = {0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58};

std::vector<std::vector<std::vector<uint64_t>>> neg_exp_imprecise_coeffs_64 = { neg_exp_coeffs_64_1, neg_exp_coeffs_64_2, neg_exp_coeffs_64_3 };
std::vector<std::vector<uint64_t>> neg_exp_imprecise_knots_64 = { neg_exp_knots_64_1, neg_exp_knots_64_2, neg_exp_knots_64_3 };

std::vector<std::vector<uint64_t>> neg_exp_coeffs_3_1 = {
    // theta_0 (e^-x)
    {0x7efffffa0, 0x7efffffaa, 0x7efffffaa, 0x7efffffb3, 0x7efffffb6, 0x7efffffbf, 0x7efffffc9, 0x7efffffc5, 0x7efffffcc, 0x7efffffcf, 0x7efffffcf, 0x7efffffcb, 0x7efffffbe, 0x7efffff94, 0x7effffef2, 0x7effffe0e, 0x7effffcec, 0x7effffb8c, 0x7effff9ed, 0x7effff810, 0x7effff5f7, 0x7effff38a, 0x7effff0f7, 0x7efffee13, 0x7efffeafe, 0x7efffe78d, 0x7efffe3ff, 0x7efffe010},
    // theta_1 (e^-x)
    {0x80e20001c, 0x7ef80001f, 0x479aa9a2ca, 0x47e948a3c2, 0x47ecdd3cda, 0x47ee985a49, 0x47ef630ad0, 0x47efad978f, 0x47efdab002, 0x47efedf977, 0x47eff6caef, 0x47effac7e6, 0x47effc3f55, 0x47effbf776, 0x47eff9f30e, 0x47eff7efe0, 0x47eff5ef76, 0x47eff3f2e6, 0x47eff1f705, 0x47efeffc84, 0x47efee0433, 0x47efebfa67, 0x47efea055c, 0x47efe803e5, 0x47efe60ba5, 0x47efe402d6, 0x47efe20de9, 0x47efe0074e}};

std::vector<uint64_t> neg_exp_knots_3_1 = { 0x8, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x74, 0x78, 0x7a, 0x7c, 0x7e, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87 };

std::vector<std::vector<uint64_t>> neg_exp_coeffs_3_2 = {
    // theta_0 (e^-x)
    {0x7efffdbff, 0x7efffd7a6, 0x7efffd30e, 0x7efffce48, 0x7efffc926, 0x7efffc3d9, 0x7efffbe42, 0x7efffb852, 0x7efffb248, 0x7efffabfc, 0x7efffa569, 0x7efff9ee2, 0x7efff97ab, 0x7efff9038, 0x7efff88f7, 0x7efff80ee, 0x7efff78ea, 0x7efff683e, 0x7efff56a6, 0x7efff43e3, 0x7efff2fe4, 0x7efff1bb3, 0x7efff063d, 0x7effeef8e, 0x7effed7c4, 0x7effebff0, 0x7effea62d, 0x7effe8bee, 0x7effe707f, 0x7effe5462, 0x7effe373b, 0x7effe1966, 0x7effdfa67, 0x7effdda06, 0x7effdb95e, 0x7effd9790, 0x7effd7476, 0x7effd50f4, 0x7effd2c63, 0x7effd062c, 0x7effcdf7a, 0x7effcb86d, 0x7effc9061, 0x7effc6771, 0x7effc3c73, 0x7effc11d6, 0x7effbe56f, 0x7effbb8ab, 0x7effb8ad7, 0x7effb5c32, 0x7effb2cdd, 0x7effafbe7, 0x7effaca59, 0x7effa97d1, 0x7effa644b, 0x7effa3046, 0x7eff9fc21, 0x7eff9c560, 0x7eff98e4c, 0x7eff956fa, 0x7eff91dcf, 0x7eff8e56e, 0x7eff8aa84, 0x7eff86fc7, 0x7eff8325e, 0x7eff7f5dc, 0x7eff7b7c8, 0x7eff778f3, 0x7eff739ac, 0x7eff6f8f2, 0x7eff6b73d, 0x7eff6745d, 0x7eff6329e, 0x7eff5eda0, 0x7eff5a939, 0x7eff5638e, 0x7eff51cb7, 0x7eff4d578, 0x7eff48cba, 0x7eff44444, 0x7eff3fa4b, 0x7eff3af58, 0x7eff3633a, 0x7eff316a7, 0x7eff2c863, 0x7eff27ac6, 0x7eff22b71, 0x7eff1db14, 0x7eff18ae4, 0x7eff138bb, 0x7eff0e687, 0x7eff092ca, 0x7eff03f39, 0x7efefe9cf, 0x7efef9460, 0x7efef3d63, 0x7efeee58a, 0x7efee8ef5, 0x7efee3448, 0x7efeddac3, 0x7efed7fc3, 0x7efed24a8, 0x7efecc8a6, 0x7efec6ac0, 0x7efec0c29, 0x7efebad78, 0x7efeb4e23, 0x7efeaed21, 0x7efea8c5e, 0x7efea2a71, 0x7efe9c798, 0x7efe963e3, 0x7efe8ffaf, 0x7efe89a82, 0x7efe833a9, 0x7efe7cc61, 0x7efe7647d, 0x7efe6fcd8, 0x7efe694be, 0x7efe628b1, 0x7efe5be2c, 0x7efe5549d, 0x7efe4e736, 0x7efe4780a, 0x7efe40bcb, 0x7efe39c92, 0x7efe32c52, 0x7efe2bca0},
    // theta_1 (e^-x)
    {0x47efde1013, 0x47efdc1471, 0x47efda18f6, 0x47efd8245b, 0x47efd62483, 0x47efd42c55, 0x47efd23064, 0x47efd02ba2, 0x47efce341c, 0x47efcc3b54, 0x47efca403f, 0x47efc85a97, 0x47efc6549c, 0x47efc45060, 0x47efc26a08, 0x47efc0607a, 0x47efbe6852, 0x47efba7dc3, 0x47efb69625, 0x47efb2a428, 0x47efaea5fe, 0x47efaaceee, 0x47efa6e8ff, 0x47efa2f805, 0x47ef9f026d, 0x47ef9b3372, 0x47ef973db7, 0x47ef935b6a, 0x47ef8f7135, 0x47ef8b9183, 0x47ef87af78, 0x47ef83d6d6, 0x47ef7ff786, 0x47ef7c0b9f, 0x47ef7834de, 0x47ef745879, 0x47ef7072cf, 0x47ef6c9cda, 0x47ef68c428, 0x47ef64d9a7, 0x47ef60fc20, 0x47ef5d2dbb, 0x47ef595e2d, 0x47ef558fd0, 0x47ef51a803, 0x47ef4ddf0a, 0x47ef4a035d, 0x47ef4634d0, 0x47ef4263d4, 0x47ef3e95b9, 0x47ef3accea, 0x47ef36f671, 0x47ef332744, 0x47ef2f57aa, 0x47ef2b876a, 0x47ef27bfa6, 0x47ef2406b3, 0x47ef202fa5, 0x47ef1c63f3, 0x47ef18a470, 0x47ef14d57b, 0x47ef1123a2, 0x47ef0d571a, 0x47ef099cd3, 0x47ef05c6d5, 0x47ef020e48, 0x47eefe4bad, 0x47eefa8bfc, 0x47eef6d3ab, 0x47eef3139e, 0x47eeef5302, 0x47eeeb8f2d, 0x47eee7e8a3, 0x47eee42163, 0x47eee06f67, 0x47eedcb8b1, 0x47eed8feb3, 0x47eed54bb6, 0x47eed19141, 0x47eecde680, 0x47eeca3410, 0x47eec680f8, 0x47eec2cae3, 0x47eebf1a9a, 0x47eebb610c, 0x47eeb7ba8e, 0x47eeb40a6f, 0x47eeb0591d, 0x47eeacb483, 0x47eea903b2, 0x47eea55ceb, 0x47eea1af24, 0x47ee9e0d84, 0x47ee9a618b, 0x47ee96bf54, 0x47ee931627, 0x47ee8f6da3, 0x47ee8bdc0b, 0x47ee882900, 0x47ee848b93, 0x47ee80e833, 0x47ee7d4cf5, 0x47ee79b1b3, 0x47ee760c9d, 0x47ee7269bc, 0x47ee6ecec9, 0x47ee6b364c, 0x47ee67966d, 0x47ee640179, 0x47ee6069f2, 0x47ee5cd234, 0x47ee593ad8, 0x47ee55a721, 0x47ee5212b3, 0x47ee4e774f, 0x47ee4ae025, 0x47ee474b95, 0x47ee43c0f2, 0x47ee4039f3, 0x47ee3c98a0, 0x47ee390c29, 0x47ee358f40, 0x47ee31f95e, 0x47ee2e5c56, 0x47ee2adeea, 0x47ee27503d, 0x47ee23c07f, 0x47ee203c63}};

std::vector<uint64_t> neg_exp_knots_3_2 = {0x4, 0x8, 0xc, 0x10, 0x14, 0x18, 0x1c, 0x20, 0x24, 0x28, 0x2c, 0x30, 0x34, 0x38, 0x3c, 0x40, 0x44, 0x48, 0x4c, 0x50, 0x54, 0x58, 0x5c, 0x60, 0x64, 0x68, 0x6c, 0x70, 0x74, 0x78, 0x7c, 0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e, 0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

std::vector<std::vector<uint64_t>> neg_exp_coeffs_3_3 = {
    // theta_0 (e^-x)
    {0x7efe24c02, 0x7efe1d9e0, 0x7efe16779, 0x7efe0f4fa, 0x7efe0812a, 0x7efe00b0e, 0x7efdf952e, 0x7efdf1fc9, 0x7efdea7ca, 0x7efde2f15, 0x7efddb788, 0x7efdd3ed7, 0x7efdcc38b, 0x7efdc4776, 0x7efdbcc6a, 0x7efdb50c1, 0x7efdad3e1, 0x7efda55bd, 0x7efd9d673, 0x7efd9574c, 0x7efd8d736, 0x7efd855e2, 0x7efd7d4ac, 0x7efd7522f, 0x7efd6cfc9, 0x7efd64b98, 0x7efd5c671, 0x7efd542c9, 0x7efd4bcd2, 0x7efd434b3, 0x7efd3ad2f, 0x7efd32534, 0x7efd2985b, 0x7efd21245, 0x7efd18664, 0x7efd0fd03, 0x7efd0705c, 0x7efcfe3a7, 0x7efcf587a, 0x7efcecac3, 0x7efce3a50, 0x7efcda939, 0x7efcd1be4, 0x7efcc893f, 0x7efcbfafc, 0x7efcb65db, 0x7efcad217, 0x7efca407f, 0x7efc9abb3, 0x7efc916cc, 0x7efc87f75, 0x7efc7ec5d, 0x7efc7535d, 0x7efc6bb72, 0x7efc622a1, 0x7efc58c0b, 0x7efc4f164, 0x7efc454df, 0x7efc3ba51, 0x7efc31e7a, 0x7efc2809f, 0x7efc1e44b, 0x7efc14667, 0x7efc0a7ee, 0x7efc008d2, 0x7efbf68b1, 0x7efbec879, 0x7efbe266f, 0x7efbd84d1, 0x7efbce21b, 0x7efbc3e1d, 0x7efbb9a98, 0x7efbaf729, 0x7efba4f50, 0x7efb9aa40, 0x7efb902c6, 0x7efb85c73, 0x7efb7b3ff, 0x7efb70d58, 0x7efb66162, 0x7efb5b9c1, 0x7efb50e43, 0x7efb46496, 0x7efb3b6c4, 0x7efb30c84, 0x7efb25d41, 0x7efb1b003, 0x7efb10261, 0x7efb05442, 0x7efafa476, 0x7efaef2b5, 0x7efae3f1e, 0x7efad8fed, 0x7efacdf67, 0x7efac2de7, 0x7efab79ce, 0x7efaac47b, 0x7efaa0ecf, 0x7efa95a22, 0x7efa8a225, 0x7efa7ee1c, 0x7efa73378, 0x7efa67d64, 0x7efa5c737, 0x7efa50c25, 0x7efa45361, 0x7efa3991f, 0x7efa2dbfc, 0x7efa22312, 0x7efa15e94, 0x7efa0a68b, 0x7ef9feb05, 0x7ef9f2cb6, 0x7ef9e709e, 0x7ef9db0a7, 0x7ef9cf019, 0x7ef9c30eb, 0x7ef9b6ef9, 0x7ef9aac76, 0x7ef99eb71, 0x7ef9929b6, 0x7ef98646b, 0x7ef979fc5, 0x7ef96df8f, 0x7ef961656, 0x7ef954f90, 0x7ef948bcf, 0x7ef93c2fa},
    // theta_1 (e^-x)
    {0x47ee1cb7e5, 0x47ee192e5e, 0x47ee15a9b2, 0x47ee122b1f, 0x47ee0ea918, 0x47ee0b1c27, 0x47ee0797d1, 0x47ee041dca, 0x47ee0096b7, 0x47edfd10fa, 0x47edf99a50, 0x47edf6219c, 0x47edf29c2e, 0x47edef1784, 0x47edeba0ac, 0x47ede82b97, 0x47ede4b403, 0x47ede1399b, 0x47edddbd3d, 0x47edda47ef, 0x47edd6d226, 0x47edd359c2, 0x47edcfe818, 0x47edcc739f, 0x47edc90586, 0x47edc59131, 0x47edc21c3b, 0x47edbeb6d2, 0x47edbb47cc, 0x47edb7d05a, 0x47edb4627b, 0x47edb0f71c, 0x47edad7229, 0x47edaa1dcd, 0x47eda6aa0e, 0x47eda34b6c, 0x47ed9fdd9c, 0x47ed9c74c9, 0x47ed991aa3, 0x47ed95b602, 0x47ed9245ee, 0x47ed8ed739, 0x47ed8b844f, 0x47ed88168d, 0x47ed84c7f5, 0x47ed81559c, 0x47ed7df06b, 0x47ed7a9ce0, 0x47ed773b86, 0x47ed73de76, 0x47ed70783a, 0x47ed6d2f20, 0x47ed69c913, 0x47ed666dfc, 0x47ed631294, 0x47ed5fc84c, 0x47ed5c6bf5, 0x47ed5909f6, 0x47ed55b77b, 0x47ed52627a, 0x47ed4f070f, 0x47ed4bb87b, 0x47ed4865e9, 0x47ed4514ac, 0x47ed41c475, 0x47ed3e733c, 0x47ed3b25e8, 0x47ed37d35f, 0x47ed348760, 0x47ed3139fc, 0x47ed2dea40, 0x47ed2aa135, 0x47ed275cc8, 0x47ed24060f, 0x47ed20c1b3, 0x47ed1d754c, 0x47ed1a32cb, 0x47ed16e9bd, 0x47ed13adb5, 0x47ed105b6c, 0x47ed0d228b, 0x47ed09daaf, 0x47ed069fae, 0x47ed03547b, 0x47ed001e7f, 0x47ecfcd449, 0x47ecf997b3, 0x47ecf65d14, 0x47ecf323f5, 0x47ecefe6ba, 0x47ececa421, 0x47ece95ccc, 0x47ece62dc3, 0x47ece2fc2f, 0x47ecdfc9c3, 0x47ecdc8f13, 0x47ecd9527f, 0x47ecd6180c, 0x47ecd2e5c4, 0x47eccfa828, 0x47eccc7feb, 0x47ecc93db5, 0x47ecc61372, 0x47ecc2ec39, 0x47ecbfb2dd, 0x47ecbc8733, 0x47ecb95873, 0x47ecb620a8, 0x47ecb2fead, 0x47ecafae1c, 0x47ecac96b6, 0x47eca973a3, 0x47eca64806, 0x47eca32924, 0x47ec9ffd2f, 0x47ec9cd21b, 0x47ec99b01b, 0x47ec9685d1, 0x47ec935c7e, 0x47ec903c9f, 0x47ec8d1d08, 0x47ec89f200, 0x47ec86cce0, 0x47ec83bd09, 0x47ec808baf, 0x47ec7d6787, 0x47ec7a52b0, 0x47ec772c8a}};

std::vector<uint64_t> neg_exp_knots_3_3 = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, };

std::vector<std::vector<uint64_t>> neg_exp_coeffs_3_4 = {
    // theta_0 (e^-x)
    {0x7ef92fa6d, 0x7ef9232df, 0x7ef916cb9, 0x7ef90a0e6, 0x7ef8fd875, 0x7ef8f0fb7, 0x7ef8e43b0, 0x7ef8d7694, 0x7ef8caa79, 0x7ef8bdde2, 0x7ef8b1149, 0x7ef8a43fc, 0x7ef897325, 0x7ef88a556, 0x7ef87d703, 0x7ef87043c, 0x7ef863507, 0x7ef856297, 0x7ef849200, 0x7ef83bdd5, 0x7ef82ee66, 0x7ef821a4e, 0x7ef81453a, 0x7ef8072d7, 0x7ef7f9acd, 0x7ef7ec52e, 0x7ef7decef, 0x7ef7d1a54, 0x7ef7c4195, 0x7ef7b69f1, 0x7ef7a903b, 0x7ef79b6c5, 0x7ef78e00c, 0x7ef7802d7, 0x7ef772718, 0x7ef764c8f, 0x7ef757050, 0x7ef749730, 0x7ef73b72b, 0x7ef72d799, 0x7ef71fbbb, 0x7ef711e06, 0x7ef703fa3, 0x7ef6f6551, 0x7ef6e8288, 0x7ef6da225, 0x7ef6cbfdd, 0x7ef6bdf1c, 0x7ef6afc9f, 0x7ef6a1a0b, 0x7ef6936c4, 0x7ef6851db, 0x7ef676eeb, 0x7ef668d30, 0x7ef65a459, 0x7ef64c247, 0x7ef63d858, 0x7ef62f237, 0x7ef620bf5, 0x7ef612663, 0x7ef6039ec, 0x7ef5f53b7, 0x7ef5e6c34, 0x7ef5d7e9e, 0x7ef5c9372, 0x7ef5ba437, 0x7ef5abd09, 0x7ef59d293, 0x7ef58e816, 0x7ef57fad0, 0x7ef570d1f, 0x7ef561fe0, 0x7ef55339c, 0x7ef544143, 0x7ef5351e8, 0x7ef526612, 0x7ef51736c, 0x7ef508213, 0x7ef4f91ee, 0x7ef4ea1f7, 0x7ef4dadcc, 0x7ef4cba0c, 0x7ef4bca90, 0x7ef4ad6a9, 0x7ef49e20c, 0x7ef48f03c, 0x7ef47f54c, 0x7ef4705c4, 0x7ef460da9, 0x7ef4514bb, 0x7ef441807, 0x7ef432861, 0x7ef422d87, 0x7ef413504, 0x7ef403cea, 0x7ef3f4261, 0x7ef3e490e, 0x7ef3d4ff0, 0x7ef3c5511, 0x7ef3b59e3, 0x7ef3a5dc0, 0x7ef396462, 0x7ef3865b2, 0x7ef376951, 0x7ef366ef4, 0x7ef356e68, 0x7ef347116, 0x7ef337184, 0x7ef32703a, 0x7ef317144, 0x7ef307001, 0x7ef2f6d5c, 0x7ef2e6e45, 0x7ef2d6eba, 0x7ef2c6b81, 0x7ef2b6a1d, 0x7ef2a6254, 0x7ef296278, 0x7ef285f0a, 0x7ef275b76, 0x7ef26570b, 0x7ef255357, 0x7ef244b9f, 0x7ef23462e, 0x7ef223fee, 0x7ef213a0e, 0x7ef2031db, 0x7ef1f2a6d, 0x7ef1e24a1, 0x7ef1d199b, 0x7ef1c1148, 0x7ef1b0b44, 0x7ef19fb96, 0x7ef18f617, 0x7ef17ea80, 0x7ef16dfcb, 0x7ef15d16f, 0x7ef14c9a9, 0x7ef13b93d, 0x7ef12b07d, 0x7ef11a138, 0x7ef1091c9, 0x7ef0f85c4, 0x7ef0e7576, 0x7ef0d6a3a, 0x7ef0c582e, 0x7ef0b48e6, 0x7ef0a393b, 0x7ef09258d, 0x7ef0817bb, 0x7ef070227, 0x7ef05f35c, 0x7ef04e40f, 0x7ef03cd41, 0x7ef02bc52, 0x7ef01a671, 0x7ef0094b8, 0x7eeff7f84, 0x7eefe6f99, 0x7eefd5b2d, 0x7eefc43b2, 0x7eefb2baa, 0x7eefa16f3, 0x7eef8fe85, 0x7eef7eb6b, 0x7eef6d09c, 0x7eef5bcc0, 0x7eef4a278, 0x7eef38800, 0x7eef27348, 0x7eef15564, 0x7eef03ec1, 0x7eeef23ea, 0x7eeee0a3e, 0x7eeecef71, 0x7eeebd1c7, 0x7eeeabb40, 0x7eee99cf7, 0x7eee88017, 0x7eee765bb, 0x7eee64413, 0x7eee52909, 0x7eee40bdc, 0x7eee2ef26, 0x7eee1cfdf, 0x7eee0b063, 0x7eedf9644, 0x7eede7282, 0x7eedd4f73, 0x7eedc3089, 0x7eedb0f6c, 0x7eed9ec2c, 0x7eed8cfde, 0x7eed7ae70, 0x7eed68fb2, 0x7eed56984, 0x7eed440eb, 0x7eed31dc5, 0x7eed1fddc, 0x7eed0d86d, 0x7eecfb74a, 0x7eece8fb2, 0x7eecd6f11, 0x7eecc4500, 0x7eecb232d, 0x7eec9fdd8, 0x7eec8d875, 0x7eec7af4f, 0x7eec685e5, 0x7eec55daf, 0x7eec43954, 0x7eec30e2e, 0x7eec1e606, 0x7eec0c02a, 0x7eebf922d, 0x7eebe6910, 0x7eebd3f9d, 0x7eebc1805, 0x7eebaeaf0, 0x7eeb9bfbf, 0x7eeb89460, 0x7eeb76b5e, 0x7eeb63cc7, 0x7eeb50fc3, 0x7eeb3e993, 0x7eeb2b5cb, 0x7eeb1875a, 0x7eeb05ce8, 0x7eeaf30b9, 0x7eeadfe81, 0x7eeacd396, 0x7eeaba584, 0x7eeaa7796, 0x7eea945cc, 0x7eea81acd, 0x7eea6e635, 0x7eea5b476, 0x7eea4857c, 0x7eea3537c, 0x7eea222d9, 0x7eea0f25a, 0x7ee9fbcd8, 0x7ee9e8944, 0x7ee9d59c6, 0x7ee9c269b, 0x7ee9aef5d, 0x7ee99bec1, 0x7ee9886ad, 0x7ee975707, 0x7ee9621d5, 0x7ee94ebe7, 0x7ee93b8ae, 0x7ee92822c, 0x7ee914cde, 0x7ee901919, 0x7ee8ee5d8},
    // theta_1 (e^-x)
    {0x47ec740ab4, 0x47ec70eff3, 0x47ec6dddd7, 0x47ec6ab855, 0x47ec67a33f, 0x47ec648ffa, 0x47ec6172e2, 0x47ec5e549d, 0x47ec5b3d49, 0x47ec58270f, 0x47ec5513c8, 0x47ec5200bb, 0x47ec4ee31f, 0x47ec4bd409, 0x47ec48c5dc, 0x47ec45a9c3, 0x47ec429e09, 0x47ec3f890d, 0x47ec3c7dd9, 0x47ec39682a, 0x47ec3666e4, 0x47ec335726, 0x47ec30469f, 0x47ec2d42c5, 0x47ec2a2d00, 0x47ec2722e3, 0x47ec241201, 0x47ec21184a, 0x47ec1e0b1b, 0x47ec1b04a4, 0x47ec17f979, 0x47ec14f1ed, 0x47ec11f6cc, 0x47ec0ee756, 0x47ec0bdfc3, 0x47ec08df1d, 0x47ec05db1a, 0x47ec02e49b, 0x47ebffd8b3, 0x47ebfcd104, 0x47ebf9d8c5, 0x47ebf6dcb7, 0x47ebf3e0e3, 0x47ebf0f581, 0x47ebedefb9, 0x47ebeaf4ae, 0x47ebe7f5bd, 0x47ebe4fe81, 0x47ebe203ea, 0x47ebdf0b89, 0x47ebdc134c, 0x47ebd9181b, 0x47ebd625f2, 0x47ebd33a32, 0x47ebd03965, 0x47ebcd5158, 0x47ebca51e1, 0x47ebc76153, 0x47ebc472be, 0x47ebc188c3, 0x47ebbe8ad0, 0x47ebbba37e, 0x47ebb8ba4e, 0x47ebb5bff6, 0x47ebb2cfbe, 0x47ebafd4f5, 0x47ebacf621, 0x47ebaa0f28, 0x47eba72a5e, 0x47eba43f16, 0x47eba154cc, 0x47eb9e6e2b, 0x47eb9b8cdb, 0x47eb989ae1, 0x47eb95b479, 0x47eb92db2e, 0x47eb8fef13, 0x47eb8d093a, 0x47eb8a2949, 0x47eb874c15, 0x47eb846442, 0x47eb817feb, 0x47eb7eaaae, 0x47eb7bca38, 0x47eb78e9ca, 0x47eb7613ee, 0x47eb7324e1, 0x47eb705a10, 0x47eb6d77cb, 0x47eb6a953d, 0x47eb67a9b1, 0x47eb64e6ca, 0x47eb6204f6, 0x47eb5f2c1d, 0x47eb5c568d, 0x47eb597bf5, 0x47eb56a6f6, 0x47eb53d49c, 0x47eb50ff3c, 0x47eb4e2b00, 0x47eb4b560f, 0x47eb488b16, 0x47eb45b2e3, 0x47eb42e34f, 0x47eb401b71, 0x47eb3d4404, 0x47eb3a77ba, 0x47eb37a710, 0x47eb34d38f, 0x47eb320892, 0x47eb2f391c, 0x47eb2c67c1, 0x47eb29a23f, 0x47eb26dd66, 0x47eb241047, 0x47eb214a25, 0x47eb1e7458, 0x47eb1bb63c, 0x47eb18f03a, 0x47eb162bb3, 0x47eb1366c4, 0x47eb10a5a7, 0x47eb0ddb8a, 0x47eb0b1975, 0x47eb08570d, 0x47eb059782, 0x47eb02d394, 0x47eb001390, 0x47eafd59b6, 0x47eafa93c8, 0x47eaf7d6dd, 0x47eaf521d8, 0x47eaf2551e, 0x47eaefa50a, 0x47eaece6b9, 0x47eaea2c82, 0x47eae76a8f, 0x47eae4bb8c, 0x47eae1f7ce, 0x47eadf49bf, 0x47eadc8c8d, 0x47ead9d0b7, 0x47ead71f6c, 0x47ead464dc, 0x47ead1b903, 0x47eacefd7a, 0x47eacc4abd, 0x47eac998b9, 0x47eac6de44, 0x47eac43456, 0x47eac17888, 0x47eabecf84, 0x47eabc26e3, 0x47eab96d22, 0x47eab6c3c0, 0x47eab40fb8, 0x47eab167af, 0x47eaaeb8ae, 0x47eaac1860, 0x47eaa96e94, 0x47eaa6bee5, 0x47eaa40f87, 0x47eaa169ed, 0x47ea9ebcdd, 0x47ea9c1e6b, 0x47ea996ed3, 0x47ea96d1b7, 0x47ea9426a3, 0x47ea917cbc, 0x47ea8ee249, 0x47ea8c335c, 0x47ea899770, 0x47ea86f303, 0x47ea8452f4, 0x47ea81b1c6, 0x47ea7f0b5b, 0x47ea7c776a, 0x47ea79d2a7, 0x47ea7732c9, 0x47ea749a60, 0x47ea71f25c, 0x47ea6f5b69, 0x47ea6cc0fc, 0x47ea6a2923, 0x47ea678cd7, 0x47ea64f193, 0x47ea626439, 0x47ea5fc215, 0x47ea5d2319, 0x47ea5a8f26, 0x47ea57f7a2, 0x47ea555cb7, 0x47ea52d327, 0x47ea503f56, 0x47ea4db316, 0x47ea4b1765, 0x47ea4877be, 0x47ea45e5e5, 0x47ea435cca, 0x47ea40c8ad, 0x47ea3e3fa7, 0x47ea3ba996, 0x47ea392488, 0x47ea368bd5, 0x47ea3406ec, 0x47ea317b9c, 0x47ea2ef195, 0x47ea2c60a5, 0x47ea29d08e, 0x47ea274485, 0x47ea24c25b, 0x47ea2232ac, 0x47ea1faaf1, 0x47ea1d299a, 0x47ea1a97ec, 0x47ea18123e, 0x47ea158d34, 0x47ea130d88, 0x47ea10835c, 0x47ea0dfea4, 0x47ea0b7ae3, 0x47ea08fd8c, 0x47ea067598, 0x47ea03f25d, 0x47ea017f05, 0x47e9fef00d, 0x47e9fc6dcd, 0x47e9f9f558, 0x47e9f77a81, 0x47e9f4f43f, 0x47e9f27eb1, 0x47e9f003cc, 0x47e9ed8a80, 0x47e9eb0a6a, 0x47e9e899d1, 0x47e9e6167d, 0x47e9e39a66, 0x47e9e1255b, 0x47e9deab51, 0x47e9dc3560, 0x47e9d9c0fd, 0x47e9d7438d, 0x47e9d4cb5e, 0x47e9d25cd6, 0x47e9cfe802, 0x47e9cd6c20, 0x47e9caff08, 0x47e9c883fa, 0x47e9c61b4e, 0x47e9c3a89a, 0x47e9c135a8, 0x47e9bec967, 0x47e9bc57c2, 0x47e9b9e9c2, 0x47e9b7800d, 0x47e9b51895}};

std::vector<uint64_t> neg_exp_knots_3_4 = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

std::vector<std::vector<uint64_t>> neg_exp_coeffs_3_5 = {
    // theta_0 (e^-x)
    {0x7ee8daa22, 0x7ee8c744d, 0x7ee8b393f, 0x7ee8a0157, 0x7ee88c630, 0x7ee87901b, 0x7ee865d50, 0x7ee852640, 0x7ee83eb53, 0x7ee82ad66, 0x7ee81776a, 0x7ee803ab9, 0x7ee7f002a, 0x7ee7dc87b, 0x7ee7c8e73, 0x7ee7b5542, 0x7ee7a12ff, 0x7ee78d60c, 0x7ee779d09, 0x7ee766420, 0x7ee75243e, 0x7ee73e53c, 0x7ee72a75f, 0x7ee716bbb, 0x7ee70311c, 0x7ee6ef0f7, 0x7ee6daec9, 0x7ee6c7372, 0x7ee6b35bd, 0x7ee69f654, 0x7ee68b57c, 0x7ee677b8a, 0x7ee6635bd, 0x7ee64f317, 0x7ee63b675, 0x7ee6279f2, 0x7ee61330a, 0x7ee5fef03, 0x7ee5ead59, 0x7ee5d6cdc, 0x7ee5c2939, 0x7ee5aec8a, 0x7ee59a947, 0x7ee58651e, 0x7ee57250c, 0x7ee55dc86, 0x7ee549aa4, 0x7ee53556a, 0x7ee521504, 0x7ee50cf1d, 0x7ee4f8a21, 0x7ee4e424f, 0x7ee4cfab4, 0x7ee4bbc99, 0x7ee4a7161, 0x7ee492dca, 0x7ee47e34b, 0x7ee46a220, 0x7ee455574, 0x7ee44102b, 0x7ee42cabf, 0x7ee418156, 0x7ee403f25, 0x7ee3ef6f8, 0x7ee3da6bd, 0x7ee3c64d1, 0x7ee3b1ac2, 0x7ee39d2f5, 0x7ee388347, 0x7ee37414c, 0x7ee35f406, 0x7ee34a707, 0x7ee336067, 0x7ee321127, 0x7ee30cced, 0x7ee2f7c7d, 0x7ee2e2da1, 0x7ee2ce6e0, 0x7ee2b98db, 0x7ee2a5156, 0x7ee2905b0, 0x7ee27b09d, 0x7ee266aa9, 0x7ee251a07, 0x7ee23d20f, 0x7ee228328, 0x7ee2131d3, 0x7ee1fe90b, 0x7ee1e97ba, 0x7ee1d4b5d, 0x7ee1bfe5a, 0x7ee1aacd1, 0x7ee195c61, 0x7ee180ce7, 0x7ee16bf4c, 0x7ee156ab3, 0x7ee141bb5, 0x7ee12d155, 0x7ee117f3e, 0x7ee102bae, 0x7ee0edfe1, 0x7ee0d8a73, 0x7ee0c3bd9, 0x7ee0aeb21, 0x7ee0994e0, 0x7ee08475a, 0x7ee06f47a, 0x7ee05a424, 0x7ee045033, 0x7ee02f92e, 0x7ee01aa02, 0x7ee00527c, 0x7edff0016, 0x7edfdac3e, 0x7edfc5c13, 0x7edfb06ac, 0x7edf9b287, 0x7edf85979, 0x7edf7068f, 0x7edf5b4b0, 0x7edf45c67, 0x7edf30aec, 0x7edf1b1e2, 0x7edf05c04, 0x7edef0600, 0x7ededb2ac, 0x7edec5e80, 0x7edeb0450, 0x7ede9a90a, 0x7ede8577e, 0x7ede705b2, 0x7ede5aa1c, 0x7ede44d9c, 0x7ede2f6f8, 0x7ede1a280, 0x7ede0480b, 0x7eddef150, 0x7eddd9838, 0x7eddc3d66, 0x7eddae80d, 0x7edd9820e, 0x7edd82ff0, 0x7edd6d8a6, 0x7edd58143, 0x7edd426f3, 0x7edd2cf73, 0x7edd17645, 0x7edd01e75, 0x7edceb4d9, 0x7edcd6046, 0x7edcc08c6, 0x7edcaacdf, 0x7edc94e1b, 0x7edc7f875, 0x7edc69a44, 0x7edc53f8b, 0x7edc3e136, 0x7edc287ea, 0x7edc126d0, 0x7edbfcb49, 0x7edbe6c6d, 0x7edbd1356, 0x7edbbb93b, 0x7edba52ef, 0x7edb8f936, 0x7edb79cd6, 0x7edb6443a, 0x7edb4e3c7, 0x7edb383a8, 0x7edb22325, 0x7edb0c71d, 0x7edaf6624, 0x7edae01f1, 0x7edaca397, 0x7edab4afe, 0x7eda9e7e6, 0x7eda88b9f, 0x7eda72736, 0x7eda5c589, 0x7eda4663d, 0x7eda307fb, 0x7eda1a7ee, 0x7eda0456f, 0x7ed9ee577, 0x7ed9d8720, 0x7ed9c22ef, 0x7ed9ac3a6, 0x7ed995fa5, 0x7ed980434, 0x7ed96a165, 0x7ed953b6a, 0x7ed93df19, 0x7ed927a06, 0x7ed9114c3, 0x7ed8fb34d, 0x7ed8e5220, 0x7ed8cead5},
    // theta_1 (e^-x)
    {0x47e9b2a169, 0x47e9b03734, 0x47e9adc3cd, 0x47e9ab57e3, 0x47e9a8e6bd, 0x47e9a680d7, 0x47e9a4229d, 0x47e9a1bd20, 0x47e99f5132, 0x47e99ce096, 0x47e99a80bf, 0x47e99814ef, 0x47e995ae7b, 0x47e9934ecd, 0x47e990ebb2, 0x47e98e8b63, 0x47e98c1aa4, 0x47e989b560, 0x47e98758df, 0x47e984fdaf, 0x47e9829638, 0x47e9803198, 0x47e97dd052, 0x47e97b746f, 0x47e9791b9c, 0x47e976b960, 0x47e974546b, 0x47e971fd95, 0x47e96fa35d, 0x47e96d470a, 0x47e96ae91d, 0x47e968994f, 0x47e9663448, 0x47e963d651, 0x47e96184bc, 0x47e95f3473, 0x47e95cd1d5, 0x47e95a75b6, 0x47e9581f17, 0x47e955cbbc, 0x47e95373a0, 0x47e9512976, 0x47e94ed432, 0x47e94c7e58, 0x47e94a311a, 0x47e947d561, 0x47e94586ee, 0x47e9433369, 0x47e940e9c7, 0x47e93e9724, 0x47e93c473d, 0x47e939f33c, 0x47e937a0af, 0x47e9356064, 0x47e9330965, 0x47e930c13c, 0x47e92e6da8, 0x47e92c2be9, 0x47e929d683, 0x47e9278f6a, 0x47e9254915, 0x47e922fca3, 0x47e920be17, 0x47e91e75e2, 0x47e91c2062, 0x47e919e54e, 0x47e9179ccb, 0x47e9155949, 0x47e91308e1, 0x47e910d1a1, 0x47e90e877b, 0x47e90c3ed6, 0x47e90a025c, 0x47e907b7c5, 0x47e905816f, 0x47e90336c5, 0x47e900efdc, 0x47e8feb807, 0x47e8fc748c, 0x47e8fa3d4d, 0x47e8f7ffde, 0x47e8f5b325, 0x47e8f38177, 0x47e8f13e5c, 0x47e8ef0b1a, 0x47e8eccce5, 0x47e8ea8b7e, 0x47e8e859ab, 0x47e8e61a36, 0x47e8e3e427, 0x47e8e1aded, 0x47e8df70f7, 0x47e8dd36d0, 0x47e8daff3c, 0x47e8d8cbc0, 0x47e8d68d69, 0x47e8d45974, 0x47e8d22e2e, 0x47e8cff6db, 0x47e8cdbe00, 0x47e8cb930f, 0x47e8c958f1, 0x47e8c72b23, 0x47e8c4fab7, 0x47e8c2c201, 0x47e8c098b4, 0x47e8be676d, 0x47e8bc3b41, 0x47e8ba0a06, 0x47e8b7d4a2, 0x47e8b5ad12, 0x47e8b378aa, 0x47e8b14d93, 0x47e8af2102, 0x47e8acfb56, 0x47e8aacdfe, 0x47e8a8a396, 0x47e8a67213, 0x47e8a44b70, 0x47e8a2275c, 0x47e89ff9c3, 0x47e89dd80b, 0x47e89baafc, 0x47e89983ec, 0x47e8975d7c, 0x47e8953c3b, 0x47e8931a78, 0x47e890efec, 0x47e88ec485, 0x47e88ca983, 0x47e88a8eef, 0x47e888659d, 0x47e8863bb4, 0x47e8841bf5, 0x47e882007d, 0x47e87fdc61, 0x47e87dbf02, 0x47e87b9ebc, 0x47e8797c96, 0x47e87763e0, 0x47e87531de, 0x47e8731fe3, 0x47e871069c, 0x47e86eedfe, 0x47e86cd1a2, 0x47e86aba78, 0x47e868a180, 0x47e8668b79, 0x47e8645aa9, 0x47e8624b49, 0x47e860382b, 0x47e85e1f0d, 0x47e85c0256, 0x47e859f478, 0x47e857da40, 0x47e855c62c, 0x47e853ad5a, 0x47e8519d0a, 0x47e84f8199, 0x47e84d6f76, 0x47e84b590b, 0x47e8494c32, 0x47e8473e92, 0x47e8451f3e, 0x47e84313c4, 0x47e841050c, 0x47e83efcca, 0x47e83ce977, 0x47e83ad76d, 0x47e838c595, 0x47e836bb3d, 0x47e834aa41, 0x47e8329539, 0x47e83089b7, 0x47e82e878b, 0x47e82c767c, 0x47e82a7055, 0x47e8285eda, 0x47e8265233, 0x47e82449d5, 0x47e82243c1, 0x47e8203bc6, 0x47e81e30ee, 0x47e81c2a91, 0x47e81a2758, 0x47e8181c43, 0x47e8161921, 0x47e8140fd2, 0x47e81213c7, 0x47e8100db2, 0x47e80e03bb, 0x47e80c0894, 0x47e80a0169, 0x47e807fab6, 0x47e805fa42, 0x47e803faf1, 0x47e801f382}};

std::vector<uint64_t> neg_exp_knots_3_5 = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4};

std::vector<std::vector<std::vector<uint64_t>>> neg_exp_imprecise_coeffs_3 = { neg_exp_coeffs_3_1, neg_exp_coeffs_3_2, neg_exp_coeffs_3_3, neg_exp_coeffs_3_4, neg_exp_coeffs_3_5 };
std::vector<std::vector<uint64_t>> neg_exp_imprecise_knots_3 = { neg_exp_knots_3_1, neg_exp_knots_3_2, neg_exp_knots_3_3, neg_exp_knots_3_4, neg_exp_knots_3_5 };

const std::vector<uint64_t> neg_exp_bfloat16 = { 16256, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16245, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16244, 16243, 16243, 16243, 16243, 16243, 16243, 16243, 16243, 16243, 16243, 16243, 16243, 16243, 16243, 16243, 16243, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16242, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16241, 16240, 16240, 16240, 16240, 16240, 16240, 16240, 16240, 16240, 16239, 16239, 16239, 16239, 16239, 16239, 16239, 16239, 16239, 16238, 16238, 16238, 16238, 16238, 16238, 16238, 16238, 16237, 16237, 16237, 16237, 16237, 16237, 16237, 16237, 16237, 16236, 16236, 16236, 16236, 16236, 16236, 16236, 16236, 16235, 16235, 16235, 16235, 16235, 16235, 16235, 16235, 16235, 16234, 16234, 16234, 16234, 16234, 16234, 16234, 16234, 16234, 16233, 16233, 16233, 16233, 16233, 16233, 16233, 16233, 16233, 16232, 16232, 16232, 16232, 16232, 16232, 16232, 16232, 16232, 16231, 16231, 16231, 16231, 16231, 16231, 16231, 16231, 16230, 16230, 16230, 16230, 16230, 16230, 16230, 16230, 16230, 16229, 16229, 16229, 16229, 16229, 16229, 16229, 16229, 16229, 16228, 16228, 16228, 16228, 16228, 16228, 16228, 16228, 16228, 16227, 16227, 16227, 16227, 16227, 16227, 16227, 16227, 16227, 16226, 16226, 16226, 16226, 16226, 16226, 16226, 16225, 16225, 16225, 16225, 16225, 16224, 16224, 16224, 16224, 16224, 16223, 16223, 16223, 16223, 16222, 16222, 16222, 16222, 16222, 16221, 16221, 16221, 16221, 16220, 16220, 16220, 16220, 16220, 16219, 16219, 16219, 16219, 16219, 16218, 16218, 16218, 16218, 16217, 16217, 16217, 16217, 16217, 16216, 16216, 16216, 16216, 16216, 16215, 16215, 16215, 16215, 16215, 16214, 16214, 16214, 16214, 16213, 16213, 16213, 16213, 16213, 16212, 16212, 16212, 16212, 16212, 16211, 16211, 16211, 16211, 16211, 16210, 16210, 16210, 16210, 16210, 16209, 16209, 16209, 16209, 16209, 16208, 16208, 16208, 16208, 16208, 16207, 16207, 16207, 16207, 16207, 16206, 16206, 16206, 16206, 16206, 16205, 16205, 16205, 16205, 16205, 16204, 16204, 16204, 16204, 16204, 16203, 16203, 16203, 16203, 16203, 16202, 16202, 16202, 16202, 16202, 16201, 16201, 16201, 16201, 16201, 16200, 16200, 16200, 16200, 16200, 16199, 16199, 16199, 16198, 16198, 16197, 16197, 16197, 16196, 16196, 16196, 16195, 16195, 16194, 16194, 16194, 16193, 16193, 16192, 16192, 16192, 16191, 16191, 16191, 16190, 16190, 16190, 16189, 16189, 16188, 16188, 16188, 16187, 16187, 16187, 16186, 16186, 16185, 16185, 16185, 16184, 16184, 16184, 16183, 16183, 16183, 16182, 16182, 16182, 16181, 16181, 16180, 16180, 16180, 16179, 16179, 16179, 16178, 16178, 16178, 16177, 16177, 16177, 16176, 16176, 16176, 16175, 16175, 16175, 16174, 16174, 16174, 16173, 16173, 16173, 16172, 16172, 16172, 16171, 16171, 16171, 16170, 16170, 16170, 16169, 16169, 16169, 16168, 16168, 16168, 16167, 16167, 16167, 16166, 16166, 16166, 16165, 16165, 16165, 16164, 16164, 16164, 16163, 16163, 16163, 16162, 16162, 16162, 16161, 16161, 16161, 16161, 16160, 16160, 16160, 16159, 16159, 16159, 16158, 16158, 16158, 16157, 16157, 16157, 16156, 16156, 16156, 16156, 16155, 16155, 16154, 16153, 16153, 16152, 16152, 16151, 16150, 16150, 16149, 16149, 16148, 16148, 16147, 16146, 16146, 16145, 16145, 16144, 16144, 16143, 16142, 16142, 16141, 16141, 16140, 16140, 16139, 16139, 16138, 16138, 16137, 16136, 16136, 16135, 16135, 16134, 16134, 16133, 16133, 16132, 16132, 16131, 16131, 16130, 16130, 16129, 16129, 16128, 16127, 16126, 16125, 16124, 16123, 16123, 16122, 16121, 16120, 16119, 16118, 16117, 16116, 16115, 16114, 16113, 16112, 16111, 16110, 16109, 16108, 16107, 16106, 16105, 16105, 16104, 16103, 16102, 16101, 16100, 16099, 16098, 16097, 16097, 16096, 16095, 16094, 16093, 16092, 16091, 16090, 16090, 16089, 16088, 16087, 16086, 16085, 16085, 16084, 16083, 16082, 16081, 16080, 16080, 16079, 16078, 16077, 16076, 16076, 16075, 16074, 16073, 16073, 16072, 16071, 16070, 16069, 16069, 16068, 16067, 16066, 16066, 16065, 16064, 16063, 16063, 16062, 16061, 16060, 16059, 16057, 16056, 16055, 16053, 16052, 16050, 16049, 16048, 16046, 16045, 16043, 16042, 16041, 16040, 16038, 16037, 16036, 16034, 16033, 16032, 16031, 16029, 16028, 16027, 16026, 16025, 16023, 16022, 16021, 16020, 16019, 16018, 16016, 16015, 16014, 16013, 16012, 16011, 16010, 16009, 16008, 16007, 16006, 16005, 16003, 16002, 16001, 16000, 15999, 15997, 15995, 15993, 15991, 15989, 15987, 15985, 15983, 15982, 15980, 15978, 15976, 15974, 15972, 15971, 15969, 15967, 15965, 15964, 15962, 15960, 15959, 15957, 15955, 15954, 15952, 15950, 15949, 15947, 15946, 15944, 15943, 15941, 15939, 15938, 15936, 15935, 15933, 15932, 15930, 15929, 15928, 15926, 15925, 15923, 15922, 15921, 15919, 15918, 15916, 15915, 15914, 15912, 15911, 15910, 15909, 15907, 15906, 15905, 15904, 15902, 15901, 15900, 15899, 15897, 15896, 15895, 15894, 15893, 15892, 15890, 15889, 15888, 15887, 15886, 15885, 15884, 15883, 15880, 15878, 15876, 15874, 15872, 15868, 15864, 15861, 15857, 15853, 15849, 15846, 15842, 15839, 15835, 15832, 15829, 15825, 15822, 15819, 15816, 15813, 15809, 15806, 15804, 15801, 15798, 15795, 15792, 15789, 15787, 15784, 15782, 15779, 15776, 15774, 15771, 15769, 15767, 15764, 15762, 15760, 15758, 15755, 15753, 15751, 15749, 15747, 15745, 15742, 15738, 15734, 15730, 15726, 15723, 15719, 15715, 15712, 15708, 15705, 15702, 15698, 15695, 15692, 15689, 15686, 15683, 15680, 15677, 15674, 15671, 15668, 15665, 15662, 15660, 15657, 15654, 15652, 15649, 15647, 15644, 15642, 15640, 15637, 15635, 15633, 15630, 15628, 15626, 15624, 15622, 15620, 15618, 15615, 15611, 15607, 15604, 15600, 15596, 15592, 15589, 15585, 15582, 15578, 15575, 15572, 15568, 15565, 15562, 15559, 15556, 15553, 15550, 15547, 15544, 15541, 15538, 15535, 15533, 15530, 15527, 15525, 15522, 15520, 15517, 15515, 15512, 15510, 15505, 15501, 15497, 15492, 15488, 15481, 15473, 15466, 15459, 15452, 15445, 15438, 15432, 15426, 15420, 15414, 15408, 15403, 15398, 15393, 15388, 15383, 15378, 15374, 15369, 15365, 15361, 15354, 15346, 15339, 15332, 15325, 15318, 15311, 15305, 15299, 15293, 15287, 15281, 15276, 15271, 15266, 15261, 15256, 15251, 15247, 15242, 15238, 15234, 15228, 15220, 15212, 15205, 15198, 15191, 15185, 15178, 15172, 15166, 15160, 15154, 15149, 15144, 15138, 15133, 15129, 15124, 15119, 15115, 15111, 15107, 15101, 15093, 15086, 15078, 15071, 15064, 15058, 15051, 15045, 15039, 15033, 15027, 15022, 15017, 15011, 15006, 15001, 14997, 14992, 14988, 14983, 14979, 14974, 14967, 14959, 14952, 14945, 14938, 14931, 14924, 14918, 14912, 14906, 14900, 14895, 14890, 14884, 14879, 14874, 14870, 14865, 14861, 14856, 14852, 14848, 14840, 14832, 14825, 14818, 14811, 14804, 14798, 14791, 14785, 14779, 14773, 14768, 14757, 14747, 14738, 14729, 14721, 14706, 14691, 14677, 14664, 14652, 14641, 14630, 14620, 14611, 14602, 14593, 14579, 14564, 14551, 14538, 14525, 14514, 14503, 14493, 14483, 14475, 14466, 14453, 14438, 14424, 14411, 14398, 14387, 14376, 14366, 14356, 14347, 14339, 14326, 14311, 14297, 14284, 14271, 14260, 14249, 14239, 14229, 14220, 14212, 14199, 14184, 14170, 14157, 14145, 14133, 14122, 14112, 14102, 14093, 14084, 14073, 14058, 14043, 14030, 14018, 14006, 13995, 13985, 13975, 13966, 13957, 13946, 13931, 13917, 13903, 13891, 13879, 13868, 13857, 13848, 13838, 13830, 13820, 13804, 13790, 13777, 13764, 13752, 13741, 13730, 13721, 13711, 13703, 13693, 13678, 13663, 13650, 13637, 13625, 13614, 13603, 13593, 13584, 13575, 13566, 13551, 13536, 13523, 13510, 13498, 13487, 13476, 13466, 13457, 13448, 13440, 13424, 13410, 13396, 13383, 13371, 13360, 13349, 13339, 13330, 13321, 13313, 13298, 13269, 13244, 13222, 13203, 13185, 13156, 13129, 13106, 13085, 13066, 13044, 13016, 12990, 12968, 12948, 12931, 12903, 12876, 12852, 12831, 12812, 12791, 12762, 12737, 12714, 12694, 12676, 12650, 12622, 12598, 12577, 12558, 12538, 12509, 12483, 12460, 12440, 12422, 12396, 12368, 12344, 12322, 12303, 12285, 12255, 12229, 12206, 12185, 12167, 12143, 12115, 12090, 12068, 12049, 12032, 12002, 11975, 11952, 11931, 11913, 11890, 11861, 11836, 11814, 11795, 11777, 11748, 11721, 11698, 11677, 11658, 11636, 11608, 11582, 11560, 11540, 11523, 11495, 11468, 11444, 11423, 11404, 11383, 11354, 11328, 11306, 11286, 11268, 11241, 11214, 11190, 11168, 11150, 11130, 11101, 11075, 11052, 11032, 11014, 10988, 10960, 10936, 10914, 10895, 10877, 10847, 10821, 10798, 10777, 10759, 10735, 10707, 10682, 10660, 10641, 10624, 10594, 10567, 10544, 10523, 10505, 10481, 10453, 10428, 10406, 10386, 10369, 10340, 10290, 10250, 10200, 10152, 10115, 10060, 10015, 9975, 9920, 9878, 9833, 9782, 9742, 9692, 9644, 9606, 9552, 9506, 9469, 9413, 9369, 9327, 9274, 9233, 9185, 9136, 9097, 9045, 8998, 8961, 8905, 8861, 8820, 8766, 8724, 8679, 8628, 8588, 8538, 8490, 8452, 8398, 8352, 8314, 8258, 8215, 8172, 8120, 8079, 8031, 7982, 7943, 7891, 7844, 7807, 7751, 7707, 7665, 7612, 7570, 7524, 7474, 7434, 7383, 7336, 7299, 7243, 7198, 7159, 7104, 7062, 7017, 6966, 6925, 6876, 6828, 6790, 6736, 6690, 6652, 6597, 6553, 6510, 6458, 6417, 6369, 6319, 6281, 6229, 6182, 6145, 6089, 6045, 6004, 5950, 5908, 5862, 5811, 5772, 5722, 5674, 5636, 5582, 5536, 5498, 5442, 5399, 5356, 5304, 5263, 5215, 5165, 5127, 5074, 5028, 4991, 4935, 4891, 4849, 4796, 4754, 4708, 4657, 4618, 4567, 4520, 4483, 4427, 4343, 4246, 4149, 4060, 3974, 3874, 3780, 3694, 3601, 3503, 3413, 3329, 3228, 3134, 3046, 2956, 2857, 2766, 2681, 2583, 2487, 2399, 2311, 2212, 2119, 2033, 1938, 1841, 1751, 1666, 1566, 1472, 1385, 1293, 1195, 1104, 1020, 921, 825, 737, 648, 550, 457, 372, 276, 179, 109, 66, 40, 24, 15, 9, 5, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const std::vector<uint64_t> sigmoid_bfloat16 = { 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16129, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16130, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16131, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16132, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16133, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16134, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16135, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16136, 16137, 16137, 16137, 16137, 16137, 16137, 16137, 16137, 16137, 16137, 16137, 16137, 16137, 16137, 16137, 16137, 16138, 16138, 16138, 16138, 16138, 16138, 16138, 16138, 16138, 16138, 16138, 16138, 16138, 16138, 16138, 16138, 16139, 16139, 16139, 16139, 16139, 16139, 16139, 16139, 16139, 16139, 16139, 16139, 16139, 16139, 16139, 16139, 16140, 16140, 16140, 16140, 16140, 16140, 16140, 16140, 16140, 16140, 16140, 16140, 16140, 16140, 16140, 16140, 16141, 16141, 16141, 16141, 16141, 16141, 16141, 16141, 16141, 16141, 16141, 16141, 16141, 16141, 16141, 16141, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16142, 16143, 16143, 16143, 16143, 16143, 16143, 16143, 16143, 16143, 16143, 16143, 16143, 16143, 16143, 16143, 16143, 16144, 16144, 16144, 16144, 16144, 16144, 16144, 16144, 16144, 16144, 16144, 16145, 16145, 16145, 16145, 16145, 16145, 16145, 16145, 16146, 16146, 16146, 16146, 16146, 16146, 16146, 16146, 16146, 16147, 16147, 16147, 16147, 16147, 16147, 16147, 16147, 16148, 16148, 16148, 16148, 16148, 16148, 16148, 16148, 16149, 16149, 16149, 16149, 16149, 16149, 16149, 16149, 16150, 16150, 16150, 16150, 16150, 16150, 16150, 16150, 16151, 16151, 16151, 16151, 16151, 16151, 16151, 16151, 16151, 16152, 16152, 16152, 16152, 16152, 16152, 16152, 16152, 16153, 16153, 16153, 16153, 16153, 16153, 16153, 16153, 16154, 16154, 16154, 16154, 16154, 16154, 16154, 16154, 16154, 16155, 16155, 16155, 16155, 16155, 16155, 16155, 16155, 16156, 16156, 16156, 16156, 16156, 16156, 16156, 16156, 16157, 16157, 16157, 16157, 16157, 16157, 16157, 16157, 16157, 16158, 16158, 16158, 16158, 16158, 16158, 16158, 16158, 16159, 16159, 16159, 16159, 16159, 16159, 16159, 16159, 16160, 16160, 16160, 16160, 16161, 16161, 16161, 16161, 16161, 16162, 16162, 16162, 16162, 16163, 16163, 16163, 16163, 16164, 16164, 16164, 16164, 16164, 16165, 16165, 16165, 16165, 16166, 16166, 16166, 16166, 16167, 16167, 16167, 16167, 16167, 16168, 16168, 16168, 16168, 16169, 16169, 16169, 16169, 16169, 16170, 16170, 16170, 16170, 16171, 16171, 16171, 16171, 16171, 16172, 16172, 16172, 16172, 16173, 16173, 16173, 16173, 16173, 16174, 16174, 16174, 16174, 16175, 16175, 16175, 16175, 16175, 16176, 16176, 16176, 16176, 16176, 16177, 16177, 16177, 16177, 16178, 16178, 16178, 16178, 16178, 16179, 16179, 16179, 16179, 16179, 16180, 16180, 16180, 16180, 16180, 16181, 16181, 16181, 16181, 16182, 16182, 16182, 16182, 16182, 16183, 16183, 16183, 16183, 16183, 16184, 16184, 16184, 16184, 16184, 16185, 16185, 16185, 16185, 16185, 16186, 16186, 16186, 16186, 16186, 16187, 16187, 16187, 16187, 16188, 16188, 16188, 16189, 16189, 16189, 16190, 16190, 16191, 16191, 16191, 16192, 16192, 16193, 16193, 16193, 16194, 16194, 16194, 16195, 16195, 16195, 16196, 16196, 16197, 16197, 16197, 16198, 16198, 16198, 16199, 16199, 16199, 16200, 16200, 16200, 16201, 16201, 16201, 16202, 16202, 16202, 16203, 16203, 16203, 16204, 16204, 16204, 16205, 16205, 16205, 16206, 16206, 16206, 16207, 16207, 16207, 16207, 16208, 16208, 16208, 16209, 16209, 16209, 16210, 16210, 16210, 16210, 16211, 16211, 16211, 16212, 16212, 16212, 16212, 16213, 16213, 16213, 16214, 16214, 16214, 16214, 16215, 16215, 16215, 16216, 16216, 16216, 16216, 16217, 16217, 16217, 16217, 16218, 16218, 16218, 16218, 16219, 16219, 16219, 16219, 16220, 16220, 16220, 16220, 16221, 16221, 16221, 16221, 16221, 16222, 16222, 16222, 16222, 16223, 16223, 16223, 16223, 16224, 16224, 16224, 16224, 16224, 16225, 16225, 16225, 16225, 16225, 16226, 16226, 16227, 16227, 16228, 16228, 16228, 16229, 16229, 16229, 16230, 16230, 16231, 16231, 16231, 16232, 16232, 16232, 16233, 16233, 16233, 16234, 16234, 16234, 16235, 16235, 16235, 16235, 16236, 16236, 16236, 16237, 16237, 16237, 16237, 16238, 16238, 16238, 16238, 16239, 16239, 16239, 16239, 16240, 16240, 16240, 16240, 16241, 16241, 16241, 16241, 16241, 16242, 16242, 16242, 16242, 16243, 16243, 16243, 16243, 16243, 16243, 16244, 16244, 16244, 16244, 16244, 16245, 16245, 16245, 16245, 16245, 16245, 16246, 16246, 16246, 16246, 16246, 16246, 16246, 16247, 16247, 16247, 16247, 16247, 16247, 16247, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16248, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16249, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16250, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16251, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16252, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16253, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16254, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16255, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16128, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16127, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16126, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16125, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16124, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16123, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16122, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16121, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16120, 16119, 16119, 16119, 16119, 16119, 16119, 16119, 16119, 16119, 16119, 16119, 16119, 16119, 16119, 16119, 16119, 16118, 16118, 16118, 16118, 16118, 16118, 16118, 16118, 16118, 16118, 16118, 16118, 16118, 16118, 16118, 16118, 16117, 16117, 16117, 16117, 16117, 16117, 16117, 16117, 16117, 16117, 16117, 16117, 16117, 16117, 16117, 16117, 16116, 16116, 16116, 16116, 16116, 16116, 16116, 16116, 16116, 16116, 16116, 16116, 16116, 16116, 16116, 16116, 16115, 16115, 16115, 16115, 16115, 16115, 16115, 16115, 16115, 16115, 16115, 16115, 16115, 16115, 16115, 16115, 16114, 16114, 16114, 16114, 16114, 16114, 16114, 16114, 16114, 16114, 16114, 16114, 16114, 16114, 16114, 16114, 16113, 16113, 16113, 16113, 16113, 16113, 16113, 16113, 16113, 16113, 16113, 16113, 16113, 16113, 16113, 16113, 16112, 16112, 16112, 16112, 16112, 16112, 16112, 16112, 16112, 16112, 16112, 16112, 16111, 16111, 16111, 16111, 16111, 16111, 16111, 16111, 16110, 16110, 16110, 16110, 16110, 16110, 16110, 16110, 16109, 16109, 16109, 16109, 16109, 16109, 16109, 16109, 16108, 16108, 16108, 16108, 16108, 16108, 16108, 16108, 16107, 16107, 16107, 16107, 16107, 16107, 16107, 16107, 16106, 16106, 16106, 16106, 16106, 16106, 16106, 16106, 16105, 16105, 16105, 16105, 16105, 16105, 16105, 16105, 16104, 16104, 16104, 16104, 16104, 16104, 16104, 16104, 16103, 16103, 16103, 16103, 16103, 16103, 16103, 16103, 16102, 16102, 16102, 16102, 16102, 16102, 16102, 16102, 16101, 16101, 16101, 16101, 16101, 16101, 16101, 16101, 16100, 16100, 16100, 16100, 16100, 16100, 16100, 16100, 16099, 16099, 16099, 16099, 16099, 16099, 16099, 16099, 16099, 16098, 16098, 16098, 16098, 16098, 16098, 16098, 16098, 16097, 16097, 16097, 16097, 16097, 16097, 16097, 16097, 16096, 16096, 16096, 16096, 16096, 16095, 16095, 16095, 16095, 16094, 16094, 16094, 16094, 16093, 16093, 16093, 16093, 16092, 16092, 16092, 16092, 16092, 16091, 16091, 16091, 16091, 16090, 16090, 16090, 16090, 16089, 16089, 16089, 16089, 16088, 16088, 16088, 16088, 16087, 16087, 16087, 16087, 16086, 16086, 16086, 16086, 16085, 16085, 16085, 16085, 16084, 16084, 16084, 16084, 16083, 16083, 16083, 16083, 16082, 16082, 16082, 16082, 16082, 16081, 16081, 16081, 16081, 16080, 16080, 16080, 16080, 16079, 16079, 16079, 16079, 16078, 16078, 16078, 16078, 16077, 16077, 16077, 16077, 16076, 16076, 16076, 16076, 16076, 16075, 16075, 16075, 16075, 16074, 16074, 16074, 16074, 16073, 16073, 16073, 16073, 16072, 16072, 16072, 16072, 16071, 16071, 16071, 16071, 16070, 16070, 16070, 16070, 16070, 16069, 16069, 16069, 16069, 16068, 16068, 16068, 16068, 16067, 16067, 16067, 16067, 16066, 16066, 16066, 16066, 16066, 16065, 16065, 16064, 16064, 16063, 16063, 16062, 16062, 16062, 16061, 16061, 16060, 16060, 16059, 16059, 16058, 16058, 16057, 16057, 16056, 16056, 16056, 16055, 16055, 16054, 16054, 16053, 16053, 16052, 16052, 16051, 16051, 16051, 16050, 16050, 16049, 16049, 16048, 16048, 16047, 16047, 16046, 16046, 16046, 16045, 16045, 16044, 16044, 16043, 16043, 16042, 16042, 16042, 16041, 16041, 16040, 16040, 16039, 16039, 16038, 16038, 16038, 16037, 16037, 16036, 16036, 16035, 16035, 16035, 16034, 16034, 16033, 16033, 16032, 16032, 16032, 16031, 16031, 16030, 16030, 16029, 16029, 16029, 16028, 16028, 16027, 16027, 16026, 16026, 16026, 16025, 16025, 16024, 16024, 16023, 16023, 16023, 16022, 16022, 16021, 16021, 16021, 16020, 16020, 16019, 16019, 16019, 16018, 16018, 16017, 16017, 16016, 16016, 16016, 16015, 16015, 16014, 16014, 16014, 16013, 16013, 16012, 16012, 16012, 16011, 16011, 16010, 16010, 16010, 16009, 16008, 16007, 16007, 16006, 16005, 16004, 16003, 16003, 16002, 16001, 16000, 15999, 15998, 15996, 15995, 15993, 15992, 15991, 15989, 15988, 15986, 15985, 15983, 15982, 15980, 15979, 15978, 15976, 15975, 15973, 15972, 15971, 15969, 15968, 15967, 15965, 15964, 15962, 15961, 15960, 15958, 15957, 15956, 15955, 15953, 15952, 15951, 15949, 15948, 15947, 15946, 15944, 15943, 15942, 15941, 15939, 15938, 15937, 15936, 15934, 15933, 15932, 15931, 15930, 15928, 15927, 15926, 15925, 15924, 15923, 15921, 15920, 15919, 15918, 15917, 15916, 15915, 15914, 15912, 15911, 15910, 15909, 15908, 15907, 15906, 15905, 15904, 15903, 15902, 15901, 15900, 15899, 15898, 15897, 15896, 15895, 15894, 15893, 15892, 15891, 15890, 15889, 15888, 15887, 15886, 15885, 15884, 15883, 15882, 15881, 15880, 15879, 15878, 15877, 15877, 15876, 15875, 15874, 15873, 15872, 15870, 15869, 15867, 15865, 15864, 15862, 15860, 15857, 15853, 15850, 15847, 15844, 15841, 15838, 15835, 15831, 15828, 15826, 15823, 15820, 15817, 15814, 15811, 15809, 15806, 15803, 15801, 15798, 15795, 15793, 15790, 15788, 15785, 15783, 15781, 15778, 15776, 15774, 15771, 15769, 15767, 15765, 15763, 15761, 15758, 15756, 15754, 15752, 15750, 15748, 15746, 15745, 15741, 15738, 15734, 15731, 15727, 15723, 15720, 15717, 15713, 15710, 15707, 15704, 15700, 15697, 15694, 15691, 15688, 15685, 15682, 15679, 15677, 15674, 15671, 15668, 15666, 15663, 15660, 15658, 15655, 15653, 15650, 15648, 15646, 15643, 15641, 15639, 15636, 15634, 15632, 15630, 15628, 15626, 15624, 15621, 15619, 15618, 15615, 15611, 15608, 15604, 15600, 15597, 15593, 15589, 15586, 15583, 15579, 15576, 15573, 15569, 15566, 15563, 15560, 15557, 15554, 15551, 15548, 15545, 15543, 15540, 15537, 15534, 15532, 15529, 15527, 15524, 15522, 15519, 15517, 15514, 15512, 15510, 15507, 15503, 15499, 15494, 15490, 15485, 15477, 15470, 15462, 15455, 15449, 15442, 15436, 15429, 15423, 15418, 15412, 15407, 15401, 15396, 15391, 15386, 15382, 15377, 15373, 15368, 15364, 15360, 15352, 15345, 15337, 15330, 15323, 15317, 15310, 15304, 15298, 15292, 15286, 15280, 15275, 15270, 15265, 15260, 15255, 15250, 15246, 15242, 15237, 15233, 15227, 15219, 15212, 15204, 15197, 15191, 15184, 15178, 15171, 15165, 15160, 15154, 15148, 15143, 15138, 15133, 15128, 15124, 15119, 15115, 15110, 15106, 15101, 15093, 15085, 15078, 15071, 15064, 15057, 15051, 15045, 15039, 15033, 15027, 15022, 15016, 15011, 15006, 15001, 14997, 14992, 14988, 14983, 14979, 14974, 14966, 14959, 14951, 14944, 14937, 14931, 14924, 14918, 14912, 14906, 14900, 14895, 14889, 14884, 14879, 14874, 14870, 14865, 14860, 14856, 14852, 14848, 14840, 14832, 14825, 14818, 14811, 14804, 14798, 14791, 14785, 14779, 14773, 14768, 14757, 14747, 14738, 14729, 14721, 14706, 14691, 14677, 14664, 14652, 14641, 14630, 14620, 14611, 14602, 14593, 14579, 14564, 14551, 14538, 14525, 14514, 14503, 14493, 14483, 14475, 14466, 14452, 14438, 14424, 14411, 14398, 14387, 14376, 14366, 14356, 14347, 14339, 14326, 14311, 14297, 14284, 14271, 14260, 14249, 14239, 14229, 14220, 14212, 14199, 14184, 14170, 14157, 14145, 14133, 14122, 14112, 14102, 14093, 14084, 14073, 14058, 14043, 14030, 14018, 14006, 13995, 13985, 13975, 13966, 13957, 13946, 13931, 13917, 13903, 13891, 13879, 13868, 13857, 13848, 13838, 13830, 13820, 13804, 13790, 13777, 13764, 13752, 13741, 13730, 13721, 13711, 13703, 13693, 13678, 13663, 13650, 13637, 13625, 13614, 13603, 13593, 13584, 13575, 13566, 13551, 13536, 13523, 13510, 13498, 13487, 13476, 13466, 13457, 13448, 13440, 13424, 13410, 13396, 13383, 13371, 13360, 13349, 13339, 13330, 13321, 13313, 13298, 13269, 13244, 13222, 13203, 13185, 13156, 13129, 13106, 13085, 13066, 13044, 13016, 12990, 12968, 12948, 12931, 12903, 12876, 12852, 12831, 12812, 12791, 12762, 12737, 12714, 12694, 12676, 12650, 12622, 12598, 12577, 12558, 12538, 12509, 12483, 12460, 12440, 12422, 12396, 12368, 12344, 12322, 12303, 12285, 12255, 12229, 12206, 12185, 12167, 12143, 12115, 12090, 12068, 12049, 12032, 12002, 11975, 11952, 11931, 11913, 11890, 11861, 11836, 11814, 11795, 11777, 11748, 11721, 11698, 11677, 11658, 11636, 11608, 11582, 11560, 11540, 11523, 11495, 11468, 11444, 11423, 11404, 11383, 11354, 11328, 11306, 11286, 11268, 11241, 11214, 11190, 11168, 11150, 11130, 11101, 11075, 11052, 11032, 11014, 10988, 10960, 10936, 10914, 10895, 10877, 10847, 10821, 10798, 10777, 10759, 10735, 10707, 10682, 10660, 10641, 10624, 10594, 10567, 10544, 10523, 10505, 10481, 10453, 10428, 10406, 10386, 10369, 10340, 10290, 10250, 10200, 10152, 10115, 10060, 10015, 9975, 9920, 9878, 9833, 9782, 9742, 9692, 9644, 9606, 9552, 9506, 9469, 9413, 9369, 9327, 9274, 9233, 9185, 9136, 9097, 9045, 8998, 8961, 8905, 8861, 8820, 8766, 8724, 8679, 8628, 8588, 8538, 8490, 8452, 8398, 8352, 8314, 8258, 8215, 8172, 8120, 8079, 8031, 7982, 7943, 7891, 7844, 7807, 7751, 7707, 7665, 7612, 7570, 7524, 7474, 7434, 7383, 7336, 7299, 7243, 7198, 7159, 7104, 7062, 7017, 6966, 6925, 6876, 6828, 6790, 6736, 6690, 6652, 6597, 6553, 6510, 6458, 6417, 6369, 6319, 6281, 6229, 6182, 6145, 6089, 6045, 6004, 5950, 5908, 5862, 5811, 5772, 5722, 5674, 5636, 5582, 5536, 5498, 5442, 5399, 5356, 5304, 5263, 5215, 5165, 5127, 5074, 5028, 4991, 4935, 4891, 4849, 4796, 4754, 4708, 4657, 4618, 4567, 4520, 4483, 4427, 4343, 4246, 4149, 4060, 3974, 3874, 3780, 3694, 3601, 3503, 3413, 3329, 3228, 3134, 3046, 2956, 2857, 2766, 2681, 2583, 2487, 2399, 2311, 2212, 2119, 2033, 1938, 1841, 1751, 1666, 1566, 1472, 1385, 1293, 1195, 1104, 1020, 921, 825, 737, 648, 550, 457, 372, 276, 179, 109, 66, 40, 24, 15, 9, 5, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#endif // FLOATING_POINT_MATH_COEFFS_H__
