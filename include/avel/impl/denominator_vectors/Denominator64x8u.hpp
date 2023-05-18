#ifndef AVEL_DENOMINATOR64X8U_HPP
#define AVEL_DENOMINATOR64X8U_HPP

namespace avel {

    using Denom64x8u = Denominator<vec64x8u>;

    template<>
    class  Denominator<vec64x8u> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = vec64x8u;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(Denom8u denom):
            m(denom.m),
            sh2(denom.sh2),
            d(denom.d) {}

        explicit Denominator(vec64x8u d):
            Denominator(d, bit_width(d - vec64x8u{1})) {}

    private:

        explicit Denominator(vec64x8u d, vec64x8u l):
            m(compute_m(d, l)),
            sh1(l),
            sh2(l - vec64x8u{sh1}),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<vec64x8u> div(vec64x8u n, Denominator denom) {
            vec64x8u t1 = mulhi(denom.m, n);
            vec64x8u t0 = (n - t1);
            vec64x8u t2 = blend(denom.sh1, bit_shift_right<1>(t0), t0);
            vec64x8u q = (t1 + t2) >> denom.sh2;

            vec64x8u r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend vec64x8u operator/(vec64x8u lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend vec64x8u operator%(vec64x8u lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        vec64x8u m{};
        mask64x8u sh1{};
        vec64x8u sh2{};
        vec64x8u d{};

        //=================================================
        // Helper functions
        //=================================================

        static vec64x8u mulhi(vec64x8u x, vec64x8u y) {
            #if defined(AVEL_AVX512BW)
            auto byte_mask = _mm512_set1_epi16(0x00FF);

            auto x_even = _mm512_and_si512(byte_mask, decay(x));
            auto x_odd  = _mm512_srli_epi16(decay(x), 8);

            auto y_even = _mm512_and_si512(byte_mask, decay(y));
            auto y_odd  = _mm512_srli_epi16(decay(y), 8);

            auto p_even = _mm512_mullo_epi16(x_even, y_even);
            auto p_odd  = _mm512_mullo_epi16(x_odd,  y_odd );

            auto ret_even = _mm512_srli_epi16(p_even, 8);
            auto ret_odd  = _mm512_andnot_si512(byte_mask, p_odd);

            auto ret = _mm512_or_si512(ret_even, ret_odd);
            return vec64x8u{ret};


            /* Old approach that's too heavy on the shuffles
            auto zeros = _mm512_setzero_si512();

            auto x_lo = _mm512_unpacklo_epi8(decay(x), zeros);
            auto x_hi = _mm512_unpackhi_epi8(decay(x), zeros);

            auto y_lo = _mm512_unpacklo_epi8(decay(y), zeros);
            auto y_hi = _mm512_unpackhi_epi8(decay(y), zeros);

            auto p_lo = _mm512_mullo_epi16(x_lo, y_lo);
            auto p_hi = _mm512_mullo_epi16(x_hi, y_hi);

            auto ret_lo = _mm512_srli_epi16(p_lo, 8);
            auto ret_hi = _mm512_srli_epi16(p_hi, 8);
            vec64x8u t1 = mulhi(denom.m, n);

            auto ret = _mm512_packus_epi16(ret_lo, ret_hi);
            return vec64x8u{ret};
            */

            #endif
        }

        static vec64x8u compute_m(vec64x8u d, vec64x8u l) {
            #if defined(AVEL_AVX512VBMI)
            alignas(256) constexpr static std::uint8_t table_data[256] {
                  0,   1,   1,  86,   1, 154,  86,  37,   1, 200, 154, 117,  86,  60,  37,  18,
                  1, 226, 200, 176, 154, 135, 117, 101,  86,  72,  60,  48,  37,  27,  18,   9,
                  1, 241, 226, 213, 200, 187, 176, 165, 154, 144, 135, 126, 117, 109, 101,  93,
                 86,  79,  72,  66,  60,  54,  48,  42,  37,  32,  27,  22,  18,  13,   9,   5,
                  1, 249, 241, 234, 226, 219, 213, 206, 200, 193, 187, 181, 176, 170, 165, 159,
                154, 149, 144, 139, 135, 130, 126, 121, 117, 113, 109, 105, 101,  97,  93,  89,
                 86,  82,  79,  75,  72,  69,  66,  63,  60,  57,  54,  51,  48,  45,  42,  40,
                 37,  34,  32,  29,  27,  25,  22,  20,  18,  15,  13,  11,   9,   7,   5,   3,
                  1, 253, 249, 245, 241, 237, 234, 230, 226, 223, 219, 216, 213, 209, 206, 203,
                200, 196, 193, 190, 187, 184, 181, 179, 176, 173, 170, 167, 165, 162, 159, 157,
                154, 152, 149, 147, 144, 142, 139, 137, 135, 132, 130, 128, 126, 123, 121, 119,
                117, 115, 113, 111, 109, 107, 105, 103, 101,  99,  97,  95,  93,  91,  89,  88,
                 86,  84,  82,  81,  79,  77,  75,  74,  72,  71,  69,  67,  66,  64,  63,  61,
                 60,  58,  57,  55,  54,  52,  51,  49,  48,  47,  45,  44,  42,  41,  40,  38,
                 37,  36,  34,  33,  32,  31,  29,  28,  27,  26,  25,  23,  22,  21,  20,  19,
                 18,  16,  15,  14,  13,  12,  11,  10,   9,   8,   7,   6,   5,   4,   3,   2
            };

            auto table0 = _mm512_load_si512(table_data +   0);
            auto table1 = _mm512_load_si512(table_data +  64);
            auto table2 = _mm512_load_si512(table_data + 128);
            auto table3 = _mm512_load_si512(table_data + 192);

            auto result0 = _mm512_permutex2var_epi8(table0, decay(d), table1);
            auto result1 = _mm512_permutex2var_epi8(table2, decay(d), table3);

            auto mask = _mm512_movepi8_mask(decay(d));
            auto result = _mm512_mask_blend_epi8(mask, result0, result1);
            return vec64x8u{result};

            #elif defined(AVEL_AVX512BW)
            vec32x16u l_lo = bit_cast<vec32x16u>(l) & vec32x16u{0x00FF};
            vec32x16u l_hi = bit_cast<vec32x16u>(l) >> 8;

            vec32x16u d_lo = bit_cast<vec32x16u>(d) & vec32x16u{0x00FF};
            vec32x16u d_hi = bit_cast<vec32x16u>(d) >> 8;

            vec32x16u tmp0_lo = (vec32x16u{1} << l_lo) - d_lo;
            vec32x16u tmp0_hi = (vec32x16u{1} << l_hi) - d_hi;

            vec32x16u tmp1_lo = (tmp0_lo << 8) / d_lo;
            vec32x16u tmp1_hi = (tmp0_hi << 8) / d_hi;

            vec64x8u m_lo = bit_cast<vec64x8u>(tmp1_lo);
            vec64x8u m_hi = bit_cast<vec64x8u>(tmp1_hi << 8);

            vec64x8u m = m_lo | m_hi;
            m += vec64x8u{1};
            return m;
            #endif
        }

        /* Older lookup table approach meant to use a smaller lookup table
        vec64x8u compute_m_lookup(vec64x8u d) {
            alignas(128) constexpr static std::uint8_t table_data[128] {
                1, 86,154, 37,200,117, 60, 18,
                226,176,135,101, 72, 48, 27,  9,
                241,213,187,165,144,126,109, 93,
                79, 66, 54, 42, 32, 22, 13,  5,
                249,234,219,206,193,181,170,159,
                149,139,130,121,113,105, 97, 89,
                82, 75, 69, 63, 57, 51, 45, 40,
                34, 29, 25, 20, 15, 11,  7,  3,
                253,245,237,230,223,216,209,203,
                196,190,184,179,173,167,162,157,
                152,147,142,137,132,128,123,119,
                115,111,107,103, 99, 95, 91, 88,
                84, 81, 77, 74, 71, 67, 64, 61,
                58, 55, 52, 49, 47, 44, 41, 38,
                36, 33, 31, 28, 26, 23, 21, 19,
                16, 14, 12, 10,  8,  6,  4,  2,
            };

            auto table0 = _mm512_load_si512(table_data +  0);
            auto table1 = _mm512_load_si512(table_data + 64);

            auto indices = compute_indices(d);

            auto result = _mm512_permutex2var_epi8(table0, decay(indices), table1);
            return vec64x8u{result};
        }

        vec64x8u compute_indices(vec64x8u x) {
            auto tz_mask = _mm512_andnot_si512(decay(x), _mm512_add_epi8(decay(x), _mm512_set1_epi8(-1)));
            auto tzcnt = _mm512_popcnt_epi8(tz_mask);

            //TODO: Optimize this
            auto zeros = _mm512_setzero_si512();
            auto tzcnt_lo = _mm512_unpacklo_epi8(tzcnt, zeros);
            auto tzcnt_hi = _mm512_unpackhi_epi8(tzcnt, zeros);

            auto x_lo = _mm512_unpacklo_epi8(decay(x), zeros);
            auto x_hi = _mm512_unpackhi_epi8(decay(x), zeros);

            auto r_lo = _mm512_srlv_epi16(x_lo, tzcnt_lo);
            auto r_hi = _mm512_srlv_epi16(x_hi, tzcnt_hi);

            auto ret = _mm512_packus_epi16(r_lo, r_hi);

            // This may erroneously set the high bit of even-indexed bytes,
            // however that bit is not taken into consideration by the vpermi2b
            // instruction so it's fine
            ret = _mm512_srli_epi16(ret, 1);
            return vec64x8u{ret};
        }
        */
        
    };

    AVEL_FINL vec64x8u& operator/=(vec64x8u& lhs, Denom64x8u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL vec64x8u& operator%=(vec64x8u& lhs, Denom64x8u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR64X8U_HPP
