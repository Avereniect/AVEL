#ifndef AVEL_VEC4X32U_TESTS_HPP
#define AVEL_VEC4X32U_TESTS_HPP

#include <gtest/gtest.h>

#include <cfenv>

namespace avel_tests {

    using namespace avel;

    //=========================================================================
    // mask4x32u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(mask4x32u, Construct_from_primitive) {
        mask4x32u mask{mask4x32u::primitive{}};

        EXPECT_FALSE(any(mask));
        EXPECT_FALSE(all(mask));
        EXPECT_TRUE(none(mask));
        EXPECT_EQ(count(mask), 0);
    }

    TEST(mask4x32u, Construct_from_bool) {
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

    TEST(mask4x32u, Construct_from_array) {
        mask4x32u mask0{{false, false, false, false}};
        EXPECT_FALSE(any(mask0));
        EXPECT_FALSE(all(mask0));
        EXPECT_TRUE(none(mask0));
        EXPECT_EQ(count(mask0), 0);

        mask4x32u mask1{{true, true, true, true}};
        EXPECT_TRUE(any(mask1));
        EXPECT_TRUE(all(mask1));
        EXPECT_FALSE(none(mask1));
        EXPECT_EQ(count(mask1), 4);

        mask4x32u mask2{{true, true, false, false}};
        EXPECT_TRUE(any(mask2));
        EXPECT_FALSE(all(mask2));
        EXPECT_FALSE(none(mask2));
        EXPECT_EQ(count(mask2), 2);

        mask4x32u mask3{{true, false, true, false}};
        EXPECT_TRUE(any(mask3));
        EXPECT_FALSE(all(mask3));
        EXPECT_FALSE(none(mask2));
        EXPECT_EQ(count(mask3), 2);
    }

    //=====================================================
    // Assignment
    //=====================================================

    TEST(mask4x32u, Assign_bool) {
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

    TEST(mask4x32u, Equality_comparison) {
        mask4x32u mask0{false};
        mask4x32u mask1{false};
        EXPECT_EQ(mask0, mask1);

        mask4x32u mask2{true};
        mask4x32u mask3{true};
        EXPECT_EQ(mask2, mask3);

        mask4x32u mask4{{false, false, true, true}};
        mask4x32u mask5{{false, false, true, true}};

        EXPECT_EQ(mask4, mask5);

        mask4x32u mask6{{true, false, true, true}};
        mask4x32u mask7{{false, true, false, true}};

        EXPECT_TRUE(!(mask6 == mask7));
    }

    TEST(mask4x32u, Inequality_comparison) {
        mask4x32u mask0{false};
        mask4x32u mask1{true};
        EXPECT_NE(mask0, mask1);
        EXPECT_NE(mask1, mask0);

        mask4x32u mask4{{false, false, true, true}};
        mask4x32u mask5{{false, false, true, true}};
        EXPECT_TRUE(!(mask4 != mask5));

        mask4x32u mask6{{true, false, true, true}};
        mask4x32u mask7{{false, true, false, true}};

        EXPECT_NE(mask6, mask7);
    }

    //=====================================================
    // Bitwise assignment
    //=====================================================

    TEST(mask4x32u, Bitwise_and_assignment) {
        mask4x32u mask0{{true, true, false, false}};
        mask4x32u mask1{{true, false, true, false}};

        mask0 &= mask1;

        mask4x32u mask2{{true, false, false, false}};

        EXPECT_EQ(mask0, mask2);
    }

    TEST(mask4x32u, Bitwise_or_assignment) {
        mask4x32u mask0{{true, true, false, false}};
        mask4x32u mask1{{true, false, true, false}};

        mask0 |= mask1;

        mask4x32u mask2{{true, true, true, false}};

        EXPECT_EQ(mask0, mask2);
    }

    TEST(mask4x32u, Bitwise_xor_assignment) {
        mask4x32u mask0{{true, true, false, false}};
        mask4x32u mask1{{true, false, true, false}};

        mask0 ^= mask1;

        mask4x32u mask2{{false, true, true, false}};

        EXPECT_EQ(mask0, mask2);
    }

    //=====================================================
    // Bitwise/logical operations
    //=====================================================

    TEST(mask4x32u, Logical_negation) {
        mask4x32u mask0{{false, false, true, true}};
        mask4x32u mask1{{true, true, false, false}};

        EXPECT_EQ(mask0, !mask1);
    }

    TEST(mask4x32u, Bitwise_and) {
        mask4x32u m =
            mask4x32u{{true, true, false, false}} &
            mask4x32u{{true, false, true, false}};

        mask4x32u mask2{{true, false, false, false}};
    }

    TEST(mask4x32u, Logical_and) {
        mask4x32u m =
            mask4x32u{{true, true, false, false}} &&
            mask4x32u{{true, false, true, false}};

        mask4x32u mask2{{true, false, false, false}};
    }

    TEST(mask4x32u, Bitwise_or) {
        mask4x32u m =
            mask4x32u{{true, true, false, false}} |
            mask4x32u{{true, false, true, false}};

        mask4x32u mask2{{true, true, true, false}};
    }

    TEST(mask4x32u, Logical_or) {
        mask4x32u m =
            mask4x32u{{true, true, false, false}} ||
            mask4x32u{{true, false, true, false}};

        mask4x32u mask2{{true, true, true, false}};
    }

    TEST(mask4x32u, Bitwise_xor) {
        mask4x32u m =
            mask4x32u{{true, true, false, false}} ^
            mask4x32u{{true, false, true, false}};

        mask4x32u mask2{{false, true, true, false}};
    }

    //=========================================================================
    // vec4x32u tests
    //=========================================================================

    //=====================================================
    // Constructors
    //=====================================================

    TEST(Vec4x32u, Construct_from_mask) {
        mask4x32u m_false{false};
        mask4x32u m_true{true};

        vec4x32u v_f{m_false};
        vec4x32u baseline_f{{0, 0, 0, 0}};

        vec4x32u v_t{m_true};
        vec4x32u baseline_t{{1, 1, 1, 1}};

        EXPECT_TRUE(all(v_f == baseline_f));
        EXPECT_TRUE(all(v_t == baseline_t));

        mask4x32u m0{{false, true, false, true}};
        vec4x32u baseline0{{0, 1, 0, 1}};
        vec4x32u v0{m0};

        EXPECT_TRUE(all(v0 == baseline0));
    }

    TEST(Vec4x32u, Construct_from_scalar) {
        std::uint32_t x = 0xF0F0F0F0;
        vec4x32u v{x};
        vec4x32u baseline{{x, x, x, x}};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(Vec4x32u, Construct_from_array) {
        std::array<std::uint32_t, 4> in{
            0xF0F0F0F0,
            0xE0E0E0E0,
            0xD0D0D0D0,
            0xC0C0C0C0
        };
        vec4x32u v{in};

        auto out = to_array(v);
        EXPECT_EQ(out[0], in[0]);
        EXPECT_EQ(out[1], in[1]);
        EXPECT_EQ(out[2], in[2]);
        EXPECT_EQ(out[3], in[3]);
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(Vec4x32u, Assign_primitive) {
        vec4x32u::primitive x{};
        vec4x32u v{0x0FF};
        v = x;
        vec4x32u baseline{};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(Vec4x32u, Assign_scalar) {
        std::uint32_t x = 0xF0F0F0F0;
        vec4x32u v{};
        v = x;
        vec4x32u baseline{{x, x, x, x}};

        EXPECT_TRUE(all(v == baseline));
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(Vec4x32u, Equality_comparison) {
        const vec4x32u u{{1, 1, 1, 1}};
        const vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{false, true, false, false}};
        mask4x32u m1 = (u == v);

        EXPECT_EQ(m0, m1);
    }

    TEST(Vec4x32u, Inequality_comparison) {
        const vec4x32u u{{1, 1, 1, 1}};
        const vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{true, false, true, true}};
        mask4x32u m1 = (u != v);

        EXPECT_EQ(m0, m1);
    }

    TEST(Vec4x32u, Less_than_comparison) {
        const vec4x32u u{{1, 1, 1, 1}};
        const vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{false, false, true, true}};
        mask4x32u m1 = (u < v);

        EXPECT_EQ(m0, m1);
    }

    TEST(Vec4x32u, Less_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = (input0 < input1);

            std::array<bool, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] < input_array1[j];
            }

            EXPECT_EQ(results, mask4x32u{results_array});
        }
    }

    TEST(Vec4x32u, Less_than_or_equal_comparison) {
        const vec4x32u u{{1, 1, 1, 1}};
        const vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{false, true, true, true}};
        mask4x32u m1 = (u <= v);

        EXPECT_EQ(m0, m1);
    }

    TEST(Vec4x32u, Less_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = (input0 <= input1);

            std::array<bool, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] <= input_array1[j];
            }

            EXPECT_EQ(results, mask4x32u{results_array});
        }
    }

    TEST(Vec4x32u, Greater_than_comparison) {
        const vec4x32u u{{1, 1, 1, 1}};
        const vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{true, false, false, false}};
        mask4x32u m1 = (u > v);

        EXPECT_EQ(m0, m1);
    }

    TEST(Vec4x32u, Greater_than_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = (input0 > input1);

            std::array<bool, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] > input_array1[j];
            }

            EXPECT_EQ(results, mask4x32u{results_array});
        }
    }

    TEST(Vec4x32u, Greater_than_or_equal_comparison) {
        const vec4x32u u{{1, 1, 1, 1}};
        const vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{true, true, false, false}};
        mask4x32u m1 = (u >= v);

        EXPECT_EQ(m0, m1);
    }

    TEST(Vec4x32u, Greater_than_or_equal_comparison_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = (input0 >= input1);

            std::array<bool, 4> results_array{};
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

    TEST(Vec4x32u, Plus_assignment) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        u += v;
        vec4x32u x{{4, 6, 8, 10}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(Vec4x32u, Plus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0;
            results += input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Minus_assignment) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        v -= u;
        vec4x32u x{{4, 4, 4, 4}};

        EXPECT_TRUE(all(v == x));
    }

    TEST(Vec4x32u, Minus_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0;
            results -= input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Times_assignment) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        u *= v;
        vec4x32u x{{0, 5, 12, 21}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(Vec4x32u, Times_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0;
            results *= input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Div_assignment) {
        vec4x32u u{{15, 16, 17, 18}};
        vec4x32u v{{3, 4, 17, 3}};

        u /= v;
        vec4x32u x{{5, 4, 1, 6}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(Vec4x32u, Div_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = std::max(static_cast<std::uint32_t>(random32u()), static_cast<std::uint32_t>(1u));
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0;
            results /= input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Mod_assignment) {
        vec4x32u u{{18, 17, 16, 15}};
        vec4x32u v{{5, 4, 17, 3}};

        u %= v;
        vec4x32u x{{3, 1, 16, 0}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(Vec4x32u, Mod_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = std::max(static_cast<std::uint32_t>(random32u()), static_cast<std::uint32_t>(1u));
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0;
            results %= input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(Vec4x32u, Addition) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        u = u + v;
        vec4x32u x{{4, 6, 8, 10}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(Vec4x32u, Addition_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0 + input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] + input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Subtraction) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        v = v - u;
        vec4x32u x{{4, 4, 4, 4}};

        EXPECT_TRUE(all(v == x));
    }

    TEST(Vec4x32u, Subtraction_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0 - input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] - input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Multiplication) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        u = u * v;
        vec4x32u x{{0, 5, 12, 21}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(Vec4x32u, Multiplication_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0 * input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] * input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Division) {
        vec4x32u u{{15, 16, 17, 18}};
        vec4x32u v{{3, 4, 17, 3}};

        u = u / v;
        vec4x32u x{{5, 4, 1, 6}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(Vec4x32u, Division_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0 / input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] / input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Mod) {
        vec4x32u u{{18, 17, 16, 15}};
        vec4x32u v{{5, 4, 17, 3}};

        u = u % v;
        vec4x32u x{{3, 1, 16, 0}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(Vec4x32u, Mod_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0 % input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] % input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    TEST(Vec4x32u, Pre_increment) {
        vec4x32u vec{{3345, 5867, 3457, 5689}};
        ++vec;
        vec4x32u baselines{{3346, 5868, 3458, 5690}};

        EXPECT_TRUE(all(vec == baselines));
    }

    TEST(Vec4x32u, Pre_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};
            ++input0;

            auto results = input0;
            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Post_increment) {
        vec4x32u vec{{3345, 5867, 3457, 5689}};
        vec++;
        vec4x32u baselines{{3346, 5868, 3458, 5690}};

        EXPECT_TRUE(all(vec == baselines));
    }

    TEST(Vec4x32u, Post_increment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};
            input0++;

            auto results = input0;
            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ++input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Pre_decrement) {
        vec4x32u vec{{3345, 5867, 3457, 5689}};
        --vec;
        vec4x32u baselines{{3344, 5866, 3456, 5688}};

        EXPECT_TRUE(all(vec == baselines));
    }

    TEST(Vec4x32u, Pre_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};
            --input0;

            auto results = input0;
            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Post_decrement) {
        vec4x32u vec{{3345, 5867, 3457, 5689}};
        vec--;
        vec4x32u baselines{{3344, 5866, 3456, 5688}};

        EXPECT_TRUE(all(vec == baselines));
    }

    TEST(Vec4x32u, Post_decrement_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};
            input0--;

            auto results = input0;
            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = --input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    TEST(Vec4x32u, Bitwise_and_assignment) {
        vec4x32u v0{{0x0000FFFF, 0xFFFFFF00, 0xFF0000FF, 0xFF00FF00}};
        v0 &= vec4x32u{{0x0000FF00, 0xFF00FF00, 0x00FF00FF, 0x0000FFFF}};

        vec4x32u b{{0x0000FF00, 0xFF00FF00, 0x000000FF, 0x0000FF00}};

        EXPECT_TRUE(all(v0 == b));
    }

    TEST(Vec4x32u, Bitwise_and_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0;
            results &= input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] & input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bitwise_or_assignment) {
        vec4x32u v0{{0x0000FFFF, 0xFFFFFF00, 0xFF0000FF, 0xFF00FF00}};
        v0 |= vec4x32u{{0x0000FF00, 0xFF00FF00, 0x00FF00FF, 0x0000FFFF}};

        vec4x32u b{{0x0000FFFF, 0xFFFFFF00, 0xFFFF00FF, 0xFF00FFFF}};

        EXPECT_TRUE(all(v0 == b));
    }

    TEST(Vec4x32u, Bitwise_or_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0;
            results |= input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] | input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bitwise_xor_assignment) {
        vec4x32u v0{{0x0000FFFF, 0xFFFFFF00, 0xFF0000FF, 0xFF00FF00}};
        v0 ^= vec4x32u{{0x0000FF00, 0xFF00FF00, 0x00FF00FF, 0x0000FFFF}};

        vec4x32u b{{0x000000FF, 0x00FF0000, 0xFFFF0000, 0xFF0000FF}};

        EXPECT_TRUE(all(v0 == b));
    }

    TEST(Vec4x32u, Bitwise_xor_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0;
            results ^= input1;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j] ^ input_array1[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Left_shift_by_scalar_assignment) {
        vec4x32u v0{{0x00550011u, 0x00334455u, 0x12345678u, 0x00000012u}};
        v0 <<= 8;
        vec4x32u b0{{0x55001100u, 0x33445500u, 0x34567800u, 0x00001200u}};

        EXPECT_TRUE(all(v0 == b0));


        vec4x32u v1{{0x00550011u, 0x00334455u, 0x12345678u, 0x00000012u}};
        v1 <<= 31;
        vec4x32u b1{{0x80000000u, 0x80000000u, 0x00000000u, 0x00000000u}};

        EXPECT_TRUE(all(v1 == b1));


        vec4x32u v2{{0x00550011u, 0x00334455u, 0x12345678u, 0x00000012u}};
        v2 <<= 32;
        vec4x32u b2{{0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u}};

        EXPECT_TRUE(all(v2 == b2));


        vec4x32u v3{{0x00550011u, 0x00334455u, 0x12345678u, 0x00000012u}};
        v3 <<= 33;
        vec4x32u b3{{0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u}};

        EXPECT_TRUE(all(v3 == b3));
    }

    TEST(Vec4x32u, Left_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::uint32_t input1 = random32u() % 64;

            vec4x32u input0{input_array0};


            auto results = input0;
            results <<= input1;

            std::array<std::uint32_t, 4> results_array{};
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

    TEST(Vec4x32u, Left_shift_by_vector_assignment) {
        vec4x32u v{{0x0000000Fu, 0x0000000Fu, 0x0000000Fu, 0x0000000Fu}};
        vec4x32u s{{0x00000000u, 0x00000001u, 0x00000002u, 0x00000003u}};

        v <<= s;

        vec4x32u b{{
            0x0000000Fu << 0x00000000u,
            0x0000000Fu << 0x00000001u,
            0x0000000Fu << 0x00000002u,
            0x0000000Fu << 0x00000003u
        }};

        EXPECT_TRUE(all(v == b));
    }

    TEST(Vec4x32u, Left_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u() % 64;
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = input0;
            results <<= input1;

            std::array<std::uint32_t, 4> results_array{};
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

    TEST(Vec4x32u, Right_shift_by_scalar_assignment) {
        vec4x32u v{{0x00550011u, 0x00334455u, 0x12345678u, 0x00000012u}};
        v >>= 8;

        vec4x32u b{{0x00005500u, 0x00003344u, 0x00123456u, 0x00000000u}};

        EXPECT_TRUE(all(v == b));
    }

    TEST(Vec4x32u, Right_shift_by_scalar_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::uint32_t input1 = random32u() % 64;

            vec4x32u input0{input_array0};


            auto results = input0;
            results >>= input1;

            std::array<std::uint32_t, 4> results_array{};
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

    TEST(Vec4x32u, Right_shift_by_vector_assignment) {
        vec4x32u v{{0x87654321u, 0xFFFFFFFFu, 0x12345678u, 0x0000000Fu}};
        vec4x32u s{{0x00000000u, 0x0000001fu, 0x00000020u, 0x00000021u}};

        v >>= s;

        vec4x32u b{{
            0x87654321u >> 0x00000000u,
            0xFFFFFFFFu >> 0x0000001fu,
            0x000000000,
            0x00000000u
        }};

        EXPECT_TRUE(all(v == b));
    }

    TEST(Vec4x32u, Right_shift_by_vector_assignment_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u() % 64;
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            auto results = input0;
            results >>= input1;

            std::array<std::uint32_t, 4> results_array{};
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

    TEST(Vec4x32u, Bitwise_negation) {
        vec4x32u v{{0x87654321u, 0xFFFFFFFFu, 0x12345678u, 0x0000000Fu}};
        vec4x32u b{{0x789abcdeu, 0x00000000u, 0xEDCBA987u, 0xFFFFFFF0u}};

        EXPECT_TRUE(all(~v == b));
    }

    TEST(Vec4x32u, Bitwise_negation_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = ~input0;

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = ~input_array0[j];
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bitwise_and) {

    }

    TEST(Vec4x32u, Bitwise_or) {

    }

    TEST(Vec4x32u, Bitwise_xor) {

    }

    TEST(Vec4x32u, Left_shift_by_scalar) {

    }

    TEST(Vec4x32u, Left_shift_by_vector) {

    }

    TEST(Vec4x32u, Right_shift_by_scalar) {

    }

    TEST(Vec4x32u, Right_shift_by_vector) {

    }

    //=====================================================
    // Conversion operators
    //=====================================================

    TEST(Vec4x32u, Conversion_to_mask) {
        vec4x32u vec{{0u, 1u, -4368365u, 4855536u}};

        auto a = vec.operator mask4x32u();
        mask4x32u b{{false, true, true, true}};

        EXPECT_TRUE(a == b);
    }

    TEST(Vec4x32u, Conversion_to_mask_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto results = mask4x32u{input0};

            std::array<bool, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = input_array0[j];
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec4x32u, Broadcast_bits) {
        mask4x32u m{{false, true, false, true}};

        vec4x32u b{{
            0x00000000,
            0xFFFFFFFF,
            0x00000000,
            0xFFFFFFFF
        }};

        EXPECT_TRUE(all(broadcast_bits(m) == b));
    }

    TEST(Vec4x32u, Broadcast_bits_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<bool, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u() & 0x1;
            }

            mask4x32u input0{input_array0};

            auto results = broadcast_bits(input0);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array0[j]) {
                    results_array[j] = 0xffffffff;
                } else {
                    results_array[j] = 0x00000000;
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Blend) {
        vec4x32u x{{0, 2, 4, 6}};
        vec4x32u y{{1, 3, 5, 7}};
        mask4x32u m{{false, true, false, true}};

        vec4x32u z = avel::blend(x, y, m);

        vec4x32u b{{0, 3, 4, 7}};
        EXPECT_TRUE(all(z == b));
    }
    
    TEST(Vec4x32u, Blend_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};
            
            
            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input1{input_array1};
            
            
            std::array<bool, 4> input_array2{};
            for (std::size_t j = 0; j < input_array2.size(); ++j) {
                input_array2[j] = random32u() & 0x1;
            }

            mask4x32u input2{input_array2};


            auto results = blend(input0, input1, input2);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                if (input_array2[j]) {
                    results_array[j] = input_array1[j];
                } else {
                    results_array[j] = input_array0[j];
                }
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Max) {
        vec4x32u x{{745, 356, 764, 589}};
        vec4x32u y{{974, 347, 560, 139}};

        vec4x32u z = max(x, y);

        vec4x32u b{{974, 356, 764, 589}};
        EXPECT_TRUE(all(z == b));
    }

    TEST(Vec4x32u, Max_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input1{input_array1};


            auto results = max(input0, input1);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Min) {
        vec4x32u x{{745, 356, 764, 589}};
        vec4x32u y{{974, 347, 560, 139}};

        vec4x32u z = min(x, y);

        vec4x32u b{{745, 347, 560, 139}};
        EXPECT_TRUE(all(z == b));
    }

    TEST(Vec4x32u, Min_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input1{input_array1};


            auto results = min(input0, input1);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = std::min(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Minmax) {
        vec4x32u x{{745, 356, 764, 589}};
        vec4x32u y{{974, 347, 560, 139}};

        auto ab = minmax(x, y);
        vec4x32u a = std::get<0>(ab);
        vec4x32u b = std::get<1>(ab);

        vec4x32u b0{{745, 347, 560, 139}};
        vec4x32u b1{{974, 356, 764, 589}};
        EXPECT_TRUE(all(a == b0));
        EXPECT_TRUE(all(b == b1));
    }

    TEST(Vec4x32u, Minmax_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input1{input_array1};


            auto results = minmax(input0, input1);

            std::array<std::uint32_t, 4> results_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array0[j] = std::min(input_array0[j], input_array1[j]);
            }

            std::array<std::uint32_t, 4> results_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array1[j] = std::max(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results[0] == vec4x32u{results_array0}));
            EXPECT_TRUE(all(results[1] == vec4x32u{results_array1}));
        }
    }

    TEST(Vec4x32u, Clamp) {
        vec4x32u x {{256,  75, 254, 200}};
        vec4x32u lo{{  0,  50, 100, 150}};
        vec4x32u hi{{100, 150, 200, 350}};

        vec4x32u y = clamp(x, lo, hi);

        vec4x32u b{{100, 75, 200, 200}};

        EXPECT_TRUE(all(y == b));
    }

    TEST(Vec4x32u, Clamp_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input1{input_array1};


            std::array<std::uint32_t, 4> input_array2{};
            for (std::size_t j = 0; j < input_array2.size(); ++j) {
                input_array2[j] = random32u();
            }

            vec4x32u input2{input_array2};

            auto xy = minmax(input1, input2);

            auto results = clamp(input0, xy[0], xy[1]);

            std::array<std::uint32_t, 4> results_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                auto xy = minmax(input_array1[j], input_array2[j]);
                results_array0[j] = clamp(input_array0[j], xy[0], xy[1]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array0}));
        }
    }

    TEST(Vec4x32u, Midpoint_low_high) {
        vec4x32u x{{  0, 31, 50, 23}};
        vec4x32u y{{150, 40, 75, 47}};

        vec4x32u z = midpoint(x, y);
        auto arr = to_array(z);

        vec4x32u b{{75, 35, 62, 35}};

        EXPECT_TRUE(all(z == b));
    }

    TEST(Vec4x32u, Midpoint_high_low) {
        vec4x32u x{{150, 40, 75, 47}};
        vec4x32u y{{  0, 31, 50, 23}};

        vec4x32u z = midpoint(x, y);
        auto arr = to_array(z);

        vec4x32u b{{75, 36, 63, 35}};

        EXPECT_TRUE(all(z == b));
    }

    TEST(Vec4x32u, Midpoint_equal) {
        vec4x32u x{{48, 75, 0, 0xFFFFFFFF}};
        vec4x32u y{{48, 75, 0, 0xFFFFFFFF}};

        vec4x32u z = midpoint(x, y);
        auto arr = to_array(z);

        vec4x32u b{{48, 75, 0, 0xFFFFFFFF}};

        EXPECT_TRUE(all(z == b));
    }

    TEST(Vec4x32u, Midpoint_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u();
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = midpoint(input0, input1);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = midpoint(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Load) {
        std::uint32_t arr[4] {
            0xFFFF0000,
            0x00FFFF00,
            0x0000FFFF,
            0xFF0000FF
        };

        vec4x32u v = load<vec4x32u>(arr);

        vec4x32u b{{
            0xFFFF0000,
            0x00FFFF00,
            0x0000FFFF,
            0xFF0000FF
        }};

        EXPECT_TRUE(all(v == b));
    }

    TEST(Vec4x32u, Load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            auto results = load<vec4x32u>(input_array0.data());

            EXPECT_TRUE(all(results == vec4x32u{input_array0}));
        }
    }

    TEST(Vec4x32u, Aligned_load) {
        alignas(alignof(vec4x32u)) std::uint32_t arr[4] {
            0xFFFF0000,
            0x00FFFF00,
            0x0000FFFF,
            0xFF0000FF
        };

        vec4x32u v = load<vec4x32u>(arr);

        vec4x32u b{{
            0xFFFF0000,
            0x00FFFF00,
            0x0000FFFF,
            0xFF0000FF
        }};
    }

    TEST(Vec4x32u, Aligned_load_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            alignas(alignof(vec4x32u)) std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            auto results = aligned_load<vec4x32u>(input_array0.data());

            EXPECT_TRUE(all(results == vec4x32u{input_array0}));
        }
    }

    TEST(Vec4x32u, Store) {
        vec4x32u v{{1, 2, 4, 8}};

        std::uint32_t arr[4]{};
        store(arr, v);

        EXPECT_EQ(arr[0], 1);
        EXPECT_EQ(arr[1], 2);
        EXPECT_EQ(arr[2], 4);
        EXPECT_EQ(arr[3], 8);
    }

    TEST(Vec4x32u, Store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
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

    TEST(Vec4x32u, Aligned_store) {
        vec4x32u v{{1, 2, 4, 8}};

        alignas(alignof(vec4x32u)) std::uint32_t arr[4]{};
        aligned_store(arr, v);

        EXPECT_EQ(arr[0], 1);
        EXPECT_EQ(arr[1], 2);
        EXPECT_EQ(arr[2], 4);
        EXPECT_EQ(arr[3], 8);
    }

    TEST(Vec4x32u, Aligned_store_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
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

    TEST(Vec4x32u, To_array) {
        vec4x32u v{{0, 1, 2, 3}};

        auto arr = to_array(v);

        EXPECT_EQ(arr[0], 0);
        EXPECT_EQ(arr[1], 1);
        EXPECT_EQ(arr[2], 2);
        EXPECT_EQ(arr[3], 3);
    }

    TEST(Vec4x32u, To_array_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
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

    //=====================================================
    // Integer functions
    //=====================================================

    TEST(Vec4x32u, Div_random) {
        for (unsigned i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> numerators{};
            for (std::size_t j = 0; j < 4; ++j) {
                numerators[j] = random32u();
            }

            std::array<std::uint32_t, 4> denominators{};
            for (std::size_t j = 0; j < 4; ++j) {
                denominators[j] = std::max(static_cast<std::uint32_t>(random32u()), static_cast<std::uint32_t>(1u));
            }

            std::array<std::uint32_t, 4> quotients{};
            std::array<std::uint32_t, 4> remainders{};

            for (std::size_t j = 0; j < 4; ++j) {
                quotients[j]  = (numerators[j] / denominators[j]);
                remainders[j] = (numerators[j] % denominators[j]);
            }

            vec4x32u n{numerators};
            vec4x32u d{denominators};

            auto div_result = div(n, d);

            auto q = to_array(div_result.quot);
            auto r = to_array(div_result.rem);

            for (std::size_t j = 0; j < 4; ++j) {
                EXPECT_EQ(q[j], quotients[j]);
                EXPECT_EQ(r[j], remainders[j]);

                //if (q[j] != quotients[j] || r[j] != remainders[j]) {
                //    int break_point_dummy = 5;
                //}
            }
        }
    }

    TEST(Vec4x32u, Popcount) {
        vec4x32u v{{
            0x00000000,
            0x01010101,
            0x11110000,
            0x34564778,
        }};

        vec4x32u c = popcount(v);
        vec4x32u b{{0, 4, 4, 15}};

        EXPECT_TRUE(all(c == b));
    }

    TEST(Vec4x32u, Popcount_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            auto results = popcount(input0);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = popcount(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Byteswap) {
        vec4x32u v{{
            0x00112233,
            0x44556677,
            0x8899aabb,
            0xccddeeff
        }};

        vec4x32u u = byteswap(v);

        vec4x32u b{{
            0x33221100,
            0x77665544,
            0xbbaa9988,
            0xffeeddcc
        }};

        EXPECT_TRUE(all(u == b));
    }

    TEST(Vec4x32u, Byteswap_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            auto results = byteswap(input0);

            std::array<std::uint32_t, 4> results_array{};
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

            vec4x32u v0{{
                0x00000000,
                0xFF03FF05,
                0x00000084,
                0x07FFFFFF
            }};

            vec4x32u u0 = countl_zero(v0);
            vec4x32u b0{{32, 0, 24, 5}};

            EXPECT_TRUE(all(u0 == b0));


            vec4x32u v1{{
                0x07FFFFFF,
                0x03FFFFFF,
                0x01FFFFFF,
                0x01000000
            }};

            vec4x32u u1 = countl_zero(v1);
            vec4x32u b1{{5, 6, 7, 7}};

            EXPECT_TRUE(all(u1 == b1));
        }
    }

    TEST(Vec4x32u, Countl_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            auto results = countl_zero(input0);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Countl_one) {
        vec4x32u v{{
            0x00000000,
            0xFF03FF05,
            0x00000084,
            0xF8000000
        }};

        vec4x32u u = countl_one(v);
        vec4x32u b{{0, 8, 0, 5}};

        EXPECT_TRUE(all(u == b));
    }

    TEST(Vec4x32u, Countl_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            auto results = countl_one(input0);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countl_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Countr_zero) {
        vec4x32u v{{
            0x00000000,
            0x00000001,
            0x40000000,
            0x80000000
        }};

        vec4x32u b{{
            32,
            0,
            30,
            31
        }};

        vec4x32u o = countr_zero(v);

        EXPECT_TRUE(all(o == b));
    }

    TEST(Vec4x32u, Countr_zero_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            auto results = countr_zero(input0);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_zero(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Countr_one) {
        vec4x32u v{{
            0x00000000,
            0x00000010,
            0x129400FF,
            0xFFFFFFFF
        }};

        vec4x32u b{{
            0,
            0,
            8,
            32
        }};

        vec4x32u o = countr_one(v);

        EXPECT_TRUE(all(o == b));
    }

    TEST(Vec4x32u, Countr_one_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            auto results = countr_one(input0);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = countr_one(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bit_floor) {
        vec4x32u v{{
            0x00000000,
            0x00000001,
            0x00000002,
            0x00000003
        }};

        vec4x32u b{{
            0x00000000,
            0x00000001,
            0x00000002,
            0x00000002
        }};

        vec4x32u o = bit_floor(v);

        EXPECT_TRUE(all(o == b));
    }

    TEST(Vec4x32u, Bit_floor_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            auto results = bit_floor(input0);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_floor(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Bit_ceil) {
        vec4x32u v{{
            0x00000000,
            0x00000001,
            0x00000002,
            0x00000003
        }};

        vec4x32u b{{
            0x00000000,
            0x00000001,
            0x00000002,
            0x00000004
        }};

        vec4x32u o = bit_ceil(v);

        EXPECT_TRUE(all(o == b));
    }

    TEST(Vec4x32u, Bit_ceil_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            auto results = bit_ceil(input0);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = bit_ceil(input_array0[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Has_single_bit) {
        vec4x32u v{{
            0x00000000,
            0x00000200,
            0x10000000,
            0x00080000
        }};

        mask4x32u m = has_single_bit(v);

        mask4x32u b{{false, true, true, true}};

        EXPECT_TRUE(m == b);
    }

    TEST(Vec4x32u, Has_single_bit_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};


            auto results = has_single_bit(input0);

            std::array<bool, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = has_single_bit(input_array0[j]);
            }

            EXPECT_TRUE(results == mask4x32u{results_array});
        }
    }

    TEST(Vec4x32u, Rotl_scalar) {
        vec4x32u v{{
            0x00ff00ff,
            0x13579BDF,
            0x87654321,
            0x12348888
        }};

        vec4x32u b{{
            0x0ff00ff0,
            0x3579BDF1,
            0x76543218,
            0x23488881
        }};

        vec4x32u o = rotl(v, 4);

        EXPECT_TRUE(all(o == b));
    }

    TEST(Vec4x32u, Rotl_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::uint32_t input1 = random32u() % 64;

            vec4x32u input0{input_array0};


            auto results = rotl(input0, input1);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotl(input_array0[j], input1);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Rotl_vector) {
        vec4x32u v{{
            0x00ff00ff,
            0x13579BDF,
            0x87654321,
            0x12348888
        }};

        vec4x32u s{{
            0,
            31,
            32,
            33
        }};

        vec4x32u b{{
            0x00ff00ff,
            0x89ABCDEF,
            0x87654321,
            0x24691110
        }};

        vec4x32u o = rotl(v, s);

        EXPECT_TRUE(all(o == b));
    }

    TEST(Vec4x32u, Rotl_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u() % 64;
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = rotl(input0, input1);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotl(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Rotr_scalar) {
        vec4x32u v{{
            0x00ff00ff,
            0x13579BDF,
            0x87654321,
            0x12348888
        }};

        vec4x32u b{{
            0xf00ff00f,
            0xF13579BD,
            0x18765432,
            0x81234888
        }};

        vec4x32u o = rotr(v, 4);

        EXPECT_TRUE(all(o == b));
    }

    TEST(Vec4x32u, Rotr_scalar_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::uint32_t input1 = random32u() % 64;

            vec4x32u input0{input_array0};


            auto results = rotr(input0, input1);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotr(input_array0[j], input1);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

    TEST(Vec4x32u, Rotr_vector) {
        vec4x32u v{{
            0x00ff00ff,
            0x13579BDF,
            0x87654321,
            0x12348888
        }};

        vec4x32u s{{
            0,
            31,
            32,
            33
        }};

        vec4x32u b{{
            0x00ff00ff,
            0x26af37be,
            0x87654321,
            0x091a4444
        }};

        vec4x32u o = rotr(v, s);

        EXPECT_TRUE(all(o == b));
    }

    TEST(Vec4x32u, Rotr_vector_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::array<std::uint32_t, 4> input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            std::array<std::uint32_t, 4> input_array1{};
            for (std::size_t j = 0; j < input_array1.size(); ++j) {
                input_array1[j] = random32u() % 64;
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};


            auto results = rotr(input0, input1);

            std::array<std::uint32_t, 4> results_array{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                results_array[j] = rotr(input_array0[j], input_array1[j]);
            }

            EXPECT_TRUE(all(results == vec4x32u{results_array}));
        }
    }

}

#endif //AVEL_VEC4X32U_TESTS_HPP
