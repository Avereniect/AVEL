#ifndef AVEL_VEC2X64F_TESTS_HPP
#define AVEL_VEC2X64F_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=========================================================================
    // mask2x64f tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask2x64f, Construct_from_primitive) {
        mask2x64f mask{mask2x64f::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask2x64f, Construct_from_bool) {
        mask2x64f mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask2x64f mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask2x64f::width);
    }

    TEST(Mask2x64f, Construct_from_array) {
        arr2xb false_array{};
        std::fill_n(false_array.data(), mask2x64f::width, false);
        mask2x64f mask0{false_array};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);


        arr2xb true_array{};
        std::fill_n(true_array.data(), mask2x64f::width, true);
        mask2x64f mask1{true_array};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_TRUE(count(mask1) == mask2x64f::width);
    }

    TEST(Mask2x64f, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();
            std::uint32_t cnt = std::count(inputs.begin(), inputs.end(), true);

            auto m = mask2x64f{inputs};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == mask2x64f::width, all(m));
            EXPECT_EQ(cnt != 0, any(m));
            EXPECT_EQ(cnt == 0, none(m));
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask2x64f, Assign_bool) {
        mask2x64f mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask2x64f mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(count(mask1) == mask2x64f::width);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask2x64f, Equality_comparison) {
        mask2x64f mask0{false};
        mask2x64f mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask2x64f mask2{true};
        mask2x64f mask3{true};
        EXPECT_EQ(mask2, mask3);

        EXPECT_FALSE(mask0 == mask2);
        EXPECT_FALSE(mask3 == mask1);
    }

    TEST(Mask2x64f, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();
            inputs1[0] = !inputs0[0];

            mask2x64f mask0{inputs0};
            mask2x64f mask1{inputs0};
            mask2x64f mask2{inputs1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask2x64f, Inequality_comparison) {
        mask2x64f mask0{false};
        mask2x64f mask1{false};
        EXPECT_FALSE(mask0 != mask1);

        mask2x64f mask2{true};
        mask2x64f mask3{true};
        EXPECT_FALSE(mask2 != mask3);

        EXPECT_TRUE(mask0 != mask2);
        EXPECT_TRUE(mask3 != mask1);
    }

    TEST(Mask2x64f, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();
            inputs1[0] = !inputs0[0];

            mask2x64f mask0{inputs0};
            mask2x64f mask1{inputs1};
            mask2x64f mask2{inputs1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask2x64f, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64f mask0{inputs0};
            mask2x64f mask1{inputs1};

            mask2x64f results = mask0;
            results &= mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask2x64f expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64f, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64f mask0{inputs0};
            mask2x64f mask1{inputs1};

            mask2x64f results = mask0;
            results |= mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask2x64f expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64f, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64f mask0{inputs0};
            mask2x64f mask1{inputs1};

            mask2x64f results = mask0;
            results ^= mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask2x64f expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask2x64f, Logical_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();

            mask2x64f mask0{inputs0};

            mask2x64f results = !mask0;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = !inputs0[j];
            }
            mask2x64f expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64f, Bitwise_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64f mask0{inputs0};
            mask2x64f mask1{inputs1};

            mask2x64f results = mask0 & mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] & inputs1[j];
            }
            mask2x64f expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64f, Logical_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64f mask0{inputs0};
            mask2x64f mask1{inputs1};

            mask2x64f results = mask0 && mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] && inputs1[j];
            }
            mask2x64f expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64f, Bitwise_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64f mask0{inputs0};
            mask2x64f mask1{inputs1};

            mask2x64f results = mask0 | mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] | inputs1[j];
            }
            mask2x64f expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64f, Logical_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64f mask0{inputs0};
            mask2x64f mask1{inputs1};

            mask2x64f results = mask0 || mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] || inputs1[j];
            }
            mask2x64f expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    TEST(Mask2x64f, Bitwise_xor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2xb>();

            mask2x64f mask0{inputs0};
            mask2x64f mask1{inputs1};

            mask2x64f results = mask0 ^ mask1;

            arr2xb expected;
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] ^ inputs1[j];
            }
            mask2x64f expected_mask{expected};

            EXPECT_EQ(results, expected_mask);
        }
    }

    //=====================================================
    // Mask functions
    //=====================================================

    TEST(Mask2x64f, Count_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();
            auto cnt = std::count(inputs.begin(), inputs.end(), true);

            mask2x64f m{inputs};

            auto results = count(m);

            EXPECT_EQ(cnt, results);
        }

    }

    TEST(Mask2x64f, Any_edge_cases) {
        EXPECT_FALSE(any(mask2x64f{false}));
        EXPECT_TRUE(any(mask2x64f{true}));
    }

    TEST(Mask2x64f, Any_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();
            auto cnt = std::count(inputs.begin(), inputs.end(), true);

            mask2x64f v{inputs};

            EXPECT_EQ(cnt != 0, any(v));
        }
    }

    TEST(Mask2x64f, All_edge_cases) {
        EXPECT_FALSE(all(mask2x64f{false}));
        EXPECT_TRUE(all(mask2x64f{true}));
    }

    TEST(Mask2x64f, All_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();
            auto cnt = std::count(inputs.begin(), inputs.end(), true);

            mask2x64f v{inputs};

            EXPECT_EQ(cnt == vec2x64f::width, all(v));
        }
    }

    TEST(Mask2x64f, None_edge_cases) {
        EXPECT_FALSE(all(mask2x64f{false}));
        EXPECT_TRUE(all(mask2x64f{true}));
    }

    TEST(Mask2x64f, None_random) {

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();
            auto cnt = std::count(inputs.begin(), inputs.end(), true);

            mask2x64f v{inputs};

            EXPECT_EQ(cnt == 0, none(v));
        }
    }

    TEST(Mask2x64f, Extract_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();
            mask2x64f v{inputs};

            EXPECT_EQ(inputs[0x00], extract<0x00>(v));
            EXPECT_EQ(inputs[0x01], extract<0x01>(v));

        }
    }

    TEST(Mask2x64f, Insert_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();
            mask2x64f v{false};

            v = insert<0x00>(v, inputs[0x00]);
            v = insert<0x01>(v, inputs[0x01]);


            EXPECT_TRUE(v == mask2x64f{inputs});
        }
    }

    //=========================================================================
    // Vec2x64f tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec2x64f, Construct_vector_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2xb>();

            mask2x64f m0{inputs};
            vec2x64f v0{m0};

            auto results = to_array(v0);
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    TEST(Vec2x64f, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto x = random_val<vec2x64f::scalar>();
            vec2x64f results{x};

            arr2x64f arr;
            std::fill_n(arr.data(), vec2x64f::width, x);
            vec2x64f expected{arr};

            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec2x64f, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto data = random_array<arr2x64f>();

            vec2x64f v{data};

            auto results = to_array(v);

            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(data[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec2x64f, Assign_primitive) {
        vec2x64f::primitive x{};
        vec2x64f v{0x0F};
        v = x;
        vec2x64f expected{0.0};

        EXPECT_TRUE(all(v == expected));
    }

    TEST(Vec2x64f, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto scalar = random_val<vec2x64f::scalar>();
            vec2x64f v;
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

    TEST(Vec2x64f, Equality_comparison) {
        mask2x64f mask0{false};
        mask2x64f mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask2x64f mask2{true};
        mask2x64f mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec2x64f, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs0};

            EXPECT_TRUE(all(v0 == v1));
        }
    }

    TEST(Vec2x64f, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs0};

            EXPECT_FALSE(any(v0 != v1));
        }
    }

    TEST(Vec2x64f, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = (v0 < v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] < inputs1[j];
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = (v0 <= v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] <= inputs1[j];
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = (v0 > v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] > inputs1[j];
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = (v0 >= v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < expected.size(); ++j) {
                expected[j] = inputs0[j] >= inputs1[j];
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec2x64f, Unary_plus) {
        auto inputs = random_array<arr2x64f>();
        const vec2x64f u{inputs};
        vec2x64f v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec2x64f, Unary_minus) {
        auto inputs = random_array<arr2x64f>();
        const vec2x64f input{inputs};

        auto results = -input;

        arr2x64f expected{};
        for (std::size_t j = 0; j < expected.size(); ++j) {
            expected[j] = -inputs[j];
        }

        EXPECT_TRUE(all(results == vec2x64f{expected}));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec2x64f, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = v0;
            results += v1;

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = v0;
            results -= v1;

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = v0;
            results *= v1;

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    /*
    TEST(Vec2x64f, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_denominator_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = v0;
            results /= v1;

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_denominator_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = v0;
            results %= v1;

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }
    */

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec2x64f, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = v0 + v1;

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] + inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = v0 - v1;

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] - inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = v0 * v1;

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] * inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    /*
    TEST(Vec2x64f, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_denominator_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = v0 / v1;

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] / inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_denominator_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = v0 % v1;

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = inputs0[j] % inputs1[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }
    */

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec2x64f, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f input{inputs};
            ++input;

            auto results = input;
            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f input{inputs};
            input++;

            auto results = input;
            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = ++inputs[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f input{inputs};
            --input;

            auto results = input;
            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f input{inputs};
            input--;

            auto results = input;
            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = --inputs[j];
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec2x64f, Conversion_to_mask) {
        vec2x64f vec{0.0};

        auto a = vec.operator mask2x64f();
        mask2x64f b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec2x64f, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = mask2x64f{v};

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = inputs[j];
            }

            EXPECT_TRUE(results == mask2x64f{expected});
        }
    }

    //=====================================================
    // Arrangement Operations
    //=====================================================

    TEST(Vec2x64f, Extract_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();
            vec2x64f v{inputs};

            EXPECT_EQ(inputs[0x00], extract<0x00>(v));
            EXPECT_EQ(inputs[0x01], extract<0x01>(v));

        }
    }

    TEST(Vec2x64f, Insert_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();
            vec2x64f v{random_val<vec2x64f::scalar>()};

            v = insert<0x00>(v, inputs[0x00]);
            v = insert<0x01>(v, inputs[0x01]);


            EXPECT_TRUE(all(v == vec2x64f{inputs}));
        }
    }



    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec2x64f, Count_random) {
        auto predicate = [] (vec2x64f::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec2x64f v{inputs};

            auto results = count(v);

            EXPECT_EQ(cnt, results);
        }
    }

    TEST(Vec2x64f, Any_edge_cases) {
        EXPECT_FALSE(avel::any(vec2x64f{0.0}));
        EXPECT_TRUE(avel::any(vec2x64f{1.0}));
    }

    TEST(Vec2x64f, Any_random) {
        auto predicate = [] (vec2x64f::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec2x64f v{inputs};

            EXPECT_EQ(cnt != 0, any(v));
        }
    }

    TEST(Vec2x64f, All_edge_cases) {
        EXPECT_FALSE(avel::all(vec2x64f{0.0}));
        EXPECT_TRUE(avel::all(vec2x64f{1.0}));
    }

    TEST(Vec2x64f, All_random) {
        auto predicate = [] (vec2x64f::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec2x64f v{inputs};

            auto results = avel::all(v);

            EXPECT_EQ(cnt == vec2x64f::width, results);
        }
    }

    TEST(Vec2x64f, None_edge_cases) {
        EXPECT_TRUE(none(vec2x64f{0.0}));
        EXPECT_FALSE(none(vec2x64f{1.0}));
    }

    TEST(Vec2x64f, None_random) {
        auto predicate = [] (vec2x64f::scalar x) {
            return x != 0;
        };

        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();
            auto cnt = std::count_if(inputs.begin(), inputs.end(), predicate);

            vec2x64f v{inputs};

            EXPECT_EQ(cnt == 0, none(v));
        }
    }

    TEST(Vec2x64f, Keep_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2x64f>();

            mask2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::keep(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::keep(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Clear_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2x64f>();

            mask2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::clear(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::clear(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2x64f>();
            auto inputs2 = random_array<arr2x64f>();

            mask2x64f v0{inputs0};
            vec2x64f v1{inputs1};
            vec2x64f v2{inputs2};

            auto results = avel::blend(v0, v1, v2);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::blend(inputs0[j], inputs1[j], inputs2[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::max(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::max(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::min(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::min(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::minmax(v0, v1);

            arr2x64f expected0{};
            arr2x64f expected1{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto xy = avel::minmax(inputs0[j], inputs1[j]);
                expected0[j] = xy[0];
                expected1[j] = xy[1];
            }

            EXPECT_TRUE(all(results[0] == vec2x64f{expected0}));
            EXPECT_TRUE(all(results[1] == vec2x64f{expected1}));
        }
    }

    TEST(Vec2x64f, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();
            auto inputs2 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};
            vec2x64f input2{inputs2};

            auto xy = avel::minmax(v1, input2);

            auto results = avel::clamp(v0, xy[0], xy[1]);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                auto xy = avel::minmax(inputs1[j], inputs2[j]);
                expected[j] = avel::clamp(inputs0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    /*
    TEST(Vec2x64f, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::average(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::average(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }
    */

    TEST(Vec2x64f, Negate_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2xb>();
            auto inputs1 = random_array<arr2x64f>();

            mask2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::negate(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::negate(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::abs(v);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::abs(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::neg_abs(v);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::neg_abs(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec2x64f, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            for (std::uint32_t j = 0; j < vec2x64f::width; ++j) {
                auto results = load<vec2x64f>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec2x64f::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec2x64f, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            EXPECT_TRUE(all(load<vec2x64f, 0x00>(inputs.data()) == load<vec2x64f>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(load<vec2x64f, 0x00>(inputs.data()) == load<vec2x64f>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(load<vec2x64f, 0x01>(inputs.data()) == load<vec2x64f>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(load<vec2x64f, 0x02>(inputs.data()) == load<vec2x64f>(inputs.data(), 0x02)));

        }
    }

    TEST(Vec2x64f, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec2x64f)) auto inputs = random_array<arr2x64f>();

            for (std::uint32_t j = 0; j < vec2x64f::width; ++j) {
                auto results = aligned_load<vec2x64f>(inputs.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec2x64f::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(inputs[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec2x64f, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec2x64f)) auto inputs = random_array<arr2x64f>();

            EXPECT_TRUE(all(aligned_load<vec2x64f, 0x00>(inputs.data()) == aligned_load<vec2x64f>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec2x64f, 0x00>(inputs.data()) == aligned_load<vec2x64f>(inputs.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec2x64f, 0x01>(inputs.data()) == aligned_load<vec2x64f>(inputs.data(), 0x01)));
            EXPECT_TRUE(all(aligned_load<vec2x64f, 0x02>(inputs.data()) == aligned_load<vec2x64f>(inputs.data(), 0x02)));

        }
    }

    TEST(Vec2x64f, Gather_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;

        std::vector<vec2x64f::scalar> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random_val<vec2x64f::scalar>();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = random_val<vec2x64u::scalar>() % test_data_size;
            }
            auto indices = load<vec2x64i>(index_array.data());

            for (std::size_t j = 0; j <= vec2x64f::width; ++j) {
                auto results = gather<vec2x64f>(test_data.data(), indices, j);

                auto loaded_data = to_array(results);
                for (std::size_t k = 0; k < vec2x64f::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(test_data[index_array[k]], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec2x64f, Gather_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;

        std::vector<vec2x64f::scalar> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random_val<vec2x64f::scalar>();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i index_array{};
            for (std::size_t j = 0; j < vec2x64f::width; ++j) {
                index_array[j] = random_val<vec2x64i::scalar>() % test_data_size;
            }
            auto indices = load<vec2x64i>(index_array.data());

            EXPECT_TRUE(all(gather<vec2x64f, 0x00>(test_data.data(), indices) == gather<vec2x64f>(test_data.data(), indices, 0x00)));
            EXPECT_TRUE(all(gather<vec2x64f, 0x01>(test_data.data(), indices) == gather<vec2x64f>(test_data.data(), indices, 0x01)));
            EXPECT_TRUE(all(gather<vec2x64f, 0x02>(test_data.data(), indices) == gather<vec2x64f>(test_data.data(), indices, 0x02)));

        }
    }

    TEST(Vec2x64f, Store_n) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            for (std::size_t j = 0; j < vec2x64f::width; ++j) {
                vec2x64f::scalar arr[vec2x64f::width]{};
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

    TEST(Vec2x64f, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            vec2x64f::scalar arr[vec2x64f::width]{};

            store<0x00>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x00));
            store<0x01>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x01));
            store<0x02>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x02));

        }
    }

    TEST(Vec2x64f, Aligned_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            for (std::size_t j = 0; j < vec2x64f::width; ++j) {
                alignas(alignof(vec2x64f)) vec2x64f::scalar arr[vec2x64f::width]{};
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

    TEST(Vec2x64f, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            alignas(alignof(vec2x64f)) vec2x64f::scalar arr[vec2x64f::width]{};

            aligned_store<0x00>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x00));
            aligned_store<0x01>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x01));
            aligned_store<0x02>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, 0x02));

        }
    }

    TEST(Vec2x64f, Scatter_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;
        static constexpr std::size_t block_size = test_data_size / vec2x64f::width;

        std::vector<vec2x64f::scalar> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr2x64f>();

            vec2x64f input0{input_array0};

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

    TEST(Vec2x64f, Scatter_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;
        static constexpr std::size_t block_size = test_data_size / vec2x64f::width;

        std::vector<vec2x64f::scalar> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr2x64f>();

            vec2x64f input0{input_array0};

            arr2x64i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random_val<vec2x64u::scalar>() % block_size) + (j * block_size);
            }

            vec2x64i indices{index_array};

            scatter<0x00>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x00));
            scatter<0x01>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x01));
            scatter<0x02>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x02));

        }
    }

    TEST(Vec2x64f, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = to_array(v);

            for (std::size_t j = 0; j < inputs.size(); ++j) {
                EXPECT_EQ(results[j], inputs[j]);
            }
        }
    }

    //=====================================================
    // Floating-point functions
    //=====================================================

    TEST(Vec2x64f, fmax_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};

        EXPECT_TRUE(all(zero == avel::fmax(zero, zero)));
        EXPECT_TRUE(all(zero == avel::fmax(nan, zero)));
        EXPECT_TRUE(all(zero == avel::fmax(zero, nan)));
        EXPECT_TRUE(all(avel::isnan(avel::fmax(nan, nan))));
    }

    TEST(Vec2x64f, fmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::fmax(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::fmax(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, fmin_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};

        EXPECT_TRUE(all(zero == avel::fmin(zero, zero)));
        EXPECT_TRUE(all(zero == avel::fmin(nan, zero)));
        EXPECT_TRUE(all(zero == avel::fmin(zero, nan)));
        EXPECT_TRUE(all(avel::isnan(avel::fmin(nan, nan))));
    }

    TEST(Vec2x64f, fmin_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::fmin(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::fmin(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, fdim_edge_cases) {
        vec2x64f one{1.0f};
        vec2x64f nan{NAN};

        EXPECT_TRUE(avel::all(avel::isnan(avel::fdim(one, nan))));
        EXPECT_TRUE(avel::all(avel::isnan(avel::fdim(nan, one))));
        EXPECT_TRUE(avel::all(avel::isnan(avel::fdim(nan, nan))));

        vec2x64f a{+std::numeric_limits<vec2x64f::scalar>::max()};
        vec2x64f b{-std::numeric_limits<vec2x64f::scalar>::max()};
        EXPECT_TRUE(avel::all(avel::isinf(avel::fdim(a, b))));
    }

    TEST(Vec2x64f, fdim_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::fdim(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::fdim(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }


    /*
    TEST(Vec2x64f, fmod_edge_cases) {
        EXPECT_TRUE(avel::all(avel::fmod(vec2x64f{+0.0f}, vec2x64f{1.0f}) == vec2x64f{+0.0f}));
        EXPECT_TRUE(avel::all(avel::fmod(vec2x64f{-0.0f}, vec2x64f{1.0f}) == vec2x64f{-0.0f}));

        EXPECT_TRUE(avel::all(avel::isnan(avel::fmod(vec2x64f{+INFINITY}, vec2x64f{1.0f}))));
        EXPECT_TRUE(avel::all(avel::isnan(avel::fmod(vec2x64f{-INFINITY}, vec2x64f{1.0f}))));

        EXPECT_TRUE(avel::all(avel::isnan(avel::fmod(vec2x64f{1.0f}, vec2x64f{+0.0f}))));
        EXPECT_TRUE(avel::all(avel::isnan(avel::fmod(vec2x64f{1.0f}, vec2x64f{-0.0f}))));

        EXPECT_TRUE(avel::all(vec2x64f{1.0f} == avel::fmod(vec2x64f{1.0f}, vec2x64f{+INFINITY})));
        EXPECT_TRUE(avel::all(vec2x64f{1.0f} == avel::fmod(vec2x64f{1.0f}, vec2x64f{-INFINITY})));

        EXPECT_TRUE(avel::all(avel::isnan(avel::fmod(vec2x64f{1.0f}, vec2x64f{NAN}))));
        EXPECT_TRUE(avel::all(avel::isnan(avel::fmod(vec2x64f{NAN}, vec2x64f{1.0f}))));
        EXPECT_TRUE(avel::all(avel::isnan(avel::fmod(vec2x64f{NAN}, vec2x64f{NAN}))));
    }

    TEST(Vec2x64f, fmod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            vec2x64f results = avel::fmod(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::fmod(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }
    */

    TEST(Vec2x64f, frac_edge_cases) {
        vec2x64f pos_zero{+0.0};
        vec2x64f neg_zero{-0.0};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};
        vec2x64f nan{NAN};

        EXPECT_TRUE(all(pos_zero == avel::frac(pos_zero)));
        EXPECT_TRUE(all(neg_zero == avel::frac(neg_zero)));
        EXPECT_TRUE(all(avel::isnan(avel::frac(pos_inf))));
        EXPECT_TRUE(all(avel::isnan(avel::frac(neg_inf))));
        EXPECT_TRUE(all(avel::isnan(avel::frac(nan))));
    }

    TEST(Vec2x64f, frac_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::frac(v);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::frac(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    //=====================================================
    // Nearest Integer Operations
    //=====================================================

    TEST(Vec2x64f, ceil_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(all(avel::isnan(avel::ceil(nan))));
        EXPECT_TRUE(all(zero == avel::ceil(zero)));
        EXPECT_TRUE(all(pos_inf == avel::ceil(pos_inf)));
        EXPECT_TRUE(all(neg_inf == avel::ceil(neg_inf)));
    }

    TEST(Vec2x64f, ceil_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::ceil(v);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::ceil(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, floor_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(all(avel::isnan(avel::floor(nan))));
        EXPECT_TRUE(all(zero == avel::floor(zero)));
        EXPECT_TRUE(all(pos_inf == avel::floor(pos_inf)));
        EXPECT_TRUE(all(neg_inf == avel::floor(neg_inf)));
    }

    TEST(Vec2x64f, floor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::floor(v);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::floor(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, trunc_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(all(avel::isnan(avel::trunc(nan))));
        EXPECT_TRUE(all(zero == avel::trunc(zero)));
        EXPECT_TRUE(all(pos_inf == avel::trunc(pos_inf)));
        EXPECT_TRUE(all(neg_inf == avel::trunc(neg_inf)));
    }

    TEST(Vec2x64f, trunc_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::trunc(v);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::trunc(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, round_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(all(avel::isnan(avel::round(nan))));
        EXPECT_TRUE(all(zero == avel::round(zero)));
        EXPECT_TRUE(all(pos_inf == avel::round(pos_inf)));
        EXPECT_TRUE(all(neg_inf == avel::round(neg_inf)));
    }

    TEST(Vec2x64f, round_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::round(v);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::round(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, nearbyint_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(all(avel::isnan(avel::nearbyint(nan))));
        EXPECT_TRUE(all(zero == avel::nearbyint(zero)));
        EXPECT_TRUE(all(pos_inf == avel::nearbyint(pos_inf)));
        EXPECT_TRUE(all(neg_inf == avel::nearbyint(neg_inf)));
    }

    TEST(Vec2x64f, nearbyint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::nearbyint(v);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::nearbyint(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, rint_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(all(avel::isnan(avel::rint(nan))));
        EXPECT_TRUE(all(zero == avel::rint(zero)));
        EXPECT_TRUE(all(pos_inf == avel::rint(pos_inf)));
        EXPECT_TRUE(all(neg_inf == avel::rint(neg_inf)));
    }

    TEST(Vec2x64f, rint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::rint(v);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::rint(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    //=====================================================
    // Floating-point Manipulation
    //=====================================================

    TEST(Vec2x64f, frexp_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        vec2x64i zero_i{0x00};

        vec2x64i e0;
        auto s0 = avel::frexp(zero, &e0);

        EXPECT_TRUE(all(zero_i == e0));
        EXPECT_TRUE(all(s0 == zero));

        vec2x64i e1;
        auto s1 = avel::frexp(nan, &e1);

        EXPECT_TRUE(all(avel::isnan(s1)));

        vec2x64i e2;
        auto s2 = avel::frexp(pos_inf, &e2);

        EXPECT_TRUE(all(s2 == pos_inf));

        vec2x64i e3;
        auto s3 = avel::frexp(neg_inf, &e3);

        EXPECT_TRUE(all(s3 == neg_inf));
    }

    TEST(Vec2x64f, frexp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            vec2x64i results0;
            auto results1 = avel::frexp(v, &results0);

            arr2x64i expected0{};
            arr2x64f expected1{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected1[j] = avel::frexp(inputs[j], expected0.data() + j);
            }

            EXPECT_TRUE(all(results0 == vec2x64i{expected0}));
            EXPECT_TRUE(all(results1 == vec2x64f{expected1}));
        }
    }

    TEST(Vec2x64f, ldexp_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        vec2x64i e0{0};
        vec2x64i e1{64};

        EXPECT_TRUE(all(zero == avel::ldexp(zero, e0)));
        EXPECT_TRUE(all(zero == avel::ldexp(zero, e1)));

        EXPECT_TRUE(all(pos_inf == avel::ldexp(pos_inf, e1)));
        EXPECT_TRUE(all(neg_inf == avel::ldexp(neg_inf, e1)));

        EXPECT_TRUE(all(avel::isnan(avel::ldexp(nan, e1))));
    }

    TEST(Vec2x64f, ldexp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_exponent_array<arr2x64i>();

            vec2x64f v0{inputs0};
            vec2x64i v1{inputs1};

            auto results = avel::ldexp(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::ldexp(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, scalbn_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        vec2x64i e0{0};
        vec2x64i e1{64};

        EXPECT_TRUE(all(zero == avel::scalbn(zero, e0)));
        EXPECT_TRUE(all(zero == avel::scalbn(zero, e1)));

        EXPECT_TRUE(all(pos_inf == avel::scalbn(pos_inf, e1)));
        EXPECT_TRUE(all(neg_inf == avel::scalbn(neg_inf, e1)));

        EXPECT_TRUE(all(avel::isnan(avel::scalbn(nan, e1))));
    }

    TEST(Vec2x64f, scalbn_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_exponent_array<arr2x64i>();

            vec2x64f v0{inputs0};
            vec2x64i v1{inputs1};

            auto results = avel::scalbn(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::scalbn(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, ilogb_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        vec2x64i e0{FP_ILOGB0};
        vec2x64i e1{FP_ILOGBNAN};
        vec2x64i e2{INT_MAX};

        EXPECT_TRUE(all(e0 == avel::ilogb(zero)));
        EXPECT_TRUE(all(e1 == avel::ilogb(nan)));
        EXPECT_TRUE(all(e2 == avel::ilogb(pos_inf)));
        EXPECT_TRUE(all(e2 == avel::ilogb(neg_inf)));
    }

    TEST(Vec2x64f, ilogb_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::ilogb(v);

            arr2x64i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::ilogb(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{expected}));
        }
    }

    TEST(Vec2x64f, logb_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(all(neg_inf == avel::logb(zero)));
        EXPECT_TRUE(all(avel::isnan(avel::logb(nan))));
        EXPECT_TRUE(all(pos_inf == avel::logb(pos_inf)));
        EXPECT_TRUE(all(pos_inf == avel::logb(neg_inf)));
    }

    TEST(Vec2x64f, logb_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::logb(v);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::logb(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    TEST(Vec2x64f, copysign) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::copysign(v0, v1);

            arr2x64f expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::copysign(inputs0[j], inputs1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64f{expected}));
        }
    }

    //=====================================================
    // Floating-point Classification
    //=====================================================

    TEST(Vec2x64f, fpclassify_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        vec2x64i c0{FP_ZERO};
        vec2x64i c1{FP_NAN};
        vec2x64i c2{FP_INFINITE};

        EXPECT_TRUE(all(c0 == avel::fpclassify(zero)));
        EXPECT_TRUE(all(c1 == avel::fpclassify(nan)));
        EXPECT_TRUE(all(c2 == avel::fpclassify(pos_inf)));
        EXPECT_TRUE(all(c2 == avel::fpclassify(neg_inf)));
    }

    TEST(Vec2x64f, fpclassify_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::fpclassify(v);

            arr2x64i expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::fpclassify(inputs[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{expected}));
        }
    }

    TEST(Vec2x64f, isfinite_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(all(avel::isfinite(zero)));
        EXPECT_TRUE(none(avel::isfinite(nan)));
        EXPECT_TRUE(none(avel::isfinite(pos_inf)));
        EXPECT_TRUE(none(avel::isfinite(neg_inf)));
    }

    TEST(Vec2x64f, isfinite_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::isfinite(v);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::isfinite(inputs[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, isinf_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(none(avel::isinf(zero)));
        EXPECT_TRUE(none(avel::isinf(nan)));
        EXPECT_TRUE(all(avel::isinf(pos_inf)));
        EXPECT_TRUE(all(avel::isinf(neg_inf)));
    }

    TEST(Vec2x64f, isinf_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::isinf(v);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::isinf(inputs[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, isnan_edge_cases) {
        vec2x64f zero{0.0};
        vec2x64f nan{NAN};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(none(avel::isnan(zero)));
        EXPECT_TRUE(all(avel::isnan(nan)));
        EXPECT_TRUE(none(avel::isnan(pos_inf)));
        EXPECT_TRUE(none(avel::isnan(neg_inf)));
    }

    TEST(Vec2x64f, isnan_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::isnan(v);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::isnan(inputs[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, isnormal_edge_cases) {
        vec2x64f nan{NAN};
        vec2x64f zero{0.0f};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(none(avel::isnormal(zero)));
        EXPECT_TRUE(none(avel::isnormal(nan)));
        EXPECT_TRUE(none(avel::isnormal(pos_inf)));
        EXPECT_TRUE(none(avel::isnormal(neg_inf)));
    }

    TEST(Vec2x64f, isnormal_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::isnormal(v);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::isnormal(inputs[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, signbit_edge_cases) {
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(none(avel::signbit(pos_inf)));
        EXPECT_TRUE(all(avel::signbit(neg_inf)));
    }

    TEST(Vec2x64f, signbit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<arr2x64f>();

            vec2x64f v{inputs};

            auto results = avel::signbit(v);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = avel::signbit(inputs[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    //=====================================================
    // Floating-point Comparisons
    //=====================================================

    TEST(Vec2x64f, isgreater_edge_cases) {
        vec2x64f nan{NAN};
        vec2x64f zero{0.0f};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(none(avel::isgreater(nan, nan)));

        EXPECT_TRUE(none(avel::isgreater(zero, nan)));
        EXPECT_TRUE(none(avel::isgreater(nan, zero)));
        EXPECT_TRUE(none(avel::isgreater(zero, zero)));

        EXPECT_TRUE(none(avel::isgreater(pos_inf, pos_inf)));
        EXPECT_TRUE(none(avel::isgreater(nan, pos_inf)));
        EXPECT_TRUE(none(avel::isgreater(pos_inf, nan)));

        EXPECT_TRUE(none(avel::isgreater(neg_inf, neg_inf)));
        EXPECT_TRUE(none(avel::isgreater(nan, neg_inf)));
        EXPECT_TRUE(none(avel::isgreater(neg_inf, nan)));
    }

    TEST(Vec2x64f, isgreater_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::isgreater(v0, v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::isgreater(inputs0[j], inputs1[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, isgreaterequal_edge_cases) {
        vec2x64f nan{NAN};
        vec2x64f zero{0.0f};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(none(avel::isgreaterequal(nan, nan)));

        EXPECT_TRUE(none(avel::isgreaterequal(zero, nan)));
        EXPECT_TRUE(none(avel::isgreaterequal(nan, zero)));
        EXPECT_TRUE(all(avel::isgreaterequal(zero, zero)));

        EXPECT_TRUE(all(avel::isgreaterequal(pos_inf, pos_inf)));
        EXPECT_TRUE(none(avel::isgreaterequal(nan, pos_inf)));
        EXPECT_TRUE(none(avel::isgreaterequal(pos_inf, nan)));

        EXPECT_TRUE(all(avel::isgreaterequal(neg_inf, neg_inf)));
        EXPECT_TRUE(none(avel::isgreaterequal(nan, neg_inf)));
        EXPECT_TRUE(none(avel::isgreaterequal(neg_inf, nan)));
    }

    TEST(Vec2x64f, isgreaterequal_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::isgreaterequal(v0, v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::isgreaterequal(inputs0[j], inputs1[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, isless_edge_cases) {
        vec2x64f nan{NAN};
        vec2x64f zero{0.0f};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(none(avel::isless(nan, nan)));

        EXPECT_TRUE(none(avel::isless(zero, nan)));
        EXPECT_TRUE(none(avel::isless(nan, zero)));
        EXPECT_TRUE(none(avel::isless(zero, zero)));

        EXPECT_TRUE(none(avel::isless(pos_inf, pos_inf)));
        EXPECT_TRUE(none(avel::isless(nan, pos_inf)));
        EXPECT_TRUE(none(avel::isless(pos_inf, nan)));

        EXPECT_TRUE(none(avel::isless(neg_inf, neg_inf)));
        EXPECT_TRUE(none(avel::isless(nan, neg_inf)));
        EXPECT_TRUE(none(avel::isless(neg_inf, nan)));
    }

    TEST(Vec2x64f, isless_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::isless(v0, v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::isless(inputs0[j], inputs1[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, islessequal_edge_cases) {
        vec2x64f nan{NAN};
        vec2x64f zero{0.0f};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(none(avel::islessequal(nan, nan)));

        EXPECT_TRUE(none(avel::islessequal(zero, nan)));
        EXPECT_TRUE(none(avel::islessequal(nan, zero)));
        EXPECT_TRUE(all(avel::islessequal(zero, zero)));

        EXPECT_TRUE(all(avel::islessequal(pos_inf, pos_inf)));
        EXPECT_TRUE(none(avel::islessequal(nan, pos_inf)));
        EXPECT_TRUE(none(avel::islessequal(pos_inf, nan)));

        EXPECT_TRUE(all(avel::islessequal(neg_inf, neg_inf)));
        EXPECT_TRUE(none(avel::islessequal(nan, neg_inf)));
        EXPECT_TRUE(none(avel::islessequal(neg_inf, nan)));
    }

    TEST(Vec2x64f, islessequal_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::islessequal(v0, v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::islessequal(inputs0[j], inputs1[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, islessgreater_edge_cases) {
        vec2x64f nan{NAN};
        vec2x64f zero{0.0f};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(none(avel::islessgreater(nan, nan)));

        EXPECT_TRUE(none(avel::islessgreater(zero, nan)));
        EXPECT_TRUE(none(avel::islessgreater(nan, zero)));
        EXPECT_TRUE(none(avel::islessgreater(zero, zero)));

        EXPECT_TRUE(none(avel::islessgreater(pos_inf, pos_inf)));
        EXPECT_TRUE(none(avel::islessgreater(nan, pos_inf)));
        EXPECT_TRUE(none(avel::islessgreater(pos_inf, nan)));

        EXPECT_TRUE(none(avel::islessgreater(neg_inf, neg_inf)));
        EXPECT_TRUE(none(avel::islessgreater(nan, neg_inf)));
        EXPECT_TRUE(none(avel::islessgreater(neg_inf, nan)));
    }

    TEST(Vec2x64f, islessgreater_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::islessgreater(v0, v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::islessgreater(inputs0[j], inputs1[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

    TEST(Vec2x64f, isunordered_random_edge_cases) {
        vec2x64f nan{NAN};
        vec2x64f zero{0.0f};
        vec2x64f pos_inf{+INFINITY};
        vec2x64f neg_inf{-INFINITY};

        EXPECT_TRUE(all(avel::isunordered(nan, nan)));

        EXPECT_TRUE(none(avel::isunordered(zero, zero)));
        EXPECT_TRUE(all(avel::isunordered(zero, nan)));
        EXPECT_TRUE(all(avel::isunordered(nan, zero)));

        EXPECT_TRUE(none(avel::isunordered(pos_inf, pos_inf)));
        EXPECT_TRUE(all(avel::isunordered(nan, pos_inf)));
        EXPECT_TRUE(all(avel::isunordered(pos_inf, nan)));

        EXPECT_TRUE(none(avel::isunordered(neg_inf, neg_inf)));
        EXPECT_TRUE(all(avel::isunordered(nan, neg_inf)));
        EXPECT_TRUE(all(avel::isunordered(neg_inf, nan)));
    }

    TEST(Vec2x64f, isunordered_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs0 = random_array<arr2x64f>();
            auto inputs1 = random_array<arr2x64f>();

            vec2x64f v0{inputs0};
            vec2x64f v1{inputs1};

            auto results = avel::isunordered(v0, v1);

            arr2xb expected{};
            for (std::size_t j = 0; j < inputs0.size(); ++j) {
                expected[j] = avel::isunordered(inputs0[j], inputs1[j]);
            }

            EXPECT_EQ(results, mask2x64f{expected});
        }
    }

}

#endif //AVEL_VEC2X64F_TESTS_HPP
