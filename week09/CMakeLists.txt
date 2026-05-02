cmake_minimum_required(VERSION 3.11)

project(2026-NC-week09)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

add_library(nc-compile-options INTERFACE)
target_compile_features(nc-compile-options INTERFACE cxx_std_23)
target_compile_options(nc-compile-options INTERFACE -Wall -Wextra -Werror)

find_package(MPI REQUIRED)

add_executable(mpi_test src/mpi_test.cpp)
target_link_libraries(mpi_test nc-compile-options MPI::MPI_CXX)

add_executable(mpi_bcast src/mpi_bcast.cpp)
target_link_libraries(mpi_bcast nc-compile-options MPI::MPI_CXX)

add_executable(mpi_scatter_gather src/mpi_scatter_gather.cpp)
target_link_libraries(mpi_scatter_gather nc-compile-options MPI::MPI_CXX)

add_executable(mpi_producer_consumer src/mpi_producer_consumer.cpp)
target_link_libraries(mpi_producer_consumer nc-compile-options MPI::MPI_CXX)

if(ENABLE_PYTHON)
    set(DEV_MODULE Development.Module)
    find_package(Python 3.8 COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

    message(STATUS "Use Pythonn at ${Python_EXECUTABLE}")
    execute_process(
        COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)
    find_package(nanobind CONFIG REQUIRED)

    nanobind_add_module(nanobind_example "src/nanobind_example.cpp")
    target_link_libraries(nanobind_example PRIVATE nc-compile-options MPI::MPI_CXX)
endif()