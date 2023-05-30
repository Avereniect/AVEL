#ifndef AVEL_DENOMINATOR4X64I_HPP
#define AVEL_DENOMINATOR4X64I_HPP

namespace avel {

    using Denom4x64i = Denominator<vec4x64i>;

    template<>
    class Denominator<vec4x64i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec4x64i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8i d):
            mp(d.mp),
            d_sign(d.d_sign),
            sh(d.sh),
            d(d.d) {}

        explicit Denominator(vec4x64i d):
            Denominator(d, max(vec4x64i{64} - countl_zero(abs(d) - vec4x64i{1}), vec4x64i{1})) {}

    private:

        explicit Denominator(vec4x64i d, vec4x64i l):
            mp(compute_mp(l, d)),
            d_sign(d >> 63),
            sh(l - vec4x64i{1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec4x64i> div(vec4x64i n, Denominator denom) {
            vec4x64i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 63);
            vec4x64i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec4x64i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec4x64i operator/(vec4x64i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec4x64i operator%(vec4x64i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec4x64i mp{};
        vec4x64i d_sign{};
        vec4x64i sh{};
        vec4x64i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec4x64i mulhi(vec4x64i x, vec4x64i y) {
            #if defined(AVEL_AVX2)
            std::int64_t x0 = extract<0>(x);
            std::int64_t x1 = extract<1>(x);
            std::int64_t x2 = extract<2>(x);
            std::int64_t x3 = extract<3>(x);

            std::int64_t y0 = extract<0>(y);
            std::int64_t y1 = extract<1>(y);
            std::int64_t y2 = extract<2>(y);
            std::int64_t y3 = extract<3>(y);

            // Implementation should leverage x86's 64x64->128-bit multiplication
            std::int64_t ret0 = (__int128_t(x0) * __int128_t(y0)) >> 64;
            std::int64_t ret1 = (__int128_t(x1) * __int128_t(y1)) >> 64;
            std::int64_t ret2 = (__int128_t(x2) * __int128_t(y2)) >> 64;
            std::int64_t ret3 = (__int128_t(x3) * __int128_t(y3)) >> 64;

            return vec4x64i{_mm256_set_epi64x(ret3, ret2, ret1, ret0)};

            #endif
        }

        static vec4x64i compute_mp(vec4x64i l, vec4x64i d) {
            //TODO: Optimize
            __int128_t l0 = extract<0>(l);
            __int128_t l1 = extract<1>(l);
            __int128_t l2 = extract<2>(l);
            __int128_t l3 = extract<3>(l);

            d = abs(d);
            __int128_t d0 = extract<0>(d);
            __int128_t d1 = extract<1>(d);
            __int128_t d2 = extract<2>(d);
            __int128_t d3 = extract<3>(d);

            auto t00 = __int128_t(0x8000000000000000) << l0;
            auto t01 = __int128_t(0x8000000000000000) << l1;
            auto t02 = __int128_t(0x8000000000000000) << l2;
            auto t03 = __int128_t(0x8000000000000000) << l3;

            auto t10 = t00 / d0;
            auto t11 = t01 / d1;
            auto t12 = t02 / d2;
            auto t13 = t03 / d3;

            auto t20 = t10 - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto t21 = t11 - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto t22 = t12 - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto t23 = t13 - __int128_t(0xFFFFFFFFFFFFFFFF);

            return vec4x64i{{
                static_cast<std::int64_t>(t20),
                static_cast<std::int64_t>(t21),
                static_cast<std::int64_t>(t22),
                static_cast<std::int64_t>(t23)
            }};
        }

    };

    AVEL_FINL vec4x64i& operator/=(vec4x64i& lhs, Denom4x64i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec4x64i& operator%=(vec4x64i& lhs, Denom4x64i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR4X64I_HPP
