#ifndef AVEL_VEC4X32U_TESTS_HPP
#define AVEL_VEC4X32U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=========================================================================
    // mask4x32u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask4x32u, Construct_from_primitive) {
        mask4x32u mask{mask4x32u::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask4x32u, Construct_from_bool) {
        mask4x32u mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask4x32u mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 4);
    }

    TEST(Mask4x32u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0;
            std::uint32_t cnt = 0;
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                cnt += input_array0[j];
            }

            mask4x32u m{input_array0};

            EXPECT_EQ(count(m), cnt);
            EXPECT_EQ(any(m), cnt != 0);
            EXPECT_EQ(all(m), cnt == 4);
            EXPECT_EQ(none(m), cnt == 0);
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask4x32u, Assign_bool) {
        mask4x32u mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask4x32u mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_EQ(count(mask1), 4);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask4x32u, Equality_comparison) {
        mask4x32u mask0{false};
        mask4x32u mask1{false};

        mask4x32u mask2{true};
        mask4x32u mask3{true};

        EXPECT_TRUE(mask0 == mask1);
        EXPECT_TRUE(mask2 == mask3);

        EXPECT_FALSE(mask0 == mask2);
        EXPECT_FALSE(mask3 == mask1);
    }

    TEST(Mask4x32u, Equality_comparison_random) {
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

            mask4x32u mask0{input_array0};
            mask4x32u mask1{input_array0};
            mask4x32u mask2{input_array1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask4x32u, Inequality_comparison) {
        mask4x32u mask0{false};
        mask4x32u mask1{false};

        mask4x32u mask2{true};
        mask4x32u mask3{true};

        EXPECT_TRUE(mask0 != mask2);
        EXPECT_TRUE(mask1 != mask3);

        EXPECT_FALSE(mask0 != mask1);
        EXPECT_FALSE(mask2 != mask3);
    }

    TEST(Mask4x32u, Inequality_comparison_random) {
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

            mask4x32u mask0{input_array0};
            mask4x32u mask1{input_array1};
            mask4x32u mask2{input_array1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask4x32u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4xb input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                input_array1[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};
            mask4x32u input1{input_array1};

            auto results = input0;
            results &= input1;

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    TEST(Mask4x32u, Bitwise_or_assignment) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4xb input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                input_array1[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};
            mask4x32u input1{input_array1};

            auto results = input0;
            results |= input1;

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    TEST(Mask4x32u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4xb input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                input_array1[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};
            mask4x32u input1{input_array1};

            auto results = input0;
            results ^= input1;

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask4x32u, Logical_negation) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};

            auto results = !input0;

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = !input_array0[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    TEST(Mask4x32u, Bitwise_and) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4xb input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                input_array1[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};
            mask4x32u input1{input_array1};

            auto results = input0 & input1;

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    TEST(Mask4x32u, Logical_and) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4xb input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                input_array1[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};
            mask4x32u input1{input_array1};

            auto results = input0 && input1;

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    TEST(Mask4x32u, Bitwise_or) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4xb input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                input_array1[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};
            mask4x32u input1{input_array1};

            auto results = input0 | input1;

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    TEST(Mask4x32u, Logical_or) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4xb input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                input_array1[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};
            mask4x32u input1{input_array1};

            auto results = input0 || input1;

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    TEST(Mask4x32u, Bitwise_xor) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4xb input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                input_array1[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};
            mask4x32u input1{input_array1};

            auto results = input0 ^ input1;

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    //=========================================================================
    // vec4x32u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec4x32u, Construct_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random32u() & 0x1;
            }

            mask4x32u m0{data};
            vec4x32u v0{m0};

            auto results = to_array(v0);
            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(results[j], data[j] ? 0x1: 0x0);
            }
        }
    }

    TEST(Vec4x32u, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random32u();
            vec4x32u v{x};
            vec4x32u baseline{{x, x, x, x}};

            EXPECT_TRUE(all(v == baseline));
        }
    }

    TEST(Vec4x32u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u data;

            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random32u();
            }

            vec4x32u v{data};

            auto results = to_array(v);

            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(data[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec4x32u, Assign_primitive) {
        vec4x32u::primitive x{};
        vec4x32u v{0x76543210};
        v = x;
        vec4x32u baseline{0x00000000};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(Vec4x32u, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random32u();
            vec4x32u v{};
            v = x;
            vec4x32u baseline{{x, x, x, x}};

            EXPECT_TRUE(all(v == baseline));
        }
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Vec4x32u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random32u();
            }

            vec4x32u v0{data};
            vec4x32u v1{data};

            EXPECT_TRUE(all(v0 == v1));
        }
    }

    TEST(Vec4x32u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u data0;
            arr4x32u data1;

            for (std::size_t j = 0; j < data0.size(); ++j) {
                data0[j] = random32u();
                data1[j] = random32u() ;
            }

            vec4x32u v0{data0};
            vec4x32u v1{data1};

            if (!all(v0 == v1)) {
                EXPECT_TRUE(any(v0 != v1));
            }
        }
    }

    TEST(Vec4x32u, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = (input0 < input1);

            arr4xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] < input_array1[j];
            }

            EXPECT_EQ(results, mask4x32u{results_array});
        }
    }

    TEST(Vec4x32u, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = (input0 <= input1);

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] <= input_array1[j];
            }

            EXPECT_EQ(results, mask4x32u{results_array});
        }
    }

    TEST(Vec4x32u, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = (input0 > input1);

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] > input_array1[j];
            }

            EXPECT_EQ(results, mask4x32u{results_array});
        }
    }

    TEST(Vec4x32u, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = (input0 >= input1);

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] >= input_array1[j];
            }

            EXPECT_EQ(results, mask4x32u{results_array});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec4x32u, Unary_plus) {
        const vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec4x32u, Unary_minus) {
        const vec4x32u u{{+0, +1, +2, +3}};
        vec4x32i v{{-0, -1, -2, -3}};
        vec4x32i w{-u};

        EXPECT_TRUE(all(v == w));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec4x32u, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results += input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results -= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results *= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = std::max(static_cast<std::uint32_t>(random32u()), static_cast<std::uint32_t>(1u));
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results /= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = std::max(static_cast<std::uint32_t>(random32u()), static_cast<std::uint32_t>(1u));
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results %= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec4x32u, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0 + input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0 - input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0 * input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = std::max(static_cast<std::uint32_t>(random32u()), static_cast<std::uint32_t>(1u));
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0 / input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = std::max(static_cast<std::uint32_t>(random32u()), static_cast<std::uint32_t>(1u));
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0 % input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec4x32u, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};
            ++input0;

            auto results = input0;
            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};
            input0++;

            auto results = input0;
            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};
            --input0;

            auto results = input0;
            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};
            input0--;

            auto results = input0;
            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec4x32u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results &= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results |= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            arr4x32u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results ^= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::uint32_t input1 = random32u() % 33;

            vec4x32u input0{input_array0};

            auto results = input0;
            results <<= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u() % 33;
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results <<= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::uint32_t input1 = random32u() % 33;

            vec4x32u input0{input_array0};

            auto results = input0;
            results >>= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u() % 33;
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results >>= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec4x32u, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = ~input0;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ~input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bitwise_and_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0 & input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bitwise_or_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0 | input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bitwise_xor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0 ^ input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Left_shift_by_scalar) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::uint32_t input1 = random32u() % 33;

            vec4x32u input0{input_array0};

            auto results = input0;
            results <<= input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Left_shift_by_vector) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u() % 33;
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0 << input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Right_shift_by_scalar) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::uint32_t input1 = random32u() % 33;

            vec4x32u input0{input_array0};

            auto results = input0 >> input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Right_shift_by_vector) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u() % 33;
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0 >> input1;

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 32) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec4x32u, Conversion_to_mask) {
        vec4x32u v{0};

        mask4x32u a{v};
        mask4x32u b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec4x32u, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = mask4x32u{input0};

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec4x32u, Broadcast_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};

            auto results = broadcast_mask(input0);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array0[j]) {
                    results_array[j] = -1;
                } else {
                    results_array[j] = 0;
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }
    
    TEST(Vec4x32u, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4xb input_array0{};
            arr4x32u input_array1{};
            arr4x32u input_array2{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
                input_array1[j] = random32u();
                input_array2[j] = random32u();
            }

            mask4x32u input0{input_array0};
            vec4x32u input1{input_array1};
            vec4x32u input2{input_array2};

            auto results = blend(input0, input1, input2);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = blend(input_array0[j], input_array1[j], input_array2[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = max(input0, input1);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            arr4x32u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input1{input_array1};

            auto results = min(input0, input1);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::min(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = minmax(input0, input1);

            arr4x32u results_array0{};
            arr4x32u results_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array0[j], input_array1[j]);
                results_array0[j] = xy[0];
                results_array1[j] = xy[1];
            }

            EXPECT_TRUE(all(results[0] == vec4x32u{results_array0}));
            EXPECT_TRUE(all(results[1] == vec4x32u{results_array1}));
        }
    }

    TEST(Vec4x32u, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};
            arr4x32u input_array2{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
                input_array2[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};
            vec4x32u input2{input_array2};

            auto range = minmax(input1, input2);

            auto results = clamp(input0, range[0], range[1]);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array1[j], input_array2[j]);
                results_array[j] = clamp(input_array0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = midpoint(input0, input1);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = midpoint(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            auto results = load<vec4x32u>(input_array0.data());

            EXPECT_TRUE(all(results == vec4x32u{input_array0}));
        }
    }

    TEST(Vec4x32u, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec4x32u)) arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            auto results = aligned_load<vec4x32u>(input_array0.data());

            EXPECT_TRUE(all(results == vec4x32u{input_array0}));
        }
    }

    TEST(Vec4x32u, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            std::uint32_t arr[4]{};
            store(arr, input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(input_array0[j], arr[j]);
            }
        }
    }

    TEST(Vec4x32u, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            alignas(alignof(vec4x32u)) std::uint32_t arr[4]{};
            aligned_store(arr, input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(arr[j], input_array0[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec4x32u, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            arr4x32u numerators{};
            for (std::size_t j = 0; j < numerators.size(); ++j) {
                numerators[j] = random32u();
            }

            arr4x32u denominators{};
            for (std::size_t j = 0; j < denominators.size(); ++j) {
                denominators[j] = std::max(static_cast<std::uint32_t>(random32u()), static_cast<std::uint32_t>(1u));
            }

            arr4x32u quotients{};
            arr4x32u remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec4x32u n{numerators};
            vec4x32u d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec4x32u, Popcount) {
        vec4x32u v0{0x00000000};
        vec4x32u b0{0x00};
        vec4x32u c0 = popcount(v0);
        EXPECT_TRUE(all(c0 == b0));

        vec4x32u v1{0xFFFFFFFF};
        vec4x32u b1{0x20};
        vec4x32u c1 = popcount(v1);
        EXPECT_TRUE(all(c1 == b1));
    }

    TEST(Vec4x32u, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = popcount(input0);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = popcount(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = byteswap(input0);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = byteswap(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Countl_zero) {
        std::array<int, 4> rounding_modes{
            FE_DOWNWARD,
            FE_TONEAREST,
            FE_TOWARDZERO,
            FE_UPWARD
        };

        for (std::size_t i = 0; i < 4; ++i) {
            std::fesetround(rounding_modes[i]);

            vec4x32u v0{0x0};
            vec4x32u u0 = countl_zero(v0);
            vec4x32u b0{32};

            EXPECT_TRUE(all(u0 == b0));

            vec4x32u v1{0xFFFFFFFF};
            vec4x32u u1 = countl_zero(v1);
            vec4x32u b1{0x0};

            EXPECT_TRUE(all(u1 == b1));
        }
    }

    TEST(Vec4x32u, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = countl_zero(input0);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Countl_one) {
        vec4x32u v0{{
            0xFFFFFFFF,
            0xFFFFFFFE,
            0x80000000,
            0x00000000
        }};

        vec4x32u r0 = countl_one(v0);
        vec4x32u b0{{32, 31, 1, 0}};

        EXPECT_TRUE(all(r0 == b0));
    }

    TEST(Vec4x32u, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = countl_one(input0);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Countr_zero) {
        vec4x32u v0{{
            0xFFFFFFFF,
            0xFFFFFFFE,
            0x80000000,
            0x00000000
        }};

        vec4x32u r0 = countr_zero(v0);
        vec4x32u b0{{0, 1, 31, 32}};

        EXPECT_TRUE(all(r0 == b0));
    }

    TEST(Vec4x32u, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = countr_zero(input0);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_zero(input_array0[j]);
            }

            bool x = all(results == vec4x32u{results_array});
            EXPECT_TRUE(x);
        }
    }

    TEST(Vec4x32u, Countr_one) {
        vec4x32u v0{{
            0xFFFFFFFF,
            0x7FFFFFFF,
            0x00000001,
            0x00000000
        }};

        vec4x32u r0 = countr_one(v0);
        vec4x32u b0{{32, 31, 1, 0}};

        EXPECT_TRUE(all(r0 == b0));
    }

    TEST(Vec4x32u, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = countr_one(input0);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bit_width) {
        vec4x32u v0{{
            0x00000000,
            0x00000001,
            0x7FFFFFFF,
            0xFFFFFFFF
        }};

        vec4x32u r0 = bit_width(v0);
        vec4x32u b0{{0, 1, 31, 32}};

        EXPECT_TRUE(all(r0 == b0));
    }

    TEST(Vec4x32u, Bit_width_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = bit_width(input0);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_width(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bit_floor) {
        vec4x32u v0{0x0};

        vec4x32u r0 = bit_floor(v0);
        vec4x32u b0{0x0};

        EXPECT_TRUE(all(r0 == b0));
    }

    TEST(Vec4x32u, Bit_floor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = bit_floor(input0);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_floor(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bit_ceil) {
        vec4x32u v0{0x0};

        vec4x32u r0 = bit_ceil(v0);
        vec4x32u b0{0x1};

        EXPECT_TRUE(all(r0 == b0));
    }

    TEST(Vec4x32u, Bit_ceil_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = bit_ceil(input0);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_ceil(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Has_single_bit) {
        for (std::size_t i = 0; i < 32; ++i) {
            EXPECT_TRUE(all(has_single_bit(vec4x32u{static_cast<std::uint32_t>(1 << i)})));
        }
    }

    TEST(Vec4x32u, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = has_single_bit(input0);

            arr4xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = has_single_bit(input_array0[j]);
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    //=====================================================
    // Bit Manipulation Instructions
    //=====================================================

    TEST(Vec4x32u, Bit_shift_left) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

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
            EXPECT_TRUE(all(bit_shift_left<0x20>(input0) == vec4x32u{0x00}));
        }
    }

    TEST(Vec4x32u, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

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
            EXPECT_TRUE(all(bit_shift_right<0x20>(input0) == vec4x32u{0x00}));
        }
    }

    TEST(Vec4x32u, Rotl) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            EXPECT_TRUE(all(rotl<0x00>(input0) == rotl(input0, 0x00)));
            EXPECT_TRUE(all(rotl<0x01>(input0) == rotl(input0, 0x01)));
            EXPECT_TRUE(all(rotl<0x02>(input0) == rotl(input0, 0x02)));
            EXPECT_TRUE(all(rotl<0x03>(input0) == rotl(input0, 0x03)));
            EXPECT_TRUE(all(rotl<0x04>(input0) == rotl(input0, 0x04)));
            EXPECT_TRUE(all(rotl<0x05>(input0) == rotl(input0, 0x05)));
            EXPECT_TRUE(all(rotl<0x06>(input0) == rotl(input0, 0x06)));
            EXPECT_TRUE(all(rotl<0x07>(input0) == rotl(input0, 0x07)));
            EXPECT_TRUE(all(rotl<0x08>(input0) == rotl(input0, 0x08)));
            EXPECT_TRUE(all(rotl<0x09>(input0) == rotl(input0, 0x09)));
            EXPECT_TRUE(all(rotl<0x0a>(input0) == rotl(input0, 0x0a)));
            EXPECT_TRUE(all(rotl<0x0b>(input0) == rotl(input0, 0x0b)));
            EXPECT_TRUE(all(rotl<0x0c>(input0) == rotl(input0, 0x0c)));
            EXPECT_TRUE(all(rotl<0x0d>(input0) == rotl(input0, 0x0d)));
            EXPECT_TRUE(all(rotl<0x0e>(input0) == rotl(input0, 0x0e)));
            EXPECT_TRUE(all(rotl<0x0f>(input0) == rotl(input0, 0x0f)));
            EXPECT_TRUE(all(rotl<0x10>(input0) == rotl(input0, 0x10)));
            EXPECT_TRUE(all(rotl<0x11>(input0) == rotl(input0, 0x11)));
            EXPECT_TRUE(all(rotl<0x12>(input0) == rotl(input0, 0x12)));
            EXPECT_TRUE(all(rotl<0x13>(input0) == rotl(input0, 0x13)));
            EXPECT_TRUE(all(rotl<0x14>(input0) == rotl(input0, 0x14)));
            EXPECT_TRUE(all(rotl<0x15>(input0) == rotl(input0, 0x15)));
            EXPECT_TRUE(all(rotl<0x16>(input0) == rotl(input0, 0x16)));
            EXPECT_TRUE(all(rotl<0x17>(input0) == rotl(input0, 0x17)));
            EXPECT_TRUE(all(rotl<0x18>(input0) == rotl(input0, 0x18)));
            EXPECT_TRUE(all(rotl<0x19>(input0) == rotl(input0, 0x19)));
            EXPECT_TRUE(all(rotl<0x1a>(input0) == rotl(input0, 0x1a)));
            EXPECT_TRUE(all(rotl<0x1b>(input0) == rotl(input0, 0x1b)));
            EXPECT_TRUE(all(rotl<0x1c>(input0) == rotl(input0, 0x1c)));
            EXPECT_TRUE(all(rotl<0x1d>(input0) == rotl(input0, 0x1d)));
            EXPECT_TRUE(all(rotl<0x1e>(input0) == rotl(input0, 0x1e)));
            EXPECT_TRUE(all(rotl<0x1f>(input0) == rotl(input0, 0x1f)));
        }
    }

    TEST(Vec4x32u, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            long long input1 = random64u();

            vec4x32u input0{input_array0};

            auto results = rotl(input0, input1);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotl(input_array0[j], input1);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            arr4x32u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = rotl(input0, input1);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotl(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Rotr) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

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
        }
    }

    TEST(Vec4x32u, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            long long input1 = random64u();

            vec4x32u input0{input_array0};

            auto results = rotr(input0, input1);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotr(input_array0[j], input1);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            arr4x32u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = rotr(input0, input1);

            arr4x32u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotr(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    //=====================================================
    // Conversions
    //=====================================================

    TEST(Vec4x32u, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = to_array(input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(results[j], input_array0[j]);
            }
        }
    }

}

#endif //AVEL_VEC4X32U_TESTS_HPP
