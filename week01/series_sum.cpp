#include <iostream>
#include <format>
#include <cstdint>

int main() {
	float s = 0.0;
	const uint32_t N = 10'000'000;
	for(uint32_t i = 0; i < N; i++) {
		s += 1e-6;
	}
	std::cout << std::format("{:.8f}\n", s);

	constexpr static uint32_t M = 1000;
	std::array<float, M> partial_sums;
	for(uint32_t i = 0; i < M; i++) {
		partial_sums[i] = 0.0f;
	}
	s = 0.0;
	for(uint32_t i = 0; i < M; i++) {
		for(uint32_t j = 0; j < N/M; j++) {
			partial_sums[i] += 1e-6;
		}
	}

	for(uint32_t i = 0; i < M; i++) {
		s += partial_sums[i];
	}
	std::cout << std::format("{:.8f}\n", s);
	return 0;
}
