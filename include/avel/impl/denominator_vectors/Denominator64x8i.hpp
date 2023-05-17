#ifndef AVEL_DENOMINATOR64X8I_HPP
#define AVEL_DENOMINATOR64X8I_HPP

namespace avel {

    using Denom64x8i = Denominator<vec64x8i>;

    template<>
    class Denominator<vec64x8i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec64x8i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8i d):
            mp(d.mp),
            d_sign(d.d_sign),
            sh(d.sh),
            d(d.d) {}

        explicit Denominator(vec64x8i d):
            Denominator(d, max(vec64x8i{8} - countl_zero(abs(d) - vec64x8i{1}), vec64x8i{1})) {}

    private:

        explicit Denominator(vec64x8i d, vec64x8i l):
            mp(compute_m(l, d)),
            d_sign(bit_shift_right<7>(d)),
            sh(l - vec64x8i{1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec64x8i> div(vec64x8i n, Denominator denom) {
            vec64x8i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 7);
            vec64x8i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec64x8i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec64x8i operator/(vec64x8i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec64x8i operator%(vec64x8i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec64x8i mp{};
        vec64x8i d_sign{};
        vec64x8i sh{};
        vec64x8i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec64x8i mulhi(vec64x8i x, vec64x8i y) {
            #if defined(AVEL_AVX512BW)
            //TODO: Optimize
            auto zeros = _mm512_setzero_si512();

            auto x_sign = _mm512_cmplt_epi8_mask(decay(x), zeros);
            auto lo_x = _mm512_unpacklo_epi8(decay(x), _mm512_movm_epi8(x_sign));
            auto hi_x = _mm512_unpackhi_epi8(decay(x), _mm512_movm_epi8(x_sign));

            auto y_sign = _mm512_cmplt_epi8_mask(decay(y), zeros);
            auto lo_y = _mm512_unpacklo_epi8(decay(y), _mm512_movm_epi8(y_sign));
            auto hi_y = _mm512_unpackhi_epi8(decay(y), _mm512_movm_epi8(y_sign));

            auto p_lo = _mm512_mullo_epi16(lo_x, lo_y);
            auto p_hi = _mm512_mullo_epi16(hi_x, hi_y);

            auto ret_lo = _mm512_srai_epi16(p_lo, 8);
            auto ret_hi = _mm512_srai_epi16(p_hi, 8);

            auto ret = _mm512_packs_epi16(ret_lo, ret_hi);
            return vec64x8i{ret};

            #endif
        }

        static vec64x8i compute_m(vec64x8i l, vec64x8i d) {
            //TODO: Optimize widening
            auto l_lo = bit_cast<vec32x16i>(l) & vec32x16i{0xFF};
            auto l_hi = bit_cast<vec32x16i>(l) >> 8;

            d = abs(d);
            auto d_lo = bit_cast<vec32x16i>(d) & vec32x16i{0xFF};
            auto d_hi = bit_cast<vec32x16i>(d) >> 8;

            auto tmp0_lo = vec32x16i{0x80} << l_lo;
            auto tmp0_hi = vec32x16i{0x80} << l_hi;

            auto tmp2_lo = tmp0_lo / d_lo;
            auto tmp2_hi = tmp0_hi / d_hi;

            auto tmp3_lo = tmp2_lo - vec32x16i{0xff};
            auto tmp3_hi = tmp2_hi - vec32x16i{0xff};

            //TODO: Optimize narrowing
            auto x = bit_cast<vec64x8i>(tmp3_lo);
            auto y = bit_cast<vec64x8i>(tmp3_hi << 8);

            auto mask = mask64x8i{{
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true
            }};
            auto ret = blend(mask, y, x);
            return vec64x8i{ret};
        }

    };

    AVEL_FINL vec64x8i& operator/=(vec64x8i& lhs, Denom64x8i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec64x8i& operator%=(vec64x8i& lhs, Denom64x8i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR64X8I_HPP
