#ifndef AVEL_DENOMINATOR64U_HPP
#define AVEL_DENOMINATOR64U_HPP

namespace avel {
    
    using Denom64u = Denominator<std::uint64_t>;

    template<>
    class Denominator<std::uint64_t> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type Aliases
        //=================================================

        using backing_type = std::uint64_t;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(std::uint64_t d):
            Denominator(d, 64 - countl_zero(d - 1)) {}

    private:

        explicit Denominator(std::uint64_t d, std::uint64_t l):
            m(compute_m(l, d)),
            sh2(l - 1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<std::uint64_t> div(std::uint64_t n, Denominator denom) {
            if (denom.d == 1) {
                return {n, 0};
            }

            #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG))
            //This compiles down to an x86 mul instruction when optimized
            __uint128_t t0 = __uint128_t(denom.m) * __uint128_t(n);
            std::uint64_t t1 = static_cast<std::uint64_t>(t0 >> 64);

            #elif defined(AVEL_X86) && defined(AVEL_MSVC)
            std::uint64_t t1 =  __umulh(denom.m, n);

            #else
            //TODO: Change casting style

            std::uint64_t a_lo = std::uint32_t(denom.m);
            std::uint64_t a_hi = denom.m >> 32;

            std::uint64_t b_lo = std::uint32_t(n);
            std::uint64_t b_hi = n >> 32;

            std::uint64_t a_x_b_hi =  a_hi * b_hi;
            std::uint64_t a_x_b_mid = a_hi * b_lo;
            std::uint64_t b_x_a_mid = b_hi * a_lo;
            std::uint64_t a_x_b_lo =  a_lo * b_lo;

            std::uint64_t carry_bit = ((std::uint64_t)(std::uint32_t)a_x_b_mid +
                                     (std::uint64_t)(std::uint32_t)b_x_a_mid +
                                     (a_x_b_lo >> 32) ) >> 32;

            std::uint64_t t1 = a_x_b_hi + (a_x_b_mid >> 32) + (b_x_a_mid >> 32) + carry_bit;

            #endif

            std::uint64_t q = (t1 + ((n - t1) >> 1)) >> denom.sh2;
            std::uint64_t r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint64_t operator/(std::uint64_t lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint64_t operator%(std::uint64_t lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        AVEL_FINL Denom64u& operator<<=(std::uint64_t s) {
            auto effective_s = avel::min(avel::countl_zero(d), s);

            d <<= s;
            sh2 += effective_s;

            return *this;
        }

        AVEL_FINL Denom64u& operator>>=(std::uint64_t s) {
            auto effective_s = avel::min(avel::countr_zero(d), s);

            d >>= s;
            sh2 -= effective_s;

            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom64u operator<<(std::uint64_t s) const {
            Denom64u ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom64u operator>>(std::uint64_t s) const {
            Denom64u ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL std::uint64_t value() const {
            return d;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::uint64_t m = 0;
        std::uint64_t sh2 = 0;
        std::uint64_t d = 0;

        //=================================================
        // Helper functions
        //=================================================

        [[nodiscard]]
        static AVEL_FINL std::uint64_t mulhi(std::uint64_t x, std::uint64_t y) {
            #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX)
            return static_cast<std::uint64_t>(__uint128_t(x) * __uint128_t(y));

            #elif defined(AVEL_MSVC) && defined(AVEL_X86)
            //TODO: test
            return __umulh(x, y);

            #else
            std::uint64_t a_lo = x & 0x00000000FFFFFFFFull;
            std::uint64_t a_hi = x >> 32;
            std::uint64_t b_lo = y & 0x00000000FFFFFFFFull;
            std::uint64_t b_hi = y >> 32;

            std::uint64_t a_x_b_hi =  a_hi * b_hi;
            std::uint64_t a_x_b_mid = a_hi * b_lo;
            std::uint64_t b_x_a_mid = b_hi * a_lo;
            std::uint64_t a_x_b_lo =  a_lo * b_lo;

            std::uint64_t carry_bit = (
                (std::uint64_t)(std::uint32_t)a_x_b_mid +
                (std::uint64_t)(std::uint32_t)b_x_a_mid +
                (a_x_b_lo >> 32)
            ) >> 32;

            std::uint64_t ret = a_x_b_hi + (a_x_b_mid >> 32) + (b_x_a_mid >> 32) + carry_bit;
            return ret;
            #endif
        }

        [[nodiscard]]
        static AVEL_FINL std::uint64_t compute_m(std::uint64_t l, std::uint64_t d) {
            #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICX)
            //TODO: Optimize. The portable divide solution is likely faster than this for x86
            auto tmp0 = __uint128_t(1) << 64;
            auto tmp1 = __uint128_t(1) << l;
            auto tmp2 = tmp1 - d;
            auto tmp3 = tmp0 * tmp2;
            auto tmp4 = __uint128_t(d);
            auto tmp5 = tmp3 / tmp4;
            auto tmp6 = tmp5 + 1;
            return tmp6;

            //return (__uint128_t((1 << l) - d) << 64) / __uint128_t(d) + 1;
            #else
            std::uint64_t n = (l == 64 ? 0 : std::uint64_t(1) << l) - d;

            auto ret = div_64uhi_by_64u(n, d);
            ret += 1;
            return ret;

            #endif
        }

    };

    AVEL_FINL std::uint64_t& operator/=(std::uint64_t& lhs, Denom64u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL std::uint64_t& operator%=(std::uint64_t& lhs, Denom64u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR64U_HPP
