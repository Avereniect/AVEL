#ifndef AVEL_VEC16X8I_HPP
#define AVEL_VEC16X8I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x8i = Vector<std::int8_t, 16>;
    using arr16x8i = std::array<std::int8_t, 16>;
    using mask16x8i = Vector_mask<std::int8_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x8i> div(vec16x8i x, vec16x8i y);
    vec16x8i set_bits(mask16x8i m);
    vec16x8i blend(mask16x8i m, vec16x8i a, vec16x8i b);
    vec16x8i negate(mask16x8i m, vec16x8i x);





    template<>
    class Vector_mask<std::int8_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        static constexpr std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
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
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
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

            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = primitive(_mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data));

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
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);

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
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);

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
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
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
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
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
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
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
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
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
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask16x8i>(mask16x8i m) {
        return {mask16x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask16x8u>(mask16x8u m) {
        return {mask16x8i(decay(m))};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask16x8i m) {
        return count(mask16x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask16x8i m) {
        return any(mask16x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask16x8i m) {
        return all(mask16x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x8i m) {
        return none(mask16x8u{m});
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL bool extract(mask16x8i m) {
        static_assert(N < mask16x8i::width, "Specified index does not exist");
        typename std::enable_if<N < mask16x8i::width, int>::type dummy_variable = 0;

        return extract<N>(mask16x8u{m});
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL mask16x8i insert(mask16x8i m, bool b) {
        static_assert(N < mask16x8i::width, "Specified index does not exist");
        typename std::enable_if<N < mask16x8i::width, int>::type dummy_variable = 0;

        return mask16x8i{insert<N>(mask16x8u{m}, b)};
    }






    template<>
    class alignas(16) Vector<std::int8_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int8_t;

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int8x16_t;
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
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            content(_mm_maskz_set1_epi8(decay(m), 0x1)) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi8(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vreinterpretq_s8_u8(vsubq_u8(vdupq_n_u8(0x00), decay(m)))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            content(_mm_broadcastb_epi8(_mm_cvtsi32_si128(x))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_set1_epi8(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_s8(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr16x8i& array):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_s8(array.data())) {}
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
            content = vdupq_n_s8(x);
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
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm_cmpeq_epi8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi8(decay(lhs), decay(rhs))};

            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_s8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
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
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm_cmplt_epi8_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_epi8(lhs.content, rhs.content)};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_s8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm_cmple_epi8_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpgt_epi8(lhs.content, rhs.content)};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_s8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm_cmpgt_epi8_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_epi8(lhs.content, rhs.content)};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_s8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm_cmpge_epi8_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmplt_epi8(lhs.content, rhs.content)};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_s8(decay(lhs), decay(rhs))};
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
            #if defined(AVEL_SSE2)
            return Vector{0x00} - *this;
            #endif

            #if defined(AVEL_NEON)
            return Vector{vnegq_s8(content)};
            #endif
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi8(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_s8(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi8(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_s8(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto a = _mm256_cvtepu8_epi16(content);
            auto b = _mm256_cvtepu8_epi16(rhs.content);

            auto c = _mm256_mullo_epi16(a, b);
            content = _mm256_cvtepi16_epi8(c);

            #elif defined(AVEL_SSE4_1)
            auto even_mask = _mm_set1_epi16(0x00FF);

            auto products_even = _mm_mullo_epi16(content, decay(rhs));
            auto products_odd  = _mm_mullo_epi16(
                _mm_srli_epi16(content, 8),
                _mm_andnot_si128(even_mask, decay(rhs))
            );

            auto products = _mm_blendv_epi8(products_odd, products_even, even_mask);
            content = products;

            #elif defined(AVEL_SSE2)
            auto even_mask = _mm_set1_epi16(0x00FF);

            auto products_even = _mm_and_si128(even_mask, _mm_mullo_epi16(content, decay(rhs)));
            auto products_odd  = _mm_mullo_epi16(
                _mm_srli_epi16(content, 8),
                _mm_andnot_si128(even_mask, decay(rhs))
            );

            auto products = _mm_or_si128(products_odd, products_even);
            content = products;

            #endif

            #if defined(AVEL_NEON)
            content = vmulq_s8(content, decay(rhs));

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
            content = vandq_s8(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_s8(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_s8(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
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
            auto s = (std::uint64_t(rhs) > 0xFF) ? 0xFF : static_cast<std::uint8_t>(rhs);
            *this = vshlq_s8(decay(*this), vdupq_n_s8(s));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto tmp = _mm256_cvtepi8_epi16(content);
            tmp = _mm256_sra_epi16(tmp, _mm_cvtsi32_si128(rhs));
            content = _mm256_cvtepi16_epi8(tmp);

            #elif defined(AVEL_AVX2)
            auto tmp = _mm256_cvtepi8_epi16(content);
            tmp = _mm256_sra_epi16(tmp, _mm_cvtsi32_si128(rhs));

            auto lo = _mm256_castsi256_si128(tmp);
            auto hi = _mm256_extractf128_si256(tmp, 0x1);

            content = _mm_packs_epi16(lo, hi);

            #elif defined(AVEL_SSE2)
            auto sign_bits = _mm_cmplt_epi8(content, _mm_setzero_si128());
            auto lo = _mm_unpacklo_epi8(content, sign_bits);
            auto hi = _mm_unpackhi_epi8(content, sign_bits);

            auto s = _mm_cvtsi32_si128(rhs);
            auto lo_shifted = _mm_sra_epi16(lo, s);
            auto hi_shifted = _mm_sra_epi16(hi, s);

            auto result = _mm_packs_epi16(lo_shifted, hi_shifted);

            content = result;

            #endif

            #if defined(AVEL_NEON)
            content =  vshlq_s8(content, vdupq_n_s8(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
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
            //TODO: Benchmark against divide and conquer approach
            alignas(16) static constexpr std::uint8_t table_data[16] {
                0x01, 0x02, 0x04, 0x08,
                0x10, 0x20, 0x40, 0x80,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00
            };

            auto table = _mm_load_si128(reinterpret_cast<const primitive*>(table_data));
            auto shifts = _mm_shuffle_epi8(table, decay(rhs));

            *this *= vec16x8i{shifts};

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
            content = vshlq_s8(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto whole = _mm256_cvtepi8_epi16(content);
            auto shifts = _mm256_cvtepi8_epi16(rhs.content);
            auto shifted = _mm256_srav_epi16(whole, shifts);
            content = _mm256_cvtepi16_epi8(shifted);

            //TODO: Offer AVX2 version?

            #elif defined(AVEL_SSE4_1)
            //TODO: Levaerage inverting negative values like is done in SSE2
            // implementation
            auto zeros = _mm_setzero_si128();
            auto non_zero_mask = _mm_cmplt_epi8(decay(rhs), _mm_set1_epi8(8));
            auto neg_mask = _mm_cmplt_epi8(content, zeros);
            auto preserve_mask = _mm_set1_epi16(0x00FF);

            //TODO: Manually unroll loop
            for (unsigned i = 0; i < 3; ++i) {
                auto t0 = _mm_and_si128(decay(rhs), _mm_set1_epi8(1u << i));
                auto m = _mm_cmplt_epi8(zeros, t0);

                auto s = _mm_cvtsi64_si128(1u << i);
                preserve_mask = _mm_blendv_epi8(preserve_mask, _mm_srl_epi16(preserve_mask, s), m);
                content = _mm_blendv_epi8(content, _mm_sra_epi16(content, s), m);
            }

            preserve_mask = _mm_or_si128(preserve_mask, _mm_set1_epi16(0xFF00));
            preserve_mask = _mm_and_si128(non_zero_mask, preserve_mask);

            auto invert_mask = _mm_xor_si128(content, neg_mask);
            auto flip_mask = _mm_andnot_si128(preserve_mask, invert_mask);

            content = _mm_xor_si128(content, flip_mask);
            content = _mm_and_si128(content, _mm_or_si128(non_zero_mask, neg_mask));

            #elif defined(AVEL_SSSE3)
            //TODO: Benchmark against divide and conquer approaches
            alignas(16) static constexpr std::uint8_t table_data[16] {
                0x80, 0x40, 0x20, 0x10,
                0x08, 0x04, 0x02, 0x01,
                0x01, 0x01, 0x01, 0x01,
                0x01, 0x01, 0x01, 0x01
            };

            auto lo = _mm_unpacklo_epi8(content, content);
            auto hi = _mm_unpackhi_epi8(content, content);

            lo = _mm_srai_epi16(lo, 8);
            hi = _mm_srai_epi16(hi, 8);

            auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
            auto shift_factors = _mm_shuffle_epi8(table, decay(rhs));

            auto zeros = _mm_setzero_si128();
            auto shift_factors_lo = _mm_unpacklo_epi8(shift_factors, zeros);
            auto shift_factors_hi = _mm_unpackhi_epi8(shift_factors, zeros);

            lo = _mm_mullo_epi16(lo, shift_factors_lo);
            hi = _mm_mullo_epi16(hi, shift_factors_hi);

            lo = _mm_srai_epi16(lo, 0x7);
            hi = _mm_srai_epi16(hi, 0x7);

            content = _mm_packs_epi16(lo, hi);

            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            // Invert negative values so that in all cases, it's appopriate to
            // shift in zeros
            auto negative_mask = _mm_cmplt_epi8(content, zeros);
            content = _mm_xor_si128(content, negative_mask);

            // Zero out any lanes which are shifted by 8 or more
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

            // Undo inversion of negative values
            content = _mm_xor_si128(content, negative_mask);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s8(content, vnegq_s8(decay(rhs)));

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return Vector{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi32(undef, undef))};

            #endif

            #if defined(AVEL_NEON)
            return Vector{vmvnq_s8(content)};
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
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm_test_epi8_mask(content, content)};

            #else
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        16 * sizeof(std::int8_t) == sizeof(vec16x8i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec16x8i>(vec16x8i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{decay(x)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec16x8u{vreinterpretq_u8_s8(decay(x))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec16x8u>(vec16x8u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{decay(x)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec16x8i{vreinterpretq_s8_u8(decay(x))}};
        #endif
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec16x8i operator-(vec16x8u v) {
        return vec16x8i{0x00} - vec16x8i{v};
    }

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL std::int8_t extract(vec16x8i v) {
        static_assert(N < vec16x8i::width, "Specified index does not exist");
        typename std::enable_if<N < vec16x8i::width, int>::type dummy_variable = 0;

        return static_cast<std::int8_t>(extract<N>(vec16x8u{v}));
    }

    template<std::uint32_t N>
    AVEL_FINL vec16x8i insert(vec16x8i v, std::int8_t x) {
        static_assert(N < vec16x8i::width, "Specified index does not exist");
        typename std::enable_if<N < vec16x8i::width, int>::type dummy_variable = 0;

        return vec16x8i{insert<N>(vec16x8u{v}, static_cast<std::uint8_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x8i bit_shift_left(vec16x8i v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        return vec16x8i{bit_shift_left<S>(vec16x8u{v})};
    }



    template<std::uint32_t S>
    AVEL_FINL vec16x8i bit_shift_right(vec16x8i v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
        auto widened = _mm256_cvtepi8_epi16(decay(v));
        widened = _mm256_srai_epi16(widened, S);

        return vec16x8i{_mm256_cvtepi16_epi8(widened)};

        #elif defined(AVEL_SSE2)
        auto lo = _mm_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm_unpackhi_epi8(decay(v), decay(v));

        lo = _mm_srai_epi16(lo, S + 8);
        hi = _mm_srai_epi16(hi, S + 8);

        return vec16x8i{_mm_packs_epi16(lo, hi)};

        #endif

        #if defined(AVEL_NEON)
        return vec16x8i{vshrq_n_s8(decay(v), S)};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i bit_shift_right<0>(vec16x8i v) {
        return v;
    }

    #if defined(AVEL_SSE2)
    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i bit_shift_right<7>(vec16x8i v) {
        return vec16x8i{_mm_cmplt_epi8(decay(v), _mm_setzero_si128())};
    }
    #endif

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i bit_shift_right<8>(vec16x8i v) {
        return set_bits(v < vec16x8i{0x00});
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x8i rotl(vec16x8i v) {
        return vec16x8i{rotl<S>(vec16x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotl(vec16x8i x, long long s) {
        return vec16x8i{rotl(vec16x8u{x}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotl(vec16x8i x, vec16x8i s) {
        return vec16x8i{rotl(vec16x8u{x}, vec16x8u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x8i rotr(vec16x8i v) {
        return vec16x8i{rotr<S>(vec16x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotr(vec16x8i x, long long s) {
        return vec16x8i{rotr(vec16x8u{x}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotr(vec16x8i x, vec16x8i s) {
        return vec16x8i{rotr(vec16x8u{x}, vec16x8u{s})};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec16x8i v) {
        return count(vec16x8u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec16x8i v) {
        return any(vec16x8u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec16x8i v) {
        return all(vec16x8u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec16x8i v) {
        return none(vec16x8u{v});
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i set_bits(mask16x8i m) {
        return vec16x8i{set_bits(mask16x8u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i keep(mask16x8i m, vec16x8i v) {
        return vec16x8i{keep(mask16x8u{m}, vec16x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i clear(mask16x8i m, vec16x8i v) {
        return vec16x8i{clear(mask16x8u{m}, vec16x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i blend(mask16x8i m, vec16x8i a, vec16x8i b) {
        return vec16x8i{blend(mask16x8u{m}, vec16x8u{a}, vec16x8u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i byteswap(vec16x8i v) {
        return v;
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i max(vec16x8i a, vec16x8i b) {
        #if defined(AVEL_SSE4_1)
        return vec16x8i{_mm_max_epi8(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec16x8i{vmaxq_s8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i min(vec16x8i a, vec16x8i b) {
        #if defined(AVEL_SSE4_1)
        return vec16x8i{_mm_min_epi8(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, a, b);

        #endif

        #if defined(AVEL_NEON)
        return vec16x8i{vminq_s8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 2> minmax(vec16x8i a, vec16x8i b) {
        #if defined(AVEL_SSE4_1)
        return {
            vec16x8i{_mm_min_epi8(decay(a), decay(b))},
            vec16x8i{_mm_max_epi8(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto mask = a < b;

        return {
            blend(mask, a, b),
            blend(mask, b, a)
        };

        #endif

        #if defined(AVEL_NEON)
        return {
            vec16x8i{vminq_s8(decay(a), decay(b))},
            vec16x8i{vmaxq_s8(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i clamp(vec16x8i x, vec16x8i lo, vec16x8i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i average(vec16x8i x, vec16x8i y) {
        //TODO: Leverage newer instruction sets
        #if defined(AVEL_SSE2)
        auto tmp = (x ^ y);
        auto avg = (x & y) + (tmp >> 1);
        auto c = set_bits((x < -y) | (y == vec16x8i(0x80))) & tmp & vec16x8i{0x01};
        return avg + c;

        #endif

        #if defined(AVEL_NEON)
        auto avg = vec16x8i{vhaddq_s8(decay(x), decay(y))};
        auto c = set_bits((x < -y) | (y == vec16x8i(0x80))) & (x ^ y) & vec16x8i{1};

        return avg + c;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i midpoint(vec16x8i a, vec16x8i b) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
        auto avg = _mm_avg_epu8(decay(a), decay(b));
        auto sign_correction = _mm_ternarylogic_epi32(decay(a), decay(b), _mm_set1_epi8(0x80), 0x28);
        auto average = _mm_xor_si128(avg, sign_correction);

        auto bias = _mm_ternarylogic_epi32(decay(a), decay(b), _mm_set1_epi8(0x1), 0x28);
        auto mask = _mm_cmplt_epi8_mask(decay(a), decay(b));
        auto ret = _mm_mask_sub_epi8(average, mask, average, bias);

        return vec16x8i{ret};

        #elif defined(AVEL_SSE2)
        auto offset = _mm_set1_epi8(0x80);

        auto a_offset = _mm_xor_si128(decay(a), offset);
        auto b_offset = _mm_xor_si128(decay(b), offset);

        auto average_offset = _mm_avg_epu8(a_offset, b_offset);
        auto average = _mm_xor_si128(average_offset, offset);

        auto m = _mm_cmplt_epi8(decay(a), decay(b));
        auto bias = _mm_and_si128(_mm_xor_si128(decay(a), decay(b)), _mm_and_si128(m, _mm_set1_epi8(0x01)));

        return vec16x8i{_mm_sub_epi8(average, bias)};
        #endif

        #if defined(AVEL_NEON)
        vec16x8i t0 = vec16x8i{vhaddq_s8(decay(a), decay(b))};
        vec16x8i t1 = (a ^ b) & vec16x8i{0x1} & set_bits(b < a);
        return t0 + t1;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i negate(mask16x8i m, vec16x8i v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
        return vec16x8i{_mm_mask_sub_epi8(decay(v), decay(m), _mm_setzero_si128(), decay(v))};

        #elif defined(AVEL_SSSE3)
        return vec16x8i{_mm_sign_epi8(decay(v), _mm_or_si128(decay(m), _mm_set1_epi8(0x01)))};
        #endif

        #if defined(AVEL_SSE2) || defined(AVEL_NEON)
        auto mask = set_bits(m);
        return (v ^ mask) - mask;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i abs(vec16x8i v) {
        #if defined(AVEL_SSSE3)
        return vec16x8i{_mm_abs_epi8(decay(v))};

        #elif defined(AVEL_SSE2)
        auto y = vec16x8i{_mm_cmplt_epi8(decay(v), _mm_setzero_si128())};
        return (v ^ y) - y;

        #endif

        #if defined(AVEL_NEON)
        auto zeros = vdupq_n_s8(0x00);
        return vec16x8i{vabdq_s8(decay(v), zeros)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i neg_abs(vec16x8i v) {
        #if defined(AVEL_SSSE3)
        return -abs(v);

        #elif defined(AVEL_SSE2)
        auto y = vec16x8i{_mm_cmplt_epi8(_mm_setzero_si128(), decay(v))};
        return (v ^ y) - y;
        #endif

        #if defined(AVEL_NEON)
        auto y = vmvnq_s8(vshrq_n_s8(decay(v), 7));
        return vec16x8i{vsubq_s8(veorq_s8(decay(v), y), y)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i neg_abs(vec16x8u v) {
        return neg_abs(vec16x8i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i load<vec16x8i>(const std::int8_t* ptr, std::uint32_t n) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
        auto mask = (n >= 16) ? std::uint16_t(-1) : (1 << n) - 1;
        return vec16x8i{_mm_maskz_loadu_epi8(mask, ptr)};

        #elif defined(AVEL_SSE2)
        // GCC 9 doesn't have _mm_loadu_si16(), _mm_loadu_si32(), or
        // _mm_loadu_si64() but the following code is optimized down to
        // reasonable machine code.
        switch (n) {
            case 0: {
                return vec16x8i{_mm_setzero_si128()};
            }
            case 1: {
                std::uint8_t one_s;
                std::memcpy(&one_s, ptr + 0, sizeof(std::uint8_t));
                return vec16x8i{_mm_cvtsi32_si128(one_s)};
            }
            case 2: {
                std::uint16_t two_s;
                std::memcpy(&two_s, ptr + 0, sizeof(std::uint16_t));

                return vec16x8i{_mm_cvtsi32_si128(two_s)};
            }
            case 3: {
                std::uint16_t two_s;
                std::memcpy(&two_s, ptr + 0, sizeof(std::uint16_t));

                std::uint8_t one_s;
                std::memcpy(&one_s, ptr + 2, sizeof(std::uint8_t));

                auto lo = _mm_cvtsi32_si128(two_s);
                auto hi = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi16(lo, hi)};
            }

            case 4: {
                std::uint32_t four_s;
                std::memcpy(&four_s, ptr + 0, sizeof(std::uint32_t));

                return vec16x8i{_mm_cvtsi32_si128(four_s)};
            }
            case 5: {
                std::uint32_t four_s = 0x00;
                std::memcpy(&four_s, ptr + 0, sizeof(std::uint32_t));

                std::uint8_t one_s = 0x00;
                std::memcpy(&one_s, ptr + 4, sizeof(std::uint8_t));

                auto four_v = _mm_cvtsi32_si128(four_s);
                auto one_v  = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi32(four_v, one_v)};
            }
            case 6: {
                std::uint32_t four_s;
                std::memcpy(&four_s, ptr + 0, sizeof(std::uint32_t));

                std::uint16_t two_s;
                std::memcpy(&two_s, ptr + 4, sizeof(std::uint16_t));

                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v  = _mm_cvtsi32_si128(two_s);

                return vec16x8i{_mm_unpacklo_epi32(four_v, two_v)};
            }
            case 7:{
                std::uint32_t four_s;
                std::memcpy(&four_s, ptr + 0, sizeof(std::uint32_t));

                std::uint16_t two_s;
                std::memcpy(&two_s, ptr + 4, sizeof(std::uint16_t));

                std::uint8_t one_s;
                std::memcpy(&one_s, ptr + 6, sizeof(std::uint8_t));

                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v  = _mm_cvtsi32_si128(two_s);
                auto one_v  = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi32(four_v, _mm_unpacklo_epi16(two_v, one_v))};
            }

            case 8: {
                std::uint64_t eight_s;
                std::memcpy(&eight_s, ptr + 0, sizeof(std::int64_t));

                return vec16x8i{_mm_cvtsi64_si128(eight_s)};
            }
            case 9: {
                std::uint64_t eight_s;
                std::memcpy(&eight_s, ptr + 0, sizeof(std::uint64_t));

                std::uint8_t one_s;
                std::memcpy(&one_s, ptr + 8, sizeof(std::uint8_t));

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, one_v)};
            }
            case 10: {
                std::uint64_t eight_s;
                std::memcpy(&eight_s, ptr + 0, sizeof(std::uint64_t));

                std::uint16_t two_s;
                std::memcpy(&two_s, ptr + 8, sizeof(std::uint16_t));

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto two_v = _mm_cvtsi32_si128(two_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, two_v)};
            }
            case 11: {
                std::uint64_t eight_s = 0x00;
                std::memcpy(&eight_s, ptr + 0, sizeof(std::uint64_t));

                std::uint16_t two_s = 0x00;
                std::memcpy(&two_s, ptr + 8, sizeof(std::uint16_t));

                std::uint8_t one_s = 0x00;
                std::memcpy(&one_s, ptr + 10, sizeof(std::uint8_t));

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto two_v = _mm_cvtsi32_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi16(two_v, one_v))};
            }

            case 12: {
                std::uint64_t eight_s;
                std::memcpy(&eight_s, ptr + 0, sizeof(std::uint64_t));

                std::uint32_t four_s;
                std::memcpy(&four_s, ptr + 8, sizeof(std::uint32_t));

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, four_v)};
            }
            case 13: {
                std::uint64_t eight_s = 0x00;
                std::memcpy(&eight_s, ptr + 0, sizeof(std::uint64_t));

                std::uint32_t four_s = 0x00;
                std::memcpy(&four_s, ptr + 8, sizeof(std::uint32_t));

                std::uint8_t one_s = 0x00;
                std::memcpy(&one_s, ptr + 12, sizeof(std::uint8_t));

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi32(four_v, one_v))};
            }
            case 14: {
                std::uint64_t eight_s;
                std::memcpy(&eight_s, ptr + 0, sizeof(std::uint64_t));

                std::uint32_t four_s;
                std::memcpy(&four_s, ptr + 8, sizeof(std::uint32_t));

                std::uint16_t two_s;
                std::memcpy(&two_s, ptr + 12, sizeof(std::uint16_t));

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v = _mm_cvtsi32_si128(two_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi32(four_v, two_v))};
            }
            case 15: {
                std::uint64_t eight_s;
                std::memcpy(&eight_s, ptr + 0, sizeof(std::uint64_t));

                std::uint32_t four_s;
                std::memcpy(&four_s, ptr + 8, sizeof(std::uint32_t));

                std::uint16_t two_s;
                std::memcpy(&two_s, ptr + 12, sizeof(std::uint16_t));

                std::uint8_t one_s;
                std::memcpy(&one_s, ptr + 14, sizeof(std::uint8_t));

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v = _mm_cvtsi32_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi32(four_v, _mm_unpacklo_epi16(two_v, one_v)))};
            }

            default: {
                return vec16x8i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
            }
        }
        #endif

        #if defined(AVEL_NEON)
        // While ARM neon doesn't provide partial load instructions, however,
        // the following code compiles down to reasonable machine code
        switch (n) {
            case 0: {
                return vec16x8i{vdupq_n_s8(0x00)};
            }
            case 1: {
                std::int8_t x0 = 0;

                std::memcpy(&x0, ptr, sizeof(std::int8_t));

                auto ret0 = vsetq_lane_s8(x0, vdupq_n_s8(0x00), 0);
                return vec16x8i{ret0};
            }
            case 2: {
                std::int16_t x0 = 0;

                std::memcpy(&x0, ptr, sizeof(std::int16_t));

                auto ret0 = vsetq_lane_s16(x0, vdupq_n_s16(0x00), 0);
                auto ret1 = vreinterpretq_s8_s16(ret0);
                return vec16x8i{ret1};
            }
            case 3: {
                std::int16_t x0 = 0;
                std::int8_t  x1 = 0;

                std::memcpy(&x0, ptr + 0, sizeof(std::int16_t));
                std::memcpy(&x1, ptr + 2, sizeof(std::int8_t ));

                auto ret0 = vsetq_lane_s16(x0, vdupq_n_s16(0x00), 0);
                auto ret1 = vsetq_lane_s8(x1, vreinterpretq_s8_s16(ret0), 2);
                return vec16x8i{ret1};
            }

            case 4: {
                std::int32_t x0 = 0;

                std::memcpy(&x0, ptr, sizeof(std::int32_t));

                auto ret0 = vsetq_lane_u32(x0, vdupq_n_u32(0x00), 0);
                auto ret1 = vreinterpretq_s8_u32(ret0);
                return vec16x8i{ret1};
            }
            case 5: {
                std::int32_t x0 = 0;
                std::int8_t  x1 = 0;

                std::memcpy(&x0, ptr + 0, sizeof(std::int32_t));
                std::memcpy(&x1, ptr + 4, sizeof(std::int8_t));

                auto ret0 = vsetq_lane_u32(x0, vdupq_n_u32(0x00), 0);
                auto ret1 = vsetq_lane_s8 (x1, vreinterpretq_s8_u32(ret0), 4);
                return vec16x8i{ret1};
            }
            case 6: {
                std::int32_t x0 = 0;
                std::int16_t x1 = 0;

                std::memcpy(&x0, ptr + 0, sizeof(std::int32_t));
                std::memcpy(&x1, ptr + 4, sizeof(std::int16_t));

                auto ret0 = vsetq_lane_u32(x0, vdupq_n_u32(0x00), 0);
                auto ret1 = vsetq_lane_s16(x1, vreinterpretq_s16_u32(ret0), 2);
                auto ret2 = vreinterpretq_s8_s16(ret1);
                return vec16x8i{ret2};
            }
            case 7:{
                std::int32_t x0 = 0;
                std::int16_t x1 = 0;
                std::int8_t  x2 = 0;

                std::memcpy(&x0, ptr + 0, sizeof(std::int32_t));
                std::memcpy(&x1, ptr + 4, sizeof(std::int16_t));
                std::memcpy(&x2, ptr + 6, sizeof(std::int8_t ));

                auto ret0 = vsetq_lane_u32(x0, vdupq_n_u32(0x00), 0);
                auto ret1 = vsetq_lane_s16(x1, vreinterpretq_s16_u32(ret0), 2);
                auto ret2 = vsetq_lane_s8 (x2, vreinterpretq_s8_s16 (ret1), 6);
                return vec16x8i{ret2};
            }

            case 8: {
                std::int64_t x0 = 0;

                std::memcpy(&x0, ptr + 0, sizeof(std::int64_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vreinterpretq_s8_s64(ret0);
                return vec16x8i{ret1};
            }
            case 9: {
                std::int64_t x0 = 0;
                std::int8_t  x1 = 0;

                std::memcpy(&x0, ptr + 0, sizeof(std::int64_t));
                std::memcpy(&x1, ptr + 8, sizeof(std::int8_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s8(x1, vreinterpretq_s8_s64(ret0), 8);
                return vec16x8i{ret1};
            }
            case 10: {
                std::int64_t x0 = 0;
                std::int16_t x1 = 0;

                std::memcpy(&x0, ptr + 0, sizeof(std::int64_t));
                std::memcpy(&x1, ptr + 8, sizeof(std::int16_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s16(x1, vreinterpretq_s16_s64(ret0), 4);
                auto ret2 = vreinterpretq_s8_s16(ret1);
                return vec16x8i{ret2};
            }
            case 11: {
                std::int64_t x0 = 0;
                std::int16_t x1 = 0;
                std::int8_t  x2 = 0;

                std::memcpy(&x0, ptr + 0, sizeof(std::int64_t));
                std::memcpy(&x1, ptr + 8, sizeof(std::int16_t));
                std::memcpy(&x2, ptr + 10, sizeof(std::int8_t ));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s16(x1, vreinterpretq_s16_s64(ret0), 4);
                auto ret2 = vsetq_lane_s8 (x2, vreinterpretq_s8_s16(ret1), 10);
                return vec16x8i{ret2};
            }

            case 12: {
                std::int64_t x0 = 0;
                std::int32_t x1 = 0;

                std::memcpy(&x0, ptr + 0, sizeof(std::int64_t));
                std::memcpy(&x1, ptr + 8, sizeof(std::int32_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s32(x1, vreinterpretq_s32_s64(ret0), 2);
                auto ret2 = vreinterpretq_s8_s32(ret1);
                return vec16x8i{ret2};
            }
            case 13: {
                std::int64_t x0 = 0;
                std::int32_t x1 = 0;
                std::int8_t  x2 = 0;

                std::memcpy(&x0, ptr +  0, sizeof(std::int64_t));
                std::memcpy(&x1, ptr +  8, sizeof(std::int32_t));
                std::memcpy(&x2, ptr + 12, sizeof(std::int8_t ));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s32(x1, vreinterpretq_s32_s64(ret0), 2);
                auto ret2 = vsetq_lane_s8 (x2, vreinterpretq_s8_s32 (ret1), 12);
                return vec16x8i{ret2};
            }
            case 14: {
                std::int64_t x0 = 0;
                std::int32_t x1 = 0;
                std::int16_t x2 = 0;

                std::memcpy(&x0, ptr +  0, sizeof(std::int64_t));
                std::memcpy(&x1, ptr +  8, sizeof(std::int32_t));
                std::memcpy(&x2, ptr + 12, sizeof(std::int16_t));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s32(x1, vreinterpretq_s32_s64(ret0), 2);
                auto ret2 = vsetq_lane_s16(x2, vreinterpretq_s16_s32(ret1), 6);
                auto ret3 = vreinterpretq_s8_s16(ret2);
                return vec16x8i{ret3};
            }
            case 15: {
                std::int64_t x0 = 0;
                std::int32_t x1 = 0;
                std::int16_t x2 = 0;
                std::int8_t  x3 = 0;

                std::memcpy(&x0, ptr +  0, sizeof(std::int64_t));
                std::memcpy(&x1, ptr +  8, sizeof(std::int32_t));
                std::memcpy(&x2, ptr + 12, sizeof(std::int16_t));
                std::memcpy(&x3, ptr + 14, sizeof(std::int8_t ));

                auto ret0 = vsetq_lane_s64(x0, vdupq_n_s64(0x00), 0);
                auto ret1 = vsetq_lane_s32(x1, vreinterpretq_s32_s64(ret0), 2);
                auto ret2 = vsetq_lane_s16(x2, vreinterpretq_s16_s32(ret1), 6);
                auto ret3 = vsetq_lane_s8 (x3, vreinterpretq_s8_s16 (ret2), 14);
                return vec16x8i{ret3};
            }

            default: {
                return vec16x8i{vld1q_s8(ptr)};
            }
        }
        #endif
    }

    //TODO: Add specializations for multiple of N that allow for 32-bit
    //masked loads to be used

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i load<vec16x8i, vec16x8i::width>(const std::int8_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec16x8i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec16x8i{vld1q_s8(ptr)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i aligned_load<vec16x8i>(const std::int8_t* ptr, std::uint32_t n) {
        return load<vec16x8i>(ptr, n);
    }

    //TODO: Add specializations for multiple of N that allow for 32-bit
    //masked aligned loads to be used

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i aligned_load<vec16x8i, vec16x8u::width>(const std::int8_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec16x8i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Utilize __builtin_assume_aligned on GCC and Clang
        //TODO: Utilize assume_aligned if C++ 20 is available
        return vec16x8i{vld1q_s8(ptr)};
        #endif
    }



    AVEL_FINL void store(std::int8_t* ptr, vec16x8i v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        n = min(n, vec16x8i::width);
        auto mask = (1 << n) - 1;
        _mm_mask_storeu_epi8(ptr, mask, decay(v));

        #elif defined(AVEL_SSE2)
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec16x8u::width;
        auto h = vec16x8u::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(8 * (h - std::min(h, n))));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(8 * (w - std::min(w, n))));
        auto mask = _mm_unpacklo_epi64(lo, hi);
        _mm_maskmoveu_si128(decay(v), mask, reinterpret_cast<char *>(ptr));
        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            case 0: {} break;
            case 1: {
                std::int8_t x0 = vgetq_lane_s8(decay(v), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::int8_t));
            } break;
            case 2: {
                std::int16_t x0 = vgetq_lane_s16(vreinterpretq_s16_s8(decay(v)), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::int16_t));
            } break;
            case 3: {
                std::int16_t x0 = vgetq_lane_s16(vreinterpretq_s16_s8(decay(v)), 0);
                std::int8_t  x1 = vgetq_lane_s8(decay(v), 2);

                std::memcpy(ptr + 0, &x0, sizeof(std::int16_t));
                std::memcpy(ptr + 2, &x1, sizeof(std::int8_t ));
            } break;

            case 4: {
                std::int32_t x0 = vgetq_lane_s32(vreinterpretq_s32_s8(decay(v)), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::int32_t));
            } break;
            case 5: {
                std::int32_t x0 = vgetq_lane_s32(vreinterpretq_s32_s8(decay(v)), 0);
                std::int8_t  x1 = vgetq_lane_s8(decay(v), 4);

                std::memcpy(ptr + 0, &x0, sizeof(std::int32_t));
                std::memcpy(ptr + 4, &x1, sizeof(std::int8_t ));
            } break;
            case 6: {
                std::int32_t x0 = vgetq_lane_s32(vreinterpretq_s32_s8(decay(v)), 0);
                std::int16_t x1 = vgetq_lane_s16(vreinterpretq_s16_s8(decay(v)), 2);

                std::memcpy(ptr + 0, &x0, sizeof(std::int32_t));
                std::memcpy(ptr + 4, &x1, sizeof(std::int16_t));
            } break;
            case 7: {
                std::int32_t x0 = vgetq_lane_s32(vreinterpretq_s32_s8(decay(v)), 0);
                std::int16_t x1 = vgetq_lane_s16(vreinterpretq_s16_s8(decay(v)), 2);
                std::int8_t  x2 = vgetq_lane_s8(decay(v), 6);

                std::memcpy(ptr + 0, &x0, sizeof(std::int32_t));
                std::memcpy(ptr + 4, &x1, sizeof(std::int16_t));
                std::memcpy(ptr + 6, &x2, sizeof(std::int8_t ));
            } break;

            case 8: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s8(decay(v)), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::int64_t));
            } break;
            case 9: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s8(decay(v)), 0);
                std::int8_t  x1 = vgetq_lane_s8(decay(v), 8);

                std::memcpy(ptr + 0, &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 8, &x1, sizeof(std::int8_t ));
            } break;
            case 10: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s8(decay(v)), 0);
                std::int16_t x1 = vgetq_lane_s16(vreinterpretq_s16_s8(decay(v)), 4);

                std::memcpy(ptr + 0, &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 8, &x1, sizeof(std::int16_t));
            } break;
            case 11: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s8(decay(v)), 0);
                std::int16_t x1 = vgetq_lane_s16(vreinterpretq_s16_s8(decay(v)), 4);
                std::int8_t  x2 = vgetq_lane_s8(decay(v), 10);

                std::memcpy(ptr + 0,  &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 8,  &x1, sizeof(std::int16_t));
                std::memcpy(ptr + 10, &x2, sizeof(std::int8_t ));
            } break;

            case 12: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s8(decay(v)), 0);
                std::int32_t x1 = vgetq_lane_s32(vreinterpretq_s32_s8(decay(v)), 2);

                std::memcpy(ptr + 0,  &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 8,  &x1, sizeof(std::int32_t));
            } break;
            case 13: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s8(decay(v)), 0);
                std::int32_t x1 = vgetq_lane_s32(vreinterpretq_s32_s8(decay(v)), 2);
                std::int8_t  x2 = vgetq_lane_s8(decay(v), 12);

                std::memcpy(ptr + 0,  &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 8,  &x1, sizeof(std::int32_t));
                std::memcpy(ptr + 12, &x2, sizeof(std::int8_t ));
            } break;
            case 14: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s8(decay(v)), 0);
                std::int32_t x1 = vgetq_lane_s32(vreinterpretq_s32_s8(decay(v)), 2);
                std::int16_t x2 = vgetq_lane_s16(vreinterpretq_s16_s8(decay(v)), 6);

                std::memcpy(ptr + 0,  &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 8,  &x1, sizeof(std::int32_t));
                std::memcpy(ptr + 12, &x2, sizeof(std::int16_t));
            } break;
            case 15: {
                std::int64_t x0 = vgetq_lane_s64(vreinterpretq_s64_s8(decay(v)), 0);
                std::int32_t x1 = vgetq_lane_s32(vreinterpretq_s32_s8(decay(v)), 2);
                std::int16_t x2 = vgetq_lane_s16(vreinterpretq_s16_s8(decay(v)), 6);
                std::int8_t  x3 = vgetq_lane_s8(decay(v), 14);

                std::memcpy(ptr + 0,  &x0, sizeof(std::int64_t));
                std::memcpy(ptr + 8,  &x1, sizeof(std::int32_t));
                std::memcpy(ptr + 12, &x2, sizeof(std::int16_t));
                std::memcpy(ptr + 14, &x3, sizeof(std::int8_t ));
            } break;

            default: {
                vst1q_s8(ptr, decay(v));
            } break;
        }
        #endif
    }

    template<std::uint32_t N = vec16x8i::width>
    AVEL_FINL void store(std::int8_t* ptr, vec16x8i v) {
        static_assert(N <= vec16x8i::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= vec16x8i::width, int>::type dummy_variable = 0;

        store(ptr, v, N);
    }

    //TODO: Add specializations for multiple of N that allow for 32-bit
    //masked stores to be used

    template<>
    AVEL_FINL void store<vec16x8u::width>(std::int8_t* ptr, vec16x8i v) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s8(ptr, decay(v));
        #endif
    }


    AVEL_FINL void aligned_store(std::int8_t* ptr, vec16x8i v, std::uint32_t n) {
        store(ptr, v, n);
    }

    template<std::uint32_t N = vec16x8i::width>
    AVEL_FINL void aligned_store(std::int8_t* ptr, vec16x8i v) {
        static_assert(N <= vec16x8i::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= vec16x8i::width, int>::type dummy_variable = 0;

        aligned_store(ptr, v, N);
    }

    //TODO: Add specializations for multiple of N that allow for 32-bit
    //masked aligned stores to be used

    template<>
    AVEL_FINL void aligned_store<vec16x8i::width>(std::int8_t* ptr, vec16x8i v) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s8(ptr, decay(v));
        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr16x8i to_array(vec16x8i v) {
        alignas(16) arr16x8i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec16x8i> div(vec16x8i x, vec16x8i y) {
        mask16x8i remainder_sign_mask = (x < vec16x8i{0x00});
        mask16x8i quotient_sign_mask = remainder_sign_mask ^ (y < vec16x8i{0x00});

        vec16x8u numerator{abs(x)};
        vec16x8u denominator{abs(y)};

        auto results = div(numerator, denominator);

        return {
            negate(quotient_sign_mask,  vec16x8i{results.quot}),
            negate(remainder_sign_mask, vec16x8i{results.rem})
        };
    }

    AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(vec16x8i, mask16x8i, vec16x8u)

    [[nodiscard]]
    AVEL_FINL vec16x8i countl_sign(vec16x8i v) {
        #if defined(AVEL_NEON)
        return vec16x8i{vclsq_s8(decay(v))};

        #else
        return avel::countl_zero((v ^ avel::bit_shift_right<1>(v))) - vec16x8i{1};

        #endif
    }

}

#endif //AVEL_VEC16X8I_HPP
