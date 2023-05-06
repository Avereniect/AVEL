#ifndef AVEL_DENOMINATOR4X32U_HPP
#define AVEL_DENOMINATOR4X32U_HPP

namespace avel {

    using Denom4x32u = Denominator<vec4x32u>;

    template<>
    class alignas(64) Denominator<vec4x32u> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec4x32u;

        //=================================================
        // -ctors
        //=================================================

        // TODO: Uncomment and fix
        //explicit Denominator(Denom32u denom):
        //    m(denom.m),
        //    sh1(denom.sh1),
        //    sh2(denom.sh2),
        //    d(denom.d) {}

        explicit Denominator(vec4x32u d):
            Denominator(d, vec4x32u{32} - countl_zero(d - vec4x32u{1})) {}

    private:

        explicit Denominator(vec4x32u d, vec4x32u l):
            m(compute_m(l, d)),
            sh1(min(l, vec4x32u{1})),
            sh2(l - vec4x32u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec4x32u> div(vec4x32u n, Denominator denom) {
            vec4x32u t1 = mulhi(denom.m, n);
            #if defined(AVEL_AVX2)
            vec4x32u q = (t1 + ((n - t1) >> denom.sh1)) >> denom.sh2;

            #elif defined(AVEL_SSE2)
            vec4x32u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec4x32u q = (t1 + t0) >> denom.sh2;

            #endif

            #if defined(AVEL_NEON)
            vec4x32u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec4x32u q = (t1 + t0) >> denom.sh2;
            #endif

            vec4x32u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec4x32u operator/(vec4x32u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec4x32u operator%(vec4x32u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec4x32u m{};

        #if defined(AVEL_AVX2)
        vec4x32u sh1{};
        #else
        mask4x32u sh1{};
        #endif

        vec4x32u sh2{};
        vec4x32u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec4x32u mulhi(vec4x32u x, vec4x32u y) {
            #if defined(AVEL_SSE2)
            vec4x32u lo{_mm_srli_si128(_mm_mul_epu32(decay(x), decay(y)), 4)};
            vec4x32u hi{_mm_mul_epu32(_mm_srli_si128(decay(x), 4), _mm_srli_si128(decay(y), 4))};

            mask4x32u m{{false, true, false, true}};
            return blend(m, hi, lo);
            #endif

            #if defined(AVEL_NEON)
            auto x_half0 = vget_low_u32(decay(x));
            auto x_half1 = vget_high_u32(decay(x));

            auto y_half0 = vget_low_u32(decay(y));
            auto y_half1 = vget_high_u32(decay(y));

            auto prod_half0 = vreinterpretq_u32_u64(vmull_u32(x_half0, y_half0));
            auto prod_half1 = vreinterpretq_u32_u64(vmull_u32(x_half1, y_half1));

            auto prod = vuzpq_u32(prod_half0, prod_half1).val[1];

            return vec4x32u{prod};

            #endif
        }

        static vec4x32u compute_m(vec4x32u l, vec4x32u d) {
            //TODO: Optimize for case where vec4x64u is available
            //TODO: Use conversion functions once implemented instead
            vec2x64u l_lo = bit_cast<vec2x64u>(l) & vec2x64u{0x00000000FFFFFFFFull};
            vec2x64u l_hi = bit_cast<vec2x64u>(l) >> 32;

            vec2x64u d_lo = bit_cast<vec2x64u>(d) & vec2x64u{0x00000000FFFFFFFFull};
            vec2x64u d_hi = bit_cast<vec2x64u>(d) >> 32;

            vec2x64u tmp0_lo = (vec2x64u{1} << l_lo) - d_lo;
            vec2x64u tmp0_hi = (vec2x64u{1} << l_hi) - d_hi;

            vec2x64u tmp1_lo = (tmp0_lo << 32) / d_lo;
            vec2x64u tmp1_hi = (tmp0_hi << 32) / d_hi;

            vec4x32u m_lo = bit_cast<vec4x32u>(tmp1_lo);
            vec4x32u m_hi = bit_cast<vec4x32u>(tmp1_hi << 32);

            vec4x32u m = m_lo | m_hi;
            m += vec4x32u{1};
            return m;
        }

    };

    AVEL_FINL vec4x32u& operator/=(vec4x32u& lhs, Denom4x32u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec4x32u& operator%=(vec4x32u& lhs, Denom4x32u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR4X32U_HPP
