//
// Created by avereniect on 8/1/22.
//

#ifndef AVEL_RECIP32I_TESTS_HPP
#define AVEL_RECIP32I_TESTS_HPP

namespace avel::tests {

    TEST(Recip32i, Identity) {
        Recip32i recip{1};

        std::int32_t x = 15;
        x *= recip;
        EXPECT_EQ(x, 15);
    }

}

#endif //AVEL_RECIP32I_TESTS_HPP
