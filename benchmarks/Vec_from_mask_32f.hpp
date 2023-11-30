#ifndef AVEL_VEC_FROM_MASK_32F_HPP
#define AVEL_VEC_FROM_MASK_32F_HPP

namespace avel::benchmarks::vec_from_mask_32f {

    //=====================================================
    // scalar 32f benchmarks
    //=====================================================

    float scalar_native_conversion_impl(bool x) {
        return static_cast<float>(x);
    }

    auto scalar_native_conversion = scalar_mask_bool_test_bench<float, scalar_native_conversion_impl>;

    BENCHMARK(vec_from_mask_32f::scalar_native_conversion);

    //=====================================================
    // vec4x32f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    vec4x32f vec4x32f_bitwise_and_impl(mask4x32f m) {
        return vec4x32f{_mm_and_ps(_mm_set1_ps(1.0f), decay(m))};
    }

    auto vec4x32f_bitwise_and = vector_vector_mask_test_bench<vec4x32f, vec4x32f_bitwise_and_impl>;

    BENCHMARK(vec_from_mask_32f::vec4x32f_bitwise_and);

    #endif



    #if defined(AVEL_AVX512VL)

    vec4x32f vec4x32f_masked_broadcast_impl(mask4x32f m) {
        return vec4x32f{_mm_maskz_broadcastss_ps(decay(m), _mm_set_ss(1.0f))};
    }

    auto vec4x32f_masked_broadcast = vector_vector_mask_test_bench<vec4x32f, vec4x32f_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_32f::vec4x32f_masked_broadcast);

    #endif

    //=====================================================
    // vec8x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    vec8x32f vec8x32f_bitwise_and_impl(mask8x32f m) {
        return vec8x32f{_mm256_and_ps(_mm256_set1_ps(1.0f), decay(m))};
    }

    auto vec8x32f_bitwise_and = vector_vector_mask_test_bench<vec8x32f, vec8x32f_bitwise_and_impl>;

    BENCHMARK(vec_from_mask_32f::vec8x32f_bitwise_and);

    #endif



    #if defined(AVEL_AVX512VL)

    vec8x32f vec8x32f_masked_broadcast_impl(mask8x32f m) {
        return vec8x32f{_mm256_maskz_broadcastss_ps(decay(m), _mm_set_ss(1.0f))};
    }

    auto vec8x32f_masked_broadcast = vector_vector_mask_test_bench<vec8x32f, vec8x32f_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_32f::vec8x32f_masked_broadcast);

    #endif

    //=====================================================
    // vec16x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32f vec16x32f_masked_broadcast_impl(mask16x32f m) {
        return vec16x32f{_mm512_maskz_broadcastss_ps(decay(m), _mm_set_ss(1.0f))};
    }

    auto vec16x32f_masked_broadcast = vector_vector_mask_test_bench<vec16x32f, vec16x32f_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_32f::vec16x32f_masked_broadcast);

    #endif

}

#endif //AVEL_VEC_FROM_MASK_32F_HPP
