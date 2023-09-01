#ifndef AVEL_BIT_CEIL_32U_HPP
#define AVEL_BIT_CEIL_32U_HPP

namespace avel::benchmarks::bit_ceil_32u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint32_t scalar_native_impl(std::uint32_t x) {
        return std::bit_ceil(x);
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(bit_ceil_32u::scalar_native);

    #endif



    std::uint32_t scalar_bitwise_impl(std::uint32_t x) {
        if (x == 0) {
            return 1;
        }

        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        ++x;
        return x;
    }

    auto scalar_bitwise = scalar_test_bench<std::uint32_t, scalar_bitwise_impl>;

    BENCHMARK(bit_ceil_32u::scalar_bitwise);



    #if defined(AVEL_x86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint32_t scalar_bsr_and_shift_impl(std::uint32_t x) {
        if (x == 0) {
            return 1;
        }

        auto width = _bit_scan_reverse(x);
        auto tmp = 1 << width;
        return tmp << (tmp != x);
    }

    auto scalar_bsr_and_shift = scalar_test_bench<std::uint32_t, scalar_bsr_and_shift_impl>;

    BENCHMARK(bit_ceil_32u::scalar_bsr_and_shift);

    #endif



    #if defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint32_t scalar_lzcnt_and_shift_impl(std::uint32_t x) {
        auto sh = (32 - _lzcnt_u32(x - 1));
        auto result = 1 << sh;
        return result;
    }

    auto scalar_lzcnt_and_shift = scalar_test_bench<std::uint32_t, scalar_lzcnt_and_shift_impl>;

    BENCHMARK(bit_ceil_32u::scalar_lzcnt_and_shift);

    #endif

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_bitwise_impl(vec4x32u v) {
        auto zero_mask = (v == vec4x32u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        v |= bit_shift_right<8>(v);
        v |= bit_shift_right<16>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec4x32u_bitwise = vector_test_bench<vec4x32u, vec4x32u_bitwise_impl>;

    BENCHMARK(bit_ceil_32u::vec4x32u_bitwise);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec4x32u vec4x32u_lzcnt_impl(vec4x32u v) {
        auto ones = _mm_set1_epi32(1);
        auto sh = _mm_sub_epi32(_mm_set1_epi32(32), _mm_lzcnt_epi32(_mm_sub_epi32(decay(v), ones)));
        auto result = _mm_sllv_epi32(ones, sh);
        return vec4x32u{_mm_sub_epi32(result, _mm_cmpeq_epi32(decay(v), _mm_setzero_si128()))};
    }

    auto vec4x32u_lzcnt = vector_test_bench<vec4x32u, vec4x32u_lzcnt_impl>;

    BENCHMARK(bit_ceil_32u::vec4x32u_lzcnt);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_bitwise_impl(vec8x32u v) {
        auto zero_mask = (v == vec8x32u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        v |= bit_shift_right<8>(v);
        v |= bit_shift_right<16>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec8x32u_bitwise = vector_test_bench<vec8x32u, vec8x32u_bitwise_impl>;

    BENCHMARK(bit_ceil_32u::vec8x32u_bitwise);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec8x32u vec8x32u_lzcnt_impl(vec8x32u v) {
        auto ones = _mm256_set1_epi32(1);
        auto sh = _mm256_sub_epi32(_mm256_set1_epi32(32), _mm256_lzcnt_epi32(_mm256_sub_epi32(decay(v), ones)));
        auto result = _mm256_sllv_epi32(ones, sh);
        return vec8x32u{_mm256_sub_epi32(result, _mm256_cmpeq_epi32(decay(v), _mm256_setzero_si256()))};
    }

    auto vec8x32u_lzcnt = vector_test_bench<vec8x32u, vec8x32u_lzcnt_impl>;

    BENCHMARK(bit_ceil_32u::vec8x32u_lzcnt);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_bitwise_impl(vec16x32u v) {
        auto zero_mask = (v == vec16x32u{0x00});

        --v;
        v |= bit_shift_right<1>(v);
        v |= bit_shift_right<2>(v);
        v |= bit_shift_right<4>(v);
        v |= bit_shift_right<8>(v);
        v |= bit_shift_right<16>(v);
        ++v;

        return v - set_bits(zero_mask);
    }

    auto vec16x32u_bitwise = vector_test_bench<vec16x32u, vec16x32u_bitwise_impl>;

    BENCHMARK(bit_ceil_32u::vec16x32u_bitwise);

    #endif



    #if defined(AVEL_AVX512CD)

    vec16x32u vec16x32u_lzcnt_impl(vec16x32u v) {
        auto is_non_zero = _mm512_test_epi32_mask(decay(v), decay(v));
        auto ones = _mm512_set1_epi32(1);
        auto lzcnt_adjusted = _mm512_lzcnt_epi32(_mm512_sub_epi32(decay(v), ones));
        auto sh = _mm512_maskz_sub_epi32(is_non_zero, _mm512_set1_epi32(32), lzcnt_adjusted);
        auto result = _mm512_sllv_epi32(ones, sh);
        return vec16x32u{result};
    }

    auto vec16x32u_lzcnt = vector_test_bench<vec16x32u, vec16x32u_lzcnt_impl>;

    BENCHMARK(bit_ceil_32u::vec16x32u_lzcnt);

    #endif

}

#endif //AVEL_BIT_CEIL_32U_HPP
