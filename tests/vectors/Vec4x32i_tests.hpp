#ifndef AVEL_VEC4X32I_TESTS_HPP
#define AVEL_VEC4X32I_TESTS_HPP

namespace avel_tests {
    
    using namespace avel;
    
    //=========================================================================
    // mas4x32i tests
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
        EXPECT_EQ(count(mask1), 4);
    }

    TEST(Mask4x32i, Construct_from_array) {
        mask4x32i mask0{{
            false, false, false, false
        }};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask4x32i mask1{{
            true, true, true, true
        }};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 4);

        mask4x32i mask2{{
            true, true, true, true
        }};

        EXPECT_TRUE(any(mask2));
        EXPECT_TRUE(all(mask2));
        EXPECT_FALSE(none(mask2));
        EXPECT_EQ(count(mask2), 4);

        mask4x32i mask3{{
            false, true, false, true
        }};

        EXPECT_TRUE(any(mask3));
        EXPECT_FALSE(all(mask3));
        EXPECT_FALSE(none(mask3));
        EXPECT_EQ(count(mask3), 2);
    }

    TEST(Mask4x32i, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0;
            std::uint16_t cnt = 0;
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                cnt += input_array0[j];
            }

            auto m = mask4x32i{input_array0};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == 4, all(m));
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
        EXPECT_EQ(count(mask1), 4);
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

        mask4x32i mask4{{
            false, false, true,  true
        }};

        mask4x32i mask5{{
            false, false, true,  true
        }};

        EXPECT_EQ(mask4, mask5);

        mask4x32i mask6{{
            false, false, true,  true
        }};

        mask4x32i mask7{{
            true, false, true,  true
        }};

        EXPECT_TRUE(!(mask6 == mask7));
    }

    TEST(Mask4x32i, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
            }

            arr4xb input_array1{};
            input_array1[0] = !input_array0[0];
            for (std::size_t j = 1; j < input_array0.size(); ++j) {
                input_array1[j] = random32u() & 0x1;
            }

            mask4x32i mask0{input_array0};
            mask4x32i mask1{input_array0};
            mask4x32i mask2{input_array1};

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

        mask4x32i mask4{{
            false, false, true,  true
        }};

        mask4x32i mask5{{
            false, false, true,  true
        }};

        EXPECT_FALSE(mask4 != mask5);

        mask4x32i mask6{{
            false, false, true,  true
        }};

        mask4x32i mask7{{
            true, false, true,  true
        }};

        EXPECT_TRUE(mask6 != mask7);
    }

    TEST(Mask4x32i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
            }

            arr4xb input_array1{};
            input_array1[0] = !input_array0[0];
            for (std::size_t j = 1; j < input_array0.size(); ++j) {
                input_array1[j] = random32u() & 0x1;
            }

            mask4x32i mask0{input_array0};
            mask4x32i mask1{input_array1};
            mask4x32i mask2{input_array1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask4x32i, Bitwise_and_assignment) {
        mask4x32i mask0{{
            true, true, false, false
        }};

        mask4x32i mask1{{
            true, false, true, false
        }};

        mask0 &= mask1;

        mask4x32i mask2{{
            true, false, false, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    TEST(Mask4x32i, Bitwise_or_assignment) {
        mask4x32i mask0{{
            true, true, false, false
        }};

        mask4x32i mask1{{
            true, false, true, false
        }};

        mask0 |= mask1;

        mask4x32i mask2{{
            true, true, true, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    TEST(Mask4x32i, Bitwise_xor_assignment) {
        mask4x32i mask0{{
            true, true, false, false
        }};

        mask4x32i mask1{{
            true, false, true, false
        }};

        mask0 ^= mask1;

        mask4x32i mask2{{
            false, true, true, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask4x32i, Logical_negation) {
        mask4x32i mask0{{
            false, false, true,  true
        }};

        mask4x32i mask1{{
            true,  true,  false, false
        }};

        EXPECT_EQ(mask0, !mask1);
    }

    TEST(Mask4x32i, Bitwise_and) {
        mask4x32i m =
            mask4x32i{{
                true, true, false, false
            }} &
            mask4x32i{{
                true, false, true, false
            }};

        mask4x32i mask2{{
            true, false, false, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask4x32i, Logical_and) {
        mask4x32i m =
            mask4x32i{{
                true, true, false, false
            }} &&
            mask4x32i{{
                true, false, true, false
            }};

        mask4x32i mask2{{
            true, false, false, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask4x32i, Bitwise_or) {
        mask4x32i m =
            mask4x32i{{
                true, true, false, false
            }} |
            mask4x32i{{
                true, false, true, false
            }};

        mask4x32i mask2{{
            true, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask4x32i, Logical_or) {
        mask4x32i m =
            mask4x32i{{
                true, true, false, false
            }} ||
            mask4x32i{{
                true, false, true, false
            }};

        mask4x32i mask2{{
            true, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask4x32i, Bitwise_xor) {
        mask4x32i m =
            mask4x32i{{
                true, true, false, false
            }} ^
            mask4x32i{{
                true, false, true, false
            }};

        mask4x32i mask2{{
            false, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    //=========================================================================
    // Vec4x32i tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec4x32i, Convert_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random32u() & 0x1;
            }

            mask4x32i m0{data};
            vec4x32i v0{m0};

            auto results = to_array(v0);
            for (std::size_t j = 0; j < data.size(); ++j) {
                if (data[j]) {
                    EXPECT_EQ(results[j], 0x1);
                } else {
                    EXPECT_EQ(results[j], 0x0);
                }
            }
        }
    }

    TEST(Vec4x32i, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t x = random32u();
            vec4x32i v{x};
            vec4x32i baseline{{
                x, x, x, x
            }};

            EXPECT_TRUE(all(v == baseline));
        }
    }

    TEST(Vec4x32i, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i data;

            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random32u();
            }

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
        vec4x32i baseline{};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(Vec4x32i, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t scalar = random32u();
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
            arr4x32i data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random32u();
            }

            vec4x32i v0{data};
            vec4x32i v1{data};

            EXPECT_TRUE(all(v0 == v1));
        }
    }

    TEST(Vec4x32i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i data0;
            arr4x32i data1;
            for (std::size_t j = 0; j < data0.size(); ++j) {
                data0[j] = random32u();
                data1[j] = random32u() ;
            }

            vec4x32i v0{data0};
            vec4x32i v1{data1};

            if (!all(v0 == v1)) {
                EXPECT_TRUE(any(v0 != v1));
            }
        }
    }

    TEST(Vec4x32i, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = (input0 < input1);

            arr4xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] < input_array1[j];
            }

            EXPECT_EQ(results, mask4x32i{results_array});
        }
    }

    TEST(Vec4x32i, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = (input0 <= input1);

            arr4xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] <= input_array1[j];
            }

            EXPECT_EQ(results, mask4x32i{results_array});
        }
    }

    TEST(Vec4x32i, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = (input0 > input1);

            arr4xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] > input_array1[j];
            }

            EXPECT_EQ(results, mask4x32i{results_array});
        }
    }

    TEST(Vec4x32i, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = (input0 >= input1);

            arr4xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] >= input_array1[j];
            }

            EXPECT_EQ(results, mask4x32i{results_array});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec4x32i, Unary_plus) {
        const vec4x32i u{{
            0x0, 0x1, 0x2, 0x3
        }};
        vec4x32i v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec4x32i, Unary_minus) {
        const vec4x32i u{{
            0x0, 0x1, 0x2, 0x3
        }};
        vec4x32i v{{
            -0x0, -0x1, -0x2, -0x3
        }};
        vec4x32i w{-u};

        EXPECT_TRUE(all(v == w));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec4x32i, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};


            auto results = input0;
            results += input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};


            auto results = input0;
            results -= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};


            auto results = input0;
            results *= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0;
            results /= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = std::max(static_cast<std::int32_t>(random32u()), static_cast<std::int32_t>(1u));
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0;
            results %= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec4x32i, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0 + input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0 - input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0 * input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0 / input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = std::max(static_cast<std::int32_t>(random32u()), static_cast<std::int32_t>(1u));
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0 % input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec4x32i, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};
            ++input0;

            auto results = input0;
            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};
            input0++;

            auto results = input0;
            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};
            --input0;

            auto results = input0;
            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = +--input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};
            input0--;

            auto results = input0;
            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec4x32i, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0;
            results &= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0;
            results |= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0;
            results ^= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::int8_t input1 = random32u() % 33;

            vec4x32i input0{input_array0};

            auto results = input0;
            results <<= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u() % 33;
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0;
            results <<= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::int8_t input1 = random32u() % 33;

            vec4x32i input0{input_array0};

            auto results = input0;
            results >>= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 32) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFFFFFF;
                    } else {
                        results_array[j] = 0x00000000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u() % 33;
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0;
            results >>= input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 32) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFFFFFF;
                    } else {
                        results_array[j] = 0x00000000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec4x32i, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto results = ~input0;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ~input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::int8_t input1 = random32u() % 33;

            vec4x32i input0{input_array0};

            auto results = input0 << input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u() % 33;
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0 << input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::int8_t input1 = random32u() % 33;

            vec4x32i input0{input_array0};

            auto results = input0 >> input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 32) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFFFFFF;
                    } else {
                        results_array[j] = 0x00000000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u() % 33;
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = input0 >> input1;

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 32) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFFFFFF;
                    } else {
                        results_array[j] = 0x00000000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
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
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto results = mask4x32i{input0};

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(results == mask4x32i{results_array});
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec4x32i, Broadcast_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
            }

            mask4x32i input0{input_array0};

            auto results = broadcast_mask(input0);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array0[j]) {
                    results_array[j] = 0xffffffff;
                } else {
                    results_array[j] = 0x00000000;
                }
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4x32i input_array1{};
            arr4x32i input_array2{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                input_array1[j] = random32u();
                input_array2[j] = random32u();
            }

            mask4x32i input0{input_array0};
            vec4x32i input1{input_array1};
            vec4x32i input2{input_array2};

            auto results = blend(input0, input1, input2);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = blend(input_array0[j], input_array1[j], input_array2[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = max(input0, input1);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = min(input0, input1);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::min(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = minmax(input0, input1);

            arr4x32i results_array0{};
            arr4x32i results_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array0[j], input_array1[j]);
                results_array0[j] = xy[0];
                results_array1[j] = xy[1];
            }

            EXPECT_TRUE(all(results[0] == vec4x32i{results_array0}));
            EXPECT_TRUE(all(results[1] == vec4x32i{results_array1}));
        }
    }

    TEST(Vec4x32i, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};
            arr4x32i input_array2{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
                input_array2[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};
            vec4x32i input2{input_array2};

            auto xy = minmax(input1, input2);

            auto results = clamp(input0, xy[0], xy[1]);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array1[j], input_array2[j]);
                results_array[j] = clamp(input_array0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = average(input0, input1);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = average(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = midpoint(input0, input1);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = midpoint(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(vec4x32i, Negate_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4x32i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
                input_array1[j] = random16u();
            }

            mask4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            auto results = negate(input0, input1);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = negate(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto results = abs(input0);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = avel::abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto results = neg_abs(input0);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = neg_abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec4x32i, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array{};
            for (std::size_t j = 0; j < input_array.size(); ++j) {
                input_array[j] = random32u();
            }

            for (std::uint32_t j = 0; j <= vec4x32i::width; ++j) {
                auto results = load<vec4x32i>(input_array.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec4x32i::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(input_array[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec4x32i, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            EXPECT_TRUE(all(load<vec4x32i, 0x0>(input_array0.data()) == load<vec4x32i>(input_array0.data(), 0x0)));
            EXPECT_TRUE(all(load<vec4x32i, 0x1>(input_array0.data()) == load<vec4x32i>(input_array0.data(), 0x1)));
            EXPECT_TRUE(all(load<vec4x32i, 0x2>(input_array0.data()) == load<vec4x32i>(input_array0.data(), 0x2)));
            EXPECT_TRUE(all(load<vec4x32i, 0x3>(input_array0.data()) == load<vec4x32i>(input_array0.data(), 0x3)));
            EXPECT_TRUE(all(load<vec4x32i, 0x4>(input_array0.data()) == load<vec4x32i>(input_array0.data(), 0x4)));
        }
    }

    TEST(Vec4x32i, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec4x32i)) arr4x32i input_array{};
            for (std::size_t j = 0; j < input_array.size(); ++j) {
                input_array[j] = random32u();
            }

            for (std::uint32_t j = 0; j <= vec4x32i::width; ++j) {
                auto results = aligned_load<vec4x32i>(input_array.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec4x32i::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(input_array[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec4x32i, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec4x32i)) arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            EXPECT_TRUE(all(aligned_load<vec4x32i, 0x0>(input_array0.data()) == aligned_load<vec4x32i>(input_array0.data(), 0x0)));
            EXPECT_TRUE(all(aligned_load<vec4x32i, 0x1>(input_array0.data()) == aligned_load<vec4x32i>(input_array0.data(), 0x1)));
            EXPECT_TRUE(all(aligned_load<vec4x32i, 0x2>(input_array0.data()) == aligned_load<vec4x32i>(input_array0.data(), 0x2)));
            EXPECT_TRUE(all(aligned_load<vec4x32i, 0x3>(input_array0.data()) == aligned_load<vec4x32i>(input_array0.data(), 0x3)));
            EXPECT_TRUE(all(aligned_load<vec4x32i, 0x4>(input_array0.data()) == aligned_load<vec4x32i>(input_array0.data(), 0x4)));
        }
    }

    TEST(Vec4x32i, Gather_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;

        std::vector<std::int32_t> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random32u();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = random32u() % test_data_size;
            }
            auto indices = load<vec4x32i>(index_array.data());

            for (std::uint32_t j = 0; j <= vec4x32i::width; ++j) {
                auto results = gather<vec4x32i>(test_data.data(), indices, j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec4x32i::width; k++) {
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

        std::vector<std::int32_t> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random32u();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = random32u() % test_data_size;
            }
            auto indices = load<vec4x32i>(index_array.data());

            EXPECT_TRUE(all(gather<vec4x32i, 0x00>(test_data.data(), indices) == gather<vec4x32i>(test_data.data(), indices, 0x00)));
            EXPECT_TRUE(all(gather<vec4x32i, 0x01>(test_data.data(), indices) == gather<vec4x32i>(test_data.data(), indices, 0x01)));
            EXPECT_TRUE(all(gather<vec4x32i, 0x02>(test_data.data(), indices) == gather<vec4x32i>(test_data.data(), indices, 0x02)));
            EXPECT_TRUE(all(gather<vec4x32i, 0x03>(test_data.data(), indices) == gather<vec4x32i>(test_data.data(), indices, 0x03)));
            EXPECT_TRUE(all(gather<vec4x32i, 0x04>(test_data.data(), indices) == gather<vec4x32i>(test_data.data(), indices, 0x04)));
        }
    }

    TEST(Vec4x32i, Store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            for (std::size_t j = 0; j <= vec4x32i::width; ++j) {
                std::int32_t arr[4]{};
                store(arr, input0, j);

                for (std::size_t k = 0; k < input_array0.size(); ++k) {
                    if (k < j) {
                        EXPECT_EQ(input_array0[k], arr[k]);
                    } else {
                        EXPECT_EQ(0x00, arr[k]);
                    }
                }
            }
        }
    }

    TEST(Vec4x32i, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            std::int32_t arr[4]{};

            store<0x0>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0));
            store<0x1>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x1));
            store<0x2>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x2));
            store<0x3>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x3));
            store<0x4>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x4));
        }
    }

    TEST(Vec4x32i, Aligned_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            for (std::size_t j = 0; j <= vec4x32i::width; ++j) {
                alignas(alignof(vec4x32i)) std::int32_t arr[4]{};
                aligned_store(arr, input0, j);

                for (std::size_t k = 0; k < input_array0.size(); ++k) {
                    if (k < j) {
                        EXPECT_EQ(input_array0[k], arr[k]);
                    } else {
                        EXPECT_EQ(0x00, arr[k]);
                    }
                }
            }
        }
    }

    TEST(Vec4x32i, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            alignas(alignof(vec4x32i)) std::int32_t arr[4]{};

            aligned_store<0x0>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0));
            aligned_store<0x1>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x1));
            aligned_store<0x2>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x2));
            aligned_store<0x3>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x3));
            aligned_store<0x4>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x4));
        }
    }

    TEST(Vec4x32i, Scatter_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;
        static constexpr std::size_t block_size = test_data_size / vec4x32i::width;

        std::vector<std::int32_t> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            arr4x32i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random32u() % block_size) + (j * block_size);
            }

            vec4x32i indices{index_array};

            for (std::size_t j = 0; j <= vec4x32i::width; ++j) {
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

        std::vector<std::int32_t> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            arr4x32i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random32u() % block_size) + (j * block_size);
            }

            vec4x32i indices{index_array};

            scatter<0x0>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x00));
            scatter<0x1>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x01));
            scatter<0x2>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x02));
            scatter<0x3>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x03));
            scatter<0x4>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x04));
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec4x32i, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            arr4x32i numerators{};
            arr4x32i denominators{};
            for (std::size_t j = 0; j < numerators.size(); ++j) {
                numerators[j] = random32u();
                denominators[j] = random32u();
                if (denominators[j] == 0) {
                    denominators[j] = 1;
                }
            }

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
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto results = popcount(input0);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = popcount(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto results = byteswap(input0);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = byteswap(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto results = countl_zero(input0);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto results = countl_one(input0);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto results = countr_zero(input0);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }

    TEST(Vec4x32i, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto results = countr_one(input0);

            arr4x32i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32i{results_array}));
        }
    }
    
}

#endif
