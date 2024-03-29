#ifndef AVEL_DENOMINATOR16U_HPP
#define AVEL_DENOMINATOR16U_HPP

namespace avel {

    using Denom16u = Denominator<std::uint16_t>;

    template<>
    class Denominator<std::uint16_t> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type Aliases
        //=================================================

        using backing_type = std::uint16_t;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(std::uint16_t d):
            Denominator(d, 16 - countl_zero(std::uint16_t(d - 1))) {}

    private:

        explicit Denominator(std::uint16_t d, std::uint16_t l):
            m((std::uint32_t((1 << l) - d) << 16) / std::uint32_t(d) + 1),
            sh2(l - 1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<std::uint16_t> div(std::uint16_t n, Denominator denom) {
            if (denom.d == 1) {
                return {n, 0};
            }

            std::uint16_t t1 = std::uint32_t(denom.m) * std::uint32_t(n) >> 16;
            std::uint16_t q = (t1 + ((n - t1) >> 1)) >> denom.sh2;
            std::uint16_t r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint16_t operator/(std::uint16_t lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint16_t operator%(std::uint16_t lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        AVEL_FINL Denom16u& operator<<=(std::uint16_t s) {
            auto effective_s = avel::max(avel::countl_zero(d), s);

            d <<= s;
            sh2 += effective_s;

            return *this;
        }

        AVEL_FINL Denom16u& operator>>=(std::uint16_t s) {
            auto effective_s = avel::min(avel::countr_zero(d), s);

            d >>= s;
            sh2 -= effective_s;

            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom16u operator<<(std::uint16_t s) const {
            Denom16u ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom16u operator>>(std::uint16_t s) const {
            Denom16u ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL std::uint16_t value() const {
            return d;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::uint16_t m = 0;
        std::uint16_t sh2 = 0;
        std::uint16_t d = 0;

    };

    AVEL_FINL std::uint16_t& operator/=(std::uint16_t& lhs, Denom16u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL std::uint16_t& operator%=(std::uint16_t& lhs, Denom16u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR16U_HPP
