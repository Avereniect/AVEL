#ifndef AVEL_SCALAR64U_TESTS_HPP
#define AVEL_SCALAR64U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // bit_ceil
    //=====================================================

    TEST(Scalar64u, Bit_ceil_edge_cases) {
        EXPECT_EQ(1, bit_ceil(std::uint64_t{0}));
        EXPECT_EQ(1, bit_ceil(std::uint64_t{1}));
    }

    TEST(Scalar64u, Bit_ceil_powers_of_two) {
        for (std::int32_t i = 0; i < 63; ++i) {
            std::uint64_t x = std::uint64_t{1} << i;

            EXPECT_EQ(x, bit_ceil(x));
        }
    }

    TEST(Scalar64u, Bit_ceil_powers_of_two_minus_one) {
        for (std::int32_t i = 2; i < 64; ++i) {
            std::uint64_t x = (std::uint64_t{1} << i);
            std::uint64_t y = x - 1;

            EXPECT_EQ(x, bit_ceil(y));
        }
    }

    TEST(Scalar64u, Bit_ceil_powers_of_two_plus_one) {
        for (std::int32_t i = 0; i < 63; ++i) {
            std::uint64_t x = (std::uint64_t{1} << i);
            std::uint64_t y = x + 1;

            EXPECT_EQ(x << 1, bit_ceil(y));
        }
    }

}

#endif //AVEL_SCALAR64U_TESTS_HPP
