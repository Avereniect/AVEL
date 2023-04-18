#ifndef AVEL_DENOMINATOR8X64I_HPP
#define AVEL_DENOMINATOR8X64I_HPP

namespace avel {

    using Denom8x64i = Denominator<vec8x64i>;

    template<>
    class alignas(128) Denominator<vec8x64i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec8x64i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8i d):
            mp(d.mp),
            d_sign(d.d_sign),
            sh(d.sh),
            d(d.d) {}

        explicit Denominator(vec8x64i d):
            Denominator(d, max(vec8x64i{64} - countl_zero(abs(d) - vec8x64i{1}), vec8x64i{1})) {}

    private:

        explicit Denominator(vec8x64i d, vec8x64i l):
            mp(compute_mp(l, d)),
            d_sign(d >> 63),
            sh(l - vec8x64i{1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec8x64i> div(vec8x64i n, Denominator denom) {
            vec8x64i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 63);
            vec8x64i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec8x64i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x64i operator/(vec8x64i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec8x64i operator%(vec8x64i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec8x64i mp{};
        vec8x64i d_sign{};
        vec8x64i sh{};
        vec8x64i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec8x64i mulhi(vec8x64i x, vec8x64i y) {
            #if defined(AVEL_AVX512F)
            std::int64_t x0 = extract<0>(x);
            std::int64_t x1 = extract<1>(x);
            std::int64_t x2 = extract<2>(x);
            std::int64_t x3 = extract<3>(x);
            std::int64_t x4 = extract<4>(x);
            std::int64_t x5 = extract<5>(x);
            std::int64_t x6 = extract<6>(x);
            std::int64_t x7 = extract<7>(x);

            std::int64_t y0 = extract<0>(y);
            std::int64_t y1 = extract<1>(y);
            std::int64_t y2 = extract<2>(y);
            std::int64_t y3 = extract<3>(y);
            std::int64_t y4 = extract<4>(y);
            std::int64_t y5 = extract<5>(y);
            std::int64_t y6 = extract<6>(y);
            std::int64_t y7 = extract<7>(y);

            // Implementation should leverage x86's 64x64->128-bit multiplication
            std::int64_t ret0 = (__int128_t(x0) * __int128_t(y0)) >> 64;
            std::int64_t ret1 = (__int128_t(x1) * __int128_t(y1)) >> 64;
            std::int64_t ret2 = (__int128_t(x2) * __int128_t(y2)) >> 64;
            std::int64_t ret3 = (__int128_t(x3) * __int128_t(y3)) >> 64;
            std::int64_t ret4 = (__int128_t(x4) * __int128_t(y4)) >> 64;
            std::int64_t ret5 = (__int128_t(x5) * __int128_t(y5)) >> 64;
            std::int64_t ret6 = (__int128_t(x6) * __int128_t(y6)) >> 64;
            std::int64_t ret7 = (__int128_t(x7) * __int128_t(y7)) >> 64;

            return vec8x64i{_mm512_set_epi64(ret7, ret6, ret5, ret4, ret3, ret2, ret1, ret0)};

            #endif
        }

        static vec8x64i compute_mp(vec8x64i l, vec8x64i d) {
            //TODO: Optimize
            __int128_t l0 = extract<0>(l);
            __int128_t l1 = extract<1>(l);
            __int128_t l2 = extract<2>(l);
            __int128_t l3 = extract<3>(l);
            __int128_t l4 = extract<4>(l);
            __int128_t l5 = extract<5>(l);
            __int128_t l6 = extract<6>(l);
            __int128_t l7 = extract<7>(l);

            d = abs(d);
            __int128_t d0 = extract<0>(d);
            __int128_t d1 = extract<1>(d);
            __int128_t d2 = extract<2>(d);
            __int128_t d3 = extract<3>(d);
            __int128_t d4 = extract<4>(d);
            __int128_t d5 = extract<5>(d);
            __int128_t d6 = extract<6>(d);
            __int128_t d7 = extract<7>(d);

            auto t00 = __int128_t(0x8000000000000000) << l0;
            auto t01 = __int128_t(0x8000000000000000) << l1;
            auto t02 = __int128_t(0x8000000000000000) << l2;
            auto t03 = __int128_t(0x8000000000000000) << l3;
            auto t04 = __int128_t(0x8000000000000000) << l4;
            auto t05 = __int128_t(0x8000000000000000) << l5;
            auto t06 = __int128_t(0x8000000000000000) << l6;
            auto t07 = __int128_t(0x8000000000000000) << l7;

            auto t10 = t00 / d0;
            auto t11 = t01 / d1;
            auto t12 = t02 / d2;
            auto t13 = t03 / d3;
            auto t14 = t04 / d4;
            auto t15 = t05 / d5;
            auto t16 = t06 / d6;
            auto t17 = t07 / d7;

            auto t20 = t10 - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto t21 = t11 - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto t22 = t12 - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto t23 = t13 - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto t24 = t14 - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto t25 = t15 - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto t26 = t16 - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto t27 = t17 - __int128_t(0xFFFFFFFFFFFFFFFF);

            return vec8x64i{{
                static_cast<std::int64_t>(t20),
                static_cast<std::int64_t>(t21),
                static_cast<std::int64_t>(t22),
                static_cast<std::int64_t>(t23),
                static_cast<std::int64_t>(t24),
                static_cast<std::int64_t>(t25),
                static_cast<std::int64_t>(t26),
                static_cast<std::int64_t>(t27),
            }};
        }

    };

    AVEL_FINL vec8x64i& operator/=(vec8x64i& lhs, Denom8x64i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec8x64i& operator%=(vec8x64i& lhs, Denom8x64i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR8X64I_HPP
