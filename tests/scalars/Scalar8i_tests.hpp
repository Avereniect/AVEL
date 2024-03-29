#ifndef AVEL_SCALAR8I_TESTS_HPP
#define AVEL_SCALAR8I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // Comparisons
    //=====================================================

    TEST(Scalar8i, cmp_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_equal(x, y);

            if (x >= 0x80 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar8i, cmp_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_equal(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar8i, cmp_not_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar8i, cmp_not_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar8i, cmp_less_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_less(x, y);

            if (x >= 0x80 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar8i, cmp_less_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_less(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar8i, Cmp_le_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_less_equal(x, y);

            if (x >= 0x80 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar8i, Cmp_le_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_less_equal(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar8i, cmp_greater_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_greater(x, y);

            if (x >= 0x80 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar8i, cmp_greater_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_greater(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar8i, cmp_greater_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_greater_equal(x, y);

            if (x >= 0x80 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    TEST(Scalar8i, cmp_greater_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_greater_equal(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    //=====================================================
    // Integer operations
    //=====================================================

    TEST(Scalar8i, countl_sign_powers_of_two) {
        for (std::size_t i = 0; i < 7; ++i) {
            std::int8_t input = std::int8_t(0x01) << (6 - i);
            std::int8_t observed = avel::countl_sign(input);
            std::int8_t expected = i;

            EXPECT_EQ(observed, expected);
        }

        {
            std::int8_t input = 0x00;
            std::int8_t observed = avel::countl_sign(input);
            std::int8_t expected = 7;

            EXPECT_EQ(observed, expected);
        }

        for (std::size_t i = 0; i < 7; ++i) {
            std::int8_t input = std::int8_t(0xff) << i;
            std::int8_t observed = avel::countl_sign(input);
            std::int8_t expected = (7 - i);

            EXPECT_EQ(observed, expected);
        }
    }

    TEST(Scalar8i, countl_sign_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t x = random_val<std::int8_t>();
            std::int8_t y = random_val<std::uint8_t>();


        }
    }

}

#endif //AVEL_SCALAR8I_TESTS_HPP
