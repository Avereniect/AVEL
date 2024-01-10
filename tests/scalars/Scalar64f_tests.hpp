#ifndef AVEL_SCALAR64F_TESTS_HPP
#define AVEL_SCALAR64F_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // General Floating-point Operations
    //=====================================================

    TEST(Scalar64f_tests, frac_edge_cases) {
        EXPECT_EQ(+0.0, avel::frac(+0.0f));
        EXPECT_EQ(-0.0, avel::frac(-0.0f));
        EXPECT_TRUE(std::isnan(avel::frac(+INFINITY)));
        EXPECT_TRUE(std::isnan(avel::frac(-INFINITY)));
        EXPECT_TRUE(std::isnan(avel::frac(NAN)));
    }

    TEST(Scalar64f_tests, frac_random) {
        for (std::uint32_t i = 0; i < iterations; ++i) {
            double x = random_val<double>();

            if (x >= 4503599627370496.0) {
                EXPECT_EQ(0.0f, avel::frac(x));
            } else {
                EXPECT_EQ(x - std::trunc(x), avel::frac(x));
            }
        }
    }

}

#endif //AVEL_SCALAR64F_TESTS_HPP
