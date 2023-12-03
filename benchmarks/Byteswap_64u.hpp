#ifndef AVEL_BYTESWAP_64U_HPP
#define AVEL_BYTESWAP_64U_HPP

namespace avel::benchmarks::byteswap_64u {

    //=====================================================
    // scalar 64u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint64_t scalar_native_impl(std::uint64_t x) {
        return std::byteswap(x);
    }

    auto scalar_native = scalar_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(byteswap_32u::scalar_native);

    #endif



    std::uint64_t scalar_shift_impl(std::uint64_t x) {
        std::uint64_t t0 = (x >> 56);
        std::uint64_t t1 = (x >> 40) & (0x000000000000FF00ul);
        std::uint64_t t2 = (x >> 24) & (0x0000000000FF0000ul);
        std::uint64_t t3 = (x >>  8) & (0x00000000FF000000ul);
        std::uint64_t t4 = (x <<  8) & (0x000000FF00000000ul);
        std::uint64_t t5 = (x << 24) & (0x0000FF0000000000ul);
        std::uint64_t t6 = (x << 40) & (0x00FF000000000000ul);
        std::uint64_t t7 = (x << 56);

        return t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7;
    }

    auto scalar_shift = scalar_test_bench<std::uint64_t, scalar_shift_impl>;

    BENCHMARK(byteswap_64u::scalar_shift);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_shuffle_shift_impl(vec2x64u v) {
        auto t0 = _mm_shufflelo_epi16(decay(v), 0xB1);
        auto t1 = _mm_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm_slli_epi16(t1, 0x8);
        auto t3 = _mm_srli_epi16(t1, 0x8);
        return vec2x64u{_mm_or_si128(t2, t3)};
    }

    auto vec2x64u_shuffle_shift = vector_test_bench<vec2x64u, vec2x64u_shuffle_shift_impl>;

    BENCHMARK(byteswap_64u::vec2x64u_shuffle_shift);

    #endif



    #if defined(AVEL_SSSE3)

    vec2x64u vec2x64u_pshufb_impl(vec2x64u v) {
        alignas(16) static constexpr std::uint8_t index_data[16] {
            0x7, 0x6, 0x5, 0x4,
            0x3, 0x2, 0x1, 0x0,
            0xf, 0xe, 0xd, 0xc,
            0xb, 0xa, 0x9, 0x8
        };

        auto indices = _mm_load_si128((const __m128i*)index_data);
        return vec2x64u{_mm_shuffle_epi8(decay(v), indices)};
    }

    auto vec2x64u_pshufb = vector_test_bench<vec2x64u, vec2x64u_pshufb_impl>;

    BENCHMARK(byteswap_64u::vec2x64u_pshufb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)

    vec2x64u vec2x64u_concat_shift_impl(vec2x64u v) {
        // Reverse 16-bit halves within 32-bit ints
        auto t0 = _mm_shldi_epi64(decay(v), decay(v), 32);

        // Reverse 16-bit halves within 32-bit ints
        auto t1 = _mm_shldi_epi32(t0, t0, 16);

        // Reverse 8-bit halves within 16-bit ints
        auto t2 = _mm_shldi_epi16(t1, t1, 8);

        return vec2x64u{t2};
    }

    auto vec2x64u_concat_shift = vector_test_bench<vec2x64u, vec2x64u_concat_shift_impl>;

    BENCHMARK(byteswap_64u::vec2x64u_concat_shift);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_shuffle_shift_impl(vec4x64u v) {
        auto t0 = _mm256_shufflelo_epi16(decay(v), 0xB1);
        auto t1 = _mm256_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm256_slli_epi16(t1, 0x8);
        auto t3 = _mm256_srli_epi16(t1, 0x8);
        return vec4x64u{_mm256_or_si256(t2, t3)};
    }

    auto vec4x64u_shuffle_shift = vector_test_bench<vec4x64u, vec4x64u_shuffle_shift_impl>;

    BENCHMARK(byteswap_64u::vec4x64u_shuffle_shift);

    #endif



    #if defined(AVEL_SSSE3)

    vec4x64u vec4x64u_pshufb_impl(vec4x64u v) {
        alignas(32) static constexpr std::uint8_t index_data[32] {
            0x7, 0x6, 0x5, 0x4,
            0x3, 0x2, 0x1, 0x0,
            0xf, 0xe, 0xd, 0xc,
            0xb, 0xa, 0x9, 0x8,
            0x7, 0x6, 0x5, 0x4,
            0x3, 0x2, 0x1, 0x0,
            0xf, 0xe, 0xd, 0xc,
            0xb, 0xa, 0x9, 0x8
        };

        auto indices = _mm256_load_si256((const __m256i*)index_data);
        return vec4x64u{_mm256_shuffle_epi8(decay(v), indices)};
    }

    auto vec4x64u_pshufb = vector_test_bench<vec4x64u, vec4x64u_pshufb_impl>;

    BENCHMARK(byteswap_64u::vec4x64u_pshufb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)

    vec4x64u vec4x64u_concat_shift_impl(vec4x64u v) {
        // Reverse 16-bit halves within 32-bit ints
        auto t0 = _mm256_shldi_epi64(decay(v), decay(v), 32);

        // Reverse 16-bit halves within 32-bit ints
        auto t1 = _mm256_shldi_epi32(t0, t0, 16);

        // Reverse 8-bit halves within 16-bit ints
        auto t2 = _mm256_shldi_epi16(t1, t1, 8);

        return vec4x64u{t2};
    }

    auto vec4x64u_concat_shift = vector_test_bench<vec4x64u, vec4x64u_concat_shift_impl>;

    BENCHMARK(byteswap_64u::vec4x64u_concat_shift);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x16u_shuffle_shift_impl(vec8x64u v) {
        auto t0 = _mm512_shufflelo_epi16(decay(v), 0xB1);
        auto t1 = _mm512_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm512_slli_epi16(t1, 0x8);
        auto t3 = _mm512_srli_epi16(t1, 0x8);
        return vec8x64u{_mm512_or_si512(t2, t3)};
    }

    auto vec8x64u_shuffle_shift = vector_test_bench<vec8x64u, vec8x16u_shuffle_shift_impl>;

    BENCHMARK(byteswap_64u::vec8x64u_shuffle_shift);

    #endif



    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_pshufb_impl(vec8x64u v) {
        alignas(64) static constexpr std::uint8_t index_data[64] {
            0x7, 0x6, 0x5, 0x4,
            0x3, 0x2, 0x1, 0x0,
            0xf, 0xe, 0xd, 0xc,
            0xb, 0xa, 0x9, 0x8,
            0x7, 0x6, 0x5, 0x4,
            0x3, 0x2, 0x1, 0x0,
            0xf, 0xe, 0xd, 0xc,
            0xb, 0xa, 0x9, 0x8,
            0x7, 0x6, 0x5, 0x4,
            0x3, 0x2, 0x1, 0x0,
            0xf, 0xe, 0xd, 0xc,
            0xb, 0xa, 0x9, 0x8,
            0x7, 0x6, 0x5, 0x4,
            0x3, 0x2, 0x1, 0x0,
            0xf, 0xe, 0xd, 0xc,
            0xb, 0xa, 0x9, 0x8
        };

        auto indices = _mm512_load_si512(reinterpret_cast<const __m512i*>(index_data));
        return vec8x64u{_mm512_shuffle_epi8(decay(v), indices)};
    }

    auto vec8x64u_pshufb = vector_test_bench<vec8x64u, vec8x64u_pshufb_impl>;

    BENCHMARK(byteswap_64u::vec8x64u_pshufb);

    #endif



    #if defined(AVEL_AVX512VBMI2)

    vec8x64u vec8x64u_concat_shift_impl(vec8x64u v) {
        // Reverse 32-bit halves within 64-bit ints
        auto t0 = _mm512_shldi_epi64(decay(v), decay(v), 32);

        // Reverse 16-bit halves within 32-bit ints
        auto t1 = _mm512_shldi_epi32(t0, t0, 16);

        // Reverse 8-bit halves within 16-bit ints
        auto t2 = _mm512_shldi_epi16(t1, t1, 8);

        return vec8x64u{t2};
    }

    auto vec8x64u_concat_shift = vector_test_bench<vec8x64u, vec8x64u_concat_shift_impl>;

    BENCHMARK(byteswap_64u::vec8x64u_concat_shift);

    #endif

}

#endif //AVEL_BYTESWAP_64U_HPP
