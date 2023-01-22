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
        EXPECT_EQ(count(mask1), 2);
    }

    TEST(Mask2x64u, Construct_from_array) {
        mask2x64u mask0{{false, false}};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask2x64u mask1{{true, true}};
        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 2);

        mask2x64u mask2{{false, true}};
        EXPECT_TRUE(any(mask2));
        EXPECT_FALSE(all(mask2));
        EXPECT_FALSE(none(mask2));
        EXPECT_EQ(count(mask2), 1);

        mask2x64u mask3{{true, false}};
        EXPECT_TRUE(any(mask3));
        EXPECT_FALSE(all(mask3));
        EXPECT_FALSE(none(mask2));
        EXPECT_EQ(count(mask3), 1);
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
        EXPECT_EQ(count(mask1), 2);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask2x64u, Equality_comparison) {
        mask2x64u mask0{{false, false}};
        mask2x64u mask1{{false, true}};
        mask2x64u mask2{{true, false}};
        mask2x64u mask3{{true, true}};

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

    TEST(Mask2x64u, Inequality_comparison) {
        mask2x64u mask0{{false, false}};
        mask2x64u mask1{{false, true}};
        mask2x64u mask2{{true, false}};
        mask2x64u mask3{{true, true}};

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

    TEST(Mask2x64u, Bitwise_and_assignment) {
        mask2x64u mask0{{false, false}};
        mask0 &= mask0;
        EXPECT_TRUE(none(mask0));

        mask2x64u mask1{{false, false}};
        mask1 &= mask2x64u{{true, true}};
        EXPECT_TRUE(none(mask1));

        mask2x64u mask2{{true, true}};
        mask2 &= mask2x64u{{false, false}};
        EXPECT_TRUE(none(mask2));

        mask2x64u mask3{{true, true}};
        mask3 &= mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64u, Bitwise_or_assignment) {
        mask2x64u mask0{{false, false}};
        mask0 |= mask0;
        EXPECT_TRUE(none(mask0));

        mask2x64u mask1{{false, false}};
        mask1 |= mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask1));

        mask2x64u mask2{{true, true}};
        mask2 |= mask2x64u{{false, false}};
        EXPECT_TRUE(all(mask2));

        mask2x64u mask3{{true, true}};
        mask3 |= mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64u, Bitwise_xor_assignment) {
        mask2x64u mask0{{false, false}};
        mask0 ^= mask0;
        EXPECT_TRUE(none(mask0));

        mask2x64u mask1{{false, false}};
        mask1 ^= mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask1));

        mask2x64u mask2{{true, true}};
        mask2 ^= mask2x64u{{false, false}};
        EXPECT_TRUE(all(mask2));

        mask2x64u mask3{{true, true}};
        mask3 ^= mask2x64u{{true, true}};
        EXPECT_TRUE(none(mask3));
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask2x64u, Logical_negation) {
        mask2x64u mask0{{false, true}};
        mask2x64u mask1{{true, false}};

        EXPECT_EQ(mask0, !mask1);
    }

    TEST(Mask2x64u, Bitwise_and) {
        auto mask0 = mask2x64u{{false, false}} & mask2x64u{{false, false}};
        EXPECT_TRUE(none(mask0));

        auto mask1 = mask2x64u{{false, false}} & mask2x64u{{true, true}};
        EXPECT_TRUE(none(mask1));

        auto mask2 = mask2x64u{{true, true}} & mask2x64u{{false, false}};
        EXPECT_TRUE(none(mask2));

        auto mask3 = mask2x64u{{true, true}} & mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64u, Logical_and) {
        auto mask0 = mask2x64u{{false, false}} && mask2x64u{{false, false}};
        EXPECT_TRUE(none(mask0));

        auto mask1 = mask2x64u{{false, false}} && mask2x64u{{true, true}};
        EXPECT_TRUE(none(mask1));

        auto mask2 = mask2x64u{{true, true}} && mask2x64u{{false, false}};
        EXPECT_TRUE(none(mask2));

        auto mask3 = mask2x64u{{true, true}} && mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64u, Bitwise_or) {
        auto mask0 = mask2x64u{{false, false}} | mask2x64u{{false, false}};
        EXPECT_TRUE(none(mask0));

        auto mask1 = mask2x64u{{false, false}} | mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask1));

        auto mask2 = mask2x64u{{true, true}} | mask2x64u{{false, false}};
        EXPECT_TRUE(all(mask2));

        auto mask3 = mask2x64u{{true, true}} | mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64u, Logical_or) {
        auto mask0 = mask2x64u{{false, false}} || mask2x64u{{false, false}};
        EXPECT_TRUE(none(mask0));

        auto mask1 = mask2x64u{{false, false}} || mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask1));

        auto mask2 = mask2x64u{{true, true}} || mask2x64u{{false, false}};
        EXPECT_TRUE(all(mask2));

        auto mask3 = mask2x64u{{true, true}} || mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask3));
    }

    TEST(Mask2x64u, Bitwise_xor) {
        auto mask0 = mask2x64u{{false, false}} ^ mask2x64u{{false, false}};
        EXPECT_TRUE(none(mask0));

        auto mask1 = mask2x64u{{false, false}} ^ mask2x64u{{true, true}};
        EXPECT_TRUE(all(mask1));

        auto mask2 = mask2x64u{{true, true}} ^ mask2x64u{{false, false}};
        EXPECT_TRUE(all(mask2));

        auto mask3 = mask2x64u{{true, true}} ^ mask2x64u{{true, true}};
        EXPECT_TRUE(none(mask3));
    }

    //=========================================================================
    // vec2x64u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec2x64u, Construct_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2xb data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random64u() & 0x1;
            }

            mask2x64u m0{data};
            vec2x64u v0{m0};

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

    TEST(Vec2x64u, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random64u();
            vec2x64u v{x};
            vec2x64u baseline{{x, x}};

            EXPECT_TRUE(all(v == baseline));
        }
    }

    TEST(Vec2x64u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u data;

            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random64u();
            }

            vec2x64u v{data};

            auto results = to_array(v);

            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(data[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec2x64u, Assign_primitive) {
        vec2x64u::primitive x{};
        vec2x64u v{0x0FF};
        v = x;
        vec2x64u baseline{};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(Vec2x64u, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random64u();
            vec2x64u v{};
            v = x;
            vec2x64u baseline{{x, x}};

            EXPECT_TRUE(all(v == baseline));
        }
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Vec2x64u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random64u();
            }

            vec2x64u v0{data};
            vec2x64u v1{data};

            EXPECT_TRUE(all(v0 == v1));
        }
    }

    TEST(Vec2x64u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u data0;
            arr2x64u data1;
            for (std::size_t j = 0; j < data0.size(); ++j) {
                data0[j] = random64u();
                data1[j] = random64u() ;
            }

            vec2x64u v0{data0};
            vec2x64u v1{data1};

            if (!all(v0 == v1)) {
                EXPECT_TRUE(any(v0 != v1));
            }
        }
    }

    TEST(Vec2x64u, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = (input0 < input1);

            arr2xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] < input_array1[j];
            }

            EXPECT_EQ(results, mask2x64u{results_array});
        }
    }

    TEST(Vec2x64u, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = (input0 <= input1);

            arr2xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] <= input_array1[j];
            }

            EXPECT_EQ(results, mask2x64u{results_array});
        }
    }

    TEST(Vec2x64u, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = (input0 > input1);

            arr2xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] > input_array1[j];
            }

            EXPECT_EQ(results, mask2x64u{results_array});
        }
    }

    TEST(Vec2x64u, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = (input0 >= input1);

            arr2xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] >= input_array1[j];
            }

            EXPECT_EQ(results, mask2x64u{results_array});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec2x64u, Unary_plus) {
        const vec2x64u u{{0, 1}};
        vec2x64u v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec2x64u, Unary_minus) {
        const vec2x64u u{{+0, +1}};
        vec2x64i v{{-0, -1}};
        vec2x64i w{-u};

        EXPECT_TRUE(all(v == w));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec2x64u, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};


            auto results = input0;
            results += input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};


            auto results = input0;
            results -= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};


            auto results = input0;
            results *= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = std::max(static_cast<std::uint64_t>(random64u()), static_cast<std::uint64_t>(1u));
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};


            auto results = input0;
            results /= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = std::max(static_cast<std::uint64_t>(random64u()), static_cast<std::uint64_t>(1u));
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0;
            results %= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec2x64u, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0 + input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0 - input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0 * input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = std::max(static_cast<std::uint64_t>(random64u()), static_cast<std::uint64_t>(1u));
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0 / input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = std::max(static_cast<std::uint64_t>(random64u()), static_cast<std::uint64_t>(1u));
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0 % input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec2x64u, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};
            ++input0;

            auto results = input0;
            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};
            input0++;

            auto results = input0;
            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};
            --input0;

            auto results = input0;
            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};
            input0--;

            auto results = input0;
            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec2x64u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0;
            results &= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0;
            results |= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            arr2x64u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};


            auto results = input0;
            results ^= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            std::uint64_t input1 = random64u() % 65;

            vec2x64u input0{input_array0};

            auto results = input0;
            results <<= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u() % 65;
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0;
            results <<= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            std::uint64_t input1 = random64u() % 65;

            vec2x64u input0{input_array0};

            auto results = input0;
            results >>= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u() % 65;
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0;
            results >>= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec2x64u, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = ~input0;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ~input_array0[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Bitwise_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0 & input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Bitwise_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0 | input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Bitwise_xor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0 ^ input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Left_shift_by_scalar) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            std::uint64_t input1 = random64u() % 65;

            vec2x64u input0{input_array0};

            auto results = input0;
            results <<= input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Left_shift_by_vector) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u() % 65;
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0 << input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Right_shift_by_scalar) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            std::uint64_t input1 = random64u() % 65;

            vec2x64u input0{input_array0};

            auto results = input0 >> input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Right_shift_by_vector) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u() % 65;
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = input0 >> input1;

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 64) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
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
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = mask2x64u{input0};

            arr2xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(results == mask2x64u{results_array});
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec2x64u, Broadcast_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u() & 0x1;
            }

            mask2x64u input0{input_array0};

            auto results = broadcast_mask(input0);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array0[j]) {
                    results_array[j] = -1;
                } else {
                    results_array[j] = 0;
                }
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }
    
    TEST(Vec2x64u, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2xb input_array0{};
            arr2x64u input_array1{};
            arr2x64u input_array2{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u() & 0x1;
                input_array1[j] = random64u();
                input_array2[j] = random64u();
            }

            mask2x64u input0{input_array0};
            vec2x64u input1{input_array1};
            vec2x64u input2{input_array2};

            auto results = blend(input0, input1, input2);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = blend(input_array0[j], input_array1[j], input_array2[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = max(input0, input1);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};


            arr2x64u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random64u();
            }

            vec2x64u input1{input_array1};


            auto results = min(input0, input1);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::min(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = minmax(input0, input1);

            arr2x64u results_array0{};
            arr2x64u results_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array0[j], input_array1[j]);
                results_array0[j] = xy[0];
                results_array1[j] = xy[1];
            }

            EXPECT_TRUE(all(results[0] == vec2x64u{results_array0}));
            EXPECT_TRUE(all(results[1] == vec2x64u{results_array1}));
        }
    }

    TEST(Vec2x64u, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};
            arr2x64u input_array2{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
                input_array2[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};
            vec2x64u input2{input_array2};

            auto xy = minmax(input1, input2);

            auto results = clamp(input0, xy[0], xy[1]);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array1[j], input_array2[j]);
                results_array[j] = clamp(input_array0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = midpoint(input0, input1);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = midpoint(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            auto results = load<vec2x64u>(input_array0.data());

            EXPECT_TRUE(all(results == vec2x64u{input_array0}));
        }
    }

    TEST(Vec2x64u, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec2x64u)) arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            auto results = aligned_load<vec2x64u>(input_array0.data());

            EXPECT_TRUE(all(results == vec2x64u{input_array0}));
        }
    }

    TEST(Vec2x64u, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            std::uint64_t arr[4]{};
            store(arr, input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(input_array0[j], arr[j]);
            }
        }
    }

    TEST(Vec2x64u, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            alignas(alignof(vec2x64u)) std::uint64_t arr[4]{};
            aligned_store(arr, input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(arr[j], input_array0[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec2x64u, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            arr2x64u numerators{};
            for (std::size_t j = 0; j < numerators.size(); ++j) {
                numerators[j] = random64u();
            }

            arr2x64u denominators{};
            for (std::size_t j = 0; j < denominators.size(); ++j) {
                denominators[j] = std::max(static_cast<std::uint64_t>(random64u()), static_cast<std::uint64_t>(1u));
            }

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

    TEST(Vec2x64u, Popcount) {
        vec2x64u v{0x00000000};
        vec2x64u c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec2x64u, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = popcount(input0);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = popcount(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = byteswap(input0);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = byteswap(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Countl_zero) {
        std::array<int, 4> rounding_modes{
            FE_DOWNWARD,
            FE_TONEAREST,
            FE_TOWARDZERO,
            FE_UPWARD
        };

        for (std::size_t i = 0; i < 4; ++i) {
            std::fesetround(rounding_modes[i]);

            vec2x64u v0{{
                0x0000000000000000,
                0xFF03FF05FFFFFFFF
            }};

            vec2x64u u0 = countl_zero(v0);
            vec2x64u b0{{64, 0}};

            EXPECT_TRUE(all(u0 == b0));


            vec2x64u v1{{
                0x07FFFFFFFFFFFFFF,
                0x03FFFFFFFFFFFFFF
            }};

            vec2x64u u1 = countl_zero(v1);
            vec2x64u b1{{5, 6}};

            EXPECT_TRUE(all(u1 == b1));
        }
    }

    TEST(Vec2x64u, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = countl_zero(input0);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Countl_one) {
        vec2x64u v{{
            0x0000000000000000,
            0xFF03FF0500000000
        }};

        vec2x64u u = countl_one(v);
        vec2x64u b{{0, 8}};

        EXPECT_TRUE(all(u == b));
    }

    TEST(Vec2x64u, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = countl_one(input0);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Countr_zero) {
        vec2x64u v{{
            0x0000000000000000,
            0x0000000100000000,
        }};

        vec2x64u b{{
            64,
            32
        }};

        vec2x64u o = countr_zero(v);

        EXPECT_TRUE(all(o == b));
    }

    TEST(Vec2x64u, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = countr_zero(input0);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Countr_one) {
        vec2x64u v{{
            0x0000000000000000,
            0x0000001000000000
        }};

        vec2x64u b{{
            0,
            0
        }};

        vec2x64u o = countr_one(v);

        EXPECT_TRUE(all(o == b));
    }

    TEST(Vec2x64u, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = countr_one(input0);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Bit_width_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = bit_width(input0);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_width(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Bit_floor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = bit_floor(input0);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_floor(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }
    TEST(Vec2x64u, Bit_ceil_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = bit_ceil(input0);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_ceil(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Has_single_bit) {
        for (std::size_t i = 0; i < 64; ++i) {
            EXPECT_TRUE(all(has_single_bit(vec2x64u{static_cast<std::uint64_t>(1ull << i)})));
        }
    }

    TEST(Vec2x64u, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = has_single_bit(input0);

            arr2xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = has_single_bit(input_array0[j]);
            }

            EXPECT_TRUE(results == mask2x64u{results_array});
        }
    }

    //=====================================================
    // Bit Manipulation Instructions
    //=====================================================

    TEST(Vec2x64u, Bit_shift_left) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            EXPECT_TRUE(all(bit_shift_left<0>(input0) == (input0 << 0)));
            EXPECT_TRUE(all(bit_shift_left<1>(input0) == (input0 << 1)));
            EXPECT_TRUE(all(bit_shift_left<2>(input0) == (input0 << 2)));
            EXPECT_TRUE(all(bit_shift_left<3>(input0) == (input0 << 3)));
            EXPECT_TRUE(all(bit_shift_left<4>(input0) == (input0 << 4)));
            EXPECT_TRUE(all(bit_shift_left<5>(input0) == (input0 << 5)));
            EXPECT_TRUE(all(bit_shift_left<6>(input0) == (input0 << 6)));
            EXPECT_TRUE(all(bit_shift_left<7>(input0) == (input0 << 7)));
            EXPECT_TRUE(all(bit_shift_left<8>(input0) == (input0 << 8)));
            EXPECT_TRUE(all(bit_shift_left<9>(input0) == (input0 << 9)));
            EXPECT_TRUE(all(bit_shift_left<10>(input0) == (input0 << 10)));
            EXPECT_TRUE(all(bit_shift_left<11>(input0) == (input0 << 11)));
            EXPECT_TRUE(all(bit_shift_left<12>(input0) == (input0 << 12)));
            EXPECT_TRUE(all(bit_shift_left<13>(input0) == (input0 << 13)));
            EXPECT_TRUE(all(bit_shift_left<14>(input0) == (input0 << 14)));
            EXPECT_TRUE(all(bit_shift_left<15>(input0) == (input0 << 15)));
            EXPECT_TRUE(all(bit_shift_left<16>(input0) == (input0 << 16)));
            EXPECT_TRUE(all(bit_shift_left<17>(input0) == (input0 << 17)));
            EXPECT_TRUE(all(bit_shift_left<18>(input0) == (input0 << 18)));
            EXPECT_TRUE(all(bit_shift_left<19>(input0) == (input0 << 19)));
            EXPECT_TRUE(all(bit_shift_left<20>(input0) == (input0 << 20)));
            EXPECT_TRUE(all(bit_shift_left<21>(input0) == (input0 << 21)));
            EXPECT_TRUE(all(bit_shift_left<22>(input0) == (input0 << 22)));
            EXPECT_TRUE(all(bit_shift_left<23>(input0) == (input0 << 23)));
            EXPECT_TRUE(all(bit_shift_left<24>(input0) == (input0 << 24)));
            EXPECT_TRUE(all(bit_shift_left<25>(input0) == (input0 << 25)));
            EXPECT_TRUE(all(bit_shift_left<26>(input0) == (input0 << 26)));
            EXPECT_TRUE(all(bit_shift_left<27>(input0) == (input0 << 27)));
            EXPECT_TRUE(all(bit_shift_left<28>(input0) == (input0 << 28)));
            EXPECT_TRUE(all(bit_shift_left<29>(input0) == (input0 << 29)));
            EXPECT_TRUE(all(bit_shift_left<30>(input0) == (input0 << 30)));
            EXPECT_TRUE(all(bit_shift_left<31>(input0) == (input0 << 31)));
            EXPECT_TRUE(all(bit_shift_left<32>(input0) == (input0 << 32)));
            EXPECT_TRUE(all(bit_shift_left<33>(input0) == (input0 << 33)));
            EXPECT_TRUE(all(bit_shift_left<34>(input0) == (input0 << 34)));
            EXPECT_TRUE(all(bit_shift_left<35>(input0) == (input0 << 35)));
            EXPECT_TRUE(all(bit_shift_left<36>(input0) == (input0 << 36)));
            EXPECT_TRUE(all(bit_shift_left<37>(input0) == (input0 << 37)));
            EXPECT_TRUE(all(bit_shift_left<38>(input0) == (input0 << 38)));
            EXPECT_TRUE(all(bit_shift_left<39>(input0) == (input0 << 39)));
            EXPECT_TRUE(all(bit_shift_left<40>(input0) == (input0 << 40)));
            EXPECT_TRUE(all(bit_shift_left<41>(input0) == (input0 << 41)));
            EXPECT_TRUE(all(bit_shift_left<42>(input0) == (input0 << 42)));
            EXPECT_TRUE(all(bit_shift_left<43>(input0) == (input0 << 43)));
            EXPECT_TRUE(all(bit_shift_left<44>(input0) == (input0 << 44)));
            EXPECT_TRUE(all(bit_shift_left<45>(input0) == (input0 << 45)));
            EXPECT_TRUE(all(bit_shift_left<46>(input0) == (input0 << 46)));
            EXPECT_TRUE(all(bit_shift_left<47>(input0) == (input0 << 47)));
            EXPECT_TRUE(all(bit_shift_left<48>(input0) == (input0 << 48)));
            EXPECT_TRUE(all(bit_shift_left<49>(input0) == (input0 << 49)));
            EXPECT_TRUE(all(bit_shift_left<50>(input0) == (input0 << 50)));
            EXPECT_TRUE(all(bit_shift_left<51>(input0) == (input0 << 51)));
            EXPECT_TRUE(all(bit_shift_left<52>(input0) == (input0 << 52)));
            EXPECT_TRUE(all(bit_shift_left<53>(input0) == (input0 << 53)));
            EXPECT_TRUE(all(bit_shift_left<54>(input0) == (input0 << 54)));
            EXPECT_TRUE(all(bit_shift_left<55>(input0) == (input0 << 55)));
            EXPECT_TRUE(all(bit_shift_left<56>(input0) == (input0 << 56)));
            EXPECT_TRUE(all(bit_shift_left<57>(input0) == (input0 << 57)));
            EXPECT_TRUE(all(bit_shift_left<58>(input0) == (input0 << 58)));
            EXPECT_TRUE(all(bit_shift_left<59>(input0) == (input0 << 59)));
            EXPECT_TRUE(all(bit_shift_left<60>(input0) == (input0 << 60)));
            EXPECT_TRUE(all(bit_shift_left<61>(input0) == (input0 << 61)));
            EXPECT_TRUE(all(bit_shift_left<62>(input0) == (input0 << 62)));
            EXPECT_TRUE(all(bit_shift_left<63>(input0) == (input0 << 63)));
            EXPECT_TRUE(all(bit_shift_left<64>(input0) == (input0 << 64)));
        }
    }

    TEST(Vec2x64u, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            EXPECT_TRUE(all(bit_shift_right<0>(input0) == (input0 >> 0)));
            EXPECT_TRUE(all(bit_shift_right<1>(input0) == (input0 >> 1)));
            EXPECT_TRUE(all(bit_shift_right<2>(input0) == (input0 >> 2)));
            EXPECT_TRUE(all(bit_shift_right<3>(input0) == (input0 >> 3)));
            EXPECT_TRUE(all(bit_shift_right<4>(input0) == (input0 >> 4)));
            EXPECT_TRUE(all(bit_shift_right<5>(input0) == (input0 >> 5)));
            EXPECT_TRUE(all(bit_shift_right<6>(input0) == (input0 >> 6)));
            EXPECT_TRUE(all(bit_shift_right<7>(input0) == (input0 >> 7)));
            EXPECT_TRUE(all(bit_shift_right<8>(input0) == (input0 >> 8)));
            EXPECT_TRUE(all(bit_shift_right<9>(input0) == (input0 >> 9)));
            EXPECT_TRUE(all(bit_shift_right<10>(input0) == (input0 >> 10)));
            EXPECT_TRUE(all(bit_shift_right<11>(input0) == (input0 >> 11)));
            EXPECT_TRUE(all(bit_shift_right<12>(input0) == (input0 >> 12)));
            EXPECT_TRUE(all(bit_shift_right<13>(input0) == (input0 >> 13)));
            EXPECT_TRUE(all(bit_shift_right<14>(input0) == (input0 >> 14)));
            EXPECT_TRUE(all(bit_shift_right<15>(input0) == (input0 >> 15)));
            EXPECT_TRUE(all(bit_shift_right<16>(input0) == (input0 >> 16)));
            EXPECT_TRUE(all(bit_shift_right<17>(input0) == (input0 >> 17)));
            EXPECT_TRUE(all(bit_shift_right<18>(input0) == (input0 >> 18)));
            EXPECT_TRUE(all(bit_shift_right<19>(input0) == (input0 >> 19)));
            EXPECT_TRUE(all(bit_shift_right<20>(input0) == (input0 >> 20)));
            EXPECT_TRUE(all(bit_shift_right<21>(input0) == (input0 >> 21)));
            EXPECT_TRUE(all(bit_shift_right<22>(input0) == (input0 >> 22)));
            EXPECT_TRUE(all(bit_shift_right<23>(input0) == (input0 >> 23)));
            EXPECT_TRUE(all(bit_shift_right<24>(input0) == (input0 >> 24)));
            EXPECT_TRUE(all(bit_shift_right<25>(input0) == (input0 >> 25)));
            EXPECT_TRUE(all(bit_shift_right<26>(input0) == (input0 >> 26)));
            EXPECT_TRUE(all(bit_shift_right<27>(input0) == (input0 >> 27)));
            EXPECT_TRUE(all(bit_shift_right<28>(input0) == (input0 >> 28)));
            EXPECT_TRUE(all(bit_shift_right<29>(input0) == (input0 >> 29)));
            EXPECT_TRUE(all(bit_shift_right<30>(input0) == (input0 >> 30)));
            EXPECT_TRUE(all(bit_shift_right<31>(input0) == (input0 >> 31)));
            EXPECT_TRUE(all(bit_shift_right<32>(input0) == (input0 >> 32)));
            EXPECT_TRUE(all(bit_shift_right<33>(input0) == (input0 >> 33)));
            EXPECT_TRUE(all(bit_shift_right<34>(input0) == (input0 >> 34)));
            EXPECT_TRUE(all(bit_shift_right<35>(input0) == (input0 >> 35)));
            EXPECT_TRUE(all(bit_shift_right<36>(input0) == (input0 >> 36)));
            EXPECT_TRUE(all(bit_shift_right<37>(input0) == (input0 >> 37)));
            EXPECT_TRUE(all(bit_shift_right<38>(input0) == (input0 >> 38)));
            EXPECT_TRUE(all(bit_shift_right<39>(input0) == (input0 >> 39)));
            EXPECT_TRUE(all(bit_shift_right<40>(input0) == (input0 >> 40)));
            EXPECT_TRUE(all(bit_shift_right<41>(input0) == (input0 >> 41)));
            EXPECT_TRUE(all(bit_shift_right<42>(input0) == (input0 >> 42)));
            EXPECT_TRUE(all(bit_shift_right<43>(input0) == (input0 >> 43)));
            EXPECT_TRUE(all(bit_shift_right<44>(input0) == (input0 >> 44)));
            EXPECT_TRUE(all(bit_shift_right<45>(input0) == (input0 >> 45)));
            EXPECT_TRUE(all(bit_shift_right<46>(input0) == (input0 >> 46)));
            EXPECT_TRUE(all(bit_shift_right<47>(input0) == (input0 >> 47)));
            EXPECT_TRUE(all(bit_shift_right<48>(input0) == (input0 >> 48)));
            EXPECT_TRUE(all(bit_shift_right<49>(input0) == (input0 >> 49)));
            EXPECT_TRUE(all(bit_shift_right<50>(input0) == (input0 >> 50)));
            EXPECT_TRUE(all(bit_shift_right<51>(input0) == (input0 >> 51)));
            EXPECT_TRUE(all(bit_shift_right<52>(input0) == (input0 >> 52)));
            EXPECT_TRUE(all(bit_shift_right<53>(input0) == (input0 >> 53)));
            EXPECT_TRUE(all(bit_shift_right<54>(input0) == (input0 >> 54)));
            EXPECT_TRUE(all(bit_shift_right<55>(input0) == (input0 >> 55)));
            EXPECT_TRUE(all(bit_shift_right<56>(input0) == (input0 >> 56)));
            EXPECT_TRUE(all(bit_shift_right<57>(input0) == (input0 >> 57)));
            EXPECT_TRUE(all(bit_shift_right<58>(input0) == (input0 >> 58)));
            EXPECT_TRUE(all(bit_shift_right<59>(input0) == (input0 >> 59)));
            EXPECT_TRUE(all(bit_shift_right<60>(input0) == (input0 >> 60)));
            EXPECT_TRUE(all(bit_shift_right<61>(input0) == (input0 >> 61)));
            EXPECT_TRUE(all(bit_shift_right<62>(input0) == (input0 >> 62)));
            EXPECT_TRUE(all(bit_shift_right<63>(input0) == (input0 >> 63)));
            EXPECT_TRUE(all(bit_shift_right<64>(input0) == (input0 >> 64)));
        }
    }

    TEST(Vec2x64u, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            std::uint64_t input1 = random64u() % 65;

            vec2x64u input0{input_array0};

            auto results = rotl(input0, input1);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] =  rotl(input_array0[j], input1);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            arr2x64u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random64u() % 65;
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = rotl(input0, input1);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotl(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            std::uint64_t input1 = random64u() % 65;

            vec2x64u input0{input_array0};

            auto results = rotr(input0, input1);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotr(input_array0[j], input1);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    TEST(Vec2x64u, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            arr2x64u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random64u() % 65;
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = rotr(input0, input1);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotr(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec2x64u{results_array}));
        }
    }

    //=====================================================
    // Conversions
    //=====================================================

    TEST(Vec2x64u, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = to_array(input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(results[j], input_array0[j]);
            }
        }
    }

}

#endif //AVEL_VEC2X64U_TESTS_HPP
