#ifndef AVEL_VEC2X64I_HPP
#define AVEL_VEC2X64I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec2x64i = Vector<std::int64_t, 2>;

    using arr2x64i = std::array<std::int64_t, 2>;

    using mask2x64i = Vector_mask<std::int64_t, 2>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec2x64i> div(vec2x64i numerator, vec2x64i denominator);
    vec2x64i broadcast_mask(mask2x64i m);
    vec2x64i blend(mask2x64i m, vec2x64i a, vec2x64i b);





    template<>
    class Vector_mask<std::int64_t, 2> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 2;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint64x2_t;
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
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            content(convert<Vector_mask>(v)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL)
            content(b ? 0x3 : 0x0) {}
        #elif defined(AVEL_SSE2)
            content(b ? _mm_set1_epi64x(-1) : _mm_setzero_si128()) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u64(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr2xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::int16_t>(arr));
            auto zeros = _mm_setzero_si128();
            content = _mm_cmplt_epu8_mask(zeros, array_data);

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::int16_t>(arr));
            array_data = _mm_cvtepu8_epi64(array_data);
            auto zeros = _mm_setzero_si128();
            content = _mm_cmplt_epu64_mask(zeros, array_data);

            #elif defined(AVEL_SSE)
            auto lo = _mm_cvtsi64_si128(arr[0]);
            auto hi = _mm_cvtsi64_si128(arr[1]);

            auto zeros = _mm_setzero_si128();
            content = _mm_sub_epi64(zeros, _mm_unpacklo_epi64(lo, hi));

            #endif

            #if defined(AVEL_NEON)
            auto lo = vcreate_u64(arr[0]);
            auto hi = vcreate_u64(arr[1]);
            auto whole = vcombine_u64(lo, hi);
            whole = vqshlq_n_u64(vqshlq_n_u64(whole, 32), 32);

            content = whole;

            #endif
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator=(bool x) {
            #if defined(AVEL_AVX512VL)
            content = x ? 0x3 : 0x0;

            #elif defined(AVEL_SSE2)
            content = x ? _mm_set1_epi64x(-1) : _mm_setzero_si128();

            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_u64(x ? -1ul : 0ul);
            #endif
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return decay(lhs) == decay(rhs);
            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));
            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u8(vreinterpretq_u8_u64(vceqq_u64(decay(lhs), decay(rhs))));
            return min == 0xFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return decay(lhs) != decay(rhs);
            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u32(vreinterpretq_u32_u64(vceqq_u64(decay(lhs), decay(rhs))));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(decay(lhs), decay(rhs));
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
            content = _mm_and_si128(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content |= rhs.content;
            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content ^= rhs.content;
            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u64(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{static_cast<primitive>(content ^ 0x3)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            auto ones = _mm_cmpeq_epi32(undef, undef);
            return Vector_mask{_mm_andnot_si128(content, ones)};

            #endif

            #if defined(AVEL_NEON)
            auto tmp = vreinterpretq_u32_u64(content);
            tmp = vmvnq_u32(tmp);
            return Vector_mask{vreinterpretq_u64_u32(tmp)};

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
    AVEL_FINL std::uint32_t count(mask2x64i m) {
        #if defined(AVEL_AVX512VL)
        return popcount(decay(m));

        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m))) / 8;

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vnegq_s64(vreinterpretq_s64_u64(decay(m)));
        return static_cast<std::uint32_t>(vaddvq_s64(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_s64(vnegq_s64(vreinterpretq_s64_u64(decay(m))));
        auto t1 = vgetq_lane_u32(t0, 0x00) + vgetq_lane_u32(t0, 0x2);

        return static_cast<std::uint32_t>(t1);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask2x64i m) {
        #if defined(AVEL_AVX512VL)
        return decay(m);

        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vmaxvq_u32(vreinterpretq_u32_u64(decay(m))) != 0x00;

        #endif
        #if defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u64(decay(m));
        auto t1 = vpmax_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmax_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) != 0x00;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask2x64i m) {
        #if defined(AVEL_AVX512VL)
        return decay(m) == 0x3;

        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(decay(m));

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vminvq_u8(vreinterpretq_u8_u64(decay(m))) == 0xFF;

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u64(decay(m));
        auto t1 = vpmin_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmin_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) == 0xFFFFFFFF;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask2x64i m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask2x64i>(mask2x64i m) {
        return std::array<mask2x64i, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 2> convert<mask1x8u, mask2x64i>(mask2x64i m) {
        alignas(2) std::array<mask1x8u, 2> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        ret = bit_cast<std::array<mask1x8u, 2>>(static_cast<std::uint16_t>(_mm_extract_epi16(t0, 0x0)));

        #elif defined(AVEL_AVX512VL)
        ret[0] = mask1x8u{static_cast<bool>(decay(m) & 0x1)};
        ret[1] = mask1x8u{static_cast<bool>(decay(m) & 0x5)};

        #elif defined(AVEL_SSE2)
        ret[0] = mask1x8u{static_cast<bool>(_mm_extract_epi16(decay(m), 0x0))};
        ret[1] = mask1x8u{static_cast<bool>(_mm_extract_epi16(decay(m), 0x5))};
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 2> convert<mask1x8i, mask2x64i>(mask2x64i m) {
        alignas(2) std::array<mask1x8i, 2> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        ret = bit_cast<std::array<mask1x8i, 2>>(static_cast<std::uint16_t>(_mm_extract_epi16(t0, 0x0)));

        #elif defined(AVEL_AVX512VL)
        ret[0] = mask1x8i{static_cast<bool>(decay(m) & 0x1)};
        ret[1] = mask1x8i{static_cast<bool>(decay(m) & 0x5)};

        #elif defined(AVEL_SSE2)
        ret[0] = mask1x8i{static_cast<bool>(_mm_extract_epi16(decay(m), 0x0))};
        ret[1] = mask1x8i{static_cast<bool>(_mm_extract_epi16(decay(m), 0x5))};
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 2> convert<mask1x16u, mask2x64i>(mask2x64i m) {
        alignas(2) std::array<mask1x16u, 2> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        ret = bit_cast<std::array<mask1x16u, 2>>(static_cast<std::uint16_t>(_mm_extract_epi16(t0, 0x0)));

        #elif defined(AVEL_AVX512VL)
        ret[0] = mask1x16u{static_cast<bool>(decay(m) & 0x1)};
        ret[1] = mask1x16u{static_cast<bool>(decay(m) & 0x5)};

        #elif defined(AVEL_SSE2)
        ret[0] = mask1x16u{static_cast<bool>(_mm_extract_epi16(decay(m), 0x0))};
        ret[1] = mask1x16u{static_cast<bool>(_mm_extract_epi16(decay(m), 0x5))};
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 2> convert<mask1x16i, mask2x64i>(mask2x64i m) {
        alignas(2) std::array<mask1x16i, 2> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        ret = bit_cast<std::array<mask1x16i, 2>>(static_cast<std::uint16_t>(_mm_extract_epi16(t0, 0x0)));

        #elif defined(AVEL_AVX512VL)
        ret[0] = mask1x16i{static_cast<bool>(decay(m) & 0x1)};
        ret[1] = mask1x16i{static_cast<bool>(decay(m) & 0x5)};

        #elif defined(AVEL_SSE2)
        ret[0] = mask1x16i{static_cast<bool>(_mm_extract_epi16(decay(m), 0x0))};
        ret[1] = mask1x16i{static_cast<bool>(_mm_extract_epi16(decay(m), 0x5))};
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 2> convert<mask1x32u, mask2x64i>(mask2x64i m) {
        alignas(2) std::array<mask1x32u, 2> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        ret = bit_cast<std::array<mask1x32u, 2>>(static_cast<std::uint16_t>(_mm_extract_epi16(t0, 0x0)));

        #elif defined(AVEL_AVX512VL)
        ret[0] = mask1x32u{static_cast<bool>(decay(m) & 0x1)};
        ret[1] = mask1x32u{static_cast<bool>(decay(m) & 0x5)};

        #elif defined(AVEL_SSE2)
        ret[0] = mask1x32u{static_cast<bool>(_mm_extract_epi16(decay(m), 0x0))};
        ret[1] = mask1x32u{static_cast<bool>(_mm_extract_epi16(decay(m), 0x5))};
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 2> convert<mask1x32i, mask2x64i>(mask2x64i m) {
        alignas(2) std::array<mask1x32i, 2> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        ret = bit_cast<std::array<mask1x32i, 2>>(static_cast<std::uint16_t>(_mm_extract_epi16(t0, 0x0)));

        #elif defined(AVEL_AVX512VL)
        ret[0] = mask1x32i{static_cast<bool>(decay(m) & 0x1)};
        ret[1] = mask1x32i{static_cast<bool>(decay(m) & 0x5)};

        #elif defined(AVEL_SSE2)
        ret[0] = mask1x32i{static_cast<bool>(_mm_extract_epi16(decay(m), 0x0))};
        ret[1] = mask1x32i{static_cast<bool>(_mm_extract_epi16(decay(m), 0x5))};
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64u, 2> convert<mask1x64u, mask2x64i>(mask2x64i m) {
        alignas(2) std::array<mask1x64u, 2> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        ret = bit_cast<std::array<mask1x64u, 2>>(static_cast<std::uint16_t>(_mm_extract_epi16(t0, 0x0)));

        #elif defined(AVEL_AVX512VL)
        ret[0] = mask1x64u{static_cast<bool>(decay(m) & 0x1)};
        ret[1] = mask1x64u{static_cast<bool>(decay(m) & 0x5)};

        #elif defined(AVEL_SSE2)
        ret[0] = mask1x64u{static_cast<bool>(_mm_extract_epi16(decay(m), 0x0))};
        ret[1] = mask1x64u{static_cast<bool>(_mm_extract_epi16(decay(m), 0x5))};
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 2> convert<mask1x64i, mask2x64i>(mask2x64i m) {
        alignas(2) std::array<mask1x64i, 2> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        ret = bit_cast<std::array<mask1x64i, 2>>(static_cast<std::uint16_t>(_mm_extract_epi16(t0, 0x0)));

        #elif defined(AVEL_AVX512VL)
        ret[0] = mask1x64i{static_cast<bool>(decay(m) & 0x1)};
        ret[1] = mask1x64i{static_cast<bool>(decay(m) & 0x5)};

        #elif defined(AVEL_SSE2)
        ret[0] = mask1x64i{static_cast<bool>(_mm_extract_epi16(decay(m), 0x0))};
        ret[1] = mask1x64i{static_cast<bool>(_mm_extract_epi16(decay(m), 0x5))};
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 2> convert<mask1x32f, mask2x64i>(mask2x64i m) {
        alignas(2) std::array<mask1x32f, 2> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        ret = bit_cast<std::array<mask1x32f, 2>>(static_cast<std::uint16_t>(_mm_extract_epi16(t0, 0x0)));

        #elif defined(AVEL_AVX512VL)
        ret[0] = mask1x32f{static_cast<bool>(decay(m) & 0x1)};
        ret[1] = mask1x32f{static_cast<bool>(decay(m) & 0x5)};

        #elif defined(AVEL_SSE2)
        ret[0] = mask1x32f{static_cast<bool>(_mm_extract_epi16(decay(m), 0x0))};
        ret[1] = mask1x32f{static_cast<bool>(_mm_extract_epi16(decay(m), 0x5))};
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64f, 2> convert<mask1x64f, mask2x64i>(mask2x64i m) {
        alignas(2) std::array<mask1x64f, 2> ret;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm_sub_epi32(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        ret = bit_cast<std::array<mask1x64f, 2>>(static_cast<std::uint16_t>(_mm_extract_epi16(t0, 0x0)));

        #elif defined(AVEL_AVX512VL)
        ret[0] = mask1x64f{static_cast<bool>(decay(m) & 0x1)};
        ret[1] = mask1x64f{static_cast<bool>(decay(m) & 0x5)};

        #elif defined(AVEL_SSE2)
        ret[0] = mask1x64f{static_cast<bool>(_mm_extract_epi16(decay(m), 0x0))};
        ret[1] = mask1x64f{static_cast<bool>(_mm_extract_epi16(decay(m), 0x5))};
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask2x64i>(mask2x64i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto t0 = _mm_movm_epi64(decay(m));
        return {mask16x8u{_mm_cvtepi64_epi8(t0)}};

        #elif defined(AVEL_AVX512VL)
        return {mask16x8u{_mm_mask_blend_epi64(decay(m), _mm_set1_epi64x(0), _mm_set1_epi64x(-1))}};

        #elif defined(AVEL_SSE2)
        auto a = _mm_extract_epi16(decay(m), 0x0);
        auto b = _mm_extract_epi16(decay(m), 0x5);
        auto c = (b >> 8) | (a & 0xFF);

        auto ret = _mm_setzero_si128();
        ret = _mm_insert_epi16(ret, c, 0x0);

        return {mask16x8u{ret}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask2x64i>(mask2x64i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto t0 = _mm_movm_epi64(decay(m));
        return {mask16x8i{_mm_cvtepi64_epi8(t0)}};

        #elif defined(AVEL_AVX512VL)
        return {mask16x8i{_mm_mask_blend_epi64(decay(m), _mm_set1_epi64x(0), _mm_set1_epi64x(-1))}};

        #elif defined(AVEL_SSE2)
        auto a = _mm_extract_epi16(decay(m), 0x0);
        auto b = _mm_extract_epi16(decay(m), 0x5);
        auto c = (b >> 8) | (a & 0xFF);

        auto ret = _mm_setzero_si128();
        ret = _mm_insert_epi16(ret, c, 0x0);

        return {mask16x8i{ret}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask2x64i>(mask2x64i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto t0 = _mm_movm_epi64(decay(m));
        return {mask8x16u{_mm_cvtepi32_epi8(t0)}};

        #elif defined(AVEL_AVX512VL)
        return {mask8x16u{_mm_mask_blend_epi64(decay(m), _mm_set1_epi64x(0), _mm_set1_epi64x(-1))}};

        #elif defined(AVEL_SSE2)
        auto a = _mm_extract_epi16(decay(m), 0x0);
        auto b = _mm_extract_epi16(decay(m), 0x5);

        auto ret = _mm_setzero_si128();
        ret = _mm_insert_epi16(ret, a, 0x0);
        ret = _mm_insert_epi16(ret, b, 0x1);

        return {mask8x16u{ret}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask2x64i>(mask2x64i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(decay(m))}};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto t0 = _mm_movm_epi64(decay(m));
        return {mask8x16i{_mm_cvtepi32_epi8(t0)}};

        #elif defined(AVEL_AVX512VL)
        return {mask8x16i{_mm_mask_blend_epi64(decay(m), _mm_set1_epi64x(0), _mm_set1_epi64x(-1))}};

        #elif defined(AVEL_SSE2)
        auto a = _mm_extract_epi16(decay(m), 0x0);
        auto b = _mm_extract_epi16(decay(m), 0x5);

        auto ret = _mm_setzero_si128();
        ret = _mm_insert_epi16(ret, a, 0x0);
        ret = _mm_insert_epi16(ret, b, 0x1);

        return {mask8x16i{ret}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask2x64i>(mask2x64i m) {
        //TODO: Implement
        #if defined(AVEL_AVX512VL)


        #elif defined(AVEL_SSE2)


        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask2x64i>(mask2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64u, 1> convert<mask2x64u, mask2x64i>(mask2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask1x8u>(mask1x8u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask1x8i>(mask1x8i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask1x16u>(mask1x16u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask1x16i>(mask1x16i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask1x32u>(mask1x32u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask1x32i>(mask1x32i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask1x64u>(mask1x64u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask1x64i>(mask1x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask1x32f>(mask1x32f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask1x64f>(mask1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 8> convert<mask2x64i, mask16x8u>(mask16x8u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 8> convert<mask2x64i, mask16x8i>(mask16x8i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 4> convert<mask2x64i, mask8x16u>(mask8x16u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 4> convert<mask2x64i, mask8x16i>(mask8x16i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 2> convert<mask2x64i, mask4x32u>(mask4x32u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 2> convert<mask2x64i, mask4x32i>(mask4x32i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask2x64u>(mask2x64u m) {
        //TODO: Implement
    }

    




    template<>
    class alignas(16) Vector<std::int64_t, 2> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 2;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int64_t;

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int64x2_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Instance members
        //=================================================

    private:

        primitive content;

    public:

        //=================================================
        // -ctors
        //=================================================

        AVEL_FINL explicit Vector(primitive p):
            content(p) {}

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            content(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
            content(_mm_sub_epi64(_mm_setzero_si128(), _mm_movm_epi64(decay(m)))) {}
        #elif defined(AVEL_AVX512VL)
            content([] (mask m) {
                auto undef = _mm_undefined_si128();
                auto t0 =  _mm_maskz_ternarylogic_epi64(decay(m), undef, undef, undef, 0xFF);
                return _mm_sub_epi64(_mm_setzero_si128(), t0);
            } (m)) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi64(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vnegq_s64(vreinterpretq_s64_u64(decay(m)))) {}
        #endif

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi64x(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_s64(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr2x64i& array):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_s64(array.data())) {}
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
            content = _mm_set1_epi64x(x);
            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_s64(x);

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
            return mask{_mm_cmpeq_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE41)
            return mask{_mm_cmpeq_epi64(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            auto t0 = _mm_cmpeq_epi32(lhs.content, rhs.content);
            auto t1 = _mm_shuffle_epi32(t0, 0xB1);
            return mask{_mm_and_si128(t0, t1)};

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            return mask{vceqq_s64(decay(lhs), decay(rhs))};

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epi64_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE41)
            return !mask{_mm_cmpeq_epi64(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            auto t0 = _mm_cmpeq_epi32(lhs.content, rhs.content);
            auto t1 = _mm_shuffle_epi32(t0, 0xB1);
            return !mask{_mm_and_si128(t0, t1)};

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            return !(lhs == rhs);

            #elif defined(AVEL_NEON)
            auto diff = vreinterpretq_u64_s64(vsubq_s64(decay(lhs), decay(rhs)));
            auto saturated = vqshlq_n_u64(vqshlq_n_u64(diff, 32), 32);
            return mask{saturated};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmplt_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE42)
            return mask{_mm_cmpgt_epi64(decay(rhs), decay(lhs))};

            #elif defined(AVEL_SSE41)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_extract_epi64(decay(lhs), 0x1);

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            auto out_lo = lhs_lo < rhs_lo;
            auto out_hi = lhs_hi < rhs_hi;

            auto out = _mm_cvtsi64_si128(out_lo);
            out = _mm_insert_epi64(out, out_hi, 0x1);

            auto zeros = _mm_setzero_si128();
            return mask{_mm_sub_epi64(zeros, out)};

            #elif defined(AVEL_SSE2)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(lhs), 0x8));

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            auto out_lo = _mm_cvtsi64_si128(lhs_lo < rhs_lo);
            auto out_hi = _mm_cvtsi64_si128(lhs_hi < rhs_hi);

            auto zeros = _mm_setzero_si128();
            auto ret = _mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8));
            return mask{_mm_sub_epi64(zeros, ret)};

            #endif

            #if defined(AVEL_NEON)
            auto diff = vqsubq_s64(decay(lhs), decay(rhs));
            auto sign_bits = vshrq_n_s64(diff, 63);
            return mask{vreinterpretq_u64_s64(sign_bits)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE42)
            return !mask{_mm_cmpgt_epi64(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE41)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_extract_epi64(decay(lhs), 0x1);

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            auto out_lo = lhs_lo <= rhs_lo;
            auto out_hi = lhs_hi <= rhs_hi;

            auto out = _mm_cvtsi64_si128(out_lo);
            out = _mm_insert_epi64(out, out_hi, 0x1);

            auto zeros = _mm_setzero_si128();
            return mask{_mm_sub_epi64(zeros, out)};

            #elif defined(AVEL_SSE2)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(lhs), 0x8));

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            auto out_lo = _mm_cvtsi64_si128(lhs_lo <= rhs_lo);
            auto out_hi = _mm_cvtsi64_si128(lhs_hi <= rhs_hi);

            auto zeros = _mm_setzero_si128();
            auto ret = _mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8));
            return mask{_mm_sub_epi64(zeros, ret)};

            #endif

            #if defined(AVEL_NEON)
            return !(lhs > rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE42)
            return mask{_mm_cmpgt_epi64(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE41)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_extract_epi64(decay(lhs), 0x1);

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            auto out_lo = lhs_lo > rhs_lo;
            auto out_hi = lhs_hi > rhs_hi;

            auto out = _mm_cvtsi64_si128(out_lo);
            out = _mm_insert_epi64(out, out_hi, 0x1);

            auto zeros = _mm_setzero_si128();
            return mask{_mm_sub_epi64(zeros, out)};

            #elif defined(AVEL_SSE2)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(lhs), 0x8));

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            auto out_lo = _mm_cvtsi64_si128(lhs_lo > rhs_lo);
            auto out_hi = _mm_cvtsi64_si128(lhs_hi > rhs_hi);

            auto zeros = _mm_setzero_si128();
            auto ret = _mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8));
            return mask{_mm_sub_epi64(zeros, ret)};

            #endif

            #if defined(AVEL_NEON)
            auto diff = vqsubq_s64(decay(rhs), decay(lhs));
            auto sign_bits = vshrq_n_s64(diff, 63);
            return mask{vreinterpretq_u64_s64(sign_bits)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE42)
            return !mask{_mm_cmpgt_epi64(decay(rhs), decay(lhs))};

            #elif defined(AVEL_SSE41)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_extract_epi64(decay(lhs), 0x1);

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            auto out_lo = lhs_lo >= rhs_lo;
            auto out_hi = lhs_hi >= rhs_hi;

            auto out = _mm_cvtsi64_si128(out_lo);
            out = _mm_insert_epi64(out, out_hi, 0x1);

            auto zeros = _mm_setzero_si128();
            return mask{_mm_sub_epi64(zeros, out)};

            #elif defined(AVEL_SSE2)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(lhs), 0x8));

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            auto out_lo = _mm_cvtsi64_si128(lhs_lo >= rhs_lo);
            auto out_hi = _mm_cvtsi64_si128(lhs_hi >= rhs_hi);

            auto zeros = _mm_setzero_si128();
            auto ret = _mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8));
            return mask{_mm_sub_epi64(zeros, ret)};

            #endif

            #if defined(AVEL_NEON)
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
            return Vector{0x00} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi64(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_s64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi64(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_s64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
            content = _mm_mullo_epi64(content, decay(rhs));

            #elif defined(AVEL_SSE41)
            std::uint64_t lhs0 = _mm_extract_epi64(content, 0x0);
            std::uint64_t lhs1 = _mm_extract_epi64(content, 0x1);

            std::uint64_t rhs0 = _mm_extract_epi64(decay(rhs), 0x0);
            std::uint64_t rhs1 = _mm_extract_epi64(decay(rhs), 0x1);

            content = _mm_insert_epi64(content, lhs0 * rhs0, 0x0);
            content = _mm_insert_epi64(content, lhs1 * rhs1, 0x1);

            #elif defined(AVEL_SSE2)
            alignas(16) std::uint64_t lhs_array[2];
            alignas(16) std::uint64_t rhs_array[2];

            _mm_store_si128(reinterpret_cast<__m128i*>(lhs_array), content);
            _mm_store_si128(reinterpret_cast<__m128i*>(rhs_array), decay(rhs));

            lhs_array[0] *= rhs_array[0];
            lhs_array[1] *= rhs_array[1];

            content = _mm_load_si128(reinterpret_cast<const __m128i*>(lhs_array));

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto a_lo = vgetq_lane_s64(content, 0x0);
            auto a_hi = vgetq_lane_s64(content, 0x1);
            auto b_lo = vgetq_lane_s64(decay(rhs), 0x0);
            auto b_hi = vgetq_lane_s64(decay(rhs), 0x1);

            auto c_lo = a_lo * b_lo;
            auto c_hi = a_hi * b_hi;

            int64x2_t c;
            c = vsetq_lane_s64(c_lo, c, 0x0);
            c = vsetq_lane_s64(c_hi, c, 0x1);
            content = c;

            #elif defined(AVEL_NEON)
            auto l0 = vreinterpretq_u64_s64(content);
            auto l1 = vreinterpretq_u64_s64(decay(rhs));

            auto l2 = vzip_u32(vreinterpret_u32_u64(vget_low_u64(l0)), vreinterpret_u32_u64(vget_high_u64(l0))).val[0];
            auto l3 = vzip_u32(vreinterpret_u32_u64(vget_low_u64(l1)), vreinterpret_u32_u64(vget_high_u64(l1))).val[0];

            auto l4 = vmull_u32(l2, l3);

            auto l5 = vshlq_n_u64(l0, 32);
            auto l6 = vshlq_n_u64(l1, 32);

            auto l7 = vmulq_u32(vreinterpretq_u32_u64(l1), vreinterpretq_u32_u64(l5));

            auto l8 = vmlaq_u32(l7, vreinterpretq_u32_u64(l6), vreinterpretq_u32_u64(l0));

            auto l9 = vaddq_u64(vreinterpretq_u64_u32(l8), l4);

            content = vreinterpretq_s64_u64(l9);

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.quot.content;
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
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
            content = vandq_s64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_s64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_s64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi64(content, _mm_cvtsi64_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s64(content, vdupq_n_s64(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX512VL)
            content = _mm_sra_epi64(content, _mm_cvtsi64_si128(rhs));

            #elif defined(AVEL_SSE2)
            std::int64_t lo = _mm_extract_epi64(content, 0x0);
            std::int64_t hi = _mm_extract_epi64(content, 0x1);

            rhs = min(rhs, 63ul);

            lo >>= rhs;
            hi >>= rhs;

            content = _mm_set_epi64x(hi, lo);
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s64(content, vdupq_n_s64(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi64(content, rhs.content);

            #elif defined(AVEL_SSE2)
            auto lhs = content;

            auto lo = _mm_sll_epi64(lhs, decay(rhs));
            auto hi = _mm_sll_epi64(_mm_bsrli_si128(lhs, 0x8), _mm_bsrli_si128(decay(rhs), 0x8));

            content = _mm_unpacklo_epi64(lo, hi);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s64(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL)
            content = _mm_srav_epi64(content, decay(rhs));

            #elif defined(AVEL_SSE2)
            std::int64_t lhs_lo = _mm_extract_epi64(content, 0x0);
            std::int64_t lhs_hi = _mm_extract_epi64(content, 0x1);

            std::uint64_t rhs_lo = _mm_extract_epi64(decay(rhs), 0x0);
            std::uint64_t rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            lhs_lo >>= min(rhs_lo, 63ul);
            lhs_hi >>= min(rhs_hi, 63ul);

            content = _mm_set_epi64x(lhs_hi, lhs_lo);
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s64(content, vnegq_s64(decay(rhs)));

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
            primitive t = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi32(t, t))};

            #endif

            #if defined(AVEL_NEON)
            auto tmp = vreinterpretq_s32_s64(content);
            tmp = vmvnq_s32(tmp);
            return Vector{vreinterpretq_s64_s32(tmp)};
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
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec2x64i operator-(vec2x64u v) {
        #if defined(AVEL_SSE2)
        return vec2x64i{_mm_sub_epi64(_mm_setzero_si128(), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64i{vreinterpretq_s64_u64(vsubq_u64(vdupq_n_u64(0x00), decay(v)))};
        #endif
    }

    //=====================================================
    // Arrangment operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int64_t extract(vec2x64i v) {
        static_assert(N <= vec2x64i::width, "Specified index does not exist");
        typename std::enable_if<N <= vec2x64i::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SS41)
        return _mm_extract_epi64(decay(v), N);

        #elif defined(AVEL_SSE2)
        return _mm_cvtsi128_si64(_mm_srli_si128(decay(v), 8 * N));

        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec2x64i x) {
        return count(vec2x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec2x64i x) {
        return any(vec2x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec2x64i x) {
        return all(vec2x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec2x64i x) {
        return none(vec2x64u{x});
    }
    */

    [[nodiscard]]
    AVEL_FINL vec2x64i broadcast_mask(mask2x64i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec2x64i{_mm_movm_epi64(decay(m))};

        #elif defined(AVEL_AVX512VL)
        auto undef = _mm_undefined_si128();
        return vec2x64i{_mm_maskz_ternarylogic_epi64(decay(m), undef, undef, undef, 0xFF)};

        #elif defined(AVEL_SSE2)
        return vec2x64i{decay(m)};

        #endif

        #if defined(AVEL_NEON)
        return vec2x64i{vreinterpretq_s64_u64(decay(m))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i blend(mask2x64i m, vec2x64i a, vec2x64i b) {
        #if defined(AVEL_AVX512VL)
        return vec2x64i{_mm_mask_blend_epi64(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_SSE41)
        return vec2x64i{_mm_blendv_epi8(decay(b), decay(a), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(decay(m), decay(b));
        auto y = _mm_and_si128(decay(m), decay(a));
        return vec2x64i{_mm_or_si128(x, y)};

        #endif

        #if defined(AVEL_NEON)
        auto x = vreinterpretq_u32_u64(decay(m));
        auto y = vreinterpretq_u32_s64(decay(a));
        auto z = vreinterpretq_u32_s64(decay(b));

        auto w = vbslq_u32(x, y, z);

        return vec2x64i{vreinterpretq_s64_u32(w)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i byteswap(vec2x64i x) {
        return vec2x64i{byteswap(vec2x64u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i max(vec2x64i a, vec2x64i b) {
        #if defined(AVEL_AVX512VL)
        return vec2x64i{_mm_max_epi64(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, b, a);

        #endif

        #if defined(AVEL_NEON)
        return blend(a < b, b, a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i min(vec2x64i a, vec2x64i b) {
        #if defined(AVEL_AVX512VL)
        return vec2x64i{_mm_min_epi64(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(b < a, b, a);

        #endif

        #if defined(AVEL_NEON)
        return blend(b < a, b, a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec2x64i, 2> minmax(vec2x64i a, vec2x64i b) {
        #if defined(AVEL_AVX512VL)
        return {
            vec2x64i{_mm_min_epi64(decay(a), decay(b))},
            vec2x64i{_mm_max_epi64(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto mask = a < b;
        return {blend(mask, a, b), blend(mask, b, a)};

        #endif

        #if defined(AVEL_NEON)
        auto mask = b < a;
        return {blend(mask, b, a), blend(mask, a, b)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i clamp(vec2x64i x, vec2x64i lo, vec2x64i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i average(vec2x64i a, vec2x64i b) {
        return ((a ^ b) >> 1) + (a & b);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i midpoint(vec2x64i a, vec2x64i b) {
        auto average = ((a ^ b) >> 1) + (a & b);
        auto bias = (broadcast_mask(b < a) & (a ^ b) & vec2x64i{0x1});
        return average + bias;
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i negate(mask2x64i m, vec2x64i x) {
        auto mask = broadcast_mask(m);
        return (x ^ mask) - mask;
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i abs(vec2x64i x) {
        #if defined(AVEL_AVX512VL)
        return vec2x64i{_mm_abs_epi64(decay(x))};

        #elif defined(AVEL_SSE2)
        auto y = x >> 63;
        return (x ^ y) - y;

        #endif

        #if defined(AVEL_NEON)
        auto y = vshrq_n_s64(decay(x), 63);
        auto z = vsubq_s64(veorq_s64(decay(x), y), y);
        return vec2x64i{z};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i neg_abs(vec2x64i x) {
        #if defined(AVEL_SSE2)
        auto y = ~(x >> 63);
        return (x ^ y) - y;
        #endif

        #if defined(AVEL_NEON)
        auto y = vshrq_n_s64(decay(x), 63);
        y = vreinterpretq_s64_s32(vmvnq_s32(vreinterpretq_s32_s64(y)));
        auto z = vsubq_s64(veorq_s64(decay(x), y), y);
        return vec2x64i{z};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i neg_abs(vec2x64u x) {
        #if defined(AVEL_SSE2)
        auto y = ~(x >> 63);
        return vec2x64i{(x ^ y) - y};
        #endif

        #if defined(AVEL_NEON)
        auto y = vshrq_n_u64(decay(x), 63);
        y = vreinterpretq_u64_u32(vmvnq_u32(vreinterpretq_u32_u64(y)));
        auto z = vsubq_u64(veorq_u64(decay(x), y), y);
        return vec2x64i{vreinterpretq_s64_u64(z)};

        #endif
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64i load<vec2x64i>(const std::int64_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec2x64i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64i{vld1q_s64(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64i aligned_load<vec2x64i>(const std::int64_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec2x64i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64i{vld1q_s64(ptr)};
        #endif
    }

    AVEL_FINL void store(std::int64_t* ptr, vec2x64i x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s64(ptr, decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(std::int64_t* ptr, vec2x64i x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s64(ptr, decay(x));
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec2x64i> div(vec2x64i x, vec2x64i y) {
        vec2x64i quotient{};

        mask2x64i sign_mask0 = (x < vec2x64i{0x0});
        mask2x64i sign_mask1 = (y < vec2x64i{0x0});

        mask2x64i sign_mask2 = sign_mask0 ^ sign_mask1;

        vec2x64u numerator{abs(x)};
        vec2x64u denominator{abs(y)};

        //TODO: Compute i more appropriately
        //TODO: Otherwise optimize

        std::int32_t i = 64;

        for (; (i-- > 0) && any(mask2x64u(numerator));) {
            mask2x64u b = ((numerator >> i) >= denominator);
            numerator -= (broadcast_mask(b) & (denominator << i));
            quotient |= vec2x64i{vec2x64u{b} << i};
        }

        //Adjust quotient's sign. Should be xor of operands' signs
        quotient = blend(sign_mask2, -quotient, quotient);

        //Adjust numerator's sign. Should be same sign as it was originally
        x = blend(sign_mask0, -vec2x64i{numerator}, vec2x64i{numerator});

        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i popcount(vec2x64i x) {
        return vec2x64i{popcount(vec2x64u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i countl_zero(vec2x64i v) {
        return vec2x64i{countl_zero(vec2x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i countl_one(vec2x64i v) {
        return vec2x64i{countl_one(vec2x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i countr_zero(vec2x64i v) {
        return vec2x64i{countr_zero(vec2x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i countr_one(vec2x64i v) {
        return vec2x64i{countr_one(vec2x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL mask2x64i has_single_bit(vec2x64i v) {
        return mask2x64i{has_single_bit(vec2x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i rotl(vec2x64i v, long long s) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i rotl(vec2x64i v, vec2x64i s) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i rotr(vec2x64i v, long long s) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i rotr(vec2x64i v, vec2x64i s) {
        //TODO: Implement
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL arr2x64i to_array(vec2x64i v) {
        alignas(16) arr2x64i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec2x64i>(vec2x64i m) {
        return std::array<vec2x64i, 1>{m};
    }

    template<>
    [[nodiscard]]
    std::array<vec1x8u, 2> convert<vec1x8u, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x8i, 2> convert<vec1x8i, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x16u, 2> convert<vec1x16u, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x16i, 2> convert<vec1x16i, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x32u, 2> convert<vec1x32u, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x32i, 2> convert<vec1x32i, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64u, 2> convert<vec1x64u, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64i, 2> convert<vec1x64i, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x32f, 2> convert<vec1x32f, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 2> convert<vec1x64f, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec16x8u, 1> convert<vec16x8u, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec16x8i, 1> convert<vec16x8i, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec8x16u, 1> convert<vec8x16u, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec8x16i, 1> convert<vec8x16i, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec4x32u, 1> convert<vec4x32u, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec4x32i, 1> convert<vec4x32i, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64u, 1> convert<vec2x64u, vec2x64i>(vec2x64i m) {
        #if defined(AVEL_SSE2)
        return {vec2x64u{decay(m)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec1x8u>(vec1x8u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec1x8i>(vec1x8i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec1x16u>(vec1x16u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec1x16i>(vec1x16i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec1x32u>(vec1x32u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec1x32i>(vec1x32i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec1x64u>(vec1x64u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec1x64i>(vec1x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec1x32f>(vec1x32f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec1x64f>(vec1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 8> convert<vec2x64i, vec16x8u>(vec16x8u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 8> convert<vec2x64i, vec16x8i>(vec16x8i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 4> convert<vec2x64i, vec8x16u>(vec8x16u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 4> convert<vec2x64i, vec8x16i>(vec8x16i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 2> convert<vec2x64i, vec4x32u>(vec4x32u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 2> convert<vec2x64i, vec4x32i>(vec4x32i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec2x64u>(vec2x64u m) {
        #if defined(AVEL_SSE2)
        return {vec2x64i{decay(m)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

}

#endif //AVEL_VEC2x64I_HPP
