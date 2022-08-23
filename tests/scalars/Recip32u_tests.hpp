//
// Created by avereniect on 8/1/22.
//

#ifndef AVEL_RECIP32U_TESTS_HPP
#define AVEL_RECIP32U_TESTS_HPP

namespace avel::tests {

    TEST(Recip32u, Identity) {
        Recip32u recip{1};

        std::uint32_t x = 15;
        x *= recip;
        EXPECT_EQ(x, 15);
    }

    TEST(Recip32u, Two) {
        Recip32u recip{2};

        std::uint32_t x = 14;
        x *= recip;
        EXPECT_EQ(x, 7);
    }

    TEST(Recip32u, Three) {
        Recip32u recip{3};

        std::uint32_t x = 15;
        x *= recip;
        EXPECT_EQ(x, 5);
    }

    TEST(Recip32u, Five) {
        Recip32u recip{5};

        std::uint32_t x = 135;
        x *= recip;
        EXPECT_EQ(x, 27);
    }

    TEST(Recip32u, Seven) {
        Recip32u recip{7};

        std::uint32_t x = 70;
        x *= recip;
        EXPECT_EQ(x, 10);
    }

}

#endif //AVEL_RECIP32U_TESTS_HPP
