#include "MyVector.hpp"
MyVectorDouble add(const MyVectorDouble& vec1, const MyVectorDouble& vec2) {
	assert(vec1.size() == vec2.size());

	size_t size = vec1.size();
	MyVectorDouble res(size);

	for(size_t i = 0; i < size; i++) {
		res[i] = vec1[i] + vec2[i];
	}

	return res;
}

MyVectorDouble add_avx(const MyVectorDouble& vec1, const MyVectorDouble& vec2) {
	assert(vec1.size() == vec2.size());

	size_t size = vec1.size();
	MyVectorDouble res(size);

	for(size_t i = 0; i < 4*((size+3)/4); i+=4) {
		__m256d val1 = _mm256_load_pd(vec1.p_ + i);
		__m256d val2 = _mm256_load_pd(vec2.p_ + i);

		__m256d val3 = _mm256_add_pd(val1, val2);
		_mm256_store_pd(res.p_+i, val3);
	}
	return res;
}
