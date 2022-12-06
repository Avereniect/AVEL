#ifndef AVEL_VEC8X16I_TESTS_HPP
#define AVEL_VEC8X16I_TESTS_HPP

namespace avel_tests {
    
    using namespace avel;
    
    //=========================================================================
    // mask8x16i tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask8x16i, Construct_from_primitive) {
        mask8x16i mask{mask8x16i::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask8x16i, Construct_from_bool) {
        mask8x16i mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask8x16i mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 8);
    }

    TEST(Mask8x16i, Construct_from_array) {
        mask8x16i mask0{{
            false, false, false, false,
            false, false, false, false
        }};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask8x16i mask1{{
            true, true, true, true,
            true, true, true, true
        }};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 8);

        mask8x16i mask2{{
            true, true, true, true,
            true, true, true, true
        }};

        EXPECT_TRUE(any(mask2));
        EXPECT_TRUE(all(mask2));
        EXPECT_FALSE(none(mask2));
        EXPECT_EQ(count(mask2), 8);

        mask8x16i mask3{{
            false, false, false, true,
            true, false, true, false
        }};

        EXPECT_TRUE(any(mask3));
        EXPECT_FALSE(all(mask3));
        EXPECT_FALSE(none(mask3));
        EXPECT_EQ(count(mask3), 3);
    }

    TEST(Mask8x16i, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 8> input_array0;
            std::uint16_t cnt = 0;
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
                cnt += input_array0[j];
            }

            auto m = mask8x16i{input_array0};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == 8, all(m));
            EXPECT_EQ(cnt != 0, any(m));
            EXPECT_EQ(cnt == 0, none(m));
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask8x16i, Assign_bool) {
        mask8x16i mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask8x16i mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_EQ(count(mask1), 8);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask8x16i, Equality_comparison) {
        mask8x16i mask0{false};
        mask8x16i mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask8x16i mask2{true};
        mask8x16i mask3{true};
        EXPECT_EQ(mask2, mask3);

        mask8x16i mask4{{
            false, false, true,  true,
            false, true,  false, false
        }};

        mask8x16i mask5{{
            false, false, true,  true,
            false, true,  false, false
        }};

        EXPECT_EQ(mask4, mask5);

        mask8x16i mask6{{
            false, false, true,  true,
            false, true,  false, false
        }};

        mask8x16i mask7{{
            true, false, true,  true,
            false, true,  false, false
        }};

        EXPECT_TRUE(!(mask6 == mask7));
    }

    TEST(Mask8x16i, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
            }

            std::array<bool, 8> input_array1{};
            input_array1[0] = !input_array0[0];
            for (std::size_t j = 1; j < input_array0.size(); ++j) {
                input_array1[j] = random16u() & 0x1;
            }

            mask8x16i mask0{input_array0};
            mask8x16i mask1{input_array0};
            mask8x16i mask2{input_array1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask8x16i, Inequality_comparison) {
        mask8x16i mask0{false};
        mask8x16i mask1{false};
        EXPECT_FALSE(mask0 != mask1);

        mask8x16i mask2{true};
        mask8x16i mask3{true};
        EXPECT_FALSE(mask2 != mask3);

        mask8x16i mask4{{
            false, false, true,  true,
            false, true,  false, false
        }};

        mask8x16i mask5{{
            false, false, true,  true,
            false, true,  false, false
        }};

        EXPECT_FALSE(mask4 != mask5);

        mask8x16i mask6{{
            false, false, true,  true,
            false, true,  false, false
        }};

        mask8x16i mask7{{
            true, false, true,  true,
            false, true,  false, false
        }};

        EXPECT_TRUE(mask6 != mask7);
    }

    TEST(Mask8x16i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
            }

            std::array<bool, 8> input_array1{};
            input_array1[0] = !input_array0[0];
            for (std::size_t j = 1; j < input_array0.size(); ++j) {
                input_array1[j] = random16u() & 0x1;
            }

            mask8x16i mask0{input_array0};
            mask8x16i mask1{input_array1};
            mask8x16i mask2{input_array1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask8x16i, Bitwise_and_assignment) {
        mask8x16i mask0{{
            true, true, false, false,
            true, true, false, false
        }};

        mask8x16i mask1{{
            true, false, true, false,
            true, false, true, false
        }};

        mask0 &= mask1;

        mask8x16i mask2{{
            true, false, false, false,
            true, false, false, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    TEST(Mask8x16i, Bitwise_or_assignment) {
        mask8x16i mask0{{
            true, true, false, false,
            true, true, false, false
        }};

        mask8x16i mask1{{
            true, false, true, false,
            true, false, true, false
        }};

        mask0 |= mask1;

        mask8x16i mask2{{
            true, true, true, false,
            true, true, true, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    TEST(Mask8x16i, Bitwise_xor_assignment) {
        mask8x16i mask0{{
            true, true, false, false,
            true, true, false, false
        }};

        mask8x16i mask1{{
            true, false, true, false,
            true, false, true, false
        }};

        mask0 ^= mask1;

        mask8x16i mask2{{
            false, true, true, false,
            false, true, true, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask8x16i, Logical_negation) {
        mask8x16i mask0{{
            false, false, true,  true,
            false, false, true,  true
        }};

        mask8x16i mask1{{
            true,  true,  false, false,
            true,  true,  false, false
        }};

        EXPECT_EQ(mask0, !mask1);
    }

    TEST(Mask8x16i, Bitwise_and) {
        mask8x16i m =
            mask8x16i{{
                true, true, false, false,
                true, true, false, false
            }} &
            mask8x16i{{
                true, false, true, false,
                true, false, true, false,
            }};

        mask8x16i mask2{{
            true, false, false, false,
            true, false, false, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask8x16i, Logical_and) {
        mask8x16i m =
            mask8x16i{{
                true, true, false, false,
                true, true, false, false
            }} &&
            mask8x16i{{
                true, false, true, false,
                true, false, true, false
            }};

        mask8x16i mask2{{
            true, false, false, false,
            true, false, false, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask8x16i, Bitwise_or) {
        mask8x16i m =
            mask8x16i{{
                true, true, false, false,
                true, true, false, false
            }} |
            mask8x16i{{
                true, false, true, false,
                true, false, true, false
            }};

        mask8x16i mask2{{
            true, true, true, false,
            true, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask8x16i, Logical_or) {
        mask8x16i m =
            mask8x16i{{
                true, true, false, false,
                true, true, false, false
            }} ||
            mask8x16i{{
                true, false, true, false,
                true, false, true, false
            }};

        mask8x16i mask2{{
            true, true, true, false,
            true, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask8x16i, Bitwise_xor) {
        mask8x16i m =
            mask8x16i{{
                true, true, false, false,
                true, true, false, false
            }} ^
            mask8x16i{{
                true, false, true, false,
                true, false, true, false
            }};

        mask8x16i mask2{{
            false, true, true, false,
            false, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    //=========================================================================
    // Vec8x16i tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec8x16i, Convert_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 8> data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random16u() & 0x1;
            }

            mask8x16i m0{data};
            vec8x16i v0{m0};

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

    TEST(Vec8x16i, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t x = random16u();
            vec8x16i v{x};
            vec8x16i baseline{{
                x, x, x, x,
                x, x, x, x
            }};

            EXPECT_TRUE(all(v == baseline));
        }
    }

    TEST(Vec8x16i, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> data;

            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random16u();
            }

            vec8x16i v{data};

            auto results = to_array(v);

            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(data[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec8x16i, Assign_primitive) {
        vec8x16i::primitive x{};
        vec8x16i v{0x0F};
        v = x;
        vec8x16i baseline{};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(Vec8x16i, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t scalar = random16u();
            vec8x16i v;
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

    TEST(Vec8x16i, Equality_comparison) {
        mask8x16i mask0{false};
        mask8x16i mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask8x16i mask2{true};
        mask8x16i mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec8x16i, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random16u();
            }

            vec8x16i v0{data};
            vec8x16i v1{data};

            EXPECT_TRUE(all(v0 == v1));
        }
    }

    TEST(Vec8x16i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> data0;
            std::array<std::int16_t, 8> data1;
            for (std::size_t j = 0; j < data0.size(); ++j) {
                data0[j] = random16u();
                data1[j] = random16u() ;
            }

            vec8x16i v0{data0};
            vec8x16i v1{data1};

            if (!all(v0 == v1)) {
                EXPECT_TRUE(any(v0 != v1));
            }
        }
    }

    TEST(Vec8x16i, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = (input0 < input1);

            std::array<bool, 8> results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] < input_array1[j];
            }

            EXPECT_EQ(results, mask8x16i{results_array});
        }
    }

    TEST(Vec8x16i, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = (input0 <= input1);

            std::array<bool, 8> results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] <= input_array1[j];
            }

            EXPECT_EQ(results, mask8x16i{results_array});
        }
    }

    TEST(Vec8x16i, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = (input0 > input1);

            std::array<bool, 8> results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] > input_array1[j];
            }

            EXPECT_EQ(results, mask8x16i{results_array});
        }
    }

    TEST(Vec8x16i, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = (input0 >= input1);

            std::array<bool, 8> results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] >= input_array1[j];
            }

            EXPECT_EQ(results, mask8x16i{results_array});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec8x16i, Unary_plus) {
        const vec8x16i u{{
            0x0, 0x1, 0x2, 0x3,
            0x4, 0x5, 0x6, 0x7
        }};
        vec8x16i v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec8x16i, Unary_minus) {
        const vec8x16i u{{
            0x0, 0x1, 0x2, 0x3,
            0x4, 0x5, 0x6, 0x7
        }};
        vec8x16i v{{
            -0x0, -0x1, -0x2, -0x3,
            -0x4, -0x5, -0x6, -0x7
        }};
        vec8x16i w{-u};

        EXPECT_TRUE(all(v == w));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec8x16i, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};


            auto results = input0;
            results += input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};


            auto results = input0;
            results -= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};


            auto results = input0;
            results *= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0;
            results /= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0;
            results %= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec8x16i, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0 + input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0 - input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0 * input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0 / input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0 % input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec8x16i, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};
            ++input0;

            auto results = input0;
            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};
            input0++;

            auto results = input0;
            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};
            --input0;

            auto results = input0;
            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = +--input_array0[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};
            input0--;

            auto results = input0;
            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec8x16i, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0;
            results &= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0;
            results |= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0;
            results ^= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            std::int8_t input1 = random16u() % 32;

            vec8x16i input0{input_array0};

            auto results = input0;
            results <<= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u() % 32;
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0;
            results <<= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            std::int8_t input1 = random16u() % 32;

            vec8x16i input0{input_array0};

            auto results = input0;
            results >>= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 16) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFF;
                    } else {
                        results_array[j] = 0x0000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u() % 32;
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0;
            results >>= input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 16) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFF;
                    } else {
                        results_array[j] = 0x0000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec8x16i, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto results = ~input0;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ~input_array0[j];
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            std::int8_t input1 = random16u() % 32;

            vec8x16i input0{input_array0};

            auto results = input0 << input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u() % 32;
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0 << input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 16) {
                    results_array[j] = 0x0000;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            std::int8_t input1 = random16u() % 32;

            vec8x16i input0{input_array0};

            auto results = input0 >> input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 16) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFF;
                    } else {
                        results_array[j] = 0x0000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u() % 32;
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = input0 >> input1;

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 16) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFF;
                    } else {
                        results_array[j] = 0x0000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec8x16i, Conversion_to_mask) {
        vec8x16i vec{0};

        auto a = vec.operator mask8x16i();
        mask8x16i b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec8x16i, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto results = mask8x16i{input0};

            std::array<bool, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(results == mask8x16i{results_array});
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec8x16i, broadcast_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
            }

            mask8x16i input0{input_array0};

            auto results = broadcast_mask(input0);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array0[j]) {
                    results_array[j] = 0xffff;
                } else {
                    results_array[j] = 0x0000;
                }
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};
            std::array<bool, 8> input_array2{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
                input_array2[j] = random16u() & 0x1;
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};
            mask8x16i input2{input_array2};

            auto results = blend(input0, input1, input2);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array2[j]) {
                    results_array[j] = input_array1[j];
                } else {
                    results_array[j] = input_array0[j];
                }
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = max(input0, input1);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = min(input0, input1);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::min(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = minmax(input0, input1);

            std::array<std::int16_t, 8> results_array0{};
            std::array<std::int16_t, 8> results_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array0[j] = std::min(input_array0[j], input_array1[j]);
                results_array1[j] = std::max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results[0] == vec8x16i{results_array0}));
            EXPECT_TRUE(all(results[1] == vec8x16i{results_array1}));
        }
    }

    TEST(Vec8x16i, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};
            std::array<std::int16_t, 8> input_array2{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
                input_array2[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};
            vec8x16i input2{input_array2};

            auto xy = minmax(input1, input2);

            auto results = clamp(input0, xy[0], xy[1]);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array1[j], input_array2[j]);
                results_array[j] = clamp(input_array0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = average(input0, input1);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = average(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            std::array<std::int16_t, 8> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            auto results = midpoint(input0, input1);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = midpoint(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto results = abs(input0);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto results = neg_abs(input0);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = neg_abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            auto results = load<vec8x16i>(input_array0.data());

            EXPECT_TRUE(all(results == vec8x16i{input_array0}));
        }
    }

    TEST(Vec8x16i, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec8x16i)) std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            auto results = aligned_load<vec8x16i>(input_array0.data());

            EXPECT_TRUE(all(results == vec8x16i{input_array0}));
        }
    }

    TEST(Vec8x16i, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            std::int16_t arr[8]{};
            store(arr, input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(input_array0[j], arr[j]);
            }
        }
    }

    TEST(Vec8x16i, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            alignas(alignof(vec8x16i)) std::int16_t arr[16]{};
            aligned_store(arr, input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(arr[j], input_array0[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec8x16i, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> numerators{};
            std::array<std::int16_t, 8> denominators{};

            for (std::size_t j = 0; j < numerators.size(); ++j) {
                numerators[j] = random16u();
                denominators[j] = random16u();
                if (denominators[j] == 0) {
                    denominators[j] = 1;
                }
            }

            std::array<std::int16_t, 8> quotients{};
            std::array<std::int16_t, 8> remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec8x16i n{numerators};
            vec8x16i d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec8x16i, Popcount) {
        vec8x16i v{0x0};
        vec8x16i c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec8x16i, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto results = popcount(input0);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = popcount(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto results = byteswap(input0);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = byteswap(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto results = countl_zero(input0);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto results = countl_one(input0);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto results = countr_zero(input0);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    TEST(Vec8x16i, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int16_t, 8> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto results = countr_one(input0);

            std::array<std::int16_t, 8> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }
    
}

#endif //AVEL_VEC8X16I_TESTS_HPP
