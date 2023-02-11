#ifndef AVEL_DENOM8U_TESTS_HPP
#define AVEL_DENOM8U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom8u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t numerator = random8u();
            std::uint8_t denominator = random8u();
            if (denominator == 0) {
                denominator = 1;
            }

            std::uint8_t baseline_quotient = numerator / denominator;
            std::uint8_t baseline_remainder = numerator % denominator;

            Denom8u denom{denominator};
            auto qr = div(numerator, denom);
            EXPECT_EQ(baseline_quotient, qr.quot);
            EXPECT_EQ(baseline_remainder, qr.rem);
        }
    }

}

#endif //AVEL_DENOM8U_TESTS_HPP
