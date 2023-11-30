#ifndef AVEL_VEC_FROM_MASK_64U_HPP
#define AVEL_VEC_FROM_MASK_64U_HPP

namespace avel::benchmarks::vec_from_mask_64u {

    //=====================================================
    // scalar 64u benchmarks
    //=====================================================

    std::uint64_t scalar_native_conversion_impl(bool x) {
        return static_cast<std::uint64_t>(x);
    }

    auto scalar_native_conversion = scalar_mask_bool_test_bench<std::uint64_t, scalar_native_conversion_impl>;

    BENCHMARK(vec_from_mask_64u::scalar_native_conversion);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    vec2x64u vec2x64u_negation_impl(mask2x64u m) {
        return vec2x64u{_mm_sub_epi64(_mm_setzero_si128(), decay(m))};
    }

    auto vec2x64u_negation = vector_vector_mask_test_bench<vec2x64u, vec2x64u_negation_impl>;

    BENCHMARK(vec_from_mask_64u::vec2x64u_negation);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec2x64u vec2x64u_move_mask_impl(mask2x64u m) {
        return vec2x64u{_mm_movm_epi64(decay(m))};
    }

    auto vec2x64u_move_mask = vector_vector_mask_test_bench<vec2x64u, vec2x64u_move_mask_impl>;

    BENCHMARK(vec_from_mask_64u::vec2x64u_move_mask);

    #endif



    #if defined(AVEL_AVX512VL)

    vec2x64u vec2x64u_masked_broadcast_impl(mask2x64u m) {
        return vec2x64u{_mm_maskz_set1_epi64(decay(m), 0x1)};
    }

    auto vec2x64u_masked_broadcast = vector_vector_mask_test_bench<vec2x64u, vec2x64u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_64u::vec2x64u_masked_broadcast);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    vec4x64u vec4x64u_negation_impl(mask4x64u m) {
        return vec4x64u{_mm256_sub_epi64(_mm256_setzero_si256(), decay(m))};
    }

    auto vec4x64u_negation = vector_vector_mask_test_bench<vec4x64u, vec4x64u_negation_impl>;

    BENCHMARK(vec_from_mask_64u::vec4x64u_negation);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec4x64u vec4x64u_move_mask_impl(mask4x64u m) {
        return vec4x64u{_mm256_movm_epi64(decay(m))};
    }

    auto vec4x64u_move_mask = vector_vector_mask_test_bench<vec4x64u, vec4x64u_move_mask_impl>;

    BENCHMARK(vec_from_mask_64u::vec4x64u_move_mask);

    #endif



    #if defined(AVEL_AVX512VL)

    vec4x64u vec4x64u_masked_broadcast_impl(mask4x64u m) {
        return vec4x64u{_mm256_maskz_set1_epi64(decay(m), 0x1)};
    }

    auto vec4x64u_masked_broadcast = vector_vector_mask_test_bench<vec4x64u, vec4x64u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_64u::vec4x64u_masked_broadcast);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512DQ)

    vec8x64u vec8x64u_move_mask_impl(mask8x64u m) {
        return vec8x64u{_mm512_movm_epi64(decay(m))};
    }

    auto vec8x64u_move_mask = vector_vector_mask_test_bench<vec8x64u, vec8x64u_move_mask_impl>;

    BENCHMARK(vec_from_mask_64u::vec8x64u_move_mask);

    #endif



    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_masked_broadcast_impl(mask8x64u m) {
        return vec8x64u{_mm512_maskz_set1_epi64(decay(m), 0x1)};
    }

    auto vec8x64u_masked_broadcast = vector_vector_mask_test_bench<vec8x64u, vec8x64u_masked_broadcast_impl>;

    BENCHMARK(vec_from_mask_64u::vec8x64u_masked_broadcast);

    #endif

}

#endif //AVEL_VEC_FROM_MASK_64U_HPP
