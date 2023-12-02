#ifndef AVEL_COMPARISON_LESS_THAN_32I_HPP
#define AVEL_COMPARISON_LESS_THAN_32I_HPP

namespace avel::benchmarks::comparison_less_than_32i {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::int32_t x, std::int32_t y) {
        return x < y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::int32_t, scalar_native_impl>;

    BENCHMARK(comparison_less_than_32i::scalar_native);

    //=====================================================
    // vec4x32i benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask4x32i vec4x32i_pcmpd_impl(vec4x32i lhs, vec4x32i rhs) {
        return mask4x32i{_mm_cmplt_epi32(decay(lhs), decay(rhs))};
    }

    auto vec4x32i_pcmpd = vector_comparison_test_bench<vec4x32i, vec4x32i_pcmpd_impl>;

    BENCHMARK(comparison_less_than_32i::vec4x32i_pcmpd);

    #endif



    #if defined(AVEL_AVX512VL)

    mask4x32i vec4x32i_pcmpd_impl(vec4x32i lhs, vec4x32i rhs) {
        return mask4x32i{_mm_cmp_epi32_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec4x32i_pcmpd_k = vector_comparison_test_bench<vec4x32i, vec4x32i_pcmpd_impl>;

    BENCHMARK(comparison_less_than_32i::vec4x32i_pcmpd_k);

    #endif

    //=====================================================
    // vec8x32i benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    mask8x32i vec8x32i_pcmpd_impl(vec8x32i lhs, vec8x32i rhs) {
        return mask8x32i{_mm256_cmpgt_epi32(decay(rhs), decay(lhs))};
    }

    auto vec8x32i_pcmpd = vector_comparison_test_bench<vec8x32i, vec8x32i_pcmpd_impl>;

    BENCHMARK(comparison_less_than_32i::vec8x32i_pcmpd);

    #endif



    #if defined(AVEL_AVX512VL)

    mask8x32i vec8x32i_pcmpd_impl(vec8x32i lhs, vec8x32i rhs) {
        return mask8x32i{_mm256_cmp_epi32_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec8x32i_pcmpd_k = vector_comparison_test_bench<vec8x32i, vec8x32i_pcmpd_impl>;

    BENCHMARK(comparison_less_than_32i::vec8x32i_pcmpd_k);

    #endif

    //=====================================================
    // vec16x32i benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    mask16x32i vec16x32i_pcmpd_impl(vec16x32i lhs, vec16x32i rhs) {
        return mask16x32i{_mm512_cmp_epi32_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec16x32i_pcmpd_k = vector_comparison_test_bench<vec16x32i, vec16x32i_pcmpd_impl>;

    BENCHMARK(comparison_less_than_32i::vec16x32i_pcmpd_k);

    #endif

}

#endif //AVEL_COMPARISON_LESS_THAN_32I_HPP
