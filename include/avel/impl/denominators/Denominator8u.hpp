#ifndef AVEL_DENOMINATOR8U_HPP
#define AVEL_DENOMINATOR8U_HPP

namespace avel {

    using Denom8u = Denominator<std::uint8_t>;

    template<>
    class alignas(4) Denominator<std::uint8_t> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(std::uint8_t d):
            Denominator(d, 8 - countl_zero(std::uint8_t(d - 1))) {}

    private:

        explicit Denominator(std::uint8_t d, std::uint8_t l):
            m((0x100u << l) / d - 0xffu),
            sh1(min(l, std::uint8_t(1))),
            sh2(l - sh1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<std::uint8_t> div(std::uint8_t n, Denominator denom) {
            std::uint8_t t1 = std::uint16_t(denom.m) * std::uint16_t(n) >> 8;
            std::uint8_t q = (t1 + ((n - t1) >> denom.sh1)) >> denom.sh2;
            std::uint8_t r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint8_t operator/(std::uint8_t lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint8_t operator%(std::uint8_t lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::uint8_t m = 0;
        bool sh1 = false;
        std::uint8_t sh2 = 0;
        std::uint8_t d = 0;

    };

    AVEL_FINL std::uint8_t& operator/=(std::uint8_t& lhs, Denom8u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL std::uint8_t& operator%=(std::uint8_t& lhs, Denom8u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR8U_HPP
