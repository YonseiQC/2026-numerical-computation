#include "MyVector.hpp"
#include "MyMatrix.hpp"
#include <benchmark/benchmark.h>

#include <list>
#include <vector>
#include <random>


static void BM_dot(benchmark::State& state) {
	std::default_random_engine re{1557};
	for (auto _ : state) {
		state.PauseTiming();
		auto vec1 = random_vector(re, state.range(0));
		auto vec2 = random_vector(re, state.range(0));
		state.ResumeTiming();
		double res = dot(vec1, vec2);
		benchmark::DoNotOptimize(res);
	}
}

static void BM_dot_fma(benchmark::State& state) {
	std::default_random_engine re{1557};
	for (auto _ : state) {
		state.PauseTiming();
		auto vec1 = random_vector(re, state.range(0));
		auto vec2 = random_vector(re, state.range(0));
		state.ResumeTiming();
		double res = dot_fma(vec1, vec2);
		benchmark::DoNotOptimize(res);
	}
}

static void BM_dot_fma_unroll(benchmark::State& state) {
	std::default_random_engine re{1557};
	for (auto _ : state) {
		state.PauseTiming();
		auto vec1 = random_vector(re, state.range(0));
		auto vec2 = random_vector(re, state.range(0));
		state.ResumeTiming();
		double res = dot_fma_unroll(vec1, vec2);
		benchmark::DoNotOptimize(res);
	}
}

// Register the function as a benchmark
BENCHMARK(BM_dot_fma_unroll)->Arg(1U << 10)->Arg(1U << 12)->Arg(1U << 14)->Arg(1U << 16);
BENCHMARK(BM_dot_fma)->Arg(1U << 10)->Arg(1U << 12)->Arg(1U << 14)->Arg(1U << 16);
BENCHMARK(BM_dot)->Arg(1U << 10)->Arg(1U << 12)->Arg(1U << 14)->Arg(1U << 16);

template<Order order>
static void BM_matrix_vector_prod(benchmark::State& state) {
	std::default_random_engine re{1557};
	for (auto _ : state) {
		state.PauseTiming();
		auto mat = random_matrix<order>(re, state.range(0), state.range(0));
		auto vec = random_vector(re, state.range(0));
		state.ResumeTiming();
		auto res = matrix_vector_prod(mat, vec);
		benchmark::DoNotOptimize(res);
	}
}

BENCHMARK(BM_matrix_vector_prod<Order::RowMajor>)->Arg(1U << 5);
BENCHMARK(BM_matrix_vector_prod<Order::ColMajor>)->Arg(1U << 5);

BENCHMARK_MAIN();

