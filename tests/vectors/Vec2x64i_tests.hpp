#ifndef AVEL_VEC2X64I_TESTS_HPP
#define AVEL_VEC2X64I_TESTS_HPP


namespace avel_tests {
    
    using namespace avel;
    
    //=========================================================================
    // mas4x32i tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask2x64i, Construct_from_primitive) {
        mask2x64i mask{mask2x64i::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask2x64i, Construct_from_bool) {
        mask2x64i mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask2x64i mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 2);
    }

    TEST(Mask2x64i, Construct_from_array) {
        mask2x64i mask0{{false, false}};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask2x64i mask1{{false, true}};

        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 1);

        mask2x64i mask2{{true, false}};

        EXPECT_TRUE(any(mask2));
        EXPECT_FALSE(all(mask2));
        EXPECT_FALSE(none(mask2));
        EXPECT_EQ(count(mask2), 1);

        mask2x64i mask3{{true, true}};

        EXPECT_TRUE(any(mask3));
        EXPECT_TRUE(all(mask3));
        EXPECT_FALSE(none(mask3));
        EXPECT_EQ(count(mask3), 2);
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask2x64i, Assign_bool) {
        mask2x64i mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask2x64i mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_EQ(count(mask1), 2);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask2x64i, Equality_comparison) {
        mask2x64i mask0{{false, false}};
        mask2x64i mask1{{false, true}};
        mask2x64i mask2{{true, false}};
        mask2x64i mask3{{true, true}};

        EXPECT_TRUE(mask0 == mask0);
        EXPECT_TRUE(mask1 == mask1);
        EXPECT_TRUE(mask2 == mask2);
        EXPECT_TRUE(mask3 == mask3);

        EXPECT_FALSE(mask0 == mask1);
        EXPECT_FALSE(mask0 == mask2);
        EXPECT_FALSE(mask0 == mask3);

        EXPECT_FALSE(mask1 == mask0);
        EXPECT_FALSE(mask1 == mask2);
        EXPECT_FALSE(mask1 == mask3);

        EXPECT_FALSE(mask2 == mask0);
        EXPECT_FALSE(mask2 == mask1);
        EXPECT_FALSE(mask2 == mask3);

        EXPECT_FALSE(mask3 == mask0);
        EXPECT_FALSE(mask3 == mask1);
        EXPECT_FALSE(mask3 == mask2);
    }

    TEST(Mask2x64i, Inequality_comparison) {
        mask2x64i mask0{{false, false}};
        mask2x64i mask1{{false, true}};
        mask2x64i mask2{{true, false}};
        mask2x64i mask3{{true, true}};

        EXPECT_FALSE(mask0 != mask0);
        EXPECT_FALSE(mask1 != mask1);
        EXPECT_FALSE(mask2 != mask2);
        EXPECT_FALSE(mask3 != mask3);

        EXPECT_TRUE(mask0 != mask1);
        EXPECT_TRUE(mask0 != mask2);
        EXPECT_TRUE(mask0 != mask3);

        EXPECT_TRUE(mask1 != mask0);
        EXPECT_TRUE(mask1 != mask2);
        EXPECT_TRUE(mask1 != mask3);

        EXPECT_TRUE(mask2 != mask0);
        EXPECT_TRUE(mask2 != mask1);
        EXPECT_TRUE(mask2 != mask3);

        EXPECT_TRUE(mask3 != mask0);
        EXPECT_TRUE(mask3 != mask1);
        EXPECT_TRUE(mask3 != mask2);
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask2x64i, Bitwise_and_assignment) {
        mask2x64i mask0{{false, false}};
        mask0 &= mask0;
        EXPECT_TRUE(none(mask0));

        mask2x64i mask1{{false, false}};
        mask1 &= mask2x64i{{true, true}};
        EXPECT_TRUE(none(mask1));

        mask2x64i mask2{{true, true}};
        mask2 &= mask2x64i{{false, false}};
        EXPECT_TRUE(none(mask2));

        mask2x64i mask3{{true, true}};
        mask3 &= mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64i, Bitwise_or_assignment) {
        mask2x64i mask0{{false, false}};
        mask0 |= mask0;
        EXPECT_TRUE(none(mask0));

        mask2x64i mask1{{false, false}};
        mask1 |= mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask1));

        mask2x64i mask2{{true, true}};
        mask2 |= mask2x64i{{false, false}};
        EXPECT_TRUE(all(mask2));

        mask2x64i mask3{{true, true}};
        mask3 |= mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64i, Bitwise_xor_assignment) {
        mask2x64i mask0{{false, false}};
        mask0 ^= mask0;
        EXPECT_TRUE(none(mask0));

        mask2x64i mask1{{false, false}};
        mask1 ^= mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask1));

        mask2x64i mask2{{true, true}};
        mask2 ^= mask2x64i{{false, false}};
        EXPECT_TRUE(all(mask2));

        mask2x64i mask3{{true, true}};
        mask3 ^= mask2x64i{{true, true}};
        EXPECT_TRUE(none(mask3));
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask2x64i, Logical_negation) {
        mask2x64i mask0{{false, false}};

        mask2x64i mask1{{true,  true}};

        EXPECT_TRUE(all(!mask0));
        EXPECT_TRUE(none(!mask1));
    }

    TEST(Mask2x64i, Bitwise_and) {
        auto mask0 = mask2x64i{{false, false}} & mask2x64i{{false, false}};
        EXPECT_TRUE(none(mask0));

        auto mask1 = mask2x64i{{false, false}} & mask2x64i{{true, true}};
        EXPECT_TRUE(none(mask1));

        auto mask2 = mask2x64i{{true, true}} & mask2x64i{{false, false}};
        EXPECT_TRUE(none(mask2));

        auto mask3 = mask2x64i{{true, true}} & mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64i, Logical_and) {
        auto mask0 = mask2x64i{{false, false}} && mask2x64i{{false, false}};
        EXPECT_TRUE(none(mask0));

        auto mask1 = mask2x64i{{false, false}} && mask2x64i{{true, true}};
        EXPECT_TRUE(none(mask1));

        auto mask2 = mask2x64i{{true, true}} && mask2x64i{{false, false}};
        EXPECT_TRUE(none(mask2));

        auto mask3 = mask2x64i{{true, true}} && mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64i, Bitwise_or) {
        auto mask0 = mask2x64i{{false, false}} | mask2x64i{{false, false}};
        EXPECT_TRUE(none(mask0));

        auto mask1 = mask2x64i{{false, false}} | mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask1));

        auto mask2 = mask2x64i{{true, true}} | mask2x64i{{false, false}};
        EXPECT_TRUE(all(mask2));

        auto mask3 = mask2x64i{{true, true}} | mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64i, Logical_or) {
        auto mask0 = mask2x64i{{false, false}} || mask2x64i{{false, false}};
        EXPECT_TRUE(none(mask0));

        auto mask1 = mask2x64i{{false, false}} || mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask1));

        auto mask2 = mask2x64i{{true, true}} || mask2x64i{{false, false}};
        EXPECT_TRUE(all(mask2));

        auto mask3 = mask2x64i{{true, true}} || mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64i, Bitwise_xor) {
        auto mask0 = mask2x64i{{false, false}} ^ mask2x64i{{false, false}};
        EXPECT_TRUE(none(mask0));

        auto mask1 = mask2x64i{{false, false}} ^ mask2x64i{{true, true}};
        EXPECT_TRUE(all(mask1));

        auto mask2 = mask2x64i{{true, true}} ^ mask2x64i{{false, false}};
        EXPECT_TRUE(all(mask2));

        auto mask3 = mask2x64i{{true, true}} ^ mask2x64i{{true, true}};
        EXPECT_TRUE(none(mask3));
    }

    //=========================================================================
    // Vec2x64i tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec2x64i, Convert_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2xb data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random64u() & 0x1;
            }

            mask2x64i m0{data};
            vec2x64i v0{m0};

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

    TEST(Vec2x64i, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t x = random64u();
            vec2x64i v{x};
            vec2x64i baseline{{x, x}};

            EXPECT_TRUE(all(v == baseline));
        }
    }

    TEST(Vec2x64i, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i data;

            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random64u();
            }

            vec2x64i v{data};

            auto results = to_array(v);

            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(data[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec2x64i, Assign_primitive) {
        vec2x64i::primitive x{};
        vec2x64i v{0x0F};
        v = x;
        vec2x64i baseline{};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(Vec2x64i, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t scalar = random64u();
            vec2x64i v;
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

    TEST(Vec2x64i, Equality_comparison) {
        mask2x64i mask0{false};
        mask2x64i mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask2x64i mask2{true};
        mask2x64i mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec2x64i, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random64u();
            }

            vec2x64i v0{data};
            vec2x64i v1{data};

            EXPECT_TRUE(all(v0 == v1));
        }
    }

    TEST(Vec2x64i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i data0;
            arr2x64i data1;
            for (std::size_t j = 0; j < data0.size(); ++j) {
                data0[j] = random64u();
                data1[j] = random64u() ;
            }

            vec2x64i v0{data0};
            vec2x64i v1{data1};

            if (!all(v0 == v1)) {
                EXPECT_TRUE(any(v0 != v1));
            }
        }
    }

    TEST(Vec2x64i, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = (input0 < input1);

            arr2xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] < input_array1[j];
            }

            EXPECT_EQ(results, mask2x64i{results_array});
        }
    }

    TEST(Vec2x64i, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = (input0 <= input1);

            arr2xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] <= input_array1[j];
            }

            EXPECT_EQ(results, mask2x64i{results_array});
        }
    }

    TEST(Vec2x64i, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = (input0 > input1);

            arr2xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] > input_array1[j];
            }

            EXPECT_EQ(results, mask2x64i{results_array});
        }
    }

    TEST(Vec2x64i, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = (input0 >= input1);

            arr2xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] >= input_array1[j];
            }

            EXPECT_EQ(results, mask2x64i{results_array});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec2x64i, Unary_plus) {
        const vec2x64i u{{0x0, 0x1}};
        vec2x64i v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec2x64i, Unary_minus) {
        const vec2x64i u{{0x0, 0x1}};
        vec2x64i v{{-0x0, -0x1}};
        vec2x64i w{-u};

        EXPECT_TRUE(all(v == w));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec2x64i, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};


            auto results = input0;
            results += input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};


            auto results = input0;
            results -= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};


            auto results = input0;
            results *= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0;
            results /= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = std::max(static_cast<std::int32_t>(random64u()), static_cast<std::int32_t>(1u));
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0;
            results %= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec2x64i, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0 + input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0 - input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0 * input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0 / input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = std::max(static_cast<std::int64_t>(random64u()), static_cast<std::int64_t>(1u));
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0 % input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec2x64i, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};
            ++input0;

            auto results = input0;
            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};
            input0++;

            auto results = input0;
            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};
            --input0;

            auto results = input0;
            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = +--input_array0[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};
            input0--;

            auto results = input0;
            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec2x64i, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0;
            results &= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0;
            results |= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0;
            results ^= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            std::int8_t input1 = random64u() % 65;

            vec2x64i input0{input_array0};

            auto results = input0;
            results <<= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u() % 65;
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0;
            results <<= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            std::int8_t input1 = random64u() % 65;

            vec2x64i input0{input_array0};

            auto results = input0;
            results >>= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 64) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFFFFFFFFFFFFFF;
                    } else {
                        results_array[j] = 0x0000000000000000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u() % 65;
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0;
            results >>= input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 64) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFFFFFFFFFFFFFF;
                    } else {
                        results_array[j] = 0x0000000000000000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec2x64i, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto results = ~input0;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ~input_array0[j];
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            std::int8_t input1 = random64u() % 65;

            vec2x64i input0{input_array0};

            auto results = input0 << input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u() % 65;
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0 << input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            std::int8_t input1 = random64u() % 65;

            vec2x64i input0{input_array0};

            auto results = input0 >> input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 64) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFFFFFFFFFFFFFF;
                    } else {
                        results_array[j] = 0x0000000000000000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u() % 65;
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = input0 >> input1;

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 64) {
                    if (input_array0[j] < 0) {
                        results_array[j] = 0xFFFFFFFFFFFFFFFF;
                    } else {
                        results_array[j] = 0x0000000000000000;
                    }
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec2x64i, Conversion_to_mask) {
        vec2x64i vec{0};

        auto a = vec.operator mask2x64i();
        mask2x64i b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec2x64i, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto results = mask2x64i{input0};

            arr2xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(results == mask2x64i{results_array});
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec2x64i, Broadcast_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u() & 0x1;
            }

            mask2x64i input0{input_array0};

            auto results = broadcast_mask(input0);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array0[j]) {
                    results_array[j] = 0xFFFFFFFFFFFFFFFF;
                } else {
                    results_array[j] = 0x0000000000000000;
                }
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2xb input_array0{};
            arr2x64i input_array1{};
            arr2x64i input_array2{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u() & 0x1;
                input_array1[j] = random64u();
                input_array2[j] = random64u();
            }

            mask2x64i input0{input_array0};
            vec2x64i input1{input_array1};
            vec2x64i input2{input_array2};

            auto results = blend(input0, input1, input2);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = blend(input_array0[j], input_array1[j], input_array2[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = max(input0, input1);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = min(input0, input1);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = min(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = minmax(input0, input1);

            arr2x64i results_array0{};
            arr2x64i results_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array0[j], input_array1[j]);
                results_array0[j] = xy[0];
                results_array1[j] = xy[1];
            }

            EXPECT_TRUE(all(results[0] == vec2x64i{results_array0}));
            EXPECT_TRUE(all(results[1] == vec2x64i{results_array1}));
        }
    }

    TEST(Vec2x64i, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};
            arr2x64i input_array2{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
                input_array2[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};
            vec2x64i input2{input_array2};

            auto xy = minmax(input1, input2);

            auto results = clamp(input0, xy[0], xy[1]);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array1[j], input_array2[j]);
                results_array[j] = clamp(input_array0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = average(input0, input1);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = average(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = midpoint(input0, input1);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = midpoint(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Negate_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2xb input_array0{};
            arr2x64i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u() & 0x1;
                input_array1[j] = random16u();
            }

            mask2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            auto results = negate(input0, input1);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = negate(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto results = abs(input0);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = avel::abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto results = neg_abs(input0);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = neg_abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            auto results = load<vec2x64i>(input_array0.data());

            EXPECT_TRUE(all(results == vec2x64i{input_array0}));
        }
    }

    TEST(Vec2x64i, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec2x64i)) arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            auto results = aligned_load<vec2x64i>(input_array0.data());

            EXPECT_TRUE(all(results == vec2x64i{input_array0}));
        }
    }

    TEST(Vec2x64i, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            std::int64_t arr[2]{};
            store(arr, input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(input_array0[j], arr[j]);
            }
        }
    }

    TEST(Vec2x64i, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            alignas(alignof(vec2x64i)) std::int64_t arr[2]{};
            aligned_store(arr, input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(arr[j], input_array0[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec2x64i, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            arr2x64i numerators{};
            arr2x64i denominators{};
            for (std::size_t j = 0; j < numerators.size(); ++j) {
                numerators[j] = random64u();
                denominators[j] = random64u();
                if (denominators[j] == 0) {
                    denominators[j] = 1;
                }
            }

            arr2x64i quotients{};
            arr2x64i remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec2x64i n{numerators};
            vec2x64i d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);
        }
    }

    TEST(Vec2x64i, Popcount) {
        vec2x64i v{0x0};
        vec2x64i c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec2x64i, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto results = popcount(input0);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = popcount(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto results = byteswap(input0);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = byteswap(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto results = countl_zero(input0);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto results = countl_one(input0);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto results = countr_zero(input0);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    TEST(Vec2x64i, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto results = countr_one(input0);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }
    
}

#endif //AVEL_VEC2X64I_TESTS_HPP
