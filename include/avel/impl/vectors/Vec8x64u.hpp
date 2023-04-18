#ifndef AVEL_VEC8X64U_HPP
#define AVEL_VEC8X64U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x64u = Vector<std::uint64_t, 8>;
    using arr8x64u = std::array<std::uint64_t, 8>;
    using mask8x64u = Vector_mask<std::uint64_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x64u> div(vec8x64u numerator, vec8x64u denominator);
    vec8x64u set_bits(mask8x64u m);
    vec8x64u blend(mask8x64u m, vec8x64u a, vec8x64u b);
    vec8x64u countl_one(vec8x64u x);





    template<>
    class Vector_mask<std::uint64_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512F)
        using primitive = __mmask8;
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
        #if defined(AVEL_AVX512F)
            content(b ? -1 : 0) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr8xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            content = _mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data);

            #elif defined(AVEL_AVX512F)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            auto expanded = _mm512_castsi256_si512(_mm256_cvtepi8_epi32(array_data));
            content = _mm512_cmplt_epu32_mask(_mm512_setzero_si512(), expanded);

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
            #if defined(AVEL_AVX512DQ)
            auto tmp = _kxor_mask8(decay(lhs), decay(rhs));
            return _kortestz_mask8_u8(tmp, tmp);

            #elif defined(AVEL_AVX512F)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512DQ)
            auto tmp = _kxor_mask8(decay(lhs), decay(rhs));
            return !_kortestz_mask8_u8(tmp, tmp);

            #elif defined(AVEL_AVX512F)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512F)
            content &= rhs.content;
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512F)
            content |= rhs.content;
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512F)
            content ^= rhs.content;
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512DQ)
            return Vector_mask{_knot_mask8(content)};

            #elif defined(AVEL_AVX512F)
            return Vector_mask{static_cast<primitive>(~content)};
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
    AVEL_FINL std::uint32_t count(mask8x64u m) {
        #if defined(AVEL_AVX512F)
        return popcount(decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x64u m) {
        #if defined(AVEL_AVX512DQ)
        return !_kortestz_mask8_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX512F)
        return !_kortestz_mask16_u8(decay(m), decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask8x64u m) {
        #if defined(AVEL_AVX512DQ)
        return _kortestc_mask8_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX512F)
        return 0xFF == decay(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x64u m) {
        #if defined(AVEL_AVX512DQ)
        return _kortestz_mask8_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX512F)
        return _kortestz_mask16_u8(decay(m), decay(m));
        #endif
    }





    template<>
    class Vector<std::uint64_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint64_t;

        #if defined(AVEL_AVX512F)
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
        #if defined(AVEL_AVX512F)
            content(_mm512_maskz_set1_epi64(decay(m), 0x1)) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX512F)
            content(_mm512_set1_epi64(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr8x64u& arr):
        #if defined(AVEL_AVX512F)
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
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmpeq_epi64_mask(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmpneq_epi64_mask(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmplt_epu64_mask(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmple_epu64_mask(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmpgt_epu64_mask(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmpge_epu64_mask(lhs.content, rhs.content)};
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
            #if defined(AVEL_AVX512F)
            content = _mm512_add_epi64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_sub_epi64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512DQ)
            content = _mm512_mullo_epi64(content, decay(rhs));
            #elif defined(AVEL_AVX512F)
            content = _mm512_mullox_epi64(content, rhs.content);
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
            #if defined(AVEL_AVX512F)
            content = _mm512_and_si512(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_or_si512(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_xor_si512(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_sll_epi64(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_srl_epi64(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_sllv_epi64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_srlv_epi64(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512F)
            return Vector{_mm512_ternarylogic_epi64(content, content, content, 0x01)};
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
            #if defined(AVEL_AVX512F)
            return mask{_mm512_test_epi64_mask(content, content)};
            #endif
        }

    };

    static_assert(
        8 * sizeof(std::uint64_t) == sizeof(vec8x64u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint64_t extract(vec8x64u v) {
        static_assert(N <= 8, "Specified index does not exist");
        typename std::enable_if<N <= 8, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512F)
        auto quarter = _mm512_extracti32x4_epi32(decay(v), N / 2);
        return _mm_extract_epi64(quarter, N % 2);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec8x64u insert(vec8x64u v, std::uint64_t x) {
        static_assert(N <= 8, "Specified index does not exist");
        typename std::enable_if<N <= 8, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512F)
        auto quarter = _mm512_extracti32x4_epi32(decay(v), N / 2);
        quarter = _mm_insert_epi64(quarter, x, N % 2);
        return vec8x64u{_mm512_inserti32x4(decay(v), quarter, N / 2)};
        #endif
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x64u bit_shift_left(vec8x64u v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512F)
        v <<= S;
        return v;
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64u bit_shift_left<0>(vec8x64u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64u bit_shift_left<1>(vec8x64u v) {
        return v + v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64u bit_shift_left<64>(vec8x64u v) {
        (void)v;
        return vec8x64u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x64u bit_shift_right(vec8x64u v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_srli_epi64(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64u bit_shift_right<0>(vec8x64u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64u bit_shift_right<64>(vec8x64u v) {
        (void)v;
        return vec8x64u{0x00};
    }




    template<std::uint32_t S, typename std::enable_if<S < 64, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x64u rotl(vec8x64u v) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_rol_epi64(decay(v), S)};
        #endif
    }

    template<>
    AVEL_FINL vec8x64u rotl<0>(vec8x64u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<64 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x64u rotl(vec8x64u v) {
        return rotl<S % 64>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u rotl(vec8x64u v, long long s) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_rolv_epi64(decay(v), _mm512_set1_epi64(s))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u rotl(vec8x64u v, vec8x64u s) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_rolv_epi64(decay(v), decay(s))};
        #endif
    }



    template<std::uint32_t S, typename std::enable_if<S < 64, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x64u rotr(vec8x64u v) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_ror_epi64(decay(v), S)};
        #endif
    }

    template<>
    AVEL_FINL vec8x64u rotr<0>(vec8x64u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<64 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x64u rotr(vec8x64u v) {
        return rotr<S % 64>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u rotr(vec8x64u v, long long s) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_rorv_epi64(decay(v), _mm512_set1_epi64(s))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u rotr(vec8x64u v, vec8x64u s) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_rorv_epi64(decay(v), decay(s))};
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec8x64u v) {
        return count(mask8x64u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec8x64u v) {
        return any(mask8x64u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec8x64u v) {
        return all(mask8x64u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec8x64u v) {
        return none(mask8x64u{v});
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u set_bits(mask8x64u m) {
        #if defined(AVEL_AVX512DQ)
        return vec8x64u{_mm512_movm_epi64(decay(m))};

        #elif defined(AVEL_AVX512F)
        return vec8x64u{_mm512_maskz_set1_epi64(decay(m), -1ull)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u keep(mask8x64u m, vec8x64u v) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_maskz_mov_epi64(decay(m), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u clear(mask8x64u m, vec8x64u v) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_maskz_mov_epi64(decay(!m), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u blend(mask8x64u m, vec8x64u a, vec8x64u b) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_mask_blend_epi64(decay(m), decay(b), decay(a))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u byteswap(vec8x64u v) {
        #if defined(AVEL_AVX512BW)
        alignas(16) static constexpr std::uint8_t index_data[16] {
             7,  6,  5,  4,
             3,  2,  1,  0,
            15, 14, 13, 12,
            11, 10,  9,  8
        };

        auto table = _mm_load_si128((const __m128i*)index_data);
        auto indices = _mm512_shuffle_i64x2(
            _mm512_castsi128_si512(table),
            _mm512_castsi128_si512(table),
            0x00
        );

        return vec8x64u{_mm512_shuffle_epi8(decay(v), indices)};

        #elif defined(AVEL_AVX512F)
        alignas(32) static constexpr std::uint8_t index_data[32] {
             7,  6,  5,  4,
             3,  2,  1,  0,
            15, 14, 13, 12,
            11, 10,  9,  8,
             7,  6,  5,  4,
             3,  2,  1,  0,
            15, 14, 13, 12,
            11, 10,  9,  8
        };

        auto indices = _mm256_load_si256(reinterpret_cast<const __m256i*>(index_data));

        auto lo = _mm512_extracti64x4_epi64(decay(v), 0x0);
        auto hi = _mm512_extracti64x4_epi64(decay(v), 0x1);

        auto ret_lo = _mm256_shuffle_epi8(lo, indices);
        auto ret_hi = _mm256_shuffle_epi8(hi, indices);

        auto ret = _mm512_castsi256_si512(ret_lo);
        ret = _mm512_inserti64x4(ret, ret_hi, 0x1);
        return vec8x64u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u max(vec8x64u a, vec8x64u b) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_max_epu64(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u min(vec8x64u a, vec8x64u b) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_min_epu64(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x64u, 2> minmax(vec8x64u a, vec8x64u b) {
        #if defined(AVEL_AVX512F)
        return {
            vec8x64u{_mm512_min_epu64(decay(a), decay(b))},
            vec8x64u{_mm512_max_epu64(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u clamp(vec8x64u x, vec8x64u lo, vec8x64u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u average(vec8x64u a, vec8x64u b) {
        #if defined(AVEL_AVX512F)
        return ((a ^ b) >> 1) + (a & b);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u midpoint(vec8x64u a, vec8x64u b) {
        #if defined(AVEL_AVX512F)
        vec8x64u t0 = a & b & vec8x64u{0x1};
        vec8x64u t1 = (a | b) & vec8x64u{0x1} & set_bits(a > b);
        vec8x64u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
        #endif
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64u load<vec8x64u>(const std::uint64_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 8) ? 0xff : (1 << n) - 1;
        return vec8x64u{_mm512_maskz_loadu_epi64(mask, ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64u load<vec8x64u, vec8x64u::width>(const std::uint64_t* ptr) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_loadu_si512(ptr)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64u aligned_load<vec8x64u>(const std::uint64_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x64u{_mm512_maskz_load_epi64(mask, ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64u aligned_load<vec8x64u, vec8x64u::width>(const std::uint64_t* ptr) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{_mm512_load_epi64(ptr)};
        #endif
    }

    //Definition of gather deferred until vec8x64i defined




    AVEL_FINL void store(std::uint64_t* ptr, vec8x64u v, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 8) ? 0xff : (1 << n) - 1;
        _mm512_mask_storeu_epi64(ptr, mask, decay(v));
        #endif
    }

    template<std::uint32_t N = vec8x64u::width>
    AVEL_FINL void store(std::uint64_t* ptr, vec8x64u v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec8x64u::width>(std::uint64_t* ptr, vec8x64u v) {
        #if defined(AVEL_AVX512F)
        _mm512_storeu_si512(ptr, decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(std::uint64_t* ptr, vec8x64u v, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 8) ? 0xff : (1 << n) - 1;
        _mm512_mask_store_epi64(ptr, mask, decay(v));
        #endif
    }

    template<std::uint32_t N = vec8x64u::width>
    AVEL_FINL void aligned_store(std::uint64_t* ptr, vec8x64u v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec8x64u::width>(std::uint64_t* ptr, vec8x64u v) {
        #if defined(AVEL_AVX512F)
        _mm512_store_si512(ptr, decay(v));
        #endif
    }

    //Definition of scatter deferred until vec8x64i defined



    [[nodiscard]]
    AVEL_FINL arr8x64u to_array(vec8x64u x) {
        alignas(64) arr8x64u ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x64u> div(vec8x64u x, vec8x64u y) {
        auto n0 = extract<0>(x);
        auto n1 = extract<1>(x);
        auto n2 = extract<2>(x);
        auto n3 = extract<3>(x);
        auto n4 = extract<4>(x);
        auto n5 = extract<5>(x);
        auto n6 = extract<6>(x);
        auto n7 = extract<7>(x);

        auto d0 = extract<0>(y);
        auto d1 = extract<1>(y);
        auto d2 = extract<2>(y);
        auto d3 = extract<3>(y);
        auto d4 = extract<4>(y);
        auto d5 = extract<5>(y);
        auto d6 = extract<6>(y);
        auto d7 = extract<7>(y);

        vec8x64u quotient{};
        quotient = insert<0>(quotient, n0 / d0);
        quotient = insert<1>(quotient, n1 / d1);
        quotient = insert<2>(quotient, n2 / d2);
        quotient = insert<3>(quotient, n3 / d3);
        quotient = insert<4>(quotient, n4 / d4);
        quotient = insert<5>(quotient, n5 / d5);
        quotient = insert<6>(quotient, n6 / d6);
        quotient = insert<7>(quotient, n7 / d7);

        vec8x64u remainder{};
        remainder = insert<0>(remainder, n0 % d0);
        remainder = insert<1>(remainder, n1 % d1);
        remainder = insert<2>(remainder, n2 % d2);
        remainder = insert<3>(remainder, n3 % d3);
        remainder = insert<4>(remainder, n4 % d4);
        remainder = insert<5>(remainder, n5 % d5);
        remainder = insert<6>(remainder, n6 % d6);
        remainder = insert<7>(remainder, n7 % d7);

        return {quotient, remainder};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u popcount(vec8x64u v) {
        #if defined(AVEL_AVX512VPOPCNTDQ)
        return vec8x64u{_mm512_popcnt_epi64(decay(v))};

        #elif defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm512_popcnt_epi16(decay(v));
        auto tmp1 = _mm512_slli_epi64(tmp0, 32);
        auto tmp2 = _mm512_add_epi32(tmp0, tmp1);
        auto tmp3 = _mm512_slli_epi32(tmp2, 16);
        auto tmp4 = _mm512_add_epi32(tmp2, tmp3);
        return vec8x64u{_mm512_srli_epi64(tmp4, 48)};

        #elif defined(AVEL_AVX512F)
        //TODO: Consider alternative implementations
        auto c0 = popcount(extract<0>(v));
        auto c1 = popcount(extract<1>(v));
        auto c2 = popcount(extract<2>(v));
        auto c3 = popcount(extract<3>(v));
        auto c4 = popcount(extract<4>(v));
        auto c5 = popcount(extract<5>(v));
        auto c6 = popcount(extract<6>(v));
        auto c7 = popcount(extract<7>(v));

        return vec8x64u{_mm512_set_epi64(c7, c6, c5, c4, c3, c2, c1, c0)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u countl_zero(vec8x64u v) {
        #if defined(AVEL_AVX512CD)
        return vec8x64u{_mm512_lzcnt_epi64(decay(v))};

        //TODO: Consider alternative approaches

        #elif defined(AVEL_AVX512F)
        auto c0 = countl_zero(extract<0>(v));
        auto c1 = countl_zero(extract<1>(v));
        auto c2 = countl_zero(extract<2>(v));
        auto c3 = countl_zero(extract<3>(v));
        auto c4 = countl_zero(extract<4>(v));
        auto c5 = countl_zero(extract<5>(v));
        auto c6 = countl_zero(extract<6>(v));
        auto c7 = countl_zero(extract<7>(v));

        return vec8x64u{_mm512_set_epi64(c7, c6, c5, c4, c3, c2, c1, c0)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u countl_one(vec8x64u v) {
        #if defined(AVEL_AVX512F)
        return countl_zero(~v);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u countr_zero(vec8x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)
        auto neg_one = _mm512_set1_epi64(-1);
        auto tz_mask = _mm512_andnot_si512(decay(v), _mm512_add_epi64(decay(v), neg_one));
        return vec8x64u{_mm512_popcnt_epi64(tz_mask)};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto zero_mask = (v == vec8x64u{0x00});
        auto y = (v & (vec8x64u{0x00} - v));
        auto z = vec8x64u{63} - countl_zero(y);
        return blend(zero_mask, vec8x64u{64}, z);

        #elif defined(AVEL_AVX512F)
        auto c0 = countr_zero(extract<0>(v));
        auto c1 = countr_zero(extract<1>(v));
        auto c2 = countr_zero(extract<2>(v));
        auto c3 = countr_zero(extract<3>(v));
        auto c4 = countr_zero(extract<4>(v));
        auto c5 = countr_zero(extract<5>(v));
        auto c6 = countr_zero(extract<6>(v));
        auto c7 = countr_zero(extract<7>(v));

        return vec8x64u{_mm512_set_epi64(c7, c6, c5, c4, c3, c2, c1, c0)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u countr_one(vec8x64u v) {
        return countr_zero(~v);
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u bit_width(vec8x64u v) {
        #if defined(AVEL_AVX512F)
        return vec8x64u{64} - vec8x64u{countl_zero(v)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u bit_floor(vec8x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        vec8x64u leading_zeros = countl_zero(v);
        mask8x64u zero_mask = (leading_zeros != vec8x64u{64});

        return (vec8x64u{zero_mask} << (vec8x64u{63} - leading_zeros));

        #elif defined(AVEL_AVX512F)
        v = v | (v >> 1);
        v = v | (v >> 2);
        v = v | (v >> 4);
        v = v | (v >> 8);
        v = v | (v >> 16);
        v = v | (v >> 32);
        return v - (v >> 1);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64u bit_ceil(vec8x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto sh = (vec8x64u{64} - countl_zero(v - vec8x64u{1}));
        auto result = vec8x64u{1} << sh;
        return result - set_bits(v == vec8x64u{0x00});

        #elif defined(AVEL_AVX512F)
        auto zero_mask = (v == vec8x64u{0});

        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;
        ++v;

        return v - set_bits(zero_mask);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x64u has_single_bit(vec8x64u v) {
        #if defined(AVX512VPOPCNTDQ)
        return mask8x64u{popcount(x) == vec8x64u{1}};

        #elif defined(AVEL_AVX512F)
        return mask8x64u{v} & !mask8x64u{v & (v - vec8x64u{1})};
        #endif
    }

}

#endif //AVEL_VEC8X64U_HPP
