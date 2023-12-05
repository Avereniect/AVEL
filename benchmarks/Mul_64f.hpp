#ifndef AVEL_MUL_64F_HPP
#define AVEL_MUL_64F_HPP

namespace avel::benchmarks::mul_64f {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    double scalar_native_impl(double x, double y) {
        return x * y;
    }

    auto scalar_native = scalar_test_bench<double, scalar_native_impl>;

    BENCHMARK(mul_64f::scalar_native);

    //=====================================================
    // vec2x64f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64f vec2x64f_mulpd_impl(vec2x64f v, vec2x64f w) {
        return vec2x64f{_mm_mul_pd(decay(v), decay(w))};
    }

    auto vec2x64f_mulpd = vector_test_bench<vec2x64f, vec2x64f_mulpd_impl>;

    BENCHMARK(mul_64f::vec2x64f_mulpd);

    #endif

    //=====================================================
    // vec4x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64f vec4x64f_mulpd_impl(vec4x64f v, vec4x64f w) {
        return vec4x64f{_mm256_mul_pd(decay(v), decay(w))};
    }

    auto vec4x64f_mulpd = vector_test_bench<vec4x64f, vec4x64f_mulpd_impl>;

    BENCHMARK(mul_64f::vec4x64f_mulpd);

    #endif

    //=====================================================
    // vec8x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64f vec8x64f_mulpd_impl(vec8x64f v, vec8x64f w) {
        return vec8x64f{_mm512_mul_pd(decay(v), decay(w))};
    }

    auto vec8x64f_mulpd = vector_test_bench<vec8x64f, vec8x64f_mulpd_impl>;

    BENCHMARK(mul_64f::vec8x64f_mulpd);

    #endif

}

#endif //AVEL_MUL_64F_HPP
