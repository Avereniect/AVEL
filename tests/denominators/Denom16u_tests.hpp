#ifndef AVEL_DENOM16U_HPP
#define AVEL_DENOM16U_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom16u, Division) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t numerator = random16u();
            std::uint16_t denominator = max(static_cast<std::uint16_t>(random16u()), static_cast<std::uint16_t>(1));

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
