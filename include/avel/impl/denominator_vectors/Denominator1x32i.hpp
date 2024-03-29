#ifndef AVEL_DENOMINATOR1X32I_HPP
#define AVEL_DENOMINATOR1X32I_HPP

namespace avel {
    
    using Denom1x32i = Denominator<vec1x32i>;
    
    template<>
    class Denominator<vec1x32i> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec1x32i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom32i denom):
            m(denom) {}

        explicit Denominator(vec1x32i d):
            m(decay(d)) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<vec1x32i> div(vec1x32i n, Denominator denom) {
            auto tmp = div(decay(n), denom.m);

            return {
                vec1x32i{tmp.quot},
                vec1x32i{tmp.rem}
            };
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x32i operator/(vec1x32i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x32i operator%(vec1x32i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        AVEL_FINL Denom1x32i& operator<<=(vec1x32i s) {
            m <<= decay(s);
            return *this;
        }

        AVEL_FINL Denom1x32i& operator>>=(vec1x32i s) {
            m >>= decay(s);
            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom1x32i operator<<(vec1x32i s) const {
            Denom1x32i ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom1x32i operator>>(vec1x32i s) const {
            Denom1x32i ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec1x32i value() const {
            return vec1x32i{m.value()};
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        Denom32i m;
        
    };

    AVEL_FINL vec1x32i& operator/=(vec1x32i& lhs, Denom1x32i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec1x32i& operator%=(vec1x32i& lhs, Denom1x32i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR1X32I_HPP
