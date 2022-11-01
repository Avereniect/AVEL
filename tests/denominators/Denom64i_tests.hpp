#ifndef AVEL_DENOM64I_TESTS_HPP
#define AVEL_DENOM64I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom64i, Division) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t numerator = random8u();
            std::int64_t denominator = random8u();
            if (denominator == 0) {
                denominator = 1;
            }

            std::int64_t baseline_quotient = numerator / denominator;
            std::int64_t baseline_remainder = numerator % denominator;

            Denom64i denom{denominator};
            auto qr = div(numerator, denom);
            EXPECT_EQ(baseline_quotient, qr.quot);
            EXPECT_EQ(baseline_remainder, qr.rem);
        }
    }

}

#endif //AVEL_DENOM64I_TESTS_HPP
