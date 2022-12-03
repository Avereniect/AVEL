#ifndef AVEL_VEC16X8I_TESTS_HPP
#define AVEL_VEC16X8I_TESTS_HPP

namespace avel_tests {
    
    using namespace avel;
    
    //=====================================================
    // mas16x8i tests
    //=====================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask16x8, Construct_from_primitive) {
        mask16x8i mask{mask16x8i::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask16x8, Construct_from_bool) {
        mask16x8i mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask16x8i mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 16);
    }

    TEST(Mask16x8, Construct_from_array) {
        mask16x8i mask0{{
            false, false, false, false,
            false, false, false, false,
            false, false, false, false,
            false, false, false, false
        }};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask16x8i mask1{{
            true, true, true, true,
            true, true, true, true,
            true, true, true, true,
            true, true, true, true
        }};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 16);

        mask16x8i mask2{{
            true, true, true, true,
            true, true, true, true,
            false, false, false, false,
            false, false, false, false
        }};

        EXPECT_TRUE(any(mask2));
        EXPECT_FALSE(all(mask2));
        EXPECT_FALSE(none(mask2));
        EXPECT_EQ(count(mask2), 8);

        mask16x8i mask3{{
            false, false, false, true,
            true, false, true, false,
            true, false, true, false,
            false, true, true, false
        }};

        EXPECT_TRUE(any(mask3));
        EXPECT_FALSE(all(mask3));
        EXPECT_FALSE(none(mask3));
        EXPECT_EQ(count(mask3), 7);
    }

    TEST(Mask16x8, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 16> input_array0;
            std::uint16_t cnt = 0;
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u() & 0x1;
                cnt += input_array0[j];
            }

            auto m = mask16x8i{input_array0};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == 16, all(m));
            EXPECT_EQ(cnt != 0, any(m));
            EXPECT_EQ(cnt == 0, none(m));
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask16x8i, Assign_bool) {
        mask16x8i mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask16x8i mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_EQ(count(mask1), 16);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask16x8i, Equality_comparison) {
        mask16x8i mask0{false};
        mask16x8i mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask16x8i mask2{true};
        mask16x8i mask3{true};
        EXPECT_EQ(mask2, mask3);

        mask16x8i mask4{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        mask16x8i mask5{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        EXPECT_EQ(mask4, mask5);

        mask16x8i mask6{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        mask16x8i mask7{{
            true, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        EXPECT_TRUE(!(mask6 == mask7));
    }

    TEST(Mask16x8i, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u() & 0x1;
            }

            std::array<bool, 16> input_array1{};
            input_array1[0] = !input_array0[0];
            for (std::size_t j = 1; j < input_array0.size(); ++j) {
                input_array1[j] = random8u() & 0x1;
            }

            mask16x8i mask0{input_array0};
            mask16x8i mask1{input_array0};
            mask16x8i mask2{input_array1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask16x8i, Inequality_comparison) {
        mask16x8i mask0{false};
        mask16x8i mask1{false};
        EXPECT_FALSE(mask0 != mask1);

        mask16x8i mask2{true};
        mask16x8i mask3{true};
        EXPECT_FALSE(mask2 != mask3);

        mask16x8i mask4{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        mask16x8i mask5{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        EXPECT_FALSE(mask4 != mask5);

        mask16x8i mask6{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        mask16x8i mask7{{
            true, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        EXPECT_TRUE(mask6 != mask7);
    }

    TEST(Mask16x8i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u() & 0x1;
            }

            std::array<bool, 16> input_array1{};
            input_array1[0] = !input_array0[0];
            for (std::size_t j = 1; j < input_array0.size(); ++j) {
                input_array1[j] = random8u() & 0x1;
            }

            mask16x8i mask0{input_array0};
            mask16x8i mask1{input_array1};
            mask16x8i mask2{input_array1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask16x8i, Bitwise_and_assignment) {
        mask16x8i mask0{{
            true, true, false, false,
            true, true, false, false,
            true, true, false, false,
            true, true, false, false
        }};

        mask16x8i mask1{{
            true, false, true, false,
            true, false, true, false,
            true, false, true, false,
            true, false, true, false
        }};

        mask0 &= mask1;

        mask16x8i mask2{{
            true, false, false, false,
            true, false, false, false,
            true, false, false, false,
            true, false, false, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    TEST(Mask16x8i, Bitwise_or_assignment) {
        mask16x8i mask0{{
            true, true, false, false,
            true, true, false, false,
            true, true, false, false,
            true, true, false, false
        }};

        mask16x8i mask1{{
            true, false, true, false,
            true, false, true, false,
            true, false, true, false,
            true, false, true, false
        }};

        mask0 |= mask1;

        mask16x8i mask2{{
            true, true, true, false,
            true, true, true, false,
            true, true, true, false,
            true, true, true, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    TEST(Mask16x8i, Bitwise_xor_assignment) {
        mask16x8i mask0{{
            true, true, false, false,
            true, true, false, false,
            true, true, false, false,
            true, true, false, false
        }};

        mask16x8i mask1{{
            true, false, true, false,
            true, false, true, false,
            true, false, true, false,
            true, false, true, false
        }};

        mask0 ^= mask1;

        mask16x8i mask2{{
            false, true, true, false,
            false, true, true, false,
            false, true, true, false,
            false, true, true, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(Mask16x8i, Logical_negation) {
        mask16x8i mask0{{
            false, false, true,  true,
            false, false, true,  true,
            false, false, true,  true,
            false, false, true,  true
        }};

        mask16x8i mask1{{
            true,  true,  false, false,
            true,  true,  false, false,
            true,  true,  false, false,
            true,  true,  false, false
        }};

        EXPECT_EQ(mask0, !mask1);
    }

    TEST(Mask16x8i, Bitwise_and) {
        mask16x8i m =
            mask16x8i{{
                true, true, false, false,
                true, true, false, false,
                true, true, false, false,
                true, true, false, false
            }} &
            mask16x8i{{
                true, false, true, false,
                true, false, true, false,
                true, false, true, false,
                true, false, true, false
            }};

        mask16x8i mask2{{
            true, false, false, false,
            true, false, false, false,
            true, false, false, false,
            true, false, false, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask16x8i, Logical_and) {
        mask16x8i m =
            mask16x8i{{
                true, true, false, false,
                true, true, false, false,
                true, true, false, false,
                true, true, false, false
            }} &&
            mask16x8i{{
                true, false, true, false,
                true, false, true, false,
                true, false, true, false,
                true, false, true, false
            }};

        mask16x8i mask2{{
            true, false, false, false,
            true, false, false, false,
            true, false, false, false,
            true, false, false, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask16x8i, Bitwise_or) {
        mask16x8i m =
            mask16x8i{{
                true, true, false, false,
                true, true, false, false,
                true, true, false, false,
                true, true, false, false
            }} |
            mask16x8i{{
                true, false, true, false,
                true, false, true, false,
                true, false, true, false,
                true, false, true, false
            }};

        mask16x8i mask2{{
            true, true, true, false,
            true, true, true, false,
            true, true, true, false,
            true, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask16x8i, Logical_or) {
        mask16x8i m =
            mask16x8i{{
                true, true, false, false,
                true, true, false, false,
                true, true, false, false,
                true, true, false, false
            }} ||
            mask16x8i{{
                true, false, true, false,
                true, false, true, false,
                true, false, true, false,
                true, false, true, false
            }};

        mask16x8i mask2{{
            true, true, true, false,
            true, true, true, false,
            true, true, true, false,
            true, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask16x8i, Bitwise_xor) {
        mask16x8i m =
            mask16x8i{{
                true, true, false, false,
                true, true, false, false,
                true, true, false, false,
                true, true, false, false
            }} ^
            mask16x8i{{
                true, false, true, false,
                true, false, true, false,
                true, false, true, false,
                true, false, true, false
            }};

        mask16x8i mask2{{
            false, true, true, false,
            false, true, true, false,
            false, true, true, false,
            false, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    //=========================================================================
    // Vec16x8i tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec16x8i, Convert_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 16> data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random8u() & 0x1;
            }

            mask16x8i m0{data};
            vec16x8i v0{m0};

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

    TEST(Vec16x8i, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t x = random8u();
            vec16x8i v{x};
            vec16x8i baseline{{
                x, x, x, x,
                x, x, x, x,
                x, x, x, x,
                x, x, x, x
            }};

            EXPECT_TRUE(all(v == baseline));
        }
    }

    TEST(Vec16x8i, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> data;

            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random8u();
            }

            vec16x8i v{data};

            auto results = to_array(v);

            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(data[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec16x8i, Assign_primitive) {
        vec16x8i::primitive x{};
        vec16x8i v{0x0F};
        v = x;
        vec16x8i baseline{};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(Vec16x8i, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t scalar = random8u();
            vec16x8i v;
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

    TEST(Vec16x8i, Equality_comparison) {
        mask16x8i mask0{false};
        mask16x8i mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask16x8i mask2{true};
        mask16x8i mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec16x8i, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random8u();
            }

            vec16x8i v0{data};
            vec16x8i v1{data};

            EXPECT_TRUE(all(v0 == v1));
        }
    }

    TEST(Vec16x8i, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> data0;
            std::array<std::int8_t, 16> data1;
            for (std::size_t j = 0; j < data0.size(); ++j) {
                data0[j] = random8u();
                data1[j] = random8u() ;
            }

            vec16x8i v0{data0};
            vec16x8i v1{data1};

            if (!all(v0 == v1)) {
                EXPECT_TRUE(any(v0 != v1));
            }
        }
    }

    TEST(Vec16x8i, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = (input0 < input1);

            std::array<bool, 16> results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] < input_array1[j];
            }

            EXPECT_EQ(results, mask16x8i{results_array});
        }
    }

    TEST(Vec16x8i, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = (input0 <= input1);

            std::array<bool, 16> results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] <= input_array1[j];
            }

            EXPECT_EQ(results, mask16x8i{results_array});
        }
    }

    TEST(Vec16x8i, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = (input0 > input1);

            std::array<bool, 16> results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] > input_array1[j];
            }

            EXPECT_EQ(results, mask16x8i{results_array});
        }
    }

    TEST(Vec16x8i, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = (input0 >= input1);

            std::array<bool, 16> results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] >= input_array1[j];
            }

            EXPECT_EQ(results, mask16x8i{results_array});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec16x8i, Unary_plus) {
        const vec16x8i u{{
            0x0, 0x1, 0x2, 0x3,
            0x4, 0x5, 0x6, 0x7,
            0x8, 0x9, 0xA, 0xB,
            0xC, 0xD, 0xE, 0xF
        }};
        vec16x8i v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec16x8i, Unary_minus) {
        const vec16x8i u{{
            0x0, 0x1, 0x2, 0x3,
            0x4, 0x5, 0x6, 0x7,
            0x8, 0x9, 0xA, 0xB,
            0xC, 0xD, 0xE, 0xF
        }};
        vec16x8i v{{
            -0x0, -0x1, -0x2, -0x3,
            -0x4, -0x5, -0x6, -0x7,
            -0x8, -0x9, -0xA, -0xB,
            -0xC, -0xD, -0xE, -0xF
        }};
        vec16x8i w{-u};

        EXPECT_TRUE(all(v == w));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(Vec16x8i, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};


            auto results = input0;
            results += input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};


            auto results = input0;
            results -= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};


            auto results = input0;
            results *= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = std::max(static_cast<std::int8_t>(random8u()), static_cast<std::int8_t>(1u));
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0;
            results /= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = std::max(static_cast<std::int8_t>(random8u()), static_cast<std::int8_t>(1u));
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0;
            results %= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec16x8i, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0 + input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0 - input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0 * input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = std::max(static_cast<std::int8_t>(random8u()), static_cast<std::int8_t>(1u));
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0 / input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = std::max(static_cast<std::int8_t>(random8u()), static_cast<std::int8_t>(1u));
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0 % input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec16x8i, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};
            ++input0;

            auto results = input0;
            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};
            input0++;

            auto results = input0;
            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};
            --input0;

            auto results = input0;
            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = +--input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};
            input0--;

            auto results = input0;
            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec16x8i, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0;
            results &= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0;
            results |= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0;
            results ^= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            std::int8_t input1 = random8u() % 16;

            vec16x8i input0{input_array0};

            auto results = input0;
            results <<= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u() % 16;
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0;
            results <<= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            std::int8_t input1 = random8u() % 16;

            vec16x8i input0{input_array0};

            auto results = input0;
            results >>= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto shift_value = (input1 >= 8) ? 7 : input1;
                results_array[j] = input_array0[j] >> shift_value;
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u() % 16;
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0;
            results >>= input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto shift_value = (input_array1[j] >= 8) ? 7 : input_array1[j];
                results_array[j] = input_array0[j] >> shift_value;
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec16x8i, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto results = ~input0;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ~input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            std::int8_t input1 = random8u() % 16;

            vec16x8i input0{input_array0};

            auto results = input0 << input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u() % 16;
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0 << input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            std::int8_t input1 = random8u() % 16;

            vec16x8i input0{input_array0};

            auto results = input0 >> input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto shift_value = (input1 >= 8) ? 7 : input1;
                results_array[j] = input_array0[j] >> shift_value;
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u() % 16;
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = input0 >> input1;

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto shift_value = (input_array1[j] >= 8) ? 7 : input_array1[j];
                results_array[j] = input_array0[j] >> shift_value;
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec16x8i, Conversion_to_mask) {
        vec16x8i vec{0};

        auto a = vec.operator mask16x8i();
        mask16x8i b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec16x8i, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto results = mask16x8i{input0};

            std::array<bool, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(results == mask16x8i{results_array});
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec16x8i, Broadcast_bits_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u() & 0x1;
            }

            mask16x8i input0{input_array0};

            auto results = broadcast_bits(input0);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array0[j]) {
                    results_array[j] = 0xff;
                } else {
                    results_array[j] = 0x00;
                }
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};
            std::array<bool, 16> input_array2{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
                input_array2[j] = random8u() & 0x1;
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};
            mask16x8i input2{input_array2};

            auto results = blend(input0, input1, input2);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array2[j]) {
                    results_array[j] = input_array1[j];
                } else {
                    results_array[j] = input_array0[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = max(input0, input1);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = min(input0, input1);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::min(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = minmax(input0, input1);

            std::array<std::int8_t, 16> results_array0{};
            std::array<std::int8_t, 16> results_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array0[j] = std::min(input_array0[j], input_array1[j]);
                results_array1[j] = std::max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results[0] == vec16x8i{results_array0}));
            EXPECT_TRUE(all(results[1] == vec16x8i{results_array1}));
        }
    }

    TEST(Vec16x8i, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};
            std::array<std::int8_t, 16> input_array2{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
                input_array2[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};
            vec16x8i input2{input_array2};

            auto xy = minmax(input1, input2);

            auto results = clamp(input0, xy[0], xy[1]);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array1[j], input_array2[j]);
                results_array[j] = clamp(input_array0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Average_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = average(input0, input1);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = average(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            std::array<std::int8_t, 16> input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            auto results = midpoint(input0, input1);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = midpoint(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto results = abs(input0);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto results = neg_abs(input0);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = neg_abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            auto results = load<vec16x8i>(input_array0.data());

            EXPECT_TRUE(all(results == vec16x8i{input_array0}));
        }
    }

    TEST(Vec16x8i, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec16x8i)) std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            auto results = aligned_load<vec16x8i>(input_array0.data());

            EXPECT_TRUE(all(results == vec16x8i{input_array0}));
        }
    }

    TEST(Vec16x8i, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec16x8i input0{input_array0};

            std::int8_t arr[16]{};
            store(arr, input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(input_array0[j], arr[j]);
            }
        }
    }

    TEST(Vec16x8i, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec16x8i input0{input_array0};

            alignas(alignof(vec16x8i)) std::int8_t arr[16]{};
            aligned_store(arr, input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(arr[j], input_array0[j]);
            }
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec16x8i, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> numerators{};
            for (std::size_t j = 0; j < numerators.size(); ++j) {
                numerators[j] = random8u();
            }

            std::array<std::int8_t, 16> denominators{};
            for (std::size_t j = 0; j < denominators.size(); ++j) {
                denominators[j] = std::max(static_cast<std::int8_t>(random8u()), static_cast<std::int8_t>(1u));
            }

            std::array<std::int8_t, 16> quotients{};
            std::array<std::int8_t, 16> remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec16x8i n{numerators};
            vec16x8i d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec16x8i, Popcount) {
        vec16x8i v{0x0};
        vec16x8i c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec16x8i, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto results = popcount(input0);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = popcount(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto results = byteswap(input0);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto results = countl_zero(input0);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto results = countl_one(input0);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto results = countr_zero(input0);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    TEST(Vec16x8i, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::int8_t, 16> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto results = countr_one(input0);

            std::array<std::int8_t, 16> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }
    
}

#endif //AVEL_VEC16X8I_TESTS_HPP
