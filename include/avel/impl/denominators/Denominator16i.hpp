#ifndef AVEL_DENOMINATOR16I_HPP
#define AVEL_DENOMINATOR16I_HPP

namespace avel {

    using Denom16i = Denominator<std::int16_t>;

    template<>
    class Denominator<std::int16_t> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type Aliases
        //=================================================

        using backing_type = std::int16_t;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(std::int16_t d):
            Denominator(d, max(bit_width(std::int16_t(abs(d) - 1)), std::int16_t(1))) {}

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

        AVEL_FINL Denom16i& operator<<=(std::int16_t s) {
            auto effective_s = avel::min(avel::countl_sign(d), s);

            d <<= s;
            sh += effective_s;

            return *this;
        }

        AVEL_FINL Denom16i& operator>>=(std::int16_t s) {
            auto effective_s = avel::min(avel::countr_zero(d), s);

            d >>= s;
            sh -= effective_s;

            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom16i operator<<(std::int16_t s) const {
            Denom16i ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom16i operator>>(std::int16_t s) const {
            Denom16i ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL std::int16_t value() const {
            return d;
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
