#ifndef AVEL_DENOMINATOR1X8U_HPP
#define AVEL_DENOMINATOR1X8U_HPP

namespace avel {
    
    using Denom1x8u = Denominator<vec1x8u>;
    
    template<>
    class Denominator<vec1x8u> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type Aliases
        //=================================================

        using backing_type = vec1x8u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8u denom):
            m(denom) {}

        explicit Denominator(vec1x8u d):
            m(decay(d)) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<vec1x8u> div(vec1x8u n, Denominator denom) {
            auto tmp = div(decay(n), denom.m);

            return {
                vec1x8u{tmp.quot},
                vec1x8u{tmp.rem}
            };
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x8u operator/(vec1x8u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x8u operator%(vec1x8u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        AVEL_FINL Denom1x8u& operator<<=(vec1x8u s) {
            m <<= decay(s);
            return *this;
        }

        AVEL_FINL Denom1x8u& operator>>=(vec1x8u s) {
            m >>= decay(s);
            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom1x8u operator<<(vec1x8u s) const {
            Denom1x8u ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom1x8u operator>>(vec1x8u s) const {
            Denom1x8u ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================
        
        [[nodiscard]]
        AVEL_FINL vec1x8u value() const {
            return vec1x8u{m.value()};
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        Denom8u m;
        
    };

    AVEL_FINL vec1x8u& operator/=(vec1x8u& lhs, Denom1x8u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec1x8u& operator%=(vec1x8u& lhs, Denom1x8u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR1X8U_HPP
