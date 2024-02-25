#ifndef AVEL_DENOMINATOR1X64I_HPP
#define AVEL_DENOMINATOR1X64I_HPP

namespace avel {
    
    using Denom1x64i = Denominator<vec1x64i>;
    
    template<>
    class Denominator<vec1x64i> {
    public:
        
        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec1x64i;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom64i denom):
            m(denom) {}

        explicit Denominator(vec1x64i d):
            m(decay(d)) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend div_type<vec1x64i> div(vec1x64i n, Denominator denom) {
            auto tmp = div(decay(n), denom.m);

            return {
                vec1x64i{tmp.quot},
                vec1x64i{tmp.rem}
            };
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x64i operator/(vec1x64i lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec1x64i operator%(vec1x64i lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

        AVEL_FINL Denom1x64i& operator<<=(vec1x64i s) {
            m <<= decay(s);
            return *this;
        }

        AVEL_FINL Denom1x64i& operator>>=(vec1x64i s) {
            m >>= decay(s);
            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Denom1x64i operator<<(vec1x64i s) const {
            Denom1x64i ret = *this;
            ret <<= s;

            return ret;
        }

        [[nodiscard]]
        AVEL_FINL Denom1x64i operator>>(vec1x64i s) const {
            Denom1x64i ret = *this;
            ret >>= s;

            return ret;
        }

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        AVEL_FINL vec1x64i value() const {
            return vec1x64i{m.value()};
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        Denom64i m;
        
    };

    AVEL_FINL vec1x64i& operator/=(vec1x64i& lhs, Denom1x64i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec1x64i& operator%=(vec1x64i& lhs, Denom1x64i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR1X64I_HPP
