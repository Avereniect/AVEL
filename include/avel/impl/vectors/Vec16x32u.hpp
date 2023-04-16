#ifndef AVEL_VEC16X32U_HPP
#define AVEL_VEC16X32U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x32u = Vector<std::uint32_t, 16>;
    using arr16x32u = std::array<std::uint32_t, 16>;
    using mask16x32u = Vector_mask<std::uint32_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x32u> div(vec16x32u numerator, vec16x32u denominator);
    vec16x32u broadcast_mask(mask16x32u m);
    vec16x32u blend(mask16x32u m, vec16x32u a, vec16x32u b);
    vec16x32u countl_one(vec16x32u x);





    template<>
    class Vector_mask<std::uint32_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = __mmask16;

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

            #if defined(AVEL_AVX512F) && defined(AVEL_AVX512BW)
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
        AVEL_FINL bool operator==(Vector_mask rhs) const noexcept {
            return content == rhs.content;
        }

        [[nodiscard]]
        AVEL_FINL bool operator!=(Vector_mask rhs) const noexcept {
            return content != rhs.content;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            content &= rhs.content;
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            content |= rhs.content;
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            content ^= rhs.content;
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            return Vector_mask{_knot_mask16(content)};
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

        static primitive from_bool(bool x) {
            static const primitive full_masks[2] {
                _cvtu32_mask16(0),
                _cvtu32_mask16(-1)
            };

            return full_masks[x];
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask16x32u m) {
        return popcount(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask16x32u m) {
        #if defined(AVEL_AVX512F)
        return !_kortestz_mask16_u8(decay(m), decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask16x32u m) {
        #if defined(AVEL_AVX512F)
        return _kortestc_mask16_u8(decay(m), decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x32u m) {
        #if defined(AVEL_AVX512F)
        return _kortestz_mask16_u8(decay(m), decay(m));
        #endif
    }





    template<>
    class alignas(64) Vector<std::uint32_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint32_t;

        using primitive = __m512i;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            Vector(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
            content(_mm512_maskz_set1_epi32(decay(m), 0x1)) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
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
            this->content = p;
            return *this;
        }

        AVEL_FINL Vector& operator=(scalar x) {
            *this = Vector{x};
            return *this;
        }

        AVEL_FINL Vector& operator=(const Vector&) = default;
        AVEL_FINL Vector& operator=(Vector&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpeq_epi32_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpneq_epi32_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            return mask{_mm512_cmplt_epu32_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmple_epu32_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpgt_epu32_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpge_epu32_mask(decay(lhs), decay(rhs))};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        //Definition of operator-() deferred until after definition of vec4x32i

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

        AVEL_FINL Vector& operator<<=(std::uint32_t s) {
            content = _mm512_sll_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t s) {
            content = _mm512_srl_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector s) {
            content = _mm512_sllv_epi32(content, s.content);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector s) {
            content = _mm512_srlv_epi32(content, s.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector operator~() const {
            return Vector{_mm512_ternarylogic_epi32(content, content, content, 0x01)};
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
        AVEL_FINL friend Vector operator<<(Vector lhs, std::uint32_t rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, std::uint32_t rhs) {
            lhs >>= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, Vector rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, Vector rhs) {
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
            #if defined(AVEL_AVX512F)
            return mask{_mm512_test_epi32_mask(content, content)};

            #else
            return *this != Vector{0x00};
            #endif
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    static_assert(
        16 * sizeof(std::uint32_t) == sizeof(vec16x32u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint32_t extract(vec16x32u v) {
        static_assert(N <= vec16x32u::width, "Specified index does not exist");
        typename std::enable_if<N <= vec16x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512F)
        auto quarter = _mm512_extracti32x4_epi32(decay(v), N / 4);
        return _mm_extract_epi32(quarter, N % 4);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec16x32u insert(vec16x32u v, std::uint32_t x) {
        static_assert(N <= vec16x32u::width, "Specified index does not exist");
        typename std::enable_if<N <= vec16x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512F)
        auto quarter = _mm512_extracti32x4_epi32(decay(v), N / 4);
        quarter = _mm_insert_epi32(quarter, x, N % 4);
        return vec16x32u{_mm512_inserti32x4(decay(v), quarter, N / 4)};
        #endif
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x32u bit_shift_left(vec16x32u v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_slli_epi32(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u bit_shift_left<0>(vec16x32u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u bit_shift_left<32>(vec16x32u v) {
        return vec16x32u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x32u bit_shift_right(vec16x32u v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec16x32u{_mm512_srli_epi32(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u bit_shift_right<0>(vec16x32u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u bit_shift_right<32>(vec16x32u v) {
        (void)v;
        return vec16x32u{0x00};
    }



    template<std::uint32_t S, typename std::enable_if<S < 32, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec16x32u rotl(vec16x32u v) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_rol_epi32(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u rotl<0>(vec16x32u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<32 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec16x32u rotl(vec16x32u v) {
        return rotl<S % 32>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u rotl(vec16x32u v, long long s) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_rolv_epi32(decay(v), _mm512_set1_epi32(s))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u rotl(vec16x32u v, vec16x32u s) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_rolv_epi32(decay(v), decay(s))};
        #endif
    }



    template<std::uint32_t S, typename std::enable_if<S < 32, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec16x32u rotr(vec16x32u v) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_ror_epi32(decay(v), S)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u rotr<0>(vec16x32u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<32 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec16x32u rotr(vec16x32u v) {
        return rotr<S % 32>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u rotr(vec16x32u v, long long s) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_rorv_epi32(decay(v), _mm512_set1_epi32(s))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u rotr(vec16x32u v, vec16x32u s) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_rorv_epi32(decay(v), decay(s))};
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================
    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec16x32u x) {
        #if defined(AVEL_AVX512F)
        return count(mask16x32u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec16x32u x) {
        #if defined(AVEL_AVX512F)
        return any(mask16x32u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec16x32u x) {
        #if defined(AVEL_AVX512F)
        return all(mask16x32u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec16x32u x) {
        #if defined(AVEL_AVX512F)
        return none(mask16x32u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u broadcast_mask(mask16x32u m) {
        return vec16x32u{_mm512_movm_epi32(decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u keep(mask16x32u m, vec16x32u v) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_maskz_mov_epi32(decay(m), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u clear(mask16x32u m, vec16x32u v) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_maskz_mov_epi32(decay(!m), decay(v))};
        #endif
    }

    AVEL_FINL vec16x32u blend(mask16x32u m, vec16x32u a, vec16x32u b) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_mask_blend_epi32(decay(m), decay(b), decay(a))};
        #endif
    }

    AVEL_FINL vec16x32u max(vec16x32u a, vec16x32u b) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_max_epu32(decay(a), decay(b))};
        #endif
    }

    AVEL_FINL vec16x32u min(vec16x32u a, vec16x32u b) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_min_epu32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec16x32u, 2> minmax(vec16x32u a, vec16x32u b) {
        #if defined(AVEL_AVX512F)
        return {
            vec16x32u{_mm512_min_epu32(decay(a), decay(b))},
            vec16x32u{_mm512_max_epu32(decay(a), decay(b))}
        };
        #endif
    }

    AVEL_FINL vec16x32u clamp(vec16x32u x, vec16x32u lo, vec16x32u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u midpoint(vec16x32u a, vec16x32u b) {
        #if defined(AVEL_AVX512F)
        vec16x32u t0 = a & b & vec16x32u{0x1};
        vec16x32u t1 = (a | b) & vec16x32u{0x1} & broadcast_mask(a > b);
        vec16x32u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u average(vec16x32u a, vec16x32u b) {
        return (a >> 1) + (b >> 1) + (a & b & vec16x32u{0x1});;
    }

    //Definition of neg_abs delayed until vec16x32i is defined

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u load<vec16x32u>(const std::uint32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        return vec16x32u{_mm512_maskz_loadu_epi32(mask, ptr)};
        #endif
    }

    template<>
    AVEL_FINL vec16x32u load<vec16x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_loadu_si512(ptr)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u aligned_load<vec16x32u>(const std::uint32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        return vec16x32u{_mm512_maskz_load_epi32(mask, ptr)};
        #endif
    }

    template<>
    AVEL_FINL vec16x32u aligned_load<vec16x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_AVX512F)
        return vec16x32u{_mm512_load_si512(ptr)};
        #endif
    }

    //Definition of gather deferred until definition of vector of signed integers

    AVEL_FINL void store(std::uint32_t* ptr, vec16x32u v, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm512_mask_storeu_epi32(ptr, mask, decay(v));
        #endif
    }

    template<std::uint32_t N = vec16x32u::width>
    AVEL_FINL void store(std::uint32_t* ptr, vec16x32u x) {
        static_assert(N <= vec16x32u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec16x32u::width, int>::type dummy_variable = 0;

        store(ptr, x, N);
    }

    AVEL_FINL void store(std::uint32_t* ptr, vec16x32u v) {
        #if defined(AVEL_AVX512F)
        _mm512_storeu_si512(reinterpret_cast<__m512i*>(ptr), decay(v));
        #endif
    }


    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec16x32u v, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm512_mask_store_epi32(ptr, mask, decay(v));
        #endif
    }

    template<std::uint32_t N = vec16x32u::width>
    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec16x32u x) {
        static_assert(N <= vec16x32u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec16x32u::width, int>::type dummy_variable = 0;

        aligned_store(ptr, x, N);
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec16x32u v) {
        #if defined(AVEL_AVX512F)
        _mm512_store_si512(reinterpret_cast<__m512i*>(ptr), decay(v));
        #endif
    }

    //Definition of scatter deferred until vector of signed integers is defined

    [[nodiscard]]
    AVEL_FINL arr16x32u to_array(vec16x32u v) {
        alignas(64) std::array<std::uint32_t, 16> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer arithmetic functions
    //=====================================================

    AVEL_FINL div_type<vec16x32u> div(vec16x32u x, vec16x32u y) {
        vec16x32u quotient{0x00};
        std::int32_t i = 32;
        for (; (i-- > 0) && any(x >= y);) {
            mask16x32u b = ((x >> i) >= y);
            x -= (broadcast_mask(b) & (y << i));
            quotient += quotient;
            quotient -= broadcast_mask(b);
        }

        quotient <<= (i + 1);
        return {quotient, x};
    }

    AVEL_FINL vec16x32u popcount(vec16x32u v) {
        #if defined(AVEL_AVX512VPOPCNTDQ)
        return vec16x32u{_mm512_popcnt_epi32(decay(v))};

        #elif defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm512_popcnt_epi16(v);
        auto tmp1 = _mm512_slli_epi32(tmp0, 16);

        auto tmp2 = _mm512_add_epi32(tmp0, tmp1);

        return vec16x32u{_mm512_srli_epi32(tmp2, 16)};
        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec16x32u{0x55555555});                    // reuse input as temporary
        v = (v & vec16x32u{0x33333333}) + ((v >> 2) & vec16x32u{0x33333333});     // temp
        v = ((v + (v >> 4) & vec16x32u{0xF0F0F0F}) * vec16x32u{0x1010101}) >> 24; // count
        return v;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u byteswap(vec16x32u v) {
        alignas(64) static constexpr std::uint8_t index_data[64] {
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12,
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12,
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12,
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12
        };

        auto indices = _mm512_load_si512((const __m512i*)index_data);
        auto ret = vec16x32u{_mm512_shuffle_epi8(decay(v), indices)};
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countl_zero(vec16x32u x) {
        #if defined(AVEL_AVX512CD)
        return vec16x32u{_mm512_lzcnt_epi32(decay(x))};
        #elif defined(AVEL_AVX512F)
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        x = _mm512_andnot_si512(decay(x >> 1), decay(x));
        auto floats = _mm512_add_ps(_mm512_cvtepi32_ps(decay(x)), _mm512_set1_ps(0.5f));
        auto biased_exponents = (vec16x32u(_mm512_castps_si512(floats)) >> 23);
        auto lzcnt = _mm512_subs_epu16(decay(vec16x32u{158}), decay(biased_exponents));
        return vec16x32u{lzcnt};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countl_one(vec16x32u x) {
        #if defined(AVEL_AVX512CD)
        return vec16x32u{_mm512_lzcnt_epi32(decay(~x))};
        #elif defined(AVEL_AVX512F)
        return countl_zero(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countr_zero(vec16x32u x) {
        #if defined(AVEL_AVX512VPOPCNTDQ)
        auto neg_one = _mm512_set1_epi32(-1);
        auto tz_mask = _mm512_andnot_si512(decay(x), _mm512_add_epi32(decay(x), neg_one));
        return vec16x32u{_mm512_popcnt_epi32(tz_mask)};

        #elif defined(AVEL_AVX512CD)
        auto zero_mask = (x == vec16x32u{0x00});
        auto y = (x & (vec16x32u{0x00} - x));
        auto z = vec16x32u{31} - countl_zero(y);
        return blend(zero_mask, vec16x32u{32}, z);

        #elif defined(AVEL_AVX512F)
        //TODO: Optimize?
        auto z = decay(x);
        auto is_zero = _mm512_cmpeq_epi32_mask(z, _mm512_setzero_si512());
        auto y = _mm512_and_si512(z, _mm512_sub_epi32(_mm512_setzero_si512(), z));
        auto floats = _mm512_castps_si512(_mm512_cvtepi32_ps(y));
        auto biased_exponents = _mm512_srli_epi32(floats, 23);
        biased_exponents = _mm512_min_epi16(_mm512_set1_epi32(158), biased_exponents);
        auto tzcnt = _mm512_sub_epi32(biased_exponents, _mm512_set1_epi32(127));
        tzcnt = _mm512_mask_mov_epi32(tzcnt, is_zero, _mm512_set1_epi32(32));
        return vec16x32u{tzcnt};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countr_one(vec16x32u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u bit_width(vec16x32u x) {
        #if defined(AVEL_AVX512CD)
        return vec16x32u{32} - vec16x32u{_mm512_lzcnt_epi32(decay(x))};

        #elif defined(AVEL_AVX512F)
        return vec16x32u{32} - vec16x32u{countl_zero(x)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u bit_floor(vec16x32u x) {
        #if defined(AVEL_AVX512CD)
        vec16x32u leading_zeros = countl_zero(x);
        mask16x32u zero_mask = (leading_zeros != vec16x32u{32});

        return (vec16x32u{zero_mask} << (vec16x32u{31} - leading_zeros));
        #elif defined(AVEL_AVX512F)
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u bit_ceil(vec16x32u v) {
        #if defined(AVEL_AVX512CD)
        auto is_zero = _mm512_cmpeq_epi32_mask(decay(v), _mm512_setzero_si512());
        auto ones = _mm512_set1_epi32(1);
        auto sh = _mm512_sub_epi32(_mm512_set1_epi32(32), _mm512_lzcnt_epi32(_mm512_sub_epi32(decay(v), ones)));
        auto result = _mm512_sllv_epi32(ones, sh);
        return vec16x32u{_mm512_mask_add_epi32(result, is_zero, result, ones)};

        #elif defined(AVEL_AVX512F)
        auto zero_mask = (v == vec16x32u{0x00});
        //TODO: Optimize

        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v = _mm512_andnot_si512(decay(broadcast_mask(zero_mask)), decay(v));
        ++v;

        return v;
        #endif
    };

    [[nodiscard]]
    AVEL_FINL mask16x32u has_single_bit(vec16x32u v) {
        #if defined(AVEL_AVX512VPOPCNTDQ)
        return popcount(v) == vec16x32u{1};

        #elif defined(AVEL_AVX512BITALG)
        return popcount(v) == vec16x32u{1};

        #else
        return mask16x32u{v} & !mask16x32u{v & (v - vec16x32u{1})};
        #endif
    }

}

#endif //AVEL_VEC16X32U_HPP
