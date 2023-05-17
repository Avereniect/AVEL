#ifndef AVEL_DENOMINATOR8X64U_HPP
#define AVEL_DENOMINATOR8X64U_HPP

namespace avel {

    using Denom8x64u = Denominator<vec8x64u>;

    template<>
    class Denominator<vec8x64u> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec8x64u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom64u denom):
            m(denom.m),
            sh2(denom.sh2),
            d(denom.d) {}

        explicit Denominator(vec8x64u d):
            Denominator(d, vec8x64u{64} - countl_zero(d - vec8x64u{1})) {}

    private:

        explicit Denominator(vec8x64u d, vec8x64u l):
            m(compute_m(l, d)),
            sh1(min(l, vec8x64u{1})),
            sh2(l - vec8x64u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec8x64u> div(vec8x64u n, Denominator denom) {
            vec8x64u t1 = mulhi(denom.m, n);

            #if defined(AVEL_AVX2)
            vec8x64u q = (t1 + ((n - t1) >> denom.sh1)) >> denom.sh2;

            #elif defined(AVEL_SSE2)
            vec8x64u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec8x64u q = (t1 + t0) >> denom.sh2;

            #endif

            #if defined(AVEL_NEON)
            vec8x64u t0 = (n - t1);
            t0 = blend(denom.sh1, t0 >> 1, t0);
            vec8x64u q = (t1 + t0) >> denom.sh2;
            #endif

            vec8x64u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x64u operator/(vec8x64u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x64u operator%(vec8x64u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec8x64u m{};

        #if defined(AVEL_AVX2)
        vec8x64u sh1{};
        #else
        mask2x64u sh1{};
        #endif

        vec8x64u sh2{};
        vec8x64u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec8x64u mulhi(vec8x64u x, vec8x64u y) {
            #if defined(AVEL_AVX2) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICX))
            std::uint64_t x0 = extract<0>(x);
            std::uint64_t x1 = extract<1>(x);
            std::uint64_t x2 = extract<2>(x);
            std::uint64_t x3 = extract<3>(x);
            std::uint64_t x4 = extract<4>(x);
            std::uint64_t x5 = extract<5>(x);
            std::uint64_t x6 = extract<6>(x);
            std::uint64_t x7 = extract<7>(x);

            std::uint64_t y0 = extract<0>(y);
            std::uint64_t y1 = extract<1>(y);
            std::uint64_t y2 = extract<2>(y);
            std::uint64_t y3 = extract<3>(y);
            std::uint64_t y4 = extract<4>(y);
            std::uint64_t y5 = extract<5>(y);
            std::uint64_t y6 = extract<6>(y);
            std::uint64_t y7 = extract<7>(y);

            // Implementation should leverage x86's 64x64->128-bit multiplication
            std::uint64_t ret0 = (__uint128_t(x0) * __uint128_t(y0)) >> 64;
            std::uint64_t ret1 = (__uint128_t(x1) * __uint128_t(y1)) >> 64;
            std::uint64_t ret2 = (__uint128_t(x2) * __uint128_t(y2)) >> 64;
            std::uint64_t ret3 = (__uint128_t(x3) * __uint128_t(y3)) >> 64;
            std::uint64_t ret4 = (__uint128_t(x4) * __uint128_t(y4)) >> 64;
            std::uint64_t ret5 = (__uint128_t(x5) * __uint128_t(y5)) >> 64;
            std::uint64_t ret6 = (__uint128_t(x6) * __uint128_t(y6)) >> 64;
            std::uint64_t ret7 = (__uint128_t(x7) * __uint128_t(y7)) >> 64;

            return vec8x64u{_mm512_set_epi64(ret7, ret6, ret5, ret4, ret3, ret2, ret1, ret0)};

            #endif
        }

        static vec8x64u compute_m(vec8x64u l, vec8x64u d) {
            #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICX)
            __uint128_t l0 = extract<0>(l);
            __uint128_t l1 = extract<1>(l);
            __uint128_t l2 = extract<2>(l);
            __uint128_t l3 = extract<3>(l);
            __uint128_t l4 = extract<4>(l);
            __uint128_t l5 = extract<5>(l);
            __uint128_t l6 = extract<6>(l);
            __uint128_t l7 = extract<7>(l);

            __uint128_t d0 = extract<0>(d);
            __uint128_t d1 = extract<1>(d);
            __uint128_t d2 = extract<2>(d);
            __uint128_t d3 = extract<3>(d);
            __uint128_t d4 = extract<4>(d);
            __uint128_t d5 = extract<5>(d);
            __uint128_t d6 = extract<6>(d);
            __uint128_t d7 = extract<7>(d);

            __uint128_t t00 = (__uint128_t{1} << l0) - d0;
            __uint128_t t01 = (__uint128_t{1} << l1) - d1;
            __uint128_t t02 = (__uint128_t{1} << l2) - d2;
            __uint128_t t03 = (__uint128_t{1} << l3) - d3;
            __uint128_t t04 = (__uint128_t{1} << l4) - d4;
            __uint128_t t05 = (__uint128_t{1} << l5) - d5;
            __uint128_t t06 = (__uint128_t{1} << l6) - d6;
            __uint128_t t07 = (__uint128_t{1} << l7) - d7;

            __uint128_t t10 = (t00 << 64) / d0;
            __uint128_t t11 = (t01 << 64) / d1;
            __uint128_t t12 = (t02 << 64) / d2;
            __uint128_t t13 = (t03 << 64) / d3;
            __uint128_t t14 = (t04 << 64) / d4;
            __uint128_t t15 = (t05 << 64) / d5;
            __uint128_t t16 = (t06 << 64) / d6;
            __uint128_t t17 = (t07 << 64) / d7;

            vec8x64u m{{
                static_cast<std::uint64_t>(t10),
                static_cast<std::uint64_t>(t11),
                static_cast<std::uint64_t>(t12),
                static_cast<std::uint64_t>(t13),
                static_cast<std::uint64_t>(t14),
                static_cast<std::uint64_t>(t15),
                static_cast<std::uint64_t>(t16),
                static_cast<std::uint64_t>(t17),
            }};
            m += vec8x64u{1};
            return m;
            #endif
        }

    };

    AVEL_FINL vec8x64u& operator/=(vec8x64u& lhs, Denom8x64u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec8x64u& operator%=(vec8x64u& lhs, Denom8x64u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR8X64U_HPP
