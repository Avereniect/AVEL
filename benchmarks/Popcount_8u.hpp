#ifndef AVEL_POPCOUNT_8U_HPP
#define AVEL_POPCOUNT_8U_HPP

namespace avel::benchmarks::popcount_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint8_t scalar_native_impl(std::uint8_t x) {
        return std::popcount(x);
    }

    auto scalar_native = scalar_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(popcount_8u::scalar_native);

    #endif



    std::uint8_t scalar_bitwise_accumulate_impl(std::uint8_t x) {
        std::uint8_t ret = 0;

        while (x & 0x1) {
            ret += 1;
            x >>= 1;
        }

        return ret;
    }

    auto scalar_bitwise_accumulate = scalar_test_bench<std::uint8_t, scalar_bitwise_accumulate_impl>;

    BENCHMARK(popcount_8u::scalar_bitwise_accumulate);



    std::uint8_t scalar_bitwise_accumulate_branchless_impl(std::uint8_t x) {
        std::uint8_t ret = 0;

        for (int i = 0; i < 8; ++i) {
            ret += (x >> i) & 0x1;
        }

        return ret;
    }

    auto scalar_bitwise_accumulate_branchless = scalar_test_bench<std::uint8_t, scalar_bitwise_accumulate_branchless_impl>;

    BENCHMARK(popcount_8u::scalar_bitwise_accumulate_branchless);



    std::uint8_t scalar_bitwise_divide_and_conquer_impl(std::uint8_t x) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & 0x55);
        x = (x & 0x33) + ((x >> 2) & 0x33);
        x = (x + (x >> 4)) & 0x0F;
        return x;
    }

    auto scalar_bitwise_divide_and_conquer = scalar_test_bench<std::uint8_t, scalar_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_8u::scalar_bitwise_divide_and_conquer);



    std::uint8_t scalar_nibble_lookup_impl(std::uint8_t x) {
        constexpr std::uint8_t table[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        std::uint8_t nibble0 = x & 0xf;
        std::uint8_t nibble1 = x >> 4;

        return table[nibble0] + table[nibble1];
    }

    auto scalar_nibble_lookup = scalar_test_bench<std::uint8_t, scalar_nibble_lookup_impl>;

    BENCHMARK(popcount_8u::scalar_nibble_lookup);



    std::uint8_t scalar_byte_lookup_impl(std::uint8_t x) {
        constexpr std::uint8_t table[256] {
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

        return table[x];
    }

    auto scalar_byte_lookup = scalar_test_bench<std::uint8_t, scalar_byte_lookup_impl>;

    BENCHMARK(popcount_8u::scalar_byte_lookup);



    #if defined(AVEL_POPCNT)

    std::uint8_t scalar_popcnt_op_impl(std::uint8_t x) {
        return _popcnt32(std::uint32_t{x});
    }

    auto scalar_popcnt_op = scalar_test_bench<std::uint8_t, scalar_popcnt_op_impl>;

    BENCHMARK(popcount_8u::scalar_popcnt_op);

    #endif

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_bitwise_divide_and_conquer_impl(vec16x8u v) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        const auto m0 = _mm_set1_epi8(0x55);
        const auto m1 = _mm_set1_epi8(0x33);
        const auto m2 = _mm_set1_epi8(0x0f);

        auto t = decay(v);
        t = _mm_sub_epi8(t, _mm_and_si128(_mm_srli_epi16(t,  1), m0));
        t = _mm_add_epi8(_mm_and_si128(t, m1), _mm_and_si128(_mm_srli_epi16(t, 2), m1));
        t = _mm_and_si128(_mm_add_epi8(t, _mm_srli_epi16(t, 4)), m2);

        return vec16x8u{t};
    }

    auto vec16x8u_bitwise_divide_and_conquer = vector_test_bench<vec16x8u, vec16x8u_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_8u::vec16x8u_bitwise_divide_and_conquer);

    #endif



    #if defined(AVEL_SSSE3)

    vec16x8u vec16x8u_pshufb_impl(vec16x8u v) {
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

        auto sum = _mm_add_epi8(partial_sum0, partial_sum1);
        return vec16x8u{sum};
    }

    auto vec16x8u_pshufb = vector_test_bench<vec16x8u, vec16x8u_pshufb_impl>;

    BENCHMARK(popcount_8u::vec16x8u_pshufb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)

    vec16x8u vec16x8u_popcnt_op_impl(vec16x8u v) {
        return vec16x8u{_mm_popcnt_epi8(decay(v))};
    }

    auto vec16x8u_popcnt_op = vector_test_bench<vec16x8u, vec16x8u_popcnt_op_impl>;

    BENCHMARK(popcount_8u::vec16x8u_popcnt_op);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_bitwise_divide_and_conquer_impl(vec32x8u v) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        const auto m0 = _mm256_set1_epi8(0x55);
        const auto m1 = _mm256_set1_epi8(0x33);
        const auto m2 = _mm256_set1_epi8(0x0f);

        auto t = decay(v);
        t = _mm256_sub_epi8(t, _mm256_and_si256(_mm256_srli_epi16(t,  1), m0));
        t = _mm256_add_epi8(_mm256_and_si256(t, m1), _mm256_and_si256(_mm256_srli_epi16(t, 2), m1));
        t = _mm256_and_si256(_mm256_add_epi8(t, _mm256_srli_epi16(t, 4)), m2);

        return vec32x8u{t};
    }

    auto vec32x8u_bitwise_divide_and_conquer = vector_test_bench<vec32x8u, vec32x8u_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_8u::vec32x8u_bitwise_divide_and_conquer);

    #endif



    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_pshufb_impl(vec32x8u v) {
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        auto table_lo = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto table = _mm256_set_m128i(table_lo, table_lo);
        auto nibble_mask = _mm256_set1_epi8(0x0F);

        auto index0 = _mm256_and_si256(decay(v), nibble_mask);
        auto index1 = _mm256_and_si256(_mm256_srli_epi16(decay(v), 0x4), nibble_mask);

        auto partial_sum0 = _mm256_shuffle_epi8(table, index0);
        auto partial_sum1 = _mm256_shuffle_epi8(table, index1);

        auto sum = _mm256_add_epi8(partial_sum0, partial_sum1);
        return vec32x8u{sum};
    }

    auto vec32x8u_pshufb = vector_test_bench<vec32x8u, vec32x8u_pshufb_impl>;

    BENCHMARK(popcount_8u::vec32x8u_pshufb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)

    vec32x8u vec32x8u_popcnt_op_impl(vec32x8u v) {
        return vec32x8u{_mm256_popcnt_epi8(decay(v))};
    }

    auto vec32x8u_popcnt_op = vector_test_bench<vec32x8u, vec32x8u_popcnt_op_impl>;

    BENCHMARK(popcount_8u::vec32x8u_popcnt_op);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_bitwise_divide_and_conquer_impl(vec64x8u v) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        const auto m0 = _mm512_set1_epi8(0x55);
        const auto m1 = _mm512_set1_epi8(0x33);
        const auto m2 = _mm512_set1_epi8(0x0f);

        auto t = decay(v);
        t = _mm512_sub_epi8(t, _mm512_and_si512(_mm512_srli_epi16(t,  1), m0));
        t = _mm512_add_epi8(_mm512_and_si512(t, m1), _mm512_and_si512(_mm512_srli_epi16(t, 2), m1));
        t = _mm512_and_si512(_mm512_add_epi8(t, _mm512_srli_epi16(t, 4)), m2);

        return vec64x8u{t};
    }

    auto vec64x8u_bitwise_divide_and_conquer = vector_test_bench<vec64x8u, vec64x8u_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_8u::vec64x8u_bitwise_divide_and_conquer);

    #endif



    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_pshufb_impl(vec64x8u v) {
        alignas(64) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4,
        };

        auto table = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data)));

        auto lo_nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(decay(v), lo_nibble_mask);
        auto hi_nibble = _mm512_and_si512(_mm512_srli_epi16(decay(v), 4), lo_nibble_mask);

        auto lo_popcnt = _mm512_shuffle_epi8(table, lo_nibble);
        auto hi_popcnt = _mm512_shuffle_epi8(table, hi_nibble);

        auto popcnt = _mm512_add_epi8(lo_popcnt, hi_popcnt);
        return vec64x8u{popcnt};
    }

    auto vec64x8u_pshufb = vector_test_bench<vec64x8u, vec64x8u_pshufb_impl>;

    BENCHMARK(popcount_8u::vec64x8u_pshufb);

    #endif



    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)

    vec64x8u vec64x8u_vpermi2b_impl(vec64x8u v) {
        alignas(128) static constexpr std::uint8_t table[128]{
            0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
        };

        auto table0 = _mm512_load_si512(table + 0x00);
        auto table1 = _mm512_load_si512(table + 0x40);

        auto msb_mask = _mm512_movepi8_mask(decay(v));
        auto table_results = _mm512_permutex2var_epi8(table0, decay(v), table1);
        auto results = _mm512_mask_add_epi8(table_results, msb_mask, table_results, _mm512_set1_epi8(0x01));

        return vec64x8u{results};
    }

    auto vec64x8u_vpermi2b = vector_test_bench<vec64x8u, vec64x8u_vpermi2b_impl>;

    BENCHMARK(popcount_8u::vec64x8u_vpermi2b);

    #endif



    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG)

    vec64x8u vec64x8u_popcnt_op_impl(vec64x8u v) {
        return vec64x8u{_mm512_popcnt_epi8(decay(v))};
    }

    auto vec64x8u_popcnt_op = vector_test_bench<vec64x8u, vec64x8u_popcnt_op_impl>;

    BENCHMARK(popcount_8u::vec64x8u_popcnt_op);

    #endif

}

#endif //AVEL_POPCOUNT_8U_HPP
