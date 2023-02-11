#ifndef AVEL_DENOM64U_TESTS_HPP
#define AVEL_DENOM64U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom64u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t numerator = random64u();
            std::uint64_t denominator = random64u();
            if (denominator == 0) {
                denominator = 1;
            }

            std::uint64_t baseline_quotient = numerator / denominator;
            std::uint64_t baseline_remainder = numerator % denominator;

            Denom64u denom{denominator};
            auto qr = div(numerator, denom);
            EXPECT_EQ(baseline_quotient, qr.quot);
            EXPECT_EQ(baseline_remainder, qr.rem);
        }
    }

}

#endif //AVEL_DENOM64U_TESTS_HPP
