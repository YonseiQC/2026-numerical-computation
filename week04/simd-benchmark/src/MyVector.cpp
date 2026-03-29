#include "MyVector.hpp"

double dot(const MyVectorDouble& vec1, const MyVectorDouble& vec2) {
	assert(vec1.size() == vec2.size());

	size_t size = vec1.size();
	double res = 0.0;

	for(size_t i=0; i < size; i++) {
		res += vec1[i] * vec2[i];
	}
	return res;
}

double dot_fma(const MyVectorDouble& vec1, const MyVectorDouble& vec2) {
	assert(vec1.size() == vec2.size());

	const size_t size = vec1.size();
	const size_t real_size = 4*((size+3)/4);

	__m256d res = _mm256_setzero_pd();
	for(size_t i=0; i < real_size; i+=4) {
		__m256d val0 = _mm256_load_pd(vec1.p_ + i);
		__m256d val1 = _mm256_load_pd(vec2.p_ + i);
		res = _mm256_fmadd_pd(val0, val1, res);

	}
	double val[4];
	_mm256_store_pd(static_cast<double*>(val), res);
	return val[0] + val[1] + val[2] + val[3];
}

double dot_fma_unroll(const MyVectorDouble& vec1, const MyVectorDouble& vec2) {
	assert(vec1.size() == vec2.size());

	const size_t size = vec1.size();
	const size_t real_size = 4*((size+3)/4);

	__m256d res0 = _mm256_setzero_pd();
	__m256d res1 = _mm256_setzero_pd();
	size_t i = 0;
	for(; i < real_size / 8; i++) {
		__m256d val0 = _mm256_load_pd(vec1.p_ + 8*i);
		__m256d val1 = _mm256_load_pd(vec2.p_ + 8*i);
		res0 = _mm256_fmadd_pd(val0, val1, res0);

		__m256d val2 = _mm256_load_pd(vec1.p_ + 8*i + 4);
		__m256d val3 = _mm256_load_pd(vec2.p_ + 8*i + 4);
		res1 = _mm256_fmadd_pd(val2, val3, res1);
	}
	if (real_size % 8 != 0) {
		__m256d val0 = _mm256_load_pd(vec1.p_ + 8*i);
		__m256d val1 = _mm256_load_pd(vec2.p_ + 8*i);
		res0 = _mm256_fmadd_pd(val0, val1, res0);
	}
	__m256d res = _mm256_add_pd(res0, res1);
	double val[4];
	_mm256_store_pd(static_cast<double*>(val), res);
	return val[0] + val[1] + val[2] + val[3];
}
