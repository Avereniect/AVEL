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

    div_type<vec64x8u> div(vec64x8u numerator, vec64x8u denominator);
    vec64x8u broadcast_mask(mask64x8u m);
    vec64x8u blend(vec64x8u a, vec64x8u b, mask64x8u m);
    vec64x8u countl_one(vec64x8u v);





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

        using primitive = __mmask64;

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

            auto array_data = _mm512_loadu_si512(arr.data());
            content = _mm512_cmplt_epu8_mask(_mm512_setzero_si512(), array_data);
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
            return decay(lhs) == decay(rhs);
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            return decay(lhs) != decay(rhs);
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            content &= decay(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            content |= decay(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            content ^= decay(rhs);
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
    AVEL_FINL std::uint32_t count(mask64x8u m) {
        return popcount(std::uint64_t(decay(m)));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask64x8u m) {
        return !_kortestz_mask64_u8(decay(m), decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask64x8u m) {
        return _kortestc_mask64_u8(decay(m), decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask64x8u m) {
        return _kortestz_mask64_u8(decay(m), decay(m));
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

        using primitive = __m512i;

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
            content(_mm512_sub_epi8(_mm512_setzero_si512(), _mm512_movm_epi8(decay(m)))) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm512_set1_epi8(x)) {}

        AVEL_FINL explicit Vector(const arr64x8u& arr):
            content(_mm512_loadu_si512(arr.data())) {}

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
            return mask{_mm512_cmpeq_epi8_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpneq_epi8_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            return mask{_mm512_cmplt_epu8_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmple_epu8_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpgt_epu8_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpge_epu8_mask(decay(lhs), decay(rhs))};
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
            content = _mm512_add_epi8(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm512_sub_epi8(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            auto even_mask = _mm512_set1_epi16(0x00FF);

            auto even_product = _mm512_and_si512(even_mask, _mm512_mullo_epi16(content, decay(rhs)));
            auto odd_product  = _mm512_slli_epi16(
                _mm512_mullo_epi16(
                    _mm512_srli_epi16(content, 0x8),
                    _mm512_srli_epi16(decay(rhs), 0x8)
                ),
                0x8
            );

            auto product = _mm512_or_si512(even_product, odd_product);
            content = product;
            //TODO: Explicitly use vpternlogd

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
            content = _mm512_and_si512(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            content = _mm512_or_si512(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            content = _mm512_xor_si512(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            auto s = _mm_cvtsi32_si128(rhs);

            auto shifted = _mm512_sll_epi16(content, s);
            auto mask = _mm512_set1_epi8(std::uint8_t(0xFF << rhs));
            content = _mm512_and_si512(mask, shifted);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            auto s = _mm_cvtsi32_si128(rhs);
            auto shifted = _mm512_srl_epi16(content, s);
            auto mask = _mm512_set1_epi8(std::uint8_t(0xFF >> rhs));
            content = _mm512_and_si512(shifted, mask);
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

            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
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

            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
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
            return mask{_mm512_test_epi8_mask(content, content)};
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
        static_assert(N <= 64, "Specified index does not exist");
        typename std::enable_if<N <= 64, int>::type dummy_variable = 0;

        return _mm_extract_epi8(_mm512_extracti32x4_epi32(decay(v), N / 16), N % 16);
    }

    template<std::uint32_t N>
    AVEL_FINL vec64x8u insert(vec64x8u v, std::uint8_t x) {
        static_assert(N <= 64, "Specified index does not exist");
        typename std::enable_if<N <= 64, int>::type dummy_variable = 0;

        auto quarter = _mm512_extracti32x4_epi32(decay(v), N / 16);
        quarter = _mm_insert_epi8(quarter, x, N % 16);
        return vec64x8u{_mm512_inserti32x4(decay(v), quarter, N / 16)};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec64x8u bit_shift_left(vec64x8u v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        auto shifted = _mm512_slli_epi16(decay(v), S);
        auto mask = _mm512_set1_epi8(std::uint8_t(0xFF << S));
        auto masked = _mm512_and_si512(mask, shifted);

        return vec64x8u{masked};
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

        auto shifted = _mm512_srli_epi16(decay(v), S);
        auto mask = _mm512_set1_epi8(std::uint8_t(0xFF >> S));
        auto masked = _mm512_and_si512(shifted, mask);

        return vec64x8u{masked};
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
        #if defined(AVEL_GFNI)
        std::uint64_t control = rotr(std::uint64_t{0x0102040810204080ull}, S * 8);
        auto control_vector = _mm512_set1_epi64(control);

        auto result = _mm512_gf2p8affine_epi64_epi8(decay(v), control_vector, 0x00);
        return vec64x8u{result};

        #else
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
        auto lo = _mm512_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm512_unpackhi_epi8(decay(v), decay(v));

        lo = _mm512_slli_epi16(lo, S);
        hi = _mm512_slli_epi16(hi, S);

        lo = _mm512_srli_epi16(lo, 8);
        hi = _mm512_srli_epi16(hi, 8);

        auto ret = _mm512_packus_epi16(lo, hi);
        return vec64x8u{ret};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u rotl(vec64x8u v, long long s) {

        #if defined(AVEL_GFNI)
        s &= 0x7;
        std::uint64_t control = rotr(std::uint64_t{0x0102040810204080ull}, s * 8);
        auto control_vector = _mm512_set1_epi64(control);

        auto result = _mm512_gf2p8affine_epi64_epi8(decay(v), control_vector, 0x00);
        return vec64x8u{result};

        #else
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
    }



    template<std::uint32_t S, typename std::enable_if<S < 8, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec64x8u rotr(vec64x8u v) {
        #if defined(AVEL_GFNI)
        std::uint64_t control = rotl(std::uint64_t{0x0102040810204080ull}, S * 8);
        auto control_vector = _mm512_set1_epi64(control);

        auto result = _mm512_gf2p8affine_epi64_epi8(decay(v), control_vector, 0x00);
        return vec64x8u{result};

        #else
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
        #if defined(AVEL_GFNI)
        s &= 0x7;
        std::uint64_t control = rotl(std::uint64_t{0x0102040810204080ull}, s * 8);
        auto control_vector = _mm512_set1_epi64(control);

        auto result = _mm512_gf2p8affine_epi64_epi8(decay(v), control_vector, 0x00);
        return vec64x8u{result};

        #else
        s &= 0x7;
        auto l = (v << (8 - s));
        auto r = (v >> s);

        return l | r;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u rotr(vec64x8u v, vec64x8u s) {
        /* //TODO: Leverage this implementation if AVEL_AVX512VBMI2 is available
        _mm512_shldi_epi16(_mm512_shrdv_epi16(a, _mm512_swaphl_epi8(a), _mm512_alignr_epi8(_rotate_vcnt_r(mm512, cnt), _rotate_vcnt_r(mm512, cnt), 1)), _mm512_shrdv_epi16(_mm512_swaphl_epi8(a), a, _rotate_vcnt_r(mm512, cnt)), 8);
        */
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
    AVEL_FINL vec64x8u broadcast_mask(mask64x8u m) {
        return vec64x8u{_mm512_movm_epi8(decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u keep(mask64x8u m, vec64x8u v) {
        return vec64x8u{_mm512_maskz_mov_epi8(decay(m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u clear(mask64x8u m, vec64x8u v) {
        return vec64x8u{_mm512_maskz_mov_epi8(decay(!m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u blend(mask64x8u m, vec64x8u a, vec64x8u b) {
        return vec64x8u{_mm512_mask_blend_epi8(decay(m), decay(b), decay(a))};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u byteswap(vec64x8u v) {
        return v;
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u max(vec64x8u a, vec64x8u b) {
        return vec64x8u{_mm512_max_epu8(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u min(vec64x8u a, vec64x8u b) {
        return vec64x8u{_mm512_min_epu8(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec64x8u, 2> minmax(vec64x8u a, vec64x8u b) {
        return {
            vec64x8u{_mm512_min_epu8(decay(a), decay(b))},
            vec64x8u{_mm512_max_epu8(decay(a), decay(b))}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u clamp(vec64x8u x, vec64x8u lo, vec64x8u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u average(vec64x8u a, vec64x8u b) {
        return vec64x8u{_mm512_avg_epu8(decay(a), decay(b))} - ((a ^ b) & vec64x8u{0x1});
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u midpoint(vec64x8u a, vec64x8u b) {
        auto t1 = _mm512_avg_epu8(decay(a), decay(b));
        auto t5 = _mm512_and_si512(_mm512_ternarylogic_epi32(decay(a), decay(b), decay(broadcast_mask(b < a)), 0x14), _mm512_set1_epi8(0x1));
        auto t6 = _mm512_sub_epi8(t1, t5);
        return vec64x8u{t6};
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u load<vec64x8u>(const std::uint8_t* ptr, std::uint32_t n) {
        auto mask = (n >= 64) ? std::uint64_t(-1) : (std::uint64_t(1) << n) - 1;
        return vec64x8u{_mm512_maskz_loadu_epi8(mask, ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u load<vec64x8u, vec64x8u::width>(const std::uint8_t* ptr) {
        return vec64x8u{_mm512_loadu_si512(reinterpret_cast<const __m512i*>(ptr))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u aligned_load<vec64x8u>(const std::uint8_t* ptr, std::uint32_t n) {
        return load<vec64x8u>(ptr, n);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8u aligned_load<vec64x8u, vec64x8u::width>(const std::uint8_t* ptr) {
        return vec64x8u{_mm512_load_si512(reinterpret_cast<const __m512i*>(ptr))};
    }




    AVEL_FINL void store(std::uint8_t* ptr, vec64x8u v, std::uint32_t n) {
        auto mask = (n >= 64) ? std::uint64_t(-1) : (std::uint64_t(1) << n) - 1;
        _mm512_mask_storeu_epi8(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec64x8u::width>
    AVEL_FINL void store(std::uint8_t* ptr, vec64x8u v) {
        static_assert(N <= vec64x8u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec64x8u::width, int>::type dummy_variable = 0;

        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec64x8u::width>(std::uint8_t* ptr, vec64x8u v) {
        _mm512_storeu_si512(reinterpret_cast<__m512i*>(ptr), decay(v));
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
        _mm512_store_si512(reinterpret_cast<__m512i*>(ptr), decay(v));
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
        vec64x8u quotient{};

        mask64x8u b;
        int i = 0;

        if (none(mask64x8u{x})) {
            return {quotient, x};
        }

        b = (bit_shift_right<7>(x) >= y);
        x -= (broadcast_mask(b) & bit_shift_left<7>(y));
        quotient -= broadcast_mask(b);

        if (none(mask64x8u{x})) {
            i = 7;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<6>(x) >= y);
        x -= (broadcast_mask(b) & bit_shift_left<6>(y));
        quotient += quotient;
        quotient -= broadcast_mask(b);

        if (none(mask64x8u{x})) {
            i = 6;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<5>(x) >= y);
        x -= (broadcast_mask(b) & bit_shift_left<5>(y));
        quotient += quotient;
        quotient -= broadcast_mask(b);

        if (none(mask64x8u{x})) {
            i = 5;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<4>(x) >= y);
        x -= (broadcast_mask(b) & bit_shift_left<4>(y));
        quotient += quotient;
        quotient -= broadcast_mask(b);

        if (none(mask64x8u{x})) {
            i = 4;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<3>(x) >= y);
        x -= (broadcast_mask(b) & bit_shift_left<3>(y));
        quotient += quotient;
        quotient -= broadcast_mask(b);

        if (none(mask64x8u{x})) {
            i = 3;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<2>(x) >= y);
        x -= (broadcast_mask(b) & bit_shift_left<2>(y));
        quotient += quotient;
        quotient -= broadcast_mask(b);

        if (none(mask64x8u{x})) {
            i = 2;
            goto avel_div64x8u_early_end;
        }

        b = (bit_shift_right<1>(x) >= y);
        x -= (broadcast_mask(b) & bit_shift_left<1>(y));
        quotient += quotient;
        quotient -= broadcast_mask(b);

        if (none(mask64x8u{x})) {
            i = 1;
            goto avel_div64x8u_early_end;
        }

        b = (x >= y);
        x -= (broadcast_mask(b) & y);
        quotient += quotient;
        quotient -= broadcast_mask(b);

        avel_div64x8u_early_end:
        quotient = _mm512_sll_epi16(decay(quotient), _mm_cvtsi32_si128(i));
        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u popcount(vec64x8u v) {
        #if defined(AVEL_BITALG)
        return vec64x8u{_mm512_popcnt_epi8(decay(v))};

        #else
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
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u countl_one(vec64x8u v) {
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
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u countr_zero(vec64x8u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        auto neg_one = _mm512_set1_epi8(-1);
        auto tz_mask = _mm512_andnot_si512(decay(v), _mm512_add_epi8(decay(v), neg_one));
        return vec64x8u{_mm512_popcnt_epi8(tz_mask)};

        #else
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
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return countr_zero(~v);

        #else
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
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u bit_floor(vec64x8u v) {
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
    }

    [[nodiscard]]
    AVEL_FINL vec64x8u bit_ceil(vec64x8u v) {
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
    }

    [[nodiscard]]
    AVEL_FINL mask64x8u has_single_bit(vec64x8u v) {
        #if defined(AVEL_AVX512BITALG)
        return popcount(v) == vec64x8u{0x01};

        #else
        return mask64x8u{v} && !mask64x8u{v & (v - vec64x8u{1})};

        #endif
    }

}

#endif //AVEL_VEC64X8U_HPP
