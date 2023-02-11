#ifndef AVEL_DENOM32U_TESTS_HPP
#define AVEL_DENOM32U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom32u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t numerator = random32u();
            std::uint32_t denominator = random32u();
            if (denominator == 0) {
                denominator = 1;
            }


            std::uint32_t baseline_quotient = numerator / denominator;
            std::uint32_t baseline_remainder = numerator % denominator;

            Denom32u denom{denominator};
            auto qr = div(numerator, denom);
            EXPECT_EQ(baseline_quotient, qr.quot);
            EXPECT_EQ(baseline_remainder, qr.rem);
        }
    }

}

#endif //AVEL_DENOM32U_TESTS_HPP
