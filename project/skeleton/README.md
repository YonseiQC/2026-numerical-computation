# MPI-Kokkos-nanobind-example

MPI, Kokkos, nanobind를 이용한 예제 프로그램

## 컴파일 방법
```bash
pip install -r requirements.txt
mkdir build && cd build
CXX=nvcc_wrapper cmake .. && make
```
