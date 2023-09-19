#ifndef AVEL_DENOMINATOR64I_HPP
#define AVEL_DENOMINATOR64I_HPP

namespace avel {

    using Denom64i = Denominator<std::int64_t>;

    template<>
    class Denominator<std::int64_t> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type Aliases
        //=================================================

        using backing_type = std::int64_t;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(std::int64_t d):
            Denominator(d, avel::max(bit_width(abs(d) - 1l), std::int64_t(1))) {}

    private:

        explicit Denominator(std::int64_t d, std::int64_t l):
            mp(compute_mp(l, d)),
            d_sign(d >> 63),
            sh(l - 1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<std::int64_t> div(std::int64_t n, Denominator denom) {
            std::int64_t q0 = n + mulhi(denom.mp, n);
            q0 = (q0 >> denom.sh) - (n >> 63);
            std::int64_t q = (q0 ^ denom.d_sign) - denom.d_sign;
            std::int64_t r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend std::int64_t operator/(std::int64_t lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend std::int64_t operator%(std::int64_t lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::int64_t mp = 0;
        std::int64_t d_sign = 0;
        std::int64_t sh = 0;
        std::int64_t d = 0;

        //=================================================
        // Helper functions
        //=================================================

        [[nodiscard]]
        static AVEL_FINL std::int64_t mulhi(std::int64_t x, std::int64_t y) {
            #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICX)
            //Should compile down to single imul instruction on X86 or a single
            //smulh instruction on ARM aarach64
            return static_cast<std::int64_t>((__int128_t(x) * __int128_t(y)) >> 64);

            #elif defined(AVEL_MSVC) && defined(AVEL_X86)
            return __mulh(x, y);

            #else
            //TODO: Change casting style
            //
            // Compute high 64-bits of unsigned 128-bit product
            std::uint64_t a_lo = std::uint32_t(x);
            std::uint64_t a_hi = std::uint64_t(x) >> 32;
            std::uint64_t b_lo = std::uint32_t(y);
            std::uint64_t b_hi = std::uint64_t(y) >> 32;

            std::uint64_t a_x_b_hi = a_hi * b_hi;
            std::uint64_t a_x_b_mid = a_hi * b_lo;
            std::uint64_t b_x_a_mid = b_hi * a_lo;
            std::uint64_t a_x_b_lo = a_lo * b_lo;

            std::uint64_t carry_bit = ((std::uint64_t)(std::uint32_t)a_x_b_mid +
                (std::uint64_t)(std::uint32_t)b_x_a_mid +
                (a_x_b_lo >> 32)) >> 32;

            std::uint64_t mul_hi = a_x_b_hi + (a_x_b_mid >> 32) + (b_x_a_mid >> 32) + carry_bit;

            // Convert to signed high half
            std::int64_t t1 = (x >> 63) & y;
            std::int64_t t2 = (y >> 63) & x;

            std::int64_t ret = mul_hi - t1 - t2;
            return ret;
            #endif
        }

        [[nodiscard]]
        static AVEL_FINL std::int64_t compute_mp(std::int64_t l, std::int64_t d) {
            #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICX)
            //TODO: Optimize. The portable divide solution is likely faster than this for x86
            // Last substraction should be redundant.
            return (__int128_t{1} << (63 + l)) / avel::abs(d) + 1 - (__int128_t{1} << 64);

            #else
            std::int64_t n = std::int64_t(1) << (l - 1);

            std::int64_t quotient = div_64uhi_by_64u(n, avel::abs(d));
            std::int64_t ret = quotient + 1;

            return ret;
            #endif
        }

    };

    AVEL_FINL std::int64_t& operator/=(std::int64_t& lhs, Denom64i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL std::int64_t& operator%=(std::int64_t& lhs, Denom64i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR64I_HPP
