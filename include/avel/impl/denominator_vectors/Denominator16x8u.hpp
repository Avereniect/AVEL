#ifndef AVEL_DENOMINATOR16X8U_HPP
#define AVEL_DENOMINATOR16X8U_HPP

namespace avel {
    
    using Denom16x8u = Denominator<vec16x8u>;
    
    template<>
    class Denominator<vec16x8u> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec16x8u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8u denom):
            m(denom.m),
            sh2(denom.sh2),
            d(denom.d) {}

        explicit Denominator(vec16x8u d):
            Denominator(d, vec16x8u{8} - countl_zero(d - vec16x8u{1})) {}

    private:

        explicit Denominator(vec16x8u d, vec16x8u l):
            m(compute_m(l, d)),
            sh1(min(l, vec16x8u{1})),
            sh2(l - vec16x8u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec16x8u> div(vec16x8u n, Denominator denom) {
            vec16x8u t1 = mulhi(denom.m, n);

            vec16x8u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec16x8u q = (t1 + t0) >> denom.sh2;

            vec16x8u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x8u operator/(vec16x8u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x8u operator%(vec16x8u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec16x8u m{};
        mask16x8u sh1{};
        vec16x8u sh2{};
        vec16x8u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec16x8u mulhi(vec16x8u x, vec16x8u y) {
            #if defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto x_lo = _mm_unpacklo_epi8(decay(x), zeros);
            auto x_hi = _mm_unpackhi_epi8(decay(x), zeros);

            auto y_lo = _mm_unpacklo_epi8(decay(y), zeros);
            auto y_hi = _mm_unpackhi_epi8(decay(y), zeros);

            auto p_lo = _mm_mullo_epi16(x_lo, y_lo);
            auto p_hi = _mm_mullo_epi16(x_hi, y_hi);

            auto ret_lo = _mm_srli_epi16(p_lo, 8);
            auto ret_hi = _mm_srli_epi16(p_hi, 8);

            auto ret = _mm_packus_epi16(ret_lo, ret_hi);
            return vec16x8u{ret};

            #endif

            #if defined(AVEL_NEON)
            auto x_half0 = vget_low_u8(decay(x));
            auto x_half1 = vget_high_u8(decay(x));

            auto y_half0 = vget_low_u8(decay(y));
            auto y_half1 = vget_high_u8(decay(y));

            auto prod_half0 = vreinterpretq_u8_u16(vmull_u8(x_half0, y_half0));
            auto prod_half1 = vreinterpretq_u8_u16(vmull_u8(x_half1, y_half1));

            auto prod = vuzpq_u8(prod_half0, prod_half1).val[1];

            return vec16x8u{prod};

            #endif
        }

        static vec16x8u compute_m(vec16x8u l, vec16x8u d) {
            //TODO: Optimize for case where vec16x16u is available
            //TODO: Use conversion functions once implemented instead
            vec8x16u l_lo = bit_cast<vec8x16u>(l) & vec8x16u{0x00FF};
            vec8x16u l_hi = bit_cast<vec8x16u>(l) >> 8;

            vec8x16u d_lo = bit_cast<vec8x16u>(d) & vec8x16u{0x00FF};
            vec8x16u d_hi = bit_cast<vec8x16u>(d) >> 8;

            vec8x16u tmp0_lo = (vec8x16u{1} << l_lo) - d_lo;
            vec8x16u tmp0_hi = (vec8x16u{1} << l_hi) - d_hi;

            vec8x16u tmp1_lo = (tmp0_lo << 8) / d_lo;
            vec8x16u tmp1_hi = (tmp0_hi << 8) / d_hi;

            vec16x8u m_lo = bit_cast<vec16x8u>(tmp1_lo);
            vec16x8u m_hi = bit_cast<vec16x8u>(tmp1_hi << 8);

            vec16x8u m = m_lo | m_hi;
            m += vec16x8u{1};
            return m;
        }
        
    };

    AVEL_FINL vec16x8u& operator/=(vec16x8u& lhs, Denom16x8u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec16x8u& operator%=(vec16x8u& lhs, Denom16x8u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR16X8U_HPP
