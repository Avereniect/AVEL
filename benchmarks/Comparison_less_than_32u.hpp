#ifndef AVEL_COMPARISON_LESS_THAN_32U_HPP
#define AVEL_COMPARISON_LESS_THAN_32U_HPP

namespace avel::benchmarks::comparison_less_than_32u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::uint32_t x, std::uint32_t y) {
        return x < y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(comparison_less_than_32u::scalar_native);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask4x32u vec4x32u_pcmpgtd_xor_impl(vec4x32u lhs, vec4x32u rhs) {
        auto high_bit = _mm_set1_epi32(0x80000000);
        auto t0 = _mm_xor_si128(decay(lhs), high_bit);
        auto t1 = _mm_xor_si128(decay(rhs), high_bit);
        return mask4x32u{_mm_cmplt_epi32(t0, t1)};
    }

    auto vec4x32u_pcmpgtd_xor = vector_comparison_test_bench<vec4x32u, vec4x32u_pcmpgtd_xor_impl>;

    BENCHMARK(comparison_less_than_32u::vec4x32u_pcmpgtd_xor);

    #endif



    #if defined(AVEL_AVX512VL)

    mask4x32u vec4x32u_pcmpud_impl(vec4x32u lhs, vec4x32u rhs) {
        return mask4x32u{_mm_cmp_epu32_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec4x32u_pcmpud = vector_comparison_test_bench<vec4x32u, vec4x32u_pcmpud_impl>;

    BENCHMARK(comparison_less_than_32u::vec4x32u_pcmpud);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    mask8x32u vec8x32u_pcmpgtd_xor_impl(vec8x32u lhs, vec8x32u rhs) {
        auto high_bit = _mm256_set1_epi32(0x80000000);
        auto t0 = _mm256_xor_si256(decay(lhs), high_bit);
        auto t1 = _mm256_xor_si256(decay(rhs), high_bit);
        return mask8x32u{_mm256_cmpgt_epi32(t1, t0)};
    }

    auto vec8x32u_pcmpgtd_xor = vector_comparison_test_bench<vec8x32u, vec8x32u_pcmpgtd_xor_impl>;

    BENCHMARK(comparison_less_than_32u::vec8x32u_pcmpgtd_xor);

    #endif



    #if defined(AVEL_AVX512VL)

    mask8x32u vec8x32u_pcmpud_impl(vec8x32u lhs, vec8x32u rhs) {
        return mask8x32u{_mm256_cmp_epu32_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec8x32u_pcmpud = vector_comparison_test_bench<vec8x32u, vec8x32u_pcmpud_impl>;

    BENCHMARK(comparison_less_than_32u::vec8x32u_pcmpud);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    mask16x32u vec16x32u_pcmpud_impl(vec16x32u lhs, vec16x32u rhs) {
        return mask16x32u{_mm512_cmp_epu32_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec16x32u_pcmpud = vector_comparison_test_bench<vec16x32u, vec16x32u_pcmpud_impl>;

    BENCHMARK(comparison_less_than_32u::vec16x32u_pcmpud);

    #endif

}

#endif //AVEL_COMPARISON_LESS_THAN_32U_HPP
