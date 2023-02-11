#ifndef AVEL_DENOM16X8U_TESTS_HPP
#define AVEL_DENOM16X8U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom16x8u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom16x8u denom{vec16x8u{0x01}};

            arr16x8u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec16x8u{0x00}));
        }
    }

    TEST(Denom16x8u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8u input_array0{};
            arr16x8u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = max(random8u(), std::uint8_t{1});
            }

            vec16x8u input0{input_array0};
            vec16x8u input1{input_array1};


            arr16x8u quotients{};
            arr16x8u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec16x8u quot{quotients};
            vec16x8u rem{remainders};

            Denom16x8u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

}

#endif //AVEL_DENOM16X8U_TESTS_HPP
