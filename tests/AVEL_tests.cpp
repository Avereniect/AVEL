#include <gtest/gtest.h>

#include <random>

//#define AVEL_FORCE_INLINE
//#define AVEL_AUTO_DETECT
//#define AVEL_GFNI
//#define AVEL_AVX512VPOPCNTDQ
//#define AVEL_AVX512BITALG
//#define AVEL_AVX512BW
//#define AVEL_AVX512VL
//#define AVEL_AVX512DQ
//#define AVEL_AVX512CD
//#define AVEL_AVX512F
//#define AVEL_FMA
//#define AVEL_AVX2
//#define AVEL_AVX
//#define AVEL_SSE42
//#define AVEL_SSE41
//#define AVEL_SSSE3
//#define AVEL_SSE3
//#define AVEL_SSE2
//#define AVEL_BMI2
//#define AVEL_BMI
//#define AVEL_LZCNT
//#define AVEL_POPCNT
//#define AVEL_X86

//#define AVEL_AARCH64
//#define AVEL_NEON

#include <avel/Avel.hpp>
//#include <avel/Cache.hpp>
#include <avel/Scalar.hpp>
#include <avel/Vector.hpp>
#include <avel/Scalar_denominator.hpp>
#include <avel/Vector_denominator.hpp>
//#include <avel/Normalized_int.hpp>
//#include <avel/Normalized_int_vector.hpp>
//#include <avel/Vector_view.hpp>
//#include <avel/Random.hpp>

#include "Type_traits_tests.hpp"

namespace avel_tests {

    static constexpr std::size_t iterations = 1024;

    static auto random8u  = std::mt19937{0xDEADBEEF};
    static auto random16u = std::mt19937{0xDEADBEEF};
    static auto random32u = std::mt19937{0xDEADBEEF};
    static auto random64u = std::mt19937_64{0xDEADBEEF};

    auto random32f = [] () {
        static std::uniform_int_distribution<std::int32_t> distribution{
            0x0,
            static_cast<int>(2 * 0x7f7fffffu)
        };

        std::int32_t bits = distribution(random32u);

        if (bits > 0x7f7fffff) {
            bits -= 0x7f7fffff;
            bits |= 0x80000000;
        }

        return avel::bit_cast<float>(bits);
    };

    auto random64f = [] () {
        static std::uniform_int_distribution<std::int64_t> distribution{
            0x0,
            static_cast<long>(2 * 0x7fefffffffffffffull)
        };

        std::int64_t bits = distribution(random64u);

        if (bits > 0x7fefffffffffffff) {
            bits -= 0x7fefffffffffffffull;
            bits |= 0x8000000000000000ull;
        }

        return avel::bit_cast<double>(bits);
    };

}

#include "scalars/Scalar8u_tests.hpp"
#include "scalars/Scalar8i_tests.hpp"

#include "scalars/Scalar16u_tests.hpp"
#include "scalars/Scalar16i_tests.hpp"

#include "scalars/Scalar32u_tests.hpp"
#include "scalars/Scalar32i_tests.hpp"

#include "scalars/Scalar64u_tests.hpp"
#include "scalars/Scalar64i_tests.hpp"

//#include "scalars/Scalar32f_tests.hpp"
//#include "scalars/Scalar64f_tests.hpp"



#include "denominators/Denom8u_tests.hpp"
#include "denominators/Denom8i_tests.hpp"

#include "denominators/Denom16u_tests.hpp"
#include "denominators/Denom16i_tests.hpp"

#include "denominators/Denom32u_tests.hpp"
#include "denominators/Denom32i_tests.hpp"

#include "denominators/Denom64u_tests.hpp"
#include "denominators/Denom64i_tests.hpp"


#if defined(AVEL_SSE2) || defined(AVEL_NEON)
    #include "vectors/Vec16x8u_tests.hpp"
    #include "vectors/Vec16x8i_tests.hpp"

    #include "vectors/Vec8x16u_tests.hpp"
    #include "vectors/Vec8x16i_tests.hpp"

    #include "vectors/Vec4x32u_tests.hpp"
    #include "vectors/Vec4x32i_tests.hpp"

    #include "vectors/Vec2x64u_tests.hpp"
    #include "vectors/Vec2x64i_tests.hpp"

    //#include "vectors/Vec4x32f_tests.hpp"
    //#include "vectors/Vec2x64f_tests.hpp"

    #include "denominator_vectors/Denom16x8u_tests.hpp"
    #include "denominator_vectors/Denom16x8i_tests.hpp"

    #include "denominator_vectors/Denom8x16u_tests.hpp"
    #include "denominator_vectors/Denom8x16i_tests.hpp"

    #include "denominator_vectors/Denom4x32u_tests.hpp"
    #include "denominator_vectors/Denom4x32i_tests.hpp"

    //#include "denominator_vectors/Denom2x64u_tests.hpp"
    //#include "denominator_vectors/Denom2x64i_tests.hpp"

#endif

using namespace avel;
using namespace avel_impl;

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
