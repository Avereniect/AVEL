#ifndef AVEL_DENOMINATOR4X64U_HPP
#define AVEL_DENOMINATOR4X64U_HPP

namespace avel {

    using Denom4x64u = Denominator<vec4x64u>;

    template<>
    class Denominator<vec4x64u> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec4x64u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom64u denom):
            m(denom.m),
            sh2(denom.sh2),
            d(denom.d) {}

        explicit Denominator(vec4x64u d):
            Denominator(d, vec4x64u{64} - countl_zero(d - vec4x64u{1})) {}

    private:

        explicit Denominator(vec4x64u d, vec4x64u l):
            m(compute_m(l, d)),
            sh1(min(l, vec4x64u{1})),
            sh2(l - vec4x64u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec4x64u> div(vec4x64u n, Denominator denom) {
            vec4x64u t1 = mulhi(denom.m, n);

            #if defined(AVEL_AVX2)
            vec4x64u q = (t1 + ((n - t1) >> denom.sh1)) >> denom.sh2;

            #elif defined(AVEL_SSE2)
            vec4x64u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec4x64u q = (t1 + t0) >> denom.sh2;

            #endif

            #if defined(AVEL_NEON)
            vec4x64u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec4x64u q = (t1 + t0) >> denom.sh2;
            #endif

            vec4x64u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec4x64u operator/(vec4x64u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec4x64u operator%(vec4x64u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec4x64u m{};

        #if defined(AVEL_AVX2)
        vec4x64u sh1{};
        #else
        mask2x64u sh1{};
        #endif

        vec4x64u sh2{};
        vec4x64u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec4x64u mulhi(vec4x64u x, vec4x64u y) {
            #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX)
            std::uint64_t x0 = extract<0>(x);
            std::uint64_t x1 = extract<1>(x);
            std::uint64_t x2 = extract<2>(x);
            std::uint64_t x3 = extract<3>(x);

            std::uint64_t y0 = extract<0>(y);
            std::uint64_t y1 = extract<1>(y);
            std::uint64_t y2 = extract<2>(y);
            std::uint64_t y3 = extract<3>(y);

            // Implementation should leverage x86's 64x64->128-bit multiplication
            std::uint64_t ret0 = (__uint128_t(x0) * __uint128_t(y0)) >> 64;
            std::uint64_t ret1 = (__uint128_t(x1) * __uint128_t(y1)) >> 64;
            std::uint64_t ret2 = (__uint128_t(x2) * __uint128_t(y2)) >> 64;
            std::uint64_t ret3 = (__uint128_t(x3) * __uint128_t(y3)) >> 64;

            return vec4x64u{_mm256_set_epi64x(ret3, ret2, ret1, ret0)};

            #elif defined(AVEL_MSVC)
            std::uint64_t x0 = extract<0>(x);
            std::uint64_t x1 = extract<1>(x);
            std::uint64_t x2 = extract<2>(x);
            std::uint64_t x3 = extract<3>(x);

            std::uint64_t y0 = extract<0>(y);
            std::uint64_t y1 = extract<1>(y);
            std::uint64_t y2 = extract<2>(y);
            std::uint64_t y3 = extract<3>(y);

            std::uint64_t ret0 = __umulh(x0, y0);
            std::uint64_t ret1 = __umulh(x1, y1);
            std::uint64_t ret2 = __umulh(x2, y2);
            std::uint64_t ret3 = __umulh(x3, y3);

            return vec4x64u{_mm256_set_epi64x(ret3, ret2, ret1, ret0)};

            #endif
        }

        static vec4x64u compute_m(vec4x64u l, vec4x64u d) {
            vec4x64u n = (vec4x64u{1} << l) - d;

            std::uint64_t quotient0 = div_64uhi_by_64u(avel::extract<0>(n), avel::extract<0>(d));
            std::uint64_t quotient1 = div_64uhi_by_64u(avel::extract<1>(n), avel::extract<1>(d));
            std::uint64_t quotient2 = div_64uhi_by_64u(avel::extract<2>(n), avel::extract<2>(d));
            std::uint64_t quotient3 = div_64uhi_by_64u(avel::extract<3>(n), avel::extract<3>(d));

            auto ret = vec4x64u{_mm256_set_epi64x(quotient3, quotient2, quotient1, quotient0)} + vec4x64u{1};
            return ret;
        }

    };

    AVEL_FINL vec4x64u& operator/=(vec4x64u& lhs, Denom4x64u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec4x64u& operator%=(vec4x64u& lhs, Denom4x64u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR4X64U_HPP
