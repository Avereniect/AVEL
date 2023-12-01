#ifndef AVEL_BIT_CEIL_8U_HPP
#define AVEL_BIT_CEIL_8U_HPP

namespace avel::benchmarks::bit_ceil_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint8_t scalar_native_impl(std::uint8_t x) {
        return std::bit_ceil(x);
    }

    auto scalar_native = scalar_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(bit_ceil_8u::scalar_native);

    #endif



    std::uint8_t scalar_bitwise_impl(std::uint8_t x) {
        if (x == 0) {
            return 1;
        }

        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        ++x;
        return x;
    }

    auto scalar_bitwise = scalar_test_bench<std::uint8_t, scalar_bitwise_impl>;

    BENCHMARK(bit_ceil_8u::scalar_bitwise);



    #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint8_t scalar_bsr_and_shift_impl(std::uint8_t x) {
        if (x == 0) {
            return 1;
        }

        auto width = _bit_scan_reverse(x);
        auto tmp = 1 << width;
        return tmp << (tmp != x);
    }

    auto scalar_bsr_and_shift = scalar_test_bench<std::uint8_t, scalar_bsr_and_shift_impl>;

    BENCHMARK(bit_ceil_8u::scalar_bsr_and_shift);

    #endif



    #if defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint8_t scalar_lzcnt_and_rotate_impl(std::uint8_t x) {
        auto sh = (32 - _lzcnt_u32(x - 1));
        auto result = _rotl(0x01010101, sh);
        return result;
    }

    auto scalar_lzcnt_and_rotate = scalar_test_bench<std::uint8_t, scalar_lzcnt_and_rotate_impl>;

    BENCHMARK(bit_ceil_8u::scalar_lzcnt_and_rotate);

    #endif

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_bitwise_impl(vec16x8u v) {
        auto zero_mask = (v == vec16x8u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec16x8u_bitwise = vector_test_bench<vec16x8u, vec16x8u_bitwise_impl>;

    BENCHMARK(bit_ceil_8u::vec16x8u_bitwise);

    #endif



    #if defined(AVEL_SSSE3)

    vec16x8u vec16x8u_pshufb_impl(vec16x8u v) {
        alignas(16) static constexpr std::uint8_t table_data0[16] {
             0,  1,  3,  3,
             7,  7,  7,  7,
            15, 15, 15, 15,
            15, 15, 15, 15
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
              0,  31,  63,  63,
            127, 127, 127, 127,
            255, 255, 255, 255,
            255, 255, 255, 255
        };

        auto zero_mask = _mm_cmpeq_epi8(decay(v), _mm_setzero_si128());

        --v;

        auto nibble_mask = _mm_set1_epi8(0x0F);
        auto lo_nibble = _mm_and_si128(nibble_mask, decay(v));
        auto hi_nibble = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(v), 0x4));

        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data0));
        auto table1 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data1));

        auto partial0 = _mm_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm_add_epi8(_mm_max_epu8(partial0, partial1), _mm_set1_epi8(0x01));
        return vec16x8u{_mm_sub_epi8(ret, zero_mask)};
    }

    auto vec16x8u_pshufb = vector_test_bench<vec16x8u, vec16x8u_pshufb_impl>;

    BENCHMARK(bit_ceil_8u::vec16x8u_pshufb);

    #endif



    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_pshufb_impl(vec32x8u v) {
        alignas(16) static constexpr std::uint8_t table_data0[16] {
             0,  1,  3,  3,
             7,  7,  7,  7,
            15, 15, 15, 15,
            15, 15, 15, 15
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
              0,  31,  63,  63,
            127, 127, 127, 127,
            255, 255, 255, 255,
            255, 255, 255, 255
        };

        auto zero_mask = _mm256_cmpeq_epi8(decay(v), _mm256_setzero_si256());

        --v;

        auto nibble_mask = _mm256_set1_epi8(0x0F);
        auto lo_nibble = _mm256_and_si256(nibble_mask, decay(v));
        auto hi_nibble = _mm256_and_si256(nibble_mask, _mm256_srli_epi16(decay(v), 0x4));

        auto table0 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm256_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm256_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm256_add_epi8(_mm256_max_epu8(partial0, partial1), _mm256_set1_epi8(0x01));
        return vec32x8u{_mm256_sub_epi8(ret, zero_mask)};
    }

    auto vec32x8u_pshufb = vector_test_bench<vec32x8u, vec32x8u_pshufb_impl>;

    BENCHMARK(bit_ceil_8u::vec32x8u_pshufb);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_pshufb_impl(vec64x8u v) {
        alignas(16) static constexpr std::uint8_t table_data0[16] {
             0,  1,  3,  3,
             7,  7,  7,  7,
            15, 15, 15, 15,
            15, 15, 15, 15
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
              0,  31,  63,  63,
            127, 127, 127, 127,
            255, 255, 255, 255,
            255, 255, 255, 255
        };

        auto zero_mask = _mm512_cmpeq_epi8_mask(decay(v), _mm512_setzero_si512());

        --v;

        auto nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(nibble_mask, decay(v));
        auto hi_nibble = _mm512_and_si512(nibble_mask, _mm512_srli_epi16(decay(v), 0x4));

        auto table0 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm512_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm512_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm512_add_epi8(_mm512_max_epu8(partial0, partial1), _mm512_set1_epi8(0x01));
        return vec64x8u{_mm512_add_epi8(ret, _mm512_maskz_set1_epi8(zero_mask, 1))};
    }

    auto vec64x8u_pshufb = vector_test_bench<vec64x8u, vec64x8u_pshufb_impl>;

    BENCHMARK(bit_ceil_8u::vec64x8u_pshufb);

    #endif

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)

    vec64x8u vec64x8u_vpermi2b_impl(vec64x8u v) {
        alignas(16) static constexpr std::uint8_t table_data0[16] {
             0,  1,  3,  3,
             7,  7,  7,  7,
            15, 15, 15, 15,
            15, 15, 15, 15
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
              0,  31,  63,  63,
            127, 127, 127, 127,
            255, 255, 255, 255,
            255, 255, 255, 255
        };

        auto zero_mask = _mm512_cmpeq_epi8_mask(decay(v), _mm512_setzero_si512());

        --v;

        auto nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(nibble_mask, decay(v));
        auto hi_nibble = _mm512_and_si512(nibble_mask, _mm512_srli_epi16(decay(v), 0x4));

        auto table0 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm512_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm512_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm512_add_epi8(_mm512_max_epu8(partial0, partial1), _mm512_set1_epi8(0x01));
        return vec64x8u{_mm512_add_epi8(ret, _mm512_maskz_set1_epi8(zero_mask, 1))};
    }

    auto vec64x8u_vpermi2b = vector_test_bench<vec64x8u, vec64x8u_vpermi2b_impl>;

    BENCHMARK(bit_ceil_8u::vec64x8u_vpermi2b);

    #endif

}

#endif //AVEL_BIT_CEIL_8U_HPP
