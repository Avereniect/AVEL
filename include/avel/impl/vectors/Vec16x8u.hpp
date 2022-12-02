#ifndef AVEL_VEC16X8U_HPP
#define AVEL_VEC16X8U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x8u = Vector<std::uint8_t, 16>;

    using mask16x8u = Vector_mask<std::uint8_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x8u> div(vec16x8u numerator, vec16x8u denominator);
    vec16x8u broadcast_bits(mask16x8u m);
    vec16x8u blend(vec16x8u a, vec16x8u b, mask16x8u m);
    vec16x8u countl_one(vec16x8u x);





    template<>
    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
    class Vector_mask<std::uint8_t, 16> : public avel_impl::Vector_mask16xT {
        using base = avel_impl::Vector_mask16xT;
    #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
    class Vector_mask<std::uint8_t, 16> : public avel_impl::Vector_mask128b {
        using base = avel_impl::Vector_mask128b;
    #endif
    public:

        //=================================================
        // Static constants
        //=================================================

        static constexpr std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint8x16_t;
        #endif

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        AVEL_FINL explicit Vector_mask(base b):
            base(b) {}

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vector_mask(primitive p):
            base(p) {}
        #endif

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            base(convert<Vector_mask>(v)[0]) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 16>& arr) {
            static_assert(sizeof(bool) == 1);

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m128i array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = primitive(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_SSE2)
            primitive array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = _mm_cmplt_epi8(_mm_setzero_si128(), array_data);

            #endif

            #if defined(AVEL_NEON)
            content = vcltq_u8(vdupq_n_u8(0x0), vld1q_u8(reinterpret_cast<const std::uint8_t*>(arr.data())));
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

        #if defined(AVEL_NEON)
        Vector_mask& operator=(primitive p) {
            content = p;
            return *this;
        }
        #endif

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
        // Bitwise operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            return Vector_mask{base::operator!()};
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
        AVEL_FINL explicit operator uint8x16_t() {
            return content;
        }
        #endif

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(_mm512_mask2int(decay(m)));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m)));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vsubq_s8(	vdupq_n_s8(0x00), decay(m));
        return static_cast<std::uint32_t>(vaddvq_s8(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vsubq_u8(vdupq_n_u8(0x00), decay(m));
        auto t1 = vpadd_u8(vget_low_u8(t0), vget_high_u8(t0));
        auto t2 = vpadd_u8(t1, t1);
        auto t3 = vpadd_u8(t2, t2);
        auto t4 = vpadd_u8(t3, t3);

        return static_cast<std::uint32_t>(vget_lane_u8(t4, 0));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vmaxvq_u8(decay(m)) != 0x00;

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u8(decay(m));
        auto t1 = vpmax_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmax_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) != 0x00;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return 0xFFFF == _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE41)
        return 0x1 == _mm_test_all_ones(decay(m));
        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vminvq_u8(decay(m)) == 0xFF;

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u8(decay(m));
        auto t1 = vpmin_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmin_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) == 0xFFFFFFFF;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x8u m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask16x8u>(mask16x8u m) {
        return {m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 16> convert<mask1x8u, mask16x8u>(mask16x8u m) {
        alignas(16) std::array<mask1x8u, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto whole = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), whole);

        #elif defined(AVEL_SSE2)
        auto whole = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), whole);

        #endif

        #if defined(AVEL_NEON)
        auto zeros = mask16x8u::primitive{};
        auto whole = vsubq_u8(zeros, decay(m));
        vst1q_u8(reinterpret_cast<std::uint8_t*>(ret.data()), whole);

        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 16> convert<mask1x8i, mask16x8u>(mask16x8u m) {
        alignas(16) std::array<mask1x8i, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        #if defined(AVEL_NEON)
        auto zeros = mask16x8u::primitive{};
        auto whole = vsubq_u8(zeros, decay(m));
        vst1q_u8(reinterpret_cast<std::uint8_t*>(ret.data()), whole);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 16> convert<mask1x16u, mask16x8u>(mask16x8u m) {
        alignas(16) std::array<mask1x16u, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        #if defined(AVEL_NEON)
        auto zeros = mask16x8u::primitive{};
        auto whole = vsubq_u8(zeros, decay(m));
        vst1q_u8(reinterpret_cast<std::uint8_t*>(ret.data()), whole);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 16> convert<mask1x16i, mask16x8u>(mask16x8u m) {
        alignas(16) std::array<mask1x16i, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        #if defined(AVEL_NEON)
        auto zeros = mask16x8u::primitive{};
        auto whole = vsubq_u8(zeros, decay(m));
        vst1q_u8(reinterpret_cast<std::uint8_t*>(ret.data()), whole);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 16> convert<mask1x32u, mask16x8u>(mask16x8u m) {
        alignas(16) std::array<mask1x32u, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        #if defined(AVEL_NEON)
        auto zeros = mask16x8u::primitive{};
        auto whole = vsubq_u8(zeros, decay(m));
        vst1q_u8(reinterpret_cast<std::uint8_t*>(ret.data()), whole);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 16> convert<mask1x32i, mask16x8u>(mask16x8u m) {
        alignas(16) std::array<mask1x32i, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        #if defined(AVEL_NEON)
        auto zeros = mask16x8u::primitive{};
        auto whole = vsubq_u8(zeros, decay(m));
        vst1q_u8(reinterpret_cast<std::uint8_t*>(ret.data()), whole);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64u, 16> convert<mask1x64u, mask16x8u>(mask16x8u m) {
        alignas(16) std::array<mask1x64u, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        #if defined(AVEL_NEON)
        auto zeros = mask16x8u::primitive{};
        auto whole = vsubq_u8(zeros, decay(m));
        vst1q_u8(reinterpret_cast<std::uint8_t*>(ret.data()), whole);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 16> convert<mask1x64i, mask16x8u>(mask16x8u m) {
        alignas(16) std::array<mask1x64i, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        #if defined(AVEL_NEON)
        auto zeros = mask16x8u::primitive{};
        auto whole = vsubq_u8(zeros, decay(m));
        vst1q_u8(reinterpret_cast<std::uint8_t*>(ret.data()), whole);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 16> convert<mask1x32f, mask16x8u>(mask16x8u m) {
        alignas(16) std::array<mask1x32f, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        #if defined(AVEL_NEON)
        auto zeros = mask16x8u::primitive{};
        auto whole = vsubq_u8(zeros, decay(m));
        vst1q_u8(reinterpret_cast<std::uint8_t*>(ret.data()), whole);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64f, 16> convert<mask1x64f, mask16x8u>(mask16x8u m) {
        alignas(16) std::array<mask1x64f, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        #if defined(AVEL_NEON)
        auto zeros = mask16x8u::primitive{};
        auto whole = vsubq_u8(zeros, decay(m));
        vst1q_u8(reinterpret_cast<std::uint8_t*>(ret.data()), whole);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask1x8u>(mask1x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        std::uint8_t x = -decay(m);
        mask16x8u::primitive reg{
            x, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        };

        return {mask16x8u{reg}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask1x8i>(mask1x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask1x16u>(mask1x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask1x16i>(mask1x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask1x32u>(mask1x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask1x32i>(mask1x32i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask1x64u>(mask1x64u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask1x64i>(mask1x64i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask1x32f>(mask1x32f m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask1x64f>(mask1x64f m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }





    template<>
    class alignas(16) Vector<std::uint8_t, 16> : public avel_impl::Vec16x8int {
        using base = avel_impl::Vec16x8int;
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint8_t;

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint8x16_t;
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

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            base(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            base(_mm_set1_epi8(x)) {}
        #endif
        #if defined(AVEL_NEON)
            base() {} //TODO: Implement
        #endif

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            base(_mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)))) {}
        #elif defined(AVEL_SSE2)
            base(_mm_sub_epi8(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            base() {} //TODO: Implement
        #endif

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
        #if defined(AVEL_SSE2)
            base(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            base() {} //TODO: Implement
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
            #if defined(AVEL_SSE2)
            content = _mm_set1_epi8(x);
            #endif
            #if defined(AVEL_NEON)
            //TODO: Implement
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmplt_epu8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            Vector addition_mask{0x80};
            return mask{_mm_cmplt_epi8(decay(lhs ^ addition_mask), decay(rhs ^ addition_mask))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmple_epu8_mask(decay(lhs), decay(rhs))};
            #elif defined(AVEL_SSE2)
            return !mask{lhs > rhs};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpgt_epu8_mask(decay(lhs), decay(rhs))};
            #elif defined(AVEL_SSE2)
            Vector add_mask{0x80};
            return mask{_mm_cmpgt_epi8(decay(lhs ^ add_mask), decay(rhs ^ add_mask))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpge_epu8_mask(decay(lhs), decay(rhs))};
            #elif defined(AVEL_SSE2)
            return !mask{lhs < rhs};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        //Definition of operator-() deferred until after definition of vec16x8i

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
            *this = decay(results.quot);
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector rhs) {
            auto results = div(*this, rhs);
            *this = decay(results.rem);
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto whole = _mm256_cvtepu8_epi16(content);
            auto shifted = _mm256_srl_epi16(whole, _mm_cvtsi32_si128(rhs));
            content = _mm256_cvtepi16_epi8(shifted);

            #elif defined(AVEL_SSE2)
            __m128i s = _mm_cvtsi32_si128(rhs);
            __m128i shifted = _mm_srl_epi16(content, s);
            __m128i mask = _mm_set1_epi8(std::uint8_t(0xFF >> rhs));
            content = _mm_and_si128(shifted, mask);

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m256i whole = _mm256_cvtepu8_epi16(content);
            __m256i shifts = _mm256_cvtepu8_epi16(rhs.content);
            content = _mm256_cvtepi16_epi8(_mm256_srlv_epi16(whole, shifts));

            #elif defined(AVEL_AVX2)
            auto lhs_whole = _mm256_cvtepu8_epi16(content);
            auto rhs_whole = _mm256_cvtepu8_epi16(rhs.content);

            auto shifted = _mm256_srlv_epi16(lhs_whole, rhs_whole);
            auto packed = _mm256_packus_epi16(shifted, _mm256_zextsi128_si256(_mm256_extractf128_si256(shifted, 0x1)));

            content = _mm256_castsi256_si128(packed);

            #elif defined(AVEL_SSE2)
            //TODO: Offer divide and conqueuer approach?

            alignas(16) std::uint8_t data[16];
            alignas(16) std::uint8_t shifts[16];

            _mm_store_si128(reinterpret_cast<__m128i*>(data), content);
            _mm_store_si128(reinterpret_cast<__m128i*>(shifts), rhs.content);

            data[0x0] >>= shifts[0x0];
            data[0x1] >>= shifts[0x1];
            data[0x2] >>= shifts[0x2];
            data[0x3] >>= shifts[0x3];
            data[0x4] >>= shifts[0x4];
            data[0x5] >>= shifts[0x5];
            data[0x6] >>= shifts[0x6];
            data[0x7] >>= shifts[0x7];
            data[0x8] >>= shifts[0x8];
            data[0x9] >>= shifts[0x9];
            data[0xa] >>= shifts[0xa];
            data[0xb] >>= shifts[0xb];
            data[0xc] >>= shifts[0xc];
            data[0xd] >>= shifts[0xd];
            data[0xe] >>= shifts[0xe];
            data[0xf] >>= shifts[0xf];

            content = _mm_load_si128(reinterpret_cast<const __m128i*>(data));

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
        // Conversion operators
        //=================================================

        #if defined(AVEL_NEON)
        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }
        #endif

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return Vector{} != *this;
        }

    };

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x8u broadcast_bits(mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec16x8u{_mm_movm_epi8(decay(m))};

        #elif defined(AVEL_SSE2)
        return vec16x8u{decay(m)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u blend(vec16x8u a, vec16x8u b, mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec16x8u{_mm_mask_blend_epi8(decay(m), decay(a), decay(b))};

        #elif defined(AVEL_SSE41)
        return vec16x8u{_mm_blendv_epi8(decay(a), decay(b), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(decay(m), decay(a));
        auto y = _mm_and_si128(decay(m), decay(b));
        return vec16x8u{_mm_or_si128(x, y)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u max(vec16x8u a, vec16x8u b) {
        #if defined(AVEL_SSE2)
        return vec16x8u{_mm_max_epu8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u min(vec16x8u a, vec16x8u b) {
        #if defined(AVEL_SSE2)
        return vec16x8u{_mm_min_epu8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 2> minmax(vec16x8u a, vec16x8u b) {
        #if defined(AVEL_SSE2)
        return {
            vec16x8u{_mm_min_epu8(decay(a), decay(b))},
            vec16x8u{_mm_max_epu8(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u clamp(vec16x8u x, vec16x8u lo, vec16x8u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u average(vec16x8u a, vec16x8u b) {
        #if defined(AVEL_SSE2)
        auto t0 = vec16x8u{_mm_avg_epu8(decay(a), decay(b))};
        auto t1 = a ^ b;
        auto t2 = vec16x8u{0x1};
        return t0 - (t1 & t2);

        #endif

        #if defined(AVEL_NEON)
        return vec16x8u{vhaddq_u8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u midpoint(vec16x8u a, vec16x8u b) {
        #if defined(AVEL_SSE2)
        auto t0 = decay(broadcast_bits(b < a));
        auto t1 = _mm_avg_epu8(decay(a), decay(b));
        auto t2 = decay(a ^ b);
        auto t3 = _mm_andnot_si128(t0, t2);
        auto t4 = _mm_set1_epi8(0x1);
        auto t5 = _mm_and_si128(t3, t4);
        auto t6 = _mm_sub_epi8(t1, t5);
        return vec16x8u{t6};
        #endif

        #if defined(AVEL_NEON)
        //auto t0 = decay(b < a);
        //auto t1 = a ^ b;
        //auto t2 = _mm_and_si128(t0, t1);
        //auto t3 = _mm_set1_epi8(0x1);
        //auto t4 = _mm_and_si128(t2, t3);
        //auto t5 = decay(vhaddq_u8(a, b));
        //auto t6 = _mm_add_epi8(t5, t4);
        //return vec16x8u{t6};

        vec16x8u t0 = vec16x8u{vhaddq_u8(decay(a), decay(b))};
        vec16x8u t1 = (a | b) & vec16x8u{0x1} & broadcast_bits(a > b);
        return t0 | t1;
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8u load<vec16x8u>(const std::uint8_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec16x8u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8u aligned_load<vec16x8u>(const std::uint8_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec16x8u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    AVEL_FINL void store(std::uint8_t* ptr, vec16x8u x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(std::uint8_t* ptr, vec16x8u x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec16x8u> div(vec16x8u x, vec16x8u y) {
        vec16x8u quotient{};
        std::int32_t i = 8;

        //TODO: Optimize body with AVX512 mask instructions
        for (; (i-- > 0) && any(mask16x8u(x));) {
            mask16x8u b = ((x >> i) >= y);
            x -= (broadcast_bits(b) & (y << i));
            quotient |= (vec16x8u{b} << i);
        }

        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u popcount(vec16x8u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        return vec16x8u{_mm_popcnt_epi8(decay(x))};

        #elif defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        __m128i table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        __m128i nibble_mask = _mm_set1_epi8(0x0F);

        __m128i index0 = _mm_and_si128(decay(x), nibble_mask);
        __m128i index1 = _mm_and_si128(_mm_srli_epi16(decay(x), 0x4), nibble_mask);

        __m128i partial_sum0 = _mm_shuffle_epi8(table, index0);
        __m128i partial_sum1 = _mm_shuffle_epi8(table, index1);

        __m128i sum = _mm_add_epi8(partial_sum0, partial_sum1);
        return vec16x8u{sum};

        #elif defined(AVEL_SSE2)
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        // Due to lack of 8-bit multiply instructions, the solution that doesn't
        // use multiplication is chosen here
        x = x - ((x >> 1) & vec16x8u{0x55});
        x = (x & vec16x8u{0x33}) + ((x >> 2) & vec16x8u{0x33});
        x = (x + (x >> 4)) & vec16x8u{0x0F};
        return x;

        #endif

        #if defined(AVEL_NEON)
        return vec16x8u{vcntq_s8(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u byteswap(vec16x8u x) {
        return x;
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u countl_zero(vec16x8u x) {
        #if defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            8, 3, 2, 2,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
            8, 7, 6, 6,
            5, 5, 5, 5,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        auto nibble_mask = _mm_set1_epi8(0x0F);
        auto lo_nibble = _mm_and_si128(nibble_mask, decay(x));
        auto hi_nibble = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x4));

        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data0));
        auto table1 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data1));

        auto partial0 = _mm_shuffle_epi8(table1, lo_nibble);
        auto partial1 = _mm_shuffle_epi8(table0, hi_nibble);

        auto ret = _mm_min_epu8(partial0, partial1);
        return vec16x8u{ret};

        #elif defined(AVEL_SSE2)
        return countl_one(~x);
        #endif

        #if defined(AVEL_NEON)
        return vec16x8u{vclzq_s8(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u countl_one(vec16x8u x) {
        #if defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            0, 0, 0, 0,
            0, 0, 0, 0,
            1, 1, 1, 1,
            2, 2, 3, 8
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
            4, 4, 4, 4,
            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 7, 8
        };

        auto nibble_mask = _mm_set1_epi8(0x0F);
        auto lo_nibble = _mm_and_si128(nibble_mask, decay(x));
        auto hi_nibble = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x4));

        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data0));
        auto table1 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data1));

        auto partial0 = _mm_shuffle_epi8(table1, lo_nibble);
        auto partial1 = _mm_shuffle_epi8(table0, hi_nibble);

        auto ret = _mm_min_epu8(partial0, partial1);
        return vec16x8u{ret};

        #elif defined(AVEL_SSE2)
        vec16x8u sum{x == vec16x8u{0xFF}};

        vec16x8u m0{0xF0u};
        mask16x8u b0 = (m0 & x) == m0;
        sum += broadcast_bits(b0) & vec16x8u{4};
        x <<= broadcast_bits(b0) & vec16x8u{4};

        vec16x8u m1{0xC0u};
        mask16x8u b1 = (m1 & x) == m1;
        sum += broadcast_bits(b1) & vec16x8u{2};
        x <<= broadcast_bits(b1) & vec16x8u{2};

        vec16x8u m2{0x80u};
        mask16x8u b2 = (m2 & x) == m2;
        sum += broadcast_bits(b2) & vec16x8u{1};

        return sum;
        #endif

        #if defined(AVEL_NEON)
        return countl_zero(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u countr_zero(vec16x8u x) {
        #if defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            8, 0, 1, 0,
            2, 0, 1, 0,
            3, 0, 1, 0,
            2, 0, 1, 0
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
            8, 4, 5, 4,
            6, 4, 5, 4,
            7, 4, 5, 4,
            6, 4, 5, 4
        };

        auto nibble_mask = _mm_set1_epi8(0x0F);
        auto lo_nibble = _mm_and_si128(nibble_mask, decay(x));
        auto hi_nibble = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x4));

        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data0));
        auto table1 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data1));

        auto partial0 = _mm_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm_min_epu8(partial0, partial1);
        return vec16x8u{ret};

        #elif defined(AVEL_SSE2)
        vec16x8u ret{0x00};

        mask16x8u zero_mask = x == vec16x8u{0x00};

        x &= vec16x8u{0x00} - x;

        mask16x8u b;
        b = mask16x8u(x & vec16x8u{0xAAu});
        ret |= (vec16x8u{b} << 0);
        b = mask16x8u(x & vec16x8u{0xCCu});
        ret |= (vec16x8u{b} << 1);
        b = mask16x8u(x & vec16x8u{0xF0u});
        ret |= (vec16x8u{b} << 2);

        ret = blend(ret, vec16x8u{8}, zero_mask);

        return ret;
        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u countr_one(vec16x8u x) {
        #if defined(AVEL_SSE3)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            0, 1, 0, 2,
            0, 1, 0, 3,
            0, 1, 0, 2,
            0, 1, 0, 8
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
            4, 5, 4, 6,
            4, 5, 4, 7,
            4, 5, 4, 6,
            4, 5, 4, 8
        };

        auto nibble_mask = _mm_set1_epi8(0x0F);
        auto lo_nibble = _mm_and_si128(nibble_mask, decay(x));
        auto hi_nibble = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x4));

        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data0));
        auto table1 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data1));

        auto partial0 = _mm_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm_min_epu8(partial0, partial1);
        return vec16x8u{ret};

        #elif defined(AVEL_SSE2)
        return countr_zero(~x);
        #endif

        #if defined(AVEL_NEON)
        return countr_zero(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u bit_width(vec16x8u x) {
        #if defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            0, 1, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
            0, 5, 6, 6,
            7, 7, 7, 7,
            8, 8, 8, 8,
            8, 8, 8, 8
        };

        auto nibble_mask = _mm_set1_epi8(0x0F);
        auto lo_nibble = _mm_and_si128(nibble_mask, decay(x));
        auto hi_nibble = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x4));

        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data0));
        auto table1 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data1));

        auto partial0 = _mm_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm_max_epu8(partial0, partial1);
        return vec16x8u{ret};

        #elif defined(AVEL_SSE2)
        mask16x8u zero_mask = (x == vec16x8u{0x00});

        vec16x8u ret{0x00};

        auto b0 = mask16x8u{x & vec16x8u{0xF0}};
        auto t0 = broadcast_bits(b0) & vec16x8u{4};
        ret += t0;
        x >>= t0;

        auto b1 = mask16x8u{x & vec16x8u{0xFC}};
        auto t1 = broadcast_bits(b1) & vec16x8u{2};
        ret += t1;
        x >>= t1;

        auto b2 = mask16x8u{x & vec16x8u{0xFE}};
        auto t2 = broadcast_bits(b2) & vec16x8u{1};
        ret += t2;

        return blend(ret + vec16x8u{1}, vec16x8u{0x0}, zero_mask);
        #endif

        #if defined(AVEL_NEON)
        return vec16x8u{8} - countl_zero(x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u bit_floor(vec16x8u x) {
        #if defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            0, 1, 2, 2,
            4, 4, 4, 4,
            8, 8, 8, 8,
            8, 8, 8, 8
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
              0,  16,  32,  32,
             64,  64,  64,  64,
            128, 128, 128, 128,
            128, 128, 128, 128
        };

        auto nibble_mask = _mm_set1_epi8(0x0F);
        auto lo_nibble = _mm_and_si128(nibble_mask, decay(x));
        auto hi_nibble = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x4));

        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data0));
        auto table1 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data1));

        auto partial0 = _mm_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm_max_epu8(partial0, partial1);
        return vec16x8u{ret};

        #elif defined(AVEL_SSE2)
        //TODO: Unroll to eliminate widening and narrowing
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u bit_ceil(vec16x8u x) {
        #if defined(AVEL_SSSE3)

        //TODO: Fix this suboptimal implementation
        auto zero_mask = (x == vec16x8u{0x0000});
        auto one_mask = (x == vec16x8u{0x0001});

        auto tmp = bit_floor(x - vec16x8u{1});
        tmp = blend(tmp + tmp, vec16x8u{0x0001}, zero_mask | one_mask);
        return tmp;

        /*
        alignas(16) static constexpr std::uint8_t table_data0[16] {
             0,  0,  1,  3,
             3,  7,  7,  7,
             7, 15, 15, 15,
            15, 15, 15, 15
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
              0,  15,  31,  63,
             63, 127, 127, 127,
            255, 255, 255, 255,
            255, 255, 255, 255
        };

        auto nibble_mask = _mm_set1_epi8(0x0F);
        auto lo_nibble = _mm_and_si128(nibble_mask, decay(x));
        auto hi_nibble = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x4));

        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data0));
        auto table1 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data1));

        auto partial0 = _mm_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm_add_epi8(_mm_max_epu8(partial0, partial1), _mm_set1_epi8(0x01));
        return vec16x8u{ret};
        */

        #elif defined(AVEL_SSE2)
        //TODO: Unroll to eliminate widening and narrowing

        auto zero_mask = (x == vec16x8u{0x00});

        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        ++x;

        return blend(x, vec16x8u{1}, zero_mask);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask16x8u has_single_bit(vec16x8u x) {
        #if defined(AVEL_AVX512BITALG)
        return popcount(x) == vec16x8u{1};

        #elif defined(AVEL_SSE2)
        return mask16x8u{x} && mask16x8u(x & (x - vec16x8u{1}));

        #endif

        #if defined(AVEL_NEON)
        return popcount(x) == vec16x8u{1};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u rotl(vec16x8u x, long long s) {
        //TODO: Manually inline to eliminate widening and narrowing
        s &= 0x7;
        return (x << s) | (x >> (8 - s));
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u rotl(vec16x8u x, vec16x8u s) {
        //TODO: Manually inline to eliminate widening and narrowing
        s &= vec16x8u{0x7};
        return (x << s) | (x >> (vec16x8u{8} - s));
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u rotr(vec16x8u x, long long s) {
        //TODO: Manually inline to eliminate widening and narrowing
        s &= 0x7;
        return (x >> s) | (x << (8 - s));
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u rotr(vec16x8u x, vec16x8u s) {
        //TODO: Manually inline to eliminate widening and narrowing
        s &= vec16x8u{0x7};
        return (x >> s) | (x << (vec16x8u{8} - s));
    }

    //=====================================================
    // Conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<std::uint8_t, 16> to_array(vec16x8u v) {
        alignas(16) std::array<std::uint8_t, 16> ret{};
        aligned_store(ret.data(), v);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec16x8u>(vec16x8u x) {
        return std::array<vec16x8u, 1>{x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 16> convert<vec1x8u, vec16x8u>(vec16x8u x) {
        alignas(16) std::array<vec1x8u, 16> ret;
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(x));
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 16> convert<vec1x8i, vec16x8u>(vec16x8u x) {
        alignas(16) std::array<vec1x8i, 16> ret;
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(x));
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 16> convert<vec1x16u, vec16x8u>(vec16x8u x) {
        alignas(32) std::array<vec1x16u, 16> ret;
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data()), _mm256_cvtepu8_epi16(decay(x)));

        #elif defined(AVEL_SSE2)
        __m128i lo = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        __m128i hi = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0), lo);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 8), hi);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 16> convert<vec1x16i, vec16x8u>(vec16x8u x) {
        alignas(32) std::array<vec1x16i, 16> ret;
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data()), _mm256_cvtepu8_epi16(decay(x)));

        #elif defined(AVEL_SSE2)
        __m128i lo = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        __m128i hi = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0), lo);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 8), hi);
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 16> convert<vec1x32u, vec16x8u>(vec16x8u x) {
        alignas(64) std::array<vec1x32u, 16> ret;
        #if defined(AVEL_AVX512F)
        __m512i lo = _mm512_cvtepu8_epi32(decay(x));
        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data()), lo);

        #elif defined(AVEL_AVX2)
        __m256i lo = _mm256_cvtepu8_epi32(decay(x));
        __m256i hi = _mm256_cvtepu8_epi32(_mm_unpackhi_epi64(decay(x), decay(x)));

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0), lo);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 8), hi);

        #elif defined(AVEL_SSE41)
        __m128i quart0 = _mm_cvtepu8_epi32(decay(x));
        __m128i quart1 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x4));
        __m128i quart2 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x8));
        __m128i quart3 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0xC));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quart0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quart1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), quart2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), quart3);

        #elif defined(AVEL_SSE2)
        __m128i lo = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        __m128i hi = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        __m128i quart0 = _mm_unpacklo_epi16(lo, _mm_setzero_si128());
        __m128i quart1 = _mm_unpackhi_epi16(lo, _mm_setzero_si128());
        __m128i quart2 = _mm_unpacklo_epi16(hi, _mm_setzero_si128());
        __m128i quart3 = _mm_unpackhi_epi16(hi, _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quart0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quart1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), quart2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), quart3);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 16> convert<vec1x32i, vec16x8u>(vec16x8u x) {
        alignas(64) std::array<vec1x32i, 16> ret;
        #if defined(AVEL_AVX512F)
        __m512i lo = _mm512_cvtepu8_epi32(decay(x));
        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data()), lo);

        #elif defined(AVEL_AVX2)
        __m256i lo = _mm256_cvtepu8_epi32(decay(x));
        __m256i hi = _mm256_cvtepu8_epi32(_mm_unpackhi_epi64(decay(x), decay(x)));

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0), lo);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 8), hi);

        #elif defined(AVEL_SSE41)
        __m128i quart0 = _mm_cvtepu8_epi32(decay(x));
        __m128i quart1 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x4));
        __m128i quart2 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x8));
        __m128i quart3 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0xC));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quart0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quart1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), quart2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), quart3);

        #elif defined(AVEL_SSE2)
        __m128i lo = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        __m128i hi = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        __m128i quart0 = _mm_unpacklo_epi16(lo, _mm_setzero_si128());
        __m128i quart1 = _mm_unpackhi_epi16(lo, _mm_setzero_si128());
        __m128i quart2 = _mm_unpacklo_epi16(hi, _mm_setzero_si128());
        __m128i quart3 = _mm_unpackhi_epi16(hi, _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quart0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quart1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), quart2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), quart3);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64u, 16> convert<vec1x64u, vec16x8u>(vec16x8u x) {
        alignas(128) std::array<vec1x64u, 16> ret;
        #if defined(AVEL_AVX512F)
        auto lo = _mm512_cvtepu8_epi64(decay(x));
        auto hi = _mm512_cvtepu8_epi64(_mm_unpackhi_epi64(decay(x), decay(x)));

        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data() + 0), lo);
        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data() + 8), hi);

        #elif defined(AVEL_AVX2)
        auto quart0 = _mm256_cvtepu8_epi64(decay(x));
        auto quart1 = _mm256_cvtepu8_epi64(_mm_srli_si128(decay(x), 0x4));
        auto quart2 = _mm256_cvtepu8_epi64(_mm_srli_si128(decay(x), 0x8));
        auto quart3 = _mm256_cvtepu8_epi64(_mm_srli_si128(decay(x), 0xC));

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x0), quart0);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x4), quart1);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x8), quart2);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0xC), quart3);

        #elif defined(AVEL_SSE41)
        auto lo = decay(x);
        auto hi = _mm_unpackhi_epi64(lo, lo);

        auto eighth0 = _mm_cvtepu8_epi64(lo);
        auto eighth1 = _mm_cvtepu8_epi64(_mm_srli_si128(lo, 0x2));
        auto eighth2 = _mm_cvtepu8_epi64(_mm_srli_si128(lo, 0x4));
        auto eighth3 = _mm_cvtepu8_epi64(_mm_srli_si128(lo, 0x6));
        auto eighth4 = _mm_cvtepu8_epi64(hi);
        auto eighth5 = _mm_cvtepu8_epi64(_mm_srli_si128(hi, 0x2));
        auto eighth6 = _mm_cvtepu8_epi64(_mm_srli_si128(hi, 0x4));
        auto eighth7 = _mm_cvtepu8_epi64(_mm_srli_si128(hi, 0x6));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), eighth0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), eighth1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), eighth2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), eighth3);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), eighth4);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xA), eighth5);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), eighth6);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xE), eighth7);

        #elif defined(AVEL_SSE2)
        auto half0 = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        auto quarter0 = _mm_unpacklo_epi16(half0, _mm_setzero_si128());
        auto quarter1 = _mm_unpackhi_epi16(half0, _mm_setzero_si128());
        auto quarter2 = _mm_unpacklo_epi16(half1, _mm_setzero_si128());
        auto quarter3 = _mm_unpackhi_epi16(half1, _mm_setzero_si128());

        auto eighth0 = _mm_unpacklo_epi32(quarter0, _mm_setzero_si128());
        auto eighth1 = _mm_unpackhi_epi32(quarter0, _mm_setzero_si128());
        auto eighth2 = _mm_unpacklo_epi32(quarter1, _mm_setzero_si128());
        auto eighth3 = _mm_unpackhi_epi32(quarter1, _mm_setzero_si128());
        auto eighth4 = _mm_unpacklo_epi32(quarter2, _mm_setzero_si128());
        auto eighth5 = _mm_unpackhi_epi32(quarter2, _mm_setzero_si128());
        auto eighth6 = _mm_unpacklo_epi32(quarter3, _mm_setzero_si128());
        auto eighth7 = _mm_unpackhi_epi32(quarter3, _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), eighth0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), eighth1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), eighth2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), eighth3);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), eighth4);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xA), eighth5);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), eighth6);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xE), eighth7);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 16> convert<vec1x64i, vec16x8u>(vec16x8u x) {
        alignas(128) std::array<vec1x64i, 16> ret;
        #if defined(AVEL_AVX512F)
        auto lo = _mm512_cvtepu8_epi64(decay(x));
        auto hi = _mm512_cvtepu8_epi64(_mm_unpackhi_epi64(decay(x), decay(x)));

        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data() + 0), lo);
        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data() + 8), hi);

        #elif defined(AVEL_AVX2)
        auto quart0 = _mm256_cvtepu8_epi64(decay(x));
        auto quart1 = _mm256_cvtepu8_epi64(_mm_srli_si128(decay(x), 0x4));
        auto quart2 = _mm256_cvtepu8_epi64(_mm_srli_si128(decay(x), 0x8));
        auto quart3 = _mm256_cvtepu8_epi64(_mm_srli_si128(decay(x), 0xC));

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x0), quart0);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x4), quart1);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x8), quart2);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0xC), quart3);

        #elif defined(AVEL_SSE41)
        auto lo = decay(x);
        auto hi = _mm_unpackhi_epi64(lo, lo);

        auto eighth0 = _mm_cvtepu8_epi64(lo);
        auto eighth1 = _mm_cvtepu8_epi64(_mm_srli_si128(lo, 0x2));
        auto eighth2 = _mm_cvtepu8_epi64(_mm_srli_si128(lo, 0x4));
        auto eighth3 = _mm_cvtepu8_epi64(_mm_srli_si128(lo, 0x6));
        auto eighth4 = _mm_cvtepu8_epi64(hi);
        auto eighth5 = _mm_cvtepu8_epi64(_mm_srli_si128(hi, 0x2));
        auto eighth6 = _mm_cvtepu8_epi64(_mm_srli_si128(hi, 0x4));
        auto eighth7 = _mm_cvtepu8_epi64(_mm_srli_si128(hi, 0x6));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), eighth0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), eighth1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), eighth2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), eighth3);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), eighth4);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xA), eighth5);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), eighth6);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xE), eighth7);

        #elif defined(AVEL_SSE2)
        auto half0 = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        auto quarter0 = _mm_unpacklo_epi16(half0, _mm_setzero_si128());
        auto quarter1 = _mm_unpackhi_epi16(half0, _mm_setzero_si128());
        auto quarter2 = _mm_unpacklo_epi16(half1, _mm_setzero_si128());
        auto quarter3 = _mm_unpackhi_epi16(half1, _mm_setzero_si128());

        auto eighth0 = _mm_unpacklo_epi32(quarter0, _mm_setzero_si128());
        auto eighth1 = _mm_unpackhi_epi32(quarter0, _mm_setzero_si128());
        auto eighth2 = _mm_unpacklo_epi32(quarter1, _mm_setzero_si128());
        auto eighth3 = _mm_unpackhi_epi32(quarter1, _mm_setzero_si128());
        auto eighth4 = _mm_unpacklo_epi32(quarter2, _mm_setzero_si128());
        auto eighth5 = _mm_unpackhi_epi32(quarter2, _mm_setzero_si128());
        auto eighth6 = _mm_unpacklo_epi32(quarter3, _mm_setzero_si128());
        auto eighth7 = _mm_unpackhi_epi32(quarter3, _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), eighth0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), eighth1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), eighth2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), eighth3);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), eighth4);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xA), eighth5);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), eighth6);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xE), eighth7);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 16> convert<vec1x32f, vec16x8u>(vec16x8u x) {
        alignas(64) std::array<vec1x32f, 16> ret;
        #if defined(AVEL_AVX512F)
        __m512 lo = _mm512_cvtepi32_ps(_mm512_cvtepu8_epi32(decay(x)));
        _mm512_store_ps(reinterpret_cast<float*>(ret.data()), lo);

        #elif defined(AVEL_AVX2)
        __m256 lo = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(decay(x)));
        __m256 hi = _mm256_cvtepi32_ps(_mm256_cvtepu8_epi32(_mm_unpackhi_epi64(decay(x), decay(x))));

        _mm256_store_ps(reinterpret_cast<float*>(ret.data() + 0), lo);
        _mm256_store_ps(reinterpret_cast<float*>(ret.data() + 8), hi);

        #elif defined(AVEL_SSE41)
        auto quart0 = _mm_cvtepu8_epi32(decay(x));
        auto quart1 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x4));
        auto quart2 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x8));
        auto quart3 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0xC));

        auto out0 = _mm_cvtepi32_ps(quart0);
        auto out1 = _mm_cvtepi32_ps(quart1);
        auto out2 = _mm_cvtepi32_ps(quart2);
        auto out3 = _mm_cvtepi32_ps(quart3);

        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x0), out0);
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x4), out1);
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x8), out2);
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0xC), out3);

        #elif defined(AVEL_SSE2)
        auto zeros = _mm_setzero_si128();

        auto half0 = _mm_unpacklo_epi8(decay(x), zeros);
        auto half1 = _mm_unpackhi_epi8(decay(x), zeros);

        auto quart0 = _mm_unpacklo_epi16(half0, zeros);
        auto quart1 = _mm_unpackhi_epi16(half0, zeros);
        auto quart2 = _mm_unpacklo_epi16(half1, zeros);
        auto quart3 = _mm_unpackhi_epi16(half1, zeros);

        auto out0 = _mm_cvtepi32_ps(quart0);
        auto out1 = _mm_cvtepi32_ps(quart1);
        auto out2 = _mm_cvtepi32_ps(quart2);
        auto out3 = _mm_cvtepi32_ps(quart3);

        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x0), out0);
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x4), out1);
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x8), out2);
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0xC), out3);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64f, 16> convert<vec1x64f, vec16x8u>(vec16x8u x) {
        alignas(128) std::array<vec1x64f, 16> ret;

        #if defined(AVEL_AVX512F)
        auto tmp = _mm512_cvtepu8_epi32(decay(x));

        auto lo = _mm512_cvtepi32_pd(_mm512_extracti64x4_epi64(tmp, 0));
        auto hi = _mm512_cvtepi32_pd(_mm512_extracti64x4_epi64(tmp, 1));

        _mm512_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), lo);
        _mm512_store_pd(reinterpret_cast<double*>(ret.data() + 0x8), hi);

        #elif defined(AVEL_AVX)
        auto half0 = _mm256_cvtepu8_epi32(decay(x));
        auto half1 = _mm256_cvtepu8_epi32(_mm_unpackhi_epi64(decay(x), decay(x)));

        auto quarter0 = _mm256_castsi256_si128(half0);
        auto quarter1 = _mm256_extractf128_si256(half0, 0x1);
        auto quarter2 = _mm256_castsi256_si128(half1);
        auto quarter3 = _mm256_extractf128_si256(half1, 0x1);

        auto out0 = _mm256_cvtepi32_pd(quarter0);
        auto out1 = _mm256_cvtepi32_pd(quarter1);
        auto out2 = _mm256_cvtepi32_pd(quarter2);
        auto out3 = _mm256_cvtepi32_pd(quarter3);

        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), out0);
        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), out1);
        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0x8), out2);
        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0xC), out3);

        #elif defined(AVEL_SSE41)
        auto quarter0 = _mm_cvtepu8_epi32(decay(x));
        auto quarter1 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x4));
        auto quarter2 = _mm_cvtepu8_epi32(_mm_unpackhi_epi32(decay(x), decay(x)));
        auto quarter3 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0xC));

        auto out0 = _mm_cvtepi32_pd(quarter0);
        auto out1 = _mm_cvtepi32_pd(_mm_srli_si128(quarter0, 0x8));
        auto out2 = _mm_cvtepi32_pd(quarter1);
        auto out3 = _mm_cvtepi32_pd(_mm_srli_si128(quarter1, 0x8));
        auto out4 = _mm_cvtepi32_pd(quarter2);
        auto out5 = _mm_cvtepi32_pd(_mm_srli_si128(quarter2, 0x8));
        auto out6 = _mm_cvtepi32_pd(quarter3);
        auto out7 = _mm_cvtepi32_pd(_mm_srli_si128(quarter3, 0x8));

        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), out0);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x2), out1);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), out2);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x6), out3);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x8), out4);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xA), out5);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xC), out6);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xE), out7);

        #elif defined(AVEL_SSE2)
        auto half0 = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        auto quarter0 = _mm_unpacklo_epi16(half0, _mm_setzero_si128());
        auto quarter1 = _mm_unpackhi_epi16(half0, _mm_setzero_si128());
        auto quarter2 = _mm_unpacklo_epi16(half1, _mm_setzero_si128());
        auto quarter3 = _mm_unpackhi_epi16(half1, _mm_setzero_si128());

        auto eighth0 = quarter0;
        auto eighth1 = _mm_srli_si128(quarter0, 0x8);
        auto eighth2 = quarter1;
        auto eighth3 = _mm_srli_si128(quarter1, 0x8);
        auto eighth4 = quarter2;
        auto eighth5 = _mm_srli_si128(quarter2, 0x8);
        auto eighth6 = quarter3;
        auto eighth7 = _mm_srli_si128(quarter3, 0x8);

        auto out0 = _mm_cvtepi32_pd(eighth0);
        auto out1 = _mm_cvtepi32_pd(eighth1);
        auto out2 = _mm_cvtepi32_pd(eighth2);
        auto out3 = _mm_cvtepi32_pd(eighth3);
        auto out4 = _mm_cvtepi32_pd(eighth4);
        auto out5 = _mm_cvtepi32_pd(eighth5);
        auto out6 = _mm_cvtepi32_pd(eighth6);
        auto out7 = _mm_cvtepi32_pd(eighth7);

        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), out0);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x2), out1);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), out2);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x6), out3);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x8), out4);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xA), out5);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xC), out6);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xE), out7);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec1x8u>(vec1x8u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_cvtsi32_si128(decay(x) & 0xFF)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec1x8i>(vec1x8i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_cvtsi32_si128(decay(x) & 0xFF)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec1x16u>(vec1x16u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_cvtsi32_si128(decay(x) & 0xFF)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec1x16i>(vec1x16i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_cvtsi32_si128(decay(x) & 0xFF)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec1x32u>(vec1x32u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_cvtsi32_si128(decay(x) & 0xFF)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec1x32i>(vec1x32i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_cvtsi32_si128(decay(x) & 0xFF)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec1x64u>(vec1x64u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_cvtsi64_si128(decay(x) & 0xFF)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec1x64i>(vec1x64i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_cvtsi64_si128(decay(x) & 0xFF)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec1x32f>(vec1x32f x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_set1_epi8(std::uint8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec1x64f>(vec1x64f x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_set1_epi8(std::uint8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

}

#endif //AVEL_VEC16X8U_TESTS_HPP
