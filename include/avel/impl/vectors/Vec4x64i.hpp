#ifndef AVEL_VEC4X64I_HPP
#define AVEL_VEC4X64I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x64i = Vector<std::int64_t, 4>;
    using arr4x64i = std::array<std::int64_t, 4>;
    using mask4x64i = Vector_mask<std::int64_t, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec4x64i> div(vec4x64i x, vec4x64i y);
    vec4x64i set_bits(mask4x64i m);
    vec4x64i blend(mask4x64i m, vec4x64i a, vec4x64i b);
    vec4x64i negate(mask4x64i m, vec4x64i x);





    template<>
    class Vector_mask<std::int64_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        using primitive = __mmask8;
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
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content(p & 0x0F) {}
        #elif defined(AVEL_AVX2)
            content(p) {}
        #endif

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content(b ? -1 : 0) {}
        #elif defined(AVEL_AVX2)
            content(b ? _mm256_set1_epi64x(-1) : _mm256_setzero_si256()) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr4xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            content = __mmask8(_mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            auto expanded = _mm_cvtepi8_epi32(array_data);
            content = _mm_cmplt_epu32_mask(_mm_setzero_si128(), expanded);

            #elif defined(AVEL_AVX2)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            auto widened = _mm256_cvtepi8_epi64(array_data);
            content = _mm256_cmpgt_epi64(widened, _mm256_setzero_si256());

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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(lhs.content) == _mm256_movemask_epi8(rhs.content);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(lhs.content) != _mm256_movemask_epi8(rhs.content);
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content &= rhs.content;
            #elif defined(AVEL_AVX2)
            content = _mm256_and_si256(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content |= rhs.content;
            #elif defined(AVEL_AVX2)
            content = _mm256_or_si256(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content ^= rhs.content;
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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return Vector_mask{primitive(content ^ 0xf)};
            #elif defined(AVEL_AVX2)
            auto ones = _mm256_set1_epi64x(-1);
            return Vector_mask{_mm256_andnot_si256(content, ones)};
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
    AVEL_FINL std::array<mask4x64u, 1> convert<mask4x64u, mask4x64i>(mask4x64i m) {
        return {mask4x64u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x64i, 1> convert<mask4x64i, mask4x64u>(mask4x64u m) {
        return {mask4x64i(decay(m))};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask4x64i m) {
        return count(mask4x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x64i m) {
        return any(mask4x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x64i m) {
        return all(mask4x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x64i m) {
        return none(mask4x64u{m});
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL bool extract(mask4x64i m) {
        static_assert(N < mask4x64i::width, "Specified index does not exist");
        typename std::enable_if<N < mask4x64i::width, int>::type dummy_variable = 0;

        return extract<N>(mask4x64u{m});
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL mask4x64i insert(mask4x64i m, bool b) {
        static_assert(N < mask4x64i::width, "Specified index does not exist");
        typename std::enable_if<N < mask4x64i::width, int>::type dummy_variable = 0;

        return mask4x64i{insert<N>(mask4x64u{m}, b)};
    }





    template<>
    class Vector<std::int64_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int64_t;

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
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content(_mm256_maskz_set1_epi64(decay(m), 0x1)) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_sub_epi64(_mm256_setzero_si256(), decay(m))) {}
        #endif

        AVEL_FINL Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            content(_mm256_set1_epi64x(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr4x64i& arr):
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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmpeq_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpeq_epi64(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmpneq_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpeq_epi64(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmplt_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpgt_epi64(decay(rhs), decay(lhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmple_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpgt_epi64(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmpgt_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpgt_epi64(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmpge_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpgt_epi64(decay(rhs), decay(lhs))};

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
            content = _mm256_add_epi64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_sub_epi64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
            content = _mm256_mullo_epi64(content, rhs.content);

            #elif defined(AVEL_AVX2)
            //TODO: Consider alternative approach based on emulation using
            // 32-bit multiplication
            auto c0 = std::uint64_t(_mm256_extract_epi64(content, 0)) * std::uint64_t(_mm256_extract_epi64(decay(rhs), 0));
            auto c1 = std::uint64_t(_mm256_extract_epi64(content, 1)) * std::uint64_t(_mm256_extract_epi64(decay(rhs), 1));
            auto c2 = std::uint64_t(_mm256_extract_epi64(content, 2)) * std::uint64_t(_mm256_extract_epi64(decay(rhs), 2));
            auto c3 = std::uint64_t(_mm256_extract_epi64(content, 3)) * std::uint64_t(_mm256_extract_epi64(decay(rhs), 3));

            auto result = _mm256_set_epi64x(c3, c2, c1, c0);

            content = result;

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
            #if defined(AVEL_AVX2)
            content = _mm256_sll_epi64(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content = _mm256_sra_epi64(content, _mm_cvtsi64_si128(rhs));

            #elif defined(AVEL_AVX2)
            auto is_negative = _mm256_cmpgt_epi64(_mm256_setzero_si256(), content);
            content = _mm256_xor_si256(content, is_negative);
            content = _mm256_srl_epi64(content, _mm_cvtsi64_si128(rhs));
            content = _mm256_xor_si256(content, is_negative);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_sllv_epi64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content = _mm256_srav_epi64(content, decay(rhs));

            #elif defined(AVEL_AVX2)
            auto is_negative = _mm256_cmpgt_epi64(_mm256_setzero_si256(), content);
            content = _mm256_xor_si256(content, is_negative);
            content = _mm256_srlv_epi64(content, rhs.content);
            content = _mm256_xor_si256(content, is_negative);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return Vector{_mm256_ternarylogic_epi64(content, content, content, 0x1)};

            #elif defined(AVEL_AVX2)
            auto ones = _mm256_set1_epi64x(-1);
            return Vector{_mm256_andnot_si256(content, ones)};
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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_test_epi64_mask(content, content)};

            #else
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        4 * sizeof(std::int64_t) == sizeof(vec4x64i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x64u, 1> convert<vec4x64u, vec4x64i>(vec4x64i x) {
        return {vec4x64u{decay(x)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x64i, 1> convert<vec4x64i, vec4x64u>(vec4x64u x) {
        return {vec4x64i{decay(x)}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec4x64i operator-(vec4x64u v) {
        return vec4x64i{0x00} - vec4x64i{v};
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int64_t extract(vec4x64i v) {
        static_assert(N < 4, "Specified index does not exist");
        typename std::enable_if<N < 4, int>::type dummy_variable = 0;

        return static_cast<std::int64_t>(extract<N>(vec4x64u{v}));
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x64i insert(vec4x64i v, std::int64_t x) {
        static_assert(N < 4, "Specified index does not exist");
        typename std::enable_if<N < 4, int>::type dummy_variable = 0;

        return vec4x64i{insert<N>(vec4x64u{v}, static_cast<std::uint64_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL  vec4x64i bit_shift_left(vec4x64i v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        return vec4x64i{bit_shift_left<S>(vec4x64u{v})};
    }

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL  vec4x64i bit_shift_right(vec4x64i v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x64i{_mm256_srai_epi64(decay(v), S)};

        #elif defined(AVEL_AVX2)
        auto is_negative = _mm256_cmpgt_epi64(_mm256_setzero_si256(), decay(v));
        auto tmp = _mm256_xor_si256(decay(v), is_negative);
        tmp = _mm256_srli_epi64(tmp, S);
        tmp = _mm256_xor_si256(tmp, is_negative);
        return vec4x64i {tmp};
        #endif
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x64i rotl(vec4x64i v) {
        return vec4x64i{rotl<S>(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i rotl(vec4x64i v, long long s) {
        return vec4x64i{rotl(vec4x64u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i rotl(vec4x64i v, vec4x64i s) {
        return vec4x64i{rotl(vec4x64u{v}, vec4x64u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x64i rotr(vec4x64i v) {
        return vec4x64i{rotr<S>(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i rotr(vec4x64i v, long long s) {
        return vec4x64i{rotr(vec4x64u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i rotr(vec4x64i v, vec4x64i s) {
        return vec4x64i{rotr(vec4x64u{v}, vec4x64u{s})};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec4x64i x) {
        return count(vec4x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec4x64i x) {
        return any(vec4x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec4x64i x) {
        return all(vec4x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec4x64i x) {
        return none(vec4x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i set_bits(mask4x64i m) {
        return vec4x64i{set_bits(mask4x64u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i keep(mask4x64i m, vec4x64i v) {
        return vec4x64i{keep(mask4x64u{m}, vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i clear(mask4x64i m, vec4x64i v) {
        return vec4x64i{clear(mask4x64u{m}, vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i blend(mask4x64i m, vec4x64i a, vec4x64i b) {
        return vec4x64i{blend(mask4x64u{m}, vec4x64u{a}, vec4x64u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i byteswap(vec4x64i v) {
        return vec4x64i{byteswap(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i max(vec4x64i a, vec4x64i b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x64i{_mm256_max_epi64(decay(a), decay(b))};
        #elif defined(AVEL_AVX2)
        return blend(a < b, b, a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i min(vec4x64i a, vec4x64i b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x64i{_mm256_min_epi64(decay(a), decay(b))};
        #elif defined(AVEL_AVX2)
        return blend(a < b, a, b);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x64i, 2> minmax(vec4x64i a, vec4x64i b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return {
            vec4x64i{_mm256_min_epi64(decay(a), decay(b))},
            vec4x64i{_mm256_max_epi64(decay(a), decay(b))}
        };
        #elif defined(AVEL_AVX2)
        auto m = a < b;
        return {
            blend(m, a, b),
            blend(m, b, a)
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i clamp(vec4x64i x, vec4x64i lo, vec4x64i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i average(vec4x64i a, vec4x64i b) {
        #if defined(AVEL_AVX2)
        auto avg = (a & b) + ((a ^ b) >> 1);
        auto c = set_bits((a < -b) | (b == vec4x64i{std::int64_t(1) << 63})) & (a ^ b) & vec4x64i{1};

        return avg + c;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i midpoint(vec4x64i a, vec4x64i b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto avg = decay(((a ^ b) >> 1) + (a & b));

        auto bias = _mm256_ternarylogic_epi32(decay(a), decay(b), _mm256_set1_epi64x(0x1), 0x28);
        auto mask = _mm256_cmplt_epi64_mask(decay(b), decay(a));
        auto ret = _mm256_mask_add_epi64(avg, mask, avg, bias);

        return vec4x64i{ret};

        #elif defined(AVEL_AVX2)
        auto average = ((a ^ b) >> 1) + (a & b);
        auto bias = (set_bits(b < a) & (a ^ b) & vec4x64i{0x1});
        return average + bias;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i negate(mask4x64i m, vec4x64i v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x64i{_mm256_mask_sub_epi64(decay(v), decay(m), _mm256_setzero_si256(), decay(v))};

        #elif defined(AVEL_AVX2)
        auto mask = set_bits(m);
        return (v ^ mask) - mask;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i abs(vec4x64i v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return _mm256_abs_epi64(decay(v));

        #elif defined(AVEL_AVX2)
        auto y = v >> 63;
        return (v ^ y) - y;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i neg_abs(vec4x64i v) {
        return -abs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i neg_abs(vec4x64u v) {
        return neg_abs(vec4x64i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64i load<vec4x64i>(const std::int64_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        return vec4x64i{_mm256_maskz_loadu_epi64(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n > 2) {
            auto lo = load<vec2x64i>(ptr + 0, n - 0);
            auto hi = load<vec2x64i>(ptr + 2, n - 2);
            return vec4x64i{_mm256_set_m128i(decay(hi), decay(lo))};
        } else {
            auto lo = load<vec2x64i>(ptr, n);
            return vec4x64i{_mm256_set_m128i(_mm_setzero_si128(), decay(lo))};
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64i load<vec4x64i, vec4x64i::width>(const std::int64_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec4x64i{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64i aligned_load<vec4x64i>(const std::int64_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        return vec4x64i{_mm256_maskz_load_epi64(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n > 2) {
            auto lo = aligned_load<vec2x64i>(ptr + 0, n - 0);
            auto hi = aligned_load<vec2x64i>(ptr + 2, n - 2);
            return vec4x64i{_mm256_set_m128i(decay(hi), decay(lo))};
        } else {
            auto lo = aligned_load<vec2x64i>(ptr, n);
            return vec4x64i{_mm256_set_m128i(_mm_setzero_si128(), decay(lo))};
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64i aligned_load<vec4x64i, vec4x64i::width>(const std::int64_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec4x64i{_mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u gather<vec4x64u>(const std::uint64_t* ptr, vec4x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        return vec4x64u{
            _mm256_mmask_i64gather_epi64(
                _mm256_setzero_si256(),
                mask,
                decay(indices),
                bit_cast<const std::int64_t*>(ptr),
                sizeof(std::uint64_t)
            )
        };

        #elif defined(AVEL_AVX2)
        std::uint64_t c0 = 0;
        std::uint64_t c1 = 0;
        std::uint64_t c2 = 0;
        std::uint64_t c3 = 0;

        switch (n) {
            default: { c3 = ptr[extract<3>(indices)]; }
            case 3:  { c2 = ptr[extract<2>(indices)]; }
            case 2:  { c1 = ptr[extract<1>(indices)]; }
            case 1:  { c0 = ptr[extract<0>(indices)]; }
            case 0:  { /* Do nothing */ }
        }

        return vec4x64u{_mm256_set_epi64x(c3, c2, c1, c0)};

        #endif
    }

    template<std::uint32_t N = vec4x64u::width>
    AVEL_FINL vec4x64u gather(const std::uint64_t* ptr, vec4x64i indices) {
        return gather<vec4x64u>(ptr, indices, N);
    }

    template<>
    AVEL_FINL vec4x64u gather<0>(const std::uint64_t* ptr, vec4x64i indices) {
        return vec4x64u{0x00};
    }

    template<>
    AVEL_FINL vec4x64u gather<vec4x64u::width>(const std::uint64_t* ptr, vec4x64i indices) {
        return vec4x64u{_mm256_i64gather_epi64(reinterpret_cast<const long long*>(ptr), decay(indices), sizeof(std::uint64_t))};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64i gather<vec4x64i>(const std::int64_t* ptr, vec4x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        return vec4x64i{
            _mm256_mmask_i64gather_epi64(
                _mm256_setzero_si256(),
                mask,
                decay(indices),
                bit_cast<const std::int64_t*>(ptr),
                sizeof(std::uint64_t)
            )
        };

        #elif defined(AVEL_AVX2)
        std::uint64_t c0 = 0;
        std::uint64_t c1 = 0;
        std::uint64_t c2 = 0;
        std::uint64_t c3 = 0;

        switch (n) {
            default: { c3 = ptr[extract<3>(indices)]; }
            case 3:  { c2 = ptr[extract<2>(indices)]; }
            case 2:  { c1 = ptr[extract<1>(indices)]; }
            case 1:  { c0 = ptr[extract<0>(indices)]; }
            case 0:  { /* Do nothing */ }
        }

        return vec4x64i{_mm256_set_epi64x(c3, c2, c1, c0)};

        #endif
    }

    template<std::uint32_t N = vec4x64i::width>
    AVEL_FINL vec4x64i gather(std::int64_t* ptr, vec4x64i indices) {
        return gather<vec4x64i>(ptr, indices, N);
    }

    template<>
    AVEL_FINL vec4x64i gather<0>(std::int64_t* ptr, vec4x64i indices) {
        return vec4x64i{0x00};
    }


    template<>
    AVEL_FINL vec4x64i gather<vec4x64i::width>(std::int64_t* ptr, vec4x64i indices) {
        return vec4x64i{_mm256_i64gather_epi64(reinterpret_cast<long long*>(ptr), decay(indices), sizeof(std::int64_t))};
    }



    AVEL_FINL void store(std::int64_t* ptr, vec4x64i v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        _mm256_mask_storeu_epi64(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n > 2) {
            store(ptr + 0, vec2x64i{_mm256_castsi256_si128(decay(v))}, n - 0);
            store(ptr + 2, vec2x64i{_mm256_extracti128_si256(decay(v), 0x1)}, n - 2);
        } else {
            store(ptr, vec2x64i{_mm256_castsi256_si128(decay(v))}, n);
        }

        #endif
    }

    template<std::uint32_t N = vec4x64i::width>
    AVEL_FINL void store(std::int64_t* ptr, vec4x64i v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec4x64i::width>(std::int64_t* ptr, vec4x64i v) {
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
    }



    AVEL_FINL void aligned_store(std::int64_t* ptr, vec4x64i v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        _mm256_mask_store_epi64(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n > 2) {
            aligned_store(ptr + 0, vec2x64i{_mm256_castsi256_si128(decay(v))}, n - 0);
            aligned_store(ptr + 2, vec2x64i{_mm256_extracti128_si256(decay(v), 0x1)}, n - 2);
        } else {
            aligned_store(ptr, vec2x64i{_mm256_castsi256_si128(decay(v))}, n);
        }

        #endif
    }

    template<std::uint32_t N = vec4x64i::width>
    AVEL_FINL void aligned_store(std::int64_t* ptr, vec4x64i v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec4x64i::width>(std::int64_t* ptr, vec4x64i v) {
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }


    
    AVEL_FINL void scatter(std::int64_t* ptr, vec4x64i v, vec4x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        _mm256_mask_i64scatter_epi64(ptr, mask, decay(indices), decay(v), sizeof(std::int64_t));

        #elif defined(AVEL_AVX2)
        switch (n) {
            default: ptr[extract<3>(indices)] = extract<3>(v);
            case 3:  ptr[extract<2>(indices)] = extract<2>(v);
            case 2:  ptr[extract<1>(indices)] = extract<1>(v);
            case 1:  ptr[extract<0>(indices)] = extract<0>(v);
            case 0: ; //Do nothing
        }

        #endif
    }

    template<std::uint32_t N = vec4x64i::width>
    AVEL_FINL void scatter(std::int64_t* ptr, vec4x64i v, vec4x64i indices) {
        scatter(ptr, v, indices, N);
    }

    template<>
    AVEL_FINL void scatter<0>(std::int64_t* ptr, vec4x64i v, vec4x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec4x64i::width>(std::int64_t* ptr, vec4x64i v, vec4x64i indices) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        _mm256_i64scatter_epi64(ptr, decay(indices), decay(v), sizeof(std::int64_t));

        #elif defined(AVEL_AVX2)
        ptr[extract<3>(indices)] = extract<3>(v);
        ptr[extract<2>(indices)] = extract<2>(v);
        ptr[extract<1>(indices)] = extract<1>(v);
        ptr[extract<0>(indices)] = extract<0>(v);

        #endif
    }



    AVEL_FINL void scatter(std::uint64_t* ptr, vec4x64u v, vec4x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        _mm256_mask_i64scatter_epi64(ptr, mask, decay(indices), decay(v), sizeof(std::int64_t));

        #elif defined(AVEL_AVX2)
        switch (n) {
            default: ptr[extract<3>(indices)] = extract<3>(v);
            case 3:  ptr[extract<2>(indices)] = extract<2>(v);
            case 2:  ptr[extract<1>(indices)] = extract<1>(v);
            case 1:  ptr[extract<0>(indices)] = extract<0>(v);
            case 0: ; //Do nothing
        }

        #endif
    }

    template<std::uint32_t N = vec4x64i::width>
    AVEL_FINL void scatter(std::uint64_t* ptr, vec4x64u v, vec4x64i indices) {
        scatter(ptr, v, indices, N);
    }

    template<>
    AVEL_FINL void scatter<0>(std::uint64_t* ptr, vec4x64u v, vec4x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec4x64i::width>(std::uint64_t* ptr, vec4x64u v, vec4x64i indices) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        _mm256_i64scatter_epi64(ptr, decay(indices), decay(v), sizeof(std::int64_t));

        #elif defined(AVEL_AVX2)
        ptr[extract<3>(indices)] = extract<3>(v);
        ptr[extract<2>(indices)] = extract<2>(v);
        ptr[extract<1>(indices)] = extract<1>(v);
        ptr[extract<0>(indices)] = extract<0>(v);

        #endif
    }




    [[nodiscard]]
    AVEL_FINL arr4x64i to_array(vec4x64i v) {
        alignas(32) arr4x64i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec4x64i> div(vec4x64i x, vec4x64i y) {
        auto n0 = extract<0>(x);
        auto n1 = extract<1>(x);
        auto n2 = extract<2>(x);
        auto n3 = extract<3>(x);

        auto d0 = extract<0>(y);
        auto d1 = extract<1>(y);
        auto d2 = extract<2>(y);
        auto d3 = extract<3>(y);

        vec4x64i quotient{};
        quotient = insert<0>(quotient, n0 / d0);
        quotient = insert<1>(quotient, n1 / d1);
        quotient = insert<2>(quotient, n2 / d2);
        quotient = insert<3>(quotient, n3 / d3);

        vec4x64i remainder{};
        remainder = insert<0>(remainder, n0 % d0);
        remainder = insert<1>(remainder, n1 % d1);
        remainder = insert<2>(remainder, n2 % d2);
        remainder = insert<3>(remainder, n3 % d3);

        return {quotient, remainder};
    }

    AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(vec4x64i, mask4x64i, vec4x64u)

    [[nodiscard]]
    AVEL_FINL vec4x64i countl_sign(vec4x64i v) {
        return avel::countl_zero((v ^ avel::bit_shift_right<1>(v))) - vec4x64i{1};
    }

}

#endif //AVEL_VEC4X64I_HPP
