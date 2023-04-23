#ifndef AVEL_VEC16X32I_HPP
#define AVEL_VEC16X32I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x32i = Vector<std::int32_t, 16>;
    using arr16x32i = std::array<std::int32_t, 16>;
    using mask16x32i = Vector_mask<std::int32_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x32i> div(vec16x32i x, vec16x32i y);
    vec16x32i set_bits(mask16x32i m);
    vec16x32i blend(mask16x32i m, vec16x32i a, vec16x32i b);
    vec16x32i negate(vec16x32i m, vec16x32i x);





    template<>
    class Vector_mask<std::int32_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512F)
        using primitive = __mmask16;
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

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool b):
            content(b ? -1 : 0) {}

        AVEL_FINL explicit Vector_mask(const arr16xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = _mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data);

            #elif defined(AVEL_AVX512F)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            auto widened_data = _mm512_cvtepi8_epi32(array_data);
            content = _mm512_cmplt_epu32_mask(_mm512_setzero_si512(), widened_data);

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
            #if defined(AVEL_AVX512F)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512F)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            content = _kand_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            content = _kor_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            content = _kxor_mask16(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            return Vector_mask{_knot_mask16(content)};
        }

        AVEL_VECTOR_MASK_BINARY_BITWISE_OPERATORS

        //=================================================
        // Conversion operators
        //=================================================

        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x32u, 1> convert<mask16x32u, mask16x32i>(mask16x32i m) {
        return {mask16x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x32i, 1> convert<mask16x32i, mask16x32u>(mask16x32u m) {
        return {mask16x32i{decay(m)}};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask16x32i m) {
        return count(mask16x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask16x32i m) {
        return any(mask16x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask16x32i m) {
        return all(mask16x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x32i m) {
        return none(mask16x32u{m});
    }



    template<>
    class Vector<std::int32_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int32_t;

        #if defined(AVEL_AVX512F)
        using primitive = __m512i;
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
            content(_mm512_maskz_set1_epi32(decay(m), 0x1)) {}

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const scalar x):
            content(_mm512_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            content(_mm512_loadu_si512(array.data())) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(primitive p) {
            content = p;
            return *this;
        }

        AVEL_FINL Vector& operator=(scalar x) {
            *this = Vector{x};
            return *this;
        }

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(const Vector vec) const {
            return mask{_mm512_cmpeq_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            return mask{_mm512_cmpneq_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            return mask{_mm512_cmplt_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            return mask{_mm512_cmple_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            return mask{_mm512_cmpgt_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            return mask{_mm512_cmpge_epi32_mask(content, vec.content)};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            return Vector{0x00} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content = _mm512_add_epi32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm512_sub_epi32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm512_mullo_epi32(content, rhs.content);
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

        AVEL_VECTOR_ARITHMETIC_OPERATORS

        //=================================================
        // Increment/Decrement operators
        //=================================================

        AVEL_VECTOR_INCREMENT_DECREMENT_OPERATORS

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector vec) {
            content = _mm512_and_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm512_or_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm512_xor_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector operator<<=(long long s) {
            content = _mm512_sll_epi32(content, _mm_cvtsi64_si128(s));
            return *this;
        }

        AVEL_FINL Vector operator>>=(long long s) {
            content = _mm512_sra_epi32(content, _mm_cvtsi64_si128(s));
            return *this;
        }

        AVEL_FINL Vector operator<<=(Vector s) {
            content = _mm512_sllv_epi32(content, primitive(s));
            return *this;
        }

        AVEL_FINL Vector operator>>=(Vector s) {
            content = _mm512_srav_epi32(content, primitive(s));
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512F)
            return Vector{_mm512_ternarylogic_epi32(content, content, content, 0x01)};
            #endif
        }

        AVEL_VECTOR_BINARY_BITWISE_OPERATORS

        //=================================================
        // Conversions
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_test_epi32_mask(content, content)};
            #endif
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    static_assert(
        16 * sizeof(std::int32_t) == sizeof(vec16x32i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x32u, 1> convert<vec16x32u, vec16x32i>(vec16x32i m) {
        #if defined(AVEL_AVX512F)
        return {vec16x32u{decay(m)}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x32i, 1> convert<vec16x32i, vec16x32u>(vec16x32u m) {
        #if defined(AVEL_AVX512F)
        return {vec16x32i{decay(m)}};
        #endif
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec16x32i operator-(vec16x32u v) {
        return vec16x32i{0x00} - vec16x32i{v};
    }

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint32_t extract(vec16x32i v) {
        static_assert(N <= vec16x32i::width, "Specified index does not exist");
        typename std::enable_if<N <= vec16x32i::width, int>::type dummy_variable = 0;

        return static_cast<std::uint32_t>(extract<N>(vec16x32u{v}));
    }

    template<std::uint32_t N>
    AVEL_FINL vec16x32i insert(vec16x32i v, std::int32_t x) {
        static_assert(N <= vec16x32i::width, "Specified index does not exist");
        typename std::enable_if<N <= vec16x32i::width, int>::type dummy_variable = 0;

        return vec16x32i{insert<N>(vec16x32u{v}, static_cast<std::uint32_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x32i bit_shift_left(vec16x32i v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        return vec16x32i{bit_shift_left<S>(vec16x32u{v})};
    }

    template<>
    AVEL_FINL vec16x32i bit_shift_left<0>(vec16x32i v) {
        return v;
    }

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x32i bit_shift_right(vec16x32i v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512F)
        return vec16x32i{_mm512_srai_epi32(decay(v), S)};
        #endif
    }

    template<>
    AVEL_FINL vec16x32i bit_shift_right<0>(vec16x32i v) {
        return v;
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x32i rotl(vec16x32i v) {
        return vec16x32i{rotl<S>(vec16x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i rotl(vec16x32i v, long long s) {
        return vec16x32i(rotl(vec16x32u(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i rotl(vec16x32i v, vec16x32i s) {
        return vec16x32i(rotl(vec16x32u(v), vec16x32u(s)));
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x32i rotr(vec16x32i v) {
        return vec16x32i{rotr<S>(vec16x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i rotr(vec16x32i v, long long s) {
        return vec16x32i(rotr(vec16x32u(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i rotr(vec16x32i v, vec16x32i s) {
        return vec16x32i(rotr(vec16x32u(v), vec16x32u(s)));
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec16x32i x) {
        return count(vec16x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec16x32i x) {
        return any(vec16x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec16x32i x) {
        return all(vec16x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec16x32i x) {
        return none(vec16x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i set_bits(mask16x32i m) {
        #if defined(AVEL_AVX512DQ)
        return vec16x32i{_mm512_movm_epi32(decay(m))};
        #elif defined(AVEL_AVX512F)
        return vec16x32i{_mm512_maskz_set1_epi32(decay(m), -1)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i keep(mask16x32i m, vec16x32i v) {
        return vec16x32i{keep(mask16x32u{m}, vec16x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i clear(mask16x32i m, vec16x32i v) {
        return vec16x32i{clear(mask16x32u{m}, vec16x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i blend(mask16x32i m, vec16x32i a, vec16x32i b) {
        return vec16x32i{blend(mask16x32u{m}, vec16x32u{a}, vec16x32u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i byteswap(vec16x32i v) {
        return vec16x32i{byteswap(vec16x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i max(vec16x32i a, vec16x32i b) {
        #if defined(AVEL_AVX512F)
        return vec16x32i{_mm512_max_epi32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i min(vec16x32i a, vec16x32i b) {
        #if defined(AVEL_AVX512F)
        return vec16x32i{_mm512_min_epi32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec16x32i, 2> minmax(vec16x32i a, vec16x32i b) {
        #if defined(AVEL_AVX512F)
        return {
            vec16x32i{_mm512_min_epi32(decay(a), decay(b))},
            vec16x32i{_mm512_max_epi32(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i clamp(vec16x32i x, vec16x32i lo, vec16x32i hi) {
        return vec16x32i{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i average(vec16x32i a, vec16x32i b) {
        #if defined(AVEL_AVX512F)
        auto avg = (a & b) + ((a ^ b) >> 1);
        auto c = set_bits((a < -b) | (b == vec16x32i{std::int32_t(1 << 31)})) & (a ^ b) & vec16x32i{1};

        return avg + c;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i midpoint(vec16x32i a, vec16x32i b) {
        const vec16x32u offset{0x80000000};

        #if defined(AVEL_AVX512DQ)
        auto x = static_cast<vec16x32u>(a) ^ offset;
        auto y = static_cast<vec16x32u>(b) ^ offset;

        return vec16x32i{midpoint(x, y) ^ offset};
        #else
        auto x = static_cast<vec16x32u>(a) + offset;
        auto y = static_cast<vec16x32u>(b) + offset;

        return vec16x32i{midpoint(x, y) + offset};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i negate(mask16x32i m, vec16x32i x) {
        #if defined(AVEL_AVX512F)
        return vec16x32i{_mm512_mask_sub_epi32(decay(x), decay(m), _mm512_setzero_si512(), decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i abs(vec16x32i v) {
        return vec16x32i{_mm512_abs_epi32(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i neg_abs(vec16x32i v) {
        return -vec16x32i{_mm512_abs_epi32(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i neg_abs(vec16x32u v) {
        return neg_abs(vec16x32i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32i load<vec16x32i>(const std::int32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        return vec16x32i{_mm512_maskz_loadu_epi32(mask, ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32i load<vec16x32i>(const std::int32_t* ptr) {
        return vec16x32i{_mm512_loadu_si512(ptr)};
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32i aligned_load<vec16x32i>(const std::int32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        return vec16x32i{_mm512_maskz_load_epi32(mask, ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32i aligned_load<vec16x32i>(const std::int32_t* ptr) {
        return vec16x32i{_mm512_load_si512(ptr)};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u gather<vec16x32u>(const std::uint32_t* ptr, vec16x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        return vec16x32u{_mm512_mask_i32gather_epi32(_mm512_setzero_si512(), mask, decay(indices), ptr, sizeof(std::uint32_t))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u gather<vec16x32u>(const std::uint32_t* ptr, vec16x32i indices) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_i32gather_epi32(decay(indices), ptr, sizeof(std::uint32_t))};
        #endif
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32i gather<vec16x32i>(const std::int32_t* ptr, vec16x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        return vec16x32i{_mm512_mask_i32gather_epi32(_mm512_setzero_si512(), mask, decay(indices), ptr, sizeof(std::int32_t))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32i gather<vec16x32i>(const std::int32_t* ptr, vec16x32i indices) {
        #if defined(AVEL_AVX512F)
        return vec16x32i{_mm512_i32gather_epi32(decay(indices), ptr, sizeof(std::int32_t))};
        #endif
    }


    AVEL_FINL void store(std::int32_t* ptr, vec16x32i x, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm512_mask_storeu_epi32(ptr, mask, decay(x));
        #endif
    }

    template<std::uint32_t N = vec16x32i::width>
    AVEL_FINL void store(std::int32_t* ptr, vec16x32i x) {
        static_assert(N <= vec16x32i::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec16x32i::width, int>::type dummy_variable = 0;

        store(ptr, x, N);
    }

    AVEL_FINL void store(std::int32_t* ptr, vec16x32i v) {
        #if defined(AVEL_AVX512F)
        _mm512_storeu_si512(ptr, decay(v));
        #endif
    }


    AVEL_FINL void aligned_store(std::int32_t* ptr, vec16x32i v, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm512_mask_store_epi32(ptr, mask, decay(v));
        #endif
    }

    template<std::uint32_t N = vec16x32i::width>
    AVEL_FINL void aligned_store(std::int32_t* ptr, vec16x32i x) {
        static_assert(N <= vec16x32i::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec16x32i::width, int>::type dummy_variable = 0;

        aligned_store(ptr, x, N);
    }

    AVEL_FINL void aligned_store(std::int32_t* ptr, vec16x32i v) {
        #if defined(AVEL_AVX512F)
        _mm512_store_si512(ptr, decay(v));
        #endif
    }


    AVEL_FINL void scatter(std::uint32_t* ptr, vec16x32u v, vec16x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm512_mask_i32scatter_epi32(ptr, mask, decay(indices), decay(v), sizeof(std::uint32_t));
        #endif
    }

    template<std::uint32_t N = vec16x32u::width>
    AVEL_FINL void scatter(std::uint32_t* ptr, vec16x32u v, vec16x32i indices) {
        scatter(ptr, v, indices, N);
    }

    AVEL_FINL void scatter(std::uint32_t* ptr, vec16x32u indices, vec16x32u v) {
        #if defined(AVEL_AVX512F)
        _mm512_i32scatter_epi32(avel::bit_cast<int*>(ptr), decay(indices), decay(v), sizeof(std::uint32_t));
        #endif
    }


    AVEL_FINL void scatter(std::int32_t* ptr, vec16x32i v, vec16x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm512_mask_i32scatter_epi32(ptr, mask, decay(indices), decay(v), sizeof(std::uint32_t));
        #endif
    }

    template<std::uint32_t N = vec16x32i::width>
    AVEL_FINL void scatter(std::int32_t* ptr, vec16x32i v, vec16x32i indices) {
        scatter(ptr, v, indices, N);
    }

    AVEL_FINL void scatter(std::int32_t* ptr, vec16x32i indices, vec16x32i v) {
        #if defined(AVEL_AVX512F)
        _mm512_i32scatter_epi32(ptr, decay(indices), decay(v), sizeof(std::int32_t));
        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr16x32i to_array(vec16x32i v) {
        alignas(64) std::array<std::int32_t, 16> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec16x32i> div(vec16x32i x, vec16x32i y) {
        mask16x32i remainder_sign_mask = (x < vec16x32i{0x00});
        mask16x32i quotient_sign_mask = remainder_sign_mask ^ (y < vec16x32i{0x00});

        vec16x32u numerator{abs(x)};
        vec16x32u denominator{abs(y)};

        auto results = div(numerator, denominator);

        return {
            negate(quotient_sign_mask,  vec16x32i{results.quot}),
            negate(remainder_sign_mask, vec16x32i{results.rem})
        };
    }

    AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(vec16x32i, mask16x32i, vec16x32u)

}

#endif //AVEL_VEC16X32I_HPP
