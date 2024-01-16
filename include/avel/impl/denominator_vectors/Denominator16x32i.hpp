#ifndef AVEL_DENOMINATOR16X32I_HPP
#define AVEL_DENOMINATOR16X32I_HPP

namespace avel {

    using Denom16x32i = Denominator<vec16x32i>;

    template<>
    class Denominator<vec16x32i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec16x32i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom32i recip):
            mp(recip.mp),
            d_sign(recip.d_sign),
            sh(recip.sh),
            d(recip.d) {}

        explicit Denominator(vec16x32i d):
            Denominator(d, max(vec16x32i{32} - vec16x32i{countl_zero(abs(d) - vec16x32i{1})}, vec16x32i{1})) {}

    private:

        explicit Denominator(vec16x32i d, vec16x32i l):
            mp(compute_mp(l, d)),
            d_sign(d >> 31),
            sh(l - vec16x32i{1}),
            d(d) {}

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec16x32i> div(vec16x32i n, Denominator denom) {
            vec16x32i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 31);
            vec16x32i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec16x32i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x32i operator/(vec16x32i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x32i operator%(vec16x32i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec16x32i value() const {
            return d;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec16x32i mp{};
        vec16x32i d_sign {};
        vec16x32i sh{};
        vec16x32i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec16x32i mulhi(vec16x32i x, vec16x32i y) {
            #if defined(AVEL_AVX512F)
            auto t0 = _mm512_unpacklo_epi32(decay(x), decay(x));
            auto t1 = _mm512_unpackhi_epi32(decay(x), decay(x));
            auto t2 = _mm512_unpacklo_epi32(decay(y), decay(y));
            auto t3 = _mm512_unpackhi_epi32(decay(y), decay(y));

            auto t4 = _mm512_mul_epi32(t0, t2);
            auto t5 = _mm512_mul_epi32(t1, t3);

            auto ret = _mm512_shuffle_ps(_mm512_castsi512_ps(t4), _mm512_castsi512_ps(t5), 0xdd);
            return vec16x32i{_mm512_castps_si512(ret)};

            #endif
        }

        static vec16x32i compute_mp(vec16x32i l, vec16x32i d) {
            //TODO: Optimize widening
            auto l_lo = bit_cast<vec8x64i>(l) & vec8x64i{0xFFFFFFFF};
            auto l_hi = bit_cast<vec8x64i>(l) >> 32;

            d = abs(d);
            auto d_lo = bit_cast<vec8x64i>(d) & vec8x64i{0xFFFFFFFF};
            auto d_hi = bit_cast<vec8x64i>(d) >> 32;

            auto tmp0_lo = vec8x64i(0x80000000) << l_lo;
            auto tmp0_hi = vec8x64i(0x80000000) << l_hi;

            auto tmp2_lo = tmp0_lo / d_lo;
            auto tmp2_hi = tmp0_hi / d_hi;

            auto tmp3_lo = tmp2_lo - vec8x64i(0xFFFFFFFF);
            auto tmp3_hi = tmp2_hi - vec8x64i(0xFFFFFFFF);

            //TODO: Optimize narrowing
            auto x = bit_cast<vec16x32i>(tmp3_lo);
            auto y = bit_cast<vec16x32i>(tmp3_hi << 32);

            auto mask = mask16x32i{{
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true
            }};
            auto ret = blend(mask, y, x);
            return vec16x32i{ret};
        }

    };

    AVEL_FINL vec16x32i& operator/=(vec16x32i& lhs, Denom16x32i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec16x32i& operator%=(vec16x32i& lhs, Denom16x32i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR16X32I_HPP
