cmake_minimum_required(VERSION 3.11)

project(2026-NC-project-skeleton)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

cmake_policy(SET CMP0074 NEW)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(nc-compile-options INTERFACE)
target_compile_options(nc-compile-options INTERFACE -Wall -Wextra -Werror)
find_package(Kokkos 4.2...<5.0 REQUIRED CONFIG)
find_package(MPI REQUIRED)

set(DEV_MODULE Development.Module)
find_package(Python 3.12 COMPONENTS Interpreter ${DEV_MODULE} REQUIRED)

message(STATUS "Use Pythonn at ${Python_EXECUTABLE}")
execute_process(
    COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(my_module "src/my_module.cpp")
target_compile_options(my_module PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-O3>) # Prevent -Os from being added by nanobind
target_link_libraries(my_module PRIVATE nc-compile-options MPI::MPI_CXX Kokkos::kokkos)