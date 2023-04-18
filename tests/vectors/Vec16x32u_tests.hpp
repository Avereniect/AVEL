#ifndef AVEL_VEC16X32U_TESTS_HPP
#define AVEL_VEC16X32U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=========================================================================
    // mask16x32u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask16x32u, Construct_from_primitive) {
        mask16x32u mask{mask16x32u::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask16x32u, Construct_from_bool) {
        mask16x32u mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask16x32u mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask16x32u::width);
    }

    TEST(Mask16x32u, Construct_from_array) {
        arr16xb false_array{};
        std::fill_n(false_array.data(), mask16x32u::width, false);
        mask16x32u mask0{false_array};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);


        arr16xb true_array{};
        std::fill_n(true_array.data(), mask16x32u::width, true);
        mask16x32u mask1{true_array};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask16x32u::width);
    }

    TEST(Mask16x32u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16xb>();
            std::uint32_t cnt = std::count(inputs.begin(), inputs.end(), true);

            auto m = mask16x32u{inputs};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == mask16x32u::width, all(m));
            EXPECT_EQ(cnt != 0, any(m));
            EXPECT_EQ(cnt == 0, none(m));
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask16x32u, Assign_bool) {
        mask16x32u mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask16x32u mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(count(mask1) == mask16x32u::width);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask16x32u, Equality_comparison) {
        mask16x32u mask0{false};
        mask16x32u mask1{false};
        EXPECT_TRUE(mask0 == mask1);

        mask16x32u mask2{true};
        mask16x32u mask3{true};
        EXPECT_TRUE(mask2 == mask3);

        EXPECT_FALSE(mask0 == mask2);
        EXPECT_FALSE(mask3 == mask1);
    }

    TEST(Mask16x32u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16xb>();
            inputs1[0] = !inputs0[0];

            mask16x32u mask0{inputs0};
            mask16x32u mask1{inputs0};
            mask16x32u mask2{inputs1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask16x32u, Inequality_comparison) {
        mask16x32u mask0{false};
        mask16x32u mask1{false};
        EXPECT_FALSE(mask0 != mask1);

        mask16x32u mask2{true};
        mask16x32u mask3{true};
        EXPECT_FALSE(mask2 != mask3);

        EXPECT_TRUE(mask0 != mask2);
        EXPECT_TRUE(mask3 != mask1);
    }

    TEST(Mask16x32u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16xb>();
            inputs1[0] = !inputs0[0];

            mask16x32u mask0{inputs0};
            mask16x32u mask1{inputs1};
            mask16x32u mask2{inputs1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask16x32u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16xb>();

            mask16x32u mask0{inputs0};
            mask16x32u mask1{inputs1};

            mask16x32u results = mask0;
            results &= mask1;

            arr16xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask16x32u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask16x32u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16xb>();

            mask16x32u mask0{inputs0};
            mask16x32u mask1{inputs1};

            mask16x32u results = mask0;
            results |= mask1;

            arr16xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask16x32u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask16x32u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16xb>();

            mask16x32u mask0{inputs0};
            mask16x32u mask1{inputs1};

            mask16x32u results = mask0;
            results ^= mask1;

            arr16xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask16x32u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask16x32u, Logical_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16xb>();

            mask16x32u mask0{inputs};

            mask16x32u results = !mask0;

            arr16xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = !inputs[j];
            }
            mask16x32u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask16x32u, Bitwise_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16xb>();

            mask16x32u mask0{inputs0};
            mask16x32u mask1{inputs1};

            mask16x32u results = mask0 & mask1;

            arr16xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask16x32u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask16x32u, Logical_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16xb>();

            mask16x32u mask0{inputs0};
            mask16x32u mask1{inputs1};

            mask16x32u results = mask0 && mask1;

            arr16xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] && inputs1[j];
            }
            mask16x32u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask16x32u, Bitwise_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16xb>();

            mask16x32u mask0{inputs0};
            mask16x32u mask1{inputs1};

            mask16x32u results = mask0 | mask1;

            arr16xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask16x32u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask16x32u, Logical_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16xb>();

            mask16x32u mask0{inputs0};
            mask16x32u mask1{inputs1};

            mask16x32u results = mask0 || mask1;

            arr16xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] || inputs1[j];
            }
            mask16x32u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask16x32u, Bitwise_xor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16xb>();

            mask16x32u mask0{inputs0};
            mask16x32u mask1{inputs1};

            mask16x32u results = mask0 ^ mask1;

            arr16xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask16x32u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=========================================================================
    // Vec16x32u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec16x32u, Construct_vector_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16xb>();

            mask16x32u m0{inputs};
            vec16x32u v0{m0};

            auto results = to_array(v0);
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    TEST(Vec16x32u, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto x = random_val<vec16x32u::scalar>();
            vec16x32u results{x};

            arr16x32u arr;
            std::fill_n(arr.data(), vec16x32u::width, x);
            vec16x32u expected{arr};

            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec16x32u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(inputs[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec16x32u, Assign_primitive) {
        vec16x32u::primitive x{};
        vec16x32u v{0x0F};
        v = x;
        vec16x32u expected{};

        EXPECT_TRUE(all(v == expected));
    }

    TEST(Vec16x32u, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto scalar = random_val<vec16x32u::scalar>();
            vec16x32u v;
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

    TEST(Vec16x32u, Equality_comparison) {
        mask16x32u mask0{false};
        mask16x32u mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask16x32u mask2{true};
        mask16x32u mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec16x32u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();
            inputs1[0] = ~inputs0[0];

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs0};
            vec16x32u v2{inputs1};

            EXPECT_TRUE(all(v0 == v1));
            EXPECT_FALSE(all(v1 == v2));
        }
    }

    TEST(Vec16x32u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();
            inputs1[0] = ~inputs0[0];

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs0};
            vec16x32u v2{inputs1};

            EXPECT_FALSE(any(v0 != v1));
            EXPECT_TRUE(any(v1 != v2));
        }
    }

    TEST(Vec16x32u, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = (v0 < v1);

            arr16xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] < inputs1[j];
            }

            EXPECT_EQ(results, mask16x32u{expected});
        }
    }

    TEST(Vec16x32u, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = (v0 <= v1);

            arr16xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] <= inputs1[j];
            }

            EXPECT_EQ(results, mask16x32u{expected});
        }
    }

    TEST(Vec16x32u, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = (v0 > v1);

            arr16xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] > inputs1[j];
            }

            EXPECT_EQ(results, mask16x32u{expected});
        }
    }

    TEST(Vec16x32u, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = (v0 >= v1);

            arr16xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] >= inputs1[j];
            }

            EXPECT_EQ(results, mask16x32u{expected});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec16x32u, Unary_plus) {
        auto inputs = random_array<arr16x32u>();
        const vec16x32u v{inputs};
        vec16x32u results{+v};

        EXPECT_TRUE(all(results == v));
    }

    TEST(Vec16x32u, Unary_minus) {
        auto inputs = random_array<arr16x32u>();
        const vec16x32u v{inputs};

        auto results = -v;

        arr16x32i expected{};
        for (std::size_t j = 0; j < expected.size(); ++j) {
            expected[j] = -inputs[j];
        }

        EXPECT_TRUE(all(results == vec16x32i{expected}));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec16x32u, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0;
            results += v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0;
            results -= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0;
            results *= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_denominator_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0;
            results /= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_denominator_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0;
            results %= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec16x32u, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0 + v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0 - v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0 * v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_denominator_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0 / v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_denominator_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0 % v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec16x32u, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};
            ++v;

            auto results = v;
            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};
            v++;

            auto results = v;
            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};
            --v;

            auto results = v;
            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};
            v--;

            auto results = v;
            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec16x32u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0;
            results &= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0;
            results |= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0;
            results ^= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            auto v1 = random_shift<vec16x32u::scalar>();

            auto results = v0;
            results <<= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec16x32u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_shift_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0;
            results <<= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec16x32u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            auto v1 = random_shift<vec16x32u::scalar>();

            auto results = v0;
            results >>= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec16x32u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_shift_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0;
            results >>= v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec16x32u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec16x32u, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = ~v;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ~inputs[j];
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            auto v1 = random_shift<vec16x32u::scalar>();

            auto results = v0 << v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec16x32u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_shift_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0 << v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec16x32u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            auto v1 = random_shift<vec16x32u::scalar>();

            auto results = v0 >> v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec16x32u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_shift_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = v0 >> v1;

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec16x32u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec16x32u, Conversion_to_mask) {
        vec16x32u vec{0};

        auto a = vec.operator mask16x32u();
        mask16x32u b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec16x32u, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = mask16x32u{v};

            arr16xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = inputs[j];
            }

            EXPECT_TRUE(results == mask16x32u{expected});
        }
    }

    //=====================================================
    // Arrangement Operations
    //=====================================================

    TEST(Vec16x32u, Extract_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();
            vec16x32u v{inputs};

            EXPECT_EQ(inputs[0x0], extract<0x0>(v));
            EXPECT_EQ(inputs[0x1], extract<0x1>(v));
            EXPECT_EQ(inputs[0x2], extract<0x2>(v));
            EXPECT_EQ(inputs[0x3], extract<0x3>(v));
            EXPECT_EQ(inputs[0x4], extract<0x4>(v));
            EXPECT_EQ(inputs[0x5], extract<0x5>(v));
            EXPECT_EQ(inputs[0x6], extract<0x6>(v));
            EXPECT_EQ(inputs[0x7], extract<0x7>(v));
            EXPECT_EQ(inputs[0x8], extract<0x8>(v));
            EXPECT_EQ(inputs[0x9], extract<0x9>(v));
            EXPECT_EQ(inputs[0xa], extract<0xa>(v));
            EXPECT_EQ(inputs[0xb], extract<0xb>(v));
            EXPECT_EQ(inputs[0xc], extract<0xc>(v));
            EXPECT_EQ(inputs[0xd], extract<0xd>(v));
            EXPECT_EQ(inputs[0xe], extract<0xe>(v));
            EXPECT_EQ(inputs[0xf], extract<0xf>(v));
        }
    }

    TEST(Vec16x32u, Insert_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();
            vec16x32u v{random_val<vec16x32u::scalar>()};

            v = insert<0x0>(v, inputs[0x0]);
            v = insert<0x1>(v, inputs[0x1]);
            v = insert<0x2>(v, inputs[0x2]);
            v = insert<0x3>(v, inputs[0x3]);
            v = insert<0x4>(v, inputs[0x4]);
            v = insert<0x5>(v, inputs[0x5]);
            v = insert<0x6>(v, inputs[0x6]);
            v = insert<0x7>(v, inputs[0x7]);
            v = insert<0x8>(v, inputs[0x8]);
            v = insert<0x9>(v, inputs[0x9]);
            v = insert<0xa>(v, inputs[0xa]);
            v = insert<0xb>(v, inputs[0xb]);
            v = insert<0xc>(v, inputs[0xc]);
            v = insert<0xd>(v, inputs[0xd]);
            v = insert<0xe>(v, inputs[0xe]);
            v = insert<0xf>(v, inputs[0xf]);

            EXPECT_TRUE(all(v == vec16x32u{inputs}));
        }
    }

    //=====================================================
    // Bit Manipulation Instructions
    //=====================================================

    TEST(Vec16x32u, Bit_shift_left) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

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
        }
    }

    TEST(Vec16x32u, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

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
        }
    }

    TEST(Vec16x32u, Rotl) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

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
        }
    }

    TEST(Vec16x32u, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            auto v1 = random64u();

            auto results = rotl(v0, v1);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = rotl(v0, v1);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Rotr) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();

            vec16x32u v{inputs0};

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
        }
    }

    TEST(Vec16x32u, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            auto v1 = random64u();

            auto results = rotr(v0, v1);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = rotr(v0, v1);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec16x32u, Count_random) {
        auto predicate = [] (vec16x32u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec16x32u v{inputs};

            auto results = count(v);

            EXPECT_EQ(cnt, results);
        }
    }

    TEST(Vec16x32u, Any_edge_cases) {
        EXPECT_FALSE(any(vec16x32u{0x00}));
        EXPECT_TRUE(any(vec16x32u{0x01}));
    }

    TEST(Vec16x32u, Any_random) {
        auto predicate = [] (vec16x32u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec16x32u v{inputs};

            EXPECT_EQ(cnt != 0, any(v));
        }
    }

    TEST(Vec16x32u, All_edge_cases) {
        EXPECT_FALSE(all(vec16x32u{0x00}));
        EXPECT_TRUE(all(vec16x32u{0x01}));
    }

    TEST(Vec16x32u, All_random) {
        auto predicate = [] (vec16x32u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec16x32u v{inputs};

            auto results = all(v);

            EXPECT_EQ(cnt == vec16x32u::width, results);
        }
    }

    TEST(Vec16x32u, None_edge_cases) {
        EXPECT_TRUE(none(vec16x32u{0x00}));
        EXPECT_FALSE(none(vec16x32u{0x01}));
    }

    TEST(Vec16x32u, None_random) {
        auto predicate = [] (vec16x32u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec16x32u v{inputs};

            EXPECT_EQ(cnt == 0, none(v));
        }
    }

    TEST(Vec16x32u, Set_bits_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16xb>();

            mask16x32u v{inputs};

            auto results = set_bits(v);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                if (inputs[j]) {
                    expected[j] = vec16x32u::scalar(-1);
                } else {
                    expected[j] = vec16x32u::scalar(0);
                }
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Keep_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16x32u>();

            mask16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = keep(v0, v1);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = keep(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Clear_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16x32u>();

            mask16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = clear(v0, v1);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = clear(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16xb>();
            auto inputs1 = random_array<arr16x32u>();
            auto inputs2 = random_array<arr16x32u>();

            mask16x32u v0{inputs0};
            vec16x32u v1{inputs1};
            vec16x32u v2{inputs2};

            auto results = blend(v0, v1, v2);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = blend(inputs0[j], inputs1[j], inputs2[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = max(v0, v1);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = max(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = min(v0, v1);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = min(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = minmax(v0, v1);

            arr16x32u expected0{};
            arr16x32u expected1{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto min_max = minmax(inputs0[j], inputs1[j]);
                expected0[j] = min_max[0];
                expected1[j] = min_max[1];
            }

            EXPECT_TRUE(all(results[0] == vec16x32u{expected0}));
            EXPECT_TRUE(all(results[1] == vec16x32u{expected1}));
        }
    }

    TEST(Vec16x32u, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();
            auto inputs2 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};
            vec16x32u v2{inputs2};

            auto ranges = minmax(v1, v2);

            auto results = clamp(v0, ranges[0], ranges[1]);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto range = minmax(inputs1[j], inputs2[j]);
                expected[j] = clamp(inputs0[j], range[0], range[1]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = average(v0, v1);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = average(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr16x32u>();
            auto inputs1 = random_array<arr16x32u>();

            vec16x32u v0{inputs0};
            vec16x32u v1{inputs1};

            auto results = midpoint(v0, v1);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = midpoint(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = neg_abs(v);

            arr16x32i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = neg_abs(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec16x32i{expected}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec16x32u, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            for (std::size_t j = 0; j <= vec16x32u::width; ++j) {
                auto results = load<vec16x32u>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::size_t k = 0; k < vec16x32u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec16x32u, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            EXPECT_TRUE(all(load<vec16x32u, 0x00>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(load<vec16x32u, 0x01>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(load<vec16x32u, 0x02>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x02)));
            EXPECT_TRUE(all(load<vec16x32u, 0x03>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x03)));
            EXPECT_TRUE(all(load<vec16x32u, 0x04>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x04)));
            EXPECT_TRUE(all(load<vec16x32u, 0x05>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x05)));
            EXPECT_TRUE(all(load<vec16x32u, 0x06>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x06)));
            EXPECT_TRUE(all(load<vec16x32u, 0x07>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x07)));
            EXPECT_TRUE(all(load<vec16x32u, 0x08>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x08)));
            EXPECT_TRUE(all(load<vec16x32u, 0x09>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x09)));
            EXPECT_TRUE(all(load<vec16x32u, 0x0a>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x0a)));
            EXPECT_TRUE(all(load<vec16x32u, 0x0b>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x0b)));
            EXPECT_TRUE(all(load<vec16x32u, 0x0c>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x0c)));
            EXPECT_TRUE(all(load<vec16x32u, 0x0e>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x0e)));
            EXPECT_TRUE(all(load<vec16x32u, 0x0e>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x0e)));
            EXPECT_TRUE(all(load<vec16x32u, 0x0f>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x0f)));

            EXPECT_TRUE(all(load<vec16x32u, 0x10>(inputs.data()) == load<vec16x32u>(inputs.data(), 0x10)));
        }
    }

    TEST(Vec16x32u, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec16x32u)) auto inputs = random_array<arr16x32u>();

            for (std::size_t j = 0; j <= vec16x32u::width; ++j) {
                auto results = aligned_load<vec16x32u>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::size_t k = 0; k < vec16x32u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec16x32u, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec16x32u)) auto inputs = random_array<arr16x32u>();

            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x00>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x01>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x02>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x02)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x03>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x03)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x04>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x04)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x05>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x05)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x06>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x06)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x07>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x07)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x08>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x08)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x09>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x09)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x0a>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x0a)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x0b>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x0b)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x0c>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x0c)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x0e>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x0e)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x0e>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x0e)));
            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x0f>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x0f)));

            EXPECT_TRUE(all(aligned_load<vec16x32u, 0x10>(inputs.data()) == aligned_load<vec16x32u>(inputs.data(), 0x10)));
        }
    }

    TEST(Vec16x32u, Gather_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;

        std::vector<vec16x32u::scalar> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random_val<vec16x32u::scalar>();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x32i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = random_val<vec16x32u::scalar>() % test_data_size;
            }
            auto indices = load<vec16x32i>(index_array.data());

            for (std::size_t j = 0; j <= vec16x32u::width; ++j) {
                auto results = gather<vec16x32u>(test_data.data(), indices, j);

                auto loaded_data = to_array(results);
                for (std::size_t k = 0; k < vec16x32u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(test_data[index_array[k]], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec16x32u, Gather_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;

        std::vector<vec16x32u::scalar> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random_val<vec16x32u::scalar>();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x32i index_array{};
            for (std::size_t j = 0; j < vec16x32u::width; ++j) {
                index_array[j] = random_val<vec16x32u::scalar>() % test_data_size;
            }
            auto indices = load<vec16x32i>(index_array.data());

            EXPECT_TRUE(all(gather<vec16x32u, 0x00>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x00)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x01>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x01)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x02>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x02)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x03>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x03)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x04>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x04)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x05>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x05)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x06>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x06)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x07>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x07)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x08>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x08)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x09>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x09)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x0a>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x0a)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x0b>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x0b)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x0c>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x0c)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x0d>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x0d)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x0e>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x0e)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x0f>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x0f)));
            EXPECT_TRUE(all(gather<vec16x32u, 0x10>(test_data.data(), indices) == gather<vec16x32u>(test_data.data(), indices, 0x10)));
        }
    }

    TEST(Vec16x32u, Store_n) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            for (std::size_t j = 0; j <= vec16x32u::width; ++j) {
                vec16x32u::scalar arr[vec16x32u::width]{};
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

    TEST(Vec16x32u, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            vec16x32u::scalar arr[vec16x32u::width]{};

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
            store<0x0e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0e));
            store<0x0e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0e));
            store<0x0f>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0f));

            store<0x10>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x10));
        }
    }

    TEST(Vec16x32u, Aligned_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            for (std::size_t j = 0; j <= vec16x32u::width; ++j) {
                alignas(alignof(vec16x32u)) vec16x32u::scalar arr[vec16x32u::width]{};
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

    TEST(Vec16x32u, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            alignas(alignof(vec16x32u)) vec16x32u::scalar arr[vec16x32u::width]{};

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
            aligned_store<0x0e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0e));
            aligned_store<0x0e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0e));
            aligned_store<0x0f>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x0f));

            aligned_store<0x10>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x10));
        }
    }

    TEST(Vec16x32u, Scatter_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;
        static constexpr std::size_t block_size = test_data_size / vec16x32u::width;

        std::vector<vec16x32u::scalar> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr16x32u>();

            vec16x32u input0{input_array0};

            arr16x32i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random_val<vec16x32u::scalar>() % block_size) + (j * block_size);
            }

            vec16x32i indices{index_array};

            for (std::size_t j = 0; j <= vec16x32u::width; ++j) {
                scatter(test_data.data(), input0, indices, j);

                for (std::size_t k = 0; k < input_array0.size(); ++k) {
                    if (k < j) {
                        EXPECT_EQ(input_array0[k], test_data[index_array[k]]);
                    }
                }
            }
        }
    }

    TEST(Vec16x32u, Scatter_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;
        static constexpr std::size_t block_size = test_data_size / vec16x32u::width;

        std::vector<vec16x32u::scalar> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr16x32u>();

            vec16x32u input0{input_array0};

            arr16x32i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random_val<vec16x32u::scalar>() % block_size) + (j * block_size);
            }

            vec16x32i indices{index_array};

            scatter<0x00>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x00));
            scatter<0x01>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x01));
            scatter<0x02>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x02));
            scatter<0x03>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x03));
            scatter<0x04>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x04));
            scatter<0x05>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x05));
            scatter<0x06>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x06));
            scatter<0x07>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x07));
            scatter<0x08>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x08));
            scatter<0x09>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x09));
            scatter<0x0a>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x0a));
            scatter<0x0b>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x0b));
            scatter<0x0c>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x0c));
            scatter<0x0e>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x0e));
            scatter<0x0e>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x0e));
            scatter<0x0f>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x0f));

            scatter<0x10>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x10));
        }
    }

    TEST(Vec16x32u, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec16x32u, Div_uniform) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto numerator = random_val<vec16x32u::scalar>();
            auto denominator = random_denominator<vec16x32u::scalar>();

            auto quotient  = (numerator / denominator);
            auto remainder = (numerator % denominator);

            vec16x32u n{numerator};
            vec16x32u d{denominator};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < q.size(); ++j) {
                EXPECT_EQ(q[j], quotient);
                EXPECT_EQ(r[j], remainder);
            }
        }
    }

    TEST(Vec16x32u, Div_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto numerators = random_array<arr16x32u>();
            auto denominators = random_denominator_array<arr16x32u>();

            arr16x32u quotients{};
            arr16x32u remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec16x32u n{numerators};
            vec16x32u d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec16x32u, Popcount_edge_cases) {
        vec16x32u v{0x0};
        vec16x32u c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec16x32u, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = popcount(v);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = popcount(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = byteswap(v);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = byteswap(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Countl_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec16x32u::scalar>::value - 1); ++i) {
            vec16x32u::scalar v = (vec16x32u::scalar(0x1) << i) - 1;
            auto results = countl_zero(vec16x32u{v});
            auto expected = vec16x32u(type_width<vec16x32u::scalar>::value - i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countl_zero(vec16x32u(-1)) == vec16x32u{0}));
    }

    TEST(Vec16x32u, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = countl_zero(v);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Countl_one_edge_cases) {
        EXPECT_TRUE(all(countl_one(vec16x32u{0x00}) == vec16x32u{0}));
        for (std::size_t i = 0; i < (type_width<vec16x32u::scalar>::value - 1); ++i) {
            vec16x32u::scalar v = vec16x32u::scalar(-1) << i;
            auto results = countl_one(vec16x32u{v});
            auto expected = vec16x32u(type_width<vec16x32u::scalar>::value - i);
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec16x32u, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = countl_one(v);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Countr_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec16x32u::scalar>::value - 1); ++i) {
            vec16x32u::scalar v = vec16x32u::scalar(-1) << i;
            auto results = countr_zero(vec16x32u{v});
            auto expected = vec16x32u(i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countr_zero(vec16x32u{0x00}) == vec16x32u{type_width<vec16x32u::scalar>::value}));
    }

    TEST(Vec16x32u, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = countr_zero(v);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Countr_one_edge_cases) {
        EXPECT_TRUE(all(countr_one(vec16x32u{0x00}) == vec16x32u{0}));

        for (std::size_t i = 0; i < (type_width<vec16x32u::scalar>::value - 1); ++i) {
            vec16x32u::scalar v = (vec16x32u::scalar(1) << i) - 1;
            auto results = countr_one(vec16x32u{v});
            auto expected = vec16x32u(i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countr_one(vec16x32u{vec16x32u::scalar(-1)}) == vec16x32u{type_width<vec16x32u::scalar>::value}));
    }

    TEST(Vec16x32u, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = countr_one(v);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Bit_width_edge_cases) {
        EXPECT_TRUE(all(bit_width(vec16x32u{0x00}) == vec16x32u{0x00}));

        for (std::size_t i = 0; i < type_width<vec16x32u::scalar>::value; ++i) {
            vec16x32u::scalar v = vec16x32u::scalar(1) << i;
            auto results = bit_width(vec16x32u{v});
            auto expected = vec16x32u(i + 1);
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec16x32u, Bit_width_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = bit_width(v);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_width(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Bit_floor_edge_cases) {
        EXPECT_TRUE(all(vec16x32u{0} == bit_floor(vec16x32u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec16x32u::scalar>::value - 1); ++i) {
            vec16x32u::scalar v = vec16x32u::scalar(1) << i;
            auto results = bit_floor(vec16x32u{v});
            auto expected = vec16x32u{v};
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec16x32u, Bit_floor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = bit_floor(v);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_floor(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Bit_ceil_edge_cases) {
        EXPECT_TRUE(all(vec16x32u{1} == bit_ceil(vec16x32u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec16x32u::scalar>::value - 1); ++i) {
            vec16x32u::scalar v = vec16x32u::scalar(1) << i;
            auto results = bit_ceil(vec16x32u{v});
            auto expected = vec16x32u{v};
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec16x32u, Bit_ceil_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = bit_ceil(v);

            arr16x32u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_ceil(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec16x32u{expected}));
        }
    }

    TEST(Vec16x32u, Has_single_bit_edge_cases) {
        EXPECT_TRUE(none(has_single_bit(vec16x32u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec16x32u::scalar>::value - 1); ++i) {
            vec16x32u::scalar v = vec16x32u::scalar(1) << i;
            auto results = has_single_bit(vec16x32u{v});
            EXPECT_TRUE(all(results));
        }
    }

    TEST(Vec16x32u, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr16x32u>();

            vec16x32u v{inputs};

            auto results = has_single_bit(v);

            arr16xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = has_single_bit(inputs[j]);
            }

            EXPECT_TRUE(results == mask16x32u{expected});
        }
    }

}

#endif //AVEL_VEC16X32U_TESTS_HPP
