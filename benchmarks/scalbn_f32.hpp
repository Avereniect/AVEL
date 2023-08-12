#ifndef AVEL_SCALBN_F32_HPP
#define AVEL_SCALBN_F32_HPP

namespace avel::benchmarks::scalbn_32f {

    float native_impl(float x, std::int32_t exp) {
        return std::scalbn(x, exp);
    }

    auto native = scalar_test_bench<float, std::int32_t, native_impl>;

    BENCHMARK(scalbn_32f::native);

}

namespace avel::benchmarks::scalbn_vec4x32f {

    #if defined(AVEL_SSE2)

    vec4x32f avel_impl(vec4x32f x, vec4x32i exp) {
        return avel::scalbn(x, exp);
    }

    auto avel_scalbn = vector_test_bench<vec4x32f, vec4x32i, avel_impl>;

    BENCHMARK(scalbn_vec4x32f::avel_scalbn);

    #endif

}

namespace avel::benchmarks::scalbn_vec8x32f {

    #if defined(AVEL_AVX2)

    vec8x32f avel_impl(vec8x32f x, vec8x32i exp) {
        return avel::scalbn(x, exp);
    }

    auto avel_scalbn = vector_test_bench<vec8x32f, vec8x32i, avel_impl>;

    BENCHMARK(scalbn_vec8x32f::avel_scalbn);

    #endif

}

namespace avel::benchmarks::scalbn_vec16x32f {

    #if defined(AVEL_AVX512F)

    vec16x32f avel_impl(vec16x32f x, vec16x32i exp) {
        return avel::scalbn(x, exp);
    }

    auto avel_scalbn = vector_test_bench<vec16x32f, vec16x32i, avel_impl>;

    BENCHMARK(scalbn_vec16x32f::avel_scalbn);

    #endif

}

#endif //AVEL_SCALBN_F32_HPP
