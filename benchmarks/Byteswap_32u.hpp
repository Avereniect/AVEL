#ifndef AVEL_BYTESWAP_32U_HPP
#define AVEL_BYTESWAP_32U_HPP

namespace avel::benchmarks::byteswap_32u {

    //=====================================================
    // scalar 32u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint32_t scalar_native_impl(std::uint32_t x) {
        return std::byteswap(x);
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(byteswap_32u::scalar_native);

    #endif



    std::uint32_t scalar_shift_impl(std::uint32_t x) {
        std::uint32_t b0 = x << 24;
        std::uint32_t b1 = (x << 8) & 0x00ff0000;
        std::uint32_t b2 = (x >> 8) & 0x0000ff00;
        std::uint32_t b3 = x >> 24;
        return b0 + b1 + b2 + b3;
    }

    auto scalar_shift = scalar_test_bench<std::uint32_t, scalar_shift_impl>;

    BENCHMARK(byteswap_32u::scalar_shift);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_shuffle_shift_impl(vec4x32u v) {
        auto t0 = _mm_shufflelo_epi16(decay(v), 0xB1);
        auto t1 = _mm_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm_slli_epi16(t1, 0x8);
        auto t3 = _mm_srli_epi16(t1, 0x8);
        return vec4x32u{_mm_or_si128(t2, t3)};
    }

    auto vec4x32u_shuffle_shift = vector_test_bench<vec4x32u, vec4x32u_shuffle_shift_impl>;

    BENCHMARK(byteswap_32u::vec4x32u_shuffle_shift);

    #endif



    #if defined(AVEL_SSSE3)

    vec4x32u vec4x32u_pshufb_impl(vec4x32u v) {
        alignas(16) static constexpr std::uint8_t index_data[16] {
            0x3, 0x2, 0x1, 0x0,
            0x7, 0x6, 0x5, 0x4,
            0xb, 0xa, 0x9, 0x8,
            0xf, 0xe, 0xd, 0xc
        };

        auto indices = _mm_load_si128((const __m128i*)index_data);
        return vec4x32u{_mm_shuffle_epi8(decay(v), indices)};
    }

    auto vec4x32u_pshufb = vector_test_bench<vec4x32u, vec4x32u_pshufb_impl>;

    BENCHMARK(byteswap_32u::vec4x32u_pshufb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)

    vec4x32u vec4x32u_concat_shift_impl(vec4x32u v) {
        // Reverse 16-bit halves within 32-bit ints
        auto t0 = _mm_shldi_epi32(decay(v), decay(v), 16);

        // Reverse 8-bit halves within 16-bit ints
        auto t1 = _mm_shldi_epi16(t0, t0, 8);

        return vec4x32u{t1};
    }

    auto vec4x32u_concat_shift = vector_test_bench<vec4x32u, vec4x32u_concat_shift_impl>;

    BENCHMARK(byteswap_32u::vec4x32u_concat_shift);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_shuffle_shift_impl(vec8x32u v) {
        auto t0 = _mm256_shufflelo_epi16(decay(v), 0xB1);
        auto t1 = _mm256_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm256_slli_epi16(t1, 0x8);
        auto t3 = _mm256_srli_epi16(t1, 0x8);
        return vec8x32u{_mm256_or_si256(t2, t3)};
    }

    auto vec8x32u_shuffle_shift = vector_test_bench<vec8x32u, vec8x32u_shuffle_shift_impl>;

    BENCHMARK(byteswap_32u::vec8x32u_shuffle_shift);

    #endif



    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_pshufb_impl(vec8x32u v) {
        alignas(32) static constexpr std::uint8_t index_data[32] {
            0x3, 0x2, 0x1, 0x0,
            0x7, 0x6, 0x5, 0x4,
            0xb, 0xa, 0x9, 0x8,
            0xf, 0xe, 0xd, 0xc,
            0x3, 0x2, 0x1, 0x0,
            0x7, 0x6, 0x5, 0x4,
            0xb, 0xa, 0x9, 0x8,
            0xf, 0xe, 0xd, 0xc
        };

        auto indices = _mm256_load_si256(reinterpret_cast<const __m256i*>(index_data));
        return vec8x32u{_mm256_shuffle_epi8(decay(v), indices)};
    }

    auto vec8x32u_pshufb = vector_test_bench<vec8x32u, vec8x32u_pshufb_impl>;

    BENCHMARK(byteswap_32u::vec8x32u_pshufb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)

    vec8x32u vec8x32u_concat_shift_impl(vec8x32u v) {
        // Reverse 16-bit halves within 32-bit ints
        auto t0 = _mm256_shldi_epi32(decay(v), decay(v), 16);

        // Reverse 8-bit halves within 16-bit ints
        auto t1 = _mm256_shldi_epi16(t0, t0, 8);

        return vec8x32u{t1};
    }

    auto vec8x32u_concat_shift = vector_test_bench<vec8x32u, vec8x32u_concat_shift_impl>;

    BENCHMARK(byteswap_32u::vec8x32u_concat_shift);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_shuffle_shift_impl(vec16x32u v) {
        auto t0 = _mm512_shufflelo_epi16(decay(v), 0xB1);
        auto t1 = _mm512_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm512_slli_epi16(t1, 0x8);
        auto t3 = _mm512_srli_epi16(t1, 0x8);
        return vec16x32u{_mm512_or_si512(t2, t3)};
    }

    auto vec16x32u_shuffle_shift = vector_test_bench<vec16x32u, vec16x32u_shuffle_shift_impl>;

    BENCHMARK(byteswap_32u::vec16x32u_shuffle_shift);

    #endif



    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_pshufb_impl(vec16x32u v) {
        alignas(64) static constexpr std::uint8_t index_data[64] {
            0x3, 0x2, 0x1, 0x0,
            0x7, 0x6, 0x5, 0x4,
            0xb, 0xa, 0x9, 0x8,
            0xf, 0xe, 0xd, 0xc,
            0x3, 0x2, 0x1, 0x0,
            0x7, 0x6, 0x5, 0x4,
            0xb, 0xa, 0x9, 0x8,
            0xf, 0xe, 0xd, 0xc,
            0x3, 0x2, 0x1, 0x0,
            0x7, 0x6, 0x5, 0x4,
            0xb, 0xa, 0x9, 0x8,
            0xf, 0xe, 0xd, 0xc,
            0x3, 0x2, 0x1, 0x0,
            0x7, 0x6, 0x5, 0x4,
            0xb, 0xa, 0x9, 0x8,
            0xf, 0xe, 0xd, 0xc,
        };

        auto indices = _mm512_load_si512(reinterpret_cast<const __m512i*>(index_data));
        return vec16x32u{_mm512_shuffle_epi8(decay(v), indices)};
    }

    auto vec16x32u_pshufb = vector_test_bench<vec16x32u, vec16x32u_pshufb_impl>;

    BENCHMARK(byteswap_32u::vec16x32u_pshufb);

    #endif



    #if defined(AVEL_AVX512VBMI2)

    vec16x32u vec16x32u_concat_shift_impl(vec16x32u v) {
        // Reverse 16-bit halves within 32-bit ints
        auto t0 = _mm512_shldi_epi32(decay(v), decay(v), 16);

        // Reverse 8-bit halves within 16-bit ints
        auto t1 = _mm512_shldi_epi16(t0, t0, 8);

        return vec16x32u{t1};
    }

    auto vec16x32u_concat_shift = vector_test_bench<vec16x32u, vec16x32u_concat_shift_impl>;

    BENCHMARK(byteswap_32u::vec16x32u_concat_shift);

    #endif
    
}

#endif //AVEL_BYTESWAP_32U_HPP
