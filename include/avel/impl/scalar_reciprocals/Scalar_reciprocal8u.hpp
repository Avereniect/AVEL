//
// Created by avereniect on 8/4/22.
//

#ifndef AVEL_SCALAR_RECIPROCAL8U_HPP
#define AVEL_SCALAR_RECIPROCAL8U_HPP

namespace avel {

    using Recip8u = Reciprocal<std::uint8_t>;

    template<>
    class alignas(4) Reciprocal<std::uint8_t> {
    public:

        //=================================================
        // -ctors
        //=================================================

        explicit Reciprocal(std::uint8_t d):
            Reciprocal(d, 8 - countl_zero(d - 1)) {}

    private:

        explicit Reciprocal(std::uint8_t d, std::uint8_t l):
            m((std::uint16_t{1} << 8) * (((std::uint16_t() << l) - d) / d) + 1),
            sh1(min(l, 1)),
            sh2(l - sh1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend std::uint8_t operator*(Reciprocal lhs, std::uint8_t n) {
            std::uint8_t t1 = std::uint16_t(lhs.m) * std::uint16_t(n) >> 8;
            std::uint8_t q = (t1 + ((n - t1) >> lhs.sh1)) >> lhs.sh2;
            return q;
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint8_t operator*(std::uint8_t lhs, Reciprocal rhs) {
            return rhs * lhs;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::uint8_t m = 0;
        std::uint8_t sh1 = 0;
        std::uint8_t sh2 = 0;
        std::uint8_t d = 0;

    };

    AVEL_FINL std::uint8_t& operator*=(std::uint8_t& lhs, Reciprocal<std::uint8_t> rhs) {
        lhs = lhs * rhs;
        return lhs;
    }

    [[nodiscard]]
    avel::div_type<std::uint8_t> div(std::uint8_t n, Recip8u d) {
        std::uint8_t q = n * d;
        std::uint8_t m = n - (q * d);
        return {q, m};
    }

}

#endif //AVEL_SCALAR_RECIPROCAL8U_HPP
