#ifndef AVEL_VEC_FROM_MASK_32U_HPP
#define AVEL_VEC_FROM_MASK_32U_HPP

namespace avel::benchmarks::vec_from_mask_32u {

    //=====================================================
    // scalar 32u benchmarks
    //=====================================================

    std::uint32_t scalar_native_conversion_impl(bool x) {
        return static_cast<std::uint32_t>(x);
    }

    auto scalar_native_conversion = scalar_mask_bool_test_bench<std::uint32_t, scalar_native_conversion_impl>;

    BENCHMARK(vec_from_mask_32u::scalar_native_conversion);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    vec4x32u vec4x32u_negation_impl(mask4x32u m) {
        return vec4x32u{_mm_sub_epi32(_mm_setzero_si128(), decay(m))};
    }

    auto vec4x32u_negation = vector_vector_mask_test_bench<vec4x32u, vec4x32u_negation_impl>;

    BENCHMARK(vec_from_mask_32u::vec4x32u_negation);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec4x32u vec4x32u_move_mask_impl(mask4x32u m) {
        return vec4x32u{_mm_movm_epi32(decay(m))};
    }

    auto vec4x32u_move_mask = vector_vector_mask_test_bench<vec4x32u, vec4x32u_move_mask_impl>;

    BENCHMARK(vec_from_mask_32u::vec4x32u_move_mask);

    #endif



    #if defined(AVEL_AVX512VL)

    vec4x32u vec4x32u_masked_broadcast_impl(mask4x32u m) {
        return vec4x32u{_mm_maskz_set1_epi32(decay(m), 0x1)};
    }

    auto vec4x32u_masked_broadcast = vector_vector_mask_test_bench<vec4x32u, vec4x32u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_32u::vec4x32u_masked_broadcast);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    vec8x32u vec8x32u_negation_impl(mask8x32u m) {
        return vec8x32u{_mm256_sub_epi32(_mm256_setzero_si256(), decay(m))};
    }

    auto vec8x32u_negation = vector_vector_mask_test_bench<vec8x32u, vec8x32u_negation_impl>;

    BENCHMARK(vec_from_mask_32u::vec8x32u_negation);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec8x32u vec8x32u_move_mask_impl(mask8x32u m) {
        return vec8x32u{_mm256_movm_epi32(decay(m))};
    }

    auto vec8x32u_move_mask = vector_vector_mask_test_bench<vec8x32u, vec8x32u_move_mask_impl>;

    BENCHMARK(vec_from_mask_32u::vec8x32u_move_mask);

    #endif



    #if defined(AVEL_AVX512VL)

    vec8x32u vec8x32u_masked_broadcast_impl(mask8x32u m) {
        return vec8x32u{_mm256_maskz_set1_epi32(decay(m), 0x1)};
    }

    auto vec8x32u_masked_broadcast = vector_vector_mask_test_bench<vec8x32u, vec8x32u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_32u::vec8x32u_masked_broadcast);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512DQ)

    vec16x32u vec16x32u_move_mask_impl(mask16x32u m) {
        return vec16x32u{_mm512_movm_epi32(decay(m))};
    }

    auto vec16x32u_move_mask = vector_vector_mask_test_bench<vec16x32u, vec16x32u_move_mask_impl>;

    BENCHMARK(vec_from_mask_32u::vec16x32u_move_mask);

    #endif



    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_masked_broadcast_impl(mask16x32u m) {
        return vec16x32u{_mm512_maskz_set1_epi32(decay(m), 0x1)};
    }

    auto vec16x32u_masked_broadcast = vector_vector_mask_test_bench<vec16x32u, vec16x32u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_32u::vec16x32u_masked_broadcast);

    #endif

}

#endif //AVEL_VEC_FROM_MASK_32I_HPP
