#ifndef AVEL_VEC8X16INT_HPP
#define AVEL_VEC8X16INT_HPP

namespace avel_impl {

    class Vec8x16int : public Vector128b {
        using base = Vector128b;
    public:

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int16x8_t;
        #endif

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        using mask = Vector_mask8xT;
        #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
        using mask = Vector_mask128b;
        #endif

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vec8x16int(primitive p):
            base(vreinterpretq_u8_s16(p)) {}
        #endif

        AVEL_FINL explicit Vec8x16int(base b):
            base(b) {}

        AVEL_FINL explicit Vec8x16int(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            base(_mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)))) {}
        #elif defined(AVEL_SSE2)
            base(_mm_sub_epi8(_mm_setzero_si128(), decay(m))) {}
        #endif

        #if defined(AVEL_NEON)
            base() {} //TODO: Implement
        #endif

        //=================================================
        // Assignment operators
        //=================================================

        using base::operator=;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vec8x16int lhs, Vec8x16int rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpeq_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi16(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vec8x16int lhs, Vec8x16int rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpneq_epu16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpeq_epi16(lhs.content, rhs.content)};
            #endif
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vec8x16int& operator+=(Vec8x16int rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vec8x16int& operator-=(Vec8x16int rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vec8x16int& operator*=(Vec8x16int rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_mullo_epi16(content, rhs.content);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vec8x16int& operator<<=(long long rhs) {
            #if defined(AVEL_SSE2)
            content  = _mm_sll_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vec8x16int& operator<<=(Vec8x16int rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm_sllv_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto t0 = _mm256_cvtepu16_epi32(content);
            auto t1 = _mm256_cvtepu16_epi32(decay(rhs));

            auto t2 = _mm256_and_si256(_mm256_sllv_epi32(t0, t1), _mm256_set1_epi32(0x0000FFFF));

            auto lo = t2;
            auto hi = _mm256_permute2f128_si256(lo, lo, 0x01);
            content = _mm256_castsi256_si128(_mm256_packus_epi32(lo, hi));

            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i < 4; ++i) {
                auto t0 = _mm_and_si128(rhs.content, _mm_set1_epi16(1u << i));
                auto m = _mm_cmplt_epi16(primitive{}, t0);

                auto a = _mm_andnot_si128(m, content);
                auto b = _mm_and_si128(m, _mm_sll_epi16(content, _mm_cvtsi64_si128(1u << i)));
                content = _mm_or_si128(a, b);
            }

            content = _mm_and_si128(content, _mm_cmplt_epi16(rhs.content, _mm_set1_epi16(16)));

            #endif
            return *this;
        }

    };

}

#endif //AVEL_VEC8X16INT_HPP
