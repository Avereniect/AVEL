#ifndef AVEL_DENOM16I_TESTS_HPP
#define AVEL_DENOM16I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom16i, Division) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t numerator = random16u();
            std::int16_t denominator = random16u();
            if (denominator == 0) {
                denominator = 1;
            }

            std::int16_t baseline_quotient = numerator / denominator;
            std::int16_t baseline_remainder = numerator % denominator;

            Denom16i denom{denominator};
            auto qr = div(numerator, denom);
            EXPECT_EQ(baseline_quotient, qr.quot);
            EXPECT_EQ(baseline_remainder, qr.rem);
        }
    }

}

#endif //AVEL_DENOM16I_TESTS_HPP
