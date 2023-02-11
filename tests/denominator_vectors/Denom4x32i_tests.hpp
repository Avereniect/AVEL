#ifndef AVEL_DENOM4X32I_TESTS_HPP
#define AVEL_DENOM4X32I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom4x32i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom4x32i denom{vec4x32i{0x01}};

            arr4x32i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
            }

            vec4x32i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec4x32i{0x00}));
        }
    }

    TEST(Denom4x32i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr4x32i input_array0{};
            arr4x32i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random32u();
                input_array1[j] = random32u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec4x32i input0{input_array0};
            vec4x32i input1{input_array1};

            arr4x32i quotients{};
            arr4x32i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec4x32i quot{quotients};
            vec4x32i rem{remainders};

            Denom4x32i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

}

#endif //AVEL_DENOM4X32I_TESTS_HPP
