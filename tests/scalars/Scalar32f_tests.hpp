#ifndef AVEL_SCALAR32F_HPP
#define AVEL_SCALAR32F_HPP

namespace avel_tests {

    using namespace avel;

    constexpr float lowest = std::numeric_limits<float>::lowest();
    constexpr float max = std::numeric_limits<float>::max();
    constexpr float min = std::numeric_limits<float>::min();

    //=====================================================
    // Midpoint
    //=====================================================

    TEST(Scalar32f, Midpoint_preselected) {
        EXPECT_EQ(avel::midpoint(+0.0f, +0.0f), +0.0f);
        EXPECT_EQ(avel::midpoint(+1.0f, +1.0f), +1.0f);
        EXPECT_EQ(avel::midpoint(-1.0f, -1.0f), -1.0f);
        EXPECT_EQ(avel::midpoint(+1.0f, +3.0f), +2.0f);
    }

    TEST(Scalar32f, Midpoint_preselected_edge_cases) {
        constexpr auto l1 = min;
        constexpr auto l2 = 2.0f * min;
        constexpr auto l3 = 3.0f * min;

        EXPECT_EQ(avel::midpoint(l1, l3), l2);
        EXPECT_EQ(avel::midpoint(l1, l2), l1);
        EXPECT_EQ(avel::midpoint(l2, l1), l2);

        constexpr auto m1 = min;
        constexpr auto m2 = 2.0f * min;
        constexpr auto m3 = 3.0f * min;

        EXPECT_EQ(avel::midpoint(m1, m3), m2);
        EXPECT_EQ(avel::midpoint(m1, m2), m1);
        EXPECT_EQ(avel::midpoint(m2, m1), m2);
    }

    /*
    //=====================================================
    // AVEL Floating-point vector operations
    //=====================================================

    TEST(Scalar32f_tests, Epsilon_increment) {
        EXPECT_EQ(epsilon_increment(bit_cast<float>(0x00000000)), bit_cast<float>(0x00000001));
        EXPECT_EQ(epsilon_increment(bit_cast<float>(0x80000000)), bit_cast<float>(0x00000001));

        EXPECT_EQ(epsilon_increment(bit_cast<float>(0x3f800000)), bit_cast<float>(0x3f800001));
        EXPECT_EQ(epsilon_increment(bit_cast<float>(0xbf800000)), bit_cast<float>(0xbf7fffff));
    }

    TEST(Scalar32f_tests, Epsilon_difference) {
        EXPECT_EQ(epsilon_difference(bit_cast<float>(0x00000001), bit_cast<float>(0x80000001)), 2);
        EXPECT_EQ(epsilon_difference(+0.0f, -0.0f), 0);
        EXPECT_EQ(epsilon_difference(1.0f, 1.0f), 0);
        EXPECT_EQ(epsilon_difference(8388608, 8388609), 1);
        EXPECT_EQ(epsilon_difference(-INFINITY, +INFINITY), 0xFF000000);
    }
    */

    //=====================================================
    // Floating-Point Manipulation Functions
    //=====================================================

    /*
    TEST(Scalar32f_tests, Frexp) {
        std::int32_t exp = 0x00;

        //Zeros
        EXPECT_EQ(bit_cast<std::uint32_t>(frexp(+0.0f, &exp)), bit_cast<std::uint32_t>(+0.0f));
        EXPECT_EQ(exp, 0);

        EXPECT_EQ(bit_cast<std::uint32_t>(frexp(-0.0f, &exp)), bit_cast<std::uint32_t>(-0.0f));
        EXPECT_EQ(exp, 0);

        //Infinities
        EXPECT_EQ(frexp(+INFINITY, &exp), +INFINITY);
        EXPECT_EQ(frexp(-INFINITY, &exp), -INFINITY);

        //NaN
        EXPECT_TRUE(std::isnan(frexp(NAN, &exp)));

        //Positive Normal numbers
        EXPECT_EQ(frexp(+1.0f, &exp), +0.5f);
        EXPECT_EQ(exp, 1);

        EXPECT_EQ(frexp(+2.0f, &exp), +0.5f);
        EXPECT_EQ(exp, 2);

        EXPECT_EQ(frexp(+4.0f, &exp), +0.5f);
        EXPECT_EQ(exp, 3);

        EXPECT_EQ(frexp(+8.0f, &exp), +0.5f);
        EXPECT_EQ(exp, 4);

        //negative Normal numbers
        EXPECT_EQ(frexp(-1.0f, &exp), -0.5f);
        EXPECT_EQ(exp, 1);

        EXPECT_EQ(frexp(-2.0f, &exp), -0.5f);
        EXPECT_EQ(exp, 2);

        EXPECT_EQ(frexp(-4.0f, &exp), -0.5f);
        EXPECT_EQ(exp, 3);

        EXPECT_EQ(frexp(-8.0f, &exp), -0.5f);
        EXPECT_EQ(exp, 4);

        //Subnormal numbers
        auto val0 = bit_cast<float>(0x00000001);
        EXPECT_EQ(frexp(val0, &exp), +0.5f);
        EXPECT_EQ(exp, -148);

        auto val1 = bit_cast<float>(0x00000010);
        EXPECT_EQ(frexp(val1, &exp), +0.5f);
        EXPECT_EQ(exp, -144);

        auto val2 = bit_cast<float>(0x00000100);
        EXPECT_EQ(frexp(val2, &exp), +0.5f);
        EXPECT_EQ(exp, -140);

        auto val3 = bit_cast<float>(0x00001000);
        EXPECT_EQ(frexp(val3, &exp), +0.5f);
        EXPECT_EQ(exp, -136);

        auto val4 = bit_cast<float>(0x00010000);
        EXPECT_EQ(frexp(val4, &exp), +0.5f);
        EXPECT_EQ(exp, -132);

        auto val5 = bit_cast<float>(0x00400000);
        EXPECT_EQ(frexp(val5, &exp), +0.5f);
        EXPECT_EQ(exp, -126);
    }
    */

    //=====================================================
    // Nearest Integer Functions
    //=====================================================

    /*
    TEST(Scalar32f_tests, Trunc) {
        EXPECT_EQ(0.0f, trunc(0.25f));
        EXPECT_EQ(0.0f, trunc(0.50f));
        EXPECT_EQ(0.0f, trunc(0.75f));
        EXPECT_EQ(1.0f, trunc(1.50f));
        EXPECT_EQ(2.0f, trunc(2.50f));
        EXPECT_EQ(3.0f, trunc(3.75f));
        EXPECT_EQ(-3.0f, trunc(-3.75f));

        EXPECT_EQ(0.0f, trunc(0.0f));
        EXPECT_EQ(-0.0f, trunc(-0.0f));
        EXPECT_EQ(INFINITY, trunc(INFINITY));
        EXPECT_EQ(-INFINITY, trunc(-INFINITY));
        EXPECT_TRUE(std::isnan(trunc(NAN)));
    }
    */

    /*
    TEST(Scalar32f_tests, Trunc_random) {
        for (int i = 0; i < iterations; ++i) {
            std::uint32_t a = rand();
            float b = float(a) / 4096.0f;

            float t0 = std::trunc(b);
            float t1 = avel::trunc(b);

            EXPECT_EQ(t0, t1);
        }
    }
    */

    //=====================================================
    // Power Functions
    //=====================================================

    /*
    TEST(Scalar32f_tests, Powi) {
        //EXPECT_EQ(pow(FLT_MAX, 0), 1.0f);
        //EXPECT_EQ(pow(FLT_MAX, 1), FLT_MAX);
        //EXPECT_EQ(pow(FLT_MAX, 2), +INFINITY);

        EXPECT_EQ(pow(+INFINITY, 0), 1.0f);
        EXPECT_EQ(pow(+INFINITY, 1), +INFINITY);
        EXPECT_EQ(pow(+INFINITY, 2), +INFINITY);

        EXPECT_EQ(pow(-INFINITY, 0), 1.0f);
        EXPECT_EQ(pow(-INFINITY, 1), -INFINITY);
        EXPECT_EQ(pow(-INFINITY, 2), +INFINITY);
        EXPECT_EQ(pow(-INFINITY, 3), -INFINITY);
        EXPECT_EQ(pow(-INFINITY, 4), +INFINITY);

        EXPECT_TRUE(std::isnan(pow(NAN, 0)));

        EXPECT_EQ(pow(7.0f, 2), 49.00f);
        EXPECT_EQ(pow(4.0f, 3), 64.00f);
        EXPECT_EQ(pow(1.5f, 2),  2.25f);
        EXPECT_EQ(pow(1.0f, 0),  1.00f);
    }
    */

}

#endif //AVEL_SCALAR32F_HPP
