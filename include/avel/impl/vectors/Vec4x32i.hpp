#ifndef AVEL_VEC4X32I_HPP
#define AVEL_VEC4X32I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x32i = Vector<std::int32_t, 4>;
    using arr4x32i = std::array<std::int32_t, 4>;
    using mask4x32i = Vector_mask<std::int32_t, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec4x32i> div(vec4x32i numerator, vec4x32i denominator);
    vec4x32i blend(mask4x32i m, vec4x32i a, vec4x32i b);
    vec4x32i broadcast_mask(mask4x32i m);
    vec4x32i countl_one(vec4x32i x);





    template<>
    class Vector_mask<std::int32_t, 4> {
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
        // Constructor
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
            content(b ? _mm_set1_epi32(-1) : _mm_setzero_si128()) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u32(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr4xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            auto expanded = _mm_cvtepi8_epi32(array_data);
            content = _mm_cmplt_epu32_mask(_mm_setzero_si128(), expanded);

            #elif defined(AVEL_SSE2)
            primitive array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
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

        Vector_mask& operator=(bool x) {
            *this = Vector_mask{x};
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
            #if defined(AVEL_AVX512VL)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);

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
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_SSE2)
            return (0xFFFF != _mm_movemask_epi8(_mm_cmpeq_epi32(decay(lhs), decay(rhs))));
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
            content &= decay(rhs);

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
            content |= decay(rhs);
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
            content ^= decay(rhs);

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

        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{static_cast<__mmask8>(content ^ 0x0f)};

            #elif defined(AVEL_SSE2)
            primitive tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};

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
        // Conversion operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32u, 1> convert<mask4x32u, mask4x32i>(mask4x32i m) {
        return {mask4x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x32i, 1> convert<mask4x32i, mask4x32u>(mask4x32u m) {
        return {mask4x32i{decay(m)}};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask4x32i m) {
        return count(mask4x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x32i m) {
        return any(mask4x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x32i m) {
        return all(mask4x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x32i m) {
        return none(mask4x32u{m});
    }





    template<>
    class alignas(16) Vector<std::int32_t, 4> {
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
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int32x4_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
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
        #if defined(AVEL_AVX512VL)
            content(_mm_maskz_set1_epi32(decay(m), 0x1)) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi32(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vnegq_s32(vreinterpretq_s32_u32(decay(m)))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi32(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_s32(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr4x32i& array):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_s32(array.data())) {}
        #endif

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(scalar x) {
            *this = Vector{x};
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
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi32(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_s32(decay(lhs), decay(rhs))};
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
            return mask{_mm_cmplt_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_epi32(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_s32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !(lhs > rhs);
            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_s32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_epi32(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_s32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !(lhs < rhs);
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_s32(decay(lhs), decay(rhs))};
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
            content = _mm_add_epi32(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_s32(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi32(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_s32(content, decay(rhs));
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
            content = vmulq_s32(content, decay(rhs));
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
            content = vandq_s32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_s32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_s32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi32(content, _mm_cvtsi64_si128(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s32(content, vdupq_n_s32(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sra_epi32(content, _mm_cvtsi64_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s32(content, vdupq_n_s32(-rhs));

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
            content = vshlq_s32(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(vec4x32i rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_srav_epi32(content, primitive(rhs));
            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto x_half0 = _mm_unpacklo_epi32(content, zeros);
            auto x_half1 = _mm_unpackhi_epi32(content, zeros);

            auto y_half0 = _mm_unpacklo_epi32(decay(rhs), zeros);
            auto y_half1 = _mm_unpackhi_epi32(decay(rhs), zeros);

            auto X0 = _mm_sra_epi32(x_half0, y_half0);
            auto X1 = _mm_sra_epi32(x_half0, _mm_srli_si128(y_half0, 8));
            auto X2 = _mm_sra_epi32(x_half1, y_half1);
            auto X3 = _mm_sra_epi32(x_half1, _mm_srli_si128(y_half1, 8));

            auto t0 = _mm_unpacklo_epi64(X0, X2);
            auto t1 = _mm_unpackhi_epi64(X1, X3);

            auto t2 = _mm_slli_si128(t1, 0x4);

            content = _mm_or_si128(t0, t2);

            #endif

            #if defined(AVEL_NEON)
            content  = vshlq_s32(content, vnegq_s32(decay(rhs)));

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
            return Vector{vmvnq_s32(content)};
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
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_test_epi32_mask(content, content)};

            #else
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        4 * sizeof(std::int32_t) == sizeof(vec4x32i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 1> convert<vec4x32u, vec4x32i>(vec4x32i m) {
        #if defined(AVEL_SSE2)
        return {vec4x32u{decay(m)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec4x32u{vreinterpretq_u32_s32(decay(m))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x32i, 1> convert<vec4x32i, vec4x32u>(vec4x32u m) {
        #if defined(AVEL_SSE2)
        return {vec4x32i{decay(m)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec4x32i{vreinterpretq_s32_u32(decay(m))}};
        #endif
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec4x32i operator-(vec4x32u v) {
        return vec4x32i{0x00} - vec4x32i{v};
    }

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL std::int32_t extract(vec4x32i v) {
        static_assert(N < vec4x32i::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x32i::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SS41)
        return _mm_extract_epi32(decay(v), N);

        #elif defined(AVEL_SSE2)
        return _mm_cvtsi128_si32(_mm_srli_si128(decay(v), 4 * N));

        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_s32(decay(v), N);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x32i insert(vec4x32i v, std::int32_t x) {
        static_assert(N < vec4x32i::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x32i::width, int>::type dummy_variable = 0;

        return vec4x32i{insert<N>(vec4x32u{v}, static_cast<std::uint32_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x32i bit_shift_left(vec4x32i v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        return vec4x32i{bit_shift_left<S>(vec4x32u{v})};
    }

    template<>
    AVEL_FINL vec4x32i bit_shift_left<0>(vec4x32i v) {
        return v;
    }

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x32i bit_shift_right(vec4x32i v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec4x32i{_mm_srai_epi32(decay(v), S)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32i{vshrq_n_s32(decay(v), S)};
        #endif
    }

    template<>
    AVEL_FINL vec4x32i bit_shift_right<0>(vec4x32i v) {
        return v;
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x32i rotl(vec4x32i v) {
        return vec4x32i{rotl<S>(vec4x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotl(vec4x32i v, std::uint32_t s) {
        return vec4x32i(rotl(vec4x32u(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotl(vec4x32i v, vec4x32i s) {
        return vec4x32i(rotl(vec4x32u(v), vec4x32u(s)));
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x32i rotr(vec4x32i v) {
        return vec4x32i{rotr<S>(vec4x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotr(vec4x32i v, std::uint32_t s) {
        return vec4x32i(rotr(vec4x32u(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i rotr(vec4x32i v, vec4x32i s) {
        return vec4x32i(rotr(vec4x32u(v), vec4x32u(s)));
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec4x32i x) {
        return count(vec4x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec4x32i x) {
        return any(vec4x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec4x32i x) {
        return all(vec4x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec4x32i x) {
        return none(vec4x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i broadcast_mask(mask4x32i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec4x32i{_mm_movm_epi32(decay(m))};

        #elif defined(AVEL_AVX512VL)
        return vec4x32i{_mm_maskz_set1_epi32(decay(m), -1)};

        #elif defined(AVEL_SSE2)
        return vec4x32i{decay(m)};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32i{vreinterpretq_s32_u32(decay(m))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i keep(mask4x32i m, vec4x32i v) {
        return vec4x32i{keep(mask4x32u{m}, vec4x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i clear(mask4x32i m, vec4x32i v) {
        return vec4x32i{clear(mask4x32u{m}, vec4x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i blend(mask4x32i m, vec4x32i a, vec4x32i b) {
        return vec4x32i{blend(mask4x32u{m}, vec4x32u{a}, vec4x32u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i max(vec4x32i a, vec4x32i b) {
        #if defined(AVEL_SSE41)
        return vec4x32i{_mm_max_epi32(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, b, a);

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
        return blend(b < a, b, a);

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
        return {blend(mask, b, a), blend(mask, a, b)};
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
        #if defined(AVEL_SSE2)
        auto avg = (x & y) + ((x ^ y) >> 1);
        auto c = broadcast_mask((x < -y) | (y == vec4x32i{std::int32_t(1 << 31)})) & (x ^ y) & vec4x32i{1};

        return avg + c;

        #endif

        #if defined(AVEL_NEON)
        auto avg = vec4x32i{vhaddq_s32(decay(x), decay(y))};
        auto c = broadcast_mask((x < -y) | (y == vec4x32i{std::int32_t(1 << 31)})) & (x ^ y) & vec4x32i{1};

        return avg + c;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i midpoint(vec4x32i a, vec4x32i b) {
        #if defined(AVEL_AVX512VL)
        auto avg = decay(((a ^ b) >> 1) + (a & b));

        auto bias = _mm_ternarylogic_epi32(decay(a), decay(b), _mm_set1_epi32(0x1), 0x28);
        auto mask = _mm_cmplt_epi32_mask(decay(b), decay(a));
        auto ret = _mm_mask_add_epi32(avg, mask, avg, bias);

        return vec4x32i{ret};

        #elif defined(AVEL_SSE2)
        auto average = ((a ^ b) >> 1) + (a & b);
        auto bias = (broadcast_mask(b < a) & (a ^ b) & vec4x32i{0x1});
        return average + bias;

        #endif

        #if defined(AVEL_NEON)
        vec4x32i t0 = vec4x32i{vhaddq_s32(decay(a), decay(b))};
        vec4x32i t1 = (a ^ b) & vec4x32i{0x1} & broadcast_mask(b < a);
        return t0 + t1;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i negate(mask4x32i m, vec4x32i x) {
        #if defined(AVEL_AVX512VL)
        return vec4x32i{_mm_mask_sub_epi32(decay(x), decay(m), _mm_setzero_si128(), decay(x))};

        #elif defined(AVEL_SSSE3)
        return vec4x32i{_mm_sign_epi32(decay(x), _mm_or_si128(decay(m), _mm_set1_epi32(0x01)))};
        #endif

        #if defined(AVEL_SSE2) || defined(AVEL_NEON)
        auto mask = broadcast_mask(m);
        return (x ^ mask) - mask;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i abs(vec4x32i x) {
        #if defined(AVEL_SSSE3)
        return vec4x32i{_mm_abs_epi32(decay(x))};
        #elif defined(AVEL_SSE2)
        auto y = x >> 31;
        return (x ^ y) - y;
        #endif

        #if defined(AVEL_NEON)
        auto zeros = vdupq_n_s32(0x00);
        return vec4x32i{vabdq_s32(decay(x), zeros)};
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

        #if defined(AVEL_NEON)
        auto y = vmvnq_s32(vshrq_n_s32(decay(x), 31));
        return vec4x32i{vsubq_s32(veorq_s32(decay(x), y), y)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i neg_abs(vec4x32u x) {
        return neg_abs(vec4x32i{x});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32i load<vec4x32i>(const std::int32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        n = std::min(4u, n);
        auto mask = (1 << n) - 1;
        return vec4x32i{_mm_maskz_load_epi32(mask, ptr)};

        #elif defined(AVEL_SSE2)
        switch (n) {
            case 0: {
                return vec4x32i{_mm_setzero_si128()};
            }
            case 1: {
                return vec4x32i{_mm_cvtsi32_si128(ptr[0])};
            }
            case 2: {
                std::int64_t two_s = 0x00;

                std::memcpy(&two_s, ptr + 0, sizeof(std::int64_t));

                return vec4x32i{_mm_cvtsi64_si128(two_s)};
            }
            case 3: {
                std::int64_t two_s;

                std::memcpy(&two_s, ptr + 0, sizeof(std::int64_t));


                std::int32_t one_s = 0x00;

                std::memcpy(&one_s, ptr + 2, sizeof(std::int32_t));

                auto two_v = _mm_cvtsi64_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec4x32i{_mm_unpacklo_epi64(two_v, one_v)};
            }
            default: {
                return vec4x32i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
            }
        }

        #elif defined(AVEL_NEON)
        switch (n) {
            case 0: {
                return vec4x32i{vdupq_n_s32(0x00)};
            };
            case 1: {
                std::uint32_t x0;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint32_t));

                auto ret0 = vsetq_lane_s32(x0, vdupq_n_s32(0x00), 0);
                return vec4x32i{ret0};
            }
            case 2: {
                std::uint64_t x0;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vreinterpretq_s32_s64(ret0);
                return vec4x32i{ret1};
            }
            case 3: {
                std::uint64_t x0;
                std::uint32_t x1;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));
                std::memcpy(&x1, ptr + 2, sizeof(std::uint32_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s32(x1, vreinterpretq_s32_s64(ret0), 2);
                return vec4x32i{ret1};
            }
            default: {
                return vec4x32i{vld1q_s32(ptr)};
            }
        }
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
    AVEL_FINL vec4x32i aligned_load<vec4x32i>(const std::int32_t* ptr, std::uint32_t n) {
        return load<vec4x32i>(ptr, n);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32i aligned_load<vec4x32i>(const std::int32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Utilize __builtin_assume_aligned on GCC and Clang
        //TODO: Utilize assume_aligned if C++ 20 is available
        return vec4x32i{vld1q_s32(ptr)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u gather<vec4x32u>(const std::uint32_t* ptr, vec4x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        n = min(n, vec4x32u::width);
        auto mask = (1 << n) - 1;

        return vec4x32u{_mm_mmask_i32gather_epi32(_mm_setzero_si128(), mask, decay(indices), ptr, sizeof(std::uint32_t))};

        #elif defined(AVEL_AVX2)
        n = min(n, vec4x32u::width);

        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec4x32u::width;
        auto h = vec4x32u::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (h - min(h, n))));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (w - min(w, n))));
        auto mask = _mm_unpacklo_epi64(lo, hi);

        auto ret = _mm_mask_i32gather_epi32(
            _mm_setzero_si128(),
            bit_cast<const int*>(ptr),
            decay(indices),
            mask,
            sizeof(std::int32_t)
        );

        return vec4x32u{ret};

        #elif defined(AVEL_SSE)
        auto a = _mm_setzero_si128();
        auto b = _mm_setzero_si128();
        auto c = _mm_setzero_si128();
        auto d = _mm_setzero_si128();

        switch (n) {
            default: d = _mm_cvtsi32_si128(ptr[extract<3>(indices)]);
            case 3:  c = _mm_cvtsi32_si128(ptr[extract<2>(indices)]);
            case 2:  b = _mm_cvtsi32_si128(ptr[extract<1>(indices)]);
            case 1:  a = _mm_cvtsi32_si128(ptr[extract<0>(indices)]);
            case 0:  ; //Do nothing
        }

        auto abab = _mm_unpacklo_epi32(a, b);
        auto cdcd = _mm_unpacklo_epi32(c, d);

        auto abcd = _mm_unpacklo_epi64(abab, cdcd);

        return vec4x32u{abcd};
        #endif

        #if defined(AVEL_NEON)
        auto ret = vdupq_n_u32(0x00);
        switch (n) {
            default: ret = vsetq_lane_u32(ptr[extract<3>(indices)], ret, 0x3);
            case 3:  ret = vsetq_lane_u32(ptr[extract<2>(indices)], ret, 0x2);
            case 2:  ret = vsetq_lane_u32(ptr[extract<1>(indices)], ret, 0x1);
            case 1:  ret = vsetq_lane_u32(ptr[extract<0>(indices)], ret, 0x0);
            case 0:  ; //Do nothing
        }

        return vec4x32u{ret};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u gather<vec4x32u>(const std::uint32_t* ptr, vec4x32i indices) {
        #if defined(AVEL_AVX2)
        return vec4x32u{_mm_i32gather_epi32(reinterpret_cast<const int*>(ptr), decay(indices), sizeof(std::uint32_t))};

        #elif defined(AVEL_SSE2)
        auto a = _mm_cvtsi32_si128(ptr[extract<0>(indices)]);
        auto b = _mm_cvtsi32_si128(ptr[extract<1>(indices)]);
        auto c = _mm_cvtsi32_si128(ptr[extract<2>(indices)]);
        auto d = _mm_cvtsi32_si128(ptr[extract<3>(indices)]);

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
    AVEL_FINL vec4x32i gather<vec4x32i>(const std::int32_t* ptr, vec4x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        n = min(n, vec4x32i::width);
        auto mask = (1 << n) - 1;

        return vec4x32i{_mm_mmask_i32gather_epi32(_mm_setzero_si128(), mask, decay(indices), ptr, sizeof(std::uint32_t))};

        #elif defined(AVEL_AVX2)
        n = min(n, vec4x32i::width);

        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec4x32i::width;
        auto h = vec4x32i::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (h - min(h, n))));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (w - min(w, n))));
        auto mask = _mm_unpacklo_epi64(lo, hi);

        auto ret = _mm_mask_i32gather_epi32(
            _mm_setzero_si128(),
            ptr,
            decay(indices),
            mask,
            sizeof(std::int32_t)
        );

        return vec4x32i{ret};

        #elif defined(AVEL_SSE2)
        auto a = _mm_setzero_si128();
        auto b = _mm_setzero_si128();
        auto c = _mm_setzero_si128();
        auto d = _mm_setzero_si128();

        switch (n) {
            default: d = _mm_cvtsi32_si128(ptr[extract<3>(indices)]);
            case 3:  c = _mm_cvtsi32_si128(ptr[extract<2>(indices)]);
            case 2:  b = _mm_cvtsi32_si128(ptr[extract<1>(indices)]);
            case 1:  a = _mm_cvtsi32_si128(ptr[extract<0>(indices)]);
            case 0: ; //Do nothing
        }

        auto abab = _mm_unpacklo_epi32(a, b);
        auto cdcd = _mm_unpacklo_epi32(c, d);

        auto abcd = _mm_unpacklo_epi64(abab, cdcd);

        return vec4x32i{abcd};
        #endif

        #if defined(AVEL_NEON)
        auto ret = vdupq_n_s32(0x00);
        switch (n) {
            default: ret = vsetq_lane_s32(ptr[extract<3>(indices)], ret, 0x3);
            case 3:  ret = vsetq_lane_s32(ptr[extract<2>(indices)], ret, 0x2);
            case 2:  ret = vsetq_lane_s32(ptr[extract<1>(indices)], ret, 0x1);
            case 1:  ret = vsetq_lane_s32(ptr[extract<0>(indices)], ret, 0x0);
            case 0:  ; //Do nothing
        }

        return vec4x32i{ret};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32i gather<vec4x32i>(const std::int32_t* ptr, vec4x32i indices) {
        #if defined(AVEL_AVX2)
        return vec4x32i{_mm_i32gather_epi32(ptr, decay(indices), sizeof(std::int32_t))};

        #elif defined(AVEL_SSE2)
        auto i0 = extract<0>(indices);
        auto i1 = extract<1>(indices);
        auto i2 = extract<2>(indices);
        auto i3 = extract<3>(indices);

        auto a = _mm_cvtsi32_si128(ptr[i0]);
        auto b = _mm_cvtsi32_si128(ptr[i1]);
        auto c = _mm_cvtsi32_si128(ptr[i2]);
        auto d = _mm_cvtsi32_si128(ptr[i3]);

        auto abab = _mm_unpacklo_epi32(a, b);
        auto cdcd = _mm_unpacklo_epi32(c, d);

        auto abcd = _mm_unpacklo_epi64(abab, cdcd);
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



    AVEL_FINL void store(std::int32_t* ptr, vec4x32i x, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        n = min(n, vec4x32i::width);
        auto mask = (1 << n) - 1;
        _mm_mask_storeu_epi32(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        n = min(n, vec4x32i::width);
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec4x32i::width;
        auto h = vec4x32i::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (h - min(h, n))));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (w - min(w, n))));
        auto mask = _mm_unpacklo_epi64(lo, hi);
        _mm_maskmoveu_si128(decay(x), mask, reinterpret_cast<char *>(ptr));

        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            case 0: {

            } break;
            case 1: {
                std::int32_t x0 = vgetq_lane_s32(decay(x), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::int32_t));
            } break;
            case 2: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s32(decay(x)), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::int64_t));
            } break;
            case 3: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s32(decay(x)), 0);
                std::int32_t x1 = vgetq_lane_s32(decay(x), 2);

                std::memcpy(ptr + 0, &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 2, &x1, sizeof(std::int32_t));
            } break;
            default: {
                vst1q_s32(ptr, decay(x));
            }
        }
        #endif
    }

    template<std::uint32_t N = vec4x32i::width>
    AVEL_FINL void store(std::int32_t* ptr, vec4x32i x) {
        static_assert(N <= vec4x32i::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec4x32i::width, int>::type dummy_variable = 0;

        store(ptr, x, N);
    }

    template<>
    AVEL_FINL void store<vec4x32i::width>(std::int32_t* ptr, vec4x32i x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s32(ptr, decay(x));
        #endif
    }



    AVEL_FINL void aligned_store(std::int32_t* ptr, vec4x32i x, std::uint32_t n) {
        store(ptr, x, n);
    }

    template<std::uint32_t N = vec4x32i::width>
    AVEL_FINL void aligned_store(std::int32_t* ptr, vec4x32i x) {
        static_assert(N <= vec4x32i::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec4x32i::width, int>::type dummy_variable = 0;

        aligned_store(ptr, x, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec4x32i::width>(std::int32_t* ptr, vec4x32i x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s32(ptr, decay(x));
        #endif
    }



    AVEL_FINL void scatter(std::uint32_t* ptr, vec4x32u x, vec4x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        n = min(n, vec4x32u::width);
        auto mask = (1 << n) - 1;
        _mm_mask_i32scatter_epi32(ptr, mask, decay(indices), decay(x), sizeof(std::uint32_t));

        #elif defined(AVEL_SSE2)
        switch (n) {
            default: ptr[extract<3>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0xC));
            case 3:  ptr[extract<2>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0x8));
            case 2:  ptr[extract<1>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0x4));
            case 1:  ptr[extract<0>(indices)] = _mm_cvtsi128_si32(decay(x));
            case 0: ; //Do nothing
        }

        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            default: ptr[extract<3>(indices)] = vgetq_lane_u32(decay(x), 0x3);
            case 3:  ptr[extract<2>(indices)] = vgetq_lane_u32(decay(x), 0x2);
            case 2:  ptr[extract<1>(indices)] = vgetq_lane_u32(decay(x), 0x1);
            case 1:  ptr[extract<0>(indices)] = vgetq_lane_u32(decay(x), 0x0);
            case 0: ; //Do nothing
        }

        #endif
    }

    template<std::uint32_t N = vec4x32u::width>
    AVEL_FINL void scatter(std::uint32_t* ptr, vec4x32u x, vec4x32i indices) {
        scatter(ptr, x, indices, N);
    }

    template<>
    AVEL_FINL void scatter<vec4x32u::width>(std::uint32_t* ptr, vec4x32u x, vec4x32i indices) {
        #if defined(AVEL_AVX512VL)
        _mm_i32scatter_epi32(ptr, decay(indices), decay(x), sizeof(std::uint32_t));

        #elif defined(AVEL_SSE2)
        ptr[extract<0>(indices)] = _mm_cvtsi128_si32(decay(x));
        ptr[extract<1>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0x4));
        ptr[extract<2>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0x8));
        ptr[extract<3>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0xC));

        #endif

        #if defined(AVEL_NEON)
        ptr[extract<0>(indices)] = vgetq_lane_u32(decay(x), 0x0);
        ptr[extract<1>(indices)] = vgetq_lane_u32(decay(x), 0x1);
        ptr[extract<2>(indices)] = vgetq_lane_u32(decay(x), 0x2);
        ptr[extract<3>(indices)] = vgetq_lane_u32(decay(x), 0x3);

        #endif
    }



    AVEL_FINL void scatter(std::int32_t* ptr, vec4x32i x, vec4x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        n = min(n, vec4x32i::width);
        auto mask = (1 << n) - 1;
        _mm_mask_i32scatter_epi32(ptr, mask, decay(indices), decay(x), sizeof(std::int32_t));

        #elif defined(AVEL_SSE2)
        switch (n) {
            default: ptr[extract<3>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0xC));
            case 3:  ptr[extract<2>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0x8));
            case 2:  ptr[extract<1>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0x4));
            case 1:  ptr[extract<0>(indices)] = _mm_cvtsi128_si32(decay(x));
            case 0: ; //Do nothing
        }

        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            default: ptr[extract<3>(indices)] = vgetq_lane_s32(decay(x), 0x3);
            case 3:  ptr[extract<2>(indices)] = vgetq_lane_s32(decay(x), 0x2);
            case 2:  ptr[extract<1>(indices)] = vgetq_lane_s32(decay(x), 0x1);
            case 1:  ptr[extract<0>(indices)] = vgetq_lane_s32(decay(x), 0x0);
            case 0: ; //Do nothing
        }

        #endif
    }

    template<std::uint32_t N = vec4x32i::width>
    AVEL_FINL void scatter(std::int32_t* ptr, vec4x32i x, vec4x32i indices) {
        scatter(ptr, x, indices, N);
    }

    template<>
    AVEL_FINL void scatter<vec4x32i::width>(std::int32_t* ptr, vec4x32i x, vec4x32i indices) {
        #if defined(AVEL_AVX512VL)
        _mm_i32scatter_epi32(ptr, decay(indices), decay(x), sizeof(std::int32_t));

        #elif defined(AVEL_SSE2)
        ptr[extract<0>(indices)] = _mm_cvtsi128_si32(decay(x));
        ptr[extract<1>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0x4));
        ptr[extract<2>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0x8));
        ptr[extract<3>(indices)] = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(x), 0xC));

        #endif

        #if defined(AVEL_NEON)
        ptr[extract<0>(indices)] = vgetq_lane_s32(decay(x), 0x0);
        ptr[extract<1>(indices)] = vgetq_lane_s32(decay(x), 0x1);
        ptr[extract<2>(indices)] = vgetq_lane_s32(decay(x), 0x2);
        ptr[extract<3>(indices)] = vgetq_lane_s32(decay(x), 0x3);

        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr4x32i to_array(vec4x32i v) {
        alignas(16) arr4x32i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec4x32i> div(vec4x32i x, vec4x32i y) {
        mask4x32i remainder_sign_mask = (x < vec4x32i{0x00});
        mask4x32i quotient_sign_mask = remainder_sign_mask ^ (y < vec4x32i{0x00});

        vec4x32u numerator{abs(x)};
        vec4x32u denominator{abs(y)};

        auto results = div(numerator, denominator);

        return {
            negate(quotient_sign_mask,  vec4x32i{results.quot}),
            negate(remainder_sign_mask, vec4x32i{results.rem})
        };
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

}

#endif
