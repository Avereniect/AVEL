#ifndef AVEL_DENOM2X64I_TESTS_HPP
#define AVEL_DENOM2X64I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    TEST(Denom2x64i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom2x64i denom{vec2x64i{0x01}};

            arr2x64i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
            }

            vec2x64i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec2x64i{0x00}));
        }
    }

    TEST(Denom2x64i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr2x64i input_array0{};
            arr2x64i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random64u();
                input_array1[j] = random64u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

            vec2x64i input0{input_array0};
            vec2x64i input1{input_array1};

            arr2x64i quotients{};
            arr2x64i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec2x64i quot{quotients};
            vec2x64i rem{remainders};

            Denom2x64i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

}

#endif //AVEL_DENOM2X64I_TESTS_HPP
