cmake_minimum_required(VERSION 3.11)

project(2026-NC-simd)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

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

add_library(MyVector_dot src/MyVector_dot.cpp)
target_include_directories(MyVector_dot PUBLIC include/)

add_library(MyVector_dot_fma src/MyVector_dot_fma.cpp)
target_include_directories(MyVector_dot_fma PUBLIC include/)
target_compile_options(MyVector_dot_fma PRIVATE -mavx2 -mfma)

add_library(MyVector INTERFACE)
target_link_libraries(MyVector INTERFACE MyVector_dot MyVector_dot_fma)
target_include_directories(MyVector INTERFACE include/)

add_executable(main src/main.cpp)
target_link_libraries(main MyVector benchmark::benchmark)
