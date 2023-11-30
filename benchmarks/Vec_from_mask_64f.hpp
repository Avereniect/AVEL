#ifndef AVEL_VEC_FROM_MASK_64F_HPP
#define AVEL_VEC_FROM_MASK_64F_HPP

namespace avel::benchmarks::vec_from_mask_64f {

    //=====================================================
    // scalar 32f benchmarks
    //=====================================================

    double scalar_native_conversion_impl(bool x) {
        return static_cast<double>(x);
    }

    auto scalar_native_conversion = scalar_mask_bool_test_bench<double, scalar_native_conversion_impl>;

    BENCHMARK(vec_from_mask_64f::scalar_native_conversion);

    //=====================================================
    // vec2x64f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    vec2x64f vec2x64f_bitwise_and_impl(mask2x64f m) {
        return vec2x64f{_mm_and_pd(_mm_set1_pd(1.0), decay(m))};
    }

    auto vec2x64f_bitwise_and = vector_vector_mask_test_bench<vec2x64f, vec2x64f_bitwise_and_impl>;

    BENCHMARK(vec_from_mask_64f::vec2x64f_bitwise_and);

    #endif



    #if defined(AVEL_AVX512VL)

    vec2x64f vec2x64f_masked_movedup_impl(mask2x64f m) {
        return vec2x64f{_mm_maskz_movedup_pd(decay(m), _mm_set_sd(1.0))};
    }

    auto vec2x64f_masked_movedup = vector_vector_mask_test_bench<vec2x64f, vec2x64f_masked_movedup_impl>;

    BENCHMARK(vec_from_mask_64f::vec2x64f_masked_movedup);

    #endif

    //=====================================================
    // vec4x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    vec4x64f vec4x64f_bitwise_and_impl(mask4x64f m) {
        return vec4x64f{_mm256_and_pd(_mm256_set1_pd(1.0), decay(m))};
    }

    auto vec4x64f_bitwise_and = vector_vector_mask_test_bench<vec4x64f, vec4x64f_bitwise_and_impl>;

    BENCHMARK(vec_from_mask_64f::vec4x64f_bitwise_and);

    #endif



    #if defined(AVEL_AVX512VL)

    vec4x64f vec4x64f_masked_broadcast_impl(mask4x64f m) {
        return vec4x64f{_mm256_maskz_broadcastsd_pd(decay(m), _mm_set_sd(1.0))};
    }

    auto vec4x64f_masked_broadcast = vector_vector_mask_test_bench<vec4x64f, vec4x64f_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_64f::vec4x64f_masked_broadcast);

    #endif

    //=====================================================
    // vec8x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64f vec8x64f_masked_broadcast_impl(mask8x64f m) {
        return vec8x64f{_mm512_maskz_broadcastsd_pd(decay(m), _mm_set_sd(1.0))};
    }

    auto vec8x64f_masked_broadcast = vector_vector_mask_test_bench<vec8x64f, vec8x64f_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_64f::vec8x64f_masked_broadcast);

    #endif

}

#endif //AVEL_VEC_FROM_MASK_64F_HPP
