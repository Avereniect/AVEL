#ifndef AVEL_POPCOUNT_16U_HPP
#define AVEL_POPCOUNT_16U_HPP

namespace avel::benchmarks::popcount_16u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint16_t scalar_native_impl(std::uint16_t x) {
        return std::popcount(x);
    }

    auto scalar_native = scalar_test_bench<std::uint16_t, scalar_native_impl>;

    BENCHMARK(popcount_16u::scalar_native);

    #endif



    std::uint16_t scalar_bitwise_accumulate_impl(std::uint16_t x) {
        std::uint16_t ret = 0;

        while (x & 0x1) {
            ret += 1;
            x >>= 1;
        }

        return ret;
    }

    auto scalar_bitwise_accumulate = scalar_test_bench<std::uint16_t, scalar_bitwise_accumulate_impl>;

    BENCHMARK(popcount_16u::scalar_bitwise_accumulate);



    std::uint16_t scalar_bitwise_accumulate_branchless_impl(std::uint16_t x) {
        std::uint16_t ret = 0;

        for (int i = 0; i < 16; ++i) {
            ret += (x >> i) & 0x1;
        }

        return ret;
    }

    auto scalar_bitwise_accumulate_branchless = scalar_test_bench<std::uint16_t, scalar_bitwise_accumulate_branchless_impl>;

    BENCHMARK(popcount_16u::scalar_bitwise_accumulate_branchless);



    std::uint16_t scalar_bitwise_divide_and_conquer_impl(std::uint16_t x) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & 0x5555);
        x = (x & 0x3333) + ((x >> 2) & 0x3333);
        x = (x + (x >> 4) & 0x0F0F);
        x = (x + (x >> 8) & 0x00FF);
        return x;
    }

    auto scalar_bitwise_divide_and_conquer = scalar_test_bench<std::uint16_t, scalar_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_16u::scalar_bitwise_divide_and_conquer);



    std::uint16_t scalar_nibble_lookup_impl(std::uint16_t x) {
        constexpr std::uint8_t table[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        std::uint8_t nibble0 = x & 0xf;
        std::uint8_t nibble1 = (x >> 0x4) & 0xf;
        std::uint8_t nibble2 = (x >> 0x8) & 0xf;
        std::uint8_t nibble3 = (x >> 0xC);

        return table[nibble0] + table[nibble1] + table[nibble2] + table[nibble3];
    }

    auto scalar_nibble_lookup = scalar_test_bench<std::uint16_t, scalar_nibble_lookup_impl>;

    BENCHMARK(popcount_16u::scalar_nibble_lookup);



    std::uint16_t scalar_byte_lookup_impl(std::uint16_t x) {
        constexpr std::uint16_t table[256] {
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
        std::uint8_t byte1 = x >> 8;

        return table[byte0] + table[byte1];
    }

    auto scalar_byte_lookup = scalar_test_bench<std::uint16_t, scalar_byte_lookup_impl>;

    BENCHMARK(popcount_16u::scalar_byte_lookup);



    #if defined(AVEL_POPCNT)

    std::uint16_t scalar_popcnt_op_impl(std::uint16_t x) {
        return _popcnt32(std::uint32_t{x});
    }

    auto scalar_popcnt_op = scalar_test_bench<std::uint16_t, scalar_popcnt_op_impl>;

    BENCHMARK(popcount_16u::scalar_popcnt_op);

    #endif

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec8x16u vec8x16u_bitwise_divide_and_conquer_impl(vec8x16u v) {
        v = v - ((v >> 1) & vec8x16u{0x5555});
        v = (v & vec8x16u{0x3333}) + ((v >> 2) & vec8x16u{0x3333});
        v = ((v + (v >> 4) & vec8x16u{0x0F0F}) * vec8x16u{0x0101}) >> 8;
        return v;
    }

    auto vec8x16u_bitwise_divide_and_conquer = vector_test_bench<vec8x16u, vec8x16u_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_16u::vec8x16u_bitwise_divide_and_conquer);

    #endif

    #if defined(AVEL_SSSE3)

    vec8x16u vec8x16u_pshufb_impl(vec8x16u v) {
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto nibble_mask = _mm_set1_epi8(0x0F);

        auto index0 = _mm_and_si128(decay(v), nibble_mask);
        auto index1 = _mm_and_si128(_mm_srli_epi16(decay(v), 0x4), nibble_mask);

        auto partial_sum0 = _mm_shuffle_epi8(table, index0);
        auto partial_sum1 = _mm_shuffle_epi8(table, index1);

        auto byte_sums = _mm_add_epi8(partial_sum0, partial_sum1);

        auto short_sums = _mm_add_epi16(byte_sums, _mm_slli_epi16(byte_sums, 8));
        auto ret = _mm_srli_epi16(short_sums, 8);

        return vec8x16u{ret};
    }

    auto vec8x16u_pshufb = vector_test_bench<vec8x16u, vec8x16u_pshufb_impl>;

    BENCHMARK(popcount_16u::vec8x16u_pshufb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)

    vec8x16u vec8x16u_popcnt_op_impl(vec8x16u v) {
        return vec8x16u{_mm_popcnt_epi16(decay(v))};
    }

    auto vec8x16u_popcnt_op = vector_test_bench<vec8x16u, vec8x16u_popcnt_op_impl>;

    BENCHMARK(popcount_16u::vec8x16u_popcnt_op);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_pshufb_impl(vec16x16u v) {
        alignas(32) static constexpr std::uint8_t table_data[32] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4,
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        auto table = _mm256_load_si256(reinterpret_cast<const __m256i*>(table_data));
        auto nibble_mask = _mm256_set1_epi8(0x0F);

        auto index0 = _mm256_and_si256(decay(v), nibble_mask);
        auto index1 = _mm256_and_si256(_mm256_srli_epi16(decay(v), 0x4), nibble_mask);

        auto partial_sum0 = _mm256_shuffle_epi8(table, index0);
        auto partial_sum1 = _mm256_shuffle_epi8(table, index1);

        auto byte_sums = _mm256_add_epi8(partial_sum0, partial_sum1);

        auto short_sums = _mm256_add_epi16(byte_sums, _mm256_slli_epi16(byte_sums, 8));
        auto ret = _mm256_srli_epi16(short_sums, 8);

        return vec16x16u{ret};
    }

    auto vec16x16u_pshufb = vector_test_bench<vec16x16u, vec16x16u_pshufb_impl>;

    BENCHMARK(popcount_16u::vec16x16u_pshufb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)

    vec16x16u vec16x16u_popcnt_op_impl(vec16x16u v) {
        return vec16x16u{_mm256_popcnt_epi16(decay(v))};
    }

    auto vec16x16u_popcnt_op = vector_test_bench<vec16x16u, vec16x16u_popcnt_op_impl>;

    BENCHMARK(popcount_16u::vec16x16u_popcnt_op);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_pshufb_impl(vec32x16u v) {
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        // TODO: Consider replacing high-latency _mm512_broadcast_i32x4
        auto table = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data)));
        auto nibble_mask = _mm512_set1_epi8(0x0F);

        auto index0 = _mm512_and_si512(decay(v), nibble_mask);
        auto index1 = _mm512_and_si512(_mm512_srli_epi16(decay(v), 0x4), nibble_mask);

        auto partial_sum0 = _mm512_shuffle_epi8(table, index0);
        auto partial_sum1 = _mm512_shuffle_epi8(table, index1);

        auto byte_sums = _mm512_add_epi8(partial_sum0, partial_sum1);

        auto short_sums = _mm512_add_epi16(byte_sums, _mm512_slli_epi16(byte_sums, 8));
        auto ret = _mm512_srli_epi16(short_sums, 8);

        return vec32x16u{ret};
    }

    auto vec32x16u_pshufb = vector_test_bench<vec32x16u, vec32x16u_pshufb_impl>;

    BENCHMARK(popcount_16u::vec32x16u_pshufb);

    #endif



    #if defined(AVEL_AVX512BITALG)

    vec32x16u vec32x16u_popcnt_op_impl(vec32x16u v) {
        return vec32x16u{_mm512_popcnt_epi16(decay(v))};
    }

    auto vec32x16u_popcnt_op = vector_test_bench<vec32x16u, vec32x16u_popcnt_op_impl>;

    BENCHMARK(popcount_16u::vec32x16u_popcnt_op);

    #endif

}

#endif //AVEL_POPCOUNT_16U_HPP
