#ifndef AVEL_FPCLASSIFY64F_HPP
#define AVEL_FPCLASSIFY64F_HPP

#include <cmath>

namespace avel::benchmarks::fpclassify_64f {

    std::int64_t scalar_native_impl(double x) {
        return std::fpclassify(x);
    }

    auto scalar_native = scalar_test_bench<double, std::int64_t, scalar_native_impl>;

    BENCHMARK(fpclassify_64f::scalar_native);



    std::int64_t scalar_branching_impl(double x) {
        // Integer comparisons are faster than fp-comparisons
        std::uint64_t x_bits = avel::bit_cast<std::uint64_t>(x);
        x_bits &= 0x7fffffffffffffff;

        if (x_bits == 0x0) {
            return FP_ZERO;
        }

        if (x_bits < 0x10000000000000ull) {
            return FP_SUBNORMAL;
        }

        if (x_bits < 0x7ff0000000000000ull) {
            return FP_NORMAL;
        }

        if (x_bits == 0x7ff0000000000000ull) {
            return FP_INFINITE;
        }

        // Relies on assumption that no implementation-defined categories exist
        return FP_NAN;
    }

    auto scalar_branching = scalar_test_bench<double, std::int64_t, scalar_branching_impl>;

    BENCHMARK(fpclassify_64f::scalar_branching);



    #if defined(AVEL_SSE2)

    vec2x64i vec2x64f_scalarized_impl(vec2x64f v) {
        auto ret0 = fpclassify_64f::scalar_branching_impl(avel::extract<0>(v));
        auto ret1 = fpclassify_64f::scalar_branching_impl(avel::extract<1>(v));
        return vec2x64i{{ret0, ret1}};
    }

    auto vec2x64f_scalarized = vector_test_bench<vec2x64f, vec2x64i, vec2x64f_scalarized_impl>;

    BENCHMARK(fpclassify_64f::vec2x64f_scalarized);

    #endif



    #if defined(AVEL_SSE2)

    vec2x64i vec2x64f_blend_sse2_impl(vec2x64f v) {
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto abs_v = decay(avel::abs(v));

        // Bit pattern for FLT_MIN
        const auto min = _mm_set1_pd(DBL_MIN);

        // Bit pattern for +INFINITY
        const auto inf = _mm_set1_pd(INFINITY);

        // Masks for individual categories
        auto m0 = _mm_castpd_si128(_mm_cmpeq_pd(decay(v), _mm_setzero_pd()));
        auto m1 = _mm_castpd_si128(_mm_andnot_pd(_mm_cmpeq_pd(abs_v, _mm_setzero_pd()), _mm_cmplt_pd(abs_v, min)));
        auto m2 = _mm_castpd_si128(_mm_andnot_pd(_mm_cmplt_pd(abs_v, min), _mm_cmplt_pd(abs_v, inf)));
        auto m3 = _mm_castpd_si128(_mm_cmpeq_pd(abs_v, inf));
        auto m4 = _mm_castpd_si128(_mm_cmpunord_pd(decay(v), decay(v)));

        // Select result via blending
        auto ret = _mm_setzero_si128();
        ret = _mm_and_si128(m0, _mm_set1_epi64x(FP_ZERO));
        ret = _mm_or_si128(ret, _mm_and_si128(m1, _mm_set1_epi64x(FP_SUBNORMAL)));
        ret = _mm_or_si128(ret, _mm_and_si128(m2, _mm_set1_epi64x(FP_NORMAL)));
        ret = _mm_or_si128(ret, _mm_and_si128(m3, _mm_set1_epi64x(FP_INFINITE)));
        ret = _mm_or_si128(ret, _mm_and_si128(m4, _mm_set1_epi64x(FP_NAN)));

        return vec2x64i{ret};
    }

    auto vec2x64f_blend_sse2 = vector_test_bench<vec2x64f, vec2x64i, vec2x64f_blend_sse2_impl>;

    BENCHMARK(fpclassify_64f::vec2x64f_blend_sse2);

    #endif



    #if defined(AVEL_SSE42)

    vec2x64i vec2x64f_blend_sse42_impl(vec2x64f v) {
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm_castpd_si128(decay(v));

        // Take absolute value
        v_bits = _mm_and_si128(v_bits, _mm_set1_epi64x(0x7fffffffffffffffull));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm_set1_epi64x(0x10000000000000ull);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm_set1_epi64x(0x7ff0000000000000ull);

        // Masks for individual categories
        auto m0 = _mm_cmpeq_epi64(v_bits, _mm_setzero_si128());
        auto m1 = _mm_andnot_si128(m0, _mm_cmpgt_epi64(min_bits, v_bits));
        auto m2 = _mm_andnot_si128(_mm_cmpgt_epi64(min_bits, v_bits), _mm_cmpgt_epi64(inf_bits, v_bits));
        auto m3 = _mm_cmpeq_epi64(v_bits, inf_bits);
        auto m4 = _mm_cmpgt_epi64(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm_setzero_si128();
        ret = _mm_and_si128(m0, _mm_set1_epi64x(FP_ZERO));
        ret = _mm_or_si128(ret, _mm_and_si128(m1, _mm_set1_epi64x(FP_SUBNORMAL)));
        ret = _mm_or_si128(ret, _mm_and_si128(m2, _mm_set1_epi64x(FP_NORMAL)));
        ret = _mm_or_si128(ret, _mm_and_si128(m3, _mm_set1_epi64x(FP_INFINITE)));
        ret = _mm_or_si128(ret, _mm_and_si128(m4, _mm_set1_epi64x(FP_NAN)));

        return vec2x64i{ret};
    }

    auto vec2x64f_blend_sse42 = vector_test_bench<vec2x64f, vec2x64i, vec2x64f_blend_sse42_impl>;

    BENCHMARK(fpclassify_64f::vec2x64f_blend_sse42);

    #endif

    #if defined(AVEL_AVX2)

    vec4x64i vec4x64f_lend_avx2_impl(vec4x64f v) {
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm256_castpd_si256(decay(v));

        // Take absolute value
        v_bits = _mm256_and_si256(v_bits, _mm256_set1_epi64x(0x7fffffffffffffffull));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm256_set1_epi64x(0x10000000000000ull);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm256_set1_epi64x(0x7ff0000000000000ull);

        // Masks for individual categories
        auto m0 = _mm256_cmpeq_epi64(v_bits, _mm256_setzero_si256());
        auto m1 = _mm256_andnot_si256(m0, _mm256_cmpgt_epi64(min_bits, v_bits));
        auto m2 = _mm256_andnot_si256(_mm256_cmpgt_epi64(min_bits, v_bits), _mm256_cmpgt_epi64(inf_bits, v_bits));
        auto m3 = _mm256_cmpeq_epi64(v_bits, inf_bits);
        auto m4 = _mm256_cmpgt_epi64(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm256_setzero_si256();
        ret = _mm256_and_si256(m0, _mm256_set1_epi64x(FP_ZERO));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m1, _mm256_set1_epi64x(FP_SUBNORMAL)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m2, _mm256_set1_epi64x(FP_NORMAL)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m3, _mm256_set1_epi64x(FP_INFINITE)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m4, _mm256_set1_epi64x(FP_NAN)));

        return vec4x64i{ret};
    }

    auto vec4x64f_lend_avx2 = vector_test_bench<vec4x64f, vec4x64i, vec4x64f_lend_avx2_impl>;

    BENCHMARK(fpclassify_64f::vec4x64f_lend_avx2);

    #endif



    #if defined(AVEL_AVX512F)

    vec8x64i vec8x64f_blend_avx512f_impl(vec8x64f v) {
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm512_castpd_si512(decay(v));

        // Take absolute value
        v_bits = _mm512_and_si512(v_bits, _mm512_set1_epi64(0x7fffffffffffffffull));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm512_set1_epi64(0x10000000000000ull);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm512_set1_epi64(0x7ff0000000000000ull);

        // Masks for individual categories
        auto m0 = _mm512_testn_epi64_mask(v_bits, v_bits);
        auto m1 = ~m0 & _mm512_cmpgt_epi64_mask(min_bits, v_bits);
        auto m2 = ~_mm512_cmpgt_epi64_mask(min_bits, v_bits) & _mm512_cmpgt_epi64_mask(inf_bits, v_bits);
        auto m3 = _mm512_cmpeq_epi64_mask(v_bits, inf_bits);
        auto m4 = _mm512_cmpgt_epi64_mask(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm512_setzero_si512();
        ret = _mm512_maskz_mov_epi64(m0, _mm512_set1_epi64(FP_ZERO));
        ret = _mm512_mask_blend_epi64(m1, ret, _mm512_set1_epi64(FP_SUBNORMAL));
        ret = _mm512_mask_blend_epi64(m2, ret, _mm512_set1_epi64(FP_NORMAL)   );
        ret = _mm512_mask_blend_epi64(m3, ret, _mm512_set1_epi64(FP_INFINITE) );
        ret = _mm512_mask_blend_epi64(m4, ret, _mm512_set1_epi64(FP_NAN)      );

        return vec8x64i{ret};
    }

    auto vec8x64f_blend_avx512f = vector_test_bench<vec8x64f, vec8x64i, vec8x64f_blend_avx512f_impl>;

    BENCHMARK(fpclassify_64f::vec8x64f_blend_avx512f);

    #endif



    #if defined(AVEL_AVX512DQ)

    ///
    /// Generic implementation using vfpclass
    /// Portable across compilers but not optimal.
    ///
    vec8x64i vec8x64f_blending_vfpclass_impl(vec8x64f v) {
        const vec8x64i fp_infinite{int(FP_INFINITE)};
        const vec8x64i fp_nan{int(FP_NAN)};
        const vec8x64i fp_normal{int(FP_NORMAL)};
        const vec8x64i fp_subnormal{int(FP_SUBNORMAL)};
        const vec8x64i fp_zero{int(FP_ZERO)};

        mask8x64i infinite_mask {_mm512_fpclass_pd_mask(decay(v), 0x08 | 0x10)};
        mask8x64i nan_mask      {_mm512_fpclass_pd_mask(decay(v), 0x01 | 0x80)};
        mask8x64i subnormal_mask{_mm512_fpclass_pd_mask(decay(v), 0x20)};
        mask8x64i zero_mask     {_mm512_fpclass_pd_mask(decay(v), 0x02 | 0x04)};
        mask8x64i normal_mask   {!(infinite_mask | nan_mask | subnormal_mask | zero_mask)};

        return
            keep(infinite_mask, fp_infinite) |
            keep(nan_mask, fp_nan) |
            keep(normal_mask, fp_normal) |
            keep(subnormal_mask, fp_subnormal) |
            keep(zero_mask, fp_zero);
    }

    auto vec8x64f_blending_vfpclass = vector_test_bench<vec8x64f, vec8x64i, vec8x64f_blending_vfpclass_impl>;

    BENCHMARK(fpclassify_64f::vec8x64f_blending_vfpclass);

    #endif

}

#endif //AVEL_FPCLASSIFY64F_HPP
