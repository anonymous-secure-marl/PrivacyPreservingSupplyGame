#include "FloatingPoint/floating-point.h"
#include <omp.h>

using namespace std;
using namespace sci;

FixArray quotient_computation_softfloat(FixOp* fix, const FixArray& x_m, const FixArray& y_m) {
    assert(x_m.ell == 32);
    assert(y_m.ell == 32);
    assert(x_m.s == 30);

    BoolArray all_0 = fix->bool_op->input(ALICE, x_m.size, uint8_t(0));
    BoolArray all_1 = fix->bool_op->input(ALICE, x_m.size, 1);
    /* Reciprocal Approximation */
    FixArray idx = fix->reduce(fix->truncate_reduce(y_m, 27), 4);
    FixArray eps = fix->reduce(fix->truncate_reduce(y_m, 11), 16);
    vector<uint64_t> spec_vec_v0 = {0xFFC4, 0xF0BE, 0xE363, 0xD76F, 0xCCAD, 0xC2F0,
        0xBA16, 0xB201, 0xAA97, 0xA3C6, 0x9D7A, 0x97A6, 0x923C, 0x8D32, 0x887E, 0x8417};
    vector<uint64_t> spec_vec_v1 = { 0xF0F1, 0xD62C, 0xBFA1, 0xAC77, 0x9C0A, 0x8DDB,
        0x8185, 0x76BA, 0x6D3B, 0x64D4, 0x5D5C, 0x56B1, 0x50B6, 0x4B55, 0x4679, 0x4211};
    FixArray v0 = fix->LUT(spec_vec_v0, idx, false, 16, 16, 4);
    FixArray v1 = fix->LUT(spec_vec_v1, idx, false, 16, 16, 4);
    FixArray v_ = fix->sub(fix->extend(v0, 32), fix->right_shift(fix->mul(v1, eps, 32), 20));
    FixArray sigma0_ = fix->reduce(fix->truncate_reduce(fix->mul(v_, y_m, 64, all_0.data), 7), 32);
    FixArray sigma0 = fix->sub((1ULL << 32) - 1, sigma0_);
    FixArray u = fix->reduce(fix->truncate_reduce(fix->mul(v_, sigma0, 64, all_0.data), 24), 32);
    FixArray v = fix->add(fix->scale_up(v_, 32, 32), u);
    FixArray sigma1 = fix->truncate_reduce(fix->mul(sigma0, sigma0, 64), 32);
    FixArray w_ = fix->truncate_reduce(fix->mul(v, sigma1, 64), 48);
    FixArray w = fix->add(v, fix->extend(w_, 32));

    /* Quotient Computation */
    FixArray q = fix->truncate_reduce(fix->mul(x_m, w, 64), 32);
    q = fix->add(q, 2);

    BoolArray cond1 = fix->LT(fix->reduce(q, 6), 2);
    FixArray q_if = fix->sub(q, fix->extend(fix->reduce(q, 2), 32));
    FixArray rem = fix->sub(fix->scale_up(x_m, 64, 62),
                            fix->mul(fix->scale_up(q_if, 32, 31), y_m, 64));
    // cond2 = rem < 0, cond3 = rem > 0
    BoolArray cond2, cond3;
    tie(cond2, cond3) = fix->MSB_and_zero_test(rem);
    cond3 = fix->bool_op->NOT(fix->bool_op->XOR(cond2, cond3));

    q_if = fix->if_else(cond2, fix->sub(q_if, 4), q_if);
    q_if = fix->if_else(cond3,
           fix->add(fix->sub(q_if, fix->extend(fix->reduce(q_if, 1), q.ell)), 1), q_if);
    q = fix->if_else(cond1, q_if, q);

    return q;
}

FPArray FPOp::div_softfloat(const FPArray& x, const FPArray& y, bool check_bounds) {
    assert(x.m_bits == 23);
    assert(x.e_bits == 8);
    assert(x.m_bits == y.m_bits);
    assert(x.e_bits == y.e_bits);
    assert(x.size == y.size);

    BoolArray all_1 = bool_op->input(ALICE, x.size, 1);

    BoolArray x_s, x_z;
    FixArray x_m, x_e;
    tie(x_s, x_z, x_m, x_e) = get_components(x);
    BoolArray y_s, y_z;
    FixArray y_m, y_e;
    tie(y_s, y_z, y_m, y_e) = get_components(y);

    // Line 116
    FixArray ret_e = fix->sub(x_e, y_e);
    ret_e = fix->add(ret_e, x.e_bias());

    // Lines 129-134
    BoolArray denormal_m = fix->LT(x_m, y_m);
    x_m = fix->extend(x_m, 25, all_1.data);
    x_m = fix->scale_up(x_m, 32, 30);
    FixArray x_m_if = fix->mul(x_m, 2, 32);
    x_m = fix->if_else(denormal_m, x_m_if, x_m);
    FixArray ret_e_if = fix->sub(ret_e, 1);
    ret_e = fix->if_else(denormal_m, ret_e_if, ret_e);

    // Line 135
    y_m = fix->scale_up(y_m, 32, 31);

    FixArray q = quotient_computation_softfloat(fix, x_m, y_m);
    q = fix->round_ties_to_even(q, 7);

    BoolArray ret_s = bool_op->XOR(x_s, y_s);
    BoolArray ret_z = x_z;

    FPArray ret = this->input(x.party, x.size, ret_s.data, ret_z.data,
            q.data, ret_e.data, x.m_bits, x.e_bits);

    if (check_bounds) {
        ret = this->check_bounds(ret);
    }

    return ret;
}

FixArray softfloat_shiftRightJam32(FPOp* fp_op, FixArray a, FixArray dist) {
    assert(a.ell = 32);
    assert(dist.ell = 16);
    FixOp* fix = fp_op->fix;
    BoolOp* bool_op = fp_op->bool_op;
    
    BoolArray dist_lt = fix->LT(dist, 31);

    // if
    dist.signed_ = false;
    FixArray a_rs_dist = fix->right_shift(a, dist, 31);
    FixArray neg_dist = fix->reduce(fix->mul(dist, -1), 5);
    FixArray a_ls_dist = fix->left_shift(a, neg_dist, 32, 31);
    BoolArray a_ls_dist_neq_0 = bool_op->NOT(fix->EQ(a_ls_dist, 0));
    BoolArray a_rs_dist_lsb = fix->LSB(a_rs_dist);
    BoolArray cond = bool_op->OR(a_rs_dist_lsb, a_ls_dist_neq_0);
    cond = bool_op->XOR(cond, a_rs_dist_lsb);
    FixArray or_arith = fix->B2A(cond, false, 32);
    or_arith.s = a_rs_dist.s;
    a_rs_dist = fix->add(a_rs_dist, or_arith);

    // else
    BoolArray a_neq_0 = bool_op->NOT(fix->EQ(a, 0));
    FixArray a_neq_0_arith = fix->B2A(a_neq_0, false, 32);
    a_neq_0_arith.s = a_rs_dist.s;

    FixArray ret = fix->if_else(dist_lt, a_rs_dist, a_neq_0_arith);
    return ret;
}

FixArray softfloat_shiftRightJam64(FPOp* fp_op, FixArray a, int dist = 32) {
    assert(a.ell = 64);
    FixOp* fix = fp_op->fix;
    BoolOp* bool_op = fp_op->bool_op;

    // if
    FixArray a_rs_dist = fix->right_shift(a, 32);
    FixArray a_lsbs = fix->reduce(a, 32);
    BoolArray a_lsbs_neq_0 = bool_op->NOT(fix->EQ(a_lsbs, 0));
    BoolArray a_lsb = fix->LSB(a);
    BoolArray cond = bool_op->OR(a_lsb, a_lsbs_neq_0);
    cond = bool_op->XOR(cond, a_lsb);
    FixArray or_arith = fix->B2A(cond, false, 64);
    or_arith.s = a_rs_dist.s;
    a_rs_dist = fix->add(a_rs_dist, or_arith);

    return a_rs_dist;
}

FPArray softfloat_roundPackToF32(FPOp* fp_op, BoolArray sign, FixArray exp, FixArray sig) {
    assert(exp.ell = 16);
    assert(sig.ell = 32);
    FixOp* fix = fp_op->fix;
    BoolOp* bool_op = fp_op->bool_op;

    FixArray roundBits = fix->reduce(sig, 7);
    sig = fix->right_shift(fix->add(sig, 0x40), 7);
    // optimized line 102
    BoolArray roundBits_xor = bool_op->NOT(fix->EQ(fix->add(roundBits, 0x40), 0));
    BoolArray sig_lsb = fix->LSB(sig);
    BoolArray corr = bool_op->XOR(bool_op->AND(roundBits_xor, sig_lsb), sig_lsb);
    FixArray corr_arith = fix->B2A(corr, false, 32);
    corr_arith.s = sig.s;
    sig = fix->add(sig, corr_arith);
    BoolArray sig_eqz = fix->EQ(sig, 0);
    exp = fix->if_else(sig_eqz, 0, exp);
    sig = fix->reduce(sig, 24);
    exp = fix->reduce(exp, 10);
    FPArray ret = fp_op->input(sig.party, sig.size, sign.data, sig_eqz.data, sig.data, exp.data, 23, 8);
    return ret;
}

FPArray softfloat_normRoundPackToF32(FPOp* fp_op, BoolArray sign, FixArray exp, FixArray sig) {
    assert(exp.ell = 16);
    assert(sig.ell = 32);
    FixOp* fix = fp_op->fix;
    BoolOp* bool_op = fp_op->bool_op;

    vector<FixArray> msnzb = fix->msnzb_one_hot(sig, 8);
    FixArray shiftDist = fix->input(fp_op->party, sig.size, (uint64_t)0ULL, true, 8, 0);
    for (int i = 0; i < msnzb.size(); i++) {
        msnzb[i].signed_ = true;
        shiftDist = fix->add(shiftDist, fix->mul(msnzb[i], 32 - 1 - i - 1));
    }
    shiftDist = fix->extend(shiftDist, 16);
    exp = fix->sub(exp, shiftDist);

    BoolArray shiftDist_geq7 = fix->GE(shiftDist, 7);
    BoolArray exp_lt_253 = fix->LT(exp, 253);
    BoolArray cond = bool_op->AND(shiftDist_geq7, exp_lt_253);

    FixArray shiftDist_if = fix->sub(shiftDist, 7);
    BoolArray zero_if = fix->EQ(sig, 0);
    shiftDist_if.signed_ = false;
    FixArray sig_if = fix->reduce(fix->left_shift(sig, shiftDist_if, 32, 23), 24);
    FixArray exp_if = fix->reduce(fix->if_else(zero_if, 0, exp), 10);
    FPArray ret_if = fp_op->input(sig.party, sig.size, sign.data, zero_if.data, sig_if.data, exp_if.data, 23, 8);

    shiftDist.signed_ = false;
    FixArray sig_else = fix->left_shift(sig, shiftDist, 32, 31);
    FPArray ret_else = softfloat_roundPackToF32(fp_op, sign, exp, sig_else);

    return fp_op->if_else(cond, ret_if, ret_else);
}

FPArray softfloat_subMagsF32(FPOp* fp_op, BoolArray signA, FixArray sigA, FixArray expA, BoolArray signB, FixArray sigB, FixArray expB) {
    assert(sigA.ell = 32);
    assert(sigB.ell = 32);
    assert(expA.ell = 16);
    assert(expB.ell = 16);
    FixOp* fix = fp_op->fix;
    BoolOp* bool_op = fp_op->bool_op;

    FixArray expDiff = fix->sub(expA, expB);

    BoolArray expDiff_neqz = bool_op->NOT(fix->EQ(expDiff, 0));
    // if from line 68 - 98 (skipping ifs from line 71 - 76 & 78 - 83)
    FixArray expA_if = fix->if_else(fix->EQ(expA, 0), expA, fix->sub(expA, 1));
    BoolArray signZ_if = signA;
    FixArray sigDiff_if = fix->sub(sigA, sigB);
    BoolArray sigDiff_if_ltz = fix->LT(sigDiff_if, 0);
    BoolArray sigDiff_if_eqz = fix->EQ(sigDiff_if, 0);
    /// if from line 86 - 89
    BoolArray signZ_if_if = bool_op->NOT(signZ_if);
    FixArray sigDiff_if_if = fix->mul(sigDiff_if, -1);
    signZ_if = bool_op->if_else(sigDiff_if_ltz, signZ_if_if, signZ_if);
    sigDiff_if = fix->if_else(sigDiff_if_ltz, sigDiff_if_if, sigDiff_if);
    vector<FixArray> msnzb = fix->msnzb_one_hot(sigDiff_if, 16);
    FixArray shiftDist_if = fix->input(fp_op->party, sigDiff_if.size, (uint64_t)0ULL, true, 16, 0);
    for (int i = 0; i < msnzb.size(); i++) {
        msnzb[i].signed_ = true;
        shiftDist_if = fix->add(shiftDist_if, fix->mul(msnzb[i], 32 - 1 - i - 8));
    }
    FixArray expZ_if = fix->sub(expA, shiftDist_if);
    BoolArray expZ_if_ltz = fix->LT(expZ_if, 0);
    /// if from line 92 - 95
    shiftDist_if = fix->if_else(expZ_if_ltz, expA_if, shiftDist_if);
    expZ_if = fix->reduce(fix->if_else(expZ_if_ltz, 0, expZ_if), 10);
    shiftDist_if.signed_ = false;
    sigDiff_if = fix->reduce(fix->left_shift(sigDiff_if, shiftDist_if, 32, 23), 24);
    FPArray ret_if = fp_op->input(sigA.party, sigA.size, signZ_if.data, sigDiff_if_eqz.data, sigDiff_if.data, expZ_if.data, 23, 8);

    // else from line 98 - 133
    BoolArray signZ_else = signA;
    FixArray sigA_else = fix->mul(sigA, 1 << 7);
    FixArray sigB_else = fix->mul(sigB, 1 << 7);
    BoolArray expDiff_ltz = fix->LT(expDiff, 0);
    /// if from line 104 - 117 (skipping NaN if from 108 - 112)
    BoolArray signZ_else_if = bool_op->NOT(signZ_else);
    FixArray expZ_else_if = fix->sub(expB, 1);
    FixArray sigX_if = fix->add(sigB, 0x40000000);
    FixArray sigY_if = fix->if_else(fix->EQ(expA, 0), sigA, 0x40000000);
    sigY_if = fix->add(sigY_if, sigA);
    FixArray expDiff_else_if = fix->mul(expDiff, -1);
    /// else from line 117 - 128
    BoolArray signZ_else_else = signZ_else;
    FixArray expZ_else_else = fix->sub(expA, 1);
    FixArray sigX_else = fix->add(sigA, 0x40000000);
    FixArray sigY_else = fix->if_else(fix->EQ(expB, 0), sigB, 0x40000000);
    sigY_else = fix->add(sigY_else, sigB);
    FixArray expDiff_else_else = expDiff;
    signZ_else = bool_op->if_else(expDiff_ltz, signZ_else_if, signZ_else_else);
    FixArray expZ_else = fix->if_else(expDiff_ltz, expZ_else_if, expZ_else_else);
    FixArray sigX = fix->if_else(expDiff_ltz, sigX_if, sigX_else);
    FixArray sigY = fix->if_else(expDiff_ltz, sigY_if, sigY_else);
    FixArray expDiff_else = fix->if_else(expDiff_ltz, expDiff_else_if, expDiff_else_else);
    sigX = fix->sub(sigX, softfloat_shiftRightJam32(fp_op, sigY, expDiff_else));
    FPArray ret_else = softfloat_normRoundPackToF32(fp_op, signZ_else, expZ_else, sigX);
    // end if

    return fp_op->if_else(expDiff_neqz, ret_if, ret_else);
}

FPArray softfloat_addMagsF32(FPOp* fp_op, BoolArray signA, FixArray sigA, FixArray expA, BoolArray signB, FixArray sigB, FixArray expB) {
    assert(sigA.ell = 32);
    assert(sigB.ell = 32);
    assert(expA.ell = 16);
    assert(expB.ell = 16);
    FixOp* fix = fp_op->fix;
    BoolOp* bool_op = fp_op->bool_op;

    FixArray expDiff = fix->sub(expA, expB);

    BoolArray expDiff_neqz = bool_op->NOT(fix->EQ(expDiff, 0));
    // if from line 65 - 85 (skipping ifs from line 68 - 76)
    FixArray sigZ_if = fix->add(fix->add(sigA, sigB), 0x01000000);
    BoolArray signZ_if = signA;
    FixArray expZ_if = expA;
    BoolArray cond = bool_op->NOT(fix->LSB(sigZ_if)); // optimized
    cond = bool_op->AND(cond, fix->LT(expZ_if, 0xFE));
    /// if from line 80 - 83
    sigZ_if = fix->right_shift(sigZ_if, 1);
    sigZ_if.s++;
    sigZ_if = fix->if_else(cond, sigZ_if, fix->mul(sigZ_if, 1<<6));
    /// end if

    // else from line 85 - 115
    BoolArray signZ_else = signA;
    FixArray sigA_else = fix->mul(sigA, 1 << 6);
    FixArray sigB_else = fix->mul(sigB, 1 << 6);
    cond = fix->LT(expDiff, 0);
    /// if from line 91 - 100 (skipping NaN if from line 92 - 96)
    FixArray expZ_else_if = expB;
    FixArray sigA_else_if = fix->add(sigA_else, fix->if_else(bool_op->NOT(fix->EQ(expA, 0)), 0x20000000, sigA_else));
    sigA_else_if = softfloat_shiftRightJam32(fp_op, sigA_else_if, fix->mul(expDiff, -1));
    /// else from line 100 - 109 (skipping NaN if from 101 - 104)
    FixArray expZ_else_else = expA;
    FixArray sigB_else_else = fix->add(sigB_else, fix->if_else(bool_op->NOT(fix->EQ(expB, 0)), 0x20000000, sigB_else));
    sigB_else_else = softfloat_shiftRightJam32(fp_op, sigB_else_else, expDiff);
    sigA_else = fix->if_else(cond, sigA_else_if, sigA_else);
    sigB_else = fix->if_else(cond, sigB_else, sigB_else_else);
    FixArray expZ_else = fix->if_else(cond, expZ_else_if, expZ_else_else);
    /// end if
    FixArray sigZ_else = fix->add(fix->add(sigA_else, sigB_else), 0x20000000);
    BoolArray sigZ_lt = fix->LT(sigZ_else, 0x40000000);
    /// if from line 111 - 113
    expZ_else_if = fix->sub(expZ_else, 1);
    FixArray sigZ_else_if = fix->mul(sigZ_else, 2);
    expZ_else = fix->if_else(sigZ_lt, expZ_else_if, expZ_else);
    sigZ_else = fix->if_else(sigZ_lt, sigZ_else_if, sigZ_else);
    // end if

    FixArray expZ = fix->if_else(expDiff_neqz, expZ_if, expZ_else);
    FixArray sigZ = fix->if_else(expDiff_neqz, sigZ_if, sigZ_else);
    BoolArray signZ = bool_op->if_else(expDiff_neqz, signZ_if, signZ_else);

    return softfloat_roundPackToF32(fp_op, signZ, expZ, sigZ);
}

FPArray FPOp::add_softfloat(const FPArray& x, const FPArray& y) {
    assert(x.m_bits == 23);
    assert(x.e_bits == 8);
    assert(x.m_bits == y.m_bits);
    assert(x.e_bits == y.e_bits);
    assert(x.size == y.size);

    BoolArray all_1 = bool_op->input(ALICE, x.size, 1);

    BoolArray x_s, x_z;
    FixArray x_m, x_e;
    tie(x_s, x_z, x_m, x_e) = get_components(x);
    BoolArray y_s, y_z;
    FixArray y_m, y_e;
    tie(y_s, y_z, y_m, y_e) = get_components(y);
    x_m = fix->extend(x_m, 32);
    y_m = fix->extend(y_m, 32);
    x_e = fix->extend(x_e, 16);
    y_e = fix->extend(y_e, 16);
    // subtract 1 << 23 from x_m, y_m
    x_m = fix->sub(x_m, 1 << 23);
    y_m = fix->sub(y_m, 1 << 23);

    BoolArray s = bool_op->XOR(x_s, y_s); // optimized

    FPArray add_res = softfloat_addMagsF32(fp_op, x_s, x_m, x_e, y_s, y_m, y_e);
    FPArray sub_res = softfloat_subMagsF32(fp_op, x_s, x_m, x_e, y_s, y_m, y_e);

    FPArray ret = fp_op->if_else(s, sub_res, add_res);

    return ret;
}

FPArray FPOp::mul_softfloat(const FPArray& x, const FPArray& y) {
    assert(x.m_bits == 23);
    assert(x.e_bits == 8);
    assert(x.m_bits == y.m_bits);
    assert(x.e_bits == y.e_bits);
    assert(x.size == y.size);

    BoolArray x_s, x_z;
    FixArray x_m, x_e;
    tie(x_s, x_z, x_m, x_e) = get_components(x);
    BoolArray y_s, y_z;
    FixArray y_m, y_e;
    tie(y_s, y_z, y_m, y_e) = get_components(y);
    FixArray sigA = fix->extend(x_m, 32);
    FixArray sigB = fix->extend(y_m, 32);
    FixArray expA = fix->extend(x_e, 16);
    FixArray expB = fix->extend(y_e, 16);
    sigA = fix->sub(sigA, 1 << 23);
    sigB = fix->sub(sigB, 1 << 23);

    BoolArray signZ = bool_op->XOR(x_s, y_s); // optimized
    // skipping ifs from line 78 - 87 (handling NaNs)
    // skipping ifs from line 90 - 101 (handling subnormals)
    FixArray expZ = fix->sub(fix->add(expA, expB), 0x7F);
    sigA = fix->mul(fix->add(sigA, 0x00800000), 1 << 7);
    sigB = fix->mul(fix->add(sigB, 0x00800000), 1 << 8);
    FixArray sigZ = fix->mul(sigA, sigB, 64);
    sigZ = softfloat_shiftRightJam64(fp_op, sigZ, 32);
    BoolArray sigZ_lt = fix->LT(sigZ, 0x40000000);
    FixArray expZ_if = fix->sub(expZ, 1);
    FixArray sigZ_if = fix->mul(sigZ, 2);
    sigZ = fix->if_else(sigZ_lt, sigZ_if, sigZ);
    expZ = fix->if_else(sigZ_lt, expZ_if, expZ);
    sigZ = fix->reduce(sigZ, 32);
    return softfloat_roundPackToF32(fp_op, signZ, expZ, sigZ);
}

BoolArray FPOp::LT_softfloat(const FPArray &x, const FPArray &y) {
    assert(x.m_bits == 23);
    assert(x.e_bits == 8);
    assert(x.m_bits == y.m_bits);
    assert(x.e_bits == y.e_bits);
    assert(x.size == y.size);
    FixOp* fix = fp_op->fix;
    BoolOp* bool_op = fp_op->bool_op;

    BoolArray x_s, x_z;
    FixArray x_m, x_e;
    tie(x_s, x_z, x_m, x_e) = get_components(x);
    BoolArray y_s, y_z;
    FixArray y_m, y_e;
    tie(y_s, y_z, y_m, y_e) = get_components(y);

    BoolArray uneq_s = bool_op->XOR(x_s, y_s);
    // if
    x_m = fix->extend(x_m, 32);
    y_m = fix->extend(y_m, 32);
    FixArray not_x_z_arith = fix->B2A(bool_op->NOT(x_z), false, 32);
    FixArray not_y_z_arith = fix->B2A(bool_op->NOT(y_z), false, 32);
    not_x_z_arith = fix->mul(not_x_z_arith, 1 << 23);
    not_y_z_arith = fix->mul(not_y_z_arith, 1 << 23);
    not_x_z_arith.s = x_m.s;
    not_y_z_arith.s = y_m.s;
    x_m = fix->sub(x_m, not_x_z_arith);
    y_m = fix->sub(y_m, not_y_z_arith);

    x_e = fix->extend(x_e, 32);
    y_e = fix->extend(y_e, 32);
    x_m.s = 0; y_m.s = 0;
    x_e.signed_ = false; y_e.signed_ = false;
    FixArray x_me = fix->add(x_m, fix->mul(x_e, 1 << 23));
    FixArray y_me = fix->add(y_m, fix->mul(y_e, 1 << 23));

    // TODO
    FixArray x_or_y = x_me;
    x_or_y = fix->mul(x_or_y, 2);
    BoolArray x_or_y_neqz = bool_op->NOT(fix->EQ(x_or_y, 0));
    BoolArray ret_if = bool_op->AND(x_or_y_neqz, x_s);

    BoolArray x_neq_y = fix->EQ(x_me, y_me);
    BoolArray x_lt_y = fix->LT(x_me, y_me);
    BoolArray ret_else = bool_op->AND(x_neq_y, bool_op->XOR(x_s, x_lt_y));

    return bool_op->if_else(uneq_s, ret_if, ret_else);
}
