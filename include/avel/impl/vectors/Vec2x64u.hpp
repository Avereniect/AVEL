//
// Created by avereniect on 8/3/22.
//

#ifndef AVEL_VEC2X64U_HPP
#define AVEL_VEC2X64U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec2x64u = Vector<std::uint64_t, 2>;

    using mask2x64u = Vector_mask<std::uint64_t, 2>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec2x64u> div(vec2x64u numerator, vec2x64u denominator);

    vec2x64u broadcast_bits(mask2x64u m);

    vec2x64u blend(vec2x64u a, vec2x64u b, mask2x64u m);

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
        #elif defined(AVEL_NEON)
        using primitive = uint64x2_t;
        #endif

        //=================================================
        // -ctors
        //=================================================

        AVEL_FINL explicit Vector_mask(Vector_mask<std::int64_t, 2>);

        AVEL_FINL explicit Vector_mask(Vector_mask<double, 2>);

        AVEL_FINL Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 2>& arr):
        #if defined(AVEL_AVX512VL)
            content() {

            int t = 0x00;
            t |= int(arr[0]) << 0;
            t |= int(arr[1]) << 1;

            content = _mm512_int2mask(t);
        }
        #elif defined(AVEL_SSE2)
            content() {
            static_assert(sizeof(bool) == 1);

            alignas(16) static const std::uint32_t mask_data[4] {
                0x000000FF,
                0x000000FF,
                0x00FF0000,
                0x00FF0000
            };

            primitive array_data = _mm_loadu_si16(arr.data());
            #if defined(AVEL_AVX2)
            array_data = _mm_broadcastd_epi32(array_data);
            #else
            array_data = _mm_shuffle_epi32(array_data, 0x00);
            #endif

            primitive mask = _mm_load_si128((const __m128i*)mask_data);

            primitive t2 = _mm_and_si128(array_data, mask);
            content = _mm_or_si128(
                _mm_cmpgt_epi32(t2, _mm_setzero_si128()),
                _mm_cmplt_epi32(t2, _mm_setzero_si128())
            );
        }
        #elif defined(AVEL_NEON)
        //TODO: Implement
        #endif

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator=(bool x) {
            content = from_bool(x);
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
            return (_mm512_mask2int(lhs) == _mm512_mask2int(rhs));
            #elif defined(AVEL_SSE2)
            return (0xFFFF == _mm_movemask_epi8(_mm_cmpeq_epi32(lhs, rhs)));
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return (_mm512_mask2int(lhs) != _mm512_mask2int(rhs));
            #elif defined(AVEL_SSE2)
            return (0xFFFF != _mm_movemask_epi8(_mm_cmpeq_epi32(lhs, rhs)));
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs);
            #elif defined(AVEL_NEON)
            content = vandq_u64(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs);
            #elif defined(AVEL_NEON)
            content = vorrq_u64(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs);
            #elif defined(AVEL_NEON)
            content = veorq_u64(content, rhs);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_knot_mask8(content)};
            #elif defined(AVEL_SSE2)
            primitive tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};
            #elif defined(AVEL_NEON)
            return Vector_mask{vmvnq_u64(content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(lhs.content, rhs)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_and_si128(lhs, rhs)};
            #elif defined(AVEL_NEON)
            return Vector_mask{vandq_u64(content, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_or_si128(lhs, rhs)};
            #elif defined(AVEL_NEON)
            return Vector_mask{vorrq_u64(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_xor_si128(lhs, rhs)};
            #elif defined(AVEL_NEON)
            return Vector_mask{veorq_u64(lhs, rhs)};
            #endif
        }

        //=================================================
        // Conversion operators
        //=================================================

        AVEL_FINL operator primitive() const {
            return content;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

        //=================================================
        // Helper functions
        //=================================================

        AVEL_FINL static primitive from_bool(bool x) {
            #if defined(AVEL_AVX512VL)
            static const primitive full_masks[2] {
                0x00,
                0xFF
            };

            return full_masks[x];
            #elif defined(AVEL_SSE2)
            static const primitive full_masks[2] {
                {0,  0},
                {-1, -1}
            };

            return full_masks[x];
            #elif defined(AVEL_NEON)
            //TODO: Consider alternative implementations
            std::uint64_t y{x};
            primitive z{};
            return vsubq_u64(veorq_u64(z, z), vdupq_n_u64(y));
            #endif
        }


    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask2x64u m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(m));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(m)) / sizeof(std::uint64_t);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask2x64u m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask2x64u m) {
        #if defined(AVEL_AVX512VL)
        return 0x0F == _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask2x64u m) {
        return !all(m);
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

        using primitive = __m128i;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // -ctors
        //=================================================

        AVEL_FINL explicit Vector(Vector<std::int64_t, width> v);

        AVEL_FINL explicit Vector(Vector<double, width> v);

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm_mask_blend_epi64(m, _mm_setzero_si128(), _mm_set1_epi64x(1))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi64(_mm_setzero_si128(), m)) {}
        #elif defined(AVEL_NEON)
            content(vsubq_u32(zeros(), m)) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_shuffle_epi32(_mm_cvtsi64_si128(x), 0x44)) {}
        #elif defined(AVEL_NEON)
            content(vdupq_n_u32(x)) {}
        #endif

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            Vector(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}

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
            this->content = p;
            return *this;
        }

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_SSE2)
            content = _mm_shuffle_epi32(_mm_cvtsi64_si128(x), 0x44);
            #elif defined(AVEL_NEON)
            content = vdupq_n_u32(x);
            #endif
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE41)
            return mask{_mm_cmpeq_epi64(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            //TODO: Implement
            return mask{};
            #elif defined(AVEL_NEON)
            return mask{vceqq_u32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epu64_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return ~(lhs == rhs);
            #elif defined(AVEL_NEON)
            return mask{vtstq_u32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmplt_epu64_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            auto tmp = (lhs - rhs);
            return mask{_mm_srai_epi32(tmp, 63)};
            #elif defined(AVEL_NEON)
            return mask{vcleq_u32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epu64_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return ~mask{lhs > rhs};
            #elif defined(AVEL_NEON)
            return mask{vcleq_u32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epu64_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return rhs < lhs;
            #elif defined(AVEL_NEON)
            return mask{vcgtq_u32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epu64_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return ~mask{lhs < rhs};
            #elif defined(AVEL_NEON)
            return mask{vcgeq_u32(lhs, rhs)};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() {
            return *this;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator-(Vector v) {
            return Vector{} - v;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi64(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = vaddq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi64(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = vsubq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512VL)
            content = _mm_mullo_epi64(content, rhs.content);
            #elif defined(AVEL_SSE2)
            vec4x32u x = bit_cast<vec4x32u>(*this);
            vec4x32u y = bit_cast<vec4x32u>(rhs);

            vec4x32u ret = (x * y) + ((x * (y >> 32) + y * (x >> 32)) << 32);
            content = ret;
            #elif defined(AVEL_NEON)
            content = vmulq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            auto results = div(*this, rhs);
            *this = results.quot;
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector rhs) {
            auto results = div(*this, rhs);
            *this = results.rem;
            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend Vector operator+(Vector lhs, Vector rhs) {
            #if defined(AVEL_SSE2)
            return Vector{_mm_add_epi64(lhs, rhs)};
            #elif defined(AVEL_NEON)
            return Vector{vaddq_u32(content, vec.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator-(Vector lhs, Vector rhs) {
            #if defined(AVEL_SSE2)
            return Vector{_mm_sub_epi64(lhs, rhs)};
            #elif defined(AVEL_NEON)
            return Vector{vsubq_u32(content, vec.content)};
            #endif
        }

        [[nodiscard]]
       AVEL_FINL friend Vector operator*(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return Vector{_mm_mullo_epi64(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            vec4x32u x = bit_cast<vec4x32u>(lhs);
            vec4x32u y = bit_cast<vec4x32u>(rhs);

            vec4x32u ret = (x * y) + ((x * (y >> 32) + y * (x >> 32)) << 32);
            return bit_cast<Vector>(ret);
            #elif defined(AVEL_NEON)
            return Vector{vmulq_u32(content, vec.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator/(Vector lhs, Vector rhs) {
            auto results = div(lhs, rhs);
            return results.quot;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator%(Vector lhs, Vector rhs) {
            auto results = div(lhs, rhs);
            return results.rem;
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
            #elif defined(AVEL_NEON)
            content = vandq_u64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = vorrq_u64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = veorq_u64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(std::uint32_t s) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi64(content, _mm_cvtsi32_si128(s));
            #elif defined(AVEL_NEON)
            content = vshlq_u64(content, vdupq_n_s32(static_cast<std::int32_t>(s)));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t s) {
            #if defined(AVEL_SSE2)
            content = _mm_srl_epi64(content, _mm_cvtsi32_si128(s));
            #elif defined(AVEL_NEON)
            content = vsraq_n_u64(content, vdupq_n_u32(s), 0x00);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi64(content, rhs.content);

            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i  < 6; ++i) {
                std::uint64_t s = 1ul << i;
                auto t0 = rhs & vec2x64u{s};
                auto m = vec2x64u{} < t0;
                content = blend(*this, *this << s, mask(m));
            }

            content = vec2x64u{content} & broadcast_bits(rhs < vec2x64u{64});

            #elif defined(AVEL_NEON)
            content = vshlq_u64(content, vreinterpretq_s32_u32(rhs.content));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_srlv_epi64(content, rhs.content);
            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i < 6; ++i) {
                vec2x64u threshold{1u << i};
                mask2x64u m = (rhs > threshold);
                *this = blend(*this, *this >> i, m);
            }

            return *this;
            #elif defined(AVEL_NEON)
            content = vsraq_n_u64(content, rhs.content, 0x00);
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
            primitive t = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi32(t, t))};
            #elif defined(AVEL_NEON)
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
        AVEL_FINL operator primitive() const {
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
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64u blend(vec2x64u a, vec2x64u b, mask2x64u m) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_mask_blend_epi64(
            m,
            static_cast<__m128i>(a),
            static_cast<__m128i>(b)
            )
        };

        #elif defined(AVEL_SSE41)
        return vec2x64u{_mm_blendv_epi8(a, b, m)};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(m, a);
        auto y = _mm_and_si128(m, b);
        return vec2x64u{_mm_or_si128(x, y)};

        #elif defined(AVEL_NEON)
        auto x = vandq_u32(b, m);
        auto y = vornq_u32(m, a);
        return vec2x64u{vornq_u32(x, y)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u max(vec2x64u a, vec2x64u b) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_max_epu64(a, b)};
        #elif defined(AVEL_SSE2)
        return blend(a, b, a < b);
        #elif defined(AVEL_NEON)
        return vec2x64u{vmaxq_u32(a, b)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u min(vec2x64u a, vec2x64u b) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_min_epu64(a, b)};
        #elif defined(AVEL_SSE2)
        return blend(a, b, b < a);
        #elif defined(AVEL_NEON)
        return vec2x64u{vminq_u32(a, b)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::pair<vec2x64u, vec2x64u> minmax(vec2x64u a, vec2x64u b) {
        #if defined(AVEL_AVX512VL)
        return {
            vec2x64u{_mm_min_epu64(a, b)},
            vec2x64u{_mm_max_epu64(a, b)}
        };
        #elif defined(AVEL_SSE2)
        auto mask = b < a;
        return {blend(a, b, mask), blend(a, b, ~mask)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u clamp(vec2x64u x, vec2x64u lo, vec2x64u hi) {
        return vec2x64u{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u midpoint(vec2x64u a, vec2x64u b) {
        vec2x64u t0 = a & b & vec2x64u{0x1};
        vec2x64u t1 = (a | b) & vec2x64u{0x1} & broadcast_bits(a > b);
        vec2x64u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u average(vec2x64u a, vec2x64u b) {
        return (a >> 1) + (b >> 1) + (a & b & vec2x64u{0x1});;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64u load<vec2x64u>(const std::uint64_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec2x64u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #elif defined(AVEL_NEON)
        return vec2x64u{vld1q_u32(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64u aligned_load<vec2x64u>(const std::uint64_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec2x64u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #elif defined(AVEL_NEON)
        return vec2x64u{vld1q_u32(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64u stream_load<vec2x64u>(const std::uint64_t* ptr) {
        #if defined(AVEL_SSE41)
        //Casting away of const required by intrinsic
        return vec2x64u{_mm_stream_load_si128((__m128i*)ptr)};
        #elif defined(AVEL_SSE2)
        return aligned_load<vec2x64u>(ptr);
        #elif defined(AVEL_NEON)
        return vec2x64u{vld1q_u32(ptr)};
        #endif
    }

    //Definition of gather deferred until vector of signed integers is defined

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64u broadcast<vec2x64u>(std::uint64_t x) {
        #if defined(AVEL_SSE2)
        return vec2x64u{_mm_set1_epi32(x)};
        #endif
    }

    AVEL_FINL void store(std::uint64_t* ptr, vec2x64u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), v);
        #endif
    }

    AVEL_FINL void aligned_store(std::uint64_t* ptr, vec2x64u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), v);
        #elif defined(AVEL_NEON)
        vst1q_u32(ptr, v);
        #endif
    }

    AVEL_FINL void stream_store(std::uint64_t* ptr, vec2x64u v) {
        #if defined(AVEL_SSE2)
        _mm_stream_si128(reinterpret_cast<__m128i*>(ptr), v);
        #elif defined(AVEL_NEON)
        vst1q_u32(ptr, v);
        #endif
    }

    //Definition of scatter deferred until vector of signed integers is defined

    [[nodiscard]]
    AVEL_FINL std::array<std::uint64_t, 2> to_array(vec2x64u v) {
        alignas(16) std::array<std::uint64_t, 2> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer vector operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec2x64u> div(vec2x64u numerator, vec2x64u denominator) {
        vec2x64u quotient{};

        vec2x64u::primitive t0 = numerator;
        vec2x64u::primitive t1 = _mm_srli_si128(t0, 8);
        vec2x64u::primitive t2 = _mm_or_si128(t0, t1);

        vec2x64u::primitive s0 = denominator;
        vec2x64u::primitive s1 = _mm_srli_si128(s0, 8);
        vec2x64u::primitive s2 = _mm_and_si128(s0, s1);

        std::int64_t x = countl_zero(std::int64_t(_mm_cvtsi128_si64(t2)));
        std::int64_t y = countl_zero(std::int64_t(_mm_cvtsi128_si64(s2)));

        std::int64_t i = y - x + 1;

        vec2x64u zeroes{};
        for (; (i-- > 0) && any(numerator != zeroes);) {
            mask2x64u b = ((numerator >> i) >= denominator);
            auto tmp0 = broadcast_bits(b);
            numerator -= (tmp0 & (denominator << i));
            quotient |= (vec2x64u{b} << i);
        }

        return {quotient, numerator};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u broadcast_bits(mask2x64u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec2x64u{_mm_movm_epi64(m)};
        #elif defined(AVEL_AVX512VL)
        const auto x = _mm512_set1_epi64(0);
        const auto y = _mm512_set1_epi64(-1);
        return vec2x64u{_mm512_mask_blend_epi32(m, x, y)};
        #elif defined(AVEL_SSE2)
        return vec2x64u{vec2x64u::primitive(m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u popcount(vec2x64u v) {
        #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512VPOPCNTDQ)
        return vec2x64u{_mm_popcnt_epi64(v)};
        #elif defined(AVELAVX512VL) & defined(AVEL_AVX512BITALG)
        //TODO: Widen implementation
        auto tmp0 = _mm_popcnt_epi16(v);
        auto tmp1 = _mm_slli_epi32(tmp0, 16);
        auto tmp2 = _mm_add_epi32(tmp0, tmp1);

        return vec2x64u{_mm_srli_epi32(tmp2, 16)};
        #elif defined(AVEL_SSE2)
        //TODO: Widen implementation

        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec2x64u{0x55555555});                    // reuse input as temporary
        v = (v & vec2x64u{0x33333333}) + ((v >> 2) & vec2x64u{0x33333333});     // temp
        v = ((v + (v >> 4) & vec2x64u{0xF0F0F0F}) * vec2x64u{0x1010101}) >> 24; // count
        return v;
        #elif defined(AVEL_NEON)
        vec2x64u reg_abcd{vreinterpretq_u32_u8(vcntq_u8(vreinterpretq_u8_u32(v)))};
        vec2x64u reg_00ab{vshrq_n_u32(reg_abcd, 16)};
        vec2x64u reg_00xy{vaddq_u32(reg_abcd, reg_00ab)};

        vec2x64u reg_000x{vshrq_n_u32(reg_00xy, 8)};
        vec2x64u reg_000z{vaddq_u32(reg_000x, reg_00xy)};

        return reg_000z;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u byteswap(vec2x64u v) {
        #if defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t index_data[16] {
             8,  9, 10, 11,
            12, 13, 14, 15,
             0,  1,  2,  3,
             4,  5,  6,  7
        };

        auto indices = _mm_load_si128((const __m128i*)index_data);
        return vec2x64u{_mm_shuffle_epi8(v, indices)};
        #elif defined(AVEL_SSE2)
        //TODO: Adjust implementation to work on 64-bit integers
        alignas(16) static constexpr std::uint32_t mask_data[4]{
            0x00000000,
            0xFFFFFFFF,
            0x00000000,
            0xFFFFFFFF
        };

        auto t0 = _mm_shufflelo_epi16(v, 0xB1);
        auto t1 = _mm_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm_srli_si128(t1, 1);
        auto t3 = _mm_srli_si128(t1, 1);
        auto t4 = _mm_load_si128(reinterpret_cast<const __m128i*>(mask_data));
        return blend(vec2x64u{t2}, vec2x64u{t3}, mask2x64u{t4});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u countl_zero(vec2x64u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec2x64u{_mm_lzcnt_epi64(x)};
        #elif defined(AVEL_SSE2)
        return countl_one(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u countl_one(vec2x64u x) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{~x};
        #else
        //TODO: Adjust to be suitable for 64-bit integers
        vec2x64u sum{x == vec2x64u{0xFFFFFFFF}};

        vec2x64u m0{0xFFFF0000u};
        mask2x64u b0 = (m0 & x) == m0;
        sum += broadcast_bits(b0) & vec2x64u{16};
        x <<= broadcast_bits(b0) * vec2x64u{16};

        vec2x64u m1{0xFF000000u};
        mask2x64u b1 = (m1 & x) == m1;
        sum += broadcast_bits(b1) & vec2x64u{8};
        x <<= broadcast_bits(b1) & vec2x64u{8};

        vec2x64u m2{0xF0000000u};
        mask2x64u b2 = (m2 & x) == m2;
        sum += broadcast_bits(b2) & vec2x64u{4};
        x <<= broadcast_bits(b2) & vec2x64u{4};

        vec2x64u m3{0xC0000000u};
        mask2x64u b3 = (m3 & x) == m3;
        sum += broadcast_bits(b3) & vec2x64u{2};
        x <<= broadcast_bits(b3) & vec2x64u{2};

        vec2x64u m4{0x80000000u};
        mask2x64u b4 = (m4 & x) == m4;
        sum += broadcast_bits(b3) & vec2x64u{1};

        return sum;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u countr_zero(vec2x64u x) {
        //TODO: Implement using binary search
        return vec2x64u{};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u countr_one(vec2x64u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u bit_floor(vec2x64u x) {
        #if defined(AVEL_AVX512CD) && defined(AVEL_AVX512VL)
        vec2x64u leading_zeros = countl_zero(x);
        mask2x64u zero_mask = (leading_zeros != vec2x64u{64});

        return (vec2x64u{zero_mask} << (vec2x64u{63} - leading_zeros));
        #elif defined(AVEL_SSE2)
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        x = x | (x >> 32);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u bit_ceil(vec2x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec2x64u{1} << (vec2x64u{64} << countl_zero(v));
        #elif defined(AVEL_SSE2)
        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;
        ++v;
        return v;
        #endif
    };

    [[nodiscard]]
    AVEL_FINL mask2x64u has_single_bit(vec2x64u x) {
        #if defined(AVELAVX512VL)
        return pop_count(x) == 1;
        #else
        return mask2x64u{x} & ~mask2x64u{x & (x - vec2x64u{1})};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u rotl(vec2x64u v, vec2x64u::scalar s) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_rolv_epi64(v, vec2x64u{s})};
        #elif defined(AVEL_SSE2)
        return (v << s) | (v >> (64 - s));
        #elif defined(AVEL_NEON)
        return (v << S) | (v >> (64 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u rotl(vec2x64u v, vec2x64u s) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_rolv_epi64(v, s)};
        #elif defined(AVEL_SSE2)
        return (v << s) | (v >> (vec2x64u{64} - s));
        #elif defined(AVEL_NEON)
        return (v << s) | (v >> (vec2x64u{64} - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u rotr(vec2x64u v, std::uint32_t s) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_rorv_epi64(v, vec2x64u{s})};
        #elif defined(AVEL_SSE2)
        return (v >> s) | (v << (64 - s));
        #elif defined(AVEL_NEON)
        return (v >> S) | (v << (64 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64u rotr(vec2x64u v, vec2x64u s) {
        #if defined(AVEL_AVX512VL)
        return vec2x64u{_mm_rorv_epi64(v, s)};
        #elif defined(AVEL_SSE2)
        return (v >> s) | (v << (vec2x64u {64} - s));
        #elif defined(AVEL_NEON)
        return (v >> s) | (v << (vec2x64u {64} - s));
        #endif
    }

}

#endif //AVEL_VEC2X64U_HPP
