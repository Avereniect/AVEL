#ifndef AVEL_DENOM8U_TESTS_HPP
#define AVEL_DENOM8U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom8u, Division) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t numerator = random8u();
            std::uint8_t denominator = max(static_cast<std::uint8_t>(random8u()), static_cast<std::uint8_t>(1));

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
