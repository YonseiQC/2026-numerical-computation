# Project description
```bash
mkdir build && cd build
CXX=nvcc_wrapper cmake ..
make
```

```bash
OMPI_ALLOW_RUN_AS_ROOT=1 OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1 mpiexec -np 2 python3 example.py
```
