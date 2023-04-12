#include "../AVEL_tests.hpp"



#ifndef AVEL_ENABLE_VEC1X8U_TESTS
    #define AVEL_ENABLE_VEC1X8U_TESTS true
#endif

#if AVEL_ENABLE_VEC1X8U_TESTS
    #include "Vec1x8u_tests.hpp"
#endif


#ifndef AVEL_ENABLE_VEC1X8I_TESTS
    #define AVEL_ENABLE_VEC1X8I_TESTS true
#endif

#if AVEL_ENABLE_VEC1X8I_TESTS
    #include "Vec1x8i_tests.hpp"
#endif



#ifndef AVEL_ENABLE_VEC1X16U_TESTS
    #define AVEL_ENABLE_VEC1X16U_TESTS true
#endif

#if AVEL_ENABLE_VEC1X16U_TESTS
    #include "Vec1x16u_tests.hpp"
#endif


#ifndef AVEL_ENABLE_VEC1X16I_TESTS
    #define AVEL_ENABLE_VEC1X16I_TESTS true
#endif

#if AVEL_ENABLE_VEC1X16I_TESTS
    #include "Vec1x16i_tests.hpp"
#endif



#ifndef AVEL_ENABLE_VEC1X32U_TESTS
    #define AVEL_ENABLE_VEC1X32U_TESTS true
#endif

#if AVEL_ENABLE_VEC1X32U_TESTS
    #include "Vec1x32u_tests.hpp"
#endif


#ifndef AVEL_ENABLE_VEC1X32I_TESTS
    #define AVEL_ENABLE_VEC1X32I_TESTS true
#endif

#if AVEL_ENABLE_VEC1X32I_TESTS
    #include "Vec1x32i_tests.hpp"
#endif



#ifndef AVEL_ENABLE_VEC1X64U_TESTS
    #define AVEL_ENABLE_VECX64U_TESTS true
#endif

#if AVEL_ENABLE_VEC1X64U_TESTS
    #include "Vec1x64u_tests.hpp"
#endif


#ifndef AVEL_ENABLE_VEC1X64I_TESTS
    #define AVEL_ENABLE_VEC1X64I_TESTS true
#endif

#if AVEL_ENABLE_VEC1X64I_TESTS
    #include "Vec1x64i_tests.hpp"
#endif



#if defined(AVEL_SSE2) || defined(AVEL_NEON)
    #ifndef AVEL_ENABLE_VEC16X8U_TESTS
        #define AVEL_ENABLE_VEC16X8U_TESTS true
    #endif

    #if AVEL_ENABLE_VEC16X8U_TESTS
        #include "Vec16x8u_tests.hpp"
    #endif


    #ifndef AVEL_ENABLE_VEC16X8I_TESTS
        #define AVEL_ENABLE_VEC16X8I_TESTS true
    #endif

    #if AVEL_ENABLE_VEC16X8I_TESTS
        #include "Vec16x8i_tests.hpp"
    #endif



    #ifndef AVEL_ENABLE_VEC8X16U_TESTS
        #define AVEL_ENABLE_VEC8X16U_TESTS true
    #endif

    #if AVEL_ENABLE_VEC8X16U_TESTS
        #include "Vec8x16u_tests.hpp"
    #endif


    #ifndef AVEL_ENABLE_VEC8X16I_TESTS
        #define AVEL_ENABLE_VEC8X16I_TESTS true
    #endif

    #if AVEL_ENABLE_VEC8X16I_TESTS
        #include "Vec8x16i_tests.hpp"
    #endif



    #ifndef AVEL_ENABLE_VEC4X32U_TESTS
        #define AVEL_ENABLE_VEC4X32U_TESTS true
    #endif

    #if AVEL_ENABLE_VEC4X32U_TESTS
        #include "Vec4x32u_tests.hpp"
    #endif


    #ifndef AVEL_ENABLE_VEC4X32I_TESTS
        #define AVEL_ENABLE_VEC4X32I_TESTS true
    #endif

    #if AVEL_ENABLE_VEC4X32I_TESTS
        #include "Vec4x32i_tests.hpp"
    #endif



    #ifndef AVEL_ENABLE_VEC2X64U_TESTS
        #define AVEL_ENABLE_VEC2X64U_TESTS true
    #endif

    #if AVEL_ENABLE_VEC2X64U_TESTS
        #include "Vec2x64u_tests.hpp"
    #endif


    #ifndef AVEL_ENABLE_VEC2X64I_TESTS
        #define AVEL_ENABLE_VEC2X64I_TESTS true
    #endif

    #if AVEL_ENABLE_VEC2X64I_TESTS
        #include "Vec2x64i_tests.hpp"
    #endif
#endif

#if defined(AVEL_AVX2)
    #ifndef AVEL_ENABLE_VEC32X8U_TESTS
        #define AVEL_ENABLE_VEC32X8U_TESTS true
    #endif

    #if AVEL_ENABLE_VEC32X8U_TESTS
        #include "Vec32x8u_tests.hpp"
    #endif


    #ifndef AVEL_ENABLE_VEC32X8I_TESTS
        #define AVEL_ENABLE_VEC32X8I_TESTS true
    #endif

    #if AVEL_ENABLE_VEC32X8I_TESTS
        #include "Vec32x8i_tests.hpp"
    #endif
#endif

#if defined(AVEL_AVX512F)

#endif

#if defined(AVEL_AVX512BW)
    #ifndef AVEL_ENABLE_VEC64X8U_TESTS
        #define AVEL_ENABLE_VEC64X8U_TESTS true
    #endif

    #if AVEL_ENABLE_VEC64X8U_TESTS
        #include "Vec64x8u_tests.hpp"
    #endif


    #ifndef AVEL_ENABLE_VEC64X8I_TESTS
        #define AVEL_ENABLE_VEC64X8I_TESTS true
    #endif

    #if AVEL_ENABLE_VEC64X8I_TESTS
        #include "Vec64x8i_tests.hpp"
    #endif
#endif
