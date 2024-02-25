#ifndef AVEL_DENOMINATOR8X16I_HPP
#define AVEL_DENOMINATOR8X16I_HPP

namespace avel {

    using Denom8x16i = Denominator<vec8x16i>;

    template<>
    class Denominator<vec8x16i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec8x16i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8i d):
            mp(d.mp),
            d_sign(d.d_sign),
            sh(d.sh),
            d(d.d) {}

        explicit Denominator(vec8x16i d):
            Denominator(d, max(vec8x16i{16} - countl_zero(abs(d) - vec8x16i{1}), vec8x16i{1})) {}

    private:

        explicit Denominator(vec8x16i d, vec8x16i l):
            mp(compute_m(l, d)),
            d_sign(d >> 15),
            sh(l - vec8x16i{1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec8x16i> div(vec8x16i n, Denominator denom) {
            vec8x16i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 15);
            vec8x16i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec8x16i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x16i operator/(vec8x16i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x16i operator%(vec8x16i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        AVEL_FINL Denom8x16i& operator<<=(vec8x16i s) {
            auto effective_s = avel::min(avel::countl_sign(d), s);

            d <<= s;
            sh += effective_s;

            return *this;
        }

        AVEL_FINL Denom8x16i& operator>>=(vec8x16i s) {
            auto effective_s = avel::min(avel::countr_zero(d), s);

            d >>= s;
            sh -= effective_s;

            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom8x16i operator<<(vec8x16i s) const {
            Denom8x16i ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom8x16i operator>>(vec8x16i s) const {
            Denom8x16i ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec8x16i value() const {
            return d;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec8x16i mp{};
        vec8x16i d_sign{};
        vec8x16i sh{};
        vec8x16i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec8x16i mulhi(vec8x16i x, vec8x16i y) {
            #if defined(AVEL_SSE2)
            return vec8x16i{_mm_mulhi_epi16(decay(x), decay(y))};

            #endif

            #if defined(AVEL_NEON)
            auto x_half0 = vget_low_s16(decay(x));
            auto x_half1 = vget_high_s16(decay(x));

            auto y_half0 = vget_low_s16(decay(y));
            auto y_half1 = vget_high_s16(decay(y));

            auto prod_half0 = vreinterpretq_s16_s32(vmull_s16(x_half0, y_half0));
            auto prod_half1 = vreinterpretq_s16_s32(vmull_s16(x_half1, y_half1));

            auto prod = vuzpq_s16(prod_half0, prod_half1).val[1];

            return vec8x16i{prod};
            #endif
        }

        static vec8x16i compute_m(vec8x16i l, vec8x16i d) {
            //TODO: Optimize widening
            auto l_lo = bit_cast<vec4x32i>(l) & vec4x32i{0xFFFF};
            auto l_hi = bit_cast<vec4x32i>(l) >> 16;

            d = abs(d);
            auto d_lo = bit_cast<vec4x32i>(d) & vec4x32i{0xFFFF};
            auto d_hi = bit_cast<vec4x32i>(d) >> 16;

            auto tmp0_lo = vec4x32i(0x8000) << l_lo;
            auto tmp0_hi = vec4x32i(0x8000) << l_hi;

            auto tmp2_lo = tmp0_lo / d_lo;
            auto tmp2_hi = tmp0_hi / d_hi;

            auto tmp3_lo = tmp2_lo - vec4x32i(0xFFFF);
            auto tmp3_hi = tmp2_hi - vec4x32i(0xFFFF);

            //TODO: Optimize narrowing
            auto x = bit_cast<vec8x16i>(tmp3_lo);
            auto y = bit_cast<vec8x16i>(tmp3_hi << 16);

            auto mask = mask8x16i{arr8xb{
                false, true, false, true,
                false, true, false, true
            }};
            auto ret = blend(mask, y, x);
            return vec8x16i{ret};
        }

    };

    AVEL_FINL vec8x16i& operator/=(vec8x16i& lhs, Denom8x16i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec8x16i& operator%=(vec8x16i& lhs, Denom8x16i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR8X16I_HPP
