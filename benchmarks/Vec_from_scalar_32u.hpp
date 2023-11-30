#ifndef AVEL_VEC_FROM_SCALAR_32U_HPP
#define AVEL_VEC_FROM_SCALAR_32U_HPP

namespace avel::benchmarks::vec_from_scalar_32u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint32_t scalar_identity_impl(std::uint32_t x) {
        return x;
    }

    auto scalar_identity = scalar_test_bench<std::uint32_t, scalar_identity_impl>;

    BENCHMARK(vec_from_scalar_32u::scalar_identity);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_shuffle_impl(std::uint32_t x) {
        auto tmp0 = _mm_shuffle_epi32(_mm_cvtsi32_si128(x), 0x00);
        return vec4x32u{tmp0};
    }

    auto vec4x32u_shuffle = vector_scalar_test_bench<vec4x32u, vec4x32u_shuffle_impl>;

    BENCHMARK(vec_from_scalar_32u::vec4x32u_shuffle);

    #endif



    #if defined(AVEL_AVX2)

    vec4x32u vec4x32u_broadcast_impl(std::uint32_t x) {
        return vec4x32u{_mm_set1_epi32(x)};
    }

    auto vec4x32u_broadcast = vector_scalar_test_bench<vec4x32u, vec4x32u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_32u::vec4x32u_broadcast);

    #endif




    #if defined(AVEL_AVX2)

    vec4x32u vec4x32u_broadcast_from_vector_impl(std::uint32_t x) {
        return vec4x32u{_mm_broadcastd_epi32(_mm_cvtsi32_si128(x))};
    }

    auto vec4x32u_broadcast_from_vector = vector_scalar_test_bench<vec4x32u, vec4x32u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_32u::vec4x32u_broadcast_from_vector);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_broadcast_impl(std::uint32_t x) {
        return vec8x32u{_mm256_set1_epi32(x)};
    }

    auto vec8x32u_broadcast = vector_scalar_test_bench<vec8x32u, vec8x32u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_32u::vec8x32u_broadcast);

    #endif



    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_broadcast_from_vector_impl(std::uint32_t x) {
        return vec8x32u{_mm256_broadcastd_epi32(_mm_cvtsi32_si128(x))};
    }

    auto vec8x32u_broadcast_from_vector = vector_scalar_test_bench<vec8x32u, vec8x32u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_32u::vec8x32u_broadcast_from_vector);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_broadcast_impl(std::uint32_t x) {
        return vec16x32u{_mm512_set1_epi32(x)};
    }

    auto vec16x32u_broadcast = vector_scalar_test_bench<vec16x32u, vec16x32u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_32u::vec16x32u_broadcast);

    #endif



    #if defined(AVEL_AVX512BW)

    vec16x32u vec16x32u_broadcast_from_vector_impl(std::uint32_t x) {
        return vec16x32u{_mm512_broadcastd_epi32(_mm_cvtsi32_si128(x))};
    }

    auto vec16x32u_broadcast_from_vector = vector_scalar_test_bench<vec16x32u, vec16x32u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_32u::vec16x32u_broadcast_from_vector);

    #endif

}

#endif //AVEL_VEC_FROM_SCALAR_32U_HPP
