#ifndef AVEL_DENOMINATOR32X8U_HPP
#define AVEL_DENOMINATOR32X8U_HPP

namespace avel {

    using Denom32x8u = Denominator<vec32x8u>;

    template<>
    class alignas(128) Denominator<vec32x8u> {
    public:

        template<class U>
        friend class Denominator;


        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec32x8u;

        //=================================================
        // -ctors
        //=================================================

        // TODO: Uncomment and fix
        //explicit Denominator(Denom8u denom):
        //    m(denom.m),
        //    sh1(denom.sh1),
        //    sh2(denom.sh2),
        //    d(denom.d) {}

        explicit Denominator(vec32x8u d):
            Denominator(d, bit_width(d - vec32x8u{1})) {}

    private:

        explicit Denominator(vec32x8u d, vec32x8u l):
            m(compute_m(l, d)),
            sh1(min(l, vec32x8u{1})),
            sh2(l - vec32x8u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec32x8u> div(vec32x8u n, Denominator denom) {
            vec32x8u t1 = mulhi(denom.m, n);

            vec32x8u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec32x8u q = (t1 + t0) >> denom.sh2;

            vec32x8u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec32x8u operator/(vec32x8u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec32x8u operator%(vec32x8u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec32x8u m{};
        mask32x8u sh1{};
        vec32x8u sh2{};
        vec32x8u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec32x8u mulhi(vec32x8u x, vec32x8u y) {
            #if defined(AVEL_SSE2)
            auto zeros = _mm256_setzero_si256();

            auto x_lo = _mm256_unpacklo_epi8(decay(x), zeros);
            auto x_hi = _mm256_unpackhi_epi8(decay(x), zeros);

            auto y_lo = _mm256_unpacklo_epi8(decay(y), zeros);
            auto y_hi = _mm256_unpackhi_epi8(decay(y), zeros);

            auto p_lo = _mm256_mullo_epi16(x_lo, y_lo);
            auto p_hi = _mm256_mullo_epi16(x_hi, y_hi);

            auto ret_lo = _mm256_srli_epi16(p_lo, 8);
            auto ret_hi = _mm256_srli_epi16(p_hi, 8);

            auto ret = _mm256_packus_epi16(ret_lo, ret_hi);
            return vec32x8u{ret};

            #endif
        }

        static vec32x8u compute_m(vec32x8u l, vec32x8u d) {
            //TODO: Optimize for case where wider registers are available
            vec16x16u l_lo = bit_cast<vec16x16u>(l) & vec16x16u{0x00FF};
            vec16x16u l_hi = bit_cast<vec16x16u>(l) >> 8;

            vec16x16u d_lo = bit_cast<vec16x16u>(d) & vec16x16u{0x00FF};
            vec16x16u d_hi = bit_cast<vec16x16u>(d) >> 8;

            vec16x16u tmp0_lo = (vec16x16u{1} << l_lo) - d_lo;
            vec16x16u tmp0_hi = (vec16x16u{1} << l_hi) - d_hi;

            vec16x16u tmp1_lo = (tmp0_lo << 8) / d_lo;
            vec16x16u tmp1_hi = (tmp0_hi << 8) / d_hi;

            vec32x8u m_lo = bit_cast<vec32x8u>(tmp1_lo);
            vec32x8u m_hi = bit_cast<vec32x8u>(tmp1_hi << 8);

            vec32x8u m = m_lo | m_hi;
            m += vec32x8u{1};
            return m;
        }

    };

    AVEL_FINL vec32x8u& operator/=(vec32x8u& lhs, Denom32x8u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec32x8u& operator%=(vec32x8u& lhs, Denom32x8u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR32X8U_HPP
