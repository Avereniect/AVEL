#ifndef AVEL_VEC64X8U_TESTS_HPP
#define AVEL_VEC64X8U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=========================================================================
    // mask64x8u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask64x8u, Construct_from_primitive) {
        mask64x8u mask{mask64x8u::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask64x8u, Construct_from_bool) {
        mask64x8u mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask64x8u mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask64x8u::width);
    }

    TEST(Mask64x8u, Construct_from_array) {
        arr64xb false_array{};
        std::fill_n(false_array.data(), mask64x8u::width, false);
        mask64x8u mask0{false_array};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);


        arr64xb true_array{};
        std::fill_n(true_array.data(), mask64x8u::width, true);
        mask64x8u mask1{true_array};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask64x8u::width);
    }

    TEST(Mask64x8u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64xb>();
            std::uint32_t cnt = std::count(inputs.begin(), inputs.end(), true);

            auto m = mask64x8u{inputs};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == mask64x8u::width, all(m));
            EXPECT_EQ(cnt != 0, any(m));
            EXPECT_EQ(cnt == 0, none(m));
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask64x8u, Assign_bool) {
        mask64x8u mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask64x8u mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(count(mask1) == mask64x8u::width);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask64x8u, Equality_comparison) {
        mask64x8u mask0{false};
        mask64x8u mask1{false};
        EXPECT_TRUE(mask0 == mask1);

        mask64x8u mask2{true};
        mask64x8u mask3{true};
        EXPECT_TRUE(mask2 == mask3);

        EXPECT_FALSE(mask0 == mask2);
        EXPECT_FALSE(mask3 == mask1);
    }

    TEST(Mask64x8u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64xb>();
            inputs1[0] = !inputs0[0];

            mask64x8u mask0{inputs0};
            mask64x8u mask1{inputs0};
            mask64x8u mask2{inputs1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask64x8u, Inequality_comparison) {
        mask64x8u mask0{false};
        mask64x8u mask1{false};
        EXPECT_FALSE(mask0 != mask1);

        mask64x8u mask2{true};
        mask64x8u mask3{true};
        EXPECT_FALSE(mask2 != mask3);

        EXPECT_TRUE(mask0 != mask2);
        EXPECT_TRUE(mask3 != mask1);
    }

    TEST(Mask64x8u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64xb>();
            inputs1[0] = !inputs0[0];

            mask64x8u mask0{inputs0};
            mask64x8u mask1{inputs1};
            mask64x8u mask2{inputs1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask64x8u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64xb>();

            mask64x8u mask0{inputs0};
            mask64x8u mask1{inputs1};

            mask64x8u results = mask0;
            results &= mask1;

            arr64xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask64x8u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask64x8u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64xb>();

            mask64x8u mask0{inputs0};
            mask64x8u mask1{inputs1};

            mask64x8u results = mask0;
            results |= mask1;

            arr64xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask64x8u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask64x8u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64xb>();

            mask64x8u mask0{inputs0};
            mask64x8u mask1{inputs1};

            mask64x8u results = mask0;
            results ^= mask1;

            arr64xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask64x8u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask64x8u, Logical_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64xb>();

            mask64x8u mask0{inputs};

            mask64x8u results = !mask0;

            arr64xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = !inputs[j];
            }
            mask64x8u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask64x8u, Bitwise_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64xb>();

            mask64x8u mask0{inputs0};
            mask64x8u mask1{inputs1};

            mask64x8u results = mask0 & mask1;

            arr64xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask64x8u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask64x8u, Logical_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64xb>();

            mask64x8u mask0{inputs0};
            mask64x8u mask1{inputs1};

            mask64x8u results = mask0 && mask1;

            arr64xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] && inputs1[j];
            }
            mask64x8u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask64x8u, Bitwise_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64xb>();

            mask64x8u mask0{inputs0};
            mask64x8u mask1{inputs1};

            mask64x8u results = mask0 | mask1;

            arr64xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask64x8u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask64x8u, Logical_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64xb>();

            mask64x8u mask0{inputs0};
            mask64x8u mask1{inputs1};

            mask64x8u results = mask0 || mask1;

            arr64xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] || inputs1[j];
            }
            mask64x8u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask64x8u, Bitwise_xor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64xb>();

            mask64x8u mask0{inputs0};
            mask64x8u mask1{inputs1};

            mask64x8u results = mask0 ^ mask1;

            arr64xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask64x8u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=========================================================================
    // Vec64x8u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec64x8u, Construct_vector_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64xb>();

            mask64x8u m0{inputs};
            vec64x8u v0{m0};

            auto results = to_array(v0);
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    TEST(Vec64x8u, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto x = random_val<vec64x8u::scalar>();
            vec64x8u results{x};

            arr64x8u arr;
            std::fill_n(arr.data(), vec64x8u::width, x);
            vec64x8u expected{arr};

            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec64x8u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(inputs[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec64x8u, Assign_primitive) {
        vec64x8u::primitive x{};
        vec64x8u v{0x0F};
        v = x;
        vec64x8u expected{};

        EXPECT_TRUE(all(v == expected));
    }

    TEST(Vec64x8u, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto scalar = random_val<vec64x8u::scalar>();
            vec64x8u v;
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

    TEST(Vec64x8u, Equality_comparison) {
        mask64x8u mask0{false};
        mask64x8u mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask64x8u mask2{true};
        mask64x8u mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec64x8u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();
            inputs1[0] = ~inputs0[0];

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs0};
            vec64x8u v2{inputs1};

            EXPECT_TRUE(all(v0 == v1));
            EXPECT_FALSE(all(v1 == v2));
        }
    }

    TEST(Vec64x8u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();
            inputs1[0] = ~inputs0[0];

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs0};
            vec64x8u v2{inputs1};

            EXPECT_FALSE(any(v0 != v1));
            EXPECT_TRUE(any(v1 != v2));
        }
    }

    TEST(Vec64x8u, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = (v0 < v1);

            arr64xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] < inputs1[j];
            }

            EXPECT_EQ(results, mask64x8u{expected});
        }
    }

    TEST(Vec64x8u, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = (v0 <= v1);

            arr64xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] <= inputs1[j];
            }

            EXPECT_EQ(results, mask64x8u{expected});
        }
    }

    TEST(Vec64x8u, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = (v0 > v1);

            arr64xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] > inputs1[j];
            }

            EXPECT_EQ(results, mask64x8u{expected});
        }
    }

    TEST(Vec64x8u, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = (v0 >= v1);

            arr64xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] >= inputs1[j];
            }

            EXPECT_EQ(results, mask64x8u{expected});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec64x8u, Unary_plus) {
        auto inputs = random_array<arr64x8u>();
        const vec64x8u v{inputs};
        vec64x8u results{+v};

        EXPECT_TRUE(all(results == v));
    }

    TEST(Vec64x8u, Unary_minus) {
        auto inputs = random_array<arr64x8u>();
        const vec64x8u v{inputs};

        auto results = -v;

        arr64x8i expected{};
        for (std::size_t j = 0; j < expected.size(); ++j) {
            expected[j] = -inputs[j];
        }

        EXPECT_TRUE(all(results == vec64x8i{expected}));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec64x8u, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0;
            results += v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0;
            results -= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0;
            results *= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_denominator_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0;
            results /= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_denominator_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0;
            results %= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec64x8u, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0 + v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0 - v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0 * v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_denominator_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0 / v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_denominator_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0 % v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec64x8u, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};
            ++v;

            auto results = v;
            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};
            v++;

            auto results = v;
            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};
            --v;

            auto results = v;
            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};
            v--;

            auto results = v;
            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec64x8u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0;
            results &= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0;
            results |= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0;
            results ^= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            auto v1 = random_shift<vec64x8u::scalar>();

            auto results = v0;
            results <<= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec64x8u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_shift_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0;
            results <<= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec64x8u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            auto v1 = random_shift<vec64x8u::scalar>();

            auto results = v0;
            results >>= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec64x8u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_shift_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0;
            results >>= v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec64x8u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec64x8u, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = ~v;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ~inputs[j];
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            auto v1 = random_shift<vec64x8u::scalar>();

            auto results = v0 << v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec64x8u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_shift_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0 << v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec64x8u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            auto v1 = random_shift<vec64x8u::scalar>();

            auto results = v0 >> v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec64x8u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_shift_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = v0 >> v1;

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec64x8u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec64x8u, Conversion_to_mask) {
        vec64x8u vec{0};

        auto a = vec.operator mask64x8u();
        mask64x8u b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec64x8u, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = mask64x8u{v};

            arr64xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = inputs[j];
            }

            EXPECT_TRUE(results == mask64x8u{expected});
        }
    }

    //=====================================================
    // Arrangement Operations
    //=====================================================

    TEST(Vec64x8u, Extract_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();
            vec64x8u v{inputs};

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
            EXPECT_EQ(inputs[0x20], extract<0x20>(v));
            EXPECT_EQ(inputs[0x21], extract<0x21>(v));
            EXPECT_EQ(inputs[0x22], extract<0x22>(v));
            EXPECT_EQ(inputs[0x23], extract<0x23>(v));
            EXPECT_EQ(inputs[0x24], extract<0x24>(v));
            EXPECT_EQ(inputs[0x25], extract<0x25>(v));
            EXPECT_EQ(inputs[0x26], extract<0x26>(v));
            EXPECT_EQ(inputs[0x27], extract<0x27>(v));
            EXPECT_EQ(inputs[0x28], extract<0x28>(v));
            EXPECT_EQ(inputs[0x29], extract<0x29>(v));
            EXPECT_EQ(inputs[0x2a], extract<0x2a>(v));
            EXPECT_EQ(inputs[0x2b], extract<0x2b>(v));
            EXPECT_EQ(inputs[0x2c], extract<0x2c>(v));
            EXPECT_EQ(inputs[0x2d], extract<0x2d>(v));
            EXPECT_EQ(inputs[0x2e], extract<0x2e>(v));
            EXPECT_EQ(inputs[0x2f], extract<0x2f>(v));
            EXPECT_EQ(inputs[0x30], extract<0x30>(v));
            EXPECT_EQ(inputs[0x31], extract<0x31>(v));
            EXPECT_EQ(inputs[0x32], extract<0x32>(v));
            EXPECT_EQ(inputs[0x33], extract<0x33>(v));
            EXPECT_EQ(inputs[0x34], extract<0x34>(v));
            EXPECT_EQ(inputs[0x35], extract<0x35>(v));
            EXPECT_EQ(inputs[0x36], extract<0x36>(v));
            EXPECT_EQ(inputs[0x37], extract<0x37>(v));
            EXPECT_EQ(inputs[0x38], extract<0x38>(v));
            EXPECT_EQ(inputs[0x39], extract<0x39>(v));
            EXPECT_EQ(inputs[0x3a], extract<0x3a>(v));
            EXPECT_EQ(inputs[0x3b], extract<0x3b>(v));
            EXPECT_EQ(inputs[0x3c], extract<0x3c>(v));
            EXPECT_EQ(inputs[0x3d], extract<0x3d>(v));
            EXPECT_EQ(inputs[0x3e], extract<0x3e>(v));
            EXPECT_EQ(inputs[0x3f], extract<0x3f>(v));

        }
    }

    TEST(Vec64x8u, Insert_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();
            vec64x8u v{random_val<vec64x8u::scalar>()};

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
            v = insert<0x20>(v, inputs[0x20]);
            v = insert<0x21>(v, inputs[0x21]);
            v = insert<0x22>(v, inputs[0x22]);
            v = insert<0x23>(v, inputs[0x23]);
            v = insert<0x24>(v, inputs[0x24]);
            v = insert<0x25>(v, inputs[0x25]);
            v = insert<0x26>(v, inputs[0x26]);
            v = insert<0x27>(v, inputs[0x27]);
            v = insert<0x28>(v, inputs[0x28]);
            v = insert<0x29>(v, inputs[0x29]);
            v = insert<0x2a>(v, inputs[0x2a]);
            v = insert<0x2b>(v, inputs[0x2b]);
            v = insert<0x2c>(v, inputs[0x2c]);
            v = insert<0x2d>(v, inputs[0x2d]);
            v = insert<0x2e>(v, inputs[0x2e]);
            v = insert<0x2f>(v, inputs[0x2f]);
            v = insert<0x30>(v, inputs[0x30]);
            v = insert<0x31>(v, inputs[0x31]);
            v = insert<0x32>(v, inputs[0x32]);
            v = insert<0x33>(v, inputs[0x33]);
            v = insert<0x34>(v, inputs[0x34]);
            v = insert<0x35>(v, inputs[0x35]);
            v = insert<0x36>(v, inputs[0x36]);
            v = insert<0x37>(v, inputs[0x37]);
            v = insert<0x38>(v, inputs[0x38]);
            v = insert<0x39>(v, inputs[0x39]);
            v = insert<0x3a>(v, inputs[0x3a]);
            v = insert<0x3b>(v, inputs[0x3b]);
            v = insert<0x3c>(v, inputs[0x3c]);
            v = insert<0x3d>(v, inputs[0x3d]);
            v = insert<0x3e>(v, inputs[0x3e]);
            v = insert<0x3f>(v, inputs[0x3f]);


            EXPECT_TRUE(all(v == vec64x8u{inputs}));
        }
    }

    //=====================================================
    // Bit Manipulation Instructions
    //=====================================================

    TEST(Vec64x8u, Bit_shift_left) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            EXPECT_TRUE(all(bit_shift_left<0x00>(v) == (v << 0x00)));
            EXPECT_TRUE(all(bit_shift_left<0x01>(v) == (v << 0x01)));
            EXPECT_TRUE(all(bit_shift_left<0x02>(v) == (v << 0x02)));
            EXPECT_TRUE(all(bit_shift_left<0x03>(v) == (v << 0x03)));
            EXPECT_TRUE(all(bit_shift_left<0x04>(v) == (v << 0x04)));
            EXPECT_TRUE(all(bit_shift_left<0x05>(v) == (v << 0x05)));
            EXPECT_TRUE(all(bit_shift_left<0x06>(v) == (v << 0x06)));
            EXPECT_TRUE(all(bit_shift_left<0x07>(v) == (v << 0x07)));
            EXPECT_TRUE(all(bit_shift_left<0x08>(v) == (v << 0x08)));

        }
    }

    TEST(Vec64x8u, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            EXPECT_TRUE(all(bit_shift_right<0x00>(v) == (v >> 0x00)));
            EXPECT_TRUE(all(bit_shift_right<0x01>(v) == (v >> 0x01)));
            EXPECT_TRUE(all(bit_shift_right<0x02>(v) == (v >> 0x02)));
            EXPECT_TRUE(all(bit_shift_right<0x03>(v) == (v >> 0x03)));
            EXPECT_TRUE(all(bit_shift_right<0x04>(v) == (v >> 0x04)));
            EXPECT_TRUE(all(bit_shift_right<0x05>(v) == (v >> 0x05)));
            EXPECT_TRUE(all(bit_shift_right<0x06>(v) == (v >> 0x06)));
            EXPECT_TRUE(all(bit_shift_right<0x07>(v) == (v >> 0x07)));
            EXPECT_TRUE(all(bit_shift_right<0x08>(v) == (v >> 0x08)));

        }
    }

    TEST(Vec64x8u, Rotl) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            EXPECT_TRUE(all(rotl<0x00>(v) == (rotl(v, 0x00))));
            EXPECT_TRUE(all(rotl<0x01>(v) == (rotl(v, 0x01))));
            EXPECT_TRUE(all(rotl<0x02>(v) == (rotl(v, 0x02))));
            EXPECT_TRUE(all(rotl<0x03>(v) == (rotl(v, 0x03))));
            EXPECT_TRUE(all(rotl<0x04>(v) == (rotl(v, 0x04))));
            EXPECT_TRUE(all(rotl<0x05>(v) == (rotl(v, 0x05))));
            EXPECT_TRUE(all(rotl<0x06>(v) == (rotl(v, 0x06))));
            EXPECT_TRUE(all(rotl<0x07>(v) == (rotl(v, 0x07))));
            EXPECT_TRUE(all(rotl<0x08>(v) == (rotl(v, 0x08))));

        }
    }

    TEST(Vec64x8u, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            auto v1 = random64u();

            auto results = rotl(v0, v1);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = rotl(v0, v1);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Rotr) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();

            vec64x8u v{inputs0};

            EXPECT_TRUE(all(rotr<0x00>(v) == (rotr(v, 0x00))));
            EXPECT_TRUE(all(rotr<0x01>(v) == (rotr(v, 0x01))));
            EXPECT_TRUE(all(rotr<0x02>(v) == (rotr(v, 0x02))));
            EXPECT_TRUE(all(rotr<0x03>(v) == (rotr(v, 0x03))));
            EXPECT_TRUE(all(rotr<0x04>(v) == (rotr(v, 0x04))));
            EXPECT_TRUE(all(rotr<0x05>(v) == (rotr(v, 0x05))));
            EXPECT_TRUE(all(rotr<0x06>(v) == (rotr(v, 0x06))));
            EXPECT_TRUE(all(rotr<0x07>(v) == (rotr(v, 0x07))));
            EXPECT_TRUE(all(rotr<0x08>(v) == (rotr(v, 0x08))));

        }
    }

    TEST(Vec64x8u, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            auto v1 = random64u();

            auto results = rotr(v0, v1);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = rotr(v0, v1);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec64x8u, Count_random) {
        auto predicate = [] (vec64x8u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec64x8u v{inputs};

            auto results = count(v);

            EXPECT_EQ(cnt, results);
        }
    }

    TEST(Vec64x8u, Any_edge_cases) {
        EXPECT_FALSE(any(vec64x8u{0x00}));
        EXPECT_TRUE(any(vec64x8u{0x01}));
    }

    TEST(Vec64x8u, Any_random) {
        auto predicate = [] (vec64x8u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec64x8u v{inputs};

            EXPECT_EQ(cnt != 0, any(v));
        }
    }

    TEST(Vec64x8u, All_edge_cases) {
        EXPECT_FALSE(all(vec64x8u{0x00}));
        EXPECT_TRUE(all(vec64x8u{0x01}));
    }

    TEST(Vec64x8u, All_random) {
        auto predicate = [] (vec64x8u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec64x8u v{inputs};

            auto results = all(v);

            EXPECT_EQ(cnt == vec64x8u::width, results);
        }
    }

    TEST(Vec64x8u, None_edge_cases) {
        EXPECT_TRUE(none(vec64x8u{0x00}));
        EXPECT_FALSE(none(vec64x8u{0x01}));
    }

    TEST(Vec64x8u, None_random) {
        auto predicate = [] (vec64x8u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec64x8u v{inputs};

            EXPECT_EQ(cnt == 0, none(v));
        }
    }

    TEST(Vec64x8u, Set_bits_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64xb>();

            mask64x8u v{inputs};

            auto results = set_bits(v);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                if (inputs[j]) {
                    expected[j] = vec64x8u::scalar(-1);
                } else {
                    expected[j] = vec64x8u::scalar(0);
                }
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Keep_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64x8u>();

            mask64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = keep(v0, v1);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = keep(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Clear_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64x8u>();

            mask64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = clear(v0, v1);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = clear(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64xb>();
            auto inputs1 = random_array<arr64x8u>();
            auto inputs2 = random_array<arr64x8u>();

            mask64x8u v0{inputs0};
            vec64x8u v1{inputs1};
            vec64x8u v2{inputs2};

            auto results = blend(v0, v1, v2);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = blend(inputs0[j], inputs1[j], inputs2[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = max(v0, v1);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = max(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = min(v0, v1);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = min(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = minmax(v0, v1);

            arr64x8u expected0{};
            arr64x8u expected1{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto min_max = minmax(inputs0[j], inputs1[j]);
                expected0[j] = min_max[0];
                expected1[j] = min_max[1];
            }

            EXPECT_TRUE(all(results[0] == vec64x8u{expected0}));
            EXPECT_TRUE(all(results[1] == vec64x8u{expected1}));
        }
    }

    TEST(Vec64x8u, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();
            auto inputs2 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};
            vec64x8u v2{inputs2};

            auto ranges = minmax(v1, v2);

            auto results = clamp(v0, ranges[0], ranges[1]);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto range = minmax(inputs1[j], inputs2[j]);
                expected[j] = clamp(inputs0[j], range[0], range[1]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = average(v0, v1);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = average(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr64x8u>();
            auto inputs1 = random_array<arr64x8u>();

            vec64x8u v0{inputs0};
            vec64x8u v1{inputs1};

            auto results = midpoint(v0, v1);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = midpoint(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = neg_abs(v);

            arr64x8i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = neg_abs(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec64x8i{expected}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec64x8u, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            for (std::size_t j = 0; j <= vec64x8u::width; ++j) {
                auto results = load<vec64x8u>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::size_t k = 0; k < vec64x8u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec64x8u, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            EXPECT_TRUE(all(load<vec64x8u, 0x00>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(load<vec64x8u, 0x00>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(load<vec64x8u, 0x01>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(load<vec64x8u, 0x02>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x02)));
            EXPECT_TRUE(all(load<vec64x8u, 0x03>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x03)));
            EXPECT_TRUE(all(load<vec64x8u, 0x04>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x04)));
            EXPECT_TRUE(all(load<vec64x8u, 0x05>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x05)));
            EXPECT_TRUE(all(load<vec64x8u, 0x06>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x06)));
            EXPECT_TRUE(all(load<vec64x8u, 0x07>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x07)));
            EXPECT_TRUE(all(load<vec64x8u, 0x08>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x08)));
            EXPECT_TRUE(all(load<vec64x8u, 0x09>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x09)));
            EXPECT_TRUE(all(load<vec64x8u, 0x0a>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x0a)));
            EXPECT_TRUE(all(load<vec64x8u, 0x0b>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x0b)));
            EXPECT_TRUE(all(load<vec64x8u, 0x0c>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x0c)));
            EXPECT_TRUE(all(load<vec64x8u, 0x0d>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x0d)));
            EXPECT_TRUE(all(load<vec64x8u, 0x0e>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x0e)));
            EXPECT_TRUE(all(load<vec64x8u, 0x0f>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x0f)));
            EXPECT_TRUE(all(load<vec64x8u, 0x10>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x10)));
            EXPECT_TRUE(all(load<vec64x8u, 0x11>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x11)));
            EXPECT_TRUE(all(load<vec64x8u, 0x12>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x12)));
            EXPECT_TRUE(all(load<vec64x8u, 0x13>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x13)));
            EXPECT_TRUE(all(load<vec64x8u, 0x14>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x14)));
            EXPECT_TRUE(all(load<vec64x8u, 0x15>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x15)));
            EXPECT_TRUE(all(load<vec64x8u, 0x16>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x16)));
            EXPECT_TRUE(all(load<vec64x8u, 0x17>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x17)));
            EXPECT_TRUE(all(load<vec64x8u, 0x18>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x18)));
            EXPECT_TRUE(all(load<vec64x8u, 0x19>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x19)));
            EXPECT_TRUE(all(load<vec64x8u, 0x1a>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x1a)));
            EXPECT_TRUE(all(load<vec64x8u, 0x1b>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x1b)));
            EXPECT_TRUE(all(load<vec64x8u, 0x1c>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x1c)));
            EXPECT_TRUE(all(load<vec64x8u, 0x1d>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x1d)));
            EXPECT_TRUE(all(load<vec64x8u, 0x1e>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x1e)));
            EXPECT_TRUE(all(load<vec64x8u, 0x1f>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x1f)));
            EXPECT_TRUE(all(load<vec64x8u, 0x20>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x20)));
            EXPECT_TRUE(all(load<vec64x8u, 0x21>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x21)));
            EXPECT_TRUE(all(load<vec64x8u, 0x22>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x22)));
            EXPECT_TRUE(all(load<vec64x8u, 0x23>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x23)));
            EXPECT_TRUE(all(load<vec64x8u, 0x24>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x24)));
            EXPECT_TRUE(all(load<vec64x8u, 0x25>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x25)));
            EXPECT_TRUE(all(load<vec64x8u, 0x26>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x26)));
            EXPECT_TRUE(all(load<vec64x8u, 0x27>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x27)));
            EXPECT_TRUE(all(load<vec64x8u, 0x28>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x28)));
            EXPECT_TRUE(all(load<vec64x8u, 0x29>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x29)));
            EXPECT_TRUE(all(load<vec64x8u, 0x2a>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x2a)));
            EXPECT_TRUE(all(load<vec64x8u, 0x2b>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x2b)));
            EXPECT_TRUE(all(load<vec64x8u, 0x2c>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x2c)));
            EXPECT_TRUE(all(load<vec64x8u, 0x2d>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x2d)));
            EXPECT_TRUE(all(load<vec64x8u, 0x2e>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x2e)));
            EXPECT_TRUE(all(load<vec64x8u, 0x2f>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x2f)));
            EXPECT_TRUE(all(load<vec64x8u, 0x30>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x30)));
            EXPECT_TRUE(all(load<vec64x8u, 0x31>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x31)));
            EXPECT_TRUE(all(load<vec64x8u, 0x32>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x32)));
            EXPECT_TRUE(all(load<vec64x8u, 0x33>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x33)));
            EXPECT_TRUE(all(load<vec64x8u, 0x34>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x34)));
            EXPECT_TRUE(all(load<vec64x8u, 0x35>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x35)));
            EXPECT_TRUE(all(load<vec64x8u, 0x36>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x36)));
            EXPECT_TRUE(all(load<vec64x8u, 0x37>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x37)));
            EXPECT_TRUE(all(load<vec64x8u, 0x38>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x38)));
            EXPECT_TRUE(all(load<vec64x8u, 0x39>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x39)));
            EXPECT_TRUE(all(load<vec64x8u, 0x3a>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x3a)));
            EXPECT_TRUE(all(load<vec64x8u, 0x3b>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x3b)));
            EXPECT_TRUE(all(load<vec64x8u, 0x3c>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x3c)));
            EXPECT_TRUE(all(load<vec64x8u, 0x3d>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x3d)));
            EXPECT_TRUE(all(load<vec64x8u, 0x3e>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x3e)));
            EXPECT_TRUE(all(load<vec64x8u, 0x3f>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x3f)));
            EXPECT_TRUE(all(load<vec64x8u, 0x40>(inputs.data()) == load<vec64x8u>(inputs.data(), 0x40)));

        }
    }

    TEST(Vec64x8u, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec64x8u)) auto inputs = random_array<arr64x8u>();

            for (std::size_t j = 0; j <= vec64x8u::width; ++j) {
                auto results = aligned_load<vec64x8u>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::size_t k = 0; k < vec64x8u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec64x8u, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec64x8u)) auto inputs = random_array<arr64x8u>();

            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x00>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x01>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x02>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x02)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x03>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x03)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x04>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x04)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x05>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x05)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x06>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x06)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x07>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x07)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x08>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x08)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x09>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x09)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x0a>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x0a)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x0b>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x0b)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x0c>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x0c)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x0d>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x0d)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x0e>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x0e)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x0f>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x0f)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x10>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x10)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x11>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x11)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x12>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x12)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x13>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x13)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x14>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x14)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x15>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x15)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x16>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x16)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x17>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x17)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x18>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x18)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x19>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x19)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x1a>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x1a)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x1b>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x1b)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x1c>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x1c)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x1d>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x1d)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x1e>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x1e)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x1f>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x1f)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x20>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x20)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x21>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x21)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x22>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x22)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x23>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x23)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x24>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x24)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x25>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x25)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x26>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x26)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x27>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x27)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x28>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x28)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x29>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x29)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x2a>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x2a)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x2b>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x2b)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x2c>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x2c)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x2d>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x2d)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x2e>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x2e)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x2f>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x2f)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x30>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x30)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x31>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x31)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x32>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x32)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x33>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x33)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x34>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x34)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x35>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x35)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x36>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x36)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x37>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x37)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x38>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x38)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x39>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x39)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x3a>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x3a)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x3b>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x3b)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x3c>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x3c)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x3d>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x3d)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x3e>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x3e)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x3f>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x3f)));
            EXPECT_TRUE(all(aligned_load<vec64x8u, 0x40>(inputs.data()) == aligned_load<vec64x8u>(inputs.data(), 0x40)));

        }
    }



    TEST(Vec64x8u, Store_n) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            for (std::size_t j = 0; j <= vec64x8u::width; ++j) {
                vec64x8u::scalar arr[vec64x8u::width]{};
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

    TEST(Vec64x8u, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            vec64x8u::scalar arr[vec64x8u::width]{};

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
            store<0x21>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x21));
            store<0x22>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x22));
            store<0x23>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x23));
            store<0x24>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x24));
            store<0x25>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x25));
            store<0x26>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x26));
            store<0x27>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x27));
            store<0x28>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x28));
            store<0x29>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x29));
            store<0x2a>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2a));
            store<0x2b>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2b));
            store<0x2c>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2c));
            store<0x2d>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2d));
            store<0x2e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2e));
            store<0x2f>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2f));
            store<0x30>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x30));
            store<0x31>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x31));
            store<0x32>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x32));
            store<0x33>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x33));
            store<0x34>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x34));
            store<0x35>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x35));
            store<0x36>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x36));
            store<0x37>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x37));
            store<0x38>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x38));
            store<0x39>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x39));
            store<0x3a>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3a));
            store<0x3b>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3b));
            store<0x3c>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3c));
            store<0x3d>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3d));
            store<0x3e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3e));
            store<0x3f>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3f));
            store<0x40>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x40));

        }
    }

    TEST(Vec64x8u, Aligned_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            for (std::size_t j = 0; j <= vec64x8u::width; ++j) {
                alignas(alignof(vec64x8u)) vec64x8u::scalar arr[vec64x8u::width]{};
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

    TEST(Vec64x8u, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            alignas(alignof(vec64x8u)) vec64x8u::scalar arr[vec64x8u::width]{};

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
            aligned_store<0x21>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x21));
            aligned_store<0x22>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x22));
            aligned_store<0x23>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x23));
            aligned_store<0x24>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x24));
            aligned_store<0x25>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x25));
            aligned_store<0x26>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x26));
            aligned_store<0x27>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x27));
            aligned_store<0x28>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x28));
            aligned_store<0x29>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x29));
            aligned_store<0x2a>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2a));
            aligned_store<0x2b>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2b));
            aligned_store<0x2c>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2c));
            aligned_store<0x2d>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2d));
            aligned_store<0x2e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2e));
            aligned_store<0x2f>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x2f));
            aligned_store<0x30>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x30));
            aligned_store<0x31>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x31));
            aligned_store<0x32>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x32));
            aligned_store<0x33>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x33));
            aligned_store<0x34>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x34));
            aligned_store<0x35>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x35));
            aligned_store<0x36>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x36));
            aligned_store<0x37>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x37));
            aligned_store<0x38>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x38));
            aligned_store<0x39>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x39));
            aligned_store<0x3a>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3a));
            aligned_store<0x3b>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3b));
            aligned_store<0x3c>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3c));
            aligned_store<0x3d>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3d));
            aligned_store<0x3e>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3e));
            aligned_store<0x3f>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x3f));
            aligned_store<0x40>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x40));

        }
    }



    TEST(Vec64x8u, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec64x8u, Div_uniform) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto numerator = random_val<vec64x8u::scalar>();
            auto denominator = random_denominator<vec64x8u::scalar>();

            auto quotient  = (numerator / denominator);
            auto remainder = (numerator % denominator);

            vec64x8u n{numerator};
            vec64x8u d{denominator};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < q.size(); ++j) {
                EXPECT_EQ(q[j], quotient);
                EXPECT_EQ(r[j], remainder);
            }
        }
    }

    TEST(Vec64x8u, Div_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto numerators = random_array<arr64x8u>();
            auto denominators = random_denominator_array<arr64x8u>();

            arr64x8u quotients{};
            arr64x8u remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec64x8u n{numerators};
            vec64x8u d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec64x8u, Popcount_edge_cases) {
        vec64x8u v{0x0};
        vec64x8u c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec64x8u, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = popcount(v);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = popcount(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = byteswap(v);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = byteswap(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Countl_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec64x8u::scalar>::value - 1); ++i) {
            vec64x8u::scalar v = (vec64x8u::scalar(0x1) << i) - 1;
            auto results = countl_zero(vec64x8u{v});
            auto expected = vec64x8u(type_width<vec64x8u::scalar>::value - i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countl_zero(vec64x8u(-1)) == vec64x8u{0}));
    }

    TEST(Vec64x8u, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = countl_zero(v);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Countl_one_edge_cases) {
        EXPECT_TRUE(all(countl_one(vec64x8u{0x00}) == vec64x8u{0}));
        for (std::size_t i = 0; i < (type_width<vec64x8u::scalar>::value - 1); ++i) {
            vec64x8u::scalar v = vec64x8u::scalar(-1) << i;
            auto results = countl_one(vec64x8u{v});
            auto expected = vec64x8u(type_width<vec64x8u::scalar>::value - i);
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec64x8u, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = countl_one(v);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Countr_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec64x8u::scalar>::value - 1); ++i) {
            vec64x8u::scalar v = vec64x8u::scalar(-1) << i;
            auto results = countr_zero(vec64x8u{v});
            auto expected = vec64x8u(i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countr_zero(vec64x8u{0x00}) == vec64x8u{type_width<vec64x8u::scalar>::value}));
    }

    TEST(Vec64x8u, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = countr_zero(v);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Countr_one_edge_cases) {
        EXPECT_TRUE(all(countr_one(vec64x8u{0x00}) == vec64x8u{0}));

        for (std::size_t i = 0; i < (type_width<vec64x8u::scalar>::value - 1); ++i) {
            vec64x8u::scalar v = (vec64x8u::scalar(1) << i) - 1;
            auto results = countr_one(vec64x8u{v});
            auto expected = vec64x8u(i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countr_one(vec64x8u{vec64x8u::scalar(-1)}) == vec64x8u{type_width<vec64x8u::scalar>::value}));
    }

    TEST(Vec64x8u, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = countr_one(v);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Bit_width_edge_cases) {
        EXPECT_TRUE(all(bit_width(vec64x8u{0x00}) == vec64x8u{0x00}));

        for (std::size_t i = 0; i < type_width<vec64x8u::scalar>::value; ++i) {
            vec64x8u::scalar v = vec64x8u::scalar(1) << i;
            auto results = bit_width(vec64x8u{v});
            auto expected = vec64x8u(i + 1);
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec64x8u, Bit_width_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = bit_width(v);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_width(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Bit_floor_edge_cases) {
        EXPECT_TRUE(all(vec64x8u{0} == bit_floor(vec64x8u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec64x8u::scalar>::value - 1); ++i) {
            vec64x8u::scalar v = vec64x8u::scalar(1) << i;
            auto results = bit_floor(vec64x8u{v});
            auto expected = vec64x8u{v};
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec64x8u, Bit_floor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = bit_floor(v);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_floor(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Bit_ceil_edge_cases) {
        EXPECT_TRUE(all(vec64x8u{1} == bit_ceil(vec64x8u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec64x8u::scalar>::value - 1); ++i) {
            vec64x8u::scalar v = vec64x8u::scalar(1) << i;
            auto results = bit_ceil(vec64x8u{v});
            auto expected = vec64x8u{v};
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec64x8u, Bit_ceil_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = bit_ceil(v);

            arr64x8u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_ceil(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec64x8u{expected}));
        }
    }

    TEST(Vec64x8u, Has_single_bit_edge_cases) {
        EXPECT_TRUE(none(has_single_bit(vec64x8u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec64x8u::scalar>::value - 1); ++i) {
            vec64x8u::scalar v = vec64x8u::scalar(1) << i;
            auto results = has_single_bit(vec64x8u{v});
            EXPECT_TRUE(all(results));
        }
    }

    TEST(Vec64x8u, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr64x8u>();

            vec64x8u v{inputs};

            auto results = has_single_bit(v);

            arr64xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = has_single_bit(inputs[j]);
            }

            EXPECT_TRUE(results == mask64x8u{expected});
        }
    }

}

#endif
