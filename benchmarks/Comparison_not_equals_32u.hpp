#ifndef AVEL_comparison_not_equals_32u_HPP
#define AVEL_comparison_not_equals_32u_HPP

namespace avel::benchmarks::comparison_not_equals_32u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::uint32_t x, std::uint32_t y) {
        return x != y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(comparison_not_equals_32u::scalar_native);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask4x32u vec4x32u_cmpeqd_impl(vec4x32u lhs, vec4x32u rhs) {
        return mask4x32u{_mm_andnot_si128(_mm_cmpeq_epi32(decay(lhs), decay(rhs)), _mm_set1_epi32(-1))};
    }

    auto vec4x32u_cmpeqd = vector_comparison_test_bench<vec4x32u, vec4x32u_cmpeqd_impl>;

    BENCHMARK(comparison_not_equals_32u::vec4x32u_cmpeqd);

    #endif



    #if defined(AVEL_AVX512VL)

    mask4x32u vec4x32u_cmpeqd_k_impl(vec4x32u lhs, vec4x32u rhs) {
        return mask4x32u{_mm_cmp_epi32_mask(decay(lhs), decay(rhs), _MM_CMPINT_NE)};
    }

    auto vec4x32u_cmpeqd_k = vector_comparison_test_bench<vec4x32u, vec4x32u_cmpeqd_k_impl>;

    BENCHMARK(comparison_not_equals_32u::vec4x32u_cmpeqd_k);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    mask8x32u vec8x32u_cmpeqd_impl(vec8x32u lhs, vec8x32u rhs) {
        return mask8x32u{_mm256_andnot_si256(_mm256_cmpeq_epi32(decay(lhs), decay(rhs)), _mm256_set1_epi32(-1))};
    }

    auto vec8x32u_cmpeqd = vector_comparison_test_bench<vec8x32u, vec8x32u_cmpeqd_impl>;

    BENCHMARK(comparison_not_equals_32u::vec8x32u_cmpeqd);

    #endif



    #if defined(AVEL_AVX512VL)

    mask8x32u vec8x32u_cmpeqd_k_impl(vec8x32u lhs, vec8x32u rhs) {
        return mask8x32u{_mm256_cmp_epi32_mask(decay(lhs), decay(rhs), _MM_CMPINT_NE)};
    }

    auto vec8x32u_cmpeqd_k = vector_comparison_test_bench<vec8x32u, vec8x32u_cmpeqd_k_impl>;

    BENCHMARK(comparison_not_equals_32u::vec8x32u_cmpeqd_k);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    mask16x32u vec16x32u_cmpeqd_k_impl(vec16x32u lhs, vec16x32u rhs) {
        return mask16x32u{_mm512_cmp_epi32_mask(decay(lhs), decay(rhs), _MM_CMPINT_NE)};
    }

    auto vec16x32u_cmpeqd_k = vector_comparison_test_bench<vec16x32u, vec16x32u_cmpeqd_k_impl>;

    BENCHMARK(comparison_not_equals_32u::vec16x32u_cmpeqd_k);

    #endif

}

#endif //AVEL_comparison_not_equals_32u_HPP
