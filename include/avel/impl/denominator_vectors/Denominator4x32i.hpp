#ifndef AVEL_DENOMINATOR4X32I_HPP
#define AVEL_DENOMINATOR4X32I_HPP

namespace avel {

    using Denom4x32i = Denominator<vec4x32i>;

    template<>
    class alignas(16) Denominator<vec4x32i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom32i recip):
            mp(recip.mp),
            d_sign(recip.d_sign),
            sh(recip.sh),
            d(recip.d) {}

        explicit Denominator(vec4x32i d):
            Denominator(d, max(vec4x32i{32} - vec4x32i{countl_zero(abs(d) - vec4x32i{1})}, vec4x32i{1})) {}

    private:

        explicit Denominator(vec4x32i d, vec4x32i l):
            mp(compute_mp(l, d)),
            d_sign(d >> 31),
            sh(l - vec4x32i{1}),
            d(d) {}

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec4x32i> div(vec4x32i n, Denominator denom) {
            vec4x32i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 31);
            vec4x32i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec4x32i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec4x32i operator/(vec4x32i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec4x32i operator%(vec4x32i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec4x32i mp{};
        vec4x32i d_sign {};
        vec4x32i sh{};
        vec4x32i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec4x32i mulhi(vec4x32i x, vec4x32i y) {
            #if defined(AVEL_SSE41)
            auto t0 = _mm_unpacklo_epi32(decay(x), decay(x));
            auto t1 = _mm_unpackhi_epi32(decay(x), decay(x));
            auto t2 = _mm_unpacklo_epi32(decay(y), decay(y));
            auto t3 = _mm_unpackhi_epi32(decay(y), decay(y));

            auto t4 = _mm_mul_epi32(t0, t2);
            auto t5 = _mm_mul_epi32(t1, t3);

            auto ret = _mm_shuffle_ps(_mm_castsi128_ps(t4), _mm_castsi128_ps(t5), 0xdd);
            return vec4x32i{_mm_castps_si128(ret)};

            #elif defined(AVEL_SSE2)
            auto t0 = _mm_unpacklo_epi32(decay(x), decay(x));
            auto t1 = _mm_unpackhi_epi32(decay(x), decay(x));
            auto t2 = _mm_unpacklo_epi32(decay(y), decay(y));
            auto t3 = _mm_unpackhi_epi32(decay(y), decay(y));

            auto t4 = _mm_mul_epu32(t0, t2);
            auto t5 = _mm_mul_epu32(t1, t3);

            auto unsigned_results = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(t4), _mm_castsi128_ps(t5), 0xdd));

            //Conversion from unsigned hi word to signed hi word
            auto s0 = _mm_and_si128(_mm_srai_epi32(decay(x), 31), decay(y));
            auto s1 = _mm_and_si128(_mm_srai_epi32(decay(y), 31), decay(x));

            auto ret = _mm_sub_epi32(_mm_sub_epi32(unsigned_results, s0), s1);
            return vec4x32i{ret};

            #endif

            #if defined(AVEL_NEON)
            auto x_half0 = vget_low_s32(decay(x));
            auto x_half1 = vget_high_s32(decay(x));

            auto y_half0 = vget_low_s32(decay(y));
            auto y_half1 = vget_high_s32(decay(y));

            auto prod_half0 = vreinterpretq_s32_s64(vmull_s32(x_half0, y_half0));
            auto prod_half1 = vreinterpretq_s32_s64(vmull_s32(x_half1, y_half1));

            auto prod = vuzpq_s32(prod_half0, prod_half1).val[1];

            return vec4x32i{prod};
            #endif
        }

        static vec4x32i compute_mp(vec4x32i l, vec4x32i d) {
            //TODO: Optimize widening
            auto l_lo = bit_cast<vec2x64i>(l) & vec2x64i{0xFFFFFFFF};
            auto l_hi = bit_cast<vec2x64i>(l) >> 32;

            d = abs(d);
            auto d_lo = bit_cast<vec2x64i>(d) & vec2x64i{0xFFFFFFFF};
            auto d_hi = bit_cast<vec2x64i>(d) >> 32;

            auto tmp0_lo = vec2x64i(0x80000000) << l_lo;
            auto tmp0_hi = vec2x64i(0x80000000) << l_hi;

            auto tmp2_lo = tmp0_lo / d_lo;
            auto tmp2_hi = tmp0_hi / d_hi;

            auto tmp3_lo = tmp2_lo - vec2x64i(0xFFFFFFFF);
            auto tmp3_hi = tmp2_hi - vec2x64i(0xFFFFFFFF);

            //TODO: Optimize narrowing
            auto x = bit_cast<vec4x32i>(tmp3_lo);
            auto y = bit_cast<vec4x32i>(tmp3_hi << 32);

            auto mask = mask4x32i{{
                false, true, false, true
            }};
            auto ret = blend(mask, y, x);
            return vec4x32i{ret};
        }

    };

}

#endif //AVEL_DENOMINATOR4X32I_HPP
