#ifndef AVEL_SCALAR32F_HPP
#define AVEL_SCALAR32F_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // General Floating-point Operations
    //=====================================================

    /*
    TEST(Scalar32f_tests, fmod_edge_cases) {
        EXPECT_TRUE(compare_common_bytes(avel::fmod(+0.0f, 1.0f), +0.0f));
        EXPECT_TRUE(compare_common_bytes(avel::fmod(-0.0f, 1.0f), -0.0f));

        EXPECT_TRUE(avel::isnan(avel::fmod(+INFINITY, 1.0f)));
        EXPECT_TRUE(avel::isnan(avel::fmod(-INFINITY, 1.0f)));

        EXPECT_TRUE(avel::isnan(avel::fmod(1.0f, +0.0f)));
        EXPECT_TRUE(avel::isnan(avel::fmod(1.0f, -0.0f)));

        EXPECT_EQ(1.0f, avel::fmod(1.0f, +INFINITY));
        EXPECT_EQ(1.0f, avel::fmod(1.0f, -INFINITY));

        EXPECT_TRUE(avel::isnan(avel::fmod(1.0f, NAN)));
        EXPECT_TRUE(avel::isnan(avel::fmod(NAN, 1.0f)));
        EXPECT_TRUE(avel::isnan(avel::fmod(NAN, NAN)));
    }

    TEST(Scalar32f_tests, fmod_preselected_simple_cases) {
        EXPECT_EQ(0.5f, avel::fmod(1.5f, +1.0f));
        EXPECT_EQ(0.5f, avel::fmod(1.5f, -1.0f));

        EXPECT_EQ(-0.5f, avel::fmod(-1.5f, +1.0f));
        EXPECT_EQ(-0.5f, avel::fmod(-1.5f, -1.0f));

        EXPECT_EQ(0.0f, avel::fmod(4.5f, 1.5f));
        EXPECT_EQ(0.0f, avel::fmod(4.5f, 1.5f));

        EXPECT_EQ(258.0f, avel::fmod(346758.0f, 3465.0f));
    }

    TEST(Scalar32f_tests, fmod_subnormal_inputs) {
        EXPECT_EQ(0.0f, avel::fmod(1.12103877145985365674e-44f, 1.40129846432481707092e-45f));
        EXPECT_EQ(0.0f, avel::fmod(1.12103877145985365674e-44f, 1.40129846432481707092e-45f));
        EXPECT_EQ(1.40129846432481707092e-45f, avel::fmod(1.26116861789233536383e-44f, 2.80259692864963414185e-45f));
        EXPECT_EQ(0.0f, avel::fmod(1.17549435082228750797e-38f, 5.87747175411143753984e-39f));
        EXPECT_EQ(0.0f, avel::fmod(1.17549435082228750797e-38f, 2.93873587705571876992e-39f));
        EXPECT_EQ(7.3468396926392969248e-40, avel::fmod(1.54283633545425235421e-38f, 2.93873587705571876992e-39f));
    }

    TEST(Scalar32f_tests, fmod_high_dynamic_range) {
        EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 1.40129846432481707092e-45f));
        EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 2.80259692864963414185e-45f));
        EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 4.20389539297445121277e-45f));
        EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 7.00649232162408535462e-45f));
        EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 1.26116861789233536383e-44f));
        EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 2.94272677508211584894e-44f));
    }

    TEST(Scalar32f_tests, fmod_preselected_random_inputs) {
        float x = 3.40282346638528859812e+38f;
        float y = 4.20389539297445121277e-45f;

        float expected = std::fmod(x, y);
        float observed = avel::fmod(x, y);

        EXPECT_EQ(expected, observed);
        if (expected != observed) {
            int breakpoint_dummy = 345;
        }
    }

    TEST(Scalar32f_tests, fmod_randomized_inputs) {
        for (std::size_t i = 0; i < iterations; ++i) {
            float x = random_val<float>();
            float y = random_val<float>();

            float expected = std::fmod(x, y);
            float observed = avel::fmod(x, y);

            EXPECT_EQ(expected, observed);
            if (expected != observed) {
                int breakpoint_dummy = 345;
            }
        }
    }
    */

    TEST(Scalar32f_tests, frac_edge_cases) {
        EXPECT_EQ(+0.0f, avel::frac(+0.0f));
        EXPECT_EQ(-0.0f, avel::frac(-0.0f));
        EXPECT_TRUE(std::isnan(avel::frac(+INFINITY)));
        EXPECT_TRUE(std::isnan(avel::frac(-INFINITY)));
        EXPECT_TRUE(std::isnan(avel::frac(NAN)));
    }

    TEST(Scalar32f_tests, frac_random) {
        for (std::uint32_t i = 0; i < iterations; ++i) {
            float x = random_val<float>();

            if (x >= 8388608.0) {
                EXPECT_EQ(0.0f, avel::frac(x));
            } else {
                EXPECT_EQ(x - std::trunc(x), avel::frac(x));
            }
        }
    }

    //=====================================================
    // Floating-Point Manipulation Functions
    //=====================================================

    //=====================================================
    // Nearest Integer Functions
    //=====================================================

    /*
    */

    //=====================================================
    // Power Functions
    //=====================================================

    TEST(Scalar32f_tests, sqrt) {

    }

    //=====================================================
    // Nearest Integer Functions
    //=====================================================

    TEST(Sclar32f_tests, ceil) {

    }

    TEST(Sclar32f_tests, floor) {

    }

    TEST(Scalar32f_tests, trunc) {
        EXPECT_EQ(0.0f, avel::trunc(0.25f));
        EXPECT_EQ(0.0f, avel::trunc(0.50f));
        EXPECT_EQ(0.0f, avel::trunc(0.75f));
        EXPECT_EQ(1.0f, avel::trunc(1.50f));
        EXPECT_EQ(2.0f, avel::trunc(2.50f));
        EXPECT_EQ(3.0f, avel::trunc(3.75f));
        EXPECT_EQ(-3.0f, avel:: trunc(-3.75f));

        EXPECT_EQ(0.0f, avel::trunc(0.0f));
        EXPECT_EQ(-0.0f, avel::trunc(-0.0f));
        EXPECT_EQ(INFINITY, avel::trunc(INFINITY));
        EXPECT_EQ(-INFINITY, avel::trunc(-INFINITY));
        EXPECT_TRUE(std::isnan(avel:: trunc(NAN)));
    }

    TEST(Sclar32f_tests, round) {

    }

    TEST(Sclar32f_tests, nearbyint) {

    }

    //=====================================================
    // Floating-point classification
    //=====================================================

    TEST(Scalar32f_tests, frexp) {
        std::int32_t exp = 0x00;

        //Zeros
        EXPECT_EQ(avel::bit_cast<std::uint32_t>(avel::frexp(+0.0f, &exp)), avel::bit_cast<std::uint32_t>(+0.0f));
        EXPECT_EQ(exp, 0);

        EXPECT_EQ(avel::bit_cast<std::uint32_t>(avel::frexp(-0.0f, &exp)), avel::bit_cast<std::uint32_t>(-0.0f));
        EXPECT_EQ(exp, 0);

        //Infinities
        EXPECT_EQ(avel::frexp(+INFINITY, &exp), +INFINITY);
        EXPECT_EQ(avel::frexp(-INFINITY, &exp), -INFINITY);

        //NaN
        EXPECT_TRUE(std::isnan(avel::frexp(NAN, &exp)));

        //Positive Normal numbers
        EXPECT_EQ(avel::frexp(+1.0f, &exp), +0.5f);
        EXPECT_EQ(exp, 1);

        EXPECT_EQ(avel::frexp(+2.0f, &exp), +0.5f);
        EXPECT_EQ(exp, 2);

        EXPECT_EQ(avel::frexp(+4.0f, &exp), +0.5f);
        EXPECT_EQ(exp, 3);

        EXPECT_EQ(avel::frexp(+8.0f, &exp), +0.5f);
        EXPECT_EQ(exp, 4);

        //negative Normal numbers
        EXPECT_EQ(avel::frexp(-1.0f, &exp), -0.5f);
        EXPECT_EQ(exp, 1);

        EXPECT_EQ(avel::frexp(-2.0f, &exp), -0.5f);
        EXPECT_EQ(exp, 2);

        EXPECT_EQ(avel::frexp(-4.0f, &exp), -0.5f);
        EXPECT_EQ(exp, 3);

        EXPECT_EQ(avel::frexp(-8.0f, &exp), -0.5f);
        EXPECT_EQ(exp, 4);

        //Subnormal numbers
        auto val0 = bit_cast<float>(0x00000001);
        EXPECT_EQ(avel::frexp(val0, &exp), +0.5f);
        EXPECT_EQ(exp, -148);

        auto val1 = bit_cast<float>(0x00000010);
        EXPECT_EQ(avel::frexp(val1, &exp), +0.5f);
        EXPECT_EQ(exp, -144);

        auto val2 = bit_cast<float>(0x00000100);
        EXPECT_EQ(avel::frexp(val2, &exp), +0.5f);
        EXPECT_EQ(exp, -140);

        auto val3 = bit_cast<float>(0x00001000);
        EXPECT_EQ(avel::frexp(val3, &exp), +0.5f);
        EXPECT_EQ(exp, -136);

        auto val4 = bit_cast<float>(0x00010000);
        EXPECT_EQ(avel::frexp(val4, &exp), +0.5f);
        EXPECT_EQ(exp, -132);

        auto val5 = bit_cast<float>(0x00400000);
        EXPECT_EQ(avel::frexp(val5, &exp), +0.5f);
        EXPECT_EQ(exp, -126);
    }

    TEST(Sclar32f_tests, ldexp) {

    }

    TEST(Sclar32f_tests, scalbn) {

    }

    TEST(Sclar32f_tests, ilogb) {

    }

    TEST(Sclar32f_tests, logb) {

    }

    TEST(Sclar32f_tests, copysign) {

    }

    //=====================================================
    // Floating-point
    //=====================================================

    //=====================================================
    // Floating-point comparisons
    //=====================================================

    TEST(Scalar32f_tests, isgreater) {
        EXPECT_FALSE(avel::isgreater(-1.0, -1.0));
        EXPECT_FALSE(avel::isgreater(+0.0, +0.0));
        EXPECT_FALSE(avel::isgreater(+1.0, +1.0));

        EXPECT_TRUE(avel::isgreater(-1.0, -2.0));
        EXPECT_TRUE(avel::isgreater(+0.0, -1.0));
        EXPECT_TRUE(avel::isgreater(+1.0, +0.0));

        EXPECT_FALSE(avel::isgreater(0.0f, NAN));
        EXPECT_FALSE(avel::isgreater(NAN, 0.0f));
        EXPECT_FALSE(avel::isgreater(NAN, NAN));

        EXPECT_FALSE(avel::isgreater(+INFINITY, +INFINITY));
        EXPECT_FALSE(avel::isgreater(-INFINITY, -INFINITY));

        EXPECT_TRUE(avel::isgreater(+0.0f, -INFINITY));
        EXPECT_TRUE(avel::isgreater(+INFINITY, +0.0f));
    }

    TEST(Scalar32f_tests, isgreaterequal) {
        EXPECT_TRUE(avel::isgreaterequal(-1.0, -1.0));
        EXPECT_TRUE(avel::isgreaterequal(+0.0, +0.0));
        EXPECT_TRUE(avel::isgreaterequal(+1.0, +1.0));

        EXPECT_TRUE(avel::isgreaterequal(-1.0, -2.0));
        EXPECT_TRUE(avel::isgreaterequal(+0.0, -1.0));
        EXPECT_TRUE(avel::isgreaterequal(+1.0, +0.0));

        EXPECT_FALSE(avel::isgreaterequal(0.0f, NAN));
        EXPECT_FALSE(avel::isgreaterequal(NAN, 0.0f));
        EXPECT_FALSE(avel::isgreaterequal(NAN, NAN));

        EXPECT_TRUE(avel::isgreaterequal(+INFINITY, +INFINITY));
        EXPECT_TRUE(avel::isgreaterequal(-INFINITY, -INFINITY));

        EXPECT_TRUE(avel::isgreaterequal(+0.0f, -INFINITY));
        EXPECT_TRUE(avel::isgreaterequal(+INFINITY, +0.0f));
    }

    TEST(Scalar32f_tests, isless) {
        EXPECT_FALSE(avel::isless(-1.0, -1.0));
        EXPECT_FALSE(avel::isless(+0.0, +0.0));
        EXPECT_FALSE(avel::isless(+1.0, +1.0));

        EXPECT_FALSE(avel::isless(-1.0, -2.0));
        EXPECT_FALSE(avel::isless(+0.0, -1.0));
        EXPECT_FALSE(avel::isless(+1.0, +0.0));

        EXPECT_FALSE(avel::isless(0.0f, NAN));
        EXPECT_FALSE(avel::isless(NAN, 0.0f));
        EXPECT_FALSE(avel::isless(NAN, NAN));

        EXPECT_FALSE(avel::isless(+INFINITY, +INFINITY));
        EXPECT_FALSE(avel::isless(-INFINITY, -INFINITY));

        EXPECT_FALSE(avel::isless(+0.0f, -INFINITY));
        EXPECT_FALSE(avel::isless(+INFINITY, +0.0f));
    }

    TEST(Scalar32f_tests, islessequal) {
        EXPECT_TRUE(avel::islessequal(-1.0, -1.0));
        EXPECT_TRUE(avel::islessequal(+0.0, +0.0));
        EXPECT_TRUE(avel::islessequal(+1.0, +1.0));

        EXPECT_FALSE(avel::islessequal(-1.0, -2.0));
        EXPECT_FALSE(avel::islessequal(+0.0, -1.0));
        EXPECT_FALSE(avel::islessequal(+1.0, +0.0));

        EXPECT_FALSE(avel::islessequal(0.0f, NAN));
        EXPECT_FALSE(avel::islessequal(NAN, 0.0f));
        EXPECT_FALSE(avel::islessequal(NAN, NAN));

        EXPECT_TRUE(avel::islessequal(+INFINITY, +INFINITY));
        EXPECT_TRUE(avel::islessequal(-INFINITY, -INFINITY));

        EXPECT_FALSE(avel::islessequal(+0.0f, -INFINITY));
        EXPECT_FALSE(avel::islessequal(+INFINITY, +0.0f));
    }

    TEST(Scalar32f_tests, islessgreater) {
        EXPECT_FALSE(avel::islessgreater(-1.0f, -1.0f));
        EXPECT_FALSE(avel::islessgreater(+0.0f, +0.0f));
        EXPECT_FALSE(avel::islessgreater(+1.0f, +1.0f));

        EXPECT_TRUE(avel::islessgreater(-1.0f, +0.0f));
        EXPECT_TRUE(avel::islessgreater(+0.0f, +1.0f));

        EXPECT_TRUE(avel::islessgreater(+0.0f, +1.0f));;
    }

    TEST(Scalar32f_tests, is_unordered) {
        EXPECT_TRUE(avel::isunordered(NAN, NAN));

        EXPECT_FALSE(avel::isunordered(0.0f, 0.0f));
        EXPECT_TRUE(avel::isunordered(NAN, 0.0f));
        EXPECT_TRUE(avel::isunordered(0.0f, NAN));

        EXPECT_FALSE(avel::isunordered(INFINITY, INFINITY));
        EXPECT_TRUE(avel::isunordered(INFINITY, NAN));
        EXPECT_TRUE(avel::isunordered(NAN, INFINITY));

        EXPECT_FALSE(avel::isunordered(1.0f, 0.0f));
        EXPECT_FALSE(avel::isunordered(0.0f, 1.0f));
        EXPECT_FALSE(avel::isunordered(1.0f, 1.0f));
    }

}

#endif //AVEL_SCALAR32F_HPP
