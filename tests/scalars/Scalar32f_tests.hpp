#ifndef AVEL_SCALAR32F_HPP
#define AVEL_SCALAR32F_HPP

namespace avel_tests {

    using namespace avel;

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
