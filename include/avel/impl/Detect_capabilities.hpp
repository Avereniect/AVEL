#ifndef AVEL_DETECT_CAPABILITIES_GCC_HPP
#define AVEL_DETECT_CAPABILITIES_GCC_HPP

#ifdef __SSE__
    #define AVEL_SSE
#endif

#ifdef __SSE2__
    #define AVEL_SSE2
#endif

#ifdef __SSE3__
    #define AVEL_SSE3
#endif

#ifdef __SSSE3__
    #define AVEL_SSSE3
#endif

#ifdef __SSE4_1__
    #define AVEL_SSE41
#endif

#ifdef __SSE4_1__
    #define AVEL_SSE41
#endif

#ifdef __SSE4_2__
    #define AVEL_SSE41
#endif

#ifdef __AVX__
    #define AVEL_AVX
#endif

#ifdef __AVX2__
    #define AVEL_AVX2
#endif

#ifdef __FMA__
    #define AVEL_FMA
#endif

#ifdef __AVX512F__
    #define AVEL_AVX512F
#endif

#ifdef __AVX512CD__
    #define AVEL_AVX512CD
#endif

#ifdef __AVX512VL__
    #define AVEL_AVX512VL
#endif

#ifdef __AVX512DQ__
    #define AVEL_AVX512DQ
#endif

#ifdef __AVX512BW__
    #define AVEL_AVX512BW
#endif

#ifdef __AVX512VPOPCNTDQ__
    #define AVEL_AVX512VPOPCNTDQ
#endif

#ifdef __AVX512VBMI__
    #define AVEL_AVX512VBMI
#endif

#ifdef __AVX512VBMI2__
    #define AVEL_AVX512VBMI2
#endif

#ifdef __AVX512BITALG__
    #define AVEL_AVX512BITALG
#endif

#ifdef __GFNI__
    #define AVEL_GFNI
#endif

#ifdef __AVX10_1__
    #define AVEL_AVX10_1
#endif

#ifdef __AVX10_2__
    #define AVEL_AVX10_2
#endif

#ifdef __POPCNT__
    #define AVEL_POPCNT
#endif

#ifdef __LZCNT__
    #define AVEL_LZCNT
#endif

#ifdef __BMI2__
    #define AVEL_BMI2
#endif

#ifdef __BMI__
    #define AVEL_BMI
#endif

// ARM capabilities

#ifdef __ARM_ARCH_ISA_A64
    #define AVEL_AARCH64
#endif

#ifdef __ARM_NEON
    #define AVEL_NEON
#endif

#endif //AVEL_DETECT_CAPABILITIES_GCC_HPP
