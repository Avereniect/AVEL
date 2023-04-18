#ifndef AVEL_DENOMINATOR4X64U_HPP
#define AVEL_DENOMINATOR4X64U_HPP

namespace avel {

    using Denom4x64u = Denominator<vec4x64u>;

    template<>
    class alignas(64) Denominator<vec4x64u> {
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
            sh1(denom.sh1),
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
            #if defined(AVEL_AVX2) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICX))
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

            #endif
        }

        static vec4x64u compute_m(vec4x64u l, vec4x64u d) {
            #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICX)
            __uint128_t l0 = extract<0>(l);
            __uint128_t l1 = extract<1>(l);
            __uint128_t l2 = extract<2>(l);
            __uint128_t l3 = extract<3>(l);

            __uint128_t d0 = extract<0>(d);
            __uint128_t d1 = extract<1>(d);
            __uint128_t d2 = extract<2>(d);
            __uint128_t d3 = extract<3>(d);

            __uint128_t t00 = (__uint128_t{1} << l0) - d0;
            __uint128_t t01 = (__uint128_t{1} << l1) - d1;
            __uint128_t t02 = (__uint128_t{1} << l2) - d2;
            __uint128_t t03 = (__uint128_t{1} << l3) - d3;

            __uint128_t t10 = (t00 << 64) / d0;
            __uint128_t t11 = (t01 << 64) / d1;
            __uint128_t t12 = (t02 << 64) / d2;
            __uint128_t t13 = (t03 << 64) / d3;

            vec4x64u m{{
                static_cast<std::uint64_t>(t10),
                static_cast<std::uint64_t>(t11),
                static_cast<std::uint64_t>(t12),
                static_cast<std::uint64_t>(t13),
            }};
            m += vec4x64u{1};
            return m;
            #endif
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
