//
// Created by avereniect on 8/4/22.
//

#ifndef AVEL_SCALAR_RECIPROCAL64U_HPP
#define AVEL_SCALAR_RECIPROCAL64U_HPP

namespace avel {
    
    using Recip64u = Reciprocal<std::uint64_t>;

    template<>
    class alignas(32) Reciprocal<std::uint64_t> {
    public:

        template<class U>
        friend class Reciprocal;

        //=================================================
        // -ctors
        //=================================================

        explicit Reciprocal(std::uint64_t d):
            Reciprocal(d, 64 - countl_zero(d - 1)) {}

    private:

        explicit Reciprocal(std::uint64_t d, std::uint64_t l):
            m(compute_m(l, d)),
            sh1(min(l, std::uint64_t(1))),
            sh2(l - sh1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend std::uint64_t operator*(Reciprocal lhs, std::uint64_t n) {
            #if defined(AVEL_X86) & (defined(AVEL_GCC) | defined(AVEL_CLANG))
            //This compiles down to an x86 mul instruction with optimizations
            __uint128_t t0 = lhs.m * n;
            std::uint64_t t1 = static_cast<std::uint64_t>(t0);
            #else
            std::uint64_t a_lo = std::uint32_t(lhs.m);
            std::uint64_t a_hi = lhs.m >> 32;
            std::uint64_t b_lo = std::uint32_t(n);
            std::uint64_t b_hi = n >> 32;

            std::uint64_t a_x_b_hi =  a_hi * b_hi;
            std::uint64_t a_x_b_mid = a_hi * b_lo;
            std::uint64_t b_x_a_mid = b_hi * a_lo;
            std::uint64_t a_x_b_lo =  a_lo * b_lo;

            std::uint64_t carry_bit = ((std::uint64_t)(std::uint32_t)a_x_b_mid +
                                     (std::uint64_t)(std::uint32_t)b_x_a_mid +
                                     (a_x_b_lo >> 32) ) >> 32;

            std::uint64_t multhi = a_x_b_hi + (a_x_b_mid >> 32) + (b_x_a_mid >> 32) + carry_bit;

            std::uint64_t t1 = multhi;
            #endif
            std::uint64_t q = (t1 + ((n - t1) >> lhs.sh1)) >> lhs.sh2;
            return q;
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint64_t operator*(std::uint64_t lhs, Reciprocal rhs) {
            return rhs * lhs;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::uint64_t m = 0;
        bool sh1 = false;
        std::uint64_t sh2 = 0;
        std::uint64_t d = 0;

        //=================================================
        // Helper functions
        //=================================================

        std::uint64_t mulhi(std::uint64_t x, std::uint64_t y) {
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
        }

        std::uint64_t compute_m(std::uint64_t l, std::uint64_t d) {
            #if defined(AVEL_GCC) | defined(AVEL_CLANG)
            return ((__uint128_t{1ull} << (64 + l)) / d) - (__uint128_t{1ull} << 64) + 1;
            #endif
            //TODO: More generic implementation
        }

    };

    AVEL_FINL std::uint64_t& operator*=(std::uint64_t& lhs, Reciprocal<std::uint64_t> rhs) {
        lhs = lhs * rhs;
        return lhs;
    }

    [[nodiscard]]
    avel::div_type<std::uint64_t> div(std::uint64_t n, Recip64u d) {
        std::uint64_t q = n * d;
        std::uint64_t m = n - (q * d);
        return {q, m};
    }

}

#endif //AVEL_SCALAR_RECIPROCAL64U_HPP
