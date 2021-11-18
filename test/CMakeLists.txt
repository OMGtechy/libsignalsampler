cmake_minimum_required(VERSION 3.21)
project(signalsampler-test LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.7
)

FetchContent_MakeAvailable(catch)

option(ENABLE_COVERAGE "Enable code coverage flags." OFF)
option(ENABLE_ASAN "Enable address sanitizer flags." OFF)
option(ENABLE_UBSAN "Enable undefined behaviour sanitizer flags." OFF)

add_executable(
    signalsampler-test
    source/signalsampler-test.cpp
    source/sampler-test.cpp
)

target_compile_options(
    signalsampler-test PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

target_include_directories(
    signalsampler-test PRIVATE
    include
    ../include
)

target_link_libraries(
    signalsampler-test
    PRIVATE
    Catch2::Catch2
    unwind
)

if(ENABLE_COVERAGE)
    target_compile_options(
        signalsampler-test
        PRIVATE
        --coverage
    )
    target_link_libraries(
        signalsampler-test
        PRIVATE
        gcov
    )
endif()

if(ENABLE_ASAN OR ENABLE_UBSAN)
    target_compile_options(
        signalsampler-test
        PRIVATE
        -fno-omit-frame-pointer
    )
endif()

if(ENABLE_ASAN)
    target_compile_options(
        signalsampler-test
        PRIVATE
        -fsanitize=address
    )
    target_link_options(
        signalsampler-test
        PUBLIC
        -fsanitize=address
    )
endif()

if(ENABLE_UBSAN)
    target_compile_options(
        signalsampler-test
        PRIVATE
        -fsanitize=undefined
    )
    target_link_options(
        signalsampler-test
        PUBLIC
        -fsanitize=undefined
    )
endif()

add_test(
    NAME signalsampler-test
    COMMAND $<TARGET_FILE:signalsampler-test>
)

