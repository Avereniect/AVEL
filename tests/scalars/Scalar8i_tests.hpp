#ifndef AVEL_SCALAR8I_TESTS_HPP
#define AVEL_SCALAR8I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

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

}

#endif //AVEL_SCALAR8I_TESTS_HPP
