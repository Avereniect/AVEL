//
// Created by avereniect on 8/6/22.
//

#ifndef AVEL_RECIP4X32U_TESTS_HPP
#define AVEL_RECIP4X32U_TESTS_HPP

namespace avel::tests {

    /*
    TEST(Recip4x32u, Identity) {
        Reciprocal<vec4x32u> recip{vec4x32u{0x01}};

        vec4x32u x{15};

        x *= recip;

        EXPECT_TRUE(all(x == vec4x32u{15}));
    }*/

    TEST(Recip4x32u, Two) {
        Reciprocal<vec4x32u> recip{vec4x32u{{0x02, 0x03, 0x05, 0x07}}};

        vec4x32u x{210};

        x *= recip;

        EXPECT_TRUE(all(x == vec4x32u{{105, 70, 42, 30}}));
    }

}

#endif //AVEL_RECIP4X32U_TESTS_HPP
