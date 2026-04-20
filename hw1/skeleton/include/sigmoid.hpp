#include <span>

void sigmoid_default(std::span<double> vec);
void sigmoid_avx2(std::span<double> vec);
void sigmoid_omp(std::span<double> vec);
