#ifndef AVEL_VEC_FROM_SCALAR_32F_HPP
#define AVEL_VEC_FROM_SCALAR_32F_HPP

namespace avel::benchmarks::vec_from_scalar_32f {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    float scalar_identity_impl(float x) {
        return x;
    }

    auto scalar_identity = scalar_test_bench<float, scalar_identity_impl>;

    BENCHMARK(vec_from_scalar_32f::scalar_identity);

    //=====================================================
    // vec4x32f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32f vec4x32f_shuffle_impl(float x) {
        auto tmp0 = _mm_set_ss(x);
        auto tmp1 = _mm_shuffle_ps(tmp0, tmp0, 0x00);
        return vec4x32f{tmp1};
    }

    auto vec4x32f_shuffle = vector_scalar_test_bench<vec4x32f, vec4x32f_shuffle_impl>;

    BENCHMARK(vec_from_scalar_32f::vec4x32f_shuffle);

    #endif



    #if defined(AVEL_AVX2)

    vec4x32f vec4x32f_broadcast_impl(float x) {
        return vec4x32f{_mm_set1_ps(x)};
    }

    auto vec4x32f_broadcast = vector_scalar_test_bench<vec4x32f, vec4x32f_broadcast_impl>;

    BENCHMARK(vec_from_scalar_32f::vec4x32f_broadcast);

    #endif




    #if defined(AVEL_AVX2)

    vec4x32f vec4x32f_broadcast_from_vector_impl(float x) {
        return vec4x32f{_mm_broadcastss_ps(_mm_set_ss(x))};
    }

    auto vec4x32f_broadcast_from_vector = vector_scalar_test_bench<vec4x32f, vec4x32f_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_32f::vec4x32f_broadcast_from_vector);

    #endif

    //=====================================================
    // vec8x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32f vec8x32f_broadcast_impl(float x) {
        return vec8x32f{_mm256_set1_ps(x)};
    }

    auto vec8x32f_broadcast = vector_scalar_test_bench<vec8x32f, vec8x32f_broadcast_impl>;

    BENCHMARK(vec_from_scalar_32f::vec8x32f_broadcast);

    #endif



    #if defined(AVEL_AVX2)

    vec8x32f vec8x32f_broadcast_from_vector_impl(float x) {
        return vec8x32f{_mm256_broadcastss_ps(_mm_set_ss(x))};
    }

    auto vec8x32f_broadcast_from_vector = vector_scalar_test_bench<vec8x32f, vec8x32f_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_32f::vec8x32f_broadcast_from_vector);

    #endif

    //=====================================================
    // vec16x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32f vec16x32f_broadcast_impl(float x) {
        return vec16x32f{_mm512_set1_ps(x)};
    }

    auto vec16x32f_broadcast = vector_scalar_test_bench<vec16x32f, vec16x32f_broadcast_impl>;

    BENCHMARK(vec_from_scalar_32f::vec16x32f_broadcast);

    #endif



    #if defined(AVEL_AVX512BW)

    vec16x32f vec16x32f_broadcast_from_vector_impl(float x) {
        return vec16x32f{_mm512_broadcastss_ps(_mm_set_ss(x))};
    }

    auto vec16x32f_broadcast_from_vector = vector_scalar_test_bench<vec16x32f, vec16x32f_broadcast_from_vector_impl>;

    BENCHMARK(vec_from_scalar_32f::vec16x32f_broadcast_from_vector);

    #endif

}

#endif //AVEL_VEC_FROM_SCALAR_32F_HPP
