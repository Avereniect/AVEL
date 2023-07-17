#ifndef AVEL_VEC32X16I_TESTS_HPP
#define AVEL_VEC32X16I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=========================================================================
    // mask32x16i tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask32x16i, Construct_from_primitive) {
        mask32x16i mask{mask32x16i::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask32x16i, Construct_from_bool) {
        mask32x16i mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask32x16i mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask32x16i::width);
    }

    TEST(Mask32x16i, Construct_from_array) {
        arr32xb false_array{};
        std::fill_n(false_array.data(), mask32x16i::width, false);
        mask32x16i mask0{false_array};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);


        arr32xb true_array{};
        std::fill_n(true_array.data(), mask32x16i::width, true);
        mask32x16i mask1{true_array};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask32x16i::width);
    }

    TEST(Mask32x16i, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32xb>();
            std::uint32_t cnt = std::count(inputs.begin(), inputs.end(), true);

            auto m = mask32x16i{inputs};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == mask32x16i::width, all(m));
            EXPECT_EQ(cnt != 0, any(m));
            EXPECT_EQ(cnt == 0, none(m));
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask32x16i, Assign_bool) {
        mask32x16i mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask32x16i mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(count(mask1) == mask32x16i::width);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask32x16i, Equality_comparison) {
        mask32x16i mask0{false};
        mask32x16i mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask32x16i mask2{true};
        mask32x16i mask3{true};
        EXPECT_EQ(mask2, mask3);

        EXPECT_FALSE(mask0 == mask2);
        EXPECT_FALSE(mask3 == mask1);
    }

    TEST(Mask32x16i, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32xb>();
            inputs1[0] = !inputs0[0];

            mask32x16i mask0{inputs0};
            mask32x16i mask1{inputs0};
            mask32x16i mask2{inputs1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask32x16i, Inequality_comparison) {
        mask32x16i mask0{false};
        mask32x16i mask1{false};
        EXPECT_FALSE(mask0 != mask1);

        mask32x16i mask2{true};
        mask32x16i mask3{true};
        EXPECT_FALSE(mask2 != mask3);

        EXPECT_TRUE(mask0 != mask2);
        EXPECT_TRUE(mask3 != mask1);
    }

    TEST(Mask32x16i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32xb>();
            inputs1[0] = !inputs0[0];

            mask32x16i mask0{inputs0};
            mask32x16i mask1{inputs1};
            mask32x16i mask2{inputs1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask32x16i, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32xb>();

            mask32x16i mask0{inputs0};
            mask32x16i mask1{inputs1};

            mask32x16i results = mask0;
            results &= mask1;

            arr32xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask32x16i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask32x16i, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32xb>();

            mask32x16i mask0{inputs0};
            mask32x16i mask1{inputs1};

            mask32x16i results = mask0;
            results |= mask1;

            arr32xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask32x16i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask32x16i, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32xb>();

            mask32x16i mask0{inputs0};
            mask32x16i mask1{inputs1};

            mask32x16i results = mask0;
            results ^= mask1;

            arr32xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask32x16i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask32x16i, Logical_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();

            mask32x16i mask0{inputs0};

            mask32x16i results = !mask0;

            arr32xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = !inputs0[j];
            }
            mask32x16i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask32x16i, Bitwise_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32xb>();

            mask32x16i mask0{inputs0};
            mask32x16i mask1{inputs1};

            mask32x16i results = mask0 & mask1;

            arr32xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask32x16i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask32x16i, Logical_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32xb>();

            mask32x16i mask0{inputs0};
            mask32x16i mask1{inputs1};

            mask32x16i results = mask0 && mask1;

            arr32xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] && inputs1[j];
            }
            mask32x16i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask32x16i, Bitwise_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32xb>();

            mask32x16i mask0{inputs0};
            mask32x16i mask1{inputs1};

            mask32x16i results = mask0 | mask1;

            arr32xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask32x16i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask32x16i, Logical_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32xb>();

            mask32x16i mask0{inputs0};
            mask32x16i mask1{inputs1};

            mask32x16i results = mask0 || mask1;

            arr32xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] || inputs1[j];
            }
            mask32x16i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask32x16i, Bitwise_xor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32xb>();

            mask32x16i mask0{inputs0};
            mask32x16i mask1{inputs1};

            mask32x16i results = mask0 ^ mask1;

            arr32xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask32x16i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=========================================================================
    // Vec32x16i tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec32x16i, Construct_vector_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32xb>();

            mask32x16i m0{inputs};
            vec32x16i v0{m0};

            auto results = to_array(v0);
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    TEST(Vec32x16i, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto x = random_val<vec32x16i::scalar>();
            vec32x16i results{x};

            arr32x16i arr;
            std::fill_n(arr.data(), vec32x16i::width, x);
            vec32x16i expected{arr};

            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec32x16i, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto data = random_array<arr32x16i>();

            vec32x16i v{data};

            auto results = to_array(v);

            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(data[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec32x16i, Assign_primitive) {
        vec32x16i::primitive x{};
        vec32x16i v{0x0F};
        v = x;
        vec32x16i expected{0x00};

        EXPECT_TRUE(all(v == expected));
    }

    TEST(Vec32x16i, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto scalar = random_val<vec32x16i::scalar>();
            vec32x16i v;
            v = scalar;

            auto results = to_array(v);
            for (std::size_t j = 0; j < results.size(); ++j) {
                EXPECT_EQ(results[j], scalar);
            }
        }
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Vec32x16i, Equality_comparison) {
        mask32x16i mask0{false};
        mask32x16i mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask32x16i mask2{true};
        mask32x16i mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec32x16i, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();
            inputs1[0] = ~inputs0[0];

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs0};
            vec32x16i v2{inputs1};

            EXPECT_TRUE(all(v0 == v1));
            EXPECT_FALSE(all(v1 == v2));
        }
    }

    TEST(Vec32x16i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();
            inputs1[0] = ~inputs0[0];

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs0};
            vec32x16i v2{inputs1};

            EXPECT_FALSE(any(v0 != v1));
            EXPECT_TRUE(any(v1 != v2));
        }
    }

    TEST(Vec32x16i, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = (v0 < v1);

            arr32xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] < inputs1[j];
            }

            EXPECT_EQ(results, mask32x16i{expected});
        }
    }

    TEST(Vec32x16i, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = (v0 <= v1);

            arr32xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] <= inputs1[j];
            }

            EXPECT_EQ(results, mask32x16i{expected});
        }
    }

    TEST(Vec32x16i, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = (v0 > v1);

            arr32xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] > inputs1[j];
            }

            EXPECT_EQ(results, mask32x16i{expected});
        }
    }

    TEST(Vec32x16i, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = (v0 >= v1);

            arr32xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] >= inputs1[j];
            }

            EXPECT_EQ(results, mask32x16i{expected});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec32x16i, Unary_plus) {
        auto inputs = random_array<arr32x16i>();
        const vec32x16i u{inputs};
        vec32x16i v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec32x16i, Unary_minus) {
        auto inputs = random_array<arr32x16i>();
        const vec32x16i input{inputs};

        auto results = -input;

        arr32x16i expected{};
        for (std::size_t j = 0; j < expected.size(); ++j) {
            expected[j] = -inputs[j];
        }

        EXPECT_TRUE(all(results == vec32x16i{expected}));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec32x16i, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0;
            results += v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0;
            results -= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0;
            results *= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_denominator_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0;
            results /= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_denominator_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0;
            results %= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec32x16i, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0 + v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0 - v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0 * v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_denominator_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0 / v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_denominator_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0 % v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec32x16i, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i input{inputs};
            ++input;

            auto results = input;
            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i input{inputs};
            input++;

            auto results = input;
            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i input{inputs};
            --input;

            auto results = input;
            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i input{inputs};
            input--;

            auto results = input;
            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec32x16i, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0;
            results &= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0;
            results |= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0;
            results ^= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            auto v1 = random_shift<vec32x16i::scalar>();

            auto results = v0;
            results <<= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec32x16i::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_shift_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0;
            results <<= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec32x16i::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            auto v1 = random_shift<vec32x16i::scalar>();

            auto results = v0;
            results >>= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec32x16i::scalar>::value) {
                    if (inputs0[j] < 0) {
                        expected[j] = vec32x16i::scalar(-1);
                    } else {
                        expected[j] = 0x00;
                    }
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_shift_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0;
            results >>= v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec32x16i::scalar>::value) {
                    if (inputs0[j] < 0) {
                        expected[j] = vec32x16i::scalar(-1);
                    } else {
                        expected[j] = 0x00;
                    }
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec32x16i, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v0{inputs};

            auto results = ~v0;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ~inputs[j];
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            auto v1 = random_shift<vec32x16i::scalar>();

            auto results = v0 << v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec32x16i::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_shift_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0 << v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec32x16i::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            auto v1 = random_shift<vec32x16i::scalar>();

            auto results = v0 >> v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec32x16i::scalar>::value) {
                    if (inputs0[j] < 0) {
                        expected[j] = vec32x16i::scalar(-1);
                    } else {
                        expected[j] = 0x00;
                    }
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_shift_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = v0 >> v1;

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec32x16i::scalar>::value) {
                    if (inputs0[j] < 0) {
                        expected[j] = vec32x16i::scalar(-1);
                    } else {
                        expected[j] = 0x00;
                    }
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec32x16i, Conversion_to_mask) {
        vec32x16i vec{0};

        auto a = vec.operator mask32x16i();
        mask32x16i b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec32x16i, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = mask32x16i{v};

            arr32xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = inputs[j];
            }

            EXPECT_TRUE(results == mask32x16i{expected});
        }
    }

    //=====================================================
    // Arrangement Operations
    //=====================================================

    TEST(Vec32x16i, Extract_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();
            vec32x16i v{inputs};

            EXPECT_EQ(inputs[0x00], extract<0x00>(v));
            EXPECT_EQ(inputs[0x01], extract<0x01>(v));
            EXPECT_EQ(inputs[0x02], extract<0x02>(v));
            EXPECT_EQ(inputs[0x03], extract<0x03>(v));
            EXPECT_EQ(inputs[0x04], extract<0x04>(v));
            EXPECT_EQ(inputs[0x05], extract<0x05>(v));
            EXPECT_EQ(inputs[0x06], extract<0x06>(v));
            EXPECT_EQ(inputs[0x07], extract<0x07>(v));
            EXPECT_EQ(inputs[0x08], extract<0x08>(v));
            EXPECT_EQ(inputs[0x09], extract<0x09>(v));
            EXPECT_EQ(inputs[0x0a], extract<0x0a>(v));
            EXPECT_EQ(inputs[0x0b], extract<0x0b>(v));
            EXPECT_EQ(inputs[0x0c], extract<0x0c>(v));
            EXPECT_EQ(inputs[0x0d], extract<0x0d>(v));
            EXPECT_EQ(inputs[0x0e], extract<0x0e>(v));
            EXPECT_EQ(inputs[0x0f], extract<0x0f>(v));
            EXPECT_EQ(inputs[0x10], extract<0x10>(v));
            EXPECT_EQ(inputs[0x11], extract<0x11>(v));
            EXPECT_EQ(inputs[0x12], extract<0x12>(v));
            EXPECT_EQ(inputs[0x13], extract<0x13>(v));
            EXPECT_EQ(inputs[0x14], extract<0x14>(v));
            EXPECT_EQ(inputs[0x15], extract<0x15>(v));
            EXPECT_EQ(inputs[0x16], extract<0x16>(v));
            EXPECT_EQ(inputs[0x17], extract<0x17>(v));
            EXPECT_EQ(inputs[0x18], extract<0x18>(v));
            EXPECT_EQ(inputs[0x19], extract<0x19>(v));
            EXPECT_EQ(inputs[0x1a], extract<0x1a>(v));
            EXPECT_EQ(inputs[0x1b], extract<0x1b>(v));
            EXPECT_EQ(inputs[0x1c], extract<0x1c>(v));
            EXPECT_EQ(inputs[0x1d], extract<0x1d>(v));
            EXPECT_EQ(inputs[0x1e], extract<0x1e>(v));
            EXPECT_EQ(inputs[0x1f], extract<0x1f>(v));

        }
    }

    TEST(Vec32x16i, Insert_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();
            vec32x16i v{random_val<vec32x16i::scalar>()};

            v = insert<0x00>(v, inputs[0x00]);
            v = insert<0x01>(v, inputs[0x01]);
            v = insert<0x02>(v, inputs[0x02]);
            v = insert<0x03>(v, inputs[0x03]);
            v = insert<0x04>(v, inputs[0x04]);
            v = insert<0x05>(v, inputs[0x05]);
            v = insert<0x06>(v, inputs[0x06]);
            v = insert<0x07>(v, inputs[0x07]);
            v = insert<0x08>(v, inputs[0x08]);
            v = insert<0x09>(v, inputs[0x09]);
            v = insert<0x0a>(v, inputs[0x0a]);
            v = insert<0x0b>(v, inputs[0x0b]);
            v = insert<0x0c>(v, inputs[0x0c]);
            v = insert<0x0d>(v, inputs[0x0d]);
            v = insert<0x0e>(v, inputs[0x0e]);
            v = insert<0x0f>(v, inputs[0x0f]);
            v = insert<0x10>(v, inputs[0x10]);
            v = insert<0x11>(v, inputs[0x11]);
            v = insert<0x12>(v, inputs[0x12]);
            v = insert<0x13>(v, inputs[0x13]);
            v = insert<0x14>(v, inputs[0x14]);
            v = insert<0x15>(v, inputs[0x15]);
            v = insert<0x16>(v, inputs[0x16]);
            v = insert<0x17>(v, inputs[0x17]);
            v = insert<0x18>(v, inputs[0x18]);
            v = insert<0x19>(v, inputs[0x19]);
            v = insert<0x1a>(v, inputs[0x1a]);
            v = insert<0x1b>(v, inputs[0x1b]);
            v = insert<0x1c>(v, inputs[0x1c]);
            v = insert<0x1d>(v, inputs[0x1d]);
            v = insert<0x1e>(v, inputs[0x1e]);
            v = insert<0x1f>(v, inputs[0x1f]);


            EXPECT_TRUE(all(v == vec32x16i{inputs}));
        }
    }

    //=====================================================
    // Bit Manipulation Instructions
    //=====================================================

    TEST(Vec32x16i, Bit_shift_left) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            EXPECT_TRUE(all(bit_shift_left<0x00>(v) == (v << 0x00)));
            EXPECT_TRUE(all(bit_shift_left<0x01>(v) == (v << 0x01)));
            EXPECT_TRUE(all(bit_shift_left<0x02>(v) == (v << 0x02)));
            EXPECT_TRUE(all(bit_shift_left<0x03>(v) == (v << 0x03)));
            EXPECT_TRUE(all(bit_shift_left<0x04>(v) == (v << 0x04)));
            EXPECT_TRUE(all(bit_shift_left<0x05>(v) == (v << 0x05)));
            EXPECT_TRUE(all(bit_shift_left<0x06>(v) == (v << 0x06)));
            EXPECT_TRUE(all(bit_shift_left<0x07>(v) == (v << 0x07)));
            EXPECT_TRUE(all(bit_shift_left<0x08>(v) == (v << 0x08)));
            EXPECT_TRUE(all(bit_shift_left<0x09>(v) == (v << 0x09)));
            EXPECT_TRUE(all(bit_shift_left<0x0a>(v) == (v << 0x0a)));
            EXPECT_TRUE(all(bit_shift_left<0x0b>(v) == (v << 0x0b)));
            EXPECT_TRUE(all(bit_shift_left<0x0c>(v) == (v << 0x0c)));
            EXPECT_TRUE(all(bit_shift_left<0x0d>(v) == (v << 0x0d)));
            EXPECT_TRUE(all(bit_shift_left<0x0e>(v) == (v << 0x0e)));
            EXPECT_TRUE(all(bit_shift_left<0x0f>(v) == (v << 0x0f)));
            EXPECT_TRUE(all(bit_shift_left<0x10>(v) == (v << 0x10)));

        }
    }

    TEST(Vec32x16i, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            EXPECT_TRUE(all(bit_shift_right<0x00>(v) == (v >> 0x00)));
            EXPECT_TRUE(all(bit_shift_right<0x01>(v) == (v >> 0x01)));
            EXPECT_TRUE(all(bit_shift_right<0x02>(v) == (v >> 0x02)));
            EXPECT_TRUE(all(bit_shift_right<0x03>(v) == (v >> 0x03)));
            EXPECT_TRUE(all(bit_shift_right<0x04>(v) == (v >> 0x04)));
            EXPECT_TRUE(all(bit_shift_right<0x05>(v) == (v >> 0x05)));
            EXPECT_TRUE(all(bit_shift_right<0x06>(v) == (v >> 0x06)));
            EXPECT_TRUE(all(bit_shift_right<0x07>(v) == (v >> 0x07)));
            EXPECT_TRUE(all(bit_shift_right<0x08>(v) == (v >> 0x08)));
            EXPECT_TRUE(all(bit_shift_right<0x09>(v) == (v >> 0x09)));
            EXPECT_TRUE(all(bit_shift_right<0x0a>(v) == (v >> 0x0a)));
            EXPECT_TRUE(all(bit_shift_right<0x0b>(v) == (v >> 0x0b)));
            EXPECT_TRUE(all(bit_shift_right<0x0c>(v) == (v >> 0x0c)));
            EXPECT_TRUE(all(bit_shift_right<0x0d>(v) == (v >> 0x0d)));
            EXPECT_TRUE(all(bit_shift_right<0x0e>(v) == (v >> 0x0e)));
            EXPECT_TRUE(all(bit_shift_right<0x0f>(v) == (v >> 0x0f)));
            EXPECT_TRUE(all(bit_shift_right<0x10>(v) == (v >> 0x10)));

        }
    }

    TEST(Vec32x16i, Rotl) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            EXPECT_TRUE(all(rotl<0x00>(v) == (rotl(v, 0x00))));
            EXPECT_TRUE(all(rotl<0x01>(v) == (rotl(v, 0x01))));
            EXPECT_TRUE(all(rotl<0x02>(v) == (rotl(v, 0x02))));
            EXPECT_TRUE(all(rotl<0x03>(v) == (rotl(v, 0x03))));
            EXPECT_TRUE(all(rotl<0x04>(v) == (rotl(v, 0x04))));
            EXPECT_TRUE(all(rotl<0x05>(v) == (rotl(v, 0x05))));
            EXPECT_TRUE(all(rotl<0x06>(v) == (rotl(v, 0x06))));
            EXPECT_TRUE(all(rotl<0x07>(v) == (rotl(v, 0x07))));
            EXPECT_TRUE(all(rotl<0x08>(v) == (rotl(v, 0x08))));
            EXPECT_TRUE(all(rotl<0x09>(v) == (rotl(v, 0x09))));
            EXPECT_TRUE(all(rotl<0x0a>(v) == (rotl(v, 0x0a))));
            EXPECT_TRUE(all(rotl<0x0b>(v) == (rotl(v, 0x0b))));
            EXPECT_TRUE(all(rotl<0x0c>(v) == (rotl(v, 0x0c))));
            EXPECT_TRUE(all(rotl<0x0d>(v) == (rotl(v, 0x0d))));
            EXPECT_TRUE(all(rotl<0x0e>(v) == (rotl(v, 0x0e))));
            EXPECT_TRUE(all(rotl<0x0f>(v) == (rotl(v, 0x0f))));
            EXPECT_TRUE(all(rotl<0x10>(v) == (rotl(v, 0x10))));

        }
    }

    TEST(Vec32x16i, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            auto v1 = random64u();

            auto results = rotl(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = rotl(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Rotr) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();

            vec32x16i v{inputs0};

            EXPECT_TRUE(all(rotr<0x00>(v) == (rotr(v, 0x00))));
            EXPECT_TRUE(all(rotr<0x01>(v) == (rotr(v, 0x01))));
            EXPECT_TRUE(all(rotr<0x02>(v) == (rotr(v, 0x02))));
            EXPECT_TRUE(all(rotr<0x03>(v) == (rotr(v, 0x03))));
            EXPECT_TRUE(all(rotr<0x04>(v) == (rotr(v, 0x04))));
            EXPECT_TRUE(all(rotr<0x05>(v) == (rotr(v, 0x05))));
            EXPECT_TRUE(all(rotr<0x06>(v) == (rotr(v, 0x06))));
            EXPECT_TRUE(all(rotr<0x07>(v) == (rotr(v, 0x07))));
            EXPECT_TRUE(all(rotr<0x08>(v) == (rotr(v, 0x08))));
            EXPECT_TRUE(all(rotr<0x09>(v) == (rotr(v, 0x09))));
            EXPECT_TRUE(all(rotr<0x0a>(v) == (rotr(v, 0x0a))));
            EXPECT_TRUE(all(rotr<0x0b>(v) == (rotr(v, 0x0b))));
            EXPECT_TRUE(all(rotr<0x0c>(v) == (rotr(v, 0x0c))));
            EXPECT_TRUE(all(rotr<0x0d>(v) == (rotr(v, 0x0d))));
            EXPECT_TRUE(all(rotr<0x0e>(v) == (rotr(v, 0x0e))));
            EXPECT_TRUE(all(rotr<0x0f>(v) == (rotr(v, 0x0f))));
            EXPECT_TRUE(all(rotr<0x10>(v) == (rotr(v, 0x10))));

        }
    }

    TEST(Vec32x16i, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            auto v1 = random64u();

            auto results = rotr(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = rotr(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec32x16i, Count_random) {
        auto predicate = [] (vec32x16i::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec32x16i v{inputs};

            auto results = count(v);

            EXPECT_EQ(cnt, results);
        }
    }

    TEST(Vec32x16i, Any_edge_cases) {
        EXPECT_FALSE(any(vec32x16i{0x00}));
        EXPECT_TRUE(any(vec32x16i{0x01}));
    }

    TEST(Vec32x16i, Any_random) {
        auto predicate = [] (vec32x16i::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec32x16i v{inputs};

            EXPECT_EQ(cnt != 0, any(v));
        }
    }

    TEST(Vec32x16i, All_edge_cases) {
        EXPECT_FALSE(all(vec32x16i{0x00}));
        EXPECT_TRUE(all(vec32x16i{0x01}));
    }

    TEST(Vec32x16i, All_random) {
        auto predicate = [] (vec32x16i::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec32x16i v{inputs};

            auto results = all(v);

            EXPECT_EQ(cnt == vec32x16i::width, results);
        }
    }

    TEST(Vec32x16i, None_edge_cases) {
        EXPECT_TRUE(none(vec32x16i{0x00}));
        EXPECT_FALSE(none(vec32x16i{0x01}));
    }

    TEST(Vec32x16i, None_random) {
        auto predicate = [] (vec32x16i::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec32x16i v{inputs};

            EXPECT_EQ(cnt == 0, none(v));
        }
    }

    TEST(Vec32x16i, Set_bits_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();

            mask32x16i v0{inputs0};

            auto results = set_bits(v0);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs0[j]) {
                    expected[j] = -1;
                } else {
                    expected[j] = 0x00;
                }
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Keep_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32x16i>();

            mask32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = keep(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = keep(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Clear_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32x16i>();

            mask32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = clear(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = clear(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32x16i>();
            auto inputs2 = random_array<arr32x16i>();

            mask32x16i v0{inputs0};
            vec32x16i v1{inputs1};
            vec32x16i v2{inputs2};

            auto results = blend(v0, v1, v2);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = blend(inputs0[j], inputs1[j], inputs2[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = max(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = std::max(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = min(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = std::min(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = minmax(v0, v1);

            arr32x16i expected0{};
            arr32x16i expected1{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto xy = minmax(inputs0[j], inputs1[j]);
                expected0[j] = xy[0];
                expected1[j] = xy[1];
            }

            EXPECT_TRUE(all(results[0] == vec32x16i{expected0}));
            EXPECT_TRUE(all(results[1] == vec32x16i{expected1}));
        }
    }

    TEST(Vec32x16i, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();
            auto inputs2 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};
            vec32x16i input2{inputs2};

            auto xy = minmax(v1, input2);

            auto results = clamp(v0, xy[0], xy[1]);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto xy = minmax(inputs1[j], inputs2[j]);
                expected[j] = clamp(inputs0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = average(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = average(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32x16i>();
            auto inputs1 = random_array<arr32x16i>();

            vec32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = midpoint(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = midpoint(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Negate_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr32xb>();
            auto inputs1 = random_array<arr32x16i>();

            mask32x16i v0{inputs0};
            vec32x16i v1{inputs1};

            auto results = negate(v0, v1);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = negate(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = abs(v);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::abs(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = neg_abs(v);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = neg_abs(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec32x16i, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            for (std::uint32_t j = 0; j < vec32x16i::width; ++j) {
                auto results = load<vec32x16i>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec32x16i::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec32x16i, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            EXPECT_TRUE(all(load<vec32x16i, 0x00>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(load<vec32x16i, 0x01>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(load<vec32x16i, 0x02>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x02)));
            EXPECT_TRUE(all(load<vec32x16i, 0x03>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x03)));
            EXPECT_TRUE(all(load<vec32x16i, 0x04>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x04)));
            EXPECT_TRUE(all(load<vec32x16i, 0x05>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x05)));
            EXPECT_TRUE(all(load<vec32x16i, 0x06>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x06)));
            EXPECT_TRUE(all(load<vec32x16i, 0x07>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x07)));
            EXPECT_TRUE(all(load<vec32x16i, 0x08>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x08)));
            EXPECT_TRUE(all(load<vec32x16i, 0x09>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x09)));
            EXPECT_TRUE(all(load<vec32x16i, 0x0a>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x0a)));
            EXPECT_TRUE(all(load<vec32x16i, 0x0b>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x0b)));
            EXPECT_TRUE(all(load<vec32x16i, 0x0c>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x0c)));
            EXPECT_TRUE(all(load<vec32x16i, 0x0d>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x0d)));
            EXPECT_TRUE(all(load<vec32x16i, 0x0e>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x0e)));
            EXPECT_TRUE(all(load<vec32x16i, 0x0f>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x0f)));
            EXPECT_TRUE(all(load<vec32x16i, 0x10>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x10)));
            EXPECT_TRUE(all(load<vec32x16i, 0x11>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x11)));
            EXPECT_TRUE(all(load<vec32x16i, 0x12>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x12)));
            EXPECT_TRUE(all(load<vec32x16i, 0x13>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x13)));
            EXPECT_TRUE(all(load<vec32x16i, 0x14>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x14)));
            EXPECT_TRUE(all(load<vec32x16i, 0x15>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x15)));
            EXPECT_TRUE(all(load<vec32x16i, 0x16>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x16)));
            EXPECT_TRUE(all(load<vec32x16i, 0x17>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x17)));
            EXPECT_TRUE(all(load<vec32x16i, 0x18>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x18)));
            EXPECT_TRUE(all(load<vec32x16i, 0x19>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x19)));
            EXPECT_TRUE(all(load<vec32x16i, 0x1a>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x1a)));
            EXPECT_TRUE(all(load<vec32x16i, 0x1b>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x1b)));
            EXPECT_TRUE(all(load<vec32x16i, 0x1c>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x1c)));
            EXPECT_TRUE(all(load<vec32x16i, 0x1d>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x1d)));
            EXPECT_TRUE(all(load<vec32x16i, 0x1e>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x1e)));
            EXPECT_TRUE(all(load<vec32x16i, 0x1f>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x1f)));
            EXPECT_TRUE(all(load<vec32x16i, 0x20>(inputs.data()) == load<vec32x16i>(inputs.data(), 0x20)));

        }
    }

    TEST(Vec32x16i, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec32x16i)) auto inputs = random_array<arr32x16i>();

            for (std::uint32_t j = 0; j < vec32x16i::width; ++j) {
                auto results = aligned_load<vec32x16i>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec32x16i::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec32x16i, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec32x16i)) auto inputs = random_array<arr32x16i>();

            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x00>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x01>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x02>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x02)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x03>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x03)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x04>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x04)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x05>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x05)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x06>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x06)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x07>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x07)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x08>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x08)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x09>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x09)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x0a>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x0a)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x0b>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x0b)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x0c>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x0c)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x0d>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x0d)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x0e>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x0e)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x0f>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x0f)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x10>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x10)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x11>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x11)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x12>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x12)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x13>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x13)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x14>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x14)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x15>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x15)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x16>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x16)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x17>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x17)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x18>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x18)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x19>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x19)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x1a>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x1a)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x1b>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x1b)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x1c>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x1c)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x1d>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x1d)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x1e>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x1e)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x1f>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x1f)));
            EXPECT_TRUE(all(aligned_load<vec32x16i, 0x20>(inputs.data()) == aligned_load<vec32x16i>(inputs.data(), 0x20)));

        }
    }

    TEST(Vec32x16i, Store_n) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            for (std::size_t j = 0; j < vec32x16i::width; ++j) {
                vec32x16i::scalar arr[vec32x16i::width]{};
                store(arr, v, j);

                for (std::size_t k = 0; k < inputs.size(); ++k) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], arr[k]);
                    } else {
                        EXPECT_EQ(0x00, arr[k]);
                    }
                }
            }
        }
    }

    TEST(Vec32x16i, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            vec32x16i::scalar arr[vec32x16i::width]{};

            store<0x00>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x00));
            store<0x01>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x01));
            store<0x02>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x02));
            store<0x03>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x03));
            store<0x04>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x04));
            store<0x05>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x05));
            store<0x06>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x06));
            store<0x07>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x07));
            store<0x08>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x08));
            store<0x09>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x09));
            store<0x0a>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0a));
            store<0x0b>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0b));
            store<0x0c>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0c));
            store<0x0d>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0d));
            store<0x0e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0e));
            store<0x0f>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0f));
            store<0x10>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x10));
            store<0x11>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x11));
            store<0x12>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x12));
            store<0x13>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x13));
            store<0x14>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x14));
            store<0x15>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x15));
            store<0x16>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x16));
            store<0x17>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x17));
            store<0x18>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x18));
            store<0x19>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x19));
            store<0x1a>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1a));
            store<0x1b>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1b));
            store<0x1c>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1c));
            store<0x1d>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1d));
            store<0x1e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1e));
            store<0x1f>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1f));
            store<0x20>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x20));

        }
    }

    TEST(Vec32x16i, Aligned_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            for (std::size_t j = 0; j < vec32x16i::width; ++j) {
                alignas(alignof(vec32x16i)) vec32x16i::scalar arr[vec32x16i::width]{};
                aligned_store(arr, v, j);

                for (std::size_t k = 0; k < inputs.size(); ++k) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], arr[k]);
                    } else {
                        EXPECT_EQ(0x00, arr[k]);
                    }
                }
            }
        }
    }

    TEST(Vec32x16i, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            alignas(alignof(vec32x16i)) vec32x16i::scalar arr[vec32x16i::width]{};

            aligned_store<0x00>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x00));
            aligned_store<0x01>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x01));
            aligned_store<0x02>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x02));
            aligned_store<0x03>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x03));
            aligned_store<0x04>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x04));
            aligned_store<0x05>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x05));
            aligned_store<0x06>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x06));
            aligned_store<0x07>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x07));
            aligned_store<0x08>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x08));
            aligned_store<0x09>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x09));
            aligned_store<0x0a>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0a));
            aligned_store<0x0b>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0b));
            aligned_store<0x0c>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0c));
            aligned_store<0x0d>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0d));
            aligned_store<0x0e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0e));
            aligned_store<0x0f>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0f));
            aligned_store<0x10>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x10));
            aligned_store<0x11>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x11));
            aligned_store<0x12>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x12));
            aligned_store<0x13>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x13));
            aligned_store<0x14>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x14));
            aligned_store<0x15>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x15));
            aligned_store<0x16>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x16));
            aligned_store<0x17>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x17));
            aligned_store<0x18>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x18));
            aligned_store<0x19>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x19));
            aligned_store<0x1a>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1a));
            aligned_store<0x1b>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1b));
            aligned_store<0x1c>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1c));
            aligned_store<0x1d>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1d));
            aligned_store<0x1e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1e));
            aligned_store<0x1f>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x1f));
            aligned_store<0x20>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x20));

        }
    }

    TEST(Vec32x16i, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec32x16i, Div_uniform) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto numerator = random_val<vec32x16i::scalar>();
            auto denominator = random_denominator<vec32x16i::scalar>();

            vec32x16i::scalar quotient  = (numerator / denominator);
            vec32x16i::scalar remainder = (numerator % denominator);

            vec32x16i n{numerator};
            vec32x16i d{denominator};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            constexpr auto min_value = std::numeric_limits<vec32x16i::scalar>::min();
            if (min_value == numerator && -1 == denominator) {
                continue; // The output in this case is unspecified
            }

            for (std::size_t j = 0; j < q.size(); ++j) {
                EXPECT_EQ(q[j], quotient);
                EXPECT_EQ(r[j], remainder);
            }
        }
    }

    TEST(Vec32x16i, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            auto numerators = random_array<arr32x16i>();
            auto denominators = random_denominator_array<arr32x16i>();

            arr32x16i quotients{};
            arr32x16i remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec32x16i n{numerators};
            vec32x16i d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            constexpr auto min_value = std::numeric_limits<vec32x16i::scalar>::min();
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                if (min_value == numerators[j] && -1 == denominators[j]) {
                    continue; // The output in this case is unspecified
                }

                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec32x16i, Popcount_edge_cases) {
        vec32x16i v{0x0};
        vec32x16i c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec32x16i, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = popcount(v);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = popcount(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = byteswap(v);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = byteswap(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Countl_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec32x16i::scalar>::value - 1); ++i) {
            vec32x16i::scalar v = (vec32x16i::scalar(0x1) << i) - 1;
            EXPECT_TRUE(all(countl_zero(vec32x16i{v}) == vec32x16i(type_width<vec32x16i::scalar>::value - i)));
        }
        EXPECT_TRUE(all(countl_zero(vec32x16i(-1)) == vec32x16i{0}));
    }

    TEST(Vec32x16i, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = countl_zero(v);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Countl_one_edge_cases) {
        EXPECT_TRUE(all(countl_one(vec32x16i{0x00}) == vec32x16i{0}));
        for (std::size_t i = 0; i < (type_width<vec32x16i::scalar>::value - 1); ++i) {
            vec32x16i::scalar v = vec32x16i::scalar(-1) << i;
            EXPECT_TRUE(all(countl_one(vec32x16i{v}) == vec32x16i(type_width<vec32x16i::scalar>::value - i)));
        }
    }

    TEST(Vec32x16i, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = countl_one(v);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Countr_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec32x16i::scalar>::value - 1); ++i) {
            vec32x16i::scalar v = vec32x16i::scalar(-1) << i;
            EXPECT_TRUE(all(countr_zero(vec32x16i{v}) == vec32x16i(i)));
        }
        EXPECT_TRUE(all(countr_zero(vec32x16i{0x00}) == vec32x16i{type_width<vec32x16i::scalar>::value}));
    }

    TEST(Vec32x16i, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = countr_zero(v);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Countr_one_edge_cases) {
        EXPECT_TRUE(all(countr_one(vec32x16i{0x00}) == vec32x16i{0}));

        for (std::size_t i = 0; i < (type_width<vec32x16i::scalar>::value - 1); ++i) {
            vec32x16i::scalar v = (vec32x16i::scalar(1) << i) - 1;
            EXPECT_TRUE(all(countr_one(vec32x16i{v}) == vec32x16i(i)));
        }
        EXPECT_TRUE(all(countr_one(vec32x16i{vec32x16i::scalar(-1)}) == vec32x16i{type_width<vec32x16i::scalar>::value}));
    }

    TEST(Vec32x16i, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = countr_one(v);

            arr32x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec32x16i{expected}));
        }
    }

    TEST(Vec32x16i, Has_single_bit_edge_cases) {
        EXPECT_TRUE(none(has_single_bit(vec32x16i{0x00})));

        for (std::size_t i = 0; i < (type_width<vec32x16i::scalar>::value - 1); ++i) {
            vec32x16i::scalar v = vec32x16i::scalar(1) << i;
            EXPECT_TRUE(all(has_single_bit(vec32x16i{v})));
        }
    }

    TEST(Vec32x16i, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr32x16i>();

            vec32x16i v{inputs};

            auto results = has_single_bit(v);

            arr32xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = has_single_bit(inputs[j]);
            }

            EXPECT_TRUE(results == mask32x16i{expected});
        }
    }

}

#endif
