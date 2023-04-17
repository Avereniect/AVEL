#ifndef AVEL_VEC32X16_HPP
#define AVEL_VEC32X16_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec32x16u = Vector<std::uint16_t, 32>;
    using arr32x16u = std::array<std::uint16_t, 32>;
    using mask32x16u = Vector_mask<std::uint16_t, 32>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec32x16u> div(vec32x16u numerator, vec32x16u denominator);
    vec32x16u broadcast_mask(mask32x16u m);
    vec32x16u blend(vec32x16u a, vec32x16u b, mask32x16u m);
    vec32x16u countl_one(vec32x16u x);





    template<>
    class Vector_mask<std::uint16_t, 32> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 32;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = __mmask32;

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
            content(p) {};

        AVEL_FINL explicit Vector_mask(bool b):
            content(b ? -1 : 0) {}

        AVEL_FINL explicit Vector_mask(const arr32xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()));
            content = _mm256_cmplt_epu8_mask(_mm256_setzero_si256(), array_data);

            #elif defined(AVEL_AVX512BW)
            auto array_data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()));
            auto widened = _mm512_zextsi256_si512(array_data);
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

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            return Vector_mask{static_cast<primitive>(~content)};
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
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask32x16u m) {
        return popcount(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask32x16u m) {
        #if defined(AVEL_AVX512BW)
        return !_kortestz_mask32_u8(decay(m), decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask32x16u m) {
        #if defined(AVEL_AVX512BW)
        return _kortestc_mask32_u8(decay(m), decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask32x16u m) {
        #if defined(AVEL_AVX512BW)
        return _kortestz_mask32_u8(decay(m), decay(m));
        #endif
    }





    template<>
    class Vector<std::uint16_t, 32> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 32;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint16_t;

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
            content(_mm512_maskz_set1_epi16(decay(m), 1)) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX512BW)
            content(_mm512_set1_epi16(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr32x16u& arr):
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
            return mask{_mm512_cmplt_epu16_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmple_epu16_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpgt_epu16_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpge_epu16_mask(decay(lhs), decay(rhs))};
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
            content = _mm512_add_epi16(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm512_sub_epi16(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm512_mullo_epi16(content, decay(rhs));
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
            content = _mm512_srl_epi16(content, _mm_cvtsi64_si128(rhs));
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
            content = _mm512_srlv_epi16(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512BW)
            return Vector{_mm512_ternarylogic_epi32(content, content, content, 0x01)};
            #endif
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
        AVEL_FINL friend Vector operator<<(Vector lhs, long long rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, long long rhs) {
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
        32 * sizeof(std::uint16_t) == sizeof(vec32x16u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint16_t extract(vec32x16u v) {
        static_assert(N < vec32x16u::width, "Specified index does not exist");
        typename std::enable_if<N < vec32x16u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512BW)
        auto quarter = _mm512_extracti32x4_epi32(decay(v), N / 8);
        return _mm_extract_epi16(quarter, N % 8);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec32x16u insert(vec32x16u v, std::uint16_t x) {
        static_assert(N < vec32x16u::width, "Specified index does not exist");
        typename std::enable_if<N < vec32x16u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512BW)
        auto quarter = _mm512_extracti32x4_epi32(decay(v), N / 8);
        quarter = _mm_insert_epi16(quarter, x, N % 8);
        return vec32x16u{_mm512_inserti32x4(decay(v), quarter, N / 8)};
        #endif
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec32x16u bit_shift_left(vec32x16u v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512BW)
        return vec32x16u{_mm512_slli_epi16(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16u bit_shift_left<0>(vec32x16u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16u bit_shift_left<1>(vec32x16u v) {
        return v + v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16u bit_shift_left<16>(vec32x16u v) {
        (void)v;
        return vec32x16u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec32x16u bit_shift_right(vec32x16u v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512BW)
        return vec32x16u{_mm512_srli_epi16(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16u bit_shift_right<0>(vec32x16u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16u bit_shift_right<16>(vec32x16u v) {
        (void)v;
        return vec32x16u{0x00};
    }




    template<std::uint32_t S, typename std::enable_if<S < 16, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec32x16u rotl(vec32x16u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI2)
        return vec32x16u{_mm512_shldi_epi16(decay(v), decay(v), S)};

        #elif defined(AVEL_AVX512BW)
        auto left_shifted  = _mm512_slli_epi16(decay(v), S);
        auto right_shifted = _mm512_srli_epi16(decay(v), 16 - S);

        auto ret = _mm512_or_si512(left_shifted, right_shifted);
        return vec32x16u{ret};
        #endif
    }

    template<>
    AVEL_FINL vec32x16u rotl<0>(vec32x16u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<16 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec32x16u rotl(vec32x16u v) {
        return rotl<S % 16>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u rotl(vec32x16u v, long long s) {
        #if defined(AVEL_AVX512VBMI2)
        s &= 15;
        return vec32x16u{_mm512_shldv_epi16(decay(v), decay(v), _mm512_set1_epi16(s))};

        #elif defined(AVEL_AVX512BW)
        s &= 15;
        return (v << s) | (v >> (16 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u rotl(vec32x16u v, vec32x16u s) {
        #if defined(AVEL_AVX512VBMI2)
        s &= vec32x16u{15};
        return vec32x16u{_mm512_shldv_epi16(decay(v), decay(v), decay(s))};

        #elif defined(AVEL_AVX512BW)
        s &= vec32x16u{15};
        return (v << s) | (v >> (vec32x16u{16} - s));
        #endif
    }



    template<std::uint32_t S, typename std::enable_if<S < 16, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec32x16u rotr(vec32x16u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI2)
        return vec32x16u{_mm512_shrdi_epi16(decay(v), decay(v), S)};

        #elif defined(AVEL_AVX512BW)
        auto left_shifted  = _mm512_slli_epi16(decay(v), 16 - S);
        auto right_shifted = _mm512_srli_epi16(decay(v), S);

        auto ret = _mm512_or_si512(left_shifted, right_shifted);
        return vec32x16u{ret};
        #endif
    }

    template<>
    AVEL_FINL vec32x16u rotr<0>(vec32x16u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<16 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec32x16u rotr(vec32x16u v) {
        return rotr<S % 16>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u rotr(vec32x16u v, long long s) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI2)
        s &= 15;
        return vec32x16u{_mm512_shrdv_epi16(decay(v), decay(v), _mm512_set1_epi16(s))};

        #elif defined(AVEL_AVX512BW)
        s &= 15;
        return (v >> s) | (v << (16 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u rotr(vec32x16u v, vec32x16u s) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI2)
        s &= vec32x16u{15};
        return vec32x16u{_mm512_shrdv_epi16(decay(v), decay(v), decay(s))};

        #elif defined(AVEL_AVX512BW)
        s &= vec32x16u{15};
        return (v >> s) | (v << (vec32x16u{16} - s));
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec32x16u v) {
        return count(mask32x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec32x16u v) {
        return any(mask32x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec32x16u v) {
        return all(mask32x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec32x16u v) {
        return none(mask32x16u{v});
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u broadcast_mask(mask32x16u m) {
        return vec32x16u{_mm512_movm_epi16(decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u keep(mask32x16u m, vec32x16u v) {
        return vec32x16u{_mm512_maskz_mov_epi16(decay(m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u clear(mask32x16u m, vec32x16u v) {
        return vec32x16u{_mm512_maskz_mov_epi16(decay(!m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u blend(mask32x16u m, vec32x16u a, vec32x16u b) {
        return vec32x16u{_mm512_mask_blend_epi16(decay(m), decay(b), decay(a))};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u byteswap(vec32x16u v) {
        #if defined(AVEL_AVX512VBMI2)
        return vec32x16u{_mm512_shldi_epi16(decay(v), decay(v), 0x8)};

        #elif defined(AVEL_AVX512BW)
        auto lo = _mm512_srli_epi16(decay(v), 8);
        auto hi = _mm512_slli_epi16(decay(v), 8);
        return vec32x16u{_mm512_or_si512(lo, hi)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u max(vec32x16u a, vec32x16u b) {
        return vec32x16u{_mm512_max_epu16(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u min(vec32x16u a, vec32x16u b) {
        return vec32x16u{_mm512_min_epu16(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec32x16u, 2> minmax(vec32x16u a, vec32x16u b) {
        return {
            vec32x16u{_mm512_min_epu16(decay(a), decay(b))},
            vec32x16u{_mm512_max_epu16(decay(a), decay(b))}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u clamp(vec32x16u x, vec32x16u lo, vec32x16u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u average(vec32x16u a, vec32x16u b) {
        #if defined(AVEL_AVX512BW)
        return vec32x16u{_mm512_avg_epu16(decay(a), decay(b))} - ((a ^ b) & vec32x16u{0x1});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u midpoint(vec32x16u a, vec32x16u b) {
        #if defined(AVEL_AVX512BW)
        auto t1 = _mm512_avg_epu16(decay(a), decay(b));
        auto t5 = _mm512_and_si512(_mm512_ternarylogic_epi32(decay(a), decay(b), decay(broadcast_mask(b < a)), 0x14), _mm512_set1_epi16(0x1));
        auto t6 = _mm512_sub_epi16(t1, t5);
        return vec32x16u{t6};
        #endif
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16u load<vec32x16u>(const std::uint16_t* ptr, std::uint32_t n) {
        auto mask = (n >= 32) ? std::uint32_t(-1) : (1 << n) - 1;
        return vec32x16u{_mm512_maskz_loadu_epi16(mask, ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16u load<vec32x16u, vec32x16u::width>(const std::uint16_t* ptr) {
        return vec32x16u{_mm512_loadu_si512(ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16u aligned_load<vec32x16u>(const std::uint16_t* ptr, std::uint32_t n) {
        auto mask = (n >= 32) ? std::uint32_t(-1) : (1 << n) - 1;
        return vec32x16u{_mm512_maskz_loadu_epi16(mask, ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x16u aligned_load<vec32x16u, vec32x16u::width>(const std::uint16_t* ptr) {
        return vec32x16u{_mm512_load_si512(ptr)};
    }



    AVEL_FINL void store(std::uint16_t* ptr, vec32x16u v, std::uint32_t n) {
        auto mask = (n >= 32) ? std::uint32_t(-1) : (1 << n) - 1;
        _mm512_mask_storeu_epi16(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec32x16u::width>
    AVEL_FINL void store(std::uint16_t* ptr, vec32x16u v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec32x16u::width>(std::uint16_t* ptr, vec32x16u v) {
        #if defined(AVEL_AVX512BW)
        _mm512_storeu_si512(ptr, decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec32x16u v, std::uint32_t n) {
        auto mask = (n >= 32) ? std::uint32_t(-1) : (1 << n) - 1;
        _mm512_mask_storeu_epi16(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec32x16u::width>
    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec32x16u v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec32x16u::width>(std::uint16_t* ptr, vec32x16u v) {
        #if defined(AVEL_AVX512BW)
        _mm512_store_si512(ptr, decay(v));
        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr32x16u to_array(vec32x16u x) {
        alignas(64) arr32x16u ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec32x16u> div(vec32x16u x, vec32x16u y) {
        vec32x16u quotient{0x00};
        std::int32_t i = 16;
        for (; (i-- > 0) && any(x >= y);) {
            mask32x16u b = ((x >> i) >= y);
            x -= (broadcast_mask(b) & (y << i));
            quotient += quotient;
            quotient -= broadcast_mask(b);
        }

        quotient <<= (i + 1);
        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u popcount(vec32x16u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG)
        return vec32x16u{_mm512_popcnt_epi16(decay(v))};

        #elif defined(AVEL_AVX512BW)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        // TODO: Consider replacing high-latency _mm512_broadcast_i32x4
        auto table = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data)));
        auto nibble_mask = _mm512_set1_epi8(0x0F);

        auto index0 = _mm512_and_si512(decay(v), nibble_mask);
        auto index1 = _mm512_and_si512(_mm512_srli_epi16(decay(v), 0x4), nibble_mask);

        auto partial_sum0 = _mm512_shuffle_epi8(table, index0);
        auto partial_sum1 = _mm512_shuffle_epi8(table, index1);

        auto byte_sums = _mm512_add_epi8(partial_sum0, partial_sum1);

        auto short_sums = _mm512_add_epi16(byte_sums, _mm512_slli_epi16(byte_sums, 8));
        auto ret = _mm512_srli_epi16(short_sums, 8);

        return vec32x16u{ret};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u countl_zero(vec32x16u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512CD)
        auto zero = _mm512_setzero_si512();
        auto widened_lo = _mm512_unpacklo_epi16(decay(v), zero);
        auto widened_hi = _mm512_unpackhi_epi16(decay(v), zero);
        auto lzcnt_lo = _mm512_lzcnt_epi32(widened_lo);
        auto lzcnt_hi = _mm512_lzcnt_epi32(widened_hi);
        auto results = _mm512_packus_epi32(lzcnt_lo, lzcnt_hi);
        return vec32x16u{results} - vec32x16u{16};

        //TODO: Explore conversion to float-16 as possible alternative

        #elif defined(AVEL_SSE2)
        auto zero = _mm512_setzero_si512();

        auto v_lo = _mm512_unpacklo_epi16(decay(v), zero);
        auto v_hi = _mm512_unpackhi_epi16(decay(v), zero);

        auto floats_lo = _mm512_cvtepi32_ps(v_lo);
        auto floats_hi = _mm512_cvtepi32_ps(v_hi);

        auto exponents_lo = _mm512_srli_epi32(_mm512_castps_si512(floats_lo), 23);
        auto exponents_hi = _mm512_srli_epi32(_mm512_castps_si512(floats_hi), 23);

        auto exponents = _mm512_packs_epi32(exponents_lo, exponents_hi);
        auto partial = _mm512_sub_epi16(_mm512_set1_epi16(142), exponents);
        auto ret = _mm512_min_epi16(_mm512_set1_epi16(16), partial);

        return vec32x16u{ret};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u countl_one(vec32x16u v) {
        return countl_zero(~v);
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u countr_zero(vec32x16u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG)
        auto neg_one = _mm512_set1_epi16(-1);
        auto tz_mask = _mm512_andnot_si512(decay(v), _mm512_add_epi16(decay(v), neg_one));
        return vec32x16u{_mm512_popcnt_epi16(tz_mask)};

        #elif defined(AVEL_AVX512BW)
        // Conversion to float implementation
        auto zero = _mm512_setzero_si512();
        auto is_zero = _mm512_cmpeq_epi16_mask(zero, decay(v));
        auto offset = _mm512_mask_blend_epi16(is_zero, _mm512_set1_epi16(127), _mm512_set1_epi16(-16));

        v = _mm512_and_si512(decay(v), _mm512_sub_epi16(zero, decay(v)));

        auto v_lo = _mm512_unpacklo_epi16(decay(v), zero);
        auto v_hi = _mm512_unpackhi_epi16(decay(v), zero);

        auto floats_lo = _mm512_cvtepi32_ps(v_lo);
        auto floats_hi = _mm512_cvtepi32_ps(v_hi);

        auto exponents_lo = _mm512_srli_epi32(_mm512_castps_si512(floats_lo), 23);
        auto exponents_hi = _mm512_srli_epi32(_mm512_castps_si512(floats_hi), 23);

        auto exponents = _mm512_packs_epi32(exponents_lo, exponents_hi);
        auto ret = _mm512_sub_epi16(exponents, offset);

        return vec32x16u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u countr_one(vec32x16u v) {
        return countr_zero(~v);
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u bit_width(vec32x16u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512CD)
        auto zero = _mm512_setzero_si512();
        auto widened_lo = _mm512_unpacklo_epi16(decay(v), zero);
        auto widened_hi = _mm512_unpackhi_epi16(decay(v), zero);
        auto counts32_lo = _mm512_lzcnt_epi32(widened_lo);
        auto counts32_hi = _mm512_lzcnt_epi32(widened_hi);
        auto counts16 = _mm512_packus_epi32(counts32_lo, counts32_hi);

        return vec32x16u{32} - vec32x16u{counts16};

        #elif defined(AVEL_AVX512BW)
        auto zero = _mm512_setzero_si512();

        auto v_lo = _mm512_unpacklo_epi16(decay(v), zero);
        auto v_hi = _mm512_unpackhi_epi16(decay(v), zero);

        auto floats_lo = _mm512_cvtepi32_ps(v_lo);
        auto floats_hi = _mm512_cvtepi32_ps(v_hi);

        auto exponents_lo = _mm512_srli_epi32(_mm512_castps_si512(floats_lo), 23);
        auto exponents_hi = _mm512_srli_epi32(_mm512_castps_si512(floats_hi), 23);

        auto exponents = _mm512_packs_epi32(exponents_lo, exponents_hi);
        auto ret = _mm512_subs_epu16(exponents, _mm512_set1_epi16(126));

        return vec32x16u{ret};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u bit_floor(vec32x16u v) {
        #if defined(AVEL_AVX512BW)
        v |= (v >> 1);
        v |= (v >> 2);
        v |= (v >> 4);
        v |= (v >> 8);
        v -= (v >> 1);

        return v;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x16u bit_ceil(vec32x16u v) {
        #if defined(AVEL_AVX512BW)
        auto zero_mask = (v == vec32x16u{0x00});

        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        ++v;

        return v - broadcast_mask(zero_mask);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask32x16u has_single_bit(vec32x16u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG)
        return popcount(v) == vec32x16u{1};

        #elif defined(AVEL_AVX512BW)
        return !mask32x16u{v & (v - vec32x16u{1})} && mask32x16u{v};

        #endif
    }

}

#endif //AVEL_VEC32X16_HPP
