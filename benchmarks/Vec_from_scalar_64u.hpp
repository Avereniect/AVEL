#ifndef AVEL_VEC_FROM_SCALAR_64U_HPP
#define AVEL_VEC_FROM_SCALAR_64U_HPP

namespace avel::benchmarks::vec_from_scalar_64u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint64_t scalar_identity_impl(std::uint64_t x) {
        return x;
    }

    auto scalar_identity = scalar_test_bench<std::uint64_t, scalar_identity_impl>;

    BENCHMARK(vec_from_scalar_64u::scalar_identity);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_shuffle_impl(std::uint64_t x) {
        auto tmp0 = _mm_cvtsi64_si128(x);
        auto tmp1 = _mm_unpacklo_epi64(tmp0, tmp0);
        return vec2x64u{tmp1};
    }

    auto vec2x64u_shuffle = vector_scalar_test_bench<vec2x64u, vec2x64u_shuffle_impl>;

    BENCHMARK(vec_from_scalar_64u::vec2x64u_shuffle);

    #endif



    #if defined(AVEL_AVX2)

    vec2x64u vec2x64u_broadcast_impl(std::uint64_t x) {
        return vec2x64u{_mm_set1_epi64x(x)};
    }

    auto vec2x64u_broadcast = vector_scalar_test_bench<vec2x64u, vec2x64u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_64u::vec2x64u_broadcast);

    #endif




    #if defined(AVEL_AVX2)

    vec2x64u vec2x64u_broadcast_from_vector_impl(std::uint64_t x) {
        return vec2x64u{_mm_broadcastq_epi64(_mm_cvtsi64_si128(x))};
    }

    auto vec2x64u_broadcast_from_vector = vector_scalar_test_bench<vec2x64u, vec2x64u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_64u::vec2x64u_broadcast_from_vector);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_broadcast_impl(std::uint64_t x) {
        return vec4x64u{_mm256_set1_epi64x(x)};
    }

    auto vec4x64u_broadcast = vector_scalar_test_bench<vec4x64u, vec4x64u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_64u::vec4x64u_broadcast);

    #endif



    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_broadcast_from_vector_impl(std::uint64_t x) {
        return vec4x64u{_mm256_broadcastq_epi64(_mm_cvtsi64_si128(x))};
    }

    auto vec4x64u_broadcast_from_vector = vector_scalar_test_bench<vec4x64u, vec4x64u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_64u::vec4x64u_broadcast_from_vector);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_broadcast_impl(std::uint64_t x) {
        return vec8x64u{_mm512_set1_epi64(x)};
    }

    auto vec8x64u_broadcast = vector_scalar_test_bench<vec8x64u, vec8x64u_broadcast_impl>;

    BENCHMARK(vec_from_scalar_64u::vec8x64u_broadcast);

    #endif



    #if defined(AVEL_AVX512BW)

    vec8x64u vec8x64u_broadcast_from_vector_impl(std::uint64_t x) {
        return vec8x64u{_mm512_broadcastq_epi64(_mm_cvtsi64_si128(x))};
    }

    auto vec8x64u_broadcast_from_vector = vector_scalar_test_bench<vec8x64u, vec8x64u_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_64u::vec8x64u_broadcast_from_vector);

    #endif

}

#endif //AVEL_VEC_FROM_SCALAR_64U_HPP
