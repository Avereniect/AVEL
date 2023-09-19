#ifndef AVEL_DENOMINATOR2X64I_HPP
#define AVEL_DENOMINATOR2X64I_HPP

namespace avel {

    using Denom2x64i = Denominator<vec2x64i>;

    template<>
    class Denominator<vec2x64i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec2x64i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8i d):
            mp(d.mp),
            d_sign(d.d_sign),
            sh(d.sh),
            d(d.d) {}

        explicit Denominator(vec2x64i d):
            Denominator(d, max(vec2x64i{64} - countl_zero(abs(d) - vec2x64i{1}), vec2x64i{1})) {}

    private:

        explicit Denominator(vec2x64i d, vec2x64i l):
            mp(compute_mp(l, d)),
            d_sign(d >> 63),
            sh(l - vec2x64i{1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec2x64i> div(vec2x64i n, Denominator denom) {
            vec2x64i q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 63);
            vec2x64i q = (q0 ^ denom.d_sign) - denom.d_sign;
            vec2x64i r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec2x64i operator/(vec2x64i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec2x64i operator%(vec2x64i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec2x64i mp{};
        vec2x64i d_sign{};
        vec2x64i sh{};
        vec2x64i d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec2x64i mulhi(vec2x64i x, vec2x64i y) {
            #if defined(AVEL_SSE2) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
            std::int64_t x_lo = extract<0>(x);
            std::int64_t x_hi = extract<1>(x);

            std::int64_t y_lo = extract<0>(y);
            std::int64_t y_hi = extract<1>(y);

            // Implementation should leverage x86's 64x64->128-bit multiplication
            std::int64_t ret_lo = (__int128_t(x_lo) * __int128_t(y_lo)) >> 64;
            std::int64_t ret_hi = (__int128_t(x_hi) * __int128_t(y_hi)) >> 64;

            return vec2x64i{_mm_set_epi64x(ret_hi, ret_lo)};

            #elif defined(AVEL_SSE2) && defined(AVEL_MSVC)
            std::int64_t x_lo = extract<0>(x);
            std::int64_t x_hi = extract<1>(x);

            std::int64_t y_lo = extract<0>(y);
            std::int64_t y_hi = extract<1>(y);

            // Implementation should leverage x86's 64x64->128-bit multiplication
            std::int64_t ret_lo = __mulh(x_lo, y_lo);
            std::int64_t ret_hi = __mulh(x_hi, y_hi);

            return vec2x64i{ _mm_set_epi64x(ret_hi, ret_lo) };

            #endif

            #if defined(AVEL_NEON)
            std::int64_t x_lo = extract<0>(x);
            std::int64_t x_hi = extract<1>(x);

            std::int64_t y_lo = extract<0>(y);
            std::int64_t y_hi = extract<1>(y);

            // Implementation should leverage ARM's Aarch64's smulh instruction
            std::int64_t ret_lo = (__int128_t(x_lo) * __int128_t(y_lo)) >> 64;
            std::int64_t ret_hi = (__int128_t(x_hi) * __int128_t(y_hi)) >> 64;

            vec2x64i::primitive ret;
            ret = vsetq_lane_s64(ret_lo, ret, 0x0);
            ret = vsetq_lane_s64(ret_hi, ret, 0x1);

            return vec2x64i{ret};

            #endif
        }

        static vec2x64i compute_mp(vec2x64i l, vec2x64i d) {
            #if defined(AVEL_AVX2)
            vec2x64i n = vec2x64i{1} << (l - vec2x64i{1});

            d = avel::abs(d);

            auto quotient0 = div_64uhi_by_64u(extract<0>(n), extract<0>(d));
            auto quotient1 = div_64uhi_by_64u(extract<1>(n), extract<1>(d));

            vec2x64i ret{_mm_set_epi64x(quotient1, quotient0)};
            ret += vec2x64i{1};

            return ret;

            #else
            l -= vec2x64i{1};

            d = avel::abs(d);
            std::int64_t d_lo = extract<0>(d);
            std::int64_t d_hi = extract<1>(d);

            auto n_lo = std::int64_t(1) << extract<0>(l);
            auto n_hi = std::int64_t(1) << extract<1>(l);

            auto quotient0 = div_64uhi_by_64u(n_lo, extract<0>(d));
            auto quotient1 = div_64uhi_by_64u(n_hi, extract<1>(d));

            vec2x64i ret{0};
            ret = insert<0>(ret, quotient0);
            ret = insert<1>(ret, quotient1);
            ret += vec2x64i{1};

            return ret;

            #endif
        }

    };

    AVEL_FINL vec2x64i& operator/=(vec2x64i& lhs, Denom2x64i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec2x64i& operator%=(vec2x64i& lhs, Denom2x64i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR2X64I_HPP
