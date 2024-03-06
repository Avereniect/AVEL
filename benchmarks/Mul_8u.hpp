#ifndef AVEL_MUL_8U_HPP
#define AVEL_MUL_8U_HPP

namespace avel::benchmarks::mul_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint8_t scalar_native_impl(std::uint8_t x, std::uint8_t y) {
        return x * y;
    }

    auto scalar_native = scalar_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(mul_8u::scalar_native);

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_pmullw_emulation_impl(vec16x8u v, vec16x8u w) {
        auto even_mask = _mm_set1_epi16(0x00FF);

        auto products_even = _mm_and_si128(even_mask, _mm_mullo_epi16(decay(v), decay(w)));
        auto products_odd  = _mm_mullo_epi16(
            _mm_srli_epi16(decay(v), 8),
            _mm_andnot_si128(even_mask, decay(w))
        );

        auto products = _mm_or_si128(products_odd, products_even);
        return vec16x8u{products};
    }

    auto vec16x8u_pmullw_emulation = vector_test_bench<vec16x8u, vec16x8u_pmullw_emulation_impl>;

    BENCHMARK(mul_8u::vec16x8u_pmullw_emulation);

    #endif



    #if defined(AVEL_SSE4_1)

    vec16x8u vec16x8u_pmullw_blend_emulation_impl(vec16x8u v, vec16x8u w) {
        auto even_mask = _mm_set1_epi16(0x00FF);

        auto products_even = _mm_and_si128(even_mask, _mm_mullo_epi16(decay(v), decay(w)));
        auto products_odd  = _mm_mullo_epi16(
            _mm_srli_epi16(decay(v), 8),
            _mm_andnot_si128(even_mask, decay(w))
        );

        auto products = _mm_blendv_epi8(products_odd, products_even, even_mask);
        return vec16x8u{products};
    }

    auto vec16x8u_pmullw_blend_emulation = vector_test_bench<vec16x8u, vec16x8u_pmullw_blend_emulation_impl>;

    BENCHMARK(mul_8u::vec16x8u_pmullw_blend_emulation);

    #endif



    #if defined(AVEL_AVX2)

    vec16x8u vec16x8u_pmullw_widening_emulation_impl(vec16x8u v, vec16x8u w) {
        auto a = _mm256_cvtepu8_epi16(decay(v));
        auto b = _mm256_cvtepu8_epi16(decay(w));

        auto c = _mm256_mullo_epi16(a, b);
        return vec16x8u{_mm256_cvtepi16_epi8(c)};
    }

    auto vec16x8u_pmullw_widening_emulation = vector_test_bench<vec16x8u, vec16x8u_pmullw_widening_emulation_impl>;

    BENCHMARK(mul_8u::vec16x8u_pmullw_widening_emulation);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    vec16x8u vec16x8u_pmullw_avx512_emulation_impl(vec16x8u v, vec16x8u w) {
        constexpr std::uint16_t odd_mask = 0xaaaa;

        auto even_products = _mm_mullo_epi16(decay(v), decay(w));
        auto odd_products = _mm_mullo_epi16(
            _mm_srli_epi16(decay(v), 8),
            _mm_maskz_mov_epi8(odd_mask, decay(w))
        );

        auto products = _mm_mask_blend_epi8(odd_mask, even_products, odd_products);

        return vec16x8u{products};
    }

    auto vec16x8u_pmullw_avx512_emulation = vector_test_bench<vec16x8u, vec16x8u_pmullw_avx512_emulation_impl>;

    BENCHMARK(mul_8u::vec16x8u_pmullw_avx512_emulation);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_pmullw_emulation_impl(vec32x8u v, vec32x8u w) {
        auto even_mask = _mm256_set1_epi16(0x00FF);

        auto products_even = _mm256_mullo_epi16(decay(v), decay(w));
        auto products_odd  = _mm256_mullo_epi16(
            _mm256_srli_epi16(decay(v), 8),
            _mm256_andnot_si256(even_mask, decay(w))
        );

        auto products = _mm256_blendv_epi8(products_odd, products_even, even_mask);
        return vec32x8u{products};
    }

    auto vec32x8u_pmullw_emulation = vector_test_bench<vec32x8u, vec32x8u_pmullw_emulation_impl>;

    BENCHMARK(mul_8u::vec32x8u_pmullw_emulation);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    vec32x8u vec32x8u_pmullw_widening_emulation_impl(vec32x8u v, vec32x8u w) {
        auto a = _mm512_cvtepu8_epi16(decay(v));
        auto b = _mm512_cvtepu8_epi16(decay(w));

        auto c = _mm512_mullo_epi16(a, b);
        return vec32x8u{_mm512_cvtepi16_epi8(c)};
    }

    auto vec32x8u_pmullw_widening_emulation = vector_test_bench<vec32x8u, vec32x8u_pmullw_widening_emulation_impl>;

    BENCHMARK(mul_8u::vec32x8u_pmullw_widening_emulation);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    vec32x8u vec32x8u_pmullw_avx512_emulation_impl(vec32x8u v, vec32x8u w) {
        constexpr std::uint32_t odd_mask = 0xaaaaaaaaull;

        auto even_products = _mm256_mullo_epi16(decay(v), decay(w));
        auto odd_products = _mm256_mullo_epi16(
            _mm256_srli_epi16(decay(v), 8),
            _mm256_maskz_mov_epi8(odd_mask, decay(w))
        );

        auto products = _mm256_mask_blend_epi8(odd_mask, even_products, odd_products);

        return vec32x8u{products};
    }

    auto vec32x8u_pmullw_avx512_emulation = vector_test_bench<vec32x8u, vec32x8u_pmullw_avx512_emulation_impl>;

    BENCHMARK(mul_8u::vec32x8u_pmullw_avx512_emulation);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_pmullw_emulation_impl(vec64x8u v, vec64x8u w) {
        constexpr std::uint64_t odd_mask = 0xaaaaaaaaaaaaaaaaull;

        auto even_products = _mm512_mullo_epi16(decay(v), decay(w));
        auto odd_products = _mm512_mullo_epi16(
            _mm512_srli_epi16(decay(v), 8),
            _mm512_maskz_mov_epi8(odd_mask, decay(w))
        );

        auto products = _mm512_mask_blend_epi8(odd_mask, even_products, odd_products);

        return vec64x8u{products};
    }

    auto vec64x8u_pmullw_emulation = vector_test_bench<vec64x8u, vec64x8u_pmullw_emulation_impl>;

    BENCHMARK(mul_8u::vec64x8u_pmullw_emulation);

    #endif

}

#endif //AVEL_MUL_8U_HPP
