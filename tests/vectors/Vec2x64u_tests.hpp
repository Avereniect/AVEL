#ifndef AVEL_VEC2X64U_TESTS_HPP
#define AVEL_VEC2X64U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=========================================================================
    // mask2x64u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask2x64u, Construct_from_primitive) {
        mask2x64u mask{mask2x64u::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask2x64u, Construct_from_bool) {
        mask2x64u mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask2x64u mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask2x64u::width);
    }

    TEST(Mask2x64u, Construct_from_array) {
        arr2xb false_array{};
        std::fill_n(false_array.data(), mask2x64u::width, false);
        mask2x64u mask0{false_array};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);


        arr2xb true_array{};
        std::fill_n(true_array.data(), mask2x64u::width, true);
        mask2x64u mask1{true_array};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask2x64u::width);
    }

    TEST(Mask2x64u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();
            std::uint32_t cnt = std::count(inputs.begin(), inputs.end(), true);

            auto m = mask2x64u{inputs};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == mask2x64u::width, all(m));
            EXPECT_EQ(cnt != 0, any(m));
            EXPECT_EQ(cnt == 0, none(m));
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask2x64u, Assign_bool) {
        mask2x64u mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask2x64u mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(count(mask1) == mask2x64u::width);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask2x64u, Equality_comparison) {
        mask2x64u mask0{false};
        mask2x64u mask1{false};
        EXPECT_TRUE(mask0 == mask1);

        mask2x64u mask2{true};
        mask2x64u mask3{true};
        EXPECT_TRUE(mask2 == mask3);

        EXPECT_FALSE(mask0 == mask2);
        EXPECT_FALSE(mask3 == mask1);
    }

    TEST(Mask2x64u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();
            inputs1[0] = !inputs0[0];

            mask2x64u mask0{inputs0};
            mask2x64u mask1{inputs0};
            mask2x64u mask2{inputs1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask2x64u, Inequality_comparison) {
        mask2x64u mask0{false};
        mask2x64u mask1{false};
        EXPECT_FALSE(mask0 != mask1);

        mask2x64u mask2{true};
        mask2x64u mask3{true};
        EXPECT_FALSE(mask2 != mask3);

        EXPECT_TRUE(mask0 != mask2);
        EXPECT_TRUE(mask3 != mask1);
    }

    TEST(Mask2x64u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();
            inputs1[0] = !inputs0[0];

            mask2x64u mask0{inputs0};
            mask2x64u mask1{inputs1};
            mask2x64u mask2{inputs1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask2x64u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64u mask0{inputs0};
            mask2x64u mask1{inputs1};

            mask2x64u results = mask0;
            results &= mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask2x64u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64u mask0{inputs0};
            mask2x64u mask1{inputs1};

            mask2x64u results = mask0;
            results |= mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask2x64u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64u mask0{inputs0};
            mask2x64u mask1{inputs1};

            mask2x64u results = mask0;
            results ^= mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask2x64u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask2x64u, Logical_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();

            mask2x64u mask0{inputs};

            mask2x64u results = !mask0;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = !inputs[j];
            }
            mask2x64u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64u, Bitwise_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64u mask0{inputs0};
            mask2x64u mask1{inputs1};

            mask2x64u results = mask0 & mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask2x64u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64u, Logical_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64u mask0{inputs0};
            mask2x64u mask1{inputs1};

            mask2x64u results = mask0 && mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] && inputs1[j];
            }
            mask2x64u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64u, Bitwise_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64u mask0{inputs0};
            mask2x64u mask1{inputs1};

            mask2x64u results = mask0 | mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask2x64u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64u, Logical_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64u mask0{inputs0};
            mask2x64u mask1{inputs1};

            mask2x64u results = mask0 || mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] || inputs1[j];
            }
            mask2x64u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64u, Bitwise_xor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64u mask0{inputs0};
            mask2x64u mask1{inputs1};

            mask2x64u results = mask0 ^ mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask2x64u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=========================================================================
    // Vec2x64u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec2x64u, Construct_vector_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();

            mask2x64u m0{inputs};
            vec2x64u v0{m0};

            auto results = to_array(v0);
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    TEST(Vec2x64u, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto x = random_val<vec2x64u::scalar>();
            vec2x64u results{x};

            arr2x64u arr;
            std::fill_n(arr.data(), vec2x64u::width, x);
            vec2x64u expected{arr};

            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec2x64u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(inputs[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec2x64u, Assign_primitive) {
        vec2x64u::primitive x{};
        vec2x64u v{0x0F};
        v = x;
        vec2x64u expected{};

        EXPECT_TRUE(all(v == expected));
    }

    TEST(Vec2x64u, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto scalar = random_val<vec2x64u::scalar>();
            vec2x64u v;
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

    TEST(Vec2x64u, Equality_comparison) {
        mask2x64u mask0{false};
        mask2x64u mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask2x64u mask2{true};
        mask2x64u mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec2x64u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();
            inputs1[0] = ~inputs0[0];

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs0};
            vec2x64u v2{inputs1};

            EXPECT_TRUE(all(v0 == v1));
            EXPECT_FALSE(all(v1 == v2));
        }
    }

    TEST(Vec2x64u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();
            inputs1[0] = ~inputs0[0];

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs0};
            vec2x64u v2{inputs1};

            EXPECT_FALSE(any(v0 != v1));
            EXPECT_TRUE(any(v1 != v2));
        }
    }

    TEST(Vec2x64u, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = (v0 < v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] < inputs1[j];
            }

            EXPECT_EQ(results, mask2x64u{expected});
        }
    }

    TEST(Vec2x64u, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = (v0 <= v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] <= inputs1[j];
            }

            EXPECT_EQ(results, mask2x64u{expected});
        }
    }

    TEST(Vec2x64u, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = (v0 > v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] > inputs1[j];
            }

            EXPECT_EQ(results, mask2x64u{expected});
        }
    }

    TEST(Vec2x64u, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = (v0 >= v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] >= inputs1[j];
            }

            EXPECT_EQ(results, mask2x64u{expected});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec2x64u, Unary_plus) {
        auto inputs = random_array<arr2x64u>();
        const vec2x64u v{inputs};
        vec2x64u results{+v};

        EXPECT_TRUE(all(results == v));
    }

    TEST(Vec2x64u, Unary_minus) {
        auto inputs = random_array<arr2x64u>();
        const vec2x64u v{inputs};

        auto results = -v;

        arr2x64i expected{};
        for (std::size_t j = 0; j < expected.size(); ++j) {
            expected[j] = -inputs[j];
        }

        EXPECT_TRUE(all(results == vec2x64i{expected}));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec2x64u, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0;
            results += v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0;
            results -= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0;
            results *= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_denominator_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0;
            results /= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_denominator_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0;
            results %= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec2x64u, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0 + v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0 - v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0 * v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_denominator_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0 / v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_denominator_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0 % v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec2x64u, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};
            ++v;

            auto results = v;
            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};
            v++;

            auto results = v;
            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};
            --v;

            auto results = v;
            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};
            v--;

            auto results = v;
            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec2x64u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0;
            results &= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0;
            results |= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0;
            results ^= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            auto v1 = random_shift<vec2x64u::scalar>();

            auto results = v0;
            results <<= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec2x64u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_shift_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0;
            results <<= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec2x64u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            auto v1 = random_shift<vec2x64u::scalar>();

            auto results = v0;
            results >>= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec2x64u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_shift_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0;
            results >>= v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec2x64u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec2x64u, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = ~v;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ~inputs[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            auto v1 = random_shift<vec2x64u::scalar>();

            auto results = v0 << v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec2x64u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_shift_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0 << v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec2x64u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            auto v1 = random_shift<vec2x64u::scalar>();

            auto results = v0 >> v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec2x64u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_shift_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = v0 >> v1;

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec2x64u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec2x64u, Conversion_to_mask) {
        vec2x64u vec{0};

        auto a = vec.operator mask2x64u();
        mask2x64u b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec2x64u, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = mask2x64u{v};

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = inputs[j];
            }

            EXPECT_TRUE(results == mask2x64u{expected});
        }
    }

    //=====================================================
    // Arrangement Operations
    //=====================================================

    TEST(Vec2x64u, Extract_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();
            vec2x64u v{inputs};

            EXPECT_EQ(inputs[0x0], extract<0x0>(v));
            EXPECT_EQ(inputs[0x1], extract<0x1>(v));
        }
    }

    TEST(Vec2x64u, Insert_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();
            vec2x64u v{random_val<vec2x64u::scalar>()};

            v = insert<0x0>(v, inputs[0x0]);
            v = insert<0x1>(v, inputs[0x1]);

            EXPECT_TRUE(all(v == vec2x64u{inputs}));
        }
    }

    //=====================================================
    // Bit Manipulation Instructions
    //=====================================================

    TEST(Vec2x64u, Bit_shift_left) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

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
            EXPECT_TRUE(all(bit_shift_left<0x11>(v) == (v << 0x11)));
            EXPECT_TRUE(all(bit_shift_left<0x12>(v) == (v << 0x12)));
            EXPECT_TRUE(all(bit_shift_left<0x13>(v) == (v << 0x13)));
            EXPECT_TRUE(all(bit_shift_left<0x14>(v) == (v << 0x14)));
            EXPECT_TRUE(all(bit_shift_left<0x15>(v) == (v << 0x15)));
            EXPECT_TRUE(all(bit_shift_left<0x16>(v) == (v << 0x16)));
            EXPECT_TRUE(all(bit_shift_left<0x17>(v) == (v << 0x17)));
            EXPECT_TRUE(all(bit_shift_left<0x18>(v) == (v << 0x18)));
            EXPECT_TRUE(all(bit_shift_left<0x19>(v) == (v << 0x19)));
            EXPECT_TRUE(all(bit_shift_left<0x1a>(v) == (v << 0x1a)));
            EXPECT_TRUE(all(bit_shift_left<0x1b>(v) == (v << 0x1b)));
            EXPECT_TRUE(all(bit_shift_left<0x1c>(v) == (v << 0x1c)));
            EXPECT_TRUE(all(bit_shift_left<0x1d>(v) == (v << 0x1d)));
            EXPECT_TRUE(all(bit_shift_left<0x1e>(v) == (v << 0x1e)));
            EXPECT_TRUE(all(bit_shift_left<0x1f>(v) == (v << 0x1f)));

            EXPECT_TRUE(all(bit_shift_left<0x20>(v) == (v << 0x20)));
            EXPECT_TRUE(all(bit_shift_left<0x21>(v) == (v << 0x21)));
            EXPECT_TRUE(all(bit_shift_left<0x22>(v) == (v << 0x22)));
            EXPECT_TRUE(all(bit_shift_left<0x23>(v) == (v << 0x23)));
            EXPECT_TRUE(all(bit_shift_left<0x24>(v) == (v << 0x24)));
            EXPECT_TRUE(all(bit_shift_left<0x25>(v) == (v << 0x25)));
            EXPECT_TRUE(all(bit_shift_left<0x26>(v) == (v << 0x26)));
            EXPECT_TRUE(all(bit_shift_left<0x27>(v) == (v << 0x27)));
            EXPECT_TRUE(all(bit_shift_left<0x28>(v) == (v << 0x28)));
            EXPECT_TRUE(all(bit_shift_left<0x29>(v) == (v << 0x29)));
            EXPECT_TRUE(all(bit_shift_left<0x2a>(v) == (v << 0x2a)));
            EXPECT_TRUE(all(bit_shift_left<0x2b>(v) == (v << 0x2b)));
            EXPECT_TRUE(all(bit_shift_left<0x2c>(v) == (v << 0x2c)));
            EXPECT_TRUE(all(bit_shift_left<0x2d>(v) == (v << 0x2d)));
            EXPECT_TRUE(all(bit_shift_left<0x2e>(v) == (v << 0x2e)));
            EXPECT_TRUE(all(bit_shift_left<0x2f>(v) == (v << 0x2f)));

            EXPECT_TRUE(all(bit_shift_left<0x30>(v) == (v << 0x30)));
            EXPECT_TRUE(all(bit_shift_left<0x31>(v) == (v << 0x31)));
            EXPECT_TRUE(all(bit_shift_left<0x32>(v) == (v << 0x32)));
            EXPECT_TRUE(all(bit_shift_left<0x33>(v) == (v << 0x33)));
            EXPECT_TRUE(all(bit_shift_left<0x34>(v) == (v << 0x34)));
            EXPECT_TRUE(all(bit_shift_left<0x35>(v) == (v << 0x35)));
            EXPECT_TRUE(all(bit_shift_left<0x36>(v) == (v << 0x36)));
            EXPECT_TRUE(all(bit_shift_left<0x37>(v) == (v << 0x37)));
            EXPECT_TRUE(all(bit_shift_left<0x38>(v) == (v << 0x38)));
            EXPECT_TRUE(all(bit_shift_left<0x39>(v) == (v << 0x39)));
            EXPECT_TRUE(all(bit_shift_left<0x3a>(v) == (v << 0x3a)));
            EXPECT_TRUE(all(bit_shift_left<0x3b>(v) == (v << 0x3b)));
            EXPECT_TRUE(all(bit_shift_left<0x3c>(v) == (v << 0x3c)));
            EXPECT_TRUE(all(bit_shift_left<0x3d>(v) == (v << 0x3d)));
            EXPECT_TRUE(all(bit_shift_left<0x3e>(v) == (v << 0x3e)));
            EXPECT_TRUE(all(bit_shift_left<0x3f>(v) == (v << 0x3f)));

            EXPECT_TRUE(all(bit_shift_left<0x40>(v) == (v << 0x40)));
        }
    }

    TEST(Vec2x64u, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

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
            EXPECT_TRUE(all(bit_shift_right<0x11>(v) == (v >> 0x11)));
            EXPECT_TRUE(all(bit_shift_right<0x12>(v) == (v >> 0x12)));
            EXPECT_TRUE(all(bit_shift_right<0x13>(v) == (v >> 0x13)));
            EXPECT_TRUE(all(bit_shift_right<0x14>(v) == (v >> 0x14)));
            EXPECT_TRUE(all(bit_shift_right<0x15>(v) == (v >> 0x15)));
            EXPECT_TRUE(all(bit_shift_right<0x16>(v) == (v >> 0x16)));
            EXPECT_TRUE(all(bit_shift_right<0x17>(v) == (v >> 0x17)));
            EXPECT_TRUE(all(bit_shift_right<0x18>(v) == (v >> 0x18)));
            EXPECT_TRUE(all(bit_shift_right<0x19>(v) == (v >> 0x19)));
            EXPECT_TRUE(all(bit_shift_right<0x1a>(v) == (v >> 0x1a)));
            EXPECT_TRUE(all(bit_shift_right<0x1b>(v) == (v >> 0x1b)));
            EXPECT_TRUE(all(bit_shift_right<0x1c>(v) == (v >> 0x1c)));
            EXPECT_TRUE(all(bit_shift_right<0x1d>(v) == (v >> 0x1d)));
            EXPECT_TRUE(all(bit_shift_right<0x1e>(v) == (v >> 0x1e)));
            EXPECT_TRUE(all(bit_shift_right<0x1f>(v) == (v >> 0x1f)));

            EXPECT_TRUE(all(bit_shift_right<0x20>(v) == (v >> 0x20)));
            EXPECT_TRUE(all(bit_shift_right<0x21>(v) == (v >> 0x21)));
            EXPECT_TRUE(all(bit_shift_right<0x22>(v) == (v >> 0x22)));
            EXPECT_TRUE(all(bit_shift_right<0x23>(v) == (v >> 0x23)));
            EXPECT_TRUE(all(bit_shift_right<0x24>(v) == (v >> 0x24)));
            EXPECT_TRUE(all(bit_shift_right<0x25>(v) == (v >> 0x25)));
            EXPECT_TRUE(all(bit_shift_right<0x26>(v) == (v >> 0x26)));
            EXPECT_TRUE(all(bit_shift_right<0x27>(v) == (v >> 0x27)));
            EXPECT_TRUE(all(bit_shift_right<0x28>(v) == (v >> 0x28)));
            EXPECT_TRUE(all(bit_shift_right<0x29>(v) == (v >> 0x29)));
            EXPECT_TRUE(all(bit_shift_right<0x2a>(v) == (v >> 0x2a)));
            EXPECT_TRUE(all(bit_shift_right<0x2b>(v) == (v >> 0x2b)));
            EXPECT_TRUE(all(bit_shift_right<0x2c>(v) == (v >> 0x2c)));
            EXPECT_TRUE(all(bit_shift_right<0x2d>(v) == (v >> 0x2d)));
            EXPECT_TRUE(all(bit_shift_right<0x2e>(v) == (v >> 0x2e)));
            EXPECT_TRUE(all(bit_shift_right<0x2f>(v) == (v >> 0x2f)));

            EXPECT_TRUE(all(bit_shift_right<0x30>(v) == (v >> 0x30)));
            EXPECT_TRUE(all(bit_shift_right<0x31>(v) == (v >> 0x31)));
            EXPECT_TRUE(all(bit_shift_right<0x32>(v) == (v >> 0x32)));
            EXPECT_TRUE(all(bit_shift_right<0x33>(v) == (v >> 0x33)));
            EXPECT_TRUE(all(bit_shift_right<0x34>(v) == (v >> 0x34)));
            EXPECT_TRUE(all(bit_shift_right<0x35>(v) == (v >> 0x35)));
            EXPECT_TRUE(all(bit_shift_right<0x36>(v) == (v >> 0x36)));
            EXPECT_TRUE(all(bit_shift_right<0x37>(v) == (v >> 0x37)));
            EXPECT_TRUE(all(bit_shift_right<0x38>(v) == (v >> 0x38)));
            EXPECT_TRUE(all(bit_shift_right<0x39>(v) == (v >> 0x39)));
            EXPECT_TRUE(all(bit_shift_right<0x3a>(v) == (v >> 0x3a)));
            EXPECT_TRUE(all(bit_shift_right<0x3b>(v) == (v >> 0x3b)));
            EXPECT_TRUE(all(bit_shift_right<0x3c>(v) == (v >> 0x3c)));
            EXPECT_TRUE(all(bit_shift_right<0x3d>(v) == (v >> 0x3d)));
            EXPECT_TRUE(all(bit_shift_right<0x3e>(v) == (v >> 0x3e)));
            EXPECT_TRUE(all(bit_shift_right<0x3f>(v) == (v >> 0x3f)));

            EXPECT_TRUE(all(bit_shift_right<0x40>(v) == (v >> 0x40)));
        }
    }

    TEST(Vec2x64u, Rotl) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

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
            EXPECT_TRUE(all(rotl<0x11>(v) == (rotl(v, 0x11))));
            EXPECT_TRUE(all(rotl<0x12>(v) == (rotl(v, 0x12))));
            EXPECT_TRUE(all(rotl<0x13>(v) == (rotl(v, 0x13))));
            EXPECT_TRUE(all(rotl<0x14>(v) == (rotl(v, 0x14))));
            EXPECT_TRUE(all(rotl<0x15>(v) == (rotl(v, 0x15))));
            EXPECT_TRUE(all(rotl<0x16>(v) == (rotl(v, 0x16))));
            EXPECT_TRUE(all(rotl<0x17>(v) == (rotl(v, 0x17))));
            EXPECT_TRUE(all(rotl<0x18>(v) == (rotl(v, 0x18))));
            EXPECT_TRUE(all(rotl<0x19>(v) == (rotl(v, 0x19))));
            EXPECT_TRUE(all(rotl<0x1a>(v) == (rotl(v, 0x1a))));
            EXPECT_TRUE(all(rotl<0x1b>(v) == (rotl(v, 0x1b))));
            EXPECT_TRUE(all(rotl<0x1c>(v) == (rotl(v, 0x1c))));
            EXPECT_TRUE(all(rotl<0x1d>(v) == (rotl(v, 0x1d))));
            EXPECT_TRUE(all(rotl<0x1e>(v) == (rotl(v, 0x1e))));
            EXPECT_TRUE(all(rotl<0x1f>(v) == (rotl(v, 0x1f))));

            EXPECT_TRUE(all(rotl<0x20>(v) == (rotl(v, 0x20))));
            EXPECT_TRUE(all(rotl<0x21>(v) == (rotl(v, 0x21))));
            EXPECT_TRUE(all(rotl<0x22>(v) == (rotl(v, 0x22))));
            EXPECT_TRUE(all(rotl<0x23>(v) == (rotl(v, 0x23))));
            EXPECT_TRUE(all(rotl<0x24>(v) == (rotl(v, 0x24))));
            EXPECT_TRUE(all(rotl<0x25>(v) == (rotl(v, 0x25))));
            EXPECT_TRUE(all(rotl<0x26>(v) == (rotl(v, 0x26))));
            EXPECT_TRUE(all(rotl<0x27>(v) == (rotl(v, 0x27))));
            EXPECT_TRUE(all(rotl<0x28>(v) == (rotl(v, 0x28))));
            EXPECT_TRUE(all(rotl<0x29>(v) == (rotl(v, 0x29))));
            EXPECT_TRUE(all(rotl<0x2a>(v) == (rotl(v, 0x2a))));
            EXPECT_TRUE(all(rotl<0x2b>(v) == (rotl(v, 0x2b))));
            EXPECT_TRUE(all(rotl<0x2c>(v) == (rotl(v, 0x2c))));
            EXPECT_TRUE(all(rotl<0x2d>(v) == (rotl(v, 0x2d))));
            EXPECT_TRUE(all(rotl<0x2e>(v) == (rotl(v, 0x2e))));
            EXPECT_TRUE(all(rotl<0x2f>(v) == (rotl(v, 0x2f))));

            EXPECT_TRUE(all(rotl<0x30>(v) == (rotl(v, 0x30))));
            EXPECT_TRUE(all(rotl<0x31>(v) == (rotl(v, 0x31))));
            EXPECT_TRUE(all(rotl<0x32>(v) == (rotl(v, 0x32))));
            EXPECT_TRUE(all(rotl<0x33>(v) == (rotl(v, 0x33))));
            EXPECT_TRUE(all(rotl<0x34>(v) == (rotl(v, 0x34))));
            EXPECT_TRUE(all(rotl<0x35>(v) == (rotl(v, 0x35))));
            EXPECT_TRUE(all(rotl<0x36>(v) == (rotl(v, 0x36))));
            EXPECT_TRUE(all(rotl<0x37>(v) == (rotl(v, 0x37))));
            EXPECT_TRUE(all(rotl<0x38>(v) == (rotl(v, 0x38))));
            EXPECT_TRUE(all(rotl<0x39>(v) == (rotl(v, 0x39))));
            EXPECT_TRUE(all(rotl<0x3a>(v) == (rotl(v, 0x3a))));
            EXPECT_TRUE(all(rotl<0x3b>(v) == (rotl(v, 0x3b))));
            EXPECT_TRUE(all(rotl<0x3c>(v) == (rotl(v, 0x3c))));
            EXPECT_TRUE(all(rotl<0x3d>(v) == (rotl(v, 0x3d))));
            EXPECT_TRUE(all(rotl<0x3e>(v) == (rotl(v, 0x3e))));
            EXPECT_TRUE(all(rotl<0x3f>(v) == (rotl(v, 0x3f))));

            EXPECT_TRUE(all(rotl<0x40>(v) == (rotl(v, 0x40))));
        }
    }

    TEST(Vec2x64u, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            auto v1 = random64u();

            auto results = rotl(v0, v1);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = rotl(v0, v1);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Rotr) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();

            vec2x64u v{inputs0};

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
            EXPECT_TRUE(all(rotr<0x11>(v) == (rotr(v, 0x11))));
            EXPECT_TRUE(all(rotr<0x12>(v) == (rotr(v, 0x12))));
            EXPECT_TRUE(all(rotr<0x13>(v) == (rotr(v, 0x13))));
            EXPECT_TRUE(all(rotr<0x14>(v) == (rotr(v, 0x14))));
            EXPECT_TRUE(all(rotr<0x15>(v) == (rotr(v, 0x15))));
            EXPECT_TRUE(all(rotr<0x16>(v) == (rotr(v, 0x16))));
            EXPECT_TRUE(all(rotr<0x17>(v) == (rotr(v, 0x17))));
            EXPECT_TRUE(all(rotr<0x18>(v) == (rotr(v, 0x18))));
            EXPECT_TRUE(all(rotr<0x19>(v) == (rotr(v, 0x19))));
            EXPECT_TRUE(all(rotr<0x1a>(v) == (rotr(v, 0x1a))));
            EXPECT_TRUE(all(rotr<0x1b>(v) == (rotr(v, 0x1b))));
            EXPECT_TRUE(all(rotr<0x1c>(v) == (rotr(v, 0x1c))));
            EXPECT_TRUE(all(rotr<0x1d>(v) == (rotr(v, 0x1d))));
            EXPECT_TRUE(all(rotr<0x1e>(v) == (rotr(v, 0x1e))));
            EXPECT_TRUE(all(rotr<0x1f>(v) == (rotr(v, 0x1f))));

            EXPECT_TRUE(all(rotr<0x20>(v) == (rotr(v, 0x20))));
            EXPECT_TRUE(all(rotr<0x21>(v) == (rotr(v, 0x21))));
            EXPECT_TRUE(all(rotr<0x22>(v) == (rotr(v, 0x22))));
            EXPECT_TRUE(all(rotr<0x23>(v) == (rotr(v, 0x23))));
            EXPECT_TRUE(all(rotr<0x24>(v) == (rotr(v, 0x24))));
            EXPECT_TRUE(all(rotr<0x25>(v) == (rotr(v, 0x25))));
            EXPECT_TRUE(all(rotr<0x26>(v) == (rotr(v, 0x26))));
            EXPECT_TRUE(all(rotr<0x27>(v) == (rotr(v, 0x27))));
            EXPECT_TRUE(all(rotr<0x28>(v) == (rotr(v, 0x28))));
            EXPECT_TRUE(all(rotr<0x29>(v) == (rotr(v, 0x29))));
            EXPECT_TRUE(all(rotr<0x2a>(v) == (rotr(v, 0x2a))));
            EXPECT_TRUE(all(rotr<0x2b>(v) == (rotr(v, 0x2b))));
            EXPECT_TRUE(all(rotr<0x2c>(v) == (rotr(v, 0x2c))));
            EXPECT_TRUE(all(rotr<0x2d>(v) == (rotr(v, 0x2d))));
            EXPECT_TRUE(all(rotr<0x2e>(v) == (rotr(v, 0x2e))));
            EXPECT_TRUE(all(rotr<0x2f>(v) == (rotr(v, 0x2f))));

            EXPECT_TRUE(all(rotr<0x30>(v) == (rotr(v, 0x30))));
            EXPECT_TRUE(all(rotr<0x31>(v) == (rotr(v, 0x31))));
            EXPECT_TRUE(all(rotr<0x32>(v) == (rotr(v, 0x32))));
            EXPECT_TRUE(all(rotr<0x33>(v) == (rotr(v, 0x33))));
            EXPECT_TRUE(all(rotr<0x34>(v) == (rotr(v, 0x34))));
            EXPECT_TRUE(all(rotr<0x35>(v) == (rotr(v, 0x35))));
            EXPECT_TRUE(all(rotr<0x36>(v) == (rotr(v, 0x36))));
            EXPECT_TRUE(all(rotr<0x37>(v) == (rotr(v, 0x37))));
            EXPECT_TRUE(all(rotr<0x38>(v) == (rotr(v, 0x38))));
            EXPECT_TRUE(all(rotr<0x39>(v) == (rotr(v, 0x39))));
            EXPECT_TRUE(all(rotr<0x3a>(v) == (rotr(v, 0x3a))));
            EXPECT_TRUE(all(rotr<0x3b>(v) == (rotr(v, 0x3b))));
            EXPECT_TRUE(all(rotr<0x3c>(v) == (rotr(v, 0x3c))));
            EXPECT_TRUE(all(rotr<0x3d>(v) == (rotr(v, 0x3d))));
            EXPECT_TRUE(all(rotr<0x3e>(v) == (rotr(v, 0x3e))));
            EXPECT_TRUE(all(rotr<0x3f>(v) == (rotr(v, 0x3f))));

            EXPECT_TRUE(all(rotr<0x40>(v) == (rotr(v, 0x40))));
        }
    }

    TEST(Vec2x64u, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            auto v1 = random64u();

            auto results = rotr(v0, v1);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = rotr(v0, v1);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec2x64u, Count_random) {
        auto predicate = [] (vec2x64u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec2x64u v{inputs};

            auto results = count(v);

            EXPECT_EQ(cnt, results);
        }
    }

    TEST(Vec2x64u, Any_edge_cases) {
        EXPECT_FALSE(any(vec2x64u{0x00}));
        EXPECT_TRUE(any(vec2x64u{0x01}));
    }

    TEST(Vec2x64u, Any_random) {
        auto predicate = [] (vec2x64u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec2x64u v{inputs};

            EXPECT_EQ(cnt != 0, any(v));
        }
    }

    TEST(Vec2x64u, All_edge_cases) {
        EXPECT_FALSE(all(vec2x64u{0x00}));
        EXPECT_TRUE(all(vec2x64u{0x01}));
    }

    TEST(Vec2x64u, All_random) {
        auto predicate = [] (vec2x64u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec2x64u v{inputs};

            auto results = all(v);

            EXPECT_EQ(cnt == vec2x64u::width, results);
        }
    }

    TEST(Vec2x64u, None_edge_cases) {
        EXPECT_TRUE(none(vec2x64u{0x00}));
        EXPECT_FALSE(none(vec2x64u{0x01}));
    }

    TEST(Vec2x64u, None_random) {
        auto predicate = [] (vec2x64u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec2x64u v{inputs};

            EXPECT_EQ(cnt == 0, none(v));
        }
    }

    TEST(Vec2x64u, Broadcast_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();

            mask2x64u v{inputs};

            auto results = broadcast_mask(v);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                if (inputs[j]) {
                    expected[j] = vec2x64u::scalar(-1);
                } else {
                    expected[j] = vec2x64u::scalar(0);
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Keep_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2x64u>();

            mask2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = keep(v0, v1);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = keep(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Clear_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2x64u>();

            mask2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = clear(v0, v1);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = clear(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2x64u>();
            auto inputs2 = random_array<arr2x64u>();

            mask2x64u v0{inputs0};
            vec2x64u v1{inputs1};
            vec2x64u v2{inputs2};

            auto results = blend(v0, v1, v2);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = blend(inputs0[j], inputs1[j], inputs2[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = max(v0, v1);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = max(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = min(v0, v1);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = min(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = minmax(v0, v1);

            arr2x64u expected0{};
            arr2x64u expected1{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto min_max = minmax(inputs0[j], inputs1[j]);
                expected0[j] = min_max[0];
                expected1[j] = min_max[1];
            }

            EXPECT_TRUE(all(results[0] == vec2x64u{expected0}));
            EXPECT_TRUE(all(results[1] == vec2x64u{expected1}));
        }
    }

    TEST(Vec2x64u, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();
            auto inputs2 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};
            vec2x64u v2{inputs2};

            auto ranges = minmax(v1, v2);

            auto results = clamp(v0, ranges[0], ranges[1]);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto range = minmax(inputs1[j], inputs2[j]);
                expected[j] = clamp(inputs0[j], range[0], range[1]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = average(v0, v1);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = average(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64u>();
            auto inputs1 = random_array<arr2x64u>();

            vec2x64u v0{inputs0};
            vec2x64u v1{inputs1};

            auto results = midpoint(v0, v1);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = midpoint(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = neg_abs(v);

            arr2x64i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = neg_abs(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{expected}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec2x64u, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            for (std::uint32_t j = 0; j <= vec2x64u::width; ++j) {
                auto results = load<vec2x64u>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec2x64u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec2x64u, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            EXPECT_TRUE(all(load<vec2x64u, 0x00>(inputs.data()) == load<vec2x64u>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(load<vec2x64u, 0x01>(inputs.data()) == load<vec2x64u>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(load<vec2x64u, 0x02>(inputs.data()) == load<vec2x64u>(inputs.data(), 0x02)));
        }
    }

    TEST(Vec2x64u, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec2x64u)) auto inputs = random_array<arr2x64u>();

            for (std::uint32_t j = 0; j <= vec2x64u::width; ++j) {
                auto results = aligned_load<vec2x64u>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec2x64u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec2x64u, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec2x64u)) auto inputs = random_array<arr2x64u>();

            EXPECT_TRUE(all(aligned_load<vec2x64u, 0x00>(inputs.data()) == aligned_load<vec2x64u>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec2x64u, 0x01>(inputs.data()) == aligned_load<vec2x64u>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(aligned_load<vec2x64u, 0x02>(inputs.data()) == aligned_load<vec2x64u>(inputs.data(), 0x02)));
        }
    }

    TEST(Vec2x64u, Gather_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;

        std::vector<vec2x64u::scalar> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random_val<vec2x64u::scalar>();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = random_val<vec2x64u::scalar>() % test_data_size;
            }
            auto indices = load<vec2x64i>(index_array.data());

            for (std::size_t j = 0; j <= vec2x64u::width; ++j) {
                auto results = gather<vec2x64u>(test_data.data(), indices, j);

                auto loaded_data = to_array(results);
                for (std::size_t k = 0; k < vec2x64u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(test_data[index_array[k]], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec2x64u, Gather_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;

        std::vector<vec2x64u::scalar> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random_val<vec2x64u::scalar>();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i index_array{};
            for (std::size_t j = 0; j < vec2x64u::width; ++j) {
                index_array[j] = random_val<vec2x64u::scalar>() % test_data_size;
            }
            auto indices = load<vec2x64i>(index_array.data());

            EXPECT_TRUE(all(gather<vec2x64u, 0x00>(test_data.data(), indices) == gather<vec2x64u>(test_data.data(), indices, 0x00)));
            EXPECT_TRUE(all(gather<vec2x64u, 0x01>(test_data.data(), indices) == gather<vec2x64u>(test_data.data(), indices, 0x01)));
            EXPECT_TRUE(all(gather<vec2x64u, 0x02>(test_data.data(), indices) == gather<vec2x64u>(test_data.data(), indices, 0x02)));
        }
    }

    TEST(Vec2x64u, Store_n) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            for (std::size_t j = 0; j <= vec2x64u::width; ++j) {
                vec2x64u::scalar arr[vec2x64u::width]{};
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

    TEST(Vec2x64u, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            vec2x64u::scalar arr[vec2x64u::width]{};

            store<0x00>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x00));
            store<0x01>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x01));
            store<0x02>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x02));
        }
    }

    TEST(Vec2x64u, Aligned_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            for (std::size_t j = 0; j <= vec2x64u::width; ++j) {
                alignas(alignof(vec2x64u)) vec2x64u::scalar arr[vec2x64u::width]{};
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

    TEST(Vec2x64u, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            alignas(alignof(vec2x64u)) vec2x64u::scalar arr[vec2x64u::width]{};

            aligned_store<0x00>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x00));
            aligned_store<0x01>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x01));
            aligned_store<0x02>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x02));
        }
    }

    TEST(Vec2x64u, Scatter_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;
        static constexpr std::size_t block_size = test_data_size / vec2x64u::width;

        std::vector<vec2x64u::scalar> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr2x64u>();

            vec2x64u input0{input_array0};

            arr2x64i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random_val<vec2x64u::scalar>() % block_size) + (j * block_size);
            }

            vec2x64i indices{index_array};

            for (std::size_t j = 0; j <= vec2x64u::width; ++j) {
                scatter(test_data.data(), input0, indices, j);

                for (std::size_t k = 0; k < input_array0.size(); ++k) {
                    if (k < j) {
                        EXPECT_EQ(input_array0[k], test_data[index_array[k]]);
                    }
                }
            }
        }
    }

    TEST(Vec2x64u, Scatter_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;
        static constexpr std::size_t block_size = test_data_size / vec2x64u::width;

        std::vector<vec2x64u::scalar> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr2x64u>();

            vec2x64u input0{input_array0};

            arr2x64i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random_val<vec2x64u::scalar>() % block_size) + (j * block_size);
            }

            vec2x64i indices{index_array};

            scatter<0x0>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x00));
            scatter<0x1>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x01));
            scatter<0x2>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x02));
        }
    }

    TEST(Vec2x64u, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec2x64u, Div_uniform) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto numerator = random_val<vec2x64u::scalar>();
            auto denominator = random_denominator<vec2x64u::scalar>();

            auto quotient  = (numerator / denominator);
            auto remainder = (numerator % denominator);

            vec2x64u n{numerator};
            vec2x64u d{denominator};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < q.size(); ++j) {
                EXPECT_EQ(q[j], quotient);
                EXPECT_EQ(r[j], remainder);
            }
        }
    }

    TEST(Vec2x64u, Div_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto numerators = random_array<arr2x64u>();
            auto denominators = random_denominator_array<arr2x64u>();

            arr2x64u quotients{};
            arr2x64u remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec2x64u n{numerators};
            vec2x64u d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec2x64u, Popcount_edge_cases) {
        vec2x64u v{0x0};
        vec2x64u c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec2x64u, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = popcount(v);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = popcount(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = byteswap(v);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = byteswap(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Countl_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec2x64u::scalar>::value - 1); ++i) {
            vec2x64u::scalar v = (vec2x64u::scalar(0x1) << i) - 1;
            auto results = countl_zero(vec2x64u{v});
            auto expected = vec2x64u(type_width<vec2x64u::scalar>::value - i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countl_zero(vec2x64u(-1)) == vec2x64u{0}));
    }

    TEST(Vec2x64u, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = countl_zero(v);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Countl_one_edge_cases) {
        EXPECT_TRUE(all(countl_one(vec2x64u{0x00}) == vec2x64u{0}));
        for (std::size_t i = 0; i < (type_width<vec2x64u::scalar>::value - 1); ++i) {
            vec2x64u::scalar v = vec2x64u::scalar(-1) << i;
            auto results = countl_one(vec2x64u{v});
            auto expected = vec2x64u(type_width<vec2x64u::scalar>::value - i);
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec2x64u, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = countl_one(v);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Countr_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec2x64u::scalar>::value - 1); ++i) {
            vec2x64u::scalar v = vec2x64u::scalar(-1) << i;
            auto results = countr_zero(vec2x64u{v});
            auto expected = vec2x64u(i);
            EXPECT_TRUE(all(results == expected));
        }

        EXPECT_TRUE(all(countr_zero(vec2x64u{0x00}) == vec2x64u{type_width<vec2x64u::scalar>::value}));
    }

    TEST(Vec2x64u, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = countr_zero(v);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Countr_one_edge_cases) {
        EXPECT_TRUE(all(countr_one(vec2x64u{0x00}) == vec2x64u{0}));

        for (std::size_t i = 0; i < (type_width<vec2x64u::scalar>::value - 1); ++i) {
            vec2x64u::scalar v = (vec2x64u::scalar(1) << i) - 1;
            auto results = countr_one(vec2x64u{v});
            auto expected = vec2x64u(i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countr_one(vec2x64u{vec2x64u::scalar(-1)}) == vec2x64u{type_width<vec2x64u::scalar>::value}));
    }

    TEST(Vec2x64u, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = countr_one(v);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Bit_width_edge_cases) {
        EXPECT_TRUE(all(bit_width(vec2x64u{0x00}) == vec2x64u{0x00}));

        for (std::size_t i = 0; i < (type_width<vec2x64u::scalar>::value - 1); ++i) {
            vec2x64u::scalar v = vec2x64u::scalar(1) << i;
            auto results = bit_width(vec2x64u{v});
            auto expected = vec2x64u(i + 1);
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec2x64u, Bit_width_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = bit_width(v);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_width(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Bit_floor_edge_cases) {
        EXPECT_TRUE(all(vec2x64u{0} == bit_floor(vec2x64u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec2x64u::scalar>::value - 1); ++i) {
            vec2x64u::scalar v = vec2x64u::scalar(1) << i;
            auto results = bit_floor(vec2x64u{v});
            auto expected = vec2x64u{v};
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec2x64u, Bit_floor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = bit_floor(v);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_floor(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Bit_ceil_edge_cases) {
        EXPECT_TRUE(all(vec2x64u{1} == bit_ceil(vec2x64u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec2x64u::scalar>::value - 1); ++i) {
            vec2x64u::scalar v = vec2x64u::scalar(1) << i;
            auto results = bit_ceil(vec2x64u{v});
            auto expected = vec2x64u{v};
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec2x64u, Bit_ceil_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = bit_ceil(v);

            arr2x64u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_ceil(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{expected}));
        }
    }

    TEST(Vec2x64u, Has_single_bit_edge_cases) {
        EXPECT_TRUE(none(has_single_bit(vec2x64u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec2x64u::scalar>::value - 1); ++i) {
            vec2x64u::scalar v = vec2x64u::scalar(1) << i;
            auto results = has_single_bit(vec2x64u{v});
            EXPECT_TRUE(all(results));
        }
    }

    TEST(Vec2x64u, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64u>();

            vec2x64u v{inputs};

            auto results = has_single_bit(v);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = has_single_bit(inputs[j]);
            }

            EXPECT_TRUE(results == mask2x64u{expected});
        }
    }

}

#endif
