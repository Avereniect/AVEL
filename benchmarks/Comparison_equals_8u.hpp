#ifndef AVEL_OPERATOR_EQUALS_8U_HPP
#define AVEL_OPERATOR_EQUALS_8U_HPP

namespace avel::benchmarks::comparison_equals_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::uint8_t x, std::uint8_t y) {
        return x == y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(comparison_equals_8u::scalar_native);

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask16x8u vec16x8u_cmpeqb_impl(vec16x8u lhs, vec16x8u rhs) {
        return mask16x8u{_mm_cmpeq_epi8(decay(lhs), decay(rhs))};
    }

    auto vec16x8u_cmpeqb = vector_comparison_test_bench<vec16x8u, vec16x8u_cmpeqb_impl>;

    BENCHMARK(comparison_equals_8u::vec16x8u_cmpeqb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask16x8u vec16x8u_cmpeqb_k_impl(vec16x8u lhs, vec16x8u rhs) {
        return mask16x8u{_mm_cmp_epi8_mask(decay(lhs), decay(rhs), _MM_CMPINT_EQ)};
    }

    auto vec16x8u_cmpeqb_k = vector_comparison_test_bench<vec16x8u, vec16x8u_cmpeqb_k_impl>;

    BENCHMARK(comparison_equals_8u::vec16x8u_cmpeqb_k);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask32x8u vec32x8u_cmpeqb_impl(vec32x8u lhs, vec32x8u rhs) {
        return mask32x8u{_mm256_cmpeq_epi8(decay(lhs), decay(rhs))};
    }

    auto vec32x8u_cmpeqb = vector_comparison_test_bench<vec32x8u, vec32x8u_cmpeqb_impl>;

    BENCHMARK(comparison_equals_8u::vec32x8u_cmpeqb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask32x8u vec32x8u_cmpeqb_k_impl(vec32x8u lhs, vec32x8u rhs) {
        return mask32x8u{_mm256_cmp_epi8_mask(decay(lhs), decay(rhs), _MM_CMPINT_EQ)};
    }

    auto vec32x8u_cmpeqb_k = vector_comparison_test_bench<vec32x8u, vec32x8u_cmpeqb_k_impl>;

    BENCHMARK(comparison_equals_8u::vec32x8u_cmpeqb_k);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    mask64x8u vec64x8u_cmpeqb_k_impl(vec64x8u lhs, vec64x8u rhs) {
        return mask64x8u{_mm512_cmp_epi8_mask(decay(lhs), decay(rhs), _MM_CMPINT_EQ)};
    }

    auto vec64x8u_cmpeqb_k = vector_comparison_test_bench<vec64x8u, vec64x8u_cmpeqb_k_impl>;

    BENCHMARK(comparison_equals_8u::vec64x8u_cmpeqb_k);

    #endif

}

#endif //AVEL_OPERATOR_EQUALS_8U_HPP
