#ifndef AVEL_VEC16X8U_TESTS_HPP
#define AVEL_VEC16X8U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=========================================================================
    // mask16x8u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Mask16x8u, Construct_from_primitive) {
        mask16x8u mask{mask16x8u::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(Mask16x8u, Construct_from_bool) {
        mask16x8u mask0{false};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask16x8u mask1{true};
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 16);
    }

    TEST(Mask16x8u, Construct_from_array) {
        mask16x8u mask0{{
            false, false, false, false,
            false, false, false, false,
            false, false, false, false,
            false, false, false, false
        }};

        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask16x8u mask1{{
            true, true, true, true,
            true, true, true, true,
            true, true, true, true,
            true, true, true, true
        }};

        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 16);

        mask16x8u mask2{{
            true, true, true, true,
            true, true, true, true,
            false, false, false, false,
            false, false, false, false
        }};

        EXPECT_TRUE(any(mask2));
        EXPECT_FALSE(all(mask2));
        EXPECT_FALSE(none(mask2));
        EXPECT_EQ(count(mask2), 8);

        mask16x8u mask3{{
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

    TEST(Mask16x8u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16xb input_array0;
            std::uint16_t cnt = 0;
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u() & 0x1;
                cnt += input_array0[j];
            }

            auto m = mask16x8u{input_array0};

            EXPECT_EQ(cnt, count(m));
            EXPECT_EQ(cnt == 16, all(m));
            EXPECT_EQ(cnt != 0, any(m));
            EXPECT_EQ(cnt == 0, none(m));
        }
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(Mask16x8u, Assign_bool) {
        mask16x8u mask0{};
        mask0 = false;
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask16x8u mask1{};
        mask1 = true;
        EXPECT_TRUE(all(mask1));
        EXPECT_TRUE(any(mask1));
        EXPECT_EQ(count(mask1), 16);
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Mask16x8u, Equality_comparison) {
        mask16x8u mask0{false};
        mask16x8u mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask16x8u mask2{true};
        mask16x8u mask3{true};
        EXPECT_EQ(mask2, mask3);

        mask16x8u mask4{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        mask16x8u mask5{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        EXPECT_EQ(mask4, mask5);

        mask16x8u mask6{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        mask16x8u mask7{{
            true, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        EXPECT_TRUE(!(mask6 == mask7));
    }

    TEST(Mask16x8u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u() & 0x1;
            }

            arr16xb input_array1{};
            input_array1[0] = !input_array0[0];
            for (std::size_t j = 1; j < input_array0.size(); ++j) {
                input_array1[j] = random8u() & 0x1;
            }

            mask16x8u mask0{input_array0};
            mask16x8u mask1{input_array0};
            mask16x8u mask2{input_array1};

            EXPECT_TRUE(mask0 == mask1);
            EXPECT_FALSE(mask1 == mask2);
        }
    }

    TEST(Mask16x8u, Inequality_comparison) {
        mask16x8u mask0{false};
        mask16x8u mask1{false};
        EXPECT_FALSE(mask0 != mask1);

        mask16x8u mask2{true};
        mask16x8u mask3{true};
        EXPECT_FALSE(mask2 != mask3);

        mask16x8u mask4{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        mask16x8u mask5{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        EXPECT_FALSE(mask4 != mask5);

        mask16x8u mask6{{
            false, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        mask16x8u mask7{{
            true, false, true,  true,
            false, true,  false, false,
            false, true,  true,  false,
            true,  true,  false, false
        }};

        EXPECT_TRUE(mask6 != mask7);
    }

    TEST(Mask16x8u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u() & 0x1;
            }

            arr16xb input_array1{};
            input_array1[0] = !input_array0[0];
            for (std::size_t j = 1; j < input_array0.size(); ++j) {
                input_array1[j] = random8u() & 0x1;
            }

            mask16x8u mask0{input_array0};
            mask16x8u mask1{input_array1};
            mask16x8u mask2{input_array1};

            EXPECT_TRUE(mask0 != mask1);
            EXPECT_FALSE(mask1 != mask2);
        }
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(Mask16x8u, Bitwise_and_assignment) {
        mask16x8u mask0{{
            true, true, false, false,
            true, true, false, false,
            true, true, false, false,
            true, true, false, false
        }};

        mask16x8u mask1{{
            true, false, true, false,
            true, false, true, false,
            true, false, true, false,
            true, false, true, false
        }};

        mask0 &= mask1;

        mask16x8u mask2{{
            true, false, false, false,
            true, false, false, false,
            true, false, false, false,
            true, false, false, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    TEST(Mask16x8u, Bitwise_or_assignment) {
        mask16x8u mask0{{
            true, true, false, false,
            true, true, false, false,
            true, true, false, false,
            true, true, false, false
        }};

        mask16x8u mask1{{
            true, false, true, false,
            true, false, true, false,
            true, false, true, false,
            true, false, true, false
        }};

        mask0 |= mask1;

        mask16x8u mask2{{
            true, true, true, false,
            true, true, true, false,
            true, true, true, false,
            true, true, true, false
        }};

        EXPECT_EQ(mask0, mask2);
    }

    TEST(Mask16x8u, Bitwise_xor_assignment) {
        mask16x8u mask0{{
            true, true, false, false,
            true, true, false, false,
            true, true, false, false,
            true, true, false, false
        }};

        mask16x8u mask1{{
            true, false, true, false,
            true, false, true, false,
            true, false, true, false,
            true, false, true, false
        }};

        mask0 ^= mask1;

        mask16x8u mask2{{
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

    TEST(Mask16x8u, Logical_negation) {
        mask16x8u mask0{{
            false, false, true,  true,
            false, false, true,  true,
            false, false, true,  true,
            false, false, true,  true
        }};

        mask16x8u mask1{{
            true,  true,  false, false,
            true,  true,  false, false,
            true,  true,  false, false,
            true,  true,  false, false
        }};

        EXPECT_EQ(mask0, !mask1);
    }

    TEST(Mask16x8u, Bitwise_and) {
        mask16x8u m =
            mask16x8u{{
                true, true, false, false,
                true, true, false, false,
                true, true, false, false,
                true, true, false, false
            }} &
            mask16x8u{{
                true, false, true, false,
                true, false, true, false,
                true, false, true, false,
                true, false, true, false
            }};

        mask16x8u mask2{{
            true, false, false, false,
            true, false, false, false,
            true, false, false, false,
            true, false, false, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask16x8u, Logical_and) {
        mask16x8u m =
            mask16x8u{{
                true, true, false, false,
                true, true, false, false,
                true, true, false, false,
                true, true, false, false
            }} &&
            mask16x8u{{
                true, false, true, false,
                true, false, true, false,
                true, false, true, false,
                true, false, true, false
            }};

        mask16x8u mask2{{
            true, false, false, false,
            true, false, false, false,
            true, false, false, false,
            true, false, false, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask16x8u, Bitwise_or) {
        mask16x8u m =
            mask16x8u{{
                true, true, false, false,
                true, true, false, false,
                true, true, false, false,
                true, true, false, false
            }} |
            mask16x8u{{
                true, false, true, false,
                true, false, true, false,
                true, false, true, false,
                true, false, true, false
            }};

        mask16x8u mask2{{
            true, true, true, false,
            true, true, true, false,
            true, true, true, false,
            true, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask16x8u, Logical_or) {
        mask16x8u m =
            mask16x8u{{
                true, true, false, false,
                true, true, false, false,
                true, true, false, false,
                true, true, false, false
            }} ||
            mask16x8u{{
                true, false, true, false,
                true, false, true, false,
                true, false, true, false,
                true, false, true, false
            }};

        mask16x8u mask2{{
            true, true, true, false,
            true, true, true, false,
            true, true, true, false,
            true, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    TEST(Mask16x8u, Bitwise_xor) {
        mask16x8u m =
            mask16x8u{{
                true, true, false, false,
                true, true, false, false,
                true, true, false, false,
                true, true, false, false
            }} ^
            mask16x8u{{
                true, false, true, false,
                true, false, true, false,
                true, false, true, false,
                true, false, true, false
            }};

        mask16x8u mask2{{
            false, true, true, false,
            false, true, true, false,
            false, true, true, false,
            false, true, true, false
        }};

        EXPECT_EQ(m, mask2);
    }

    //=========================================================================
    // Vec16x8u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec16x8u, Convert_from_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16xb data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random8u() & 0x1;
            }

            mask16x8u m0{data};
            vec16x8u v0{m0};

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

    TEST(Vec16x8u, Construct_from_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random8u();
            vec16x8u v{x};
            vec16x8u baseline{{
                x, x, x, x,
                x, x, x, x,
                x, x, x, x,
                x, x, x, x
            }};

            EXPECT_TRUE(all(v == baseline));
        }
    }

    TEST(Vec16x8u, Construct_from_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u data;

            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random8u();
            }

            vec16x8u v{data};

            auto results = to_array(v);

            for (std::size_t j = 0; j < data.size(); ++j) {
                EXPECT_EQ(data[j], results[j]);
            }
        }
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec16x8u, Assign_primitive) {
        vec16x8u::primitive x{};
        vec16x8u v{0x0F};
        v = x;
        vec16x8u baseline{};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(Vec16x8u, Assign_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t scalar = random8u();
            vec16x8u v;
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

    TEST(Vec16x8u, Equality_comparison) {
        mask16x8u mask0{false};
        mask16x8u mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask16x8u mask2{true};
        mask16x8u mask3{true};
        EXPECT_EQ(mask2, mask3);
    }

    TEST(Vec16x8u, Equality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u data;
            for (std::size_t j = 0; j < data.size(); ++j) {
                data[j] = random8u();
            }

            vec16x8u v0{data};
            vec16x8u v1{data};

            EXPECT_TRUE(all(v0 == v1));
        }
    }

    TEST(Vec16x8u, Inequality_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u data0;
            arr16x8u data1;
            for (std::size_t j = 0; j < data0.size(); ++j) {
                data0[j] = random8u();
                data1[j] = random8u() ;
            }

            vec16x8u v0{data0};
            vec16x8u v1{data1};

            if (!all(v0 == v1)) {
                EXPECT_TRUE(any(v0 != v1));
            }
        }
    }

    TEST(Vec16x8u, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = (input0 < input1);

            arr16xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] < input_array1[j];
            }

            EXPECT_EQ(results, mask16x8u{results_array});
        }
    }

    TEST(Vec16x8u, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = (input0 <= input1);

            arr16xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] <= input_array1[j];
            }

            EXPECT_EQ(results, mask16x8u{results_array});
        }
    }

    TEST(Vec16x8u, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = (input0 > input1);

            arr16xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] > input_array1[j];
            }

            EXPECT_EQ(results, mask16x8u{results_array});
        }
    }

    TEST(Vec16x8u, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = (input0 >= input1);

            arr16xb results_array{};
            for (std::size_t j = 0; j < results_array.size(); ++j) {
                results_array[j] = input_array0[j] >= input_array1[j];
            }

            EXPECT_EQ(results, mask16x8u{results_array});
        }
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(Vec16x8u, Unary_plus) {
        const vec16x8u u{{
            0x0, 0x1, 0x2, 0x3,
            0x4, 0x5, 0x6, 0x7,
            0x8, 0x9, 0xA, 0xB,
            0xC, 0xD, 0xE, 0xF
        }};
        vec16x8u v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(Vec16x8u, Unary_minus) {
        const vec16x8u u{{
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

    TEST(Vec16x8u, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};


            auto results = input0;
            results += input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};


            auto results = input0;
            results -= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};


            auto results = input0;
            results *= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = std::max(static_cast<std::uint8_t>(random8u()), static_cast<std::uint8_t>(1u));
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0;
            results /= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = std::max(static_cast<std::uint8_t>(random8u()), static_cast<std::uint8_t>(1u));
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0;
            results %= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec16x8u, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0 + input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0 - input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0 * input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = std::max(static_cast<std::uint8_t>(random8u()), static_cast<std::uint8_t>(1u));
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0 / input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = std::max(static_cast<std::uint8_t>(random8u()), static_cast<std::uint8_t>(1u));
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0 % input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec16x8u, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};
            ++input0;

            auto results = input0;
            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};
            input0++;

            auto results = input0;
            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};
            --input0;

            auto results = input0;
            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = +--input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};
            input0--;

            auto results = input0;
            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec16x8u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0;
            results &= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0;
            results |= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0;
            results ^= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            std::uint8_t input1 = random8u() % 9;

            vec16x8u input0{input_array0};

            auto results = input0;
            results <<= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u() % 9;
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0;
            results <<= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            std::uint8_t input1 = random8u() % 9;

            vec16x8u input0{input_array0};

            auto results = input0;
            results >>= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u() % 9;
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0;
            results >>= input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    //=====================================================
    // Bitwise operators
    //=====================================================

    TEST(Vec16x8u, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = ~input0;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ~input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Left_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            std::uint8_t input1 = random8u() % 9;

            vec16x8u input0{input_array0};

            auto results = input0 << input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input1;
                }
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Left_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u() % 9;
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0 << input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] << input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Right_shift_by_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            std::uint8_t input1 = random8u() % 9;

            vec16x8u input0{input_array0};

            auto results = input0 >> input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input1 >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input1;
                }
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Right_shift_by_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u() % 9;
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = input0 >> input1;

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array1[j] >= 8) {
                    results_array[j] = 0x00;
                } else {
                    results_array[j] = input_array0[j] >> input_array1[j];
                }
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec16x8u, Conversion_to_mask) {
        vec16x8u vec{0};

        auto a = vec.operator mask16x8u();
        mask16x8u b{false};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec16x8u, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = mask16x8u{input0};

            arr16xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(results == mask16x8u{results_array});
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec16x8u, Broadcast_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16xb input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u() & 0x1;
            }

            mask16x8u input0{input_array0};

            auto results = broadcast_mask(input0);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array0[j]) {
                    results_array[j] = 0xff;
                } else {
                    results_array[j] = 0x00;
                }
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16xb input_array0{};
            arr16x8u input_array1{};
            arr16x8u input_array2{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u() & 0x1;
                input_array1[j] = random8u();
                input_array2[j] = random8u();
            }

            mask16x8u input0{input_array0};
            vec16x8u input1{input_array1};
            vec16x8u input2{input_array2};

            auto results = blend(input0, input1, input2);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = blend(input_array0[j], input_array1[j], input_array2[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = max(input0, input1);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = min(input0, input1);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::min(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = minmax(input0, input1);

            arr16x8u results_array0{};
            arr16x8u results_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array0[j], input_array1[j]);
                results_array0[j] = xy[0];
                results_array1[j] = xy[1];
            }

            EXPECT_TRUE(all(results[0] == vec16x8u{results_array0}));
            EXPECT_TRUE(all(results[1] == vec16x8u{results_array1}));
        }
    }

    TEST(Vec16x8u, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};
            arr16x8u input_array2{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
                input_array2[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};
            vec16x8u input2{input_array2};

            auto xy = minmax(input1, input2);

            auto results = clamp(input0, xy[0], xy[1]);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array1[j], input_array2[j]);
                results_array[j] = clamp(input_array0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = midpoint(input0, input1);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = midpoint(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Neg_abs_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = neg_abs(input0);

            arr16x8i results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = neg_abs(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8i{results_array}));
        }
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    TEST(Vec16x8u, Load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array{};
            for (std::size_t j = 0; j < input_array.size(); ++j) {
                input_array[j] = random8u();
            }

            for (std::uint32_t j = 0; j <= vec16x8u::width; ++j) {
                auto results = load<vec16x8u>(input_array.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec16x8u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(input_array[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec16x8u, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            EXPECT_TRUE(all(load<vec16x8u, 0x00>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x00)));
            EXPECT_TRUE(all(load<vec16x8u, 0x01>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x01)));
            EXPECT_TRUE(all(load<vec16x8u, 0x02>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x02)));
            EXPECT_TRUE(all(load<vec16x8u, 0x03>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x03)));
            EXPECT_TRUE(all(load<vec16x8u, 0x04>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x04)));
            EXPECT_TRUE(all(load<vec16x8u, 0x05>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x05)));
            EXPECT_TRUE(all(load<vec16x8u, 0x06>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x06)));
            EXPECT_TRUE(all(load<vec16x8u, 0x07>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x07)));
            EXPECT_TRUE(all(load<vec16x8u, 0x08>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x08)));
            EXPECT_TRUE(all(load<vec16x8u, 0x09>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x09)));
            EXPECT_TRUE(all(load<vec16x8u, 0x0a>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x0a)));
            EXPECT_TRUE(all(load<vec16x8u, 0x0b>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x0b)));
            EXPECT_TRUE(all(load<vec16x8u, 0x0c>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x0c)));
            EXPECT_TRUE(all(load<vec16x8u, 0x0d>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x0d)));
            EXPECT_TRUE(all(load<vec16x8u, 0x0e>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x0e)));
            EXPECT_TRUE(all(load<vec16x8u, 0x0f>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x0f)));
            EXPECT_TRUE(all(load<vec16x8u, 0x10>(input_array0.data()) == load<vec16x8u>(input_array0.data(), 0x10)));
        }
    }

    TEST(Vec16x8u, Aligned_load_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec16x8u)) arr16x8u input_array{};
            for (std::size_t j = 0; j < input_array.size(); ++j) {
                input_array[j] = random8u();
            }

            for (std::uint32_t j = 0; j <= vec16x8u::width; ++j) {
                auto results = aligned_load<vec16x8u>(input_array.data(), j);

                auto loaded_data = to_array(results);
                for (std::uint32_t k = 0; k < vec16x8u::width; k++) {
                    if (k < j) {
                        EXPECT_EQ(input_array[k], loaded_data[k]);
                    } else {
                        EXPECT_EQ(0x0, loaded_data[k]);
                    }
                }
            }
        }
    }

    TEST(Vec16x8u, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec16x8u)) arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x00>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x01>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x01)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x02>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x02)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x03>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x03)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x04>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x04)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x05>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x05)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x06>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x06)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x07>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x07)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x08>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x08)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x00>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x00)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x0a>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x0a)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x0b>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x0b)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x0c>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x0c)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x0d>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x0d)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x0e>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x0e)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x0f>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x0f)));
            EXPECT_TRUE(all(aligned_load<vec16x8u, 0x10>(input_array0.data()) == aligned_load<vec16x8u>(input_array0.data(), 0x10)));
        }
    }

    TEST(Vec16x8u, Store_n) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            for (std::size_t j = 0; j <= vec16x8u::width; ++j) {
                std::uint8_t arr[16]{};
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

    TEST(Vec16x8u, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            std::uint8_t arr[16]{};

            store<0x00>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x00));
            store<0x01>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x01));
            store<0x02>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x02));
            store<0x03>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x03));
            store<0x04>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x04));
            store<0x05>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x05));
            store<0x06>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x06));
            store<0x07>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x07));
            store<0x08>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x08));
            store<0x09>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x09));
            store<0x0a>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0a));
            store<0x0b>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0b));
            store<0x0c>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0c));
            store<0x0d>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0d));
            store<0x0e>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0e));
            store<0x0f>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0f));
            store<0x10>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x10));
        }
    }

    TEST(Vec16x8u, Aligned_store_n_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            for (std::size_t j = 0; j <= vec16x8u::width; ++j) {
                alignas(alignof(vec16x8u)) std::uint8_t arr[16]{};
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

    TEST(Vec16x8u, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            alignas(alignof(vec16x8u)) std::uint8_t arr[16]{};

            aligned_store<0x00>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x00));
            aligned_store<0x01>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x01));
            aligned_store<0x02>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x02));
            aligned_store<0x03>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x03));
            aligned_store<0x04>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x04));
            aligned_store<0x05>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x05));
            aligned_store<0x06>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x06));
            aligned_store<0x07>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x07));
            aligned_store<0x08>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x08));
            aligned_store<0x09>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x09));
            aligned_store<0x0a>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0a));
            aligned_store<0x0b>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0b));
            aligned_store<0x0c>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0c));
            aligned_store<0x0d>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0d));
            aligned_store<0x0e>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0e));
            aligned_store<0x0f>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x0f));
            aligned_store<0x10>(arr, input0); EXPECT_TRUE(compare_stored_data(arr, input0, 0x10));
        }
    }

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec16x8u, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            arr16x8u numerators{};
            for (std::size_t j = 0; j < numerators.size(); ++j) {
                numerators[j] = random8u();
            }

            arr16x8u denominators{};
            for (std::size_t j = 0; j < denominators.size(); ++j) {
                denominators[j] = std::max(static_cast<std::uint8_t>(random8u()), static_cast<std::uint8_t>(1u));
            }

            arr16x8u quotients{};
            arr16x8u remainders{};

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec16x8u n{numerators};
            vec16x8u d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < quotients.size(); ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);
            }
        }
    }

    TEST(Vec16x8u, Popcount) {
        vec16x8u v{0x0};
        vec16x8u c = popcount(v);
        EXPECT_TRUE(all(c == v));
    }

    TEST(Vec16x8u, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = popcount(input0);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = popcount(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};


            auto results = byteswap(input0);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = countl_zero(input0);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = countl_one(input0);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = countr_zero(input0);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = countr_one(input0);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Bit_width_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = bit_width(input0);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_width(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Bit_floor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = bit_floor(input0);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_floor(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Bit_ceil_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = bit_ceil(input0);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_ceil(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Has_single_bit) {
        for (std::size_t i = 0; i < 8; ++i) {
            EXPECT_TRUE(all(has_single_bit(vec16x8u{static_cast<std::uint8_t>(1 << i)})));
        }
    }

    TEST(Vec16x8u, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = has_single_bit(input0);

            arr16xb results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = has_single_bit(input_array0[j]);
            }

            EXPECT_TRUE(results == mask16x8u{results_array});
        }
    }

    //=====================================================
    // Bit Manipulation Instructions
    //=====================================================

    TEST(Vec16x8u, Bit_shift_left) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            EXPECT_TRUE(all(bit_shift_left<0x0>(input0) == (input0 << 0x0)));
            EXPECT_TRUE(all(bit_shift_left<0x1>(input0) == (input0 << 0x1)));
            EXPECT_TRUE(all(bit_shift_left<0x2>(input0) == (input0 << 0x2)));
            EXPECT_TRUE(all(bit_shift_left<0x3>(input0) == (input0 << 0x3)));
            EXPECT_TRUE(all(bit_shift_left<0x4>(input0) == (input0 << 0x4)));
            EXPECT_TRUE(all(bit_shift_left<0x5>(input0) == (input0 << 0x5)));
            EXPECT_TRUE(all(bit_shift_left<0x6>(input0) == (input0 << 0x6)));
            EXPECT_TRUE(all(bit_shift_left<0x7>(input0) == (input0 << 0x7)));
            EXPECT_TRUE(all(bit_shift_left<0x8>(input0) == vec16x8u{0x00}));
        }
    }

    TEST(Vec16x8u, Bit_shift_right) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            EXPECT_TRUE(all(bit_shift_right<0x0>(input0) == (input0 >> 0x0)));
            EXPECT_TRUE(all(bit_shift_right<0x1>(input0) == (input0 >> 0x1)));
            EXPECT_TRUE(all(bit_shift_right<0x2>(input0) == (input0 >> 0x2)));
            EXPECT_TRUE(all(bit_shift_right<0x3>(input0) == (input0 >> 0x3)));
            EXPECT_TRUE(all(bit_shift_right<0x4>(input0) == (input0 >> 0x4)));
            EXPECT_TRUE(all(bit_shift_right<0x5>(input0) == (input0 >> 0x5)));
            EXPECT_TRUE(all(bit_shift_right<0x6>(input0) == (input0 >> 0x6)));
            EXPECT_TRUE(all(bit_shift_right<0x7>(input0) == (input0 >> 0x7)));
            EXPECT_TRUE(all(bit_shift_right<0x8>(input0) == vec16x8u{0x00}));
        }
    }

    TEST(Vec16x8u, Rotl) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            EXPECT_TRUE(all(rotl<0x0>(input0) == (rotl(input0, 0x0))));
            EXPECT_TRUE(all(rotl<0x1>(input0) == (rotl(input0, 0x1))));
            EXPECT_TRUE(all(rotl<0x2>(input0) == (rotl(input0, 0x2))));
            EXPECT_TRUE(all(rotl<0x3>(input0) == (rotl(input0, 0x3))));
            EXPECT_TRUE(all(rotl<0x4>(input0) == (rotl(input0, 0x4))));
            EXPECT_TRUE(all(rotl<0x5>(input0) == (rotl(input0, 0x5))));
            EXPECT_TRUE(all(rotl<0x6>(input0) == (rotl(input0, 0x6))));
            EXPECT_TRUE(all(rotl<0x7>(input0) == (rotl(input0, 0x7))));
        }
    }

    TEST(Vec16x8u, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            long long input1 = random64u();

            vec16x8u input0{input_array0};

            auto results = rotl(input0, input1);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotl(input_array0[j], input1);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            arr16x8u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = rotl(input0, input1);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotl(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Rotr) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            EXPECT_TRUE(all(rotr<0x0>(input0) == (rotr(input0, 0x0))));
            EXPECT_TRUE(all(rotr<0x1>(input0) == (rotr(input0, 0x1))));
            EXPECT_TRUE(all(rotr<0x2>(input0) == (rotr(input0, 0x2))));
            EXPECT_TRUE(all(rotr<0x3>(input0) == (rotr(input0, 0x3))));
            EXPECT_TRUE(all(rotr<0x4>(input0) == (rotr(input0, 0x4))));
            EXPECT_TRUE(all(rotr<0x5>(input0) == (rotr(input0, 0x5))));
            EXPECT_TRUE(all(rotr<0x6>(input0) == (rotr(input0, 0x6))));
            EXPECT_TRUE(all(rotr<0x7>(input0) == (rotr(input0, 0x7))));
        }
    }

    TEST(Vec16x8u, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            long long input1 = random64u();

            vec16x8u input0{input_array0};

            auto results = rotr(input0, input1);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotr(input_array0[j], input1);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    TEST(Vec16x8u, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            arr16x8u input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random8u();
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};

            auto results = rotr(input0, input1);

            arr16x8u results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotr(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec16x8u{results_array}));
        }
    }

    //=====================================================
    // Conversions
    //=====================================================

    TEST(Vec16x8u, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = to_array(input0);

            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                EXPECT_EQ(results[j], input_array0[j]);
            }
        }
    }

    TEST(Vec16x8u, Convert_to_vec16x8u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto results = convert<vec16x8u>(input0)[0];

            EXPECT_TRUE(all(input0 == results));
        }
    }

    /*
    TEST(Vec16x8u, Convert_to_vec1x8u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto outputs = convert<vec1x8u>(input0);

            for (std::size_t j = 0; j < outputs.size(); ++j) {
                std::uint8_t output = decay(outputs[j]);
                std::uint8_t baseline = input_array0[j];
                EXPECT_EQ(output, baseline);
            }
        }
    }

    TEST(Vec16x8u, Convert_to_vec1x8i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto outputs = convert<vec1x8i>(input0);

            for (std::size_t j = 0; j < outputs.size(); ++j) {
                std::int8_t output = decay(outputs[j]);
                std::int8_t baseline = input_array0[j];
                EXPECT_EQ(output, baseline);
            }
        }
    }

    TEST(Vec16x8u, Convert_to_vec1x16u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto outputs = convert<vec1x16u>(input0);

            for (std::size_t j = 0; j < outputs.size(); ++j) {
                std::uint16_t output = decay(outputs[j]);
                std::uint16_t baseline = input_array0[j];
                EXPECT_EQ(output, baseline);
            }
        }
    }

    TEST(Vec16x8u, Convert_to_vec1x16i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto outputs = convert<vec1x16i>(input0);

            for (std::size_t j = 0; j < outputs.size(); ++j) {
                std::int16_t output = decay(outputs[j]);
                std::int16_t baseline = input_array0[j];
                EXPECT_EQ(output, baseline);
            }
        }
    }

    TEST(Vec16x8u, Convert_to_vec1x32u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto outputs = convert<vec1x32u>(input0);

            for (std::size_t j = 0; j < outputs.size(); ++j) {
                std::uint32_t output = decay(outputs[j]);
                std::uint32_t baseline = input_array0[j];
                EXPECT_EQ(output, baseline);
            }
        }
    }

    TEST(Vec16x8u, Convert_to_vec1x32i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto outputs = convert<vec1x32i>(input0);

            for (std::size_t j = 0; j < outputs.size(); ++j) {
                std::int32_t output = decay(outputs[j]);
                std::int32_t baseline = input_array0[j];
                EXPECT_EQ(output, baseline);
            }
        }
    }

    TEST(Vec16x8u, Convert_to_vec1x64u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto outputs = convert<vec1x64u>(input0);

            for (std::size_t j = 0; j < outputs.size(); ++j) {
                std::uint64_t output = decay(outputs[j]);
                std::uint64_t baseline = input_array0[j];
                EXPECT_EQ(output, baseline);
            }
        }
    }

    TEST(Vec16x8u, Convert_to_vec1x64i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto outputs = convert<vec1x64i>(input0);

            for (std::size_t j = 0; j < outputs.size(); ++j) {
                std::int64_t output = decay(outputs[j]);
                std::int64_t baseline = input_array0[j];
                EXPECT_EQ(output, baseline);
            }
        }
    }

    TEST(Vec16x8u, Convert_to_vec1x32f) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto outputs = convert<vec1x32f>(input0);

            for (std::size_t j = 0; j < outputs.size(); ++j) {
                float output = decay(outputs[j]);
                float baseline = static_cast<float>(input_array0[j]);
                EXPECT_EQ(output, baseline);
            }
        }
    }

    TEST(Vec16x8u, Convert_to_vec1x64f) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto outputs = convert<vec1x64f>(input0);

            for (std::size_t j = 0; j < outputs.size(); ++j) {
                double output = decay(outputs[j]);
                double baseline = static_cast<double>(input_array0[j]);
                EXPECT_EQ(output, baseline);
            }
        }
    }

    TEST(Vec16x8u, Convert_from_vec1x8u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            vec1x8u input0{static_cast<std::uint8_t>(random8u())};

            auto outputs = convert<vec16x8u>(input0);

            auto output = to_array(outputs[0]);

            EXPECT_EQ(output[0], decay(input0));
            for (std::size_t j = 1; j < vec16x8u::width; ++j) {
                EXPECT_EQ(output[j], 0x00);
            }
        }
    }

    TEST(Vec16x8u, Convert_from_vec1x8i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            vec1x8i input0{static_cast<std::int8_t>(random8u())};

            auto outputs = convert<vec16x8u>(input0);

            auto output = to_array(outputs[0]);

            EXPECT_TRUE(avel_impl::compare_common_bytes(output[0], decay(input0)));
            for (std::size_t j = 1; j < vec16x8u::width; ++j) {
                EXPECT_EQ(output[j], 0x00);
            }
        }
    }

    TEST(Vec16x8u, Convert_from_vec1x16u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            vec1x16u input0{static_cast<std::uint16_t>(random16u())};

            auto outputs = convert<vec16x8u>(input0);

            auto output = to_array(outputs[0]);

            EXPECT_TRUE(avel_impl::compare_common_bytes(output[0], decay(input0)));
            for (std::size_t j = 1; j < vec16x8u::width; ++j) {
                EXPECT_EQ(output[j], 0x00);
            }
        }
    }

    TEST(Vec16x8u, Convert_from_vec1x16i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            vec1x16i input0{static_cast<std::int16_t>(random16u())};

            auto outputs = convert<vec16x8u>(input0);

            auto output = to_array(outputs[0]);

            EXPECT_TRUE(avel_impl::compare_common_bytes(output[0], decay(input0)));
            for (std::size_t j = 1; j < vec16x8u::width; ++j) {
                EXPECT_EQ(output[j], 0x00);
            }
        }
    }

    TEST(Vec16x8u, Convert_from_vec1x32u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            vec1x32u input0{static_cast<std::uint32_t>(random32u())};

            auto outputs = convert<vec16x8u>(input0);

            auto output = to_array(outputs[0]);

            EXPECT_TRUE(avel_impl::compare_common_bytes(output[0], decay(input0)));
            for (std::size_t j = 1; j < vec16x8u::width; ++j) {
                EXPECT_EQ(output[j], 0x00);
            }
        }
    }

    TEST(Vec16x8u, Convert_from_vec1x32i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            vec1x32i input0{static_cast<std::int32_t>(random32u())};

            auto outputs = convert<vec16x8u>(input0);

            auto output = to_array(outputs[0]);

            EXPECT_TRUE(avel_impl::compare_common_bytes(output[0], decay(input0)));
            for (std::size_t j = 1; j < vec16x8u::width; ++j) {
                EXPECT_EQ(output[j], 0x00);
            }
        }
    }

    TEST(Vec16x8u, Convert_from_vec1x64u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            vec1x64u input0{static_cast<std::uint64_t>(random64u())};

            auto outputs = convert<vec16x8u>(input0);

            auto output = to_array(outputs[0]);

            EXPECT_TRUE(avel_impl::compare_common_bytes(output[0], decay(input0)));
            for (std::size_t j = 1; j < vec16x8u::width; ++j) {
                EXPECT_EQ(output[j], 0x00);
            }
        }
    }

    TEST(Vec16x8u, Convert_from_vec1x64i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            vec1x64i input0{static_cast<std::int64_t>(random64u())};

            auto outputs = convert<vec16x8u>(input0);

            auto output = to_array(outputs[0]);

            EXPECT_TRUE(avel_impl::compare_common_bytes(output[0], decay(input0)));
            for (std::size_t j = 1; j < vec16x8u::width; ++j) {
                EXPECT_EQ(output[j], 0x00);
            }
        }
    }
    */

    //TODO: Add tests for conversions from floating-point types

}

#endif //AVEL_VEC16X8U_TESTS_HPP
