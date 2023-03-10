#ifndef AVEL_VEC8X16U_TESTS_HPP
#define AVEL_VEC8X16U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // mask8x16u tests
    //=====================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask8x16u, Construct_from_primitive) {
        mask8x16u mask{mask8x16u::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask8x16u, Construct_from_bool) {
        mask8x16u mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask8x16u mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 8);
    }

    TEST(Mask8x16u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8xb input_array0;
            std::uint16_t cnt = 0;
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
                cnt += input_array0[j];
            }

            mask8x16u m{input_array0};

            EXPECT_EQ(count(m), cnt);
            EXPECT_EQ(any(m), cnt != 0);
            EXPECT_EQ(all(m), cnt == 8);
            EXPECT_EQ(none(m), cnt == 0);
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask8x16u, Assign_bool) {
        mask8x16u mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask8x16u mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_EQ(count(mask1), 8);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask8x16u, Equality_comparison) {
        mask8x16u mask0{false};
        mask8x16u mask1{false};

        mask8x16u mask2{true};
        mask8x16u mask3{true};

        EXPECT_TRUE(mask0 == mask1);
        EXPECT_TRUE(mask2 == mask3);

        EXPECT_FALSE(mask0 == mask2);
        EXPECT_FALSE(mask3 == mask1);
    }

    TEST(Mask8x16u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
            }

            arr8xb input_array1{};
            input_array1[0] = !input_array0[0];
            for (std::size_t j = 1; j < input_array0.size(); ++j) {
                input_array1[j] = random16u() & 0x1;
            }

            mask8x16u mask0{input_array0};
            mask8x16u mask1{input_array0};
            mask8x16u mask2{input_array1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask8x16u, Inequality_comparison) {
        mask8x16u mask0{false};
        mask8x16u mask1{false};

        mask8x16u mask2{true};
        mask8x16u mask3{true};

        EXPECT_TRUE(mask0 != mask2);
        EXPECT_TRUE(mask1 != mask3);

        EXPECT_FALSE(mask0 != mask1);
        EXPECT_FALSE(mask2 != mask3);
    }

    TEST(Mask8x16u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
            }

            arr8xb input_array1{};
            input_array1[0] = !input_array0[0];
            for (std::size_t j = 1; j < input_array0.size(); ++j) {
                input_array1[j] = random16u() & 0x1;
            }

            mask8x16u mask0{input_array0};
            mask8x16u mask1{input_array1};
            mask8x16u mask2{input_array1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask8x16u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < 8; ++i) {
            arr8xb input_array0;
            arr8xb input_array1;

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            mask8x16u input0{input_array0};
            mask8x16u input1{input_array1};

            auto results = input0;
            results &= input1;

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    TEST(Mask8x16u, Bitwise_or_assignment) {
        for (std::size_t i = 0; i < 8; ++i) {
            arr8xb input_array0;
            arr8xb input_array1;

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            mask8x16u input0{input_array0};
            mask8x16u input1{input_array1};

            auto results = input0;
            results |= input1;

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    TEST(Mask8x16u, Bitwise_xor_assignment) {
        for (std::size_t i = 0; i < 8; ++i) {
            arr8xb input_array0;
            arr8xb input_array1;

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            mask8x16u input0{input_array0};
            mask8x16u input1{input_array1};

            auto results = input0;
            results ^= input1;

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask8x16u, Logical_negation) {
        for (std::size_t i = 0; i < 8; ++i) {
            arr8xb input_array0;

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            mask8x16u input0{input_array0};

            auto results = !input0;

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = !input_array0[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    TEST(Mask8x16u, Bitwise_and) {
        for (std::size_t i = 0; i < 8; ++i) {
            arr8xb input_array0;
            arr8xb input_array1;

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            mask8x16u input0{input_array0};
            mask8x16u input1{input_array1};

            auto results = input0 & input1;

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    TEST(Mask8x16u, Logical_and) {
        for (std::size_t i = 0; i < 8; ++i) {
            arr8xb input_array0;
            arr8xb input_array1;

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            mask8x16u input0{input_array0};
            mask8x16u input1{input_array1};

            auto results = input0 && input1;

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] && input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    TEST(Mask8x16u, Bitwise_or) {
        for (std::size_t i = 0; i < 8; ++i) {
            arr8xb input_array0;
            arr8xb input_array1;

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            mask8x16u input0{input_array0};
            mask8x16u input1{input_array1};

            auto results = input0 | input1;

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    TEST(Mask8x16u, Logical_or) {
        for (std::size_t i = 0; i < 8; ++i) {
            arr8xb input_array0;
            arr8xb input_array1;

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            mask8x16u input0{input_array0};
            mask8x16u input1{input_array1};

            auto results = input0 || input1;

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] || input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    TEST(Mask8x16u, Bitwise_xor) {
        for (std::size_t i = 0; i < 8; ++i) {
            arr8xb input_array0;
            arr8xb input_array1;

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            mask8x16u input0{input_array0};
            mask8x16u input1{input_array1};

            auto results = input0 ^ input1;

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    //=========================================================================
    // Vec8x16u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec8x16u, Convert_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8xb data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random16u() & 0x1;
            }

            mask8x16u m0{data};
            vec8x16u v0{m0};

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

    TEST(Vec8x16u, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random16u();
            vec8x16u v{x};
            vec8x16u baseline{{
                x, x, x, x,
                x, x, x, x
            }};

            EXPECT_TRUE(all(v == baseline));
        }
    }

    TEST(Vec8x16u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u data;

            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random16u();
            }

            vec8x16u v{data};

            auto results = to_array(v);

            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(data[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec8x16u, Assign_primitive) {
        vec8x16u::primitive x{};
        vec8x16u v{0x0F};
        v = x;
        vec8x16u baseline{};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(Vec8x16u, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t scalar = random16u();
            vec8x16u v;
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

    TEST(Vec8x16u, Equality_comparison) {
        mask8x16u mask0{false};
        mask8x16u mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask8x16u mask2{true};
        mask8x16u mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec8x16u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random16u();
            }

            vec8x16u v0{data};
            vec8x16u v1{data};

            EXPECT_TRUE(all(v0 == v1));
        }
    }

    TEST(Vec8x16u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u data0;
            arr8x16u data1;
            for (std::size_t j = 0; j < data0.size(); ++j) {
                data0[j] = random16u();
                data1[j] = random16u() ;
            }

            vec8x16u v0{data0};
            vec8x16u v1{data1};

            if (!all(v0 == v1)) {
                EXPECT_TRUE(any(v0 != v1));
            }
        }
    }

    TEST(Vec8x16u, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = (input0 < input1);

            arr8xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] < input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    TEST(Vec8x16u, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = (input0 <= input1);

            arr8xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] <= input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    TEST(Vec8x16u, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = (input0 > input1);

            arr8xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] > input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    TEST(Vec8x16u, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = (input0 >= input1);

            arr8xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] >= input_array1[j];
            }

            EXPECT_EQ(results, mask8x16u{results_array});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec8x16u, Unary_plus) {
        const vec8x16u u{{
            0x0, 0x1, 0x2, 0x3,
            0x4, 0x5, 0x6, 0x7
        }};
        vec8x16u v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec8x16u, Unary_minus) {
        const vec8x16u u{{
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

    TEST(Vec8x16u, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0;
            results += input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0;
            results -= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0;
            results *= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = std::max(static_cast<std::uint8_t>(random16u()), static_cast<std::uint8_t>(1u));
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0;
            results /= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = std::max(static_cast<std::uint8_t>(random16u()), static_cast<std::uint8_t>(1u));
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0;
            results %= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec8x16u, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0 + input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0 - input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0 * input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = std::max(static_cast<std::uint8_t>(random16u()), static_cast<std::uint8_t>(1u));
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0 / input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = std::max(static_cast<std::uint8_t>(random16u()), static_cast<std::uint8_t>(1u));
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0 % input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec8x16u, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};
            ++input0;

            auto results = input0;
            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};
            input0++;

            auto results = input0;
            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};
            --input0;

            auto results = input0;
            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = +--input_array0[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};
            input0--;

            auto results = input0;
            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec8x16u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0;
            results &= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0;
            results |= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0;
            results ^= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            std::uint8_t input1 = random16u() % 17;

            vec8x16u input0{input_array0};

            auto results = input0;
            results <<= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u() % 17;
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0;
            results <<= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            std::uint8_t input1 = random16u() % 17;

            vec8x16u input0{input_array0};

            auto results = input0;
            results >>= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u() % 17;
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0;
            results >>= input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec8x16u, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = ~input0;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ~input_array0[j];
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            std::uint8_t input1 = random16u() % 17;

            vec8x16u input0{input_array0};

            auto results = input0 << input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u() % 17;
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0 << input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            std::uint8_t input1 = random16u() % 17;

            vec8x16u input0{input_array0};

            auto results = input0 >> input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u() % 17;
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = input0 >> input1;

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 16) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec8x16u, Conversion_to_mask) {
        vec8x16u vec{0};

        auto a = vec.operator mask8x16u();
        mask8x16u b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec8x16u, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = mask8x16u{input0};

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(results == mask8x16u{results_array});
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec8x16u, Broadcast_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
            }

            mask8x16u input0{input_array0};

            auto results = broadcast_mask(input0);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array0[j]) {
                    results_array[j] = 0xffff;
                } else {
                    results_array[j] = 0x00;
                }
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8xb input_array0{};
            arr8x16u input_array1{};
            arr8x16u input_array2{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
                input_array1[j] = random16u();
                input_array2[j] = random16u();
            }

            mask8x16u input0{input_array0};
            vec8x16u input1{input_array1};
            vec8x16u input2{input_array2};

            auto results = blend(input0, input1, input2);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = blend(input_array0[j], input_array1[j], input_array2[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = max(input0, input1);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = min(input0, input1);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::min(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = minmax(input0, input1);

            arr8x16u results_array0{};
            arr8x16u results_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array0[j], input_array1[j]);
                results_array0[j] = xy[0];
                results_array1[j] = xy[1];
            }

            EXPECT_TRUE(all(results[0] == vec8x16u{results_array0}));
            EXPECT_TRUE(all(results[1] == vec8x16u{results_array1}));
        }
    }

    TEST(Vec8x16u, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};
            arr8x16u input_array2{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
                input_array2[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};
            vec8x16u input2{input_array2};

            auto xy = minmax(input1, input2);

            auto results = clamp(input0, xy[0], xy[1]);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array1[j], input_array2[j]);
                results_array[j] = clamp(input_array0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = average(input0, input1);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = average(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = midpoint(input0, input1);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = midpoint(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = neg_abs(input0);

            arr8x16i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = neg_abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16i{results_array}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec8x16u, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array{};
            for (std::size_t j = 0; j < input_array.size(); ++j) {
                input_array[j] = random16u();
            }

            for (std::uint32_t j = 0; j <= vec8x16u::width; ++j) {
                auto results = load<vec8x16u>(input_array.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec8x16u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(input_array[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec8x16u, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            EXPECT_TRUE(all(load<vec8x16u, 0x00>(input_array0.data()) == load<vec8x16u>(input_array0.data(), 0x00)));
            EXPECT_TRUE(all(load<vec8x16u, 0x01>(input_array0.data()) == load<vec8x16u>(input_array0.data(), 0x01)));
            EXPECT_TRUE(all(load<vec8x16u, 0x02>(input_array0.data()) == load<vec8x16u>(input_array0.data(), 0x02)));
            EXPECT_TRUE(all(load<vec8x16u, 0x03>(input_array0.data()) == load<vec8x16u>(input_array0.data(), 0x03)));
            EXPECT_TRUE(all(load<vec8x16u, 0x04>(input_array0.data()) == load<vec8x16u>(input_array0.data(), 0x04)));
            EXPECT_TRUE(all(load<vec8x16u, 0x05>(input_array0.data()) == load<vec8x16u>(input_array0.data(), 0x05)));
            EXPECT_TRUE(all(load<vec8x16u, 0x06>(input_array0.data()) == load<vec8x16u>(input_array0.data(), 0x06)));
            EXPECT_TRUE(all(load<vec8x16u, 0x07>(input_array0.data()) == load<vec8x16u>(input_array0.data(), 0x07)));
            EXPECT_TRUE(all(load<vec8x16u, 0x08>(input_array0.data()) == load<vec8x16u>(input_array0.data(), 0x08)));
        }
    }

    TEST(Vec8x16u, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec8x16u)) arr8x16u input_array{};
            for (std::size_t j = 0; j < input_array.size(); ++j) {
                input_array[j] = random16u();
            }

            for (std::uint32_t j = 0; j <= vec8x16u::width; ++j) {
                auto results = aligned_load<vec8x16u>(input_array.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec8x16u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(input_array[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec8x16u, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec8x16u)) arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            EXPECT_TRUE(all(aligned_load<vec8x16u, 0x00>(input_array0.data()) == aligned_load<vec8x16u>(input_array0.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec8x16u, 0x01>(input_array0.data()) == aligned_load<vec8x16u>(input_array0.data(), 0x01)));
            EXPECT_TRUE(all(aligned_load<vec8x16u, 0x02>(input_array0.data()) == aligned_load<vec8x16u>(input_array0.data(), 0x02)));
            EXPECT_TRUE(all(aligned_load<vec8x16u, 0x03>(input_array0.data()) == aligned_load<vec8x16u>(input_array0.data(), 0x03)));
            EXPECT_TRUE(all(aligned_load<vec8x16u, 0x04>(input_array0.data()) == aligned_load<vec8x16u>(input_array0.data(), 0x04)));
            EXPECT_TRUE(all(aligned_load<vec8x16u, 0x05>(input_array0.data()) == aligned_load<vec8x16u>(input_array0.data(), 0x05)));
            EXPECT_TRUE(all(aligned_load<vec8x16u, 0x06>(input_array0.data()) == aligned_load<vec8x16u>(input_array0.data(), 0x06)));
            EXPECT_TRUE(all(aligned_load<vec8x16u, 0x07>(input_array0.data()) == aligned_load<vec8x16u>(input_array0.data(), 0x07)));
            EXPECT_TRUE(all(aligned_load<vec8x16u, 0x08>(input_array0.data()) == aligned_load<vec8x16u>(input_array0.data(), 0x08)));
        }
    }

    TEST(Vec8x16u, Store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            for (std::size_t j = 0; j <= vec8x16u::width; ++j) {
                std::uint16_t arr[8]{};
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

    TEST(Vec8x16u, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec8x16u input0{input_array0};

            std::uint16_t arr[8]{};

            store<0x00>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x00));
            store<0x01>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x01));
            store<0x02>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x02));
            store<0x03>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x03));
            store<0x04>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x04));
            store<0x05>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x05));
            store<0x06>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x06));
            store<0x07>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x07));
            store<0x08>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x08));
        }
    }

    TEST(Vec8x16u, Aligned_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            for (std::size_t j = 0; j <= vec8x16u::width; ++j) {
                alignas(alignof(vec8x16u))std::uint16_t arr[8]{};
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

    TEST(Vec8x16u, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec8x16u input0{input_array0};

            alignas(alignof(vec8x16u)) std::uint16_t arr[8]{};

            aligned_store<0x00>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x00));
            aligned_store<0x01>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x01));
            aligned_store<0x02>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x02));
            aligned_store<0x03>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x03));
            aligned_store<0x04>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x04));
            aligned_store<0x05>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x05));
            aligned_store<0x06>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x06));
            aligned_store<0x07>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x07));
            aligned_store<0x08>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x08));
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec8x16u, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            arr8x16u numerators{};
            for (std::size_t j = 0; j < numerators.size(); ++j) {
                numerators[j] = random16u();
            }

            arr8x16u denominators{};
            for (std::size_t j = 0; j < denominators.size(); ++j) {
                denominators[j] = std::max(static_cast<std::uint8_t>(random16u()), static_cast<std::uint8_t>(1u));
            }

            arr8x16u quotients{};
            arr8x16u remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec8x16u n{numerators};
            vec8x16u d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec8x16u, Popcount) {
        vec8x16u v0{0x0000};
        vec8x16u b0{0x00};
        vec8x16u c0 = popcount(v0);
        EXPECT_TRUE(all(c0 == b0));

        vec8x16u v1{0xFFFF};
        vec8x16u b1{0x10};
        vec8x16u c1 = popcount(v1);
        EXPECT_TRUE(all(c1 == b1));
    }

    TEST(Vec8x16u, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = popcount(input0);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = popcount(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = byteswap(input0);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = byteswap(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = countl_zero(input0);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = countl_one(input0);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = countr_zero(input0);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = countr_one(input0);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Bit_width_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = bit_width(input0);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_width(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Bit_floor_edge_cases) {
        EXPECT_TRUE(all(vec8x16u{0} == bit_floor(vec8x16u{0x00})));

        for (std::size_t i = 0; i < 15; ++i) {
            vec8x16u::scalar v = vec8x16u::scalar(1) << i;
            vec8x16u results = bit_floor(vec8x16u{v});
            vec8x16u expected{v};
            EXPECT_TRUE(all(results == expected));
        }
    }

    TEST(Vec8x16u, Bit_floor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = bit_floor(input0);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_floor(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Bit_ceil_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = bit_ceil(input0);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_ceil(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Has_single_bit) {
        for (std::size_t i = 0; i < 16; ++i) {
            EXPECT_TRUE(all(has_single_bit(vec8x16u{static_cast<std::uint16_t>(1 << i)})));
        }
    }

    TEST(Vec8x16u, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = has_single_bit(input0);

            arr8xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = has_single_bit(input_array0[j]);
            }

            EXPECT_TRUE(results == mask8x16u{results_array});
        }
    }

    //=====================================================
    // Bit Manipulation Instructions
    //=====================================================

    TEST(Vec8x16u, Bit_shift_left) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            EXPECT_TRUE(all(bit_shift_left<0x00>(input0) == (input0 << 0x00)));
            EXPECT_TRUE(all(bit_shift_left<0x01>(input0) == (input0 << 0x01)));
            EXPECT_TRUE(all(bit_shift_left<0x02>(input0) == (input0 << 0x02)));
            EXPECT_TRUE(all(bit_shift_left<0x03>(input0) == (input0 << 0x03)));
            EXPECT_TRUE(all(bit_shift_left<0x04>(input0) == (input0 << 0x04)));
            EXPECT_TRUE(all(bit_shift_left<0x05>(input0) == (input0 << 0x05)));
            EXPECT_TRUE(all(bit_shift_left<0x06>(input0) == (input0 << 0x06)));
            EXPECT_TRUE(all(bit_shift_left<0x07>(input0) == (input0 << 0x07)));
            EXPECT_TRUE(all(bit_shift_left<0x08>(input0) == (input0 << 0x08)));
            EXPECT_TRUE(all(bit_shift_left<0x09>(input0) == (input0 << 0x09)));
            EXPECT_TRUE(all(bit_shift_left<0x0a>(input0) == (input0 << 0x0a)));
            EXPECT_TRUE(all(bit_shift_left<0x0b>(input0) == (input0 << 0x0b)));
            EXPECT_TRUE(all(bit_shift_left<0x0c>(input0) == (input0 << 0x0c)));
            EXPECT_TRUE(all(bit_shift_left<0x0d>(input0) == (input0 << 0x0d)));
            EXPECT_TRUE(all(bit_shift_left<0x0e>(input0) == (input0 << 0x0e)));
            EXPECT_TRUE(all(bit_shift_left<0x0f>(input0) == (input0 << 0x0f)));
            EXPECT_TRUE(all(bit_shift_left<0x10>(input0) == vec8x16u{0x00}));
        }
    }

    TEST(Vec8x16u, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            EXPECT_TRUE(all(bit_shift_right<0x00>(input0) == (input0 >> 0x00)));
            EXPECT_TRUE(all(bit_shift_right<0x01>(input0) == (input0 >> 0x01)));
            EXPECT_TRUE(all(bit_shift_right<0x02>(input0) == (input0 >> 0x02)));
            EXPECT_TRUE(all(bit_shift_right<0x03>(input0) == (input0 >> 0x03)));
            EXPECT_TRUE(all(bit_shift_right<0x04>(input0) == (input0 >> 0x04)));
            EXPECT_TRUE(all(bit_shift_right<0x05>(input0) == (input0 >> 0x05)));
            EXPECT_TRUE(all(bit_shift_right<0x06>(input0) == (input0 >> 0x06)));
            EXPECT_TRUE(all(bit_shift_right<0x07>(input0) == (input0 >> 0x07)));
            EXPECT_TRUE(all(bit_shift_right<0x08>(input0) == (input0 >> 0x08)));
            EXPECT_TRUE(all(bit_shift_right<0x09>(input0) == (input0 >> 0x09)));
            EXPECT_TRUE(all(bit_shift_right<0x0a>(input0) == (input0 >> 0x0a)));
            EXPECT_TRUE(all(bit_shift_right<0x0b>(input0) == (input0 >> 0x0b)));
            EXPECT_TRUE(all(bit_shift_right<0x0c>(input0) == (input0 >> 0x0c)));
            EXPECT_TRUE(all(bit_shift_right<0x0d>(input0) == (input0 >> 0x0d)));
            EXPECT_TRUE(all(bit_shift_right<0x0e>(input0) == (input0 >> 0x0e)));
            EXPECT_TRUE(all(bit_shift_right<0x0f>(input0) == (input0 >> 0x0f)));
            EXPECT_TRUE(all(bit_shift_right<0x10>(input0) == vec8x16u{0x00}));
        }
    }

    TEST(Vec8x16u, Rotl) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            EXPECT_TRUE(all(rotl<0x0>(input0) == (rotl(input0, 0x0))));
            EXPECT_TRUE(all(rotl<0x1>(input0) == (rotl(input0, 0x1))));
            EXPECT_TRUE(all(rotl<0x2>(input0) == (rotl(input0, 0x2))));
            EXPECT_TRUE(all(rotl<0x3>(input0) == (rotl(input0, 0x3))));
            EXPECT_TRUE(all(rotl<0x4>(input0) == (rotl(input0, 0x4))));
            EXPECT_TRUE(all(rotl<0x5>(input0) == (rotl(input0, 0x5))));
            EXPECT_TRUE(all(rotl<0x6>(input0) == (rotl(input0, 0x6))));
            EXPECT_TRUE(all(rotl<0x7>(input0) == (rotl(input0, 0x7))));
            EXPECT_TRUE(all(rotl<0x8>(input0) == (rotl(input0, 0x8))));
            EXPECT_TRUE(all(rotl<0x9>(input0) == (rotl(input0, 0x9))));
            EXPECT_TRUE(all(rotl<0xa>(input0) == (rotl(input0, 0xa))));
            EXPECT_TRUE(all(rotl<0xb>(input0) == (rotl(input0, 0xb))));
            EXPECT_TRUE(all(rotl<0xc>(input0) == (rotl(input0, 0xc))));
            EXPECT_TRUE(all(rotl<0xd>(input0) == (rotl(input0, 0xd))));
            EXPECT_TRUE(all(rotl<0xe>(input0) == (rotl(input0, 0xe))));
            EXPECT_TRUE(all(rotl<0xf>(input0) == (rotl(input0, 0xf))));
        }
    }

    TEST(Vec8x16u, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            long long input1 = random64u();

            vec8x16u input0{input_array0};

            auto results = rotl(input0, input1);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotl(input_array0[j], input1);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            arr8x16u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = rotl(input0, input1);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotl(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Rotr) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            EXPECT_TRUE(all(rotr<0x0>(input0) == (rotr(input0, 0x0))));
            EXPECT_TRUE(all(rotr<0x1>(input0) == (rotr(input0, 0x1))));
            EXPECT_TRUE(all(rotr<0x2>(input0) == (rotr(input0, 0x2))));
            EXPECT_TRUE(all(rotr<0x3>(input0) == (rotr(input0, 0x3))));
            EXPECT_TRUE(all(rotr<0x4>(input0) == (rotr(input0, 0x4))));
            EXPECT_TRUE(all(rotr<0x5>(input0) == (rotr(input0, 0x5))));
            EXPECT_TRUE(all(rotr<0x6>(input0) == (rotr(input0, 0x6))));
            EXPECT_TRUE(all(rotr<0x7>(input0) == (rotr(input0, 0x7))));
            EXPECT_TRUE(all(rotr<0x8>(input0) == (rotr(input0, 0x8))));
            EXPECT_TRUE(all(rotr<0x9>(input0) == (rotr(input0, 0x9))));
            EXPECT_TRUE(all(rotr<0xa>(input0) == (rotr(input0, 0xa))));
            EXPECT_TRUE(all(rotr<0xb>(input0) == (rotr(input0, 0xb))));
            EXPECT_TRUE(all(rotr<0xc>(input0) == (rotr(input0, 0xc))));
            EXPECT_TRUE(all(rotr<0xd>(input0) == (rotr(input0, 0xd))));
            EXPECT_TRUE(all(rotr<0xe>(input0) == (rotr(input0, 0xe))));
            EXPECT_TRUE(all(rotr<0xf>(input0) == (rotr(input0, 0xf))));
        }
    }

    TEST(Vec8x16u, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            long long input1 = random64u();

            vec8x16u input0{input_array0};

            auto results = rotr(input0, input1);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotr(input_array0[j], input1);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    TEST(Vec8x16u, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            arr8x16u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random16u();
            }

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            auto results = rotr(input0, input1);

            arr8x16u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotr(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec8x16u{results_array}));
        }
    }

    //=====================================================
    // Conversions
    //=====================================================

    TEST(Vec8x16u, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto results = to_array(input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(results[j], input_array0[j]);
            }
        }
    }

    //TODO: Add tests for conversions from floating-point types

}

#endif
