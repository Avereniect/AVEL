#ifndef AVEL_DENOMINATOR16X16U_HPP
#define AVEL_DENOMINATOR16X16U_HPP

namespace avel {
    
    using Denom16x16u = Denominator<vec16x16u>;
    
    template<>
    class alignas(64) Denominator<vec16x16u> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec16x16u;

        //=================================================
        // -ctors
        //=================================================

        // TODO: Uncomment and fix
        //explicit Denominator(Denom16u denom):
        //    m(denom.m),
        //    sh1(denom.sh1),
        //    sh2(denom.sh2),
        //    d(denom.d) {}

        explicit Denominator(vec16x16u d):
            Denominator(d, vec16x16u{16} - countl_zero(d - vec16x16u{1})) {}

    private:

        explicit Denominator(vec16x16u d, vec16x16u l):
            m(compute_m(l, d)),
            sh1(min(l, vec16x16u{1})),
            sh2(l - vec16x16u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec16x16u> div(vec16x16u n, Denominator denom) {
            vec16x16u t1 = mulhi(denom.m, n);

            vec16x16u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec16x16u q = (t1 + t0) >> denom.sh2;

            vec16x16u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x16u operator/(vec16x16u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec16x16u operator%(vec16x16u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec16x16u m{};
        mask16x16u sh1{};
        vec16x16u sh2{};
        vec16x16u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec16x16u mulhi(vec16x16u x, vec16x16u y) {
            #if defined(AVEL_AVX2)
            auto ret = _mm256_mulhi_epu16(decay(x), decay(y));
            return vec16x16u{ret};

            #endif
        }

        static vec16x16u compute_m(vec16x16u l, vec16x16u d) {
            //TODO: Optimize for case where vec16x16u is available
            //TODO: Use conversion functions once implemented instead
            vec8x32u l_lo = bit_cast<vec8x32u>(l) & vec8x32u{0x0000FFFF};
            vec8x32u l_hi = bit_cast<vec8x32u>(l) >> 16;

            vec8x32u d_lo = bit_cast<vec8x32u>(d) & vec8x32u{0x0000FFFF};
            vec8x32u d_hi = bit_cast<vec8x32u>(d) >> 16;

            vec8x32u tmp0_lo = (vec8x32u{1} << l_lo) - d_lo;
            vec8x32u tmp0_hi = (vec8x32u{1} << l_hi) - d_hi;

            vec8x32u tmp1_lo = (tmp0_lo << 16) / d_lo;
            vec8x32u tmp1_hi = (tmp0_hi << 16) / d_hi;

            vec16x16u m_lo = bit_cast<vec16x16u>(tmp1_lo);
            vec16x16u m_hi = bit_cast<vec16x16u>(tmp1_hi << 16);

            vec16x16u m = m_lo | m_hi;
            m += vec16x16u{1};
            return m;
        }
        
    };

    AVEL_FINL vec16x16u& operator/=(vec16x16u& lhs, Denom16x16u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec16x16u& operator%=(vec16x16u& lhs, Denom16x16u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR16X16U_HPP
