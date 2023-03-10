#ifndef AVEL_DENOMINATOR_VECTOR_TESTS_HPP
#define AVEL_DENOMINATOR_VECTOR_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    #if defined(AVEL_SSE2) || defined(AVEL_NEON)

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

    TEST(Denom16x8i, One) {
        for (std::size_t i = 0; i < iterations; ++i) {
            Denom16x8i denom{vec16x8i{0x01}};

            arr16x8i input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
            }

            vec16x8i input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec16x8i{0x00}));
        }
    }

    TEST(Denom16x8i, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr16x8i input_array0{};
            arr16x8i input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random8u();
                input_array1[j] = random8u();
                if (input_array1[j] == 0) {
                    input_array1[j] = 1;
                }
            }

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

            arr8x16u input_array0{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
            }

            vec8x16u input0{input_array0};

            auto qr = div(input0, denom);

            EXPECT_TRUE(all(qr.quot == input0));
            EXPECT_TRUE(all(qr.rem  == vec8x16u{0x00}));
        }
    }

    TEST(Denom8x16u, Random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            arr8x16u input_array0{};
            arr8x16u input_array1{};
            for (std::size_t j = 0; j < input_array0.size(); ++j) {
                input_array0[j] = random16u();
                input_array1[j] = max(random16u(), std::uint16_t{1});
            }

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

    #endif

}

#endif
