#ifndef AVEL_FPCLASSIFY32F_HPP
#define AVEL_FPCLASSIFY32F_HPP

#include <cmath>

namespace avel::benchmarks::fpclassify_32f {

    //=====================================================
    // scalar f32 benchmarks
    //=====================================================

    std::int32_t scalar_native_impl(float x) {
        return std::fpclassify(x);
    }

    auto scalar_native = scalar_test_bench<float, std::int32_t, scalar_native_impl>;

    BENCHMARK(fpclassify_32f::scalar_native);



    std::int32_t scalar_branching_impl(float x) {
        // Integer comparisons are faster than fp-comparisons
        std::uint32_t x_bits = avel::bit_cast<std::uint32_t>(x);
        x_bits &= 0x7fffffff;

        if (x_bits == 0x0) {
            return FP_ZERO;
        }

        if (x_bits < 0x00800000) {
            return FP_SUBNORMAL;
        }

        if (x_bits < 0x7f800000) {
            return FP_NORMAL;
        }

        if (x_bits == 0x7f800000) {
            return FP_INFINITE;
        }

        // Relies on assumption that no implementation-defined categories exist
        return FP_NAN;
    }

    auto scalar_branching = scalar_test_bench<float, std::int32_t, scalar_branching_impl>;

    BENCHMARK(fpclassify_32f::scalar_branching);



    std::int32_t scalar_blending0_impl(float x) {
        auto abs_x = avel::abs(x);

        int ret = 0;
        int is_uncategorized = true;

        ret |= FP_ZERO & -int(abs_x == 0.0f);
        is_uncategorized &= !(abs_x == 0.0f);

        ret |= FP_SUBNORMAL & -int(is_uncategorized && (abs_x < FLT_MIN));
        is_uncategorized &= !(abs_x < FLT_MIN);

        ret |= FP_NORMAL & -int(is_uncategorized && (abs_x < INFINITY));
        is_uncategorized &= !(abs_x < INFINITY);

        ret |= FP_INFINITE & -int(is_uncategorized && (abs_x == INFINITY));
        is_uncategorized &= !(abs_x == INFINITY);

        ret |= FP_NAN & -int(is_uncategorized);

        return ret;
    }

    auto scalar_blending0 = scalar_test_bench<float, std::int32_t, scalar_blending0_impl>;

    BENCHMARK(fpclassify_32f::scalar_blending0);



    std::int32_t scalar_blending1_impl(float x) {
        auto abs_x = avel::abs(x);

        int ret = 0;

        ret |= FP_ZERO & -(abs_x == 0.0f);
        ret |= FP_SUBNORMAL & -((0.0f < abs_x) && (abs_x < FLT_MIN));
        ret |= FP_NORMAL & -((FLT_MIN <= abs_x) && (abs_x < INFINITY));
        ret |= FP_INFINITE & -(abs_x == INFINITY);
        ret |= FP_NAN & -(x != x);
 
        return ret;
    }

    auto scalar_blending1 = scalar_test_bench<float, std::int32_t, scalar_blending1_impl>;

    BENCHMARK(fpclassify_32f::scalar_blending1);

    //=====================================================
    // vec4x32f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) || defined(AVEL_NEON)

    vec4x32i vec4x32f_avel_impl(vec4x32f v) {
        return avel::fpclassify(v);
    }

    auto vec4x32f_avel = vector_test_bench<vec4x32f, vec4x32i, vec4x32f_avel_impl>;

    BENCHMARK(fpclassify_32f::vec4x32f_avel);

    #endif



    #if defined(AVEL_SSE2)

    ///
    /// Generic implementation based on blending of results.
    /// Portable across compilers but not optimal
    ///
    vec4x32i vec4x32f_blending_impl(vec4x32f v) {
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm_castps_si128(decay(v));

        // Take absolute value
        v_bits = _mm_and_si128(v_bits, _mm_set1_epi32(0x7fffffff));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm_set1_epi32(0x00800000);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm_set1_epi32(0x7f800000);

        // Masks for individual categories
        auto m0 = _mm_cmpeq_epi32(v_bits, _mm_setzero_si128());
        auto m1 = _mm_andnot_si128(m0, _mm_cmplt_epi32(v_bits, min_bits));
        auto m2 = _mm_andnot_si128(_mm_cmplt_epi32(v_bits, min_bits), _mm_cmplt_epi32(v_bits, inf_bits));
        auto m3 = _mm_cmpeq_epi32(v_bits, inf_bits);
        auto m4 = _mm_cmpgt_epi32(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm_setzero_si128();
        ret = _mm_and_si128(m0, _mm_set1_epi32(FP_ZERO));
        ret = _mm_or_si128(ret, _mm_and_si128(m1, _mm_set1_epi32(FP_SUBNORMAL)));
        ret = _mm_or_si128(ret, _mm_and_si128(m2, _mm_set1_epi32(FP_NORMAL)));
        ret = _mm_or_si128(ret, _mm_and_si128(m3, _mm_set1_epi32(FP_INFINITE)));
        ret = _mm_or_si128(ret, _mm_and_si128(m4, _mm_set1_epi32(FP_NAN)));

        return vec4x32i{ret};
    }

    auto vec4x32f_blending = vector_test_bench<vec4x32f, vec4x32i, vec4x32f_blending_impl>;

    BENCHMARK(fpclassify_32f::vec4x32f_blending);

    #endif



    #if defined(AVEL_AVX512VL)

    ///
    /// Generic implementation based on blending of results.
    /// Portable across compilers but not optimal.e
    /// Explicitly uses vternlog
    ///
    vec4x32i vec4x32f_blending_ternlog_impl(vec4x32f v) {
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm_castps_si128(decay(v));

        // Take absolute value
        v_bits = _mm_and_si128(v_bits, _mm_set1_epi32(0x7fffffff));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm_set1_epi32(0x00800000);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm_set1_epi32(0x7f800000);

        // Masks for individual categories
        auto m0 = _mm_cmpeq_epi32(v_bits, _mm_setzero_si128());
        auto m1 = _mm_andnot_si128(m0, _mm_cmplt_epi32(v_bits, min_bits));
        auto m2 = _mm_andnot_si128(_mm_cmplt_epi32(v_bits, min_bits), _mm_cmplt_epi32(v_bits, inf_bits));
        auto m3 = _mm_cmpeq_epi32(v_bits, inf_bits);
        auto m4 = _mm_cmpgt_epi32(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm_setzero_si128();
        ret = _mm_and_si128(m0, _mm_set1_epi32(FP_ZERO));
        ret = _mm_ternarylogic_epi32(ret, m1, _mm_set1_epi32(FP_SUBNORMAL), 0xf8);
        ret = _mm_ternarylogic_epi32(ret, m2, _mm_set1_epi32(FP_NORMAL),    0xf8);
        ret = _mm_ternarylogic_epi32(ret, m3, _mm_set1_epi32(FP_INFINITE),  0xf8);
        ret = _mm_ternarylogic_epi32(ret, m4, _mm_set1_epi32(FP_NAN),       0xf8);

        return vec4x32i{ret};
    }

    auto vec4x32f_blending_ternlog = vector_test_bench<vec4x32f, vec4x32i, vec4x32f_blending_ternlog_impl>;

    BENCHMARK(fpclassify_32f::vec4x32f_blending_ternlog);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    ///
    /// Generic implementation using vfpclass
    /// Portable across compilers but not optimal.
    ///
    vec4x32i vec4x32f_blending_vfpclass_impl(vec4x32f v) {
        const vec4x32i fp_infinite{int(FP_INFINITE)};
        const vec4x32i fp_nan{int(FP_NAN)};
        const vec4x32i fp_normal{int(FP_NORMAL)};
        const vec4x32i fp_subnormal{int(FP_SUBNORMAL)};
        const vec4x32i fp_zero{int(FP_ZERO)};

        mask4x32i infinite_mask {_mm_fpclass_ps_mask(decay(v), 0x08 | 0x10)};
        mask4x32i nan_mask      {_mm_fpclass_ps_mask(decay(v), 0x01 | 0x80)};
        mask4x32i subnormal_mask{_mm_fpclass_ps_mask(decay(v), 0x20)};
        mask4x32i zero_mask     {_mm_fpclass_ps_mask(decay(v), 0x02 | 0x04)};
        mask4x32i normal_mask   {!(infinite_mask | nan_mask | subnormal_mask | zero_mask)};

        return
            keep(infinite_mask, fp_infinite) |
            keep(nan_mask, fp_nan) |
            keep(normal_mask, fp_normal) |
            keep(subnormal_mask, fp_subnormal) |
            keep(zero_mask, fp_zero);
    }

    auto vec4x32f_blending_vfpclass = vector_test_bench<vec4x32f, vec4x32i, vec4x32f_blending_vfpclass_impl>;

    BENCHMARK(fpclassify_32f::vec4x32f_blending_vfpclass);

    #endif

    //=====================================================
    // vec8x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    ///
    /// Generic implementation based on blending of results.
    /// Portable across compilers but not optimal
    ///
    vec8x32i vec8x32f_blending_impl(vec8x32f v) {
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm256_castps_si256(decay(v));

        // Take absolute value
        v_bits = _mm256_and_si256(v_bits, _mm256_set1_epi32(0x7fffffff));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm256_set1_epi32(0x00800000);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm256_set1_epi32(0x7f800000);

        // Masks for individual categories
        auto m0 = _mm256_cmpeq_epi32(v_bits, _mm256_setzero_si256());
        auto m1 = _mm256_andnot_si256(m0, _mm256_cmpgt_epi32(min_bits, v_bits));
        auto m2 = _mm256_andnot_si256(_mm256_cmpgt_epi32(min_bits, v_bits), _mm256_cmpgt_epi32(inf_bits, v_bits));
        auto m3 = _mm256_cmpeq_epi32(v_bits, inf_bits);
        auto m4 = _mm256_cmpgt_epi32(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm256_setzero_si256();
        ret = _mm256_and_si256(m0, _mm256_set1_epi32(FP_ZERO));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m1, _mm256_set1_epi32(FP_SUBNORMAL)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m2, _mm256_set1_epi32(FP_NORMAL)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m3, _mm256_set1_epi32(FP_INFINITE)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m4, _mm256_set1_epi32(FP_NAN)));

        return vec8x32i{ret};
    }

    auto vec8x32f_blending = vector_test_bench<vec8x32f, vec8x32i, vec8x32f_blending_impl>;

    BENCHMARK(fpclassify_32f::vec8x32f_blending);

    #endif



    #if defined(AVEL_AVX512VL)

    ///
    /// Generic implementation based on blending of results.
    /// Portable across compilers but not optimal.e
    /// Explicitly uses vternlog
    ///
    vec8x32i vec8x32f_blending_ternlog_impl(vec8x32f v) {
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm256_castps_si256(decay(v));

        // Take absolute value
        v_bits = _mm256_and_si256(v_bits, _mm256_set1_epi32(0x7fffffff));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm256_set1_epi32(0x00800000);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm256_set1_epi32(0x7f800000);

        // Masks for individual categories
        auto m0 = _mm256_cmpeq_epi32(v_bits, _mm256_setzero_si256());
        auto m1 = _mm256_andnot_si256(m0, _mm256_cmpgt_epi32(min_bits, v_bits));
        auto m2 = _mm256_andnot_si256(_mm256_cmpgt_epi32(min_bits, v_bits), _mm256_cmpgt_epi32(inf_bits, v_bits));
        auto m3 = _mm256_cmpeq_epi32(v_bits, inf_bits);
        auto m4 = _mm256_cmpgt_epi32(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm256_setzero_si256();
        ret = _mm256_and_si256(m0, _mm256_set1_epi32(FP_ZERO));
        ret = _mm256_ternarylogic_epi32(ret, m1, _mm256_set1_epi32(FP_SUBNORMAL), 0xf8);
        ret = _mm256_ternarylogic_epi32(ret, m2, _mm256_set1_epi32(FP_NORMAL),    0xf8);
        ret = _mm256_ternarylogic_epi32(ret, m3, _mm256_set1_epi32(FP_INFINITE),  0xf8);
        ret = _mm256_ternarylogic_epi32(ret, m4, _mm256_set1_epi32(FP_NAN),       0xf8);

        return vec8x32i{ret};
    }

    auto vec8x32f_blending_ternlog = vector_test_bench<vec8x32f, vec8x32i, vec8x32f_blending_ternlog_impl>;

    BENCHMARK(fpclassify_32f::vec8x32f_blending_ternlog);

    #endif


    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    ///
    /// Generic implementation using vfpclass
    /// Portable across compilers but not optimal.
    ///
    vec8x32i vec8x32f_blending_vfpclass_impl(vec8x32f v) {
        const vec8x32i fp_infinite{int(FP_INFINITE)};
        const vec8x32i fp_nan{int(FP_NAN)};
        const vec8x32i fp_normal{int(FP_NORMAL)};
        const vec8x32i fp_subnormal{int(FP_SUBNORMAL)};
        const vec8x32i fp_zero{int(FP_ZERO)};

        mask8x32i infinite_mask {_mm256_fpclass_ps_mask(decay(v), 0x08 | 0x10)};
        mask8x32i nan_mask      {_mm256_fpclass_ps_mask(decay(v), 0x01 | 0x80)};
        mask8x32i subnormal_mask{_mm256_fpclass_ps_mask(decay(v), 0x20)};
        mask8x32i zero_mask     {_mm256_fpclass_ps_mask(decay(v), 0x02 | 0x04)};
        mask8x32i normal_mask   {!(infinite_mask | nan_mask | subnormal_mask | zero_mask)};

        return
            keep(infinite_mask, fp_infinite) |
            keep(nan_mask, fp_nan) |
            keep(normal_mask, fp_normal) |
            keep(subnormal_mask, fp_subnormal) |
            keep(zero_mask, fp_zero);
    }

    auto vec8x32f_blending_vfpclass = vector_test_bench<vec8x32f, vec8x32i, vec8x32f_blending_vfpclass_impl>;

    BENCHMARK(fpclassify_32f::vec8x32f_blending_vfpclass);

    #endif

    //=====================================================
    // vec16x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    ///
    /// Generic implementation using vfpclass
    /// Portable across compilers but not optimal.
    ///
    vec16x32i vec16x32f_blending_ternlog_impl(vec16x32f v) {
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm512_castps_si512(decay(v));

        // Take absolute value
        v_bits = _mm512_and_si512(v_bits, _mm512_set1_epi32(0x7fffffff));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm512_set1_epi32(0x00800000);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm512_set1_epi32(0x7f800000);

        // Masks for individual categories
        auto m0 = _mm512_testn_epi32_mask(v_bits, v_bits);
        auto m1 = ~m0 & _mm512_cmplt_epi32_mask(v_bits, min_bits);
        auto m2 = ~_mm512_cmplt_epi32_mask(v_bits, min_bits) & _mm512_cmplt_epi32_mask(v_bits, inf_bits);
        auto m3 = _mm512_cmpeq_epi32_mask(v_bits, inf_bits);
        auto m4 = _mm512_cmpgt_epi32_mask(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm512_setzero_si512();
        ret = _mm512_maskz_mov_epi32(m0, _mm512_set1_epi32(FP_ZERO));
        ret = _mm512_mask_blend_epi32(m1, ret, _mm512_set1_epi32(FP_SUBNORMAL));
        ret = _mm512_mask_blend_epi32(m2, ret, _mm512_set1_epi32(FP_NORMAL)   );
        ret = _mm512_mask_blend_epi32(m3, ret, _mm512_set1_epi32(FP_INFINITE) );
        ret = _mm512_mask_blend_epi32(m4, ret, _mm512_set1_epi32(FP_NAN)      );

        return vec16x32i{ret};
    }

    auto vec16x32f_blending_ternlog = vector_test_bench<vec16x32f, vec16x32i, vec16x32f_blending_ternlog_impl>;

    BENCHMARK(fpclassify_32f::vec16x32f_blending_ternlog);

    #endif



    #if defined(AVEL_AVX512DQ)

    ///
    /// Generic implementation using vfpclass
    /// Portable across compilers but not optimal.
    ///
    vec16x32i vec16x32f_blending_vfpclass_impl(vec16x32f v) {
        const vec16x32i fp_infinite{int(FP_INFINITE)};
        const vec16x32i fp_nan{int(FP_NAN)};
        const vec16x32i fp_normal{int(FP_NORMAL)};
        const vec16x32i fp_subnormal{int(FP_SUBNORMAL)};
        const vec16x32i fp_zero{int(FP_ZERO)};

        mask16x32i infinite_mask {_mm512_fpclass_ps_mask(decay(v), 0x08 | 0x10)};
        mask16x32i nan_mask      {_mm512_fpclass_ps_mask(decay(v), 0x01 | 0x80)};
        mask16x32i subnormal_mask{_mm512_fpclass_ps_mask(decay(v), 0x20)};
        mask16x32i zero_mask     {_mm512_fpclass_ps_mask(decay(v), 0x02 | 0x04)};
        mask16x32i normal_mask   {!(infinite_mask | nan_mask | subnormal_mask | zero_mask)};

        return
            keep(infinite_mask, fp_infinite) |
            keep(nan_mask, fp_nan) |
            keep(normal_mask, fp_normal) |
            keep(subnormal_mask, fp_subnormal) |
            keep(zero_mask, fp_zero);
    }

    auto vec16x32f_blending_vfpclass = vector_test_bench<vec16x32f, vec16x32i, vec16x32f_blending_vfpclass_impl>;

    BENCHMARK(fpclassify_32f::vec16x32f_blending_vfpclass);

    #endif

}

#endif //AVEL_FPCLASSIFY32F_HPP
