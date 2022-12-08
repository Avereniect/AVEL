#ifndef AVEL_VEC4X32U_HPP
#define AVEL_VEC4X32U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x32u = Vector<std::uint32_t, 4>;

    using mask4x32u = Vector_mask<std::uint32_t, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec4x32u> div(vec4x32u numerator, vec4x32u denominator);
    vec4x32u broadcast_mask(mask4x32u m);
    vec4x32u blend(vec4x32u a, vec4x32u b, mask4x32u m);
    vec4x32u countl_one(vec4x32u x);





    template<>
    #if defined(AVEL_AVX512VL)
    class Vector_mask<std::uint32_t, 4> : public avel_impl::Vector_mask8xT {
        using base = avel_impl::Vector_mask8xT;
    #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
    class Vector_mask<std::uint32_t, 4> : public avel_impl::Vector_mask128b {
        using base = avel_impl::Vector_mask128b;
    #endif
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint32x4_t;
        #endif

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        AVEL_FINL explicit Vector_mask(base b):
            base(b) {}

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            base(convert<Vector_mask>(v)[0]) {}

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vector_mask(primitive p):
            base(vreinterpretq_u8_u32(p)) {}
        #endif

        #if defined(AVEL_AVX512VL)
        AVEL_FINL explicit Vector_mask(bool b):
            base{primitive(b ? 0xF : 0x0)} {}
        #endif

        AVEL_FINL explicit Vector_mask(const std::array<bool, 4>& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if defined(AVEL_AVX512VL)
            __m128i array_data = _mm_loadu_si32(arr.data());
            content = __mmask8(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_SSE2)
            primitive array_data = _mm_loadu_si32(arr.data());
            array_data = _mm_unpacklo_epi8(array_data, array_data);
            array_data = _mm_unpacklo_epi16(array_data, array_data);
            content = _mm_cmplt_epi32(_mm_setzero_si128(), array_data);

            #endif

            #if defined(AVEL_NEON)
            auto t0 = vld1q_u32(reinterpret_cast<const std::uint32_t*>(arr.data()));
            //TODO: Complete implementation
            #endif
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector_mask& operator=(bool x) {
            #if defined(AVEL_AVX512VL)
            content = x ? 0xF : 0x0;
            #elif defined(AVEL_SSE2)
            content = x ? _mm_set1_epi32(-1) : _mm_setzero_si128();
            #endif
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            base::operator&=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            base::operator|=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            base::operator^=(rhs);
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

        #if defined(AVEL_NEON)
        [[nodiscard]]
        AVEL_FINL explicit operator primitive() {
            return vreinterpretq_u32_u8(content);
        }
        #endif

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
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x32u m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));
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
        _mm_storeu_si32(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 4> convert<mask1x8i, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x8i, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_storeu_si32(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 4> convert<mask1x16u, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x16u, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_storeu_si32(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 4> convert<mask1x16i, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x16i, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_storeu_si32(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 4> convert<mask1x32u, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x32u, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_storeu_si32(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 4> convert<mask1x32i, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x32i, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_storeu_si32(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64u, 4> convert<mask1x64u, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x64u, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_storeu_si32(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 4> convert<mask1x64i, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x64i, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_storeu_si32(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 4> convert<mask1x32f, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x32f, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_storeu_si32(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64f, 4> convert<mask1x64f, mask4x32u>(mask4x32u m) {
        alignas(4) std::array<mask1x64f, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_storeu_si32(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        _mm_storeu_si32(ret.data(), _mm_cvtepi32_epi8(t0));

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), decay(m));
        auto t1 = _mm_packs_epi32(t0, t0);
        auto t2 = _mm_packs_epi16(t1, t1);
        _mm_storeu_si32(ret.data(), t2);

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
    class Vector<std::uint32_t, 4> : public avel_impl::Vec4x32int {
        using base = avel_impl::Vec4x32int;
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
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint32x4_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        AVEL_FINL explicit Vector(base b):
            base(b) {}

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vector(primitive p):
            base(vreinterpretq_u8_u32(p)) {}
        #endif

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            base(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            base(_mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(1))) {}
        #elif defined(AVEL_SSE2)
            base(_mm_sub_epi32(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            Vector(vsubq_u32(vdupq_n_u32(0x00), decay(m))) {}
        #endif

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            base(_mm_broadcastd_epi32(_mm_cvtsi32_si128(x))) {}
        #elif defined(AVEL_SSE2)
            base(_mm_set1_epi32(x)) {}
        #endif
        #if defined(AVEL_NEON)
            Vector(vdupq_n_u32(x)) {}
        #endif

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            #if defined(AVEL_SSE2)
            base(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}
            #endif
            #if defined(AVEL_NEON)
            Vector(vld1q_u32(array.data())) {}
            #endif

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        using base::operator=;

        #if defined(AVEL_NEON)
        AVEL_FINL Vector& operator=(primitive x) {
            content = vreinterpretq_u8_u32(x);
            return *this;
        }
        #endif

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

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            return mask{base(lhs) == base(rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            return mask{base(lhs) != base(rhs)};
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

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_srl_epi32(content, _mm_cvtsi64_si128(rhs));

            #endif

            #if defined(AVEL_NEON)
            *this = vsraq_n_u32(decay(*this), vdupq_n_u32(rhs), 0x00);

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_srlv_epi32(content, rhs.content);
            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i  < 5; ++i) {
                auto t0 = _mm_and_si128(decay(rhs), _mm_set1_epi32(1u << i));
                mask m{_mm_cmplt_epi32(__m128i{}, t0)};
                *this = blend(*this, *this >> (1 << i), m);
            }

            content = _mm_and_si128(content, _mm_cmplt_epi32(decay(rhs), _mm_set1_epi32(32u)));

            #endif

            #if defined(AVEL_NEON)
            *this = vsraq_n_u32(decay(*this), decay(rhs), 0x00);
            #endif

            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            return Vector{base::operator~()};
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
        AVEL_FINL explicit operator mask() const {
            return Vector{} != *this;
        }

    };

    //=====================================================
    // General vector operations
    //=====================================================

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
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u blend(vec4x32u a, vec4x32u b, mask4x32u m) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_mask_blend_epi32(decay(m), decay(a), decay(b))};

        #elif defined(AVEL_SSE41)
        return vec4x32u{_mm_blendv_epi8(decay(a), decay(b), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(decay(m), decay(a));
        auto y = _mm_and_si128(decay(m), decay(b));
        return vec4x32u{_mm_or_si128(x, y)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vbslq_u32(decay(a), decay(b), decay(m))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u max(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_max_epu32(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a, b, a < b);
        #elif defined(AVEL_NEON)
        return vec4x32u{vmaxq_u32(decay(a), decay(b))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u min( vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_min_epu32(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a, b, b < a);
        #elif defined(AVEL_NEON)
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
        return {blend(a, b, mask), blend(a, b, !mask)};

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
        vec4x32u t0 = a & b & vec4x32u{0x1};
        vec4x32u t1 = (a | b) & vec4x32u{0x1} & broadcast_mask(a > b);
        vec4x32u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    //Definition of neg_abs delayed until vec4x32i is defined

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u load<vec4x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};

        #elif defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u aligned_load<vec4x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};

        #elif defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};

        #endif
    }

    //Definition of gather deferred until vec4x32i is defined

    AVEL_FINL void store(std::uint32_t* ptr, vec4x32u x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec4x32u x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #elif defined(AVEL_NEON)
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
        auto n0 = vclzq_s32(decay(x));
        auto d0 = vclzq_s32(decay(y));

        std::int32_t i = vminvq_u32(d0) - vminvq_u32(n0) + 1;

        #elif defined(AVEL_NEON)
        auto n0 = vclzq_u32(decay(x));
        auto d0 = vclzq_u32(decay(y));

        //TODO: Implement

        std::int32_t i =  32;
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
        auto tmp0 = _mm_popcnt_epi16(x);
        auto tmp1 = _mm_slli_epi32(tmp0, 16);
        auto tmp2 = _mm_add_epi32(tmp0, tmp1);
        return vec4x32u{_mm_srli_epi32(tmp2, 16)};

        #elif defined(AVEL_SSE2)
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & vec4x32u{0x55555555u});
        x = (x & vec4x32u{0x33333333u}) + ((x >> 2) & vec4x32u{0x33333333u});
        x = ((x + (x >> 4) & vec4x32u{0x0F0F0F0Fu}) * vec4x32u{0x01010101u}) >> 24;
        return x;

        #endif

        #if defined(AVEL_NEON)
        vec4x32u reg_abcd{vreinterpretq_u32_u8(vcntq_u8(vreinterpretq_u8_u32(decay(x))))};
        vec4x32u reg_00ab{vshrq_n_u32(decay(reg_abcd), 16)};
        vec4x32u reg_00xy{vaddq_u32(decay(reg_abcd), decay(reg_00ab))};

        vec4x32u reg_000x{vshrq_n_u32(decay(reg_00xy), 8)};
        vec4x32u reg_000z{vaddq_u32(decay(reg_000x), decay(reg_00xy))};

        return reg_000z;
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
        alignas(16) static constexpr std::uint32_t mask_data[4]{
            0x00ff00ff,
            0x00ff00ff,
            0x00ff00ff,
            0x00ff00ff
        };

        auto t0 = _mm_shufflelo_epi16(x, 0xB1);
        auto t1 = _mm_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm_bslli_si128(t1, 1);
        auto t3 = _mm_bsrli_si128(t1, 1);
        auto t4 = _mm_load_si128(reinterpret_cast<const __m128i*>(mask_data));
        return blend(vec4x32u{t2}, vec4x32u{t3}, mask4x32u{t4});

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(decay(x))))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countl_zero(vec4x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec4x32u{_mm_lzcnt_epi32(decay(x))};

        #elif defined(AVEL_AVX512VL)
        auto floats = _mm_add_ps(_mm_cvtepu32_ps(decay(x)), _mm_set1_ps(0.5f));
        __m128i biased_exponents = _mm_srli_epi32(_mm_castps_si128(floats), 23);
        __m128i lzcnt = _mm_subs_epu16(_mm_set1_epi32(158), biased_exponents);
        return vec4x32u{biased_exponents};

        #elif defined(AVEL_SSE2)
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        x = _mm_andnot_si128(decay(x >> 1), decay(x));
        auto floats = _mm_add_ps(_mm_cvtepi32_ps(decay(x)), _mm_set1_ps(0.5f));
        auto biased_exponents = (vec4x32u(_mm_castps_si128(floats)) >> 23);
        auto lzcnt = _mm_subs_epu16(decay(vec4x32u{158}), decay(biased_exponents));
        return vec4x32u{lzcnt};

        #else
        return countl_one(~x);
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vclzq_u32(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countl_one(vec4x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec4x32u{_mm_lzcnt_epi32(~x)};
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
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto zero_mask = (x == vec4x32u{0x00});
        auto y = (x & (vec4x32u{0x00} - x));
        return blend(vec4x32u{31} - countl_zero(y), vec4x32u{32}, zero_mask);

        #elif defined(AVEL_SSE2)
        auto y = (x & (vec4x32u{0x00} - x));
        auto floats = _mm_cvtepi32_ps(decay(y));
        auto biased_exponents = (vec4x32u(_mm_castps_si128(floats)) >> 23);
        biased_exponents = _mm_min_epi16(decay(vec4x32u{158}), decay(biased_exponents));
        auto tzcnt = biased_exponents - vec4x32u{127};
        tzcnt = blend(tzcnt, vec4x32u{32}, x == vec4x32u{});

        return tzcnt;
        #endif

        #if defined(AVEL_NEON)
        auto zero_mask = (x == vec4x32u{0x00});
        auto y = (x & (vec4x32u{0x00} - x));
        return blend(vec4x32u{31} - countl_zero(y), vec4x32u{32}, zero_mask);

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
        vec4x32u leading_zeros = countl_zero(x);
        mask4x32u zero_mask = (leading_zeros != vec4x32u{32});

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
        return vec4x32u{1} << tmp;

        #elif defined(AVEL_SSE2)
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        ++x;
        return x;

        #endif

        #if defined(AVEL_NEON)
        auto tmp = (vec4x32u{32} - countl_zero(x - vec4x32u{1}));
        return vec4x32u{1} << tmp;
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
    AVEL_FINL std::array<std::uint32_t, 4> to_array(vec4x32u v) {
        alignas(16) std::array<std::uint32_t, 4> array{};
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
        _mm_storeu_si32(ret.data(), _mm512_cvtepi32_epi8(_mm512_zextsi128_si512(decay(m))));

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_and_si128(decay(m), _mm_set1_epi32(0xFF));
        auto tmp2 = _mm_packus_epi32(decay(m), _mm_setzero_si128());
        auto tmp3 = _mm_packus_epi16(decay(m), _mm_setzero_si128());
        _mm_storeu_si32(ret.data(), tmp3);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 4> convert<vec1x8i, vec4x32u>(vec4x32u m) {
        alignas(4) std::array<vec1x8i, 4> ret;

        #if defined(AVEL_AVX512F)
        _mm_storeu_si32(ret.data(), _mm512_cvtepi32_epi8(_mm512_zextsi128_si512(decay(m))));

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_and_si128(decay(m), _mm_set1_epi32(0xFF));
        auto tmp1 = _mm_packus_epi32(tmp0, _mm_setzero_si128());
        auto tmp2 = _mm_packus_epi16(tmp1, _mm_setzero_si128());
        _mm_storeu_si32(ret.data(), tmp2);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 4> convert<vec1x16u, vec4x32u>(vec4x32u m) {
        alignas(4) std::array<vec1x16u, 4> ret;

        #if defined(AVEL_AVX512F)
        _mm_storeu_si64(ret.data(), _mm256_cvtepi32_epi16(_mm256_zextsi128_si256(decay(m))));

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
        _mm_storeu_si64(ret.data(), _mm256_cvtepi32_epi16(_mm256_zextsi128_si256(decay(m))));

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
