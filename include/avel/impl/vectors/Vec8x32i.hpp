#ifndef AVEL_VEC8X32I_HPP
#define AVEL_VEC8X32I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x32i = Vector<std::int32_t, 8>;
    using arr8x32i = std::array<std::int32_t, 8>;
    using mask8x32i = Vector_mask<std::int32_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x32i> div(vec8x32i numerator, vec8x32i denominator);
    vec8x32i set_bits(mask8x32i m);
    vec8x32i blend(mask8x32i m, vec8x32i a, vec8x32i b);
    vec8x32i negate(mask8x32i m, vec8x32i x);





    template<>
    class Vector_mask<std::int32_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL)
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
        // Constructor
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> m):
            Vector_mask(convert<Vector_mask>(m)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL)
            content(b ? -1 : 0) {}
        #elif defined(AVEL_AVX2)
            content(b ? _mm256_set1_epi32(-1) : _mm256_setzero_si256()) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr8xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            content = __mmask8(_mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            auto expanded = _mm256_cvtepi8_epi32(array_data);
            content = _mm256_cmplt_epu32_mask(_mm256_setzero_si256(), expanded);

            #elif defined(AVEL_AVX2)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            auto expanded = _mm256_cvtepi8_epi32(array_data);
            content = _mm256_cmpgt_epi32(expanded, _mm256_setzero_si256());

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

        AVEL_FINL Vector_mask& operator=(const Vector_mask&) = default;
        AVEL_FINL Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return lhs.content == rhs.content;

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(lhs.content) == _mm256_movemask_epi8(rhs.content);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return lhs.content != rhs.content;

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(lhs.content) != _mm256_movemask_epi8(rhs.content);
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content &= rhs.content;

            #elif defined(AVEL_AVX2)
            content = _mm256_and_si256(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content |= rhs.content;

            #elif defined(AVEL_AVX2)
            content = _mm256_or_si256(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content ^= rhs.content;

            #elif defined(AVEL_AVX2)
            content = _mm256_xor_si256(content, rhs.content);

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{static_cast<primitive>(~content)};

            #elif defined(AVEL_AVX2)
            primitive tmp = _mm256_undefined_si256();
            return Vector_mask{_mm256_andnot_si256(content, _mm256_cmpeq_epi32(tmp, tmp))};
            #endif
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
    AVEL_FINL std::array<mask8x32u, 1> convert<mask8x32u, mask8x32i>(mask8x32i m) {
        return {mask8x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x32i, 1> convert<mask8x32i, mask8x32u>(mask8x32u m) {
        return {mask8x32i{decay(m)}};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask8x32i m) {
        return count(mask8x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x32i m) {
        return any(mask8x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask8x32i m) {
        return all(mask8x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x32i m) {
        return none(mask8x32u{m});
    }



    template<>
    class Vector<std::int32_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int32_t;

        #if defined(AVEL_AVX2)
        using primitive = __m256i;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> m):
            Vector(convert<Vector>(m)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm256_maskz_set1_epi32(decay(m), 0x1)) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_sub_epi32(_mm256_setzero_si256(), decay(m))) {}
        #endif

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const scalar x):
            content(_mm256_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            content(_mm256_loadu_si256(reinterpret_cast<const primitive*>(array.data()))) {}

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

        AVEL_FINL Vector& operator=(const Vector&) = default;
        AVEL_FINL Vector& operator=(Vector&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpeq_epi32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpeq_epi32(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpneq_epi32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_AVX2)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmplt_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpgt_epi32(rhs.content, lhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmple_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_AVX2)
            return !mask{lhs > rhs};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpgt_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpgt_epi32(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpge_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_AVX2)
            return !mask{lhs < rhs};
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

        AVEL_FINL Vector& operator+=(Vector vec) {
            content = _mm256_add_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm256_sub_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm256_mullo_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            auto results = div(*this, vec);
            content = results.quot.content;
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector vec) {
            auto results = div(*this, vec);
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
            content = _mm256_and_si256(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm256_or_si256(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm256_xor_si256(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector operator<<=(long long s) {
            content = _mm256_sll_epi32(content, _mm_cvtsi64_si128(s));
            return *this;
        }

        AVEL_FINL Vector operator>>=(long long s) {
            content = _mm256_sra_epi32(content, _mm_cvtsi64_si128(s));
            return *this;
        }

        AVEL_FINL Vector operator<<=(Vector s) {
            content = _mm256_sllv_epi32(content, primitive(s));
            return *this;
        }

        AVEL_FINL Vector operator>>=(Vector s) {
            content = _mm256_srav_epi32(content, primitive(s));
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector{_mm256_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_AVX2)
            return Vector{_mm256_andnot_si256(content, _mm256_set1_epi32(-1))};
            #endif
        }

        AVEL_VECTOR_BINARY_BITWISE_OPERATORS

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX2)
            return *this != Vector{};
            #endif
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    static_assert(
        8 * sizeof(std::int32_t) == sizeof(vec8x32i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x32u, 1> convert<vec8x32u, vec8x32i>(vec8x32i x) {
        #if defined(AVEL_AVX2)
        return {vec8x32u{decay(x)}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x32i, 1> convert<vec8x32i, vec8x32u>(vec8x32u x) {
        #if defined(AVEL_AVX2)
        return {vec8x32i{decay(x)}};
        #endif
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec8x32i operator-(vec8x32u v) {
        return vec8x32i{0x00} - vec8x32i{v};
    }

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL std::int32_t extract(vec8x32i v) {
        static_assert(N < vec8x32i::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x32i::width, int>::type dummy_variable = 0;

        return static_cast<std::int32_t>(extract<N>(vec8x32u{v}));
    }

    template<std::uint32_t N>
    AVEL_FINL vec8x32i insert(vec8x32i v, std::int32_t x) {
        static_assert(N < vec8x32i::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x32i::width, int>::type dummy_variable = 0;

        return vec8x32i{insert<N>(vec8x32u{v}, static_cast<std::uint32_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x32i bit_shift_left(vec8x32i v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        return vec8x32i{bit_shift_left<S>(vec8x32u{v})};
    }

    template<>
    AVEL_FINL vec8x32i bit_shift_left<0>(vec8x32i v) {
        return v;
    }

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x32i bit_shift_right(vec8x32i v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return vec8x32i{_mm256_srai_epi32(decay(v), S)};
        #endif
    }

    template<>
    AVEL_FINL vec8x32i bit_shift_right<0>(vec8x32i v) {
        return v;
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x32i rotl(vec8x32i v) {
        return vec8x32i{rotl<S>(vec8x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i rotl(vec8x32i v, long long s) {
        return vec8x32i(rotl(vec8x32u(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i rotl(vec8x32i v, vec8x32i s) {
        return vec8x32i(rotl(vec8x32u(v), vec8x32u(s)));
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x32i rotr(vec8x32i v) {
        return vec8x32i{rotr<S>(vec8x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i rotr(vec8x32i v, long long s) {
        return vec8x32i(rotr(vec8x32u(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i rotr(vec8x32i v, vec8x32i s) {
        return vec8x32i(rotr(vec8x32u(v), vec8x32u(s)));
    }

    //=====================================================
    // General vector operations
    //=====================================================
    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec8x32i x) {
        return count(vec8x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec8x32i x) {
        return any(vec8x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec8x32i x) {
        return all(vec8x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec8x32i x) {
        return none(vec8x32u{x});
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i set_bits(mask8x32i m) {
        return vec8x32i{set_bits(mask8x32u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i keep(mask8x32i m, vec8x32i v) {
        return vec8x32i{keep(mask8x32u{m}, vec8x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i clear(mask8x32i m, vec8x32i v) {
        return vec8x32i{clear(mask8x32u{m}, vec8x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i blend(mask8x32i m, vec8x32i a, vec8x32i b) {
        return vec8x32i{blend(mask8x32u{m}, vec8x32u{a}, vec8x32u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i byteswap(vec8x32i v) {
        return vec8x32i{byteswap(vec8x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i max(vec8x32i a, vec8x32i b) {
        return vec8x32i{_mm256_max_epi32(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i min(vec8x32i a, vec8x32i b) {
        return vec8x32i{_mm256_min_epi32(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x32i, 2> minmax(vec8x32i a, vec8x32i b) {
        return {
            vec8x32i{_mm256_min_epi32(decay(a), decay(b))},
            vec8x32i{_mm256_max_epi32(decay(a), decay(b))}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i clamp(vec8x32i x, vec8x32i lo, vec8x32i hi) {
        return vec8x32i{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i average(vec8x32i a, vec8x32i b) {
        #if defined(AVEL_AVX2)
        auto avg = (a & b) + ((a ^ b) >> 1);
        auto c = set_bits((a < -b) | (b == vec8x32i{std::int32_t(1 << 31)})) & (a ^ b) & vec8x32i{1};

        return avg + c;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i midpoint(vec8x32i a, vec8x32i b) {
        #if defined(AVEL_AVX512VL)
        auto avg = decay(((a ^ b) >> 1) + (a & b));

        auto bias = _mm256_ternarylogic_epi32(decay(a), decay(b), _mm256_set1_epi32(0x1), 0x28);
        auto mask = _mm256_cmplt_epi32_mask(decay(b), decay(a));
        auto ret = _mm256_mask_add_epi32(avg, mask, avg, bias);

        return vec8x32i{ret};

        #elif defined(AVEL_AVX2)
        auto average = ((a ^ b) >> 1) + (a & b);
        auto bias = (set_bits(b < a) & (a ^ b) & vec8x32i{0x1});
        return average + bias;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i negate(mask8x32i m, vec8x32i x) {
        #if defined(AVEL_AVX512VL)
        return vec8x32i{_mm256_mask_sub_epi32(decay(x), decay(m), _mm256_setzero_si256(), decay(x))};

        #elif defined(AVEL_AVX2)
        return vec8x32i{_mm256_sign_epi32(decay(x), _mm256_or_si256(decay(m), _mm256_set1_epi32(0x01)))};
        #endif
    }

    AVEL_FINL vec8x32i abs(vec8x32i v) {
        return vec8x32i{_mm256_abs_epi32(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i neg_abs(vec8x32i v) {
        return -vec8x32i{_mm256_abs_epi32(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i neg_abs(vec8x32u v) {
        return neg_abs(vec8x32i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32i load<vec8x32i>(const std::int32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32i{_mm256_maskz_loadu_epi32(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n > 4) {
            auto lo = load<vec4x32i>(ptr + 0, n - 0);
            auto hi = load<vec4x32i>(ptr + 4, n - 4);
            return vec8x32i{_mm256_set_m128i(decay(hi), decay(lo))};
        } else {
            auto lo = load<vec4x32i>(ptr, n);
            return vec8x32i{_mm256_set_m128i(_mm_setzero_si128(), decay(lo))};
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32i load<vec8x32i>(const std::int32_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec8x32i{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32i aligned_load<vec8x32i>(const std::int32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32i{_mm256_maskz_load_epi32(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n > 4) {
            auto lo = aligned_load<vec4x32i>(ptr + 0, n - 0);
            auto hi = aligned_load<vec4x32i>(ptr + 4, n - 4);
            return vec8x32i{_mm256_set_m128i(decay(hi), decay(lo))};
        } else {
            auto lo = aligned_load<vec4x32i>(ptr, n);
            return vec8x32i{_mm256_set_m128i(_mm_setzero_si128(), decay(lo))};
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32i aligned_load<vec8x32i>(const std::int32_t* ptr) {
        return vec8x32i{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u gather<vec8x32u>(const std::uint32_t* ptr, vec8x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32u{_mm256_mmask_i32gather_epi32(_mm256_setzero_si256(), mask, decay(indices), ptr, sizeof(std::uint32_t))};

        #elif defined(AVEL_AVX2)
        std::uint32_t c0 = 0;
        std::uint32_t c1 = 0;
        std::uint32_t c2 = 0;
        std::uint32_t c3 = 0;
        std::uint32_t c4 = 0;
        std::uint32_t c5 = 0;
        std::uint32_t c6 = 0;
        std::uint32_t c7 = 0;

        switch (n) {
            default: { c7 = ptr[extract<7>(indices)]; }
            case 7:  { c6 = ptr[extract<6>(indices)]; }
            case 6:  { c5 = ptr[extract<5>(indices)]; }
            case 5:  { c4 = ptr[extract<4>(indices)]; }
            case 4:  { c3 = ptr[extract<3>(indices)]; }
            case 3:  { c2 = ptr[extract<2>(indices)]; }
            case 2:  { c1 = ptr[extract<1>(indices)]; }
            case 1:  { c0 = ptr[extract<0>(indices)]; }
            case 0:  { /* Do nothing */ }
        }

        return vec8x32u{_mm256_set_epi32(c7, c6, c5, c4, c3, c2, c1, c0)};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u gather<vec8x32u>(const std::uint32_t* ptr, vec8x32i indices) {
       #if defined(AVEL_AVX2)
        return vec8x32u{_mm256_i32gather_epi32(avel::bit_cast<const int*>(ptr), indices, sizeof(std::uint32_t))};

       #elif defined(AVEL_AVX2)
        std::uint32_t c7 = ptr[extract<7>(indices)];
        std::uint32_t c6 = ptr[extract<6>(indices)];
        std::uint32_t c5 = ptr[extract<5>(indices)];
        std::uint32_t c4 = ptr[extract<4>(indices)];
        std::uint32_t c3 = ptr[extract<3>(indices)];
        std::uint32_t c2 = ptr[extract<2>(indices)];
        std::uint32_t c1 = ptr[extract<1>(indices)];
        std::uint32_t c0 = ptr[extract<0>(indices)];

        return vec8x32u{_mm256_set_epi32(c7, c6, c5, c4, c3, c2, c1, c0)};
        #endif
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32i gather<vec8x32i>(const std::int32_t* ptr, vec8x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32i{_mm256_mmask_i32gather_epi32(_mm256_setzero_si256(), mask, decay(indices), ptr, sizeof(std::uint32_t))};

        #elif defined(AVEL_AVX2)
        std::uint32_t c0 = 0;
        std::uint32_t c1 = 0;
        std::uint32_t c2 = 0;
        std::uint32_t c3 = 0;
        std::uint32_t c4 = 0;
        std::uint32_t c5 = 0;
        std::uint32_t c6 = 0;
        std::uint32_t c7 = 0;

        switch (n) {
            default: { c7 = ptr[extract<7>(indices)]; }
            case 7:  { c6 = ptr[extract<6>(indices)]; }
            case 6:  { c5 = ptr[extract<5>(indices)]; }
            case 5:  { c4 = ptr[extract<4>(indices)]; }
            case 4:  { c3 = ptr[extract<3>(indices)]; }
            case 3:  { c2 = ptr[extract<2>(indices)]; }
            case 2:  { c1 = ptr[extract<1>(indices)]; }
            case 1:  { c0 = ptr[extract<0>(indices)]; }
            case 0:  { /* Do nothing */ }
        }

        return vec8x32i{_mm256_set_epi32(c7, c6, c5, c4, c3, c2, c1, c0)};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32i gather<vec8x32i>(const std::int32_t* ptr, vec8x32i indices) {
        #if defined(AVEL_AVX2)
        return vec8x32i{_mm256_i32gather_epi32(ptr, indices, sizeof(std::int32_t))};

        #elif defined(AVEL_AVX2)
        auto i = to_array(indices);

        __m128i a = _mm_loadu_si32(ptr + i[0]);
        __m128i b = _mm_loadu_si32(ptr + i[1]);
        __m128i c = _mm_loadu_si32(ptr + i[2]);
        __m128i d = _mm_loadu_si32(ptr + i[3]);

        __m128i abab = _mm_unpacklo_epi32(a, b);
        __m128i cdcd = _mm_unpacklo_epi32(c, d);

        __m128i abcd = _mm_unpacklo_epi64(abab, cdcd);

        __m128i e = _mm_loadu_si32(ptr + i[4]);
        __m128i f = _mm_loadu_si32(ptr + i[5]);
        __m128i g = _mm_loadu_si32(ptr + i[6]);
        __m128i h = _mm_loadu_si32(ptr + i[7]);

        __m128i efef = _mm_unpacklo_epi32(e, f);
        __m128i ghgh = _mm_unpacklo_epi32(g, h);

        __m128i efgh = _mm_unpacklo_epi64(efef, ghgh);

        return vec8x32i{_mm256_set_m128i(efgh, abcd)};
        #endif
    }



    AVEL_FINL void store(std::int32_t* ptr, vec8x32i v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_storeu_epi32(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n > 4) {
            store(ptr + 0, vec4x32i{_mm256_castsi256_si128(decay(v))}, n - 0);
            store(ptr + 4, vec4x32i{_mm256_extracti128_si256(decay(v), 0x1)}, n - 4);
        } else {
            store(ptr, vec4x32i{_mm256_castsi256_si128(decay(v))}, n);
        }

        #endif
    }

    template<std::uint32_t N = vec8x32i::width>
    AVEL_FINL void store(std::int32_t* ptr, vec8x32i v) {
        store(ptr, v, N);
    }

    AVEL_FINL void store(std::int32_t* ptr, vec8x32i v) {
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
    }



    AVEL_FINL void aligned_store(std::int32_t* ptr, vec8x32i v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_store_epi32(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n > 4) {
            aligned_store(ptr + 0, vec4x32i{_mm256_castsi256_si128(decay(v))}, n - 0);
            aligned_store(ptr + 4, vec4x32i{_mm256_extracti128_si256(decay(v), 0x1)}, n - 4);
        } else {
            aligned_store(ptr, vec4x32i{_mm256_castsi256_si128(decay(v))}, n);
        }

        #endif
    }

    template<std::uint32_t N = vec8x32i::width>
    AVEL_FINL void aligned_store(std::int32_t* ptr, vec8x32i v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec8x32i::width>(std::int32_t* ptr, vec8x32i v) {
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }



    AVEL_FINL void scatter(std::uint32_t* ptr, vec8x32u v, vec8x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_i32scatter_epi32(ptr, mask, decay(indices), decay(v), sizeof(std::uint32_t));

        #elif defined(AVEL_AVX2)
        switch (n) {
            default: { ptr[extract<7>(indices)] = extract<7>(v); }
            case 7:  { ptr[extract<6>(indices)] = extract<6>(v); }
            case 6:  { ptr[extract<5>(indices)] = extract<5>(v); }
            case 5:  { ptr[extract<4>(indices)] = extract<4>(v); }
            case 4:  { ptr[extract<3>(indices)] = extract<3>(v); }
            case 3:  { ptr[extract<2>(indices)] = extract<2>(v); }
            case 2:  { ptr[extract<1>(indices)] = extract<1>(v); }
            case 1:  { ptr[extract<0>(indices)] = extract<0>(v); }
            case 0:  {; /* Do nothing */ }
        }

        #endif
    }

    template<std::uint32_t N = vec8x32u::width>
    AVEL_FINL void scatter(std::uint32_t* ptr,  vec8x32u v, vec8x32i indices) {
        scatter(ptr, v, indices, N);
    }

    //TODO: Add specializations _mm256_i32scatter_epi32

    AVEL_FINL void scatter(std::uint32_t* ptr, vec8x32u v, vec8x32i indices) {
        #if defined(AVEL_AVX512VL)
        _mm256_i32scatter_epi32(ptr, decay(indices), decay(v), sizeof(std::int32_t));

        #elif defined(AVEL_AVX2)
        ptr[extract<0>(indices)] = extract<0>(v);
        ptr[extract<1>(indices)] = extract<1>(v);
        ptr[extract<2>(indices)] = extract<2>(v);
        ptr[extract<3>(indices)] = extract<3>(v);
        ptr[extract<4>(indices)] = extract<4>(v);
        ptr[extract<5>(indices)] = extract<5>(v);
        ptr[extract<6>(indices)] = extract<6>(v);
        ptr[extract<7>(indices)] = extract<7>(v);
        #endif
    }



    AVEL_FINL void scatter(std::int32_t* ptr, vec8x32i v, vec8x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_i32scatter_epi32(ptr, mask, decay(indices), decay(v), sizeof(std::int32_t));

        #elif defined(AVEL_AVX2)
        switch (n) {
            default: { ptr[extract<7>(indices)] = extract<7>(v); }
            case 7:  { ptr[extract<6>(indices)] = extract<6>(v); }
            case 6:  { ptr[extract<5>(indices)] = extract<5>(v); }
            case 5:  { ptr[extract<4>(indices)] = extract<4>(v); }
            case 4:  { ptr[extract<3>(indices)] = extract<3>(v); }
            case 3:  { ptr[extract<2>(indices)] = extract<2>(v); }
            case 2:  { ptr[extract<1>(indices)] = extract<1>(v); }
            case 1:  { ptr[extract<0>(indices)] = extract<0>(v); }
            case 0:  {; /* Do nothing */ }
        }

        #endif
    }

    template<std::uint32_t N = vec8x32i::width>
    AVEL_FINL void scatter(std::int32_t* ptr, vec8x32i v, vec8x32i indices) {
        scatter(ptr, v, indices, N);
    }

    AVEL_FINL void scatter(std::int32_t* ptr, vec8x32i indices, vec8x32i v) {
        #if defined(AVEL_AVX512VL)
        _mm256_i32scatter_epi32(ptr, decay(indices), decay(v), sizeof(std::int32_t));

        #else
        ptr[extract<0>(indices)] = extract<0>(v);
        ptr[extract<1>(indices)] = extract<1>(v);
        ptr[extract<2>(indices)] = extract<2>(v);
        ptr[extract<3>(indices)] = extract<3>(v);
        ptr[extract<4>(indices)] = extract<4>(v);
        ptr[extract<5>(indices)] = extract<5>(v);
        ptr[extract<6>(indices)] = extract<6>(v);
        ptr[extract<7>(indices)] = extract<7>(v);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL arr8x32i to_array(vec8x32i v) {
        alignas(32) std::array<std::int32_t, 8> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x32i> div(vec8x32i x, vec8x32i y) {
        mask8x32i remainder_sign_mask = (x < vec8x32i{0x00});
        mask8x32i quotient_sign_mask = remainder_sign_mask ^ (y < vec8x32i{0x00});

        vec8x32u numerator{abs(x)};
        vec8x32u denominator{abs(y)};

        auto results = div(numerator, denominator);

        return {
            negate(quotient_sign_mask,  vec8x32i{results.quot}),
            negate(remainder_sign_mask, vec8x32i{results.rem})
        };
    }

    AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(vec8x32i, mask8x32i, vec8x32u)

}

#endif //AVEL_VEC8X32I_HPP
