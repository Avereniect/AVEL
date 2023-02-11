#ifndef AVEL_DENOMINATOR_VECTORS_HPP
#define AVEL_DENOMINATOR_VECTORS_HPP

#include "../denominators/Denominators.hpp"

// Native Denominators

#include "Denominator1x8u.hpp"
#include "Denominator1x8i.hpp"

#include "Denominator1x16u.hpp"
#include "Denominator1x16i.hpp"

#include "Denominator1x32u.hpp"
#include "Denominator1x32i.hpp"

#include "Denominator1x64u.hpp"
#include "Denominator1x64i.hpp"

// 128-bit Denominators

#if defined(AVEL_SSE2) || defined(AVEL_NEON)
    #include "Denominator16x8u.hpp"
    #include "Denominator16x8i.hpp"

    #include "Denominator8x16u.hpp"
    #include "Denominator8x16i.hpp"

    #include "Denominator4x32u.hpp"
    #include "Denominator4x32i.hpp"

    #include "Denominator2x64u.hpp"
    #include "Denominator2x64i.hpp"

#endif

/*
// 256-bit Denominators

#if defined(AVEL_AVX2)
    #include "Denominator8x32u.hpp"
#endif
*/

/*
// 512-bit Denominators

#if defined(AVEL_AVX512F)
    #include "Denominator16x32u.hpp"
#endif
*/

#endif //AVEL_DENOMINATOR_VECTORS_HPP
