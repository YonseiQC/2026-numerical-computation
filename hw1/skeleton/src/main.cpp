#include "AlignedAllocator.hpp"
#include "sigmoid.hpp"

#include <benchmark/benchmark.h>

#include <list>
#include <random>
#include <vector>

template<typename T>
using AlignedVector = std::vector<T, AlignedAllocator<T, 32>>;

template<typename T, typename RandomEngine>
AlignedVector<T> random_vector(RandomEngine& re, size_t size) {
	std::normal_distribution<double> nrd{0.0, 1.0};

	AlignedVector<T> vec(size);
	for(size_t i = 0; i < size; i++) {
		vec[i] = nrd(re);
	}
	return vec;
}

static void BM_sigmoid_default(benchmark::State& state) {
	std::default_random_engine re{1557};
	for(auto _ : state) {
		state.PauseTiming();
		auto vec = random_vector<double>(re, state.range(0));
		state.ResumeTiming();
		sigmoid_default(vec);
	}
}

static void BM_sigmoid_avx2(benchmark::State& state) {
	std::default_random_engine re{1557};
	for(auto _ : state) {
		state.PauseTiming();
		auto vec = random_vector<double>(re, state.range(0));
		state.ResumeTiming();
		sigmoid_avx2(vec);
	}
}

static void BM_sigmoid_omp(benchmark::State& state) {
	std::default_random_engine re{1557};
	for(auto _ : state) {
		state.PauseTiming();
		auto vec = random_vector<double>(re, state.range(0));
		state.ResumeTiming();
		sigmoid_omp(vec);
	}
}

// Register the function as a benchmark
BENCHMARK(BM_sigmoid_default)->Arg(1U << 8)->Arg(1U << 10)->Arg(1U << 12)->Arg(1U << 14);
BENCHMARK(BM_sigmoid_avx2)->Arg(1U << 8)->Arg(1U << 10)->Arg(1U << 12)->Arg(1U << 14);
BENCHMARK(BM_sigmoid_omp)->Arg(1U << 8)->Arg(1U << 10)->Arg(1U << 12)->Arg(1U << 14);
BENCHMARK_MAIN();
