#ifndef AVEL_IMPL_VECTORS_HPP
#define AVEL_IMPL_VECTORS_HPP

//128-bit vectors

#ifdef AVEL_SSE2

#include "SSE2/Vec4x32f.hpp"

#endif

//256-bit vectors

#ifdef AVEL_AVX

#include "AVX/Vec8x32f.hpp"

#endif

//512-bit vectors

#ifdef AVEL_AVX512F

#include "AVX512F/Vec16x32f.hpp"

#endif

#endif //AVEL_IMPL_VECTORS_HPP
