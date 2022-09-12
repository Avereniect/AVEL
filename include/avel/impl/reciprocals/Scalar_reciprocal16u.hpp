//
// Created by avereniect on 8/4/22.
//

#ifndef AVEL_SCALAR_RECIPROCAL16U_HPP
#define AVEL_SCALAR_RECIPROCAL16U_HPP

namespace avel {

    using Recip16u = Reciprocal<std::uint16_t>;

    template<>
    class alignas(8) Reciprocal<std::uint16_t> {
    public:

        template<class U>
        friend class Reciprocal;

        //=================================================
        // -ctors
        //=================================================

        explicit Reciprocal(std::uint16_t d):
            Reciprocal(d, 16 - countl_zero(d - 1)) {}

    private:

        explicit Reciprocal(std::uint16_t d, std::uint16_t l):
            m((0x10000u << l) / d - 0xffffu),
            sh1(min(l, std::uint16_t(1))),
            sh2(l - sh1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend std::uint16_t operator*(Reciprocal lhs, std::uint16_t n) {
            std::uint16_t t1 = std::uint32_t(lhs.m) * std::uint32_t(n) >> 16;
            std::uint16_t q = (t1 + ((n - t1) >> lhs.sh1)) >> lhs.sh2;
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

        std::uint16_t m = 0;
        bool sh1 = false;
        std::uint16_t sh2 = 0;
        std::uint16_t d = 0;

    };

    AVEL_FINL std::uint16_t& operator*=(std::uint16_t& lhs, Reciprocal<std::uint16_t> rhs) {
        lhs = lhs * rhs;
        return lhs;
    }

    [[nodiscard]]
    avel::div_type<std::uint16_t> div(std::uint16_t n, Recip16u d) {
        std::uint16_t q = n * d;
        std::uint16_t m = n - (q * d);
        return {q, m};
    }

}

#endif //AVEL_SCALAR_RECIPROCAL16U_HPP
