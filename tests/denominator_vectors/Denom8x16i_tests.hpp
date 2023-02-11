#ifndef AVEL_DENOM8X16I_TESTS_HPP
#define AVEL_DENOM8X16I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom8x16i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom8x16i denom{vec8x16i{0x01}};

            arr8x16i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec8x16i{0x00}));
        }
    }

    TEST(Denom8x16i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16i input_array0{};
            arr8x16i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = random16u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec8x16i input0{input_array0};
            vec8x16i input1{input_array1};

            arr8x16i quotients{};
            arr8x16i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec8x16i quot{quotients};
            vec8x16i rem{remainders};

            Denom8x16i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

}

#endif //AVEL_DENOM8X16I_TESTS_HPP
