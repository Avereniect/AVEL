#ifndef AVEL_DENOMINATOR8X64I_HPP
#define AVEL_DENOMINATOR8X64I_HPP

namespace avel {

    using Denom8x64i = Denominator<vec8x64i>;

    template<>
    class Denominator<vec8x64i> {
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
            #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX)
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

            #elif defined(AVEL_MSVC)
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

            std::int64_t ret0 = __mulh(x0, y0);
            std::int64_t ret1 = __mulh(x1, y1);
            std::int64_t ret2 = __mulh(x2, y2);
            std::int64_t ret3 = __mulh(x3, y3);
            std::int64_t ret4 = __mulh(x4, y4);
            std::int64_t ret5 = __mulh(x5, y5);
            std::int64_t ret6 = __mulh(x6, y6);
            std::int64_t ret7 = __mulh(x7, y7);

            return vec8x64i{_mm512_set_epi64(ret7, ret6, ret5, ret4, ret3, ret2, ret1, ret0)};

            #endif
        }

        static vec8x64i compute_mp(vec8x64i l, vec8x64i d) {
            vec8x64i n = vec8x64i{1} << (l - vec8x64i{1});
            n = clear(vec8x64i{1} == d, n);

            d = avel::abs(d);

            auto quotient0 = div_64uhi_by_64u(extract<0>(n), extract<0>(d));
            auto quotient1 = div_64uhi_by_64u(extract<1>(n), extract<1>(d));
            auto quotient2 = div_64uhi_by_64u(extract<2>(n), extract<2>(d));
            auto quotient3 = div_64uhi_by_64u(extract<3>(n), extract<3>(d));
            auto quotient4 = div_64uhi_by_64u(extract<4>(n), extract<4>(d));
            auto quotient5 = div_64uhi_by_64u(extract<5>(n), extract<5>(d));
            auto quotient6 = div_64uhi_by_64u(extract<6>(n), extract<6>(d));
            auto quotient7 = div_64uhi_by_64u(extract<7>(n), extract<7>(d));

            vec8x64i ret{_mm512_set_epi64(
                quotient7, quotient6, quotient5, quotient4,
                quotient3, quotient2, quotient1, quotient0
            )};
            ret += vec8x64i{1};

            return ret;
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
