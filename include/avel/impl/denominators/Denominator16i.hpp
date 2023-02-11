#ifndef AVEL_DENOMINATOR16I_HPP
#define AVEL_DENOMINATOR16I_HPP

namespace avel {

    using Denom16i = Denominator<std::int16_t>;

    template<>
    class alignas(8) Denominator<std::int16_t> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(std::int16_t d):
            Denominator(d, max(16 - countl_zero(std::int16_t(abs(d) - 1)), 1)) {}

    private:

        explicit Denominator(std::int16_t d, std::int16_t l):
            mp((0x8000 << l) / abs(d) - 0xffff),
            d_sign(d >> 15),
            sh(l - 1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<std::int16_t> div(std::int16_t n, Denominator denom) {
            std::int16_t q0 = n + (std::int32_t(denom.mp) * std::int32_t(n) >> 16);
            q0 = (q0 >> denom.sh) - (n >> 15);
            std::int16_t q = (q0 ^ denom.d_sign) - denom.d_sign;
            std::int16_t r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend std::int16_t operator/(std::int16_t lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend std::int16_t operator%(std::int16_t lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::int16_t mp = 0;
        std::int16_t d_sign = 0;
        std::int16_t sh = 0;
        std::int16_t d = 0;

    };

    AVEL_FINL std::int16_t& operator/=(std::int16_t& lhs, Denom16i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL std::int16_t& operator%=(std::int16_t& lhs, Denom16i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR16I_HPP
