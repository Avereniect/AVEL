#ifndef AVEL_CAPABILITIES_HPP
#define AVEL_CAPABILITIES_HPP


#ifdef __clang__
    #define AVEL_CLANG
    #define AVML_FINL __attribute__((__always_inline__)) inline
    #include <immintrin.h>

#elif defined(__GNUC__)
    #define AVEL_GCC
    #define AVML_FINL __forceinline
    #include <immintrin.h>

#elif defined(_MSC_VER)
    #define AVEL_MSC
    #define AVEL_FINL __forceinline
    #include "intrin.h"
#elif
    static_assert(false, "Compiler not supported");
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


#ifdef AVEL_AVX512_BITALG
    #define AVEL_AVX512F
#endif


#ifdef AVEL_AVX512VPOPCNTDQ
    #define AVEL_AVX512F
#endif


#ifdef AVEL_AVX512F
    #define AVEL_AVX2
#endif


#ifdef AVEL_AVX2
    #define AVEL_AVX
#endif


#ifdef AVEL_AVX
    #define AVEL_SSE42
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
#endif


#ifdef AVEL_PREFETCH
#endif

#ifdef AVEL_FLUSH
#endif

#ifdef AVEL_POPCNT
#endif



#endif //AVEL_CAPABILITIES_HPP
