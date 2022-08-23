//
// Created by avereniect on 8/14/22.
//

#ifndef AVEL_VEC2X64U_TESTS_HPP
#define AVEL_VEC2X64U_TESTS_HPP

namespace avel::tests {

    TEST(vec2x64u, Division) {
        vec2x64u x{{64, 39}};
        vec2x64u y{{4, 13}};

        vec2x64u r = x / y;

        vec2x64u z{{16, 3}};
        EXPECT_TRUE(all(r == z));
    }

}

#endif //AVEL_VEC2X64U_TESTS_HPP
