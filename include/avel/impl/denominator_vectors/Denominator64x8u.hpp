#ifndef AVEL_DENOMINATOR64X8U_HPP
#define AVEL_DENOMINATOR64X8U_HPP

namespace avel {
    
    using Denom64x8u = Denominator<vec64x8u>;
    
    template<>
    class alignas(64) Denominator<vec64x8u> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec64x8u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8u denom):
            m(denom.m),
            sh1(denom.sh1),
            sh2(denom.sh2),
            d(denom.d) {}

        explicit Denominator(vec64x8u d):
            Denominator(d, vec64x8u{8} - countl_zero(d - vec64x8u{1})) {}

    private:

        explicit Denominator(vec64x8u d, vec64x8u l):
            m(compute_m(l, d)),
            sh1(min(l, vec64x8u{1})),
            sh2(l - vec64x8u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec64x8u> div(vec64x8u n, Denominator denom) {
            vec64x8u t1 = mulhi(denom.m, n);

            vec64x8u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec64x8u q = (t1 + t0) >> denom.sh2;

            vec64x8u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec64x8u operator/(vec64x8u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec64x8u operator%(vec64x8u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec64x8u m{};
        mask64x8u sh1{};
        vec64x8u sh2{};
        vec64x8u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec64x8u mulhi(vec64x8u x, vec64x8u y) {
            #if defined(AVEL_SSE2)
            auto zeros = _mm512_setzero_si512();

            auto x_lo = _mm512_unpacklo_epi8(decay(x), zeros);
            auto x_hi = _mm512_unpackhi_epi8(decay(x), zeros);

            auto y_lo = _mm512_unpacklo_epi8(decay(y), zeros);
            auto y_hi = _mm512_unpackhi_epi8(decay(y), zeros);

            auto p_lo = _mm512_mullo_epi16(x_lo, y_lo);
            auto p_hi = _mm512_mullo_epi16(x_hi, y_hi);

            auto ret_lo = _mm512_srli_epi16(p_lo, 8);
            auto ret_hi = _mm512_srli_epi16(p_hi, 8);

            auto ret = _mm512_packus_epi16(ret_lo, ret_hi);
            return vec64x8u{ret};

            #endif
        }

        static vec64x8u compute_m(vec64x8u l, vec64x8u d) {
            //TODO: Optimize
            vec32x16u l_lo = bit_cast<vec32x16u>(l) & vec32x16u{0x00FF};
            vec32x16u l_hi = bit_cast<vec32x16u>(l) >> 8;

            vec32x16u d_lo = bit_cast<vec32x16u>(d) & vec32x16u{0x00FF};
            vec32x16u d_hi = bit_cast<vec32x16u>(d) >> 8;

            vec32x16u tmp0_lo = (vec32x16u{1} << l_lo) - d_lo;
            vec32x16u tmp0_hi = (vec32x16u{1} << l_hi) - d_hi;

            vec32x16u tmp1_lo = (tmp0_lo << 8) / d_lo;
            vec32x16u tmp1_hi = (tmp0_hi << 8) / d_hi;

            vec64x8u m_lo = bit_cast<vec64x8u>(tmp1_lo);
            vec64x8u m_hi = bit_cast<vec64x8u>(tmp1_hi << 8);

            vec64x8u m = m_lo | m_hi;
            m += vec64x8u{1};
            return m;
        }
        
    };

    AVEL_FINL vec64x8u& operator/=(vec64x8u& lhs, Denom64x8u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec64x8u& operator%=(vec64x8u& lhs, Denom64x8u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR64X8U_HPP
