#ifndef AVEL_comparison_not_equals_64u_HPP
#define AVEL_comparison_not_equals_64u_HPP

namespace avel::benchmarks::comparison_not_equals_64u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::uint64_t x, std::uint64_t y) {
        return x != y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(comparison_not_equals_64u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask2x64u vec2x64u_32_bit_comparison_and_combine_impl(vec2x64u lhs, vec2x64u rhs) {
        auto t0 = _mm_cmpeq_epi32(decay(lhs), decay(rhs));
        auto t1 = _mm_shuffle_epi32(t0, 0xb1);
        auto t2 = _mm_and_si128(t0, t1);
        auto t3 = _mm_andnot_si128(t2, _mm_set1_epi32(-1));
        return mask2x64u{t3};
    }

    auto vec2x64u_32_bit_comparison_and_combine = vector_comparison_test_bench<vec2x64u, vec2x64u_32_bit_comparison_and_combine_impl>;

    BENCHMARK(comparison_not_equals_64u::vec2x64u_32_bit_comparison_and_combine);

    #endif



    #if defined(AVEL_SSE41) && !defined(AVEL_AVX512VL)

    mask2x64u vec2x64u_cmpeqq_impl(vec2x64u lhs, vec2x64u rhs) {
        return mask2x64u{_mm_andnot_si128(_mm_cmpeq_epi64(decay(lhs), decay(rhs)), _mm_set1_epi32(-1))};
    }

    auto vec2x64u_cmpeqq = vector_comparison_test_bench<vec2x64u, vec2x64u_cmpeqq_impl>;

    BENCHMARK(comparison_not_equals_64u::vec2x64u_cmpeqq);

    #endif



    #if defined(AVEL_AVX512VL)

    mask2x64u vec2x64u_cmpeqq_k_impl(vec2x64u lhs, vec2x64u rhs) {
        return mask2x64u{_mm_cmp_epi64_mask(decay(lhs), decay(rhs), _MM_CMPINT_NE)};
    }

    auto vec2x64u_cmpeqq_k = vector_comparison_test_bench<vec2x64u, vec2x64u_cmpeqq_k_impl>;

    BENCHMARK(comparison_not_equals_64u::vec2x64u_cmpeqq_k);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    mask4x64u vec4x64u_cmpeqq_impl(vec4x64u lhs, vec4x64u rhs) {
        return mask4x64u{_mm256_andnot_si256(_mm256_cmpeq_epi64(decay(lhs), decay(rhs)), _mm256_set1_epi32(-1))};
    }

    auto vec4x64u_cmpeqq = vector_comparison_test_bench<vec4x64u, vec4x64u_cmpeqq_impl>;

    BENCHMARK(comparison_not_equals_64u::vec4x64u_cmpeqq);

    #endif



    #if defined(AVEL_AVX512VL)

    mask4x64u vec4x64u_cmpeqq_k_impl(vec4x64u lhs, vec4x64u rhs) {
        return mask4x64u{_mm256_cmp_epi64_mask(decay(lhs), decay(rhs), _MM_CMPINT_NE)};
    }

    auto vec4x64u_cmpeqq_k = vector_comparison_test_bench<vec4x64u, vec4x64u_cmpeqq_k_impl>;

    BENCHMARK(comparison_not_equals_64u::vec4x64u_cmpeqq_k);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    mask8x64u vec8x64u_cmpeqq_k_impl(vec8x64u lhs, vec8x64u rhs) {
        return mask8x64u{_mm512_cmp_epi64_mask(decay(lhs), decay(rhs), _MM_CMPINT_NE)};
    }

    auto vec8x64u_cmpeqq_k = vector_comparison_test_bench<vec8x64u, vec8x64u_cmpeqq_k_impl>;

    BENCHMARK(comparison_not_equals_64u::vec8x64u_cmpeqq_k);

    #endif

}

#endif //AVEL_comparison_not_equals_64u_HPP
