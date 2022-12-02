#ifndef AVEL_VEC16X8INT_HPP
#define AVEL_VEC16X8INT_HPP

namespace avel_impl {

    class Vec16x8int : public Vector128b {
        using base = Vector128b;
    public:

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int8x16_t;
        #endif

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        using mask = Vector_mask16xT;
        #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
        using mask = Vector_mask128b;
        #endif

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vec16x8int(primitive p):
            base(vreinterpretq_u8_s8(p)) {}
        #endif

        AVEL_FINL explicit Vec16x8int(base b):
            base(b) {}

        AVEL_FINL explicit Vec16x8int(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            base(_mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)))) {}
        #elif defined(AVEL_SSE2)
            base(_mm_sub_epi8(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            base(vsubq_u8(vdupq_n_u8(0x00), decay(m))) {}
        #endif

        //=================================================
        // Assignment operators
        //=================================================

        using base::operator=;

        #if defined(AVEL_NEON)
        Vec16x8int& operator=(primitive p) {
            content = vreinterpretq_u8_s8(p);
            return *this;
        }
        #endif

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vec16x8int lhs, Vec16x8int rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpeq_epi8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi8(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_s8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vec16x8int lhs, Vec16x8int rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpneq_epu8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpeq_epi8(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vmvnq_u8(vceqq_s8(decay(lhs), decay(rhs)))};
            #endif
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vec16x8int& operator+=(Vec16x8int rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi8(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            *this = vaddq_s8(decay(*this), decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vec16x8int& operator-=(Vec16x8int rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi8(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            *this = vsubq_s8(decay(*this), decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vec16x8int& operator*=(Vec16x8int rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m256i a = _mm256_cvtepu8_epi16(content);
            __m256i b = _mm256_cvtepu8_epi16(rhs.content);

            __m256i c = _mm256_mullo_epi16(a, b);
            content = _mm256_cvtepi16_epi8(c);

            #elif defined(AVEL_AVX2)
            auto lhs_whole = _mm256_cvtepu8_epi16(content);
            auto rhs_whole = _mm256_cvtepu8_epi16(rhs.content);

            auto mask = _mm256_set1_epi16(0x00FF);
            auto product = _mm256_mullo_epi16(lhs_whole, rhs_whole);
            auto masked = _mm256_and_si256(product, mask);
            auto packed = _mm256_packus_epi16(masked, _mm256_zextsi128_si256(_mm256_extractf128_si256(masked, 0x1)));

            content = _mm256_castsi256_si128(packed);

            #elif defined(AVEL_SSE2)
            primitive zeros = _mm_setzero_si128();
            primitive byte_mask = _mm_set1_epi16(0x00FF);
            primitive lhs_whole = content;
            primitive rhs_whole = rhs.content;

            primitive lhs_lo = _mm_unpacklo_epi8(lhs_whole, zeros);
            primitive lhs_hi = _mm_unpackhi_epi8(lhs_whole, zeros);

            primitive rhs_lo = _mm_unpacklo_epi8(rhs_whole, zeros);
            primitive rhs_hi = _mm_unpackhi_epi8(rhs_whole, zeros);

            primitive out_lo = _mm_mullo_epi16(lhs_lo, rhs_lo);
            primitive out_hi = _mm_mullo_epi16(lhs_hi, rhs_hi);

            out_lo = _mm_and_si128(byte_mask, out_lo);
            out_hi = _mm_and_si128(byte_mask, out_hi);

            content = _mm_packus_epi16(out_lo, out_hi);

            /* Old implementation
            primitive lo_mask = _mm_set1_epi16(0x00FF);
            primitive hi_mask = _mm_set1_epi16(0xFF00);

            primitive lhs_lo = _mm_and_si128(lo_mask, content);
            primitive lhs_hi = _mm_srli_epi16(content, 8);

            primitive rhs_lo = _mm_and_si128(lo_mask, rhs.content);
            primitive rhs_hi = _mm_srli_epi16(rhs.content, 8);

            primitive prod_lo = _mm_mullo_epi16(lhs_lo, rhs_lo);
            primitive prod_hi = _mm_mullo_epi16(rhs_lo, rhs_lo);

            content = decay(blend(
                Vector{prod_lo},
                Vector{_mm_slli_epi16(prod_hi, 8)},
                mask{_mm_set1_epi16(0xFF00)}
            ));
            */
            #endif

            #if defined(AVEL_NEON)
            *this = vmulq_s8(decay(*this), decay(rhs));
            #endif

            return *this;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vec16x8int& operator<<=(long long rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m256i tmp = _mm256_cvtepi8_epi16(content);
            tmp = _mm256_sll_epi16(tmp, _mm_cvtsi32_si128(rhs));
            content = _mm256_cvtepi16_epi8(tmp);

            #elif defined(AVEL_SSE2)
            primitive s = _mm_cvtsi32_si128(rhs);

            primitive shifted = _mm_sll_epi16(content, s);
            primitive mask = _mm_set1_epi8(std::uint8_t(0xFF << rhs));
            content = _mm_and_si128(mask, shifted);

            #endif

            #if defined(AVEL_NEON)
            auto s = (std::uint64_t(rhs) > 0xFF) ? 0xFF : static_cast<std::uint8_t>(rhs);
            *this = vshlq_s8(decay(*this), vdupq_n_s8(s));

            #endif

            return *this;
        }

        AVEL_FINL Vec16x8int& operator<<=(Vec16x8int rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m256i whole = _mm256_cvtepu8_epi16(content);
            __m256i shifts = _mm256_cvtepu8_epi16(rhs.content);
            content = _mm256_cvtepi16_epi8(_mm256_sllv_epi16(whole, shifts));

            #elif defined(AVEL_AVX2)
            auto lhs_whole = _mm256_cvtepu8_epi16(content);
            auto rhs_whole = _mm256_cvtepu8_epi16(rhs.content);

            auto mask = _mm256_set1_epi16(0x00FF);
            auto shifted = _mm256_sllv_epi16(lhs_whole, rhs_whole);
            auto masked = _mm256_and_si256(shifted, mask);
            auto packed = _mm256_packus_epi16(masked, _mm256_zextsi128_si256(_mm256_extractf128_si256(masked, 0x1)));

            content = _mm256_castsi256_si128(packed);

            //content = _mm256_castsi256_si128(packed);
            //__m256i whole = _mm256_cvtepu8_epi16(content);
            //__m256i shifts = _mm256_cvtepu8_epi16(rhs.content);
            //__m256i truncated = _mm256_and_si256(_mm256_set1_epi16(0x00FF), _mm256_sllv_epi16(whole, shifts));
            //content = _mm256_castsi256_si128(_mm256_packs_epi16(truncated, _mm256_undefined_si256()));

            #elif defined(AVEL_SSSE3)
            alignas(16) static constexpr std::uint8_t table_data[16] {
                0x01, 0x02, 0x04, 0x08,
                0x10, 0x20, 0x40, 0x80,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00
            };

            primitive zeros = _mm_setzero_si128();
            primitive table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));

            primitive lhs_lo = _mm_unpacklo_epi8(content, zeros);
            primitive lhs_hi = _mm_unpackhi_epi8(content, zeros);

            primitive valid_mask = _mm_cmplt_epi8(_mm_set1_epi8(0x7), decay(rhs));
            primitive indices = _mm_or_si128(decay(rhs), valid_mask);
            primitive shifts = _mm_shuffle_epi8(table, indices);

            primitive rhs_lo = _mm_unpacklo_epi8(shifts, zeros);
            primitive rhs_hi = _mm_unpackhi_epi8(shifts, zeros);

            lhs_lo = _mm_mullo_epi16(lhs_lo, rhs_lo);
            lhs_hi = _mm_mullo_epi16(lhs_hi, rhs_hi);

            primitive byte_mask = _mm_set1_epi16(0xFF);
            lhs_lo = _mm_and_si128(lhs_lo, byte_mask);
            lhs_hi = _mm_and_si128(lhs_hi, byte_mask);

            content = _mm_packus_epi16(lhs_lo, lhs_hi);

            #elif defined(AVEL_SSE2)
            alignas(16) std::int8_t data[16];
            alignas(16) std::int8_t shifts[16];

            _mm_store_si128(reinterpret_cast<__m128i*>(data), content);
            _mm_store_si128(reinterpret_cast<__m128i*>(shifts), rhs.content);

            data[0x0] <<= shifts[0x0];
            data[0x1] <<= shifts[0x1];
            data[0x2] <<= shifts[0x2];
            data[0x3] <<= shifts[0x3];
            data[0x4] <<= shifts[0x4];
            data[0x5] <<= shifts[0x5];
            data[0x6] <<= shifts[0x6];
            data[0x7] <<= shifts[0x7];
            data[0x8] <<= shifts[0x8];
            data[0x9] <<= shifts[0x9];
            data[0xa] <<= shifts[0xa];
            data[0xb] <<= shifts[0xb];
            data[0xc] <<= shifts[0xc];
            data[0xd] <<= shifts[0xd];
            data[0xe] <<= shifts[0xe];
            data[0xf] <<= shifts[0xf];

            content = _mm_load_si128(reinterpret_cast<const __m128i*>(data));

            /* Older attempt to produce vectorized version
            primitive zeroes = _mm_setzero_si128();
            primitive byte_mask = _mm_set1_epi16(0x00FF);

            primitive lhs_lo = _mm_unpacklo_epi8(content, zeroes);
            primitive lhs_hi = _mm_unpacklo_epi8(content, zeroes);

            primitive rhs_lo = _mm_unpacklo_epi8(content, zeroes);
            primitive rhs_hi = _mm_unpacklo_epi8(content, zeroes);

            for (unsigned i = 0; i < 3; ++i) {
                auto test_bit = _mm_set1_epi8(1u << i);
                auto t0_lo = _mm_and_si128(rhs_lo, test_bit);
                auto t0_hi = _mm_and_si128(rhs_hi, test_bit);

                auto m0 = _mm_cmplt_epi16(zeroes, t0_lo);

                auto lo_a = lhs_lo;
                auto lo_b = _mm_slli_epi16(lhs_lo, 1);
                auto lo_blend_a = _mm_andnot_si128(m0, lo_a);
                auto lo_blend_b = _mm_and_si128(m0, lo_b);
                lhs_lo = _mm_or_si128(lo_blend_a, lo_blend_b);

                auto m1 = _mm_cmplt_epi16(zeroes, t0_hi);
                auto hi_a = lhs_hi;
                auto hi_b = _mm_slli_epi16(lhs_hi, 1);
                auto hi_blend_a = _mm_andnot_si128(m1, hi_a);
                auto hi_blend_b = _mm_and_si128(m1, hi_b);
                lhs_hi = _mm_or_si128(hi_blend_a, hi_blend_b);
            }

            lhs_lo = _mm_and_si128(lhs_lo, _mm_cmplt_epi16(rhs_lo, _mm_set1_epi8(8u)));
            lhs_hi = _mm_and_si128(lhs_hi, _mm_cmplt_epi16(rhs_hi, _mm_set1_epi8(8u)));

            lhs_lo = _mm_and_si128(lhs_lo, byte_mask);
            lhs_hi = _mm_and_si128(lhs_hi, byte_mask);

            content = _mm_packs_epi16(lhs_lo, lhs_hi);
            */

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u8(content, decay(rhs));
            #endif

            return *this;
        }

        //=================================================
        // Conversion operators
        //=================================================

        #if defined(AVEL_NEON)
        [[nodiscard]]
        AVEL_FINL explicit operator int8x16_t() const {
            return vreinterpretq_s8_u8(content);
        }
        #endif

    };

}

#endif //AVEL_VEC16X8INT_HPP
