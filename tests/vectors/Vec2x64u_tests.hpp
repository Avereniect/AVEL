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

    TEST(Vec2x64u, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            auto results = average(input0, input1);

            arr2x64u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = average(input_array0[j], input_array1[j]);
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

    TEST(Vec2x64u, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto results = neg_abs(input0);

            arr2x64i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = neg_abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec2x64i{results_array}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec2x64u, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array{};
            for (std::size_t j = 0; j < input_array.size(); ++j) {
                input_array[j] = random64u();
            }

            for (std::uint32_t j = 0; j <= vec2x64u::width; ++j) {
                auto results = load<vec2x64u>(input_array.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec2x64u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(input_array[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec2x64u, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            EXPECT_TRUE(all(load<vec2x64u, 0x00>(input_array0.data()) == load<vec2x64u>(input_array0.data(), 0x00)));
            EXPECT_TRUE(all(load<vec2x64u, 0x01>(input_array0.data()) == load<vec2x64u>(input_array0.data(), 0x01)));
            EXPECT_TRUE(all(load<vec2x64u, 0x02>(input_array0.data()) == load<vec2x64u>(input_array0.data(), 0x02)));
        }
    }

    TEST(Vec2x64u, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec2x64u)) arr2x64u input_array{};
            for (std::size_t j = 0; j < input_array.size(); ++j) {
                input_array[j] = random64u();
            }

            for (std::uint32_t j = 0; j <= vec2x64u::width; ++j) {
                auto results = load<vec2x64u>(input_array.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec2x64u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(input_array[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec2x64u, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec2x64u)) arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            EXPECT_TRUE(all(aligned_load<vec2x64u, 0x00>(input_array0.data()) == aligned_load<vec2x64u>(input_array0.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec2x64u, 0x01>(input_array0.data()) == aligned_load<vec2x64u>(input_array0.data(), 0x01)));
            EXPECT_TRUE(all(aligned_load<vec2x64u, 0x02>(input_array0.data()) == aligned_load<vec2x64u>(input_array0.data(), 0x02)));
        }
    }

    TEST(Vec2x64u, Gather_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;

        std::vector<std::uint64_t> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random64u();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = random64u() % test_data_size;
            }
            auto indices = load<vec2x64i>(index_array.data());

            for (std::uint32_t j = 0; j <= vec2x64u::width; ++j) {
                auto results = gather<vec2x64u>(test_data.data(), indices, j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec2x64u::width; k++) {
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

        std::vector<std::uint64_t> test_data{};
        test_data.resize(test_data_size);
        for (auto& x : test_data) {
            x = random64u();
        }

        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = random64u() % test_data_size;
            }
            auto indices = load<vec2x64i>(index_array.data());

            EXPECT_TRUE(all(gather<vec2x64u, 0x00>(test_data.data(), indices) == gather<vec2x64u>(test_data.data(), indices, 0x00)));
            EXPECT_TRUE(all(gather<vec2x64u, 0x01>(test_data.data(), indices) == gather<vec2x64u>(test_data.data(), indices, 0x01)));
            EXPECT_TRUE(all(gather<vec2x64u, 0x02>(test_data.data(), indices) == gather<vec2x64u>(test_data.data(), indices, 0x02)));
        }
    }

    TEST(Vec2x64u, Store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec2x64u input0{input_array0};

            for (std::size_t j = 0; j <= vec4x32u::width; ++j) {
                std::uint64_t arr[2]{};
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

    TEST(Vec2x64u, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            std::uint64_t arr[2]{};

            store<0x00>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x00));
            store<0x01>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x01));
            store<0x02>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x02));
        }
    }

    TEST(Vec2x64u, Algined_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec2x64u input0{input_array0};

            for (std::size_t j = 0; j <= vec4x32u::width; ++j) {
                alignas(alignof(vec2x64u)) std::uint64_t arr[2]{};
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

    TEST(Vec2x64u, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            alignas(alignof(vec2x64u)) std::uint64_t arr[2]{};

            aligned_store<0x00>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x00));
            aligned_store<0x01>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x01));
            aligned_store<0x02>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x02));
        }
    }

    TEST(Vec2x64u, Scatter_n_random) {
        static constexpr std::size_t test_data_size = 16 * 1024;
        static constexpr std::size_t block_size = test_data_size / vec2x64u::width;

        std::vector<std::uint64_t> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            arr2x64i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random64u() % block_size) + (j * block_size);
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

        std::vector<std::uint64_t> test_data{};
        test_data.resize(test_data_size);

        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            arr2x64i index_array{};
            for (std::size_t j = 0; j < index_array.size(); ++j) {
                index_array[j] = (random64u() % block_size) + (j * block_size);
            }

            vec2x64i indices{index_array};

            scatter<0x0>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x00));
            scatter<0x1>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x01));
            scatter<0x2>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, 0x02));
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
            EXPECT_TRUE(all(bit_shift_left<0x10>(input0) == (input0 << 0x10)));
            EXPECT_TRUE(all(bit_shift_left<0x11>(input0) == (input0 << 0x11)));
            EXPECT_TRUE(all(bit_shift_left<0x12>(input0) == (input0 << 0x12)));
            EXPECT_TRUE(all(bit_shift_left<0x13>(input0) == (input0 << 0x13)));
            EXPECT_TRUE(all(bit_shift_left<0x14>(input0) == (input0 << 0x14)));
            EXPECT_TRUE(all(bit_shift_left<0x15>(input0) == (input0 << 0x15)));
            EXPECT_TRUE(all(bit_shift_left<0x16>(input0) == (input0 << 0x16)));
            EXPECT_TRUE(all(bit_shift_left<0x17>(input0) == (input0 << 0x17)));
            EXPECT_TRUE(all(bit_shift_left<0x18>(input0) == (input0 << 0x18)));
            EXPECT_TRUE(all(bit_shift_left<0x19>(input0) == (input0 << 0x19)));
            EXPECT_TRUE(all(bit_shift_left<0x1a>(input0) == (input0 << 0x1a)));
            EXPECT_TRUE(all(bit_shift_left<0x1b>(input0) == (input0 << 0x1b)));
            EXPECT_TRUE(all(bit_shift_left<0x1c>(input0) == (input0 << 0x1c)));
            EXPECT_TRUE(all(bit_shift_left<0x1d>(input0) == (input0 << 0x1d)));
            EXPECT_TRUE(all(bit_shift_left<0x1e>(input0) == (input0 << 0x1e)));
            EXPECT_TRUE(all(bit_shift_left<0x1f>(input0) == (input0 << 0x1f)));
            EXPECT_TRUE(all(bit_shift_left<0x20>(input0) == (input0 << 0x20)));
            EXPECT_TRUE(all(bit_shift_left<0x21>(input0) == (input0 << 0x21)));
            EXPECT_TRUE(all(bit_shift_left<0x22>(input0) == (input0 << 0x22)));
            EXPECT_TRUE(all(bit_shift_left<0x23>(input0) == (input0 << 0x23)));
            EXPECT_TRUE(all(bit_shift_left<0x24>(input0) == (input0 << 0x24)));
            EXPECT_TRUE(all(bit_shift_left<0x25>(input0) == (input0 << 0x25)));
            EXPECT_TRUE(all(bit_shift_left<0x26>(input0) == (input0 << 0x26)));
            EXPECT_TRUE(all(bit_shift_left<0x27>(input0) == (input0 << 0x27)));
            EXPECT_TRUE(all(bit_shift_left<0x28>(input0) == (input0 << 0x28)));
            EXPECT_TRUE(all(bit_shift_left<0x29>(input0) == (input0 << 0x29)));
            EXPECT_TRUE(all(bit_shift_left<0x2a>(input0) == (input0 << 0x2a)));
            EXPECT_TRUE(all(bit_shift_left<0x2b>(input0) == (input0 << 0x2b)));
            EXPECT_TRUE(all(bit_shift_left<0x2c>(input0) == (input0 << 0x2c)));
            EXPECT_TRUE(all(bit_shift_left<0x2d>(input0) == (input0 << 0x2d)));
            EXPECT_TRUE(all(bit_shift_left<0x2e>(input0) == (input0 << 0x2e)));
            EXPECT_TRUE(all(bit_shift_left<0x2f>(input0) == (input0 << 0x2f)));
            EXPECT_TRUE(all(bit_shift_left<0x30>(input0) == (input0 << 0x30)));
            EXPECT_TRUE(all(bit_shift_left<0x31>(input0) == (input0 << 0x31)));
            EXPECT_TRUE(all(bit_shift_left<0x32>(input0) == (input0 << 0x32)));
            EXPECT_TRUE(all(bit_shift_left<0x33>(input0) == (input0 << 0x33)));
            EXPECT_TRUE(all(bit_shift_left<0x34>(input0) == (input0 << 0x34)));
            EXPECT_TRUE(all(bit_shift_left<0x35>(input0) == (input0 << 0x35)));
            EXPECT_TRUE(all(bit_shift_left<0x36>(input0) == (input0 << 0x36)));
            EXPECT_TRUE(all(bit_shift_left<0x37>(input0) == (input0 << 0x37)));
            EXPECT_TRUE(all(bit_shift_left<0x38>(input0) == (input0 << 0x38)));
            EXPECT_TRUE(all(bit_shift_left<0x39>(input0) == (input0 << 0x39)));
            EXPECT_TRUE(all(bit_shift_left<0x3a>(input0) == (input0 << 0x3a)));
            EXPECT_TRUE(all(bit_shift_left<0x3b>(input0) == (input0 << 0x3b)));
            EXPECT_TRUE(all(bit_shift_left<0x3c>(input0) == (input0 << 0x3c)));
            EXPECT_TRUE(all(bit_shift_left<0x3d>(input0) == (input0 << 0x3d)));
            EXPECT_TRUE(all(bit_shift_left<0x3e>(input0) == (input0 << 0x3e)));
            EXPECT_TRUE(all(bit_shift_left<0x3f>(input0) == (input0 << 0x3f)));
            EXPECT_TRUE(all(bit_shift_left<0x40>(input0) == vec2x64u{0x00}));
        }
    }

    TEST(Vec2x64u, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

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
            EXPECT_TRUE(all(bit_shift_right<0x10>(input0) == (input0 >> 0x10)));
            EXPECT_TRUE(all(bit_shift_right<0x11>(input0) == (input0 >> 0x11)));
            EXPECT_TRUE(all(bit_shift_right<0x12>(input0) == (input0 >> 0x12)));
            EXPECT_TRUE(all(bit_shift_right<0x13>(input0) == (input0 >> 0x13)));
            EXPECT_TRUE(all(bit_shift_right<0x14>(input0) == (input0 >> 0x14)));
            EXPECT_TRUE(all(bit_shift_right<0x15>(input0) == (input0 >> 0x15)));
            EXPECT_TRUE(all(bit_shift_right<0x16>(input0) == (input0 >> 0x16)));
            EXPECT_TRUE(all(bit_shift_right<0x17>(input0) == (input0 >> 0x17)));
            EXPECT_TRUE(all(bit_shift_right<0x18>(input0) == (input0 >> 0x18)));
            EXPECT_TRUE(all(bit_shift_right<0x19>(input0) == (input0 >> 0x19)));
            EXPECT_TRUE(all(bit_shift_right<0x1a>(input0) == (input0 >> 0x1a)));
            EXPECT_TRUE(all(bit_shift_right<0x1b>(input0) == (input0 >> 0x1b)));
            EXPECT_TRUE(all(bit_shift_right<0x1c>(input0) == (input0 >> 0x1c)));
            EXPECT_TRUE(all(bit_shift_right<0x1d>(input0) == (input0 >> 0x1d)));
            EXPECT_TRUE(all(bit_shift_right<0x1e>(input0) == (input0 >> 0x1e)));
            EXPECT_TRUE(all(bit_shift_right<0x1f>(input0) == (input0 >> 0x1f)));
            EXPECT_TRUE(all(bit_shift_right<0x20>(input0) == (input0 >> 0x20)));
            EXPECT_TRUE(all(bit_shift_right<0x21>(input0) == (input0 >> 0x21)));
            EXPECT_TRUE(all(bit_shift_right<0x22>(input0) == (input0 >> 0x22)));
            EXPECT_TRUE(all(bit_shift_right<0x23>(input0) == (input0 >> 0x23)));
            EXPECT_TRUE(all(bit_shift_right<0x24>(input0) == (input0 >> 0x24)));
            EXPECT_TRUE(all(bit_shift_right<0x25>(input0) == (input0 >> 0x25)));
            EXPECT_TRUE(all(bit_shift_right<0x26>(input0) == (input0 >> 0x26)));
            EXPECT_TRUE(all(bit_shift_right<0x27>(input0) == (input0 >> 0x27)));
            EXPECT_TRUE(all(bit_shift_right<0x28>(input0) == (input0 >> 0x28)));
            EXPECT_TRUE(all(bit_shift_right<0x29>(input0) == (input0 >> 0x29)));
            EXPECT_TRUE(all(bit_shift_right<0x2a>(input0) == (input0 >> 0x2a)));
            EXPECT_TRUE(all(bit_shift_right<0x2b>(input0) == (input0 >> 0x2b)));
            EXPECT_TRUE(all(bit_shift_right<0x2c>(input0) == (input0 >> 0x2c)));
            EXPECT_TRUE(all(bit_shift_right<0x2d>(input0) == (input0 >> 0x2d)));
            EXPECT_TRUE(all(bit_shift_right<0x2e>(input0) == (input0 >> 0x2e)));
            EXPECT_TRUE(all(bit_shift_right<0x2f>(input0) == (input0 >> 0x2f)));
            EXPECT_TRUE(all(bit_shift_right<0x30>(input0) == (input0 >> 0x30)));
            EXPECT_TRUE(all(bit_shift_right<0x31>(input0) == (input0 >> 0x31)));
            EXPECT_TRUE(all(bit_shift_right<0x32>(input0) == (input0 >> 0x32)));
            EXPECT_TRUE(all(bit_shift_right<0x33>(input0) == (input0 >> 0x33)));
            EXPECT_TRUE(all(bit_shift_right<0x34>(input0) == (input0 >> 0x34)));
            EXPECT_TRUE(all(bit_shift_right<0x35>(input0) == (input0 >> 0x35)));
            EXPECT_TRUE(all(bit_shift_right<0x36>(input0) == (input0 >> 0x36)));
            EXPECT_TRUE(all(bit_shift_right<0x37>(input0) == (input0 >> 0x37)));
            EXPECT_TRUE(all(bit_shift_right<0x38>(input0) == (input0 >> 0x38)));
            EXPECT_TRUE(all(bit_shift_right<0x39>(input0) == (input0 >> 0x39)));
            EXPECT_TRUE(all(bit_shift_right<0x3a>(input0) == (input0 >> 0x3a)));
            EXPECT_TRUE(all(bit_shift_right<0x3b>(input0) == (input0 >> 0x3b)));
            EXPECT_TRUE(all(bit_shift_right<0x3c>(input0) == (input0 >> 0x3c)));
            EXPECT_TRUE(all(bit_shift_right<0x3d>(input0) == (input0 >> 0x3d)));
            EXPECT_TRUE(all(bit_shift_right<0x3e>(input0) == (input0 >> 0x3e)));
            EXPECT_TRUE(all(bit_shift_right<0x3f>(input0) == (input0 >> 0x3f)));
            EXPECT_TRUE(all(bit_shift_right<0x40>(input0) == vec2x64u{0x00}));
        }
    }

    TEST(Vec2x64u, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            long long input1 = random64u();

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
                input_array1[j] = random64u();
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

    TEST(Vec2x64u, Rotr) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            EXPECT_TRUE(all(rotr<0x00>(input0) == rotr(input0, 0x00)));
            EXPECT_TRUE(all(rotr<0x01>(input0) == rotr(input0, 0x01)));
            EXPECT_TRUE(all(rotr<0x02>(input0) == rotr(input0, 0x02)));
            EXPECT_TRUE(all(rotr<0x03>(input0) == rotr(input0, 0x03)));
            EXPECT_TRUE(all(rotr<0x04>(input0) == rotr(input0, 0x04)));
            EXPECT_TRUE(all(rotr<0x05>(input0) == rotr(input0, 0x05)));
            EXPECT_TRUE(all(rotr<0x06>(input0) == rotr(input0, 0x06)));
            EXPECT_TRUE(all(rotr<0x07>(input0) == rotr(input0, 0x07)));
            EXPECT_TRUE(all(rotr<0x08>(input0) == rotr(input0, 0x08)));
            EXPECT_TRUE(all(rotr<0x09>(input0) == rotr(input0, 0x09)));
            EXPECT_TRUE(all(rotr<0x0a>(input0) == rotr(input0, 0x0a)));
            EXPECT_TRUE(all(rotr<0x0b>(input0) == rotr(input0, 0x0b)));
            EXPECT_TRUE(all(rotr<0x0c>(input0) == rotr(input0, 0x0c)));
            EXPECT_TRUE(all(rotr<0x0d>(input0) == rotr(input0, 0x0d)));
            EXPECT_TRUE(all(rotr<0x0e>(input0) == rotr(input0, 0x0e)));
            EXPECT_TRUE(all(rotr<0x0f>(input0) == rotr(input0, 0x0f)));
            EXPECT_TRUE(all(rotr<0x10>(input0) == rotr(input0, 0x10)));
            EXPECT_TRUE(all(rotr<0x11>(input0) == rotr(input0, 0x11)));
            EXPECT_TRUE(all(rotr<0x12>(input0) == rotr(input0, 0x12)));
            EXPECT_TRUE(all(rotr<0x13>(input0) == rotr(input0, 0x13)));
            EXPECT_TRUE(all(rotr<0x14>(input0) == rotr(input0, 0x14)));
            EXPECT_TRUE(all(rotr<0x15>(input0) == rotr(input0, 0x15)));
            EXPECT_TRUE(all(rotr<0x16>(input0) == rotr(input0, 0x16)));
            EXPECT_TRUE(all(rotr<0x17>(input0) == rotr(input0, 0x17)));
            EXPECT_TRUE(all(rotr<0x18>(input0) == rotr(input0, 0x18)));
            EXPECT_TRUE(all(rotr<0x19>(input0) == rotr(input0, 0x19)));
            EXPECT_TRUE(all(rotr<0x1a>(input0) == rotr(input0, 0x1a)));
            EXPECT_TRUE(all(rotr<0x1b>(input0) == rotr(input0, 0x1b)));
            EXPECT_TRUE(all(rotr<0x1c>(input0) == rotr(input0, 0x1c)));
            EXPECT_TRUE(all(rotr<0x1d>(input0) == rotr(input0, 0x1d)));
            EXPECT_TRUE(all(rotr<0x1e>(input0) == rotr(input0, 0x1e)));
            EXPECT_TRUE(all(rotr<0x1f>(input0) == rotr(input0, 0x1f)));
            EXPECT_TRUE(all(rotr<0x20>(input0) == rotr(input0, 0x20)));
            EXPECT_TRUE(all(rotr<0x21>(input0) == rotr(input0, 0x21)));
            EXPECT_TRUE(all(rotr<0x22>(input0) == rotr(input0, 0x22)));
            EXPECT_TRUE(all(rotr<0x23>(input0) == rotr(input0, 0x23)));
            EXPECT_TRUE(all(rotr<0x24>(input0) == rotr(input0, 0x24)));
            EXPECT_TRUE(all(rotr<0x25>(input0) == rotr(input0, 0x25)));
            EXPECT_TRUE(all(rotr<0x26>(input0) == rotr(input0, 0x26)));
            EXPECT_TRUE(all(rotr<0x27>(input0) == rotr(input0, 0x27)));
            EXPECT_TRUE(all(rotr<0x28>(input0) == rotr(input0, 0x28)));
            EXPECT_TRUE(all(rotr<0x29>(input0) == rotr(input0, 0x29)));
            EXPECT_TRUE(all(rotr<0x2a>(input0) == rotr(input0, 0x2a)));
            EXPECT_TRUE(all(rotr<0x2b>(input0) == rotr(input0, 0x2b)));
            EXPECT_TRUE(all(rotr<0x2c>(input0) == rotr(input0, 0x2c)));
            EXPECT_TRUE(all(rotr<0x2d>(input0) == rotr(input0, 0x2d)));
            EXPECT_TRUE(all(rotr<0x2e>(input0) == rotr(input0, 0x2e)));
            EXPECT_TRUE(all(rotr<0x2f>(input0) == rotr(input0, 0x2f)));
            EXPECT_TRUE(all(rotr<0x30>(input0) == rotr(input0, 0x30)));
            EXPECT_TRUE(all(rotr<0x31>(input0) == rotr(input0, 0x31)));
            EXPECT_TRUE(all(rotr<0x32>(input0) == rotr(input0, 0x32)));
            EXPECT_TRUE(all(rotr<0x33>(input0) == rotr(input0, 0x33)));
            EXPECT_TRUE(all(rotr<0x34>(input0) == rotr(input0, 0x34)));
            EXPECT_TRUE(all(rotr<0x35>(input0) == rotr(input0, 0x35)));
            EXPECT_TRUE(all(rotr<0x36>(input0) == rotr(input0, 0x36)));
            EXPECT_TRUE(all(rotr<0x37>(input0) == rotr(input0, 0x37)));
            EXPECT_TRUE(all(rotr<0x38>(input0) == rotr(input0, 0x38)));
            EXPECT_TRUE(all(rotr<0x39>(input0) == rotr(input0, 0x39)));
            EXPECT_TRUE(all(rotr<0x3a>(input0) == rotr(input0, 0x3a)));
            EXPECT_TRUE(all(rotr<0x3b>(input0) == rotr(input0, 0x3b)));
            EXPECT_TRUE(all(rotr<0x3c>(input0) == rotr(input0, 0x3c)));
            EXPECT_TRUE(all(rotr<0x3d>(input0) == rotr(input0, 0x3d)));
            EXPECT_TRUE(all(rotr<0x3e>(input0) == rotr(input0, 0x3e)));
            EXPECT_TRUE(all(rotr<0x3f>(input0) == rotr(input0, 0x3f)));
        }
    }

    TEST(Vec2x64u, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            long long input1 = random64u();

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
                input_array1[j] = random64u();
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

#endif
