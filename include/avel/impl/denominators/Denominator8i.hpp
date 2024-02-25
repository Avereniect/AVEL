#ifndef AVEL_DENOMINATOR8I_HPP
#define AVEL_DENOMINATOR8I_HPP

namespace avel {
    
    using Denom8i = Denominator<std::int8_t>;

    template<>
    class Denominator<std::int8_t> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type Aliases
        //=================================================

        using backing_type = std::int8_t;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(std::int8_t d):
            Denominator(d, max(bit_width(std::int8_t(abs(d) - 1)), std::int8_t(1))) {}

    private:

        explicit Denominator(std::int8_t d, std::int8_t l):
            mp((0x80 << l) / abs(d) - 0xff),
            d_sign(d >> 7),
            sh(l - 1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<std::int8_t> div(std::int8_t n, Denominator denom) {
            std::int8_t q0 = n + ((std::int16_t(denom.mp) * std::int16_t(n)) >> 8);
            q0 = (q0 >> denom.sh) - (n >> 7);
            std::int8_t q = (q0 ^ denom.d_sign) - denom.d_sign;
            std::int8_t r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend std::int8_t operator/(std::int8_t lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend std::int8_t operator%(std::int8_t lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        AVEL_FINL Denom8i& operator<<=(std::int8_t s) {
            auto effective_s = avel::min(avel::countl_sign(d), s);

            d <<= s;
            sh += effective_s;

            return *this;
        }

        AVEL_FINL Denom8i& operator>>=(std::int8_t s) {
            auto effective_s = avel::min(avel::countr_zero(d), s);

            d >>= s;
            sh -= effective_s;

            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom8i operator<<(std::int8_t s) const {
            Denom8i ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom8i operator>>(std::int8_t s) const {
            Denom8i ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL std::int8_t value() const {
            return d;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::int8_t mp = 0;
        std::int8_t d_sign = 0;
        std::int8_t sh = 0;
        std::int8_t d = 0;

        //=================================================
        // Helper functions
        //=================================================

    };

    AVEL_FINL std::int8_t& operator/=(std::int8_t& lhs, Denom8i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL std::int8_t& operator%=(std::int8_t& lhs, Denom8i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }
    
}

#endif //AVEL_DENOMINATOR8I_HPP
