"""
MPI-distributed matrix-vector product using my_module.

Distributes an N x M matrix across mpi_size ranks (each rank holds N/mpi_size rows),
then computes the local matrix-vector product on GPU via my_module.apply, and gathers
the result to rank 0.

Run with:
    mpirun -n <mpi_size> python example.py
"""

import os
os.environ["XLA_PYTHON_CLIENT_PREALLOCATE"] = "false"

import numpy as np
import jax
import jax.numpy as jnp
from mpi4py import MPI
import my_module

jax.config.update("jax_enable_x64", True)
comm = MPI.COMM_WORLD


def main():
    rank = comm.Get_rank()
    size = comm.Get_size()

    N = 4096 * size  # total rows — evenly divisible by mpi_size
    M = 4096         # columns

    local_rows = N // size

    # Rank 0 builds the full matrix and vector
    if rank == 0:
        mat = np.random.rand(N, M).astype(np.float64)
        vec = np.random.rand(M).astype(np.float64)
    else:
        mat = None
        vec = np.empty(M, dtype=np.float64)

    # Scatter N/mpi_size rows to each rank; broadcast the shared vector
    local_mat = np.empty((local_rows, M), dtype=np.float64)
    comm.Scatter(mat, local_mat, root=0)
    comm.Bcast(vec, root=0)

    local_mat_jax = jax.device_put(local_mat)
    vec_jax = jax.device_put(vec)

    # Compute local matrix-vector product on GPU; returns a JAX array directly.
    local_result_jax = my_module.apply(local_mat_jax, vec_jax)

    # Copy device result to host for the MPI gather.
    local_result = np.asarray(local_result_jax)

    # Gather all partial results to rank 0
    result = np.empty(N, dtype=np.float64) if rank == 0 else None
    comm.Gather(local_result, result, root=0)

    if rank == 0:
        expected = mat @ vec
        print(result)
        print(expected)
        max_err = np.max(np.abs(result - expected))
        print(f"Matrix shape : {mat.shape}")
        print(f"Vector shape : {vec.shape}")
        print(f"Result shape : {result.shape}")
        print(f"Max error vs numpy: {max_err:.2e}")


if __name__ == "__main__":
    rank = comm.Get_rank()
    my_module.initialize_kokkos(rank)  # Initialize Kokkos before any GPU work
    main()