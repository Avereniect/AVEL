//
// Created by avereniect on 12/26/21.
//

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

#ifdef __AVX512BW__
    #define AVEL_AVX512BW
#endif

#ifdef __AVX512VL__
    #define AVEL_AVX512VL
#endif

#ifdef __AVX512BITALG__
    #define AVEL_AVX512_BITALG
#endif

#ifdef __AVX512VPOPCNTDQ__
    #define AVEL_AVX512VPOPCNTDQ
#endif

#ifdef __POPCNT__
    #define AVEL_POPCNT
#endif

#ifdef __AVX512VPOPCNTDQ__
    #define AVEL_AVX512VPOPCNTDQ
#endif

#endif //AVEL_DETECT_CAPABILITIES_GCC_HPP
