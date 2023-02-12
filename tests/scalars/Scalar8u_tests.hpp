#ifndef AVEL_SCALAR8U_TESTS_HPP
#define AVEL_SCALAR8U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // broadcast_mask
    //=====================================================

    TEST(Scalar8u, Broadcast_mask) {
        EXPECT_EQ(std::uint8_t{0x00}, broadcast_mask<std::uint8_t>(false));
        EXPECT_EQ(std::uint8_t{0xFF}, broadcast_mask<std::uint8_t>(true));
    }

    //=====================================================
    // popcount
    //=====================================================

    TEST(Scalar8u, Pop_count_contiguous_low) {
        EXPECT_EQ(popcount(std::uint8_t(0x00u)), 0x00);
        EXPECT_EQ(popcount(std::uint8_t(0x01u)), 0x01);
        EXPECT_EQ(popcount(std::uint8_t(0x03u)), 0x02);
        EXPECT_EQ(popcount(std::uint8_t(0x07u)), 0x03);
        EXPECT_EQ(popcount(std::uint8_t(0x0Fu)), 0x04);
        EXPECT_EQ(popcount(std::uint8_t(0x1Fu)), 0x05);
        EXPECT_EQ(popcount(std::uint8_t(0x3Fu)), 0x06);
        EXPECT_EQ(popcount(std::uint8_t(0x7Fu)), 0x07);
    }

    TEST(Scalar8u, Popcount_edge_cases) {
        EXPECT_EQ(0, popcount(std::uint8_t{0x00}));
        EXPECT_EQ(8, popcount(std::uint8_t{0xFF}));
    }

    TEST(Scalar8u, Popcount_ones) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::uint8_t x = (1 << i);

            EXPECT_EQ(1, popcount(x));
        }
    }

    TEST(Scalar8u, Popcount_preselected) {
        EXPECT_EQ(2, popcount(std::uint8_t{0x03}));
        EXPECT_EQ(6, popcount(std::uint8_t{0xF3}));
        EXPECT_EQ(4, popcount(std::uint8_t{0x3C}));
        EXPECT_EQ(6, popcount(std::uint8_t{0xE7}));
        EXPECT_EQ(4, popcount(std::uint8_t{0x5A}));
        EXPECT_EQ(2, popcount(std::uint8_t{0x21}));
    }

    //=====================================================
    // byteswap
    //=====================================================

    TEST(Scalar8u, Byteswap) {
        EXPECT_EQ(byteswap(std::uint8_t(0x00)), 0x00);
        EXPECT_EQ(byteswap(std::uint8_t(0x01)), 0x01);
        EXPECT_EQ(byteswap(std::uint8_t(0x0a)), 0x0a);
        EXPECT_EQ(byteswap(std::uint8_t(0x12)), 0x12);
    }

    //=====================================================
    // countl_zero
    //=====================================================

    TEST(Scalar8u, Countl_zero_edge_cases) {
        EXPECT_EQ(8, countl_zero(std::uint8_t{0x00}));
        EXPECT_EQ(0, countl_zero(std::uint8_t{0xFF}));
    }

    TEST(Scalar8u, Countl_zero_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_EQ(7 - i, countl_zero(x));
        }
    }

    TEST(Scalar8u, Countl_zero_powers_of_two_minus_one) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;
            std::uint8_t y = x - 1;

            EXPECT_EQ(8 - i, countl_zero(y));
        }
    }

    //=====================================================
    // countl_one
    //=====================================================

    TEST(Scalar8u, Countl_one_edge_cases) {
        EXPECT_EQ(0, countl_one(std::uint8_t(0x00)));
        EXPECT_EQ(8, countl_one(std::uint8_t(0xFF)));
    }

    TEST(Scalar8u, Countl_one_contiguous_leading_bits) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::uint8_t x = std::uint8_t{0xFF} << i;

            EXPECT_EQ(8 - i, countl_one(x));
        }
    }

    //=====================================================
    // countr_zero
    //=====================================================

    TEST(Scalar8u, Countr_zero_edge_cases) {
        EXPECT_EQ(8, countr_zero(std::uint8_t(0x00)));
        EXPECT_EQ(0, countr_zero(std::uint8_t(0xFF)));
    }

    TEST(Scalar8u, Countr_zero_powers_of_two) {
        for (std::int32_t i = 0; i < 8; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_EQ(i, countr_zero(x));
        }
    }

    TEST(Scalar8u, Countr_zero_contiguous_leading_bits) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{0xFF} << i;

            EXPECT_EQ(i, countr_zero(x));
        }
    }

    //=====================================================
    // countr_one
    //=====================================================

    //=====================================================
    // bit_width
    //=====================================================

    TEST(Scalar8u, Bit_width_edge_cases) {
        EXPECT_EQ(0, bit_width(std::uint8_t{0x00}));
        EXPECT_EQ(8, bit_width(std::uint8_t{0xFF}));
    }

    TEST(Scalar8u, Bit_width_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_EQ(i + 1, bit_width(x));
        }
    }

    //=====================================================
    // bit_floor
    //=====================================================

    TEST(Scalar8u, Bit_floor_edge_cases) {
        EXPECT_EQ(0x00, bit_floor(std::uint8_t{0x00}));
        EXPECT_EQ(0x01, bit_floor(std::uint8_t{0x01}));
        EXPECT_EQ(0x80, bit_floor(std::uint8_t{0xFF}));
    }

    TEST(Scalar8u, Bit_floor_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_EQ(x, bit_floor(x));
        }
    }

    TEST(Scalar8u, Bit_floor_powers_of_two_minus_one) {
        for (std::int32_t i = 2; i < 8; ++i) {
            std::uint8_t x = (std::uint8_t{1} << i);
            std::uint8_t y = x - 1;

            EXPECT_EQ(std::uint8_t{1} << (i - 1), bit_floor(y));
        }
    }

    TEST(Scalar8u, Bit_floor_powers_of_two_plus_one) {
        for (std::int32_t i = 1; i < 7; ++i) {
            std::uint8_t x = (std::uint8_t{1} << i);
            std::uint8_t y = x + 1;

            EXPECT_EQ(x, bit_floor(y));
        }
    }

    //=====================================================
    // bit_ceil
    //=====================================================

    TEST(Scalar8u, Bit_ceil_edge_cases) {
        EXPECT_EQ(1, bit_ceil(std::uint8_t{0}));
        EXPECT_EQ(1, bit_ceil(std::uint8_t{1}));
    }

    TEST(Scalar8u, Bit_ceil_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_EQ(x, bit_ceil(x));
        }
    }

    TEST(Scalar8u, Bit_ceil_powers_of_two_minus_one) {
        for (std::int32_t i = 2; i < 8; ++i) {
            std::uint8_t x = (std::uint8_t{1} << i);
            std::uint8_t y = x - 1;

            EXPECT_EQ(x, bit_ceil(y));
        }
    }

    TEST(Scalar8u, Bit_ceil_powers_of_two_plus_one) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = (std::uint8_t{1} << i);
            std::uint8_t y = x + 1;

            EXPECT_EQ(x << 1, bit_ceil(y));
        }
    }

    //=====================================================
    // has_single_bit
    //=====================================================

    TEST(Scalar8u, Has_single_bit_edge_cases) {
        EXPECT_FALSE(has_single_bit(std::uint8_t(0x00)));
        EXPECT_FALSE(has_single_bit(std::uint8_t(0xFF)));
    }

    TEST(Scalar8u, Has_single_bit_powers_of_two) {
        for (std::int32_t i = 0; i < 7; ++i) {
            std::uint8_t x = std::uint8_t{1} << i;

            EXPECT_TRUE(has_single_bit(x));
        }
    }

    //=====================================================
    // rotl
    //=====================================================

    TEST(Sclaar8u, rotl_preselected) {
        EXPECT_EQ(0x80, rotl(std::uint8_t(0x80), 0x0));
        EXPECT_EQ(0x01, rotl(std::uint8_t(0x80), 0x1));
    }

    //=====================================================
    // rotr
    //=====================================================

    TEST(Sclaar8u, rotr_preselected) {
        EXPECT_EQ(0x01, rotr(std::uint8_t(0x01), 0));
        EXPECT_EQ(0x80, rotr(std::uint8_t(0x01), 1));
    }

    //=====================================================
    // neg_abs
    //=====================================================

    TEST(Scalar8u, Neg_abs_preselected) {
        EXPECT_EQ(std::int8_t{0},  neg_abs(std::uint8_t{+0}));
        EXPECT_EQ(std::int8_t{-1}, neg_abs(std::uint8_t{+1}));
        EXPECT_EQ(std::int8_t{-127}, neg_abs(std::uint8_t{+127}));
        EXPECT_EQ(std::int8_t{-128}, neg_abs(std::uint8_t{+128}));
    }

    //=====================================================
    // blend
    //=====================================================

    TEST(Scalar8u, Blend_preselected) {
        EXPECT_EQ(5, blend(true, 5, 3));
        EXPECT_EQ(3, blend(false, 5, 3));
        EXPECT_EQ(7, blend(true, 7, 3));
        EXPECT_EQ(2, blend(false, 9, 2));
    }

    //=====================================================
    // min
    //=====================================================

    TEST(Scalar8u, Min_edge_cases) {
        EXPECT_EQ(0x00, min(0x00, 0x00));
        EXPECT_EQ(0xFF, min(0xFF, 0xFF));
        EXPECT_EQ(0xFF, min(0xFF, 0xFF));
    }

    //=====================================================
    // max
    //=====================================================

    TEST(Scalar8u, Max_edge_cases) {
        EXPECT_EQ(0x00, max(0x00, 0x00));
        EXPECT_EQ(0xFF, max(0xFF, 0xFF));
        EXPECT_EQ(0xFF, max(0xFF, 0xFF));
    }

    //=====================================================
    // minmax
    //=====================================================

    TEST(Scalar8u, Minmax_edge_cases) {
        auto t0 = minmax(std::uint8_t{0x00}, std::uint8_t{0xFF});
        EXPECT_EQ(t0[0], std::uint8_t{0x00});
        EXPECT_EQ(t0[1], std::uint8_t{0xFF});

        auto t1 = minmax(std::uint8_t{0xFF}, std::uint8_t{0x00});
        EXPECT_EQ(t1[0], std::uint8_t{0x00});
        EXPECT_EQ(t1[1], std::uint8_t{0xFF});

        auto t2 = minmax(std::uint8_t{0x00}, std::uint8_t{0x00});
        EXPECT_EQ(t2[0], std::uint8_t{0x00});
        EXPECT_EQ(t2[1], std::uint8_t{0x00});

        auto t3 = minmax(std::uint8_t{0xFF}, std::uint8_t{0xFF});
        EXPECT_EQ(t3[0], std::uint8_t{0xFF});
        EXPECT_EQ(t3[1], std::uint8_t{0xFF});
    }

    //=====================================================
    // clamp
    //=====================================================

    TEST(Scalar8u, Clamp_edge_cases) {
        EXPECT_EQ(0x00, clamp(std::uint8_t{0x00}, std::uint8_t{0x00}, std::uint8_t{0x00}));
        EXPECT_EQ(0xFF, clamp(std::uint8_t{0xFF}, std::uint8_t{0xFF}, std::uint8_t{0xFF}));

        EXPECT_EQ(0x00, clamp(std::uint8_t{0xFF}, std::uint8_t{0x00}, std::uint8_t{0x00}));
        EXPECT_EQ(0xFF, clamp(std::uint8_t{0x00}, std::uint8_t{0xFF}, std::uint8_t{0xFF}));
    }

    TEST(Scalar8u, Clamp_preselected) {
        EXPECT_EQ(10, clamp(0, 10, 20));
        EXPECT_EQ(10, clamp(10, 5, 20));
        EXPECT_EQ(10, clamp(15, 5, 10));
    }

    //=====================================================
    // average
    //=====================================================

    TEST(Scalar8u, Average_edge_cases) {
        EXPECT_EQ(0x00, average(std::uint8_t{0x00}, std::uint8_t{0x00}));
        EXPECT_EQ(0xFF, average(std::uint8_t{0xFF}, std::uint8_t{0xFF}));
        EXPECT_EQ(0x7F, average(std::uint8_t{0x00}, std::uint8_t{0xFF}));
        EXPECT_EQ(0x7F, average(std::uint8_t{0xFF}, std::uint8_t{0x00}));
    }

    TEST(Scalar8u, Average_preselected) {
        EXPECT_EQ(0x08, average(std::uint8_t{0x00}, std::uint8_t{0x10}));
        EXPECT_EQ(0x08, average(std::uint8_t{0x10}, std::uint8_t{0x00}));

        EXPECT_EQ(0x09, average(std::uint8_t{0x01}, std::uint8_t{0x11}));
        EXPECT_EQ(0x09, average(std::uint8_t{0x11}, std::uint8_t{0x01}));

        EXPECT_EQ(0x08, average(std::uint8_t{0x00}, std::uint8_t{0x11}));
        EXPECT_EQ(0x08, average(std::uint8_t{0x11}, std::uint8_t{0x00}));
    }

    //=====================================================
    // midpoint
    //=====================================================

    TEST(Scalar8u, Midpoint_edge_cases) {
        EXPECT_EQ(0x00, midpoint(std::uint8_t{0x00}, std::uint8_t{0x00}));
        EXPECT_EQ(0xFF, midpoint(std::uint8_t{0xFF}, std::uint8_t{0xFF}));
        EXPECT_EQ(0x7F, midpoint(std::uint8_t{0x00}, std::uint8_t{0xFF}));
        EXPECT_EQ(0x80, midpoint(std::uint8_t{0xFF}, std::uint8_t{0x00}));
    }

    TEST(Scalar8u, Midpoint_preselected) {
        EXPECT_EQ(0x08, midpoint(std::uint8_t{0x00}, std::uint8_t{0x10}));
        EXPECT_EQ(0x08, midpoint(std::uint8_t{0x10}, std::uint8_t{0x00}));

        EXPECT_EQ(0x09, midpoint(std::uint8_t{0x01}, std::uint8_t{0x11}));
        EXPECT_EQ(0x09, midpoint(std::uint8_t{0x11}, std::uint8_t{0x01}));

        EXPECT_EQ(0x08, midpoint(std::uint8_t{0x00}, std::uint8_t{0x11}));
        EXPECT_EQ(0x09, midpoint(std::uint8_t{0x11}, std::uint8_t{0x00}));
    }

}

#endif //AVEL_SCALAR8U_TESTS_HPP
