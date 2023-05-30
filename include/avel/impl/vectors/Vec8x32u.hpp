#ifndef AVEL_VEC8X32U_HPP
#define AVEL_VEC8X32U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x32u = Vector<std::uint32_t, 8>;
    using arr8x32u = std::array<std::uint32_t, 8>;
    using mask8x32u = Vector_mask<std::uint32_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x32u> div(vec8x32u x, vec8x32u y);
    vec8x32u set_bits(mask8x32u m);
    vec8x32u blend(mask8x32u m, vec8x32u a, vec8x32u b);
    vec8x32u countl_one(vec8x32u x);





    template<>
    class Vector_mask<std::uint32_t, 8> {
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

        //=================================================
        // Constructor
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> m):
            Vector_mask(convert<Vector_mask>(m)[0]) {}

        AVEL_FINL Vector_mask(primitive content):
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

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask8x32u m) {
        #if defined(AVEL_AVX512VL)
        return popcount(decay(m));

        #elif defined(AVEL_AVX2)
        return popcount(_mm256_movemask_epi8(decay(m))) / sizeof(std::uint32_t);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return !_kortestz_mask8_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX512VL)
        return !_kortestz_mask16_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX2)
        return _mm256_movemask_epi8(decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask8x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return _kortestc_mask8_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX512VL)
        return 0xFF == decay(m);

        #elif defined(AVEL_AVX2)
        return 0xFFFFFFFFu == _mm256_movemask_epi8(decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x32u m) {
        #if defined(AVEL_AVX512VL)
        return _kortestz_mask16_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX2)
        return _mm256_testz_si256(decay(m), decay(m));
        #endif
    }





    template<>
    class Vector<std::uint32_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint32_t;

        #if defined(AVEL_AVX2)
        using primitive = __m256i;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<unsigned M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            Vector(convert<Vector>(v)[0]) {}

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
            content(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(array.data()))) {}

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
            return mask{_mm256_cmpeq_epu32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpeq_epi32(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpneq_epu32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_AVX2)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmplt_epu32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_AVX2)
            Vector addition_mask{0x80000000};
            return mask{_mm256_cmpgt_epi32(decay(rhs ^ addition_mask), decay(lhs ^ addition_mask))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmple_epu32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_AVX2)
            return !mask{lhs > rhs};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpgt_epu32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_AVX2)
            Vector addition_mask{0x80000000};
            return mask{_mm256_cmpgt_epi32(decay(lhs ^ addition_mask), decay(rhs ^ addition_mask))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpge_epu32_mask(lhs.content, rhs.content)};

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

        //Definition of operator-() deferred until later

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content = _mm256_add_epi32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm256_sub_epi32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm256_mullo_epi32(content, rhs.content);
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
            content = _mm256_and_si256(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            content = _mm256_or_si256(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            content = _mm256_xor_si256(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            content = _mm256_sll_epi32(content, _mm_cvtsi64_si128(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            content = _mm256_srl_epi32(content, _mm_cvtsi64_si128(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            content = _mm256_sllv_epi32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            content = _mm256_srlv_epi32(content, rhs.content);
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

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_test_epi32_mask(content, content)};

            #elif defined(AVEL_AVX2)
            return Vector{0x00} != *this;
            #endif
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    static_assert(
        8 * sizeof(std::uint32_t) == sizeof(vec8x32u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint32_t extract(vec8x32u v) {
        static_assert(N < vec8x32u::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return _mm256_extract_epi32(decay(v), N);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec8x32u insert(vec8x32u v, std::uint32_t x) {
        static_assert(N < vec8x32u::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE41)
        return vec8x32u{_mm256_insert_epi32(decay(v), x, N)};

        #endif
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x32u bit_shift_left(vec8x32u v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return vec8x32u{_mm256_slli_epi32(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u bit_shift_left<0>(vec8x32u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u bit_shift_left<32>(vec8x32u v) {
        return vec8x32u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x32u bit_shift_right(vec8x32u v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return vec8x32u{_mm256_srli_epi32(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u bit_shift_right<0>(vec8x32u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u bit_shift_right<32>(vec8x32u v) {
        (void)v;
        return vec8x32u{0x00};
    }



    template<std::uint32_t S, typename std::enable_if<S < 32, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x32u rotl(vec8x32u v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rol_epi32(decay(v), S)};

        #elif defined(AVEL_AVX2)
        auto left_shifted  = _mm256_slli_epi32(decay(v), S);
        auto right_shifted = _mm256_srli_epi32(decay(v), 32 - S);

        auto ret = _mm256_or_si256(left_shifted, right_shifted);
        return vec8x32u{ret};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u rotl<0>(vec8x32u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<32 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x32u rotl(vec8x32u v) {
        return rotl<S % 32>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u rotl(vec8x32u v, long long s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rolv_epi32(decay(v), _mm256_set1_epi32(s))};

        #elif defined(AVEL_AVX2)
        s &= 0x1F;
        return (v << s) | (v >> (32 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u rotl(vec8x32u v, vec8x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rolv_epi32(decay(v), decay(s))};

        #elif defined(AVEL_AVX2)
        s &= vec8x32u(0x1F);
        return (v << s) | (v >> (vec8x32u{32} - s));
        #endif
    }



    template<std::uint32_t S, typename std::enable_if<S < 32, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x32u rotr(vec8x32u v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_ror_epi32(decay(v), S)};

        #elif defined(AVEL_AVX2)
        auto left_shifted  = _mm256_slli_epi32(decay(v), 32 - S);
        auto right_shifted = _mm256_srli_epi32(decay(v), S);

        auto ret = _mm256_or_si256(left_shifted, right_shifted);
        return vec8x32u{ret};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u rotr<0>(vec8x32u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<32 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x32u rotr(vec8x32u v) {
        return rotr<S % 32>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u rotr(vec8x32u v, long long s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rorv_epi32(decay(v), _mm256_set1_epi32(s))};

        #elif defined(AVEL_AVX2)
        s &= 0x1F;
        return (v >> s) | (v << (32 - s));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u rotr(vec8x32u v, vec8x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rorv_epi32(decay(v), decay(s))};

        #elif defined(AVEL_AVX2)
        s &= vec8x32u(0x1F);
        return (v >> s) | (v << (vec8x32u{32} - s));

        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================
    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec8x32u x) {
        #if defined(AVEL_AVX512VL)
        return count(mask8x32u{x});

        #elif defined(AVEL_AVX2)
        auto compared = _mm256_cmpeq_epi32(decay(x), _mm256_setzero_si256());
        return 8 - (popcount(_mm256_movemask_epi8(compared)) / sizeof(vec8x32u::scalar));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec8x32u x) {
        #if defined(AVEL_AVX2)
        return !_mm256_testz_si256(decay(x), decay(x));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec8x32u x) {
        #if defined(AVEL_AVX2)
        auto compared = _mm256_cmpeq_epi32(decay(x), _mm256_setzero_si256());
        return 0x00 == _mm256_movemask_epi8(compared);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec8x32u v) {
        #if defined(AVEL_AVX2)
        return _mm256_testz_si256(decay(v), decay(v));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u set_bits(mask8x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec8x32u{_mm256_movm_epi32(decay(m))};

        #elif defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_maskz_set1_epi32(decay(m), -1)};

        #elif defined(AVEL_AVX2)
        return vec8x32u{mask8x32u::primitive(m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u keep(mask8x32u m, vec8x32u v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_maskz_mov_epi32(decay(m), decay(v))};

        #elif defined(AVEL_AVX2)
        return set_bits(m) & v;

        #endif

    }

    [[nodiscard]]
    AVEL_FINL vec8x32u clear(mask8x32u m, vec8x32u v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_maskz_mov_epi32(decay(!m), decay(v))};

        #elif defined(AVEL_AVX2)
        return vec8x32u{_mm256_andnot_si256(decay(m), decay(v))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u blend(mask8x32u m, vec8x32u a, vec8x32u b) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_mask_blend_epi32(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_AVX2)
        return vec8x32u{_mm256_blendv_epi8(decay(b), decay(a), decay(m))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u max(vec8x32u a, vec8x32u b) {
        return vec8x32u{_mm256_max_epu32(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u min(vec8x32u a, vec8x32u b) {
        return vec8x32u{_mm256_min_epu32(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x32u, 2> minmax(vec8x32u a, vec8x32u b) {
        return {
            vec8x32u{_mm256_min_epu32(decay(a), decay(b))},
            vec8x32u{_mm256_max_epu32(decay(a), decay(b))}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u clamp(vec8x32u x, vec8x32u lo, vec8x32u hi) {
        return vec8x32u{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u average(vec8x32u a, vec8x32u b) {
        return (a >> 1) + (b >> 1) + (a & b & vec8x32u{0x1});;
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u midpoint(vec8x32u a, vec8x32u b) {
        vec8x32u t0 = a & b & vec8x32u{0x1};
        vec8x32u t1 = (a | b) & vec8x32u{0x1} & set_bits(a > b);
        vec8x32u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    //Definition of neg_abs delayed until vec4x32i is defined

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u load<vec8x32u>(const std::uint32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32u{_mm256_maskz_loadu_epi32(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n > 4) {
            auto lo = load<vec4x32u>(ptr + 0, n - 0);
            auto hi = load<vec4x32u>(ptr + 4, n - 4);
            return vec8x32u{_mm256_set_m128i(decay(hi), decay(lo))};
        } else {
            auto lo = load<vec4x32u>(ptr, n);
            return vec8x32u{_mm256_set_m128i(_mm_setzero_si128(), decay(lo))};
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u load<vec8x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec8x32u{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u aligned_load<vec8x32u>(const std::uint32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32u{_mm256_maskz_load_epi32(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n > 4) {
            auto lo = aligned_load<vec4x32u>(ptr + 0, n - 0);
            auto hi = aligned_load<vec4x32u>(ptr + 4, n - 4);
            return vec8x32u{_mm256_set_m128i(decay(hi), decay(lo))};
        } else {
            auto lo = aligned_load<vec4x32u>(ptr, n);
            return vec8x32u{_mm256_set_m128i(_mm_setzero_si128(), decay(lo))};
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u aligned_load<vec8x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec8x32u{_mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }

    //Definition of gather deferred until vector of signed integers is defined

    AVEL_FINL void store(std::uint32_t* ptr, vec8x32u v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_storeu_epi32(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n > 4) {
            store(ptr + 0, vec4x32u{_mm256_castsi256_si128(decay(v))}, n - 0);
            store(ptr + 4, vec4x32u{_mm256_extracti128_si256(decay(v), 0x1)}, n - 4);
        } else {
            store(ptr, vec4x32u{_mm256_castsi256_si128(decay(v))}, n);
        }

        #endif
    }

    template<std::uint32_t N = vec8x32u::width>
    AVEL_FINL void store(std::uint32_t* ptr, vec8x32u x) {
        static_assert(N <= vec8x32u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec8x32u::width, int>::type dummy_variable = 0;

        store(ptr, x, N);
    }

    //TODO: Add specializations

    AVEL_FINL void store(std::uint32_t* ptr, vec8x32u v) {
        #if defined(AVEL_AVX2)
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
   }



    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec8x32u v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_store_epi32(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n > 4) {
            aligned_store(ptr + 0, vec4x32u{_mm256_castsi256_si128(decay(v))}, n - 0);
            aligned_store(ptr + 4, vec4x32u{_mm256_extracti128_si256(decay(v), 0x1)}, n - 4);
        } else {
            aligned_store(ptr, vec4x32u{_mm256_castsi256_si128(decay(v))}, n);
        }

        #endif
    }

    template<std::uint32_t N = vec8x32u::width>
    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec8x32u x) {
        static_assert(N <= vec8x32u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec8x32u::width, int>::type dummy_variable = 0;

        aligned_store(ptr, x, N);
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec8x32u v) {
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }

    //Definition of scatter deferred until vector of signed integers is defined



    [[nodiscard]]
    AVEL_FINL arr8x32u to_array(vec8x32u v) {
        alignas(32) std::array<std::uint32_t, 8> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer arithmetic functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x32u> div(vec8x32u x, vec8x32u y) {
        #if defined(AVEL_AVX512VL)
        auto x_widened = _mm512_cvtepu32_pd(decay(x));
        auto y_widened = _mm512_cvtepu32_pd(decay(y));

        auto quotient = _mm512_cvttpd_epu32(_mm512_div_pd(x_widened, y_widened));

        auto offset = _mm256_mullo_epi32(decay(y), quotient);
        auto remainder = _mm256_sub_epi32(decay(x), offset);

        return {
            vec8x32u{quotient},
            vec8x32u{remainder}
        };

        #elif defined(AVEL_AVX2)
        //TODO: Optimize
        vec8x32u quotient{};
        std::int32_t i = 32;
        for (; (i-- > 0) && any(mask8x32u(x));) {
            mask8x32u b = ((x >> i) >= y);
            x -= (set_bits(b) & (y << i));
            quotient |= (vec8x32u{b} << i);
        }

        return {quotient, x};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u popcount(vec8x32u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)
        return vec8x32u{_mm256_popcnt_epi32(decay(v))};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm256_popcnt_epi16(decay(v));
        auto tmp1 = _mm256_slli_epi32(tmp0, 16);
        auto tmp2 = _mm256_add_epi32(tmp0, tmp1);
        return vec8x32u{_mm256_srli_epi32(tmp2, 16)};

        #elif defined(AVEL_AVX2)
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec8x32u{0x55555555});                    // reuse input as temporary
        v = (v & vec8x32u{0x33333333}) + ((v >> 2) & vec8x32u{0x33333333});     // temp
        v = ((v + (v >> 4) & vec8x32u{0xF0F0F0F}) * vec8x32u{0x1010101}) >> 24; // count
        return v;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u byteswap(vec8x32u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        // Reverse 16-bit halves within 32-bit ints
        auto t0 = _mm256_shldi_epi32(decay(v), decay(v), 16);

        // Reverse 8-bit halves within 16-bit ints
        auto t1 = _mm256_shldi_epi16(t0, t0, 8);

        return vec8x32u{t1};


        #elif defined(AVEL_AVX2)
        alignas(32) static constexpr std::uint8_t index_data[32] {
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12,
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12,
        };

        auto indices = _mm256_load_si256((const __m256i*)index_data);
        auto ret = vec8x32u{_mm256_shuffle_epi8(decay(v), indices)};
        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u countl_zero(vec8x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec8x32u{_mm256_lzcnt_epi32(decay(x))};

        #elif defined(AVEL_AVX2)
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        //TODO: Clean this mess up
        x = _mm256_andnot_si256(decay(x >> 1), decay(x));
        auto floats = _mm256_add_ps(_mm256_cvtepi32_ps(decay(x)), _mm256_set1_ps(0.5f));
        auto biased_exponents = (vec8x32u(_mm256_castps_si256(floats)) >> 23);
        auto lzcnt = _mm256_subs_epu16(decay(vec8x32u{158}), decay(biased_exponents));
        return vec8x32u{lzcnt};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u countl_one(vec8x32u x) {
        return countl_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u countr_zero(vec8x32u x) {
        //TODO: Use lzcnt
        //TODO: Other optimize
        //TODO: Clean this mess up

        auto y = decay(x & (vec8x32u{} - x));
        auto floats = _mm256_cvtepi32_ps(y);
        auto biased_exponents = (vec8x32u(_mm256_castps_si256(floats)) >> 23);
        biased_exponents = _mm256_min_epi16(decay(vec8x32u{158}), decay(biased_exponents));
        auto tzcnt = biased_exponents - vec8x32u{127};
        tzcnt = blend(x == vec8x32u{0x00}, vec8x32u{32}, tzcnt);

        return vec8x32u{tzcnt};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u countr_one(vec8x32u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u bit_width(vec8x32u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec8x32u{32} - vec8x32u{_mm256_lzcnt_epi32(decay(v))};

        #elif defined(AVEL_AVX2)
        return vec8x32u{32} - countl_zero(v);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u bit_floor(vec8x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        vec8x32u leading_zeros = countl_zero(x);
        mask8x32u zero_mask = (leading_zeros != vec8x32u{32});

        return (vec8x32u{zero_mask} << (vec8x32u{31} - leading_zeros));
        #elif defined(AVEL_AVX2)
        //TODO: Optimize ?
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u bit_ceil(vec8x32u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto ones = _mm256_set1_epi32(1);
        auto sh = _mm256_sub_epi32(_mm256_set1_epi32(32), _mm256_lzcnt_epi32(_mm256_sub_epi32(decay(v), ones)));
        auto result = _mm256_sllv_epi32(ones, sh);
        return vec8x32u{_mm256_sub_epi32(result, _mm256_cmpeq_epi32(decay(v), _mm256_setzero_si256()))};

        #elif defined(AVEL_AVX2)
        auto zero_mask = (v == vec8x32u{0x00});

        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v = _mm256_andnot_si256(decay(set_bits(zero_mask)), decay(v));
        ++v;
        return v;
        #endif
    };

    [[nodiscard]]
    AVEL_FINL mask8x32u has_single_bit(vec8x32u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)
        return popcount(v) == vec8x32u{1};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        return popcount(v) == vec8x32u{1};

        #elif defined(AVEL_AVX2)
        return mask8x32u{v} & !mask8x32u{v & (v - vec8x32u{1})};
        #endif
    }

}

#endif //AVEL_VEC8X32U_HPP
