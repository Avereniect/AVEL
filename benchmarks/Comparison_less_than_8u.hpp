#ifndef AVEL_COMPARISON_LESS_THAN_8U_HPP
#define AVEL_COMPARISON_LESS_THAN_8U_HPP

namespace avel::benchmarks::comparison_less_than_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    bool scalar_native_impl(std::uint8_t x, std::uint8_t y) {
        return x < y;
    }

    auto scalar_native = scalar_comparison_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(comparison_less_than_8u::scalar_native);

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask16x8u vec16x8u_pcmpgtb_xor_impl(vec16x8u lhs, vec16x8u rhs) {
        auto high_bit = _mm_set1_epi8(0x80);
        auto t0 = _mm_xor_si128(decay(lhs), high_bit);
        auto t1 = _mm_xor_si128(decay(rhs), high_bit);
        auto t2 = _mm_cmplt_epi8(t0, t1);
        return mask16x8u{t2};
    }

    auto vec16x8u_pcmpgtb_xor = vector_comparison_test_bench<vec16x8u, vec16x8u_pcmpgtb_xor_impl>;

    BENCHMARK(comparison_less_than_8u::vec16x8u_pcmpgtb_xor);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask16x8u vec16x8u_pcmpub_impl(vec16x8u lhs, vec16x8u rhs) {
        return mask16x8u{_mm_cmp_epi8_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec16x8u_pcmpub = vector_comparison_test_bench<vec16x8u, vec16x8u_pcmpub_impl>;

    BENCHMARK(comparison_less_than_8u::vec16x8u_pcmpub);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !(defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW))

    mask32x8u vec32x8u_pcmpgtb_xor_impl(vec32x8u lhs, vec32x8u rhs) {
        auto high_bit = _mm256_set1_epi8(0x80);
        auto t0 = _mm256_xor_si256(decay(lhs), high_bit);
        auto t1 = _mm256_xor_si256(decay(rhs), high_bit);
        auto t2 = _mm256_cmpgt_epi8(t1, t0);
        return mask32x8u{t2};
    }

    auto vec32x8u_pcmpgtb_xor = vector_comparison_test_bench<vec32x8u, vec32x8u_pcmpgtb_xor_impl>;

    BENCHMARK(comparison_less_than_8u::vec32x8u_pcmpgtb_xor);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    mask32x8u vec32x8u_pcmpub_impl(vec32x8u lhs, vec32x8u rhs) {
        return mask32x8u{_mm256_cmp_epi8_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec32x8u_pcmpub = vector_comparison_test_bench<vec32x8u, vec32x8u_pcmpub_impl>;

    BENCHMARK(comparison_less_than_8u::vec32x8u_pcmpub);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    mask64x8u vec64x8u_pcmpub_impl(vec64x8u lhs, vec64x8u rhs) {
        return mask64x8u{_mm512_cmp_epi8_mask(decay(lhs), decay(rhs), _MM_CMPINT_LT)};
    }

    auto vec64x8u_pcmpub = vector_comparison_test_bench<vec64x8u, vec64x8u_pcmpub_impl>;

    BENCHMARK(comparison_less_than_8u::vec64x8u_pcmpub);

    #endif

}

#endif //AVEL_COMPARISON_LESS_THAN_8U_HPP
