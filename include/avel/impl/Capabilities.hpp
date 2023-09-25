#ifndef AVEL_CAPABILITIES_HPP
#define AVEL_CAPABILITIES_HPP

#include <climits>
#include <limits>
#include <cfenv>

static_assert(CHAR_BIT == 8, "x86 and ARM bytes are 8 bits. Compiling for non x86 architecture");
static_assert(sizeof(bool) == 1, "Functions that operate on arrays of bools assume bools occupy a single byte");
static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 floats required");
static_assert(sizeof(float) == 4, "Size of floats should be 32 bits");
static_assert(sizeof(double) == 8, "Size of doubles should be 64 bits");

#if defined(__INTEL_LLVM_COMPILER)
    #define AVEL_ICPX

    #if defined(AVEL_FORCE_INLINE)
        #define AVEL_FINL __attribute__((__always_inline__)) inline
    #else
        #define AVEL_FINL inline
    #endif

    #if defined(AVEL_AUTO_DETECT)
        #include "Detect_capabilities.hpp"
    #endif

    #include "Verify_capabilities.hpp"

#elif defined(__clang__)
    #define AVEL_CLANG

    #if defined(AVEL_FORCE_INLINE)
        #define AVEL_FINL __attribute__((__always_inline__)) inline
    #else
        #define AVEL_FINL inline
    #endif

    #if defined(AVEL_AUTO_DETECT)
        #include "Detect_capabilities.hpp"
    #endif

    #include "Verify_capabilities.hpp"

#elif defined(__GNUC__)
    #define AVEL_GCC

    #if defined(AVEL_FORCE_INLINE)
        #define AVEL_FINL __attribute__((__always_inline__)) inline
    #else
        #define AVEL_FINL inline
    #endif

    #if defined(AVEL_AUTO_DETECT)
        #include "Detect_capabilities.hpp"
    #endif

    #include "Verify_capabilities.hpp"

#elif defined(_MSC_VER)
    #define AVEL_MSVC

    #if defined(AVEL_FORCE_INLINE)
        #define AVEL_FINL __forceinline inline
    #else
        #define AVEL_FINL inline
    #endif

    #if defined(AVEL_AUTO_DETECT)
        static_assert(false, "Cannot detect capabilites on MSVC.");
    #endif
#else
    static_assert(
        false,
        "Compiler is not supported. Compilation not likely to function"
    );
#endif

//=========================================================
// x86 macros
//=========================================================

#if defined(AVEL_AVX10_2)
    #define AVEL_AVX10_1
#endif

#if defined(AVEL_AVX10_1)
    #define AVEL_AVX2
#endif

#if defined(AVEL_GFNI)
    #define AVEL_AVX512F
#endif

#if defined(AVEL_AVX512VBMI2)
    #define AVEL_AVX512F
#endif

#if defined(AVEL_AVX512VBMI)
    #define AVEL_AVX512F
#endif

#if defined(AVEL_AVX512BITALG)
    #define AVEL_AVX512F
#endif

#if defined(AVEL_AVX512VPOPCNTDQ)
    #define AVEL_AVX512F
#endif

#if defined(AVEL_AVX512CD)
    #define AVEL_AVX512F
#endif

#if defined(AVEL_AVX512VL)
    #define AVEL_AVX512F
#endif

#if defined(AVEL_AVX512DQ)
    #define AVEL_AVX512F
#endif

#if defined(AVEL_AVX512BW)
    #define AVEL_AVX512F
#endif

#if defined(AVEL_AVX512F)
    #define AVEL_AVX2
    #define AVEL_FMA
#endif

#if defined(AVEL_FMA)
    #define AVEL_AVX
#endif

#if defined(AVEL_AVX2)
    #define AVEL_AVX
#endif

#if defined(AVEL_AVX)
    #define AVEL_SSE42
#endif

#if defined(AVEL_SSE42)
    #define AVEL_SSE41
    #define AVEL_POPCNT
#endif

#if defined(AVEL_SSE41)
    #define AVEL_SSSE3
#endif

#if defined(AVEL_SSSE3)
    #define AVEL_SSE3
#endif

#if defined(AVEL_SSE3)
    #define AVEL_SSE2
#endif

#if defined(AVEL_SSE2)
    #define AVEL_SSE
#endif

#if defined(AVEL_SSE)
    #define AVEL_PREFETCH
    #define AVEL_X86
#endif

#if defined(AVEL_BMI2)
    #define AVEL_X86
#endif

#if defined(AVEL_BMI)
    #define AVEL_X86
#endif

#if defined(AVEL_LZCNT)
    #define AVEL_X86
#endif

#if defined(AVEL_POPCNT)
    #define AVEL_X86
#endif

#if defined(AVEL_PREFETCH)
    #define AVEL_X86
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
// Cache constants
//=========================================================

#ifndef AVEL_L1_CACHE_LINE_SIZE
    #define AVEL_L1_CACHE_LINE_SIZE 64
#endif
#ifndef AVEL_L2_CACHE_LINE_SIZE
    #define AVEL_L2_CACHE_LINE_SIZE 64
#endif
#ifndef AVEL_L3_CACHE_LINE_SIZE
    #define AVEL_L3_CACHE_LINE_SIZE 64
#endif

//=========================================================
// Intrinsic headers
//=========================================================

#if defined(AVEL_ICX)
    #if defined(AVEL_X86)
        #include <x86intrin.h>
        #include <immintrin.h>
    #endif

#elif defined(AVEL_GCC)
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

#elif defined(AVEL_MSVC)
    #if defined(AVEL_X86)
    #include <intrin.h>
    #include <immintrin.h>
    #endif


#else
    static_assert(false, "Compiler not supported");
#endif

//=========================================================
// Error checking
//=========================================================

#if defined(AVEL_SSE) && !defined(AVEL_SSE2)
static_assert(false, "AVEL does not support SSE on its own. SSE2 required");
#endif

#if defined(AVEL_NEON) && !defined(AVEL_AARCH64)
static_assert(false, "AVEL does not support 32-bit ARM at this time");
#endif

#if defined(AVEL_ARM) && defined(AVEL_X86)
static_assert(false, "Macros for conflicting ISAs specified.");
#endif

#if defined(AVEL_ARM) && defined(AVEL_ICPX)
static_assert(false, "Combination of ARM and ICPX is not supported")
#endif

#if defined(AVEL_ARM) && defined(AVEL_MSVC)
static_assert(false, "Combination of ARM and MSVC is not supported")
#endif

#endif //AVEL_CAPABILITIES_HPP
