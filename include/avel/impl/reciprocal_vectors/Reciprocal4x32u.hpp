//
// Created by avereniect on 7/27/22.
//

#ifndef AVEL_RECIPROCAL4X32U_HPP
#define AVEL_RECIPROCAL4X32U_HPP

namespace avel {

    using Recip4x32u = Reciprocal<vec4x32u>;

    template<>
    class Reciprocal<vec4x32u> {
    public:

        //=================================================
        // -ctors
        //=================================================

        explicit Reciprocal(Recip32u recip):
            m(recip.m),
            sh1(recip.sh1),
            sh2(recip.sh2),
            d(recip.d) {}

        explicit Reciprocal(vec4x32u d):
            Reciprocal(d, vec4x32u{32} - countl_zero(d - vec4x32u{1})) {}

    private:

        explicit Reciprocal(vec4x32u d, vec4x32u l):
            m(compute_m(l, d)),
            sh1(min(l, vec4x32u{1})),
            sh2(l - vec4x32u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operators
        //=================================================

        friend vec4x32u operator*(Reciprocal lhs, vec4x32u n) {
            auto t1 = mulhi(lhs.m, n);

            n -= t1;

            #if defined(AVEL_AVX2)
            vec4x32u tmp = n >> lhs.sh1;
            #else
            vec4x32u tmp = blend(n, n >> 1, lhs.sh1);
            #endif

            return (t1 + tmp) >> lhs.sh2;
        }

        friend vec4x32u operator*(vec4x32u lhs, Reciprocal rhs) {
            return rhs * lhs;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec4x32u m{};

        #if defined(AVEL_AVX2)
        vec4x32u sh1{};
        #else
        mask4x32u sh1{};
        #endif

        vec4x32u sh2{};
        vec4x32u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec4x32u mulhi(vec4x32u x, vec4x32u y) {
            vec4x32u lo{_mm_srli_si128(_mm_mul_epu32(x, y), 4)};
            vec4x32u hi{_mm_mul_epu32(_mm_srli_si128(x, 4), _mm_srli_si128(y, 4))};

            mask4x32u m{{false, true, false, true}};
            return blend(lo, hi, m);
        }

        static vec4x32u compute_m(vec4x32u l, vec4x32u d) {
            vec2x64u l_lo = bit_cast<vec2x64u>(l) & vec2x64u{0x00000000FFFFFFFF};
            vec2x64u l_hi = bit_cast<vec2x64u>(l) >> 32;

            vec2x64u d_lo = bit_cast<vec2x64u>(d) & vec2x64u{0x00000000FFFFFFFF};
            vec2x64u d_hi = bit_cast<vec2x64u>(d) >> 32;

            vec2x64u lo_part = (vec2x64u{0x100000000ul} << l_lo) / d_lo;
            lo_part -= vec2x64u{0xFFFFFFFFull};

            vec2x64u hi_part = (vec2x64u{0x100000000ul} << l_hi) / d_hi;
            hi_part -= vec2x64u{0xFFFFFFFFull};
            hi_part <<= 32;

            vec4x32u x = bit_cast<vec4x32u>(lo_part);
            vec4x32u y = bit_cast<vec4x32u>(hi_part);
            mask4x32u m{{false, true, false, true}};

            vec4x32u ret = blend(x, y, m);
            return ret;
        }

    };

    AVEL_FINL vec4x32u& operator*=(vec4x32u& lhs, Reciprocal<vec4x32u> rhs) {
        lhs = lhs * rhs;
        return lhs;
    }

}

#endif //AVEL_RECIPROCAL4X32U_HPP
