//
// Created by avereniect on 8/4/22.
//

#ifndef AVEL_SCALAR_RECIPROCAL16I_HPP
#define AVEL_SCALAR_RECIPROCAL16I_HPP

namespace avel {

    using Recip16i = Reciprocal<std::int16_t>;

    template<>
    class alignas(8) Reciprocal<std::int16_t> {
    public:

        template<class U>
        friend class Reciprocal;

        //=================================================
        // -ctors
        //=================================================

        explicit Reciprocal(std::int16_t d):
            Reciprocal(d, max(16 - countl_zero(abs(d) - 1), 1)) {}

    private:

        explicit Reciprocal(std::int16_t d, std::int16_t l):
            mp((0x8000 << l) - 0xffff),
            d_sign(d >> 15),
            sh(l - 1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend std::uint16_t operator*(Reciprocal lhs, std::uint16_t n) {
            std::int16_t q0 = n + (std::uint32_t(lhs.mp) * std::uint32_t(n) >> 16);
            q0 = (q0 >> lhs.sh) - (n >> 15);
            std::int16_t q = (q0 ^ lhs.d_sign) - lhs.d_sign;
            return q;
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint16_t operator*(std::uint16_t lhs, Reciprocal rhs) {
            return rhs * lhs;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::int16_t mp = 0;
        std::uint16_t d_sign = 0;
        std::uint16_t sh = 0;
        std::uint16_t d = 0;

    };

    AVEL_FINL std::int16_t& operator*=(std::int16_t& lhs, Reciprocal<std::int16_t> rhs) {
        lhs = lhs * rhs;
        return lhs;
    }

}

#endif //AVEL_SCALAR_RECIPROCAL16I_HPP
