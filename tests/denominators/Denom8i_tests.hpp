#ifndef AVEL_DENOM8I_TESTS_HPP
#define AVEL_DENOM8I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom8i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t numerator = random8u();
            std::int8_t denominator = random8u();
            if (denominator == 0) {
                denominator = 1;
            }

            std::int8_t baseline_quotient = numerator / denominator;
            std::int8_t baseline_remainder = numerator % denominator;

            Denom8i denom{denominator};
            auto qr = div(numerator, denom);
            EXPECT_EQ(baseline_quotient, qr.quot);
            EXPECT_EQ(baseline_remainder, qr.rem);
        }
    }

}

#endif //AVEL_DENOM8I_TESTS_HPP
