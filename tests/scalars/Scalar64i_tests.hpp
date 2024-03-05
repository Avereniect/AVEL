#ifndef AVEL_SCALAR64I_TESTS_HPP
#define AVEL_SCALAR64I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // Comparisons
    //=====================================================

    TEST(Scalar64i, cmp_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_equal(x, y);

            if (x >= 0x8000000000000000ull || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar64i, cmp_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_equal(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar64i, cmp_not_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar64i, cmp_not_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar64i, cmp_less_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_less(x, y);

            if (x >= 0x8000000000000000ull || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar64i, cmp_less_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_less(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar64i, Cmp_le_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_less_equal(x, y);

            if (x >= 0x8000000000000000ull || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar64i, Cmp_le_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_less_equal(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar64i, cmp_greater_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_greater(x, y);

            if (x >= 0x8000000000000000ull || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar64i, cmp_greater_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_greater(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar64i, cmp_greater_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_greater_equal(x, y);

            if (x >= 0x8000000000000000ull || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    TEST(Scalar64i, cmp_greater_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_greater_equal(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    //=====================================================
    // Integer operations
    //=====================================================

    TEST(Scalar64i, countl_sign_powers_of_two) {
        for (std::size_t i = 0; i < 63; ++i) {
            std::int64_t input = std::int64_t(0x0000000000000001) << (62 - i);
            std::int64_t observed = avel::countl_sign(input);
            std::int64_t expected = i;

            EXPECT_EQ(observed, expected);
        }

        {
            std::int64_t input = 0x0000000000000000;
            std::int64_t observed = avel::countl_sign(input);
            std::int64_t expected = 63;

            EXPECT_EQ(observed, expected);
        }

        for (std::size_t i = 0; i < 63; ++i) {
            std::int64_t input = std::int64_t(0xffffffffffffffff) << i;
            std::int64_t observed = avel::countl_sign(input);
            std::int64_t expected = (63 - i);

            EXPECT_EQ(observed, expected);
        }
    }

}

#endif //AVEL_SCALAR64I_TESTS_HPP
