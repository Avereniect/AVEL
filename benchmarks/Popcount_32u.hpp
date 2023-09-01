#ifndef AVEL_POPCOUNT_32U_HPP
#define AVEL_POPCOUNT_32U_HPP

namespace avel::benchmarks::popcount_32u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint32_t scalar_native_impl(std::uint32_t x) {
        return std::popcount(x);
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(popcount_32u::scalar_native);

    #endif



    std::uint32_t scalar_bitwise_accumulate_impl(std::uint32_t x) {
        std::uint32_t ret = 0;

        while (x & 0x1) {
            ret += 1;
            x >>= 1;
        }

        return ret;
    }

    auto scalar_bitwise_accumulate = scalar_test_bench<std::uint32_t, scalar_bitwise_accumulate_impl>;

    BENCHMARK(popcount_32u::scalar_bitwise_accumulate);



    std::uint32_t scalar_bitwise_accumulate_branchless_impl(std::uint32_t x) {
        std::uint32_t ret = 0;

        for (int i = 0; i < 32; ++i) {
            ret += (x >> i) & 0x1;
        }

        return ret;
    }

    auto scalar_bitwise_accumulate_branchless = scalar_test_bench<std::uint32_t, scalar_bitwise_accumulate_branchless_impl>;

    BENCHMARK(popcount_32u::scalar_bitwise_accumulate_branchless);



    std::uint32_t scalar_bitwise_divide_and_conquer_impl(std::uint32_t x) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & 0x55555555u);
        x = (x & 0x33333333u) + ((x >> 2) & 0x33333333u);
        x = ((x + (x >> 4) & 0x0F0F0F0Fu) * 0x01010101u) >> 24;
        return x;
    }

    auto scalar_bitwise_divide_and_conquer = scalar_test_bench<std::uint32_t, scalar_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_32u::scalar_bitwise_divide_and_conquer);



    std::uint32_t scalar_nibble_lookup_impl(std::uint32_t x) {
        constexpr std::uint8_t table[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        std::uint8_t nibble0 = x & 0xf;
        std::uint8_t nibble1 = (x >> 0x04) & 0xf;
        std::uint8_t nibble2 = (x >> 0x08) & 0xf;
        std::uint8_t nibble3 = (x >> 0x0C) & 0xf;
        std::uint8_t nibble4 = (x >> 0x10) & 0xf;
        std::uint8_t nibble5 = (x >> 0x14) & 0xf;
        std::uint8_t nibble6 = (x >> 0x18) & 0xf;
        std::uint8_t nibble7 = (x >> 0x1C);

        return
            table[nibble0] + table[nibble1] + table[nibble2] + table[nibble3] +
            table[nibble4] + table[nibble5] + table[nibble6] + table[nibble7];
    }

    auto scalar_nibble_lookup = scalar_test_bench<std::uint32_t, scalar_nibble_lookup_impl>;

    BENCHMARK(popcount_32u::scalar_nibble_lookup);



    std::uint32_t scalar_byte_lookup_impl(std::uint32_t x) {
        constexpr std::uint32_t table[256] {
            0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
            4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
        };

        std::uint8_t byte0 = x & 0xff;
        std::uint8_t byte1 = (x >> 0x08) & 0xff;
        std::uint8_t byte2 = (x >> 0x10) & 0xff;
        std::uint8_t byte3 = x >> 0x18;

        return table[byte0] + table[byte1] + table[byte2] + table[byte3];
    }

    auto scalar_byte_lookup = scalar_test_bench<std::uint32_t, scalar_byte_lookup_impl>;

    BENCHMARK(popcount_32u::scalar_byte_lookup);



    #if defined(AVEL_POPCNT)

    std::uint32_t scalar_popcnt_op_impl(std::uint32_t x) {
        return _popcnt32(std::uint32_t{x});
    }

    auto scalar_popcnt_op = scalar_test_bench<std::uint32_t, scalar_popcnt_op_impl>;

    BENCHMARK(popcount_32u::scalar_popcnt_op);

    #endif

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_bitwise_divide_and_conquer_impl(vec4x32u x) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & vec4x32u{0x55555555u});
        x = (x & vec4x32u{0x33333333u}) + ((x >> 2) & vec4x32u{0x33333333u});
        x = ((x + (x >> 4) & vec4x32u{0x0F0F0F0Fu}) * vec4x32u{0x01010101u}) >> 24;
        return x;
    }

    auto vec4x32u_bitwise_divide_and_conquer = vector_test_bench<vec4x32u, vec4x32u_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_32u::vec4x32u_bitwise_divide_and_conquer);

    #endif



    #if defined(AVEL_SSSE3)

    vec4x32u vec4x32u_pshufb_impl(vec4x32u x) {
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto nibble_mask = _mm_set1_epi8(0x0F);

        auto index0 = _mm_and_si128(decay(x), nibble_mask);
        auto index1 = _mm_and_si128(_mm_srli_epi16(decay(x), 0x4), nibble_mask);

        auto partial_byte_sum0 = _mm_shuffle_epi8(table, index0);
        auto partial_byte_sum1 = _mm_shuffle_epi8(table, index1);

        auto byte_sum = _mm_add_epi8(partial_byte_sum0, partial_byte_sum1);

        auto partial_sum0 = _mm_add_epi8(byte_sum, _mm_slli_epi32(byte_sum, 16));
        auto partial_sum1 = _mm_add_epi8(partial_sum0, _mm_slli_epi32(partial_sum0, 8));

        auto ret = _mm_srli_epi32(partial_sum1, 24);

        return vec4x32u{ret};
    }

    auto vec4x32u_pshufb = vector_test_bench<vec4x32u, vec4x32u_pshufb_impl>;

    BENCHMARK(popcount_32u::vec4x32u_pshufb);

    #endif

    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)

    vec4x32u vec4x32u_pocnt16_op_impl(vec4x32u x) {
        auto tmp0 = _mm_popcnt_epi16(decay(x));
        auto tmp1 = _mm_slli_epi32(tmp0, 16);
        auto tmp2 = _mm_add_epi32(tmp0, tmp1);
        return vec4x32u{_mm_srli_epi32(tmp2, 16)};
    }

    auto vec4x32u_pocnt16_op = vector_test_bench<vec4x32u, vec4x32u_pocnt16_op_impl>;

    BENCHMARK(popcount_32u::vec4x32u_pocnt16_op);

    #endif

    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)

    vec4x32u vec4x32u_pocnt_op_impl(vec4x32u x) {
        return vec4x32u{_mm_popcnt_epi32(decay(x))};
    }

    auto vec4x32u_pocnt_op = vector_test_bench<vec4x32u, vec4x32u_pocnt_op_impl>;

    BENCHMARK(popcount_32u::vec4x32u_pocnt_op);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

}

#endif //AVEL_POPCOUNT_32U_HPP
