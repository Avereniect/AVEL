#ifndef AVEL_COMPARISON_LESS_THAN_16U_HPP
#define AVEL_COMPARISON_LESS_THAN_16U_HPP

namespace avel::benchmarks::comparison_less_than_16u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::uint16_t x, std::uint16_t y) {
        return x < y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::uint16_t, scalar_native_impl>;

    BENCHMARK(comparison_less_than_16u::scalar_native);

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask8x16u vec8x16u_pcmpgtw_xor_impl(vec8x16u lhs, vec8x16u rhs) {
        auto high_bit = _mm_set1_epi16(0x8000);
        auto t0 = _mm_xor_si128(decay(lhs), high_bit);
        auto t1 = _mm_xor_si128(decay(rhs), high_bit);
        return mask8x16u{_mm_cmplt_epi16(t0, t1)};
    }

    auto vec8x16u_pcmpgtw_xor = vector_comparison_test_bench<vec8x16u, vec8x16u_pcmpgtw_xor_impl>;

    BENCHMARK(comparison_less_than_16u::vec8x16u_pcmpgtw_xor);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask8x16u vec8x16u_pcmpuw_impl(vec8x16u lhs, vec8x16u rhs) {
        return mask8x16u{_mm_cmp_epu16_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec8x16u_pcmpuw = vector_comparison_test_bench<vec8x16u, vec8x16u_pcmpuw_impl>;

    BENCHMARK(comparison_less_than_16u::vec8x16u_pcmpuw);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask16x16u vec16x16u_pcmpgtw_xor_impl(vec16x16u lhs, vec16x16u rhs) {
        auto high_bit = _mm256_set1_epi16(0x8000);
        auto t0 = _mm256_xor_si256(decay(lhs), high_bit);
        auto t1 = _mm256_xor_si256(decay(rhs), high_bit);
        return mask16x16u{_mm256_cmpgt_epi16(t1, t0)};
    }

    auto vec16x16u_pcmpgtw_xor = vector_comparison_test_bench<vec16x16u, vec16x16u_pcmpgtw_xor_impl>;

    BENCHMARK(comparison_less_than_16u::vec16x16u_pcmpgtw_xor);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask16x16u vec16x16u_pcmpuw_impl(vec16x16u lhs, vec16x16u rhs) {
        return mask16x16u{_mm256_cmp_epu16_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec16x16u_pcmpuw = vector_comparison_test_bench<vec16x16u, vec16x16u_pcmpuw_impl>;

    BENCHMARK(comparison_less_than_16u::vec16x16u_pcmpuw);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    mask32x16u vec32x16u_pcmpuw_impl(vec32x16u lhs, vec32x16u rhs) {
        return mask32x16u{_mm512_cmp_epu16_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec32x16u_pcmpuw = vector_comparison_test_bench<vec32x16u, vec32x16u_pcmpuw_impl>;

    BENCHMARK(comparison_less_than_16u::vec32x16u_pcmpuw);

    #endif

}

#endif //AVEL_COMPARISON_LESS_THAN_16U_HPP
