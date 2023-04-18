#ifndef AVEL_VEC32X16I_HPP
#define AVEL_VEC32X16I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec32x16i = Vector<std::int16_t, 32>;
    using arr32x16i = std::array<std::int16_t, 32>;
    using mask32x16i = Vector_mask<std::int16_t, 32>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec32x16i> div(vec32x16i numerator, vec32x16i denominator);
    vec32x16i broadcast_mask(mask32x16i m);
    vec32x16i blend(mask32x16i m, vec32x16i a, vec32x16i b);
    vec32x16i negate(mask32x16i m, vec32x16i x);





    template<>
    class Vector_mask<std::int16_t, 32> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 32;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512BW)
        using primitive = __mmask32;
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
            content(b ? 0xFFFFFFFF : 0) {}

        AVEL_FINL explicit Vector_mask(const arr32xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()));
            content = _mm256_cmplt_epu8_mask(_mm256_setzero_si256(), array_data);

            #elif defined(AVEL_AVX512BW)
            auto array_data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()));
            auto widened = _mm512_castsi256_si512(array_data);
            content = _mm512_cmplt_epu8_mask(_mm512_setzero_si512(), widened);

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
            #if defined(AVEL_AVX512BW)
            auto tmp = _kxor_mask32(decay(lhs), decay(rhs));
            return _kortestz_mask32_u8(tmp, tmp);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512BW)
            auto tmp = _kxor_mask32(decay(lhs), decay(rhs));
            return !_kortestz_mask32_u8(tmp, tmp);
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512BW)
            content &= decay(rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512BW)
            content |= decay(rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512BW)
            content ^= decay(rhs);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512BW)
            return Vector_mask{~content};
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
    AVEL_FINL std::array<mask32x16u, 1> convert<mask32x16u, mask32x16i>(mask32x16i m) {
        return {mask32x16u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask32x16i, 1> convert<mask32x16i, mask32x16u>(mask32x16u m) {
        return {mask32x16i(decay(m))};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask32x16i m) {
        return count(mask32x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask32x16i m) {
        return any(mask32x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask32x16i m) {
        return all(mask32x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask32x16i m) {
        return none(mask32x16u{m});
    }





    template<>
    class Vector<std::int16_t, 32> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 32;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int16_t;

        #if defined(AVEL_AVX512BW)
        using primitive = __m512i;
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
        #if defined(AVEL_AVX512BW)
            content(_mm512_maskz_set1_epi16(decay(m), 0x1)) {}
        #endif

        AVEL_FINL Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX512BW)
            content(_mm512_set1_epi16(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr32x16i& arr):
        #if defined(AVEL_AVX512BW)
            content(_mm512_loadu_si512(arr.data())) {}
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
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpeq_epi16_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpneq_epi16_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmplt_epi16_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmple_epi16_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpgt_epi16_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpge_epi16_mask(decay(lhs), decay(rhs))};
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
            #if defined(AVEL_AVX512BW)
            content = _mm512_add_epi16(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_sub_epi16(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_mullo_epi16(content, decay(rhs));
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
            #if defined(AVEL_AVX512BW)
            content = _mm512_and_si512(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_or_si512(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_xor_si512(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_sll_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_sra_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_sllv_epi16(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_srav_epi16(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512BW)
            return Vector{_mm512_ternarylogic_epi32(content, content, content, 0x1)};
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
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_test_epi16_mask(content, content)};
            #endif
        }

    };

    static_assert(
        32 * sizeof(std::int16_t) == sizeof(vec32x16i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec32x16u, 1> convert<vec32x16u, vec32x16i>(vec32x16i x) {
        return {vec32x16u{decay(x)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec32x16i, 1> convert<vec32x16i, vec32x16u>(vec32x16u x) {
        return {vec32x16i{decay(x)}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec32x16i operator-(vec32x16u v) {
        return vec32x16i{0x00} - vec32x16i{v};
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int16_t extract(vec32x16i v) {
        static_assert(N < 32, "Specified index does not exist");
        typename std::enable_if<N < 32, int>::type dummy_variable = 0;

        return static_cast<std::int16_t>(extract<N>(vec32x16u{v}));
    }

    template<std::uint32_t N>
    AVEL_FINL vec32x16i insert(vec32x16i v, std::int16_t x) {
        static_assert(N < 32, "Specified index does not exist");
        typename std::enable_if<N < 32, int>::type dummy_variable = 0;

        return vec32x16i{insert<N>(vec32x16u{v}, static_cast<std::uint16_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL  vec32x16i bit_shift_left(vec32x16i v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        return vec32x16i{bit_shift_left<S>(vec32x16u{v})};
    }

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL  vec32x16i bit_shift_right(vec32x16i v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        return vec32x16i{_mm512_srai_epi16(decay(v), S)};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec32x16i rotl(vec32x16i v) {
        return vec32x16i{rotl<S>(vec32x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i rotl(vec32x16i v, long long s) {
        return vec32x16i{rotl(vec32x16u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i rotl(vec32x16i v, vec32x16i s) {
        return vec32x16i{rotl(vec32x16u{v}, vec32x16u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec32x16i rotr(vec32x16i v) {
        return vec32x16i{rotr<S>(vec32x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i rotr(vec32x16i v, long long s) {
        return vec32x16i{rotr(vec32x16u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i rotr(vec32x16i v, vec32x16i s) {
        return vec32x16i{rotr(vec32x16u{v}, vec32x16u{s})};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec32x16i x) {
        return count(vec32x16u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec32x16i x) {
        return any(vec32x16u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec32x16i x) {
        return all(vec32x16u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec32x16i x) {
        return none(vec32x16u{x});
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i broadcast_mask(mask32x16i m) {
        return vec32x16i{broadcast_mask(mask32x16u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i keep(mask32x16i m, vec32x16i v) {
        return vec32x16i{keep(mask32x16u{m}, vec32x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i clear(mask32x16i m, vec32x16i v) {
        return vec32x16i{clear(mask32x16u{m}, vec32x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i blend(mask32x16i m, vec32x16i a, vec32x16i b) {
        return vec32x16i{blend(mask32x16u{m}, vec32x16u{a}, vec32x16u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i byteswap(vec32x16i v) {
        return vec32x16i{byteswap(vec32x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i max(vec32x16i a, vec32x16i b) {
        #if defined(AVEL_AVX512BW)
        return vec32x16i{_mm512_max_epi16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i min(vec32x16i a, vec32x16i b) {
        #if defined(AVEL_AVX512BW)
        return vec32x16i{_mm512_min_epi16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec32x16i, 2> minmax(vec32x16i a, vec32x16i b) {
        #if defined(AVEL_AVX512BW)
        return {
            vec32x16i{_mm512_min_epi16(decay(a), decay(b))},
            vec32x16i{_mm512_max_epi16(decay(a), decay(b))},
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i clamp(vec32x16i x, vec32x16i lo, vec32x16i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i average(vec32x16i a, vec32x16i b) {
        #if defined(AVEL_AVX512BW)
        auto avg = (a & b) + ((a ^ b) >> 1);
        auto c = broadcast_mask((a < -b) | (b == vec32x16i{std::int16_t(1 << 15)})) & (a ^ b) & vec32x16i{1};
        return avg + c;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i midpoint(vec32x16i a, vec32x16i b) {
        #if defined(AVEL_AVX512BW)
        auto offset = _mm512_set1_epi16(0x8000);

        auto a_offset = _mm512_xor_si512(decay(a), offset);
        auto b_offset = _mm512_xor_si512(decay(b), offset);

        auto average_offset = _mm512_avg_epu16(a_offset, b_offset);
        auto average = _mm512_xor_si512(average_offset, offset);

        auto bias = _mm512_ternarylogic_epi32(decay(a), decay(b), _mm512_set1_epi16(0x1), 0x28);
        auto mask = _mm512_cmplt_epi16_mask(decay(a), decay(b));
        auto ret = _mm512_mask_sub_epi16(average, mask, average, bias);
        return vec32x16i{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i negate(mask32x16i m, vec32x16i v) {
        #if defined(AVEL_AVX512BW)
        return vec32x16i{_mm512_mask_sub_epi16(decay(v), decay(m), _mm512_setzero_si512(), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i abs(vec32x16i v) {
        #if defined(AVEL_AVX512BW)
        return vec32x16i{_mm512_abs_epi16(decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i neg_abs(vec32x16i v) {
        return -abs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec32x16i neg_abs(vec32x16u v) {
        return neg_abs(vec32x16i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16i load<vec32x16i>(const std::int16_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512BW)
        auto mask = (n >= 32) ? -1 : (1 << n)  - 1;
        return vec32x16i{_mm512_maskz_loadu_epi16(mask, ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16i load<vec32x16i, vec32x16i::width>(const std::int16_t* ptr) {
        #if defined(AVEL_AVX512BW)
        return vec32x16i{_mm512_loadu_si512(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16i aligned_load<vec32x16i>(const std::int16_t* ptr, std::uint32_t n) {
        return load<vec32x16i>(ptr, n);
    }

    // Add specializations using 32-bit aligned loads

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16i aligned_load<vec32x16i, vec32x16i::width>(const std::int16_t* ptr) {
        #if defined(AVEL_AVX512BW)
        return vec32x16i{_mm512_load_si512(ptr)};
        #endif
    }



    AVEL_FINL void store(std::int16_t* ptr, vec32x16i v, std::uint32_t n) {
        #if defined(AVEL_AVX512BW)
        auto mask = (n >= 32) ? -1 : (1 << n)  - 1;
        _mm512_mask_storeu_epi16(ptr, mask, decay(v));
        #endif
    }

    template<std::uint32_t N = vec32x16i::width>
    AVEL_FINL void store(std::int16_t* ptr, vec32x16i v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec32x16i::width>(std::int16_t* ptr, vec32x16i v) {
        #if defined(AVEL_AVX512BW)
        _mm512_storeu_si512(ptr, decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(std::int16_t* ptr, vec32x16i v, std::uint32_t n) {
        #if defined(AVEL_AVX512BW)
        auto mask = (n >= 32) ? -1 : (1 << n)  - 1;
        _mm512_mask_storeu_epi16(ptr, mask, decay(v));
        #endif
    }

    template<std::uint32_t N = vec32x16i::width>
    AVEL_FINL void aligned_store(std::int16_t* ptr, vec32x16i v) {
        aligned_store(ptr, v, N);
    }

    // Add specializations using 32-bit aligned stores

    template<>
    AVEL_FINL void aligned_store<vec32x16i::width>(std::int16_t* ptr, vec32x16i v) {
        #if defined(AVEL_AVX512BW)
        _mm512_store_si512(ptr, decay(v));
        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr32x16i to_array(vec32x16i v) {
        alignas(64) arr32x16i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec32x16i> div(vec32x16i x, vec32x16i y) {
        mask32x16i remainder_sign_mask = (x < vec32x16i{0x00});
        mask32x16i quotient_sign_mask = remainder_sign_mask ^ (y < vec32x16i{0x00});

        vec32x16u numerator{abs(x)};
        vec32x16u denominator{abs(y)};

        auto results = div(numerator, denominator);

        return {
            negate(quotient_sign_mask,  vec32x16i{results.quot}),
            negate(remainder_sign_mask, vec32x16i{results.rem})
        };
    }

    AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(vec32x16i, mask32x16i, vec32x16u)

}

#endif //AVEL_VEC32X16I_HPP
