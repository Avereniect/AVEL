#ifndef AVEL_VEC8X16I_HPP
#define AVEL_VEC8X16I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x16i = Vector<std::int16_t, 8>;
    using arr8x16i = std::array<std::int16_t, 8>;
    using mask8x16i = Vector_mask<std::int16_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x16i> div(vec8x16i numerator, vec8x16i denominator);
    vec8x16i broadcast_mask(mask8x16i m);
    vec8x16i blend(mask8x16i m, vec8x16i a, vec8x16i b);
    vec8x16i negate(mask8x16i m, vec8x16i x);






    template<>
    class Vector_mask<std::int16_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint16x8_t;
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
            content(b ? 0xFF : 0x00) {}
        #elif defined(AVEL_SSE2)
            content(b ? _mm_set1_epi16(-1) : _mm_setzero_si128()) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u16(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr8xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si64(arr.data());
            content = static_cast<primitive>(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_SSE2)
            auto array_data = _mm_loadu_si64(arr.data());

            array_data = _mm_unpacklo_epi8(array_data, array_data);
            content = _mm_cmplt_epi16(_mm_setzero_si128(), array_data);

            #endif

            #if defined(AVEL_NEON)
            auto array_data = vld1_u8(bit_cast<const std::uint8_t*>(arr.data()));
            auto mask = vclt_u8(vdup_n_u8(0x00), array_data);
            auto widened_mask = vcombine_u8(mask, mask);
            content = vreinterpretq_u16_u8(vzipq_u8(widened_mask, widened_mask).val[0]);
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
            *this = Vector_mask{b};
            return *this;
        }

        AVEL_FINL Vector_mask& operator=(primitive p) {
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
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u16(vceqq_u16(decay(lhs), decay(rhs)));
            return min == 0xFFFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u8(vreinterpretq_u8_u16(vceqq_u16(decay(lhs), decay(rhs))));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(vreinterpretq_u64_u16(decay(lhs)), vreinterpretq_u64_u16(decay(rhs)));
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
            content = _kand_mask16(content, rhs.content);

            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = vandq_u16(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _kor_mask16(content, rhs.content);

            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u16(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _kand_mask16(_kxor_mask16(content, rhs.content), _cvtu32_mask16(0xFF));

            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = veorq_u16(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector_mask{primitive(_knot_mask16(content))};

            #elif defined(AVEL_AVX512VL)
            return Vector_mask{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi16(undef, undef))};

            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vmvnq_u16(content)};
            #endif
        }

        AVEL_VECTOR_MASK_BINARY_BITWISE_OPERATORS

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
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask8x16i>(mask8x16i m) {
        return {mask8x16u{mask8x16u::primitive(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask8x16u>(mask8x16u m) {
        return {mask8x16i{mask8x16i::primitive(decay(m))}};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask8x16i m) {
        return count(mask8x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x16i m) {
        return any(mask8x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask8x16i m) {
        return all(mask8x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x16i m) {
        return none(mask8x16u{m});
    }






    template<>
    class alignas(16) Vector<std::int16_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int16_t;

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int16x8_t;
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

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> x):
            Vector(convert<Vector>(x)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content(_mm_maskz_set1_epi16(decay(m), 0x1)) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi16(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vnegq_s16(vreinterpretq_s16_u16(decay(m)))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi16(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_s16(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr8x16i& arr):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_s16(array.data()) {}
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
        Vector& operator=(Vector&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpeq_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_s16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpneq_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpeq_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmplt_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_s16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{mask::primitive(_mm_cmple_epi16_mask(lhs.content, rhs.content))};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpgt_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_s16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpgt_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_s16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpge_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmplt_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_s16(decay(lhs), decay(rhs))};
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
            #if defined(AVEL_SSE2)
            content = _mm_add_epi16(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_s16(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi16(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_s16(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_mullo_epi16(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vmulq_s16(content, decay(rhs));
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

        AVEL_VECTOR_ARITHMETIC_OPERATORS

        //=================================================
        // Increment/Decrement operators
        //=================================================

        AVEL_VECTOR_INCREMENT_DECREMENT_OPERATORS

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_s16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_s16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_s16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_SSE2)
            content  = _mm_sll_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s16(content, vdupq_n_s16(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sra_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s16(content, vdupq_n_s16(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm_sllv_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto t0 = _mm256_cvtepu16_epi32(content);
            auto t1 = _mm256_cvtepu16_epi32(decay(rhs));

            auto t2 = _mm256_and_si256(_mm256_sllv_epi32(t0, t1), _mm256_set1_epi32(0x0000FFFF));

            auto lo = t2;
            auto hi = _mm256_permute2f128_si256(lo, lo, 0x01);
            content = _mm256_castsi256_si128(_mm256_packus_epi32(lo, hi));

            //TODO: Optimize for other instruction sets

            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i < 4; ++i) {
                auto t0 = _mm_and_si128(rhs.content, _mm_set1_epi16(1u << i));
                auto m = _mm_cmplt_epi16(_mm_setzero_si128(), t0);

                auto a = _mm_andnot_si128(m, content);
                auto b = _mm_and_si128(m, _mm_sll_epi16(content, _mm_cvtsi64_si128(1u << i)));
                content = _mm_or_si128(a, b);
            }

            auto m = _mm_cmplt_epi16(rhs.content, _mm_set1_epi16(16));
            content = _mm_and_si128(content, m);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s16(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm_srav_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto t0 = _mm256_cvtepi16_epi32(content);
            auto t1 = _mm256_cvtepi16_epi32(decay(rhs));

            auto t2 = _mm256_and_si256(_mm256_srav_epi32(t0, t1), _mm256_set1_epi32(0x0000FFFF));

            auto lo = t2;
            auto hi = _mm256_permute2f128_si256(lo, lo, 0x01);
            content = _mm256_castsi256_si128(_mm256_packus_epi32(lo, hi));

            #elif defined(AVEL_SSE41)
            auto threshold = _mm_set1_epi16(8);

            //TODO: manually unroll loop
            for (unsigned i = 4; i-- > 0; ) {
                auto m = _mm_cmplt_epi16(decay(rhs), threshold);

                auto a = _mm_sra_epi16(content, _mm_cvtsi64_si128(1u << i));
                auto b = content;

                content = _mm_blendv_epi8(a, b, m);

                rhs = _mm_sub_epi16(decay(rhs), _mm_andnot_si128(m, threshold));
                threshold = _mm_srai_epi16(threshold, 0x1);
            }

            //TODO: Consider implementation using pshufb and 32-bit
            // multiplication

            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto negative_mask = _mm_cmplt_epi16(content, zeros);
            content = _mm_xor_si128(content, negative_mask);

            auto non_zero_mask = _mm_cmplt_epi16(decay(rhs), _mm_set1_epi16(16));
            content = _mm_and_si128(content, non_zero_mask);

            auto c0 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 12), zeros);
            content = _mm_max_epi16(_mm_andnot_si128(c0, content), _mm_srli_epi16(content, 8));

            auto c1 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 13), zeros);
            content = _mm_max_epi16(_mm_andnot_si128(c1, content), _mm_srli_epi16(content, 4));

            auto c2 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 14), zeros);
            content = _mm_max_epi16(_mm_andnot_si128(c2, content), _mm_srli_epi16(content, 2));

            auto c3 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 15), zeros);
            content = _mm_max_epi16(_mm_andnot_si128(c3, content), _mm_srli_epi16(content, 1));


            content = _mm_xor_si128(content, negative_mask);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s16(content, vnegq_s16(decay(rhs)));

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
            return Vector{vmvnq_s16(content)};
            #endif
        }

        AVEL_VECTOR_BINARY_BITWISE_OPERATORS

        //=================================================
        // Conversion operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_test_epi16_mask(content, content)};

            #elif defined(AVEL_SSE2)
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        8 * sizeof(std::int16_t) == sizeof(vec8x16i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec8x16i>(vec8x16i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{decay(x)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec8x16u{vreinterpretq_u16_s16(decay(x))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec8x16u>(vec8x16u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{decay(x)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec8x16i{vreinterpretq_s16_u16(decay(x))}};
        #endif
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec8x16i operator-(vec8x16u v) {
        return vec8x16i{0x00} - vec8x16i{v};
    }

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL std::int16_t extract(vec8x16i v) {
        static_assert(N < vec8x16i::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x16i::width, int>::type dummy_variable = 0;

        return static_cast<std::int16_t>(extract<N>(vec8x16u{v}));
    }

    template<std::uint32_t N>
    AVEL_FINL vec8x16i insert(vec8x16i v, std::int16_t x) {
        static_assert(N < vec8x16i::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x16i::width, int>::type dummy_variable = 0;

        return vec8x16i{insert<N>(vec8x16u{v}, static_cast<std::uint16_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x16i bit_shift_left(vec8x16i v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        return vec8x16i{bit_shift_left<S>(vec8x16u{v})};
    }

    template<std::uint32_t S>
    AVEL_FINL vec8x16i bit_shift_right(vec8x16i v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec8x16i{_mm_srai_epi16(decay(v), S)};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16i{vshrq_n_s16(decay(v), S)};
        #endif
    }

    template<>
    AVEL_FINL vec8x16i bit_shift_right<0>(vec8x16i v) {
        return v;
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x16i rotl(vec8x16i v) {
        return vec8x16i{rotl<S>(vec8x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotl(vec8x16i v, long long s) {
        return vec8x16i{rotl(vec8x16u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotl(vec8x16i v, vec8x16i s) {
        return vec8x16i{rotl(vec8x16u{v}, vec8x16u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x16i rotr(vec8x16i v) {
        return vec8x16i{rotr<S>(vec8x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotr(vec8x16i v, long long s) {
        return vec8x16i{rotr(vec8x16u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotr(vec8x16i v, vec8x16i s) {
        return vec8x16i{rotr(vec8x16u{v}, vec8x16u{s})};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec8x16i v) {
        return count(vec8x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec8x16i v) {
        return any(vec8x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec8x16i v) {
        return all(vec8x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec8x16i v) {
        return none(vec8x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i broadcast_mask(mask8x16i m) {
        return vec8x16i{broadcast_mask(mask8x16u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i keep(mask8x16i m, vec8x16i v) {
        return vec8x16i{keep(mask8x16u{m}, vec8x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i clear(mask8x16i m, vec8x16i v) {
        return vec8x16i{clear(mask8x16u{m}, vec8x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i blend(mask8x16i m, vec8x16i a, vec8x16i b) {
        return vec8x16i{blend(mask8x16u{m}, vec8x16u{a}, vec8x16u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i byteswap(vec8x16i v) {
        return vec8x16i{byteswap(vec8x16u(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i max(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_SSE41)
        return vec8x16i{_mm_max_epi16(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec8x16i{vmaxq_s16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i min(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_SSE41)
        return vec8x16i{_mm_min_epi16(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(b < a, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec8x16i{vminq_s16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 2> minmax(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_SSE41)
        return std::array<vec8x16i, 2>{
            vec8x16i{_mm_min_epi16(decay(a), decay(b))},
            vec8x16i{_mm_max_epi16(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto m = b < a;
        return {blend(m, b, a), blend(m, a, b)};

        #endif

        #if defined(AVEL_NEON)
        return {
            vec8x16i{vminq_s16(decay(a), decay(b))},
            vec8x16i{vmaxq_s16(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i clamp(vec8x16i x, vec8x16i lo, vec8x16i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i average(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_SSE2)
        auto avg = (a & b) + ((a ^ b) >> 1);
        auto c = broadcast_mask((a < -b) | (b == vec8x16i{std::int16_t(1 << 15)})) & (a ^ b) & vec8x16i{1};
        return avg + c;

        #endif

        #if defined(AVEL_NEON)
        auto avg = vec8x16i{vhaddq_s16(decay(x), decay(y))};
        auto c = broadcast_mask((x < -y) | (y == vec8x16i{std::int16_t(1 << 15)})) & (x ^ y) & vec8x16i{1};

        return avg + c;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i midpoint(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto offset = _mm_set1_epi16(0x8000);

        auto a_offset = _mm_xor_si128(decay(a), offset);
        auto b_offset = _mm_xor_si128(decay(b), offset);

        auto average_offset = _mm_avg_epu16(a_offset, b_offset);
        auto average = _mm_xor_si128(average_offset, offset);

        auto bias = _mm_ternarylogic_epi32(decay(a), decay(b), _mm_set1_epi16(0x1), 0x28);
        auto mask = _mm_cmplt_epi16_mask(decay(a), decay(b));
        auto ret = _mm_mask_sub_epi16(average, mask, average, bias);

        return vec8x16i{ret};

        #elif defined(AVEL_SSE2)
        auto offset = _mm_set1_epi16(0x8000);

        auto a_offset = _mm_xor_si128(decay(a), offset);
        auto b_offset = _mm_xor_si128(decay(b), offset);

        auto average_offset = _mm_avg_epu16(a_offset, b_offset);
        auto average = _mm_xor_si128(average_offset, offset);

        auto m = _mm_cmplt_epi16(decay(a), decay(b));
        auto bias = _mm_and_si128(_mm_xor_si128(decay(a), decay(b)), _mm_and_si128(m, _mm_set1_epi16(0x01)));

        return vec8x16i{_mm_sub_epi16(average, bias)};
        #endif

        #if defined(AVEL_NEON)
        vec8x16i t0 = vec8x16i{vhaddq_s16(decay(a), decay(b))};
        vec8x16i t1 = (a ^ b) & vec8x16i{0x1} & broadcast_mask(b < a);
        return t0 + t1;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i negate(mask8x16i m, vec8x16i v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec8x16i{_mm_mask_sub_epi16(decay(v), decay(m), _mm_setzero_si128(), decay(v))};

        #elif defined(AVEL_SSSE3)
        return vec8x16i{_mm_sign_epi16(decay(v), _mm_or_si128(decay(m), _mm_set1_epi16(0x01)))};
        #endif

        #if defined(AVEL_SSE2) || defined(AVEL_NEON)
        auto mask = broadcast_mask(m);
        return (v ^ mask) - mask;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i abs(vec8x16i v) {
        #if defined(AVEL_SSSE3)
        return vec8x16i{_mm_abs_epi16(decay(v))};

        #elif defined(AVEL_SSE2)
        auto y = v >> 15;
        return (v ^ y) - y;

        #endif

        #if defined(AVEL_NEON)
        auto zeros = vdupq_n_s16(0x00);
        return vec8x16i{vabdq_s16(decay(v), zeros)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i neg_abs(vec8x16i v) {
        #if defined(AVEL_SSSE3)
        return -vec8x16i{_mm_abs_epi16(decay(v))};

        #elif defined(AVEL_SSE2)
        auto y = ~(v >> 15);
        return (v ^ y) - y;

        #endif

        #if defined(AVEL_NEON)
        auto y = vmvnq_s16(vshrq_n_s16(decay(v), 15));
        return vec8x16i{vsubq_s16(veorq_s16(decay(v), y), y)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i neg_abs(vec8x16u v) {
        #if defined(AVEL_SSSE3)
        return -vec8x16i{_mm_abs_epi16(decay(v))};

        #elif defined(AVEL_SSE2)
        auto z = vec8x16i(v);
        auto y = ~z >> 15;
        return (z ^ y) - y;

        #endif

        #if defined(AVEL_NEON)
        auto y = vmvnq_s16(vshrq_n_s16(vreinterpretq_s16_u16(decay(v)), 15));
        return vec8x16i{vsubq_s16(veorq_s16(vreinterpretq_s16_u16(decay(v)), y), y)};
        #endif
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16i load<vec8x16i>(const std::int16_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = (n >= 8) ? -1 : (1 << n)  - 1;
        return vec8x16i{_mm_maskz_loadu_epi16(mask, ptr)};

        #elif defined(AVEL_SSE2)
        // GCC 9 doesn't have _mm_loadu_si16(), _mm_loadu_si32(), or
        // _mm_loadu_si64() but the following code is optimized down to
        // reasonable machine code.
        switch (n) {
            case 0: {
                return vec8x16i{_mm_setzero_si128()};
            }
            case 1: {
                std::uint16_t one_s;
                std::memcpy(&one_s, ptr + 0, sizeof(std::uint16_t));
                return vec8x16i{_mm_cvtsi32_si128(one_s)};
            }
            case 2: {
                std::uint32_t two_s;
                std::memcpy(&two_s, ptr + 0, sizeof(std::uint32_t));
                return vec8x16i{_mm_cvtsi32_si128(two_s)};
            }
            case 3: {
                std::uint32_t two_s;
                std::memcpy(&two_s, ptr + 0, sizeof(std::uint32_t));

                std::uint16_t one_s;
                std::memcpy(&one_s, ptr + 2, sizeof(std::uint16_t));

                auto two_v = _mm_cvtsi32_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec8x16i{_mm_unpacklo_epi32(two_v, one_v)};
            }

            case 4: {
                std::uint64_t four_s;
                std::memcpy(&four_s, ptr + 0, sizeof(std::uint64_t));

                auto four_v = _mm_cvtsi64_si128(four_s);
                return vec8x16i{four_v};
            }
            case 5: {
                std::uint64_t four_s;
                std::memcpy(&four_s, ptr + 0, sizeof(std::uint64_t));

                std::uint16_t one_s;
                std::memcpy(&one_s, ptr + 4, sizeof(std::uint16_t));

                auto four_v = _mm_cvtsi64_si128(four_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec8x16i{_mm_unpacklo_epi64(four_v, one_v)};
            }
            case 6: {
                std::uint64_t four_s;
                std::memcpy(&four_s, ptr + 0, sizeof(std::uint64_t));

                std::uint32_t two_s;
                std::memcpy(&two_s, ptr + 4, sizeof(std::uint32_t));

                auto four_v = _mm_cvtsi64_si128(four_s);
                auto two_v = _mm_cvtsi32_si128(two_s);

                return vec8x16i{_mm_unpacklo_epi64(four_v, two_v)};
            }
            case 7: {
                std::uint64_t four_s ;
                std::memcpy(&four_s, ptr + 0, sizeof(std::uint64_t));

                std::uint32_t two_s;
                std::memcpy(&two_s, ptr + 4, sizeof(std::uint32_t));

                std::uint16_t one_s;
                std::memcpy(&one_s, ptr + 6, sizeof(std::uint16_t));

                auto four_v = _mm_cvtsi64_si128(four_s);
                auto two_v = _mm_cvtsi32_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec8x16i{_mm_unpacklo_epi64(four_v, _mm_unpacklo_epi32(two_v, one_v))};
            }

            default: {
                return vec8x16i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
            }
        }
        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            case 0: {
                return vec8x16i{vdupq_n_s16(0x00)};
            }
            case 1: {
                std::uint16_t x0;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint16_t));

                auto ret0 = vsetq_lane_s16(x0, vdupq_n_s16(0x00), 0);
                return vec8x16i{ret0};
            }
            case 2: {
                std::uint32_t x0;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint32_t));

                auto ret0 = vsetq_lane_s32(x0, vdupq_n_s32(0x00), 0);
                auto ret1 = vreinterpretq_s16_s32(ret0);
                return vec8x16i{ret1};
            }
            case 3: {
                std::uint32_t x0;
                std::uint16_t x1;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint32_t));
                std::memcpy(&x1, ptr + 2, sizeof(std::uint16_t));

                auto ret0 = vsetq_lane_s32(x0, vdupq_n_s32(0x00), 0);
                auto ret1 = vsetq_lane_s16(x1, vreinterpretq_s16_s32(ret0), 2);
                return vec8x16i{ret1};
            }
            case 4: {
                std::uint64_t x0;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vreinterpretq_s16_s64(ret0);
                return vec8x16i{ret1};
            }

            case 5: {
                std::uint64_t x0;
                std::uint16_t x1;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));
                std::memcpy(&x1, ptr + 4, sizeof(std::uint16_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s16(x1, vreinterpretq_s16_s64(ret0), 4);
                return vec8x16i{ret1};
            }
            case 6: {
                std::uint64_t x0;
                std::uint32_t x1;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));
                std::memcpy(&x1, ptr + 4, sizeof(std::uint32_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s32(x1, vreinterpretq_s32_s64(ret0), 2);
                auto ret2 = vreinterpretq_s16_s32(ret1);
                return vec8x16i{ret2};
            }
            case 7: {
                std::uint64_t x0;
                std::uint32_t x1;
                std::uint16_t x2;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));
                std::memcpy(&x1, ptr + 4, sizeof(std::uint32_t));
                std::memcpy(&x2, ptr + 6, sizeof(std::uint16_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s32(x1, vreinterpretq_s32_s64(ret0), 2);
                auto ret2 = vsetq_lane_s16(x2, vreinterpretq_s16_s32(ret1), 6);
                return vec8x16i{ret2};
            }

            default: {
                return vec8x16i{vld1q_s16(ptr)};
            }
        }
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16i load<vec8x16i, vec8x16i::width>(const std::int16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16i{vld1q_s16(ptr)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16i aligned_load<vec8x16i>(const std::int16_t* ptr, std::uint32_t n) {
        return load<vec8x16i>(ptr, n);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16i aligned_load<vec8x16i>(const std::int16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Utilize __builtin_assume_aligned on GCC and Clang
        //TODO: Utilize assume_aligned if C++ 20 is available
        return vec8x16i{vld1q_s16(ptr)};
        #endif
    }



    AVEL_FINL void store(std::int16_t* ptr, vec8x16i v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        n = min(n, vec8x16i::width);
        auto mask = (1 << n) - 1;
        _mm_mask_storeu_epi16(ptr, mask, decay(v));

        #elif defined(AVEL_SSE2)
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec8x16u::width;
        auto h = vec8x16u::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(16 * (h - min(h, n))));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(16 * (w - min(w, n))));
        auto mask = _mm_unpacklo_epi64(lo, hi);
        _mm_maskmoveu_si128(decay(v), mask, reinterpret_cast<char *>(ptr));
        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            case 0: {} break;
            case 1: {
                std::int16_t x0 = vgetq_lane_s16(decay(v), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::int16_t));
            } break;
            case 2: {
                std::int32_t x0 = vgetq_lane_s32(vreinterpretq_s32_s16(decay(v)), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::int32_t));
            } break;
            case 3: {
                std::int32_t x0 = vgetq_lane_s32(vreinterpretq_s32_s16(decay(v)), 0);
                std::int16_t x1 = vgetq_lane_s16(decay(v), 2);

                std::memcpy(ptr + 0, &x0, sizeof(std::int32_t));
                std::memcpy(ptr + 2, &x1, sizeof(std::int16_t));
            } break;

            case 4: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s16(decay(v)), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::int64_t));
            } break;
            case 5: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s16(decay(v)), 0);
                std::int16_t x1 = vgetq_lane_s16(decay(v), 4);

                std::memcpy(ptr + 0, &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 4, &x1, sizeof(std::int16_t));
            } break;
            case 6: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s16(decay(v)), 0);
                std::int32_t x1 = vgetq_lane_s32(vreinterpretq_s32_s16(decay(v)), 2);

                std::memcpy(ptr + 0, &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 4, &x1, sizeof(std::int32_t));
            } break;
            case 7: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s16(decay(v)), 0);
                std::int32_t x1 = vgetq_lane_s32(vreinterpretq_s32_s16(decay(v)), 2);
                std::int16_t x2 = vgetq_lane_s16(decay(v), 6);

                std::memcpy(ptr + 0, &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 4, &x1, sizeof(std::int32_t));
                std::memcpy(ptr + 6, &x2, sizeof(std::int16_t));
            } break;
            default: {
                vst1q_s16(ptr, decay(v));
            }
        }
        #endif
    }

    template<std::uint32_t N = vec8x16i::width>
    AVEL_FINL void store(std::int16_t* ptr, vec8x16i v) {
        static_assert(N <= vec8x16i::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec8x16i::width, int>::type dummy_variable = 0;

        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec8x16i::width>(std::int16_t* ptr, vec8x16i v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s16(ptr, decay(v));
        #endif
    }


    AVEL_FINL void aligned_store(std::int16_t* ptr, vec8x16i v, std::uint32_t n) {
        store(ptr, v, n);
    }

    template<std::uint32_t N = vec8x16i::width>
    AVEL_FINL void aligned_store(std::int16_t* ptr, vec8x16i v) {
        static_assert(N <= vec8x16i::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec8x16i::width, int>::type dummy_variable = 0;

        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec8x16i::width>(std::int16_t* ptr, vec8x16i v) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s16(ptr, decay(v));
        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr8x16i to_array(vec8x16i v) {
        alignas(16) arr8x16i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x16i> div(vec8x16i x, vec8x16i y) {
        mask8x16i remainder_sign_mask = (x < vec8x16i{0x00});
        mask8x16i quotient_sign_mask = remainder_sign_mask ^ (y < vec8x16i{0x00});

        vec8x16u numerator{abs(x)};
        vec8x16u denominator{abs(y)};

        auto results = div(numerator, denominator);

        return {
            negate(quotient_sign_mask,  vec8x16i{results.quot}),
            negate(remainder_sign_mask, vec8x16i{results.rem})
        };
    }

    AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(vec8x16i, mask8x16i, vec8x16u)

}

#endif //AVEL_VEC8x16I_HPP
