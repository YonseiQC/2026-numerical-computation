#include <iostream>
#include <format>
#include <cstdint>
#include <array>
#include <vector>

// Kahan compensated summation algorithm.
// Reduces floating-point error from O(n*eps) to O(eps) regardless of n.
template<typename T>
T kahan_sum(const std::vector<T>& values) {
	T sum = 0.0;
	T comp = 0.0;  // running compensation for lost low-order bits
	for (T x : values) {
		T y = x - comp;
		T t = sum + y;
		comp = (t - sum) - y;
		sum = t;
	}
	return sum;
}

int main() {
	const uint32_t N = 10'000'000;

	// Naive summation
	float naive = 0.0f;
	for (uint32_t i = 0; i < N; i++) {
		naive += 1e-6f;
	}

	// Kahan summation
	std::vector<float> values(N, 1e-6f);
	float kahan = kahan_sum(values);

	float expected = N * 1e-6f;

	std::cout << std::format("Expected:  {:.8f}\n", expected);
	std::cout << std::format("Naive sum: {:.8f}  error: {:.2e}\n", naive, naive - expected);
	std::cout << std::format("Kahan sum: {:.8f}  error: {:.2e}\n", kahan, kahan - expected);

	return 0;
}
