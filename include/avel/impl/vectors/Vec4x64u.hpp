#ifndef AVEL_VEC4X64U_HPP
#define AVEL_VEC4X64U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x64u = Vector<std::uint64_t, 4>;
    using arr4x64u = std::array<std::uint64_t, 4>;
    using mask4x64u = Vector_mask<std::uint64_t, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec4x64u> div(vec4x64u numerator, vec4x64u denominator);
    vec4x64u broadcast_mask(mask4x64u m);
    vec4x64u blend(mask4x64u m, vec4x64u a, vec4x64u b);
    vec4x64u countl_one(vec4x64u x);





    template<>
    class Vector_mask<std::uint64_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

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
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> m):
            Vector_mask(convert<Vector_mask>(m)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive p):
        #if defined(AVEL_AVX512VL)
            content(p & 0x0F) {}
        #elif defined(AVEL_AVX2)
            content(p) {}
        #endif

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL)
            content(b ? 0x0F : 0) {}
        #elif defined(AVEL_AVX2)
            content(b ? _mm256_set1_epi64x(-1) : _mm256_setzero_si256()) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr4xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
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
            #if defined(AVEL_AVX512VL)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(lhs.content) == _mm256_movemask_epi8(rhs.content);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
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
            #if defined(AVEL_AVX512VL)
            content &= rhs.content;

            #elif defined(AVEL_AVX2)
            content = _mm256_and_si256(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content |= rhs.content;

            #elif defined(AVEL_AVX2)
            content = _mm256_or_si256(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
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
            #if defined(AVEL_AVX512VL)
            return Vector_mask{primitive(content ^ 0xf)};

            #elif defined(AVEL_AVX2)
            auto ones = _mm256_set1_epi64x(-1);
            return Vector_mask{_mm256_andnot_si256(content, ones)};

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
    AVEL_FINL std::uint32_t count(mask4x64u m) {
        #if defined(AVEL_AVX512VL)
        return popcount(decay(m));

        #elif defined(AVEL_AVX2)
        return popcount(_mm256_movemask_epi8(decay(m))) / sizeof(std::uint64_t);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x64u m) {
        #if defined(AVEL_AVX512VL)
        return !_kortestz_mask16_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX2)
        return !_mm256_testz_si256(decay(m), decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x64u m) {
        #if defined(AVEL_AVX512VL)
        return decay(m) == 0x0F;

        #elif defined(AVEL_AVX2)
        return 0xFFFFFFFF == _mm256_movemask_epi8(decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x64u m) {
        #if defined(AVEL_AVX512VL)
        return _kortestz_mask16_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX2)
        return _mm256_testz_si256(decay(m), decay(m));
        #endif
    }





    template<>
    class Vector<std::uint64_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint64_t;

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
        #if defined(AVEL_AVX512VL)
            content(_mm256_maskz_set1_epi64(decay(m), 0x1)) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_sub_epi64(_mm256_setzero_si256(), decay(m))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            content(_mm256_set1_epi64x(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr4x64u& arr):
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
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpeq_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpeq_epi64(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpneq_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpeq_epi64(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmplt_epu64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            auto addition_mask = _mm256_set1_epi64x(0x8000000000000000ull);
            auto lhs_tmp = _mm256_xor_si256(decay(lhs), addition_mask);
            auto rhs_tmp = _mm256_xor_si256(decay(rhs), addition_mask);

            return mask{_mm256_cmpgt_epi64(rhs_tmp, lhs_tmp)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmple_epu64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE2)
            return !mask{lhs > rhs};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpgt_epu64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return rhs < lhs;

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpge_epu64_mask(decay(lhs), decay(rhs))};

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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
            content = _mm256_mullo_epi64(content, rhs.content);

            #elif defined(AVEL_AVX2)
            //TODO: Consider alternative approach based on emulation using
            // 32-bit multiplication
            std::uint64_t c0 = _mm256_extract_epi64(content, 0) * _mm256_extract_epi64(decay(rhs), 0);
            std::uint64_t c1 = _mm256_extract_epi64(content, 1) * _mm256_extract_epi64(decay(rhs), 1);
            std::uint64_t c2 = _mm256_extract_epi64(content, 2) * _mm256_extract_epi64(decay(rhs), 2);
            std::uint64_t c3 = _mm256_extract_epi64(content, 3) * _mm256_extract_epi64(decay(rhs), 3);

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
            content = _mm256_sll_epi64(content, _mm_cvtsi64x_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX2)
            content = _mm256_srl_epi64(content, _mm_cvtsi64x_si128(rhs));
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
            #if defined(AVEL_AVX2)
            content = _mm256_srlv_epi64(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector{_mm256_ternarylogic_epi64(content, content, content, 0x1)};

            #elif defined(AVEL_AVX2)
            auto ones = _mm256_set1_epi64x(-1);
            return Vector{_mm256_andnot_si256(content, ones)};
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
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_test_epi64_mask(content, content)};

            #else
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        4 * sizeof(std::uint64_t) == sizeof(vec4x64u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint64_t extract(vec4x64u v) {
        static_assert(N < 4, "Specified index does not exist");
        typename std::enable_if<N < 4, int>::type dummy_variable = 0;

        return _mm256_extract_epi64(decay(v), N);
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x64u insert(vec4x64u v, std::uint64_t x) {
        static_assert(N < 4, "Specified index does not exist");
        typename std::enable_if<N < 4, int>::type dummy_variable = 0;

        return vec4x64u{_mm256_insert_epi64(decay(v), x, N)};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_left(vec4x64u v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return vec4x64u{_mm256_slli_epi64(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_left<0>(vec4x64u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_left<1>(vec4x64u v) {
        return v + v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_left<64>(vec4x64u v) {
        (void)v;
        return vec4x64u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_right(vec4x64u v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX2)
        return vec4x64u{_mm256_srli_epi64(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_right<0>(vec4x64u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_right<64>(vec4x64u v) {
        (void)v;
        return vec4x64u{0x00};
    }




    template<std::uint32_t S, typename std::enable_if<S < 64, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x64u rotl(vec4x64u v) {
        #if defined(AVEL_AVX512VL)
        return vec4x64u{_mm256_rol_epi64(decay(v), S)};

        #elif defined(AVEL_AVX2)
        return bit_shift_left<S>(v) | bit_shift_right<64 - S>(v);

        #endif
    }

    template<>
    AVEL_FINL vec4x64u rotl<0>(vec4x64u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<64 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x64u rotl(vec4x64u v) {
        return rotl<S % 64>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u rotl(vec4x64u v, long long s) {
        #if defined(AVEL_AVX512VL)
        return vec4x64u{_mm256_rolv_epi64(decay(v), _mm256_set1_epi64x(s))};

        #elif defined(AVEL_AVX2)
        s &= 0x3F;
        return (v << s) | (v >> (64 - s));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u rotl(vec4x64u v, vec4x64u s) {
        #if defined(AVEL_AVX512VL)
        return vec4x64u{_mm256_rolv_epi64(decay(v), decay(s))};

        #elif defined(AVEL_AVX2)
        s &= vec4x64u{0x3F};
        return (v << s) | (v >> (vec4x64u{64} - s));
        #endif
    }



    template<std::uint32_t S, typename std::enable_if<S < 64, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x64u rotr(vec4x64u v) {
        #if defined(AVEL_AVX512VL)
        return vec4x64u{_mm256_ror_epi64(decay(v), S)};

        #elif defined(AVEL_AVX2)
        return bit_shift_right<S>(v) | bit_shift_left<64 - S>(v);

        #endif
    }

    template<>
    AVEL_FINL vec4x64u rotr<0>(vec4x64u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<64 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x64u rotr(vec4x64u v) {
        return rotr<S % 64>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u rotr(vec4x64u v, long long s) {
        #if defined(AVEL_AVX512VL)
        return vec4x64u{_mm256_rorv_epi64(decay(v), _mm256_set1_epi64x(s))};

        #elif defined(AVEL_AVX2)
        s &= 0x3f;
        return (v >> s) | (v << (64 - s));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u rotr(vec4x64u v, vec4x64u s) {
        #if defined(AVEL_AVX512VL)
        return vec4x64u{_mm256_rorv_epi64(decay(v), decay(s))};

        #elif defined(AVEL_AVX2)
        s &= vec4x64u{0x3F};
        return (v >> s) | (v << (vec4x64u{64} - s));
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec4x64u v) {
        #if defined(AVEL_AVX512VL)
        return count(mask4x64u{v});

        #elif defined(AVEL_AVX2)
        auto compared = _mm256_cmpeq_epi64(decay(v), _mm256_setzero_si256());
        return 4 - popcount(_mm256_movemask_epi8(compared)) / sizeof(std::uint64_t);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec4x64u v) {
        //TODO: Optimize with newer instruction sets
        #if defined(AVEL_AVX2)
        auto compared = _mm256_cmpeq_epi64(decay(v), _mm256_setzero_si256());
        return 0xFFFFFFFF != _mm256_movemask_epi8(compared);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec4x64u v) {
        #if defined(AVEL_AVX2)
        auto compared = _mm256_cmpeq_epi64(decay(v), _mm256_setzero_si256());
        return 0x00 == _mm256_movemask_epi8(compared);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec4x64u v) {
        #if defined(AVEL_AVX2)
        return _mm256_testz_si256(decay(v), decay(v));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u broadcast_mask(mask4x64u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec4x64u{_mm256_movm_epi64(decay(m))};

        #elif defined(AVEL_AVX512VL)
        return vec4x64u{_mm256_maskz_set1_epi64(decay(m), -1)};

        #elif defined(AVEL_AVX2)
        return vec4x64u{decay(m)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u keep(mask4x64u m, vec4x64u v) {
        #if defined(AVEL_AVX512VL)
        return vec4x64u{_mm256_maskz_mov_epi64(decay(m), decay(v))};

        #elif defined(AVEL_AVX2)
        return broadcast_mask(m) & v;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u clear(mask4x64u m, vec4x64u v) {
        #if defined(AVEL_AVX512VL)
        return vec4x64u{_mm256_maskz_mov_epi64(decay(!m), decay(v))};

        #elif defined(AVEL_AVX2)
        return vec4x64u{_mm256_andnot_si256(decay(m), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u blend(mask4x64u m, vec4x64u a, vec4x64u b) {
        #if defined(AVEL_AVX512VL)
        return vec4x64u{_mm256_mask_blend_epi64(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_AVX2)
        return vec4x64u{_mm256_blendv_epi8(decay(b), decay(a), decay(m))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u byteswap(vec4x64u v) {
        #if defined(AVEL_AVX2)
        alignas(16) static constexpr std::uint8_t index_data[32] {
             7,  6,  5,  4,
             3,  2,  1,  0,
            15, 14, 13, 12,
            11, 10,  9,  8,
             7,  6,  5,  4,
             3,  2,  1,  0,
            15, 14, 13, 12,
            11, 10,  9,  8
        };

        auto indices = _mm256_load_si256((const __m256i*)index_data);
        return vec4x64u{_mm256_shuffle_epi8(decay(v), indices)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u max(vec4x64u a, vec4x64u b) {
        #if defined(AVEL_AVX2)
        return vec4x64u{_mm256_max_epu64(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u min(vec4x64u a, vec4x64u b) {
        #if defined(AVEL_AVX2)
        return vec4x64u{_mm256_min_epu64(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x64u, 2> minmax(vec4x64u a, vec4x64u b) {
        #if defined(AVEL_AVX2)
        return {
            vec4x64u{_mm256_min_epu64(decay(a), decay(b))},
            vec4x64u{_mm256_max_epu64(decay(a), decay(b))},
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u clamp(vec4x64u x, vec4x64u lo, vec4x64u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u average(vec4x64u a, vec4x64u b) {
        #if defined(AVEL_AVX2)
        return ((a ^ b) >> 1) + (a & b);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u midpoint(vec4x64u a, vec4x64u b) {
        vec4x64u t0 = a & b & vec4x64u{0x1};
        vec4x64u t1 = (a | b) & vec4x64u{0x1} & broadcast_mask(a > b);
        vec4x64u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u load<vec4x64u>(const std::uint64_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        return vec4x64u{_mm256_maskz_loadu_epi64(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n > 2) {
            auto lo = load<vec2x64u>(ptr + 0, n - 0);
            auto hi = load<vec2x64u>(ptr + 2, n - 2);
            return vec4x64u{_mm256_set_m128i(decay(hi), decay(lo))};
        } else {
            auto lo = load<vec2x64u>(ptr, n);
            return vec4x64u{_mm256_zextsi128_si256(decay(lo))};
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u load<vec4x64u, vec4x64u::width>(const std::uint64_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec4x64u{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u aligned_load<vec4x64u>(const std::uint64_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        return vec4x64u{_mm256_maskz_load_epi64(mask, ptr)};

        #elif defined(AVEL_AVX2)
        if (n > 2) {
            auto lo = aligned_load<vec2x64u>(ptr + 0, n - 0);
            auto hi = aligned_load<vec2x64u>(ptr + 2, n - 2);
            return vec4x64u{_mm256_set_m128i(decay(hi), decay(lo))};
        } else {
            auto lo = aligned_load<vec2x64u>(ptr, n);
            return vec4x64u{_mm256_zextsi128_si256(decay(lo))};
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u aligned_load<vec4x64u, vec4x64u::width>(const std::uint64_t* ptr) {
        #if defined(AVEL_AVX2)
        return vec4x64u{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
        #endif
    }



    AVEL_FINL void store(std::uint64_t* ptr, vec4x64u v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        _mm256_mask_storeu_epi64(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n > 2) {
            store(ptr + 0, vec2x64u{_mm256_castsi256_si128(decay(v))}, n - 0);
            store(ptr + 2, vec2x64u{_mm256_extracti128_si256(decay(v), 0x1)}, n - 2);
        } else {
            store(ptr, vec2x64u{_mm256_castsi256_si128(decay(v))}, n);
        }

        #endif
    }

    template<std::uint32_t N = vec4x64u::width>
    AVEL_FINL void store(std::uint64_t* ptr, vec4x64u v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec4x64u::width>(std::uint64_t* ptr, vec4x64u v) {
        #if defined(AVEL_AVX2)
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(std::uint64_t* ptr, vec4x64u v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 4) ? 0x0f : (1 << n) - 1;
        _mm256_mask_store_epi64(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n > 2) {
            aligned_store(ptr + 0, vec2x64u{_mm256_castsi256_si128(decay(v))}, n - 0);
            aligned_store(ptr + 2, vec2x64u{_mm256_extracti128_si256(decay(v), 0x1)}, n - 2);
        } else {
            aligned_store(ptr, vec2x64u{_mm256_castsi256_si128(decay(v))}, n);
        }

        #endif
    }

    template<std::uint32_t N = vec4x64u::width>
    AVEL_FINL void aligned_store(std::uint64_t* ptr, vec4x64u v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec4x64u::width>(std::uint64_t* ptr, vec4x64u v) {
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr4x64u to_array(vec4x64u x) {
        alignas(32) arr4x64u ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec4x64u> div(vec4x64u x, vec4x64u y) {
        auto n0 = extract<0>(x);
        auto n1 = extract<1>(x);
        auto n2 = extract<2>(x);
        auto n3 = extract<3>(x);

        auto d0 = extract<0>(y);
        auto d1 = extract<1>(y);
        auto d2 = extract<2>(y);
        auto d3 = extract<3>(y);

        vec4x64u quotient{};
        quotient = insert<0>(quotient, n0 / d0);
        quotient = insert<1>(quotient, n1 / d1);
        quotient = insert<2>(quotient, n2 / d2);
        quotient = insert<3>(quotient, n3 / d3);

        vec4x64u remainder{};
        remainder = insert<0>(remainder, n0 % d0);
        remainder = insert<1>(remainder, n1 % d1);
        remainder = insert<2>(remainder, n2 % d2);
        remainder = insert<3>(remainder, n3 % d3);

        return {quotient, remainder};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u popcount(vec4x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)
        return vec4x64u{_mm256_popcnt_epi64(decay(v))};

        #elif defined(AVEL_AVX2)
        //TODO: Consider alternative implementations
        auto c0 = popcount(extract<0>(v));
        auto c1 = popcount(extract<1>(v));
        auto c2 = popcount(extract<2>(v));
        auto c3 = popcount(extract<3>(v));

        return vec4x64u{_mm256_set_epi64x(c3, c2, c1, c0)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u countl_zero(vec4x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec4x64u{_mm256_lzcnt_epi64(decay(v))};

        #elif defined(AVEL_AVX2)
        auto c0 = countl_zero(extract<0>(v));
        auto c1 = countl_zero(extract<1>(v));
        auto c2 = countl_zero(extract<2>(v));
        auto c3 = countl_zero(extract<3>(v));

        return vec4x64u{_mm256_set_epi64x(c3, c2, c1, c0)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u countl_one(vec4x64u v) {
        return countl_zero(~v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u countr_zero(vec4x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)
        auto undef = _mm256_undefined_si256();
        auto neg_one = _mm256_cmpeq_epi64(undef, undef);
        auto tz_mask = _mm256_andnot_si256(decay(v), _mm256_add_epi64(decay(v), neg_one));
        return vec4x64u{_mm256_popcnt_epi64(tz_mask)};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto zero_mask = (v == vec4x64u{0x00});
        auto y = (v & (vec4x64u{0x00} - v));
        auto z = vec4x64u{63} - countl_zero(y);
        return blend(zero_mask, vec4x64u{64}, z);

        #elif defined(AVEL_AVX2)
        auto c0 = countr_zero(extract<0>(v));
        auto c1 = countr_zero(extract<1>(v));
        auto c2 = countr_zero(extract<2>(v));
        auto c3 = countr_zero(extract<3>(v));

        return vec4x64u{_mm256_set_epi64x(c3, c2, c1, c0)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u countr_one(vec4x64u v) {
        return countr_zero(~v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u bit_width(vec4x64u v) {
        return vec4x64u{64} - countl_zero(v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u bit_floor(vec4x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        vec4x64u leading_zeros = countl_zero(v);
        mask4x64u zero_mask = (leading_zeros != vec4x64u{64});
        return (vec4x64u{zero_mask} << (vec4x64u{63} - leading_zeros));

        #elif defined(AVEL_AVX2)
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
    AVEL_FINL vec4x64u bit_ceil(vec4x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto sh = (vec4x64u{64} - countl_zero(v - vec4x64u{1}));
        auto result = vec4x64u{1} << sh;
        return result - broadcast_mask(v == vec4x64u{0x00});

        #elif defined(AVEL_AVX2)
        auto zero_mask = (v == vec4x64u{0});

        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v |= v >> 32;
        ++v;

        return v - broadcast_mask(zero_mask);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64u has_single_bit(vec4x64u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)
        return mask4x64u{popcount(v) == vec4x64u{1}};

        #elif defined(AVEL_SSE2)
        return mask4x64u{v} & !mask4x64u{v & (v - vec4x64u{1})};

        #endif
    }

}

#endif //AVEL_VEC4X64U_HPP
