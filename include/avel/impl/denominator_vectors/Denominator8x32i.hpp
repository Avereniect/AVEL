#ifndef AVEL_DENOMINATOR8X32I_HPP
#define AVEL_DENOMINATOR8X32I_HPP

namespace avel {

    using Denom8x32i = Denominator<vec8x32i>;

    template<>
    class Denominator<vec8x32i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec8x32i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom32i recip):
            mp(recip.mp),
            d_sign(recip.d_sign),
            sh(recip.sh),
            d(recip.d) {}

        explicit Denominator(vec8x32i d):
            Denominator(d, max(vec8x32i{32} - vec8x32i{countl_zero(abs(d) - vec8x32i{1})}, vec8x32i{1})) {}

    private:

        explicit Denominator(vec8x32i d, vec8x32i l):
            mp(compute_mp(l, d)),
            d_sign(d >> 31),
            sh(l - vec8x32i{1}),
            d(d) {}

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec8x32i> div(vec8x32i n, Denominator denom) {
            vec8x32i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 31);
            vec8x32i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec8x32i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x32i operator/(vec8x32i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x32i operator%(vec8x32i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        AVEL_FINL Denom8x32i& operator<<=(vec8x32i s) {
            auto effective_s = avel::min(avel::countl_sign(d), s);

            d <<= s;
            sh += effective_s;

            return *this;
        }

        AVEL_FINL Denom8x32i& operator>>=(vec8x32i s) {
            auto effective_s = avel::min(avel::countr_zero(d), s);

            d >>= s;
            sh -= effective_s;

            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom8x32i operator<<(vec8x32i s) const {
            Denom8x32i ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom8x32i operator>>(vec8x32i s) const {
            Denom8x32i ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec8x32i value() const {
            return d;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec8x32i mp{};
        vec8x32i d_sign {};
        vec8x32i sh{};
        vec8x32i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec8x32i mulhi(vec8x32i x, vec8x32i y) {
            #if defined(AVEL_AVX2)
            auto t0 = _mm256_unpacklo_epi32(decay(x), decay(x));
            auto t1 = _mm256_unpackhi_epi32(decay(x), decay(x));
            auto t2 = _mm256_unpacklo_epi32(decay(y), decay(y));
            auto t3 = _mm256_unpackhi_epi32(decay(y), decay(y));

            auto t4 = _mm256_mul_epi32(t0, t2);
            auto t5 = _mm256_mul_epi32(t1, t3);

            auto ret = _mm256_shuffle_ps(_mm256_castsi256_ps(t4), _mm256_castsi256_ps(t5), 0xdd);
            return vec8x32i{_mm256_castps_si256(ret)};

            #endif
        }

        static vec8x32i compute_mp(vec8x32i l, vec8x32i d) {
            //TODO: Optimize widening
            auto l_lo = bit_cast<vec4x64i>(l) & vec4x64i{0xFFFFFFFF};
            auto l_hi = bit_cast<vec4x64i>(l) >> 32;

            d = abs(d);
            auto d_lo = bit_cast<vec4x64i>(d) & vec4x64i{0xFFFFFFFF};
            auto d_hi = bit_cast<vec4x64i>(d) >> 32;

            auto tmp0_lo = vec4x64i(0x80000000) << l_lo;
            auto tmp0_hi = vec4x64i(0x80000000) << l_hi;

            auto tmp2_lo = tmp0_lo / d_lo;
            auto tmp2_hi = tmp0_hi / d_hi;

            auto tmp3_lo = tmp2_lo - vec4x64i(0xFFFFFFFF);
            auto tmp3_hi = tmp2_hi - vec4x64i(0xFFFFFFFF);

            //TODO: Optimize narrowing
            auto x = bit_cast<vec8x32i>(tmp3_lo);
            auto y = bit_cast<vec8x32i>(tmp3_hi << 32);

            auto mask = mask8x32i{arr8xb{
                false, true, false, true,
                false, true, false, true
            }};
            auto ret = blend(mask, y, x);
            return vec8x32i{ret};
        }

    };

    AVEL_FINL vec8x32i& operator/=(vec8x32i& lhs, Denom8x32i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec8x32i& operator%=(vec8x32i& lhs, Denom8x32i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR8X32I_HPP
