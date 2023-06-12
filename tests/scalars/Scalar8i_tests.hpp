#ifndef AVEL_SCALAR8I_TESTS_HPP
#define AVEL_SCALAR8I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // set_bits
    //=====================================================

    TEST(Scalar8i, Set_bits) {
        EXPECT_EQ(std::int8_t{+0}, avel::set_bits<std::int8_t>(false));
        EXPECT_EQ(std::int8_t{-1}, avel::set_bits<std::int8_t>(true));
    }

    //=====================================================
    // isqrt
    //=====================================================

    TEST(Scalar8i, Isqrt_edge_cases) {
        EXPECT_EQ(avel::isqrt(std::int8_t(0)), 0);
        EXPECT_EQ(avel::isqrt(std::int8_t(127)), 11);
    }

    TEST(Scalar8i, Isqrt_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input = random_val<std::int8_t>();

            auto result = avel::isqrt(input);
            auto expected = std::int8_t(std::trunc(std::sqrt(float(input))));

            EXPECT_EQ(result, expected);
        }
    }

    //=====================================================
    // popcount
    //=====================================================

    TEST(Scalar8i, Pop_count_contiguous_low) {
        EXPECT_EQ(avel::popcount(std::uint8_t(0x00u)), 0x00);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x01u)), 0x01);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x03u)), 0x02);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x07u)), 0x03);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x0Fu)), 0x04);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x1Fu)), 0x05);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x3Fu)), 0x06);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x7Fu)), 0x07);
    }

    TEST(Scalar8i, Popcount_edge_cases) {
        EXPECT_EQ(0, avel::popcount(std::uint8_t{0x00}));
        EXPECT_EQ(8, avel::popcount(std::uint8_t{0xFF}));
    }

    TEST(Scalar8i, Popcount_ones) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::uint8_t x = (1 << i);

            EXPECT_EQ(1, avel::popcount(x));
        }
    }

    TEST(Scalar8i, Popcount_preselected) {
        EXPECT_EQ(2, avel::popcount(std::uint8_t{0x03}));
        EXPECT_EQ(6, avel::popcount(std::uint8_t{0xF3}));
        EXPECT_EQ(4, avel::popcount(std::uint8_t{0x3C}));
        EXPECT_EQ(6, avel::popcount(std::uint8_t{0xE7}));
        EXPECT_EQ(4, avel::popcount(std::uint8_t{0x5A}));
        EXPECT_EQ(2, avel::popcount(std::uint8_t{0x21}));
    }

    //=====================================================
    // byteswap
    //=====================================================

    TEST(Scalar8i, Byteswap) {
        EXPECT_EQ(avel::byteswap(std::int8_t(0x00)), 0x00);
        EXPECT_EQ(avel::byteswap(std::int8_t(0x01)), 0x01);
        EXPECT_EQ(avel::byteswap(std::int8_t(0x0a)), 0x0a);
        EXPECT_EQ(avel::byteswap(std::int8_t(0x12)), 0x12);
    }

    //=====================================================
    // countl_zero
    //=====================================================

    TEST(Scalar8i, Countl_zero_edge_cases) {
        EXPECT_EQ(8, avel::countl_zero(std::int8_t{0x00}));
        EXPECT_EQ(0, avel::countl_zero(std::int8_t{-1}));
    }

    TEST(Scalar8i, Countl_zero_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::int8_t x = std::int8_t{1} << i;

            EXPECT_EQ(7 - i, avel::countl_zero(x));
        }
    }

    TEST(Scalar8i, Countl_zero_powers_of_two_minus_one) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;
            std::uint8_t y = x - 1;

            EXPECT_EQ(8 - i, avel::countl_zero(y));
        }
    }

    //=====================================================
    // countl_one
    //=====================================================

    TEST(Scalar8i, Countl_one_edge_cases) {
        EXPECT_EQ(0, avel::countl_one(std::int8_t(0x00)));
        EXPECT_EQ(8, avel::countl_one(std::int8_t(0xFF)));
    }

    TEST(Scalar8i, Countl_one_contiguous_leading_bits) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::uint8_t x = std::int8_t{-1} << i;

            EXPECT_EQ(8 - i, avel::countl_one(x));
        }
    }

    //=====================================================
    // countr_zero
    //=====================================================

    TEST(Scalar8i, Countr_zero_edge_cases) {
        EXPECT_EQ(8, avel::countr_zero(std::int8_t(0x00)));
        EXPECT_EQ(0, avel::countr_zero(std::int8_t(0xFF)));
    }

    TEST(Scalar8i, Countr_zero_powers_of_two) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::int8_t x = std::int8_t{1} << i;

            EXPECT_EQ(i, avel::countr_zero(x));
        }
    }

    TEST(Scalar8i, Countr_zero_contiguous_leading_bits) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::int8_t x = std::int8_t{-1} << i;

            EXPECT_EQ(i, avel::countr_zero(x));
        }
    }

    //=====================================================
    // countr_one
    //=====================================================

    TEST(Scalar8i, Countr_one_edge_cases) {
        EXPECT_EQ(0, avel::countr_one(std::int8_t(0x00)));
        EXPECT_EQ(8, avel::countr_one(std::int8_t(0xFF)));
    }

    TEST(Scalar8i, Countr_one_contiguous_trailing_bits) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::int8_t x = std::uint8_t{0xFF} >> i;

            EXPECT_EQ(8 - i, avel::countr_one(x));
        }
    }

    //=====================================================
    // bit_width
    //=====================================================

    TEST(Scalar8i, Bit_width_edge_cases) {
        EXPECT_EQ(0, avel::bit_width(std::int8_t{0x00}));
        EXPECT_EQ(8, avel::bit_width(std::int8_t{-1}));
    }

    TEST(Scalar8i, Bit_width_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::int8_t x = std::int8_t{1} << i;

            EXPECT_EQ(i + 1, avel::bit_width(x));
        }
    }

    //=====================================================
    // has_single_bit
    //=====================================================

    TEST(Scalar8i, Has_single_bit_edge_cases) {
        EXPECT_FALSE(avel::has_single_bit(std::int8_t(0x00)));
        EXPECT_FALSE(avel::has_single_bit(std::int8_t(0xFF)));
    }

    TEST(Scalar8i, Has_single_bit_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::int8_t x = std::int8_t{1} << i;

            EXPECT_TRUE(avel::has_single_bit(x));
        }
    }

    //=====================================================
    // rotl
    //=====================================================

    TEST(Sclaar8i, rotl_edge_cases) {
        EXPECT_EQ(std::int8_t(0x80), avel::rotl(std::int8_t(0x80), 0x0));
        EXPECT_EQ(std::int8_t(0x01), avel::rotl(std::int8_t(0x80), 0x1));
    }

    TEST(Sclaar8i, rotl_preselected) {
        EXPECT_EQ(std::int8_t(0x55), avel::rotl(std::int8_t(0x55), 0x0));
        EXPECT_EQ(std::int8_t(0x55), avel::rotl(std::int8_t(0x55), 0x4));
        EXPECT_EQ(std::int8_t(0x55), avel::rotl(std::int8_t(0x55), 0x8));

        EXPECT_EQ(std::int8_t(0x0a), avel::rotl(std::int8_t(0x0a), 0x0));
        EXPECT_EQ(std::int8_t(0xa0), avel::rotl(std::int8_t(0x0a), 0x4));
        EXPECT_EQ(std::int8_t(0x0a), avel::rotl(std::int8_t(0x0a), 0x8));

        EXPECT_EQ(std::int8_t(0x43), avel::rotl(std::int8_t(0x43), 0x0));
        EXPECT_EQ(std::int8_t(0x1a), avel::rotl(std::int8_t(0x43), 0x3));
        EXPECT_EQ(std::int8_t(0xd0), avel::rotl(std::int8_t(0x43), 0x6));
    }

    //=====================================================
    // rotr
    //=====================================================

    TEST(Sclaar8i, rotr_edge_cases) {
        EXPECT_EQ(std::int8_t(0x01), avel::rotr(std::int8_t(0x01), 0));
        EXPECT_EQ(std::int8_t(0x80), avel::rotr(std::int8_t(0x01), 1));
    }

    TEST(Sclaar8i, rotr_preselected) {
        EXPECT_EQ(std::int8_t(0x55), avel::rotr(std::int8_t(0x55), 0x0));
        EXPECT_EQ(std::int8_t(0x55), avel::rotr(std::int8_t(0x55), 0x4));
        EXPECT_EQ(std::int8_t(0x55), avel::rotr(std::int8_t(0x55), 0x8));

        EXPECT_EQ(std::int8_t(0x0a), avel::rotr(std::int8_t(0x0a), 0x0));
        EXPECT_EQ(std::int8_t(0xa0), avel::rotr(std::int8_t(0x0a), 0x4));
        EXPECT_EQ(std::int8_t(0x0a), avel::rotr(std::int8_t(0x0a), 0x8));

        EXPECT_EQ(std::int8_t(0x43), avel::rotr(std::int8_t(0x43), 0x0));
        EXPECT_EQ(std::int8_t(0x68), avel::rotr(std::int8_t(0x43), 0x3));
        EXPECT_EQ(std::int8_t(0x0d), avel::rotr(std::int8_t(0x43), 0x6));
    }

    //=====================================================
    // neg_abs
    //=====================================================

    TEST(Scalar8i, Neg_abs_preselected) {
        EXPECT_EQ(std::int8_t{0},    avel::neg_abs(std::int8_t{+0}));
        EXPECT_EQ(std::int8_t{-1},   avel::neg_abs(std::int8_t{+1}));
        EXPECT_EQ(std::int8_t{-127}, avel::neg_abs(std::int8_t{+127}));
        EXPECT_EQ(std::int8_t{-128}, avel::neg_abs(std::int8_t{-128}));
    }

    //=====================================================
    // blend
    //=====================================================

    TEST(Scalar8i, Blend_preselected) {
        EXPECT_EQ(std::int8_t(5), avel::blend(true, std::int8_t(5), std::int8_t(3)));
        EXPECT_EQ(std::int8_t(3), avel::blend(false, std::int8_t(5), std::int8_t(3)));
        EXPECT_EQ(std::int8_t(7), avel::blend(true, std::int8_t(7), std::int8_t(3)));
        EXPECT_EQ(std::int8_t(2), avel::blend(false, std::int8_t(9), std::int8_t(2)));
    }

    //=====================================================
    // min
    //=====================================================

    TEST(Scalar8i, Min_edge_cases) {
        EXPECT_EQ(std::int8_t(0x00), avel::min(std::int8_t(0x00), std::int8_t(0x00)));
        EXPECT_EQ(std::int8_t(-128), avel::min(std::int8_t(-128), std::int8_t(-128)));
        EXPECT_EQ(std::int8_t(+127), avel::min(std::int8_t(+127), std::int8_t(+127)));
    }

    //=====================================================
    // max
    //=====================================================

    TEST(Scalar8i, Max_edge_cases) {
        EXPECT_EQ(std::int8_t(0x00), avel::max(std::int8_t(0x00), std::int8_t(0x00)));
        EXPECT_EQ(std::int8_t(-128), avel::max(std::int8_t(-128), std::int8_t(-128)));
        EXPECT_EQ(std::int8_t(+127), avel::max(std::int8_t(+127), std::int8_t(+127)));
    }

    //=====================================================
    // minmax
    //=====================================================

    TEST(Scalar8i, Minmax_edge_cases) {
        auto t0 = avel::minmax(std::int8_t{0}, std::int8_t{+127});
        EXPECT_EQ(t0[0], std::int8_t(0));
        EXPECT_EQ(t0[1], std::int8_t(127));

        auto t1 = avel::minmax(std::int8_t{+127}, std::int8_t{0});
        EXPECT_EQ(t1[0], std::int8_t(0));
        EXPECT_EQ(t1[1], std::int8_t(+127));

        auto t2 = avel::minmax(std::int8_t{0}, std::int8_t{0});
        EXPECT_EQ(t2[0], std::int8_t(0));
        EXPECT_EQ(t2[1], std::int8_t(0));

        auto t3 = avel::minmax(std::int8_t(+127), std::int8_t(+127));
        EXPECT_EQ(t3[0], std::int8_t(+127));
        EXPECT_EQ(t3[1], std::int8_t(+127));
    }

    //=====================================================
    // clamp
    //=====================================================

    TEST(Scalar8i, Clamp_edge_cases) {
        EXPECT_EQ(std::int8_t(0), avel::clamp(std::int8_t(0), std::int8_t(0), std::int8_t(0)));
        EXPECT_EQ(std::int8_t(127), avel::clamp(std::int8_t(127), std::int8_t(127), std::int8_t(127)));

        EXPECT_EQ(0x00, avel::clamp(std::uint8_t{0xFF}, std::uint8_t{0x00}, std::uint8_t{0x00}));
        EXPECT_EQ(0xFF, avel::clamp(std::uint8_t{0x00}, std::uint8_t{0xFF}, std::uint8_t{0xFF}));
    }

    TEST(Scalar8i, Clamp_preselected) {
        EXPECT_EQ(std::int8_t(10), avel::clamp(std::int8_t(0),  std::int8_t(10), std::int8_t(20)));
        EXPECT_EQ(std::int8_t(10), avel::clamp(std::int8_t(10), std::int8_t(5),  std::int8_t(20)));
        EXPECT_EQ(std::int8_t(10), avel::clamp(std::int8_t(15), std::int8_t(5),  std::int8_t(10)));
    }

    //=====================================================
    // average
    //=====================================================

    TEST(Scalar8i, Average_edge_cases) {
        EXPECT_EQ(std::int8_t(0), avel::average(std::uint8_t(0), std::uint8_t(0)));
        EXPECT_EQ(std::int8_t(127), avel::average(std::uint8_t(127), std::uint8_t(127)));
        EXPECT_EQ(std::int8_t(63), avel::average(std::uint8_t(0), std::uint8_t(127)));
        EXPECT_EQ(std::int8_t(63), avel::average(std::uint8_t(127), std::uint8_t(0)));
    }

    TEST(Scalar8i, Average_preselected) {
        EXPECT_EQ(std::int8_t(0x08), avel::average(std::int8_t(0x00), std::int8_t(0x10)));
        EXPECT_EQ(std::int8_t(0x08), avel::average(std::int8_t(0x10), std::int8_t(0x00)));

        EXPECT_EQ(std::int8_t(0x09), avel::average(std::int8_t(0x01), std::int8_t(0x11)));
        EXPECT_EQ(std::int8_t(0x09), avel::average(std::int8_t(0x11), std::int8_t(0x01)));

        EXPECT_EQ(std::int8_t(0x08), avel::average(std::int8_t(0x00), std::int8_t(0x11)));
        EXPECT_EQ(std::int8_t(0x08), avel::average(std::int8_t(0x11), std::int8_t(0x00)));
    }

    //=====================================================
    // midpoint
    //=====================================================

    TEST(Scalar8i, Midpoint_edge_cases) {
        EXPECT_EQ(0, avel::midpoint(std::int8_t(0), std::int8_t(0)));
        EXPECT_EQ(127, avel::midpoint(std::int8_t(127), std::int8_t(127)));
        EXPECT_EQ(0x3F, avel::midpoint(std::int8_t(0), std::int8_t(127)));
        EXPECT_EQ(0x40, avel::midpoint(std::int8_t(127), std::int8_t(0)));
    }

    TEST(Scalar8i, Midpoint_preselected) {
        EXPECT_EQ(0x08, avel::midpoint(std::int8_t(0x00), std::int8_t(0x10)));
        EXPECT_EQ(0x08, avel::midpoint(std::int8_t(0x10), std::int8_t(0x00)));

        EXPECT_EQ(0x09, avel::midpoint(std::int8_t(0x01), std::int8_t(0x11)));
        EXPECT_EQ(0x09, avel::midpoint(std::int8_t(0x11), std::int8_t(0x01)));

        EXPECT_EQ(0x08, avel::midpoint(std::int8_t(0x00), std::int8_t(0x11)));
        EXPECT_EQ(0x09, avel::midpoint(std::int8_t(0x11), std::int8_t(0x00)));
    }

}

#endif //AVEL_SCALAR8I_TESTS_HPP
