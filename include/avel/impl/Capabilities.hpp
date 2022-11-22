#ifndef AVEL_CAPABILITIES_HPP
#define AVEL_CAPABILITIES_HPP

#include <climits>
#include <limits>
#include <cfenv>

static_assert(CHAR_BIT == 8, "x86 bytes are 8 bits. Compiling for non x86 architecture");
static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 floats required");
static_assert(sizeof(float) == 4, "Size of floats should be 32 bits");
static_assert(sizeof(double) == 8, "Size of doubles should be 64 bits");

#ifdef __clang__
    #define AVEL_CLANG
    //#define AVEL_FINL __attribute__((__always_inline__)) inline
    #define AVEL_FINL inline

    #if defined(AVEL_AUTO_DETECT)
        #include "Detect_capabilities.hpp"
    #endif

#elif defined(__GNUC__)
    #define AVEL_GCC
    //#define AVEL_FINL __attribute__((__always_inline__)) inline
    #define AVEL_FINL inline

    #if defined(AVEL_AUTO_DETECT)
        #include "Detect_capabilities.hpp"
    #endif

#elif defined(_MSC_VER)
    #define AVEL_MSVC
    //#define AVEL_FINL __forceinline
    #define AVEL_FINL inline
    #include "intrin.h"

    #if defined(AVEL_AUTO_DETECT)
        static_assert(false, "Cannot detect capabilites on MSVC.");
    #endif
#elif
    static_assert(false, "Compiler not supported");
#endif

//=========================================================
// x86 macros
//=========================================================

#ifdef AVEL_GFNI
    #define AVEL_AVX512F
#endif

#ifdef AVEL_AVX512BITALG
    #define AVEL_AVX512F
#endif


#ifdef AVEL_AVX512VPOPCNTDQ
    #define AVEL_AVX512F
#endif

#ifdef AVEL_AVX512BW
    #define AVEL_AVX512F
#endif


#ifdef AVEL_AVX512DQ
    #define AVEL_AVX512F
#endif


#ifdef AVEL_AVX512VL
    #define AVEL_AVX512F
#endif


#ifdef AVEL_AVX512CD
    #define AVEL_AVX512F
#endif


#ifdef AVEL_AVX512F
    #define AVEL_AVX2
    #define AVELF_FMA
#endif


#ifdef AVEL_FMA
    #define AVEL_AVX
#endif


#ifdef AVEL_AVX2
    #define AVEL_AVX
    #define AVEL_BMI2
#endif


#ifdef AVEL_AVX
    #define AVEL_SSE42
    #define AVEL_BMI
#endif


#ifdef AVEL_SSE42
    #define AVEL_SSE41
#endif


#ifdef AVEL_SSE41
    #define AVEL_SSSE3
#endif


#ifdef AVEL_SSSE3
    #define AVEL_SSE3
#endif


#ifdef AVEL_SSE3
    #define AVEL_SSE2
#endif


#ifdef AVEL_SSE2
    #define AVEL_SSE
#endif

#ifdef AVEL_SSE
    #define AVEL_PREFETCH
    #define AVEL_X86
#endif


#ifdef AVEL_PREFETCH
    #define AVEL_X86
#endif

#ifdef AVEL_FLUSH
    #define AVEL_X86
#endif

#ifdef AVEL_BMI2
    #define AVEVL_BMI
#endif

#ifdef AVEL_BMI
    #define AVEL_X86
#endif

#ifdef AVEL_POPCNT
    #define AVEL_X86
#endif

#if defined(AVEL_SSE) && !defined(AVEL_SSE2)
static_assert(false, "AVEL does not support SSE on its own. SSE2 required");
#endif

#if defined(AVEL_X86)

#endif

//=========================================================
// ARM macros
//=========================================================

#if defined(AVEL_ARMV9)
    #define AVEL_SVE
#endif

#if defined(AVEL_SVE2)
    #define AVEL_SVE
#endif

#if defined(AVEL_SVE)
    #define AVEL_AARCH64
    #define AVEL_ARM
#endif

#if defined(AVEL_AARCH64)
    #define AVEL_NEON
    #define AVEL_ARM
#endif

#if defined(AVEL_AARCH32)
    #define AVEL_ARM
#endif

#if defined(AVEL_NEON)
    #define AVEL_ARM
#endif

#if defined(AVEL_ARM)

#endif

//=========================================================
// Intrinsic headers
//=========================================================

#if defined(AVEL_GCC)
    #if defined(AVEL_X86)
        #include <x86intrin.h>
        #include <immintrin.h>

        #include <avx512vlintrin.h>
    #endif

    #if defined(AVEL_NEON)
        #include <arm_neon.h>
    #endif

    #if defined(AVEL_SVE)
        #include <arm_sve.h>
    #endif
#elif defined(AVEL_CLANG)
    #if defined(AVEL_X86)
        #include <x86intrin.h>
        #include <immintrin.h>
    #endif

    #if defined(AVEL_NEON)
        #include <arm_neon.h>
    #endif

    #if defined(AVEL_SVE)
        #include <arm_sve.h>
    #endif
#else
    static_assert(false, "Compiler not supported");
#endif

//=========================================================
// Error checking
//=========================================================

#if defined(AVEL_ARM) & defined(AVEL_X86)
    static_assert(false, "Macros for conflicting ISAs specified.");
#endif

#endif //AVEL_CAPABILITIES_HPP
