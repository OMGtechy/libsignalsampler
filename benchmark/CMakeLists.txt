cmake_minimum_required(VERSION 3.16)
project(signalsampler-benchmark VERSION 0.0.1 LANGUAGES CXX)

include(FetchContent)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
set(BUILD_GMOCK OFF)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
)

FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.5.2
)

FetchContent_MakeAvailable(benchmark)

add_executable(
    signalsampler-benchmark
    source/signalsampler-benchmark.cpp
    source/sampler-benchmark.cpp
)

target_include_directories(
    signalsampler-benchmark
    PUBLIC
    include
)

target_link_libraries(
    signalsampler-benchmark
    benchmark::benchmark
    signalsampler
)

