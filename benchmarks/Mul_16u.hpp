#ifndef AVEL_MUL_16U_HPP
#define AVEL_MUL_16U_HPP

namespace avel::benchmarks::mul_16u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint16_t scalar_native_impl(std::uint16_t x, std::uint16_t y) {
        return x * y;
    }

    auto scalar_native = scalar_test_bench<std::uint16_t, scalar_native_impl>;

    BENCHMARK(mul_16u::scalar_native);

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec8x16u vec8x16u_mullw_impl(vec8x16u v, vec8x16u w) {
        return vec8x16u{_mm_mullo_epi16(decay(v), decay(w))};
    }

    auto vec8x16u_mullw = vector_test_bench<vec8x16u, vec8x16u_mullw_impl>;

    BENCHMARK(mul_16u::vec8x16u_mullw);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_mullw_impl(vec16x16u v, vec16x16u w) {
        return vec16x16u{_mm256_mullo_epi16(decay(v), decay(w))};
    }

    auto vec16x16u_mullw = vector_test_bench<vec16x16u, vec16x16u_mullw_impl>;

    BENCHMARK(mul_16u::vec16x16u_mullw);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_mullw_impl(vec32x16u v, vec32x16u w) {
        return vec32x16u{_mm512_mullo_epi16(decay(v), decay(w))};
    }

    auto vec32x16u_mullw = vector_test_bench<vec32x16u, vec32x16u_mullw_impl>;

    BENCHMARK(mul_16u::vec32x16u_mullw);

    #endif

}

#endif //AVEL_MUL_16U_HPP
