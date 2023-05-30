#ifndef AVEL_VEC64X8U_HPP
#define AVEL_VEC64X8U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec64x8u = Vector<std::uint8_t, 64>;
    using arr64x8u = std::array<std::uint8_t, 64>;
    using mask64x8u = Vector_mask<std::uint8_t, 64>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec64x8u> div(vec64x8u x, vec64x8u y);
    vec64x8u set_bits(mask64x8u m);
    vec64x8u blend(vec64x8u a, vec64x8u b, mask64x8u m);
    vec64x8u countl_one(vec64x8u v);
    vec64x8u bit_width(vec64x8u v);





    template<>
    class Vector_mask<std::uint8_t, 64> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 64;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512BW)
        using primitive = __mmask64;
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
            content(b ? 0xFFFFFFFFFFFFFFFF : 0x0000000000000000) {}

        AVEL_FINL explicit Vector_mask(const arr64xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512BW)
            auto array_data = _mm512_loadu_si512(arr.data());
            content = _mm512_cmplt_epu8_mask(_mm512_setzero_si512(), array_data);
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
            *this = Vector_mask{p};
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
            auto tmp = _kxor_mask64(decay(lhs), decay(rhs));
            return _kortestz_mask64_u8(tmp, tmp);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512BW)
            auto tmp = _kxor_mask64(decay(lhs), decay(rhs));
            return !_kortestz_mask64_u8(tmp, tmp);
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
            return Vector_mask{_knot_mask64(content)};
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
    AVEL_FINL std::uint32_t count(mask64x8u m) {
        return popcount(std::uint64_t(decay(m)));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask64x8u m) {
        #if defined(AVEL_AVX512BW)
        return !_kortestz_mask64_u8(decay(m), decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask64x8u m) {
        #if defined(AVEL_AVX512BW)
        return _kortestc_mask64_u8(decay(m), decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask64x8u m) {
        #if defined(AVEL_AVX512BW)
        return _kortestz_mask64_u8(decay(m), decay(m));
        #endif
    }




    template<>
    class Vector<std::uint8_t, 64> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 64;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint8_t;

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
            content(_mm512_maskz_set1_epi8(decay(m), 1)) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX512BW)
            content(_mm512_set1_epi8(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr64x8u& arr):
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
            return mask{_mm512_cmpeq_epi8_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpneq_epi8_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmplt_epu8_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmple_epu8_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpgt_epu8_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpge_epu8_mask(decay(lhs), decay(rhs))};
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
            #if defined(AVEL_AVX512BW)
            content = _mm512_add_epi8(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_sub_epi8(content, decay(rhs));
            #endif
            return *this;
        }
        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            auto even_mask = _mm512_set1_epi16(0x00FF);

            auto products_even = _mm512_and_si512(even_mask, _mm512_mullo_epi16(content, decay(rhs)));
            auto products_odd  = _mm512_mullo_epi16(
                _mm512_srli_epi16(content, 8),
                _mm512_andnot_si512(even_mask, decay(rhs))
            );

            auto products = _mm512_or_si512(products_even, products_odd);
            content = products;

            //TODO: Explicitly use vpternlogd
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
            auto s = _mm_cvtsi32_si128(rhs);

            auto shifted = _mm512_sll_epi16(content, s);
            auto mask = _mm512_set1_epi8(std::uint8_t(0xFF << rhs));
            content = _mm512_and_si512(mask, shifted);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX512BW)
            auto s = _mm_cvtsi32_si128(rhs);
            auto shifted = _mm512_srl_epi16(content, s);
            auto mask = _mm512_set1_epi8(std::uint8_t(0xFF >> rhs));
            content = _mm512_and_si512(shifted, mask);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            /* Appears to be less efficient than the alternative
            auto lhs_lo = _mm512_cvtepu8_epi16(_mm512_extracti64x4_epi64(content, 0x0));
            auto lhs_hi = _mm512_cvtepu8_epi16(_mm512_extracti64x4_epi64(content, 0x1));

            auto rhs_lo = _mm512_cvtepu8_epi16(_mm512_extracti64x4_epi64(decay(rhs), 0x0));
            auto rhs_hi = _mm512_cvtepu8_epi16(_mm512_extracti64x4_epi64(decay(rhs), 0x1));

            auto result_lo = _mm512_cvtepi16_epi8(_mm512_sllv_epi16(lhs_lo, rhs_lo));
            auto result_hi = _mm512_cvtepi16_epi8(_mm512_sllv_epi16(lhs_hi, rhs_hi));

            auto ret = _mm512_undefined_si512();
            ret = _mm512_inserti64x4(ret, result_lo, 0x0);
            ret = _mm512_inserti64x4(ret, result_hi, 0x1);

            content = ret;
            */

            #if defined(AVEL_AVX512BW)
            auto even_mask = _mm512_set1_epi16(0x00FF);

            auto even_result = _mm512_and_si512(
                even_mask,
                _mm512_sllv_epi16(content, _mm512_and_si512(even_mask, decay(rhs)))
            );

            auto odd_result  = _mm512_sllv_epi16(
                _mm512_andnot_si512(even_mask, content),
                _mm512_srli_epi16(decay(rhs), 0x8)
            );

            auto result = _mm512_or_si512(even_result, odd_result);
            content = result;
            //TODO: Explicitly use vpternlogd
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            auto even_mask = _mm512_set1_epi16(0x00FF);

            auto even_result = _mm512_srlv_epi16(
                _mm512_and_si512(even_mask, content),
                _mm512_and_si512(even_mask, decay(rhs))
            );

            auto odd_result  = _mm512_andnot_si512(
                even_mask,
                _mm512_srlv_epi16(
                    content,
                    _mm512_srli_epi16(decay(rhs), 0x8)
                )
            );

            auto result = _mm512_or_si512(even_result, odd_result);
            content = result;

            //TODO: Explicitly use vpternlogd
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
            return mask{_mm512_test_epi8_mask(content, content)};
            #endif
        }

    };

    static_assert(
        64 * sizeof(std::uint8_t) == sizeof(vec64x8u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint8_t extract(vec64x8u v) {
        static_assert(N < vec64x8u::width, "Specified index does not exist");
        typename std::enable_if<N < vec64x8u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512BW)
        return _mm_extract_epi8(_mm512_extracti32x4_epi32(decay(v), N / 16), N % 16);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec64x8u insert(vec64x8u v, std::uint8_t x) {
        static_assert(N < vec64x8u::width, "Specified index does not exist");
        typename std::enable_if<N < vec64x8u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512BW)
        auto quarter = _mm512_extracti32x4_epi32(decay(v), N / 16);
        quarter = _mm_insert_epi8(quarter, x, N % 16);
        return vec64x8u{_mm512_inserti32x4(decay(v), quarter, N / 16)};
        #endif
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec64x8u bit_shift_left(vec64x8u v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512BW)
        auto shifted = _mm512_slli_epi16(decay(v), S);
        auto mask = _mm512_set1_epi8(std::uint8_t(0xFF << S));
        auto masked = _mm512_and_si512(mask, shifted);

        return vec64x8u{masked};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u bit_shift_left<0>(vec64x8u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u bit_shift_left<1>(vec64x8u v) {
        return v + v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u bit_shift_left<2>(vec64x8u v) {
        auto t0 = v + v;
        auto t1 = t0 + t0;
        return t1;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u bit_shift_left<3>(vec64x8u v) {
        auto t0 = v + v;
        auto t1 = t0 + t0;
        auto t2 = t1 + t1;
        return t2;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u bit_shift_left<8>(vec64x8u v) {
        (void)v;
        return vec64x8u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec64x8u bit_shift_right(vec64x8u v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512BW)
        auto shifted = _mm512_srli_epi16(decay(v), S);
        auto mask = _mm512_set1_epi8(0xFF >> S);
        auto masked = _mm512_and_si512(shifted, mask);

        return vec64x8u{masked};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u bit_shift_right<0>(vec64x8u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u bit_shift_right<8>(vec64x8u v) {
        (void)v;
        return vec64x8u{0x00};
    }




    template<std::uint32_t S, typename std::enable_if<S < 8, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec64x8u rotl(vec64x8u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_GFNI)
        std::uint64_t control = rotr(std::uint64_t{0x0102040810204080ull}, S * 8);
        auto control_vector = _mm512_set1_epi64(control);

        auto result = _mm512_gf2p8affine_epi64_epi8(decay(v), control_vector, 0x00);
        return vec64x8u{result};

        #elif defined(AVEL_AVX512BW)
        auto lo = _mm512_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm512_unpackhi_epi8(decay(v), decay(v));

        lo = _mm512_slli_epi16(lo, S);
        hi = _mm512_slli_epi16(hi, S);

        lo = _mm512_srli_epi16(lo, 8);
        hi = _mm512_srli_epi16(hi, 8);

        auto ret = _mm512_packus_epi16(lo, hi);
        return vec64x8u{ret};

        #endif
    }

    template<>
    AVEL_FINL vec64x8u rotl<0>(vec64x8u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<8 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec64x8u rotl(vec64x8u v) {
        #if defined(AVEL_AVX512BW)
        auto lo = _mm512_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm512_unpackhi_epi8(decay(v), decay(v));

        lo = _mm512_slli_epi16(lo, S);
        hi = _mm512_slli_epi16(hi, S);

        lo = _mm512_srli_epi16(lo, 8);
        hi = _mm512_srli_epi16(hi, 8);

        auto ret = _mm512_packus_epi16(lo, hi);
        return vec64x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u rotl(vec64x8u v, long long s) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_GFNI)
        s &= 0x7;
        std::uint64_t control = rotr(std::uint64_t{0x0102040810204080ull}, s * 8);
        auto control_vector = _mm512_set1_epi64(control);

        auto result = _mm512_gf2p8affine_epi64_epi8(decay(v), control_vector, 0x00);
        return vec64x8u{result};

        #elif defined(AVEL_AVX512BW)
        s &= 0x7;
        auto shift_amount = _mm_cvtsi64_si128(8 - s);
        auto lo = _mm512_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm512_unpackhi_epi8(decay(v), decay(v));

        lo = _mm512_srl_epi16(lo, shift_amount);
        hi = _mm512_srl_epi16(hi, shift_amount);

        auto byte_mask = _mm512_set1_epi16(0x00FF);
        lo = _mm512_and_si512(lo, byte_mask);
        hi = _mm512_and_si512(hi, byte_mask);

        return vec64x8u{_mm512_packus_epi16(lo, hi)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u rotl(vec64x8u v, vec64x8u s) {
        #if defined(AVEL_AVX512BW)
        s = vec64x8u{8} - (s & vec64x8u{0x7});
        auto lo = _mm512_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm512_unpackhi_epi8(decay(v), decay(v));

        auto zeros = _mm512_setzero_si512();
        lo = _mm512_srlv_epi16(lo, _mm512_unpacklo_epi8(decay(s), zeros));
        hi = _mm512_srlv_epi16(hi, _mm512_unpackhi_epi8(decay(s), zeros));

        auto byte_mask = _mm512_set1_epi16(0x00FF);
        lo = _mm512_and_si512(lo, byte_mask);
        hi = _mm512_and_si512(hi, byte_mask);

        return vec64x8u{_mm512_packus_epi16(lo, hi)};
        #endif
    }



    template<std::uint32_t S, typename std::enable_if<S < 8, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec64x8u rotr(vec64x8u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_GFNI)
        std::uint64_t control = rotl(std::uint64_t{0x0102040810204080ull}, S * 8);
        auto control_vector = _mm512_set1_epi64(control);

        auto result = _mm512_gf2p8affine_epi64_epi8(decay(v), control_vector, 0x00);
        return vec64x8u{result};

        #elif defined(AVEL_AVX512BW)
        auto l = bit_shift_left<8 - S>(v);
        auto r = bit_shift_right<S>(v);

        return l | r;
        #endif
    }

    template<>
    AVEL_FINL vec64x8u rotr<0>(vec64x8u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<8 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec64x8u rotr(vec64x8u v) {
        return rotr<S % 8>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u rotr(vec64x8u v, long long s) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_GFNI)
        s &= 0x7;
        std::uint64_t control = rotl(std::uint64_t{0x0102040810204080ull}, s * 8);
        auto control_vector = _mm512_set1_epi64(control);

        auto result = _mm512_gf2p8affine_epi64_epi8(decay(v), control_vector, 0x00);
        return vec64x8u{result};

        #elif defined(AVEL_AVX512BW)
        s &= 0x7;
        auto l = (v << (8 - s));
        auto r = (v >> s);

        return l | r;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u rotr(vec64x8u v, vec64x8u s) {
        #if defined(AVEL_AVX512BW)
        s &= vec64x8u{0x7};
        auto lo = _mm512_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm512_unpackhi_epi8(decay(v), decay(v));

        auto zeros = _mm512_setzero_si512();
        lo = _mm512_srlv_epi16(lo, _mm512_unpacklo_epi8(decay(s), zeros));
        hi = _mm512_srlv_epi16(hi, _mm512_unpackhi_epi8(decay(s), zeros));

        auto byte_mask = _mm512_set1_epi16(0x00FF);
        lo = _mm512_and_si512(lo, byte_mask);
        hi = _mm512_and_si512(hi, byte_mask);

        return vec64x8u{_mm512_packus_epi16(lo, hi)};
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec64x8u x) {
        return count(mask64x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec64x8u x) {
        return any(mask64x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec64x8u x) {
        return all(mask64x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec64x8u x) {
        return none(mask64x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u set_bits(mask64x8u m) {
        #if defined(AVEL_AVX512BW)
        return vec64x8u{_mm512_movm_epi8(decay(m))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u keep(mask64x8u m, vec64x8u v) {
        #if defined(AVEL_AVX512BW)
        return vec64x8u{_mm512_maskz_mov_epi8(decay(m), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u clear(mask64x8u m, vec64x8u v) {
        #if defined(AVEL_AVX512BW)
        return vec64x8u{_mm512_maskz_mov_epi8(decay(!m), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u blend(mask64x8u m, vec64x8u a, vec64x8u b) {
        #if defined(AVEL_AVX512BW)
        return vec64x8u{_mm512_mask_blend_epi8(decay(m), decay(b), decay(a))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u byteswap(vec64x8u v) {
        return v;
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u max(vec64x8u a, vec64x8u b) {
        #if defined(AVEL_AVX512BW)
        return vec64x8u{_mm512_max_epu8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u min(vec64x8u a, vec64x8u b) {
        #if defined(AVEL_AVX512BW)
        return vec64x8u{_mm512_min_epu8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec64x8u, 2> minmax(vec64x8u a, vec64x8u b) {
        #if defined(AVEL_AVX512BW)
        return {
            vec64x8u{_mm512_min_epu8(decay(a), decay(b))},
            vec64x8u{_mm512_max_epu8(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u clamp(vec64x8u x, vec64x8u lo, vec64x8u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u average(vec64x8u a, vec64x8u b) {
        #if defined(AVEL_AVX512BW)
        return vec64x8u{_mm512_avg_epu8(decay(a), decay(b))} - ((a ^ b) & vec64x8u{0x1});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u midpoint(vec64x8u a, vec64x8u b) {
        #if defined(AVEL_AVX512BW)
        auto t1 = _mm512_avg_epu8(decay(a), decay(b));
        auto t5 = _mm512_and_si512(_mm512_ternarylogic_epi32(decay(a), decay(b), decay(set_bits(b < a)), 0x14), _mm512_set1_epi8(0x1));
        auto t6 = _mm512_sub_epi8(t1, t5);
        return vec64x8u{t6};
        #endif
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u load<vec64x8u>(const std::uint8_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512BW)
        auto mask = (n >= 64) ? std::uint64_t(-1) : (std::uint64_t(1) << n) - 1;
        return vec64x8u{_mm512_maskz_loadu_epi8(mask, ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u load<vec64x8u, vec64x8u::width>(const std::uint8_t* ptr) {
        #if defined(AVEL_AVX512BW)
        return vec64x8u{_mm512_loadu_si512(reinterpret_cast<const __m512i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u aligned_load<vec64x8u>(const std::uint8_t* ptr, std::uint32_t n) {
        return load<vec64x8u>(ptr, n);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u aligned_load<vec64x8u, vec64x8u::width>(const std::uint8_t* ptr) {
        #if defined(AVEL_AVX512BW)
        return vec64x8u{_mm512_load_si512(reinterpret_cast<const __m512i*>(ptr))};
        #endif
    }




    AVEL_FINL void store(std::uint8_t* ptr, vec64x8u v, std::uint32_t n) {
        #if defined(AVEL_AVX512BW)
        auto mask = (n >= 64) ? std::uint64_t(-1) : (std::uint64_t(1) << n) - 1;
        _mm512_mask_storeu_epi8(ptr, mask, decay(v));
        #endif
    }

    template<std::uint32_t N = vec64x8u::width>
    AVEL_FINL void store(std::uint8_t* ptr, vec64x8u v) {
        static_assert(N <= vec64x8u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec64x8u::width, int>::type dummy_variable = 0;

        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec64x8u::width>(std::uint8_t* ptr, vec64x8u v) {
        #if defined(AVEL_AVX512BW)
        _mm512_storeu_si512(reinterpret_cast<__m512i*>(ptr), decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(std::uint8_t* ptr, vec64x8u v, std::uint32_t n) {
        store(ptr, v, n);
    }

    template<std::uint32_t N = vec64x8u::width>
    AVEL_FINL void aligned_store(std::uint8_t* ptr, vec64x8u v) {
        static_assert(N <= vec64x8u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec64x8u::width, int>::type dummy_variable = 0;

        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec64x8u::width>(std::uint8_t* ptr, vec64x8u v) {
        #if defined(AVEL_AVX512BW)
        _mm512_store_si512(reinterpret_cast<__m512i*>(ptr), decay(v));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL arr64x8u to_array(vec64x8u v) {
        alignas(64) arr64x8u ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec64x8u> div(vec64x8u x, vec64x8u y) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)
        // This approach fundamentally takes the same approach as Denom64x8u.
        // Using a lookup table is fast enough to beat the long-division
        // approach under the assumption that the table is already cached

        //Special thanks to u/YumiYumiYumi
        alignas(64) static constexpr std::uint8_t recip_hi_table_data[128] {
            0x00, 0x80, 0x55, 0x40, 0x33, 0x2a, 0x24, 0x20,
            0x1c, 0x19, 0x17, 0x15, 0x13, 0x12, 0x11, 0x10,
            0x0f, 0x0e, 0x0d, 0x0c, 0x0c, 0x0b, 0x0b, 0x0a,
            0x0a, 0x09, 0x09, 0x09, 0x08, 0x08, 0x08, 0x08,
            0x07, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06,
            0x06, 0x06, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
        };

        alignas(64) static constexpr std::uint8_t recip_lo_table_data[256] {
            0x00, 0x00, 0x56, 0x00, 0x34, 0xab, 0x93, 0x00,
            0x72, 0x9a, 0x46, 0x56, 0xb2, 0x4a, 0x12, 0x00,
            0x10, 0x39, 0x7a, 0xcd, 0x31, 0xa3, 0x22, 0xab,
            0x3e, 0xd9, 0x7c, 0x25, 0xd4, 0x89, 0x43, 0x00,
            0xc2, 0x88, 0x51, 0x1d, 0xec, 0xbd, 0x91, 0x67,
            0x3f, 0x19, 0xf5, 0xd2, 0xb1, 0x91, 0x73, 0x56,
            0x3a, 0x1f, 0x06, 0xed, 0xd5, 0xbe, 0xa8, 0x93,
            0x7e, 0x6a, 0x57, 0x45, 0x33, 0x22, 0x11, 0x00,
            0xf1, 0xe1, 0xd3, 0xc4, 0xb6, 0xa9, 0x9c, 0x8f,
            0x82, 0x76, 0x6a, 0x5f, 0x54, 0x49, 0x3e, 0x34,
            0x2a, 0x20, 0x16, 0x0d, 0x04, 0xfb, 0xf2, 0xe9,
            0xe1, 0xd9, 0xd1, 0xc9, 0xc1, 0xba, 0xb2, 0xab,
            0xa4, 0x9d, 0x96, 0x90, 0x89, 0x83, 0x7d, 0x77,
            0x71, 0x6b, 0x65, 0x5f, 0x5a, 0x54, 0x4f, 0x4a,
            0x44, 0x3f, 0x3a, 0x35, 0x31, 0x2c, 0x27, 0x23,
            0x1e, 0x1a, 0x15, 0x11, 0x0d, 0x09, 0x05, 0x00,
            0xfd, 0xf9, 0xf5, 0xf1, 0xed, 0xea, 0xe6, 0xe2,
            0xdf, 0xdb, 0xd8, 0xd5, 0xd1, 0xce, 0xcb, 0xc8,
            0xc4, 0xc1, 0xbe, 0xbb, 0xb8, 0xb5, 0xb3, 0xb0,
            0xad, 0xaa, 0xa7, 0xa5, 0xa2, 0x9f, 0x9d, 0x9a,
            0x98, 0x95, 0x93, 0x90, 0x8e, 0x8b, 0x89, 0x87,
            0x84, 0x82, 0x80, 0x7e, 0x7b, 0x79, 0x77, 0x75,
            0x73, 0x71, 0x6f, 0x6d, 0x6b, 0x69, 0x67, 0x65,
            0x63, 0x61, 0x5f, 0x5d, 0x5b, 0x59, 0x58, 0x56,
            0x54, 0x52, 0x51, 0x4f, 0x4d, 0x4b, 0x4a, 0x48,
            0x47, 0x45, 0x43, 0x42, 0x40, 0x3f, 0x3d, 0x3c,
            0x3a, 0x39, 0x37, 0x36, 0x34, 0x33, 0x31, 0x30,
            0x2f, 0x2d, 0x2c, 0x2a, 0x29, 0x28, 0x26, 0x25,
            0x24, 0x22, 0x21, 0x20, 0x1f, 0x1d, 0x1c, 0x1b,
            0x1a, 0x19, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12,
            0x10, 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09,
            0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x00
        };

        __m512i recip_hi0 = _mm512_load_si512(recip_hi_table_data +   0);
        __m512i recip_hi1 = _mm512_load_si512(recip_hi_table_data +  64);

        __m512i recip_lo0 = _mm512_load_si512(recip_lo_table_data +   0);
        __m512i recip_lo1 = _mm512_load_si512(recip_lo_table_data +  64);
        __m512i recip_lo2 = _mm512_load_si512(recip_lo_table_data + 128);
        __m512i recip_lo3 = _mm512_load_si512(recip_lo_table_data + 192);

        __m512i byte_mask = _mm512_set1_epi16(0x00FF);
        __m512i one = _mm512_set1_epi8(1);

        // Don't care about y == 0
        __m512i z = _mm512_sub_epi8(decay(y), one);
        __mmask64 high_bit_mask = _mm512_movepi8_mask(z);

        // Lookup low/high reciprocal halves
        __m512i recip_hi = _mm512_mask_blend_epi8(
            high_bit_mask,
            _mm512_permutex2var_epi8(recip_hi0, z, recip_hi1),
            one
        );

        __m512i recip_lo = _mm512_mask_blend_epi8(
            high_bit_mask,
            _mm512_permutex2var_epi8(recip_lo0, z, recip_lo1),
            _mm512_permutex2var_epi8(recip_lo2, z, recip_lo3)
        );

        // Construct 16-bit reciprocal
        __m512i zeros = _mm512_setzero_si512();
        __m512i reciprocals_lo = _mm512_unpacklo_epi8(recip_lo, recip_hi);
        __m512i reciprocals_hi = _mm512_unpackhi_epi8(recip_lo, recip_hi);

        __m512i x_lo = _mm512_unpacklo_epi8(decay(x), zeros);
        __m512i x_hi = _mm512_unpackhi_epi8(decay(x), zeros);

        // Compute quotient
        __m512i quotient_lo = _mm512_mulhi_epu16(x_lo, reciprocals_lo);
        __m512i quotient_hi = _mm512_mulhi_epu16(x_hi, reciprocals_hi);

        __m512i quotient = _mm512_packus_epi16(quotient_lo, quotient_hi);

        // Handle division by 1
        __mmask64 is_denom_one = _mm512_testn_epi8_mask(z, z);
        quotient = _mm512_mask_blend_epi8(is_denom_one, quotient, decay(x));

        // Construct remainder
        __m512i y_lo = _mm512_unpacklo_epi8(decay(y), zeros);
        __m512i y_hi = _mm512_unpackhi_epi8(decay(y), zeros);

        __m512i tmp_lo = _mm512_mullo_epi16(quotient_lo, y_lo);
        __m512i tmp_hi = _mm512_mullo_epi16(quotient_hi, y_hi);

        __m512i r = _mm512_maskz_sub_epi8(_knot_mask64(is_denom_one), decay(x), _mm512_packus_epi16(tmp_lo, tmp_hi));

        return {
            vec64x8u{quotient},
            vec64x8u{r}
        };

        /*
        alignas(128) static constexpr std::uint8_t bit_width_table_data[128] {
            0, 1, 2, 2, 3, 3, 3, 3,
            4, 4, 4, 4, 4, 4, 4, 4,
            5, 5, 5, 5, 5, 5, 5, 5,
            5, 5, 5, 5, 5, 5, 5, 5,
            6, 6, 6, 6, 6, 6, 6, 6,
            6, 6, 6, 6, 6, 6, 6, 6,
            6, 6, 6, 6, 6, 6, 6, 6,
            6, 6, 6, 6, 6, 6, 6, 6,
            7, 7, 7, 7, 7, 7, 7, 7,
            7, 7, 7, 7, 7, 7, 7, 7,
            7, 7, 7, 7, 7, 7, 7, 7,
            7, 7, 7, 7, 7, 7, 7, 7,
            7, 7, 7, 7, 7, 7, 7, 7,
            7, 7, 7, 7, 7, 7, 7, 7,
            7, 7, 7, 7, 7, 7, 7, 7,
            7, 7, 7, 7, 7, 7, 7, 7
        };

        __m512i bit_width_table0 = _mm512_load_si512(bit_width_table_data +  0);
        __m512i bit_width_table1 = _mm512_load_si512(bit_width_table_data + 64);

        alignas(256) constexpr static std::uint8_t m_table_data[256] {
                0,   1,   1,  86,   1, 154,  86,  37,   1, 200, 154, 117,  86,  60,  37,  18,
                1, 226, 200, 176, 154, 135, 117, 101,  86,  72,  60,  48,  37,  27,  18,   9,
                1, 241, 226, 213, 200, 187, 176, 165, 154, 144, 135, 126, 117, 109, 101,  93,
                86,  79,  72,  66,  60,  54,  48,  42,  37,  32,  27,  22,  18,  13,   9,   5,
                1, 249, 241, 234, 226, 219, 213, 206, 200, 193, 187, 181, 176, 170, 165, 159,
            154, 149, 144, 139, 135, 130, 126, 121, 117, 113, 109, 105, 101,  97,  93,  89,
                86,  82,  79,  75,  72,  69,  66,  63,  60,  57,  54,  51,  48,  45,  42,  40,
                37,  34,  32,  29,  27,  25,  22,  20,  18,  15,  13,  11,   9,   7,   5,   3,
                1, 253, 249, 245, 241, 237, 234, 230, 226, 223, 219, 216, 213, 209, 206, 203,
            200, 196, 193, 190, 187, 184, 181, 179, 176, 173, 170, 167, 165, 162, 159, 157,
            154, 152, 149, 147, 144, 142, 139, 137, 135, 132, 130, 128, 126, 123, 121, 119,
            117, 115, 113, 111, 109, 107, 105, 103, 101,  99,  97,  95,  93,  91,  89,  88,
                86,  84,  82,  81,  79,  77,  75,  74,  72,  71,  69,  67,  66,  64,  63,  61,
                60,  58,  57,  55,  54,  52,  51,  49,  48,  47,  45,  44,  42,  41,  40,  38,
                37,  36,  34,  33,  32,  31,  29,  28,  27,  26,  25,  23,  22,  21,  20,  19,
                18,  16,  15,  14,  13,  12,  11,  10,   9,   8,   7,   6,   5,   4,   3,   2
        };

        __m512i m_table0 = _mm512_load_si512(m_table_data +   0);
        __m512i m_table1 = _mm512_load_si512(m_table_data +  64);
        __m512i m_table2 = _mm512_load_si512(m_table_data + 128);
        __m512i m_table3 = _mm512_load_si512(m_table_data + 192);

        // Compute l via lookup

        __m512i v = _mm512_sub_epi8(decay(y), _mm512_set1_epi8(1));

        __m512i partial_result = _mm512_permutex2var_epi8(bit_width_table0, v, bit_width_table1);

        __mmask64 high_bit_mask = _mm512_movepi8_mask(v);

        __m512i l = _mm512_mask_blend_epi8(high_bit_mask, partial_result, _mm512_set1_epi8(8));

        // Compute m via lookup

        __m512i result0 = _mm512_permutex2var_epi8(m_table0, decay(y), m_table1);
        __m512i result1 = _mm512_permutex2var_epi8(m_table2, decay(y), m_table3);

        __mmask64 table_mask = _mm512_movepi8_mask(decay(y));
        __m512i m = _mm512_mask_blend_epi8(table_mask, result0, result1);

        __mmask64 sh1 = _mm512_test_epi8_mask(l, l);
        __m512i sh2 = _mm512_sub_epi8(l, _mm512_maskz_set1_epi8(sh1, 0x1));

        //Mulhi implementation
        __m512i even_mask = _mm512_set1_epi16(0x00FF);

        __m512i x_even = _mm512_and_si512(even_mask, decay(x));
        __m512i x_odd  = _mm512_srli_epi16(x, 8);

        __m512i y_even = _mm512_and_si512(even_mask, m);
        __m512i y_odd  = _mm512_srli_epi16(m, 8);

        __m512i p_even = _mm512_mullo_epi16(x_even, y_even);
        __m512i p_odd  = _mm512_mullo_epi16(x_odd,  y_odd );

        __m512i mulhi_even = _mm512_srli_epi16(p_even, 8);
        __m512i mulhi_odd  = _mm512_andnot_si512(even_mask, p_odd);

        __m512i mulhi = _mm512_or_si512(mulhi_even, mulhi_odd);

        // Construct quotient
        __m512i t0 = _mm512_sub_epi8(decay(x), mulhi);

        __m512i t0_shifted = _mm512_and_si512( _mm512_srli_epi16(t0, 1), _mm512_set1_epi8(0x7F));
        __m512i t2 = _mm512_mask_blend_epi8(sh1, t0, t0_shifted);

        __m512i t3 = _mm512_add_epi8(mulhi, t2);

        __m512i even_result = _mm512_srlv_epi16(
            _mm512_and_si512(even_mask, t3),
            _mm512_and_si512(even_mask, sh2)
        );

        __m512i odd_result = _mm512_andnot_si512(even_mask, _mm512_srlv_epi16(t3, _mm512_srli_epi16(sh2, 0x8)));

        __m512i quotient = _mm512_or_si512(even_result, odd_result);

        // Construct remainder
        vec64x8u r = x - (vec64x8u{quotient} * y);
        return {vec64x8u{quotient}, r};
        */

        /*
        //TODO: Further optimize
        auto l = bit_width(y - vec64x8u{1});

        // Compute m via lookup
        alignas(256) constexpr static std::uint8_t table_data[256] {
              0,   1,   1,  86,   1, 154,  86,  37,   1, 200, 154, 117,  86,  60,  37,  18,
              1, 226, 200, 176, 154, 135, 117, 101,  86,  72,  60,  48,  37,  27,  18,   9,
              1, 241, 226, 213, 200, 187, 176, 165, 154, 144, 135, 126, 117, 109, 101,  93,
             86,  79,  72,  66,  60,  54,  48,  42,  37,  32,  27,  22,  18,  13,   9,   5,
              1, 249, 241, 234, 226, 219, 213, 206, 200, 193, 187, 181, 176, 170, 165, 159,
            154, 149, 144, 139, 135, 130, 126, 121, 117, 113, 109, 105, 101,  97,  93,  89,
             86,  82,  79,  75,  72,  69,  66,  63,  60,  57,  54,  51,  48,  45,  42,  40,
             37,  34,  32,  29,  27,  25,  22,  20,  18,  15,  13,  11,   9,   7,   5,   3,
              1, 253, 249, 245, 241, 237, 234, 230, 226, 223, 219, 216, 213, 209, 206, 203,
            200, 196, 193, 190, 187, 184, 181, 179, 176, 173, 170, 167, 165, 162, 159, 157,
            154, 152, 149, 147, 144, 142, 139, 137, 135, 132, 130, 128, 126, 123, 121, 119,
            117, 115, 113, 111, 109, 107, 105, 103, 101,  99,  97,  95,  93,  91,  89,  88,
             86,  84,  82,  81,  79,  77,  75,  74,  72,  71,  69,  67,  66,  64,  63,  61,
             60,  58,  57,  55,  54,  52,  51,  49,  48,  47,  45,  44,  42,  41,  40,  38,
             37,  36,  34,  33,  32,  31,  29,  28,  27,  26,  25,  23,  22,  21,  20,  19,
             18,  16,  15,  14,  13,  12,  11,  10,   9,   8,   7,   6,   5,   4,   3,   2
        };

        auto table0 = _mm512_load_si512(table_data +   0);
        auto table1 = _mm512_load_si512(table_data +  64);
        auto table2 = _mm512_load_si512(table_data + 128);
        auto table3 = _mm512_load_si512(table_data + 192);

        auto result0 = _mm512_permutex2var_epi8(table0, decay(y), table1);
        auto result1 = _mm512_permutex2var_epi8(table2, decay(y), table3);

        auto table_mask = _mm512_movepi8_mask(decay(y));
        auto m = _mm512_mask_blend_epi8(table_mask, result0, result1);

        mask64x8u sh1{l};
        auto sh2 = l - vec64x8u{sh1};

        //Mulhi implementation
        auto byte_mask = _mm512_set1_epi16(0x00FF);

        auto x_even = _mm512_and_si512(byte_mask, decay(x));
        auto x_odd  = _mm512_srli_epi16(decay(x), 8);

        auto y_even = _mm512_and_si512(byte_mask, m);
        auto y_odd  = _mm512_srli_epi16(m, 8);

        auto p_even = _mm512_mullo_epi16(x_even, y_even);
        auto p_odd  = _mm512_mullo_epi16(x_odd,  y_odd );

        auto ret_even = _mm512_srli_epi16(p_even, 8);
        auto ret_odd  = _mm512_andnot_si512(byte_mask, p_odd);

        auto mulhi = _mm512_or_si512(ret_even, ret_odd);

        // Construct quotient
        vec64x8u t1{mulhi};
        vec64x8u t0 = (x - t1);
        vec64x8u t2 = blend(sh1, bit_shift_right<1>(t0), t0);
        vec64x8u quotient = (t1 + t2) >> sh2;

        // Construct remainder
        vec64x8u r = x - (vec64x8u{quotient} * y);
        return {vec64x8u{quotient}, r};
        */

        #elif defined(AVEL_AVX512BW)
        vec64x8u quotient{0x00};

        auto ones = _mm512_set1_epi8(0x01);

        mask64x8u b;
        int i = 0;

        if (none(x >= y)) {
            return {quotient, x};
        }

        b = (bit_shift_right<7>(x) >= y);
        x = _mm512_mask_sub_epi8(decay(x), decay(b), decay(x), decay(bit_shift_left<7>(y)));
        quotient = _mm512_mask_add_epi8(decay(quotient), decay(b), decay(quotient), ones);

        if (none(x >= y)) {
            i = 7;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<6>(x) >= y);
        x = _mm512_mask_sub_epi8(decay(x), decay(b), decay(x), decay(bit_shift_left<6>(y)));
        quotient += quotient;
        quotient = _mm512_mask_add_epi8(decay(quotient), decay(b), decay(quotient), ones);

        if (none(x >= y)) {
            i = 6;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<5>(x) >= y);
        x = _mm512_mask_sub_epi8(decay(x), decay(b), decay(x), decay(bit_shift_left<5>(y)));
        quotient += quotient;
        quotient = _mm512_mask_add_epi8(decay(quotient), decay(b), decay(quotient), ones);

        if (none(x >= y)) {
            i = 5;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<4>(x) >= y);
        x = _mm512_mask_sub_epi8(decay(x), decay(b), decay(x), decay(bit_shift_left<4>(y)));
        quotient += quotient;
        quotient = _mm512_mask_add_epi8(decay(quotient), decay(b), decay(quotient), ones);

        if (none(x >= y)) {
            i = 4;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<3>(x) >= y);
        x = _mm512_mask_sub_epi8(decay(x), decay(b), decay(x), decay(bit_shift_left<3>(y)));
        quotient += quotient;
        quotient = _mm512_mask_add_epi8(decay(quotient), decay(b), decay(quotient), ones);

        if (none(x >= y)) {
            i = 3;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<2>(x) >= y);
        x = _mm512_mask_sub_epi8(decay(x), decay(b), decay(x), decay(bit_shift_left<2>(y)));
        quotient += quotient;
        quotient = _mm512_mask_add_epi8(decay(quotient), decay(b), decay(quotient), ones);

        if (none(x >= y)) {
            i = 2;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<1>(x) >= y);
        x = _mm512_mask_sub_epi8(decay(x), decay(b), decay(x), decay(bit_shift_left<1>(y)));
        quotient += quotient;
        quotient = _mm512_mask_add_epi8(decay(quotient), decay(b), decay(quotient), ones);

        if (none(x >= y)) {
            i = 1;
            goto avel_div64x8u_early_end;
        }

        b = (x >= y);
        x = _mm512_mask_sub_epi8(decay(x), decay(b), decay(x), decay(bit_shift_left<0>(y)));
        quotient += quotient;
        quotient = _mm512_mask_add_epi8(decay(quotient), decay(b), decay(quotient), ones);

        avel_div64x8u_early_end:
        quotient = _mm512_sll_epi16(decay(quotient), _mm_cvtsi32_si128(i));
        return {quotient, x};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u popcount(vec64x8u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG)
        return vec64x8u{_mm512_popcnt_epi8(decay(v))};

        #elif defined(AVEL_AVX512BW)
        alignas(64) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4,
        };

        auto table = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data)));

        auto lo_nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(decay(v), lo_nibble_mask);
        auto hi_nibble = _mm512_and_si512(_mm512_srli_epi16(decay(v), 4), lo_nibble_mask);

        auto lo_popcnt = _mm512_shuffle_epi8(table, lo_nibble);
        auto hi_popcnt = _mm512_shuffle_epi8(table, hi_nibble);

        auto popcnt = _mm512_add_epi8(lo_popcnt, hi_popcnt);
        return vec64x8u{popcnt};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u countl_zero(vec64x8u v) {
        //TODO: Consider leveraging conversion to 32-bit lzcnt
        //TODO: Consider leveraging conversion to fp16
        #if defined(AVEL_AVX512BW)
        //Combined high and low nibble lookup table data
        alignas(32) static constexpr std::uint8_t table_data[32] {
            8, 3, 2, 2,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0,
            8, 7, 6, 6,
            5, 5, 5, 5,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        auto nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(nibble_mask, decay(v));
        auto hi_nibble = _mm512_and_si512(nibble_mask, _mm512_srli_epi16(decay(v), 0x4));

        //GCC 9 doesn't recognize _mm512_zextsi128_si512
        //TODO: Consider performance implications of this
        auto tables = _mm512_castsi256_si512(_mm256_load_si256(reinterpret_cast<const __m256i*>(table_data)));

        auto table0 = _mm512_shuffle_i32x4(tables, tables, 0x00);
        auto table1 = _mm512_shuffle_i32x4(tables, tables, 0x55);

        auto partial0 = _mm512_shuffle_epi8(table1, lo_nibble);
        auto partial1 = _mm512_shuffle_epi8(table0, hi_nibble);

        auto ret = _mm512_min_epu8(partial0, partial1);
        return vec64x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u countl_one(vec64x8u v) {
        //TODO: Consider leveraging conversion to 32-bit lzcnt
        //TODO: Consider leveraging conversion to fp16

        #if defined(AVEL_AVX512BW)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            0, 0, 0, 0,
            0, 0, 0, 0,
            1, 1, 1, 1,
            2, 2, 3, 8
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
            4, 4, 4, 4,
            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 7, 8
        };

        auto nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(nibble_mask, decay(v));
        auto hi_nibble = _mm512_and_si512(nibble_mask, _mm512_srli_epi16(decay(v), 0x4));

        auto table0 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm512_shuffle_epi8(table1, lo_nibble);
        auto partial1 = _mm512_shuffle_epi8(table0, hi_nibble);

        auto ret = _mm512_min_epu8(partial0, partial1);
        return vec64x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u countr_zero(vec64x8u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG)
        auto tz_mask = _mm512_andnot_si512(decay(v), _mm512_add_epi8(decay(v), _mm512_set1_epi8(-1)));
        return vec64x8u{_mm512_popcnt_epi8(tz_mask)};

        //TODO: Consider leveraging conversion to fp16

        #elif defined(AVEL_AVX512BW)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            8, 0, 1, 0,
            2, 0, 1, 0,
            3, 0, 1, 0,
            2, 0, 1, 0
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
            8, 4, 5, 4,
            6, 4, 5, 4,
            7, 4, 5, 4,
            6, 4, 5, 4
        };

        auto nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(nibble_mask, decay(v));
        auto hi_nibble = _mm512_and_si512(nibble_mask, _mm512_srli_epi16(decay(v), 0x4));

        auto table0 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm512_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm512_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm512_min_epu8(partial0, partial1);
        return vec64x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u countr_one(vec64x8u v) {
        //TODO: Consider leveraging tzcnt
        //TODO: Consider leveraging conversion to fp16
        #if defined(AVEL_AVX512BW)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            0, 1, 0, 2,
            0, 1, 0, 3,
            0, 1, 0, 2,
            0, 1, 0, 8
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
            4, 5, 4, 6,
            4, 5, 4, 7,
            4, 5, 4, 6,
            4, 5, 4, 8
        };

        auto nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(nibble_mask, decay(v));
        auto hi_nibble = _mm512_and_si512(nibble_mask, _mm512_srli_epi16(decay(v), 0x4));

        auto table0 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm512_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm512_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm512_min_epu8(partial0, partial1);
        return vec64x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u bit_width(vec64x8u v) {
        //TODO: Consider leveraging 32-bit tzcnt
        //TODO: Consider leveraging conversion to fp16

        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)
        auto table0 = _mm512_load_si512(bit_width_table_data + 0x00);
        auto table1 = _mm512_load_si512(bit_width_table_data + 0x40);

        auto partial_result = _mm512_permutex2var_epi8(table0, decay(v), table1);

        auto is_high_bit_set_mask = _mm512_movepi8_mask(decay(v));

        auto ret = _mm512_mask_blend_epi8(
            is_high_bit_set_mask,
            partial_result,
            _mm512_set1_epi8(8)
        );

        return vec64x8u{ret};

        #elif defined(AVEL_AVX512BW)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            0, 1, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
            0, 5, 6, 6,
            7, 7, 7, 7,
            8, 8, 8, 8,
            8, 8, 8, 8
        };

        auto nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(nibble_mask, decay(v));
        auto hi_nibble = _mm512_and_si512(nibble_mask, _mm512_srli_epi16(decay(v), 0x4));

        auto table0 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm512_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm512_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm512_max_epu8(partial0, partial1);
        return vec64x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u bit_floor(vec64x8u v) {
        //TODO: Consider leveraging tzcnt
        //TODO: Consider leveraging conversion to fp16

        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)
        auto table_half0 = _mm512_load_si512(bit_floor_table_data + 0x00);
        auto table_half1 = _mm512_load_si512(bit_floor_table_data + 0x40);

        auto table_results = _mm512_permutex2var_epi8(table_half0, decay(v), table_half1);

        auto is_high_bit_set_mask = _mm512_movepi8_mask(decay(v));
        auto result = _mm512_mask_blend_epi8(is_high_bit_set_mask, table_results, _mm512_set1_epi8(0x80));
        return vec64x8u{result};

        #elif defined(AVEL_AVX512BW)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            0, 1, 2, 2,
            4, 4, 4, 4,
            8, 8, 8, 8,
            8, 8, 8, 8,
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
              0,  16,  32,  32,
             64,  64,  64,  64,
            128, 128, 128, 128,
            128, 128, 128, 128
        };

        auto nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(nibble_mask, decay(v));
        auto hi_nibble = _mm512_and_si512(nibble_mask, _mm512_srli_epi16(decay(v), 0x4));

        auto table0 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm512_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm512_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm512_max_epu8(partial0, partial1);
        return vec64x8u{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u bit_ceil(vec64x8u v) {
        //TODO: Consider leveraging tzcnt
        //TODO: Consider leveraging conversion to fp16

        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)
        auto ones = _mm512_set1_epi8(0x01);
        auto v_clamped = _mm512_max_epu8(decay(v), ones);
        auto table_indices = _mm512_sub_epi8(v_clamped, ones);

        auto table_half0 = _mm512_load_si512(bit_ceil_table_data + 0x00);
        auto table_half1 = _mm512_load_si512(bit_ceil_table_data + 0x40);

        auto table_results = _mm512_permutex2var_epi8(table_half0, table_indices, table_half1);

        auto is_result_non_zero_mask = _knot_mask64(_mm512_movepi8_mask(table_indices));
        auto result = _mm512_maskz_mov_epi8(is_result_non_zero_mask, table_results);
        return vec64x8u{result};

        #elif defined(AVEL_AVX512BW)
        alignas(16) static constexpr std::uint8_t table_data0[16] {
             0,  1,  3,  3,
             7,  7,  7,  7,
            15, 15, 15, 15,
            15, 15, 15, 15
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
              0,  31,  63,  63,
            127, 127, 127, 127,
            255, 255, 255, 255,
            255, 255, 255, 255
        };

        auto zero_mask = _mm512_cmpeq_epi8_mask(decay(v), _mm512_setzero_si512());

        --v;

        auto nibble_mask = _mm512_set1_epi8(0x0F);
        auto lo_nibble = _mm512_and_si512(nibble_mask, decay(v));
        auto hi_nibble = _mm512_and_si512(nibble_mask, _mm512_srli_epi16(decay(v), 0x4));

        auto table0 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data0)));
        auto table1 = _mm512_broadcast_i32x4(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data1)));

        auto partial0 = _mm512_shuffle_epi8(table0, lo_nibble);
        auto partial1 = _mm512_shuffle_epi8(table1, hi_nibble);

        auto ret = _mm512_add_epi8(_mm512_max_epu8(partial0, partial1), _mm512_set1_epi8(0x01));
        return vec64x8u{_mm512_add_epi8(ret, _mm512_maskz_set1_epi8(zero_mask, 1))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask64x8u has_single_bit(vec64x8u v) {
        #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG)
        return popcount(v) == vec64x8u{0x01};

        #elif defined(AVEL_AVX512BW)
        return mask64x8u{v} && !mask64x8u{v & (v - vec64x8u{1})};

        #endif
    }

}

#endif //AVEL_VEC64X8U_HPP
