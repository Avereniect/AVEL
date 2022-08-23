#ifndef AVEL_VEC4x32I_HPP
#define AVEL_VEC4x32I_HPP

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

    vec4x32i blend(vec4x32i a, vec4x32i b, mask4x32u m);

    std::array<std::int32_t, 4> to_array(vec4x32i v);





    template<>
    class alignas(AVEL_MASK_ALIGNMENT_4X32I) Vector_mask<std::int32_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 4;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask(Vector_mask<std::uint32_t, 4> v):
            #if defined(AVEL_AVX512VL)
            content(v) {}
            #elif defined(AVEL_SSE2)
            content() {}
            #endif

        AVEL_FINL explicit Vector_mask(Vector_mask<float, 4> v);

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 4>& arr):
        #if defined(AVEL_AVX512VL)
            content() {

            int t = 0x00;
            t |= int(arr[0]) << 0;
            t |= int(arr[1]) << 1;
            t |= int(arr[2]) << 2;
            t |= int(arr[3]) << 3;

            content = _mm512_int2mask(t);
        }
        #elif defined(AVEL_SSE2)
            content() {
            static_assert(sizeof(bool) == 1);

            alignas(16) static const std::uint32_t mask_data[4] {
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000
            };

            primitive array_data = _mm_loadu_si32(arr.data());
            #if defined(AVEL_AVX2)
            array_data = _mm_broadcastd_epi32(t0);
            #else
            array_data = _mm_shuffle_epi32(array_data, 0x00);
            #endif

            primitive mask = _mm_load_si128((const __m128i*)mask_data);

            primitive t2 = _mm_and_si128(array_data, mask);

            // The two comparisons are used because it works without
            // assuming a particular representation of how true is
            // represented, and instead only assumes that all zeros indicates
            // false.
            content = _mm_or_si128(
                _mm_cmpgt_epi32(t2, _mm_setzero_si128()),
                _mm_cmplt_epi32(t2, _mm_setzero_si128())
            );
        }
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
            content = _kand_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = vandq_s32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = vorrq_s32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = veorrq_s32(contnet, rhs.content);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_knot_mask8(content)};
            #elif defined(AVEL_SSE2)
            primitive tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};
            #elif defined(AVEL_NEON)
            return Vector_mask{vmvnq_s32(content)};
            #endif
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_and_si128(content, rhs.content)};
            #elif defined(AVEL_NEON)
            return Vector_mask{vandq_s32(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_or_si128(content, rhs.content)};
            #elif defined(AVEL_NEON)
            return Vector_mask{vorrq_s32(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_xor_si128(content, rhs.content)};
            #elif defined(AVEL_NEON)
            return Vector_mask{veorq_s32(content, rhs.content)};
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
            std::uint32_t y{x};
            primitive z{};
            return vsubq_u32(veorq_s32(z, z), vdupq_n_s32(y));
            #endif
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask4x32i m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(m));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(m)) / sizeof(std::uint32_t);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x32i m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x32i m) {
        #if defined(AVEL_AVX512VL)
        return 0x0F == _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x32i m) {
        return !all(m);
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL mask4x32u::Vector_mask(mask4x32i x):
        content(x) {}





    template<>
    class alignas(16) Vector<std::int32_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int32_t;

        using primitive = __m128i;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(vec4x32u v):
            content(primitive(v)) {}

        AVEL_FINL explicit Vector(Vector<float, width> v);

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm_mask_blend_epi32(m, _mm_setzero_si128(), _mm_set1_epi32(1))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi32(_mm_setzero_si128(), m)) {}
        #endif

        /*
        AVEL_FINL explicit Vector(std::int32_t a, std::int32_t b, std::int32_t c, std::int32_t d):
            content(_mm_set_epi32(d, c, b, a)) {}
        */

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const scalar x):
            content(_mm_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(array.data()))) {}

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
            content = _mm_set1_epi32(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epi32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return ~(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmplt_epi32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_epi32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epi32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return ~(lhs > rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epi32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_epi32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epi32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return ~(lhs < rhs);
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
        AVEL_FINL Vector operator-() const {
            return Vector{} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content = _mm_add_epi32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm_sub_epi32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_SSE41)
            content = _mm_mullo_epi32(content, rhs.content);
            #elif defined(AVEL_SSE2)
            primitive lo0 = _mm_and_si128(content, _mm_set1_epi32(0x0000FFFF));
            primitive hi0 = _mm_srli_epi64(content, 32);

            primitive lo1 = _mm_and_si128(rhs, _mm_set1_epi32(0x0000FFFF));
            primitive hi1 = _mm_srli_epi64(rhs, 32);

            primitive lo = _mm_mul_epu32(lo0, lo1);
            primitive hi = _mm_mul_epu32(hi0, hi1);

            content = _mm_or_si128(lo, _mm_slli_epi64(hi, 32));
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
            return Vector{_mm_add_epi32(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator-(Vector lhs, Vector rhs) {
            return Vector{_mm_sub_epi32(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL  Vector operator*(const Vector vec) const {
        #if defined(AVEL_SSE41)
            return Vector{_mm_mullo_epi32(content, vec.content)};
        #elif defined(AVEL_SSE2)
            primitive lo0 = _mm_and_si128(content, _mm_set1_epi32(0x0000FFFF));
            primitive hi0 = _mm_srli_epi64(content, 32);

            primitive lo1 = _mm_and_si128(vec, _mm_set1_epi32(0x0000FFFF));
            primitive hi1 = _mm_srli_epi64(vec, 32);

            primitive lo = _mm_mul_epu32(lo0, lo1);
            primitive hi = _mm_mul_epu32(hi0, hi1);

            primitive result = _mm_or_si128(lo, _mm_slli_epi64(hi, 32));
            return Vector{result};
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
            content = _mm_and_si128(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            content = _mm_or_si128(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            content = _mm_xor_si128(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(std::uint32_t s) {
            content = _mm_sll_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t s) {
            content = _mm_sra_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(vec4x32u s) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi32(content, primitive(s));
            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i < 5; ++i) {
                vec4x32u threshold{1u << i};
                mask4x32u m = (s > threshold);
                *this = blend(*this, *this << i, m);
            }

            return *this;
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(vec4x32u s) {
            #if defined(AVEL_AVX2)
            content = _mm_srav_epi32(content, primitive(s));
            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i < 5; ++i) {
                vec4x32u threshold{1u << i};
                mask4x32u m = (s > threshold);
                *this = blend(*this, *this >> i, m);
            }
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            return Vector{_mm_andnot_si128(content, _mm_set1_epi32(0xFFFFFFFF))};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator&(Vector lhs, Vector rhs) {
            return Vector{_mm_and_si128(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator|(Vector lhs, Vector rhs) {
            return Vector{_mm_or_si128(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator^(Vector lhs, Vector rhs) {
            return Vector{_mm_xor_si128(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, std::uint32_t s) {
            return Vector{_mm_sll_epi32(lhs, _mm_cvtsi32_si128(s))};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, std::uint32_t s) {
            return Vector{_mm_sra_epi32(lhs, _mm_cvtsi32_si128(s))};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, vec4x32u rhs) {
            #if defined(AVEL_AVX2)
            return Vector{_mm_sllv_epi32(content, primitive(vec))};
            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i < 5; ++i) {
                vec4x32u threshold{1u << i};
                mask4x32u m = (rhs > threshold);
                lhs = blend(lhs, lhs << i, m);
            }

            return lhs;
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, vec4x32u rhs) {
            #if defined(AVEL_AVX2)
            return Vector{_mm_srav_epi32(content, primitive(vec))};
            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i < 5; ++i) {
                vec4x32u threshold{1u << i};
                mask4x32u m = (rhs > threshold);
                lhs = blend(lhs, lhs >> i, m);
            }

            return lhs;
            #endif
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
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec4x32u::Vector(vec4x32i c):
        content(c) {}

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32i blend(vec4x32i a, vec4x32i b, mask4x32i m) {
        #if defined(AVEL_AVX512VL)
        return vec4x32i{_mm_mask_blend_epi32(
            m,
            static_cast<__m128i>(a),
            static_cast<__m128i>(b)
            )
        };
        #elif defined(AVEL_SSE41)
        return vec4x32i{_mm_blendv_epi8(a, b, m)};
        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(m, a);
        auto y = _mm_and_si128(m, b);
        return vec4x32i{_mm_or_si128(x, y)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i max(vec4x32i a, vec4x32i b) {
        #if defined(AVEL_SSE41)
        return vec4x32i{_mm_max_epi32(a, b)};
        #elif defined(AVEL_SSE2)
        return blend(a, b, a < b);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i min(vec4x32i a, vec4x32i b) {
        #if defined(AVEL_SSE41)
        return vec4x32i{_mm_min_epi32(a, b)};
        #elif defined(AVEL_SSE2)
        return blend(a, b, b < a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::pair<vec4x32i, vec4x32i> minmax(vec4x32i a, vec4x32i b) {
        #if defined(AVEL_SSE41)
        return {
            vec4x32u{_mm_min_epi32(a, b)},
            vec4x32u{_mm_max_epi32(a, b)}
        };
        #elif defined(AVEL_SSE2)
        auto mask = b < a;
        return {blend(a, b, mask), blend(a, b, ~mask)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i clamp(vec4x32i x, vec4x32i lo, vec4x32i hi) {
        return vec4x32i{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i midpoint(vec4x32i a, vec4x32i b) {
        const vec4x32u mask{0x80000000};

        auto x = vec4x32u{a} ^ mask;
        auto y = vec4x32u{b} ^ mask;

        return vec4x32i{midpoint(x, y) ^ mask};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i average(vec4x32i a, vec4x32i b) {
        //TODO: More efficient implementation?
        const vec4x32u mask{0x80000000};

        auto x = vec4x32u{a} ^ mask;
        auto y = vec4x32u{b} ^ mask;

        return vec4x32i{average(x, y) ^ mask};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i abs(vec4x32i v) {
        #ifdef AVEL_SSSE3
        return vec4x32i{_mm_abs_epi32(v)};
        #elif defined(AVEL_SSE2)
        auto y = v >> 31;
        return (v ^ y) - y;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i neg_abs(vec4x32i v) {
        #ifdef AVEL_SSSE3
        return -vec4x32i{_mm_abs_epi32(v)};
        #elif defined(AVEL_SSE2)
        auto y = ~(v >> 31);
        return (v ^ y) - y;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i neg_abs(vec4x32u v) {
        return neg_abs(bit_cast<vec4x32i>(v));
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32i load<vec4x32i>(const std::int32_t* ptr) {
        return vec4x32i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32i aligned_load<vec4x32i>(const std::int32_t* ptr) {
        return vec4x32i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32i stream_load<vec4x32i>(const std::int32_t* ptr) {
        #if defined(AVEL_SSE41)
        //Casting away const is required by intrinsic
        return vec4x32i{_mm_stream_load_si128((__m128i*)(ptr))};
        #elif defined(AVEL_SSE2)
        return aligned_load<vec4x32i>(ptr);
        #endif
    }

    template<>
    AVEL_FINL vec4x32u gather<vec4x32u>(const std::uint32_t* ptr, vec4x32i indices) {
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
        return vec4x32u{abcd};
        #endif
    }

    template<>
    AVEL_FINL vec4x32i gather<vec4x32i>(const std::int32_t* ptr, vec4x32i indices) {
        #if defined(AVEL_AVX2)
        return vec4x32i{_mm_i32gather_epi32(ptr, indices, sizeof(std::int32_t))};
        #elif defined(AVEL_SSE2)
        auto i = indices.as_array();

        __m128i a = _mm_loadu_si32(ptr + i[0]);
        __m128i b = _mm_loadu_si32(ptr + i[1]);
        __m128i c = _mm_loadu_si32(ptr + i[2]);
        __m128i d = _mm_loadu_si32(ptr + i[3]);

        __m128i abab = _mm_unpacklo_epi32(a, b);
        __m128i cdcd = _mm_unpacklo_epi32(c, d);

        __m128i abcd = _mm_unpacklo_epi64(abab, cdcd);
        return vec4x32i{abcd};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32i broadcast<vec4x32i>(std::int32_t x) {
        #if defined(AVEL_SSE2)
        return vec4x32i{_mm_set1_epi32(x)};
        #endif
    }

    AVEL_FINL void store(std::int32_t* ptr, vec4x32i v) {
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), v);
    }

    AVEL_FINL void aligned_store(std::int32_t* ptr, vec4x32i v) {
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), v);
    }

    AVEL_FINL void stream_store(std::int32_t* ptr, vec4x32i v) {
        _mm_stream_si128(reinterpret_cast<__m128i*>(ptr), v);
    }

    AVEL_FINL void scatter(std::uint32_t* ptr, vec4x32i indices, vec4x32u v) {
        #if defined(AVEL_AVX512VL)
        _mm_i32scatter_epi32(ptr, indices, v, sizeof(std::int32_t));
        #elif defined(AVEL_SSE2)
        auto i = to_array(indices);

        _mm_storeu_si32(ptr + i[0], v);
        _mm_storeu_si32(ptr + i[1], _mm_bsrli_si128(v, 0x4));
        _mm_storeu_si32(ptr + i[2], _mm_bsrli_si128(v, 0x8));
        _mm_storeu_si32(ptr + i[3], _mm_bsrli_si128(v, 0xC));
        #endif
    }

    AVEL_FINL void scatter(std::int32_t* ptr, vec4x32i indices, vec4x32i v) {
        #if defined(AVEL_AVX512VL)
        _mm_i32scatter_epi32(ptr, indices, v, sizeof(std::int32_t));
        #elif defined(AVEL_SSE2)
        auto i = to_array(indices);

        _mm_storeu_si32(ptr + i[0], v);
        _mm_storeu_si32(ptr + i[1], _mm_bsrli_si128(v, 0x4));
        _mm_storeu_si32(ptr + i[2], _mm_bsrli_si128(v, 0x8));
        _mm_storeu_si32(ptr + i[3], _mm_bsrli_si128(v, 0xC));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<std::int32_t, 4> to_array(vec4x32i v) {
        alignas(16) std::array<std::int32_t, 4> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32u popcount(vec4x32i v) {
        return popcount(bit_cast<vec4x32u>(v));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i byteswap(vec4x32i v) {
        return byteswap(bit_cast<vec4x32i>(v));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countl_zero(vec4x32i x) {
        return countl_zero(bit_cast<vec4x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countl_one(vec4x32i x) {
        return countl_zero(bit_cast<vec4x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countr_zero(vec4x32i x) {
        return countr_zero(bit_cast<vec4x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countr_one(vec4x32i x) {
        return countr_one(bit_cast<vec4x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL mask4x32i has_single_bit(vec4x32i x) {
        return bit_cast<mask4x32i>(has_single_bit(bit_cast<vec4x32u>(x)));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotl(vec4x32i v, std::uint32_t s) {
        return bit_cast<vec4x32i>(rotl(bit_cast<vec4x32u>(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotl(vec4x32i v, vec4x32u s) {
        return bit_cast<vec4x32i>(rotl(bit_cast<vec4x32u>(v), bit_cast<vec4x32u>(s)));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotr(vec4x32i v, std::uint32_t s) {
        return bit_cast<vec4x32i>(rotr(bit_cast<vec4x32u>(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotr(vec4x32i v, vec4x32u s) {
        return bit_cast<vec4x32i>(rotr(bit_cast<vec4x32u>(v), bit_cast<vec4x32u>(s)));
    }

}

#endif
