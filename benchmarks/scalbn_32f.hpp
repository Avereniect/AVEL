#ifndef AVEL_SCALBN_32F_HPP
#define AVEL_SCALBN_32F_HPP

namespace avel::benchmarks::scalbn_32f {

    float scalar_native_impl(float x, std::int32_t exp) {
        return std::scalbn(x, exp);
    }

    auto scalar_native = scalar_test_bench<float, std::int32_t, scalar_native_impl>;

    BENCHMARK(scalbn_32f::scalar_native);

    #if defined(AVEL_SSE2)

    vec4x32f vec4x32f_avel_impl(vec4x32f x, vec4x32i exp) {
        return avel::scalbn(x, exp);
    }

    auto vec4x32f_avel = vector_test_bench<vec4x32f, vec4x32i, vec4x32f_avel_impl>;

    BENCHMARK(scalbn_32f::vec4x32f_avel);

    #endif


    #if defined(AVEL_AVX2)

    vec8x32f vec8x32f_avel_impl(vec8x32f x, vec8x32i exp) {
        return avel::scalbn(x, exp);
    }

    auto vec8x32f_avel = vector_test_bench<vec8x32f, vec8x32i, vec8x32f_avel_impl>;

    BENCHMARK(scalbn_32f::vec8x32f_avel);

    #endif


    #if defined(AVEL_AVX512F)

    vec16x32f vec16x32f_avel_impl(vec16x32f x, vec16x32i exp) {
        return avel::scalbn(x, exp);
    }

    auto vec16x32f_avel = vector_test_bench<vec16x32f, vec16x32i, vec16x32f_avel_impl>;

    BENCHMARK(scalbn_32f::vec16x32f_avel);

    #endif

}

#endif //AVEL_SCALBN_32F_HPP
