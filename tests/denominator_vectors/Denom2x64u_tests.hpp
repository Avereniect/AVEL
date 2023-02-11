#ifndef AVEL_DENOM2X64U_TESTS_HPP
#define AVEL_DENOM2X64U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    /*
    TEST(Denom2x64u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom2x64u denom{vec2x64u{0x01}};

            arr2x64u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec2x64u{0x00}));
        }
    }
    */

    TEST(Denom2x64u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64u input_array0{};
            arr2x64u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = max(random64u(), std::uint64_t{1});
            }

            vec2x64u input0{input_array0};
            vec2x64u input1{input_array1};

            arr2x64u quotients{};
            arr2x64u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec2x64u quot{quotients};
            vec2x64u rem{remainders};

            Denom2x64u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

}

#endif //AVEL_DENOM2X64U_TESTS_HPP
