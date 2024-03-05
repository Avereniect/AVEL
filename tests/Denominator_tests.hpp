#ifndef AVEL_DENOMINATOR_TESTS_HPP
#define AVEL_DENOMINATOR_TESTS_HPP

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

    TEST(Denom32i, Division) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t numerator = random32u();
            std::int32_t denominator = random32u();
            if (denominator == 0) {
                denominator = 1;
            }

            std::int32_t baseline_quotient = numerator / denominator;
            std::int32_t baseline_remainder = numerator % denominator;

            Denom32i denom{denominator};
            auto qr = div(numerator, denom);
            EXPECT_EQ(baseline_quotient, qr.quot);
            EXPECT_EQ(baseline_remainder, qr.rem);
        }
    }

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

    TEST(Denom64i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t numerator = random64u();
            std::int64_t denominator = 1;
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

    //TODO: Add tests for shift operators

}

#endif
