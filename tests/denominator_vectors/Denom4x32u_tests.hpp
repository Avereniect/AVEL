#ifndef AVEL_DENOM4X32U_TESTS_HPP
#define AVEL_DENOM4X32U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom4x32u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom4x32u denom{vec4x32u{0x01}};

            arr4x32u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec4x32u{0x00}));
        }
    }

    TEST(Denom4x32u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32u input_array0{};
            arr4x32u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = max(random32u(), std::uint32_t{1});
            }

            vec4x32u input0{input_array0};
            vec4x32u input1{input_array1};

            arr4x32u quotients{};
            arr4x32u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec4x32u quot{quotients};
            vec4x32u rem{remainders};

            Denom4x32u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

}

#endif //AVEL_DENOM4X32U_TESTS_HPP
