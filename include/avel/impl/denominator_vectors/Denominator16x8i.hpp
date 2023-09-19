#ifndef AVEL_DENOMINATOR16X8I_HPP
#define AVEL_DENOMINATOR16X8I_HPP

namespace avel {

    using Denom16x8i = Denominator<vec16x8i>;

    template<>
    class Denominator<vec16x8i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec16x8i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8i d):
            mp(d.mp),
            d_sign(d.d_sign),
            sh(d.sh),
            d(d.d) {}

        explicit Denominator(vec16x8i d):
            Denominator(d, max(vec16x8i{8} - countl_zero(abs(d) - vec16x8i{1}), vec16x8i{1})) {}

    private:

        explicit Denominator(vec16x8i d, vec16x8i l):
            mp(compute_m(l, d)),
            d_sign(d >> 31),
            sh(l - vec16x8i{1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec16x8i> div(vec16x8i n, Denominator denom) {
            vec16x8i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 7);
            vec16x8i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec16x8i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x8i operator/(vec16x8i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x8i operator%(vec16x8i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec16x8i mp{};
        vec16x8i d_sign{};
        vec16x8i sh{};
        vec16x8i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec16x8i mulhi(vec16x8i x, vec16x8i y) {
            #if defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto x_sign = _mm_cmplt_epi8(decay(x), zeros);
            auto lo_x = _mm_unpacklo_epi8(decay(x), x_sign);
            auto hi_x = _mm_unpackhi_epi8(decay(x), x_sign);

            auto y_sign = _mm_cmplt_epi8(decay(y), zeros);
            auto lo_y = _mm_unpacklo_epi8(decay(y), y_sign);
            auto hi_y = _mm_unpackhi_epi8(decay(y), y_sign);

            auto p_lo = _mm_mullo_epi16(lo_x, lo_y);
            auto p_hi = _mm_mullo_epi16(hi_x, hi_y);

            auto ret_lo = _mm_srai_epi16(p_lo, 8);
            auto ret_hi = _mm_srai_epi16(p_hi, 8);

            auto ret = _mm_packs_epi16(ret_lo, ret_hi);
            return vec16x8i{ret};

            #endif

            #if defined(AVEL_NEON)
            auto x_half0 = vget_low_s8(decay(x));
            auto x_half1 = vget_high_s8(decay(x));

            auto y_half0 = vget_low_s8(decay(y));
            auto y_half1 = vget_high_s8(decay(y));

            auto prod_half0 = vreinterpretq_s8_s16(vmull_s8(x_half0, y_half0));
            auto prod_half1 = vreinterpretq_s8_s16(vmull_s8(x_half1, y_half1));

            auto prod = vuzpq_s8(prod_half0, prod_half1).val[1];

            return vec16x8i{prod};
            #endif
        }

        static vec16x8i compute_m(vec16x8i l, vec16x8i d) {
            //TODO: Optimize widening
            auto l_lo = bit_cast<vec8x16i>(l) & vec8x16i{0xFF};
            auto l_hi = bit_cast<vec8x16i>(l) >> 8;

            d = abs(d);
            auto d_lo = bit_cast<vec8x16i>(d) & vec8x16i{0xFF};
            auto d_hi = bit_cast<vec8x16i>(d) >> 8;

            auto tmp0_lo = vec8x16i{0x80} << l_lo;
            auto tmp0_hi = vec8x16i{0x80} << l_hi;

            auto tmp2_lo = tmp0_lo / d_lo;
            auto tmp2_hi = tmp0_hi / d_hi;

            auto tmp3_lo = tmp2_lo - vec8x16i{0xff};
            auto tmp3_hi = tmp2_hi - vec8x16i{0xff};

            //TODO: Optimize narrowing
            auto x = bit_cast<vec16x8i>(tmp3_lo);
            auto y = bit_cast<vec16x8i>(tmp3_hi << 8);

            auto mask = mask16x8i{arr16xb{
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true
            }};
            auto ret = blend(mask, y, x);
            return vec16x8i{ret};
        }

    };

    AVEL_FINL vec16x8i& operator/=(vec16x8i& lhs, Denom16x8i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec16x8i& operator%=(vec16x8i& lhs, Denom16x8i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR16X8I_HPP
