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

        AVEL_FINL Denom8x64u& operator<<=(vec8x64u s) {
            auto effective_s = avel::max(avel::countl_zero(d), s);

            d <<= s;
            sh2 += effective_s;
            return *this;
        }

        AVEL_FINL Denom8x64u& operator>>=(vec8x64u s) {
            auto effective_s = avel::min(avel::countr_zero(d), s);

            d >>= s;
            sh2 -= effective_s;

            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom8x64u operator<<(vec8x64u s) const {
            Denom8x64u ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom8x64u operator>>(vec8x64u s) const {
            Denom8x64u ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec8x64u value() const {
            return d;
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
            #if defined(AVEL_AVX512F) && defined(AVEL_PCLMULQDQ)
            auto prod0 = _mm512_clmulepi64_epi128(decay(x), decay(y), 0x00);
            auto prod1 = _mm512_clmulepi64_epi128(decay(x), decay(y), 0x11);

            auto t1 = (x >> 63) & y;
            auto t2 = (y >> 63) & x;

            auto prod = _mm512_unpackhi_epi64(prod0, prod1);
            auto ret =  vec8x64u{prod} + t1 + t2;
            return ret;

            #elif defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX)
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

            std::int64_t ret0 = __umulh(x0, y0);
            std::int64_t ret1 = __umulh(x1, y1);
            std::int64_t ret2 = __umulh(x2, y2);
            std::int64_t ret3 = __umulh(x3, y3);
            std::int64_t ret4 = __umulh(x4, y4);
            std::int64_t ret5 = __umulh(x5, y5);
            std::int64_t ret6 = __umulh(x6, y6);
            std::int64_t ret7 = __umulh(x7, y7);

            return vec8x64u{_mm512_set_epi64(ret7, ret6, ret5, ret4, ret3, ret2, ret1, ret0)};

            #endif
        }

        static vec8x64u compute_m(vec8x64u l, vec8x64u d) {
            vec8x64u n = (vec8x64u{1} << l) - d;

            std::uint64_t quotient0 = div_64uhi_by_64u(avel::extract<0>(n), avel::extract<0>(d));
            std::uint64_t quotient1 = div_64uhi_by_64u(avel::extract<1>(n), avel::extract<1>(d));
            std::uint64_t quotient2 = div_64uhi_by_64u(avel::extract<2>(n), avel::extract<2>(d));
            std::uint64_t quotient3 = div_64uhi_by_64u(avel::extract<3>(n), avel::extract<3>(d));
            std::uint64_t quotient4 = div_64uhi_by_64u(avel::extract<4>(n), avel::extract<4>(d));
            std::uint64_t quotient5 = div_64uhi_by_64u(avel::extract<5>(n), avel::extract<5>(d));
            std::uint64_t quotient6 = div_64uhi_by_64u(avel::extract<6>(n), avel::extract<6>(d));
            std::uint64_t quotient7 = div_64uhi_by_64u(avel::extract<7>(n), avel::extract<7>(d));

            auto ret = vec8x64u{_mm512_set_epi64(
                quotient7, quotient6, quotient5, quotient4,
                quotient3, quotient2, quotient1, quotient0
            )} + vec8x64u{1};
            return ret;
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
