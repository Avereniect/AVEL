#ifndef AVEL_DENOM16U_HPP
#define AVEL_DENOM16U_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom16u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t numerator = random16u();
            std::uint16_t denominator = random16u();
            if (denominator == 0) {
                denominator = 1;
            }

            std::uint16_t baseline_quotient = numerator / denominator;
            std::uint16_t baseline_remainder = numerator % denominator;

            Denom16u denom{denominator};
            auto qr = div(numerator, denom);
            EXPECT_EQ(baseline_quotient, qr.quot);
            EXPECT_EQ(baseline_remainder, qr.rem);
        }
    }

}

#endif //AVEL_DENOM16U_HPP
