#ifndef AVEL_DENOMINATOR1X8I_HPP
#define AVEL_DENOMINATOR1X8I_HPP

namespace avel {
    
    using Denom1x8i = Denominator<vec1x8i>;
    
    template<>
    class Denominator<vec1x8i> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type Aliases
        //=================================================

        using backing_type = vec1x8i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8i denom):
            m(denom) {}

        explicit Denominator(vec1x8i d):
            m(decay(d)) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<vec1x8i> div(vec1x8i n, Denominator denom) {
            auto tmp = div(decay(n), denom.m);

            return {
                vec1x8i{tmp.quot},
                vec1x8i{tmp.rem}
            };
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x8i operator/(vec1x8i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x8i operator%(vec1x8i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        Denom8i m;
        
    };

    AVEL_FINL vec1x8i& operator/=(vec1x8i& lhs, Denom1x8i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec1x8i& operator%=(vec1x8i& lhs, Denom1x8i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR1X8I_HPP
