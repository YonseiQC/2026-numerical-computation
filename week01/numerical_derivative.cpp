#include <iostream>
#include <cmath>
#include <format>
#include <cstdint>

int main() {
	const double x0 = 1.2;
	const double exact_der = std::cos(1.2);

	std::cout << "#h\tabs err\n";

	for (uint32_t n = 1; n < 16; n++) {
		const double h = std::pow(0.1, n);

		const double num_der = (std::sin(x0+h) - std::sin(x0))/h;

		std::cout << std::format("{:.4e}\t{:.4e}\n", h, std::abs(num_der - exact_der));
	}
	return 0;
}
