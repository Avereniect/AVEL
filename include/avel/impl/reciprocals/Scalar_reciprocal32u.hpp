//
// Created by avereniect on 8/4/22.
//

#ifndef AVEL_SCALAR_RECIPROCAL32U_HPP
#define AVEL_SCALAR_RECIPROCAL32U_HPP

namespace avel {

    using Recip32u = Reciprocal<std::uint32_t>;

    template<>
    class alignas(16) Reciprocal<std::uint32_t> {
    public:

        template<class U>
        friend class Reciprocal;

        //=================================================
        // -ctors
        //=================================================

        explicit Reciprocal(std::uint32_t d):
            Reciprocal(d, 32 - countl_zero(d - 1)) {}

    private:

        explicit Reciprocal(std::uint32_t d, std::uint32_t l):
            m((0x100000000ul << l) / d - 0xfffffffful),
            sh1(min(l, std::uint32_t(1))),
            sh2(l - sh1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend std::uint32_t operator*(Reciprocal lhs, std::uint32_t n) {
            std::uint32_t t1 = std::uint64_t(lhs.m) * std::uint64_t(n) >> 32;
            std::uint32_t q = (t1 + ((n - t1) >> lhs.sh1)) >> lhs.sh2;
            return q;
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint32_t operator*(std::uint32_t lhs, Reciprocal rhs) {
            return rhs * lhs;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::uint32_t m = 0;
        bool sh1 = false;
        std::uint32_t sh2 = 0;
        std::uint32_t d = 0;

    };

    AVEL_FINL std::uint32_t& operator*=(std::uint32_t& lhs, Reciprocal<std::uint32_t> rhs) {
        lhs = lhs * rhs;
        return lhs;
    }

    [[nodiscard]]
    avel::div_type<std::uint32_t> div(std::uint32_t n, Recip32u d) {
        std::uint32_t q = n * d;
        std::uint32_t m = n - (q * d);
        return {q, m};
    }

}

#endif //AVEL_SCALAR_RECIPROCAL32U_HPP
