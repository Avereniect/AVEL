#ifndef AVEL_DENOMINATOR8U_HPP
#define AVEL_DENOMINATOR8U_HPP

namespace avel {

    using Denom8u = Denominator<std::uint8_t>;

    template<>
    class Denominator<std::uint8_t> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type Aliases
        //=================================================

        using backing_type = std::uint8_t;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(std::uint8_t d):
            Denominator(d, 8 - countl_zero(std::uint8_t(d - 1))) {}

    private:

        explicit Denominator(std::uint8_t d, std::uint8_t l):
            m(std::uint32_t(0x0100u << l) / d - std::uint16_t{0xffu}),
            sh2(l - 1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<std::uint8_t> div(std::uint8_t n, Denominator denom) {
            if (denom.d == 1) {
                return {n, 0};
            }

            std::uint8_t t1 = std::uint16_t(denom.m) * std::uint16_t(n) >> 8;
            std::uint8_t q = (t1 + ((n - t1) >> 1)) >> denom.sh2;
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
