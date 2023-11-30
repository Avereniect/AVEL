#ifndef AVEL_VEC_FROM_MASK_16U_HPP
#define AVEL_VEC_FROM_MASK_16U_HPP

namespace avel::benchmarks::vec_from_mask_16u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    std::uint16_t scalar_native_conversion_impl(bool x) {
        return static_cast<std::uint16_t>(x);
    }

    auto scalar_native_conversion = scalar_mask_bool_test_bench<std::uint16_t, scalar_native_conversion_impl>;

    BENCHMARK(vec_from_mask_16u::scalar_native_conversion);

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec8x16u vec8x16u_negation_impl(mask8x16u m) {
        return vec8x16u{_mm_sub_epi16(_mm_setzero_si128(), decay(m))};
    }

    auto vec8x16u_negation = vector_vector_mask_test_bench<vec8x16u, vec8x16u_negation_impl>;

    BENCHMARK(vec_from_mask_16u::vec8x16u_negation);

    #endif



    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec8x16u vec8x16u_move_mask_impl(mask8x16u m) {
        return vec8x16u{_mm_movm_epi16(decay(m))};
    }

    auto vec8x16u_move_mask = vector_vector_mask_test_bench<vec8x16u, vec8x16u_move_mask_impl>;

    BENCHMARK(vec_from_mask_16u::vec8x16u_move_mask);

    #endif



    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec8x16u vec8x16u_masked_broadcast_impl(mask8x16u m) {
        return vec8x16u{_mm_maskz_set1_epi16(decay(m), 0x1)};
    }

    auto vec8x16u_masked_broadcast = vector_vector_mask_test_bench<vec8x16u, vec8x16u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_16u::vec8x16u_masked_broadcast);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec16x16u vec16x16u_negation_impl(mask16x16u m) {
        return vec16x16u{_mm256_sub_epi16(_mm256_setzero_si256(), decay(m))};
    }

    auto vec16x16u_negation = vector_vector_mask_test_bench<vec16x16u, vec16x16u_negation_impl>;

    BENCHMARK(vec_from_mask_16u::vec16x16u_negation);

    #endif



    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec16x16u vec16x16u_move_mask_impl(mask16x16u m) {
        return vec16x16u{_mm256_movm_epi16(decay(m))};
    }

    auto vec16x16u_move_mask = vector_vector_mask_test_bench<vec16x16u, vec16x16u_move_mask_impl>;

    BENCHMARK(vec_from_mask_16u::vec16x16u_move_mask);

    #endif



    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec16x16u vec16x16u_masked_broadcast_impl(mask16x16u m) {
        return vec16x16u{_mm256_maskz_set1_epi16(decay(m), 0x1)};
    }

    auto vec16x16u_masked_broadcast = vector_vector_mask_test_bench<vec16x16u, vec16x16u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_16u::vec16x16u_masked_broadcast);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_move_mask_impl(mask32x16u m) {
        return vec32x16u{_mm512_movm_epi16(decay(m))};
    }

    auto vec32x16u_move_mask = vector_vector_mask_test_bench<vec32x16u, vec32x16u_move_mask_impl>;

    BENCHMARK(vec_from_mask_16u::vec32x16u_move_mask);

    #endif



    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_masked_broadcast_impl(mask32x16u m) {
        return vec32x16u{_mm512_maskz_set1_epi16(decay(m), 0x1)};
    }

    auto vec32x16u_masked_broadcast = vector_vector_mask_test_bench<vec32x16u, vec32x16u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_16u::vec32x16u_masked_broadcast);

    #endif

}

#endif //AVEL_VEC_FROM_MASK_16U_HPP
