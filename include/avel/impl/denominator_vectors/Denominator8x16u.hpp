#ifndef AVEL_DENOMINATOR8X16U_HPP
#define AVEL_DENOMINATOR8X16U_HPP

namespace avel {
    
    using Denom8x16u = Denominator<vec8x16u>;
    
    template<>
    class alignas(64) Denominator<vec8x16u> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec8x16u;

        //=================================================
        // -ctors
        //=================================================

        // TODO: Uncomment and fix
        //explicit Denominator(Denom16u denom):
        //    m(denom.m),
        //    sh1(denom.sh1),
        //    sh2(denom.sh2),
        //    d(denom.d) {}

        explicit Denominator(vec8x16u d):
            Denominator(d, vec8x16u{16} - countl_zero(d - vec8x16u{1})) {}

    private:

        explicit Denominator(vec8x16u d, vec8x16u l):
            m(compute_m(l, d)),
            sh1(min(l, vec8x16u{1})),
            sh2(l - vec8x16u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec8x16u> div(vec8x16u n, Denominator denom) {
            vec8x16u t1 = mulhi(denom.m, n);

            vec8x16u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec8x16u q = (t1 + t0) >> denom.sh2;

            vec8x16u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x16u operator/(vec8x16u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x16u operator%(vec8x16u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec8x16u m{};
        mask8x16u sh1{};
        vec8x16u sh2{};
        vec8x16u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec8x16u mulhi(vec8x16u x, vec8x16u y) {
            #if defined(AVEL_SSE2)
            auto ret = _mm_mulhi_epu16(decay(x), decay(y));
            return vec8x16u{ret};

            #endif

            #if defined(AVEL_NEON)
            auto x_half0 = vget_low_u16(decay(x));
            auto x_half1 = vget_high_u16(decay(x));

            auto y_half0 = vget_low_u16(decay(y));
            auto y_half1 = vget_high_u16(decay(y));

            auto prod_half0 = vreinterpretq_u16_u32(vmull_u16(x_half0, y_half0));
            auto prod_half1 = vreinterpretq_u16_u32(vmull_u16(x_half1, y_half1));

            auto prod = vuzpq_u16(prod_half0, prod_half1).val[1];

            return vec8x16u{prod};

            #endif
        }

        static vec8x16u compute_m(vec8x16u l, vec8x16u d) {
            //TODO: Optimize for case where vec16x16u is available
            //TODO: Use conversion functions once implemented instead
            vec4x32u l_lo = bit_cast<vec4x32u>(l) & vec4x32u{0x0000FFFF};
            vec4x32u l_hi = bit_cast<vec4x32u>(l) >> 16;

            vec4x32u d_lo = bit_cast<vec4x32u>(d) & vec4x32u{0x0000FFFF};
            vec4x32u d_hi = bit_cast<vec4x32u>(d) >> 16;

            vec4x32u tmp0_lo = (vec4x32u{1} << l_lo) - d_lo;
            vec4x32u tmp0_hi = (vec4x32u{1} << l_hi) - d_hi;

            vec4x32u tmp1_lo = (tmp0_lo << 16) / d_lo;
            vec4x32u tmp1_hi = (tmp0_hi << 16) / d_hi;

            vec8x16u m_lo = bit_cast<vec8x16u>(tmp1_lo);
            vec8x16u m_hi = bit_cast<vec8x16u>(tmp1_hi << 16);

            vec8x16u m = m_lo | m_hi;
            m += vec8x16u{1};
            return m;
        }
        
    };

    AVEL_FINL vec8x16u& operator/=(vec8x16u& lhs, Denom8x16u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec8x16u& operator%=(vec8x16u& lhs, Denom8x16u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR8X16U_HPP
