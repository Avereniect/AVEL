Vec2x64u//
// Created by avereniect on 12/27/21.
//

#ifndef AVEL_VEC4X32U_TESTS_HPP
#define AVEL_VEC4X32U_TESTS_HPP

#include <gtest/gtest.h>

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // Constructors
    //=====================================================

    TEST(vec4x32u, Conversion_tests1) {
        vec4x32i veci{{0, 1, 2, 3}};
        vec4x32u vecu{veci};

        vec4x32u baseline{{0, 1, 2, 3}};

        EXPECT_TRUE(all(baseline == vecu));
    }

    TEST(vec4x32u, Construct_from_mask) {
        mask4x32u m_false{false};
        mask4x32u m_true{true};

        vec4x32u v0{m_false};
        vec4x32u baseline0{{0, 0, 0, 0}};

        vec4x32u v1{m_true};
        vec4x32u baseline1{{1, 1, 1, 1}};

        EXPECT_TRUE(all(v0 == baseline0));
        EXPECT_TRUE(all(v1 == baseline1));
    }

    TEST(vec4x32u, Construct_from_sclar) {
        std::uint32_t x = 0xF0F0F0F0;
        vec4x32u v{x};
        vec4x32u baseline{{x, x, x, x}};

        EXPECT_TRUE(all(v == baseline));
    }

    TEST(vec4x32u, Construct_from_pointer) {
        std::array<std::uint32_t, 4> data{
            0xF0F0F0F0,
            0xE0E0E0E0,
            0xD0D0D0D0,
            0xC0C0C0C0
        };
        vec4x32u v{data};
        vec4x32u baseline{data};

        EXPECT_TRUE(all(v == baseline));
    }

    //=====================================================
    // Assignment operators
    //=====================================================

    TEST(vec4x32u, Assign_scalar) {
        std::uint32_t x = 0xF0F0F0F0;
        vec4x32u v{};
        v = x;
        vec4x32u baseline{{x, x, x, x}};

        EXPECT_TRUE(all(v == baseline));
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    TEST(vec4x32u, Equality_comparison) {
        vec4x32u u{{1, 1, 1, 1}};
        vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{false, true, false, false}};
        mask4x32u m1 = (u == v);

        EXPECT_EQ(m0, m1);
    }

    TEST(vec4x32u, Inequality_comparison) {
        vec4x32u u{{1, 1, 1, 1}};
        vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{true, false, true, true}};
        mask4x32u m1 = (u != v);

        EXPECT_EQ(m0, m1);
    }

    TEST(vec4x32u, Less_than_comparison) {
        vec4x32u u{{1, 1, 1, 1}};
        vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{false, false, true, true}};
        mask4x32u m1 = (u < v);

        EXPECT_EQ(m0, m1);
    }

    TEST(vec4x32u, Less_than_or_equal_comparison) {
        vec4x32u u{{1, 1, 1, 1}};
        vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{false, true, true, true}};
        mask4x32u m1 = (u <= v);

        EXPECT_EQ(m0, m1);
    }

    TEST(vec4x32u, Greater_than_comparison) {
        vec4x32u u{{1, 1, 1, 1}};
        vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{true, false, false, false}};
        mask4x32u m1 = (u > v);

        EXPECT_EQ(m0, m1);
    }

    TEST(vec4x32u, Greater_than_or_equal_comparison) {
        vec4x32u u{{1, 1, 1, 1}};
        vec4x32u v{{0, 1, 2, 3}};

        mask4x32u m0{{true, true, false, false}};
        mask4x32u m1 = (u >= v);

        EXPECT_EQ(m0, m1);
    }

    //=====================================================
    // Unary arithmetic operators
    //=====================================================

    TEST(vec4x32u, Unary_plus) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{+u};

        EXPECT_TRUE(all(u == v));
    }

    TEST(vec4x32u, Unary_minus) {
        vec4x32u u{{+0, +1, +2, +3}};
        vec4x32i v{{-0, -1, -2, -3}};
        vec4x32i w{-u};

        EXPECT_TRUE(all(v == w));
    }

    //=====================================================
    // Arithmetic assignment operators
    //=====================================================

    TEST(vec4x32u, Plus_assignment) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        u += v;
        vec4x32u x{{4, 6, 8, 10}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(vec4x32u, Minus_assignment) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        v -= u;
        vec4x32u x{{4, 4, 4, 4}};

        EXPECT_TRUE(all(v == x));
    }

    TEST(vec4x32u, Times_assignment) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        u *= v;
        vec4x32u x{{0, 5, 12, 21}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(vec4x32u, Div_assignment) {
        vec4x32u u{{15, 16, 17, 18}};
        vec4x32u v{{3, 4, 17, 3}};

        u /= v;
        vec4x32u x{{5, 4, 1, 6}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(vec4x32u, Mod_assignment) {
        vec4x32u u{{18, 17, 16, 15}};
        vec4x32u v{{5, 4, 17, 3}};

        u %= v;
        vec4x32u x{{3, 1, 16, 0}};

        EXPECT_TRUE(all(u == x));
    }

    //=====================================================
    // Arithmetic operators
    //=====================================================

    TEST(vec4x32u, Plus) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        u += v;
        vec4x32u x{{4, 6, 8, 10}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(vec4x32u, Minus) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        v -= u;
        vec4x32u x{{4, 4, 4, 4}};

        EXPECT_TRUE(all(v == x));
    }

    TEST(vec4x32u, Times) {
        vec4x32u u{{0, 1, 2, 3}};
        vec4x32u v{{4, 5, 6, 7}};

        u *= v;
        vec4x32u x{{0, 5, 12, 21}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(vec4x32u, Div) {
        vec4x32u u{{15, 16, 17, 18}};
        vec4x32u v{{3, 4, 17, 3}};

        u /= v;
        vec4x32u x{{5, 4, 1, 6}};

        EXPECT_TRUE(all(u == x));
    }

    TEST(vec4x32u, Mod) {
        vec4x32u u{{18, 17, 16, 15}};
        vec4x32u v{{5, 4, 17, 3}};

        u %= v;
        vec4x32u x{{3, 1, 16, 0}};

        EXPECT_TRUE(all(u == x));
    }

    //=====================================================
    // Increment/Decrement operators
    //=====================================================

    //=====================================================
    // Bitwise assignment operators
    //=====================================================

    //=====================================================
    // Bitwise operators
    //=====================================================

    //=====================================================
    // Conversions
    //=====================================================

    //=====================================================
    // General vector functions
    //=====================================================

    TEST(Vec4x32u, Blend) {
        vec4x32u x{{0, 2, 4, 6}};
        vec4x32u y{{1, 3, 5, 7}};
        mask4x32u m{{false, true, false, true}};

        vec4x32u z = avel::blend(x, y, m);

        vec4x32u b{{0, 3, 4, 7}};
        EXPECT_TRUE(all(z == b));
    }

    TEST(Vec4x32u, Max) {
        vec4x32u x{{745, 356, 764, 589}};
        vec4x32u y{{974, 347, 560, 139}};

        vec4x32u z = max(x, y);

        vec4x32u b{{974, 356, 764, 589}};
        EXPECT_TRUE(all(z == b));
    }

    TEST(Vec4x32u, Min) {
        vec4x32u x{{745, 356, 764, 589}};
        vec4x32u y{{974, 347, 560, 139}};

        vec4x32u z = min(x, y);

        vec4x32u b{{745, 347, 560, 139}};
        EXPECT_TRUE(all(z == b));
    }

    TEST(Vec4x32u, Clamp) {
        vec4x32u x {{256,  75, 254, 200}};
        vec4x32u lo{{  0,  50, 100, 150}};
        vec4x32u hi{{100, 150, 200, 350}};

        vec4x32u y = clamp(x, lo, hi);

        vec4x32u b{{100, 75, 200, 200}};

        EXPECT_TRUE(all(y == b));
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

    //=====================================================
    // Integer functions
    //=====================================================

    //=====================================================
    // Bitwise functions
    //=====================================================

    /*
    TEST(Vex4x32u, Pop_count) {
        vec4x32u x{{0x00, 0x01, 0x03, 0xFF}};
        vec4x32u y = pop_count(x);
        vec4x32u b{{0, 1, 2, 8}};

        EXPECT_EQ(b == y, mask4x32u{true});
    }
    */

}

#endif //AVEL_VEC4X32U_TESTS_HPP
