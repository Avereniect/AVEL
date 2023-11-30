#ifndef AVEL_VEC_FROM_SCALAR_64F_HPP
#define AVEL_VEC_FROM_SCALAR_64F_HPP

namespace avel::benchmarks::vec_from_scalar_64f {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    double scalar_identity_impl(double x) {
        return x;
    }

    auto scalar_identity = scalar_test_bench<double, scalar_identity_impl>;

    BENCHMARK(vec_from_scalar_64f::scalar_identity);

    //=====================================================
    // vec2x64f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64f vec2x64f_shuffle_impl(double x) {
        auto tmp0 = _mm_set_sd(x);
        auto tmp1 = _mm_unpacklo_pd(tmp0, tmp0);
        return vec2x64f{tmp1};
    }

    auto vec2x64f_shuffle = vector_scalar_test_bench<vec2x64f, vec2x64f_shuffle_impl>;

    BENCHMARK(vec_from_scalar_64f::vec2x64f_shuffle);

    #endif



    #if defined(AVEL_AVX2)

    vec2x64f vec2x64f_broadcast_impl(double x) {
        return vec2x64f{_mm_set1_pd(x)};
    }

    auto vec2x64f_broadcast = vector_scalar_test_bench<vec2x64f, vec2x64f_broadcast_impl>;

    BENCHMARK(vec_from_scalar_64f::vec2x64f_broadcast);

    #endif




    #if defined(AVEL_AVX2)

    vec2x64f vec2x64f_movedup_impl(double x) {
        return vec2x64f{_mm_movedup_pd(_mm_set_sd(x))};
    }

    auto vec2x64f_movedup = vector_scalar_test_bench<vec2x64f, vec2x64f_movedup_impl>;

    BENCHMARK(vec_from_scalar_64f::vec2x64f_movedup);

    #endif

    //=====================================================
    // vec4x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64f vec4x64f_broadcast_impl(double x) {
        return vec4x64f{_mm256_set1_pd(x)};
    }

    auto vec4x64f_broadcast = vector_scalar_test_bench<vec4x64f, vec4x64f_broadcast_impl>;

    BENCHMARK(vec_from_scalar_64f::vec4x64f_broadcast);

    #endif



    #if defined(AVEL_AVX2)

    vec4x64f vec4x64f_broadcast_from_vector_impl(double x) {
        return vec4x64f{_mm256_broadcastsd_pd(_mm_set_sd(x))};
    }

    auto vec4x64f_broadcast_from_vector = vector_scalar_test_bench<vec4x64f, vec4x64f_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_64f::vec4x64f_broadcast_from_vector);

    #endif

    //=====================================================
    // vec8x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64f vec8x64f_broadcast_impl(double x) {
        return vec8x64f{_mm512_set1_pd(x)};
    }

    auto vec8x64f_broadcast = vector_scalar_test_bench<vec8x64f, vec8x64f_broadcast_impl>;

    BENCHMARK(vec_from_scalar_64f::vec8x64f_broadcast);

    #endif



    #if defined(AVEL_AVX512BW)

    vec8x64f vec8x64f_broadcast_from_vector_impl(double x) {
        return vec8x64f{_mm512_broadcastsd_pd(_mm_set_sd(x))};
    }

    auto vec8x64f_broadcast_from_vector = vector_scalar_test_bench<vec8x64f, vec8x64f_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_64f::vec8x64f_broadcast_from_vector);

    #endif

}

#endif //AVEL_VEC_FROM_SCALAR_64F_HPP
