#ifndef AVEL_POPCOUNT_64U_HPP
#define AVEL_POPCOUNT_64U_HPP

namespace avel::benchmarks::popcount_64u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint64_t scalar_native_impl(std::uint64_t x) {
        return std::popcount(x);
    }

    auto scalar_native = scalar_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(popcount_64u::scalar_native);

    #endif



    std::uint64_t scalar_bitwise_accumulate_impl(std::uint64_t x) {
        std::uint64_t ret = 0;

        while (x & 0x1) {
            ret += 1;
            x >>= 1;
        }

        return ret;
    }

    auto scalar_bitwise_accumulate = scalar_test_bench<std::uint64_t, scalar_bitwise_accumulate_impl>;

    BENCHMARK(popcount_64u::scalar_bitwise_accumulate);



    std::uint64_t scalar_bitwise_accumulate_branchless_impl(std::uint64_t x) {
        std::uint64_t ret = 0;

        for (int i = 0; i < 64; ++i) {
            ret += (x >> i) & 0x1;
        }

        return ret;
    }

    auto scalar_bitwise_accumulate_branchless = scalar_test_bench<std::uint64_t, scalar_bitwise_accumulate_branchless_impl>;

    BENCHMARK(popcount_64u::scalar_bitwise_accumulate_branchless);



    std::uint64_t scalar_bitwise_divide_and_conquer_impl(std::uint64_t x) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & 0x5555555555555555);
        x = ((x >> 2) & 0x3333333333333333) + (x & 0x3333333333333333);
        x = ((x >> 4) + x) & 0x0F0F0F0F0F0F0F0F;
        x = ((x >> 8) + x) & 0x00FF00FF00FF00FF;
        x = ((x >> 16) + x) & 0x0000FFFF0000FFFF;
        x = ((x >> 32) + x) & 0x00000000FFFFFFFF;
        return x;
    }

    auto scalar_bitwise_divide_and_conquer = scalar_test_bench<std::uint64_t, scalar_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_64u::scalar_bitwise_divide_and_conquer);



    std::uint64_t scalar_nibble_lookup_impl(std::uint64_t x) {
        constexpr std::uint8_t table[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        std::uint8_t nibble00 = x & 0xf;
        std::uint8_t nibble01 = (x >> 0x04) & 0xf;
        std::uint8_t nibble02 = (x >> 0x08) & 0xf;
        std::uint8_t nibble03 = (x >> 0x0C) & 0xf;
        std::uint8_t nibble04 = (x >> 0x10) & 0xf;
        std::uint8_t nibble05 = (x >> 0x14) & 0xf;
        std::uint8_t nibble06 = (x >> 0x18) & 0xf;
        std::uint8_t nibble07 = (x >> 0x1C);
        std::uint8_t nibble08 = (x >> 0x20) & 0xf;
        std::uint8_t nibble09 = (x >> 0x24) & 0xf;
        std::uint8_t nibble10 = (x >> 0x28) & 0xf;
        std::uint8_t nibble11 = (x >> 0x2C) & 0xf;
        std::uint8_t nibble12 = (x >> 0x30) & 0xf;
        std::uint8_t nibble13 = (x >> 0x34) & 0xf;
        std::uint8_t nibble14 = (x >> 0x38) & 0xf;
        std::uint8_t nibble15 = (x >> 0x3C);

        return
            table[nibble00] + table[nibble01] + table[nibble02] + table[nibble03] +
            table[nibble04] + table[nibble05] + table[nibble06] + table[nibble07] +
            table[nibble08] + table[nibble09] + table[nibble10] + table[nibble11] +
            table[nibble12] + table[nibble13] + table[nibble14] + table[nibble15];
    }

    auto scalar_nibble_lookup = scalar_test_bench<std::uint64_t, scalar_nibble_lookup_impl>;

    BENCHMARK(popcount_64u::scalar_nibble_lookup);



    std::uint64_t scalar_byte_lookup_impl(std::uint64_t x) {
        constexpr std::uint64_t table[256] {
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
        std::uint8_t byte3 = (x >> 0x18) & 0xff;
        std::uint8_t byte4 = (x >> 0x20) & 0xff;
        std::uint8_t byte5 = (x >> 0x28) & 0xff;
        std::uint8_t byte6 = (x >> 0x30) & 0xff;
        std::uint8_t byte7 = (x >> 0x38);

        return
            table[byte0] + table[byte1] + table[byte2] + table[byte3] +
            table[byte4] + table[byte5] + table[byte6] + table[byte7];
    }

    auto scalar_byte_lookup = scalar_test_bench<std::uint64_t, scalar_byte_lookup_impl>;

    BENCHMARK(popcount_64u::scalar_byte_lookup);



    #if defined(AVEL_POPCNT)

    std::uint64_t scalar_popcnt_op_impl(std::uint64_t x) {
        return _popcnt64(std::uint64_t{x});
    }

    auto scalar_popcnt_op = scalar_test_bench<std::uint64_t, scalar_popcnt_op_impl>;

    BENCHMARK(popcount_64u::scalar_popcnt_op);

    #endif

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_bitwise_divide_and_conquer_impl(vec2x64u v) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec2x64u{0x5555555555555555});
        v = ((v >> 2) & vec2x64u{0x3333333333333333}) + (v & vec2x64u{0x3333333333333333});
        v = ((v >> 4) + v) & vec2x64u{0x0F0F0F0F0F0F0F0F};
        v = ((v >> 8) + v) & vec2x64u{0x00FF00FF00FF00FF};
        v = ((v >> 16) + v) & vec2x64u{0x0000FFFF0000FFFF};
        v = ((v >> 32) + v) & vec2x64u{0x00000000FFFFFFFF};

        return v;
    }

    auto vec2x64u_bitwise_divide_and_conquer = vector_test_bench<vec2x64u, vec2x64u_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_64u::vec2x64u_bitwise_divide_and_conquer);

    #endif



    #if defined(AVEL_SSE2) && defined(AVEL_POPCNT)

    vec2x64u vec2x64u_scalarized_popcnt_op_impl(vec2x64u v) {
        std::uint64_t lo = _mm_cvtsi128_si64(decay(v));
        std::uint64_t hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(v), 0x8));

        auto popcnt_lo = avel::popcount(lo);
        auto popcnt_hi = avel::popcount(hi);

        return vec2x64u{_mm_set_epi64x(popcnt_hi, popcnt_lo)};
    }

    auto vec2x64u_scalarized_popcnt_op = vector_test_bench<vec2x64u, vec2x64u_scalarized_popcnt_op_impl>;

    BENCHMARK(popcount_64u::vec2x64u_scalarized_popcnt_op);

    #endif



    #if defined(AVEL_SSE41) && defined(AVEL_POPCNT)

    vec2x64u vec2x64u_scalarized_popcnt_op_with_extract_impl(vec2x64u v) {
        std::uint64_t lo = _mm_cvtsi128_si64(decay(v));
        std::uint64_t hi = _mm_extract_epi64(decay(v), 0x1);

        auto popcnt_lo = avel::popcount(lo);
        auto popcnt_hi = avel::popcount(hi);

        return vec2x64u{_mm_set_epi64x(popcnt_hi, popcnt_lo)};
    }

    auto vec2x64u_scalarized_popcnt_op_with_extract = vector_test_bench<vec2x64u, vec2x64u_scalarized_popcnt_op_with_extract_impl>;

    BENCHMARK(popcount_64u::vec2x64u_scalarized_popcnt_op_with_extract);

    #endif



    #if defined(AVEL_SSSE3)

    vec2x64u vec2x64u_bitwise_divide_and_conquer_sad_impl(vec2x64u v) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        const auto m0 = _mm_set1_epi8(0x55);
        const auto m1 = _mm_set1_epi8(0x33);
        const auto m2 = _mm_set1_epi8(0x0f);

        auto t = decay(v);
        t = _mm_sub_epi8(t, _mm_and_si128(_mm_srli_epi16(t,  1), m0));
        t = _mm_add_epi8(_mm_and_si128(t, m1), _mm_and_si128(_mm_srli_epi16(t, 2), m1));
        t = _mm_and_si128(_mm_add_epi8(t, _mm_srli_epi16(t, 4)), m2);
        t = _mm_sad_epu8(t, _mm_setzero_si128());

        return vec2x64u{t};
    }

    auto vec2x64u_bitwise_divide_and_conquer_sad = vector_test_bench<vec2x64u, vec2x64u_bitwise_divide_and_conquer_sad_impl>;

    BENCHMARK(popcount_64u::vec2x64u_bitwise_divide_and_conquer_sad);

    #endif



    #if defined(AVEL_SSSE3)

    vec2x64u vec2x64u_pshufb_impl(vec2x64u x) {
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

        auto partial_sum0 = _mm_add_epi8(byte_sum, _mm_slli_epi64(byte_sum, 32));
        auto partial_sum1 = _mm_add_epi8(partial_sum0, _mm_slli_epi64(partial_sum0, 16));
        auto partial_sum2 = _mm_add_epi8(partial_sum1, _mm_slli_epi64(partial_sum1, 8));

        auto ret = _mm_srli_epi64(partial_sum2, 56);

        return vec2x64u{ret};
    }

    auto vec2x64u_pshufb = vector_test_bench<vec2x64u, vec2x64u_pshufb_impl>;

    BENCHMARK(popcount_64u::vec2x64u_pshufb);

    #endif



    #if defined(AVEL_SSSE3)

    vec2x64u vec2x64u_pshufb_sad_impl(vec2x64u x) {
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
        auto ret = _mm_sad_epu8(byte_sum, _mm_setzero_si128());
        return vec2x64u{ret};
    }

    auto vec2x64u_pshufb_sad = vector_test_bench<vec2x64u, vec2x64u_pshufb_sad_impl>;

    BENCHMARK(popcount_64u::vec2x64u_pshufb_sad);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)

    vec2x64u vec2x64u_popcnt16_op_impl(vec2x64u v) {
        auto tmp0 = _mm_popcnt_epi16(decay(v));
        auto tmp1 = _mm_slli_epi64(tmp0, 32);
        auto tmp2 = _mm_add_epi32(tmp0, tmp1);
        auto tmp3 = _mm_slli_epi32(tmp2, 16);
        auto tmp4 = _mm_add_epi16(tmp2, tmp3);

        return vec2x64u{_mm_srli_epi64(tmp4, 48)};
    }

    auto vec2x64u_popcnt16_op = vector_test_bench<vec2x64u, vec2x64u_popcnt16_op_impl>;

    BENCHMARK(popcount_64u::vec2x64u_popcnt16_op);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)

    vec2x64u vec2x64u_popcnt16_op_sad_impl(vec2x64u v) {
        auto tmp0 = _mm_popcnt_epi16(decay(v));
        return vec2x64u{_mm_sad_epu8(tmp0, _mm_setzero_si128())};
    }

    auto vec2x64u_popcnt16_op_sad = vector_test_bench<vec2x64u, vec2x64u_popcnt16_op_sad_impl>;

    BENCHMARK(popcount_64u::vec2x64u_popcnt16_op_sad);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)

    vec2x64u vec2x64u_popcnt_op_impl(vec2x64u v) {
        return vec2x64u{_mm_popcnt_epi64(decay(v))};
    }

    auto vec2x64u_popcnt_op = vector_test_bench<vec2x64u, vec2x64u_popcnt_op_impl>;

    BENCHMARK(popcount_64u::vec2x64u_popcnt_op);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_scalarized_impl(vec4x64u v) {
        auto c0 = popcount(extract<0>(v));
        auto c1 = popcount(extract<1>(v));
        auto c2 = popcount(extract<2>(v));
        auto c3 = popcount(extract<3>(v));

        return vec4x64u{_mm256_set_epi64x(c3, c2, c1, c0)};
    }

    auto vec4x64u_scalarized = vector_test_bench<vec4x64u, vec4x64u_scalarized_impl>;

    BENCHMARK(popcount_64u::vec4x64u_scalarized);

    #endif



    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_bitwise_divide_and_conquer_sad_impl(vec4x64u v) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        const auto m0 = _mm256_set1_epi8(0x55);
        const auto m1 = _mm256_set1_epi8(0x33);
        const auto m2 = _mm256_set1_epi8(0x0f);

        auto t = decay(v);
        t = _mm256_sub_epi8(t, _mm256_and_si256(_mm256_srli_epi16(t,  1), m0));
        t = _mm256_add_epi8(_mm256_and_si256(t, m1), _mm256_and_si256(_mm256_srli_epi16(t, 2), m1));
        t = _mm256_and_si256(_mm256_add_epi8(t, _mm256_srli_epi16(t, 4)), m2);
        t = _mm256_sad_epu8(t, _mm256_setzero_si256());

        return vec4x64u{t};
    }

    auto vec4x64u_bitwise_divide_and_conquer_sad = vector_test_bench<vec4x64u, vec4x64u_bitwise_divide_and_conquer_sad_impl>;

    BENCHMARK(popcount_64u::vec4x64u_bitwise_divide_and_conquer_sad);

    #endif



    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_pshufb_impl(vec4x64u x) {
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

        auto index0 = _mm256_and_si256(decay(x), nibble_mask);
        auto index1 = _mm256_and_si256(_mm256_srli_epi16(decay(x), 0x4), nibble_mask);

        auto partial_byte_sum0 = _mm256_shuffle_epi8(table, index0);
        auto partial_byte_sum1 = _mm256_shuffle_epi8(table, index1);

        auto byte_sum = _mm256_add_epi8(partial_byte_sum0, partial_byte_sum1);

        auto partial_sum0 = _mm256_add_epi8(byte_sum, _mm256_slli_epi64(byte_sum, 32));
        auto partial_sum1 = _mm256_add_epi8(partial_sum0, _mm256_slli_epi64(partial_sum0, 16));
        auto partial_sum2 = _mm256_add_epi8(partial_sum1, _mm256_slli_epi64(partial_sum1, 8));

        auto ret = _mm256_srli_epi64(partial_sum2, 56);

        return vec4x64u{ret};
    }

    auto vec4x64u_pshufb = vector_test_bench<vec4x64u, vec4x64u_pshufb_impl>;

    BENCHMARK(popcount_64u::vec4x64u_pshufb);

    #endif



    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_pshufb_sad_impl(vec4x64u x) {
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

        auto index0 = _mm256_and_si256(decay(x), nibble_mask);
        auto index1 = _mm256_and_si256(_mm256_srli_epi16(decay(x), 0x4), nibble_mask);

        auto partial_byte_sum0 = _mm256_shuffle_epi8(table, index0);
        auto partial_byte_sum1 = _mm256_shuffle_epi8(table, index1);

        auto byte_sum = _mm256_add_epi8(partial_byte_sum0, partial_byte_sum1);
        auto ret = _mm256_sad_epu8(byte_sum, _mm256_setzero_si256());
        return vec4x64u{ret};
    }

    auto vec4x64u_pshufb_sad = vector_test_bench<vec4x64u, vec4x64u_pshufb_sad_impl>;

    BENCHMARK(popcount_64u::vec4x64u_pshufb_sad);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)

    vec4x64u vec4x64u_popcnt16_op_impl(vec4x64u v) {
        auto tmp0 = _mm256_popcnt_epi16(decay(v));
        auto tmp1 = _mm256_slli_epi64(tmp0, 32);
        auto tmp2 = _mm256_add_epi32(tmp0, tmp1);
        auto tmp3 = _mm256_slli_epi32(tmp2, 16);
        auto tmp4 = _mm256_add_epi32(tmp2, tmp3);
        return vec4x64u{_mm256_srli_epi64(tmp4, 48)};
    }

    auto vec4x64u_popcnt16_op = vector_test_bench<vec4x64u, vec4x64u_popcnt16_op_impl>;

    BENCHMARK(popcount_64u::vec4x64u_popcnt16_op);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)

    vec4x64u vec4x64u_popcnt16_op_sad_impl(vec4x64u v) {
        auto tmp0 = _mm256_popcnt_epi16(decay(v));
        return vec4x64u{_mm256_sad_epu8(tmp0, _mm256_setzero_si256())};
    }

    auto vec4x64u_popcnt16_op_sad = vector_test_bench<vec4x64u, vec4x64u_popcnt16_op_sad_impl>;

    BENCHMARK(popcount_64u::vec4x64u_popcnt16_op_sad);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)

    vec4x64u vec4x64u_popcnt_op_impl(vec4x64u v) {
        return vec4x64u{_mm256_popcnt_epi64(decay(v))};
    }

    auto vec4x64u_popcnt_op = vector_test_bench<vec4x64u, vec4x64u_popcnt_op_impl>;

    BENCHMARK(popcount_64u::vec4x64u_popcnt_op);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_scalarized_impl(vec8x64u v) {
        auto c0 = popcount(extract<0>(v));
        auto c1 = popcount(extract<1>(v));
        auto c2 = popcount(extract<2>(v));
        auto c3 = popcount(extract<3>(v));
        auto c4 = popcount(extract<4>(v));
        auto c5 = popcount(extract<5>(v));
        auto c6 = popcount(extract<6>(v));
        auto c7 = popcount(extract<7>(v));

        return vec8x64u{_mm512_set_epi64(c7, c6, c5, c4, c3, c2, c1, c0)};
    }

    auto vec8x64u_scalarized = vector_test_bench<vec8x64u, vec8x64u_scalarized_impl>;

    BENCHMARK(popcount_64u::vec8x64u_scalarized);

    #endif



    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_bitwise_divide_and_conquer_impl(vec8x64u v) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec8x64u{0x5555555555555555ull});
        v = ((v >> 2) & vec8x64u{0x3333333333333333ull}) + (v & vec8x64u{0x3333333333333333ull});
        v = ((v >> 4) + v) & vec8x64u{0x0F0F0F0F0F0F0F0Full};
        v = ((v >> 8) + v) & vec8x64u{0x00FF00FF00FF00FFull};
        v = ((v >> 16) + v) & vec8x64u{0x0000FFFF0000FFFFull};
        v = ((v >> 32) + v) & vec8x64u{0x00000000FFFFFFFFull};

        return vec8x64u{v};
    }

    auto vec8x64u_bitwise_divide_and_conquer = vector_test_bench<vec8x64u, vec8x64u_bitwise_divide_and_conquer_impl>;

    BENCHMARK(popcount_64u::vec8x64u_bitwise_divide_and_conquer);

    #endif



    #if defined(AVEL_AVX512BW)

    vec8x64u vec8x64u_bitwise_divide_and_conquer_sad_impl(vec8x64u v) {
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        const auto m0 = _mm512_set1_epi8(0x55);
        const auto m1 = _mm512_set1_epi8(0x33);
        const auto m2 = _mm512_set1_epi8(0x0f);

        auto t = decay(v);
        t = _mm512_sub_epi8(t, _mm512_and_si512(_mm512_srli_epi16(t,  1), m0));
        t = _mm512_add_epi8(_mm512_and_si512(t, m1), _mm512_and_si512(_mm512_srli_epi16(t, 2), m1));
        t = _mm512_and_si512(_mm512_add_epi8(t, _mm512_srli_epi16(t, 4)), m2);
        t = _mm512_sad_epu8(t, _mm512_setzero_si512());

        return vec8x64u{t};
    }

    auto vec8x64u_bitwise_divide_and_conquer_sad = vector_test_bench<vec8x64u, vec8x64u_bitwise_divide_and_conquer_sad_impl>;

    BENCHMARK(popcount_64u::vec8x64u_bitwise_divide_and_conquer_sad);

    #endif



    #if defined(AVEL_AVX512BITALG)

    vec8x64u vec8x64u_popcnt16_op_impl(vec8x64u v) {
        auto tmp0 = _mm512_popcnt_epi16(decay(v));
        auto tmp1 = _mm512_slli_epi64(tmp0, 32);
        auto tmp2 = _mm512_add_epi32(tmp0, tmp1);
        auto tmp3 = _mm512_slli_epi32(tmp2, 16);
        auto tmp4 = _mm512_add_epi32(tmp2, tmp3);
        return vec8x64u{_mm512_srli_epi64(tmp4, 48)};
    }

    auto vec8x64u_popcnt16_op = vector_test_bench<vec8x64u, vec8x64u_popcnt16_op_impl>;

    BENCHMARK(popcount_64u::vec8x64u_popcnt16_op);

    #endif



    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG)

    vec8x64u vec8x64u_popcnt16_op_sad_impl(vec8x64u v) {
        auto tmp0 = _mm512_popcnt_epi16(decay(v));
        return vec8x64u{_mm512_sad_epu8(tmp0, _mm512_setzero_si512())};
    }

    auto vec8x64u_popcnt16_op_sad = vector_test_bench<vec8x64u, vec8x64u_popcnt16_op_sad_impl>;

    BENCHMARK(popcount_64u::vec8x64u_popcnt16_op_sad);

    #endif



    #if defined(AVEL_AVX512VPOPCNTDQ)

    vec8x64u vec8x64u_popcnt_op_impl(vec8x64u v) {
        return vec8x64u{_mm512_popcnt_epi64(decay(v))};
    }

    auto vec8x64u_popcnt_op = vector_test_bench<vec8x64u, vec8x64u_popcnt_op_impl>;

    BENCHMARK(popcount_64u::vec8x64u_popcnt_op);

    #endif

}

#endif //AVEL_POPCOUNT_64U_HPP
