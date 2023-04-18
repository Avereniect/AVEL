#ifndef AVEL_VEC32X8U_HPP
#define AVEL_VEC32X8U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec32x8u = Vector<std::uint8_t, 32>;
    using arr32x8u = std::array<std::uint8_t, 32>;
    using mask32x8u = Vector_mask<std::uint8_t, 32>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec32x8u> div(vec32x8u numerator, vec32x8u denominator);
    vec32x8u broadcast_mask(mask32x8u m);
    vec32x8u blend(vec32x8u a, vec32x8u b, mask32x8u m);
    vec32x8u countl_one(vec32x8u v);





    template<>
    class Vector_mask<std::uint8_t, 32> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 32;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        using primitive = __mmask32;
        #elif defined(AVEL_AVX2)
        using primitive = __m256i;
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
            content(b ? 0xFFFFFFFF : 0x00000000) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_set1_epi8(b ? -1 : 0)) {}
        #endif


        AVEL_FINL explicit Vector_mask(const arr32xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()));
            content = primitive(_mm256_cmplt_epu8_mask(_mm256_setzero_si256(), array_data));

            #elif defined(AVEL_AVX2)
            primitive array_data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()));
            content = _mm256_cmpgt_epi8(array_data, _mm256_setzero_si256());

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
            auto tmp = _kxor_mask32(decay(lhs), decay(rhs));
            return _kortestz_mask32_u8(tmp, tmp);

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(decay(lhs)) == _mm256_movemask_epi8(decay(rhs));
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto tmp = _kxor_mask32(decay(lhs), decay(rhs));
            return !_kortestz_mask32_u8(tmp, tmp);

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(decay(lhs)) != _mm256_movemask_epi8(decay(rhs));
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content &= decay(rhs);

            #elif defined(AVEL_AVX2)
            content = _mm256_and_si256(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content |= decay(rhs);

            #elif defined(AVEL_AVX2)
            content = _mm256_or_si256(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content ^= decay(rhs);

            #elif defined(AVEL_AVX2)
            content = _mm256_xor_si256(content, decay(rhs));

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector_mask{static_cast<primitive>(~content)};

            #elif defined(AVEL_AVX2)
            auto undef = _mm256_undefined_si256();
            auto full = _mm256_cmpeq_epi8(undef, undef);
            return Vector_mask{_mm256_andnot_si256(content, full)};

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
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask32x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(decay(m));

        #elif defined(AVEL_AVX2)
        return popcount(_mm256_movemask_epi8(decay(m)));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask32x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return !_kortestz_mask16_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX2)
        return _mm256_movemask_epi8(decay(m));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask32x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _kortestc_mask32_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX2)
        return 0xFFFFFFFF == _mm256_movemask_epi8(decay(m));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask32x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _kortestz_mask32_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX2)
        return _mm256_testz_si256(decay(m), decay(m));

        #endif
    }




    template<>
    class Vector<std::uint8_t, 32> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 32;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint8_t;

        #if defined(AVEL_AVX2)
        using primitive = __m256i;
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
        AVEL_FINL explicit Vector(Vector<U, width> x):
            Vector(convert<Vector>(x)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content(_mm256_maskz_set1_epi8(decay(m), 1)) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_sub_epi8(_mm256_setzero_si256(), decay(m))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            content(_mm256_set1_epi8(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr32x8u& arr):
        #if defined(AVEL_AVX2)
            content(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()))) {}
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
            return mask{_mm256_cmpeq_epi8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpeq_epi8(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmpneq_epi8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpeq_epi8(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmplt_epu8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            Vector addition_mask{0x80};
            return mask{_mm256_cmpgt_epi8(decay(rhs ^ addition_mask), decay(lhs ^ addition_mask))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmple_epu8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            auto mins = _mm256_min_epu8(decay(lhs), decay(rhs));
            return mask{_mm256_cmpeq_epi8(mins, decay(lhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmpgt_epu8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            Vector add_mask{0x80};
            return mask{_mm256_cmpgt_epi8(decay(lhs ^ add_mask), decay(rhs ^ add_mask))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmpge_epu8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            auto mins = _mm256_min_epu8(decay(lhs), decay(rhs));
            return mask{_mm256_cmpeq_epi8(mins, decay(rhs))};

            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        //Definition of operator-() deferred until later

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_add_epi8(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_sub_epi8(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto a = _mm512_cvtepu8_epi16(content);
            auto b = _mm512_cvtepu8_epi16(rhs.content);

            auto c = _mm512_mullo_epi16(a, b);
            content = _mm512_cvtepi16_epi8(c);

            #elif defined(AVEL_AVX2)
            auto lhs_lo = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(content, 0x0));
            auto lhs_hi = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(content, 0x1));

            auto rhs_lo = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(decay(rhs), 0x0));
            auto rhs_hi = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(decay(rhs), 0x1));

            auto products_lo = _mm256_mullo_epi16(lhs_lo, rhs_lo);
            auto products_hi = _mm256_mullo_epi16(lhs_hi, rhs_hi);

            auto shuffled_lo = _mm256_permute2x128_si256(products_lo, products_hi, 0x20);
            auto shuffled_hi = _mm256_permute2x128_si256(products_lo, products_hi, 0x31);

            auto byte_mask = _mm256_set1_epi16(0x00FF);

            auto masked_lo = _mm256_and_si256(shuffled_lo, byte_mask);
            auto masked_hi = _mm256_and_si256(shuffled_hi, byte_mask);


            auto packed = _mm256_packus_epi16(masked_lo, masked_hi);

            content = packed;
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
            #if defined(AVEL_AVX2)
            content = _mm256_and_si256(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_or_si256(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_xor_si256(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto widened = _mm512_cvtepi8_epi16(content);
            auto shifted = _mm512_sll_epi16(widened, _mm_cvtsi32_si128(rhs));
            content = _mm512_cvtepi16_epi8(shifted);

            #elif defined(AVEL_AVX2)
            auto s = _mm_cvtsi32_si128(rhs);

            auto shifted = _mm256_sll_epi16(content, s);
            auto mask = _mm256_set1_epi8(std::uint8_t(0xFF << rhs));
            content = _mm256_and_si256(mask, shifted);

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX2)
            auto s = _mm_cvtsi32_si128(rhs);

            auto shifted = _mm256_srl_epi16(content, s);
            auto mask = _mm256_set1_epi8(std::uint8_t(0xFF >> rhs));
            content = _mm256_and_si256(shifted, mask);

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto lhs_widened = _mm512_cvtepu8_epi16(content);
            auto rhs_widened = _mm512_cvtepu8_epi16(decay(rhs));

            auto shifted = _mm512_sllv_epi16(lhs_widened, rhs_widened);
            auto narrowed = _mm512_cvtepi16_epi8(shifted);

            content = narrowed;

            #elif defined(AVEL_AVX2)
            // 16-bit multiplication approach
            alignas(16) static constexpr std::uint8_t table_data[16] {
                0x01, 0x02, 0x04, 0x08,
                0x10, 0x20, 0x40, 0x80,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00
            };

            auto zeros = _mm256_setzero_si256();
            auto table = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data)));

            auto lhs_lo = _mm256_unpacklo_epi8(content, zeros);
            auto lhs_hi = _mm256_unpackhi_epi8(content, zeros);

            auto valid_mask = _mm256_cmpgt_epi8(decay(rhs), _mm256_set1_epi8(0x7));
            auto indices = _mm256_or_si256(decay(rhs), valid_mask);
            auto shifts = _mm256_shuffle_epi8(table, indices);

            auto rhs_lo = _mm256_unpacklo_epi8(shifts, zeros);
            auto rhs_hi = _mm256_unpackhi_epi8(shifts, zeros);

            lhs_lo = _mm256_mullo_epi16(lhs_lo, rhs_lo);
            lhs_hi = _mm256_mullo_epi16(lhs_hi, rhs_hi);

            auto byte_mask = _mm256_set1_epi16(0xFF);
            lhs_lo = _mm256_and_si256(lhs_lo, byte_mask);
            lhs_hi = _mm256_and_si256(lhs_hi, byte_mask);

            content = _mm256_packus_epi16(lhs_lo, lhs_hi);

            /*
            // Divide and conquer approach

            auto zeros = _mm256_setzero_si256();

            auto a = _mm256_cmpeq_epi8(_mm256_and_si256(decay(rhs), _mm256_set1_epi8(0x01)), zeros);
            content = _mm256_add_epi8(content, _mm256_andnot_si256(a, content));

            auto b = _mm256_cmpeq_epi8(_mm256_and_si256(decay(rhs), _mm256_set1_epi8(0x02)), zeros);
            content = _mm256_add_epi8(content, _mm256_andnot_si256(b, content));
            content = _mm256_add_epi8(content, _mm256_andnot_si256(b, content));

            auto c = _mm256_cmpeq_epi8(_mm256_and_si256(decay(rhs), _mm256_set1_epi8(0x04)), zeros);
            auto tmp = _mm256_and_si256(_mm256_slli_epi16(content, 4), _mm256_set1_epi16(0xF0FF));
            content = _mm256_blendv_epi8(tmp, content, c);

            content = _mm256_andnot_si256(_mm256_cmpgt_epi8(decay(rhs), _mm256_set1_epi8(0x7)), content);
            */

            #endif

            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto lhs_widened = _mm512_cvtepu8_epi16(content);
            auto rhs_widened = _mm512_cvtepu8_epi16(decay(rhs));

            auto shifted = _mm512_srlv_epi16(lhs_widened, rhs_widened);
            auto narrowed = _mm512_cvtepi16_epi8(shifted);

            content = narrowed;

            #elif defined(AVEL_AVX2)
            alignas(16) static constexpr std::uint8_t table_data[16] {
                0x80, 0x40, 0x20, 0x10,
                0x08, 0x04, 0x02, 0x01,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00
            };

            auto s = _mm256_min_epu8(decay(rhs), _mm256_set1_epi8(0x8));

            auto table = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data)));

            auto shift_factors = _mm256_shuffle_epi8(table, s);

            auto zeros = _mm256_setzero_si256();
            auto lo = _mm256_unpacklo_epi8(content, zeros);
            auto hi = _mm256_unpackhi_epi8(content, zeros);

            auto shift_factors_lo = _mm256_unpacklo_epi8(shift_factors, zeros);
            auto shift_factors_hi = _mm256_unpackhi_epi8(shift_factors, zeros);

            lo = _mm256_mullo_epi16(lo, shift_factors_lo);
            hi = _mm256_mullo_epi16(hi, shift_factors_hi);

            lo = _mm256_srli_epi16(lo, 0x7);
            hi = _mm256_srli_epi16(hi, 0x7);

            content = _mm256_packus_epi16(lo, hi);

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector{_mm256_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_AVX2)
            auto undef = _mm256_undefined_si256();
            return Vector{_mm256_andnot_si256(content, _mm256_cmpeq_epi32(undef, undef))};

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
            return mask{_mm256_test_epi8_mask(content, content)};

            #elif defined(AVEL_AVX2)
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        32 * sizeof(std::uint8_t) == sizeof(vec32x8u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint8_t extract(vec32x8u v) {
        static_assert(N <= 32, "Specified index does not exist");
        typename std::enable_if<N <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return _mm256_extract_epi8(decay(v), N);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec32x8u insert(vec32x8u v, std::uint8_t x) {
        static_assert(N <= 32, "Specified index does not exist");
        typename std::enable_if<N <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return vec32x8u{_mm256_insert_epi8(decay(v), x, N)};
        #endif
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec32x8u bit_shift_left(vec32x8u v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        auto shifted = _mm256_slli_epi16(decay(v), S);
        auto mask = _mm256_set1_epi8(std::uint8_t(0xFF << S));
        auto masked = _mm256_and_si256(mask, shifted);

        return vec32x8u{masked};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u bit_shift_left<0>(vec32x8u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u bit_shift_left<1>(vec32x8u v) {
        return v + v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u bit_shift_left<2>(vec32x8u v) {
        auto tmp0 = v + v;
        return tmp0 + tmp0;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u bit_shift_left<3>(vec32x8u v) {
        auto tmp0 = v + v;
        auto tmp1 = tmp0 + tmp0;
        return tmp1 + tmp1;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u bit_shift_left<8>(vec32x8u v) {
        (void)v;
        return vec32x8u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec32x8u bit_shift_right(vec32x8u v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        auto shifted = _mm256_srli_epi16(decay(v), S);
        auto mask = _mm256_set1_epi8(std::uint8_t(0xFF >> S));
        auto masked = _mm256_and_si256(shifted, mask);

        return vec32x8u{masked};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u bit_shift_right<0>(vec32x8u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u bit_shift_right<8>(vec32x8u v) {
        (void)v;
        return vec32x8u{0x00};
    }




    template<std::uint32_t S, typename std::enable_if<S < 8, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec32x8u rotl(vec32x8u v) {
        #if defined(AVEL_AVX2)
        auto lo = _mm256_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm256_unpackhi_epi8(decay(v), decay(v));

        lo = _mm256_slli_epi16(lo, S);
        hi = _mm256_slli_epi16(hi, S);

        lo = _mm256_srli_epi16(lo, 8);
        hi = _mm256_srli_epi16(hi, 8);

        auto ret = _mm256_packus_epi16(lo, hi);
        return vec32x8u{ret};
        #endif
    }

    template<>
    AVEL_FINL vec32x8u rotl<0>(vec32x8u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<8 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec32x8u rotl(vec32x8u v) {
        return rotl<S % 8>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u rotl(vec32x8u v, long long s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        s &= 0x7;
        auto r_shift_amount = _mm_cvtsi64_si128(8 - s);
        auto l_shift_amount = _mm_cvtsi64_si128(s);

        auto whole = _mm512_cvtepu8_epi16(decay(v));
        auto r_shifted = _mm512_srl_epi16(whole, r_shift_amount);
        auto l_shifted =_mm512_sll_epi16(whole, l_shift_amount);

        auto rotated = _mm512_or_si512(l_shifted, r_shifted);
        return vec32x8u{_mm512_cvtepi16_epi8(rotated)};

        #elif defined(AVEL_AVX2)
        s &= 0x7;
        auto shift_amount = _mm_cvtsi64_si128(8 - s);
        auto lo = _mm256_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm256_unpackhi_epi8(decay(v), decay(v));

        lo = _mm256_srl_epi16(lo, shift_amount);
        hi = _mm256_srl_epi16(hi, shift_amount);

        auto byte_mask = _mm256_set1_epi16(0x00FF);
        lo = _mm256_and_si256(lo, byte_mask);
        hi = _mm256_and_si256(hi, byte_mask);

        return vec32x8u{_mm256_packus_epi16(lo, hi)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u rotl(vec32x8u v, vec32x8u s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto widened_x = _mm512_cvtepu8_epi16(decay(v));
        auto widened_s = _mm512_cvtepu8_epi16(_mm256_and_si256(decay(s), _mm256_set1_epi8(0x7)));

        auto shifted_left = _mm512_sllv_epi16(widened_x, widened_s);
        auto shifted_right = _mm512_srlv_epi16(widened_x, _mm512_sub_epi16(_mm512_set1_epi16(8), widened_s));

        auto rotated = _mm512_or_si512(shifted_left, shifted_right);

        auto ret = _mm512_cvtepi16_epi8(rotated);

        return vec32x8u{ret};

        #elif defined(AVEL_AVX2)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0x01, 0x02, 0x04, 0x08,
            0x10, 0x20, 0x40, 0x80,
            0x01, 0x01, 0x01, 0x01,
            0x01, 0x01, 0x01, 0x01
        };

        s &= vec32x8u{0x7};

        auto lo = _mm256_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm256_unpackhi_epi8(decay(v), decay(v));

        auto table = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data)));

        auto shift_factors = _mm256_shuffle_epi8(table, decay(s));

        auto zeros = _mm256_setzero_si256();
        auto shift_factors_lo = _mm256_unpacklo_epi8(shift_factors, zeros);
        auto shift_factors_hi = _mm256_unpackhi_epi8(shift_factors, zeros);

        lo = _mm256_mullo_epi16(lo, shift_factors_lo);
        hi = _mm256_mullo_epi16(hi, shift_factors_hi);

        lo = _mm256_srli_epi16(lo, 0x8);
        hi = _mm256_srli_epi16(hi, 0x8);

        return vec32x8u{_mm256_packus_epi16(lo, hi)};

        #endif
    }



    template<std::uint32_t S, typename std::enable_if<S < 8, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec32x8u rotr(vec32x8u v) {
        #if defined(AVEL_AVX2)
        auto lo = _mm256_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm256_unpackhi_epi8(decay(v), decay(v));

        lo = _mm256_slli_epi16(lo, 8 - S);
        hi = _mm256_slli_epi16(hi, 8 - S);

        lo = _mm256_srli_epi16(lo, 8);
        hi = _mm256_srli_epi16(hi, 8);

        auto ret = _mm256_packus_epi16(lo, hi);
        return vec32x8u{ret};
        #endif
    }

    template<>
    AVEL_FINL vec32x8u rotr<0>(vec32x8u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<8 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec32x8u rotr(vec32x8u v) {
        return rotr<S % 8>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u rotr(vec32x8u v, long long s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        s &= 0x7;
        auto r_shift_amount = _mm_cvtsi64_si128(s);
        auto l_shift_amount = _mm_cvtsi64_si128(8 - s);

        auto whole = _mm512_cvtepu8_epi16(decay(v));
        auto r_shifted = _mm512_srl_epi16(whole, r_shift_amount);
        auto l_shifted =_mm512_sll_epi16(whole, l_shift_amount);

        auto rotated = _mm512_or_si512(l_shifted, r_shifted);
        return vec32x8u{_mm512_cvtepi16_epi8(rotated)};

        #elif defined(AVEL_AVX2)
        s &= 0x7;
        auto shift_amount = _mm_cvtsi64_si128(8 - s);

        auto lo = _mm256_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm256_unpackhi_epi8(decay(v), decay(v));

        lo = _mm256_sll_epi16(lo, shift_amount);
        hi = _mm256_sll_epi16(hi, shift_amount);

        lo = _mm256_srli_epi16(lo, 8);
        hi = _mm256_srli_epi16(hi, 8);

        return vec32x8u{_mm256_packus_epi16(lo, hi)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u rotr(vec32x8u v, vec32x8u s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto widened_x = _mm512_cvtepu8_epi16(decay(v));
        widened_x = _mm512_or_si512(widened_x, _mm512_slli_epi16(widened_x, 0x8));

        auto widened_s = _mm512_cvtepu8_epi16(_mm256_and_si256(decay(s), _mm256_set1_epi8(0x7)));
        auto rotated = _mm512_srlv_epi16(widened_x, widened_s);

        return vec32x8u{_mm512_cvtepi16_epi8(rotated)};

        //TODO: Offer AVX2 version?

        #elif defined(AVEL_AVX2)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0x01, 0x80, 0x40, 0x20,
            0x10, 0x08, 0x04, 0x02,
            0x01, 0x01, 0x01, 0x01,
            0x01, 0x01, 0x01, 0x01
        };

        s &= vec32x8u{0x7};

        auto lo = _mm256_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm256_unpackhi_epi8(decay(v), decay(v));

        auto table = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data)));
        auto shift_factors = _mm256_shuffle_epi8(table, decay(s));

        auto zeros = _mm256_setzero_si256();
        auto shift_factors_lo = _mm256_unpacklo_epi8(shift_factors, zeros);
        auto shift_factors_hi = _mm256_unpackhi_epi8(shift_factors, zeros);

        lo = _mm256_mullo_epi16(lo, shift_factors_lo);
        hi = _mm256_mullo_epi16(hi, shift_factors_hi);

        lo = _mm256_srli_epi16(lo, 0x8);
        hi = _mm256_srli_epi16(hi, 0x8);

        return vec32x8u{_mm256_packus_epi16(lo, hi)};
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec32x8u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return count(mask32x8u{x});

        #elif defined(AVEL_AVX2)
        auto compared = _mm256_cmpeq_epi8(decay(x), _mm256_setzero_si256());
        return (vec32x8u::width - popcount(_mm256_movemask_epi8(compared))) / sizeof(vec32x8u::scalar);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec32x8u x) {
        return !_mm256_testz_si256(decay(x), decay(x));
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec32x8u x) {
        #if defined(AVEL_AVX2)
        auto compared = _mm256_cmpeq_epi8(decay(x), _mm256_setzero_si256());
        return 0x0000 == _mm256_movemask_epi8(compared);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec32x8u x) {
        #if defined(AVEL_AVX2)
        return _mm256_testz_si256(decay(x), decay(x));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u broadcast_mask(mask32x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec32x8u{_mm256_movm_epi8(decay(m))};

        #elif defined(AVEL_AVX2)
        return vec32x8u{decay(m)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u keep(mask32x8u m, vec32x8u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec32x8u{_mm256_maskz_mov_epi8(decay(m), decay(v))};

        #elif defined(AVEL_AVX2)
        return broadcast_mask(m) & v;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u clear(mask32x8u m, vec32x8u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec32x8u{_mm256_maskz_mov_epi8(decay(!m), decay(v))};

        #elif defined(AVEL_AVX2)
        return vec32x8u{_mm256_andnot_si256(decay(m), decay(v))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u blend(mask32x8u m, vec32x8u a, vec32x8u b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec32x8u{_mm256_mask_blend_epi8(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_AVX2)
        return vec32x8u{_mm256_blendv_epi8(decay(b), decay(a), decay(m))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u byteswap(vec32x8u v) {
        return v;
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u max(vec32x8u a, vec32x8u b) {
        #if defined(AVEL_AVX2)
        return vec32x8u{_mm256_max_epu8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u min(vec32x8u a, vec32x8u b) {
        #if defined(AVEL_AVX2)
        return vec32x8u{_mm256_min_epu8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec32x8u, 2> minmax(vec32x8u a, vec32x8u b) {
        #if defined(AVEL_AVX2)
        return {
            vec32x8u{_mm256_min_epu8(decay(a), decay(b))},
            vec32x8u{_mm256_max_epu8(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u clamp(vec32x8u x, vec32x8u lo, vec32x8u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u average(vec32x8u a, vec32x8u b) {
        #if defined(AVEL_AVX2)
        return vec32x8u{_mm256_avg_epu8(decay(a), decay(b))} - ((a ^ b) & vec32x8u{0x1});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u midpoint(vec32x8u a, vec32x8u b) {
        #if defined(AVEL_AVX512VL)
        auto t1 = _mm256_avg_epu8(decay(a), decay(b));
        auto t5 = _mm256_and_si256(_mm256_ternarylogic_epi32(decay(a), decay(b), decay(broadcast_mask(b < a)), 0x14), _mm256_set1_epi8(0x1));
        auto t6 = _mm256_sub_epi8(t1, t5);
        return vec32x8u{t6};

        #elif defined(AVEL_AVX2)
        auto t1 = _mm256_avg_epu8(decay(a), decay(b));
        auto t3 = _mm256_andnot_si256(decay(broadcast_mask(b <= a)), _mm256_xor_si256(decay(a), decay(b)));
        auto t5 = _mm256_and_si256(t3, _mm256_set1_epi8(0x1));
        auto t6 = _mm256_sub_epi8(t1, t5);
        return vec32x8u{t6};

        #endif
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u load<vec32x8u>(const std::uint8_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        std::uint32_t mask = (n >= 32) ? std::uint32_t(-1) : (1 << n) - 1;
        return vec32x8u{_mm256_maskz_loadu_epi8(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n == 32) {
            return vec32x8u{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
        }

        if (n <= 16) {
            auto low_half = decay(load<vec16x8u>(ptr, n));
            // _mm256_zextsi128_si256 isn't recognized by GCC 9
            return vec32x8u{_mm256_set_m128i(_mm_setzero_si128(), low_half)};
        } else {
            auto low_half = _mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr));
            auto high_half = decay(load<vec16x8u>(ptr + 16, n - 16));
            return vec32x8u{_mm256_set_m128i(high_half, low_half)};
        }

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u load<vec32x8u, vec32x8u::width>(const std::uint8_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec32x8u{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u aligned_load<vec32x8u>(const std::uint8_t* ptr, std::uint32_t n) {
        //TODO: Leverage AVX512VL aligned load?
        #if defined(AVEL_AVX2)
        if (n <= 16) {
            auto low_half = decay(load<vec16x8u>(ptr, n));
            // _mm256_zextsi128_si256 isn't recognized by GCC 9
            return vec32x8u{_mm256_set_m128i(_mm_setzero_si128(), low_half)};
        } else {
            auto low_half = _mm_load_si128(reinterpret_cast<const __m128i*>(ptr));
            auto high_half = decay(load<vec16x8u>(ptr + 16, n - 16));
            return vec32x8u{_mm256_set_m128i(high_half, low_half)};
        }
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8u aligned_load<vec32x8u, vec32x8u::width>(const std::uint8_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec32x8u{_mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }



    AVEL_FINL void store(std::uint8_t* ptr, vec32x8u v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = (std::uint64_t(1) << n) - 1;
        _mm256_mask_storeu_epi8(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n == 32) {
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        } else if (n <= 16) {
            vec16x8u lo{_mm256_extracti128_si256(decay(v), 0x0)};
            store(ptr, lo, n);
        } else {
            vec16x8u lo{_mm256_extracti128_si256(decay(v), 0x0)};
            vec16x8u hi{_mm256_extracti128_si256(decay(v), 0x1)};

            store(ptr + 0x00, lo);
            store(ptr + 0x10, hi, n - 16);
        }

        #endif
    }

    template<std::uint32_t N = vec32x8u::width>
    AVEL_FINL void store(std::uint8_t* ptr, vec32x8u v) {
        static_assert(N < vec32x8u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N < vec32x8u::width, int>::type dummy_variable = 0;

        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec32x8u::width>(std::uint8_t* ptr, vec32x8u v) {
        #if defined(AVEL_AVX2)
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(std::uint8_t* ptr, vec32x8u v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = (std::uint64_t(1) << n) - 1;
        _mm256_mask_storeu_epi8(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n == 32) {
            _mm256_store_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        } else if (n <= 16) {
            vec16x8u lo{_mm256_extracti128_si256(decay(v), 0x0)};
            aligned_store(ptr, lo, n);
        } else {
            vec16x8u lo{_mm256_extracti128_si256(decay(v), 0x0)};
            vec16x8u hi{_mm256_extracti128_si256(decay(v), 0x1)};

            aligned_store(ptr + 0x00, lo);
            aligned_store(ptr + 0x10, hi, n - 16);
        }

        #endif
    }

    template<std::uint32_t N = vec32x8u::width>
    AVEL_FINL void aligned_store(std::uint8_t* ptr, vec32x8u v) {
        static_assert(N < vec32x8u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N < vec32x8u::width, int>::type dummy_variable = 0;

        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec32x8u::width>(std::uint8_t* ptr, vec32x8u v) {
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL arr32x8u to_array(vec32x8u v) {
        alignas(32) arr32x8u ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec32x8u> div(vec32x8u x, vec32x8u y) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto quotient = _mm512_setzero_si512();
        auto widened_x = _mm512_cvtepu8_epi16(decay(x));
        auto widened_y = _mm512_cvtepu8_epi16(decay(y));

        auto bits = _mm512_set1_epi16(0x1);

        std::int32_t i = 8;
        for (; (i-- > 0) && 0x0000 != _mm512_cmpge_epi16_mask(widened_x, widened_y);) {
            auto count = _mm_cvtsi32_si128(i);
            auto shifted_widened_x = _mm512_srl_epi16(widened_x, count);
            auto b = _mm512_cmpge_epu16_mask(shifted_widened_x, widened_y);
            widened_x = _mm512_mask_sub_epi16(widened_x, b, widened_x, _mm512_sll_epi16(widened_y, count));

            quotient = _mm512_mask_add_epi16(quotient, b, quotient, _mm512_sll_epi16(bits, count));
        }

        return {
            vec32x8u{_mm512_cvtepi16_epi8(quotient)},
            vec32x8u{_mm512_cvtepi16_epi8(widened_x)}
        };

        #elif defined(AVEL_AVX2)
        vec32x8u quotient{};
        std::int32_t i = 8;

        //TODO: Optimize loop
        for (; (i-- > 0) && any(mask32x8u(x));) {
            mask32x8u b = ((x >> i) >= y);
            x -= (broadcast_mask(b) & (y << i));
            quotient |= (vec32x8u{b} << i);
        }

        return {quotient, x};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u popcount(vec32x8u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        return vec32x8u{_mm256_popcnt_epi8(decay(v))};

        #elif defined(AVEL_AVX2)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        auto table_lo = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto table = _mm256_set_m128i(table_lo, table_lo);
        auto nibble_mask = _mm256_set1_epi8(0x0F);

        auto index0 = _mm256_and_si256(decay(v), nibble_mask);
        auto index1 = _mm256_and_si256(_mm256_srli_epi16(decay(v), 0x4), nibble_mask);

        auto partial_sum0 = _mm256_shuffle_epi8(table, index0);
        auto partial_sum1 = _mm256_shuffle_epi8(table, index1);

        auto sum = _mm256_add_epi8(partial_sum0, partial_sum1);
        return vec32x8u{sum};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u countl_zero(vec32x8u v) {
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

        auto nibble_mask = _mm256_set1_epi8(0x0F);
        auto lo_nibble = _mm256_and_si256(nibble_mask, decay(v));
        auto hi_nibble = _mm256_and_si256(nibble_mask, _mm256_srli_epi16(decay(v), 0x4));

        auto table0 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm256_shuffle_epi8(table1, lo_nibble);
        auto partial1 = _mm256_shuffle_epi8(table0, hi_nibble);

        auto ret = _mm256_min_epu8(partial0, partial1);
        return vec32x8u{ret};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u countl_one(vec32x8u v) {
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

        auto nibble_mask = _mm256_set1_epi8(0x0F);
        auto lo_nibble = _mm256_and_si256(nibble_mask, decay(v));
        auto hi_nibble = _mm256_and_si256(nibble_mask, _mm256_srli_epi16(decay(v), 0x4));

        auto table0 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm256_shuffle_epi8(table1, lo_nibble);
        auto partial1 = _mm256_shuffle_epi8(table0, hi_nibble);

        auto ret = _mm256_min_epu8(partial0, partial1);
        return vec32x8u{ret};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u countr_zero(vec32x8u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        auto neg_one = _mm256_set1_epi8(-1);
        auto tz_mask = _mm256_andnot_si256(decay(v), _mm256_add_epi8(decay(v), neg_one));
        return vec32x8u{_mm256_popcnt_epi8(tz_mask)};

        #elif defined(AVEL_AVX2)
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

        auto nibble_mask = _mm256_set1_epi8(0x0F);
        auto lo_nibble = _mm256_and_si256(nibble_mask, decay(v));
        auto hi_nibble = _mm256_and_si256(nibble_mask, _mm256_srli_epi16(decay(v), 0x4));

        auto table0 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm256_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm256_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm256_min_epu8(partial0, partial1);
        return vec32x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u countr_one(vec32x8u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return countr_zero(~v);

        #elif defined(AVEL_AVX2)
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

        auto nibble_mask = _mm256_set1_epi8(0x0F);
        auto lo_nibble = _mm256_and_si256(nibble_mask, decay(v));
        auto hi_nibble = _mm256_and_si256(nibble_mask, _mm256_srli_epi16(decay(v), 0x4));

        auto table0 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm256_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm256_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm256_min_epu8(partial0, partial1);
        return vec32x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u bit_width(vec32x8u v) {
        #if defined(AVEL_AVX2)
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

        auto nibble_mask = _mm256_set1_epi8(0x0F);
        auto lo_nibble = _mm256_and_si256(nibble_mask, decay(v));
        auto hi_nibble = _mm256_and_si256(nibble_mask, _mm256_srli_epi16(decay(v), 0x4));

        auto table0 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm256_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm256_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm256_max_epu8(partial0, partial1);
        return vec32x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u bit_floor(vec32x8u v) {
        #if defined(AVEL_AVX2)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            0, 1, 2, 2,
            4, 4, 4, 4,
            8, 8, 8, 8,
            8, 8, 8, 8,
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
              0,  16,  32,  32,
             64,  64,  64,  64,
            128, 128, 128, 128,
            128, 128, 128, 128
        };

        auto nibble_mask = _mm256_set1_epi8(0x0F);
        auto lo_nibble = _mm256_and_si256(nibble_mask, decay(v));
        auto hi_nibble = _mm256_and_si256(nibble_mask, _mm256_srli_epi16(decay(v), 0x4));

        auto table0 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm256_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm256_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm256_max_epu8(partial0, partial1);
        return vec32x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8u bit_ceil(vec32x8u v) {
        #if defined(AVEL_AVX2)
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

        auto zero_mask = _mm256_cmpeq_epi8(decay(v), _mm256_setzero_si256());

        --v;

        auto nibble_mask = _mm256_set1_epi8(0x0F);
        auto lo_nibble = _mm256_and_si256(nibble_mask, decay(v));
        auto hi_nibble = _mm256_and_si256(nibble_mask, _mm256_srli_epi16(decay(v), 0x4));

        auto table0 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm256_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm256_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm256_add_epi8(_mm256_max_epu8(partial0, partial1), _mm256_set1_epi8(0x01));
        return vec32x8u{_mm256_sub_epi8(ret, zero_mask)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask32x8u has_single_bit(vec32x8u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return (popcount(v) == vec32x8u{1});

        #elif defined(AVEL_AVX2)
        return mask32x8u{v} && !mask32x8u{v & (v - vec32x8u{1})};

        #endif
    }

}

#endif //AVEL_VEC32X8U_HPP
