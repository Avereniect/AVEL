#ifndef AVEL_MUL_64U_HPP
#define AVEL_MUL_64U_HPP

namespace avel::benchmarks::mul_64u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint64_t scalar_native_impl(std::uint64_t x, std::uint64_t y) {
        return x * y;
    }

    auto scalar_native = scalar_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(mul_64u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_scalarization_impl(vec2x64u v, vec2x64u w) {
        auto lhs_lo = _mm_cvtsi128_si64(decay(v));
        auto lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(v), 0x8));

        auto rhs_lo = _mm_cvtsi128_si64(decay(w));
        auto rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(w), 0x8));

        auto out_lo = _mm_cvtsi64_si128(std::uint64_t(lhs_lo) * std::uint64_t(rhs_lo));
        auto out_hi = _mm_cvtsi64_si128(std::uint64_t(lhs_hi) * std::uint64_t(rhs_hi));

        return vec2x64u{_mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8))};
    }

    auto vec2x64u_scalarization = vector_test_bench<vec2x64u, vec2x64u_scalarization_impl>;

    BENCHMARK(mul_64u::vec2x64u_scalarization);

    #endif



    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_muludq_emulation_impl(vec2x64u v, vec2x64u w) {
        auto t0 = _mm_mul_epu32(decay(v), decay(w));
        auto t1 = _mm_mul_epu32(_mm_srli_epi64(decay(w), 32), decay(v));
        auto t2 = _mm_mul_epu32(_mm_srli_epi64(decay(v), 32), decay(w));

        auto t3 = _mm_slli_epi64(_mm_add_epi32(t1, t2), 32);
        return vec2x64u{_mm_add_epi32(t0, t3)};
    }

    auto vec2x64u_muludq_emulation = vector_test_bench<vec2x64u, vec2x64u_muludq_emulation_impl>;

    BENCHMARK(mul_64u::vec2x64u_muludq_emulation);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec2x64u vec2x64u_mullq_impl(vec2x64u v, vec2x64u w) {
        return vec2x64u{_mm_mullo_epi64(decay(v), decay(w))};
    }

    auto vec2x64u_mullq = vector_test_bench<vec2x64u, vec2x64u_mullq_impl>;

    BENCHMARK(mul_64u::vec2x64u_mullq);

    #endif



    #if defined(AVEL_PCLMULQDQ)

    vec2x64u vec2x64u_vpclmulqdq_impl(vec2x64u v, vec2x64u w) {
        auto prod_even = _mm_clmulepi64_si128(decay(v), decay(w), 0x00);
        auto prod_odd  = _mm_clmulepi64_si128(decay(v), decay(w), 0x11);
        auto products = _mm_unpackhi_epi64(prod_even, prod_odd);

        return vec2x64u{products};
    }

    auto vec2x64u_vpclmulqdq = vector_test_bench<vec2x64u, vec2x64u_vpclmulqdq_impl>;

    BENCHMARK(mul_64u::vec2x64u_vpclmulqdq);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_muludq_emulation_impl(vec4x64u v, vec4x64u w) {
        auto t0 = _mm256_mul_epu32(decay(v), decay(w));
        auto t1 = _mm256_mul_epu32(_mm256_srli_epi64(decay(w), 32), decay(v));
        auto t2 = _mm256_mul_epu32(_mm256_srli_epi64(decay(v), 32), decay(w));

        auto t3 = _mm256_slli_epi64(_mm256_add_epi32(t1, t2), 32);
        return vec4x64u{_mm256_add_epi32(t0, t3)};
    }

    auto vec4x64u_muludq_emulation = vector_test_bench<vec4x64u, vec4x64u_muludq_emulation_impl>;

    BENCHMARK(mul_64u::vec4x64u_muludq_emulation);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec4x64u vec4x64u_mullq_impl(vec4x64u v, vec4x64u w) {
        return vec4x64u{_mm256_mullo_epi64(decay(v), decay(w))};
    }

    auto vec4x64u_mullq = vector_test_bench<vec4x64u, vec4x64u_mullq_impl>;

    BENCHMARK(mul_64u::vec4x64u_mullq);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_PCLMULQDQ)

    vec4x64u vec4x64u_vpclmulqdq_impl(vec4x64u v, vec4x64u w) {
        auto prod_even = _mm256_clmulepi64_epi128(decay(v), decay(w), 0x00);
        auto prod_odd  = _mm256_clmulepi64_epi128(decay(v), decay(w), 0x11);
        auto products = _mm256_unpackhi_epi64(prod_even, prod_odd);

        return vec4x64u{products};
    }

    auto vec4x64u_vpclmulqdq = vector_test_bench<vec4x64u, vec4x64u_vpclmulqdq_impl>;

    BENCHMARK(mul_64u::vec4x64u_vpclmulqdq);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_muludq_emulation_impl(vec8x64u v, vec8x64u w) {
        auto t0 = _mm512_mul_epu32(decay(v), decay(w));
        auto t1 = _mm512_mul_epu32(_mm512_srli_epi64(decay(w), 32), decay(v));
        auto t2 = _mm512_mul_epu32(_mm512_srli_epi64(decay(v), 32), decay(w));

        auto t3 = _mm512_slli_epi64(_mm512_add_epi32(t1, t2), 32);
        return vec8x64u{_mm512_add_epi32(t0, t3)};
    }

    auto vec8x64u_muludq_emulation = vector_test_bench<vec8x64u, vec8x64u_muludq_emulation_impl>;

    BENCHMARK(mul_64u::vec8x64u_muludq_emulation);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec8x64u vec8x64u_mullq_impl(vec8x64u v, vec8x64u w) {
        return vec8x64u{_mm512_mullo_epi64(decay(v), decay(w))};
    }

    auto vec8x64u_mullq = vector_test_bench<vec8x64u, vec8x64u_mullq_impl>;

    BENCHMARK(mul_64u::vec8x64u_mullq);

    #endif



    #if defined(AVEL_AVX512F) && defined(AVEL_PCLMULQDQ)

    vec8x64u vec8x64u_vpclmulqdq_impl(vec8x64u v, vec8x64u w) {
        auto prod_even = _mm512_clmulepi64_epi128(decay(v), decay(w), 0x00);
        auto prod_odd  = _mm512_clmulepi64_epi128(decay(v), decay(w), 0x11);
        auto products = _mm512_unpackhi_epi64(prod_even, prod_odd);

        return vec8x64u{products};
    }

    auto vec8x64u_vpclmulqdq = vector_test_bench<vec8x64u, vec8x64u_vpclmulqdq_impl>;

    BENCHMARK(mul_64u::vec8x64u_vpclmulqdq);

    #endif

}

#endif //AVEL_MUL_64U_HPP
