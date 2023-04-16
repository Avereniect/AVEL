#ifndef AVEL_VEC16X16U_HPP
#define AVEL_VEC16X16U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x16u = Vector<std::uint16_t, 16>;
    using arr16x16u = std::array<std::uint16_t, 16>;
    using mask16x16u = Vector_mask<std::uint16_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x16u> div(vec16x16u numerator, vec16x16u denominator);
    vec16x16u broadcast_mask(mask16x16u m);
    vec16x16u blend(mask16x16u m, vec16x16u a, vec16x16u b);
    vec16x16u countl_one(vec16x16u x);





    template<>
    class Vector_mask<std::uint16_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
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
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content(b ? 0xFFFF : 0x0000) {}
        #elif defined(AVEL_AVX2)
            content(b ? _mm256_set1_epi16(-1) : _mm256_setzero_si256()) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr16xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = primitive(_mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX2)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            auto widened = _mm256_cvtepi8_epi16(array_data);
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = b ? 0xFFFF : 0x0000;

            #elif defined(AVEL_AVX2)
            content = b ? _mm256_set1_epi16(-1) : _mm256_setzero_si256();

            #endif
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
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(decay(lhs)) == _mm256_movemask_epi8(decay(rhs));

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(decay(lhs)) != _mm256_movemask_epi8(decay(rhs));

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content &= rhs.content;

            #elif defined(AVEL_AVX2)
            content = _mm256_and_si256(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content |= rhs.content;

            #elif defined(AVEL_AVX2)
            content = _mm256_or_si256(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector_mask{static_cast<primitive>(~content)};

            #elif defined(AVEL_AVX512VL)
            return Vector_mask{_mm256_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_AVX2)
            auto undef = _mm256_undefined_si256();
            auto full = _mm256_cmpeq_epi8(undef, undef);
            return Vector_mask{_mm256_andnot_si256(content, full)};
            #endif
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
    AVEL_FINL std::uint32_t count(mask16x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(_mm512_mask2int(decay(m)));

        #elif defined(AVEL_AVX2)
        return popcount(_mm256_movemask_epi8(decay(m))) / sizeof(std::uint16_t);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask16x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return !_kortestz_mask16_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX2)
        return !_mm256_testz_si256(decay(m), decay(m));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask16x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _kortestc_mask16_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX2)
        return 0xFFFFFFFF == _mm256_movemask_epi8(decay(m));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _kortestz_mask32_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX2)
        return _mm256_testz_si256(decay(m), decay(m));

        #endif
    }





    template<>
    class Vector<std::uint16_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint16_t;

        using primitive = __m256i;

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
            content(_mm256_maskz_set1_epi16(decay(m), 1)) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_sub_epi16(_mm256_setzero_si256(), decay(m))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm256_set1_epi16(x)) {}

        AVEL_FINL explicit Vector(const arr16x16u& arr):
            content(_mm256_loadu_si256(reinterpret_cast<const primitive*>(arr.data()))) {}

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
            return mask{_mm256_cmpeq_epi16_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpeq_epi16(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmpneq_epi16_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpeq_epi16(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmplt_epu16_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            Vector addition_mask{0x8000};
            return mask{_mm256_cmpgt_epi16(decay(rhs ^ addition_mask), decay(lhs ^ addition_mask))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmple_epu16_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            Vector addition_mask{0x8000};
            return !mask{_mm256_cmpgt_epi16(decay(lhs ^ addition_mask), decay(rhs ^ addition_mask))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmpgt_epu16_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            Vector addition_mask{0x8000};
            return mask{_mm256_cmpgt_epi16(decay(lhs ^ addition_mask), decay(rhs ^ addition_mask))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmpge_epu16_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            Vector addition_mask{0x8000};
            return !mask{_mm256_cmpgt_epi16(decay(rhs ^ addition_mask), decay(lhs ^ addition_mask))};

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
            content = _mm256_add_epi16(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm256_sub_epi16(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm256_mullo_epi16(content, decay(rhs));
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
            content = _mm256_sll_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_srl_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm256_sllv_epi16(content, decay(rhs));

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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm256_srlv_epi16(content, decay(rhs));

            #elif defined(AVEL_AVX2)
            auto even_mask = _mm256_set1_epi32(0x0000FFFF);

            auto rhs_even = _mm256_and_si256(even_mask, decay(rhs));
            auto lhs_even = _mm256_and_si256(even_mask, content);
            auto result_even = _mm256_srlv_epi32(lhs_even, rhs_even);

            auto rhs_odd = _mm256_srli_epi32(decay(rhs), 16);
            auto result_odd32 = _mm256_srlv_epi32(content, rhs_odd);
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector{_mm256_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_AVX2)
            auto undef = _mm256_undefined_si256();
            return Vector{_mm256_andnot_si256(content, _mm256_cmpeq_epi32(undef, undef))};

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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_test_epi16_mask(content, content)};

            #elif defined(AVEL_AVX2)
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        16 * sizeof(std::uint16_t) == sizeof(vec16x16u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint16_t extract(vec16x16u v) {
        static_assert(N < vec16x16u::width, "Specified index does not exist");
        typename std::enable_if<N < vec16x16u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return _mm256_extract_epi16(decay(v), N);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec16x16u insert(vec16x16u v, std::uint16_t x) {
        static_assert(N < vec16x16u::width, "Specified index does not exist");
        typename std::enable_if<N < vec16x16u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return vec16x16u{_mm256_insert_epi16(decay(v), x, N)};
        #endif
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x16u bit_shift_left(vec16x16u v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return vec16x16u{_mm256_slli_epi16(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16u bit_shift_left<0>(vec16x16u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16u bit_shift_left<1>(vec16x16u v) {
        return v + v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16u bit_shift_left<16>(vec16x16u v) {
        (void)v;
        return vec16x16u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec16x16u bit_shift_right(vec16x16u v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        return vec16x16u{_mm256_srli_epi16(decay(v), S)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16u bit_shift_right<0>(vec16x16u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16u bit_shift_right<16>(vec16x16u v) {
        (void)v;
        return vec16x16u{0x00};
    }




    template<std::uint32_t S, typename std::enable_if<S < 16, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec16x16u rotl(vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        return vec16x16u{_mm256_shldi_epi16(decay(v), decay(v), S)};

        #elif defined(AVEL_AVX2)
        auto left_shifted  = _mm256_slli_epi16(decay(v), S);
        auto right_shifted = _mm256_srli_epi16(decay(v), 16 - S);

        auto ret = _mm256_or_si256(left_shifted, right_shifted);
        return vec16x16u{ret};
        #endif
    }

    template<>
    AVEL_FINL vec16x16u rotl<0>(vec16x16u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<16 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec16x16u rotl(vec16x16u v) {
        return rotl<S % 16>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u rotl(vec16x16u v, long long s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        s &= 15;
        return vec16x16u{_mm256_shldv_epi16(decay(v), decay(v), _mm256_set1_epi16(s))};

        #elif defined(AVEL_AVX2)
        s &= 15;
        return (v << s) | (v >> (16 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u rotl(vec16x16u v, vec16x16u s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        s &= vec16x16u{15};
        return vec16x16u{_mm256_shldv_epi16(decay(v), decay(v), decay(s))};

        #elif defined(AVEL_AVX2)
        s &= vec16x16u{15};
        return (v << s) | (v >> (vec16x16u{16} - s));
        #endif
    }



    template<std::uint32_t S, typename std::enable_if<S < 16, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec16x16u rotr(vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        return vec16x16u{_mm256_shrdi_epi16(decay(v), decay(v), S)};

        #elif defined(AVEL_AVX2)
        auto left_shifted  = _mm256_slli_epi16(decay(v), 16 - S);
        auto right_shifted = _mm256_srli_epi16(decay(v), S);

        auto ret = _mm256_or_si256(left_shifted, right_shifted);
        return vec16x16u{ret};
        #endif
    }

    template<>
    AVEL_FINL vec16x16u rotr<0>(vec16x16u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<16 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec16x16u rotr(vec16x16u v) {
        return rotr<S % 16>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u rotr(vec16x16u v, long long s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        s &= 15;
        return vec16x16u{_mm256_shrdv_epi16(decay(v), decay(v), _mm256_set1_epi16(s))};

        #elif defined(AVEL_AVX2)
        s &= 15;
        return (v >> s) | (v << (16 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u rotr(vec16x16u v, vec16x16u s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        s &= vec16x16u{15};
        return vec16x16u{_mm256_shrdv_epi16(decay(v), decay(v), decay(s))};

        #elif defined(AVEL_AVX2)
        s &= vec16x16u{15};
        return (v >> s) | (v << (vec16x16u{16} - s));
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return count(mask16x16u{v});

        #elif defined(AVEL_AVX2)
        auto compared = _mm256_cmpeq_epi16(decay(v), _mm256_setzero_si256());
        return vec16x16u::width - (popcount(_mm256_movemask_epi8(compared)) / sizeof(vec16x16u::scalar));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec16x16u v) {
        #if defined(AVEL_AVX2)
        return !_mm256_testz_si256(decay(v), decay(v));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec16x16u v) {
        #if defined(AVEL_AVX2)
        auto compared = _mm256_cmpeq_epi16(decay(v), _mm256_setzero_si256());
        return 0x00 == _mm256_movemask_epi8(compared);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec16x16u v) {
        #if defined(AVEL_AVX2)
        return _mm256_testz_si256(decay(v), decay(v));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u broadcast_mask(mask16x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec16x16u{_mm256_movm_epi16(decay(m))};

        #elif defined(AVEL_AVX2)
        return vec16x16u{decay(m)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u keep(mask16x16u m, vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec16x16u{_mm256_maskz_mov_epi16(decay(m), decay(v))};

        #elif defined(AVEL_AVX2)
        return broadcast_mask(m) & v;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u clear(mask16x16u m, vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec16x16u{_mm256_maskz_mov_epi16(decay(!m), decay(v))};

        #elif defined(AVEL_AVX2)
        return vec16x16u{_mm256_andnot_si256(decay(m), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u blend(mask16x16u m, vec16x16u a, vec16x16u b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec16x16u{_mm256_mask_blend_epi16(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_AVX2)
        return vec16x16u{_mm256_blendv_epi8(decay(b), decay(a), decay(m))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u byteswap(vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        return vec16x16u{_mm256_shldi_epi16(decay(v), decay(v), 0x8)};

        #elif defined(AVEL_AVX2)
        auto lo = _mm256_srli_epi16(decay(v), 8);
        auto hi = _mm256_slli_epi16(decay(v), 8);
        return vec16x16u{_mm256_or_si256(lo, hi)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u max(vec16x16u a, vec16x16u b) {
        return vec16x16u{_mm256_max_epu16(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u min(vec16x16u a, vec16x16u b) {
        return vec16x16u{_mm256_min_epu16(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec16x16u, 2> minmax(vec16x16u a, vec16x16u b) {
        return {
            vec16x16u{_mm256_min_epu16(decay(a), decay(b))},
            vec16x16u{_mm256_max_epu16(decay(a), decay(b))}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u clamp(vec16x16u x, vec16x16u lo, vec16x16u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u average(vec16x16u a, vec16x16u b) {
        return vec16x16u{_mm256_avg_epu16(decay(a), decay(b))} - ((a ^ b) & vec16x16u{0x1});
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u midpoint(vec16x16u a, vec16x16u b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t1 = _mm256_avg_epu16(decay(a), decay(b));
        auto t5 = _mm256_and_si256(_mm256_ternarylogic_epi32(decay(a), decay(b), decay(broadcast_mask(b < a)), 0x14), _mm256_set1_epi16(0x1));
        auto t6 = _mm256_sub_epi16(t1, t5);
        return vec16x16u{t6};

        #elif defined(AVEL_AVX2)
        auto t1 = _mm256_avg_epu16(decay(a), decay(b));
        auto t3 = _mm256_andnot_si256(decay(broadcast_mask(b <= a)), _mm256_xor_si256(decay(a), decay(b)));
        auto t5 = _mm256_and_si256(t3, _mm256_set1_epi16(0x1));
        auto t6 = _mm256_sub_epi16(t1, t5);
        return vec16x16u{t6};

        #endif
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16u load<vec16x16u>(const std::uint16_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        return vec16x16u{_mm256_maskz_loadu_epi16(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n > 8) {
            auto lo = load<vec8x16u>(ptr + 0, n - 0);
            auto hi = load<vec8x16u>(ptr + 8, n - 8);
            return vec16x16u{_mm256_set_m128i(decay(hi), decay(lo))};
        } else {
            auto lo = load<vec8x16u>(ptr, n);
            return vec16x16u{_mm256_zextsi128_si256(decay(lo))};
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16u load<vec16x16u, vec16x16u::width>(const std::uint16_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec16x16u{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16u aligned_load<vec16x16u>(const std::uint16_t* ptr, std::uint32_t n) {
        return load<vec16x16u>(ptr, n);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x16u aligned_load<vec16x16u, vec16x16u::width>(const std::uint16_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec16x16u{_mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }



    AVEL_FINL void store(std::uint16_t* ptr, vec16x16u v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm256_mask_storeu_epi16(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n > 8) {
            store(ptr + 0, vec8x16u{_mm256_castsi256_si128(decay(v))}, n - 0);
            store(ptr + 8, vec8x16u{_mm256_extracti128_si256(decay(v), 0x1)}, n - 8);
        } else {
            store(ptr, vec8x16u{_mm256_castsi256_si128(decay(v))}, n);
        }

        #endif
    }

    template<std::uint32_t N = vec16x16u::width>
    AVEL_FINL void store(std::uint16_t* ptr, vec16x16u v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec16x16u::width>(std::uint16_t* ptr, vec16x16u v) {
        #if defined(AVEL_AVX2)
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec16x16u v, std::uint32_t n) {
        store(ptr, v, n);
    }

    template<std::uint32_t N = vec16x16u::width>
    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec16x16u v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec16x16u::width>(std::uint16_t* ptr, vec16x16u v) {
        #if defined(AVEL_AVX2)
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr16x16u to_array(vec16x16u x) {
        alignas(32) arr16x16u ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec16x16u> div(vec16x16u x, vec16x16u y) {
        //TODO: Optimize
        vec16x16u quotient{0x00};
        std::int32_t i = 16;
        for (; (i-- > 0) && any(x >= y);) {
            mask16x16u b = ((x >> i) >= y);
            x -= (broadcast_mask(b) & (y << i));
            quotient += quotient;
            quotient -= broadcast_mask(b);
        }

        quotient <<= (i + 1);
        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u popcount(vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        return vec16x16u{_mm256_popcnt_epi16(decay(v))};

        #elif defined(AVEL_AVX2)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        auto table = _mm_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data)));
        auto nibble_mask = _mm256_set1_epi8(0x0F);

        auto index0 = _mm256_and_si256(decay(v), nibble_mask);
        auto index1 = _mm256_and_si256(_mm256_srli_epi16(decay(v), 0x4), nibble_mask);

        auto partial_sum0 = _mm256_shuffle_epi8(table, index0);
        auto partial_sum1 = _mm256_shuffle_epi8(table, index1);

        auto byte_sums = _mm256_add_epi8(partial_sum0, partial_sum1);

        auto short_sums = _mm256_add_epi16(byte_sums, _mm256_slli_epi16(byte_sums, 8));
        auto ret = _mm256_srli_epi16(short_sums, 8);

        return vec16x16u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u countl_zero(vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512CD)
        auto widened = _mm512_cvtepu16_epi32(decay(v));
        auto lzcnt = _mm512_lzcnt_epi32(widened);
        auto results = _mm512_cvtepi16_epi8(lzcnt);
        return vec16x16u{results} - vec16x16u{16};

        #elif defined(AVEL_AVX2)
        auto zero = _mm256_setzero_si256();

        auto v_lo = _mm256_unpacklo_epi16(decay(v), zero);
        auto v_hi = _mm256_unpackhi_epi16(decay(v), zero);

        auto floats_lo = _mm256_cvtepi32_ps(v_lo);
        auto floats_hi = _mm256_cvtepi32_ps(v_hi);

        auto exponents_lo = _mm256_srli_epi32(_mm256_castps_si256(floats_lo), 23);
        auto exponents_hi = _mm256_srli_epi32(_mm256_castps_si256(floats_hi), 23);

        auto exponents = _mm256_packs_epi32(exponents_lo, exponents_hi);
        auto partial = _mm256_sub_epi16(_mm256_set1_epi16(142), exponents);
        auto ret = _mm256_min_epi16(_mm256_set1_epi16(16), partial);

        return vec16x16u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u countl_one(vec16x16u v) {
        return countl_zero(~v);
    };

    [[nodiscard]]
    AVEL_FINL vec16x16u countr_zero(vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        auto neg_one = _mm256_set1_epi16(-1);
        auto tz_mask = _mm256_andnot_si256(decay(v), _mm256_add_epi16(decay(v), neg_one));
        return vec16x16u{_mm256_popcnt_epi16(tz_mask)};

        #elif defined(AVEL_AVX2)
        // Conversion to float implementation with blend instruction
        auto zero = _mm256_setzero_si256();
        auto is_zero = _mm256_cmpeq_epi16(zero, decay(v));
        auto offset = _mm256_blendv_epi8(_mm256_set1_epi16(127), _mm256_set1_epi16(-16), is_zero);

        v = _mm256_and_si256(decay(v), _mm256_sub_epi16(zero, decay(v)));

        auto v_lo = _mm256_unpacklo_epi16(decay(v), zero);
        auto v_hi = _mm256_unpackhi_epi16(decay(v), zero);

        auto floats_lo = _mm256_cvtepi32_ps(v_lo);
        auto floats_hi = _mm256_cvtepi32_ps(v_hi);

        auto exponents_lo = _mm256_srli_epi32(_mm256_castps_si256(floats_lo), 23);
        auto exponents_hi = _mm256_srli_epi32(_mm256_castps_si256(floats_hi), 23);

        auto exponents = _mm256_packs_epi32(exponents_lo, exponents_hi);
        auto ret = _mm256_sub_epi16(exponents, offset);

        return vec16x16u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u countr_one(vec16x16u v) {
        return countr_zero(~v);
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u bit_width(vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto widened = _mm512_cvtepu16_epi32(decay(v));
        auto counts32 = _mm512_lzcnt_epi32(widened);
        auto counts16 = _mm512_cvtepi32_epi16(counts32);

        return vec16x16u{32} - vec16x16u{counts16};

        #elif defined(AVEL_AVX2)
        auto zero = _mm256_setzero_si256();

        auto v_lo = _mm256_unpacklo_epi16(decay(v), zero);
        auto v_hi = _mm256_unpackhi_epi16(decay(v), zero);

        auto floats_lo = _mm256_cvtepi32_ps(v_lo);
        auto floats_hi = _mm256_cvtepi32_ps(v_hi);

        auto exponents_lo = _mm256_srli_epi32(_mm256_castps_si256(floats_lo), 23);
        auto exponents_hi = _mm256_srli_epi32(_mm256_castps_si256(floats_hi), 23);

        auto exponents = _mm256_packs_epi32(exponents_lo, exponents_hi);
        auto ret = _mm256_subs_epu16(exponents, _mm256_set1_epi16(126));

        return vec16x16u{ret};

        /* Older implementation
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        alignas(16) static constexpr std::uint8_t table_increments_data[16] {
            0, 4, 4, 4,
            4, 4, 4, 4,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        auto nibble_mask = _mm256_set1_epi8(0x0F);

        auto nibble0 = _mm256_and_si256(nibble_mask, decay(v));
        auto nibble1 = _mm256_and_si256(nibble_mask, _mm256_srli_epi16(decay(v), 0x4));
        auto nibble2 = _mm256_and_si256(nibble_mask, _mm256_srli_epi16(decay(v), 0x8));
        auto nibble3 = _mm256_srli_epi16(decay(v), 0xC);

        auto table_incrementers = _mm256_load_si256(reinterpret_cast<const __m256i*>(table_increments_data));
        auto table0 = _mm256_load_si256(reinterpret_cast<const __m256i*>(table_data));
        auto table1 = _mm256_add_epi16(table0, table_incrementers);
        auto table2 = _mm256_add_epi16(table1, table_incrementers);
        auto table3 = _mm256_add_epi16(table2, table_incrementers);

        auto partial0 = _mm256_shuffle_epi8(table0, nibble0);
        auto partial1 = _mm256_shuffle_epi8(table1, nibble1);
        auto partial2 = _mm256_shuffle_epi8(table2, nibble2);
        auto partial3 = _mm256_shuffle_epi8(table3, nibble3);

        auto partial4 = _mm256_max_epu8(partial0, partial1);
        auto partial5 = _mm256_max_epu8(partial2, partial3);
        auto partial6 = _mm256_max_epu8(partial4, partial5);

        auto ret = _mm256_and_si256(partial6, _mm256_set1_epi16(0x00FF));

        return vec16x16u{ret};
        */

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u bit_floor(vec16x16u v) {
        #if defined(AVEL_AVX2)
        v |= (v >> 1);
        v |= (v >> 2);
        v |= (v >> 4);
        v |= (v >> 8);
        v -= (v >> 1);

        return v;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x16u bit_ceil(vec16x16u v) {
        #if defined(AVEL_AVX2)
        auto zero_mask = (v == vec16x16u{0x00});

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
    AVEL_FINL mask16x16u has_single_bit(vec16x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        return popcount(v) == vec16x16u{1};

        #elif defined(AVEL_AVX2)
        return !mask16x16u{v & (v - vec16x16u{1})} && mask16x16u{v};

        #endif
    }

}

#endif //AVEL_VEC16X16U_HPP
