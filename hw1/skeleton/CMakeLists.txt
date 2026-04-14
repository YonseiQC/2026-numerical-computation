cmake_minimum_required(VERSION 3.11)

project(2026-NC-hw1)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

add_library(nc-compile-options INTERFACE)
target_compile_features(nc-compile-options INTERFACE cxx_std_23)
target_compile_options(nc-compile-options INTERFACE -Wall -Wextra -Werror)

find_package(OpenMP REQUIRED)
find_package(benchmark QUIET)

if (NOT benchmark_FOUND)
    message(STATUS "GBenchmark is not found. Fetch GBenchmark.")

    include(FetchContent)

    # Fetch GTest; required for GBenchmark
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.16.0 # latest
    )
    FetchContent_GetProperties(googletest)
    set(BUILD_GMOCK OFF CACHE INTERNAL "")
    set(INSTALL_GTEST OFF CACHE INTERNAL "")
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()

    # Fetch GBenchmark and surpress internal tests.
    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG        v1.9.2 # latest
    )
    set(BENCHMARK_ENABLE_TESTING OFF)
    FetchContent_GetProperties(googlebenchmark)
    if(NOT googlebenchmark_POPULATED)
        FetchContent_Populate(googlebenchmark)
        add_subdirectory(${googlebenchmark_SOURCE_DIR} ${googlebenchmark_BINARY_DIR})
    endif()
else()
    message(STATUS "GBenchmark found.")
endif()

add_library(sigmoid_default src/sigmoid_default.cpp)
target_link_libraries(sigmoid_default PRIVATE nc-compile-options)
target_include_directories(sigmoid_default PUBLIC include/)

add_library(sigmoid_avx2 src/sigmoid_avx2.cpp)
target_include_directories(sigmoid_avx2 PUBLIC include/)
target_link_libraries(sigmoid_avx2 PRIVATE nc-compile-options)
target_compile_options(sigmoid_avx2 PRIVATE -mavx2 -mfma)

add_library(sigmoid_omp src/sigmoid_omp.cpp)
target_link_libraries(sigmoid_omp PRIVATE nc-compile-options OpenMP::OpenMP_CXX)
target_include_directories(sigmoid_omp PRIVATE include/)

add_library(sigmoid INTERFACE)
target_link_libraries(sigmoid INTERFACE sigmoid_default sigmoid_omp sigmoid_avx2)

add_executable(main src/main.cpp)
target_link_libraries(main nc-compile-options sigmoid benchmark::benchmark)

add_executable(test src/test.cpp)
target_link_libraries(test nc-compile-options sigmoid)
