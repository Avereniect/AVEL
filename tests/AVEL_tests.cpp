#include <gtest/gtest.h>

//#define AVEL_AUTO_DETECT
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
//#define AVEL_SSE41
#define AVEL_SSE2
//#define AVEL_CLFLUSH
//#define AVEL_BMI2
//#define AVEL_BMI
//#define AVEL_POPCNT
#define AVEL_X86
//#define AVEL_NEON

//#include <avel/Avel.hpp>
//#include <avel/Cache.hpp>
#include <avel/Scalar.hpp>
#include <avel/Vector.hpp>
#include <avel/Scalar_reciprocal.hpp>
//#include <avel/Bit_set.hpp>
//#include <avel/Vector_view.hpp>
//#include <avel/Random.hpp>



#include "scalars/Scalar8u_tests.hpp"
#include "scalars/Scalar8i_tests.hpp"

#include "scalars/Scalar16u_tests.hpp"
#include "scalars/Scalar16i_tests.hpp"

#include "scalars/Scalar32u_tests.hpp"
#include "scalars/Scalar32i_tests.hpp"

#include "scalars/Scalar64u_tests.hpp"
#include "scalars/Scalar64i_tests.hpp"



#include "scalars/Recip32u_tests.hpp"
//#include "scalars/Recip32i_tests.hpp"

//#include "vectors/Vec4x32u_tests.hpp"
//#include "vectors/Recip4x32u_tests.hpp"

//#include "vectors/Vec2x64u_tests.hpp"

using namespace avel;

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
