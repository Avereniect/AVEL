#ifndef AVEL_DENOMINATOR_VECTOR_TESTS_HPP
#define AVEL_DENOMINATOR_VECTOR_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    #if defined(AVEL_SSE2) || defined(AVEL_NEON)

    TEST(Denom16x8u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom16x8u denom{vec16x8u{0x01}};

            auto input_array0 = random_array<arr16x8u>();

            vec16x8u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec16x8u{0x00}));
        }
    }

    TEST(Denom16x8u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr16x8u>();
            auto input_array1 = random_denominator_array<arr16x8u>();

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

    TEST(Denom16x8i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom16x8i denom{vec16x8i{0x01}};

            auto input_array0 = random_array<arr16x8i>();

            vec16x8i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec16x8i{0x00}));
        }
    }

    TEST(Denom16x8i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr16x8i>();
            auto input_array1 = random_denominator_array<arr16x8i>();

            vec16x8i input0{input_array0};
            vec16x8i input1{input_array1};

            arr16x8i quotients{};
            arr16x8i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec16x8i quot{quotients};
            vec16x8i rem{remainders};

            Denom16x8i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom8x16u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom8x16u denom{vec8x16u{0x01}};

            auto input_array0 = random_array<arr8x16u>();

            vec8x16u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec8x16u{0x00}));
        }
    }

    TEST(Denom8x16u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr8x16u>();
            auto input_array1 = random_denominator_array<arr8x16u>();

            vec8x16u input0{input_array0};
            vec8x16u input1{input_array1};

            arr8x16u quotients{};
            arr8x16u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec8x16u quot{quotients};
            vec8x16u rem{remainders};

            Denom8x16u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom8x16i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom8x16i denom{vec8x16i{0x01}};

            auto input_array0 = random_array<arr8x16i>();

            vec8x16i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec8x16i{0x00}));
        }
    }

    TEST(Denom8x16i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr8x16i>();
            auto input_array1 = random_denominator_array<arr8x16i>();

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

    TEST(Denom4x32u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom4x32u denom{vec4x32u{0x01}};

            auto input_array0 = random_array<arr4x32u>();

            vec4x32u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec4x32u{0x00}));
        }
    }

    TEST(Denom4x32u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr4x32u>();
            auto input_array1 = random_denominator_array<arr4x32u>();

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

    TEST(Denom4x32i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom4x32i denom{vec4x32i{0x01}};

            auto input_array0 = random_array<arr4x32i>();

            vec4x32i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec4x32i{0x00}));
        }
    }

    TEST(Denom4x32i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr4x32i>();
            auto input_array1 = random_denominator_array<arr4x32i>();

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

    TEST(Denom2x64u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom2x64u denom{vec2x64u{0x01}};

            auto input_array0 = random_array<arr2x64u>();

            vec2x64u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec2x64u{0x00}));
        }
    }

    TEST(Denom2x64u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr2x64u>();
            auto input_array1 = random_denominator_array<arr2x64u>();

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

    TEST(Denom2x64i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom2x64i denom{vec2x64i{0x01}};

            auto input_array0 = random_array<arr2x64i>();

            vec2x64i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec2x64i{0x00}));
        }
    }

    TEST(Denom2x64i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr2x64i>();
            auto input_array1 = random_denominator_array<arr2x64i>();

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

    #endif





    #if defined(AVEL_AVX2)

    TEST(Denom32x8u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom32x8u denom{vec32x8u{0x01}};

            auto input_array0 = random_array<arr32x8u>();

            vec32x8u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec32x8u{0x00}));
        }
    }

    TEST(Denom32x8u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr32x8u>();
            auto input_array1 = random_denominator_array<arr32x8u>();

            vec32x8u input0{input_array0};
            vec32x8u input1{input_array1};


            arr32x8u quotients{};
            arr32x8u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec32x8u quot{quotients};
            vec32x8u rem{remainders};

            Denom32x8u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom32x8i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom32x8i denom{vec32x8i{0x01}};

            auto input_array0 = random_array<arr32x8i>();

            vec32x8i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec32x8i{0x00}));
        }
    }

    TEST(Denom32x8i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr32x8i>();
            auto input_array1 = random_denominator_array<arr32x8i>();

            vec32x8i input0{input_array0};
            vec32x8i input1{input_array1};

            arr32x8i quotients{};
            arr32x8i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec32x8i quot{quotients};
            vec32x8i rem{remainders};

            Denom32x8i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom16x16u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom16x16u denom{vec16x16u{0x01}};

            auto input_array0 = random_array<arr16x16u>();

            vec16x16u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec16x16u{0x00}));
        }
    }

    TEST(Denom16x16u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr16x16u>();
            auto input_array1 = random_denominator_array<arr16x16u>();

            vec16x16u input0{input_array0};
            vec16x16u input1{input_array1};

            arr16x16u quotients{};
            arr16x16u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec16x16u quot{quotients};
            vec16x16u rem{remainders};

            Denom16x16u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom16x16i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom16x16i denom{vec16x16i{0x01}};

            auto input_array0 = random_array<arr16x16i>();

            vec16x16i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec16x16i{0x00}));
        }
    }

    TEST(Denom16x16i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr16x16i>();
            auto input_array1 = random_denominator_array<arr16x16i>();

            vec16x16i input0{input_array0};
            vec16x16i input1{input_array1};

            arr16x16i quotients{};
            arr16x16i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec16x16i quot{quotients};
            vec16x16i rem{remainders};

            Denom16x16i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom8x32u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom8x32u denom{vec8x32u{0x01}};

            auto input_array0 = random_array<arr8x32u>();

            vec8x32u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec8x32u{0x00}));
        }
    }

    TEST(Denom8x32u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr8x32u>();
            auto input_array1 = random_denominator_array<arr8x32u>();

            vec8x32u input0{input_array0};
            vec8x32u input1{input_array1};

            arr8x32u quotients{};
            arr8x32u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec8x32u quot{quotients};
            vec8x32u rem{remainders};

            Denom8x32u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom8x32i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom8x32i denom{vec8x32i{0x01}};

            auto input_array0 = random_array<arr8x32i>();

            vec8x32i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec8x32i{0x00}));
        }
    }

    TEST(Denom8x32i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr8x32i>();
            auto input_array1 = random_denominator_array<arr8x32i>();

            vec8x32i input0{input_array0};
            vec8x32i input1{input_array1};

            arr8x32i quotients{};
            arr8x32i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec8x32i quot{quotients};
            vec8x32i rem{remainders};

            Denom8x32i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom4x64u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom4x64u denom{vec4x64u{0x01}};

            auto input_array0 = random_array<arr4x64u>();

            vec4x64u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec4x64u{0x00}));
        }
    }

    TEST(Denom4x64u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr4x64u>();
            auto input_array1 = random_denominator_array<arr4x64u>();

            vec4x64u input0{input_array0};
            vec4x64u input1{input_array1};

            arr4x64u quotients{};
            arr4x64u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec4x64u quot{quotients};
            vec4x64u rem{remainders};

            Denom4x64u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom4x64i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom4x64i denom{vec4x64i{0x01}};

            auto input_array0 = random_array<arr4x64i>();

            vec4x64i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec4x64i{0x00}));
        }
    }

    TEST(Denom4x64i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr4x64i>();
            auto input_array1 = random_denominator_array<arr4x64i>();

            vec4x64i input0{input_array0};
            vec4x64i input1{input_array1};

            arr4x64i quotients{};
            arr4x64i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec4x64i quot{quotients};
            vec4x64i rem{remainders};

            Denom4x64i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    #endif







    #if defined(AVEL_AVX512F)

    TEST(Denom16x32u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom16x32u denom{vec16x32u{0x01}};

            auto input_array0 = random_array<arr16x32u>();

            vec16x32u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec16x32u{0x00}));
        }
    }

    TEST(Denom16x32u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr16x32u>();
            auto input_array1 = random_denominator_array<arr16x32u>();

            vec16x32u input0{input_array0};
            vec16x32u input1{input_array1};

            arr16x32u quotients{};
            arr16x32u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec16x32u quot{quotients};
            vec16x32u rem{remainders};

            Denom16x32u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom16x32i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom16x32i denom{vec16x32i{0x01}};

            auto input_array0 = random_array<arr16x32i>();

            vec16x32i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec16x32i{0x00}));
        }
    }

    TEST(Denom16x32i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr16x32i>();
            auto input_array1 = random_denominator_array<arr16x32i>();

            vec16x32i input0{input_array0};
            vec16x32i input1{input_array1};

            arr16x32i quotients{};
            arr16x32i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec16x32i quot{quotients};
            vec16x32i rem{remainders};

            Denom16x32i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom8x64u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom8x64u denom{vec8x64u{0x01}};

            auto input_array0 = random_array<arr8x64u>();

            vec8x64u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec8x64u{0x00}));
        }
    }

    TEST(Denom8x64u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr8x64u>();
            auto input_array1 = random_denominator_array<arr8x64u>();

            vec8x64u input0{input_array0};
            vec8x64u input1{input_array1};

            arr8x64u quotients{};
            arr8x64u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec8x64u quot{quotients};
            vec8x64u rem{remainders};

            Denom8x64u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom8x64i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom8x64i denom{vec8x64i{0x01}};

            auto input_array0 = random_array<arr8x64i>();

            vec8x64i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec8x64i{0x00}));
        }
    }

    TEST(Denom8x64i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr8x64i>();
            auto input_array1 = random_denominator_array<arr8x64i>();

            vec8x64i input0{input_array0};
            vec8x64i input1{input_array1};

            arr8x64i quotients{};
            arr8x64i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec8x64i quot{quotients};
            vec8x64i rem{remainders};

            Denom8x64i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    #endif

    #if defined(AVEL_AVX512BW)

    TEST(Denom64x8u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom64x8u denom{vec64x8u{0x01}};

            auto input_array0 = random_array<arr64x8u>();

            vec64x8u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec64x8u{0x00}));
        }
    }

    TEST(Denom64x8u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr64x8u>();
            auto input_array1 = random_denominator_array<arr64x8u>();

            vec64x8u input0{input_array0};
            vec64x8u input1{input_array1};


            arr64x8u quotients{};
            arr64x8u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec64x8u quot{quotients};
            vec64x8u rem{remainders};

            Denom64x8u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom64x8i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom64x8i denom{vec64x8i{0x01}};

            auto input_array0 = random_array<arr64x8i>();

            vec64x8i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec64x8i{0x00}));
        }
    }

    TEST(Denom64x8i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr64x8i>();
            auto input_array1 = random_denominator_array<arr64x8i>();

            vec64x8i input0{input_array0};
            vec64x8i input1{input_array1};

            arr64x8i quotients{};
            arr64x8i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec64x8i quot{quotients};
            vec64x8i rem{remainders};

            Denom64x8i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom32x16u, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom32x16u denom{vec32x16u{0x01}};

            auto input_array0 = random_array<arr32x16u>();

            vec32x16u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec32x16u{0x00}));
        }
    }

    TEST(Denom32x16u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr32x16u>();
            auto input_array1 = random_denominator_array<arr32x16u>();

            vec32x16u input0{input_array0};
            vec32x16u input1{input_array1};

            arr32x16u quotients{};
            arr32x16u remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec32x16u quot{quotients};
            vec32x16u rem{remainders};

            Denom32x16u denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    TEST(Denom32x16i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom32x16i denom{vec32x16i{0x01}};

            auto input_array0 = random_array<arr32x16i>();

            vec32x16i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec32x16i{0x00}));
        }
    }

    TEST(Denom32x16i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto input_array0 = random_array<arr32x16i>();
            auto input_array1 = random_denominator_array<arr32x16i>();

            vec32x16i input0{input_array0};
            vec32x16i input1{input_array1};

            arr32x16i quotients{};
            arr32x16i remainders{};
            for (std::size_t j = 0; j < quotients.size(); ++j) {
                quotients[j]  = input_array0[j] / input_array1[j];
                remainders[j] = input_array0[j] % input_array1[j];
            }

            vec32x16i quot{quotients};
            vec32x16i rem{remainders};

            Denom32x16i denom{input1};
            auto qr = div(input0, denom);

            EXPECT_TRUE(all(quot == qr.quot));
            EXPECT_TRUE(all(rem  == qr.rem ));
        }
    }

    #endif

}

#endif
