#ifndef AVEL_VEC1X16U_TESTS_HPP
#define AVEL_VEC1X16U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=========================================================================
    // mask1x16u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask1x16u, Construct_from_primitive) {
        mask1x16u mask{mask1x16u::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask1x16u, Construct_from_bool) {
        mask1x16u mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask1x16u mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask1x16u::width);
    }

    TEST(Mask1x16u, Construct_from_array) {
        arr1xb false_array{};
        std::fill_n(false_array.data(), mask1x16u::width, false);
        mask1x16u mask0{false_array};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);


        arr1xb true_array{};
        std::fill_n(true_array.data(), mask1x16u::width, true);
        mask1x16u mask1{true_array};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask1x16u::width);
    }

    TEST(Mask1x16u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1xb>();
            std::uint32_t cnt = std::count(inputs.begin(), inputs.end(), true);

            auto m = mask1x16u{inputs};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == mask1x16u::width, all(m));
            EXPECT_EQ(cnt != 0, any(m));
            EXPECT_EQ(cnt == 0, none(m));
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask1x16u, Assign_bool) {
        mask1x16u mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask1x16u mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(count(mask1) == mask1x16u::width);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask1x16u, Equality_comparison) {
        mask1x16u mask0{false};
        mask1x16u mask1{false};
        EXPECT_TRUE(mask0 == mask1);

        mask1x16u mask2{true};
        mask1x16u mask3{true};
        EXPECT_TRUE(mask2 == mask3);

        EXPECT_FALSE(mask0 == mask2);
        EXPECT_FALSE(mask3 == mask1);
    }

    TEST(Mask1x16u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1xb>();
            inputs1[0] = !inputs0[0];

            mask1x16u mask0{inputs0};
            mask1x16u mask1{inputs0};
            mask1x16u mask2{inputs1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask1x16u, Inequality_comparison) {
        mask1x16u mask0{false};
        mask1x16u mask1{false};
        EXPECT_FALSE(mask0 != mask1);

        mask1x16u mask2{true};
        mask1x16u mask3{true};
        EXPECT_FALSE(mask2 != mask3);

        EXPECT_TRUE(mask0 != mask2);
        EXPECT_TRUE(mask3 != mask1);
    }

    TEST(Mask1x16u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1xb>();
            inputs1[0] = !inputs0[0];

            mask1x16u mask0{inputs0};
            mask1x16u mask1{inputs1};
            mask1x16u mask2{inputs1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask1x16u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1xb>();

            mask1x16u mask0{inputs0};
            mask1x16u mask1{inputs1};

            mask1x16u results = mask0;
            results &= mask1;

            arr1xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask1x16u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask1x16u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1xb>();

            mask1x16u mask0{inputs0};
            mask1x16u mask1{inputs1};

            mask1x16u results = mask0;
            results |= mask1;

            arr1xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask1x16u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask1x16u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1xb>();

            mask1x16u mask0{inputs0};
            mask1x16u mask1{inputs1};

            mask1x16u results = mask0;
            results ^= mask1;

            arr1xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask1x16u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask1x16u, Logical_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1xb>();

            mask1x16u mask0{inputs};

            mask1x16u results = !mask0;

            arr1xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = !inputs[j];
            }
            mask1x16u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask1x16u, Bitwise_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1xb>();

            mask1x16u mask0{inputs0};
            mask1x16u mask1{inputs1};

            mask1x16u results = mask0 & mask1;

            arr1xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask1x16u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask1x16u, Logical_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1xb>();

            mask1x16u mask0{inputs0};
            mask1x16u mask1{inputs1};

            mask1x16u results = mask0 && mask1;

            arr1xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] && inputs1[j];
            }
            mask1x16u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask1x16u, Bitwise_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1xb>();

            mask1x16u mask0{inputs0};
            mask1x16u mask1{inputs1};

            mask1x16u results = mask0 | mask1;

            arr1xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask1x16u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask1x16u, Logical_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1xb>();

            mask1x16u mask0{inputs0};
            mask1x16u mask1{inputs1};

            mask1x16u results = mask0 || mask1;

            arr1xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] || inputs1[j];
            }
            mask1x16u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask1x16u, Bitwise_xor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1xb>();

            mask1x16u mask0{inputs0};
            mask1x16u mask1{inputs1};

            mask1x16u results = mask0 ^ mask1;

            arr1xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask1x16u expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=====================================================
    // Mask functions
    //=====================================================

    TEST(Mask1x16u, Count_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1xb>();
            auto cnt = std::count(inputs.begin(), inputs.end(), true);

            mask1x16u m{inputs};

            auto results = count(m);

            EXPECT_EQ(cnt, results);
        }

    }

    TEST(Mask1x16u, Any_edge_cases) {
        EXPECT_FALSE(any(mask1x16u{false}));
        EXPECT_TRUE(any(mask1x16u{true}));
    }

    TEST(Mask1x16u, Any_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1xb>();
            auto cnt = std::count(inputs.begin(), inputs.end(), true);

            mask1x16u v{inputs};

            EXPECT_EQ(cnt != 0, any(v));
        }
    }

    TEST(Mask1x16u, All_edge_cases) {
        EXPECT_FALSE(all(mask1x16u{false}));
        EXPECT_TRUE(all(mask1x16u{true}));
    }

    TEST(Mask1x16u, All_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1xb>();
            auto cnt = std::count(inputs.begin(), inputs.end(), true);

            mask1x16u v{inputs};

            EXPECT_EQ(cnt == vec1x16u::width, all(v));
        }
    }

    TEST(Mask1x16u, None_edge_cases) {
        EXPECT_FALSE(all(mask1x16u{false}));
        EXPECT_TRUE(all(mask1x16u{true}));
    }

    TEST(Mask1x16u, None_random) {

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1xb>();
            auto cnt = std::count(inputs.begin(), inputs.end(), true);

            mask1x16u v{inputs};

            EXPECT_EQ(cnt == 0, none(v));
        }
    }

    TEST(Mask1x16u, Extract_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1xb>();
            mask1x16u v{inputs};

            EXPECT_EQ(inputs[0x00], extract<0x00>(v));

        }
    }

    TEST(Mask1x16u, Insert_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1xb>();
            mask1x16u v{false};

            v = insert<0x00>(v, inputs[0x00]);


            EXPECT_TRUE(v == mask1x16u{inputs});
        }
    }

    //=========================================================================
    // Vec1x16u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec1x16u, Construct_vector_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1xb>();

            mask1x16u m0{inputs};
            vec1x16u v0{m0};

            auto results = to_array(v0);
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    TEST(Vec1x16u, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto x = random_val<vec1x16u::scalar>();
            vec1x16u results{x};

            arr1x16u arr;
            std::fill_n(arr.data(), vec1x16u::width, x);
            vec1x16u expected{arr};

            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec1x16u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(inputs[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec1x16u, Assign_primitive) {
        vec1x16u::primitive x{};
        vec1x16u v{0x0F};
        v = x;
        vec1x16u expected{};

        EXPECT_TRUE(all(v == expected));
    }

    TEST(Vec1x16u, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto scalar = random_val<vec1x16u::scalar>();
            vec1x16u v;
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

    TEST(Vec1x16u, Equality_comparison) {
        mask1x16u mask0{false};
        mask1x16u mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask1x16u mask2{true};
        mask1x16u mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec1x16u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();
            inputs1[0] = ~inputs0[0];

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs0};
            vec1x16u v2{inputs1};

            EXPECT_TRUE(all(v0 == v1));
            EXPECT_FALSE(all(v1 == v2));
        }
    }

    TEST(Vec1x16u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();
            inputs1[0] = ~inputs0[0];

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs0};
            vec1x16u v2{inputs1};

            EXPECT_FALSE(any(v0 != v1));
            EXPECT_TRUE(any(v1 != v2));
        }
    }

    TEST(Vec1x16u, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = (v0 < v1);

            arr1xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] < inputs1[j];
            }

            EXPECT_EQ(results, mask1x16u{expected});
        }
    }

    TEST(Vec1x16u, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = (v0 <= v1);

            arr1xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] <= inputs1[j];
            }

            EXPECT_EQ(results, mask1x16u{expected});
        }
    }

    TEST(Vec1x16u, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = (v0 > v1);

            arr1xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] > inputs1[j];
            }

            EXPECT_EQ(results, mask1x16u{expected});
        }
    }

    TEST(Vec1x16u, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = (v0 >= v1);

            arr1xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] >= inputs1[j];
            }

            EXPECT_EQ(results, mask1x16u{expected});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec1x16u, Unary_plus) {
        auto inputs = random_array<arr1x16u>();
        const vec1x16u v{inputs};
        vec1x16u results{+v};

        EXPECT_TRUE(all(results == v));
    }

    TEST(Vec1x16u, Unary_minus) {
        auto inputs = random_array<arr1x16u>();
        const vec1x16u v{inputs};

        auto results = -v;

        arr1x16i expected{};
        for (std::size_t j = 0; j < expected.size(); ++j) {
            expected[j] = -inputs[j];
        }

        EXPECT_TRUE(all(results == vec1x16i{expected}));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec1x16u, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0;
            results += v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0;
            results -= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0;
            results *= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_denominator_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0;
            results /= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_denominator_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0;
            results %= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec1x16u, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0 + v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0 - v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0 * v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_denominator_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0 / v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_denominator_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0 % v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec1x16u, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};
            ++v;

            auto results = v;
            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};
            v++;

            auto results = v;
            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};
            --v;

            auto results = v;
            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};
            v--;

            auto results = v;
            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec1x16u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0;
            results &= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0;
            results |= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0;
            results ^= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            auto v1 = random_shift<vec1x16u::scalar>();

            auto results = v0;
            results <<= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec1x16u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_shift_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0;
            results <<= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec1x16u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            auto v1 = random_shift<vec1x16u::scalar>();

            auto results = v0;
            results >>= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec1x16u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_shift_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0;
            results >>= v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec1x16u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec1x16u, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = ~v;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ~inputs[j];
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            auto v1 = random_shift<vec1x16u::scalar>();

            auto results = v0 << v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec1x16u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << v1;
                }
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_shift_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0 << v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec1x16u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] << inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            auto v1 = random_shift<vec1x16u::scalar>();

            auto results = v0 >> v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (v1 >= type_width<vec1x16u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> v1;
                }
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_shift_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = v0 >> v1;

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                if (inputs1[j] >= type_width<vec1x16u::scalar>::value) {
                    expected[j] = 0x00;
                } else {
                    expected[j] = inputs0[j] >> inputs1[j];
                }
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec1x16u, Conversion_to_mask) {
        vec1x16u vec{0};

        auto a = vec.operator mask1x16u();
        mask1x16u b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec1x16u, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = mask1x16u{v};

            arr1xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = inputs[j];
            }

            EXPECT_TRUE(results == mask1x16u{expected});
        }
    }

    //=====================================================
    // Arrangement Operations
    //=====================================================

    TEST(Vec1x16u, Extract_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();
            vec1x16u v{inputs};

            EXPECT_EQ(inputs[0x00], extract<0x00>(v));

        }
    }

    TEST(Vec1x16u, Insert_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();
            vec1x16u v{random_val<vec1x16u::scalar>()};

            v = insert<0x00>(v, inputs[0x00]);


            EXPECT_TRUE(all(v == vec1x16u{inputs}));
        }
    }

    //=====================================================
    // Bit Manipulation Instructions
    //=====================================================

    TEST(Vec1x16u, Bit_shift_left) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

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

    TEST(Vec1x16u, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

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

    TEST(Vec1x16u, Rotl) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

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

    TEST(Vec1x16u, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            auto v1 = random64u();

            auto results = rotl(v0, v1);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = rotl(v0, v1);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotl(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Rotr) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();

            vec1x16u v{inputs0};

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

    TEST(Vec1x16u, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            auto v1 = random64u();

            auto results = rotr(v0, v1);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], v1);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = rotr(v0, v1);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = rotr(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec1x16u, Count_random) {
        auto predicate = [] (vec1x16u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec1x16u v{inputs};

            auto results = count(v);

            EXPECT_EQ(cnt, results);
        }
    }

    TEST(Vec1x16u, Any_edge_cases) {
        EXPECT_FALSE(any(vec1x16u{0x00}));
        EXPECT_TRUE(any(vec1x16u{0x01}));
    }

    TEST(Vec1x16u, Any_random) {
        auto predicate = [] (vec1x16u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec1x16u v{inputs};

            EXPECT_EQ(cnt != 0, any(v));
        }
    }

    TEST(Vec1x16u, All_edge_cases) {
        EXPECT_FALSE(all(vec1x16u{0x00}));
        EXPECT_TRUE(all(vec1x16u{0x01}));
    }

    TEST(Vec1x16u, All_random) {
        auto predicate = [] (vec1x16u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec1x16u v{inputs};

            auto results = all(v);

            EXPECT_EQ(cnt == vec1x16u::width, results);
        }
    }

    TEST(Vec1x16u, None_edge_cases) {
        EXPECT_TRUE(none(vec1x16u{0x00}));
        EXPECT_FALSE(none(vec1x16u{0x01}));
    }

    TEST(Vec1x16u, None_random) {
        auto predicate = [] (vec1x16u::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec1x16u v{inputs};

            EXPECT_EQ(cnt == 0, none(v));
        }
    }

    TEST(Vec1x16u, Set_bits_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1xb>();

            mask1x16u v{inputs};

            auto results = set_bits(v);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                if (inputs[j]) {
                    expected[j] = vec1x16u::scalar(-1);
                } else {
                    expected[j] = vec1x16u::scalar(0);
                }
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Keep_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1x16u>();

            mask1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = keep(v0, v1);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = keep(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Clear_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1x16u>();

            mask1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = clear(v0, v1);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = clear(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1xb>();
            auto inputs1 = random_array<arr1x16u>();
            auto inputs2 = random_array<arr1x16u>();

            mask1x16u v0{inputs0};
            vec1x16u v1{inputs1};
            vec1x16u v2{inputs2};

            auto results = blend(v0, v1, v2);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = blend(inputs0[j], inputs1[j], inputs2[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = max(v0, v1);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = max(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = min(v0, v1);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = min(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = minmax(v0, v1);

            arr1x16u expected0{};
            arr1x16u expected1{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto min_max = minmax(inputs0[j], inputs1[j]);
                expected0[j] = min_max[0];
                expected1[j] = min_max[1];
            }

            EXPECT_TRUE(all(results[0] == vec1x16u{expected0}));
            EXPECT_TRUE(all(results[1] == vec1x16u{expected1}));
        }
    }

    TEST(Vec1x16u, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();
            auto inputs2 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};
            vec1x16u v2{inputs2};

            auto ranges = minmax(v1, v2);

            auto results = clamp(v0, ranges[0], ranges[1]);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto range = minmax(inputs1[j], inputs2[j]);
                expected[j] = clamp(inputs0[j], range[0], range[1]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = average(v0, v1);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = average(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr1x16u>();
            auto inputs1 = random_array<arr1x16u>();

            vec1x16u v0{inputs0};
            vec1x16u v1{inputs1};

            auto results = midpoint(v0, v1);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = midpoint(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = neg_abs(v);

            arr1x16i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = neg_abs(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec1x16i{expected}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec1x16u, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            for (std::size_t j = 0; j <= vec1x16u::width; ++j) {
                auto results = load<vec1x16u>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::size_t k = 0; k < vec1x16u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec1x16u, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            EXPECT_TRUE(all(load<vec1x16u, 0x00>(inputs.data()) == load<vec1x16u>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(load<vec1x16u, 0x00>(inputs.data()) == load<vec1x16u>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(load<vec1x16u, 0x01>(inputs.data()) == load<vec1x16u>(inputs.data(), 0x01)));

        }
    }

    TEST(Vec1x16u, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec1x16u)) auto inputs = random_array<arr1x16u>();

            for (std::size_t j = 0; j <= vec1x16u::width; ++j) {
                auto results = aligned_load<vec1x16u>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::size_t k = 0; k < vec1x16u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec1x16u, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec1x16u)) auto inputs = random_array<arr1x16u>();

            EXPECT_TRUE(all(aligned_load<vec1x16u, 0x00>(inputs.data()) == aligned_load<vec1x16u>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec1x16u, 0x01>(inputs.data()) == aligned_load<vec1x16u>(inputs.data(), 0x01)));

        }
    }



    TEST(Vec1x16u, Store_n) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            for (std::size_t j = 0; j <= vec1x16u::width; ++j) {
                vec1x16u::scalar arr[vec1x16u::width]{};
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

    TEST(Vec1x16u, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            vec1x16u::scalar arr[vec1x16u::width]{};

            store<0x00>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x00));
            store<0x01>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x01));

        }
    }

    TEST(Vec1x16u, Aligned_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            for (std::size_t j = 0; j <= vec1x16u::width; ++j) {
                alignas(alignof(vec1x16u)) vec1x16u::scalar arr[vec1x16u::width]{};
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

    TEST(Vec1x16u, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            alignas(alignof(vec1x16u)) vec1x16u::scalar arr[vec1x16u::width]{};

            aligned_store<0x00>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x00));
            aligned_store<0x01>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x01));

        }
    }



    TEST(Vec1x16u, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec1x16u, Div_uniform) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto numerator = random_val<vec1x16u::scalar>();
            auto denominator = random_denominator<vec1x16u::scalar>();

            auto quotient  = (numerator / denominator);
            auto remainder = (numerator % denominator);

            vec1x16u n{numerator};
            vec1x16u d{denominator};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < q.size(); ++j) {
                EXPECT_EQ(q[j], quotient);
                EXPECT_EQ(r[j], remainder);
            }
        }
    }

    TEST(Vec1x16u, Div_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto numerators = random_array<arr1x16u>();
            auto denominators = random_denominator_array<arr1x16u>();

            arr1x16u quotients{};
            arr1x16u remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec1x16u n{numerators};
            vec1x16u d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec1x16u, Popcount_edge_cases) {
        vec1x16u v{0x0};
        vec1x16u c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec1x16u, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = popcount(v);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = popcount(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = byteswap(v);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = byteswap(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Countl_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec1x16u::scalar>::value - 1); ++i) {
            vec1x16u::scalar v = (vec1x16u::scalar(0x1) << i) - 1;
            auto results = countl_zero(vec1x16u{v});
            auto expected = vec1x16u(type_width<vec1x16u::scalar>::value - i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countl_zero(vec1x16u(-1)) == vec1x16u{0}));
    }

    TEST(Vec1x16u, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = countl_zero(v);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Countl_one_edge_cases) {
        EXPECT_TRUE(all(countl_one(vec1x16u{0x00}) == vec1x16u{0}));
        for (std::size_t i = 0; i < (type_width<vec1x16u::scalar>::value - 1); ++i) {
            vec1x16u::scalar v = vec1x16u::scalar(-1) << i;
            auto results = countl_one(vec1x16u{v});
            auto expected = vec1x16u(type_width<vec1x16u::scalar>::value - i);
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec1x16u, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = countl_one(v);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countl_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Countr_zero_edge_cases) {
        for (std::size_t i = 0; i < (type_width<vec1x16u::scalar>::value - 1); ++i) {
            vec1x16u::scalar v = vec1x16u::scalar(-1) << i;
            auto results = countr_zero(vec1x16u{v});
            auto expected = vec1x16u(i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countr_zero(vec1x16u{0x00}) == vec1x16u{type_width<vec1x16u::scalar>::value}));
    }

    TEST(Vec1x16u, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = countr_zero(v);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_zero(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Countr_one_edge_cases) {
        EXPECT_TRUE(all(countr_one(vec1x16u{0x00}) == vec1x16u{0}));

        for (std::size_t i = 0; i < (type_width<vec1x16u::scalar>::value - 1); ++i) {
            vec1x16u::scalar v = (vec1x16u::scalar(1) << i) - 1;
            auto results = countr_one(vec1x16u{v});
            auto expected = vec1x16u(i);
            EXPECT_TRUE(all(results == expected));
        }
        EXPECT_TRUE(all(countr_one(vec1x16u{vec1x16u::scalar(-1)}) == vec1x16u{type_width<vec1x16u::scalar>::value}));
    }

    TEST(Vec1x16u, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = countr_one(v);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = countr_one(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Bit_width_edge_cases) {
        EXPECT_TRUE(all(bit_width(vec1x16u{0x00}) == vec1x16u{0x00}));

        for (std::size_t i = 0; i < type_width<vec1x16u::scalar>::value; ++i) {
            vec1x16u::scalar v = vec1x16u::scalar(1) << i;
            auto results = bit_width(vec1x16u{v});
            auto expected = vec1x16u(i + 1);
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec1x16u, Bit_width_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = bit_width(v);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_width(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Bit_floor_edge_cases) {
        EXPECT_TRUE(all(vec1x16u{0} == bit_floor(vec1x16u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec1x16u::scalar>::value - 1); ++i) {
            vec1x16u::scalar v = vec1x16u::scalar(1) << i;
            auto results = bit_floor(vec1x16u{v});
            auto expected = vec1x16u{v};
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec1x16u, Bit_floor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = bit_floor(v);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_floor(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Bit_ceil_edge_cases) {
        EXPECT_TRUE(all(vec1x16u{1} == bit_ceil(vec1x16u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec1x16u::scalar>::value - 1); ++i) {
            vec1x16u::scalar v = vec1x16u::scalar(1) << i;
            auto results = bit_ceil(vec1x16u{v});
            auto expected = vec1x16u{v};
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec1x16u, Bit_ceil_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = bit_ceil(v);

            arr1x16u expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = bit_ceil(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec1x16u{expected}));
        }
    }

    TEST(Vec1x16u, Has_single_bit_edge_cases) {
        EXPECT_TRUE(none(has_single_bit(vec1x16u{0x00})));

        for (std::size_t i = 0; i < (type_width<vec1x16u::scalar>::value - 1); ++i) {
            vec1x16u::scalar v = vec1x16u::scalar(1) << i;
            auto results = has_single_bit(vec1x16u{v});
            EXPECT_TRUE(all(results));
        }
    }

    TEST(Vec1x16u, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr1x16u>();

            vec1x16u v{inputs};

            auto results = has_single_bit(v);

            arr1xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = has_single_bit(inputs[j]);
            }

            EXPECT_TRUE(results == mask1x16u{expected});
        }
    }

}

#endif
