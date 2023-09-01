#ifndef AVEL_BIT_CEIL_16U_HPP
#define AVEL_BIT_CEIL_16U_HPP

namespace avel::benchmarks::bit_ceil_16u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint16_t scalar_native_impl(std::uint16_t x) {
        return std::bit_ceil(x);
    }

    auto scalar_native = scalar_test_bench<std::uint16_t, scalar_native_impl>;

    BENCHMARK(bit_ceil_16u::scalar_native);

    #endif



    std::uint16_t scalar_bitwise_impl(std::uint16_t x) {
        if (x == 0) {
            return 1;
        }

        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        ++x;
        return x;
    }

    auto scalar_bitwise = scalar_test_bench<std::uint16_t, scalar_bitwise_impl>;

    BENCHMARK(bit_ceil_16u::scalar_bitwise);



    #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint16_t scalar_bsr_and_shift_impl(std::uint16_t x) {
        if (x == 0) {
            return 1;
        }

        auto width = _bit_scan_reverse(x);
        auto tmp = 1 << width;
        return tmp << (tmp != x);
    }

    auto scalar_bsr_and_shift = scalar_test_bench<std::uint16_t, scalar_bsr_and_shift_impl>;

    BENCHMARK(bit_ceil_16u::scalar_bsr_and_shift);

    #endif



    #if defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint16_t scalar_lzcnt_and_shift_impl(std::uint16_t x) {
        auto sh = (32 - _lzcnt_u32(x - 1));
        auto result = 1 << sh;
        return result;
    }

    auto scalar_lzcnt_and_shift = scalar_test_bench<std::uint16_t, scalar_lzcnt_and_shift_impl>;

    BENCHMARK(bit_ceil_16u::scalar_lzcnt_and_shift);

    #endif

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec8x16u vec8x16u_bitwise_impl(vec8x16u v) {
        auto zero_mask = (v == vec8x16u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        v |= bit_shift_right<8>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec8x16u_bitwise = vector_test_bench<vec8x16u, vec8x16u_bitwise_impl>;

    BENCHMARK(bit_ceil_16u::vec8x16u_bitwise);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512CD)

    vec8x16u vec8x16u_lzcnt32_impl(vec8x16u v) {
        auto zero_mask = (v == vec8x16u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        v |= bit_shift_right<8>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec8x16u_lzcnt32 = vector_test_bench<vec8x16u, vec8x16u_lzcnt32_impl>;

    BENCHMARK(bit_ceil_16u::vec8x16u_lzcnt32);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_bitwise_impl(vec16x16u v) {
        auto zero_mask = (v == vec16x16u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        v |= bit_shift_right<8>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec16x16u_bitwise = vector_test_bench<vec16x16u, vec16x16u_bitwise_impl>;

    BENCHMARK(bit_ceil_16u::vec16x16u_bitwise);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_bitwise_impl(vec32x16u v) {
        auto zero_mask = (v == vec32x16u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        v |= bit_shift_right<8>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec32x16u_bitwise = vector_test_bench<vec32x16u, vec32x16u_bitwise_impl>;

    BENCHMARK(bit_ceil_16u::vec32x16u_bitwise);

    #endif

}

#endif //AVEL_BIT_CEIL_16U_HPP
