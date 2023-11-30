#ifndef AVEL_VEC_FROM_SCALAR_8U_HPP
#define AVEL_VEC_FROM_SCALAR_8U_HPP

namespace avel::benchmarks::vec_from_scalar_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint8_t scalar_identity_impl(std::uint8_t x) {
        return x;
    }

    auto scalar_identity = scalar_test_bench<std::uint8_t, scalar_identity_impl>;

    BENCHMARK(vec_from_scalar_8u::scalar_identity);

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_unpack_shuffle_impl(std::uint8_t x) {
        auto tmp0 = _mm_cvtsi32_si128(x);
        auto tmp1 = _mm_unpacklo_epi8(tmp0, tmp0);
        auto tmp2 = _mm_unpacklo_epi16(tmp1, tmp1);
        auto tmp3 = _mm_shuffle_epi32(tmp2, 0x00);
        return vec16x8u{tmp3};
    }

    auto vec16x8u_unpack_shuffle = vector_scalar_test_bench<vec16x8u, vec16x8u_unpack_shuffle_impl>;

    BENCHMARK(vec_from_scalar_8u::vec16x8u_unpack_shuffle);

    #endif



    #if defined(AVEL_SSSE3)

    vec16x8u vec16x8u_pshufb_impl(std::uint8_t x) {
        auto tmp0 = _mm_cvtsi32_si128(x);
        auto tmp1 = _mm_shuffle_epi8(tmp0, _mm_setzero_si128());
        return vec16x8u{tmp1};
    }

    auto vec16x8u_pshufb = vector_scalar_test_bench<vec16x8u, vec16x8u_pshufb_impl>;

    BENCHMARK(vec_from_scalar_8u::vec16x8u_pshufb);

    #endif



    #if defined(AVEL_AVX2)

    vec16x8u vec16x8u_broadcast_impl(std::uint8_t x) {
        return vec16x8u{_mm_set1_epi8(x)};
    }

    auto vec16x8u_broadcast = vector_scalar_test_bench<vec16x8u, vec16x8u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_8u::vec16x8u_broadcast);

    #endif



    #if defined(AVEL_AVX2)

    vec16x8u vec16x8u_broadcast_from_vector_impl(std::uint8_t x) {
        return vec16x8u{_mm_broadcastb_epi8(_mm_cvtsi32_si128(x))};
    }

    auto vec16x8u_broadcast_from_vector = vector_scalar_test_bench<vec16x8u, vec16x8u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_8u::vec16x8u_broadcast_from_vector);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_broadcast_impl(std::uint8_t x) {
        return vec32x8u{_mm256_set1_epi8(x)};
    }

    auto vec32x8u_broadcast = vector_scalar_test_bench<vec32x8u, vec32x8u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_8u::vec32x8u_broadcast);

    #endif



    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_broadcast_from_vector_impl(std::uint8_t x) {
        return vec32x8u{_mm256_broadcastb_epi8(_mm_cvtsi32_si128(x))};
    }

    auto vec32x8u_broadcast_from_vector = vector_scalar_test_bench<vec32x8u, vec32x8u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_8u::vec32x8u_broadcast_from_vector);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_broadcast_impl(std::uint8_t x) {
        return vec64x8u{_mm512_set1_epi8(x)};
    }

    auto vec64x8u_broadcast = vector_scalar_test_bench<vec64x8u, vec64x8u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_8u::vec64x8u_broadcast);

    #endif



    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_broadcast_from_vector_impl(std::uint8_t x) {
        return vec64x8u{_mm512_broadcastb_epi8(_mm_cvtsi32_si128(x))};
    }

    auto vec64x8u_broadcast_from_vector = vector_scalar_test_bench<vec64x8u, vec64x8u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_8u::vec64x8u_broadcast_from_vector);

    #endif

}

#endif //AVEL_VEC_FROM_SCALAR_8U_HPP
