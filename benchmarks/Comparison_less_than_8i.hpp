#ifndef AVEL_COMPARISON_LESS_THAN_8I_HPP
#define AVEL_COMPARISON_LESS_THAN_8I_HPP

namespace avel::benchmarks::comparison_less_than_8i {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::int8_t x, std::int8_t y) {
        return x < y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::int8_t, scalar_native_impl>;

    BENCHMARK(comparison_less_than_8i::scalar_native);

    //=====================================================
    // vec16x8i benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask16x8i vec16x8i_pcmpgtb_impl(vec16x8i lhs, vec16x8i rhs) {
        return mask16x8i{_mm_cmplt_epi8(decay(lhs), decay(rhs))};
    }

    auto vec16x8i_pcmpgtb = vector_comparison_test_bench<vec16x8i, vec16x8i_pcmpgtb_impl>;

    BENCHMARK(comparison_less_than_8i::vec16x8i_pcmpgtb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask16x8i vec16x8i_pcmpb_impl(vec16x8i lhs, vec16x8i rhs) {
        return mask16x8i{_mm_cmp_epi8_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec16x8i_pcmpb = vector_comparison_test_bench<vec16x8i, vec16x8i_pcmpb_impl>;

    BENCHMARK(comparison_less_than_8i::vec16x8i_pcmpb);

    #endif

    //=====================================================
    // vec32x8i benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask32x8i vec32x8i_pcmpgtb_impl(vec32x8i lhs, vec32x8i rhs) {
        return mask32x8i{_mm256_cmpgt_epi8(decay(rhs), decay(lhs))};
    }

    auto vec32x8i_pcmpgtb = vector_comparison_test_bench<vec32x8i, vec32x8i_pcmpgtb_impl>;

    BENCHMARK(comparison_less_than_8i::vec32x8i_pcmpgtb);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask32x8i vec32x8i_pcmpb_impl(vec32x8i lhs, vec32x8i rhs) {
        return mask32x8i{_mm256_cmp_epi8_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec32x8i_pcmpb = vector_comparison_test_bench<vec32x8i, vec32x8i_pcmpb_impl>;

    BENCHMARK(comparison_less_than_8i::vec32x8i_pcmpb);

    #endif

    //=====================================================
    // vec64x8i benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    mask64x8i vec64x8i_pcmpb_impl(vec64x8i lhs, vec64x8i rhs) {
        return mask64x8i{_mm512_cmp_epi8_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec64x8i_pcmpb = vector_comparison_test_bench<vec64x8i, vec64x8i_pcmpb_impl>;

    BENCHMARK(comparison_less_than_8i::vec64x8i_pcmpb);

    #endif

}

#endif //AVEL_COMPARISON_LESS_THAN_8I_HPP
