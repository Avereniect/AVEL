#ifndef AVEL_VEC4X32INT_HPP
#define AVEL_VEC4X32INT_HPP

namespace avel_impl {

    #if defined(AVEL_SSE2) || defined(AVEL_NEON)
    class Vec4x32int : public Vector128b {
        using base = Vector128b;
    #endif
    public:

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int32x4_t;
        #endif

        #if defined(AVEL_AVX512VL)
        using mask = Vector_mask8xT;
        #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
        using mask = Vector_mask128b;
        #endif

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        AVEL_FINL explicit Vec4x32int(base b):
            base(b) {}

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vec4x32int(primitive p):
            base(vreinterpretq_u8_s32(p)) {}
        #endif

        AVEL_FINL explicit Vec4x32int(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
            base(_mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi32(decay(m)))) {}
        #elif defined(AVEL_AVX512VL)
            base([] (mask m) {
                auto undef = _mm_undefined_si128();
                return _mm_maskz_ternarylogic_epi32(decay(m), undef, undef, undef, 0xFF);
            } (m)) {}
        #elif defined(AVEL_SSE2)
            base(_mm_sub_epi32(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            base(vreinterpretq_u8_u32(vsubq_u32(vdupq_n_u32(0x00), vreinterpretq_u32_u8(decay(m))))) {}
        #endif

        Vec4x32int() = default;
        Vec4x32int(const Vec4x32int&) = default;
        Vec4x32int(Vec4x32int&&) noexcept = default;
        ~Vec4x32int() = default;

        //=================================================
        // Assignment operators
        //=================================================

        using base::operator=;

        Vec4x32int& operator=(const Vec4x32int&) = default;
        Vec4x32int& operator=(Vec4x32int&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vec4x32int lhs, Vec4x32int rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi32(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vreinterpretq_u8_u32(vceqq_s32(decay(lhs), decay(rhs)))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vec4x32int lhs, Vec4x32int rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epu32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpeq_epi32(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return !mask{vreinterpretq_u8_u32(vceqq_s32(decay(lhs), decay(rhs)))};
            #endif
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vec4x32int& operator+=(Vec4x32int rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi32(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vreinterpretq_u8_s32(vaddq_s32(decay(*this), decay(rhs)));
            #endif

            return *this;
        }

        AVEL_FINL Vec4x32int& operator-=(Vec4x32int rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi32(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vreinterpretq_u8_s32(vsubq_s32(decay(*this), decay(rhs)));
            #endif

            return *this;
        }

        AVEL_FINL Vec4x32int& operator*=(Vec4x32int rhs) {
            #if defined(AVEL_SSE41)
            content = _mm_mullo_epi32(content, decay(rhs));
            #elif defined(AVEL_SSE2)
            auto lhs_lo = content;
            auto lhs_hi = _mm_srli_si128(content, 4);

            auto rhs_lo = decay(rhs);
            auto rhs_hi = _mm_srli_si128(decay(rhs), 4);

            auto lo = _mm_mul_epu32(lhs_lo, rhs_lo);
            auto hi = _mm_mul_epu32(lhs_hi, rhs_hi);

            auto half0 = _mm_unpacklo_epi32(lo, hi);
            auto half1 = _mm_unpackhi_epi32(lo, hi);

            content = _mm_unpacklo_epi64(half0, half1);

            #endif

            #if defined(AVEL_NEON)
            content = vreinterpretq_u8_s32(vmulq_s32(decay(*this), decay(rhs)));
            #endif

            return *this;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vec4x32int& operator<<=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi32(content, _mm_cvtsi64_si128(rhs));

            #elif defined(AVEL_NEON)
            auto s = (std::uint64_t(rhs) > 0xFFFFFFFF) ? 0xFFFFFFFF : static_cast<std::uint32_t>(rhs);
            content = vreinterpretq_u8_s32(vshlq_s32(decay(*this), vdupq_n_s32(s)));

            #endif
            return *this;
        }

        AVEL_FINL Vec4x32int& operator<<=(Vec4x32int rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi32(content, rhs.content);

            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i  < 5; ++i) {
                auto s = 1u << i;
                auto t0 = _mm_and_si128(decay(rhs), _mm_set1_epi32(s));
                auto m = _mm_cmplt_epi32(_mm_setzero_si128(), t0);
                auto a = content;
                auto b = _mm_sll_epi32(a, _mm_cvtsi32_si128(s));
                content = _mm_or_si128(_mm_andnot_si128(m, a), _mm_and_si128(m, b));
            }

            content = _mm_and_si128(content, _mm_cmplt_epi32(decay(rhs), _mm_set1_epi32(32u)));

            #elif defined(AVEL_NEON)
            content = vreinterpretq_u8_s32(vshlq_s32(decay(*this), decay(rhs)));

            #endif

            return *this;
        }

        //=================================================
        // Conversion operators
        //=================================================

        #if defined(AVEL_NEON)
        [[nodiscard]]
        AVEL_FINL explicit operator uint32x4_t() const {
            return vreinterpretq_u32_u8(content);
        }
        #endif

    };

}

#endif //AVEL_VEC4X32INT_HPP
