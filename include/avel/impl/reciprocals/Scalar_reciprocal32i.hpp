//
// Created by avereniect on 8/4/22.
//

#ifndef AVEL_SCALAR_RECIPROCAL32I_HPP
#define AVEL_SCALAR_RECIPROCAL32I_HPP

namespace avel {

    using Recip32i = Reciprocal<std::int32_t>;

    template<>
    class alignas(16) Reciprocal<std::int32_t> {
    public:

        template<class U>
        friend class Reciprocal;

        //=================================================
        // -ctors
        //=================================================

        explicit Reciprocal(std::int32_t d):
            Reciprocal(d, max(32 - countl_zero(abs(d) - 1), 1)) {}

    private:

        explicit Reciprocal(std::int32_t d, std::int32_t l):
            mp((0x80000000 << l) - 0xffffffff),
            d_sign(d >> 31),
            sh(l - 1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend std::int32_t operator*(Reciprocal lhs, std::int32_t n) {
            std::int32_t q0 = n + (std::uint64_t(lhs.mp) * std::uint64_t(n) >> 32);
            q0 = (q0 >> lhs.sh) - (n >> 31);
            std::int32_t q = (q0 ^ lhs.d_sign) - lhs.d_sign;
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

        std::int32_t mp = 0;
        std::uint32_t d_sign = 0;
        std::uint32_t sh = 0;
        std::uint32_t d = 0;

    };

    AVEL_FINL std::int32_t& operator*=(std::int32_t& lhs, Reciprocal<std::int32_t> rhs) {
        lhs = lhs * rhs;
        return lhs;
    }

}

#endif //AVEL_SCALAR_RECIPROCAL32I_HPP
