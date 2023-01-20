#ifndef AVEL_VEC16X8U_HPP
#define AVEL_VEC16X8U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x8u = Vector<std::uint8_t, 16>;

    using arr16x8u = std::array<std::uint8_t, 16>;

    using mask16x8u = Vector_mask<std::uint8_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x8u> div(vec16x8u numerator, vec16x8u denominator);
    vec16x8u broadcast_mask(mask16x8u m);
    vec16x8u blend(mask16x8u m, vec16x8u a, vec16x8u b);
    vec16x8u countl_one(vec16x8u x);





    template<>
    class Vector_mask<std::uint8_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        static constexpr std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        using primitive = __mmask16;
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint8x16_t;
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
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content(b ? 0xFFFF : 0x0000) {}
        #elif defined(AVEL_SSE2)
            content(b ? _mm_set1_epi8(-1) : _mm_setzero_si128()) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u8(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr16xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = primitive(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_SSE2)
            primitive array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = _mm_cmplt_epi8(_mm_setzero_si128(), array_data);

            #endif

            #if defined(AVEL_NEON)
            auto array_data = vld1q_u8(bit_cast<const std::uint8_t*>(arr.data()));
            content = vcltq_u8(vdupq_n_u8(0x00), array_data);
            #endif
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator=(bool b) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = b ? 0xFFFF : 0x0000;

            #elif defined(AVEL_SSE2)
            content = b ? _mm_set1_epi8(-1) : _mm_setzero_si128();

            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_u8(b ? -1 : 0);
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
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return decay(lhs) == decay(rhs);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u8(vceqq_u8(decay(lhs), decay(rhs)));
            return min == 0xFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return decay(lhs) != decay(rhs);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u8(vceqq_u8(decay(lhs), decay(rhs)));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(vreinterpretq_u64_u8(decay(lhs)), vreinterpretq_u64_u8(decay(rhs)));
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content &= decay(rhs);

            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, decay(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = vandq_u8(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content |= decay(rhs);

            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, decay(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u8(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content ^= decay(rhs);

            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, decay(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = veorq_u8(content, decay(rhs));
            #endif

            return *this;
        }

        //=================================================
        // Bitwise operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector_mask{static_cast<primitive>(~content)};

            #elif defined(AVEL_AVX512VL)
            return Vector_mask{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            auto full = _mm_cmpeq_epi8(undef, undef);
            return Vector_mask{_mm_andnot_si128(content, full)};

            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vmvnq_u8(content)};
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
    AVEL_FINL std::uint32_t count(mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(_mm512_mask2int(decay(m)));

        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m)));

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vnegq_s8(vreinterpretq_s8_u8(decay(m)));
        return static_cast<std::uint32_t>(vaddvq_s8(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u8_s8(vnegq_s8(vreinterpretq_s8_u8(decay(m))));
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
        auto t0 = vmaxvq_u8(decay(m));
        auto t1 = t0 != 0x00;
        return t1;

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
        return _mm_test_all_ones(decay(m));

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
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
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
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
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
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
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
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
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
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
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
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
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
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
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
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
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
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        auto reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
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
    class alignas(16) Vector<std::uint8_t, 16> {
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
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint8x16_t;
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
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content(_mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi8(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vreinterpretq_u8_s8(vnegq_s8(vreinterpretq_s8_u8(decay(m))))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi8(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_u8(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr16x8u& array):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_u8(array.data())) {}
        #endif

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_SSE2)
            content = _mm_set1_epi8(x);
            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_u8(x);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpeq_epi8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi8(decay(lhs), decay(rhs))};

            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_u8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpneq_epi8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpeq_epi8(decay(lhs), decay(rhs))};

            #endif

            #if defined(AVEL_NEON)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmplt_epu8_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            Vector addition_mask{0x80};
            return mask{_mm_cmplt_epi8(decay(lhs ^ addition_mask), decay(rhs ^ addition_mask))};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_u8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmple_epu8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE2)
            auto mins = _mm_min_epu8(decay(lhs), decay(rhs));
            return mask{_mm_cmpeq_epi8(mins, decay(lhs))};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_u8(decay(lhs), decay(rhs))};
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

            #if defined(AVEL_NEON)
            return mask{vcgtq_u8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpge_epu8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE2)
            auto mins = _mm_min_epu8(decay(lhs), decay(rhs));
            return mask{_mm_cmpeq_epi8(mins, decay(rhs))};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_u8(decay(lhs), decay(rhs))};
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
            #if defined(AVEL_SSE2)
            content = _mm_add_epi8(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_u8(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi8(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_u8(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto a = _mm256_cvtepu8_epi16(content);
            auto b = _mm256_cvtepu8_epi16(rhs.content);

            auto c = _mm256_mullo_epi16(a, b);
            content = _mm256_cvtepi16_epi8(c);

            #elif defined(AVEL_AVX2)
            auto lhs_whole = _mm256_cvtepu8_epi16(content);
            auto rhs_whole = _mm256_cvtepu8_epi16(rhs.content);

            auto mask = _mm256_set1_epi16(0x00FF);
            auto product = _mm256_mullo_epi16(lhs_whole, rhs_whole);
            auto masked = _mm256_and_si256(product, mask);
            auto packed = _mm256_packus_epi16(masked, _mm256_set_m128i(_mm_setzero_si128(), (_mm256_extractf128_si256(masked, 0x1))));

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

            #endif

            #if defined(AVEL_NEON)
            content = vmulq_u8(content, decay(rhs));

            #endif
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

        AVEL_FINL Vector& operator&=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u8(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u8(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u8(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto tmp = _mm256_cvtepi8_epi16(content);
            tmp = _mm256_sll_epi16(tmp, _mm_cvtsi32_si128(rhs));
            content = _mm256_cvtepi16_epi8(tmp);

            #elif defined(AVEL_AVX2)
            auto whole = _mm256_cvtepu8_epi16(content);
            auto shifted = _mm256_sll_epi16(whole, _mm_cvtsi32_si128(rhs));
            auto masked = _mm256_and_si256(shifted, _mm256_set1_epi16(0x00FF));

            auto lo = _mm256_extracti128_si256(masked, 0x0);
            auto hi = _mm256_extracti128_si256(masked, 0x1);
            content = _mm_packus_epi16(lo, hi);

            #elif defined(AVEL_SSE2)
            auto s = _mm_cvtsi32_si128(rhs);

            auto shifted = _mm_sll_epi16(content, s);
            auto mask = _mm_set1_epi8(std::uint8_t(0xFF << rhs));
            content = _mm_and_si128(mask, shifted);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u8(content, vdupq_n_s8(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto whole = _mm256_cvtepu8_epi16(content);
            auto shifted = _mm256_srl_epi16(whole, _mm_cvtsi32_si128(rhs));
            content = _mm256_cvtepi16_epi8(shifted);

            #elif defined(AVEL_AVX2)
            auto whole = _mm256_cvtepu8_epi16(content);
            auto shifted = _mm256_srl_epi16(whole, _mm_cvtsi32_si128(rhs));

            auto lo = _mm256_extracti128_si256(shifted, 0x0);
            auto hi = _mm256_extracti128_si256(shifted, 0x1);
            content = _mm_packus_epi16(lo, hi);

            #elif defined(AVEL_SSE2)
            auto s = _mm_cvtsi32_si128(rhs);
            auto shifted = _mm_srl_epi16(content, s);
            auto mask = _mm_set1_epi8(std::uint8_t(0xFF >> rhs));
            content = _mm_and_si128(shifted, mask);

            #endif

            #if defined(AVEL_NEON)
            content =  vshlq_u8(content, vdupq_n_s8(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto whole = _mm256_cvtepu8_epi16(content);
            auto shifts = _mm256_cvtepu8_epi16(rhs.content);
            content = _mm256_cvtepi16_epi8(_mm256_sllv_epi16(whole, shifts));

            #elif defined(AVEL_AVX2)
            auto lhs_lo = _mm256_cvtepu8_epi32(content);
            auto lhs_hi = _mm256_cvtepu8_epi32(_mm_srli_si128(content, 0x8));

            auto rhs_lo = _mm256_cvtepu8_epi32(decay(rhs));
            auto rhs_hi = _mm256_cvtepu8_epi32(_mm_srli_si128(decay(rhs), 0x8));

            auto lo_shifted = _mm256_sllv_epi32(lhs_lo, rhs_lo);
            auto hi_shifted = _mm256_sllv_epi32(lhs_hi, rhs_hi);

            auto byte_mask = _mm256_set1_epi32(0x000000FF);
            auto lo_results = _mm256_and_si256(lo_shifted, byte_mask);
            auto hi_results = _mm256_and_si256(hi_shifted, byte_mask);

            auto results16 = _mm256_packus_epi32(lo_results, hi_results);

            auto lo = _mm256_extracti128_si256(results16, 0x0);
            auto hi = _mm256_extracti128_si256(results16, 0x1);
            auto results_out_of_order = _mm_packus_epi16(lo, hi);

            content = _mm_shuffle_epi32(results_out_of_order, 0xD8);

            #elif defined(AVEL_SSSE3)
            alignas(16) static constexpr std::uint8_t table_data[16] {
                0x01, 0x02, 0x04, 0x08,
                0x10, 0x20, 0x40, 0x80,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00
            };

            auto zeros = _mm_setzero_si128();
            auto table = _mm_load_si128(reinterpret_cast<const primitive*>(table_data));

            auto lhs_lo = _mm_unpacklo_epi8(content, zeros);
            auto lhs_hi = _mm_unpackhi_epi8(content, zeros);

            auto valid_mask = _mm_cmplt_epi8(_mm_set1_epi8(0x7), decay(rhs));
            auto indices = _mm_or_si128(decay(rhs), valid_mask);
            auto shifts = _mm_shuffle_epi8(table, indices);

            auto rhs_lo = _mm_unpacklo_epi8(shifts, zeros);
            auto rhs_hi = _mm_unpackhi_epi8(shifts, zeros);

            lhs_lo = _mm_mullo_epi16(lhs_lo, rhs_lo);
            lhs_hi = _mm_mullo_epi16(lhs_hi, rhs_hi);

            auto byte_mask = _mm_set1_epi16(0xFF);
            lhs_lo = _mm_and_si128(lhs_lo, byte_mask);
            lhs_hi = _mm_and_si128(lhs_hi, byte_mask);

            content = _mm_packus_epi16(lhs_lo, lhs_hi);

            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto a = _mm_cmpeq_epi8(_mm_and_si128(decay(rhs), _mm_set1_epi8(0x01)), zeros);
            content = _mm_add_epi8(content, _mm_andnot_si128(a, content));

            auto b = _mm_cmpeq_epi8(_mm_and_si128(decay(rhs), _mm_set1_epi8(0x02)), zeros);
            content = _mm_add_epi8(content, _mm_andnot_si128(b, content));
            content = _mm_add_epi8(content, _mm_andnot_si128(b, content));

            auto c = _mm_cmpeq_epi8(_mm_and_si128(decay(rhs), _mm_set1_epi8(0x04)), zeros);
            auto tmp = _mm_and_si128(_mm_slli_epi16(content, 4), _mm_set1_epi16(0xF0FF));
            content = _mm_or_si128(_mm_andnot_si128(c, tmp), _mm_and_si128(c, content));

            content = _mm_andnot_si128(_mm_cmpgt_epi8(decay(rhs), _mm_set1_epi8(0x7)), content);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u8(content, vreinterpretq_s8_u8(decay(rhs)));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto whole = _mm256_cvtepu8_epi16(content);
            auto shifts = _mm256_cvtepu8_epi16(rhs.content);
            content = _mm256_cvtepi16_epi8(_mm256_srlv_epi16(whole, shifts));

            //TODO: Offer AVX2 version?

            #elif defined(AVEL_SSSE3)
            alignas(16) static constexpr std::uint8_t table_data[16] {
                0x80, 0x40, 0x20, 0x10,
                0x08, 0x04, 0x02, 0x01,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00
            };

            auto s = _mm_min_epu8(decay(rhs), _mm_set1_epi8(0x8));

            auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
            auto shift_factors = _mm_shuffle_epi8(table, s);

            auto zeros = _mm_setzero_si128();
            auto lo = _mm_unpacklo_epi8(content, zeros);
            auto hi = _mm_unpackhi_epi8(content, zeros);

            auto shift_factors_lo = _mm_unpacklo_epi8(shift_factors, zeros);
            auto shift_factors_hi = _mm_unpackhi_epi8(shift_factors, zeros);

            lo = _mm_mullo_epi16(lo, shift_factors_lo);
            hi = _mm_mullo_epi16(hi, shift_factors_hi);

            lo = _mm_srli_epi16(lo, 0x7);
            hi = _mm_srli_epi16(hi, 0x7);

            content = _mm_packus_epi16(lo, hi);

            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto non_zero_mask = _mm_cmplt_epi8(decay(rhs), _mm_set1_epi8(8));
            content = _mm_and_si128(content, non_zero_mask);

            auto m0 = _mm_set1_epi16(0xFF0F);
            auto c0 = _mm_cmplt_epi8(_mm_slli_epi16(decay(rhs), 5), zeros);
            content = _mm_max_epu8(_mm_andnot_si128(c0, content), _mm_and_si128(_mm_srli_epi16(content, 4), m0));

            auto m1 = _mm_set1_epi16(0xFF3F);
            auto c1 = _mm_cmplt_epi8(_mm_slli_epi16(decay(rhs), 6), zeros);
            content = _mm_max_epu8(_mm_andnot_si128(c1, content), _mm_and_si128(_mm_srli_epi16(content, 2), m1));

            auto m2 = _mm_set1_epi16(0xFF7F);
            auto c2 = _mm_cmplt_epi8(_mm_slli_epi16(decay(rhs), 7), zeros);
            content = _mm_max_epu8(_mm_andnot_si128(c2, content), _mm_and_si128(_mm_srli_epi16(content, 1), m2));

            #endif

            #if defined(AVEL_NEON)
            content =  vshlq_u8(content, vnegq_s8(vreinterpretq_s8_u8(decay(rhs))));

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi32(undef, undef))};

            #endif

            #if defined(AVEL_NEON)
            return Vector{vmvnq_u8(content)};
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
        // Conversion operators
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
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint8_t extract(vec16x8u v) {
        static_assert(N <= vec16x8u::width, "Specified index does not exist");
        typename std::enable_if<N <= vec16x8u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SS41)
        return _mm_extract_epi8(decay(v), N);

        #elif defined(AVEL_SSE2)
        return _mm_cvtsi128_si32(_mm_srli_si128(decay(v), N)) & 0xFF;

        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_u8(decay(v), N);
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec16x8u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(static_cast<std::uint32_t>(_mm_testn_epi8_mask(decay(x), decay(x))));

        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return popcount(_mm_movemask_epi8(compared));

        #endif

        #if defined(AVEL_AARCH64)
        //TODO:Implement

        return 16 - vcltq_u8(vmovq_n_u8(0x00), decay(x));

        #elif defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec16x8u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return static_cast<bool>(_mm_test_epi8_mask(decay(x), decay(x)));

        #elif defined(AVEL_SSE41)
        return !_mm_testz_si128(decay(x), decay(x));

        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0x0000 != _mm_movemask_epi8(compared);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec16x8u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _mm_test_epi8_mask(decay(x), decay(x)) == 0xFF;

        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0xFFFF == _mm_movemask_epi8(compared);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec16x8u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _mm_testn_epi8_mask(decay(x), decay(x)) == 0x00;

        #elif defined(AVEL_SS41)
        return _mm_test_all_zeros(x, x);

        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0x0000 == _mm_movemask_epi8(compared);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }
    */

    [[nodiscard]]
    AVEL_FINL vec16x8u broadcast_mask(mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec16x8u{_mm_movm_epi8(decay(m))};

        #elif defined(AVEL_SSE2)
        return vec16x8u{decay(m)};

        #endif

        #if defined(AVEL_NEON)
        return vec16x8u{decay(m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u blend(mask16x8u m, vec16x8u a, vec16x8u b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec16x8u{_mm_mask_blend_epi8(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_SSE41)
        return vec16x8u{_mm_blendv_epi8(decay(b), decay(a), decay(m))};


        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(decay(m), decay(b));
        auto y = _mm_and_si128(decay(m), decay(a));
        return vec16x8u{_mm_or_si128(x, y)};

        #endif

        #if defined(AVEL_NEON)
        return vec16x8u{vbslq_u8(decay(m), decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u max(vec16x8u a, vec16x8u b) {
        #if defined(AVEL_SSE2)
        return vec16x8u{_mm_max_epu8(decay(a), decay(b))};
        #endif

        #if defined(AVEL_NEON)
        return vec16x8u{vmaxq_u8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u min(vec16x8u a, vec16x8u b) {
        #if defined(AVEL_SSE2)
        return vec16x8u{_mm_min_epu8(decay(a), decay(b))};
        #endif

        #if defined(AVEL_NEON)
        return vec16x8u{vminq_u8(decay(a), decay(b))};
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

        #if defined(AVEL_NEON)
        return {
            vec16x8u{vminq_u8(decay(a), decay(b))},
            vec16x8u{vmaxq_u8(decay(a), decay(b))}
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
        return vec16x8u{_mm_avg_epu8(decay(a), decay(b))} - ((a ^ b) & vec16x8u{0x1});
        #endif

        #if defined(AVEL_NEON)
        return vec16x8u{vhaddq_u8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u midpoint(vec16x8u a, vec16x8u b) {
        #if defined(AVEL_AVX512VL)
        auto t1 = _mm_avg_epu8(decay(a), decay(b));
        auto t5 = _mm_and_si128(_mm_ternarylogic_epi32(a, b, decay(broadcast_mask(b < a)), 0x14), _mm_set1_epi8(0x1));
        auto t6 = _mm_sub_epi8(t1, t5);
        return vec16x8u{t6};

        #elif defined(AVEL_SSE2)
        auto t1 = _mm_avg_epu8(decay(a), decay(b));
        auto t3 = _mm_andnot_si128(decay(broadcast_mask(b <= a)), _mm_xor_si128(decay(a), decay(b)));
        auto t5 = _mm_and_si128(t3, _mm_set1_epi8(0x1));
        auto t6 = _mm_sub_epi8(t1, t5);
        return vec16x8u{t6};

        #endif

        #if defined(AVEL_NEON)
        vec16x8u t0 = vec16x8u{vhaddq_u8(decay(a), decay(b))};
        vec16x8u t1 = (a ^ b) & vec16x8u{0x1} & broadcast_mask(b < a);
        return t0 + t1;

        #endif
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8u load<vec16x8u>(const std::uint8_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        n = min(16u, n);
        auto mask = 0xFFFF >> (16 - n);
        return vec16x8u{_mm_maskz_loadu_epi8(mask, ptr)};

        #elif defined(AVEL_SSE2)
        // GCC 9 doesn't have _mm_loadu_si16(), _mm_loadu_si32(), or
        // _mm_loadu_si64() but the following code is optimized down to
        // reasonable machine code.
        switch (n) {
            case 0: {
                return vec16x8u{_mm_setzero_si128()};
            }
            case 1: {
                return vec16x8u{_mm_cvtsi32_si128(ptr[0])};
            }
            case 2: {
                std::int32_t two = 0x00;
                two |= ptr[0] << 0x00;
                two |= ptr[1] << 0x08;
                return vec16x8u{_mm_cvtsi32_si128(two)};
            }
            case 3: {
                std::int32_t two_s = 0x00;
                two_s |= ptr[0] << 0x00;
                two_s |= ptr[1] << 0x08;

                std::int32_t one = 0x00;
                one |= ptr[2] << 0x00;

                auto lo = _mm_cvtsi32_si128(two_s);
                auto hi = _mm_cvtsi32_si128(one);

                return vec16x8u{_mm_unpacklo_epi16(lo, hi)};
            }

            case 4: {
                std::int32_t four_s = 0x00;
                four_s |= ptr[0] << 0x00;
                four_s |= ptr[1] << 0x08;
                four_s |= ptr[2] << 0x10;
                four_s |= ptr[3] << 0x18;
                return vec16x8u{_mm_cvtsi32_si128(four_s)};
            }
            case 5: {
                std::int32_t four_s = 0x00;
                four_s |= ptr[0] << 0x00;
                four_s |= ptr[1] << 0x08;
                four_s |= ptr[2] << 0x10;
                four_s |= ptr[3] << 0x18;

                std::int32_t one_s = 0x00;
                one_s |= ptr[4] << 0x00;

                auto four_v = _mm_cvtsi32_si128(four_s);
                auto one_v  = _mm_cvtsi32_si128(one_s);

                return vec16x8u{_mm_unpacklo_epi32(four_v, one_v)};
            }
            case 6: {
                std::int32_t four_s = 0x00;
                four_s |= ptr[0] << 0x00;
                four_s |= ptr[1] << 0x08;
                four_s |= ptr[2] << 0x10;
                four_s |= ptr[3] << 0x18;

                std::int32_t two_s = 0x00;
                two_s |= ptr[4] << 0x00;
                two_s |= ptr[5] << 0x08;

                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v  = _mm_cvtsi32_si128(two_s);

                return vec16x8u{_mm_unpacklo_epi32(four_v, two_v)};
            }
            case 7:{
                std::int32_t four_s = 0x00;
                four_s |= ptr[0] << 0x00;
                four_s |= ptr[1] << 0x08;
                four_s |= ptr[2] << 0x10;
                four_s |= ptr[3] << 0x18;

                std::int32_t two_s = 0x00;
                two_s |= ptr[4] << 0x00;
                two_s |= ptr[5] << 0x08;

                std::int32_t one_s = 0x00;
                one_s |= ptr[6] << 0x00;

                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v  = _mm_cvtsi32_si128(two_s);
                auto one_v  = _mm_cvtsi32_si128(one_s);

                return vec16x8u{_mm_unpacklo_epi32(four_v, _mm_unpacklo_epi16(two_v, one_v))};
            }

            case 8: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                return vec16x8u{_mm_cvtsi64_si128(eight_s)};
            }
            case 9: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t one_s = 0x00;
                one_s |= ptr[8] << 0x00;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8u{_mm_unpacklo_epi64(eight_v, one_v)};
            }
            case 10: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t two_s = 0x00;
                two_s |= ptr[8] << 0x00;
                two_s |= ptr[9] << 0x08;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto two_v = _mm_cvtsi32_si128(two_s);

                return vec16x8u{_mm_unpacklo_epi64(eight_v, two_v)};
            }
            case 11: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t two_s = 0x00;
                two_s |= ptr[8] << 0x00;
                two_s |= ptr[9] << 0x08;

                std::int32_t one_s = 0x00;
                one_s |= ptr[10] << 0x00;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto two_v = _mm_cvtsi32_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8u{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi16(two_v, one_v))};
            }

            case 12: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t four_s = 0x00;
                four_s |= ptr[8]  << 0x00;
                four_s |= ptr[9]  << 0x08;
                four_s |= ptr[10] << 0x10;
                four_s |= ptr[11] << 0x18;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);

                return vec16x8u{_mm_unpacklo_epi64(eight_v, four_v)};
            }
            case 13: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t four_s = 0x00;
                four_s |= ptr[8]  << 0x00;
                four_s |= ptr[9]  << 0x08;
                four_s |= ptr[10] << 0x10;
                four_s |= ptr[11] << 0x18;

                std::int32_t one_s = 0x00;
                one_s |= ptr[12] << 0x00;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8u{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi32(four_v, one_v))};
            }
            case 14: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t four_s = 0x00;
                four_s |= ptr[8]  << 0x00;
                four_s |= ptr[9]  << 0x08;
                four_s |= ptr[10] << 0x10;
                four_s |= ptr[11] << 0x18;

                std::int32_t two_s = 0x00;
                two_s |= ptr[12] << 0x00;
                two_s |= ptr[13] << 0x08;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v = _mm_cvtsi32_si128(two_s);

                return vec16x8u{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi32(four_v, two_v))};
            }
            case 15: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t four_s = 0x00;
                four_s |= ptr[8]  << 0x00;
                four_s |= ptr[9]  << 0x08;
                four_s |= ptr[10] << 0x10;
                four_s |= ptr[11] << 0x18;

                std::int32_t two_s = 0x00;
                two_s |= ptr[12] << 0x00;
                two_s |= ptr[13] << 0x08;

                std::int32_t one_s = 0x00;
                one_s |= ptr[14] << 0x00;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v = _mm_cvtsi32_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8u{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi32(four_v, _mm_unpacklo_epi16(two_v, one_v)))};
            }

            default: {
                return vec16x8u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
            }
        }
        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            case 0: {
                return vec16x8u{vdupq_n_u8(0x00)};
            }
            case 1: {
                return vec16x8u{};
            }
            case 2: {
                return vec16x8u{};
            }
            case 3: {
                return vec16x8u{};
            }
            case 4: {
                return vec16x8u{};
            }
            case 5: {
                return vec16x8u{};
            }
            case 6: {
                return vec16x8u{};
            }
            case 7: {
                return vec16x8u{};
            }
            case 8: {

                return vec16x8u{};
            }
            case 9: {
                return vec16x8u{};
            }
            case 10: {
                return vec16x8u{};
            }
            case 11: {
                return vec16x8u{};
            }
            case 12: {
                return vec16x8u{};
            }
            case 13: {
                return vec16x8u{};
            }
            case 14: {
                return vec16x8u{};
            }
            case 15: {
                return vec16x8u{};
            }
            default: {
                return vec16x8u{vld1q_u8(ptr)};
            }
        }
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8u load<vec16x8u, vec16x8u::width>(const std::uint8_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec16x8u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec16x8u{vld1q_u8(ptr)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8u aligned_load<vec16x8u>(const std::uint8_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        return load<vec16x8u>(ptr, n);

        #elif defined(AVEL_SSE2)
        return load<vec16x8u>(ptr, n);

        #endif

        #if defined(AVEL_NEON)
        return load<vec16x8u>(ptr, n);
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8u aligned_load<vec16x8u, vec16x8u::width>(const std::uint8_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec16x8u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        //TODO: Utilize __builtin_assume_aligned on GCC and Clang
        #if defined(AVEL_NEON)
        return vec16x8u{vld1q_u8(ptr)};
        #endif
    }


    template<std::uint32_t N = vec16x8u::width>
    AVEL_FINL void store(std::uint8_t* ptr, vec16x8u x) {
        static_assert(N <= vec16x8u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec16x8u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = (1 << N) - 1;
        _mm_mask_storeu_epi8(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto mask = _mm_srli_si128(full, vec16x8u::width - N);
        _mm_maskmoveu_si128(decay(x), mask, reinterpret_cast<char *>(ptr));
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    AVEL_FINL void store<vec16x8u::width>(std::uint8_t* ptr, vec16x8u x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u8(ptr, decay(x));
        #endif
    }

    AVEL_FINL void store(std::uint8_t* ptr, vec16x8u x, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = 0xFFFF >> std::min(vec16x8u::width, n);
        _mm_mask_storeu_epi8(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec16x8u::width;
        auto h = vec16x8u::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(w - std::min(w, n)));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(h - std::min(h, n)));
        auto mask = _mm_unpacklo_epi64(lo, hi);
        _mm_maskmoveu_si128(decay(x), mask, reinterpret_cast<char *>(ptr));

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }


    template<std::uint32_t N = vec16x8u::width>
    AVEL_FINL void aligned_store(std::uint8_t* ptr, vec16x8u x) {
        static_assert(N <= vec16x8u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec16x8u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u8(ptr, decay(x));
        #endif
    }

    template<>
    AVEL_FINL void aligned_store<vec16x8u::width>(std::uint8_t* ptr, vec16x8u x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u8(ptr, decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(std::uint8_t* ptr, vec16x8u x, std::uint32_t n) {
        #if defined(AVEL_SSE2)
        store(ptr, x, n);
        #endif

        #if defined(AVEL_NEON)
        store(ptr, x, n);
        #endif
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec16x8u> div(vec16x8u x, vec16x8u y) {
        vec16x8u quotient{};
        std::int32_t i = 8;

        //TODO: Select i more appropriately? at least on AARCH64 with Neon it
        // should still be feasible
        //TODO: Optimize body with AVX512 mask instructions
        for (; (i-- > 0) && any(mask16x8u(x));) {
            mask16x8u b = ((x >> i) >= y);
            x -= (broadcast_mask(b) & (y << i));
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

        auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto nibble_mask = _mm_set1_epi8(0x0F);

        auto index0 = _mm_and_si128(decay(x), nibble_mask);
        auto index1 = _mm_and_si128(_mm_srli_epi16(decay(x), 0x4), nibble_mask);

        auto partial_sum0 = _mm_shuffle_epi8(table, index0);
        auto partial_sum1 = _mm_shuffle_epi8(table, index1);

        auto sum = _mm_add_epi8(partial_sum0, partial_sum1);
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
        return vec16x8u{vcntq_u8(decay(x))};
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
        return vec16x8u{vclzq_u8(decay(x))};
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
        sum += broadcast_mask(b0) & vec16x8u{4};
        x <<= broadcast_mask(b0) & vec16x8u{4};

        vec16x8u m1{0xC0u};
        mask16x8u b1 = (m1 & x) == m1;
        sum += broadcast_mask(b1) & vec16x8u{2};
        x <<= broadcast_mask(b1) & vec16x8u{2};

        vec16x8u m2{0x80u};
        mask16x8u b2 = (m2 & x) == m2;
        sum += broadcast_mask(b2) & vec16x8u{1};

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

        ret = blend(zero_mask, vec16x8u{8}, ret);
        //TODO: Use 16-bit shifts explicitly to avoid 8-bit emulation overhead

        return ret;
        #endif

        #if defined(AVEL_AARCH64)
        auto reversed_bits = vrbitq_u8(decay(x));
        return vec16x8u{vclzq_u8(reversed_bits)};

        #elif defined(AVEL_NEON)
        auto zero_mask = (x == vec16x8u{0x00});
        x &= vec16x8u{0x00} - x;

        auto ret = (vec16x8u{7} - countl_zero(x));
        ret = blend(zero_mask, vec16x8u{8}, ret);

        //TODO: Optimize handling of zero_mask
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u countr_one(vec16x8u x) {
        #if defined(AVEL_SSSE3)
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
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto widened = _mm512_cvtepu8_epi32(decay(x));
        auto counts32 = _mm512_lzcnt_epi32(widened);
        auto counts8 = _mm512_cvtepi32_epi8(counts32);

        return vec16x8u{32} - vec16x8u{counts8};

        #elif defined(AVEL_AVX2)
        alignas(32) static constexpr std::uint8_t table_data[2][16] {
            {
                0, 1, 2, 2,
                3, 3, 3, 3,
                4, 4, 4, 4,
                4, 4, 4, 4
            },
            {
                0, 5, 6, 6,
                7, 7, 7, 7,
                8, 8, 8, 8,
                8, 8, 8, 8
            }
        };

        auto nibbles = _mm256_set_m128i(_mm_srli_epi16(decay(x), 0x4), decay(x));
        nibbles = _mm256_and_si256(nibbles, _mm256_set1_epi8(0x0F));

        auto table_vector = _mm256_load_si256(reinterpret_cast<const __m256i*>(table_data));
        auto partials = _mm256_shuffle_epi8(table_vector, nibbles);

        auto ret = _mm_max_epu8(
            _mm256_extracti128_si256(partials, 0x0),
            _mm256_extracti128_si256(partials, 0x1)
        );

        return vec16x8u{ret};

        #elif defined(AVEL_SSSE3)
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

        vec16x8u result{0x00};

        auto b0 = mask16x8u{x & vec16x8u{0xF0}};
        auto t0 = broadcast_mask(b0) & vec16x8u{4};
        result += t0;
        x >>= t0;

        auto b1 = mask16x8u{x & vec16x8u{0xFC}};
        auto t1 = broadcast_mask(b1) & vec16x8u{2};
        result += t1;
        x >>= t1;

        auto b2 = mask16x8u{x & vec16x8u{0xFE}};
        auto t2 = broadcast_mask(b2) & vec16x8u{1};
        result += t2;

        auto ret = _mm_andnot_si128(decay(zero_mask), decay(result + vec16x8u{1}));
        return vec16x8u{ret};

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
        auto zero = _mm_setzero_si128();
        auto lo = _mm_unpacklo_epi8(decay(x), zero);
        auto hi = _mm_unpackhi_epi8(decay(x), zero);

        lo = _mm_or_si128(lo, _mm_srli_epi16(lo, 1));
        hi = _mm_or_si128(hi, _mm_srli_epi16(hi, 1));

        lo = _mm_or_si128(lo, _mm_srli_epi16(lo, 2));
        hi = _mm_or_si128(hi, _mm_srli_epi16(hi, 2));

        lo = _mm_or_si128(lo, _mm_srli_epi16(lo, 4));
        hi = _mm_or_si128(hi, _mm_srli_epi16(hi, 4));

        lo = _mm_sub_epi16(lo, _mm_srli_epi16(lo, 1));
        hi = _mm_sub_epi16(hi, _mm_srli_epi16(hi, 1));

        return vec16x8u{_mm_packus_epi16(lo, hi)};

        #endif

        #if defined(AVEL_NEON)
        mask16x8u zero_mask = (x != vec16x8u{0x00});
        vec16x8u leading_zeros = countl_zero(x);

        return (vec16x8u{zero_mask} << (vec16x8u{7} - leading_zeros));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u bit_ceil(vec16x8u x) {
        #if defined(AVEL_SSSE3)

        alignas(16) static constexpr std::uint8_t table_data0[16] {
             0,  1,  3,  3,
             7,  7,  7,  7,
            15, 15, 15, 15,
            15, 15, 15, 15
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
              0,  31,  63,  63,
            127, 127, 127, 127,
            255, 255, 255, 255,
            255, 255, 255, 255
        };

        auto zero_mask = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());

        --x;

        auto nibble_mask = _mm_set1_epi8(0x0F);
        auto lo_nibble = _mm_and_si128(nibble_mask, decay(x));
        auto hi_nibble = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x4));

        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data0));
        auto table1 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data1));

        auto partial0 = _mm_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm_add_epi8(_mm_max_epu8(partial0, partial1), _mm_set1_epi8(0x01));
        return vec16x8u{_mm_sub_epi8(ret, zero_mask)};

        #elif defined(AVEL_SSE2)
        auto zeros = _mm_setzero_si128();

        auto undef = _mm_undefined_si128();
        auto ones = _mm_cmpeq_epi32(undef, undef);

        auto zero_mask = (x == vec16x8u{0x00});

        x = _mm_add_epi8(decay(x), ones);
        auto lo = _mm_unpacklo_epi8(decay(x), zeros);
        lo = _mm_or_si128(lo, _mm_srli_epi16(lo, 1));
        lo = _mm_or_si128(lo, _mm_srli_epi16(lo, 2));
        lo = _mm_or_si128(lo, _mm_srli_epi16(lo, 4));

        auto hi = _mm_unpackhi_epi8(decay(x), zeros);
        hi = _mm_or_si128(hi, _mm_srli_epi16(hi, 1));
        hi = _mm_or_si128(hi, _mm_srli_epi16(hi, 2));
        hi = _mm_or_si128(hi, _mm_srli_epi16(hi, 4));

        vec16x8u v{_mm_packus_epi16(lo, hi)};
        v = _mm_sub_epi8(decay(v), ones);

        return blend(zero_mask, vec16x8u{1}, v);

        #endif

        #if defined(AVEL_NEON)
        auto tmp = (vec16x8u{8} - countl_zero(x - vec16x8u{1}));
        return blend(x == vec16x8u{}, vec16x8u{1}, vec16x8u{1} << tmp);
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
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        s &= 0x7;
        auto r_shift_amount = _mm_cvtsi64_si128(8 - s);
        auto l_shift_amount = _mm_cvtsi64_si128(s);

        auto whole = _mm256_cvtepu8_epi16(decay(x));
        auto r_shifted = _mm256_srl_epi16(whole, r_shift_amount);
        auto l_shifted =_mm256_sll_epi16(whole, l_shift_amount);

        auto rotated = _mm256_or_si256(l_shifted, r_shifted);
        return vec16x8u{_mm256_cvtepi16_epi8(rotated)};

        #elif defined(AVEL_AVX2)
        s &= 0x7;
        auto r_shift_amount = _mm_cvtsi64_si128(8 - s);
        auto l_shift_amount = _mm_cvtsi64_si128(s);

        auto whole = _mm256_cvtepu8_epi16(decay(x));
        auto r_shifted = _mm256_srl_epi16(whole, r_shift_amount);
        auto l_shifted =_mm256_sll_epi16(whole, l_shift_amount);

        auto rotated = _mm256_or_si256(l_shifted, r_shifted);

        auto lo = _mm256_extracti128_si256(rotated, 0x0);
        auto hi = _mm256_extracti128_si256(rotated, 0x1);
        return vec16x8u{_mm_packus_epi16(lo, hi)};

        #elif defined(AVEL_SSE2)
        s &= 0x7;
        auto shift_amount = _mm_cvtsi64_si128(8 - s);
        auto lo = _mm_unpacklo_epi8(decay(x), decay(x));
        auto hi = _mm_unpackhi_epi8(decay(x), decay(x));

        lo = _mm_srl_epi16(lo, shift_amount);
        hi = _mm_srl_epi16(hi, shift_amount);

        auto byte_mask = _mm_set1_epi16(0x00FF);
        lo = _mm_and_si128(lo, byte_mask);
        hi = _mm_and_si128(hi, byte_mask);

        return vec16x8u{_mm_packus_epi16(lo, hi)};

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u rotl(vec16x8u x, vec16x8u s) {
        //TODO: Manually inline to eliminate widening and narrowing
        // Leverage unpacking doubly
        s &= vec16x8u{0x7};
        return (x << s) | (x >> (vec16x8u{8} - s));
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u rotr(vec16x8u x, long long s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        s &= 0x7;
        auto r_shift_amount = _mm_cvtsi64_si128(s);
        auto l_shift_amount = _mm_cvtsi64_si128(8 - s);

        auto whole = _mm256_cvtepu8_epi16(decay(x));
        auto r_shifted = _mm256_srl_epi16(whole, r_shift_amount);
        auto l_shifted =_mm256_sll_epi16(whole, l_shift_amount);

        auto rotated = _mm256_or_si256(l_shifted, r_shifted);
        return vec16x8u{_mm256_cvtepi16_epi8(rotated)};

        #elif defined(AVEL_AVX2)
        s &= 0x7;
        auto r_shift_amount = _mm_cvtsi64_si128(s);
        auto l_shift_amount = _mm_cvtsi64_si128(8 - s);

        auto whole = _mm256_cvtepu8_epi16(decay(x));
        auto r_shifted = _mm256_srl_epi16(whole, r_shift_amount);
        auto l_shifted =_mm256_sll_epi16(whole, l_shift_amount);

        auto rotated = _mm256_or_si256(l_shifted, r_shifted);

        auto lo = _mm256_extracti128_si256(rotated, 0x0);
        auto hi = _mm256_extracti128_si256(rotated, 0x1);
        return vec16x8u{_mm_packus_epi16(lo, hi)};

        #elif defined(AVEL_SSE2)
        s &= 0x7;
        auto shift_amount = _mm_cvtsi64_si128(s);
        auto lo = _mm_unpacklo_epi8(decay(x), decay(x));
        auto hi = _mm_unpackhi_epi8(decay(x), decay(x));

        lo = _mm_srl_epi16(lo, shift_amount);
        hi = _mm_srl_epi16(hi, shift_amount);

        auto byte_mask = _mm_set1_epi16(0x00FF);
        lo = _mm_and_si128(lo, byte_mask);
        hi = _mm_and_si128(hi, byte_mask);

        return vec16x8u{_mm_packus_epi16(lo, hi)};

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8u rotr(vec16x8u x, vec16x8u s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto widened_x = _mm256_cvtepi8_epi16(decay(x));
        widened_x = _mm256_or_si256(widened_x, _mm256_slli_epi16(widened_x, 0x8));

        auto widened_s = _mm256_cvtepi8_epi16(decay(s));
        auto rotated = _mm256_srlv_epi16(widened_x, widened_s);

        return vec16x8u{_mm256_cvtepi16_epi8(rotated)};

        //TODO: Offer AVX2 version?

        #elif defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0x01, 0x80, 0x40, 0x20,
            0x10, 0x08, 0x04, 0x02,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        };

        auto lo = _mm_unpacklo_epi8(decay(x), decay(x));
        auto hi = _mm_unpackhi_epi8(decay(x), decay(x));

        auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));

        lo = _mm_mullo_epi16(lo, table);
        hi = _mm_mullo_epi16(hi, table);

        lo = _mm_srli_epi16(lo, 0x8);
        hi = _mm_srli_epi16(hi, 0x8);

        return vec16x8u{_mm_unpacklo_epi8(lo, hi)};

        #elif defined(AVEL_SSE2)
        s &= vec16x8u{0x7};

        auto zeros = _mm_setzero_si128();

        auto shift_factors = _mm_set1_epi8(0x1);

        auto mask0 = _mm_cmplt_epi8(_mm_slli_epi16(decay(s), 5), zeros);
        shift_factors = _mm_max_epu8(shift_factors, _mm_andnot_si128(mask0, _mm_slli_epi16(shift_factors, 4)));

        auto mask1 = _mm_cmplt_epi8(_mm_slli_epi16(decay(s), 6), zeros);
        shift_factors = _mm_max_epu8(shift_factors, _mm_andnot_si128(mask1, _mm_slli_epi16(shift_factors, 2)));

        auto mask2 = _mm_cmplt_epi8(_mm_slli_epi16(decay(s), 7), zeros);
        shift_factors = _mm_max_epu8(shift_factors, _mm_andnot_si128(mask2, _mm_slli_epi16(shift_factors, 1)));

        auto lo_s = _mm_unpacklo_epi8(shift_factors, zeros);
        auto hi_s = _mm_unpackhi_epi8(shift_factors, zeros);

        auto lo_x = _mm_unpacklo_epi8(decay(x), decay(x));
        auto hi_x = _mm_unpackhi_epi8(decay(x), decay(x));

        lo_x = _mm_mullo_epi16(lo_x, lo_s);
        hi_x = _mm_mullo_epi16(hi_x, hi_s);

        lo_x = _mm_srli_epi16(lo_x, 7);
        hi_x = _mm_srli_epi16(hi_x, 7);

        auto ret = _mm_packus_epi16(lo_x, hi_x);
        return vec16x8u{ret};

        #endif

        #if defined(AVEL_NEON)
        //TODO: Optimize implementation
        s &= vec16x8u{0x7};
        return (x >> s) | (x << (vec16x8u{8} - s));
        #endif
    }

    //=====================================================
    // Conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL arr16x8u to_array(vec16x8u v) {
        alignas(16) arr16x8u ret{};
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
        ret = bit_cast<std::array<vec1x8u, 16>>(x);
        #endif

        #if defined(AVEL_NEON)
        ret = bit_cast<std::array<vec1x8u, 16>>(x);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 16> convert<vec1x8i, vec16x8u>(vec16x8u x) {
        alignas(16) std::array<vec1x8i, 16> ret;
        #if defined(AVEL_SSE2)
        ret = bit_cast<std::array<vec1x8i, 16>>(x);
        #endif

        #if defined(AVEL_NEON)
        ret = bit_cast<std::array<vec1x8i, 16>>(x);

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
        auto lo = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        auto hi = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0), lo);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 8), hi);

        #endif

        #if defined(AVEL_NEON)
        auto zeros = vdupq_n_u8(0x00);
        auto halves = vzipq_u8(decay(x), zeros);
        ret = bit_cast<std::array<vec1x16u, 16>>(halves);

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
        auto lo = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        auto hi = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0), lo);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 8), hi);
        #endif

        #if defined(AVEL_NEON)
        auto zeros = vdupq_n_u8(0x00);
        auto halves = vzipq_u8(decay(x), zeros);
        ret = bit_cast<std::array<vec1x16i, 16>>(halves);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 16> convert<vec1x32u, vec16x8u>(vec16x8u x) {
        alignas(64) std::array<vec1x32u, 16> ret;
        #if defined(AVEL_AVX512F)
        auto lo = _mm512_cvtepu8_epi32(decay(x));
        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data()), lo);

        #elif defined(AVEL_AVX2)
        auto lo = _mm256_cvtepu8_epi32(decay(x));
        auto hi = _mm256_cvtepu8_epi32(_mm_unpackhi_epi64(decay(x), decay(x)));

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0), lo);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 8), hi);

        #elif defined(AVEL_SSE41)
        auto quart0 = _mm_cvtepu8_epi32(decay(x));
        auto quart1 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x4));
        auto quart2 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x8));
        auto quart3 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0xC));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quart0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quart1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), quart2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), quart3);

        #elif defined(AVEL_SSE2)
        auto lo = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        auto hi = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        auto quart0 = _mm_unpacklo_epi16(lo, _mm_setzero_si128());
        auto quart1 = _mm_unpackhi_epi16(lo, _mm_setzero_si128());
        auto quart2 = _mm_unpacklo_epi16(hi, _mm_setzero_si128());
        auto quart3 = _mm_unpackhi_epi16(hi, _mm_setzero_si128());

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
        auto lo = _mm512_cvtepu8_epi32(decay(x));
        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data()), lo);

        #elif defined(AVEL_AVX2)
        auto lo = _mm256_cvtepu8_epi32(decay(x));
        auto hi = _mm256_cvtepu8_epi32(_mm_unpackhi_epi64(decay(x), decay(x)));

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0), lo);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 8), hi);

        #elif defined(AVEL_SSE41)
        auto quart0 = _mm_cvtepu8_epi32(decay(x));
        auto quart1 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x4));
        auto quart2 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0x8));
        auto quart3 = _mm_cvtepu8_epi32(_mm_srli_si128(decay(x), 0xC));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quart0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quart1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), quart2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), quart3);

        #elif defined(AVEL_SSE2)
        auto lo = _mm_unpacklo_epi8(decay(x), _mm_setzero_si128());
        auto hi = _mm_unpackhi_epi8(decay(x), _mm_setzero_si128());

        auto quart0 = _mm_unpacklo_epi16(lo, _mm_setzero_si128());
        auto quart1 = _mm_unpackhi_epi16(lo, _mm_setzero_si128());
        auto quart2 = _mm_unpacklo_epi16(hi, _mm_setzero_si128());
        auto quart3 = _mm_unpackhi_epi16(hi, _mm_setzero_si128());

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

        #elif defined(AVEL_AVX2)
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
