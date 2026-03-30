#pragma once
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <immintrin.h>
#include <random>
#include <functional>
#include <cpuid.h>


struct MyVectorDouble {
	double* p_;
	size_t size_;

	MyVectorDouble(size_t size) : size_{size} {
		size_t real_size = 4 * ((size_ + 3) / 4); // =4*ciel(size/4)
		p_ = static_cast<double*>(
			aligned_alloc(32, real_size * sizeof(double))); // 256 bit aligned
		std::fill(p_, p_ + real_size, 0.0);
	}

	MyVectorDouble(const MyVectorDouble& rhs) : size_{rhs.size_} {
		size_t real_size = 4 * ((size_ + 3) / 4); // =4*ciel(size/4)
		p_ = static_cast<double*>(
			aligned_alloc(32, real_size * sizeof(double))); // 256 bit aligned
		std::copy(rhs.p_, rhs.p_ + real_size, p_);
	}

	MyVectorDouble(MyVectorDouble&& rhs) : p_{rhs.p_}, size_{rhs.size_} {
		rhs.p_ = nullptr;
		rhs.size_ = 0;
	}

	MyVectorDouble& operator=(const MyVectorDouble& rhs) {
		std::free(p_);
		size_ = rhs.size_;
		size_t real_size = 4 * ((size_ + 3) / 4); // =4*ciel(size/4)
		p_ = static_cast<double*>(
			aligned_alloc(32, real_size * sizeof(double))); // 256 bit aligned
		std::copy(rhs.p_, rhs.p_ + real_size, p_);
		return *this;
	}

	MyVectorDouble& operator=(MyVectorDouble&& rhs) {
		std::free(p_);
		p_ = rhs.p_;
		size_ = rhs.size_;

		rhs.p_ = nullptr;
		rhs.size_ = 0;
		return *this;
	}

	inline double& operator[](size_t idx) { return p_[idx]; }

	inline const double& operator[](size_t idx) const { return p_[idx]; }

	size_t size() const { return size_; }

	~MyVectorDouble() { std::free(p_); }
};

template<typename RandomEngine> MyVectorDouble random_vector(RandomEngine& re, size_t size) {
	std::normal_distribution<double> ndist;
	MyVectorDouble res(size);
	for(size_t i = 0; i < size; i++) {
		res[i] = ndist(re);
	}
	return res;
}

double dot(const MyVectorDouble& vec1, const MyVectorDouble& vec2);
double dot_fma(const MyVectorDouble& vec1, const MyVectorDouble& vec2);
double dot_fma_unroll(const MyVectorDouble& vec1, const MyVectorDouble& vec2);

inline static std::function<double(const MyVectorDouble&, const MyVectorDouble&)> dot_function = []() {
	unsigned int eax = 0;
	unsigned int ebx = 0;
	unsigned int ecx = 0;
	unsigned int edx = 0;
	__get_cpuid(1, &eax, &ebx, &ecx, &edx);

	bool fma = (ecx & (1U << 12));

	if(fma) {
		return dot_fma;
	}
	return dot;
}();