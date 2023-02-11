#ifndef AVEL_DENOMINATOR1X16I_HPP
#define AVEL_DENOMINATOR1X16I_HPP

namespace avel {
    
    using Denom1x16i = Denominator<vec1x16i>;
    
    template<>
    class alignas(8) Denominator<vec1x16i> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom16i denom):
            m(denom) {}

        explicit Denominator(vec1x16i d):
            m(decay(d)) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<vec1x16i> div(vec1x16i n, Denominator denom) {
            auto tmp = div(decay(n), denom.m);

            return {
                vec1x16i{tmp.quot},
                vec1x16i{tmp.rem}
            };
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x16i operator/(vec1x16i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x16i operator%(vec1x16i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        Denom16i m;
        
    };

    AVEL_FINL vec1x16i& operator/=(vec1x16i& lhs, Denom1x16i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec1x16i& operator%=(vec1x16i& lhs, Denom1x16i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR1X16I_HPP
