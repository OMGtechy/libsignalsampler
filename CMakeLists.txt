cmake_minimum_required(VERSION 3.21)
project(signalsampler VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_library(signalsampler INTERFACE)
target_include_directories(signalsampler INTERFACE include)
target_link_libraries(signalsampler INTERFACE unwind)

add_subdirectory(benchmark)
add_subdirectory(test)

