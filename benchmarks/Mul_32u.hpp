#ifndef AVEL_MUL_32U_HPP
#define AVEL_MUL_32U_HPP

namespace avel::benchmarks::mul_32u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint32_t scalar_native_impl(std::uint32_t x, std::uint32_t y) {
        return x * y;
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(mul_32u::scalar_native);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_muludq_unpack_emulation_impl(vec4x32u v, vec4x32u w) {
        auto lo = _mm_mul_epu32(decay(v), decay(w));
        auto hi = _mm_mul_epu32(_mm_srli_si128(decay(v), 4), _mm_srli_si128(decay(w), 4));

        auto half0 = _mm_unpacklo_epi32(lo, hi);
        auto half1 = _mm_unpackhi_epi32(lo, hi);

        return vec4x32u{_mm_unpacklo_epi64(half0, half1)};
    }

    auto vec4x32u_muludq_unpack_emulation = vector_test_bench<vec4x32u, vec4x32u_muludq_unpack_emulation_impl>;

    BENCHMARK(mul_32u::vec4x32u_muludq_unpack_emulation);

    #endif



    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_muludq_mask_and_shift_emulation_impl(vec4x32u v, vec4x32u w) {
        auto even_mask = _mm_set1_epi64x(0xffffffff);

        auto even_products = _mm_and_si128(even_mask, _mm_mul_epu32(decay(v), decay(w)));
        auto odd_products = _mm_slli_si128(_mm_mul_epu32(_mm_srli_si128(decay(v), 4), _mm_srli_si128(decay(w), 4)), 4);

        auto result = _mm_or_si128(even_products, odd_products);

        return vec4x32u{result};
    }

    auto vec4x32u_muludq_mask_and_shift_emulation = vector_test_bench<vec4x32u, vec4x32u_muludq_mask_and_shift_emulation_impl>;

    BENCHMARK(mul_32u::vec4x32u_muludq_mask_and_shift_emulation);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_mullw_impl(vec4x32u v, vec4x32u w) {
        return vec4x32u{_mm_mullo_epi32(decay(v), decay(w))};
    }

    auto vec4x32u_mullw = vector_test_bench<vec4x32u, vec4x32u_mullw_impl>;

    BENCHMARK(mul_32u::vec4x32u_mullw);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_mullw_impl(vec8x32u v, vec8x32u w) {
        return vec8x32u{_mm256_mullo_epi32(decay(v), decay(w))};
    }

    auto vec8x32u_mullw = vector_test_bench<vec8x32u, vec8x32u_mullw_impl>;

    BENCHMARK(mul_32u::vec8x32u_mullw);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_mullw_impl(vec16x32u v, vec16x32u w) {
        return vec16x32u{_mm512_mullo_epi32(decay(v), decay(w))};
    }

    auto vec16x32u_mullw = vector_test_bench<vec16x32u, vec16x32u_mullw_impl>;

    BENCHMARK(mul_32u::vec16x32u_mullw);

    #endif

}

#endif //AVEL_MUL_32U_HPP
