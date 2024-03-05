#ifndef AVEL_SCALAR32I_TESTS_HPP
#define AVEL_SCALAR32I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // Comparisons
    //=====================================================

    TEST(Scalar32i, cmp_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_equal(x, y);

            if (x >= 0x80000000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar32i, cmp_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_equal(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar32i, cmp_not_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar32i, cmp_not_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar32i, cmp_less_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_less(x, y);

            if (x >= 0x80000000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar32i, cmp_less_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_less(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar32i, Cmp_le_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_less_equal(x, y);

            if (x >= 0x80000000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar32i, Cmp_le_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_less_equal(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar32i, cmp_greater_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_greater(x, y);

            if (x >= 0x80000000 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar32i, cmp_greater_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_greater(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar32i, cmp_greater_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_greater_equal(x, y);

            if (x >= 0x80000000 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    TEST(Scalar32i, cmp_greater_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_greater_equal(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    //=====================================================
    // Integer operations
    //=====================================================

    TEST(Scalar32i, countl_sign_powers_of_two) {
        for (std::size_t i = 0; i < 31; ++i) {
            std::int32_t input = std::int32_t(0x00000001) << (30 - i);
            std::int32_t observed = avel::countl_sign(input);
            std::int32_t expected = i;

            EXPECT_EQ(observed, expected);
        }

        {
            std::int32_t input = 0x00;
            std::int32_t observed = avel::countl_sign(input);
            std::int32_t expected = 31;

            EXPECT_EQ(observed, expected);
        }

        for (std::size_t i = 0; i < 31; ++i) {
            std::int32_t input = std::int32_t(0xffffffff) << i;
            std::int32_t observed = avel::countl_sign(input);
            std::int32_t expected = (31 - i);

            EXPECT_EQ(observed, expected);
        }
    }

}

#endif //AVEL_SCALAR32I_TESTS_HPP
