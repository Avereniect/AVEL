#ifndef AVEL_VEC_FROM_SCALAR_16U_HPP
#define AVEL_VEC_FROM_SCALAR_16U_HPP

namespace avel::benchmarks::vec_from_scalar_16u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint16_t scalar_identity_impl(std::uint16_t x) {
        return x;
    }

    auto scalar_identity = scalar_test_bench<std::uint16_t, scalar_identity_impl>;

    BENCHMARK(vec_from_scalar_16u::scalar_identity);

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec8x16u vec8x16u_unpack_shuffle_impl(std::uint16_t x) {
        auto tmp0 = _mm_cvtsi32_si128(x);
        auto tmp1 = _mm_unpacklo_epi16(tmp0, tmp0);
        auto tmp2 = _mm_shuffle_epi32(tmp1, 0x00);
        return vec8x16u{tmp2};
    }

    auto vec8x16u_unpack_shuffle = vector_scalar_test_bench<vec8x16u, vec8x16u_unpack_shuffle_impl>;

    BENCHMARK(vec_from_scalar_16u::vec8x16u_unpack_shuffle);

    #endif



    #if defined(AVEL_SSE2)

    vec8x16u vec8x16u_shift_shuffle_impl(std::uint16_t x) {
        auto tmp0 = _mm_cvtsi32_si128(x + (x << 16));
        auto tmp1 = _mm_shuffle_epi32(tmp0, 0x00);
        return vec8x16u{tmp1};
    }

    auto vec8x16u_shift_shuffle = vector_scalar_test_bench<vec8x16u, vec8x16u_shift_shuffle_impl>;

    BENCHMARK(vec_from_scalar_16u::vec8x16u_shift_shuffle);

    #endif



    #if defined(AVEL_SSE2)

    vec8x16u vec8x16u_mul_shuffle_impl(std::uint16_t x) {
        auto tmp0 = _mm_cvtsi32_si128(x * 0x00010001);
        auto tmp1 = _mm_shuffle_epi32(tmp0, 0x00);
        return vec8x16u{tmp1};
    }

    auto vec8x16u_mul_shuffle = vector_scalar_test_bench<vec8x16u, vec8x16u_mul_shuffle_impl>;

    BENCHMARK(vec_from_scalar_16u::vec8x16u_mul_shuffle);

    #endif



    #if defined(AVEL_SSSE3)

    vec8x16u vec8x16u_pshufb_impl(std::uint16_t x) {
        alignas(16) static constexpr std::uint8_t table[16] {
            0, 1, 0, 1,
            0, 1, 0, 1,
            0, 1, 0, 1,
            0, 1, 0, 1
        };

        auto tmp0 = _mm_cvtsi32_si128(x);
        auto tmp1 = _mm_shuffle_epi8(tmp0, _mm_load_si128(reinterpret_cast<const __m128i*>(table)));
        return vec8x16u{tmp1};
    }

    auto vec8x16u_pshufb = vector_scalar_test_bench<vec8x16u, vec8x16u_pshufb_impl>;

    BENCHMARK(vec_from_scalar_16u::vec8x16u_pshufb);

    #endif



    #if defined(AVEL_AVX2)

    vec8x16u vec8x16u_broadcast_impl(std::uint16_t x) {
        return vec8x16u{_mm_set1_epi16(x)};
    }

    auto vec8x16u_broadcast = vector_scalar_test_bench<vec8x16u, vec8x16u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_16u::vec8x16u_broadcast);

    #endif



    #if defined(AVEL_AVX2)

    vec8x16u vec8x16u_broadcast_from_vector_impl(std::uint16_t x) {
        return vec8x16u{_mm_broadcastw_epi16(_mm_cvtsi32_si128(x))};
    }

    auto vec8x16u_broadcast_from_vector = vector_scalar_test_bench<vec8x16u, vec8x16u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_16u::vec8x16u_broadcast_from_vector);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_broadcast_impl(std::uint16_t x) {
        return vec16x16u{_mm256_set1_epi16(x)};
    }

    auto vec16x16u_broadcast = vector_scalar_test_bench<vec16x16u, vec16x16u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_16u::vec16x16u_broadcast);

    #endif



    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_broadcast_from_vector_impl(std::uint16_t x) {
        return vec16x16u{_mm256_broadcastw_epi16(_mm_cvtsi32_si128(x))};
    }

    auto vec16x16u_broadcast_from_vector = vector_scalar_test_bench<vec16x16u, vec16x16u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_16u::vec16x16u_broadcast_from_vector);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_broadcast_impl(std::uint16_t x) {
        return vec32x16u{_mm512_set1_epi16(x)};
    }

    auto vec32x16u_broadcast = vector_scalar_test_bench<vec32x16u, vec32x16u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_16u::vec32x16u_broadcast);

    #endif



    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_broadcast_from_vector_impl(std::uint16_t x) {
        return vec32x16u{_mm512_broadcastw_epi16(_mm_cvtsi32_si128(x))};
    }

    auto vec32x16u_broadcast_from_vector = vector_scalar_test_bench<vec32x16u, vec32x16u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_16u::vec32x16u_broadcast_from_vector);

    #endif

}


#endif //AVEL_VEC_FROM_SCALAR_16U_HPP
