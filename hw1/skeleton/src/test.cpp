#include "AlignedAllocator.hpp"
#include "sigmoid.hpp"

#include <format>
#include <iostream>
#include <vector>
#include <random>

int main() {
	using AlignedVector = std::vector<double, AlignedAllocator<double, 32>>;
	AlignedVector vec(16);
	std::mt19937 re{1557};
	std::normal_distribution<double> nrd{0.0, 10.0};

	for(size_t i = 0; i < 16; i++) {
		vec[i] = nrd(re);
	}
	auto arr1 = AlignedVector(vec);
	auto arr2 = AlignedVector(vec);

	sigmoid_default(arr1);
	sigmoid_avx2(arr2);

	for(size_t i = 0; i < 16; i++) {
		std::cout << std::format("{}\t{}\n", arr1[i], arr2[i]);
	}
	return 0;
}
