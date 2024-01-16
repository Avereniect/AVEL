#ifndef AVEL_DENOMINATOR16X16I_HPP
#define AVEL_DENOMINATOR16X16I_HPP

namespace avel {

    using Denom16x16i = Denominator<vec16x16i>;

    template<>
    class Denominator<vec16x16i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec16x16i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8i d):
            mp(d.mp),
            d_sign(d.d_sign),
            sh(d.sh),
            d(d.d) {}

        explicit Denominator(vec16x16i d):
            Denominator(d, max(vec16x16i{16} - countl_zero(abs(d) - vec16x16i{1}), vec16x16i{1})) {}

    private:

        explicit Denominator(vec16x16i d, vec16x16i l):
            mp(compute_m(l, d)),
            d_sign(d >> 15),
            sh(l - vec16x16i{1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec16x16i> div(vec16x16i n, Denominator denom) {
            vec16x16i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 15);
            vec16x16i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec16x16i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x16i operator/(vec16x16i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x16i operator%(vec16x16i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec16x16i value() const {
            return d;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec16x16i mp{};
        vec16x16i d_sign{};
        vec16x16i sh{};
        vec16x16i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec16x16i mulhi(vec16x16i x, vec16x16i y) {
            #if defined(AVEL_AVX2)
            return vec16x16i{_mm256_mulhi_epi16(decay(x), decay(y))};

            #endif
        }

        static vec16x16i compute_m(vec16x16i l, vec16x16i d) {
            //TODO: Optimize widening
            auto l_lo = bit_cast<vec8x32i>(l) & vec8x32i{0xFFFF};
            auto l_hi = bit_cast<vec8x32i>(l) >> 16;

            d = abs(d);
            auto d_lo = bit_cast<vec8x32i>(d) & vec8x32i{0xFFFF};
            auto d_hi = bit_cast<vec8x32i>(d) >> 16;

            auto tmp0_lo = vec8x32i(0x8000) << l_lo;
            auto tmp0_hi = vec8x32i(0x8000) << l_hi;

            auto tmp2_lo = tmp0_lo / d_lo;
            auto tmp2_hi = tmp0_hi / d_hi;

            auto tmp3_lo = tmp2_lo - vec8x32i(0xFFFF);
            auto tmp3_hi = tmp2_hi - vec8x32i(0xFFFF);

            //TODO: Optimize narrowing
            auto x = bit_cast<vec16x16i>(tmp3_lo);
            auto y = bit_cast<vec16x16i>(tmp3_hi << 16);

            auto mask = mask16x16i{arr16xb{
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true
            }};
            auto ret = blend(mask, y, x);
            return vec16x16i{ret};
        }

    };

    AVEL_FINL vec16x16i& operator/=(vec16x16i& lhs, Denom16x16i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec16x16i& operator%=(vec16x16i& lhs, Denom16x16i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR16X16I_HPP
