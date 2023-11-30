#ifndef AVEL_COMPARISON_EQUALS_32F_HPP
#define AVEL_COMPARISON_EQUALS_32F_HPP

namespace avel::benchmarks::comparison_equals_32f {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(float x, float y) {
        return x == y;
    }

    auto scalar_native = scalar_comparison_test_bench<float, scalar_native_impl>;

    BENCHMARK(comparison_equals_32f::scalar_native);

    //=====================================================
    // vec4x32f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask4x32f vec4x32f_cmpps_impl(vec4x32f lhs, vec4x32f rhs) {
        return mask4x32f{_mm_cmpeq_ps(decay(lhs), decay(rhs))};
    }

    auto vec4x32f_cmpps = vector_comparison_test_bench<vec4x32f, vec4x32f_cmpps_impl>;

    BENCHMARK(comparison_equals_32f::vec4x32f_cmpps);

    #endif



    #if defined(AVEL_AVX512VL)

    mask4x32f vec4x32f_cmpeqd_k_impl(vec4x32f lhs, vec4x32f rhs) {
        return mask4x32f{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_EQ_OS)};
    }

    auto vec4x32f_cmpeqd_k = vector_comparison_test_bench<vec4x32f, vec4x32f_cmpeqd_k_impl>;

    BENCHMARK(comparison_equals_32f::vec4x32f_cmpeqd_k);

    #endif

    //=====================================================
    // vec8x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    mask8x32f vec8x32f_cmpps_impl(vec8x32f lhs, vec8x32f rhs) {
        return mask8x32f{_mm256_cmp_ps(decay(lhs), decay(rhs), _CMP_EQ_OS)};
    }

    auto vec8x32f_cmpeqd = vector_comparison_test_bench<vec8x32f, vec8x32f_cmpps_impl>;

    BENCHMARK(comparison_equals_32f::vec8x32f_cmpeqd);

    #endif



    #if defined(AVEL_AVX512VL)

    mask8x32f vec8x32f_vcmpps_impl(vec8x32f lhs, vec8x32f rhs) {
        return mask8x32f{_mm256_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_EQ_OS)};
    }

    auto vec8x32f_vcmpps = vector_comparison_test_bench<vec8x32f, vec8x32f_vcmpps_impl>;

    BENCHMARK(comparison_equals_32f::vec8x32f_vcmpps);

    #endif

    //=====================================================
    // vec16x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    mask16x32f vec16x32f_vcmpps_impl(vec16x32f lhs, vec16x32f rhs) {
        return mask16x32f{_mm512_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_EQ_OS)};
    }

    auto vec16x32f_vcmpps = vector_comparison_test_bench<vec16x32f, vec16x32f_vcmpps_impl>;

    BENCHMARK(comparison_equals_32f::vec16x32f_vcmpps);

    #endif

}

#endif //AVEL_COMPARISON_EQUALS_32F_HPP
