#ifndef AVEL_DENOMINATOR4X32I_HPP
#define AVEL_DENOMINATOR4X32I_HPP

namespace avel {

    using Denom4x32i = Denominator<vec4x32i>;

    template<>
    class alignas(16) Denominator<vec4x32i> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom32i recip):
            mp(recip.mp),
            d_sign(recip.d_sign),
            sh(recip.sh),
            d(recip.d) {}

        explicit Denominator(vec4x32i d):
            Denominator(d, max(vec4x32i{32} - vec4x32i{countl_zero(abs(d) - vec4x32i{1})}, vec4x32i{1})) {}

    private:

        explicit Denominator(vec4x32i d, vec4x32i l):
            mp(compute_mp(d, l)),
            d_sign(d >> 31),
            sh(l - vec4x32i{1}),
            d(d) {}

    private:

        //=================================================
        // Instance members
        //=================================================

        vec4x32i mp{};
        vec4x32u d_sign {};
        vec4x32u sh{};
        vec4x32u d{};

        //=================================================
        // Compute mp
        //=================================================

        vec4x32i compute_mp(vec4x32i d, vec4x32i l) {
            vec2x64u l_lo = bit_cast<vec2x64u>(l) & vec2x64u{0x00000000FFFFFFFF};
            vec2x64u l_hi = bit_cast<vec2x64u>(l) >> 32;

            vec2x64u d_lo = bit_cast<vec2x64u>(d) & vec2x64u{0x00000000FFFFFFFF};
            vec2x64u d_hi = bit_cast<vec2x64u>(d) >> 32;

            vec2x64u lo_part = (vec2x64u{0x80000000} << l_lo) / d_lo;
            lo_part -= vec2x64u{0xFFFFFFFFull};

            vec2x64u hi_part = (vec2x64u{0x100000000} << l_hi) / d_hi;
            hi_part -= vec2x64u{0xFFFFFFFFull};
            hi_part <<= 32;

            vec4x32i x = bit_cast<vec4x32i>(lo_part);
            vec4x32i y = bit_cast<vec4x32i>(hi_part);
            mask4x32i m{{false, true, false, true}};

            return blend(m, y, x);
        }

    };

}

#endif //AVEL_DENOMINATOR4X32I_HPP
