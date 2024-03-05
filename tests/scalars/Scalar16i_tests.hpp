#ifndef AVEL_SCALAR16I_TESTS_HPP
#define AVEL_SCALAR16I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // Comparisons
    //=====================================================

    TEST(Scalar16i, cmp_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_equal(x, y);

            if (x >= 0x8000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar16i, cmp_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_equal(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar16i, cmp_not_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar16i, cmp_not_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar16i, cmp_less_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_less(x, y);

            if (x >= 0x8000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar16i, cmp_less_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_less(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar16i, Cmp_le_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_less_equal(x, y);

            if (x >= 0x8000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar16i, Cmp_le_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_less_equal(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar16i, cmp_greater_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_greater(x, y);

            if (x >= 0x8000 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar16i, cmp_greater_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_greater(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar16i, cmp_greater_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_greater_equal(x, y);

            if (x >= 0x8000 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    TEST(Scalar16i, cmp_greater_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_greater_equal(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    //=====================================================
    // Integer operations
    //=====================================================

    TEST(Scalar16i, countl_sign_powers_of_two) {
        for (std::size_t i = 0; i < 15; ++i) {
            std::int16_t input = std::int16_t(0x0001) << (14 - i);
            std::int16_t observed = avel::countl_sign(input);
            std::int16_t expected = i;

            EXPECT_EQ(observed, expected);
        }

        {
            std::int16_t input = 0x0000;
            std::int16_t observed = avel::countl_sign(input);
            std::int16_t expected = 15;

            EXPECT_EQ(observed, expected);
        }

        for (std::size_t i = 0; i < 15; ++i) {
            std::int16_t input = std::int8_t(0xffff) << i;
            std::int16_t observed = avel::countl_sign(input);
            std::int16_t expected = (15 - i);

            EXPECT_EQ(observed, expected);
        }
    }

}

#endif //AVEL_SCALAR16I_TESTS_HPP
