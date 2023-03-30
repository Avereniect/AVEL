#ifndef AVEL_VEC4X32I_TESTS_HPP
#define AVEL_VEC4X32I_TESTS_HPP

namespace avel_tests {
    
    using namespace avel;
    
    //=========================================================================
    // mask4x32i tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask4x32i, Construct_from_primitive) {
        mask4x32i mask{mask4x32i::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask4x32i, Construct_from_bool) {
        mask4x32i mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask4x32i mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask4x32i::width);
    }

    TEST(Mask4x32i, Construct_from_array) {
        arr4xb false_array{};
        std::fill_n(false_array.data(), mask4x32i::width, false);
        mask4x32i mask0{false_array};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);


        arr4xb true_array{};
        std::fill_n(true_array.data(), mask4x32i::width, true);
        mask4x32i mask1{true_array};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask4x32i::width);
    }

    TEST(Mask4x32i, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4xb>();
            std::uint32_t cnt = std::count(inputs.begin(), inputs.end(), true);

            auto m = mask4x32i{inputs};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == mask4x32i::width, all(m));
            EXPECT_EQ(cnt != 0, any(m));
            EXPECT_EQ(cnt == 0, none(m));
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask4x32i, Assign_bool) {
        mask4x32i mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask4x32i mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(count(mask1) == mask4x32i::width);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask4x32i, Equality_comparison) {
        mask4x32i mask0{false};
        mask4x32i mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask4x32i mask2{true};
        mask4x32i mask3{true};
        EXPECT_EQ(mask2, mask3);

        EXPECT_FALSE(mask0 == mask2);
        EXPECT_FALSE(mask3 == mask1);
    }

    TEST(Mask4x32i, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4xb>();
            inputs1[0] = !inputs0[0];

            mask4x32i mask0{inputs0};
            mask4x32i mask1{inputs0};
            mask4x32i mask2{inputs1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask4x32i, Inequality_comparison) {
        mask4x32i mask0{false};
        mask4x32i mask1{false};
        EXPECT_FALSE(mask0 != mask1);

        mask4x32i mask2{true};
        mask4x32i mask3{true};
        EXPECT_FALSE(mask2 != mask3);

        EXPECT_TRUE(mask0 != mask2);
        EXPECT_TRUE(mask3 != mask1);
    }

    TEST(Mask4x32i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4xb>();
            inputs1[0] = !inputs0[0];

            mask4x32i mask0{inputs0};
            mask4x32i mask1{inputs1};
            mask4x32i mask2{inputs1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask4x32i, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4xb>();

            mask4x32i mask0{inputs0};
            mask4x32i mask1{inputs1};

            mask4x32i results = mask0;
            results &= mask1;

            arr4xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask4x32i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask4x32i, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4xb>();

            mask4x32i mask0{inputs0};
            mask4x32i mask1{inputs1};

            mask4x32i results = mask0;
            results |= mask1;

            arr4xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask4x32i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask4x32i, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4xb>();

            mask4x32i mask0{inputs0};
            mask4x32i mask1{inputs1};

            mask4x32i results = mask0;
            results ^= mask1;

            arr4xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask4x32i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask4x32i, Logical_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();

            mask4x32i mask0{inputs0};

            mask4x32i results = !mask0;

            arr4xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = !inputs0[j];
            }
            mask4x32i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask4x32i, Bitwise_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4xb>();

            mask4x32i mask0{inputs0};
            mask4x32i mask1{inputs1};

            mask4x32i results = mask0 & mask1;

            arr4xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask4x32i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask4x32i, Logical_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4xb>();

            mask4x32i mask0{inputs0};
            mask4x32i mask1{inputs1};

            mask4x32i results = mask0 && mask1;

            arr4xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] && inputs1[j];
            }
            mask4x32i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask4x32i, Bitwise_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4xb>();

            mask4x32i mask0{inputs0};
            mask4x32i mask1{inputs1};

            mask4x32i results = mask0 | mask1;

            arr4xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask4x32i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask4x32i, Logical_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4xb>();

            mask4x32i mask0{inputs0};
            mask4x32i mask1{inputs1};

            mask4x32i results = mask0 || mask1;

            arr4xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] || inputs1[j];
            }
            mask4x32i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask4x32i, Bitwise_xor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4xb>();

            mask4x32i mask0{inputs0};
            mask4x32i mask1{inputs1};

            mask4x32i results = mask0 ^ mask1;

            arr4xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask4x32i expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=========================================================================
    // Vec4x32i tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec4x32i, Construct_vector_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4xb>();

            mask4x32i m0{inputs};
            vec4x32i v0{m0};

            auto results = to_array(v0);
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    TEST(Vec4x32i, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto x = random_val<vec4x32i::scalar>();
            vec4x32i results{x};

            arr4x32i arr;
            std::fill_n(arr.data(), vec4x32i::width, x);
            vec4x32i expected{arr};

            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec4x32i, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto data = random_array<arr4x32i>();

            vec4x32i v{data};

            auto results = to_array(v);

            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(data[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec4x32i, Assign_primitive) {
        vec4x32i::primitive x{};
        vec4x32i v{0x0F};
        v = x;
        vec4x32i expected{0x00};

        EXPECT_TRUE(all(v == expected));
    }

    TEST(Vec4x32i, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto scalar = random_val<vec4x32i::scalar>();
            vec4x32i v;
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

    TEST(Vec4x32i, Equality_comparison) {
        mask4x32i mask0{false};
        mask4x32i mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask4x32i mask2{true};
        mask4x32i mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec4x32i, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();
            inputs1[0] = ~inputs0[0];

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs0};
            vec4x32i v2{inputs1};

            EXPECT_TRUE(all(v0 == v1));
            EXPECT_FALSE(all(v1 == v2));
        }
    }

    TEST(Vec4x32i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();
            inputs1[0] = ~inputs0[0];

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs0};
            vec4x32i v2{inputs1};

            EXPECT_FALSE(any(v0 != v1));
            EXPECT_TRUE(any(v1 != v2));
        }
    }

    TEST(Vec4x32i, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = (v0 < v1);

            arr4xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] < inputs1[j];
            }

            EXPECT_EQ(results, mask4x32i{expected});
        }
    }

    TEST(Vec4x32i, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = (v0 <= v1);

            arr4xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] <= inputs1[j];
            }

            EXPECT_EQ(results, mask4x32i{expected});
        }
    }

    TEST(Vec4x32i, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = (v0 > v1);

            arr4xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] > inputs1[j];
            }

            EXPECT_EQ(results, mask4x32i{expected});
        }
    }

    TEST(Vec4x32i, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = (v0 >= v1);

            arr4xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] >= inputs1[j];
            }

            EXPECT_EQ(results, mask4x32i{expected});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec4x32i, Unary_plus) {
        auto inputs = random_array<arr4x32i>();
        const vec4x32i u{inputs};
        vec4x32i v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec4x32i, Unary_minus) {
        auto inputs = random_array<arr4x32i>();
        const vec4x32i input{inputs};

        auto results = -input;

        arr4x32i expected{};
        for (std::size_t j = 0; j < expected.size(); ++j) {
            expected[j] = -inputs[j];
        }

        EXPECT_TRUE(all(results == vec4x32i{expected}));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec4x32i, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0;
            results += v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0;
            results -= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0;
            results *= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_denominator_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0;
            results /= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_denominator_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0;
            results %= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec4x32i, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0 + v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0 - v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0 * v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_denominator_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0 / v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_denominator_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0 % v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec4x32i, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i input{inputs};
            ++input;

            auto results = input;
            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i input{inputs};
            input++;

            auto results = input;
            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i input{inputs};
            --input;

            auto results = input;
            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i input{inputs};
            input--;

            auto results = input;
            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec4x32i, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0;
            results &= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0;
            results |= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0;
            results ^= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            auto v1 = random_shift<vec4x32i::scalar>();

            auto results = v0;
            results <<= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec4x32i::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_shift_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0;
            results <<= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec4x32i::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            auto v1 = random_shift<vec4x32i::scalar>();

            auto results = v0;
            results >>= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec4x32i::scalar>::value) {
                    if (inputs0[j] < 0) {
                        expected[j] = vec4x32i::scalar(-1);
                    } else {
                        expected[j] = 0x00;
                    }
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_shift_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0;
            results >>= v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec4x32i::scalar>::value) {
                    if (inputs0[j] < 0) {
                        expected[j] = vec4x32i::scalar(-1);
                    } else {
                        expected[j] = 0x00;
                    }
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec4x32i, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v0{inputs};

            auto results = ~v0;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ~inputs[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            auto v1 = random_shift<vec4x32i::scalar>();

            auto results = v0 << v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec4x32i::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_shift_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0 << v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec4x32i::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            auto v1 = random_shift<vec4x32i::scalar>();

            auto results = v0 >> v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec4x32i::scalar>::value) {
                    if (inputs0[j] < 0) {
                        expected[j] = vec4x32i::scalar(-1);
                    } else {
                        expected[j] = 0x00;
                    }
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_shift_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = v0 >> v1;

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec4x32i::scalar>::value) {
                    if (inputs0[j] < 0) {
                        expected[j] = vec4x32i::scalar(-1);
                    } else {
                        expected[j] = 0x00;
                    }
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec4x32i, Conversion_to_mask) {
        vec4x32i vec{0};

        auto a = vec.operator mask4x32i();
        mask4x32i b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec4x32i, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = mask4x32i{v};

            arr4xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = inputs[j];
            }

            EXPECT_TRUE(results == mask4x32i{expected});
        }
    }

    //=====================================================
    // Arrangement Operations
    //=====================================================

    TEST(Vec4x32i, Extract_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();
            vec4x32i v{inputs};

            EXPECT_EQ(inputs[0x0], extract<0x0>(v));
            EXPECT_EQ(inputs[0x1], extract<0x1>(v));
            EXPECT_EQ(inputs[0x2], extract<0x2>(v));
            EXPECT_EQ(inputs[0x3], extract<0x3>(v));
        }
    }

    TEST(Vec4x32i, Insert_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();
            vec4x32i v{0x00};

            v = insert<0x0>(v, inputs[0x0]);
            v = insert<0x1>(v, inputs[0x1]);
            v = insert<0x2>(v, inputs[0x2]);
            v = insert<0x3>(v, inputs[0x3]);

            EXPECT_TRUE(all(v == vec4x32i{inputs}));
        }
    }

    //=====================================================
    // Bit Manipulation Instructions
    //=====================================================

    TEST(Vec4x32i, Bit_shift_left) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

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

    TEST(Vec4x32i, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

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

    TEST(Vec4x32i, Rotl) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            EXPECT_TRUE(all(rotl<0x0>(v) == (rotl(v, 0x0))));
            EXPECT_TRUE(all(rotl<0x1>(v) == (rotl(v, 0x1))));
            EXPECT_TRUE(all(rotl<0x2>(v) == (rotl(v, 0x2))));
            EXPECT_TRUE(all(rotl<0x3>(v) == (rotl(v, 0x3))));
            EXPECT_TRUE(all(rotl<0x4>(v) == (rotl(v, 0x4))));
            EXPECT_TRUE(all(rotl<0x5>(v) == (rotl(v, 0x5))));
            EXPECT_TRUE(all(rotl<0x6>(v) == (rotl(v, 0x6))));
            EXPECT_TRUE(all(rotl<0x7>(v) == (rotl(v, 0x7))));
            EXPECT_TRUE(all(rotl<0x8>(v) == (rotl(v, 0x8))));
            EXPECT_TRUE(all(rotl<0x9>(v) == (rotl(v, 0x9))));
            EXPECT_TRUE(all(rotl<0xa>(v) == (rotl(v, 0xa))));
            EXPECT_TRUE(all(rotl<0xb>(v) == (rotl(v, 0xb))));
            EXPECT_TRUE(all(rotl<0xc>(v) == (rotl(v, 0xc))));
            EXPECT_TRUE(all(rotl<0xd>(v) == (rotl(v, 0xd))));
            EXPECT_TRUE(all(rotl<0xe>(v) == (rotl(v, 0xe))));
            EXPECT_TRUE(all(rotl<0xf>(v) == (rotl(v, 0xf))));
        }
    }

    TEST(Vec4x32i, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            auto v1 = random64u();

            auto results = rotl(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = rotl(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Rotr) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();

            vec4x32i v{inputs0};

            EXPECT_TRUE(all(rotr<0x0>(v) == (rotr(v, 0x0))));
            EXPECT_TRUE(all(rotr<0x1>(v) == (rotr(v, 0x1))));
            EXPECT_TRUE(all(rotr<0x2>(v) == (rotr(v, 0x2))));
            EXPECT_TRUE(all(rotr<0x3>(v) == (rotr(v, 0x3))));
            EXPECT_TRUE(all(rotr<0x4>(v) == (rotr(v, 0x4))));
            EXPECT_TRUE(all(rotr<0x5>(v) == (rotr(v, 0x5))));
            EXPECT_TRUE(all(rotr<0x6>(v) == (rotr(v, 0x6))));
            EXPECT_TRUE(all(rotr<0x7>(v) == (rotr(v, 0x7))));
            EXPECT_TRUE(all(rotr<0x8>(v) == (rotr(v, 0x8))));
            EXPECT_TRUE(all(rotr<0x9>(v) == (rotr(v, 0x9))));
            EXPECT_TRUE(all(rotr<0xa>(v) == (rotr(v, 0xa))));
            EXPECT_TRUE(all(rotr<0xb>(v) == (rotr(v, 0xb))));
            EXPECT_TRUE(all(rotr<0xc>(v) == (rotr(v, 0xc))));
            EXPECT_TRUE(all(rotr<0xd>(v) == (rotr(v, 0xd))));
            EXPECT_TRUE(all(rotr<0xe>(v) == (rotr(v, 0xe))));
            EXPECT_TRUE(all(rotr<0xf>(v) == (rotr(v, 0xf))));
        }
    }

    TEST(Vec4x32i, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            auto v1 = random64u();

            auto results = rotr(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = rotr(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec4x32i, Count_random) {
        auto predicate = [] (vec4x32i::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec4x32i v{inputs};

            auto results = count(v);

            EXPECT_EQ(cnt, results);
        }
    }

    TEST(Vec4x32i, Any_edge_cases) {
        EXPECT_FALSE(any(vec4x32i{0x00}));
        EXPECT_TRUE(any(vec4x32i{0x01}));
    }

    TEST(Vec4x32i, Any_random) {
        auto predicate = [] (vec4x32i::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec4x32i v{inputs};

            EXPECT_EQ(cnt != 0, any(v));
        }
    }

    TEST(Vec4x32i, All_edge_cases) {
        EXPECT_FALSE(all(vec4x32i{0x00}));
        EXPECT_TRUE(all(vec4x32i{0x01}));
    }

    TEST(Vec4x32i, All_random) {
        auto predicate = [] (vec4x32i::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec4x32i v{inputs};

            auto results = all(v);

            EXPECT_EQ(cnt == vec4x32i::width, results);
        }
    }

    TEST(Vec4x32i, None_edge_cases) {
        EXPECT_TRUE(none(vec4x32i{0x00}));
        EXPECT_FALSE(none(vec4x32i{0x01}));
    }

    TEST(Vec4x32i, None_random) {
        auto predicate = [] (vec4x32i::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec4x32i v{inputs};

            EXPECT_EQ(cnt == 0, none(v));
        }
    }

    TEST(Vec4x32i, Broadcast_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();

            mask4x32i v0{inputs0};

            auto results = broadcast_mask(v0);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs0[j]) {
                    expected[j] = -1;
                } else {
                    expected[j] = 0x00;
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Keep_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4x32i>();

            mask4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = keep(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = keep(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Clear_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4x32i>();

            mask4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = clear(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = clear(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4x32i>();
            auto inputs2 = random_array<arr4x32i>();

            mask4x32i v0{inputs0};
            vec4x32i v1{inputs1};
            vec4x32i v2{inputs2};

            auto results = blend(v0, v1, v2);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = blend(inputs0[j], inputs1[j], inputs2[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = max(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = std::max(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = min(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = std::min(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = minmax(v0, v1);

            arr4x32i expected0{};
            arr4x32i expected1{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto xy = minmax(inputs0[j], inputs1[j]);
                expected0[j] = xy[0];
                expected1[j] = xy[1];
            }

            EXPECT_TRUE(all(results[0] == vec4x32i{expected0}));
            EXPECT_TRUE(all(results[1] == vec4x32i{expected1}));
        }
    }

    TEST(Vec4x32i, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();
            auto inputs2 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};
            vec4x32i input2{inputs2};

            auto xy = minmax(v1, input2);

            auto results = clamp(v0, xy[0], xy[1]);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto xy = minmax(inputs1[j], inputs2[j]);
                expected[j] = clamp(inputs0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = average(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = average(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4x32i>();
            auto inputs1 = random_array<arr4x32i>();

            vec4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = midpoint(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = midpoint(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Negate_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr4xb>();
            auto inputs1 = random_array<arr4x32i>();

            mask4x32i v0{inputs0};
            vec4x32i v1{inputs1};

            auto results = negate(v0, v1);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = negate(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = abs(v);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::abs(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = neg_abs(v);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = neg_abs(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec4x32i, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            for (std::uint32_t j = 0; j < vec4x32i::width; ++j) {
                auto results = load<vec4x32i>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec4x32i::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec4x32i, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            EXPECT_TRUE(all(load<vec4x32i, 0x00>(inputs.data()) == load<vec4x32i>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(load<vec4x32i, 0x01>(inputs.data()) == load<vec4x32i>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(load<vec4x32i, 0x02>(inputs.data()) == load<vec4x32i>(inputs.data(), 0x02)));
            EXPECT_TRUE(all(load<vec4x32i, 0x03>(inputs.data()) == load<vec4x32i>(inputs.data(), 0x03)));
            EXPECT_TRUE(all(load<vec4x32i, 0x04>(inputs.data()) == load<vec4x32i>(inputs.data(), 0x04)));
        }
    }

    TEST(Vec4x32i, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec4x32i)) auto inputs = random_array<arr4x32i>();

            for (std::uint32_t j = 0; j < vec4x32i::width; ++j) {
                auto results = aligned_load<vec4x32i>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec4x32i::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec4x32i, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec4x32i)) auto inputs = random_array<arr4x32i>();

            EXPECT_TRUE(all(aligned_load<vec4x32i, 0x00>(inputs.data()) == aligned_load<vec4x32i>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec4x32i, 0x01>(inputs.data()) == aligned_load<vec4x32i>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(aligned_load<vec4x32i, 0x02>(inputs.data()) == aligned_load<vec4x32i>(inputs.data(), 0x02)));
            EXPECT_TRUE(all(aligned_load<vec4x32i, 0x03>(inputs.data()) == aligned_load<vec4x32i>(inputs.data(), 0x03)));
            EXPECT_TRUE(all(aligned_load<vec4x32i, 0x04>(inputs.data()) == aligned_load<vec4x32i>(inputs.data(), 0x04)));
        }
    }

    TEST(Vec4x32i, Gather_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;

        std::vector<vec4x32i::scalar> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random_val<vec4x32i::scalar>();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = random_val<vec4x32u::scalar>() % test_data_size;
            }
            auto indices = load<vec4x32i>(index_array.data());

            for (std::size_t j = 0; j <= vec4x32i::width; ++j) {
                auto results = gather<vec4x32i>(test_data.data(), indices, j);

                auto loaded_data = to_array(results);
                for (std::size_t k = 0; k < vec4x32i::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(test_data[index_array[k]], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec4x32i, Gather_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;

        std::vector<vec4x32i::scalar> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random_val<vec4x32i::scalar>();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i index_array{};
            for (std::size_t j = 0; j < vec4x32i::width; ++j) {
                index_array[j] = random_val<vec4x32i::scalar>() % test_data_size;
            }
            auto indices = load<vec4x32i>(index_array.data());

            EXPECT_TRUE(all(gather<vec4x32i, 0x00>(test_data.data(), indices) == gather<vec4x32i>(test_data.data(), indices, 0x00)));
            EXPECT_TRUE(all(gather<vec4x32i, 0x01>(test_data.data(), indices) == gather<vec4x32i>(test_data.data(), indices, 0x01)));
            EXPECT_TRUE(all(gather<vec4x32i, 0x02>(test_data.data(), indices) == gather<vec4x32i>(test_data.data(), indices, 0x02)));
            EXPECT_TRUE(all(gather<vec4x32i, 0x03>(test_data.data(), indices) == gather<vec4x32i>(test_data.data(), indices, 0x03)));
            EXPECT_TRUE(all(gather<vec4x32i, 0x04>(test_data.data(), indices) == gather<vec4x32i>(test_data.data(), indices, 0x04)));
        }
    }

    TEST(Vec4x32i, Store_n) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            for (std::size_t j = 0; j < vec4x32i::width; ++j) {
                vec4x32i::scalar arr[vec4x32i::width]{};
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

    TEST(Vec4x32i, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            vec4x32i::scalar arr[vec4x32i::width]{};

            store<0x00>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x00));
            store<0x01>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x01));
            store<0x02>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x02));
            store<0x03>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x03));
            store<0x04>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x04));
        }
    }

    TEST(Vec4x32i, Aligned_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            for (std::size_t j = 0; j < vec4x32i::width; ++j) {
                alignas(alignof(vec4x32i)) vec4x32i::scalar arr[vec4x32i::width]{};
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

    TEST(Vec4x32i, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            alignas(alignof(vec4x32i)) vec4x32i::scalar arr[vec4x32i::width]{};

            aligned_store<0x00>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x00));
            aligned_store<0x01>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x01));
            aligned_store<0x02>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x02));
            aligned_store<0x03>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x03));
            aligned_store<0x04>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x04));
        }
    }

    TEST(Vec4x32i, Scatter_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;
        static constexpr std::size_t block_size = test_data_size / vec4x32i::width;

        std::vector<vec4x32i::scalar> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr4x32i>();

            vec4x32i input0{input_array0};

            arr4x32i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random_val<vec4x32u::scalar>() % block_size) + (j * block_size);
            }

            vec4x32i indices{index_array};

            for (std::size_t j = 0; j <= vec4x32u::width; ++j) {
                scatter(test_data.data(), input0, indices, j);

                for (std::size_t k = 0; k < input_array0.size(); ++k) {
                    if (k < j) {
                        EXPECT_EQ(input_array0[k], test_data[index_array[k]]);
                    }
                }
            }
        }
    }

    TEST(Vec4x32i, Scatter_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;
        static constexpr std::size_t block_size = test_data_size / vec4x32i::width;

        std::vector<vec4x32i::scalar> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr4x32i>();

            vec4x32i input0{input_array0};

            arr4x32i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random_val<vec4x32u::scalar>() % block_size) + (j * block_size);
            }

            vec4x32i indices{index_array};

            scatter<0x0>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x00));
            scatter<0x1>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x01));
            scatter<0x2>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x02));
            scatter<0x3>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x03));
            scatter<0x4>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x04));
        }
    }

    TEST(Vec4x32i, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec4x32i, Div_uniform) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto numerator = random_val<vec4x32i::scalar>();
            auto denominator = random_denominator<vec4x32i::scalar>();

            auto quotient  = (numerator / denominator);
            auto remainder = (numerator % denominator);

            vec4x32i n{numerator};
            vec4x32i d{denominator};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < q.size(); ++j) {
                EXPECT_EQ(q[j], quotient);
                EXPECT_EQ(r[j], remainder);
            }
        }
    }

    TEST(Vec4x32i, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            auto numerators = random_array<arr4x32i>();
            auto denominators = random_denominator_array<arr4x32i>();

            arr4x32i quotients{};
            arr4x32i remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec4x32i n{numerators};
            vec4x32i d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec4x32i, Popcount) {
        vec4x32i v{0x0};
        vec4x32i c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec4x32i, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = popcount(v);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = popcount(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = byteswap(v);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = byteswap(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Countl_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec4x32i::scalar>::value - 1); ++i) {
            vec4x32i::scalar v = (vec4x32i::scalar(0x1) << i) - 1;
            EXPECT_TRUE(all(countl_zero(vec4x32i{v}) == vec4x32i(type_width<vec4x32i::scalar>::value - i)));
        }
        EXPECT_TRUE(all(countl_zero(vec4x32i(-1)) == vec4x32i{0}));
    }

    TEST(Vec4x32i, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = countl_zero(v);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Countl_one_edge_cases) {
        EXPECT_TRUE(all(countl_one(vec4x32i{0x00}) == vec4x32i{0}));
        for (std::size_t i = 0; i < (type_width<vec4x32i::scalar>::value - 1); ++i) {
            vec4x32i::scalar v = vec4x32i::scalar(-1) << i;
            EXPECT_TRUE(all(countl_one(vec4x32i{v}) == vec4x32i(type_width<vec4x32i::scalar>::value - i)));
        }
    }

    TEST(Vec4x32i, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = countl_one(v);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Countr_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec4x32i::scalar>::value - 1); ++i) {
            vec4x32i::scalar v = vec4x32i::scalar(-1) << i;
            EXPECT_TRUE(all(countr_zero(vec4x32i{v}) == vec4x32i(i)));
        }
        EXPECT_TRUE(all(countr_zero(vec4x32i{0x00}) == vec4x32i{type_width<vec4x32i::scalar>::value}));
    }

    TEST(Vec4x32i, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = countr_zero(v);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Countr_one_edge_cases) {
        EXPECT_TRUE(all(countr_one(vec4x32i{0x00}) == vec4x32i{0}));

        for (std::size_t i = 0; i < (type_width<vec4x32i::scalar>::value - 1); ++i) {
            vec4x32i::scalar v = (vec4x32i::scalar(1) << i) - 1;
            EXPECT_TRUE(all(countr_one(vec4x32i{v}) == vec4x32i(i)));
        }
        EXPECT_TRUE(all(countr_one(vec4x32i{vec4x32i::scalar(-1)}) == vec4x32i{type_width<vec4x32i::scalar>::value}));
    }

    TEST(Vec4x32i, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = countr_one(v);

            arr4x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{expected}));
        }
    }

    TEST(Vec4x32i, Has_single_bit_edge_cases) {
        EXPECT_TRUE(none(has_single_bit(vec4x32i{0x00})));

        for (std::size_t i = 0; i < (type_width<vec4x32i::scalar>::value - 1); ++i) {
            vec4x32i::scalar v = vec4x32i::scalar(1) << i;
            EXPECT_TRUE(all(has_single_bit(vec4x32i{v})));
        }
    }

    TEST(Vec4x32i, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr4x32i>();

            vec4x32i v{inputs};

            auto results = has_single_bit(v);

            arr4xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = has_single_bit(inputs[j]);
            }

            EXPECT_TRUE(results == mask4x32i{expected});
        }
    }
    
}

#endif
