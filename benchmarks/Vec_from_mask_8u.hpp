#ifndef AVEL_VEC_FROM_MASK_8U_HPP
#define AVEL_VEC_FROM_MASK_8U_HPP

namespace avel::benchmarks::vec_from_mask_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint8_t scalar_native_conversion_impl(bool x) {
        return static_cast<std::uint8_t>(x);
    }

    auto scalar_native_conversion = scalar_mask_bool_test_bench<std::uint8_t, scalar_native_conversion_impl>;

    BENCHMARK(vec_from_mask_8u::scalar_native_conversion);

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec16x8u vec16x8u_negation_impl(mask16x8u m) {
        return vec16x8u{_mm_sub_epi8(_mm_setzero_si128(), decay(m))};
    }

    auto vec16x8u_negation = vector_vector_mask_test_bench<vec16x8u, vec16x8u_negation_impl>;

    BENCHMARK(vec_from_mask_8u::vec16x8u_negation);

    #endif



    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec16x8u vec16x8u_move_mask_impl(mask16x8u m) {
        return vec16x8u{_mm_movm_epi8(decay(m))};
    }

    auto vec16x8u_move_mask = vector_vector_mask_test_bench<vec16x8u, vec16x8u_move_mask_impl>;

    BENCHMARK(vec_from_mask_8u::vec16x8u_move_mask);

    #endif



    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec16x8u vec16x8u_masked_broadcast_impl(mask16x8u m) {
        return vec16x8u{_mm_maskz_set1_epi8(decay(m), 0x1)};
    }

    auto vec16x8u_masked_broadcast = vector_vector_mask_test_bench<vec16x8u, vec16x8u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_8u::vec16x8u_masked_broadcast);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec32x8u vec32x8u_negation_impl(mask32x8u m) {
        return vec32x8u{_mm256_sub_epi8(_mm256_setzero_si256(), decay(m))};
    }

    auto vec32x8u_negation = vector_vector_mask_test_bench<vec32x8u, vec32x8u_negation_impl>;

    BENCHMARK(vec_from_mask_8u::vec32x8u_negation);

    #endif



    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec32x8u vec32x8u_move_mask_impl(mask32x8u m) {
        return vec32x8u{_mm256_movm_epi8(decay(m))};
    }

    auto vec32x8u_move_mask = vector_vector_mask_test_bench<vec32x8u, vec32x8u_move_mask_impl>;

    BENCHMARK(vec_from_mask_8u::vec32x8u_move_mask);

    #endif



    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    vec32x8u vec32x8u_masked_broadcast_impl(mask32x8u m) {
        return vec32x8u{_mm256_maskz_set1_epi8(decay(m), 0x1)};
    }

    auto vec32x8u_masked_broadcast = vector_vector_mask_test_bench<vec32x8u, vec32x8u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_8u::vec32x8u_masked_broadcast);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_move_mask_impl(mask64x8u m) {
        return vec64x8u{_mm512_movm_epi8(decay(m))};
    }

    auto vec64x8u_move_mask = vector_vector_mask_test_bench<vec64x8u, vec64x8u_move_mask_impl>;

    BENCHMARK(vec_from_mask_8u::vec64x8u_move_mask);

    #endif



    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_masked_broadcast_impl(mask64x8u m) {
        return vec64x8u{_mm512_maskz_set1_epi8(decay(m), 0x1)};
    }

    auto vec64x8u_masked_broadcast = vector_vector_mask_test_bench<vec64x8u, vec64x8u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_8u::vec64x8u_masked_broadcast);

    #endif

}

#endif //AVEL_VEC_FROM_MASK_8U_HPP
