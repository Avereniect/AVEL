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
            #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX)
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

            #elif defined(AVEL_MSVC)
            std::int64_t x0 = extract<0>(x);
            std::int64_t x1 = extract<1>(x);
            std::int64_t x2 = extract<2>(x);
            std::int64_t x3 = extract<3>(x);

            std::int64_t y0 = extract<0>(y);
            std::int64_t y1 = extract<1>(y);
            std::int64_t y2 = extract<2>(y);
            std::int64_t y3 = extract<3>(y);

            std::int64_t ret0 = __mulh(x0, y0);
            std::int64_t ret1 = __mulh(x1, y1);
            std::int64_t ret2 = __mulh(x2, y2);
            std::int64_t ret3 = __mulh(x3, y3);

            return vec4x64i{_mm256_set_epi64x(ret3, ret2, ret1, ret0)};

            #endif
        }

        static vec4x64i compute_mp(vec4x64i l, vec4x64i d) {
            vec4x64i n = vec4x64i{1} << (l - vec4x64i{1});

            d = avel::abs(d);

            auto quotient0 = div_64uhi_by_64u(extract<0>(n), extract<0>(d));
            auto quotient1 = div_64uhi_by_64u(extract<1>(n), extract<1>(d));
            auto quotient2 = div_64uhi_by_64u(extract<2>(n), extract<2>(d));
            auto quotient3 = div_64uhi_by_64u(extract<3>(n), extract<3>(d));

            vec4x64i ret{_mm256_set_epi64x(quotient3, quotient2, quotient1, quotient0)};
            ret += vec4x64i{1};

            return ret;
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
