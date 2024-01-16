#ifndef AVEL_DENOMINATOR8X32U_HPP
#define AVEL_DENOMINATOR8X32U_HPP

namespace avel {

    using Denom8x32u = Denominator<vec8x32u>;

    template<>
    class Denominator<vec8x32u> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec8x32u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom32u denom):
            m(denom.m),
            sh2(denom.sh2),
            d(denom.d) {}

        explicit Denominator(vec8x32u d):
            Denominator(d, vec8x32u{32} - countl_zero(d - vec8x32u{1})) {}

    private:

        explicit Denominator(vec8x32u d, vec8x32u l):
            m(compute_m(l, d)),
            sh1(min(l, vec8x32u{1})),
            sh2(l - vec8x32u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec8x32u> div(vec8x32u n, Denominator denom) {
            vec8x32u t1 = mulhi(denom.m, n);
            vec8x32u q = (t1 + ((n - t1) >> denom.sh1)) >> denom.sh2;
            vec8x32u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x32u operator/(vec8x32u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x32u operator%(vec8x32u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec8x32u value() const {
            return d;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec8x32u m{};
        vec8x32u sh1{};
        vec8x32u sh2{};
        vec8x32u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec8x32u mulhi(vec8x32u x, vec8x32u y) {
            #if defined(AVEL_AVX2)
            vec8x32u lo{_mm256_srli_si256(_mm256_mul_epu32(decay(x), decay(y)), 4)};
            vec8x32u hi{_mm256_mul_epu32(_mm256_srli_si256(decay(x), 4), _mm256_srli_si256(decay(y), 4))};

            mask8x32u m{arr8xb{
                false, true, false, true,
                false, true, false, true
            }};
            return blend(m, hi, lo);
            #endif
        }

        static vec8x32u compute_m(vec8x32u l, vec8x32u d) {
            //TODO: Optimize
            vec4x64u l_lo = bit_cast<vec4x64u>(l) & vec4x64u{0x00000000FFFFFFFFull};
            vec4x64u l_hi = bit_cast<vec4x64u>(l) >> 32;

            vec4x64u d_lo = bit_cast<vec4x64u>(d) & vec4x64u{0x00000000FFFFFFFFull};
            vec4x64u d_hi = bit_cast<vec4x64u>(d) >> 32;

            vec4x64u tmp0_lo = (vec4x64u{1} << l_lo) - d_lo;
            vec4x64u tmp0_hi = (vec4x64u{1} << l_hi) - d_hi;

            vec4x64u tmp1_lo = (tmp0_lo << 32) / d_lo;
            vec4x64u tmp1_hi = (tmp0_hi << 32) / d_hi;

            vec8x32u m_lo = bit_cast<vec8x32u>(tmp1_lo);
            vec8x32u m_hi = bit_cast<vec8x32u>(tmp1_hi << 32);

            vec8x32u m = m_lo | m_hi;
            m += vec8x32u{1};
            return m;
        }

    };

    AVEL_FINL vec8x32u& operator/=(vec8x32u& lhs, Denom8x32u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec8x32u& operator%=(vec8x32u& lhs, Denom8x32u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR8X32U_HPP
