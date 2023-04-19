#include "globals_float.h"
#include "library_float.h"
#include <math.h>

using namespace std ;
using namespace sci ;

#define WHICHPARTY tid&1?3-__party:__party

// Packs
IOPack *__iopack = nullptr ;		
OTPack *__otpack = nullptr ;

// Addressing stuff
string __address = "127.0.0.1" ;
int __port = 32000 ;

// Operations
BoolOp *__bool_op = nullptr ;		// bool
FixOp *__fix_op = nullptr ;			// int
FPOp *__fp_op = nullptr ;			// float
FPMath *__fp_math = nullptr ;		// float math operations

// Floating point descriptors
int __m_bits = 23 ;					// mantissa bits
int __e_bits = 8 ;					// exponent bits

// Other stuff from defines_float.h
int __nt = MAX_THREADS ;
int __party = 0 ;

// Handy globals for experiments
// int BATCH = 0 ;
// extern int BATCH ;
int __sz1 = 0 ;
int __sz2 = 0 ;
int __sz3 = 0 ;
int __sz4 = 0 ;

// Output operations
BoolArray __bool_pub ;				// bool
FixArray __fix_pub ;				// int
FPArray __fp_pub ;					// float

// Initialization
void __init(int __argc, char **__argv) {
	cout.precision(15) ;
	ArgMapping __amap ;

	__amap.arg("r", __party, "Role of party: ALICE/SERVER = 1; BOB/CLIENT = 2") ;
	__amap.arg("nt", __nt, "Number of threads") ;
	__amap.arg("mbits", __m_bits, "mantissa bits") ;
	__amap.arg("ebits", __e_bits, "exponent bits") ;
	__amap.arg("port", __port, "port") ;
	__amap.arg("add", __address, "address") ;

	__amap.arg("sz1", __sz1, "size 1") ;
	__amap.arg("sz2", __sz2, "size 2") ;
	__amap.arg("sz3", __sz3, "size 3") ;
	__amap.arg("sz4", __sz4, "size 4") ;

	// __amap.arg("batch", BATCH, "batch size for experiments") ;
	__amap.parse(__argc, __argv);

    for (int i = 0; i < __nt ; i++) {
    	iopackArr[i] = new IOPack(__party, __port+i, __address) ;
    	if (i & 1) {
    		otpackArr[i] = new OTPack(iopackArr[i], 3-__party) ;
    	}
    	else {
    		otpackArr[i] = new OTPack(iopackArr[i], __party) ;
    	}
    }

    for (int i = 0 ; i < __nt ; i++) {
    	int pt ;
    	if (i & 1)
    		pt = 3 - __party ;
    	else
    		pt = __party ;

    	boolopArr[i] = new BoolOp(pt, iopackArr[i], otpackArr[i]) ;
    	fixopArr[i] = new FixOp(pt, iopackArr[i], otpackArr[i]) ;
    	fpopArr[i] = new FPOp(pt, iopackArr[i], otpackArr[i]) ;
    	fpmathArr[i] = new FPMath(pt, iopackArr[i], otpackArr[i]) ;
    }

    __iopack = iopackArr[0] ;
    __otpack = otpackArr[0] ;

    __bool_op = boolopArr[0] ;
    __fix_op = fixopArr[0] ;
    __fp_op = fpopArr[0] ;    
    __fp_math = fpmathArr[0] ;
}

float __get_comm() {
	float c = 0.0 ;
	for (int i = 0 ; i < __nt ; i++)
		c += (float)iopackArr[i]->get_comm() ;

	return c ;
}

BoolArray __public_bool_to_boolean(uint8_t b, int party=ALICE) {
	uint8_t *_dummy = new uint8_t[1] ;
	_dummy[0] = b ;
	BoolArray _ret = __bool_op->input(party, 1, _dummy) ;
	delete[] _dummy ;
	return _ret ;
}

FixArray __public_int_to_arithmetic(uint64_t i, bool sign, int len, int party=ALICE) {
	uint64_t *_dummy = new uint64_t[1] ;
	_dummy[0] = i ;
	FixArray _ret = __fix_op->input(party, 1, _dummy, sign, len, 0) ;
	delete[] _dummy ;
	return _ret ;
}

FPArray __public_float_to_arithmetic(float f, int party=ALICE) {
	float *_dummy = new float[1] ;
	_dummy[0] = f ;
	FPArray _ret = __fp_op->input<float>(party, 1, _dummy, __m_bits, __e_bits) ;
	delete[] _dummy ;
	return _ret ;
}

FPArray __rand_float(int party) {
	float *_dummy = new float[1] ;
	_dummy[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ;
	FPArray _ret = __fp_op->input<float>(party, 1, _dummy, __m_bits, __e_bits) ;
	delete[] _dummy ;
	return _ret ;
}

BoolArray __rand_bool(int party) {
	bool b ;
	if (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) > 0.5)
		b = true ;
	else
		b = false ;
	return __public_bool_to_boolean(b, party) ;
}

vector<BoolArray> make_vector_bool(int party, size_t last) {
	vector<BoolArray> _ret ;
	for (size_t i = 0 ; i < last ; i++) {
		_ret.push_back(__public_bool_to_boolean(false, party)) ;
	}
	return _ret ;
}


vector<FixArray> make_vector_int(int party, bool sign, int len, size_t last) {
	vector<FixArray> _ret ;
	for (size_t i = 0 ; i < last ; i++) {
		_ret.push_back(__public_int_to_arithmetic(0, sign, len, party)) ;
	}
	return _ret ;
}

vector<FPArray> make_vector_float(int party, size_t last) {
	vector<FPArray> _ret ;
	for (size_t i = 0 ; i < last ; i++) {
		_ret.push_back(__public_float_to_arithmetic(0.0, party)) ;
	}
	return _ret ;
}

vector<FPArray> make_vector_float_rand(int party, size_t last) {
	vector<FPArray> _ret ;
	for (size_t i = 0 ; i < last ; i++) {
		_ret.push_back(__rand_float(party)) ;
	}
	return _ret ;
}

vector<BoolArray> make_vector_bool_rand(int party, size_t last) {
	vector<BoolArray> _ret ;
	for (size_t i = 0 ; i < last ; i++) {
		_ret.push_back(__rand_bool(party)) ;
	}
	return _ret ;
}

vector<int> get_chunks(int items, int slots) {
	int allocated, chunk, remaining ;

	chunk = items/slots ;
	vector<int> ret(slots, chunk) ;

	allocated = chunk*slots ;
	remaining = items - allocated ;
	for (int i = 0 ; i < remaining ; i++)
		ret[i]++ ;

	return ret ;
}

tuple<BoolArray,BoolArray,FixArray,FixArray> get_components(int tid, const FPArray &x) {
  BoolArray x_s = boolopArr[tid]->input(x.party, x.size, x.s);
  BoolArray x_z = boolopArr[tid]->input(x.party, x.size, x.z);
  FixArray x_m = fixopArr[tid]->input(x.party, x.size, x.m, false, x.m_bits + 1, x.m_bits);
  FixArray x_e = fixopArr[tid]->input(x.party, x.size, x.e, true, x.e_bits + 2, 0);
  return make_tuple(x_s, x_z, x_m, x_e);
}

void ElemWiseSub_thread(
	int32_t tid, int32_t sz, int m_bits, int e_bits,
	uint8_t *arr1_s, uint8_t *arr1_z, uint64_t *arr1_m, uint64_t *arr1_e,
	uint8_t *arr2_s, uint8_t *arr2_z, uint64_t *arr2_m, uint64_t *arr2_e,
	uint8_t *out_s, uint8_t *out_z, uint64_t *out_m, uint64_t *out_e
	) {
	FPArray arr1_flat = fpopArr[tid]->input(WHICHPARTY, sz, arr1_s, arr1_z, arr1_m, arr1_e, m_bits, e_bits) ;
	FPArray arr2_flat = fpopArr[tid]->input(WHICHPARTY, sz, arr2_s, arr2_z, arr2_m, arr2_e, m_bits, e_bits) ;
	FPArray out = fpopArr[tid]->sub(arr1_flat, arr2_flat) ;

	memcpy(out_s, out.s, sz*sizeof(uint8_t)) ;
	memcpy(out_z, out.z, sz*sizeof(uint8_t)) ;
	memcpy(out_m, out.m, sz*sizeof(uint64_t)) ;
	memcpy(out_e, out.e, sz*sizeof(uint64_t)) ;
}

void ElemWiseSub(int32_t s1, vector<FPArray>& arr1, vector<FPArray>& arr2, vector<FPArray>& outArr) {
	int m_bits, e_bits ;
	m_bits = arr1[0].m_bits ;
	e_bits = arr1[0].e_bits ;

	uint8_t *arr1_s = new uint8_t[s1] ;
	uint8_t *arr1_z = new uint8_t[s1] ;
	uint64_t *arr1_m = new uint64_t[s1] ;
	uint64_t *arr1_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		arr1_s[i] = arr1[i].s[0] ;
		arr1_z[i] = arr1[i].z[0] ;
		arr1_m[i] = arr1[i].m[0] ;
		arr1_e[i] = arr1[i].e[0] ;
	}

	uint8_t *arr2_s = new uint8_t[s1] ;
	uint8_t *arr2_z = new uint8_t[s1] ;
	uint64_t *arr2_m = new uint64_t[s1] ;
	uint64_t *arr2_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		arr2_s[i] = arr2[i].s[0] ;
		arr2_z[i] = arr2[i].z[0] ;
		arr2_m[i] = arr2[i].m[0] ;
		arr2_e[i] = arr2[i].e[0] ;
	}

	uint8_t *out_s = new uint8_t[s1] ;
	uint8_t *out_z = new uint8_t[s1] ;
	uint64_t *out_m = new uint64_t[s1] ;
	uint64_t *out_e = new uint64_t[s1] ;

	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(ElemWiseSub_thread,
				i, chunks[i], m_bits, e_bits,
				arr1_s+offset, arr1_z+offset, arr1_m+offset, arr1_e+offset,
				arr2_s+offset, arr2_z+offset, arr2_m+offset, arr2_e+offset,
				out_s+offset, out_z+offset, out_m+offset, out_e+offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0)
			threads[i].join() ;
	}
	
	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].m_bits = m_bits ;
		outArr[i].e_bits = e_bits ;

		outArr[i].s[0] = out_s[i] ;
		outArr[i].z[0] = out_z[i] ;
		outArr[i].m[0] = out_m[i] ;
		outArr[i].e[0] = out_e[i] ;
	}

	delete[] arr1_s ; delete[] arr2_s ; delete[] out_s ;
	delete[] arr1_z ; delete[] arr2_z ; delete[] out_z ;
	delete[] arr1_m ; delete[] arr2_m ; delete[] out_m ;
	delete[] arr1_e ; delete[] arr2_e ; delete[] out_e ;
}

void SubtractOne_thread(
	int32_t tid, int32_t sz, int m_bits, int e_bits,
	uint8_t *in_s, uint8_t *in_z, uint64_t *in_m, uint64_t *in_e,
	uint8_t *out_s, uint8_t *out_z, uint64_t *out_m, uint64_t *out_e
	) {
	FPArray in_flat = fpopArr[tid]->input(WHICHPARTY, sz, in_s, in_z, in_m, in_e, m_bits, e_bits) ;
	FPArray one_flat = fpopArr[tid]->input<float>(ALICE, sz, (float)1.0, m_bits, e_bits) ;
	FPArray out = fpopArr[tid]->sub(one_flat, in_flat) ;

	memcpy(out_s, out.s, sz*sizeof(uint8_t)) ;
	memcpy(out_z, out.z, sz*sizeof(uint8_t)) ;
	memcpy(out_m, out.m, sz*sizeof(uint64_t)) ;
	memcpy(out_e, out.e, sz*sizeof(uint64_t)) ;
}

void SubtractOne(int32_t s1, vector<FPArray>& inArr, vector<FPArray>& outArr) {
	int m_bits, e_bits ;
	m_bits = inArr[0].m_bits ;
	e_bits = inArr[0].e_bits ;

	uint8_t *inArr_s = new uint8_t[s1] ;
	uint8_t *inArr_z = new uint8_t[s1] ;
	uint64_t *inArr_m = new uint64_t[s1] ;
	uint64_t *inArr_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		inArr_s[i] = inArr[i].s[0] ;
		inArr_z[i] = inArr[i].z[0] ;
		inArr_m[i] = inArr[i].m[0] ;
		inArr_e[i] = inArr[i].e[0] ;
	}

	uint8_t *outArr_s = new uint8_t[s1] ;
	uint8_t *outArr_z = new uint8_t[s1] ;
	uint64_t *outArr_m = new uint64_t[s1] ;
	uint64_t *outArr_e = new uint64_t[s1] ;

	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0 ) {
			threads[i] = thread(SubtractOne_thread,
				i, chunks[i], m_bits, e_bits,
				inArr_s+offset, inArr_z+offset, inArr_m+offset, inArr_e+offset,
				outArr_s+offset, outArr_z+offset, outArr_m+offset, outArr_e+offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0)
			threads[i].join() ;
	}
	
	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].m_bits = m_bits ;
		outArr[i].e_bits = e_bits ;

		outArr[i].s[0] = outArr_s[i] ;
		outArr[i].z[0] = outArr_z[i] ;
		outArr[i].m[0] = outArr_m[i] ;
		outArr[i].e[0] = outArr_e[i] ;
	}

	delete[] inArr_s ; delete[] outArr_s ;
	delete[] inArr_z ; delete[] outArr_z ;
	delete[] inArr_m ; delete[] outArr_m ;
	delete[] inArr_e ; delete[] outArr_e ;
}

void ElemWiseMul_thread(
	int32_t tid, int32_t sz, int m_bits, int e_bits,
	uint8_t *arr1_s, uint8_t *arr1_z, uint64_t *arr1_m, uint64_t *arr1_e,
	uint8_t *arr2_s, uint8_t *arr2_z, uint64_t *arr2_m, uint64_t *arr2_e,
	uint8_t *out_s, uint8_t *out_z, uint64_t *out_m, uint64_t *out_e
	) {
	FPArray arr1_flat = fpopArr[tid]->input(WHICHPARTY, sz, arr1_s, arr1_z, arr1_m, arr1_e, m_bits, e_bits) ;
	FPArray arr2_flat = fpopArr[tid]->input(WHICHPARTY, sz, arr2_s, arr2_z, arr2_m, arr2_e, m_bits, e_bits) ;
	FPArray out = fpopArr[tid]->mul(arr1_flat, arr2_flat) ;

	memcpy(out_s, out.s, sz*sizeof(uint8_t)) ;
	memcpy(out_z, out.z, sz*sizeof(uint8_t)) ;
	memcpy(out_m, out.m, sz*sizeof(uint64_t)) ;
	memcpy(out_e, out.e, sz*sizeof(uint64_t)) ;
}

void ElemWiseMul(int32_t s1, vector<FPArray>& arr1, vector<FPArray>& arr2, vector<FPArray>& outArr) {
	int m_bits, e_bits ;
	m_bits = arr1[0].m_bits ;
	e_bits = arr1[0].e_bits ;

	uint8_t *arr1_s = new uint8_t[s1] ;
	uint8_t *arr1_z = new uint8_t[s1] ;
	uint64_t *arr1_m = new uint64_t[s1] ;
	uint64_t *arr1_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		arr1_s[i] = arr1[i].s[0] ;
		arr1_z[i] = arr1[i].z[0] ;
		arr1_m[i] = arr1[i].m[0] ;
		arr1_e[i] = arr1[i].e[0] ;
	}

	uint8_t *arr2_s = new uint8_t[s1] ;
	uint8_t *arr2_z = new uint8_t[s1] ;
	uint64_t *arr2_m = new uint64_t[s1] ;
	uint64_t *arr2_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		arr2_s[i] = arr2[i].s[0] ;
		arr2_z[i] = arr2[i].z[0] ;
		arr2_m[i] = arr2[i].m[0] ;
		arr2_e[i] = arr2[i].e[0] ;
	}

	uint8_t *out_s = new uint8_t[s1] ;
	uint8_t *out_z = new uint8_t[s1] ;
	uint64_t *out_m = new uint64_t[s1] ;
	uint64_t *out_e = new uint64_t[s1] ;

	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0 ) {
			threads[i] = thread(ElemWiseMul_thread,
				i, chunks[i], m_bits, e_bits,
				arr1_s+offset, arr1_z+offset, arr1_m+offset, arr1_e+offset,
				arr2_s+offset, arr2_z+offset, arr2_m+offset, arr2_e+offset,
				out_s+offset, out_z+offset, out_m+offset, out_e+offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0)
			threads[i].join() ;
	}
	
	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].m_bits = m_bits ;
		outArr[i].e_bits = e_bits ;

		outArr[i].s[0] = out_s[i] ;
		outArr[i].z[0] = out_z[i] ;
		outArr[i].m[0] = out_m[i] ;
		outArr[i].e[0] = out_e[i] ;
	}

	delete[] arr1_s ; delete[] arr2_s ; delete[] out_s ;
	delete[] arr1_z ; delete[] arr2_z ; delete[] out_z ;
	delete[] arr1_m ; delete[] arr2_m ; delete[] out_m ;
	delete[] arr1_e ; delete[] arr2_e ; delete[] out_e ;
}

void ElemWiseDiv_thread(
	int32_t tid, int32_t sz, int m_bits, int e_bits,
	uint8_t *arr1_s, uint8_t *arr1_z, uint64_t *arr1_m, uint64_t *arr1_e,
	uint8_t *arr2_s, uint8_t *arr2_z, uint64_t *arr2_m, uint64_t *arr2_e,
	uint8_t *out_s, uint8_t *out_z, uint64_t *out_m, uint64_t *out_e
	) {
	FPArray arr1_flat = fpopArr[tid]->input(WHICHPARTY, sz, arr1_s, arr1_z, arr1_m, arr1_e, m_bits, e_bits) ;
	FPArray arr2_flat = fpopArr[tid]->input(WHICHPARTY, sz, arr2_s, arr2_z, arr2_m, arr2_e, m_bits, e_bits) ;
	FPArray out = fpopArr[tid]->div(arr1_flat, arr2_flat) ;

	memcpy(out_s, out.s, sz*sizeof(uint8_t)) ;
	memcpy(out_z, out.z, sz*sizeof(uint8_t)) ;
	memcpy(out_m, out.m, sz*sizeof(uint64_t)) ;
	memcpy(out_e, out.e, sz*sizeof(uint64_t)) ;
}

void ElemWiseDiv(int32_t s1, vector<FPArray>& arr1, vector<FPArray>& arr2, vector<FPArray>& outArr) {
	int m_bits, e_bits ;
	m_bits = arr1[0].m_bits ;
	e_bits = arr1[0].e_bits ;

	uint8_t *arr1_s = new uint8_t[s1] ;
	uint8_t *arr1_z = new uint8_t[s1] ;
	uint64_t *arr1_m = new uint64_t[s1] ;
	uint64_t *arr1_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		arr1_s[i] = arr1[i].s[0] ;
		arr1_z[i] = arr1[i].z[0] ;
		arr1_m[i] = arr1[i].m[0] ;
		arr1_e[i] = arr1[i].e[0] ;
	}

	uint8_t *arr2_s = new uint8_t[s1] ;
	uint8_t *arr2_z = new uint8_t[s1] ;
	uint64_t *arr2_m = new uint64_t[s1] ;
	uint64_t *arr2_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		arr2_s[i] = arr2[i].s[0] ;
		arr2_z[i] = arr2[i].z[0] ;
		arr2_m[i] = arr2[i].m[0] ;
		arr2_e[i] = arr2[i].e[0] ;
	}

	uint8_t *out_s = new uint8_t[s1] ;
	uint8_t *out_z = new uint8_t[s1] ;
	uint64_t *out_m = new uint64_t[s1] ;
	uint64_t *out_e = new uint64_t[s1] ;

	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0 ) {
			threads[i] = thread(ElemWiseDiv_thread,
				i, chunks[i], m_bits, e_bits,
				arr1_s+offset, arr1_z+offset, arr1_m+offset, arr1_e+offset,
				arr2_s+offset, arr2_z+offset, arr2_m+offset, arr2_e+offset,
				out_s+offset, out_z+offset, out_m+offset, out_e+offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0)
			threads[i].join() ;
	}
	
	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].m_bits = m_bits ;
		outArr[i].e_bits = e_bits ;

		outArr[i].s[0] = out_s[i] ;
		outArr[i].z[0] = out_z[i] ;
		outArr[i].m[0] = out_m[i] ;
		outArr[i].e[0] = out_e[i] ;
	}

	delete[] arr1_s ; delete[] arr2_s ; delete[] out_s ;
	delete[] arr1_z ; delete[] arr2_z ; delete[] out_z ;
	delete[] arr1_m ; delete[] arr2_m ; delete[] out_m ;
	delete[] arr1_e ; delete[] arr2_e ; delete[] out_e ;
}

void getOutDer(int32_t s1, int32_t s2, vector<vector<FPArray>>& P, vector<vector<FPArray>>& Phat, vector<vector<FPArray>>& der) {
	int sz = s1*s2 ;
	int m_bits, e_bits ;
	m_bits = P[0][0].m_bits ;
	e_bits = P[0][0].e_bits ;

	vector<FPArray> flat1, flat2 ;
	vector<FPArray> flat3 = make_vector_float(ALICE, sz) ;

	for (int i = 0 ; i < s1 ; i++)
		for (int j = 0 ; j < s2 ; j++)
			flat1.push_back(P[i][j]) ;
	
	for (int i = 0 ; i < s1 ; i++)
		for (int j = 0 ; j < s2 ; j++)
			flat2.push_back(Phat[i][j]) ;

	ElemWiseSub(sz, flat1, flat2, flat3) ;

	vector<FPArray> divver = make_vector_float(ALICE, sz) ;
	for (int i = 0 ; i < sz ; i++)
		divver[i] = __fp_op->input<float>(ALICE, 1, (float)(1.0/s1), m_bits, e_bits) ;
	ElemWiseMul(sz, flat3, divver, flat3) ;

	for (int i = 0, k = 0 ; i < s1 ; i++) 
		for (int j = 0 ; j < s2 ; j++, k++)
			der[i][j] = flat3[k] ;
}

void MatMul_thread(
	int tid, int m_chunk, int n, int p, int m_bits, int e_bits, FPMatrix B,
	uint8_t *A_s, uint8_t *A_z, uint64_t *A_m, uint64_t *A_e,
	uint8_t *res_s, uint8_t *res_z, uint64_t *res_m, uint64_t *res_e
	) {

	FPMatrix A_chunk = fpopArr[tid]->input(WHICHPARTY, m_chunk, n, A_s, A_z, A_m, A_e, m_bits, e_bits) ;
	FPMatrix res = fpopArr[tid]->matrix_multiplication_baseline(A_chunk, B) ;
	
	memcpy(res_s, res.s, m_chunk*p*sizeof(uint8_t)) ;
	memcpy(res_z, res.z, m_chunk*p*sizeof(uint8_t)) ;
	memcpy(res_m, res.m, m_chunk*p*sizeof(uint64_t)) ;
	memcpy(res_e, res.e, m_chunk*p*sizeof(uint64_t)) ;
}

void MatMul(int32_t m, int32_t n, int32_t p, 
	vector<vector<FPArray>> &A, 
	vector<vector<FPArray>> &B, 
	vector<vector<FPArray>> &C) {

	if (m <= __nt && p > __nt) {
		vector<vector<FPArray>> BT = make_vector_float(ALICE, p, n) ;
		vector<vector<FPArray>> AT = make_vector_float(ALICE, n, m) ;
		vector<vector<FPArray>> CT = make_vector_float(ALICE, p, m) ;

		for (int i = 0 ; i < n ; i++) {
			for (int j = 0 ; j < m ; j++) {
				AT[i][j] = A[j][i] ;
			}
		}

		for (int i = 0 ; i < p ; i++)
			for (int j = 0 ; j < n ; j++)
				BT[i][j] = B[j][i] ;

		MatMul(p, n, m, BT, AT, CT) ;

		for (int i = 0 ; i < m ; i++)
			for (int j = 0 ; j < p ; j++)
				C[i][j] = CT[j][i] ;

		return ;
	} 

	int m_bits = A[0][0].m_bits ;
	int e_bits = A[0][0].e_bits ;

	uint8_t *A_s = new uint8_t[m*n] ;
	uint8_t *A_z = new uint8_t[m*n] ;
	uint64_t *A_m = new uint64_t[m*n] ;
	uint64_t *A_e = new uint64_t[m*n] ;
	for (int i = 0, k=0 ; i < m ; i++) {
		for (int j = 0 ; j < n ; j++, k++) {
			A_s[k] = A[i][j].s[0] ;
			A_z[k] = A[i][j].z[0] ;
			A_m[k] = A[i][j].m[0] ;
			A_e[k] = A[i][j].e[0] ;
		}
	}

	uint8_t *B_s = new uint8_t[n*p] ;
	uint8_t *B_z = new uint8_t[n*p] ;
	uint64_t *B_m = new uint64_t[n*p] ;
	uint64_t *B_e = new uint64_t[n*p] ;
	for (int i = 0, k = 0 ; i < n ; i++) {
		for (int j = 0 ; j < p ; j++, k++) {
			B_s[k] = B[i][j].s[0] ;
			B_z[k] = B[i][j].z[0] ;
			B_m[k] = B[i][j].m[0] ;
			B_e[k] = B[i][j].e[0] ;
		}
	}
	FPMatrix mat2 = __fp_op->input(__party, n, p, B_s, B_z, B_m, B_e, m_bits, e_bits) ;

	uint8_t *res_s = new uint8_t[m*p] ;
	uint8_t *res_z = new uint8_t[m*p] ;
	uint64_t *res_m = new uint64_t[m*p] ;
	uint64_t *res_e = new uint64_t[m*p] ;

	vector<int> chunks = get_chunks(m, __nt) ;
	thread threads[MAX_THREADS] ;
	int m_offset, A_offset, res_offset ;
	m_offset = A_offset = res_offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(MatMul_thread,
				i, chunks[i], n, p, m_bits, e_bits, mat2,
				A_s+A_offset, A_z+A_offset, A_m+A_offset, A_e+A_offset,
				res_s+res_offset, res_z+res_offset, res_m+res_offset, res_e+res_offset
			) ;

			m_offset += chunks[i] ;
			A_offset += chunks[i]*n ;
			res_offset += chunks[i]*p ;
		}
	}

	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0)
			threads[i].join() ;
	}

	for (int i = 0, k = 0 ; i < m ; i++) {
		for (int j = 0 ; j < p ; j++, k++) {
			C[i][j].m_bits = m_bits ;
			C[i][j].e_bits = e_bits ;

			C[i][j].s[0] = res_s[k] ;
			C[i][j].z[0] = res_z[k] ;
			C[i][j].m[0] = res_m[k] ;
			C[i][j].e[0] = res_e[k] ;
		}
	}

	delete[] A_s ; delete[] B_s ; delete[] res_s ;
	delete[] A_z ; delete[] B_z ; delete[] res_z ;
	delete[] A_m ; delete[] B_m ; delete[] res_m ;
	delete[] A_e ; delete[] B_e ; delete[] res_e ; 
}

void MatMul3(
	int32_t d1, int32_t d2, int32_t d3, int32_t d4, 
	vector<vector<vector<FPArray>>> &arr1, 
	vector<vector<vector<FPArray>>> &arr2, 
	vector<vector<vector<FPArray>>> &arr3) {
	vector<vector<FPArray>> mat1 = make_vector_float(ALICE, d1*d2, d3) ;
	vector<vector<FPArray>> matout = make_vector_float(ALICE, d1*d2, d4) ;

	for (int i1 = 0 ; i1 < d1 ; i1++) {
		for (int i2 = 0 ; i2 < d2 ; i2++) {
			for (int i3 = 0 ; i3 < d3 ; i3++) {
				mat1[i1*d2+i2][i3] = arr1[i1][i2][i3] ;
			}
		}
	}

	MatMul(d1*d2, d3, d4, mat1, arr2[0], matout) ;

	for (int i1 = 0 ; i1 < d1 ; i1++) {
		for (int i2 = 0 ; i2 < d2 ; i2++) {
			for (int i4 = 0 ; i4 < d4 ; i4++) {
				arr3[i1][i2][i4] = matout[i1*d2+i2][i4] ;
			}
		}
	}
}

void IfElse_thread(
	int tid, int sz, int m_bits, int e_bits,
	uint8_t *in_s, uint8_t *in_z, uint64_t *in_m, uint64_t *in_e, uint8_t *in_hot,
	uint8_t *out_s, uint8_t *out_z, uint64_t *out_m, uint64_t *out_e, bool flip=false
	) {

	FPArray in_flat = fpopArr[tid]->input(WHICHPARTY, sz, in_s, in_z, in_m, in_e, m_bits, e_bits) ;
	BoolArray cond_flat = boolopArr[tid]->input(WHICHPARTY, sz, in_hot) ;
	FPArray zero_flat = fpopArr[tid]->input<float>(ALICE, sz, (float)0.0, m_bits, e_bits) ;

	FPArray out_flat ;
	if (flip)
		out_flat = fpopArr[tid]->if_else(cond_flat, zero_flat, in_flat) ;
	else
		out_flat = fpopArr[tid]->if_else(cond_flat, in_flat, zero_flat) ;

	memcpy(out_s, out_flat.s, sz*sizeof(uint8_t)) ;
	memcpy(out_z, out_flat.z, sz*sizeof(uint8_t)) ;
	memcpy(out_m, out_flat.m, sz*sizeof(uint64_t)) ;
	memcpy(out_e, out_flat.e, sz*sizeof(uint64_t)) ;
}

void IfElse(
	int32_t s1, 
	vector<FPArray> &inArr,
	vector<BoolArray> &condArr, 
	vector<FPArray> &outArr, bool flip=false) {
	int m_bits, e_bits ;
	m_bits = inArr[0].m_bits ;
	e_bits = inArr[0].e_bits ;

	uint8_t *in_s = new uint8_t[s1] ;
	uint8_t *in_z = new uint8_t[s1] ;
	uint64_t *in_m = new uint64_t[s1] ;
	uint64_t *in_e = new uint64_t[s1] ;
	uint8_t *in_hot = new uint8_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		in_s[i] = inArr[i].s[0] ;
		in_z[i] = inArr[i].z[0] ;
		in_m[i] = inArr[i].m[0] ;
		in_e[i] = inArr[i].e[0] ;

		in_hot[i] = condArr[i].data[0] ;
	}

	uint8_t *out_s = new uint8_t[s1] ;
	uint8_t *out_z = new uint8_t[s1] ;
	uint64_t *out_m = new uint64_t[s1] ;
	uint64_t *out_e = new uint64_t[s1] ;

	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(IfElse_thread,
				i, chunks[i], m_bits, e_bits,
				in_s + offset, in_z + offset, in_m + offset, in_e + offset, in_hot+offset,
				out_s + offset, out_z + offset, out_m + offset, out_e + offset, flip
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].m_bits = m_bits ;
		outArr[i].e_bits = e_bits ;

		outArr[i].s[0] = out_s[i] ;
		outArr[i].z[0] = out_z[i] ;
		outArr[i].m[0] = out_m[i] ;
		outArr[i].e[0] = out_e[i] ;
	}

	delete[] in_s ; delete[] out_s ;
	delete[] in_z ; delete[] out_z ;
	delete[] in_m ; delete[] out_m ;
	delete[] in_e ; delete[] out_e ;
	delete[] in_hot ;
}

void ElemWiseAdd_thread(
	int32_t tid, int32_t sz, int m_bits, int e_bits,
	uint8_t *arr1_s, uint8_t *arr1_z, uint64_t *arr1_m, uint64_t *arr1_e,
	uint8_t *arr2_s, uint8_t *arr2_z, uint64_t *arr2_m, uint64_t *arr2_e,
	uint8_t *out_s, uint8_t *out_z, uint64_t *out_m, uint64_t *out_e
	) {
	FPArray arr1_flat = fpopArr[tid]->input(WHICHPARTY, sz, arr1_s, arr1_z, arr1_m, arr1_e, m_bits, e_bits) ;
	FPArray arr2_flat = fpopArr[tid]->input(WHICHPARTY, sz, arr2_s, arr2_z, arr2_m, arr2_e, m_bits, e_bits) ;
	FPArray out = fpopArr[tid]->add(arr1_flat, arr2_flat) ;

	memcpy(out_s, out.s, sz*sizeof(uint8_t)) ;
	memcpy(out_z, out.z, sz*sizeof(uint8_t)) ;
	memcpy(out_m, out.m, sz*sizeof(uint64_t)) ;
	memcpy(out_e, out.e, sz*sizeof(uint64_t)) ;
}

void ElemWiseAdd(int32_t s1, vector<FPArray>& arr1, vector<FPArray>& arr2, vector<FPArray>& outArr) {
	int m_bits, e_bits ;
	m_bits = arr1[0].m_bits ;
	e_bits = arr1[0].e_bits ;

	uint8_t *arr1_s = new uint8_t[s1] ;
	uint8_t *arr1_z = new uint8_t[s1] ;
	uint64_t *arr1_m = new uint64_t[s1] ;
	uint64_t *arr1_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		arr1_s[i] = arr1[i].s[0] ;
		arr1_z[i] = arr1[i].z[0] ;
		arr1_m[i] = arr1[i].m[0] ;
		arr1_e[i] = arr1[i].e[0] ;
	}

	uint8_t *arr2_s = new uint8_t[s1] ;
	uint8_t *arr2_z = new uint8_t[s1] ;
	uint64_t *arr2_m = new uint64_t[s1] ;
	uint64_t *arr2_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		arr2_s[i] = arr2[i].s[0] ;
		arr2_z[i] = arr2[i].z[0] ;
		arr2_m[i] = arr2[i].m[0] ;
		arr2_e[i] = arr2[i].e[0] ;
	}

	uint8_t *out_s = new uint8_t[s1] ;
	uint8_t *out_z = new uint8_t[s1] ;
	uint64_t *out_m = new uint64_t[s1] ;
	uint64_t *out_e = new uint64_t[s1] ;


	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(ElemWiseAdd_thread,
				i, chunks[i], m_bits, e_bits,
				arr1_s+offset, arr1_z+offset, arr1_m+offset, arr1_e+offset,
				arr2_s+offset, arr2_z+offset, arr2_m+offset, arr2_e+offset,
				out_s+offset, out_z+offset, out_m+offset, out_e+offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0)
			threads[i].join() ;
	}
	
	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].m_bits = m_bits ;
		outArr[i].e_bits = e_bits ;

		outArr[i].s[0] = out_s[i] ;
		outArr[i].z[0] = out_z[i] ;
		outArr[i].m[0] = out_m[i] ;
		outArr[i].e[0] = out_e[i] ;
	}

	delete[] arr1_s ; delete[] arr2_s ; delete[] out_s ;
	delete[] arr1_z ; delete[] arr2_z ; delete[] out_z ;
	delete[] arr1_m ; delete[] arr2_m ; delete[] out_m ;
	delete[] arr1_e ; delete[] arr2_e ; delete[] out_e ;
}

void GemmAdd(int32_t s1, int32_t s2, 
	vector<vector<FPArray>> &inArr, 
	vector<FPArray> &bias, 
	vector<vector<FPArray>> &outArr) {

	int m_bits, e_bits ;
	int sz ;

	m_bits = inArr[0][0].m_bits ;
	e_bits = inArr[0][0].e_bits ;
	sz = s1*s2 ;

	vector<FPArray> arr1 = make_vector_float(ALICE, sz) ;
	vector<FPArray> arr2 = make_vector_float(ALICE, sz) ;
	vector<FPArray> out = make_vector_float(ALICE, sz) ;

	for (int i1=0, k=0 ; i1 < s1 ; i1++) {
		for (int i2 = 0 ; i2 < s2 ; i2++, k++) {
			arr1[k] = inArr[i1][i2] ;
			arr2[k] = bias[i2] ;
		}
	}

	ElemWiseAdd(sz, arr1, arr2, out) ;

	for (int i1 = 0, k = 0 ; i1 < s1 ; i1++) {
		for (int i2 = 0 ; i2 < s2 ; i2++, k++) {
			outArr[i1][i2] = out[k] ;
		}
	}
}

void GemmAdd3(int32_t s1, int32_t s2, int32_t s3,
	vector<vector<vector<FPArray>>> &inArr, 
	vector<FPArray> &bias, 
	vector<vector<vector<FPArray>>> &outArr) {

	int m_bits, e_bits ;
	int sz ;

	m_bits = inArr[0][0][0].m_bits ;
	e_bits = inArr[0][0][0].e_bits ;
	sz = s1*s2*s3 ;

	vector<FPArray> arr1 = make_vector_float(ALICE, sz) ;
	vector<FPArray> arr2 = make_vector_float(ALICE, sz) ;
	vector<FPArray> out = make_vector_float(ALICE, sz) ;

	for (int i1=0, k=0 ; i1 < s1 ; i1++) {
		for (int i2 = 0 ; i2 < s2 ; i2++) {
			for (int i3 = 0 ; i3 < s3 ; i3++, k++) {
				arr1[k] = inArr[i1][i2][i3] ;
				arr2[k] = bias[i2] ;				
			}
		}
	}

	ElemWiseAdd(sz, arr1, arr2, out) ;

	for (int i1 = 0, k = 0 ; i1 < s1 ; i1++) {
		for (int i2 = 0 ; i2 < s2 ; i2++) {
			for (int i3 = 0 ; i3 < s3 ; i3++, k++) {
				outArr[i1][i2][i3] = out[k] ;	
			}			
		}
	}
}

void Relu_thread(
	int tid, int sz, int m_bits, int e_bits,
	uint8_t *in_s, uint8_t *in_z, uint64_t *in_m, uint64_t *in_e,   
	uint8_t *out_s, uint8_t *out_z, uint64_t *out_m, uint64_t *out_e, uint8_t *hot
	) {

	FPArray in_flat = fpopArr[tid]->input(WHICHPARTY, sz, in_s, in_z, in_m, in_e, m_bits, e_bits) ;

	BoolArray sgn, zero ;
	FixArray _2, _3 ;
	std::tie(sgn, zero, _2, _3) = get_components(tid, in_flat) ;

	FPArray zero_flat = fpopArr[tid]->input<float>(ALICE, sz, (float)0.0, m_bits, e_bits) ;
	FPArray out_flat = fpopArr[tid]->if_else(sgn, zero_flat, in_flat) ;

	memcpy(out_s, out_flat.s, sz*sizeof(uint8_t)) ;
	memcpy(out_z, out_flat.z, sz*sizeof(uint8_t)) ;
	memcpy(out_m, out_flat.m, sz*sizeof(uint64_t)) ;
	memcpy(out_e, out_flat.e, sz*sizeof(uint64_t)) ;
	memcpy(hot, sgn.data, sz*sizeof(uint8_t))  ;
}


void Relu(
	int32_t s1, 
	vector<FPArray> &inArr, 
	vector<FPArray> &outArr,
	vector<BoolArray> &hotArr) {
	int m_bits, e_bits ;
	m_bits = inArr[0].m_bits ;
	e_bits = inArr[0].e_bits ;

	uint8_t *in_s = new uint8_t[s1] ;
	uint8_t *in_z = new uint8_t[s1] ;
	uint64_t *in_m = new uint64_t[s1] ;
	uint64_t *in_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		in_s[i] = inArr[i].s[0] ;
		in_z[i] = inArr[i].z[0] ;
		in_m[i] = inArr[i].m[0] ;
		in_e[i] = inArr[i].e[0] ;
	}

	uint8_t *out_s = new uint8_t[s1] ;
	uint8_t *out_z = new uint8_t[s1] ;
	uint64_t *out_m = new uint64_t[s1] ;
	uint64_t *out_e = new uint64_t[s1] ;
	uint8_t *hot = new uint8_t[s1] ;

	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(Relu_thread,
				i, chunks[i], m_bits, e_bits,
				in_s + offset, in_z + offset, in_m + offset, in_e + offset,
				out_s + offset, out_z + offset, out_m + offset, out_e + offset, hot + offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].m_bits = m_bits ;
		outArr[i].e_bits = e_bits ;

		outArr[i].s[0] = out_s[i] ;
		outArr[i].z[0] = out_z[i] ;
		outArr[i].m[0] = out_m[i] ;
		outArr[i].e[0] = out_e[i] ;
		hotArr[i].data[0] = hot[i] ;
	}

	delete[] in_s ; delete[] out_s ;
	delete[] in_z ; delete[] out_z ;
	delete[] in_m ; delete[] out_m ;
	delete[] in_e ; delete[] out_e ;
	delete[] hot ;
}

void vectorSum_thread(
	int tid, int chunk, int colsize, int m_bits, int e_bits,
	uint8_t **Row_s, uint8_t **Row_z, uint64_t **Row_m, uint64_t **Row_e,
	uint8_t *row_s, uint8_t *row_z, uint64_t *row_m, uint64_t *row_e
	) {
	vector<FPArray> sums ;
	for (int i = 0 ; i < chunk ; i++) {
		sums.push_back(
			fpopArr[tid]->input(
				WHICHPARTY, colsize, Row_s[i], Row_z[i], Row_m[i], Row_e[i], m_bits, e_bits
			)
		) ;
	}

	FPArray vsum = fpopArr[tid]->treesum(sums) ;

	memcpy(row_s, vsum.s, chunk*sizeof(uint8_t)) ;
	memcpy(row_z, vsum.z, chunk*sizeof(uint8_t)) ;
	memcpy(row_m, vsum.m, chunk*sizeof(uint64_t)) ;
	memcpy(row_e, vsum.e, chunk*sizeof(uint64_t)) ;
}

void getBiasDer(int32_t m, int32_t s2, vector<vector<FPArray>>& batchSoftDer, vector<FPArray> &biasDer) {
	int m_bits, e_bits ;
	m_bits = batchSoftDer[0][0].m_bits ;
	e_bits = batchSoftDer[0][0].e_bits ;

	uint8_t **Row_s = new uint8_t*[s2] ;
	uint8_t **Row_z = new uint8_t*[s2] ;
	uint64_t **Row_m = new uint64_t*[s2] ;
	uint64_t **Row_e = new uint64_t*[s2] ;

	uint8_t *row_s = new uint8_t[s2] ;
	uint8_t *row_z = new uint8_t[s2] ;
	uint64_t *row_m = new uint64_t[s2] ;
	uint64_t *row_e = new uint64_t[s2] ;

	for (int i = 0 ; i < s2 ; i++) {
		Row_s[i] = new uint8_t[m] ;
		Row_z[i] = new uint8_t[m] ;
		Row_m[i] = new uint64_t[m] ;
		Row_e[i] = new uint64_t[m] ;

		for (int j = 0 ; j < m ; j++) {
			Row_s[i][j] = batchSoftDer[j][i].s[0] ;
			Row_z[i][j] = batchSoftDer[j][i].z[0] ;
			Row_m[i][j] = batchSoftDer[j][i].m[0] ;
			Row_e[i][j] = batchSoftDer[j][i].e[0] ;
		}
	}


	vector<int> chunks = get_chunks(s2, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(vectorSum_thread,
				i, chunks[i], m, m_bits, e_bits,
				Row_s+offset, Row_z+offset, Row_m+offset, Row_e+offset,
				row_s+offset, row_z+offset, row_m+offset, row_e+offset
			) ;
			offset += chunks[i] ;
		}
	}	

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	for (int i = 0 ; i < s2 ; i++) {
		biasDer[i].m_bits = m_bits ;
		biasDer[i].e_bits = e_bits ;

		biasDer[i].s[0] = row_s[i] ;
		biasDer[i].z[0] = row_z[i] ;
		biasDer[i].m[0] = row_m[i] ;
		biasDer[i].e[0] = row_e[i] ;
	}

	for (int i = 0 ; i < s2 ; i++) {
		delete[] Row_s[i] ;
		delete[] Row_z[i] ;
		delete[] Row_m[i] ;
		delete[] Row_e[i] ;
	}

	delete[] Row_s ; delete[] row_s ;
	delete[] Row_z ; delete[] row_z ;
	delete[] Row_m ; delete[] row_m ;
	delete[] Row_e ; delete[] row_e ;
}

void updateWeights_thread(
	int tid, int chunk, float lr, int m_bits, int e_bits,
	uint8_t *in_s, uint8_t *in_z, uint64_t *in_m, uint64_t *in_e,
	uint8_t *der_s, uint8_t *der_z, uint64_t *der_m, uint64_t *der_e
	) {
	FPArray weight = fpopArr[tid]->input(WHICHPARTY, chunk, in_s, in_z, in_m, in_e, m_bits, e_bits) ;
	FPArray der = fpopArr[tid]->input(WHICHPARTY, chunk, der_s, der_z, der_m, der_e, m_bits, e_bits) ;
	FPArray muller = fpopArr[tid]->input<float>(PUBLIC, chunk, lr, m_bits, e_bits) ;

	der = fpopArr[tid]->mul(der, muller) ;
	weight = fpopArr[tid]->sub(weight, der) ;

	memcpy(in_s, weight.s, chunk*sizeof(uint8_t)) ;
	memcpy(in_z, weight.z, chunk*sizeof(uint8_t)) ;
	memcpy(in_m, weight.m, chunk*sizeof(uint64_t)) ;
	memcpy(in_e, weight.e, chunk*sizeof(uint64_t)) ;
}

void updateWeights(
	int32_t sz, float lr, 
	vector<FPArray>& inArr, vector<FPArray>& derArr) {
	int m_bits, e_bits ;
	m_bits = inArr[0].m_bits ;
	e_bits = inArr[0].e_bits ;

	uint8_t *in_s = new uint8_t[sz] ;
	uint8_t *in_z = new uint8_t[sz] ;
	uint64_t *in_m = new uint64_t[sz] ;
	uint64_t *in_e = new uint64_t[sz] ;

	uint8_t *der_s = new uint8_t[sz] ;
	uint8_t *der_z = new uint8_t[sz] ;
	uint64_t *der_m = new uint64_t[sz] ;
	uint64_t *der_e = new uint64_t[sz] ;

	for (int i = 0 ; i < sz ; i++) {
		in_s[i] = inArr[i].s[0] ; der_s[i] = derArr[i].s[0] ;
		in_z[i] = inArr[i].z[0] ; der_z[i] = derArr[i].z[0] ;
		in_m[i] = inArr[i].m[0] ; der_m[i] = derArr[i].m[0] ;
		in_e[i] = inArr[i].e[0] ; der_e[i] = derArr[i].e[0] ;
	}

	vector<int> chunks = get_chunks(sz, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(updateWeights_thread,
				i, chunks[i], lr, m_bits, e_bits,
				in_s+offset, in_z+offset, in_m+offset, in_e+offset,
				der_s+offset, der_z+offset, der_m+offset, der_e+offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	for (int i = 0 ; i < sz ; i++) {
		inArr[i].m_bits = m_bits ;
		inArr[i].e_bits = e_bits ;

		inArr[i].s[0] = in_s[i] ;
		inArr[i].z[0] = in_z[i] ;
		inArr[i].m[0] = in_m[i] ;
		inArr[i].e[0] = in_e[i] ;
	}

	delete[] in_s ; delete[] der_s ;
	delete[] in_z ; delete[] der_z ;
	delete[] in_m ; delete[] der_m ;
	delete[] in_e ; delete[] der_e ;
}


void updateWeightsMomentum_thread(
	int tid, int chunk, float lr, float beta, int m_bits, int e_bits,
	uint8_t *in_s, uint8_t *in_z, uint64_t *in_m, uint64_t *in_e,
	uint8_t *der_s, uint8_t *der_z, uint64_t *der_m, uint64_t *der_e,
	uint8_t *mom_s, uint8_t *mom_z, uint64_t *mom_m, uint64_t *mom_e
	) {

	FPArray weight = fpopArr[tid]->input(WHICHPARTY, chunk, in_s, in_z, in_m, in_e, m_bits, e_bits) ;
	FPArray der = fpopArr[tid]->input(WHICHPARTY, chunk, der_s, der_z, der_m, der_e, m_bits, e_bits) ;
	FPArray mom = fpopArr[tid]->input(WHICHPARTY, chunk, mom_s, mom_z, mom_m, mom_e, m_bits, e_bits) ;
	FPArray muller_lr = fpopArr[tid]->input<float>(PUBLIC, chunk, lr, m_bits, e_bits) ;
	FPArray muller_beta = fpopArr[tid]->input<float>(PUBLIC, chunk, beta, m_bits, e_bits) ;
	FPArray muller_Nbeta = fpopArr[tid]->input<float>(PUBLIC, chunk, (float)1.0-beta, m_bits, e_bits) ;

	mom = fpopArr[tid]->add(
		fpopArr[tid]->mul(muller_beta, mom), 
		fpopArr[tid]->mul(muller_Nbeta, der)
	) ;
	der = fpopArr[tid]->mul(muller_lr, mom) ;
	weight = fpopArr[tid]->sub(weight, der) ;

	memcpy(in_s, weight.s, chunk*sizeof(uint8_t)) ; memcpy(mom_s, mom.s, chunk*sizeof(uint8_t)) ;
	memcpy(in_z, weight.z, chunk*sizeof(uint8_t)) ; memcpy(mom_z, mom.z, chunk*sizeof(uint8_t)) ;
	memcpy(in_m, weight.m, chunk*sizeof(uint64_t)) ; memcpy(mom_m, mom.m, chunk*sizeof(uint64_t)) ;
	memcpy(in_e, weight.e, chunk*sizeof(uint64_t)) ; memcpy(mom_e, mom.e, chunk*sizeof(uint64_t)) ;
}

void updateWeightsMomentum(
	int32_t sz, float lr, float beta, 
	vector<FPArray>& inArr, vector<FPArray>& derArr, vector<FPArray> &momArr
	) {
	int m_bits, e_bits ;
	m_bits = inArr[0].m_bits ;
	e_bits = inArr[0].e_bits ;

	uint8_t *in_s = new uint8_t[sz] ;
	uint8_t *in_z = new uint8_t[sz] ;
	uint64_t *in_m = new uint64_t[sz] ;
	uint64_t *in_e = new uint64_t[sz] ;

	uint8_t *der_s = new uint8_t[sz] ;
	uint8_t *der_z = new uint8_t[sz] ;
	uint64_t *der_m = new uint64_t[sz] ;
	uint64_t *der_e = new uint64_t[sz] ;

	uint8_t *mom_s = new uint8_t[sz] ;
	uint8_t *mom_z = new uint8_t[sz] ;
	uint64_t *mom_m = new uint64_t[sz] ;
	uint64_t *mom_e = new uint64_t[sz] ;

	for (int i = 0 ; i < sz ; i++) {
		in_s[i] = inArr[i].s[0] ; der_s[i] = derArr[i].s[0] ; mom_s[i] = momArr[i].s[0] ;
		in_z[i] = inArr[i].z[0] ; der_z[i] = derArr[i].z[0] ; mom_z[i] = momArr[i].z[0] ;
		in_m[i] = inArr[i].m[0] ; der_m[i] = derArr[i].m[0] ; mom_m[i] = momArr[i].m[0] ;
		in_e[i] = inArr[i].e[0] ; der_e[i] = derArr[i].e[0] ; mom_e[i] = momArr[i].e[0] ;
	}

	vector<int> chunks = get_chunks(sz, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(updateWeightsMomentum_thread,
				i, chunks[i], lr, beta, m_bits, e_bits,
				in_s+offset, in_z+offset, in_m+offset, in_e+offset,
				der_s+offset, der_z+offset, der_m+offset, der_e+offset,
				mom_s+offset, mom_z+offset, mom_m+offset, mom_e+offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	for (int i = 0 ; i < sz ; i++) {
		inArr[i].m_bits = m_bits ; momArr[i].m_bits = m_bits ;
		inArr[i].e_bits = e_bits ; momArr[i].e_bits = e_bits ;

		inArr[i].s[0] = in_s[i] ; momArr[i].s[0] = mom_s[i] ;
		inArr[i].z[0] = in_z[i] ; momArr[i].z[0] = mom_z[i] ;
		inArr[i].m[0] = in_m[i] ; momArr[i].m[0] = mom_m[i] ;
		inArr[i].e[0] = in_e[i] ; momArr[i].e[0] = mom_e[i] ;
	}

	delete[] in_s ; delete[] der_s ; delete[] mom_s ;
	delete[] in_z ; delete[] der_z ; delete[] mom_z ;
	delete[] in_m ; delete[] der_m ; delete[] mom_m ;
	delete[] in_e ; delete[] der_e ; delete[] mom_e ;

}


void updateWeightsAdam_thread(
	int tid, int chunk, int t, float lr, float beta1, float beta2, float eps, int m_bits, int e_bits,
	uint8_t *in_s, uint8_t *in_z, uint64_t *in_m, uint64_t *in_e,
	uint8_t *der_s, uint8_t *der_z, uint64_t *der_m, uint64_t *der_e,
	uint8_t *m_s, uint8_t *m_z, uint64_t *m_m, uint64_t *m_e,
	uint8_t *v_s, uint8_t *v_z, uint64_t *v_m, uint64_t *v_e
	) {
	FPArray weight = fpopArr[tid]->input(tid&1?3-__party:__party, chunk, in_s, in_z, in_m, in_e, m_bits, e_bits) ;
	FPArray der = fpopArr[tid]->input(tid&1?3-__party:__party, chunk, der_s, der_z, der_m, der_e, m_bits, e_bits) ;
	FPArray m_t = fpopArr[tid]->input(tid&1?3-__party:__party, chunk, m_s, m_z, m_m, m_e, m_bits, e_bits) ;
	FPArray v_t = fpopArr[tid]->input(tid&1?3-__party:__party, chunk, v_s, v_z, v_m, v_e, m_bits, e_bits) ;

    FPArray muller = fpopArr[tid]->input<float>(ALICE, chunk, lr, m_bits, e_bits) ;
    FPArray b1 = fpopArr[tid]->input<float>(ALICE, chunk, beta1, m_bits, e_bits) ;
    FPArray b2 = fpopArr[tid]->input<float>(ALICE, chunk, beta2, m_bits, e_bits) ;
	FPArray e = fpopArr[tid]->input<float>(ALICE, chunk, eps, m_bits, e_bits) ;
	FPArray sub1=fpopArr[tid]->input<float>(ALICE, chunk, (float)1.0, m_bits, e_bits);

    m_t=fpopArr[tid]->add(fpopArr[tid]->mul(m_t, b1), fpopArr[tid]->mul(der, fpopArr[tid]->sub(sub1, b1)));
	v_t=fpopArr[tid]->add(fpopArr[tid]->mul(v_t, b2), fpopArr[tid]->mul(fpopArr[tid]->mul(der, der), fpopArr[tid]->sub(sub1, b2)));

	memcpy(m_s, m_t.s, chunk*sizeof(uint8_t)) ;
	memcpy(m_z, m_t.z, chunk*sizeof(uint8_t)) ;
	memcpy(m_m, m_t.m, chunk*sizeof(uint64_t)) ;
	memcpy(m_e, m_t.e, chunk*sizeof(uint64_t)) ;

	memcpy(v_s, v_t.s, chunk*sizeof(uint8_t)) ;
	memcpy(v_z, v_t.z, chunk*sizeof(uint8_t)) ;
	memcpy(v_m, v_t.m, chunk*sizeof(uint64_t)) ;
	memcpy(v_e, v_t.e, chunk*sizeof(uint64_t)) ;

	FPArray b1t = fpopArr[tid]->input<float>(ALICE, chunk, float(pow(beta1, t)), m_bits, e_bits) ;
	FPArray b2t = fpopArr[tid]->input<float>(ALICE, chunk, float(pow(beta2, t)), m_bits, e_bits) ;
	
	m_t=fpopArr[tid]->div(m_t, fpopArr[tid]->sub(sub1, b1t));
	v_t=fpopArr[tid]->div(v_t, fpopArr[tid]->sub(sub1, b2t));
    
    der=fpopArr[tid]->div(m_t, fpopArr[tid]->add(fpopArr[tid]->sqrt(v_t), e));
	der = fpopArr[tid]->mul(der, muller) ;
	weight = fpopArr[tid]->sub(weight, der) ;

	memcpy(in_s, weight.s, chunk*sizeof(uint8_t)) ;
	memcpy(in_z, weight.z, chunk*sizeof(uint8_t)) ;
	memcpy(in_m, weight.m, chunk*sizeof(uint64_t)) ;
	memcpy(in_e, weight.e, chunk*sizeof(uint64_t)) ;
}


void updateWeightsAdam(
	int32_t sz, int32_t t, float lr, float beta1, float beta2, float eps, 
	vector<FPArray>& inArr, vector<FPArray>& derArr, vector<FPArray>& mArr, vector<FPArray>& vArr)
{
	int m_bits, e_bits ;
	m_bits = inArr[0].m_bits ;
	e_bits = inArr[0].e_bits ;

	uint8_t *in_s = new uint8_t[sz] ;
	uint8_t *in_z = new uint8_t[sz] ;
	uint64_t *in_m = new uint64_t[sz] ;
	uint64_t *in_e = new uint64_t[sz] ;

	uint8_t *der_s = new uint8_t[sz] ;
	uint8_t *der_z = new uint8_t[sz] ;
	uint64_t *der_m = new uint64_t[sz] ;
	uint64_t *der_e = new uint64_t[sz] ;
	
	uint8_t *m_s = new uint8_t[sz] ;
	uint8_t *m_z = new uint8_t[sz] ;
	uint64_t *m_m = new uint64_t[sz] ;
	uint64_t *m_e = new uint64_t[sz] ;

	uint8_t *v_s = new uint8_t[sz] ;
	uint8_t *v_z = new uint8_t[sz] ;
	uint64_t *v_m = new uint64_t[sz] ;
	uint64_t *v_e = new uint64_t[sz] ;

	for (int i = 0 ; i < sz ; i++) {
		in_s[i] = inArr[i].s[0] ; der_s[i] = derArr[i].s[0] ; 
		in_z[i] = inArr[i].z[0] ; der_z[i] = derArr[i].z[0] ;
		in_m[i] = inArr[i].m[0] ; der_m[i] = derArr[i].m[0] ;
		in_e[i] = inArr[i].e[0] ; der_e[i] = derArr[i].e[0] ; 
		m_s[i] = mArr[i].s[0] ; 
		m_z[i] = mArr[i].z[0] ;
		m_m[i] = mArr[i].m[0] ;
		m_e[i] = mArr[i].e[0] ; 
		v_s[i] = vArr[i].s[0] ; 
		v_z[i] = vArr[i].z[0] ;
		v_m[i] = vArr[i].m[0] ;
		v_e[i] = vArr[i].e[0] ; 
	}

	vector<int> chunks = get_chunks(sz, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(updateWeightsAdam_thread,
				i, chunks[i], t, lr, beta1, beta2, eps, m_bits, e_bits,
				in_s+offset, in_z+offset, in_m+offset, in_e+offset,
				der_s+offset, der_z+offset, der_m+offset, der_e+offset,
				m_s+offset, m_z+offset, m_m+offset, m_e+offset,
				v_s+offset, v_z+offset, v_m+offset, v_e+offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	for (int i = 0 ; i < sz ; i++) {
		inArr[i].m_bits = m_bits ;
		inArr[i].e_bits = e_bits ;

		inArr[i].s[0] = in_s[i] ;
		inArr[i].z[0] = in_z[i] ;
		inArr[i].m[0] = in_m[i] ;
		inArr[i].e[0] = in_e[i] ;
	}

	delete[] in_s ; delete[] der_s ;
	delete[] in_z ; delete[] der_z ;
	delete[] in_m ; delete[] der_m ;
	delete[] in_e ; delete[] der_e ; 
	delete[] v_e; delete[] m_m;
	delete[] v_s; delete[] m_z;
	delete[] v_z; delete[] m_s;
	delete[] v_m; delete[] m_e;
}



void Softmax2_thread(
	int tid, int mchunk, int n, int m_bits, int e_bits,
	uint8_t **in_s, uint8_t **in_z, uint64_t **in_m, uint64_t **in_e,
	uint8_t **out_s, uint8_t **out_z, uint64_t **out_m, uint64_t **out_e
	) {
	vector<FPArray> softin, softout ;

	for (int i = 0 ; i < mchunk ; i++)
		softin.push_back(fpopArr[tid]->input(WHICHPARTY, n, in_s[i], in_z[i], in_m[i], in_e[i], m_bits, e_bits)) ;

	softout = fpmathArr[tid]->softmax_old(softin) ;	
	for (int i = 0 ; i < mchunk ; i++) {
		memcpy(out_s[i], softout[i].s, n*sizeof(uint8_t)) ;
		memcpy(out_z[i], softout[i].z, n*sizeof(uint8_t)) ;
		memcpy(out_m[i], softout[i].m, n*sizeof(uint64_t)) ;
		memcpy(out_e[i], softout[i].e, n*sizeof(uint64_t)) ;
	}
}

void Softmax2(
	int32_t s1, 
	int32_t s2, 
	vector<vector<FPArray>> &inArr, 
	vector<vector<FPArray>> &outArr) {
	int m_bits = inArr[0][0].m_bits ;
	int e_bits = inArr[0][0].e_bits ;

	uint8_t **row_s = new uint8_t*[s1] ;
	uint8_t **row_z = new uint8_t*[s1] ;
	uint64_t **row_m = new uint64_t*[s1] ;
	uint64_t **row_e = new uint64_t*[s1] ;

	uint8_t **out_s = new uint8_t*[s1] ;
	uint8_t **out_z = new uint8_t*[s1] ;
	uint64_t **out_m = new uint64_t*[s1] ;
	uint64_t **out_e = new uint64_t*[s1] ;

	for (int i = 0 ; i < s1 ; i++) {
		row_s[i] = new uint8_t[s2] ;
		row_z[i] = new uint8_t[s2] ;
		row_m[i] = new uint64_t[s2] ;
		row_e[i] = new uint64_t[s2] ;

		out_s[i] = new uint8_t[s2] ;
		out_z[i] = new uint8_t[s2] ;
		out_m[i] = new uint64_t[s2] ;
		out_e[i] = new uint64_t[s2] ;

		for (int j = 0 ; j < s2 ; j++) {
			row_s[i][j] = inArr[i][j].s[0] ;
			row_z[i][j] = inArr[i][j].z[0] ;
			row_m[i][j] = inArr[i][j].m[0] ;
			row_e[i][j] = inArr[i][j].e[0] ;
		}
	}

	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(Softmax2_thread,
				i, chunks[i], s2, m_bits, e_bits,
				row_s+offset, row_z+offset, row_m+offset, row_e+offset,
				out_s+offset, out_z+offset, out_m+offset, out_e+offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;


	for (int i = 0 ; i < s1 ; i++) {
		for (int j = 0 ; j < s2 ; j++) {
			outArr[i][j].s[0] = out_s[i][j] ;
			outArr[i][j].z[0] = out_z[i][j] ;
			outArr[i][j].m[0] = out_m[i][j] ;
			outArr[i][j].e[0] = out_e[i][j] ;
		}
	}

	for (int i = 0 ; i < s1 ; i++) {
		delete[] row_s[i] ; delete[] out_s[i] ;
		delete[] row_z[i] ; delete[] out_z[i] ;
		delete[] row_m[i] ; delete[] out_m[i] ;
		delete[] row_e[i] ; delete[] out_e[i] ;
	}

	delete[] row_s ; delete[] out_s ;
	delete[] row_z ; delete[] out_z ;
	delete[] row_m ; delete[] out_m ;
	delete[] row_e ; delete[] out_e ;
}

void Ln_thread(
	int tid, int sz, int m_bits, int e_bits,
	uint8_t *in_s, uint8_t *in_z, uint64_t *in_m, uint64_t *in_e,   
	uint8_t *out_s, uint8_t *out_z, uint64_t *out_m, uint64_t *out_e
	) {

	FPArray in_flat = fpopArr[tid]->input(WHICHPARTY, sz, in_s, in_z, in_m, in_e, m_bits, e_bits) ;
	FPArray out_flat = fpmathArr[tid]->ln(in_flat) ;

	memcpy(out_s, out_flat.s, sz*sizeof(uint8_t)) ;
	memcpy(out_z, out_flat.z, sz*sizeof(uint8_t)) ;
	memcpy(out_m, out_flat.m, sz*sizeof(uint64_t)) ;
	memcpy(out_e, out_flat.e, sz*sizeof(uint64_t)) ;
}

void Ln(
	int32_t s1, 
	vector<FPArray> &inArr, 
	vector<FPArray> &outArr) {
	int m_bits, e_bits ;
	m_bits = inArr[0].m_bits ;
	e_bits = inArr[0].e_bits ;

	uint8_t *in_s = new uint8_t[s1] ;
	uint8_t *in_z = new uint8_t[s1] ;
	uint64_t *in_m = new uint64_t[s1] ;
	uint64_t *in_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		in_s[i] = inArr[i].s[0] ;
		in_z[i] = inArr[i].z[0] ;
		in_m[i] = inArr[i].m[0] ;
		in_e[i] = inArr[i].e[0] ;
	}

	uint8_t *out_s = new uint8_t[s1] ;
	uint8_t *out_z = new uint8_t[s1] ;
	uint64_t *out_m = new uint64_t[s1] ;
	uint64_t *out_e = new uint64_t[s1] ;
	uint8_t *hot = new uint8_t[s1] ;

	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(Ln_thread,
				i, chunks[i], m_bits, e_bits,
				in_s + offset, in_z + offset, in_m + offset, in_e + offset,
				out_s + offset, out_z + offset, out_m + offset, out_e + offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].m_bits = m_bits ;
		outArr[i].e_bits = e_bits ;

		outArr[i].s[0] = out_s[i] ;
		outArr[i].z[0] = out_z[i] ;
		outArr[i].m[0] = out_m[i] ;
		outArr[i].e[0] = out_e[i] ;
	}

	delete[] in_s ; delete[] out_s ;
	delete[] in_z ; delete[] out_z ;
	delete[] in_m ; delete[] out_m ;
	delete[] in_e ; delete[] out_e ;
}

void Sigmoid_thread(
	int tid, int sz, int m_bits, int e_bits,
	uint8_t *in_s, uint8_t *in_z, uint64_t *in_m, uint64_t *in_e,   
	uint8_t *out_s, uint8_t *out_z, uint64_t *out_m, uint64_t *out_e
	) {

	FPArray in_flat = fpopArr[tid]->input(WHICHPARTY, sz, in_s, in_z, in_m, in_e, m_bits, e_bits) ;
	FPArray out_flat ;
	if (m_bits == 23)
	out_flat = fpmathArr[tid]->sigmoid_fp32(in_flat) ;
	else if (m_bits == 7)
	out_flat = fpmathArr[tid]->sigmoid_bf16(in_flat) ; 

	memcpy(out_s, out_flat.s, sz*sizeof(uint8_t)) ;
	memcpy(out_z, out_flat.z, sz*sizeof(uint8_t)) ;
	memcpy(out_m, out_flat.m, sz*sizeof(uint64_t)) ;
	memcpy(out_e, out_flat.e, sz*sizeof(uint64_t)) ;
}

void Sigmoid(
	int32_t s1, 
	vector<FPArray> &inArr, 
	vector<FPArray> &outArr) {
	int m_bits, e_bits ;
	m_bits = inArr[0].m_bits ;
	e_bits = inArr[0].e_bits ;

	uint8_t *in_s = new uint8_t[s1] ;
	uint8_t *in_z = new uint8_t[s1] ;
	uint64_t *in_m = new uint64_t[s1] ;
	uint64_t *in_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		in_s[i] = inArr[i].s[0] ;
		in_z[i] = inArr[i].z[0] ;
		in_m[i] = inArr[i].m[0] ;
		in_e[i] = inArr[i].e[0] ;
	}

	uint8_t *out_s = new uint8_t[s1] ;
	uint8_t *out_z = new uint8_t[s1] ;
	uint64_t *out_m = new uint64_t[s1] ;
	uint64_t *out_e = new uint64_t[s1] ;
	uint8_t *hot = new uint8_t[s1] ;

	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(Sigmoid_thread,
				i, chunks[i], m_bits, e_bits,
				in_s + offset, in_z + offset, in_m + offset, in_e + offset,
				out_s + offset, out_z + offset, out_m + offset, out_e + offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].m_bits = m_bits ;
		outArr[i].e_bits = e_bits ;

		outArr[i].s[0] = out_s[i] ;
		outArr[i].z[0] = out_z[i] ;
		outArr[i].m[0] = out_m[i] ;
		outArr[i].e[0] = out_e[i] ;
	}

	delete[] in_s ; delete[] out_s ;
	delete[] in_z ; delete[] out_z ;
	delete[] in_m ; delete[] out_m ;
	delete[] in_e ; delete[] out_e ;
}

void Tanh_thread(
	int tid, int sz, int m_bits, int e_bits,
	uint8_t *in_s, uint8_t *in_z, uint64_t *in_m, uint64_t *in_e,   
	uint8_t *out_s, uint8_t *out_z, uint64_t *out_m, uint64_t *out_e
	) {

	FPArray in_flat = fpopArr[tid]->input(WHICHPARTY, sz, in_s, in_z, in_m, in_e, m_bits, e_bits) ;

	FPArray out_flat ;
	if (m_bits == 23)
		out_flat = fpmathArr[tid]->tanh_fp32(in_flat) ;
	else if (m_bits == 7)
		out_flat = fpmathArr[tid]->tanh_bf16(in_flat) ;

	memcpy(out_s, out_flat.s, sz*sizeof(uint8_t)) ;
	memcpy(out_z, out_flat.z, sz*sizeof(uint8_t)) ;
	memcpy(out_m, out_flat.m, sz*sizeof(uint64_t)) ;
	memcpy(out_e, out_flat.e, sz*sizeof(uint64_t)) ;
}

void Tanh(
	int32_t s1, 
	vector<FPArray> &inArr, 
	vector<FPArray> &outArr) {
	int m_bits, e_bits ;
	m_bits = inArr[0].m_bits ;
	e_bits = inArr[0].e_bits ;

	uint8_t *in_s = new uint8_t[s1] ;
	uint8_t *in_z = new uint8_t[s1] ;
	uint64_t *in_m = new uint64_t[s1] ;
	uint64_t *in_e = new uint64_t[s1] ;
	for (int i = 0 ; i < s1 ; i++) {
		in_s[i] = inArr[i].s[0] ;
		in_z[i] = inArr[i].z[0] ;
		in_m[i] = inArr[i].m[0] ;
		in_e[i] = inArr[i].e[0] ;
	}

	uint8_t *out_s = new uint8_t[s1] ;
	uint8_t *out_z = new uint8_t[s1] ;
	uint64_t *out_m = new uint64_t[s1] ;
	uint64_t *out_e = new uint64_t[s1] ;
	uint8_t *hot = new uint8_t[s1] ;

	vector<int> chunks = get_chunks(s1, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(Tanh_thread,
				i, chunks[i], m_bits, e_bits,
				in_s + offset, in_z + offset, in_m + offset, in_e + offset,
				out_s + offset, out_z + offset, out_m + offset, out_e + offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].m_bits = m_bits ;
		outArr[i].e_bits = e_bits ;

		outArr[i].s[0] = out_s[i] ;
		outArr[i].z[0] = out_z[i] ;
		outArr[i].m[0] = out_m[i] ;
		outArr[i].e[0] = out_e[i] ;
	}

	delete[] in_s ; delete[] out_s ;
	delete[] in_z ; delete[] out_z ;
	delete[] in_m ; delete[] out_m ;
	delete[] in_e ; delete[] out_e ;
}

void dotProduct2(int32_t s1, int32_t s2, vector<vector<FPArray>>& arr1, vector<vector<FPArray>>& arr2, vector<FPArray>& outArr) {
	int m_bits, e_bits ;
	m_bits = arr1[0][0].m_bits ;
	e_bits = arr1[0][0].e_bits ;

	vector<FPArray> dot1, dot2 ;

	uint8_t *arr1_s = new uint8_t[s2] ;
	uint8_t *arr1_z = new uint8_t[s2] ;
	uint64_t *arr1_m = new uint64_t[s2] ;
	uint64_t *arr1_e = new uint64_t[s2] ;

	uint8_t *arr2_s = new uint8_t[s2] ;
	uint8_t *arr2_z = new uint8_t[s2] ;
	uint64_t *arr2_m = new uint64_t[s2] ;
	uint64_t *arr2_e = new uint64_t[s2] ;

	for (int i = 0 ; i < s1 ; i++) {
		for (int j = 0 ; j < s2 ; j++) {
			arr1_s[j] = arr1[i][j].s[0] ; arr2_s[j] = arr2[i][j].s[0] ;
			arr1_z[j] = arr1[i][j].z[0] ; arr2_z[j] = arr2[i][j].z[0] ;
			arr1_m[j] = arr1[i][j].m[0] ; arr2_m[j] = arr2[i][j].m[0] ;
			arr1_e[j] = arr1[i][j].e[0] ; arr2_e[j] = arr2[i][j].e[0] ;
		}

		dot1.push_back(__fp_op->input(__party, s2, arr1_s, arr1_z, arr1_m, arr1_e, m_bits, e_bits)) ;
		dot2.push_back(__fp_op->input(__party, s2, arr2_s, arr2_z, arr2_m, arr2_e, m_bits, e_bits)) ;
	}

	FPArray res = __fp_op->treesum(__fp_op->mul(dot1, dot2)) ;
	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].s[0] = res.s[i] ;
		outArr[i].z[0] = res.z[i] ;
		outArr[i].m[0] = res.m[i] ;
		outArr[i].e[0] = res.e[i] ;
	}

	delete[] arr1_s ; delete[] arr2_s ;
	delete[] arr1_z ; delete[] arr2_z ;
	delete[] arr1_m ; delete[] arr2_m ;
	delete[] arr1_e ; delete[] arr2_e ;
}

void vsumIfElse(int32_t s1, int32_t s2, vector<vector<FPArray>>& arr, vector<vector<BoolArray>>& condArr, vector<FPArray>& outArr) {
	int m_bits, e_bits ;
	int sz = s1*s2 ;
	m_bits = arr[0][0].m_bits ;
	e_bits = arr[0][0].e_bits ;

	vector<FPArray> arr_flat = make_vector_float(ALICE, sz) ;
	vector<BoolArray> cond_flat = make_vector_bool(ALICE, sz) ;
	vector<FPArray> conded_flat = make_vector_float(ALICE, sz) ;

	IfElse(sz, arr_flat, cond_flat, conded_flat) ;

	uint8_t *conded_s = new uint8_t[s2] ;
	uint8_t *conded_z = new uint8_t[s2] ;
	uint64_t *conded_m = new uint64_t[s2] ;
	uint64_t *conded_e = new uint64_t[s2] ;

	vector<FPArray> dot ;
	for (int i = 0 ; i < s1 ; i++) {
		for (int j = 0 ; j < s2 ; j++) {
			conded_s[j] = conded_flat[i*s2 + j].s[0] ;
			conded_z[j] = conded_flat[i*s2 + j].z[0] ;
			conded_m[j] = conded_flat[i*s2 + j].m[0] ;
			conded_e[j] = conded_flat[i*s2 + j].e[0] ;
		}
		dot.push_back(__fp_op->input(__party, s2, conded_s, conded_z, conded_m, conded_e, m_bits, e_bits)) ;
	}

	FPArray res = __fp_op->treesum(dot) ;
	for (int i = 0 ; i < s1 ; i++) {
		outArr[i].s[0] = res.s[i] ;
		outArr[i].z[0] = res.z[i] ;
		outArr[i].m[0] = res.m[i] ;
		outArr[i].e[0] = res.e[i] ;
	}

	delete[] conded_s ;
	delete[] conded_z ;
	delete[] conded_m ;
	delete[] conded_e ;
}

void getLoss(int32_t s, vector<FPArray>& arr, vector<FPArray>& outArr) {
	int m_bits, e_bits ;
	m_bits = arr[0].m_bits ;
	e_bits = arr[0].e_bits ;

	uint8_t *in_s = new uint8_t[s] ;
	uint8_t *in_z = new uint8_t[s] ;
	uint64_t *in_m = new uint64_t[s] ;
	uint64_t *in_e = new uint64_t[s] ;

	for (int i = 0 ; i < s ; i++) {
		in_s[i] = arr[i].s[0] ;
		in_z[i] = arr[i].z[0] ;
		in_m[i] = arr[i].m[0] ;
		in_e[i] = arr[i].e[0] ;
	}

	vector<FPArray> sum ;
	sum.push_back(__fp_op->input(__party, s, in_s, in_z, in_m, in_e, m_bits, e_bits)) ;
	
	FPArray res = __fp_op->treesum(sum) ;
	FPArray div = __fp_op->input<float>(ALICE, 1, (float)-1.0/s, m_bits, e_bits) ;
	res = __fp_op->mul(res, div) ;

	outArr[0].s[0] = res.s[0] ;
	outArr[0].z[0] = res.z[0] ;
	outArr[0].m[0] = res.m[0] ;
	outArr[0].e[0] = res.e[0] ;

	delete[] in_s ;
	delete[] in_z ;
	delete[] in_m ;
	delete[] in_e ;
}

// void computeMSELoss(int32_t m, int32_t s, vector<vector<FPArray>>& target, vector<vector<FPArray>>& fwdOut, vector<FPArray>& loss) {
// 	vector<FPArray> target_flat = make_vector_float(ALICE, m) ;
// 	vector<FPArray> out_flat = make_vector_float(ALICE, m) ;

// 	for (int i = 0 ; i < m ; i++) {
// 		target_flat[i] = target[i][0] ;
// 		out_flat[i] = fwdOut[i][0] ;
// 	}

// 	vector<FPArray> subbed = make_vector_float(ALICE, m) ;
// 	vector<FPArray> loss_terms = make_vector_float(ALICE, m) ;

// 	ElemWiseSub(m, out_flat, target_flat, subbed) ;
// 	ElemWiseMul(m, subbed, subbed, loss_terms) ;
// 	getLoss(m, loss_terms, loss) ;
// }

// void computeCELoss(int32_t m, int32_t s2, vector < vector < FPArray > >& labels, vector < vector < FPArray > >& batchSoft, vector < FPArray >& loss){

// vector < vector < FPArray > > batchLn = make_vector_float(ALICE, m, s2) ;
// vector < FPArray > lossTerms = make_vector_float(ALICE, m) ;

// Ln2(m, s2, batchSoft, batchLn);
// dotProduct2(m, s2, batchLn, labels, lossTerms);
// getLoss(m, lossTerms, loss);
// }

void Conv2DReshapeMatMulOPGroup(int32_t N, int32_t finalH, int32_t finalW, int32_t CO, int32_t g, int32_t G, vector<vector<FPArray>>& inputArr, vector<vector<vector<vector<FPArray>>>>& outputArr){
	int32_t COG = (CO / G) ;
	int32_t startCO = (g * COG) ;
	for (uint32_t co = 0; co < COG; co++) {
		for (uint32_t n = 0; n < N; n++) {
			for (uint32_t h = 0; h < finalH; h++) {
				for (uint32_t w = 0; w < finalW; w++) {
					outputArr[n][h][w][(co + startCO)] = inputArr[co][((((n * finalH) * finalW) + (h * finalW)) + w)] ;
				}
			}
		}
	}
}

void Conv2DReshapeFilterGroup(int32_t FH, int32_t FW, int32_t CI, int32_t CO, int32_t g, int32_t G, vector<vector<vector<vector<FPArray>>>>& inputArr, vector<vector<FPArray>>& outputArr){
	int32_t CIG = (CI / G) ;
	int32_t COG = (CO / G) ;
	int32_t startCO = (g * COG) ;

	for (uint32_t co = 0; co < COG; co++) {
		for (uint32_t fh = 0; fh < FH; fh++) {
			for (uint32_t fw = 0; fw < FW; fw++) {
				for (uint32_t ci = 0; ci < CIG; ci++) {
					int32_t linIdx = ((((fh * FW) * CIG) + (fw * CIG)) + ci) ;
					outputArr[co][linIdx] = inputArr[fh][fw][ci][(co + startCO)] ;
				}
			}
		}
	}
}

void Conv2DReshapeInputGroup(int32_t N, int32_t H, int32_t W, int32_t CI, int32_t FH, int32_t FW, int32_t zPadHLeft, int32_t zPadHRight, int32_t zPadWLeft, int32_t zPadWRight, int32_t strideH, int32_t strideW, int32_t g, int32_t G, int32_t RRows, int32_t RCols, vector<vector<vector<vector<FPArray>>>>& inputArr,  vector<vector<FPArray>>& outputArr){
	int32_t linIdxFilterMult = 0 ;
	int32_t CIG = (CI / G) ;

	for (uint32_t n = 0; n < N; n++) {
		int32_t leftTopCornerH = (0 - zPadHLeft) ;
		int32_t extremeRightBottomCornerH = ((H - 1) + zPadHRight) ;

		while ((((leftTopCornerH + FH) - 1) <= extremeRightBottomCornerH)) {
			int32_t leftTopCornerW = (0 - zPadWLeft) ;
			int32_t extremeRightBottomCornerW = ((W - 1) + zPadWRight) ;

			while ((((leftTopCornerW + FW) - 1) <= extremeRightBottomCornerW)) {
				for (uint32_t fh = 0; fh < FH; fh++) {
					for (uint32_t fw = 0; fw < FW; fw++) {

						int32_t curPosH = (leftTopCornerH + fh) ;
						int32_t curPosW = (leftTopCornerW + fw) ;
						FPArray val = __public_float_to_arithmetic(0.) ;
						int32_t startCI = (g * CIG) ;

						for (uint32_t ci = 0; ci < CIG; ci++) {
							if ((((curPosH < 0) || (curPosH >= H)) || ((curPosW < 0) || (curPosW >= W)))) {
								val = __public_float_to_arithmetic(0.) ;
							} else {
								val = inputArr[n][curPosH][curPosW][(ci + startCI)] ;
							}

							outputArr[((((fh * FW) * CIG) + (fw * CIG)) + ci)][linIdxFilterMult] = val ;
						}
					}
				}

				linIdxFilterMult = (linIdxFilterMult + 1) ;
				leftTopCornerW = (leftTopCornerW + strideW) ;
			}

			leftTopCornerH = (leftTopCornerH + strideH) ;
		}

	}
}


void Conv2DGroupWrapper(
	int32_t N, int32_t H, int32_t W, int32_t CI, int32_t FH, int32_t FW, int32_t CO, 
	int32_t zPadHLeft, int32_t zPadHRight, int32_t zPadWLeft, int32_t zPadWRight, 
	int32_t strideH, int32_t strideW, int32_t G, 
	vector<vector<vector<vector<FPArray>>>>& inputArr, 
	vector<vector<vector<vector<FPArray>>>>& filterArr, 
	vector<vector<vector<vector<FPArray>>>>& outArr) {

	int32_t CIG = (CI / G) ;
	int32_t reshapedFilterRows = (CO / G) ;
	int32_t reshapedFilterCols = ((FH * FW) * CIG) ;
	int32_t reshapedIPRows = ((FH * FW) * CIG) ;
	int32_t outH = ((((H + (zPadHLeft + zPadHRight)) - FH) / strideH) + 1) ;
	int32_t outW = ((((W + (zPadWLeft + zPadWRight)) - FW) / strideW) + 1) ;
	int32_t reshapedIPCols = ((N * outH) * outW) ;

	for (uint32_t g = 0; g < G; g++) {
		vector<vector<FPArray>> inputReshaped = make_vector_float(ALICE, reshapedIPRows, reshapedIPCols) ;
		vector<vector<FPArray>> filterReshaped = make_vector_float(ALICE, reshapedFilterRows, reshapedFilterCols) ;
		vector<vector<FPArray>> matmulOP = make_vector_float(ALICE, reshapedFilterRows, reshapedIPCols) ;

		Conv2DReshapeFilterGroup(FH, FW, CI, CO, g, G, filterArr, filterReshaped);
		Conv2DReshapeInputGroup(N, H, W, CI, FH, FW, zPadHLeft, zPadHRight, zPadWLeft, zPadWRight, strideH, strideW, g, G, reshapedIPRows, reshapedIPCols, inputArr, inputReshaped);
		MatMul(reshapedFilterRows, reshapedFilterCols, reshapedIPCols, filterReshaped, inputReshaped, matmulOP);
		Conv2DReshapeMatMulOPGroup(N, outH, outW, CO, g, G, matmulOP, outArr);
	}
}

void Maxpool_thread(
	int tid, int chunk, int filterSize, int m_bits, int e_bits,
	uint8_t **Row_s, uint8_t **Row_z, uint64_t **Row_m, uint64_t **Row_e, uint8_t **Mask, 
	uint8_t *pooled_s, uint8_t *pooled_z, uint64_t *pooled_m, uint64_t *pooled_e
	) {

	vector<FPArray> maxs ;
	for (int i = 0 ; i < chunk ; i++) {
		maxs.push_back(
			fpopArr[tid]->input(
				tid&1?3-__party:__party, filterSize, Row_s[i], Row_z[i], Row_m[i], Row_e[i], m_bits, e_bits
			)
		) ;
	}

	vector<BoolArray> filterMask ;
	FPArray filterMax ; 
	tie(filterMax, filterMask) = fpopArr[tid]->max_with_mask(maxs) ;

	for (int i = 0 ; i < chunk ; i++)		
		memcpy(Mask[i], filterMask[i].data, filterSize*sizeof(uint8_t)) ;
	
	memcpy(pooled_s, filterMax.s, chunk*sizeof(uint8_t)) ;
	memcpy(pooled_z, filterMax.z, chunk*sizeof(uint8_t)) ;
	memcpy(pooled_m, filterMax.m, chunk*sizeof(uint64_t)) ;
	memcpy(pooled_e, filterMax.e, chunk*sizeof(uint64_t)) ;
}

void MaxPool(
	int32_t N, int32_t imgH, int32_t imgW, int32_t C, 
	int32_t ksizeH, int32_t ksizeW, 
	int32_t strideH, int32_t strideW,
	int32_t H, int32_t W,
	vector<vector<vector<vector<FPArray>>>>& inArr, 
	vector<vector<vector<vector<BoolArray>>>> &poolmask, 
	vector<vector<vector<vector<FPArray>>>>& outArr) {

	int m_bits = inArr[0][0][0][0].m_bits, e_bits = inArr[0][0][0][0].e_bits ;
	int size = N*H*C*W ;
	int filter_size = ksizeH*ksizeW; 

	uint8_t **Mask = new uint8_t*[size] ;

	uint8_t **Row_s = new uint8_t*[size] ;
	uint8_t **Row_z = new uint8_t*[size] ;
	uint64_t **Row_m = new uint64_t*[size] ;
	uint64_t **Row_e = new uint64_t*[size] ;

	uint8_t *pooled_s = new uint8_t[size] ;
	uint8_t *pooled_z = new uint8_t[size] ;
	uint64_t *pooled_m = new uint64_t[size] ;
	uint64_t *pooled_e = new uint64_t[size] ;

	for (int i = 0 ; i < size ; i++) {
		Row_s[i] = new uint8_t[filter_size] ;
		Row_z[i] = new uint8_t[filter_size] ;
		Row_m[i] = new uint64_t[filter_size] ;
		Row_e[i] = new uint64_t[filter_size] ;

		Mask[i] = new uint8_t[filter_size] ;
	}


	for (int n = 0, size_k=0 ; n < N ; n++) {
		for (int c = 0 ; c < C ; c++) {
			for (int h = 0 ; h < H ; h++) {
				for (int w = 0 ; w < W ; w++, size_k++) {
					for (int kh = 0, filter_k = 0 ; kh < ksizeH ; kh++) {
						for (int kw = 0 ; kw < ksizeW ; kw++, filter_k++) {

							int img_h = h*strideH + kh ; 
							int img_w = w*strideW + kw ;
							uint8_t s, z ;
							uint64_t m, e ;

							if (img_h < 0 || img_h >= imgH || img_w < 0 || img_w >= imgW) {
								s = 0 ;
								z = 1 ;
								m = 0 ;
								e = 0 ;
							} else {
								s = inArr[n][img_h][img_w][c].s[0] ;
								z = inArr[n][img_h][img_w][c].z[0] ;
								m = inArr[n][img_h][img_w][c].m[0] ;
								e = inArr[n][img_h][img_w][c].e[0] ;
							}

							Row_s[size_k][filter_k] = s ;
							Row_z[size_k][filter_k] = z ;
							Row_m[size_k][filter_k] = m ;
							Row_e[size_k][filter_k] = e ;
						}
					}
				}
			}
		}
	}


	vector<int> chunks = get_chunks(size, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(Maxpool_thread,
				i, chunks[i], filter_size, m_bits, e_bits,
				Row_s+offset, Row_z+offset, Row_m+offset, Row_e+offset, Mask+offset,
				pooled_s+offset, pooled_z+offset, pooled_m+offset, pooled_e+offset
			) ;
			offset += chunks[i] ;
		}
	}	

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;
	
	for (uint32_t n = 0, outarr_k=0; n < N; n++) {
		for (uint32_t c = 0; c < C; c++) {
			for (uint32_t h = 0; h < H; h++) {
				for (uint32_t w = 0; w < W; w++, outarr_k++) {
					outArr[n][h][w][c].s[0] = pooled_s[outarr_k] ;
					outArr[n][h][w][c].z[0] = pooled_z[outarr_k] ;
					outArr[n][h][w][c].m[0] = pooled_m[outarr_k] ;
					outArr[n][h][w][c].e[0] = pooled_e[outarr_k] ;
				}
			}
		}
	}

	// Ignored padding for convenience
	for (int n = 0, size_k=0 ; n < N ; n++) {
		for (int c = 0 ; c < C ; c++) {
			for (int h = 0 ; h < H ; h++) {
				for (int w = 0 ; w < W ; w++, size_k++) {
					for (int kh = 0, filter_k = 0 ; kh < ksizeH ; kh++) {
						for (int kw = 0 ; kw < ksizeW ; kw++, filter_k++) {
							int mask_h = h*ksizeH + kh ; 			// multiply with ksize instead of stride
							int mask_w = w*ksizeW + kw ;			// multiply with ksize instead of stride
							uint8_t s, z ;
							uint64_t m, e ;

							poolmask[n][mask_h][mask_w][c].data[0] = Mask[size_k][filter_k] ;
						}
					}
				}
			}
		}
	}

	for (int i = 0 ; i < size ; i++) {
		delete[] Row_s[i] ; 
		delete[] Row_z[i] ; 
		delete[] Row_m[i] ; 
		delete[] Row_e[i] ; 

		delete[] Mask[i] ;
	}

	delete[] pooled_s ; delete[] Row_s ; 
	delete[] pooled_z ; delete[] Row_z ; 
	delete[] pooled_m ; delete[] Row_m ; 
	delete[] pooled_e ; delete[] Row_e ;
	delete[] Mask ;
}


void ConvAdd(int32_t s1, int32_t s2, int32_t s3, int32_t s4, 
	vector<vector<vector<vector<FPArray>>>>& inArr, 
	vector<FPArray>& biasArr, 
	vector<vector<vector<vector<FPArray>>>>& outArr) {
	int m_bits, e_bits ;
	int sz ;

	m_bits = inArr[0][0][0][0].m_bits ;
	e_bits = inArr[0][0][0][0].e_bits ;
	sz = s1*s2*s3*s4 ;

	vector<FPArray> arr1 = make_vector_float(ALICE, sz) ;
	vector<FPArray> arr2 = make_vector_float(ALICE, sz) ;
	vector<FPArray> out = make_vector_float(ALICE, sz) ;

	for (int i1=0 ; i1 < s1 ; i1++) {
		for (int i2 = 0 ; i2 < s2 ; i2++) {
			for (int i3 = 0 ; i3 < s3 ; i3++) {
				for (int i4 = 0 ; i4 < s4 ; i4++) {
					arr1[i1*s2*s3*s4 + i2*s3*s4 + i3*s4 + i4] = inArr[i1][i2][i3][i4] ;
					arr2[i1*s2*s3*s4 + i2*s3*s4 + i3*s4 + i4] = biasArr[i4] ;
				}
			}
		}
	}

	ElemWiseAdd(sz, arr1, arr2, out) ;

	for (int i1=0 ; i1 < s1 ; i1++) {
		for (int i2 = 0 ; i2 < s2 ; i2++) {
			for (int i3 = 0 ; i3 < s3 ; i3++) {
				for (int i4 = 0 ; i4 < s4 ; i4++) {
					outArr[i1][i2][i3][i4] = out[i1*s2*s3*s4 + i2*s3*s4 + i3*s4 + i4] ;
				}
			}
		}
	}
}

void ConvDerReshapeMatMulOPGroup(int32_t N, int32_t finalH, int32_t finalW, int32_t CO, int32_t g, int32_t G, 
vector<vector<FPArray>>& inputArr, 
vector<vector<vector<vector<FPArray>>>>& outputArr){
	int32_t COG = (CO / G) ;
	int32_t startCO = (g * COG) ;
	for (uint32_t co = 0; co < COG; co++){
	for (uint32_t n = 0; n < N; n++){
	for (uint32_t h = 0; h < finalH; h++){
	for (uint32_t w = 0; w < finalW; w++){
	inputArr[co][((((n * finalH) * finalW) + (h * finalW)) + w)] = outputArr[n][h][w][(co + startCO)] ;
	}
	}
	}
	}
}

void ConvDerReshapeInputGroup(
	int32_t N, int32_t H, int32_t W, int32_t CI, int32_t FH, int32_t FW, 
	int32_t zPadHLeft, int32_t zPadHRight, int32_t zPadWLeft, int32_t zPadWRight, int32_t strideH, int32_t strideW, int32_t g, int32_t G, int32_t RRows, int32_t RCols, 
	vector<vector<vector<vector<FPArray>>>>& inputArr, 
	vector<vector<FPArray>>& outputArr){
	int32_t linIdxFilterMult = 0 ;

	int32_t CIG = (CI / G) ;

	for (uint32_t n = 0; n < N; n++){
	int32_t leftTopCornerH = (0 - zPadHLeft) ;

	int32_t extremeRightBottomCornerH = ((H - 1) + zPadHRight) ;

	while ((((leftTopCornerH + FH) - 1) <= extremeRightBottomCornerH)) {
	int32_t leftTopCornerW = (0 - zPadWLeft) ;

	int32_t extremeRightBottomCornerW = ((W - 1) + zPadWRight) ;

	while ((((leftTopCornerW + FW) - 1) <= extremeRightBottomCornerW)) {
	for (uint32_t fh = 0; fh < FH; fh++){
	for (uint32_t fw = 0; fw < FW; fw++){
	int32_t curPosH = (leftTopCornerH + fh) ;

	int32_t curPosW = (leftTopCornerW + fw) ;

	FPArray val = __public_float_to_arithmetic(0.) ;

	int32_t startCI = (g * CIG) ;

	for (uint32_t ci = 0; ci < CIG; ci++){
	if ((((curPosH < 0) || (curPosH >= H)) || ((curPosW < 0) || (curPosW >= W)))) {
	val = __public_float_to_arithmetic(0.) ;

	} else {
	val = inputArr[n][curPosH][curPosW][(ci + startCI)] ;

	}
	outputArr[linIdxFilterMult][((((fh * FW) * CIG) + (fw * CIG)) + ci)] = val ;

	}
	}
	}
	linIdxFilterMult = (linIdxFilterMult + 1) ;

	leftTopCornerW = (leftTopCornerW + strideW) ;

	}
	leftTopCornerH = (leftTopCornerH + strideH) ;
	}
	}
}

// inputArr needed 	--> [5, 5, 20, 50]
// we have 			--> [50, 20, 5, 5]
// loop 			--> [50, 5, 5, 20]
void ConvDerReshapeFilterGroup(int32_t FH, int32_t FW, int32_t CI, int32_t CO, int32_t g, int32_t G, 
vector<vector<vector<vector<FPArray>>>>& inputArr, 
vector<vector<FPArray>>& outputArr){
	int32_t CIG = (CI / G) ;
	int32_t COG = (CO / G) ;
	int32_t startCO = (g * COG) ;

	for (uint32_t co = 0; co < COG; co++){
	for (uint32_t fh = 0; fh < FH; fh++){
	for (uint32_t fw = 0; fw < FW; fw++){
	for (uint32_t ci = 0; ci < CIG; ci++){
	int32_t linIdx = ((((fh * FW) * CIG) + (fw * CIG)) + ci) ;
	inputArr[fh][fw][ci][(co + startCO)] = outputArr[co][linIdx] ;
	}
	}
	}
	}
}


// filterCols = 25*6 = 150
void ConvDerWrapper(
	int32_t N, int32_t H, int32_t W, int32_t CI, int32_t FH, int32_t FW, int32_t CO, 
	int32_t zPadHLeft, int32_t zPadHRight, int32_t zPadWLeft, int32_t zPadWRight, 
	int32_t strideH, int32_t strideW, int32_t G, 
	vector<vector<vector<vector<FPArray>>>>& inputArr, 
	vector<vector<vector<vector<FPArray>>>>& filterArr, 
	vector<vector<vector<vector<FPArray>>>>& outArr) {

	int32_t CIG = (CI / G) ;
	int32_t reshapedFilterRows = (CO / G) ;
	int32_t reshapedFilterCols = ((FH * FW) * CIG) ;
	int32_t reshapedIPRows = ((FH * FW) * CIG) ;
	int32_t outH = ((((H + (zPadHLeft + zPadHRight)) - FH) / strideH) + 1) ;
	int32_t outW = ((((W + (zPadWLeft + zPadWRight)) - FW) / strideW) + 1) ;
	int32_t reshapedIPCols = ((N * outH) * outW) ;

	for (uint32_t g = 0; g < G; g++) {
		vector<vector<FPArray>> matmulOP = make_vector_float(ALICE, reshapedFilterRows, reshapedIPCols) ;
		ConvDerReshapeMatMulOPGroup(N, outH, outW, CO, g, G, matmulOP, outArr);

		vector<vector<FPArray>> inputReshaped = make_vector_float(ALICE, reshapedIPCols, reshapedIPRows) ;
		ConvDerReshapeInputGroup(N, H, W, CI, FH, FW, zPadHLeft, zPadHRight, zPadWLeft, zPadWRight, strideH, strideW, g, G, reshapedIPRows, reshapedIPCols, inputArr, inputReshaped);
		
		vector<vector<FPArray>> filterReshaped = make_vector_float(ALICE, reshapedFilterRows, reshapedFilterCols) ;

		MatMul(reshapedFilterRows, reshapedIPCols, reshapedFilterCols, matmulOP, inputReshaped, filterReshaped);
		ConvDerReshapeFilterGroup(FH, FW, CI, CO, g, G, filterArr, filterReshaped);
	}
}

void ConvBiasDer(
	int m, int W, int H, int chan, vector<vector<vector<vector<FPArray>>>> &der, vector<FPArray> &biasDer) {
	int m_bits, e_bits ;
	m_bits = der[0][0][0][0].m_bits ;
	e_bits = der[0][0][0][0].e_bits ;

	uint8_t **Row_s = new uint8_t*[chan] ;
	uint8_t **Row_z = new uint8_t*[chan] ;
	uint64_t **Row_m = new uint64_t*[chan] ;
	uint64_t **Row_e = new uint64_t*[chan] ;

	uint8_t *der_s = new uint8_t[chan] ;
	uint8_t *der_z = new uint8_t[chan] ;
	uint64_t *der_m = new uint64_t[chan] ;
	uint64_t *der_e = new uint64_t[chan] ;

	int sz = W*H*m ;
	for (int i = 0 ; i < chan ; i++) {
		Row_s[i] = new uint8_t[sz] ;
		Row_z[i] = new uint8_t[sz] ;
		Row_m[i] = new uint64_t[sz] ;
		Row_e[i] = new uint64_t[sz] ;
	}

	for (int i1 = 0 ; i1 < chan ; i1++) {
		for (int i2 = 0, k = 0 ; i2 < W ; i2++) {
			for (int i3 = 0 ; i3 < H ; i3++) {
				for (int i4 = 0 ; i4 < m ; i4++, k++) {
					Row_s[i1][k] = der[i4][i2][i3][i1].s[0] ;
					Row_z[i1][k] = der[i4][i2][i3][i1].z[0] ;
					Row_m[i1][k] = der[i4][i2][i3][i1].m[0] ;
					Row_e[i1][k] = der[i4][i2][i3][i1].e[0] ;
				}
			}
		}
	}

	// cout << "\tGonna chunk\n" ;

	vector<int> chunks = get_chunks(chan, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			threads[i] = thread(vectorSum_thread, 
				i, chunks[i], sz, m_bits, e_bits,
				Row_s+offset, Row_z+offset, Row_m+offset, Row_e+offset,
				der_s+offset, der_z+offset, der_m+offset, der_e+offset
			) ;
			offset += chunks[i] ;
		}
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	// cout << "\tJoined threads\n" ;

	for (int i = 0 ; i < chan ; i++) {
		biasDer[i].s[0] = der_s[i] ;
		biasDer[i].z[0] = der_z[i] ;
		biasDer[i].m[0] = der_m[i] ;
		biasDer[i].e[0] = der_e[i] ;
	}

	// cout << "\tCopied to vector<FPArray>\n" ;

	for (int i = 0 ; i < chan ; i++) {
		delete[] Row_s[i] ;
		delete[] Row_z[i] ;
		delete[] Row_m[i] ;
		delete[] Row_e[i] ;
	}

	delete[] Row_s ; delete[] der_s ;
	delete[] Row_z ; delete[] der_z ;
	delete[] Row_m ; delete[] der_m ;
	delete[] Row_e ; delete[] der_e ;
}

void batched_matrix_multiplication_thread(int tid, int chunk, int matsize, int m_bits, int e_bits,
	vector<FPMatrix> x_chunk, vector<FPMatrix> y_chunk,
	uint8_t** Row_s, uint8_t** Row_z, uint64_t** Row_m, uint64_t** Row_e
	) {

	vector<FPMatrix> res ;
	res = fpopArr[tid]->matrix_multiplication_baseline(x_chunk, y_chunk) ;
	// res = fpopArr[tid]->matrix_multiplication(x_chunk, y_chunk) ;

	for (int i = 0 ; i < chunk ; i++) {
		memcpy(Row_s[i], res[i].s, matsize*sizeof(uint8_t)) ;
		memcpy(Row_z[i], res[i].z, matsize*sizeof(uint8_t)) ;
		memcpy(Row_m[i], res[i].m, matsize*sizeof(uint64_t)) ;
		memcpy(Row_e[i], res[i].e, matsize*sizeof(uint64_t)) ;
	}
}

vector<FPMatrix> batched_matrix_multiplication(vector<FPMatrix> &x, vector<FPMatrix> &y) {
	int m, n, p, L ;
	int m_bits, e_bits ;

	m = x[0].dim1 ;
	n = x[0].dim2 ;
	p = y[0].dim2 ;
	L = x.size() ;

	m_bits = x[0].m_bits ;
	e_bits = x[0].e_bits ;

	uint8_t** Row_s = new uint8_t*[L] ;
	uint8_t** Row_z = new uint8_t*[L] ;
	uint64_t** Row_m = new uint64_t*[L] ;
	uint64_t** Row_e = new uint64_t*[L] ;

	for (int i = 0 ; i < L ; i++) {
		Row_s[i] = new uint8_t[m*p] ;
		Row_z[i] = new uint8_t[m*p] ;
		Row_m[i] = new uint64_t[m*p] ;
		Row_e[i] = new uint64_t[m*p] ;
	}

	vector<int> chunks = get_chunks(L, __nt) ;
	thread threads[MAX_THREADS] ;
	int offset = 0 ;
	for (int i = 0 ; i < __nt ; i++) {
		if (chunks[i] > 0) {
			vector<FPMatrix> x_chunk = {x.begin()+offset, x.begin()+offset+chunks[i]} ;
			vector<FPMatrix> y_chunk = {y.begin()+offset, y.begin()+offset+chunks[i]} ;
			threads[i] = thread(batched_matrix_multiplication_thread,
				i, chunks[i], m*p, m_bits, e_bits,
				x_chunk, y_chunk,
				Row_s+offset, Row_z+offset, Row_m+offset, Row_e+offset
			) ;
		}
		offset += chunks[i] ;
	}

	for (int i = 0 ; i < __nt ; i++)
		if (chunks[i] > 0)
			threads[i].join() ;

	vector<FPMatrix> ret ;
	for (int i = 0 ; i < L ; i++) {
		FPMatrix ret_l = __fp_op->input(__party, m, p, Row_s[i], Row_z[i], Row_m[i], Row_e[i], m_bits, e_bits) ;
		ret.push_back(ret_l) ;

		delete[] Row_s[i] ;	
		delete[] Row_z[i] ;	
		delete[] Row_m[i] ;	
		delete[] Row_e[i] ;	
	}

	delete[] Row_s ;
	delete[] Row_z ;
	delete[] Row_m ;
	delete[] Row_e ;

	return ret ;
}


int num_mul_terms(
	int c, int i, int j,
	int inH, int inW,
	int filterH, int filterW,
	int outC
	) {
	int min_h, max_h, min_w, max_w ;

	min_h = max(i - filterH + 1, 0) ;
	max_h = min(i, inH - filterH) ;
	min_w = max(j - filterW + 1, 0) ;
	max_w = min(j, inW - filterW) ;

	return (max_h - min_h + 1)*(max_w - min_w + 1)*outC ;
}

// der     	--> [BATCH, 8, 8, 50]
// filter 	--> [50, 20, 5, 5]
void get_mul_terms(
	int BATCH,
	int inc, int inh, int inw,
	int inH, int inW,
	int filterH, int filterW,
	int outC,
	vector<vector<vector<vector<FPArray>>>> &der, vector<vector<vector<vector<FPArray>>>> &filter,
	map<int, vector<FPMatrix>> &ld1, map<int, vector<FPMatrix>> &ld2,
	map<tuple<int, int, int>, int> &tup_mp
	) {
	int min_h, max_h, min_w, max_w ;
	int m_bits, e_bits ;
	m_bits = der[0][0][0][0].m_bits ;
	e_bits = der[0][0][0][0].e_bits ;

	min_h = max(inh - filterH + 1, 0) ;
	max_h = min(inh, inH - filterH) ;
	min_w = max(inw - filterW + 1, 0) ;
	max_w = min(inw, inW - filterW) ;

	// int terms = (max_h - min_h + 1)*(max_w - min_w + 1)*outC ;
	int terms = num_mul_terms(inc, inh, inw, inH, inW, filterH, filterW, outC) ;
	
	uint8_t* term1_s = new uint8_t[BATCH*terms] ;
	uint8_t* term1_z = new uint8_t[BATCH*terms] ;
	uint64_t* term1_m = new uint64_t[BATCH*terms] ;
	uint64_t* term1_e = new uint64_t[BATCH*terms] ;

	uint8_t* term2_s = new uint8_t[terms] ;
	uint8_t* term2_z = new uint8_t[terms] ;
	uint64_t* term2_m = new uint64_t[terms] ;
	uint64_t* term2_e = new uint64_t[terms] ;

	for (int oc = 0, k = 0 ; oc < outC ; oc++) {
		for (int h = min_h ; h < max_h+1 ; h++) {
			for (int w = min_w ; w < max_w+1 ; w++, k++) {

				int batch_row = outC * (max_h - min_h + 1) * (max_w - min_w + 1) ;
				for (int m = 0 ; m < BATCH ; m++) {
					term1_s[m*batch_row + k] = der[m][h][w][oc].s[0] ;
					term1_z[m*batch_row + k] = der[m][h][w][oc].z[0] ;
					term1_m[m*batch_row + k] = der[m][h][w][oc].m[0] ;
					term1_e[m*batch_row + k] = der[m][h][w][oc].e[0] ;
				}

				term2_s[k] = filter[inh-h][inw-w][inc][oc].s[0] ;
				term2_z[k] = filter[inh-h][inw-w][inc][oc].z[0] ;
				term2_m[k] = filter[inh-h][inw-w][inc][oc].m[0] ;
				term2_e[k] = filter[inh-h][inw-w][inc][oc].e[0] ;
			}
		}
	}

	FPMatrix term1 = __fp_op->input(__party, BATCH, terms, term1_s, term1_z, term1_m, term1_e, m_bits, e_bits) ;
	FPMatrix term2 = __fp_op->input(__party, terms, 1, term2_s, term2_z, term2_m, term2_e, m_bits, e_bits) ;
	if (ld1.find(terms) != ld1.end()) {
		tup_mp[make_tuple(inc, inh, inw)] = (int)ld1[terms].size() ;
		ld1[terms].push_back(term1) ;
		ld2[terms].push_back(term2) ;
	} else {
		vector<FPMatrix> fp1_vec, fp2_vec ;
		fp1_vec.push_back(term1) ;
		fp2_vec.push_back(term2) ;

		ld1[terms] = fp1_vec ;
		ld2[terms] = fp2_vec ;
		tup_mp[make_tuple(inc, inh, inw)] = 0 ;
	}

	delete[] term1_s ; delete[] term2_s ;
	delete[] term1_z ; delete[] term2_z ;
	delete[] term1_m ; delete[] term2_m ;
	delete[] term1_e ; delete[] term2_e ;
}


void GetPooledDer(
	int batch_size,
	int inH, int inW, int inC,
	int outC, int outH, int outW,
	int filterH, int filterW,
	vector<vector<vector<vector<FPArray>>>> &conv_weights, 
	vector<vector<vector<vector<FPArray>>>> &outDer, 
	vector<vector<vector<vector<FPArray>>>> &inDer) {

	map<int, vector<FPMatrix>> len_dot1 ;
	map<int, vector<FPMatrix>> len_dot2 ;
	map<tuple<int, int, int>, int> ind_mp ;
	map<int, vector<FPMatrix>> len_res ;

	for (int c = 0 ; c < inC ; c++) {
		for (int h = 0 ; h < inH ; h++) {
			for (int w = 0 ; w < inW ; w++) {
				get_mul_terms(batch_size, c, h, w, inH, inW, filterH, filterW, outC, outDer, conv_weights, len_dot1, len_dot2, ind_mp) ;
			}
		}
	}

	for (int i = 1 ; i <= filterH*filterW*outC ; i++)
		if (len_dot1.find(i) != len_dot1.end())
			len_res[i] = batched_matrix_multiplication(len_dot1[i], len_dot2[i]) ;

	for (int c = 0 ; c < inC ; c++) {
		for (int h = 0 ; h < inH ; h++) {
			for (int w = 0 ; w < inW ; w++) {
				int terms = num_mul_terms(c, h, w, inH, inW, filterH, filterW, outC) ;
				int ind = ind_mp[make_tuple(c, h, w)] ;

				int print1, print2 ;
				print1 = ind ;
				print2 = (int)len_dot1[terms].size() ;

				for (int m = 0 ; m < batch_size ; m++) {
					inDer[m][h][w][c].s[0] = len_res[terms][ind].s[m] ;
					inDer[m][h][w][c].z[0] = len_res[terms][ind].z[m] ;
					inDer[m][h][w][c].m[0] = len_res[terms][ind].m[m] ;
					inDer[m][h][w][c].e[0] = len_res[terms][ind].e[m] ;
				}
			}
		}
	}
}

void PoolProp(
	int32_t BATCH, int32_t outc, int32_t img2, int32_t imgp, int32_t img1, 
	int pk, int ps,
	vector<vector<vector<vector<FPArray>>>> &PooledDer, 
	vector<vector<vector<vector<BoolArray>>>> &Pool, 
	vector<vector<vector<vector<FPArray>>>> &ActDer, 
	bool flip) {
	map<pair<int, int>, int> inp_len ;					// cood -> len
	map<int, int> len_nchan ;			    			// len -> number of input indices
	map<pair<int, int>, int> inp_vind ;					// cood -> vector index

	map<int, vector<vector<FPArray>>> len_derarr ;				// len -> derarr
	map<int, vector<vector<BoolArray>>> len_condarr ;			// len -> condarr
	map<int, vector<FPArray>> len_resarr ;						// len -> res

	/*
	1. fill in inp_len map. 
	2. fill in len_derarr, len_condarr map and execute len_resarrq for BATCH, ouc
	  : fill in inp_vind while iterating. Add index if no entry exists in inp_vind
	  : fill in len_nchan only when (b == 0 and c == 1). 
	Iterate over output image
	*/

	/*************** Building inp_len ***************/
	
	// Iterating over output image
	for (int oh = 0 ; oh < img2 ;  oh++) {
		for (int ow = 0 ; ow < img2 ; ow++) {

			// Iterate over pool indices
			for (int ph = 0 ; ph < pk ; ph++) {
				for (int pw = 0 ; pw < pk ; pw++) {
					int ih, iw ;
					ih = oh*ps + ph ;
					iw = ow*ps + pw ;
					pair<int, int> icood = make_pair(ih, iw) ;

					if (inp_len.find(icood) == inp_len.end()) {
						inp_len[icood] = 1 ;
					} else {
						inp_len[icood]++ ;
					}
				}
			}

		}
	}

	/*************** Building inp_vind and len_nchan ***************/

	// Iterating over input image
	for (int ih = 0 ; ih < (img2-1)*ps + pk ; ih++) {
		for (int iw = 0 ; iw < (img2-1)*ps + pk ; iw++) {
			pair<int, int> icood = make_pair(ih, iw) ;
			int len = inp_len[icood] ;

			if (len_nchan.find(len) == len_nchan.end()) {
				inp_vind[icood] = 0 ;
				len_nchan[len] = 1 ;
			} else {
				inp_vind[icood] = len_nchan[len] ;
				len_nchan[len]++ ;
			}
		}
	}

	/*************** Building len_derarr and len_condarr ***************/

	// Create empty len -> arr pairings
	FPArray empty_farr() ;
	BoolArray empty_barr() ;
	vector<FPArray> empty_farray;
	vector<BoolArray> empty_barray ;
	for (auto it = len_nchan.begin() ; it != len_nchan.end() ; it++) {
		int len = it->first ;
		int nchan = it->second ;

		vector<vector<FPArray>> empty_derarr ;
		vector<vector<BoolArray>> empty_condarr ;
		for (int i = 0 ; i < BATCH*outc*nchan ; i++) {
			empty_derarr.push_back(empty_farray) ;
			empty_condarr.push_back(empty_barray) ;
		}

		len_derarr[len] = empty_derarr ;
		len_condarr[len] = empty_condarr ;
	}

	// Iterating over batchsize, channels
	for (int b = 0 ; b < BATCH ; b++) {
		for (int c = 0 ; c < outc ; c++) {

			// Iterate over output image
			for (int oh = 0 ; oh < img2 ;  oh++) {
				for (int ow = 0 ; ow < img2 ; ow++) {

					// Iterate over pool indices
					for (int ph = 0 ; ph < pk ; ph++) {
						for (int pw = 0 ; pw < pk ; pw++) {
							int ih, iw ;
							ih = oh*ps + ph ;
							iw = ow*ps + pw ;
							pair<int, int> icood = make_pair(ih, iw) ;
							int len = inp_len[icood] ;
							int nchan = len_nchan[len] ;
							int vind = inp_vind[icood] ;

							len_derarr[len][b*outc*nchan + c*nchan + vind].push_back(PooledDer[b][oh][ow][c]) ;
							// len_derarr[len][b*outc*nchan + c*nchan + vind].s[] = PooledDer[b][oh][ow][c].s[0] ;
							// len_derarr[len][b*outc*nchan + c*nchan + vind].z[] = PooledDer[b][oh][ow][c].z[0] ;
							// len_derarr[len][b*outc*nchan + c*nchan + vind].m[] = PooledDer[b][oh][ow][c].m[0] ;
							// len_derarr[len][b*outc*nchan + c*nchan + vind].e[] = PooledDer[b][oh][ow][c].e[0] ;


							len_condarr[len][b*outc*nchan + c*nchan + vind].push_back(Pool[b][oh*pk+ph][ow*pk+pw][c]) ;
							// len_condarr[len][b*outc*nchan + c*nchan + vind].data[] = PooledDer[b][oh][ow][c].data[0] ;
						}
					}

				}
			}

		}
	}

	for (auto it = len_nchan.begin() ; it != len_nchan.end() ; it++) {
		int len = it->first ;
		int nchan = it->second ;
		int s1, s2 ;
		s1 = BATCH*outc*nchan ;
		s2 = len ;

		// len_resarr[len] = vector<float>(s1, 0.0) ;
		// len_resarr[len] =  FPArray(ALICE, s1) ;
		len_resarr[len] = vector<FPArray>(s1, FPArray(ALICE, 1)) ;
		vsumIfElse(s1, s2, len_derarr[len], len_condarr[len], len_resarr[len]) ;
	}

	/*************** Computing len_resarr ***************/

	for (int b = 0 ; b < BATCH ; b++) {
		for (int c = 0 ; c < outc ; c++) {

			for (int ih = 0 ; ih < (img2-1)*ps + pk ; ih++) {
				for (int iw = 0 ; iw < (img2-1)*ps + pk ; iw++) {
					pair<int, int> icood = make_pair(ih, iw) ;
					int len = inp_len[icood] ;
					int nchan = len_nchan[len] ;

					ActDer[b][ih][iw][c].s[0] = len_resarr[len][b*outc*nchan + c*nchan + inp_vind[icood]].s[0] ;
					ActDer[b][ih][iw][c].z[0] = len_resarr[len][b*outc*nchan + c*nchan + inp_vind[icood]].z[0] ;
					ActDer[b][ih][iw][c].m[0] = len_resarr[len][b*outc*nchan + c*nchan + inp_vind[icood]].m[0] ;
					ActDer[b][ih][iw][c].e[0] = len_resarr[len][b*outc*nchan + c*nchan + inp_vind[icood]].e[0] ;
				}
			}

		}
	}
}