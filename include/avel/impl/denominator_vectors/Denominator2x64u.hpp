#ifndef AVEL_DENOMINATOR2X64U_HPP
#define AVEL_DENOMINATOR2X64U_HPP

namespace avel {

    using Denom2x64u = Denominator<vec2x64u>;

    template<>
    class alignas(64) Denominator<vec2x64u> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom64u denom):
            m(denom.m),
            sh1(denom.sh1),
            sh2(denom.sh2),
            d(denom.d) {}

        explicit Denominator(vec2x64u d):
            Denominator(d, vec2x64u{64} - countl_zero(d - vec2x64u{1})) {}

    private:

        explicit Denominator(vec2x64u d, vec2x64u l):
            m(compute_m(l, d)),
            sh1(min(l, vec2x64u{1})),
            sh2(l - vec2x64u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec2x64u> div(vec2x64u n, Denominator denom) {
            vec2x64u t1 = mulhi(denom.m, n);

            #if defined(AVEL_SSE2)
            vec2x64u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec2x64u q = (t1 + t0) >> denom.sh2;

            #endif

            #if defined(AVEL_NEON)
            vec2x64u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec2x64u q = (t1 + t0) >> denom.sh2;
            #endif

            vec2x64u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec2x64u operator/(vec2x64u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec2x64u operator%(vec2x64u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec2x64u m{};

        #if defined(AVEL_AVX2)
        vec2x64u sh1{};
        #else
        mask2x64u sh1{};
        #endif

        vec2x64u sh2{};
        vec2x64u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec2x64u mulhi(vec2x64u x, vec2x64u y) {
            #if defined(AVEL_SSE2)
            std::uint64_t x_lo = extract<0>(x);
            std::uint64_t x_hi = extract<1>(x);

            std::uint64_t y_lo = extract<0>(y);
            std::uint64_t y_hi = extract<1>(y);

            // Implementation should leverage x86's 64x64->128-bit multiplication
            std::uint64_t ret_lo = (__uint128_t(x_lo) * __uint128_t(y_lo)) >> 64;
            std::uint64_t ret_hi = (__uint128_t(x_hi) * __uint128_t(y_hi)) >> 64;

            return vec2x64u{_mm_set_epi64x(ret_hi, ret_lo)};

            #endif

            #if defined(AVEL_NEON)
            //TODO: Implement
            auto x_half0 = vget_low_u32(decay(x));
            auto x_half1 = vget_high_u32(decay(x));

            auto y_half0 = vget_low_u32(decay(y));
            auto y_half1 = vget_high_u32(decay(y));

            auto prod_half0 = vreinterpretq_u32_u64(vmull_u32(x_half0, y_half0));
            auto prod_half1 = vreinterpretq_u32_u64(vmull_u32(x_half1, y_half1));

            auto prod = vuzpq_u32(prod_half0, prod_half1).val[1];

            return vec2x64u{prod};

            #endif
        }

        static vec2x64u compute_m(vec2x64u l, vec2x64u d) {
            //TODO: Use conversion functions once implemented instead
            __uint128_t l_lo = extract<0>(l);
            __uint128_t l_hi = extract<1>(l);

            __uint128_t d_lo = extract<0>(d);
            __uint128_t d_hi = extract<1>(d);

            __uint128_t tmp0_lo = (__uint128_t{1} << l_lo) - d_lo;
            __uint128_t tmp0_hi = (__uint128_t{1} << l_hi) - d_hi;

            __uint128_t tmp1_lo = (tmp0_lo << 64) / d_lo;
            __uint128_t tmp1_hi = (tmp0_hi << 64) / d_hi;

            vec2x64u m{{
                static_cast<std::uint64_t>(tmp1_lo),
                static_cast<std::uint64_t>(tmp1_hi),
           }};
            m += vec2x64u{1};
            return m;
        }

    };

    AVEL_FINL vec2x64u& operator/=(vec2x64u& lhs, Denom2x64u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec2x64u& operator%=(vec2x64u& lhs, Denom2x64u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR2X64U_HPP
