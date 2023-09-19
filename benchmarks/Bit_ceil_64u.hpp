#ifndef AVEL_BIT_CEIL_64U_HPP
#define AVEL_BIT_CEIL_64U_HPP

namespace avel::benchmarks::bit_ceil_64u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint64_t scalar_native_impl(std::uint64_t x) {
        return std::bit_ceil(x);
    }

    auto scalar_native = scalar_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(bit_ceil_64u::scalar_native);

    #endif



    std::uint64_t scalar_bitwise_impl(std::uint64_t x) {
        if (x == 0) {
            return 1;
        }

        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x |= x >> 32;
        ++x;
        return x;
    }

    auto scalar_bitwise = scalar_test_bench<std::uint64_t, scalar_bitwise_impl>;

    BENCHMARK(bit_ceil_64u::scalar_bitwise);



    #if defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint64_t scalar_bsr_and_shift_impl(std::uint64_t x) {
        if (x == 0) {
            return 1;
        }

        auto tmp = std::uint64_t{1} << (63 - __builtin_clzll(x));
        return tmp << (tmp != x);
    }

    auto scalar_bsr_and_shift = scalar_test_bench<std::uint64_t, scalar_bsr_and_shift_impl>;

    BENCHMARK(bit_ceil_64u::scalar_bsr_and_shift);

    #endif



    #if defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint64_t scalar_lzcnt_and_rotate_impl(std::uint64_t x) {
        auto sh = -_lzcnt_u64(x - 1);
        auto result = _lrotl(0x01, sh);
        return result;
    }

    auto scalar_lzcnt_and_rotate = scalar_test_bench<std::uint64_t, scalar_lzcnt_and_rotate_impl>;

    BENCHMARK(bit_ceil_64u::scalar_lzcnt_and_rotate);

    #endif

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_bitwise_impl(vec2x64u v) {
        auto zero_mask = (v == vec2x64u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        v |= bit_shift_right<8>(v);
        v |= bit_shift_right<16>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec2x64u_bitwise = vector_test_bench<vec2x64u, vec2x64u_bitwise_impl>;

    BENCHMARK(bit_ceil_64u::vec2x64u_bitwise);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec2x64u vec2x64u_lzcnt_impl(vec2x64u v) {
        auto sh = (vec2x64u{64} - countl_zero(v - vec2x64u{1}));
        auto result = vec2x64u{1} << sh;
        return result - set_bits(v == vec2x64u{0x00});
    }

    auto vec2x64u_lzcnt = vector_test_bench<vec2x64u, vec2x64u_lzcnt_impl>;

    BENCHMARK(bit_ceil_64u::vec2x64u_lzcnt);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_bitwise_impl(vec4x64u v) {
        auto zero_mask = (v == vec4x64u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        v |= bit_shift_right<8>(v);
        v |= bit_shift_right<16>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec4x64u_bitwise = vector_test_bench<vec4x64u, vec4x64u_bitwise_impl>;

    BENCHMARK(bit_ceil_64u::vec4x64u_bitwise);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec4x64u vec4x64u_lzcnt_impl(vec4x64u v) {
        auto sh = (vec4x64u{64} - countl_zero(v - vec4x64u{1}));
        auto result = vec4x64u{1} << sh;
        return result - set_bits(v == vec4x64u{0x00});
    }

    auto vec4x64u_lzcnt = vector_test_bench<vec4x64u, vec4x64u_lzcnt_impl>;

    BENCHMARK(bit_ceil_64u::vec4x64u_lzcnt);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_bitwise_impl(vec8x64u v) {
        auto zero_mask = (v == vec8x64u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        v |= bit_shift_right<8>(v);
        v |= bit_shift_right<16>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec8x64u_bitwise = vector_test_bench<vec8x64u, vec8x64u_bitwise_impl>;

    BENCHMARK(bit_ceil_64u::vec8x64u_bitwise);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec8x64u vec4x64u_lzcnt_impl(vec8x64u v) {
        auto sh = (vec8x64u{64} - countl_zero(v - vec8x64u{1}));
        auto result = vec8x64u{1} << sh;
        return result - set_bits(v == vec8x64u{0x00});
    }

    auto vec8x64u_lzcnt = vector_test_bench<vec8x64u, vec4x64u_lzcnt_impl>;

    BENCHMARK(bit_ceil_64u::vec8x64u_lzcnt);

    #endif

}

#endif //AVEL_BIT_CEIL_64U_HPP
