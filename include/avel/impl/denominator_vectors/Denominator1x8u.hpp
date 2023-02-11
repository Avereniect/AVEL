#ifndef AVEL_DENOMINATOR1X8U_HPP
#define AVEL_DENOMINATOR1X8U_HPP

namespace avel {
    
    using Denom1x8u = Denominator<vec1x8u>;
    
    template<>
    class alignas(4) Denominator<vec1x8u> {
    public:
        
        template<class U>
        friend class Denominator;

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
