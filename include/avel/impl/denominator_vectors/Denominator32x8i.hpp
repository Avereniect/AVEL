#ifndef AVEL_DENOMINATOR32X8I_HPP
#define AVEL_DENOMINATOR32X8I_HPP

namespace avel {

    using Denom32x8i = Denominator<vec32x8i>;

    template<>
    class Denominator<vec32x8i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec32x8i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8i d):
            mp(d.mp),
            d_sign(d.d_sign),
            sh(d.sh),
            d(d.d) {}

        explicit Denominator(vec32x8i d):
            Denominator(d, max(vec32x8i{8} - countl_zero(abs(d) - vec32x8i{1}), vec32x8i{1})) {}

    private:

        explicit Denominator(vec32x8i d, vec32x8i l):
            mp(compute_m(l, d)),
            d_sign(d >> 31),
            sh(l - vec32x8i{1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec32x8i> div(vec32x8i n, Denominator denom) {
            vec32x8i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 7);
            vec32x8i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec32x8i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec32x8i operator/(vec32x8i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec32x8i operator%(vec32x8i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec32x8i value() const {
            return d;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec32x8i mp{};
        vec32x8i d_sign{};
        vec32x8i sh{};
        vec32x8i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec32x8i mulhi(vec32x8i x, vec32x8i y) {
            #if defined(AVEL_SSE2)
            auto zeros = _mm256_setzero_si256();

            auto x_sign = _mm256_cmpgt_epi8(zeros, decay(x));
            auto lo_x = _mm256_unpacklo_epi8(decay(x), x_sign);
            auto hi_x = _mm256_unpackhi_epi8(decay(x), x_sign);

            auto y_sign = _mm256_cmpgt_epi8(zeros, decay(y));
            auto lo_y = _mm256_unpacklo_epi8(decay(y), y_sign);
            auto hi_y = _mm256_unpackhi_epi8(decay(y), y_sign);

            auto p_lo = _mm256_mullo_epi16(lo_x, lo_y);
            auto p_hi = _mm256_mullo_epi16(hi_x, hi_y);

            auto ret_lo = _mm256_srai_epi16(p_lo, 8);
            auto ret_hi = _mm256_srai_epi16(p_hi, 8);

            auto ret = _mm256_packs_epi16(ret_lo, ret_hi);
            return vec32x8i{ret};

            #endif
        }

        static vec32x8i compute_m(vec32x8i l, vec32x8i d) {
            //TODO: Optimize widening
            auto l_lo = bit_cast<vec16x16i>(l) & vec16x16i{0xFF};
            auto l_hi = bit_cast<vec16x16i>(l) >> 8;

            d = abs(d);
            auto d_lo = bit_cast<vec16x16i>(d) & vec16x16i{0xFF};
            auto d_hi = bit_cast<vec16x16i>(d) >> 8;

            auto tmp0_lo = vec16x16i{0x80} << l_lo;
            auto tmp0_hi = vec16x16i{0x80} << l_hi;

            auto tmp2_lo = tmp0_lo / d_lo;
            auto tmp2_hi = tmp0_hi / d_hi;

            auto tmp3_lo = tmp2_lo - vec16x16i{0xff};
            auto tmp3_hi = tmp2_hi - vec16x16i{0xff};

            //TODO: Optimize narrowing
            auto x = bit_cast<vec32x8i>(tmp3_lo);
            auto y = bit_cast<vec32x8i>(tmp3_hi << 8);

            auto mask = mask32x8i{arr32xb{
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
            return vec32x8i{ret};
        }

    };

    AVEL_FINL vec32x8i& operator/=(vec32x8i& lhs, Denom32x8i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec32x8i& operator%=(vec32x8i& lhs, Denom32x8i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR32X8I_HPP
