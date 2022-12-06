#ifndef AVEL_VEC4X32I_HPP
#define AVEL_VEC4X32I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x32i = Vector<std::int32_t, 4>;

    using mask4x32i = Vector_mask<std::int32_t, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec4x32i> div(vec4x32i numerator, vec4x32i denominator);
    vec4x32i blend(vec4x32i a, vec4x32i b, mask4x32i m);
    std::array<std::int32_t, 4> to_array(vec4x32i v);





    template<>
    #if defined(AVEL_AVX512VL)
    class Vector_mask<std::int32_t, 4> : public avel_impl::Vector_mask8xT {
        using base = Vector_mask8xT;
    #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
    class Vector_mask<std::int32_t, 4> : public avel_impl::Vector_mask128b {
        using base = Vector_mask128b;
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
        using primitive = int32x4_t;
        #endif

        //=================================================
        // Constructor
        //=================================================

        using base::base;

        AVEL_FINL explicit Vector_mask(base b):
            base(b) {}

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> m):
            base(convert<Vector_mask>(m)[0]) {}

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vector_mask(primitive p):
            base(vreinterpretq_u8_s32(p)) {}
        #endif

        #if defined(AVEL_AVX512VL)
        AVEL_FINL explicit Vector_mask(bool b):
            base(primitive(b ? 0xf : 0x0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const std::array<bool, 4>& arr) {
            static_assert(sizeof(bool) == 1);

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
            //TODO: Implement
            #endif
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        using base::operator=;

        #if defined(AVEL_AVX512VL)
        Vector_mask& operator=(bool b) {
            content = primitive(b ? 0xf : 0x0);
            return *this;
        }
        #endif

        #if defined(AVEL_NEON)
        Vector_mask& operator=(primitive p) {
            content = vreinterpretq_u8_s32(p);
            return *this;
        }
        #endif

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return (_mm512_mask2int(decay(lhs)) == _mm512_mask2int(decay(rhs)));
            #elif defined(AVEL_SSE2)
            return (0xFFFF == _mm_movemask_epi8(_mm_cmpeq_epi32(decay(lhs), decay(rhs))));
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return (_mm512_mask2int(decay(lhs)) != _mm512_mask2int(decay(rhs)));
            #elif defined(AVEL_SSE2)
            return (0xFFFF != _mm_movemask_epi8(_mm_cmpeq_epi32(decay(lhs), decay(rhs))));
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            *this = vandq_s32(decay(*this), decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            *this = vorrq_s32(decay(*this), decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            *this = veorq_s32(decay(*this), decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, 0x0f)};
            #elif defined(AVEL_SSE2)
            primitive tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};
            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vmvnq_s32(decay(*this))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {
            return lhs & rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {
            return lhs | rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {
            lhs ^= rhs;
            return lhs;
        }

        //=================================================
        // Conversion Operators
        //=================================================

        #if defined(AVEL_NEON)
        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return vreinterpretq_s32_u8(content);
        }
        #endif

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask4x32i m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(decay(m)));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m))) / sizeof(std::uint32_t);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x32i m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x32i m) {
        #if defined(AVEL_AVX512VL)
        return 0xF == _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE41)
        return _mm_test_all_ones(decay(m));
        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x32i m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask4x32i>(mask4x32i m) {
        return std::array<mask4x32i, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 4> convert<mask1x8u, mask4x32i>(mask4x32i m) {
        alignas(4) std::array<mask1x8u, 4> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_storeu_si32(ret.data(), _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m))));

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
    AVEL_FINL std::array<mask1x8i, 4> convert<mask1x8i, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask1x16u, 4> convert<mask1x16u, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask1x16i, 4> convert<mask1x16i, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask1x32u, 4> convert<mask1x32u, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask1x32i, 4> convert<mask1x32i, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask1x64u, 4> convert<mask1x64u, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask1x64i, 4> convert<mask1x64i, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask1x32f, 4> convert<mask1x32f, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask1x64f, 4> convert<mask1x64f, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask4x32i>(mask4x32i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto t0 = _mm_movm_epi32(decay(m));
        return {mask16x8u{_mm_cvtepi32_epi8(t0)}};

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        return {mask16x8u{_mm_cvtepi32_epi8(t0)}};

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_packs_epi32(decay(m), _mm_setzero_si128());
        auto tmp1 = _mm_packs_epi16(tmp0, _mm_setzero_si128());
        return {mask16x8u{tmp1}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask4x32i>(mask4x32i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto t0 = _mm_movm_epi32(decay(m));
        return {mask16x8i{_mm_cvtepi32_epi8(t0)}};

        #elif defined(AVEL_AVX512VL)
        auto t0 = _mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(0x1));
        return {mask16x8i{_mm_cvtepi32_epi8(t0)}};

        #elif defined(AVEL_SSE2)
        auto tmp0 = _mm_packs_epi32(decay(m), _mm_setzero_si128());
        auto tmp1 = _mm_packs_epi16(tmp0, _mm_setzero_si128());
        return {mask16x8i{tmp1}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask4x32i>(mask4x32i m) {
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
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask4x32i>(mask4x32i m) {
        return {mask4x32u{mask4x32u::primitive(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask1x8u>(mask1x8u m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32i{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32i{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask1x8i>(mask1x8i m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32i{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32i{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask1x16u>(mask1x16u m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32i{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32i{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask1x16i>(mask1x16i m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32i{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32i{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask1x32u>(mask1x32u m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32i{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32i{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask1x32i>(mask1x32i m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32i{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32i{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask1x64u>(mask1x64u m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32i{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32i{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask1x64i>(mask1x64i m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32i{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32i{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask1x32f>(mask1x32f m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32i{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32i{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask1x64f>(mask1x64f m) {
        #if defined(AVEL_AVX512VL)
        return {mask4x32i{decay(m)}};

        #elif defined(AVEL_SSE2)
        return {mask4x32i{_mm_cvtsi32_si128(-std::uint32_t(decay(m)))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 4> convert<mask4x32i, mask16x8u>(mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {
            mask4x32i{mask4x32i::primitive(decay(m) & 0xF)},
            mask4x32i{mask4x32i::primitive((decay(m) >> 0x4) & 0xF)},
            mask4x32i{mask4x32i::primitive((decay(m) >> 0x8) & 0xF)},
            mask4x32i{mask4x32i::primitive((decay(m) >> 0xC))}
        };

        #elif defined(AVEL_AVX512VL)
        auto tmp0 = _mm_movemask_epi8(decay(m));
        return {
            mask4x32i{mask4x32i::primitive(tmp0 & 0xF)},
            mask4x32i{mask4x32i::primitive((tmp0 >> 0x4) & 0xF)},
            mask4x32i{mask4x32i::primitive((tmp0 >> 0x8) & 0xF)},
            mask4x32i{mask4x32i::primitive((tmp0 >> 0xC))}
        };

        #elif defined(AVEL_SSE2)
        auto half0 = _mm_unpacklo_epi8(decay(m), decay(m));
        auto half1 = _mm_unpackhi_epi8(decay(m), decay(m));

        return {
            mask4x32i{_mm_unpacklo_epi16(half0, half0)},
            mask4x32i{_mm_unpackhi_epi16(half0, half0)},
            mask4x32i{_mm_unpacklo_epi16(half1, half1)},
            mask4x32i{_mm_unpackhi_epi16(half1, half1)}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 4> convert<mask4x32i, mask16x8i>(mask16x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {
            mask4x32i{mask4x32i::primitive(decay(m) & 0xF)},
            mask4x32i{mask4x32i::primitive((decay(m) >> 0x4) & 0xF)},
            mask4x32i{mask4x32i::primitive((decay(m) >> 0x8) & 0xF)},
            mask4x32i{mask4x32i::primitive((decay(m) >> 0xC))}
        };

        #elif defined(AVEL_AVX512VL)
        auto tmp0 = _mm_movemask_epi8(decay(m));
        return {
            mask4x32i{mask4x32i::primitive(tmp0 & 0xF)},
            mask4x32i{mask4x32i::primitive((tmp0 >> 0x4) & 0xF)},
            mask4x32i{mask4x32i::primitive((tmp0 >> 0x8) & 0xF)},
            mask4x32i{mask4x32i::primitive((tmp0 >> 0xC))}
        };

        #elif defined(AVEL_SSE2)
        auto half0 = _mm_unpacklo_epi8(decay(m), decay(m));
        auto half1 = _mm_unpackhi_epi8(decay(m), decay(m));

        return {
            mask4x32i{_mm_unpacklo_epi16(half0, half0)},
            mask4x32i{_mm_unpackhi_epi16(half0, half0)},
            mask4x32i{_mm_unpacklo_epi16(half1, half1)},
            mask4x32i{_mm_unpackhi_epi16(half1, half1)}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 2> convert<mask4x32i, mask8x16u>(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto tmp0 = decay(m);
        return {
            mask4x32i{mask4x32u::primitive(tmp0 & 0xF)},
            mask4x32i{mask4x32u::primitive(tmp0 >> 4)}
        };

        #elif defined(AVEL_AVX512VL)
        auto tmp0 = _mm_movemask_epi8(decay(m));
        auto tmp1 = _pext_u32(tmp0, 0xAAAA);

        return {
            mask4x32i{mask4x32i::primitive(tmp1 & 0xFF)},
            mask4x32i{mask4x32i::primitive(tmp1 >> 8)}
        };

        #elif defined(AVEL_SSE2)
        return {
            mask4x32i{_mm_unpacklo_epi16(decay(m), decay(m))},
            mask4x32i{_mm_unpackhi_epi16(decay(m), decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 2> convert<mask4x32i, mask8x16i>(mask8x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto tmp0 = decay(m);
        return {
            mask4x32i{mask4x32i::primitive(tmp0 & 0xF)},
            mask4x32i{mask4x32i::primitive(tmp0 >> 4)}
        };

        #elif defined(AVEL_AVX512VL)
        auto tmp0 = _mm_movemask_epi8(decay(m));
        auto tmp1 = _pext_u32(tmp0, 0xAAAA);

        return {
            mask4x32i{mask4x32i::primitive(tmp1 & 0xFF)},
            mask4x32i{mask4x32i::primitive(tmp1 >> 8)}
        };

        #elif defined(AVEL_SSE2)
        return {
            mask4x32i{_mm_unpacklo_epi16(decay(m), decay(m))},
            mask4x32i{_mm_unpackhi_epi16(decay(m), decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask4x32u>(mask4x32u m) {
        return {mask4x32i{mask4x32i::primitive(decay(m))}};
    }





    template<>
    class alignas(16) Vector<std::int32_t, 4> : public avel_impl::Vec4x32int {
        using base = avel_impl::Vec4x32int;
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int32_t;

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int32x4_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> m):
            base(convert<Vector>(m)[0]) {}

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vector(primitive p):
            base(p) {}
        #endif

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            base(_mm_mask_blend_epi32(decay(m), _mm_setzero_si128(), _mm_set1_epi32(1))) {}
        #elif defined(AVEL_SSE2)
            base(_mm_sub_epi32(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            Vector() {} //TODO: Implement
        #endif

        AVEL_FINL explicit Vector(const scalar x):
        #if defined(AVEL_SSE2)
            base(_mm_set1_epi32(x)) {}
        #endif
        #if defined(AVEL_NEON)
            Vector(vdupq_n_s32(x)) {}
        #endif

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
        #if defined(AVEL_SSE2)
            base(_mm_loadu_si128(reinterpret_cast<const primitive*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            Vector() {} //TODO: Implement
        #endif

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        using base::operator=;

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_AVX2)
            content = _mm_broadcastd_epi32(_mm_cvtsi32_si128(x));
            #elif defined(AVEL_SSE2)
            content = _mm_shuffle_epi32(_mm_cvtsi32_si128(x), 0x00);
            #endif
            return *this;
        }

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

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
            return mask{_mm_cmplt_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_epi32(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !(lhs > rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_epi32(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !(lhs < rhs);
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Vector operator-() const {
            return Vector{} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            base::operator+=(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            base::operator-=(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            base::operator*=(rhs);
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
            base::operator&=(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            base::operator|=(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            base::operator^=(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sra_epi32(content, _mm_cvtsi64_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            //TODO: Implement
            //auto s = (std::uint64_t(rhs) > 0xFFFFFFFF) ? 0xFFFFFFFF : static_cast<std::uint32_t>(rhs);
            //*this = vshrq_n_s32(decay(*this), vdupq_n_s32(s));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(vec4x32i rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_srav_epi32(content, primitive(rhs));
            #elif defined(AVEL_SSE2)
            __m128i zeros = _mm_setzero_si128();

            __m128i x_half0 = _mm_unpacklo_epi32(content, zeros);
            __m128i x_half1 = _mm_unpackhi_epi32(content, zeros);

            __m128i y_half0 = _mm_unpacklo_epi32(decay(rhs), zeros);
            __m128i y_half1 = _mm_unpackhi_epi32(decay(rhs), zeros);

            __m128i X0 = _mm_sra_epi32(x_half0, y_half0);
            __m128i X1 = _mm_sra_epi32(x_half0, _mm_srli_si128(y_half0, 8));
            __m128i X2 = _mm_sra_epi32(x_half1, y_half1);
            __m128i X3 = _mm_sra_epi32(x_half1, _mm_srli_si128(y_half1, 8));

            __m128i t0 = _mm_unpacklo_epi64(X0, X2);
            __m128i t1 = _mm_unpackhi_epi64(X1, X3);

            __m128i t2 = _mm_slli_si128(t1, 0x4);

            content = _mm_or_si128(t0, t2);

            #endif

            #if defined(AVEL_NEON)
            //TODO: Implement
            #endif

            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512F)
            return Vector{_mm_ternarylogic_epi32(content, content, content, 0x01)};
            #elif defined(AVEL_SSE2)
            return Vector{_mm_andnot_si128(content, _mm_set1_epi32(0xFFFFFFFF))};
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
        AVEL_FINL friend Vector operator<<(Vector lhs, long long rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, long long rhs) {
            lhs >>= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, vec4x32i rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, vec4x32i rhs) {
            lhs >>= rhs;
            return lhs;
        }

        //=================================================
        // Conversions
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return *this != Vector{};
        }

    };

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec4x32i operator-(vec4x32u v) {
        return vec4x32i{vec4x32u{} - v};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    /*
    template<unsigned O>
    [[nodiscard]]
    AVEL_FINL vec4x32i funnel_shift(vec4x32i a, vec4x32i b) {
        static_assert(O < 4, "");

        #if defined(AVEL_SSSE3)
        return vec4x32i{_mm_alignr_epi8(a, b, 4 * O)};
        #elif defined(AVEL_SSE2)
        return vec4x32i{_mm_or_si128(_mm_bslli_si128(a, 4 * (4 - O)), _mm_bsrli_si128(b, O))};
        #endif
    }
    */

    [[nodiscard]]
    AVEL_FINL vec4x32i broadcast_mask(mask4x32i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec4x32i{_mm_movm_epi32(decay(m))};

        #elif defined(AVEL_AVX512VL)
        const auto x = _mm_set1_epi32(0);
        const auto y = _mm_set1_epi32(-1);
        return vec4x32i{_mm_mask_blend_epi32(decay(m), x, y)};

        #elif defined(AVEL_SSE2)
        return vec4x32i{decay(m)};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32i{decay(m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i blend(vec4x32i a, vec4x32i b, mask4x32i m) {
        return vec4x32i{blend(vec4x32u{a}, vec4x32u{b}, mask4x32u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i max(vec4x32i a, vec4x32i b) {
        #if defined(AVEL_SSE41)
        return vec4x32i{_mm_max_epi32(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a, b, a < b);

        #endif

        #if defined(AVEL_NEON)
        return vec4x32i{vmaxq_s32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i min(vec4x32i a, vec4x32i b) {
        #if defined(AVEL_SSE41)
        return vec4x32i{_mm_min_epi32(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a, b, b < a);

        #endif

        #if defined(AVEL_NEON)
        return vec4x32i{vminq_s32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 2> minmax(vec4x32i a, vec4x32i b) {
        #if defined(AVEL_SSE41)
        return {
            vec4x32i{_mm_min_epi32(decay(a), decay(b))},
            vec4x32i{_mm_max_epi32(decay(a), decay(b))}
        };
        #elif defined(AVEL_SSE2)
        auto mask = b < a;
        return {blend(a, b, mask), blend(a, b, !mask)};
        #endif

        #if defined(AVEL_NEON)
        return {
            vec4x32i{vminq_s32(decay(a), decay(b))},
            vec4x32i{vmaxq_s32(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i clamp(vec4x32i x, vec4x32i lo, vec4x32i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i average(vec4x32i x, vec4x32i y) {
        #if defined(AVEL_AVX512VL)
        auto z = (x >> 1) + (y >> 1);
        auto c = _mm_cmplt_epi32(decay(z), _mm_setzero_si128());
        auto d = _mm_ternarylogic_epi32(decay(x), decay(y), c, 0xE8);

        return z + (vec4x32i{d} & vec4x32i{0x1});

        #elif defined(AVEL_SSE2)
        auto z = (x >> 1) + (y >> 1);
        auto c = z < vec4x32i{0};
        auto d = (x & y) | (broadcast_mask(c) & (x ^ y));

        return z + (d & vec4x32i{0x1});

        #endif

        #if defined(AVEL_NEON)
        return vec4x32i{vhaddq_s32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i midpoint(vec4x32i a, vec4x32i b) {
        const vec4x32u addition_mask{0x80000000};

        auto x = vec4x32u{a} ^ addition_mask;
        auto y = vec4x32u{b} ^ addition_mask;

        return vec4x32i{midpoint(x, y) ^ addition_mask};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i abs(vec4x32i x) {
        #if defined(AVEL_SSSE3)
        return vec4x32i{_mm_abs_epi32(decay(x))};
        #elif defined(AVEL_SSE2)
        auto y = x >> 31;
        return (x ^ y) - y;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i neg_abs(vec4x32i x) {
        #if defined(AVEL_SSSE3)
        return -vec4x32i{_mm_abs_epi32(decay(x))};
        #elif defined(AVEL_SSE2)
        auto y = ~(x >> 31);
        return (x ^ y) - y;
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32i load<vec4x32i>(const std::int32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32i{vld1q_s32(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32i aligned_load<vec4x32i>(const std::int32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32i{vld1q_s32(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u gather<vec4x32u>(const std::uint32_t* ptr, vec4x32i indices) {
        #if defined(AVEL_AVX2)
        return vec4x32u{_mm_i32gather_epi32(ptr, indices, sizeof(std::uint32_t))};
        #elif defined(AVEL_SSE2)
        auto i = to_array(indices);

        auto a = _mm_loadu_si32(ptr + i[0]);
        auto b = _mm_loadu_si32(ptr + i[1]);
        auto c = _mm_loadu_si32(ptr + i[2]);
        auto d = _mm_loadu_si32(ptr + i[3]);

        auto abab = _mm_unpacklo_epi32(a, b);
        auto cdcd = _mm_unpacklo_epi32(c, d);

        auto abcd = _mm_unpacklo_epi64(abab, cdcd);
        return vec4x32u{abcd};
        #endif

        #if defined(AVEL_NEON)
        vec4x32u::primitive read_data;
        read_data = vld1q_lane_u32(ptr + vgetq_lane_s32(decay(indices), 0x0), read_data, 0x0);
        read_data = vld1q_lane_u32(ptr + vgetq_lane_s32(decay(indices), 0x1), read_data, 0x1);
        read_data = vld1q_lane_u32(ptr + vgetq_lane_s32(decay(indices), 0x2), read_data, 0x2);
        read_data = vld1q_lane_u32(ptr + vgetq_lane_s32(decay(indices), 0x3), read_data, 0x3);

        return vec4x32u{read_data};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32i gather<vec4x32i>(const std::int32_t* ptr, vec4x32i indices) {
        #if defined(AVEL_AVX2)
        return vec4x32i{_mm_i32gather_epi32(ptr, indices, sizeof(std::int32_t))};
        #elif defined(AVEL_SSE2)
        auto i = to_array(indices);

        __m128i a = _mm_loadu_si32(ptr + i[0]);
        __m128i b = _mm_loadu_si32(ptr + i[1]);
        __m128i c = _mm_loadu_si32(ptr + i[2]);
        __m128i d = _mm_loadu_si32(ptr + i[3]);

        __m128i abab = _mm_unpacklo_epi32(a, b);
        __m128i cdcd = _mm_unpacklo_epi32(c, d);

        __m128i abcd = _mm_unpacklo_epi64(abab, cdcd);
        return vec4x32i{abcd};
        #endif

        #if defined(AVEL_NEON)
        vec4x32i::primitive read_data;
        read_data = vld1q_lane_s32(ptr + vgetq_lane_s32(decay(indices), 0x0), read_data, 0x0);
        read_data = vld1q_lane_s32(ptr + vgetq_lane_s32(decay(indices), 0x1), read_data, 0x1);
        read_data = vld1q_lane_s32(ptr + vgetq_lane_s32(decay(indices), 0x2), read_data, 0x2);
        read_data = vld1q_lane_s32(ptr + vgetq_lane_s32(decay(indices), 0x3), read_data, 0x3);

        return vec4x32i{read_data};
        #endif
    }

    AVEL_FINL void store(std::int32_t* ptr, vec4x32i x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s32(ptr, decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(std::int32_t* ptr, vec4x32i x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s32(ptr, decay(x));
        #endif
    }

    AVEL_FINL void scatter(std::uint32_t* ptr, vec4x32i indices, vec4x32u x) {
        #if defined(AVEL_AVX512VL)
        _mm_i32scatter_epi32(ptr, indices, x, sizeof(std::int32_t));
        #elif defined(AVEL_SSE2)
        auto i = to_array(indices);

        _mm_storeu_si32(ptr + i[0], decay(x));
        _mm_storeu_si32(ptr + i[1], _mm_bsrli_si128(decay(x), 0x4));
        _mm_storeu_si32(ptr + i[2], _mm_bsrli_si128(decay(x), 0x8));
        _mm_storeu_si32(ptr + i[3], _mm_bsrli_si128(decay(x), 0xC));
        #endif

        #if defined(AVEL_NEON)
        vst1q_lane_u32(ptr + vgetq_lane_s32(decay(indices), 0x0), decay(x), 0x0);
        vst1q_lane_u32(ptr + vgetq_lane_s32(decay(indices), 0x1), decay(x), 0x1);
        vst1q_lane_u32(ptr + vgetq_lane_s32(decay(indices), 0x2), decay(x), 0x2);
        vst1q_lane_u32(ptr + vgetq_lane_s32(decay(indices), 0x3), decay(x), 0x3);
        #endif
    }

    AVEL_FINL void scatter(std::int32_t* ptr, vec4x32i indices, vec4x32i x) {
        #if defined(AVEL_AVX512VL)
        _mm_i32scatter_epi32(ptr, indices, x, sizeof(std::int32_t));
        #elif defined(AVEL_SSE2)
        auto i = to_array(indices);

        _mm_storeu_si32(ptr + i[0], decay(x));
        _mm_storeu_si32(ptr + i[1], _mm_bsrli_si128(decay(x), 0x4));
        _mm_storeu_si32(ptr + i[2], _mm_bsrli_si128(decay(x), 0x8));
        _mm_storeu_si32(ptr + i[3], _mm_bsrli_si128(decay(x), 0xC));
        #endif

        #if defined(AVEL_NEON)
        vst1q_lane_s32(ptr + vgetq_lane_s32(decay(indices), 0x0), decay(x), 0x0);
        vst1q_lane_s32(ptr + vgetq_lane_s32(decay(indices), 0x1), decay(x), 0x1);
        vst1q_lane_s32(ptr + vgetq_lane_s32(decay(indices), 0x2), decay(x), 0x2);
        vst1q_lane_s32(ptr + vgetq_lane_s32(decay(indices), 0x3), decay(x), 0x3);
        #endif
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec4x32i> div(vec4x32i x, vec4x32i y) {
        vec4x32i quotient{};

        mask4x32i sign_mask0 = (x < vec4x32i{0x0});
        mask4x32i sign_mask1 = (y < vec4x32i{0x0});

        mask4x32i sign_mask2 = sign_mask0 ^ sign_mask1;

        vec4x32u numerator{abs(x)};
        vec4x32u denominator{abs(y)};

        //TODO: Compute i more appropriately
        //TODO: Otherwise optimize

        std::int32_t i = 32;

        for (; (i-- > 0) && any(mask4x32u(numerator));) {
            mask4x32u b = ((numerator >> i) >= denominator);
            numerator -= (broadcast_mask(b) & (denominator << i));
            quotient |= vec4x32i{vec4x32u{b} << i};
        }

        //Adjust quotient's sign. Should be xor of operands' signs
        quotient = blend(quotient, -quotient, sign_mask2);

        //Adjust numerator's sign. Should be same sign as it was originally
        x = blend(vec4x32i{numerator}, -vec4x32i{numerator}, sign_mask0);

        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i popcount(vec4x32i v) {
        return vec4x32i{popcount(vec4x32u(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i byteswap(vec4x32i v) {
        return vec4x32i(byteswap(vec4x32u(v)));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i countl_zero(vec4x32i x) {
        return vec4x32i{countl_zero(vec4x32u(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i countl_one(vec4x32i x) {
        return vec4x32i{countl_one(vec4x32u(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i countr_zero(vec4x32i x) {
        return vec4x32i{countr_zero(vec4x32u(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i countr_one(vec4x32i x) {
        return vec4x32i{countr_one(vec4x32u(x))};
    }

    [[nodiscard]]
    AVEL_FINL mask4x32i has_single_bit(vec4x32i x) {
        return mask4x32i(has_single_bit(vec4x32u(x)));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotl(vec4x32i v, std::uint32_t s) {
        return vec4x32i(rotl(vec4x32u(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotl(vec4x32i v, vec4x32u s) {
        return vec4x32i(rotl(vec4x32u(v), vec4x32u(s)));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotr(vec4x32i v, std::uint32_t s) {
        return vec4x32i(rotr(vec4x32u(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotr(vec4x32i v, vec4x32u s) {
        return vec4x32i(rotr(vec4x32u(v), vec4x32u(s)));
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<std::int32_t, 4> to_array(vec4x32i v) {
        alignas(16) std::array<std::int32_t, 4> ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec4x32i>(vec4x32i m) {
        return std::array<vec4x32i, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 4> convert<vec1x8u, vec4x32i>(vec4x32i m) {
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
    AVEL_FINL std::array<vec1x8i, 4> convert<vec1x8i, vec4x32i>(vec4x32i m) {
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
    AVEL_FINL std::array<vec1x16u, 4> convert<vec1x16u, vec4x32i>(vec4x32i m) {
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
    AVEL_FINL std::array<vec1x16i, 4> convert<vec1x16i, vec4x32i>(vec4x32i m) {
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
    AVEL_FINL std::array<vec1x32u, 4> convert<vec1x32u, vec4x32i>(vec4x32i m) {
        alignas(16) std::array<vec1x32u, 4> ret;

        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(m));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 4> convert<vec1x32i, vec4x32i>(vec4x32i m) {
        alignas(16) std::array<vec1x32i, 4> ret;
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(m));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64u, 4> convert<vec1x64u, vec4x32i>(vec4x32i m) {
        alignas(32) std::array<vec1x64u, 4> ret;

        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data()), _mm256_cvtepi32_epi64(decay(m)));

        #elif defined(AVEL_SSE2)
        auto whole = decay(m);
        auto wholeb = _mm_cmplt_epi32(whole, _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), _mm_unpacklo_epi32(whole, wholeb));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), _mm_unpackhi_epi32(whole, wholeb));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 4> convert<vec1x64i, vec4x32i>(vec4x32i m) {
        alignas(32) std::array<vec1x64i, 4> ret;

        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data()), _mm256_cvtepi32_epi64(decay(m)));

        #elif defined(AVEL_SSE2)
        auto whole = decay(m);
        auto wholeb = _mm_cmplt_epi32(whole, _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), _mm_unpacklo_epi32(whole, wholeb));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), _mm_unpackhi_epi32(whole, wholeb));

        #endif
        return ret;
    }

    /*
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 4> convert<vec1x32f, vec4x32i>(vec4x32i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64f, 4> convert<vec1x64f, vec4x32i>(vec4x32i m) {
        //TODO: Implement
    }
    */

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec4x32i>(vec4x32i m) {
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
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec4x32i>(vec4x32i m) {
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
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec4x32i>(vec4x32i m) {
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
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec4x32i>(vec4x32i m) {
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
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec4x32i>(vec4x32i m) {
        return {vec4x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec1x8u>(vec1x8u m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32i{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec1x8i>(vec1x8i m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32i{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec1x16u>(vec1x16u m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32i{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec1x16i>(vec1x16i m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32i{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec1x32u>(vec1x32u m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32i{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec1x32i>(vec1x32i m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32i{_mm_cvtsi32_si128(decay(m))}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec1x64u>(vec1x64u m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32i{_mm_cvtsi32_si128(decay(m) & 0x00000000FFFFFFFFull)}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec1x64i>(vec1x64i m) {
        #if defined(AVEL_SSE2)
        return {
            vec4x32i{_mm_cvtsi32_si128(decay(m) & 0x00000000FFFFFFFFull)}
        };
        #endif
    }

    /*
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec1x32f>(vec1x32f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec1x64f>(vec1x64f m) {
        //TODO: Implement
    }
    */

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 4> convert<vec4x32i, vec16x8u>(vec16x8u m) {
        #if defined(AVEL_SSE41)
        return {
            vec4x32i{_mm_cvtepu8_epi32(decay(m))},
            vec4x32i{_mm_cvtepu8_epi32(_mm_srli_si128(decay(m), 0x4))},
            vec4x32i{_mm_cvtepu8_epi32(_mm_srli_si128(decay(m), 0x8))},
            vec4x32i{_mm_cvtepu8_epi32(_mm_srli_si128(decay(m), 0xC))}
        };

        #elif defined(AVEL_SSE2)
        auto zeros = _mm_setzero_si128();

        auto half0 = _mm_unpacklo_epi8(decay(m), zeros);
        auto half1 = _mm_unpacklo_epi8(decay(m), zeros);

        return {
            vec4x32i{_mm_unpacklo_epi16(half0, zeros)},
            vec4x32i{_mm_unpackhi_epi16(half0, zeros)},
            vec4x32i{_mm_unpacklo_epi16(half1, zeros)},
            vec4x32i{_mm_unpackhi_epi16(half1, zeros)}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 4> convert<vec4x32i, vec16x8i>(vec16x8i m) {
        #if defined(AVEL_SSE41)
        return {
            vec4x32i{_mm_cvtepi8_epi32(decay(m))},
            vec4x32i{_mm_cvtepi8_epi32(_mm_srli_si128(decay(m), 0x4))},
            vec4x32i{_mm_cvtepi8_epi32(_mm_srli_si128(decay(m), 0x8))},
            vec4x32i{_mm_cvtepi8_epi32(_mm_srli_si128(decay(m), 0xC))}
        };

        #elif defined(AVEL_SSE2)
        auto whole = decay(m);
        auto wholeb = _mm_cmplt_epi8(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi8(whole, wholeb);
        auto half0b = _mm_cmplt_epi16(half0, _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi8(whole, wholeb);
        auto half1b = _mm_cmplt_epi16(half1, _mm_setzero_si128());

        return {
            vec4x32i{_mm_unpacklo_epi16(half0, half0b)},
            vec4x32i{_mm_unpackhi_epi16(half0, half0b)},
            vec4x32i{_mm_unpacklo_epi16(half1, half1b)},
            vec4x32i{_mm_unpackhi_epi16(half1, half1b)}
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 2> convert<vec4x32i, vec8x16u>(vec8x16u m) {
        #if defined(AVEL_SSE41)
        auto whole = decay(m);

        return {
            vec4x32i{_mm_cvtepu16_epi32(whole)},
            vec4x32i{_mm_cvtepu16_epi32(_mm_srli_si128(whole, 8))}
        };

        #elif defined(AVEL_SSE2)
        auto whole = decay(m);

        return {
            vec4x32i{_mm_unpacklo_epi16(whole, _mm_setzero_si128())},
            vec4x32i{_mm_unpackhi_epi16(whole, _mm_setzero_si128())}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 2> convert<vec4x32i, vec8x16i>(vec8x16i m) {

        #if defined(AVEL_SSE41)
        auto whole = decay(m);

        return {
            vec4x32i{_mm_cvtepi16_epi32(whole)},
            vec4x32i{_mm_cvtepi16_epi32(_mm_srli_si128(whole, 8))}
        };

        #elif defined(AVEL_SSE2)
        auto whole = decay(m);
        auto wholeb = _mm_cmplt_epi16(whole, _mm_setzero_si128());

        return {
            vec4x32i{_mm_unpacklo_epi16(whole, wholeb)},
            vec4x32i{_mm_unpackhi_epi16(whole, wholeb)}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec4x32u>(vec4x32u m) {
        return {vec4x32i{vec4x32i::primitive(decay(m))}};
    }

}

#endif
