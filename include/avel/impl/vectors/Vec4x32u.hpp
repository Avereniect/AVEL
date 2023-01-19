#ifndef AVEL_VEC4X32U_HPP
#define AVEL_VEC4X32U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x32u = Vector<std::uint32_t, 4>;

    using arr4x32u = std::array<std::uint32_t, 4>;

    using mask4x32u = Vector_mask<std::uint32_t, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec4x32u> div(vec4x32u numerator, vec4x32u denominator);
    vec4x32u broadcast_mask(mask4x32u m);
    vec4x32u blend(mask4x32u m, vec4x32u a, vec4x32u b);
    vec4x32u countl_one(vec4x32u x);





    template<>
    class Vector_mask<std::uint32_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint32x4_t;
        #endif

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> m):
            Vector_mask(convert<Vector_mask>(m)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive p):
            content(p) {}

        AVEL_FINL explicit Vector_mask(bool b):
            #if defined(AVEL_AVX512VL)
            content(b ? 0xF : 0x00) {}
            #elif defined(AVEL_SSE2)
            content(b ? _mm_set1_epi16(-1) : _mm_setzero_si128()) {}
            #endif
            #if defined(AVEL_NEON)
            content(vmovq_n_u32(b ? -1 : 0)) {}
            #endif

        AVEL_FINL explicit Vector_mask(const arr4xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            content = __mmask8(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            auto expanded = _mm_cvtepi8_epi32(array_data);
            content = _mm_cmplt_epu32_mask(_mm_setzero_si128(), expanded);

            #elif defined(AVEL_SSE2)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            array_data = _mm_unpacklo_epi8(array_data, array_data);
            array_data = _mm_unpacklo_epi16(array_data, array_data);
            content = _mm_cmplt_epi32(_mm_setzero_si128(), array_data);

            #endif

            #if defined(AVEL_NEON)
            auto array_data = vsetq_lane_u32(
                bit_cast<std::uint32_t>(arr),
                vdupq_n_u32(0x00),
                0x00
            );

            auto zeros = vdupq_n_u8(0x00);
            auto t0 = vreinterpretq_u8_u32(array_data);
            auto mask0 = vcltq_u8(zeros, t0);
            auto mask1 = vzipq_u8(mask0, mask0).val[0];
            auto mask2 = vzipq_u8(mask1, mask1).val[0];

            content = vreinterpretq_u32_u8(mask2);
            #endif
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector_mask& operator=(bool b) {
            #if defined(AVEL_AVX512VL)
            content = b ? 0xF : 0x0;
            #elif defined(AVEL_SSE2)
            content = b ? _mm_set1_epi32(-1) : _mm_setzero_si128();
            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_u32(b ? -1 : 0);
            #endif

            return *this;
        }

        Vector_mask& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return decay(lhs) == decay(rhs);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));
            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u32(vceqq_u32(decay(lhs), decay(rhs)));
            return min == 0xFFFFFFFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return decay(lhs) != decay(rhs);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u32(vceqq_u32(decay(lhs), decay(rhs)));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(vreinterpretq_u64_u32(decay(lhs)), vreinterpretq_u64_u32(decay(rhs)));
            auto shifted = vqshlq_n_u64(diff, 63);
            auto shiftedx4 = vreinterpretq_u32_u64(shifted);
            auto x = vgetq_lane_u32(shiftedx4, 0x1);
            auto y = vgetq_lane_u32(shiftedx4, 0x3);
            return (x | y) >> 31;

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content &= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = vandq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content |= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = (content ^ rhs.content) & 0xF;

            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = veorq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{primitive(content ^ 0x0F)};
            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(undef, undef))};
            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vmvnq_u32(content)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {
            lhs ^= rhs;
            return lhs;
        }

        //=================================================
        // Conversion operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask4x32u m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(decay(m)));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m))) / sizeof(std::uint32_t);
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vnegq_s32(vreinterpretq_s32_u32(decay(m)));
        return static_cast<std::uint32_t>(vaddvq_s32(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(decay(m))));
        auto t1 = vpadd_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpadd_u32(t1, t1);

        return static_cast<std::uint32_t>(vget_lane_u32(t2, 0));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x32u m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vmaxvq_u32(decay(m)) != 0x00;

        #elif defined(AVEL_NEON)
        auto t0 = decay(m);
        auto t1 = vpmax_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmax_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) != 0x00;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x32u m) {
        #if defined(AVEL_AVX512VL)
        return 0xF == _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE41)
        return _mm_test_all_ones(decay(m));
        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vminvq_u8(vreinterpretq_u8_u32(decay(m))) == 0xFF;

        #elif defined(AVEL_NEON)
        auto t0 = decay(m);
        auto t1 = vpmin_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmin_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) == 0xFFFFFFFF;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x32u m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask4x32u>(mask4x32u m) {
        return std::array<mask4x32u, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 4> convert<mask1x8u, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x8u, 4> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        ret = bit_cast<std::array<mask1x8u, 4>>(_mm_cvtsi128_si32(_mm_movm_epi8(decay(m))));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x8u, 4>>(_mm_cvtsi128_si32(t2));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x8u, 4>>(_mm_cvtsi128_si32(t2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 4> convert<mask1x8i, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x8i, 4> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        ret = bit_cast<std::array<mask1x8i, 4>>(_mm_cvtsi128_si32(_mm_movm_epi8(decay(m))));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x8i, 4>>(_mm_cvtsi128_si32(t2));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x8i, 4>>(_mm_cvtsi128_si32(t2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 4> convert<mask1x16u, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x16u, 4> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        ret = bit_cast<std::array<mask1x16u, 4>>(_mm_cvtsi128_si32(_mm_movm_epi8(decay(m))));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x16u, 4>>(_mm_cvtsi128_si32(t2));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x16u, 4>>(_mm_cvtsi128_si32(t2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 4> convert<mask1x16i, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x16i, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        ret = bit_cast<std::array<mask1x16i, 4>>(_mm_cvtsi128_si32(_mm_movm_epi8(decay(m))));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x16i, 4>>(_mm_cvtsi128_si32(t2));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x16i, 4>>(_mm_cvtsi128_si32(t2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 4> convert<mask1x32u, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x32u, 4> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        ret = bit_cast<std::array<mask1x32u, 4>>(_mm_cvtsi128_si32(_mm_movm_epi8(decay(m))));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x32u, 4>>(_mm_cvtsi128_si32(t2));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x32u, 4>>(_mm_cvtsi128_si32(t2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 4> convert<mask1x32i, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x32i, 4> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        ret = bit_cast<std::array<mask1x32i, 4>>(_mm_cvtsi128_si32(_mm_movm_epi8(decay(m))));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x32i, 4>>(_mm_cvtsi128_si32(t2));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x32i, 4>>(_mm_cvtsi128_si32(t2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64u, 4> convert<mask1x64u, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x64u, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        ret = bit_cast<std::array<mask1x64u, 4>>(_mm_cvtsi128_si32(_mm_movm_epi8(decay(m))));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x64u, 4>>(_mm_cvtsi128_si32(t2));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x64u, 4>>(_mm_cvtsi128_si32(t2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 4> convert<mask1x64i, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x64i, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        ret = bit_cast<std::array<mask1x64i, 4>>(_mm_cvtsi128_si32(_mm_movm_epi8(decay(m))));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x64i, 4>>(_mm_cvtsi128_si32(t2));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x64i, 4>>(_mm_cvtsi128_si32(t2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 4> convert<mask1x32f, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x32f, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        ret = bit_cast<std::array<mask1x32f, 4>>(_mm_cvtsi128_si32(_mm_movm_epi8(decay(m))));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x32f, 4>>(_mm_cvtsi128_si32(t2));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x32f, 4>>(_mm_cvtsi128_si32(t2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64f, 4> convert<mask1x64f, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x64f, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        ret = bit_cast<std::array<mask1x64f, 4>>(_mm_cvtsi128_si32(_mm_movm_epi8(decay(m))));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        ret = bit_cast<std::array<mask1x64f, 4>>(_mm_cvtsi128_si32(t0));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        ret = bit_cast<std::array<mask1x64f, 4>>(_mm_cvtsi128_si32(t2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask4x32u>(mask4x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto t0 = _mm_movm_epi32(decay(m));
        return {mask16x8u{_mm_cvtepi32_epi8(t0)}};

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_maskz_set1_epi32(decay(m), 0xFFFFFFFF);
        return {mask16x8u{_mm_cvtepi32_epi8(t0)}};

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_packs_epi32(decay(m), _mm_setzero_si128());
        auto tmp1 = _mm_packs_epi16(tmp0, _mm_setzero_si128());
        return {mask16x8u{tmp1}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask4x32u>(mask4x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto t0 = _mm_movm_epi32(decay(m));
        return {mask16x8i{_mm_cvtepi32_epi8(t0)}};

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_maskz_set1_epi32(decay(m), 0xFFFFFFFF);
        return {mask16x8i{_mm_cvtepi32_epi8(t0)}};

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_packs_epi32(decay(m), _mm_setzero_si128());
        auto tmp1 = _mm_packs_epi16(tmp0, _mm_setzero_si128());
        return {mask16x8i{tmp1}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask4x32u>(mask4x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_AVX512VL)
        return {mask8x16u{_mm_cvtepi32_epi16(_mm_maskz_set1_epi32(decay(m), 0xFFFFFFFF))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16u{_mm_packs_epi32(decay(m), _mm_setzero_si128())}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask4x32u>(mask4x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(decay(m))}};

        #elif defined(AVEL_AVX512VL)
        return {mask8x16i{_mm_cvtepi32_epi16(_mm_maskz_set1_epi32(decay(m), 0xFFFFFFFF))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_packs_epi32(decay(m), _mm_setzero_si128())}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask1x8u>(mask1x8u m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32u{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32u{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask1x8i>(mask1x8i m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32u{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32u{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask1x16u>(mask1x16u m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32u{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32u{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask1x16i>(mask1x16i m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32u{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32u{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask1x32u>(mask1x32u m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32u{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32u{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask1x32i>(mask1x32i m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32u{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32u{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask1x64u>(mask1x64u m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32u{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32u{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask1x64i>(mask1x64i m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32u{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32u{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask1x32f>(mask1x32f m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32u{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32u{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask1x64f>(mask1x64f m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32u{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32u{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 4> convert<mask4x32u, mask16x8u>(mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {
            mask4x32u{mask4x32u::primitive(decay(m) & 0xF)},
            mask4x32u{mask4x32u::primitive((decay(m) >> 0x4) & 0xF)},
            mask4x32u{mask4x32u::primitive((decay(m) >> 0x8) & 0xF)},
            mask4x32u{mask4x32u::primitive((decay(m) >> 0xC))}
        };

        #elif defined(AVEL_AVX512VL)
        auto tmp0 = _mm_movemask_epi8(decay(m));
        return {
            mask4x32u{mask4x32u::primitive(tmp0 & 0xF)},
            mask4x32u{mask4x32u::primitive((tmp0 >> 0x4) & 0xF)},
            mask4x32u{mask4x32u::primitive((tmp0 >> 0x8) & 0xF)},
            mask4x32u{mask4x32u::primitive((tmp0 >> 0xC))}
        };

        #elif defined(AVEL_SSE2)
        auto half0 = _mm_unpacklo_epi8(decay(m), decay(m));
        auto half1 = _mm_unpackhi_epi8(decay(m), decay(m));

        return {
            mask4x32u{_mm_unpacklo_epi16(half0, half0)},
            mask4x32u{_mm_unpackhi_epi16(half0, half0)},
            mask4x32u{_mm_unpacklo_epi16(half1, half1)},
            mask4x32u{_mm_unpackhi_epi16(half1, half1)}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 4> convert<mask4x32u, mask16x8i>(mask16x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {
            mask4x32u{mask4x32u::primitive(decay(m) & 0xF)},
            mask4x32u{mask4x32u::primitive((decay(m) >> 0x4) & 0xF)},
            mask4x32u{mask4x32u::primitive((decay(m) >> 0x8) & 0xF)},
            mask4x32u{mask4x32u::primitive((decay(m) >> 0xC))}
        };

        #elif defined(AVEL_AVX512VL)
        auto tmp0 = _mm_movemask_epi8(decay(m));
        return {
            mask4x32u{mask4x32u::primitive(tmp0 & 0xF)},
            mask4x32u{mask4x32u::primitive((tmp0 >> 0x4) & 0xF)},
            mask4x32u{mask4x32u::primitive((tmp0 >> 0x8) & 0xF)},
            mask4x32u{mask4x32u::primitive((tmp0 >> 0xC))}
        };

        #elif defined(AVEL_SSE2)
        auto half0 = _mm_unpacklo_epi8(decay(m), decay(m));
        auto half1 = _mm_unpackhi_epi8(decay(m), decay(m));

        return {
            mask4x32u{_mm_unpacklo_epi16(half0, half0)},
            mask4x32u{_mm_unpackhi_epi16(half0, half0)},
            mask4x32u{_mm_unpacklo_epi16(half1, half1)},
            mask4x32u{_mm_unpackhi_epi16(half1, half1)}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 2> convert<mask4x32u, mask8x16u>(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto tmp0 = decay(m);
        return {
            mask4x32u{mask4x32u::primitive(tmp0 & 0xF)},
            mask4x32u{mask4x32u::primitive(tmp0 >> 4)}
        };

        #elif defined(AVEL_AVX512VL)
        auto tmp0 = _mm_movemask_epi8(decay(m));
        auto tmp1 = _pext_u32(tmp0, 0xAAAA);

        return {
            mask4x32u{mask4x32u::primitive(tmp1 & 0xFF)},
            mask4x32u{mask4x32u::primitive(tmp1 >> 8)}
        };

        #elif defined(AVEL_SSE2)
        return {
            mask4x32u{_mm_unpacklo_epi16(decay(m), decay(m))},
            mask4x32u{_mm_unpackhi_epi16(decay(m), decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 2> convert<mask4x32u, mask8x16i>(mask8x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto tmp0 = decay(m);
        return {
            mask4x32u{mask4x32u::primitive(tmp0 & 0xF)},
            mask4x32u{mask4x32u::primitive(tmp0 >> 4)}
        };

        #elif defined(AVEL_AVX512VL)
        auto tmp0 = _mm_movemask_epi8(decay(m));
        auto tmp1 = _pext_u32(tmp0, 0xAAAA);

        return {
            mask4x32u{mask4x32u::primitive(tmp1 & 0xFF)},
            mask4x32u{mask4x32u::primitive(tmp1 >> 8)}
        };

        #elif defined(AVEL_SSE2)
        return {
            mask4x32u{_mm_unpacklo_epi16(decay(m), decay(m))},
            mask4x32u{_mm_unpackhi_epi16(decay(m), decay(m))}
        };
        #endif
    }





    template<>
    class Vector<std::uint32_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint32_t;

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint32x4_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
        using rebind_width = Vector<scalar, M>;

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            Vector(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
            content(_mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi32(decay(m)))) {}
        #elif defined(AVEL_AVX512VL)
            content(_mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(1))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi32(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(decay(m))))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi32(x)) {}
        #endif
        #if defined(AVEL_NEON)
            Vector(vdupq_n_u32(x)) {}
        #endif


        AVEL_FINL explicit Vector(const arr4x32u& array):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_u32(array.data())) {}
        #endif

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_AVX2)
            content = _mm_broadcastd_epi32(_mm_cvtsi32_si128(x));
            #elif defined(AVEL_SSE2)
            content = _mm_set1_epi32(x);
            #endif

            #if defined(AVEL_NEON)
            *this = vdupq_n_u32(x);
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi32(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_u32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpeq_epi32(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmplt_epu32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            Vector add_mask{0x80000000};
            return mask{_mm_cmplt_epi32(decay(lhs ^ add_mask), decay(rhs ^ add_mask))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_u32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epu32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE41)
            auto mins = _mm_min_epu32(decay(lhs), decay(rhs));
            return mask{_mm_cmpeq_epi32(mins, decay(lhs))};
            #elif defined(AVEL_SSE2)
            return !mask{lhs > rhs};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_u32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epu32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            Vector add_mask{0x80000000};
            return mask{_mm_cmpgt_epi32(decay(lhs ^ add_mask), decay(rhs ^ add_mask))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_u32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epu32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE41)
            auto mins = _mm_min_epu32(decay(lhs), decay(rhs));
            return mask{_mm_cmpeq_epi32(mins, decay(rhs))};

            #elif defined(AVEL_SSE2)
            return !mask{lhs < rhs};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_u32(decay(lhs), decay(rhs))};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        //Definition of operator-() deferred until after definition of vec4x32i

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi32(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_u32(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi32(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_u32(content, rhs.content);
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
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
            content = vmulq_u32(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.quot.content;
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector rhs) {
            auto results = div(*this, rhs);
            content = results.rem.content;
            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend Vector operator+(Vector lhs, Vector rhs) {
            lhs += rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator-(Vector lhs, Vector rhs) {
            lhs -= rhs;
            return lhs;
        }

        [[nodiscard]]
       AVEL_FINL friend Vector operator*(Vector lhs, Vector rhs) {
            lhs *= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator/(Vector lhs, Vector rhs) {
            lhs /= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator%(Vector lhs, Vector rhs) {
            lhs %= rhs;
            return lhs;
        }

        //=================================================
        // Increment/Decrement operators
        //=================================================

        AVEL_FINL Vector& operator++() {
            *this += Vector{1};
            return *this;
        }

        AVEL_FINL Vector operator++(int) {
            auto temp = *this;
            *this += Vector{1};
            return temp;
        }

        AVEL_FINL Vector& operator--() {
            *this -= Vector{1};
            return *this;
        }

        AVEL_FINL Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{1};
            return temp;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi32(content, _mm_cvtsi64_si128(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u32(content, vdupq_n_s32(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_srl_epi32(content, _mm_cvtsi64_si128(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u32(content, vdupq_n_s32(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi32(content, rhs.content);

            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto x_half0 = _mm_unpacklo_epi32(content, zeros);
            auto x_half1 = _mm_unpackhi_epi32(content, zeros);

            auto y_half0 = _mm_unpacklo_epi32(decay(rhs), zeros);
            auto y_half1 = _mm_unpackhi_epi32(decay(rhs), zeros);

            auto X0 = _mm_sll_epi32(x_half0, y_half0);
            auto X1 = _mm_sll_epi32(x_half0, _mm_srli_si128(y_half0, 8));
            auto X2 = _mm_sll_epi32(x_half1, y_half1);
            auto X3 = _mm_sll_epi32(x_half1, _mm_srli_si128(y_half1, 8));

            auto t0 = _mm_unpacklo_epi64(X0, X2);
            auto t1 = _mm_unpackhi_epi64(X1, X3);

            auto t2 = _mm_slli_si128(t1, 0x4);

            content = _mm_or_si128(t0, t2);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u32(content, vreinterpretq_s32_u32(decay(rhs)));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_srlv_epi32(content, rhs.content);
            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto x_half0 = _mm_unpacklo_epi32(content, zeros);
            auto x_half1 = _mm_unpackhi_epi32(content, zeros);

            auto y_half0 = _mm_unpacklo_epi32(decay(rhs), zeros);
            auto y_half1 = _mm_unpackhi_epi32(decay(rhs), zeros);

            auto X0 = _mm_srl_epi32(x_half0, y_half0);
            auto X1 = _mm_srl_epi32(x_half0, _mm_srli_si128(y_half0, 8));
            auto X2 = _mm_srl_epi32(x_half1, y_half1);
            auto X3 = _mm_srl_epi32(x_half1, _mm_srli_si128(y_half1, 8));

            auto t0 = _mm_unpacklo_epi64(X0, X2);
            auto t1 = _mm_unpackhi_epi64(X1, X3);

            auto t2 = _mm_slli_si128(t1, 0x4);

            content = _mm_or_si128(t0, t2);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u32(content, vnegq_s32(vreinterpretq_s32_u32(decay(rhs))));

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            primitive t = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi32(t, t))};

            #endif

            #if defined(AVEL_NEON)
            return Vector{vmvnq_u32(content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator&(Vector lhs, Vector rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator|(Vector lhs, Vector rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator^(Vector lhs, Vector rhs) {
            lhs ^= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, std::uint32_t rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, std::uint32_t rhs) {
            lhs >>= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, Vector rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, Vector rhs) {
            lhs >>= rhs;
            return lhs;
        }

        //=================================================
        // Conversions
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return Vector{} != *this;
        }

    };

    //=====================================================
    // Arrangment operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint32_t extract(vec4x32u v) {
        static_assert(N <= vec4x32u::width, "Specified index does not exist");
        typename std::enable_if<N <= vec4x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SS41)
        return _mm_extract_epi32(decay(v), N);

        #elif defined(AVEL_SSE2)
        return _mm_cvtsi128_si32(_mm_srli_si128(decay(v), 4 * N));

        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_u32(decay(v), N);
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec4x32u x) {
        #if defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi32(decay(x), _mm_setzero_si128());
        return popcount(_mm_movemask_epi8(compared)) / 4;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec4x32u x) {
        #if defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0x00 != _mm_movemask_epi8(compared);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec4x32u x) {
        #if defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0x00 == _mm_movemask_epi8(compared);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec4x32u x) {
        #if defined(AVEL_SS41)
        return _mm_test_all_zeros(x, x);
        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0xFF == _mm_movemask_epi8(compared);
        #endif
    }
    */

    [[nodiscard]]
    AVEL_FINL vec4x32u broadcast_mask(mask4x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec4x32u{_mm_movm_epi32(decay(m))};

        #elif defined(AVEL_AVX512VL)
        auto undef = _mm_undefined_si128();
        return vec4x32u{_mm_maskz_ternarylogic_epi32(decay(m), undef, undef, undef, 0xFF)};

        #elif defined(AVEL_SSE2)
        return vec4x32u{decay(m)};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{decay(m)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u blend(mask4x32u m, vec4x32u a, vec4x32u b) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_mask_blend_epi32(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_SSE41)
        return vec4x32u{_mm_blendv_epi8(decay(b), decay(a), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(decay(m), decay(b));
        auto y = _mm_and_si128(decay(m), decay(a));
        return vec4x32u{_mm_or_si128(x, y)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vbslq_u32(decay(m), decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u max(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_max_epu32(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vmaxq_u32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u min( vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_min_epu32(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(b < a, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vminq_u32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 2> minmax(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return {
            vec4x32u{_mm_min_epu32(decay(a), decay(b))},
            vec4x32u{_mm_max_epu32(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto mask = b < a;
        return {blend(mask, b, a), blend(mask, a, b)};

        #endif

        #if defined(AVEL_NEON)
        return {
            vec4x32u{vminq_u32(decay(a), decay(b))},
            vec4x32u{vmaxq_u32(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u clamp(vec4x32u x, vec4x32u lo, vec4x32u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u average(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE2)
        return (a >> 1) + (b >> 1) + (a & b & vec4x32u{0x1});
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vhaddq_u32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u midpoint(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE2)
        //TODO: Optimize
        vec4x32u t0 = a & b & vec4x32u{0x1};
        vec4x32u t1 = (a | b) & vec4x32u{0x1} & broadcast_mask(a > b);
        vec4x32u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
        #endif

        #if defined(AVEL_NEON)
        vec4x32u t0 = vec4x32u{vhaddq_u32(decay(a), decay(b))};
        vec4x32u t1 = (a ^ b) & vec4x32u{0x1} & broadcast_mask(a > b);
        return t0 + t1;

        #endif
    }

    //Definition of neg_abs delayed until vec4x32i is defined

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u load<vec4x32u>(const std::uint32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        n = std::min(4u, n);
        auto mask = 0xF >> (4 - n);
        return vec4x32u{_mm_maskz_load_epi32(mask, ptr)};

        #elif defined(AVEL_SSE2)
        switch (n) {
            case 0: {
                return vec4x32u{_mm_setzero_si128()};
            }
            case 1: {
                return vec4x32u{_mm_cvtsi32_si128(ptr[0])};
            }
            case 2: {
                std::int64_t two = 0x00;
                two |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                two |= static_cast<std::int64_t>(ptr[1]) << 0x20;
                return vec4x32u{_mm_cvtsi64_si128(two)};
            }
            case 3: {
                std::int64_t two_s = 0x00;
                two_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                two_s |= static_cast<std::int64_t>(ptr[1]) << 0x20;

                std::int32_t one_s = 0x00;
                one_s |= ptr[3] << 0x00;

                auto two_v = _mm_cvtsi64_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec4x32u{_mm_unpacklo_epi64(two_v, one_v)};
            }
            case 4: {
                return vec4x32u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
            }
        }

        #elif defined(AVEL_NEON)
        //TODO: Implement properly

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u load<vec4x32u, vec4x32u::width>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};

        #elif defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};

        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u aligned_load<vec4x32u>(const std::uint32_t* ptr, std::uint32_t n) {
        return load<vec4x32u>(ptr, n);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u aligned_load<vec4x32u, vec4x32u::width>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};
        #endif
    }

    //Definition of gather deferred until vec4x32i is defined


    template<std::uint32_t N = vec4x32u::width>
    AVEL_FINL void store(std::uint32_t* ptr, vec4x32u x) {
        static_assert(N <= vec4x32u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec4x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEVL_AVX512VL)
        auto mask = 0xF >> N;
        _mm_mask_storeu_epi32(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto mask = _mm_srli_si128(full, sizeof(vec4x32u::scalar) * (vec4x32u::width - N));
        _mm_maskmoveu_si128(decay(x), mask, reinterpret_cast<char *>(ptr));
        #endif
    }

    template<>
    AVEL_FINL void store<vec4x32u::width>(std::uint32_t* ptr, vec4x32u x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u32(ptr, decay(x));
        #endif
    }

    AVEL_FINL void store(std::uint32_t* ptr, vec4x32u x, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = 0xF >> std::min(vec4x32u::width, n);
        _mm_mask_storeu_epi32(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec4x32u::width;
        auto h = vec4x32u::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(w - std::min(w, n)));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(h - std::min(h, n)));
        auto mask = _mm_unpacklo_epi64(lo, hi);
        _mm_maskmoveu_si128(decay(x), mask, reinterpret_cast<char *>(ptr));

        #endif
    }


    template<std::uint32_t N = vec4x32u::width>
    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec4x32u x) {
        static_assert(N <= vec4x32u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec4x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif
    }

    template<>
    AVEL_FINL void aligned_store<vec4x32u::width>(std::uint32_t* ptr, vec4x32u x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u32(ptr, decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec4x32u x, std::uint32_t n) {
        #if defined(AVEL_SSE2)
        store(ptr, x, n);
        #endif

        #if defined(AVEL_NEON)
        vst1q_u32(ptr, decay(x));
        #endif
    }

    //Definition of scatter deferred until vec4x32i is defined

    //=====================================================
    // Integer vector operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec4x32u> div(vec4x32u x, vec4x32u y) {
        vec4x32u quotient{};

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto t0 = _mm_lzcnt_epi32(decay(y));
        auto t1 = _mm_lzcnt_epi32(decay(x));
        auto t2 = _mm_subs_epu16(t0, t1);
        auto t3 = _mm_add_epi32(t2, _mm_set1_epi32(1));

        auto s0 = t3;
        auto s1 = _mm_bsrli_si128(s0, 8);
        auto s2 = _mm_max_epi32(s0, s1);
        auto s3 = _mm_bsrli_si128(s2, 4);
        auto s4 = _mm_max_epi32(s2, s3);

        std::int32_t i = _mm_cvtsi128_si32(s4);

        //TODO: Optimize case were a denominator is zero?
        #elif defined(AVEL_SSE41)

        auto n0 = decay(x);
        auto n1 = _mm_bsrli_si128(n0, 8);
        auto n2 = _mm_max_epu32(n0, n1);
        auto n3 = _mm_bsrli_si128(n2, 4);
        auto n4 = _mm_max_epu32(n2, n3);

        auto d0 = decay(y);
        auto d1 = _mm_bsrli_si128(d0, 8);
        auto d2 = _mm_min_epu32(d0, d1);
        auto d3 = _mm_bsrli_si128(d2, 4);
        auto d4 = _mm_min_epu32(d2, d3);

        std::int32_t t0 = countl_zero(_mm_cvtsi128_si32(n4));
        std::int32_t t1 = countl_zero(_mm_cvtsi128_si32(d4));

        //TODO: Optimize case were a denominator is zero?
        //TODO: Correct this calculation
        std::int32_t i = std::max(t1 - t0 + 1, 0);

        #elif defined(AVEL_SSE2)

        //8-bit max/min suffice since all that matters is keeping the leading
        //bit in each 32-bit int set

        auto n0 = decay(x);
        auto n1 = _mm_bsrli_si128(n0, 8);
        auto n2 = _mm_max_epu8(n0, n1);
        auto n3 = _mm_bsrli_si128(n2, 4);
        auto n4 = _mm_max_epu8(n2, n3);

        auto d0 = decay(y);
        auto d1 = _mm_bsrli_si128(d0, 8);
        auto d2 = _mm_min_epu8(d0, d1);
        auto d3 = _mm_bsrli_si128(d2, 4);
        auto d4 = _mm_min_epu8(d2, d3);

        std::int32_t t0 = countl_zero(_mm_cvtsi128_si32(n4));
        std::int32_t t1 = countl_zero(_mm_cvtsi128_si32(d4));

        //TODO: Optimize case were a denominator is zero?
        //TODO: Correct this calculation
        std::int32_t i = std::max(t1 - t0 + 1, 0);
        #endif

        #if defined(AVEL_AARCH64)
        std::uint32_t i = 32;
        /*
        //TODO: Correct this optimization
        auto n0 = vclzq_u32(decay(x));
        auto d0 = vclzq_u32(decay(y));

        std::int32_t i = vminvq_u32(d0) - vminvq_u32(n0) + 1;
        */

        #elif defined(AVEL_NEON)
        std::uint32_t i = 32;

        /*
        auto n0 = vclzq_u32(decay(x));
        auto d0 = vclzq_u32(decay(y));

        //TODO: Implement

        std::int32_t i =  32;
        */
        #endif

        //TODO: Optimize body with masked instructions
        for (; (i-- > 0) && any(mask4x32u(x));) {
            mask4x32u b = ((x >> i) >= y);
            x -= (broadcast_mask(b) & (y << i));
            quotient |= (vec4x32u{b} << i);
        }

        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u popcount(vec4x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)
        return vec4x32u{_mm_popcnt_epi32(decay(x))};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm_popcnt_epi16(decay(x));
        auto tmp1 = _mm_slli_epi32(tmp0, 16);
        auto tmp2 = _mm_add_epi32(tmp0, tmp1);
        return vec4x32u{_mm_srli_epi32(tmp2, 16)};

        #elif defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto nibble_mask = _mm_set1_epi8(0x0F);

        auto index0 = _mm_and_si128(decay(x), nibble_mask);
        auto index1 = _mm_and_si128(_mm_srli_epi16(decay(x), 0x4), nibble_mask);

        auto partial_byte_sum0 = _mm_shuffle_epi8(table, index0);
        auto partial_byte_sum1 = _mm_shuffle_epi8(table, index1);

        auto byte_sum = _mm_add_epi8(partial_byte_sum0, partial_byte_sum1);

        auto partial_sum0 = _mm_add_epi8(byte_sum, _mm_slli_epi32(byte_sum, 16));
        auto partial_sum1 = _mm_add_epi8(partial_sum0, _mm_slli_epi32(partial_sum0, 8));

        auto ret = _mm_srli_epi32(partial_sum1, 24);

        return vec4x32u{ret};

        #elif defined(AVEL_SSE2)
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & vec4x32u{0x55555555u});
        x = (x & vec4x32u{0x33333333u}) + ((x >> 2) & vec4x32u{0x33333333u});
        x = ((x + (x >> 4) & vec4x32u{0x0F0F0F0Fu}) * vec4x32u{0x01010101u}) >> 24;
        return x;

        #endif

        #if defined(AVEL_NEON)
        auto reg_abcd = vreinterpretq_u32_u8(vcntq_u8(vreinterpretq_u8_u32(decay(x))));
        auto reg_00ab = vshlq_n_u32(reg_abcd, 16);
        auto reg_00xy = vaddq_u32(reg_abcd, reg_00ab);

        auto reg_000x = vshlq_n_u32(reg_00xy, 8);
        auto reg_000z = vaddq_u32(reg_000x, reg_00xy);

        return vec4x32u{vshrq_n_u32(reg_000z, 24)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u byteswap(vec4x32u x) {
        #if defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t index_data[16] {
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12
        };

        auto indices = _mm_load_si128((const __m128i*)index_data);
        return vec4x32u{_mm_shuffle_epi8(decay(x), indices)};

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_shufflelo_epi16(decay(x), 0xB1);
        auto t1 = _mm_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm_slli_epi16(t1, 0x8);
        auto t3 = _mm_srli_epi16(t1, 0x8);
        return vec4x32u{_mm_or_si128(t2, t3)};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(decay(x))))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countl_zero(vec4x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec4x32u{_mm_lzcnt_epi32(decay(x))};

        #elif defined(AVEL_SSE2)
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        x = _mm_andnot_si128(decay(x >> 1), decay(x));
        auto floats = _mm_add_ps(_mm_cvtepi32_ps(decay(x)), _mm_set1_ps(0.5f));
        auto biased_exponents = (vec4x32u(_mm_castps_si128(floats)) >> 23);
        auto lzcnt = _mm_subs_epu16(_mm_set1_epi32(158), decay(biased_exponents));
        return vec4x32u{lzcnt};

        //#else
        //return countl_one(~x);
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vclzq_u32(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countl_one(vec4x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec4x32u{_mm_lzcnt_epi32(decay(~x))};
        #elif defined(AVEL_SSE2)
        return countl_zero(~x);
        #endif

        /* Pure software implementation
        vec4x32u sum{x == vec4x32u{0xFFFFFFFF}};

        vec4x32u m0{0xFFFF0000u};
        mask4x32u b0 = (m0 & x) == m0;
        sum += broadcast_mask(b0) & vec4x32u{16};
        x <<= broadcast_mask(b0) * vec4x32u{16};

        vec4x32u m1{0xFF000000u};
        mask4x32u b1 = (m1 & x) == m1;
        sum += broadcast_mask(b1) & vec4x32u{8};
        x <<= broadcast_mask(b1) & vec4x32u{8};

        vec4x32u m2{0xF0000000u};
        mask4x32u b2 = (m2 & x) == m2;
        sum += broadcast_mask(b2) & vec4x32u{4};
        x <<= broadcast_mask(b2) & vec4x32u{4};

        vec4x32u m3{0xC0000000u};
        mask4x32u b3 = (m3 & x) == m3;
        sum += broadcast_mask(b3) & vec4x32u{2};
        x <<= broadcast_mask(b3) & vec4x32u{2};

        vec4x32u m4{0x80000000u};
        mask4x32u b4 = (m4 & x) == m4;
        sum += broadcast_mask(b4) & vec4x32u{1};

        return sum;
        */

        #if defined(AVEL_NEON)
        return vec4x32u{vclzq_u32(decay(~x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countr_zero(vec4x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD) && defined(AVEL_GFNI)
        static constexpr std::uint8_t table_data[16] {
            3, 2, 1, 0,
            7, 6, 5, 4,
            11, 10, 9, 8,
            15, 14, 13, 12
        };

        auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto reversed_bytes = _mm_shuffle_epi8(decay(x), table);

        auto gf_table = _mm_set1_epi64x(0x8040201008040201ul);
        auto reversed_bits = _mm_gf2p8affine_epi64_epi8(reversed_bytes, gf_table, 0x0);

        return vec4x32u{_mm_lzcnt_epi32(reversed_bits)};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto zero_mask = (x == vec4x32u{0x00});
        auto y = (x & (vec4x32u{0x00} - x));
        auto z = vec4x32u{31} - countl_zero(y);
        return blend(zero_mask, vec4x32u{32}, z);

        #elif defined(AVEL_SSE2)
        auto y = (x & (vec4x32u{0x00} - x));
        auto floats = _mm_cvtepi32_ps(decay(y));
        auto biased_exponents = (vec4x32u(_mm_castps_si128(floats)) >> 23);
        biased_exponents = _mm_min_epi16(decay(vec4x32u{158}), decay(biased_exponents));
        auto tzcnt = biased_exponents - vec4x32u{127};
        tzcnt = blend(x == vec4x32u{}, vec4x32u{32}, tzcnt);

        return tzcnt;
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto reversed_bytes = vrev32q_u8(vreinterpretq_u8_u32(decay(x)));
        auto reversed_bits = vrbitq_u8(reversed_bytes);
        return vec4x32u{vclzq_u32(vreinterpretq_u32_u8(reversed_bits))};

        #elif defined(AVEL_NEON)
        auto zero_mask = (x == vec4x32u{0x00});
        auto y = (x & (vec4x32u{0x00} - x));
        return blend(zero_mask, vec4x32u{32}, vec4x32u{31} - countl_zero(y));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countr_one(vec4x32u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u bit_width(vec4x32u x) {
        #if defined(AVEL_SSE2)
        return vec4x32u{32} - vec4x32u{countl_zero(x)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{32} - vec4x32u{countl_zero(x)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u bit_floor(vec4x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        mask4x32u zero_mask = (x != vec4x32u{0x00});
        vec4x32u leading_zeros = countl_zero(x);

        return (vec4x32u{zero_mask} << (vec4x32u{31} - leading_zeros));
        #elif defined(AVEL_SSE2)
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        return x - (x >> 1);
        #endif

        #if defined(AVEL_NEON)
        vec4x32u leading_zeros = countl_zero(x);
        mask4x32u zero_mask = (leading_zeros != vec4x32u{32});

        return (vec4x32u{zero_mask} << (vec4x32u{31} - leading_zeros));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u bit_ceil(vec4x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto tmp = (vec4x32u{32} - countl_zero(x - vec4x32u{1}));
        return blend(x == vec4x32u{}, vec4x32u{1}, vec4x32u{1} << tmp);

        #elif defined(AVEL_SSE2)
        auto zero_mask = (x == vec4x32u{});

        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x = _mm_andnot_si128(decay(broadcast_mask(zero_mask)), decay(x));
        ++x;

        return x;

        #endif

        #if defined(AVEL_NEON)
        //TODO: Optimize Subtract 1 from x?
        auto zero_mask = vec4x32u{0x00} == x;
        auto tmp = (vec4x32u{32} - countl_zero(x - vec4x32u{1}));
        return blend(zero_mask, vec4x32u{1}, vec4x32u{1} << tmp);
        #endif
    };

    [[nodiscard]]
    AVEL_FINL mask4x32u has_single_bit(vec4x32u v) {
        #if defined(AVEL_AVX512VL) && (defined(AVEL_AVX512VPOPCNTDQ) || defined(AVEL_AVX512BITALG))
        return popcount(v) == vec4x32u{1};
        #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
        return mask4x32u{v} & !mask4x32u{v & (v - vec4x32u{1})};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotl(vec4x32u v, std::uint32_t s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rolv_epi32(decay(v), decay(vec4x32u{s}))};

        #elif defined(AVEL_SSE2)
        s &= 0x1F;
        return (v << s) | (v >> (32 - s));
        #endif

        #if defined(AVEL_NEON)
        s &= 0x1F;
        return (v << s) | (v >> (32 - s));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotl(vec4x32u v, vec4x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rolv_epi32(decay(v), decay(s))};

        #elif defined(AVEL_SSE2)
        s &= vec4x32u(0x1F);
        return (v << s) | (v >> (vec4x32u{32} - s));
        #endif

        #if defined(AVEL_NEON)
        s &= vec4x32u(0x1F);
        return (v << s) | (v >> (vec4x32u{32} - s));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotr(vec4x32u v, std::uint32_t s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rorv_epi32(decay(v), decay(vec4x32u{s}))};

        #elif defined(AVEL_SSE2)
        s &= 0x1F;
        return (v >> s) | (v << (32 - s));

        #endif

        #if defined(AVEL_NEON)
        s &= 0x1F;
        return (v >> s) | (v << (32 - s));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotr(vec4x32u v, vec4x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rorv_epi32(decay(v), decay(s))};

        #elif defined(AVEL_SSE2)
        s &= vec4x32u(0x1F);
        return (v >> s) | (v << (vec4x32u{32} - s));

        #endif

        #if defined(AVEL_NEON)
        s &= vec4x32u(0x1F);
        return (v >> s) | (v << (vec4x32u {32} - s));
        #endif
    }

    //=====================================================
    // Conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL arr4x32u to_array(vec4x32u v) {
        alignas(16) arr4x32u array{};
        aligned_store(array.data(), v);
        return array;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec4x32u>(vec4x32u m) {
        return std::array<vec4x32u, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 4> convert<vec1x8u, vec4x32u>(vec4x32u m) {
        alignas(4) std::array<vec1x8u, 4> ret;

        #if defined(AVEL_AVX512F)
        ret = bit_cast<std::array<vec1x8u, 4>>(_mm_cvtsi128_si32(_mm_cvtepi32_epi8(decay(m))));

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_and_si128(decay(m), _mm_set1_epi32(0xFF));
        auto tmp2 = _mm_packus_epi32(decay(m), _mm_setzero_si128());
        auto tmp3 = _mm_packus_epi16(decay(m), _mm_setzero_si128());
        ret = bit_cast<std::array<vec1x8u, 4>>(_mm_cvtsi128_si32(tmp3));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 4> convert<vec1x8i, vec4x32u>(vec4x32u m) {
        alignas(4) std::array<vec1x8i, 4> ret;

        #if defined(AVEL_AVX512F)
        ret = bit_cast<std::array<vec1x8i, 4>>(_mm_cvtsi128_si32(_mm_cvtepi32_epi8(decay(m))));

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_and_si128(decay(m), _mm_set1_epi32(0xFF));
        auto tmp1 = _mm_packus_epi32(tmp0, _mm_setzero_si128());
        auto tmp2 = _mm_packus_epi16(tmp1, _mm_setzero_si128());
        ret = bit_cast<std::array<vec1x8i, 4>>(_mm_cvtsi128_si32(tmp2));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 4> convert<vec1x16u, vec4x32u>(vec4x32u m) {
        alignas(4) std::array<vec1x16u, 4> ret;

        #if defined(AVEL_AVX512F)
        ret = bit_cast<std::array<vec1x16u, 4>>(_mm_cvtsi128_si64(_mm_cvtepi32_epi16(decay(m))));

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_and_si128(decay(m), _mm_set1_epi32(0xFF));
        auto tmp1 = _mm_packus_epi32(tmp0, _mm_setzero_si128());
        _mm_storeu_si64(ret.data(), tmp1);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 4> convert<vec1x16i, vec4x32u>(vec4x32u m) {
        alignas(8) std::array<vec1x16i, 4> ret;

        #if defined(AVEL_AVX512F)
        auto t0 = _mm_cvtepi32_epi16(decay(m));
        ret = bit_cast<std::array<vec1x16i, 4>>(_mm_cvtsi128_si64(t0));

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_and_si128(decay(m), _mm_set1_epi32(0xFF));
        auto tmp1 = _mm_packus_epi32(tmp0, _mm_setzero_si128());
        _mm_storeu_si64(ret.data(), tmp1);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 4> convert<vec1x32u, vec4x32u>(vec4x32u m) {
        alignas(16) std::array<vec1x32u, 4> ret;

        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(m));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 4> convert<vec1x32i, vec4x32u>(vec4x32u m) {
        alignas(16) std::array<vec1x32i, 4> ret;
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(m));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64u, 4> convert<vec1x64u, vec4x32u>(vec4x32u m) {
        alignas(32) std::array<vec1x64u, 4> ret;

        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data()), _mm256_cvtepi32_epi64(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), _mm_unpacklo_epi32(decay(m), _mm_setzero_si128()));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), _mm_unpackhi_epi32(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 4> convert<vec1x64i, vec4x32u>(vec4x32u m) {
        alignas(32) std::array<vec1x64i, 4> ret;

        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data()), _mm256_cvtepi32_epi64(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), _mm_unpacklo_epi32(decay(m), _mm_setzero_si128()));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), _mm_unpackhi_epi32(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 4> convert<vec1x32f, vec4x32u>(vec4x32u m) {
        alignas(16) std::array<vec1x32f, 4> ret;
        #if defined(AVEL_AVX512F)
        _mm_store_ps(reinterpret_cast<float*>(ret.data()), _mm_cvtepu32_ps(decay(m)));

        #elif defined(AVEL_FMA)
        auto mask = _mm_set1_epi32(0x0000FFFF);
        auto corrective_factor = _mm_set1_ps(64536.0f);

        auto half0 = _mm_cvtepi32_ps(_mm_and_si128(mask, decay(m)));
        auto half1 = _mm_cvtepi32_ps(_mm_srli_si128(decay(m), 0x2));

        _mm_store_ps(reinterpret_cast<float*>(ret.data()), _mm_fmadd_ps(corrective_factor, half1, half0));

        #elif defined(AVEL_SSE2)
        auto mask = _mm_set1_epi32(0x0000FFFF);
        auto corrective_factor = _mm_set1_ps(64536.0f);

        auto half0 = _mm_cvtepi32_ps(_mm_and_si128(mask, decay(m)));
        auto half1 = _mm_cvtepi32_ps(_mm_srli_si128(decay(m), 0x2));

        half1 = _mm_mul_ps(half1, corrective_factor);
        _mm_store_ps(reinterpret_cast<float*>(ret.data()), _mm_add_ps(half0, _mm_mul_ps(half1, corrective_factor)));

        #endif
        return ret;
    }

    /*
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64f, 4> convert<vec1x64f, vec4x32u>(vec4x32u m) {
        alignas(32) std::array<vec1x64f, 4> ret;
        //TODO: Implement

        #if defined(AVEL_AVX512VL)
        auto whole = _mm256_cvtepu32_pd(decay(m));
        _mm_store_pd(reinterpret_cast<double*>(ret.data()), whole);

        #elif defined(AVEL_FMA)


        #elif defined(AVEL_AVX2)

        #elif defined(AVEL_SSE2)


        #endif
    }
    */

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec4x32u>(vec4x32u m) {
        #if defined(AVEL_AVX512F)
        return {
            vec16x8u{_mm_cvtepi32_epi8(decay(m))}
        };

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_and_si128(decay(m), _mm_set1_epi32(0x000000FF));
        auto tmp1 = _mm_packs_epi32(tmp0, _mm_setzero_si128());
        auto tmp2 = _mm_packs_epi16(tmp1, _mm_setzero_si128());

        return {
            vec16x8u{tmp2}
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec4x32u>(vec4x32u m) {
        #if defined(AVEL_AVX512F)
        return {
            vec16x8i{_mm_cvtepi32_epi8(decay(m))}
        };

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_and_si128(decay(m), _mm_set1_epi32(0x000000FF));
        auto tmp1 = _mm_packs_epi32(tmp0, _mm_setzero_si128());
        auto tmp2 = _mm_packs_epi16(tmp1, _mm_setzero_si128());

        return {
            vec16x8i{tmp2}
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec4x32u>(vec4x32u m) {
        #if defined(AVEL_AVX512F)
        return {
            vec8x16u{_mm_cvtepi32_epi16(decay(m))}
        };

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_and_si128(decay(m), _mm_set1_epi32(0x0000FFFF));
        return {
            vec8x16u{_mm_packs_epi32(tmp0, _mm_setzero_si128())}
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec4x32u>(vec4x32u m) {
        #if defined(AVEL_AVX512F)
        return {
            vec8x16i{_mm_cvtepi32_epi16(decay(m))}
        };

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_and_si128(decay(m), _mm_set1_epi32(0x0000FFFF));
        return {
            vec8x16i{_mm_packs_epi32(tmp0, _mm_setzero_si128())}
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec1x8u>(vec1x8u m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32u{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec1x8i>(vec1x8i m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32u{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec1x16u>(vec1x16u m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32u{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec1x16i>(vec1x16i m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32u{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec1x32u>(vec1x32u m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32u{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec1x32i>(vec1x32i m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32u{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec1x64u>(vec1x64u m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32u{_mm_cvtsi32_si128(decay(m) & 0x00000000FFFFFFFFull)}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec1x64i>(vec1x64i m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32u{_mm_cvtsi32_si128(decay(m) & 0x00000000FFFFFFFFull)}
        };
        #endif
    }

    /*
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec1x32f>(vec1x32f m) {
        //TODO: Implement
    }
    */

    /*
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec1x64f>(vec1x64f m) {
        //TODO: Implement
    }
    */

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 4> convert<vec4x32u, vec16x8u>(vec16x8u m) {
        #if defined(AVEL_SSE41)
        return {
            vec4x32u{_mm_cvtepu8_epi32(decay(m))},
            vec4x32u{_mm_cvtepu8_epi32(_mm_srli_si128(decay(m), 0x4))},
            vec4x32u{_mm_cvtepu8_epi32(_mm_srli_si128(decay(m), 0x8))},
            vec4x32u{_mm_cvtepu8_epi32(_mm_srli_si128(decay(m), 0xC))}
        };

        #elif defined(AVEL_SSE2)
        auto zeros = _mm_setzero_si128();

        auto half0 = _mm_unpacklo_epi8(decay(m), zeros);
        auto half1 = _mm_unpacklo_epi8(decay(m), zeros);

        return {
            vec4x32u{_mm_unpacklo_epi16(half0, zeros)},
            vec4x32u{_mm_unpackhi_epi16(half0, zeros)},
            vec4x32u{_mm_unpacklo_epi16(half1, zeros)},
            vec4x32u{_mm_unpackhi_epi16(half1, zeros)}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 4> convert<vec4x32u, vec16x8i>(vec16x8i m) {
        #if defined(AVEL_SSE41)
        return {
            vec4x32u{_mm_cvtepi8_epi32(decay(m))},
            vec4x32u{_mm_cvtepi8_epi32(_mm_srli_si128(decay(m), 0x4))},
            vec4x32u{_mm_cvtepi8_epi32(_mm_srli_si128(decay(m), 0x8))},
            vec4x32u{_mm_cvtepi8_epi32(_mm_srli_si128(decay(m), 0xC))}
        };

        #elif defined(AVEL_SSE2)
        auto whole = decay(m);
        auto wholeb = _mm_cmplt_epi8(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi8(whole, wholeb);
        auto half0b = _mm_cmplt_epi16(half0, _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi8(whole, wholeb);
        auto half1b = _mm_cmplt_epi16(half1, _mm_setzero_si128());

        return {
            vec4x32u{_mm_unpacklo_epi16(half0, half0b)},
            vec4x32u{_mm_unpackhi_epi16(half0, half0b)},
            vec4x32u{_mm_unpacklo_epi16(half1, half1b)},
            vec4x32u{_mm_unpackhi_epi16(half1, half1b)}
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 2> convert<vec4x32u, vec8x16u>(vec8x16u m) {
        #if defined(AVEL_SSE41)
        auto whole = decay(m);

        return {
            vec4x32u{_mm_cvtepu16_epi32(whole)},
            vec4x32u{_mm_cvtepu16_epi32(_mm_srli_si128(whole, 8))}
        };

        #elif defined(AVEL_SSE2)
        auto whole = decay(m);

        return {
            vec4x32u{_mm_unpacklo_epi16(whole, _mm_setzero_si128())},
            vec4x32u{_mm_unpackhi_epi16(whole, _mm_setzero_si128())}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 2> convert<vec4x32u, vec8x16i>(vec8x16i m) {
        #if defined(AVEL_SSE41)
        auto whole = decay(m);

        return {
            vec4x32u{_mm_cvtepi16_epi32(whole)},
            vec4x32u{_mm_cvtepi16_epi32(_mm_srli_si128(whole, 8))}
        };

        #elif defined(AVEL_SSE2)
        auto whole = decay(m);
        auto wholeb = _mm_cmplt_epi16(whole, _mm_setzero_si128());

        return {
            vec4x32u{_mm_unpacklo_epi16(whole, wholeb)},
            vec4x32u{_mm_unpackhi_epi16(whole, wholeb)}
        };
        #endif
    }

}

#endif //AVEL_VEC4X32U_HPP
