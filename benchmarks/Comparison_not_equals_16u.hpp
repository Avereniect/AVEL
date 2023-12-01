#ifndef AVEL_comparison_not_equals_16u_HPP
#define AVEL_comparison_not_equals_16u_HPP

namespace avel::benchmarks::comparison_not_equals_16u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::uint16_t x, std::uint16_t y) {
        return x != y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::uint16_t, scalar_native_impl>;

    BENCHMARK(comparison_not_equals_16u::scalar_native);

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask8x16u vec8x16u_cmpeqw_impl(vec8x16u lhs, vec8x16u rhs) {
        return mask8x16u{_mm_andnot_si128(_mm_cmpeq_epi16(decay(lhs), decay(rhs)), _mm_set1_epi32(-1))};
    }

    auto vec8x16u_cmpeqw = vector_comparison_test_bench<vec8x16u, vec8x16u_cmpeqw_impl>;

    BENCHMARK(comparison_not_equals_16u::vec8x16u_cmpeqw);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask8x16u vec8x16u_cmpeqw_k_impl(vec8x16u lhs, vec8x16u rhs) {
        return mask8x16u{_mm_cmp_epi16_mask(decay(lhs), decay(rhs), _MM_CMPINT_NE)};
    }

    auto vec8x16u_cmpeqw_k = vector_comparison_test_bench<vec8x16u, vec8x16u_cmpeqw_k_impl>;

    BENCHMARK(comparison_not_equals_16u::vec8x16u_cmpeqw_k);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask16x16u vec16x16u_cmpeqw_impl(vec16x16u lhs, vec16x16u rhs) {
        return mask16x16u{_mm256_andnot_si256(_mm256_cmpeq_epi16(decay(lhs), decay(rhs)), _mm256_set1_epi32(-1))};
    }

    auto vec16x16u_cmpeqw = vector_comparison_test_bench<vec16x16u, vec16x16u_cmpeqw_impl>;

    BENCHMARK(comparison_not_equals_16u::vec16x16u_cmpeqw);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask16x16u vec16x16u_cmpeqw_k_impl(vec16x16u lhs, vec16x16u rhs) {
        return mask16x16u{_mm256_cmp_epi16_mask(decay(lhs), decay(rhs), _MM_CMPINT_NE)};
    }

    auto vec16x16u_cmpeqw_k = vector_comparison_test_bench<vec16x16u, vec16x16u_cmpeqw_k_impl>;

    BENCHMARK(comparison_not_equals_16u::vec16x16u_cmpeqw_k);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    mask32x16u vec32x16u_cmpeqw_k_impl(vec32x16u lhs, vec32x16u rhs) {
        return mask32x16u{_mm512_cmp_epi16_mask(decay(lhs), decay(rhs), _MM_CMPINT_NE)};
    }

    auto vec32x16u_cmpeqw_k = vector_comparison_test_bench<vec32x16u, vec32x16u_cmpeqw_k_impl>;

    BENCHMARK(comparison_not_equals_16u::vec32x16u_cmpeqw_k);

    #endif

}

#endif //AVEL_comparison_not_equals_16u_HPP
