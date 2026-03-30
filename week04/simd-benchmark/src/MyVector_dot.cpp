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