#ifndef AVEL_COMPARISON_LESS_THAN_16I_HPP
#define AVEL_COMPARISON_LESS_THAN_16I_HPP

namespace avel::benchmarks::comparison_less_than_16i {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::int16_t x, std::int16_t y) {
        return x < y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::int16_t, scalar_native_impl>;

    BENCHMARK(comparison_less_than_16i::scalar_native);

    //=====================================================
    // vec8x16i benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask8x16i vec8x16i_pcmpgtw_impl(vec8x16i lhs, vec8x16i rhs) {
        return mask8x16i{_mm_cmplt_epi16(decay(lhs), decay(rhs))};
    }

    auto vec8x16i_pcmpgtw = vector_comparison_test_bench<vec8x16i, vec8x16i_pcmpgtw_impl>;

    BENCHMARK(comparison_less_than_16i::vec8x16i_pcmpgtw);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask8x16i vec8x16i_pcmpw_impl(vec8x16i lhs, vec8x16i rhs) {
        return mask8x16i{_mm_cmp_epi16_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec8x16i_pcmpw_k = vector_comparison_test_bench<vec8x16i, vec8x16i_pcmpw_impl>;

    BENCHMARK(comparison_less_than_16i::vec8x16i_pcmpw_k);

    #endif

    //=====================================================
    // vec16x16i benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask16x16i vec16x16i_pcmpw_impl(vec16x16i lhs, vec16x16i rhs) {
        return mask16x16i{_mm256_cmpgt_epi16(decay(rhs), decay(lhs))};
    }

    auto vec16x16i_pcmpw = vector_comparison_test_bench<vec16x16i, vec16x16i_pcmpw_impl>;

    BENCHMARK(comparison_less_than_16i::vec16x16i_pcmpw);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask16x16i vec16x16i_pcmpw_impl(vec16x16i lhs, vec16x16i rhs) {
        return mask16x16i{_mm256_cmp_epi16_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec16x16i_pcmpw_k = vector_comparison_test_bench<vec16x16i, vec16x16i_pcmpw_impl>;

    BENCHMARK(comparison_less_than_16i::vec16x16i_pcmpw_k);

    #endif

    //=====================================================
    // vec32x16i benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    mask32x16i vec32x16i_pcmpw_impl(vec32x16i lhs, vec32x16i rhs) {
        return mask32x16i{_mm512_cmp_epi16_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec32x16i_pcmpw_k = vector_comparison_test_bench<vec32x16i, vec32x16i_pcmpw_impl>;

    BENCHMARK(comparison_less_than_16i::vec32x16i_pcmpw_k);

    #endif

}

#endif //AVEL_COMPARISON_LESS_THAN_16I_HPP
