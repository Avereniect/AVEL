#ifndef AVEL_SCALAR8U_TESTS_HPP
#define AVEL_SCALAR8U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // set_bits
    //=====================================================

    TEST(Scalar8u, Set_bits) {
        EXPECT_EQ(std::uint8_t{0x00}, avel::set_bits<std::uint8_t>(false));
        EXPECT_EQ(std::uint8_t{0xFF}, avel::set_bits<std::uint8_t>(true));
    }

    //=====================================================
    // isqrt
    //=====================================================

    TEST(Scalar8u, Isqrt_edge_cases) {
        EXPECT_EQ(avel::isqrt(std::uint8_t(0)), 0);
        EXPECT_EQ(avel::isqrt(std::uint8_t(255)), 15);
    }

    TEST(Scalar8u, Isqrt_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input = random_val<std::uint8_t>();

            auto result = avel::isqrt(input);
            auto expected = std::uint8_t(std::trunc(std::sqrt(float(input))));

            EXPECT_EQ(result, expected);
        }
    }

    //=====================================================
    // popcount
    //=====================================================

    TEST(Scalar8u, Pop_count_contiguous_low) {
        EXPECT_EQ(avel::popcount(std::uint8_t(0x00u)), 0x00);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x01u)), 0x01);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x03u)), 0x02);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x07u)), 0x03);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x0Fu)), 0x04);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x1Fu)), 0x05);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x3Fu)), 0x06);
        EXPECT_EQ(avel::popcount(std::uint8_t(0x7Fu)), 0x07);
    }

    TEST(Scalar8u, Popcount_edge_cases) {
        EXPECT_EQ(0, avel::popcount(std::uint8_t{0x00}));
        EXPECT_EQ(8, avel::popcount(std::uint8_t{0xFF}));
    }

    TEST(Scalar8u, Popcount_ones) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::uint8_t x = (1 << i);

            EXPECT_EQ(1, avel::popcount(x));
        }
    }

    TEST(Scalar8u, Popcount_preselected) {
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

    TEST(Scalar8u, Byteswap) {
        EXPECT_EQ(avel::byteswap(std::uint8_t(0x00)), 0x00);
        EXPECT_EQ(avel::byteswap(std::uint8_t(0x01)), 0x01);
        EXPECT_EQ(avel::byteswap(std::uint8_t(0x0a)), 0x0a);
        EXPECT_EQ(avel::byteswap(std::uint8_t(0x12)), 0x12);
    }

    //=====================================================
    // countl_zero
    //=====================================================

    TEST(Scalar8u, Countl_zero_edge_cases) {
        EXPECT_EQ(8, avel::countl_zero(std::uint8_t{0x00}));
        EXPECT_EQ(0, avel::countl_zero(std::uint8_t{0xFF}));
    }

    TEST(Scalar8u, Countl_zero_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_EQ(7 - i, avel::countl_zero(x));
        }
    }

    TEST(Scalar8u, Countl_zero_powers_of_two_minus_one) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;
            std::uint8_t y = x - 1;

            EXPECT_EQ(8 - i, avel::countl_zero(y));
        }
    }

    //=====================================================
    // countl_one
    //=====================================================

    TEST(Scalar8u, Countl_one_edge_cases) {
        EXPECT_EQ(0, avel::countl_one(std::uint8_t(0x00)));
        EXPECT_EQ(8, avel::countl_one(std::uint8_t(0xFF)));
    }

    TEST(Scalar8u, Countl_one_contiguous_leading_bits) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::uint8_t x = std::uint8_t{0xFF} << i;

            EXPECT_EQ(8 - i, avel::countl_one(x));
        }
    }

    //=====================================================
    // countr_zero
    //=====================================================

    TEST(Scalar8u, Countr_zero_edge_cases) {
        EXPECT_EQ(8, avel::countr_zero(std::uint8_t(0x00)));
        EXPECT_EQ(0, avel::countr_zero(std::uint8_t(0xFF)));
    }

    TEST(Scalar8u, Countr_zero_powers_of_two) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_EQ(i, avel::countr_zero(x));
        }
    }

    TEST(Scalar8u, Countr_zero_contiguous_leading_bits) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{0xFF} << i;

            EXPECT_EQ(i, avel::countr_zero(x));
        }
    }

    //=====================================================
    // countr_one
    //=====================================================

    TEST(Scalar8u, Countr_one_edge_cases) {
        EXPECT_EQ(0, avel::countr_one(std::uint8_t(0x00)));
        EXPECT_EQ(8, avel::countr_one(std::uint8_t(0xFF)));
    }

    TEST(Scalar8u, Countr_one_contiguous_trailing_bits) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::uint8_t x = std::uint8_t{0xFF} >> i;

            EXPECT_EQ(8 - i, avel::countr_one(x));
        }
    }

    //=====================================================
    // bit_width
    //=====================================================

    TEST(Scalar8u, Bit_width_edge_cases) {
        EXPECT_EQ(0, avel::bit_width(std::uint8_t{0x00}));
        EXPECT_EQ(8, avel::bit_width(std::uint8_t{0xFF}));
    }

    TEST(Scalar8u, Bit_width_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_EQ(i + 1, avel::bit_width(x));
        }
    }

    //=====================================================
    // bit_floor
    //=====================================================

    TEST(Scalar8u, Bit_floor_edge_cases) {
        EXPECT_EQ(0x00, avel::bit_floor(std::uint8_t{0x00}));
        EXPECT_EQ(0x01, avel::bit_floor(std::uint8_t{0x01}));
        EXPECT_EQ(0x80, avel::bit_floor(std::uint8_t{0xFF}));
    }

    TEST(Scalar8u, Bit_floor_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_EQ(x, avel::bit_floor(x));
        }
    }

    TEST(Scalar8u, Bit_floor_powers_of_two_minus_one) {
        for (std::int32_t i = 2; i < 8; ++i) {
            std::uint8_t x = (std::uint8_t{1} << i);
            std::uint8_t y = x - 1;

            EXPECT_EQ(std::uint8_t{1} << (i - 1), avel::bit_floor(y));
        }
    }

    TEST(Scalar8u, Bit_floor_powers_of_two_plus_one) {
        for (std::int32_t i = 1; i < 7; ++i) {
            std::uint8_t x = (std::uint8_t{1} << i);
            std::uint8_t y = x + 1;

            EXPECT_EQ(x, avel::bit_floor(y));
        }
    }

    //=====================================================
    // bit_ceil
    //=====================================================

    TEST(Scalar8u, Bit_ceil_edge_cases) {
        EXPECT_EQ(1, avel::bit_ceil(std::uint8_t{0}));
        EXPECT_EQ(1, avel::bit_ceil(std::uint8_t{1}));
    }

    TEST(Scalar8u, Bit_ceil_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_EQ(x, avel::bit_ceil(x));
        }
    }

    TEST(Scalar8u, Bit_ceil_powers_of_two_minus_one) {
        for (std::int32_t i = 2; i < 8; ++i) {
            std::uint8_t x = (std::uint8_t{1} << i);
            std::uint8_t y = x - 1;

            EXPECT_EQ(x, avel::bit_ceil(y));
        }
    }

    TEST(Scalar8u, Bit_ceil_powers_of_two_plus_one) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = (std::uint8_t{1} << i);
            std::uint8_t y = x + 1;

            EXPECT_EQ(x << 1, avel::bit_ceil(y));
        }
    }

    //=====================================================
    // has_single_bit
    //=====================================================

    TEST(Scalar8u, Has_single_bit_edge_cases) {
        EXPECT_FALSE(avel::has_single_bit(std::uint8_t(0x00)));
        EXPECT_FALSE(avel::has_single_bit(std::uint8_t(0xFF)));
    }

    TEST(Scalar8u, Has_single_bit_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_TRUE(avel::has_single_bit(x));
        }
    }

    //=====================================================
    // rotl
    //=====================================================

    TEST(Sclaar8u, rotl_edge_cases) {
        EXPECT_EQ(0x80, avel::rotl(std::uint8_t(0x80), 0x0));
        EXPECT_EQ(0x01, avel::rotl(std::uint8_t(0x80), 0x1));
    }

    TEST(Sclaar8u, rotl_preselected) {
        EXPECT_EQ(0x55, avel::rotl(std::uint8_t(0x55), 0x0));
        EXPECT_EQ(0x55, avel::rotl(std::uint8_t(0x55), 0x4));
        EXPECT_EQ(0x55, avel::rotl(std::uint8_t(0x55), 0x8));

        EXPECT_EQ(0x0a, avel::rotl(std::uint8_t(0x0a), 0x0));
        EXPECT_EQ(0xa0, avel::rotl(std::uint8_t(0x0a), 0x4));
        EXPECT_EQ(0x0a, avel::rotl(std::uint8_t(0x0a), 0x8));

        EXPECT_EQ(0x43, avel::rotl(std::uint8_t(0x43), 0x0));
        EXPECT_EQ(0x1a, avel::rotl(std::uint8_t(0x43), 0x3));
        EXPECT_EQ(0xd0, avel::rotl(std::uint8_t(0x43), 0x6));
    }

    //=====================================================
    // rotr
    //=====================================================

    TEST(Sclaar8u, rotr_edge_cases) {
        EXPECT_EQ(0x01, avel::rotr(std::uint8_t(0x01), 0));
        EXPECT_EQ(0x80, avel::rotr(std::uint8_t(0x01), 1));
    }

    TEST(Sclaar8u, rotr_preselected) {
        EXPECT_EQ(0x55, avel::rotr(std::uint8_t(0x55), 0x0));
        EXPECT_EQ(0x55, avel::rotr(std::uint8_t(0x55), 0x4));
        EXPECT_EQ(0x55, avel::rotr(std::uint8_t(0x55), 0x8));

        EXPECT_EQ(0x0a, avel::rotr(std::uint8_t(0x0a), 0x0));
        EXPECT_EQ(0xa0, avel::rotr(std::uint8_t(0x0a), 0x4));
        EXPECT_EQ(0x0a, avel::rotr(std::uint8_t(0x0a), 0x8));

        EXPECT_EQ(0x43, avel::rotr(std::uint8_t(0x43), 0x0));
        EXPECT_EQ(0x68, avel::rotr(std::uint8_t(0x43), 0x3));
        EXPECT_EQ(0x0d, avel::rotr(std::uint8_t(0x43), 0x6));
    }

    //=====================================================
    // neg_abs
    //=====================================================

    TEST(Scalar8u, Neg_abs_preselected) {
        EXPECT_EQ(std::int8_t{0},    avel::neg_abs(std::uint8_t{+0}));
        EXPECT_EQ(std::int8_t{-1},   avel::neg_abs(std::uint8_t{+1}));
        EXPECT_EQ(std::int8_t{-127}, avel::neg_abs(std::uint8_t{+127}));
        EXPECT_EQ(std::int8_t{-128}, avel::neg_abs(std::uint8_t{+128}));
    }

    //=====================================================
    // blend
    //=====================================================

    TEST(Scalar8u, Blend_preselected) {
        EXPECT_EQ(std::uint8_t(5), avel::blend(true, std::uint8_t(5), std::uint8_t(3)));
        EXPECT_EQ(std::uint8_t(3), avel::blend(false, std::uint8_t(5), std::uint8_t(3)));
        EXPECT_EQ(std::uint8_t(7), avel::blend(true, std::uint8_t(7), std::uint8_t(3)));
        EXPECT_EQ(std::uint8_t(2), avel::blend(false, std::uint8_t(9), std::uint8_t(2)));
    }

    //=====================================================
    // min
    //=====================================================

    TEST(Scalar8u, Min_edge_cases) {
        EXPECT_EQ(std::uint8_t(0x00), avel::min(std::uint8_t(0x00), std::uint8_t(0x00)));
        EXPECT_EQ(std::uint8_t(0xFF), avel::min(std::uint8_t(0xFF), std::uint8_t(0xFF)));
        EXPECT_EQ(std::uint8_t(0xFF), avel::min(std::uint8_t(0xFF), std::uint8_t(0xFF)));
    }

    //=====================================================
    // max
    //=====================================================

    TEST(Scalar8u, Max_edge_cases) {
        EXPECT_EQ(std::uint8_t(0x00), avel::max(std::uint8_t(0x00), std::uint8_t(0x00)));
        EXPECT_EQ(std::uint8_t(0xFF), avel::max(std::uint8_t(0xFF), std::uint8_t(0xFF)));
        EXPECT_EQ(std::uint8_t(0xFF), avel::max(std::uint8_t(0xFF), std::uint8_t(0xFF)));
    }

    //=====================================================
    // minmax
    //=====================================================

    TEST(Scalar8u, Minmax_edge_cases) {
        auto t0 = avel::minmax(std::uint8_t{0x00}, std::uint8_t{0xFF});
        EXPECT_EQ(t0[0], std::uint8_t{0x00});
        EXPECT_EQ(t0[1], std::uint8_t{0xFF});

        auto t1 = avel::minmax(std::uint8_t{0xFF}, std::uint8_t{0x00});
        EXPECT_EQ(t1[0], std::uint8_t{0x00});
        EXPECT_EQ(t1[1], std::uint8_t{0xFF});

        auto t2 = avel::minmax(std::uint8_t{0x00}, std::uint8_t{0x00});
        EXPECT_EQ(t2[0], std::uint8_t{0x00});
        EXPECT_EQ(t2[1], std::uint8_t{0x00});

        auto t3 = avel::minmax(std::uint8_t{0xFF}, std::uint8_t{0xFF});
        EXPECT_EQ(t3[0], std::uint8_t{0xFF});
        EXPECT_EQ(t3[1], std::uint8_t{0xFF});
    }

    //=====================================================
    // clamp
    //=====================================================

    TEST(Scalar8u, Clamp_edge_cases) {
        EXPECT_EQ(std::uint8_t(0x00), avel::clamp(std::uint8_t(0x00), std::uint8_t(0x00), std::uint8_t(0x00)));
        EXPECT_EQ(std::uint8_t(0xFF), avel::clamp(std::uint8_t(0xFF), std::uint8_t(0xFF), std::uint8_t(0xFF)));

        EXPECT_EQ(std::uint8_t(0x00), avel::clamp(std::uint8_t(0xFF), std::uint8_t(0x00), std::uint8_t(0x00)));
        EXPECT_EQ(std::uint8_t(0xFF), avel::clamp(std::uint8_t(0x00), std::uint8_t(0xFF), std::uint8_t(0xFF)));
    }

    TEST(Scalar8u, Clamp_preselected) {
        EXPECT_EQ(std::uint8_t(10), avel::clamp(std::uint8_t(0),  std::uint8_t(10), std::uint8_t(20)));
        EXPECT_EQ(std::uint8_t(10), avel::clamp(std::uint8_t(10), std::uint8_t(5),  std::uint8_t(20)));
        EXPECT_EQ(std::uint8_t(10), avel::clamp(std::uint8_t(15), std::uint8_t(5),  std::uint8_t(10)));
    }

    //=====================================================
    // average
    //=====================================================

    TEST(Scalar8u, Average_edge_cases) {
        EXPECT_EQ(0x00, avel::average(std::uint8_t(0x00), std::uint8_t(0x00)));
        EXPECT_EQ(0xFF, avel::average(std::uint8_t(0xFF), std::uint8_t(0xFF)));
        EXPECT_EQ(0x7F, avel::average(std::uint8_t(0x00), std::uint8_t(0xFF)));
        EXPECT_EQ(0x7F, avel::average(std::uint8_t(0xFF), std::uint8_t(0x00)));
    }

    TEST(Scalar8u, Average_preselected) {
        EXPECT_EQ(0x08, avel::average(std::uint8_t(0x00), std::uint8_t(0x10)));
        EXPECT_EQ(0x08, avel::average(std::uint8_t(0x10), std::uint8_t(0x00)));

        EXPECT_EQ(0x09, avel::average(std::uint8_t(0x01), std::uint8_t(0x11)));
        EXPECT_EQ(0x09, avel::average(std::uint8_t(0x11), std::uint8_t(0x01)));

        EXPECT_EQ(0x08, avel::average(std::uint8_t(0x00), std::uint8_t(0x11)));
        EXPECT_EQ(0x08, avel::average(std::uint8_t(0x11), std::uint8_t(0x00)));
    }

    //=====================================================
    // midpoint
    //=====================================================

    TEST(Scalar8u, Midpoint_edge_cases) {
        EXPECT_EQ(0x00, avel::midpoint(std::uint8_t(0x00), std::uint8_t(0x00)));
        EXPECT_EQ(0xFF, avel::midpoint(std::uint8_t(0xFF), std::uint8_t(0xFF)));
        EXPECT_EQ(0x7F, avel::midpoint(std::uint8_t(0x00), std::uint8_t(0xFF)));
        EXPECT_EQ(0x80, avel::midpoint(std::uint8_t(0xFF), std::uint8_t(0x00)));
    }

    TEST(Scalar8u, Midpoint_preselected) {
        EXPECT_EQ(0x08, avel::midpoint(std::uint8_t(0x00), std::uint8_t(0x10)));
        EXPECT_EQ(0x08, avel::midpoint(std::uint8_t(0x10), std::uint8_t(0x00)));

        EXPECT_EQ(0x09, avel::midpoint(std::uint8_t(0x01), std::uint8_t(0x11)));
        EXPECT_EQ(0x09, avel::midpoint(std::uint8_t(0x11), std::uint8_t(0x01)));

        EXPECT_EQ(0x08, avel::midpoint(std::uint8_t(0x00), std::uint8_t(0x11)));
        EXPECT_EQ(0x09, avel::midpoint(std::uint8_t(0x11), std::uint8_t(0x00)));
    }

}

#endif //AVEL_SCALAR8U_TESTS_HPP
