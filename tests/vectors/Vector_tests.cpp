#include "../AVEL_tests.hpp"

#if defined(AVEL_SSE2) || defined(AVEL_NEON)
    #include "Vec16x8u_tests.hpp"
    #include "Vec16x8i_tests.hpp"

    #include "Vec8x16u_tests.hpp"
    #include "Vec8x16i_tests.hpp"

    #include "Vec4x32u_tests.hpp"
    #include "Vec4x32i_tests.hpp"

    #include "Vec2x64u_tests.hpp"
    #include "Vec2x64i_tests.hpp"
#endif

#if defined(AVEL_AVX2)
    #include "Vec32x8u_tests.hpp"
    #include "Vec32x8i_tests.hpp"
#endif

#if defined(AVEL_AVX512F)

#endif

#if defined(AVEL_AVX512BW)
    #include "Vec64x8u_tests.hpp"
    #include "Vec64x8i_tests.hpp"
#endif
