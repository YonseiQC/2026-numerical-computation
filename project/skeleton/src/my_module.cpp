#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/ndarray.h>

#include <Kokkos_Core.hpp>

#include <mpi.h>

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>

namespace nb = nanobind;

int add(int a, int b) { return a + b; }

// Singleton class
class MPIManager {
private:
    int mpi_rank_;
    int mpi_size_;

    MPIManager() {
        MPI_Comm_size(MPI_COMM_WORLD, &mpi_size_);
        MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank_);
    }
public:
    static MPIManager& get_instance() {
        static MPIManager instance;
        return instance; 
    }

    int mpi_rank() {
        return mpi_rank_;
    }

    int mpi_size() {
        return mpi_size_;
    }
};

void kokkos_finalizer() {
    std::cerr << "Finalize kokkos when exit\n";
    Kokkos::finalize();
}

// The module name must be same as that defined in `CMakeLists.txt`
NB_MODULE(my_module, m) {
    m.def("add", &add);
    m.def("get_mpi_size", []() {
        return MPIManager::get_instance().mpi_size();
    });
    m.def("get_mpi_rank", []() {
        return MPIManager::get_instance().mpi_rank();
    });
    m.def("initialize_kokkos", [](int device_id) {
        Kokkos::InitializationSettings settings;
        settings.set_device_id(device_id);
        Kokkos::initialize(settings);
        std::atexit(kokkos_finalizer);
    });
    m.def("apply", [](nb::ndarray<double, nb::shape<-1, -1>, nb::device::cuda, nb::c_contig> mat, nb::ndarray<double, nb::shape<-1>, nb::device::cuda, nb::c_contig> vec) {
        if(mat.shape(1) != vec.shape(0)) {
            throw std::runtime_error("Matrix and vector dimensions do not match");
        }
        int rows = mat.shape(0);
        int cols = mat.shape(1);

        typedef Kokkos::View<double**,
                     Kokkos::LayoutRight,
                     Kokkos::CudaSpace,
                     Kokkos::MemoryTraits<Kokkos::Unmanaged>> UnmanagedCudaView2D;
        typedef Kokkos::View<double*,
                     Kokkos::CudaSpace,
                     Kokkos::MemoryTraits<Kokkos::Unmanaged>> UnmanagedCudaView1D;
        UnmanagedCudaView2D mat_d(mat.data(), rows, cols);
        UnmanagedCudaView1D vec_d(vec.data(), cols);

        double *ptr = (double *)Kokkos::kokkos_malloc<Kokkos::CudaSpace>(rows * sizeof(double));
        // Unmanaged view over ptr — does not free ptr on destruction
        UnmanagedCudaView1D result_d(ptr, rows);
        // Perform matrix-vector multiplication on GPU
        Kokkos::parallel_for("MatVecMul", Kokkos::RangePolicy<>(0, rows), KOKKOS_LAMBDA(const int i) {
            double sum = 0.0;
            for(int j = 0; j < cols; ++j) {
                sum += mat_d(i, j) * vec_d(j);
            }
            result_d(i) = sum;
        });
        Kokkos::fence();

        nb::capsule owner(ptr, [](void *p) noexcept {
            Kokkos::kokkos_free<Kokkos::CudaSpace>(p);
        });

        nb::ndarray<double, nb::jax, nb::shape<-1>, nb::device::cuda, nb::c_contig> result(ptr, {(size_t)rows}, owner);
        return result;
    });
}