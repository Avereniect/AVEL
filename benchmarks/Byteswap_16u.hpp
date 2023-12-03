#ifndef AVEL_BYTESWAP_16U_HPP
#define AVEL_BYTESWAP_16U_HPP

namespace avel::benchmarks::byteswap_16u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint16_t scalar_native_impl(std::uint16_t x) {
        return std::byteswap(x);
    }

    auto scalar_native = scalar_test_bench<std::uint16_t, scalar_native_impl>;

    BENCHMARK(byteswap_16u::scalar_native);

    #endif



    std::uint16_t scalar_rotate_impl(std::uint16_t x) {
        return avel::rotr(x, 8);
    }

    auto scalar_rotate = scalar_test_bench<std::uint16_t, scalar_rotate_impl>;

    BENCHMARK(byteswap_16u::scalar_rotate);

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec8x16u vec8x16u_shift_impl(vec8x16u v) {
        auto lo = _mm_srli_epi16(decay(v), 8);
        auto hi = _mm_slli_epi16(decay(v), 8);
        return vec8x16u{_mm_or_si128(lo, hi)};
    }

    auto vec8x16u_shift = vector_test_bench<vec8x16u, vec8x16u_shift_impl>;

    BENCHMARK(byteswap_16u::vec8x16u_shift);

    #endif



    #if defined(AVEL_SSSE3)

    vec8x16u vec8x16u_pshufb_impl(vec8x16u v) {
        alignas(16) constexpr static std::uint8_t table_data[16] {
            0x1, 0x0, 0x3, 0x2,
            0x5, 0x4, 0x7, 0x6,
            0x9, 0x8, 0xb, 0xa,
            0xc, 0xb, 0xf, 0xe
        };

        auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto ret = _mm_shuffle_epi8(decay(v), table);
        return vec8x16u{ret};
    }

    auto vec8x16u_pshufb = vector_test_bench<vec8x16u, vec8x16u_pshufb_impl>;

    BENCHMARK(byteswap_16u::vec8x16u_pshufb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)

    vec8x16u vec8x16u_concat_shift_impl(vec8x16u v) {
        return vec8x16u{_mm_shldi_epi16(decay(v), decay(v), 0x8)};
    }

    auto vec8x16u_concat_shift = vector_test_bench<vec8x16u, vec8x16u_concat_shift_impl>;

    BENCHMARK(byteswap_16u::vec8x16u_concat_shift);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_shift_impl(vec16x16u v) {
        auto lo = _mm256_srli_epi16(decay(v), 8);
        auto hi = _mm256_slli_epi16(decay(v), 8);
        return vec16x16u{_mm256_or_si256(lo, hi)};
    }

    auto vec16x16u_shift = vector_test_bench<vec16x16u, vec16x16u_shift_impl>;

    BENCHMARK(byteswap_16u::vec16x16u_shift);
    
    #endif



    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_pshufb_impl(vec16x16u v) {
        alignas(32) constexpr static std::uint8_t table_data[32] {
            0x1, 0x0, 0x3, 0x2,
            0x5, 0x4, 0x7, 0x6,
            0x9, 0x8, 0xb, 0xa,
            0xc, 0xb, 0xf, 0xe,
            0x1, 0x0, 0x3, 0x2,
            0x5, 0x4, 0x7, 0x6,
            0x9, 0x8, 0xb, 0xa,
            0xc, 0xb, 0xf, 0xe
        };

        auto table = _mm256_load_si256(reinterpret_cast<const __m256i*>(table_data));
        auto ret = _mm256_shuffle_epi8(decay(v), table);
        return vec16x16u{ret};
    }

    auto vec16x16u_pshufb = vector_test_bench<vec16x16u, vec16x16u_pshufb_impl>;

    BENCHMARK(byteswap_16u::vec16x16u_pshufb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)

    vec16x16u vec16x16u_concat_shift_impl(vec16x16u v) {
        return vec16x16u{_mm256_shldi_epi16(decay(v), decay(v), 0x8)};
    }

    auto vec16x16u_concat_shift = vector_test_bench<vec16x16u, vec16x16u_concat_shift_impl>;

    BENCHMARK(byteswap_16u::vec16x16u_concat_shift);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_shift_impl(vec32x16u v) {
        auto lo = _mm512_srli_epi16(decay(v), 8);
        auto hi = _mm512_slli_epi16(decay(v), 8);
        return vec32x16u{_mm512_or_si512(lo, hi)};
    }

    auto vec32x16u_shift = vector_test_bench<vec32x16u, vec32x16u_shift_impl>;

    BENCHMARK(byteswap_16u::vec32x16u_shift);

    #endif



    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_pshufb_impl(vec32x16u v) {
        alignas(64) constexpr static std::uint8_t table_data[64] {
            0x1, 0x0, 0x3, 0x2,
            0x5, 0x4, 0x7, 0x6,
            0x9, 0x8, 0xb, 0xa,
            0xc, 0xb, 0xf, 0xe,
            0x1, 0x0, 0x3, 0x2,
            0x5, 0x4, 0x7, 0x6,
            0x9, 0x8, 0xb, 0xa,
            0xc, 0xb, 0xf, 0xe,
            0x1, 0x0, 0x3, 0x2,
            0x5, 0x4, 0x7, 0x6,
            0x9, 0x8, 0xb, 0xa,
            0xc, 0xb, 0xf, 0xe,
            0x1, 0x0, 0x3, 0x2,
            0x5, 0x4, 0x7, 0x6,
            0x9, 0x8, 0xb, 0xa,
            0xc, 0xb, 0xf, 0xe,
        };

        auto table = _mm512_load_si512(reinterpret_cast<const __m512i*>(table_data));
        auto ret = _mm512_shuffle_epi8(decay(v), table);
        return vec32x16u{ret};
    }

    auto vec32x16u_pshufb = vector_test_bench<vec32x16u, vec32x16u_pshufb_impl>;

    BENCHMARK(byteswap_16u::vec32x16u_pshufb);

    #endif



    #if defined(AVEL_AVX512VBMI2)

    vec32x16u vec32x16u_concat_shift_impl(vec32x16u v) {
        return vec32x16u{_mm512_shldi_epi16(decay(v), decay(v), 0x8)};
    }

    auto vec32x16u_concat_shift = vector_test_bench<vec32x16u, vec32x16u_concat_shift_impl>;

    BENCHMARK(byteswap_16u::vec32x16u_concat_shift);

    #endif

}

#endif //AVEL_BYTESWAP_16U_HPP
