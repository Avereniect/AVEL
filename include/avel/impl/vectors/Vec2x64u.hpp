#ifndef AVEL_VEC2X64U_HPP
#define AVEL_VEC2X64U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec2x64u = Vector<std::uint64_t, 2>;

    using arr2x64u = std::array<std::uint64_t, 2>;

    using mask2x64u = Vector_mask<std::uint64_t, 2>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec2x64u> div(vec2x64u numerator, vec2x64u denominator);
    vec2x64u broadcast_mask(mask2x64u m);
    vec2x64u blend(mask2x64u m, vec2x64u a, vec2x64u b);
    vec2x64u countl_one(vec2x64u x);





    template<>
    class Vector_mask<std::uint64_t, 2> {
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

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL)
            content(primitive(b ? 0x3 : 0x0)) {}
        #elif defined(AVEL_SSE2)
            content(b ? _mm_set1_epi64x(-1ull) : _mm_setzero_si128()) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u64(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr2xb& arr) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint16_t>(arr));
            content = _mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data);

            #elif defined(AVEL_AVX512VL)
            content = (arr[1] << 1) | arr[0];

            #elif defined(AVEL_SSE2)
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            auto lo = _mm_cvtsi64_si128(arr[0] ? -1ull : 0);
            auto hi = _mm_cvtsi64_si128(arr[1] ? -1ull : 0);

            content = _mm_or_si128(lo, _mm_slli_si128(hi, 0x8));

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
            content = primitive(x ? 0x3 : 0x0);

            #elif defined(AVEL_SSE2)
            content = x ? _mm_set1_epi64x(-1ull) : _mm_setzero_si128();

            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_u64(x ? -1 : 0);
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
            return (_mm512_mask2int(decay(lhs)) == _mm512_mask2int(decay(rhs)));
            #elif defined(AVEL_SSE2)
            return (0xFFFF == _mm_movemask_epi8(_mm_cmpeq_epi32(decay(lhs), decay(rhs))));
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
            return (_mm512_mask2int(decay(lhs)) != _mm512_mask2int(decay(rhs)));
            #elif defined(AVEL_SSE2)
            return (0xFFFF != _mm_movemask_epi8(_mm_cmpeq_epi32(decay(lhs), decay(rhs))));
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
            content = content & decay(rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = content | decay(rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = content ^ decay(rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u64(content, rhs.content);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{primitive(content ^ 0x3)};

            #elif defined(AVEL_SSE2)
            primitive tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};
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
    AVEL_FINL std::uint32_t count(mask2x64u m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(decay(m)));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m))) / sizeof(std::uint64_t);
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vnegq_s64(vreinterpretq_s64_u64(decay(m)));
        return static_cast<std::uint32_t>(vaddvq_s64(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u64(decay(m))));
        auto t1 = vgetq_lane_u32(t0, 0x00) + vgetq_lane_u32(t0, 0x2);

        return static_cast<std::uint32_t>(t1);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask2x64u m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vmaxvq_u32(vreinterpretq_u32_u64(decay(m))) != 0x00;

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u64(decay(m));
        auto t1 = vpmax_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmax_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) != 0x00;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask2x64u m) {
        #if defined(AVEL_AVX512VL)
        return 0x03 == _mm512_mask2int(decay(m));
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
    AVEL_FINL bool none(mask2x64u m) {
        return !any(m);
    }





    template<>
    class Vector<std::uint64_t, 2> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 2;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint64_t;

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint64x2_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            Vector(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm_mask_blend_epi64(decay(m), _mm_setzero_si128(), _mm_set1_epi64x(1))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi64(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vreinterpretq_u64_s64(vnegq_s64(vreinterpretq_s64_u64(decay(m))))) {}
        #endif

        AVEL_FINL Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi64x(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_u64(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr2x64u& array):
        #if defined(AVEL_SSE2)
            Vector(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_u64(array.data())) {}
        #endif

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        AVEL_FINL Vector& operator=(primitive p) {
            content = p;
            return *this;
        }

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_SSE2)
            content = _mm_set1_epi64x(x);
            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_u64(x);
            #endif
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE41)
            return mask{_mm_cmpeq_epi64(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE2)
            auto t0 = _mm_cmpeq_epi32(decay(lhs), decay(rhs));
            auto t1 = _mm_shuffle_epi32(t0, 0xB1);
            auto t2 = _mm_and_si128(t0, t1);
            return mask{t2};

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            return mask{vceqq_u64(decay(lhs), decay(rhs))};

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epu64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE2)
            return !(lhs == rhs);

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            return !(lhs == rhs);

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(decay(lhs), decay(rhs));
            auto saturated = vqshlq_n_u64(vqshlq_n_u64(diff, 32), 32);
            return mask{saturated};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmplt_epu64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE42)
            auto addition_mask = _mm_set1_epi64x(0x8000000000000000ull);
            auto lhs_tmp = _mm_xor_si128(decay(lhs), addition_mask);
            auto rhs_tmp = _mm_xor_si128(decay(rhs), addition_mask);

            return mask{_mm_cmpgt_epi64(rhs_tmp, lhs_tmp)};

            #elif defined(AVEL_SSE41)
            std::uint64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::uint64_t lhs_hi = _mm_extract_epi64(decay(lhs), 0x1);

            std::uint64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::uint64_t rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            auto out_lo = lhs_lo < rhs_lo;
            auto out_hi = lhs_hi < rhs_hi;

            auto out = _mm_cvtsi64_si128(out_lo);
            out = _mm_insert_epi64(out, out_hi, 0x1);

            auto zeros = _mm_setzero_si128();
            return mask{_mm_sub_epi64(zeros, out)};

            #elif defined(AVEL_SSE2)
            std::uint64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::uint64_t lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(lhs), 0x8));

            std::uint64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::uint64_t rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            auto out_lo = _mm_cvtsi64_si128(lhs_lo < rhs_lo);
            auto out_hi = _mm_cvtsi64_si128(lhs_hi < rhs_hi);

            auto zeros = _mm_setzero_si128();
            auto ret = _mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8));
            return mask{_mm_sub_epi64(zeros, ret)};

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            return mask{vcleq_u64(lhs.content, rhs.content)};

            #elif defined(AVEL_NEON)
            auto diff = vqsubq_u64(decay(rhs), decay(lhs));
            auto saturated = vqshlq_n_u64(vqshlq_n_u64(diff, 32), 32);
            return mask{saturated};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epu64_mask(decay(lhs), decay(rhs))};
            #elif defined(AVEL_SSE2)
            return !mask{lhs > rhs};
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            return mask{vcleq_u64(lhs.content, rhs.content)};

            #elif defined(AVEL_NEON)
            return !(lhs > rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epu64_mask(decay(lhs), decay(rhs))};
            #elif defined(AVEL_SSE2)
            return rhs < lhs;
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            return mask{vcgtq_u64(lhs.content, rhs.content)};

            #elif defined(AVEL_NEON)
            auto diff = vqsubq_u64(decay(lhs), decay(rhs));
            auto saturated = vqshlq_n_u64(vqshlq_n_u64(diff, 32), 32);
            return mask{saturated};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epu64_mask(decay(lhs), decay(rhs))};
            #elif defined(AVEL_SSE2)
            return !mask{lhs < rhs};
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            return mask{vcgeq_u64(lhs.content, rhs.content)};

            #elif defined(AVEL_NEON)
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

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi64(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_u64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi64(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_u64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
            content = _mm_mullo_epi64(content, rhs.content);

            #elif defined(AVEL_SSE41)
            auto lhs_lo = _mm_cvtsi128_si64(content);
            auto lhs_hi = _mm_extract_epi64(content, 0x1);

            auto rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            auto rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            auto out_lo = lhs_lo * rhs_lo;
            auto out_hi = lhs_hi * rhs_hi;

            auto out = _mm_cvtsi64_si128(out_lo);
            out = _mm_insert_epi64(out, out_hi, 0x1);

            content = out;

            #elif defined(AVEL_SSE2)
            auto lhs_lo = _mm_cvtsi128_si64(content);
            auto lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(content, 0x8));

            auto rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            auto rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            auto out_lo = _mm_cvtsi64_si128(lhs_lo * rhs_lo);
            auto out_hi = _mm_cvtsi64_si128(lhs_hi * rhs_hi);

            content = _mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8));
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto a_lo = vgetq_lane_u64(content, 0x0);
            auto a_hi = vgetq_lane_u64(content, 0x1);
            auto b_lo = vgetq_lane_u64(decay(rhs), 0x0);
            auto b_hi = vgetq_lane_u64(decay(rhs), 0x1);

            auto c_lo = a_lo * b_lo;
            auto c_hi = a_hi * b_hi;

            uint64x2_t c;
            c = vsetq_lane_u64(c_lo, c, 0x0);
            c = vsetq_lane_u64(c_hi, c, 0x1);
            content = c;

            #elif defined(AVEL_NEON)
            auto l0 = content;
            auto l1 = decay(rhs);

            auto l2 = vzip_u32(vreinterpret_u32_u64(vget_low_u64(l0)), vreinterpret_u32_u64(vget_high_u64(l0))).val[0];
            auto l3 = vzip_u32(vreinterpret_u32_u64(vget_low_u64(l1)), vreinterpret_u32_u64(vget_high_u64(l1))).val[0];

            auto l4 = vmull_u32(l2, l3);

            auto l5 = vshlq_n_u64(l0, 32);
            auto l6 = vshlq_n_u64(l1, 32);

            auto l7 = vmulq_u32(vreinterpretq_u32_u64(l1), vreinterpretq_u32_u64(l5));

            auto l8 = vmlaq_u32(l7, vreinterpretq_u32_u64(l6), vreinterpretq_u32_u64(l0));

            auto l9 = vaddq_u64(vreinterpretq_u64_u32(l8), l4);

            content = l9;

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
            content = vandq_u64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long s) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi64(content, _mm_cvtsi32_si128(s));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u64(content, vdupq_n_s64(s));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_srl_epi64(content, _mm_cvtsi32_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u64(content, vdupq_n_s64(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi64(content, decay(rhs));

            #elif defined(AVEL_SSE2)
            auto mask = _mm_set_epi64x(-1ull, 0ull);

            auto rhs_lo = decay(rhs);
            auto rhs_hi = _mm_srli_si128(decay(rhs), 0x8);

            auto result_lo = _mm_sll_epi64(content, decay(rhs));
            result_lo = _mm_andnot_si128(mask, result_lo);

            auto result_hi = _mm_and_si128(mask, content);
            result_hi = _mm_sll_epi64(result_hi, rhs_hi);

            content = _mm_or_si128(result_hi, result_lo);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u64(content, vreinterpretq_s64_u64(rhs.content));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_srlv_epi64(content, decay(rhs));

            #elif defined(AVEL_SSE2)
            auto mask = _mm_set_epi64x(-1ull, 0ull);

            auto rhs_lo = decay(rhs);
            auto rhs_hi = _mm_srli_si128(decay(rhs), 0x8);

            auto result_lo = _mm_srl_epi64(content, decay(rhs));
            result_lo = _mm_andnot_si128(mask, result_lo);

            auto result_hi = _mm_and_si128(mask, content);
            result_hi = _mm_srl_epi64(result_hi, rhs_hi);

            content = _mm_or_si128(result_hi, result_lo);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u64(content, vnegq_s64(vreinterpretq_s64_u64(decay(rhs))));

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
            auto tmp = vreinterpretq_u32_u64(content);
            tmp = vmvnq_u32(tmp);
            return Vector{vreinterpretq_u64_u32(tmp)};
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
            return *this != Vector{};
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    //=====================================================
    // Arrangment operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint64_t extract(vec2x64u v) {
        static_assert(N <= vec2x64u::width, "Specified index does not exist");
        typename std::enable_if<N <= vec2x64u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SS41)
        return _mm_extract_epi64(decay(v), N);

        #elif defined(AVEL_SSE2)
        return _mm_cvtsi128_si64(_mm_srli_si128(decay(v), 8 * N));

        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_u64(decay(v), N);
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec2x64u x) {
        #if defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi32(decay(x), _mm_setzero_si128());
        compared = _mm_or_si128(compared, _mm_shuffle_epi32(compared, 0xB1));
        return popcount(_mm_movemask_epi8(compared)) / 8;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec2x64u x) {
        #if defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0x00 != _mm_movemask_epi8(compared);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec2x64u x) {
        #if defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0x00 == _mm_movemask_epi8(compared);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec2x64u x) {
        #if defined(AVEL_SS41)
        return _mm_test_all_zeros(x, x);
        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0xFF == _mm_movemask_epi8(compared);
        #endif
    }
    */

    [[nodiscard]]
    AVEL_FINL vec2x64u broadcast_mask(mask2x64u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec2x64u{_mm_movm_epi64(decay(m))};

        #elif defined(AVEL_AVX512VL)
        const auto x = _mm_set1_epi64x(0);
        const auto y = _mm_set1_epi64x(-1);
        return _mm_mask_blend_epi64(decay(m), x, y);

        #elif defined(AVEL_SSE2)
        return vec2x64u{decay(m)};

        #endif

        #if defined(AVEL_NEON)
        return vec2x64u{decay(m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u blend(mask2x64u m, vec2x64u a, vec2x64u b) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_mask_blend_epi64(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_SSE41)
        return vec2x64u{_mm_blendv_epi8(decay(b), decay(a), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(decay(m), decay(b));
        auto y = _mm_and_si128(decay(m), decay(a));
        return vec2x64u{_mm_or_si128(x, y)};

        #endif

        #if defined(AVEL_NEON)
        auto x = vreinterpretq_u32_u64(decay(m));
        auto y = vreinterpretq_u32_u64(decay(a));
        auto z = vreinterpretq_u32_u64(decay(b));

        auto w = vbslq_u32(x, y, z);

        return vec2x64u{vreinterpretq_u64_u32(w)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u max(vec2x64u a, vec2x64u b) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_max_epu64(decay(a), decay(b))};
        #elif defined(AVEL_SSE2)
        return blend(a < b, b, a);
        #endif

        #if defined(AVEL_NEON)
        return blend(a < b, b, a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u min(vec2x64u a, vec2x64u b) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_min_epu64(decay(a), decay(b))};
        #elif defined(AVEL_SSE2)
        return blend(b < a, b, a);
        #endif

        #if defined(AVEL_NEON)
        return blend(b < a, b, a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec2x64u, 2> minmax(vec2x64u a, vec2x64u b) {
        #if defined(AVEL_AVX512VL)
        return {
            vec2x64u{_mm_min_epu64(decay(a), decay(b))},
            vec2x64u{_mm_max_epu64(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto mask = b < a;
        return {blend(mask, b, a), blend(mask, a, b)};

        #endif

        #if defined(AVEL_NEON)
        auto mask = b < a;
        return {blend(mask, b, a), blend(mask, a, b)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u clamp(vec2x64u x, vec2x64u lo, vec2x64u hi) {
        return vec2x64u{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u midpoint(vec2x64u a, vec2x64u b) {
        vec2x64u t0 = a & b & vec2x64u{0x1};
        vec2x64u t1 = (a | b) & vec2x64u{0x1} & broadcast_mask(a > b);
        vec2x64u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u average(vec2x64u a, vec2x64u b) {
        return (a & b) + (a ^ b) >> 1;
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64u load<vec2x64u>(const std::uint64_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (1 << n) - 1;
        return vec2x64u{_mm_maskz_loadu_epi64(mask, ptr)};

        //TODO: Consider AVX2 version

        #elif defined(AVEL_SSE2)
        switch (n) {
            case 0: {
                return _mm_setzero_si128();
            }
            case 1: {
                return _mm_cvtsi64_si128(*ptr);
            }
            default: {
                return _mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr));
            }
        }
        #endif

        #if defined(AVEL_NEON)
        return vec2x64u{vld1q_u64(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64u load<vec2x64u, vec2x64u::width>(const std::uint64_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec2x64u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64u{vld1q_u64(ptr)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64u aligned_load<vec2x64u>(const std::uint64_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (1 << n) - 1;
        return vec2x64u{_mm_maskz_load_epi64(mask, ptr)};

        #elif defined(AVEL_SSE2)
        return load<vec2x64u>(ptr, n);
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64u aligned_load<vec2x64u, vec2x64u::width>(const std::uint64_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec2x64u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64u{vld1q_u64(ptr)};
        #endif
    }


    //Definition of gather deferred until vec2x64i defined


    template<std::uint32_t N = vec2x64u::width>
    AVEL_FINL void store(std::uint64_t* ptr, vec2x64u x) {
        static_assert(N <= vec2x64u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec2x64u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL)
        auto mask = (1 << N) - 1;
        _mm_mask_storeu_epi64(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto mask = _mm_srli_si128(full, sizeof(vec2x64u::scalar) * (vec2x64u::width - N));
        _mm_maskmoveu_si128(decay(x), mask, reinterpret_cast<char *>(ptr));

        #endif
    }

    template<>
    AVEL_FINL void store<vec2x64u::width>(std::uint64_t* ptr, vec2x64u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u64(ptr, decay(v));
        #endif
    }

    AVEL_FINL void store(std::uint64_t* ptr, vec2x64u v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (1 << n) - 1;

        _mm_mask_storeu_epi64(ptr, mask, decay(v));

        #elif defined(AVEL_SSE2)
        switch (n) {
            case 0: {
                return;
            }
            case 1: {
                ptr[0] = _mm_cvtsi128_si64(decay(v));
            }
            default: {
                _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
            }
        }
        #endif
    }


    template<std::uint32_t N = vec2x64u::width>
    AVEL_FINL void aligned_store(std::uint64_t* ptr, vec2x64u v) {
        static_assert(N <= vec2x64u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec2x64u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u64(ptr, decay(v));
        #endif
    }

    template<>
    AVEL_FINL void aligned_store<vec2x64u::width>(std::uint64_t* ptr, vec2x64u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u64(ptr, decay(v));
        #endif
    }

    AVEL_FINL void aligned_store(std::uint64_t* ptr, vec2x64u v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (1 << n) - 1;
        _mm_mask_store_epi64(ptr, mask, decay(v));

        #elif defined(AVEL_SSE2)
        switch (n) {
            case 0: {
                return;
            }
            case 1: {
                ptr[0] = _mm_cvtsi128_si64(decay(v));
            }
            default: {
                _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
            }
        }
        #endif

        #if defined(AVEL_NEON)
        vst1q_u64(ptr, decay(v));
        #endif
    }


    //Definition of scatter deferred until vec2x64i defined

    //=====================================================
    // Integer vector operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec2x64u> div(vec2x64u numerator, vec2x64u denominator) {
        vec2x64u quotient{};

        /*
        #if defined(AVEL_AVX512VL)

        std::int32_t i = ;
        #elif defined(AVEL_SSE2)

        vec2x64u::primitive t0 = numerator;
        vec2x64u::primitive t1 = _mm_srli_si128(t0, 8);
        vec2x64u::primitive t2 = _mm_or_si128(t0, t1);

        vec2x64u::primitive s0 = denominator;
        vec2x64u::primitive s1 = _mm_srli_si128(s0, 8);
        vec2x64u::primitive s2 = _mm_and_si128(s0, s1);

        std::int64_t x = countl_zero(std::int64_t(_mm_cvtsi128_si64(t2)));
        std::int64_t y = countl_zero(std::int64_t(_mm_cvtsi128_si64(s2)));

        std::int32_t i = y - x + 1;
        #endif
        */

        std::int32_t i = 64;

        vec2x64u zeroes{};
        for (; (i-- > 0) && any(numerator != zeroes);) {
            mask2x64u b = ((numerator >> i) >= denominator);
            auto tmp0 = broadcast_mask(b);
            numerator -= (tmp0 & (denominator << i));
            quotient |= (vec2x64u{b} << i);
        }

        return {quotient, numerator};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u popcount(vec2x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)
        return vec2x64u{_mm_popcnt_epi64(decay(v))};
        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        //TODO: Widen implementation
        auto tmp0 = _mm_popcnt_epi16(decay(v));
        auto tmp1 = _mm_slli_epi64(tmp0, 32);
        auto tmp2 = _mm_add_epi32(tmp0, tmp1);
        auto tmp3 = _mm_slli_epi32(tmp2, 16);
        auto tmp4 = _mm_add_epi16(tmp2, tmp3);

        return vec2x64u{_mm_srli_epi64(tmp4, 48)};
        #elif defined(AVEL_SSE41) && defined(AVEL_POPCNT)
        auto lo = _mm_cvtsi128_si64(decay(v));
        auto hi = _mm_extract_epi64(decay(v), 0x1);

        auto popcnt_lo = _mm_popcnt_u64(lo);
        auto popcnt_hi = _mm_popcnt_u64(hi);

        return vec2x64u{_mm_set_epi64x(popcnt_hi, popcnt_lo)};

        #elif defined(AVEL_SSE2) && defined(AVEL_POPCNT)
        auto lo = _mm_cvtsi128_si64(decay(v));
        auto hi = _mm_cvtsi128_si64(_mm_srli_si128(v, 0x8));

        auto popcnt_lo = _mm_popcnt_u64(lo);
        auto popcnt_hi = _mm_popcnt_u64(hi);

        return vec2x64u{_mm_set_epi64x(popcnt_hi, popcnt_lo)};

        #elif defined(AVEL_SSE2)
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec2x64u{0x5555555555555555});
        v = ((v >> 2) & vec2x64u{0x3333333333333333}) + (v & vec2x64u{0x3333333333333333});
        v = ((v >> 4) + v) & vec2x64u{0x0F0F0F0F0F0F0F0F};
        v = ((v >> 8) + v) & vec2x64u{0x00FF00FF00FF00FF};
        v = ((v >> 16) + v) & vec2x64u{0x0000FFFF0000FFFF};
        v = ((v >> 32) + v) & vec2x64u{0x00000000FFFFFFFF};

        return v;

        #endif

        #if defined(AVEL_NEON)
        auto t0 = vreinterpretq_u64_u8(vcntq_u8(vreinterpretq_u8_u64(decay(v))));
        auto t1 = vshlq_n_u64(t0, 32);
        auto t2 = vaddq_u64(t0, t1);
        auto t3 = vshlq_n_u64(t2, 16);
        auto t4 = vaddq_u64(t2, t3);
        auto t5 = vshlq_n_u64(t4, 8);
        auto t6 = vaddq_u64(t4, t5);
        auto t7 = vshrq_n_u64(t6, 56);
        return t7;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u byteswap(vec2x64u v) {
        #if defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t index_data[16] {
             7,  6,  5,  4,
             3,  2,  1,  0,
            15, 14, 13, 12,
            11, 10,  9,  8
        };

        auto indices = _mm_load_si128((const __m128i*)index_data);
        return vec2x64u{_mm_shuffle_epi8(decay(v), indices)};

        #elif defined(AVEL_SSE2)
        //TODO: Adjust implementation to work on 64-bit integers
        alignas(16) static constexpr std::uint32_t mask_data[4]{
            0x00000000,
            0xFFFFFFFF,
            0x00000000,
            0xFFFFFFFF
        };

        auto t0 = _mm_shufflelo_epi16(decay(v), 0x1B);
        auto t1 = _mm_shufflehi_epi16(t0, 0x1B);
        auto t2 = _mm_slli_epi16(t1, 0x8);
        auto t3 = _mm_srli_epi16(t1, 0x8);
        return vec2x64u{_mm_or_si128(t2, t3)};
        #endif

        #if defined(AVEL_NEON)
        auto t0 = vrev64q_u8(vreinterpretq_u8_u64(decay(v)));
        return vec2x64u{vreinterpretq_u64_u8(t0)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u countl_zero(vec2x64u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec2x64u{_mm_lzcnt_epi64(decay(x))};

        #elif defined(AVEL_SSE2)
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        auto y = _mm_andnot_si128(decay(x >> 1), decay(x));

        auto lo_double = _mm_cvtsi64_sd(_mm_undefined_pd(), _mm_cvtsi128_si64(y));
        auto hi_double = _mm_cvtsi64_sd(_mm_undefined_pd(), _mm_cvtsi128_si64(_mm_srli_si128(y, 0x8)));

        auto biases = _mm_set1_pd(0.5);
        auto doubles = _mm_shuffle_pd(lo_double, hi_double, 0x0);
        auto biased_doubles = _mm_add_pd(doubles, biases);

        auto biased_exponents = _mm_srli_epi64(_mm_castpd_si128(biased_doubles), 52);
        auto lzcnt = _mm_subs_epu16(_mm_set1_epi64x(1086), biased_exponents);
        return vec2x64u{lzcnt};

        #endif

        #if defined(AVEL_NEON)
        auto whole = vclzq_u32(vreinterpretq_u32_u64(decay(x)));
        auto lo = vandq_u32(whole, vreinterpretq_u32_u64(vdupq_n_u64(0xFFFF)));
        auto hi = vreinterpretq_u32_u64(vshrq_n_u64(vreinterpretq_u64_u32(whole), 32));

        auto half = vreinterpretq_u32_u64(vdupq_n_u64(32));

        auto mask_lo = vceqq_u32(hi, half);
        auto masked_lo = vandq_u32(vaddq_u32(lo, half), mask_lo);

        auto ret = vmaxq_u32(hi, masked_lo);
        return vec2x64u{vreinterpretq_u64_u32(ret)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u countl_one(vec2x64u x) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{countl_zero(~x)};

        #elif defined(AVEL_SSE2)
        return vec2x64u{countl_zero(~x)};

        /*
        // Adjust to be suitable for 64-bit integers
        vec2x64u sum{x == vec2x64u{0xFFFFFFFF}};

        vec2x64u m0{0xFFFF0000u};
        mask2x64u b0 = (m0 & x) == m0;
        sum += broadcast_mask(b0) & vec2x64u{16};
        x <<= broadcast_mask(b0) * vec2x64u{16};

        vec2x64u m1{0xFF000000u};
        mask2x64u b1 = (m1 & x) == m1;
        sum += broadcast_mask(b1) & vec2x64u{8};
        x <<= broadcast_mask(b1) & vec2x64u{8};

        vec2x64u m2{0xF0000000u};
        mask2x64u b2 = (m2 & x) == m2;
        sum += broadcast_mask(b2) & vec2x64u{4};
        x <<= broadcast_mask(b2) & vec2x64u{4};

        vec2x64u m3{0xC0000000u};
        mask2x64u b3 = (m3 & x) == m3;
        sum += broadcast_mask(b3) & vec2x64u{2};
        x <<= broadcast_mask(b3) & vec2x64u{2};

        vec2x64u m4{0x80000000u};
        mask2x64u b4 = (m4 & x) == m4;
        sum += broadcast_mask(b3) & vec2x64u{1};

        return sum;
        */

        #endif

        #if defined(AVEL_NEON)
        return countl_zero(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u countr_zero(vec2x64u x) {
        #if defined(AVEL_SSE2)
        auto y = decay(x & (vec2x64u{0x00} - x));

        auto lo_double = _mm_cvtsi64_sd(_mm_undefined_pd(), _mm_cvtsi128_si64(y));
        auto hi_double = _mm_cvtsi64_sd(_mm_undefined_pd(), _mm_cvtsi128_si64(_mm_srli_si128(y, 0x8)));
        auto doubles = _mm_shuffle_pd(lo_double, hi_double, 0x0);

        auto biased_exponents = (vec2x64u(_mm_castpd_si128(doubles)) >> 52);
        biased_exponents = _mm_min_epi16(decay(vec2x64u{1086}), decay(biased_exponents));
        auto tzcnt = biased_exponents - vec2x64u{1023};
        tzcnt = blend(x == vec2x64u{}, vec2x64u{64}, tzcnt);
        return vec2x64u{tzcnt};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Optimize
        auto zero_mask = (x == vec2x64u{0x00});
        x &= vec2x64u{0x00} - x;

        auto ret = (vec2x64u{63} - countl_zero(x));
        ret = blend(zero_mask, vec2x64u{64}, ret);
        return ret;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u countr_one(vec2x64u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u bit_width(vec2x64u x) {
        #if defined(AVEL_SSE2)
        return vec2x64u{64} - vec2x64u{countl_zero(x)};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64u{64} - vec2x64u{countl_zero(x)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u bit_floor(vec2x64u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        vec2x64u leading_zeros = countl_zero(x);
        mask2x64u zero_mask = (leading_zeros != vec2x64u{64});

        return (vec2x64u{zero_mask} << (vec2x64u{63} - leading_zeros));
        #elif defined(AVEL_SSE2)
        auto zero_mask = (x == vec2x64u{0});

        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        x = x | (x >> 32);

        //TODO: Optimize
        return blend(zero_mask, vec2x64u{0x0}, x - (x >> 1));

        #endif

        #if defined(AVEL_NEON)
        vec2x64u leading_zeros = countl_zero(x);
        mask2x64u zero_mask = (leading_zeros != vec2x64u{64});

        return (vec2x64u{zero_mask} << (vec2x64u{63} - leading_zeros));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u bit_ceil(vec2x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto zero_mask = (v == vec2x64u{0});
        auto x = vec2x64u{1} << (vec2x64u{64} - countl_zero(v));
        return blend(zero_mask, vec2x64u{1}, x);

        #elif defined(AVEL_SSE2)
        auto zero_mask = (v == vec2x64u{0});

        //TODO: Optimize body
        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;
        ++v;

        //TODO: Optimize?
        return blend(zero_mask, vec2x64u{1}, v);
        #endif

        #if defined(AVEL_NEON)
        auto zero_mask = (v == vec2x64u{0});
        auto x = vec2x64u{1} << (vec2x64u{64} - countl_zero(v));
        return blend(zero_mask, vec2x64u{1}, x);
        #endif
    };

    [[nodiscard]]
    AVEL_FINL mask2x64u has_single_bit(vec2x64u x) {
        #if defined(AVEL_AVX512VL)
        return popcount(decay(x)) == vec2x64u{1};

        #elif defined(AVEL_SSE2)
        return mask2x64u{x} & !mask2x64u{x & (x - vec2x64u{1})};

        #endif

        #if defined(AVEL_NEON)
        return mask2x64u{x} & !mask2x64u{x & (x - vec2x64u{1})};
        #endif
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    vec2x64u bit_shift_left(vec2x64u v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec2x64u{_mm_slli_epi64(decay(v), S)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vshlq_n_u64(decay(v), S)};
        #endif
    }

    template<>
    vec2x64u bit_shift_left<0>(vec2x64u v) {
        return v;
    }

    template<std::uint32_t S>
    [[nodiscard]]
    vec2x64u bit_shift_right(vec2x64u v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec2x64u{_mm_srli_epi64(decay(v), S)};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64u{vshrq_n_u64(decay(v), S)};
        #endif
    }

    template<>
    vec2x64u bit_shift_right<0>(vec2x64u v) {
        return v;
    }



    [[nodiscard]]
    AVEL_FINL vec2x64u rotl(vec2x64u v, vec2x64u::scalar s) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_rolv_epi64(decay(v), decay(vec2x64u{s}))};

        #elif defined(AVEL_SSE2)
        return (v << s) | (v >> (64 - s));

        #endif

        #if defined(AVEL_NEON)
        s &= 0x3F;
        return (v << s) | (v >> (64 - s));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u rotl(vec2x64u v, vec2x64u s) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_rolv_epi64(decay(v), decay(s))};
        #elif defined(AVEL_SSE2)
        return (v << s) | (v >> (vec2x64u{64} - s));
        #endif

        #if defined(AVEL_NEON)
        s &= vec2x64u{0x3F};
        return (v << s) | (v >> (vec2x64u{64} - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u rotr(vec2x64u v, long long s) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_rorv_epi64(decay(v), decay(vec2x64u{static_cast<std::uint64_t>(s)}))};
        #elif defined(AVEL_SSE2)
        return (v >> s) | (v << (64 - s));
        #endif

        #if defined(AVEL_NEON)
        return (v >> s) | (v << (64 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u rotr(vec2x64u v, vec2x64u s) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_rorv_epi64(decay(v), decay(s))};
        #elif defined(AVEL_SSE2)
        return (v >> s) | (v << (vec2x64u {64} - s));
        #endif

        #if defined(AVEL_NEON)
        return (v >> s) | (v << (vec2x64u {64} - s));
        #endif
    }

    //=====================================================
    // Conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL arr2x64u to_array(vec2x64u v) {
        alignas(16) arr2x64u array{};
        aligned_store(array.data(), v);
        return array;
    }

    //TODO: Add conversions

}

#endif //AVEL_VEC2X64U_HPP
