#ifndef AVEL_DENOMINATOR32X16U_HPP
#define AVEL_DENOMINATOR32X16U_HPP

namespace avel {
    
    using Denom32x16u = Denominator<vec32x16u>;
    
    template<>
    class alignas(64) Denominator<vec32x16u> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec32x16u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom16u denom):
            m(denom.m),
            sh1(denom.sh1),
            sh2(denom.sh2),
            d(denom.d) {}

        explicit Denominator(vec32x16u d):
            Denominator(d, vec32x16u{16} - countl_zero(d - vec32x16u{1})) {}

    private:

        explicit Denominator(vec32x16u d, vec32x16u l):
            m(compute_m(l, d)),
            sh1(min(l, vec32x16u{1})),
            sh2(l - vec32x16u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec32x16u> div(vec32x16u n, Denominator denom) {
            vec32x16u t1 = mulhi(denom.m, n);

            vec32x16u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec32x16u q = (t1 + t0) >> denom.sh2;

            vec32x16u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec32x16u operator/(vec32x16u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec32x16u operator%(vec32x16u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec32x16u m{};
        mask32x16u sh1{};
        vec32x16u sh2{};
        vec32x16u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec32x16u mulhi(vec32x16u x, vec32x16u y) {
            #if defined(AVEL_AVX2)
            auto ret = _mm512_mulhi_epu16(decay(x), decay(y));
            return vec32x16u{ret};

            #endif
        }

        static vec32x16u compute_m(vec32x16u l, vec32x16u d) {
            //TODO: Optimize for case where vec32x16u is available
            //TODO: Use conversion functions once implemented instead
            vec16x32u l_lo = bit_cast<vec16x32u>(l) & vec16x32u{0x0000FFFF};
            vec16x32u l_hi = bit_cast<vec16x32u>(l) >> 16;

            vec16x32u d_lo = bit_cast<vec16x32u>(d) & vec16x32u{0x0000FFFF};
            vec16x32u d_hi = bit_cast<vec16x32u>(d) >> 16;

            vec16x32u tmp0_lo = (vec16x32u{1} << l_lo) - d_lo;
            vec16x32u tmp0_hi = (vec16x32u{1} << l_hi) - d_hi;

            vec16x32u tmp1_lo = (tmp0_lo << 16) / d_lo;
            vec16x32u tmp1_hi = (tmp0_hi << 16) / d_hi;

            vec32x16u m_lo = bit_cast<vec32x16u>(tmp1_lo);
            vec32x16u m_hi = bit_cast<vec32x16u>(tmp1_hi << 16);

            vec32x16u m = m_lo | m_hi;
            m += vec32x16u{1};
            return m;
        }
        
    };

    AVEL_FINL vec32x16u& operator/=(vec32x16u& lhs, Denom32x16u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec32x16u& operator%=(vec32x16u& lhs, Denom32x16u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR32X16U_HPP
