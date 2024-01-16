#ifndef AVEL_DENOMINATOR32X16I_HPP
#define AVEL_DENOMINATOR32X16I_HPP

namespace avel {

    using Denom32x16i = Denominator<vec32x16i>;

    template<>
    class Denominator<vec32x16i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec32x16i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8i d):
            mp(d.mp),
            d_sign(d.d_sign),
            sh(d.sh),
            d(d.d) {}

        explicit Denominator(vec32x16i d):
            Denominator(d, max(vec32x16i{16} - countl_zero(abs(d) - vec32x16i{1}), vec32x16i{1})) {}

    private:

        explicit Denominator(vec32x16i d, vec32x16i l):
            mp(compute_m(l, d)),
            d_sign(d >> 15),
            sh(l - vec32x16i{1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec32x16i> div(vec32x16i n, Denominator denom) {
            vec32x16i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 15);
            vec32x16i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec32x16i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec32x16i operator/(vec32x16i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec32x16i operator%(vec32x16i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec32x16i value() const {
            return d;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec32x16i mp{};
        vec32x16i d_sign{};
        vec32x16i sh{};
        vec32x16i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec32x16i mulhi(vec32x16i x, vec32x16i y) {
            #if defined(AVEL_AVX512F)
            return vec32x16i{_mm512_mulhi_epi16(decay(x), decay(y))};

            #endif
        }

        static vec32x16i compute_m(vec32x16i l, vec32x16i d) {
            //TODO: Optimize widening
            auto l_lo = bit_cast<vec16x32i>(l) & vec16x32i{0xFFFF};
            auto l_hi = bit_cast<vec16x32i>(l) >> 16;

            d = abs(d);
            auto d_lo = bit_cast<vec16x32i>(d) & vec16x32i{0xFFFF};
            auto d_hi = bit_cast<vec16x32i>(d) >> 16;

            auto tmp0_lo = vec16x32i(0x8000) << l_lo;
            auto tmp0_hi = vec16x32i(0x8000) << l_hi;

            auto tmp2_lo = tmp0_lo / d_lo;
            auto tmp2_hi = tmp0_hi / d_hi;

            auto tmp3_lo = tmp2_lo - vec16x32i(0xFFFF);
            auto tmp3_hi = tmp2_hi - vec16x32i(0xFFFF);

            //TODO: Optimize narrowing
            auto x = bit_cast<vec32x16i>(tmp3_lo);
            auto y = bit_cast<vec32x16i>(tmp3_hi << 16);

            auto mask = mask32x16i{{
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
            return vec32x16i{ret};
        }

    };

    AVEL_FINL vec32x16i& operator/=(vec32x16i& lhs, Denom32x16i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec32x16i& operator%=(vec32x16i& lhs, Denom32x16i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR32X16I_HPP
