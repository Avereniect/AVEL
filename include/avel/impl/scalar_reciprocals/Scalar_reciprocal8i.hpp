//
// Created by avereniect on 8/4/22.
//

#ifndef AVEL_SCALAR_RECIPROCAL8I_HPP
#define AVEL_SCALAR_RECIPROCAL8I_HPP

namespace avel {
    
    using Recip8i = Reciprocal<std::int8_t>;

    template<>
    class alignas(4) Reciprocal<std::int8_t> {
    public:

        //=================================================
        // -ctors
        //=================================================

        explicit Reciprocal(std::int8_t d):
            Reciprocal(d, max(8 - countl_zero(abs(d) - 1), 1)) {}

    private:

        explicit Reciprocal(std::int8_t d, std::int8_t l):
            mp(1 + (std::uint64_t{1} << (7 + l)) - (std::uint64_t{1} << 8)),
            d_sign(d >> 31),
            sh(l - 1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend std::int8_t operator*(Reciprocal lhs, std::int8_t n) {
            std::int8_t q0 = n + (std::uint64_t(lhs.mp) * std::uint64_t(n) >> 8);
            q0 = (q0 >> lhs.sh) - (n >> 7);
            std::int8_t q = (q0 ^ lhs.d_sign) - lhs.d_sign;
            return q;
        }

        [[nodiscard]]
        AVEL_FINL friend std::int8_t operator*(std::int8_t lhs, Reciprocal rhs) {
            return rhs * lhs;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::int8_t mp = 0;
        std::uint8_t d_sign = 0;
        std::uint8_t sh = 0;
        std::uint8_t d = 0;

    };

    AVEL_FINL std::int8_t& operator*=(std::int8_t& lhs, Reciprocal<std::int8_t> rhs) {
        lhs = lhs * rhs;
        return lhs;
    }
    
}

#endif //AVEL_SCALAR_RECIPROCAL8I_HPP
