#ifndef AVEL_COMPARISON_LESS_THAN_64I_HPP
#define AVEL_COMPARISON_LESS_THAN_64I_HPP

namespace avel::benchmarks::comparison_less_than_64i {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::int64_t x, std::int64_t y) {
        return x < y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::int64_t, scalar_native_impl>;

    BENCHMARK(comparison_less_than_64i::scalar_native);

    //=====================================================
    // vec2x64i benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask2x64i vec2x64i_scalarized_impl(vec2x64i lhs, vec2x64i rhs) {
        std::int64_t lhs_lo = avel::extract<0>(lhs);
        std::int64_t lhs_hi = avel::extract<1>(lhs);

        std::int64_t rhs_lo = avel::extract<0>(rhs);
        std::int64_t rhs_hi = avel::extract<1>(rhs);

        std::int64_t out_lo = (lhs_lo < rhs_lo) ? -1 : 0;
        std::int64_t out_hi = (lhs_hi < rhs_hi) ? -1 : 0;

        return mask2x64i{_mm_set_epi64x(out_lo, out_hi)};
    }

    auto vec2x64i_scalarized = vector_comparison_test_bench<vec2x64i, vec2x64i_scalarized_impl>;

    BENCHMARK(comparison_less_than_64i::vec2x64i_scalarized);

    #endif



    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask2x64i vec2x64i_32_bit_comparison_emulation_impl(vec2x64i lhs, vec2x64i rhs) {
        auto t0 = decay(lhs);
        auto t1 = decay(rhs);

        auto c0 = _mm_cmplt_epi32(t0, t1);
        auto c1 = _mm_cmplt_epi32(t1, t0);
        auto c2 = _mm_cmpeq_epi32(t0, t1);
        auto c3 = _mm_slli_epi64(c0, 32);

        auto comparison_result = _mm_or_si128(c0, _mm_and_si128(_mm_andnot_si128(c1, c2), c3));
        auto ret = _mm_shuffle_epi32(comparison_result, 0xf5);
        return mask2x64i{ret};
    }

    auto vec2x64i_32_bit_comparison_emulation = vector_comparison_test_bench<vec2x64i, vec2x64i_32_bit_comparison_emulation_impl>;

    BENCHMARK(comparison_less_than_64i::vec2x64i_32_bit_comparison_emulation);

    #endif



    #if defined(AVEL_SSE4_1) && !defined(AVEL_AVX512VL)

    mask2x64i vec2x64i_pcmpq_impl(vec2x64i lhs, vec2x64i rhs) {
        return mask2x64i{_mm_cmpgt_epi64(decay(rhs), decay(lhs))};
    }

    auto vec2x64i_pcmpq = vector_comparison_test_bench<vec2x64i, vec2x64i_pcmpq_impl>;

    BENCHMARK(comparison_less_than_64i::vec2x64i_pcmpq);

    #endif



    #if defined(AVEL_AVX512VL)

    mask2x64i vec2x64i_pcmpq_impl(vec2x64i lhs, vec2x64i rhs) {
        return mask2x64i{_mm_cmp_epi64_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec2x64i_pcmpq_k = vector_comparison_test_bench<vec2x64i, vec2x64i_pcmpq_impl>;

    BENCHMARK(comparison_less_than_64i::vec2x64i_pcmpq_k);

    #endif

    //=====================================================
    // vec4x64i benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    mask4x64i vec4x64i_pcmpq_impl(vec4x64i lhs, vec4x64i rhs) {
        return mask4x64i{_mm256_cmpgt_epi64(decay(rhs), decay(lhs))};
    }

    auto vec4x64i_pcmpq = vector_comparison_test_bench<vec4x64i, vec4x64i_pcmpq_impl>;

    BENCHMARK(comparison_less_than_64i::vec4x64i_pcmpq);

    #endif



    #if defined(AVEL_AVX512VL)

    mask4x64i vec4x64i_pcmpq_impl(vec4x64i lhs, vec4x64i rhs) {
        return mask4x64i{_mm256_cmp_epi64_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec4x64i_pcmpq_k = vector_comparison_test_bench<vec4x64i, vec4x64i_pcmpq_impl>;

    BENCHMARK(comparison_less_than_64i::vec4x64i_pcmpq_k);

    #endif

    //=====================================================
    // vec8x64i benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    mask8x64i vec8x64i_pcmpq_impl(vec8x64i lhs, vec8x64i rhs) {
        return mask8x64i{_mm512_cmp_epi64_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec8x64i_pcmpq_k = vector_comparison_test_bench<vec8x64i, vec8x64i_pcmpq_impl>;

    BENCHMARK(comparison_less_than_64i::vec8x64i_pcmpq_k);

    #endif

}

#endif //AVEL_COMPARISON_LESS_THAN_64I_HPP
