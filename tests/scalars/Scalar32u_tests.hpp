#ifndef AVEL_SCALAR32U_TESTS_HPP
#define AVEL_SCALAR32U_TESTS_HPP

#include <gtest/gtest.h>
#include <bit>

namespace avel_tests {

    using namespace avel;

    TEST(Scalar32u_broadcast_mask, False) {
        EXPECT_EQ(broadcast_mask<std::uint32_t>(false), 0x00000000);
    }

    TEST(Scalar32u_broadcast_mask, True) {
        EXPECT_EQ(broadcast_mask<std::uint32_t>(true), 0xFFFFFFFF);
    }

    TEST(Scalar32u, Pop_count_contiguous_low) {
        EXPECT_EQ(popcount(0x00000000u), 0x00);
        EXPECT_EQ(popcount(0x00000001u), 0x01);
        EXPECT_EQ(popcount(0x00000003u), 0x02);
        EXPECT_EQ(popcount(0x00000007u), 0x03);
        EXPECT_EQ(popcount(0x0000000Fu), 0x04);
        EXPECT_EQ(popcount(0x0000001Fu), 0x05);
        EXPECT_EQ(popcount(0x0000003Fu), 0x06);
        EXPECT_EQ(popcount(0x0000007Fu), 0x07);

        EXPECT_EQ(popcount(0x000000FFu), 0x08);
        EXPECT_EQ(popcount(0x000001FFu), 0x09);
        EXPECT_EQ(popcount(0x000003FFu), 0x0A);
        EXPECT_EQ(popcount(0x000007FFu), 0x0B);
        EXPECT_EQ(popcount(0x00000FFFu), 0x0C);
        EXPECT_EQ(popcount(0x00001FFFu), 0x0D);
        EXPECT_EQ(popcount(0x00003FFFu), 0x0E);
        EXPECT_EQ(popcount(0x00007FFFu), 0x0F);

        EXPECT_EQ(popcount(0x0000FFFFu), 0x10);
        EXPECT_EQ(popcount(0x0001FFFFu), 0x11);
        EXPECT_EQ(popcount(0x0003FFFFu), 0x12);
        EXPECT_EQ(popcount(0x0007FFFFu), 0x13);
        EXPECT_EQ(popcount(0x000FFFFFu), 0x14);
        EXPECT_EQ(popcount(0x001FFFFFu), 0x15);
        EXPECT_EQ(popcount(0x003FFFFFu), 0x16);
        EXPECT_EQ(popcount(0x007FFFFFu), 0x17);

        EXPECT_EQ(popcount(0x00FFFFFFu), 0x18);
        EXPECT_EQ(popcount(0x01FFFFFFu), 0x19);
        EXPECT_EQ(popcount(0x03FFFFFFu), 0x1A);
        EXPECT_EQ(popcount(0x07FFFFFFu), 0x1B);
        EXPECT_EQ(popcount(0x0FFFFFFFu), 0x1C);
        EXPECT_EQ(popcount(0x1FFFFFFFu), 0x1D);
        EXPECT_EQ(popcount(0x3FFFFFFFu), 0x1E);
        EXPECT_EQ(popcount(0x7FFFFFFFu), 0x1F);

        EXPECT_EQ(popcount(0xFFFFFFFFu), 0x20);
    }

    TEST(Scalar32u, Pop_count_contiguous_high) {
        EXPECT_EQ(popcount(0x00000000u), 0x00);
        EXPECT_EQ(popcount(0x80000000u), 0x01);
        EXPECT_EQ(popcount(0xC0000000u), 0x02);
        EXPECT_EQ(popcount(0xE0000000u), 0x03);
        EXPECT_EQ(popcount(0xF0000000u), 0x04);
        EXPECT_EQ(popcount(0xF8000000u), 0x05);
        EXPECT_EQ(popcount(0xFC000000u), 0x06);
        EXPECT_EQ(popcount(0xFE000000u), 0x07);

        EXPECT_EQ(popcount(0xFF000000u), 0x08);
        EXPECT_EQ(popcount(0xFF800000u), 0x09);
        EXPECT_EQ(popcount(0xFFC00000u), 0x0A);
        EXPECT_EQ(popcount(0xFFE00000u), 0x0B);
        EXPECT_EQ(popcount(0xFFF00000u), 0x0C);
        EXPECT_EQ(popcount(0xFFF80000u), 0x0D);
        EXPECT_EQ(popcount(0xFFFC0000u), 0x0E);
        EXPECT_EQ(popcount(0xFFFE0000u), 0x0F);

        EXPECT_EQ(popcount(0xFFFF0000u), 0x10);
        EXPECT_EQ(popcount(0xFFFF8000u), 0x11);
        EXPECT_EQ(popcount(0xFFFFC000u), 0x12);
        EXPECT_EQ(popcount(0xFFFFE000u), 0x13);
        EXPECT_EQ(popcount(0xFFFFF000u), 0x14);
        EXPECT_EQ(popcount(0xFFFFF800u), 0x15);
        EXPECT_EQ(popcount(0xFFFFFC00u), 0x16);
        EXPECT_EQ(popcount(0xFFFFFE00u), 0x17);

        EXPECT_EQ(popcount(0xFFFFFF00u), 0x18);
        EXPECT_EQ(popcount(0xFFFFFF80u), 0x19);
        EXPECT_EQ(popcount(0xFFFFFFC0u), 0x1A);
        EXPECT_EQ(popcount(0xFFFFFFE0u), 0x1B);
        EXPECT_EQ(popcount(0xFFFFFFF0u), 0x1C);
        EXPECT_EQ(popcount(0xFFFFFFF8u), 0x1D);
        EXPECT_EQ(popcount(0xFFFFFFFCu), 0x1E);
        EXPECT_EQ(popcount(0xFFFFFFFEu), 0x1F);

        EXPECT_EQ(popcount(0xFFFFFFFFu), 0x20);
    }

    TEST(Scalar32u, Pop_count_strided_1) {
        EXPECT_EQ(popcount(0x00000001u), 0x01);
        EXPECT_EQ(popcount(0x00000005u), 0x02);
        EXPECT_EQ(popcount(0x00000015u), 0x03);
        EXPECT_EQ(popcount(0x00000055u), 0x04);
        EXPECT_EQ(popcount(0x00000155u), 0x05);
        EXPECT_EQ(popcount(0x00000555u), 0x06);
        EXPECT_EQ(popcount(0x00001555u), 0x07);
        EXPECT_EQ(popcount(0x00005555u), 0x08);
        EXPECT_EQ(popcount(0x00015555u), 0x09);
        EXPECT_EQ(popcount(0x00055555u), 0x0A);
        EXPECT_EQ(popcount(0x00155555u), 0x0B);
        EXPECT_EQ(popcount(0x00555555u), 0x0C);
        EXPECT_EQ(popcount(0x01555555u), 0x0D);
        EXPECT_EQ(popcount(0x05555555u), 0x0E);
        EXPECT_EQ(popcount(0x15555555u), 0x0F);
        EXPECT_EQ(popcount(0x55555555u), 0x10);
    }

    TEST(Scalar32u, Pop_count_strided_2) {
        EXPECT_EQ(popcount(0x00000001u), 0x01);
        EXPECT_EQ(popcount(0x00000009u), 0x02);
        EXPECT_EQ(popcount(0x00000049u), 0x03);
        EXPECT_EQ(popcount(0x00000249u), 0x04);
        EXPECT_EQ(popcount(0x00001249u), 0x05);
        EXPECT_EQ(popcount(0x00009249u), 0x06);
        EXPECT_EQ(popcount(0x00049249u), 0x07);
        EXPECT_EQ(popcount(0x00249249u), 0x08);
        EXPECT_EQ(popcount(0x01249249u), 0x09);
        EXPECT_EQ(popcount(0x09249249u), 0x0A);
        EXPECT_EQ(popcount(0x49249249u), 0x0B);
    }

    TEST(Scalar32u, Pop_count_misc) {
        EXPECT_EQ(popcount(0x11110000u), 0x04);
    }

    TEST(Scalar32u_byteswap, Lo_byte) {
        EXPECT_EQ(byteswap(0x000000FFu), 0xFF000000);
    }

    TEST(Scalar32u_countl_zero, Contiguous_set_bits) {
        EXPECT_EQ(countl_zero(0xFFFFFFFFu), 0x0000000000);
        EXPECT_EQ(countl_zero(0x7FFFFFFFu), 0x0000000001);
        EXPECT_EQ(countl_zero(0x3FFFFFFFu), 0x0000000002);
        EXPECT_EQ(countl_zero(0x1FFFFFFFu), 0x0000000003);

        EXPECT_EQ(countl_zero(0x0FFFFFFFu), 0x0000000004);
        EXPECT_EQ(countl_zero(0x07FFFFFFu), 0x0000000005);
        EXPECT_EQ(countl_zero(0x03FFFFFFu), 0x0000000006);
        EXPECT_EQ(countl_zero(0x01FFFFFFu), 0x0000000007);

        EXPECT_EQ(countl_zero(0x00FFFFFFu), 0x0000000008);
        EXPECT_EQ(countl_zero(0x007FFFFFu), 0x0000000009);
        EXPECT_EQ(countl_zero(0x003FFFFFu), 0x000000000A);
        EXPECT_EQ(countl_zero(0x001FFFFFu), 0x000000000B);

        EXPECT_EQ(countl_zero(0x000FFFFFu), 0x000000000C);
        EXPECT_EQ(countl_zero(0x0007FFFFu), 0x000000000D);
        EXPECT_EQ(countl_zero(0x0003FFFFu), 0x000000000E);
        EXPECT_EQ(countl_zero(0x0001FFFFu), 0x000000000F);



        EXPECT_EQ(countl_zero(0x0000FFFFu), 0x00000010);
        EXPECT_EQ(countl_zero(0x00007FFFu), 0x00000011);
        EXPECT_EQ(countl_zero(0x00003FFFu), 0x00000012);
        EXPECT_EQ(countl_zero(0x00001FFFu), 0x00000013);

        EXPECT_EQ(countl_zero(0x00000FFFu), 0x00000014);
        EXPECT_EQ(countl_zero(0x000007FFu), 0x00000015);
        EXPECT_EQ(countl_zero(0x000003FFu), 0x00000016);
        EXPECT_EQ(countl_zero(0x000001FFu), 0x00000017);

        EXPECT_EQ(countl_zero(0x000000FFu), 0x00000018);
        EXPECT_EQ(countl_zero(0x0000007Fu), 0x00000019);
        EXPECT_EQ(countl_zero(0x0000003Fu), 0x0000001A);
        EXPECT_EQ(countl_zero(0x0000001Fu), 0x0000001B);

        EXPECT_EQ(countl_zero(0x0000000Fu), 0x0000001C);
        EXPECT_EQ(countl_zero(0x00000007u), 0x0000001D);
        EXPECT_EQ(countl_zero(0x00000003u), 0x0000001E);
        EXPECT_EQ(countl_zero(0x00000001u), 0x0000001F);

        EXPECT_EQ(countl_zero(0x00000000u), 0x0000000020);
    }

    TEST(Scalar32u_countr_zero, Contiguous_set_bits) {
        EXPECT_EQ(countr_zero(0x00000000u), 32);
        EXPECT_EQ(countr_zero(0x80000000u), 31);
        EXPECT_EQ(countr_zero(0xC0000000u), 30);
        EXPECT_EQ(countr_zero(0xE0000000u), 29);
        EXPECT_EQ(countr_zero(0xF0000000u), 28);
        EXPECT_EQ(countr_zero(0xF8000000u), 27);
        EXPECT_EQ(countr_zero(0xFC000000u), 26);
        EXPECT_EQ(countr_zero(0xFE000000u), 25);
        EXPECT_EQ(countr_zero(0xFF000000u), 24);
        EXPECT_EQ(countr_zero(0xFF800000u), 23);
        EXPECT_EQ(countr_zero(0xFFC00000u), 22);
        EXPECT_EQ(countr_zero(0xFFE00000u), 21);
        EXPECT_EQ(countr_zero(0xFFF00000u), 20);
        EXPECT_EQ(countr_zero(0xFFF80000u), 19);
        EXPECT_EQ(countr_zero(0xFFFC0000u), 18);
        EXPECT_EQ(countr_zero(0xFFFE0000u), 17);
        EXPECT_EQ(countr_zero(0xFFFF0000u), 16);
        EXPECT_EQ(countr_zero(0xFFFF8000u), 15);
        EXPECT_EQ(countr_zero(0xFFFFC000u), 14);
        EXPECT_EQ(countr_zero(0xFFFFE000u), 13);
        EXPECT_EQ(countr_zero(0xFFFFF000u), 12);
        EXPECT_EQ(countr_zero(0xFFFFF800u), 11);
        EXPECT_EQ(countr_zero(0xFFFFFC00u), 10);
        EXPECT_EQ(countr_zero(0xFFFFFE00u), 9);
        EXPECT_EQ(countr_zero(0xFFFFFF00u), 8);
        EXPECT_EQ(countr_zero(0xFFFFFF80u), 7);
        EXPECT_EQ(countr_zero(0xFFFFFFC0u), 6);
        EXPECT_EQ(countr_zero(0xFFFFFFE0u), 5);
        EXPECT_EQ(countr_zero(0xFFFFFFF0u), 4);
        EXPECT_EQ(countr_zero(0xFFFFFFF8u), 3);
        EXPECT_EQ(countr_zero(0xFFFFFFFCu), 2);
        EXPECT_EQ(countr_zero(0xFFFFFFFEu), 1);
        EXPECT_EQ(countr_zero(0xFFFFFFFFu), 0);
    }

    TEST(Scalar32u_countr_zero, Discontiguous_set_bits) {
        EXPECT_EQ(countr_zero(0x00000001u), 0);
    }

    TEST(Scalar32u, Bit_floor) {
        EXPECT_EQ(bit_floor(0x00000000u), 0x00000000);
        EXPECT_EQ(bit_floor(0x00000001u), 0x00000001);
        EXPECT_EQ(bit_floor(0x00000002u), 0x00000002);
        EXPECT_EQ(bit_floor(0x00000003u), 0x00000002);
        EXPECT_EQ(bit_floor(0x00000004u), 0x00000004);
        EXPECT_EQ(bit_floor(0x00000005u), 0x00000004);
        EXPECT_EQ(bit_floor(0x00000006u), 0x00000004);
        EXPECT_EQ(bit_floor(0x00000007u), 0x00000004);
        EXPECT_EQ(bit_floor(0x00000008u), 0x00000008);
    }

    TEST(Scalar32u, Bit_width) {
        EXPECT_EQ(bit_width(0x00000000u), 0x00000000);
        EXPECT_EQ(bit_width(0x00000001u), 0x00000001);
        EXPECT_EQ(bit_width(0x00000002u), 0x00000002);
        EXPECT_EQ(bit_width(0x00000003u), 0x00000002);
        EXPECT_EQ(bit_width(0x00000004u), 0x00000003);
        EXPECT_EQ(bit_width(0x00000005u), 0x00000003);
        EXPECT_EQ(bit_width(0x00000006u), 0x00000003);
        EXPECT_EQ(bit_width(0x00000007u), 0x00000003);
        EXPECT_EQ(bit_width(0x00000008u), 0x00000004);
    }

    TEST(Scalar32u, Bit_ceil) {
        EXPECT_EQ(bit_ceil(0x00000000u), 0x00000001);
        EXPECT_EQ(bit_ceil(0x00000001u), 0x00000001);
        EXPECT_EQ(bit_ceil(0x00000002u), 0x00000002);
        EXPECT_EQ(bit_ceil(0x00000003u), 0x00000004);
        EXPECT_EQ(bit_ceil(0x00000004u), 0x00000004);
        EXPECT_EQ(bit_ceil(0x00000005u), 0x00000008);
        EXPECT_EQ(bit_ceil(0x00000006u), 0x00000008);
        EXPECT_EQ(bit_ceil(0x00000007u), 0x00000008);
        EXPECT_EQ(bit_ceil(0x00000008u), 0x00000008);
    }

    TEST(Scalar32u, Has_single_bit) {
        EXPECT_FALSE(has_single_bit(0x0000u));
        EXPECT_TRUE(has_single_bit(0x0001u));
        EXPECT_TRUE(has_single_bit(0x0002u));
        EXPECT_FALSE(has_single_bit(0x0003u));

        EXPECT_TRUE(has_single_bit(0x0004u));
        EXPECT_FALSE(has_single_bit(0x0005u));
        EXPECT_FALSE(has_single_bit(0x0006u));
        EXPECT_FALSE(has_single_bit(0x0007u));
    }

    /*
    TEST(Scalar32u, Midpoint) {
        //EXPECT_EQ(std::midpoint(0u, 0u), avel::midpoint(0u, 0u));
        //EXPECT_EQ(std::midpoint(0u, 1u), avel::midpoint(0u, 1u));
        //EXPECT_EQ(std::midpoint(1u, 0u), avel::midpoint(1u, 0u));
        //EXPECT_EQ(std::midpoint(1u, 1u), avel::midpoint(1u, 1u));
        //EXPECT_EQ(std::midpoint(0u, 2u), avel::midpoint(0u, 2u));
        //EXPECT_EQ(std::midpoint(2u, 0u), avel::midpoint(2u, 0u));
    }

    TEST(Scalar32u, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t a = random32u();
            std::uint32_t b = random32u();

            auto result = avel::midpoint(a, b);
            auto baseline = std::midpoint(a, b);

            EXPECT_EQ(result, baseline);
        }
    }
    */

}

#endif //AVEL_SCALAR32U_TESTS_HPP
