#include <span>
#include <cmath>

void sigmoid_omp(std::span<double> vec) {
#pragma omp parallel for schedule(static, 8)
	for(size_t i = 0; i < vec.size(); i++) {
		vec[i] = 1.0/(1.0 + std::exp(-vec[i]));
	}
}
