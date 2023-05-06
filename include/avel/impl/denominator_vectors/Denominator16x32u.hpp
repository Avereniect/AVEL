#ifndef AVEL_RECIPROCAL16X32U_HPP
#define AVEL_RECIPROCAL16X32U_HPP

namespace avel {

    using Denom16x32u = Denominator<vec16x32u>;

    template<>
    class alignas(64) Denominator<vec16x32u> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec16x32u;

        //=================================================
        // -ctors
        //=================================================

        // TODO: Uncomment and fix
        //explicit Denominator(Denom32u denom):
        //    m(denom.m),
        //    sh1(denom.sh1),
        //    sh2(denom.sh2),
        //    d(denom.d) {}

        explicit Denominator(vec16x32u d):
            Denominator(d, vec16x32u{32} - countl_zero(d - vec16x32u{1})) {}

    private:

        explicit Denominator(vec16x32u d, vec16x32u l):
            m(compute_m(l, d)),
            sh1(min(l, vec16x32u{1})),
            sh2(l - vec16x32u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec16x32u> div(vec16x32u n, Denominator denom) {
            vec16x32u t1 = mulhi(denom.m, n);
            vec16x32u q = (t1 + ((n - t1) >> denom.sh1)) >> denom.sh2;
            vec16x32u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x32u operator/(vec16x32u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x32u operator%(vec16x32u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec16x32u m{};
        vec16x32u sh1{};
        vec16x32u sh2{};
        vec16x32u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec16x32u mulhi(vec16x32u x, vec16x32u y) {
            #if defined(AVEL_AVX2)
            vec16x32u lo{_mm512_srli_epi64(_mm512_mul_epu32(decay(x), decay(y)), 32)};
            vec16x32u hi{_mm512_mul_epu32(_mm512_srli_epi64(decay(x), 32), _mm512_srli_epi64(decay(y), 32))};

            mask16x32u m{{
                false, true, false, true,
                false, true, false, true,
                false, true, false, true,
                false, true, false, true
            }};
            return blend(m, hi, lo);
            #endif
        }

        static vec16x32u compute_m(vec16x32u l, vec16x32u d) {
            //TODO: Optimize
            vec8x64u l_lo = bit_cast<vec8x64u>(l) & vec8x64u{0x00000000FFFFFFFFull};
            vec8x64u l_hi = bit_cast<vec8x64u>(l) >> 32;

            vec8x64u d_lo = bit_cast<vec8x64u>(d) & vec8x64u{0x00000000FFFFFFFFull};
            vec8x64u d_hi = bit_cast<vec8x64u>(d) >> 32;

            vec8x64u tmp0_lo = (vec8x64u{1} << l_lo) - d_lo;
            vec8x64u tmp0_hi = (vec8x64u{1} << l_hi) - d_hi;

            vec8x64u tmp1_lo = (tmp0_lo << 32) / d_lo;
            vec8x64u tmp1_hi = (tmp0_hi << 32) / d_hi;

            vec16x32u m_lo = bit_cast<vec16x32u>(tmp1_lo);
            vec16x32u m_hi = bit_cast<vec16x32u>(tmp1_hi << 32);

            vec16x32u m = m_lo | m_hi;
            m += vec16x32u{1};
            return m;
        }

    };

    AVEL_FINL vec16x32u& operator/=(vec16x32u& lhs, Denom16x32u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec16x32u& operator%=(vec16x32u& lhs, Denom16x32u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_RECIPROCAL16X32U_HPP
