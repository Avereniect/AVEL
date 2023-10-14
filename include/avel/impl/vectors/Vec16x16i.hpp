#ifndef AVEL_VEC16X16I_HPP
#define AVEL_VEC16X16I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x16i = Vector<std::int16_t, 16>;
    using arr16x16i = std::array<std::int16_t, 16>;
    using mask16x16i = Vector_mask<std::int16_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x16i> div(vec16x16i x, vec16x16i y);
    vec16x16i set_bits(mask16x16i m);
    vec16x16i blend(mask16x16i m, vec16x16i a, vec16x16i b);
    vec16x16i negate(mask16x16i m, vec16x16i x);





    template<>
    class Vector_mask<std::int16_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
        using primitive = __mmask16;
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
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            content(b ? -1 : 0) {}
        #elif defined(AVEL_AVX2)
            content(b ? _mm256_set1_epi16(-1) : _mm256_setzero_si256()) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr16xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = _mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data);

            #elif defined(AVEL_AVX2)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));

            auto widened = _mm256_cvtepu8_epi16(array_data);
            content = _mm256_cmpgt_epi16(widened, _mm256_setzero_si256());

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

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(decay(lhs)) == _mm256_movemask_epi8(decay(rhs));

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_SSE2)
            return _mm256_movemask_epi8(decay(lhs)) != _mm256_movemask_epi8(decay(rhs));

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            content &= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm256_and_si256(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            content |= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm256_or_si256(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            content ^= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm256_xor_si256(content, rhs.content);

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return Vector_mask{primitive(_knot_mask16(content))};

            #elif defined(AVEL_AVX512VL)
            return Vector_mask{_mm256_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm256_undefined_si256();
            return Vector_mask{_mm256_andnot_si256(content, _mm256_cmpeq_epi16(undef, undef))};

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
    AVEL_FINL std::array<mask16x16u, 1> convert<mask16x16u, mask16x16i>(mask16x16i m) {
        return {mask16x16u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x16i, 1> convert<mask16x16i, mask16x16u>(mask16x16u m) {
        return {mask16x16i(decay(m))};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask16x16i m) {
        return count(mask16x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask16x16i m) {
        return any(mask16x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask16x16i m) {
        return all(mask16x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x16i m) {
        return none(mask16x16u{m});
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL bool extract(mask16x16i m) {
        static_assert(N < mask16x16i::width, "Specified index does not exist");
        typename std::enable_if<N < mask16x16i::width, int>::type dummy_variable = 0;

        return extract<N>(mask16x16u{m});
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL mask16x16i insert(mask16x16i m, bool b) {
        static_assert(N < mask16x16i::width, "Specified index does not exist");
        typename std::enable_if<N < mask16x16i::width, int>::type dummy_variable = 0;

        return mask16x16i{insert<N>(mask16x16u{m}, b)};
    }





    template<>
    class Vector<std::int16_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int16_t;

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
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            content(_mm256_maskz_set1_epi16(decay(m), 0x1)) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_sub_epi16(_mm256_setzero_si256(), decay(m))) {}
        #endif

        AVEL_FINL Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            content(_mm256_set1_epi16(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr16x16i& arr):
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
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmpeq_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpeq_epi16(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmpneq_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpeq_epi16(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmplt_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpgt_epi16(rhs.content, lhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmple_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpgt_epi16(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmpgt_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpgt_epi16(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmpge_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpgt_epi16(rhs.content, lhs.content)};
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
            #if defined(AVEL_AVX2)
            content = _mm256_add_epi16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_sub_epi16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_mullo_epi16(content, rhs.content);
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
            content = _mm256_and_si256(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_or_si256(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_xor_si256(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_AVX2)
            content  = _mm256_sll_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX2)
            content  = _mm256_sra_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            content = _mm256_sllv_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto even_mask = _mm256_set1_epi32(0x0000FFFF);

            auto rhs_even = _mm256_and_si256(even_mask, decay(rhs));
            auto even_result32 = _mm256_sllv_epi32(content, rhs_even);
            auto even_result = _mm256_and_si256(even_mask, even_result32);

            auto rhs_odd = _mm256_srli_epi32(decay(rhs), 16);
            auto lhs_odd_32 = _mm256_andnot_si256(even_mask, content);
            auto odd_result = _mm256_sllv_epi32(lhs_odd_32, rhs_odd);

            auto result = _mm256_or_si256(even_result, odd_result);
            content = result;

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            content = _mm256_srav_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto even_mask = _mm256_set1_epi32(0x0000FFFF);

            auto rhs_even = _mm256_and_si256(even_mask, decay(rhs));
            auto lhs_even = _mm256_slli_epi32(content, 16);
            auto result_even = _mm256_srli_epi32(_mm256_srav_epi32(lhs_even, rhs_even), 16);

            auto rhs_odd = _mm256_srli_epi32(decay(rhs), 16);
            auto result_odd32 = _mm256_srav_epi32(content, rhs_odd);
            auto result_odd = _mm256_andnot_si256(even_mask, result_odd32);

            auto result = _mm256_or_si256(result_odd, result_even);
            content = result;

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
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
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return mask{_mm256_test_epi16_mask(content, content)};

            #elif defined(AVEL_AVX2)
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        16 * sizeof(std::int16_t) == sizeof(vec16x16i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x16u, 1> convert<vec16x16u, vec16x16i>(vec16x16i x) {
        return {vec16x16u{decay(x)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x16i, 1> convert<vec16x16i, vec16x16u>(vec16x16u x) {
        return {vec16x16i{decay(x)}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec16x16i operator-(vec16x16u v) {
        return vec16x16i{0x00} - vec16x16i{v};
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int16_t extract(vec16x16i v) {
        static_assert(N < 16, "Specified index does not exist");
        typename std::enable_if<N < 16, int>::type dummy_variable = 0;

        return static_cast<std::int16_t>(extract<N>(vec16x16u{v}));
    }

    template<std::uint32_t N>
    AVEL_FINL vec16x16i insert(vec16x16i v, std::int16_t x) {
        static_assert(N < 16, "Specified index does not exist");
        typename std::enable_if<N < 16, int>::type dummy_variable = 0;

        return vec16x16i{insert<N>(vec16x16u{v}, static_cast<std::uint16_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL  vec16x16i bit_shift_left(vec16x16i v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        return vec16x16i{bit_shift_left<S>(vec16x16u{v})};
    }

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL  vec16x16i bit_shift_right(vec16x16i v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return vec16x16i{_mm256_srai_epi16(decay(v), S)};
        #endif
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x16i rotl(vec16x16i v) {
        return vec16x16i{rotl<S>(vec16x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i rotl(vec16x16i v, long long s) {
        return vec16x16i{rotl(vec16x16u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i rotl(vec16x16i v, vec16x16i s) {
        return vec16x16i{rotl(vec16x16u{v}, vec16x16u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x16i rotr(vec16x16i v) {
        return vec16x16i{rotr<S>(vec16x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i rotr(vec16x16i v, long long s) {
        return vec16x16i{rotr(vec16x16u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i rotr(vec16x16i v, vec16x16i s) {
        return vec16x16i{rotr(vec16x16u{v}, vec16x16u{s})};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec16x16i v) {
        return count(vec16x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec16x16i v) {
        return any(vec16x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec16x16i v) {
        return all(vec16x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec16x16i v) {
        return none(vec16x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i set_bits(mask16x16i m) {
        return vec16x16i{set_bits(mask16x16u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i keep(mask16x16i m, vec16x16i v) {
        return vec16x16i{keep(mask16x16u{m}, vec16x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i clear(mask16x16i m, vec16x16i v) {
        return vec16x16i{clear(mask16x16u{m}, vec16x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i blend(mask16x16i m, vec16x16i a, vec16x16i b) {
        return vec16x16i{blend(mask16x16u{m}, vec16x16u{a}, vec16x16u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i byteswap(vec16x16i v) {
        return vec16x16i{byteswap(vec16x16u(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i max(vec16x16i a, vec16x16i b) {
        #if defined(AVEL_AVX2)
        return vec16x16i{_mm256_max_epi16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i min(vec16x16i a, vec16x16i b) {
        #if defined(AVEL_AVX2)
        return vec16x16i{_mm256_min_epi16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec16x16i, 2> minmax(vec16x16i a, vec16x16i b) {
        #if defined(AVEL_AVX2)
        return std::array<vec16x16i, 2>{
            vec16x16i{_mm256_min_epi16(decay(a), decay(b))},
            vec16x16i{_mm256_max_epi16(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i clamp(vec16x16i x, vec16x16i lo, vec16x16i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i average(vec16x16i a, vec16x16i b) {
        #if defined(AVEL_AVX2)
        auto avg = (a & b) + ((a ^ b) >> 1);
        auto c = set_bits((a < -b) | (b == vec16x16i{std::int16_t(1 << 15)})) & (a ^ b) & vec16x16i{1};
        return avg + c;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i midpoint(vec16x16i a, vec16x16i b) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
        auto offset = _mm256_set1_epi16(0x8000);

        auto a_offset = _mm256_xor_si256(decay(a), offset);
        auto b_offset = _mm256_xor_si256(decay(b), offset);

        auto average_offset = _mm256_avg_epu16(a_offset, b_offset);
        auto average = _mm256_xor_si256(average_offset, offset);

        auto bias = _mm256_ternarylogic_epi32(decay(a), decay(b), _mm256_set1_epi16(0x1), 0x28);
        auto mask = _mm256_cmplt_epi16_mask(decay(a), decay(b));
        auto ret = _mm256_mask_sub_epi16(average, mask, average, bias);

        return vec16x16i{ret};

        #elif defined(AVEL_AVX2)
        auto offset = _mm256_set1_epi16(0x8000);

        auto a_offset = _mm256_xor_si256(decay(a), offset);
        auto b_offset = _mm256_xor_si256(decay(b), offset);

        auto average_offset = _mm256_avg_epu16(a_offset, b_offset);
        auto average = _mm256_xor_si256(average_offset, offset);

        auto m = _mm256_cmpgt_epi16(decay(b), decay(a));
        auto bias = _mm256_and_si256(_mm256_xor_si256(decay(a), decay(b)), _mm256_and_si256(m, _mm256_set1_epi16(0x01)));

        return vec16x16i{_mm256_sub_epi16(average, bias)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i negate(mask16x16i m, vec16x16i v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
        return vec16x16i{_mm256_mask_sub_epi16(decay(v), decay(m), _mm256_setzero_si256(), decay(v))};

        #elif defined(AVEL_AVX2)
        return vec16x16i{_mm256_sign_epi16(decay(v), _mm256_or_si256(decay(m), _mm256_set1_epi16(0x01)))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i abs(vec16x16i v) {
        #if defined(AVEL_AVX2)
        return vec16x16i{_mm256_abs_epi16(decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i neg_abs(vec16x16i v) {
        #if defined(AVEL_AVX2)
        return -vec16x16i{_mm256_abs_epi16(decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16i neg_abs(vec16x16u v) {
        #if defined(AVEL_AVX2)
        return -vec16x16i{_mm256_abs_epi16(decay(v))};
        #endif
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16i load<vec16x16i>(const std::int16_t* ptr, std::uint32_t n) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
        auto mask = (n >= 16) ? -1 : (1 << n)  - 1;
        return vec16x16i{_mm256_maskz_loadu_epi16(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n > 8) {
            auto lo = load<vec8x16i>(ptr + 0, n - 0);
            auto hi = load<vec8x16i>(ptr + 8, n - 8);
            auto ret = _mm256_set_m128i(decay(hi), decay(lo));
            return vec16x16i{ret};
        } else {
            auto lo = load<vec8x16i>(ptr, n);
            auto ret = _mm256_set_m128i(_mm_setzero_si128(), decay(lo));
            return vec16x16i{ret};
        }
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16i load<vec16x16i, vec16x16i::width>(const std::int16_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec16x16i{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16i aligned_load<vec16x16i>(const std::int16_t* ptr, std::uint32_t n) {
        return load<vec16x16i>(ptr, n);
    }

    // Add specializations using 32-bit aligned loads

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16i aligned_load<vec16x16i, vec16x16i::width>(const std::int16_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec16x16i{_mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }



    AVEL_FINL void store(std::int16_t* ptr, vec16x16i v, std::uint32_t n) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm256_mask_storeu_epi16(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n > 8) {
            store(ptr + 0, vec8x16i{_mm256_castsi256_si128(decay(v))}, n - 0);
            store(ptr + 8, vec8x16i{_mm256_extracti128_si256(decay(v), 0x1)}, n - 8);
        } else {
            store(ptr, vec8x16i{_mm256_castsi256_si128(decay(v))}, n);
        }

        #endif
    }

    template<std::uint32_t N = vec16x16i::width>
    AVEL_FINL void store(std::int16_t* ptr, vec16x16i v) {
        store(ptr, v, N);
    }

    // Add specializations using 32-bit aligned stores

    template<>
    AVEL_FINL void store<vec16x16i::width>(std::int16_t* ptr, vec16x16i v) {
        #if defined(AVEL_AVX2)
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(std::int16_t* ptr, vec16x16i v, std::uint32_t n) {
        store(ptr, v, n);
    }

    template<std::uint32_t N = vec16x16i::width>
    AVEL_FINL void aligned_store(std::int16_t* ptr, vec16x16i v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec16x16i::width>(std::int16_t* ptr, vec16x16i v) {
        #if defined(AVEL_AVX2)
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr16x16i to_array(vec16x16i v) {
        alignas(32) arr16x16i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec16x16i> div(vec16x16i x, vec16x16i y) {
        mask16x16i remainder_sign_mask = (x < vec16x16i{0x00});
        mask16x16i quotient_sign_mask = remainder_sign_mask ^ (y < vec16x16i{0x00});

        vec16x16u numerator{abs(x)};
        vec16x16u denominator{abs(y)};

        auto results = div(numerator, denominator);

        return {
            negate(quotient_sign_mask,  vec16x16i{results.quot}),
            negate(remainder_sign_mask, vec16x16i{results.rem})
        };
    }

    AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(vec16x16i, mask16x16i, vec16x16u)

}

#endif //AVEL_VEC16X16I_HPP
