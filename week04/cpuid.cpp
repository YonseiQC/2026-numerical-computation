#include <iostream>
#include <array>
#include <cpuid.h>

int main() {
	unsigned int eax = 0;
	unsigned int ebx = 0;
	unsigned int ecx = 0;
	unsigned int edx = 0;

	__get_cpuid(0x00, &eax, &ebx, &ecx, &edx); // supported in GCC and clang

	const auto nids = eax; // This is now the highest parameter CPU supports
	std::string vendor;

	{ // nids == 0 for vendor
		std::array<char, 0x20> tmp = {
			0,
		};
		*reinterpret_cast<int *>(tmp.data()) = ebx;
		*reinterpret_cast<int *>(tmp.data() + 4) = edx;
		*reinterpret_cast<int *>(tmp.data() + 8) = ecx;
		vendor = tmp.data();
	}

	std::cout << vendor << '\n';

	// To determine whether the CPU supports AVX, we call cpuid with eax = 1;
	__get_cpuid(1, &eax, &ebx, &ecx, &edx);
	// then ecx contains the relevant information
	bool fma = (ecx & (1U << 12));
	bool avx = (ecx & (1U << 28));

	std::cout << std::format("FMA: {}, AVX: {}\n", fma, avx);

	// To determine for AVX2, we call cpuid with eax=7, ecx=0
	__get_cpuid_count(7, 0, &eax, &ebx, &ecx, &edx);
	bool avx2 = (ecx & (1U << 5));

	std::cout << std::format("AVX2: {}\n", avx2);

	return 0;
}
