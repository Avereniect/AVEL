#ifndef AVEL_DENOMINATOR1X64U_HPP
#define AVEL_DENOMINATOR1X64U_HPP

namespace avel {
    
    using Denom1x64u = Denominator<vec1x64u>;
    
    template<>
    class Denominator<vec1x64u> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec1x64u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom64u denom):
            m(denom) {}

        explicit Denominator(vec1x64u d):
            m(decay(d)) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<vec1x64u> div(vec1x64u n, Denominator denom) {
            auto tmp = div(decay(n), denom.m);

            return {
                vec1x64u{tmp.quot},
                vec1x64u{tmp.rem}
            };
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x64u operator/(vec1x64u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x64u operator%(vec1x64u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        Denom64u m;
        
    };

    AVEL_FINL vec1x64u& operator/=(vec1x64u& lhs, Denom1x64u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec1x64u& operator%=(vec1x64u& lhs, Denom1x64u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR1X64U_HPP
