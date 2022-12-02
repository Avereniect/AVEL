#ifndef AVEL_SCALAR8U_TESTS_HPP
#define AVEL_SCALAR8U_TESTS_HPP

namespace avel {

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

}

#endif //AVEL_SCALAR8U_TESTS_HPP
