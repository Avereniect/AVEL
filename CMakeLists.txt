cmake_minimum_required(VERSION 3.16)

project(AVEL)

# List of AVEL CMake options
set(AVEL_OPTIONS
    AVEL_X86;
    AVEL_POPCNT;
    AVEL_LZCNT;
    AVEL_PREFETCH;
    AVEL_BMI;
    AVEL_BMI2;
    AVEL_SSE2;
    AVEL_SSE3;
    AVEL_SSSE3;
    AVEL_SSE41;
    AVEL_SSE42;
    AVEL_AVX;
    AVEL_AVX2;
    AVEL_FMA;
    AVEL_AVX512F;
    AVEL_AVX512CD;
    AVEL_AVX512DQ;
    AVEL_AVX512VL;
    AVEL_AVX512BW;
    AVEL_AVX512VPOPCNTDQ;
    AVEL_AVX512VBMI;
    AVEL_AVX512VBMI2;
    AVEL_AVX512BITALG;
    AVEL_GFNI;
    AVEL_AVX_10_1;
    AVEL_AVX_10_2;
    AVEL_ARM;
    AVEL_NEON;
    AVEL_AARCH64;
)

# AVEL

add_library(AVEL INTERFACE)
target_compile_features(AVEL INTERFACE cxx_std_11)
target_include_directories(AVEL INTERFACE include/)



## Create feature options
foreach (extension IN LISTS AVEL_OPTIONS)
    option(${extension} OFF)
endforeach()

option(AVEL_USE_PRECOMPILED_HEADERS OFF "Make AVEL.hpp a precompiled header")

set(AVEL_L1_CACHE_LINE_SIZE "64" CACHE STRING "Size of L1 cache lines in bytes")
set(AVEL_L2_CACHE_LINE_SIZE "64" CACHE STRING "Size of L2 cache lines in bytes")
set(AVEL_L3_CACHE_LINE_SIZE "64" CACHE STRING "Size of L3 cache lines in bytes")



## Define macros for requested features
foreach(extension IN LISTS AVEL_OPTIONS)
    if(${extension})
        list(APPEND AVEL_REQUESTED_EXTENSIONS ${extension})
    endif()
endforeach()


set(AVEL_CACHE_LINE_SIZES "")

## Define macros for cache line sizes
if(NOT AVEL_L1_CACHE_LINE_SIZE STREQUAL "0")
    list(APPEND "AVEL_L1_CACHE_LINE_SIZE=${AVEL_L1_CACHE_LINE_SIZE}")
endif()

if(NOT AVEL_L2_CACHE_LINE_SIZE STREQUAL "0")
    list(APPEND "AVEL_L2_CACHE_LINE_SIZE=${AVEL_L2_CACHE_LINE_SIZE}")
endif()

if(NOT AVEL_L3_CACHE_LINE_SIZE STREQUAL "0")
    list(APPEND "AVEL_L3_CACHE_LINE_SIZE=${AVEL_L3_CACHE_LINE_SIZE}")
endif()

## Set AVEL compile definitions
target_compile_definitions(AVEL INTERFACE ${AVEL_REQUESTED_EXTENSIONS} ${AVEL_CACHE_LINE_SIZES})


## Handle precompiled headers
if (AVEL_USE_PRECOMPILED_HEADERS)
    target_precompile_headers(AVEL
        INTERFACE
        include/avel/Avel.hpp
    )
endif()



# AVEL tests
option(AVEL_BUILD_TESTS OFF)

if (AVEL_BUILD_TESTS)
    add_subdirectory(external/googletest/)
    add_subdirectory(tests/)
endif()



# AVEL benchmarks
option(AVEL_BUILD_BENCHMARKS OFF)

if (AVEL_BUILD_BENCHMARKS)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
    add_subdirectory(external/benchmark/)
    add_subdirectory(benchmarks)
endif()



# AVEL support
#option(AVEL_BUILD_SUPPORT ON)

#if (AVEL_BUILD_SUPPORT)
#    add_subdirectory(support)
#endif()
