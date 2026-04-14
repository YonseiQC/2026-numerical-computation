# Homework 1

본 숙제는 AVX2 및 OpenMP를 이용해 여러 activation 함수를 구현하고 벤치마크하는 것을 목표로 합니다. 2페이지 내의 보고서 및 코드를 제출해 주시기를 바랍니다.

## Setting
본 숙제를 위해서 GCC version 13 이상 사용을 권장합니다. Windows WSL 환경에서 개발 환경 설정은 [링크](https://github.com/YonseiQC/2025-oop-and-ds/blob/main/hw0.md)를 참고하세요.

또한 OpenMP 설치가 필요합니다.
```bash
$ apt install libopenmp-dev
```

## 프로젝트 구조
먼저 [skeleton.tar](./skeleton.tar) 파일의 압축을 풀면 기본 프로젝트 파일이 나옵니다. 다음 명령어를 통해 기본 프로젝트를 빌드할 수 있습니다.

```bash
$ tar -xvf skeleton.tar
$ cd skeleton && mkdir build
$ cd build && cmake ..
$ make
```

해당 명령어를 실행하면 `main` 파일이 생성됩니다. 다음처럼 파일을 실행하면 결과를 얻을 수 있습니다.

```bash
$ OMP_NUM_THREADS=8 ./main
2026-04-14T09:52:57+09:00
Running ./main
Run on (16 X 5585.27 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1024 KiB (x8)
  L3 Unified 32768 KiB (x1)
Load Average: 6.76, 2.71, 1.23
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
BM_sigmoid_default/256          641 ns          642 ns      1100436
BM_sigmoid_default/1024        2224 ns         2224 ns       314878
BM_sigmoid_default/4096        8486 ns         8486 ns        81933
BM_sigmoid_default/16384      33651 ns        33648 ns        20795
BM_sigmoid_avx2/256             293 ns          293 ns      2398501
BM_sigmoid_avx2/1024            831 ns          832 ns       842507
BM_sigmoid_avx2/4096           3001 ns         3001 ns       234820
BM_sigmoid_avx2/16384         11676 ns        11675 ns        59871
BM_sigmoid_omp/256              757 ns          758 ns       970063
BM_sigmoid_omp/1024            1084 ns         1083 ns       615144
BM_sigmoid_omp/4096            2456 ns         2453 ns       269238
BM_sigmoid_omp/16384           7601 ns         7590 ns       107519
```
위의 명령어에서 `./main`앞에 `OMP_NUM_THREADS=8`는 OpenMP가 최대 8개의 쓰레드를 이용하는 것으로 제한합니다.

함수는 `include/sigmoid.hpp`에 선언되고, `src/sigmoid_default.cpp`, `src/sigmoid_avx2.cpp`, `src/sigmoid_omp.cpp` 파일에 정의된 함수들을 실행합니다. 예를 들어 `src/sigmoid_default.cpp`에 정의된 `sigmoid_default` 함수는 다음과 같습니다.

```cpp
void sigmoid_default(std::span<double> vec) {
	for(size_t i = 0; i < vec.size(); i++) {
		vec[i] = 1.0/(1.0 + std::exp(-vec[i]));
	}
}
```
본 함수는 `vec[i]`의 값을 `sigmoid`함수를 적용한 값으로 치환합니다.

특별히 `sigmoid_avx2.cpp`파일에는 AVX2를 이용한 exponential 함수가 정의되어 있습니다. 이는 AVX2로 실행되는 exponential함수가 GCC 환경에서 기본 제공되지 않기 때문입니다.

## 할일
비슷하게 C++ native, AVX2, OpenMP 버전의 다음 함수를 구현하고 벤치마크를 수행하십시오.
함수 목록: `Rectified linear unit (ReLU)`$=\max\{0,x\}$, `Gaussian Error Linear Unit (GELU)`$=x(1+\mathrm{erf}(x/\sqrt{2}))/2$, `Softplus`$=\ln(1+e^x)$.

추가점수 항목: AVX2 구현에 Loop unrolling을 추가하여 더 빨리지는지 확인해 보세요.

구현을 위해서 Error function과 `log`함수의 AVX2 구현이 필요합니다. 다음 링크를 참고하세요: [Error function 구현](https://www.johndcook.com/blog/cpp_erf/), [SIMD 수학 함수 구현 라이브러리](https://github.com/vectorclass/version2/blob/f4617df57e17efcd754f5bbe0ec87883e0ed9ce6/vectormath_exp.h), [관련 Stack Overflow](https://stackoverflow.com/a/45787548).

## 제출파일
2페이지 내의 보고서 및 프로젝트 파일을 압축하여 제출합니다. 프로젝트 파일 압축파일 이름은 `2026-nc-hw1-{학번}.tar`로 해 주세요.
