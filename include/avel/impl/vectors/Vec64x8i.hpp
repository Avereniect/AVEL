#ifndef AVEL_VEC64X8I_HPP
#define AVEL_VEC64X8I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec64x8i = Vector<std::int8_t, 64>;
    using arr64x8i = std::array<std::int8_t, 64>;
    using mask64x8i = Vector_mask<std::int8_t, 64>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec64x8i> div(vec64x8i x, vec64x8i y);
    vec64x8i blend(vec64x8i a, vec64x8i b, mask64x8i m);
    vec64x8i set_bits(mask64x8i m);
    vec64x8i negate(mask64x8i m, vec64x8i x);





    template<>
    class Vector_mask<std::int8_t, 64> {
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
        // Assignment operators
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
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask64x8u, 1> convert<mask64x8u, mask64x8i>(mask64x8i m) {
        return {mask64x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask64x8i, 1> convert<mask64x8i, mask64x8u>(mask64x8u m) {
        return {mask64x8i(decay(m))};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask64x8i m) {
        return count(mask64x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask64x8i m) {
        return any(mask64x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask64x8i m) {
        return all(mask64x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask64x8i m) {
        return none(mask64x8u{m});
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL bool extract(mask64x8i m) {
        static_assert(N < mask64x8i::width, "Specified index does not exist");
        typename std::enable_if<N < mask64x8i::width, int>::type dummy_variable = 0;

        return extract<N>(mask64x8u{m});
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL mask64x8i insert(mask64x8i m, bool b) {
        static_assert(N < mask64x8i::width, "Specified index does not exist");
        typename std::enable_if<N < mask64x8i::width, int>::type dummy_variable = 0;

        return mask64x8i{insert<N>(mask64x8u{m}, b)};
    }





    template<>
    class alignas(64) Vector<std::int8_t, 64> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 64;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int8_t;

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
        AVEL_FINL explicit Vector(Vector<U, width> v):
            Vector(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512BW)
            content(_mm512_maskz_set1_epi8(decay(m), 0x1)) {}
        #endif

        AVEL_FINL explicit Vector(primitive m):
            content(m) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX512BW)
            content(_mm512_set1_epi8(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr64x8i& array):
        #if defined(AVEL_AVX512BW)
            content(_mm512_loadu_si512(array.data())) {}
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
        Vector& operator=(Vector&&) = default;

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
            return mask{_mm512_cmplt_epi8_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmple_epi8_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpgt_epi8_mask(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512BW)
            return mask{_mm512_cmpge_epi8_mask(decay(lhs), decay(rhs))};
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

        AVEL_FINL Vector& operator&=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_and_si512(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_or_si512(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_AVX512BW)
            content = _mm512_xor_si512(content, rhs.content);
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
            /* //TODO: Fix implementation
            static constexpr std::uint64_t shift_matrices[9] {
                0x0102040810204080ull,
                0x0204081020408080ull,
                0x0408102040808080ull,
                0x0810204080808080ull,

                0x1020408080808080ull,
                0x2040808080808080ull,
                0x4080808080808080ull,
                0x8080808080808080ull,
                0x8080808080808080ull,
            };

            const auto matrix = shift_matrices[rhs];
            content = _mm512_gf2p8affine_epi64_epi8(content, _mm512_set1_epi64(matrix), 0x00);
            */

            #if defined(AVEL_AVX512BW)
            //TODO: Optimize
            auto sign_bits = _mm512_cmplt_epi8_mask(content, _mm512_setzero_si512());
            auto lo = _mm512_unpacklo_epi8(content, _mm512_movm_epi8(sign_bits));
            auto hi = _mm512_unpackhi_epi8(content, _mm512_movm_epi8(sign_bits));

            auto s = _mm_cvtsi32_si128(rhs);
            auto lo_shifted = _mm512_sra_epi16(lo, s);
            auto hi_shifted = _mm512_sra_epi16(hi, s);

            auto result = _mm512_packs_epi16(lo_shifted, hi_shifted);

            content = result;
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
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
            //TODO: Optimize?
            auto sign_bits = _mm512_cmplt_epi8_mask(content, _mm512_setzero_si512());
            auto lhs_lo = _mm512_unpacklo_epi8(content, _mm512_movm_epi8(sign_bits));
            auto lhs_hi = _mm512_unpackhi_epi8(content, _mm512_movm_epi8(sign_bits));

            auto rhs_lo = _mm512_unpacklo_epi8(decay(rhs), _mm512_setzero_si512());
            auto rhs_hi = _mm512_unpackhi_epi8(decay(rhs), _mm512_setzero_si512());

            auto lo_shifted = _mm512_srav_epi16(lhs_lo, rhs_lo);
            auto hi_shifted = _mm512_srav_epi16(lhs_hi, rhs_hi);

            auto result = _mm512_packs_epi16(lo_shifted, hi_shifted);

            content = result;
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
        64 * sizeof(std::int8_t) == sizeof(vec64x8i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec64x8u, 1> convert<vec64x8u, vec64x8i>(vec64x8i x) {
        return {vec64x8u{decay(x)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec64x8i, 1> convert<vec64x8i, vec64x8u>(vec64x8u x) {
        return {vec64x8i{decay(x)}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec64x8i operator-(vec64x8u v) {
        return vec64x8i{0x00} - vec64x8i{v};
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL std::int8_t extract(vec64x8i v) {
        static_assert(N < vec64x8i::width, "Specified index does not exist");
        typename std::enable_if<N < vec64x8i::width, int>::type dummy_variable = 0;

        return static_cast<std::int8_t>(extract<N>(vec64x8u{v}));
    }

    template<std::uint32_t N>
    AVEL_FINL vec64x8i insert(vec64x8i v, std::int8_t x) {
        static_assert(N < vec64x8i::width, "Specified index does not exist");
        typename std::enable_if<N < vec64x8i::width, int>::type dummy_variable = 0;

        return vec64x8i{insert<N>(vec64x8u{v}, static_cast<std::uint8_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec64x8i bit_shift_left(vec64x8i v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        return vec64x8i{bit_shift_left<S>(vec64x8u{v})};
    }

    template<std::uint32_t S>
    AVEL_FINL vec64x8i bit_shift_right(vec64x8i v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512BW)
        auto lo = _mm512_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm512_unpackhi_epi8(decay(v), decay(v));

        auto lo_shifted = _mm512_srai_epi16(lo, S + 8);
        auto hi_shifted = _mm512_srai_epi16(hi, S + 8);

        auto result = _mm512_packs_epi16(lo_shifted, hi_shifted);
        return vec64x8i{result};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8i bit_shift_right<0>(vec64x8i v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8i bit_shift_right<7>(vec64x8i v) {
        #if defined(AVEL_AVX512BW)
        return vec64x8i{_mm512_maskz_set1_epi8(_mm512_movepi8_mask(decay(v)), -1)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8i bit_shift_right<8>(vec64x8i v) {
        return set_bits(v < vec64x8i{0x00});
    }

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec64x8i rotl(vec64x8i v) {
        return vec64x8i{rotl<S>(vec64x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i rotl(vec64x8i v, std::uint32_t s) {
        return vec64x8i(rotl(vec64x8u(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i rotl(vec64x8i v, vec64x8i s) {
        return vec64x8i(rotl(vec64x8u(v), vec64x8u(s)));
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec64x8i rotr(vec64x8i v) {
        return vec64x8i{rotr<S>(vec64x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i rotr(vec64x8i v, std::uint32_t s) {
        return vec64x8i(rotr(vec64x8u(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i rotr(vec64x8i v, vec64x8i s) {
        return vec64x8i(rotr(vec64x8u(v), vec64x8u(s)));
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec64x8i x) {
        return count(vec64x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec64x8i x) {
        return any(vec64x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec64x8i x) {
        return all(vec64x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec64x8i x) {
        return none(vec64x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i set_bits(mask64x8i m) {
        return vec64x8i{set_bits(mask64x8u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i keep(mask64x8i m, vec64x8i v) {
        return vec64x8i{keep(mask64x8u{m}, vec64x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i clear(mask64x8i m, vec64x8i v) {
        return vec64x8i{clear(mask64x8u{m}, vec64x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i blend(mask64x8i m, vec64x8i a, vec64x8i b) {
        return vec64x8i{blend(mask64x8u{m}, vec64x8u{a}, vec64x8u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i byteswap(vec64x8i v) {
        return v;
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i max(vec64x8i a, vec64x8i b) {
        #if defined(AVEL_AVX512BW)
        return vec64x8i{_mm512_max_epi8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i min(vec64x8i a, vec64x8i b) {
        #if defined(AVEL_AVX512BW)
        return vec64x8i{_mm512_min_epi8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec64x8i, 2> minmax(vec64x8i a, vec64x8i b) {
        #if defined(AVEL_AVX512BW)
        return {
            vec64x8i{_mm512_min_epi8(decay(a), decay(b))},
            vec64x8i{_mm512_max_epi8(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i clamp(vec64x8i x, vec64x8i lo, vec64x8i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i average(vec64x8i x, vec64x8i y) {
        auto avg = (x & y) + ((x ^ y) >> 1);
        auto c = set_bits((x < -y) | (y == vec64x8i(0x80))) & (x ^ y) & vec64x8i{1};

        return avg + c;
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i midpoint(vec64x8i a, vec64x8i b) {
        #if defined(AVEL_AVX512BW)
        auto avg = _mm512_avg_epu8(decay(a), decay(b));
        auto sign_correction = _mm512_ternarylogic_epi32(decay(a), decay(b), _mm512_set1_epi8(0x80), 0x28);
        auto average = _mm512_xor_si512(avg, sign_correction);

        auto bias = _mm512_ternarylogic_epi32(decay(a), decay(b), _mm512_set1_epi8(0x1), 0x28);
        auto mask = _mm512_cmplt_epi8_mask(decay(a), decay(b));
        auto ret = _mm512_mask_sub_epi8(average, mask, average, bias);

        return vec64x8i{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i negate(mask64x8i m, vec64x8i x) {
        #if defined(AVEL_AVX512BW)
        return vec64x8i{_mm512_mask_sub_epi8(decay(x), decay(m), _mm512_setzero_si512(), decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i abs(vec64x8i x) {
        #if defined(AVEL_AVX512BW)
        return vec64x8i{_mm512_abs_epi8(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i neg_abs(vec64x8i x) {
        #if defined(AVEL_AVX512BW)
        return -vec64x8i{_mm512_abs_epi8(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec64x8i neg_abs(vec64x8u x) {
        return neg_abs(vec64x8i{x});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8i load<vec64x8i>(const std::int8_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512BW)
        auto mask = (n >= 64) ? std::uint64_t(-1) : (std::uint64_t(1) << n) - 1;
        return vec64x8i{_mm512_maskz_loadu_epi8(mask, ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8i load<vec64x8i, vec64x8i::width>(const std::int8_t* ptr) {
        #if defined(AVEL_AVX512BW)
        return vec64x8i{_mm512_loadu_si512(ptr)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8i aligned_load<vec64x8i>(const std::int8_t* ptr, std::uint32_t n) {
        return load<vec64x8i>(ptr, n);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec64x8i aligned_load<vec64x8i, vec64x8u::width>(const std::int8_t* ptr) {
        #if defined(AVEL_AVX512BW)
        return vec64x8i{_mm512_load_si512(ptr)};
        #endif
    }



    AVEL_FINL void store(std::int8_t* ptr, vec64x8i x, std::uint32_t n) {
        #if defined(AVEL_AVX512BW)
        auto mask = (n >= 64) ? std::uint64_t(-1) : (std::uint64_t(1) << n) - 1;
        _mm512_mask_storeu_epi8(ptr, mask, decay(x));
        #endif
    }

    template<std::uint32_t N = vec64x8i::width>
    AVEL_FINL void store(std::int8_t* ptr, vec64x8i x) {
        static_assert(N <= vec64x8i::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= vec64x8i::width, int>::type dummy_variable = 0;

        store(ptr, x, N);
    }

    template<>
    AVEL_FINL void store<vec64x8u::width>(std::int8_t* ptr, vec64x8i x) {
        #if defined(AVEL_AVX512BW)
        _mm512_storeu_si512(ptr, decay(x));
        #endif
    }


    AVEL_FINL void aligned_store(std::int8_t* ptr, vec64x8i x, std::uint32_t n) {
        store(ptr, x, n);
    }

    template<std::uint32_t N = vec64x8i::width>
    AVEL_FINL void aligned_store(std::int8_t* ptr, vec64x8i x) {
        static_assert(N <= vec64x8i::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= vec64x8i::width, int>::type dummy_variable = 0;

        aligned_store(ptr, x, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec64x8i::width>(std::int8_t* ptr, vec64x8i x) {
        #if defined(AVEL_AVX512BW)
        _mm512_store_si512(ptr, decay(x));
        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr64x8i to_array(vec64x8i v) {
        alignas(64) arr64x8i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec64x8i> div(vec64x8i x, vec64x8i y) {
        mask64x8i remainder_sign_mask{_mm512_movepi8_mask(decay(x))};
        mask64x8i quotient_sign_mask = remainder_sign_mask ^ mask64x8i{_mm512_movepi8_mask(decay(y))};

        vec64x8u numerator{abs(x)};
        vec64x8u denominator{abs(y)};

        auto results = div(numerator, denominator);

        return {
            negate(quotient_sign_mask,  vec64x8i{results.quot}),
            negate(remainder_sign_mask, vec64x8i{results.rem})
        };
    }

    AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(vec64x8i, mask64x8i, vec64x8u)

}

#endif //AVEL_VEC64X8I_HPP
