cmake_minimum_required(VERSION 3.1)

project(AVEL)

# List of AVEL CMake options
set(AVEL_OPTIONS
    AVEL_X86
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
    AVEL_ARM
    AVEL_NEON;
    AVEL_AARCH64;
)

# AVEL

add_library(AVEL INTERFACE)
target_compile_features(AVEL INTERFACE "cxx_std_11")
target_include_directories(AVEL INTERFACE include/)

##Create options
foreach (extension IN LISTS AVEL_OPTIONS)
    option(${extension} OFF)
endforeach()

option(AVEL_USE_PRECOMPILED_HEADERS OFF)

## Define macros for requested features
foreach(extension IN LISTS AVEL_OPTIONS)
    if(${extension})
        list(APPEND AVEL_REQUESTED_EXTENSIONS ${extension})
    endif()
endforeach()

target_compile_definitions(AVEL INTERFACE ${AVEL_REQUESTED_EXTENSIONS})

## Define macros for cache line sizes
if(AVEL_L1_CACHE_LINE_SIZE)
    target_compile_definitions(AVEL INTERFACE AVEL_L1_CACHE_LINE_SIZE=${AVEL_L1_CACHE_LINE_SIZE})
endif()

if(AVEL_L2_CACHE_LINE_SIZE)
    target_compile_definitions(AVEL INTERFACE AVEL_L2_CACHE_LINE_SIZE=${AVEL_L2_CACHE_LINE_SIZE})
endif()

if(AVEL_L3_CACHE_LINE_SIZE)
    target_compile_definitions(AVEL INTERFACE AVEL_L3_CACHE_LINE_SIZE=${AVEL_L3_CACHE_LINE_SIZE})
endif()


## Handle precompiled headers
if(AVEL_USE_PRECOMPILED_HEADERS AND (${CMAKE_VERSION} VERSION_LESS "3.16.0"))
    message(FATAL_ERROR "The use of precompiled headers requires at least CMake 3.16.0")
endif()

if (AVEL_USE_PRECOMPILED_HEADERS)
    target_precompile_headers(AVEL
        INTERFACE
        include/avel/Avel.hpp
    )
endif()


# AVEL tests
option(AVEL_BUILD_TESTS OFF)

if (AVEL_BUILD_TESTS)
    add_subdirectory(tests/)
endif()

# AVEL benchmarks
option(AVEL_BUILD_BENCHMARKS OFF)

if (AVEL_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

#add_subdirectory(support)
