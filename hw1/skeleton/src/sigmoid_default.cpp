#include <cmath>
#include <span>

void sigmoid_default(std::span<double> vec) {
	for(size_t i = 0; i < vec.size(); i++) {
		vec[i] = 1.0/(1.0 + std::exp(-vec[i]));
	}
}