#ifndef AVEL_DENOMINATOR1X32U_HPP
#define AVEL_DENOMINATOR1X32U_HPP

namespace avel {
    
    using Denom1x32u = Denominator<vec1x32u>;
    
    template<>
    class Denominator<vec1x32u> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec1x32u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom32u denom):
            m(denom) {}

        explicit Denominator(vec1x32u d):
            m(decay(d)) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<vec1x32u> div(vec1x32u n, Denominator denom) {
            auto tmp = div(decay(n), denom.m);

            return {
                vec1x32u{tmp.quot},
                vec1x32u{tmp.rem}
            };
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x32u operator/(vec1x32u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x32u operator%(vec1x32u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        AVEL_FINL Denom1x32u& operator<<=(vec1x32u s) {
            m <<= decay(s);
            return *this;
        }

        AVEL_FINL Denom1x32u& operator>>=(vec1x32u s) {
            m >>= decay(s);
            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom1x32u operator<<(vec1x32u s) const {
            Denom1x32u ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom1x32u operator>>(vec1x32u s) const {
            Denom1x32u ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec1x32u value() const {
            return vec1x32u{m.value()};
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        Denom32u m;
        
    };

    AVEL_FINL vec1x32u& operator/=(vec1x32u& lhs, Denom1x32u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec1x32u& operator%=(vec1x32u& lhs, Denom1x32u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR1X32U_HPP
