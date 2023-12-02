#ifndef AVEL_COMPARISON_EQUALS_64F_HPP
#define AVEL_COMPARISON_EQUALS_64F_HPP

namespace avel::benchmarks::comparison_equals_64f {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(double x, double y) {
        return x < y;
    }

    auto scalar_native = scalar_comparison_test_bench<double, scalar_native_impl>;

    BENCHMARK(comparison_equals_64f::scalar_native);

    //=====================================================
    // vec2x64f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask2x64f vec2x64f_cmppd_impl(vec2x64f lhs, vec2x64f rhs) {
        return mask2x64f{_mm_cmplt_pd(decay(lhs), decay(rhs))};
    }

    auto vec2x64f_cmppd = vector_comparison_test_bench<vec2x64f, vec2x64f_cmppd_impl>;

    BENCHMARK(comparison_equals_64f::vec2x64f_cmppd);

    #endif



    #if defined(AVEL_AVX512VL)

    mask2x64f vec2x64f_cmpltd_k_impl(vec2x64f lhs, vec2x64f rhs) {
        return mask2x64f{_mm_cmp_pd_mask(decay(lhs), decay(rhs), _CMP_LT_OS)};
    }

    auto vec2x64f_cmpltd_k = vector_comparison_test_bench<vec2x64f, vec2x64f_cmpltd_k_impl>;

    BENCHMARK(comparison_equals_64f::vec2x64f_cmpltd_k);

    #endif

    //=====================================================
    // vec4x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    mask4x64f vec4x64f_cmppd_impl(vec4x64f lhs, vec4x64f rhs) {
        return mask4x64f{_mm256_cmp_pd(decay(lhs), decay(rhs), _CMP_LT_OS)};
    }

    auto vec4x64f_cmpltd = vector_comparison_test_bench<vec4x64f, vec4x64f_cmppd_impl>;

    BENCHMARK(comparison_equals_64f::vec4x64f_cmpltd);

    #endif



    #if defined(AVEL_AVX512VL)

    mask4x64f vec4x64f_vcmppd_impl(vec4x64f lhs, vec4x64f rhs) {
        return mask4x64f{_mm256_cmp_pd_mask(decay(lhs), decay(rhs), _CMP_LT_OS)};
    }

    auto vec4x64f_vcmppd = vector_comparison_test_bench<vec4x64f, vec4x64f_vcmppd_impl>;

    BENCHMARK(comparison_equals_64f::vec4x64f_vcmppd);

    #endif

    //=====================================================
    // vec8x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    mask8x64f vec8x64f_vcmppd_impl(vec8x64f lhs, vec8x64f rhs) {
        return mask8x64f{_mm512_cmp_pd_mask(decay(lhs), decay(rhs), _CMP_LT_OS)};
    }

    auto vec8x64f_vcmppd = vector_comparison_test_bench<vec8x64f, vec8x64f_vcmppd_impl>;

    BENCHMARK(comparison_equals_64f::vec8x64f_vcmppd);

    #endif

}

#endif //AVEL_COMPARISON_EQUALS_64F_HPP
