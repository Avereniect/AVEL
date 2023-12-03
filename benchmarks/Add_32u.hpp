#ifndef AVEL_ADD_32U_HPP
#define AVEL_ADD_32U_HPP

namespace avel::benchmarks::add_32u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint32_t scalar_native_impl(std::uint32_t x, std::uint32_t y) {
        return x + y;
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(add_32u::scalar_native);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_addd_impl(vec4x32u v, vec4x32u w) {
        return vec4x32u{_mm_add_epi32(decay(v), decay(w))};
    }

    auto vec4x32u_addd = vector_test_bench<vec4x32u, vec4x32u_addd_impl>;

    BENCHMARK(add_32u::vec4x32u_addd);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_addd_impl(vec8x32u v, vec8x32u w) {
        return vec8x32u{_mm256_add_epi32(decay(v), decay(w))};
    }

    auto vec8x32u_addd = vector_test_bench<vec8x32u, vec8x32u_addd_impl>;

    BENCHMARK(add_32u::vec8x32u_addd);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_addd_impl(vec16x32u v, vec16x32u w) {
        return vec16x32u{_mm512_add_epi32(decay(v), decay(w))};
    }

    auto vec16x32u_addd = vector_test_bench<vec16x32u, vec16x32u_addd_impl>;

    BENCHMARK(add_32u::vec16x32u_addd);

    #endif

}

#endif //AVEL_ADD_32U_HPP
