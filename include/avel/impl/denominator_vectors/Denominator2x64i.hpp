#ifndef AVEL_DENOMINATOR2X64I_HPP
#define AVEL_DENOMINATOR2X64I_HPP

namespace avel {

    using Denom2x64i = Denominator<vec2x64i>;

    template<>
    class alignas(64) Denominator<vec2x64i> {
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
            #if defined(AVEL_SSE2)
            std::int64_t x_lo = extract<0>(x);
            std::int64_t x_hi = extract<1>(x);

            std::int64_t y_lo = extract<0>(y);
            std::int64_t y_hi = extract<1>(y);

            // Implementation should leverage x86's 64x64->128-bit multiplication
            std::int64_t ret_lo = (__int128_t(x_lo) * __int128_t(y_lo)) >> 64;
            std::int64_t ret_hi = (__int128_t(x_hi) * __int128_t(y_hi)) >> 64;

            return vec2x64i{_mm_set_epi64x(ret_hi, ret_lo)};

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
            //TODO: Optimize
            __int128_t l_lo = extract<0>(l);
            __int128_t l_hi = extract<1>(l);

            d = abs(d);
            __int128_t d_lo = extract<0>(d);
            __int128_t d_hi = extract<1>(d);

            auto tmp0_lo = __int128_t(0x8000000000000000) << l_lo;
            auto tmp0_hi = __int128_t(0x8000000000000000) << l_hi;

            auto tmp2_lo = tmp0_lo / d_lo;
            auto tmp2_hi = tmp0_hi / d_hi;

            auto tmp3_lo = tmp2_lo - __int128_t(0xFFFFFFFFFFFFFFFF);
            auto tmp3_hi = tmp2_hi - __int128_t(0xFFFFFFFFFFFFFFFF);

            return vec2x64i{{
                static_cast<std::int64_t>(tmp3_lo),
                static_cast<std::int64_t>(tmp3_hi)
            }};
        }

    };

}

#endif //AVEL_DENOMINATOR2X64I_HPP
