#ifndef AVEL_DENOMINATOR1X16U_HPP
#define AVEL_DENOMINATOR1X16U_HPP

namespace avel {
    
    using Denom1x16u = Denominator<vec1x16u>;
    
    template<>
    class Denominator<vec1x16u> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type Aliases
        //=================================================
        
        using backing_type = vec1x16u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom16u denom):
            m(denom) {}

        explicit Denominator(vec1x16u d):
            m(decay(d)) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<vec1x16u> div(vec1x16u n, Denominator denom) {
            auto tmp = div(decay(n), denom.m);

            return {
                vec1x16u{tmp.quot},
                vec1x16u{tmp.rem}
            };
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x16u operator/(vec1x16u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x16u operator%(vec1x16u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        Denom16u m;
        
    };

    AVEL_FINL vec1x16u& operator/=(vec1x16u& lhs, Denom1x16u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec1x16u& operator%=(vec1x16u& lhs, Denom1x16u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR1X16U_HPP
