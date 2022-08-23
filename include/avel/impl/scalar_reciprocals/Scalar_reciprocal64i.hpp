//
// Created by avereniect on 8/4/22.
//

#ifndef AVEL_SCALAR_RECIPROCAL64I_HPP
#define AVEL_SCALAR_RECIPROCAL64I_HPP

namespace avel {

    using Recip64i = Reciprocal<std::int64_t>;

    template<>
    class alignas(16) Reciprocal<std::int64_t> {
    public:

        //TODO: Complete implementation

        //=================================================
        // -ctors
        //=================================================

        explicit Reciprocal(std::int64_t d):
            Reciprocal(d, max(64 - countl_zero(abs(d) - 1), 1)) {}

    private:

        explicit Reciprocal(std::int64_t d, std::int64_t l):
            mp(compute_mp(l)),
            d_sign(d >> 63),
            sh(l - 1) {}

    public:

        //=================================================
        // Arithmetic Operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend std::int32_t operator*(Reciprocal lhs, std::int32_t n) {
            //TODO: Extend implementation
            std::int64_t q0 = n + mulhi(lhs.mp, n);//(std::uint64_t(lhs.mp) * std::uint64_t(n) >> 32);
            q0 = (q0 >> lhs.sh) - (n >> 31);
            std::int64_t q = (q0 ^ lhs.d_sign) - lhs.d_sign;
            return q;
        }

        [[nodiscard]]
        AVEL_FINL friend std::int32_t operator*(std::int32_t lhs, Reciprocal rhs) {
            return rhs * lhs;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::int64_t mp = 0;
        std::uint64_t d_sign = 0;
        std::uint64_t sh = 0;
        std::uint64_t d = 0;

        //=================================================
        // Helper functions
        //=================================================

        static std::int64_t mulhi(std::int64_t x, std::int64_t y) {
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

        static std::int64_t compute_mp(std::int64_t l) {
            #if defined(AVEL_GCC) | defined(AVEL_CLANG)
            return 1 + (__int128_t{1} << (63 + l)) - (__int128_t{1} << 64);
            #endif
            //TODO: More generic implementation
        }

    };

    AVEL_FINL std::int64_t& operator*=(std::int64_t& lhs, Reciprocal<std::int64_t> rhs) {
        lhs = lhs * rhs;
        return lhs;
    }

}

#endif //AVEL_SCALAR_RECIPROCAL64I_HPP
