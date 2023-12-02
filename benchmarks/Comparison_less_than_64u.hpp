#ifndef AVEL_COMPARISON_EQUALS_64U_HPP
#define AVEL_COMPARISON_EQUALS_64U_HPP

namespace avel::benchmarks::comparison_less_than_64u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::uint64_t x, std::uint64_t y) {
        return x < y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(comparison_less_than_64u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask2x64u vec2x64u_scalarized_impl(vec2x64u lhs, vec2x64u rhs) {
        std::uint64_t lhs_lo = avel::extract<0>(lhs);
        std::uint64_t lhs_hi = avel::extract<1>(lhs);

        std::uint64_t rhs_lo = avel::extract<0>(rhs);
        std::uint64_t rhs_hi = avel::extract<1>(rhs);

        std::int64_t out_lo = (lhs_lo < rhs_lo) ? -1 : 0;
        std::int64_t out_hi = (lhs_hi < rhs_hi) ? -1 : 0;

        return mask2x64u{_mm_set_epi64x(out_lo, out_hi)};
    }

    auto vec2x64u_scalarized = vector_comparison_test_bench<vec2x64u, vec2x64u_scalarized_impl>;

    BENCHMARK(comparison_less_than_64u::vec2x64u_scalarized);

    #endif



    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask2x64u vec2x64u_32_bit_comparison_emulation_xor_impl(vec2x64u lhs, vec2x64u rhs) {
        auto high_bit = _mm_set1_epi64x(0x8000000000000000);

        auto t0 = _mm_xor_si128(decay(lhs), high_bit);
        auto t1 = _mm_xor_si128(decay(rhs), high_bit);

        auto c0 = _mm_cmplt_epi32(t0, t1);
        auto c1 = _mm_cmplt_epi32(t1, t0);
        auto c2 = _mm_cmpeq_epi32(t0, t1);
        auto c3 = _mm_slli_epi64(c0, 32);

        auto comparison_result = _mm_or_si128(c0, _mm_and_si128(_mm_andnot_si128(c1, c2), c3));
        auto ret = _mm_shuffle_epi32(comparison_result, 0xf5);
        return mask2x64u{ret};
    }

    auto vec2x64u_32_bit_comparison_emulation_xor = vector_comparison_test_bench<vec2x64u, vec2x64u_32_bit_comparison_emulation_xor_impl>;

    BENCHMARK(comparison_less_than_64u::vec2x64u_32_bit_comparison_emulation_xor);

    #endif



    #if defined(AVEL_SSE41) && !defined(AVEL_AVX512VL)

    mask2x64u vec2x64u_pcmpgtq_impl(vec2x64u lhs, vec2x64u rhs) {
        auto high_bit = _mm_set1_epi64x(0x8000000000000000);
        auto t0 = _mm_xor_si128(decay(lhs), high_bit);
        auto t1 = _mm_xor_si128(decay(rhs), high_bit);
        return mask2x64u{_mm_cmpgt_epi64(t1, t0)};
    }

    auto vec2x64u_pcmpgtq = vector_comparison_test_bench<vec2x64u, vec2x64u_pcmpgtq_impl>;

    BENCHMARK(comparison_less_than_64u::vec2x64u_pcmpgtq);

    #endif



    #if defined(AVEL_AVX512VL)

    mask2x64u vec2x64u_pcmpuq_impl(vec2x64u lhs, vec2x64u rhs) {
        return mask2x64u{_mm_cmp_epu64_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec2x64u_pcmpuq = vector_comparison_test_bench<vec2x64u, vec2x64u_pcmpuq_impl>;

    BENCHMARK(comparison_less_than_64u::vec2x64u_pcmpuq);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    mask4x64u vec4x64u_pcmpgtq_impl(vec4x64u lhs, vec4x64u rhs) {
        auto high_bit = _mm256_set1_epi64x(0x8000000000000000);
        auto t0 = _mm256_xor_si256(decay(lhs), high_bit);
        auto t1 = _mm256_xor_si256(decay(rhs), high_bit);
        return mask4x64u{_mm256_cmpgt_epi64(t1, t0)};
    }

    auto vec4x64u_pcmpgtq = vector_comparison_test_bench<vec4x64u, vec4x64u_pcmpgtq_impl>;

    BENCHMARK(comparison_less_than_64u::vec4x64u_pcmpgtq);

    #endif



    #if defined(AVEL_AVX512VL)

    mask4x64u vec4x64u_pcmpuq_impl(vec4x64u lhs, vec4x64u rhs) {
        return mask4x64u{_mm256_cmp_epu64_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec4x64u_pcmpuq = vector_comparison_test_bench<vec4x64u, vec4x64u_pcmpuq_impl>;

    BENCHMARK(comparison_less_than_64u::vec4x64u_pcmpuq);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    mask8x64u vec8x64u_pcmpuq_impl(vec8x64u lhs, vec8x64u rhs) {
        return mask8x64u{_mm512_cmp_epu64_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec8x64u_pcmpuq = vector_comparison_test_bench<vec8x64u, vec8x64u_pcmpuq_impl>;

    BENCHMARK(comparison_less_than_64u::vec8x64u_pcmpuq);

    #endif

}

#endif //AVEL_COMPARISON_EQUALS_64U_HPP
