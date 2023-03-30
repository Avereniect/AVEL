#ifndef AVEL_VEC32X8I_HPP
#define AVEL_VEC32X8I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec32x8i = Vector<std::int8_t, 32>;
    using arr32x8i = std::array<std::int8_t, 32>;
    using mask32x8i = Vector_mask<std::int8_t, 32>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec32x8i> div(vec32x8i numerator, vec32x8i denominator);
    vec32x8i blend(mask32x8i m, vec32x8i a, vec32x8i b);
    vec32x8i broadcast_mask(mask32x8i m);
    vec32x8i countl_one(vec32x8i x);





    template<>
    class alignas(32) Vector_mask<std::int8_t, 32> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 32;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        using primitive = __mmask32;
        #elif defined(AVEL_AVX)
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
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            content(convert<Vector_mask>(v)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content(b ? 0xFFFFFFFF : 0x00000000) {}
        #elif defined(AVEL_AVX2)
            content(b ? _mm256_set1_epi8(-1) : _mm256_setzero_si256()) {}
        #endif

        AVEL_FINL explicit Vector_mask(const std::array<bool, 32>& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()));
            content = primitive(_mm256_cmplt_epu8_mask(_mm256_setzero_si256(), array_data));

            #elif defined(AVEL_AVX2)
            primitive array_data = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()));
            content = _mm256_cmpgt_epi8(array_data, _mm256_setzero_si256());

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
            content = b ? std::uint32_t(-1) : std::uint32_t(0);

            #elif defined(AVEL_AVX2)
            content = b ? _mm256_set1_epi8(-1) : _mm256_setzero_si256();

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
            auto tmp = _kxor_mask32(decay(lhs), decay(rhs));
            return _kortestz_mask32_u8(tmp, tmp);

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(decay(lhs)) == _mm256_movemask_epi8(decay(rhs));

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto tmp = _kxor_mask32(decay(lhs), decay(rhs));
            return !_kortestz_mask32_u8(tmp, tmp);

            #elif defined(AVEL_AVX2)
            return _mm256_movemask_epi8(decay(lhs)) != _mm256_movemask_epi8(decay(rhs));

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content &= decay(rhs);

            #elif defined(AVEL_SSE2)
            content = _mm256_and_si256(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content |= decay(rhs);

            #elif defined(AVEL_SSE2)
            content = _mm256_or_si256(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content ^= decay(rhs);

            #elif defined(AVEL_SSE2)
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

        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask32x8u, 1> convert<mask32x8u, mask32x8i>(mask32x8i m) {
        return {mask32x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask32x8i, 1> convert<mask32x8i, mask32x8u>(mask32x8u m) {
        return {mask32x8i(decay(m))};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask32x8i m) {
        return count(mask32x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask32x8i m) {
        return any(mask32x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask32x8i m) {
        return all(mask32x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask32x8i m) {
        return none(mask32x8u{m});
    }






    template<>
    class alignas(32) Vector<std::int8_t, 32> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 32;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int8_t;

        #if defined(AVEL_AVX)
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
        AVEL_FINL explicit Vector(Vector<U, width> v):
            content(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content(_mm256_maskz_set1_epi8(decay(m), 0x1)) {}
        #elif defined(AVEL_SSE2)
            content(_mm256_sub_epi8(_mm256_setzero_si256(), decay(m))) {}
        #endif

        AVEL_FINL Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm256_set1_epi8(x)) {}

        AVEL_FINL explicit Vector(const arr32x8i& arr):
            content(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()))) {}

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
            return mask{_mm256_cmpeq_epi8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpeq_epi8(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmpneq_epi8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpeq_epi8(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmplt_epi8_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmpgt_epi8(rhs.content, lhs.content)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmple_epi8_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_AVX2)
            return !mask{_mm256_cmpgt_epi8(lhs.content, rhs.content)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmpgt_epi8_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            return mask{_mm256_cmpgt_epi8(lhs.content, rhs.content)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm256_cmpge_epi8_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            return !mask{_mm256_cmpgt_epi8(rhs.content, lhs.content)};

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
            content = _mm256_add_epi8(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm256_sub_epi8(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto a = _mm512_cvtepu8_epi16(content);
            auto b = _mm512_cvtepu8_epi16(rhs.content);

            auto c = _mm512_mullo_epi16(a, b);
            content = _mm512_cvtepi16_epi8(c);

            #elif defined(AVEL_AVX2)
            auto even_mask = _mm256_set1_epi16(0x00FF);

            auto even_product = _mm256_and_si256(even_mask, _mm256_mullo_epi16(content, decay(rhs)));
            auto odd_product  = _mm256_slli_epi16(
                _mm256_mullo_epi16(
                    _mm256_srli_epi16(content, 0x8),
                    _mm256_srli_epi16(decay(rhs), 0x8)
                ),
                0x8
            );

            auto product = _mm256_or_si256(even_product, odd_product);
            content = product;

            /* //Old bugged implementation
            auto byte_mask = _mm256_set1_epi16(0x00FF);

            auto lhs_even = _mm256_and_si256(byte_mask, content);
            auto lhs_odd  = _mm256_andnot_si256(byte_mask, content);

            auto rhs_even = _mm256_and_si256(byte_mask, decay(rhs));
            auto rhs_odd  = _mm256_andnot_si256(byte_mask, decay(rhs));

            auto product_even = _mm256_mullo_epi16(lhs_even, rhs_even);
            auto product_odd  = _mm256_mullo_epi16(lhs_odd,  rhs_odd );
            product_odd = _mm256_slli_epi16(product_odd, 0x8);

            content = _mm256_blendv_epi8(product_odd, product_even, byte_mask);
            */
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
            content = _mm256_and_si256(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            content = _mm256_or_si256(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            content = _mm256_xor_si256(content, decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto widened = _mm512_cvtepi8_epi16(content);
            auto shifted = _mm512_sll_epi16(widened, _mm_cvtsi32_si128(rhs));
            content = _mm512_cvtepi16_epi8(shifted);

            #elif defined(AVEL_AVX2)
            auto s = _mm_cvtsi32_si128(rhs);

            auto shifted = _mm256_sll_epi16(content, s);
            auto mask = _mm256_set1_epi8(std::uint8_t(0xFF << rhs));
            content = _mm256_and_si256(mask, shifted);

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto tmp = _mm512_cvtepi8_epi16(content);
            tmp = _mm512_sra_epi16(tmp, _mm_cvtsi32_si128(rhs));
            content = _mm512_cvtepi16_epi8(tmp);

            #elif defined(AVEL_SSE2)
            auto sign_bits = _mm256_cmpgt_epi8(_mm256_setzero_si256(), content);
            auto lo = _mm256_unpacklo_epi8(content, sign_bits);
            auto hi = _mm256_unpackhi_epi8(content, sign_bits);

            auto s = _mm_cvtsi32_si128(rhs);
            auto lo_shifted = _mm256_sra_epi16(lo, s);
            auto hi_shifted = _mm256_sra_epi16(hi, s);

            auto result = _mm256_packs_epi16(lo_shifted, hi_shifted);
            content = result;

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto lhs_widened = _mm512_cvtepu8_epi16(content);
            auto rhs_widened = _mm512_cvtepu8_epi16(decay(rhs));

            auto shifted = _mm512_sllv_epi16(lhs_widened, rhs_widened);
            auto narrowed = _mm512_cvtepi16_epi8(shifted);

            content = narrowed;

            #elif defined(AVEL_AVX2)
            // 16-bit multiplication approach
            alignas(16) static constexpr std::uint8_t table_data[16] {
                0x01, 0x02, 0x04, 0x08,
                0x10, 0x20, 0x40, 0x80,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00
            };

            auto zeros = _mm256_setzero_si256();
            auto table = _mm256_broadcastsi128_si256(_mm_load_si128(reinterpret_cast<const __m128i*>(table_data)));

            auto lhs_lo = _mm256_unpacklo_epi8(content, zeros);
            auto lhs_hi = _mm256_unpackhi_epi8(content, zeros);

            auto valid_mask = _mm256_cmpgt_epi8(decay(rhs), _mm256_set1_epi8(0x7));
            auto indices = _mm256_or_si256(decay(rhs), valid_mask);
            auto shifts = _mm256_shuffle_epi8(table, indices);

            auto rhs_lo = _mm256_unpacklo_epi8(shifts, zeros);
            auto rhs_hi = _mm256_unpackhi_epi8(shifts, zeros);

            lhs_lo = _mm256_mullo_epi16(lhs_lo, rhs_lo);
            lhs_hi = _mm256_mullo_epi16(lhs_hi, rhs_hi);

            auto byte_mask = _mm256_set1_epi16(0xFF);
            lhs_lo = _mm256_and_si256(lhs_lo, byte_mask);
            lhs_hi = _mm256_and_si256(lhs_hi, byte_mask);

            content = _mm256_packus_epi16(lhs_lo, lhs_hi);

            /*
            // Divide and conquer approach

            auto zeros = _mm256_setzero_si256();

            auto a = _mm256_cmpeq_epi8(_mm256_and_si256(decay(rhs), _mm256_set1_epi8(0x01)), zeros);
            content = _mm256_add_epi8(content, _mm256_andnot_si256(a, content));

            auto b = _mm256_cmpeq_epi8(_mm256_and_si256(decay(rhs), _mm256_set1_epi8(0x02)), zeros);
            content = _mm256_add_epi8(content, _mm256_andnot_si256(b, content));
            content = _mm256_add_epi8(content, _mm256_andnot_si256(b, content));

            auto c = _mm256_cmpeq_epi8(_mm256_and_si256(decay(rhs), _mm256_set1_epi8(0x04)), zeros);
            auto tmp = _mm256_and_si256(_mm256_slli_epi16(content, 4), _mm256_set1_epi16(0xF0FF));
            content = _mm256_blendv_epi8(tmp, content, c);

            content = _mm256_andnot_si256(_mm256_cmpgt_epi8(decay(rhs), _mm256_set1_epi8(0x7)), content);
            */

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto whole = _mm512_cvtepi8_epi16(content);
            auto shifts = _mm512_cvtepi8_epi16(rhs.content);
            auto shifted = _mm512_srav_epi16(whole, shifts);
            content = _mm512_cvtepi16_epi8(shifted);

            #elif defined(AVEL_AVX2)
            auto zeros = _mm256_setzero_si256();
            auto non_zero_mask = _mm256_cmpgt_epi8(_mm256_set1_epi8(8), decay(rhs));
            auto neg_mask = _mm256_cmpgt_epi8(zeros, content);
            auto preserve_mask = _mm256_set1_epi16(0x00FF);

            for (unsigned i = 0; i < 3; ++i) {
                auto t0 = _mm256_and_si256(decay(rhs), _mm256_set1_epi8(1u << i));
                auto m = _mm256_cmpgt_epi8(t0, zeros);

                auto s = _mm_cvtsi64_si128(1u << i);
                preserve_mask = _mm256_blendv_epi8(preserve_mask, _mm256_srl_epi16(preserve_mask, s), m);
                content = _mm256_blendv_epi8(content, _mm256_sra_epi16(content, s), m);
            }

            preserve_mask = _mm256_or_si256(preserve_mask, _mm256_set1_epi16(0xFF00));
            preserve_mask = _mm256_and_si256(non_zero_mask, preserve_mask);

            auto invert_mask = _mm256_xor_si256(content, neg_mask);
            auto flip_mask = _mm256_andnot_si256(preserve_mask, invert_mask);

            content = _mm256_xor_si256(content, flip_mask);
            content = _mm256_and_si256(content, _mm256_or_si256(non_zero_mask, neg_mask));

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
            return mask{_mm256_test_epi8_mask(content, content)};

            #elif defined(AVEL_AVX2)
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        32 * sizeof(std::int8_t) == sizeof(vec32x8i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec32x8u, 1> convert<vec32x8u, vec32x8i>(vec32x8i x) {
        return {vec32x8u{decay(x)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec32x8i, 1> convert<vec32x8i, vec32x8u>(vec32x8u x) {
        return {vec32x8i{decay(x)}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec32x8i operator-(vec32x8u v) {
        return vec32x8i{0x00} - vec32x8i{v};
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL std::int8_t extract(vec32x8i v) {
        static_assert(N <= 32, "Specified index does not exist");
        typename std::enable_if<N <= 32, int>::type dummy_variable = 0;

        return static_cast<std::int8_t>(extract<N>(vec32x8u{v}));
    }

    template<std::uint32_t N>
    AVEL_FINL vec32x8i insert(vec32x8i v, std::int8_t x) {
        static_assert(N <= 32, "Specified index does not exist");
        typename std::enable_if<N <= 32, int>::type dummy_variable = 0;

        return vec32x8i{insert<N>(vec32x8u{v}, static_cast<std::uint8_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec32x8i bit_shift_left(vec32x8i v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        return vec32x8i{bit_shift_left<S>(vec32x8u{v})};
    }



    template<std::uint32_t S>
    AVEL_FINL vec32x8i bit_shift_right(vec32x8i v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto widened = _mm512_cvtepi8_epi16(decay(v));
        widened = _mm512_srai_epi16(widened, S);

        return vec32x8i{_mm512_cvtepi16_epi8(widened)};

        #elif defined(AVEL_SSE2)
        auto lo = _mm256_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm256_unpackhi_epi8(decay(v), decay(v));

        lo = _mm256_srai_epi16(lo, S + 8);
        hi = _mm256_srai_epi16(hi, S + 8);

        return vec32x8i{_mm256_packs_epi16(lo, hi)};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8i bit_shift_right<0>(vec32x8i v) {
        return v;
    }

    #if defined(AVEL_SSE2)
    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8i bit_shift_right<7>(vec32x8i v) {
        return vec32x8i{_mm256_cmpgt_epi8(_mm256_setzero_si256(), decay(v))};
    }
    #endif

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8i bit_shift_right<8>(vec32x8i v) {
        return broadcast_mask(v < vec32x8i{0x00});
    }





    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec32x8i rotl(vec32x8i v) {
        return vec32x8i{rotl<S>(vec32x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i rotl(vec32x8i v, long long s) {
        return vec32x8i{rotl(vec32x8u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i rotl(vec32x8i v, vec32x8i s) {
        return vec32x8i{rotl(vec32x8u{v}, vec32x8u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec32x8i rotr(vec32x8i v) {
        return vec32x8i{rotr<S>(vec32x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i rotr(vec32x8i v, long long s) {
        return vec32x8i{rotr(vec32x8u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i rotr(vec32x8i v, vec32x8i s) {
        return vec32x8i{rotr(vec32x8u{v}, vec32x8u{s})};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec32x8i x) {
        return count(vec32x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec32x8i x) {
        return any(vec32x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec32x8i x) {
        return all(vec32x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec32x8i x) {
        return none(vec32x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i broadcast_mask(mask32x8i m) {
        return vec32x8i{broadcast_mask(mask32x8u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i keep(mask32x8i m, vec32x8i v) {
        return vec32x8i{keep(mask32x8u{m}, vec32x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i clear(mask32x8i m, vec32x8i v) {
        return vec32x8i{clear(mask32x8u{m}, vec32x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i blend(mask32x8i m, vec32x8i a, vec32x8i b) {
        return vec32x8i{blend(mask32x8u{m}, vec32x8u{a}, vec32x8u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i max(vec32x8i a, vec32x8i b) {
        return vec32x8i{_mm256_max_epi8(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i min(vec32x8i a, vec32x8i b) {
        return vec32x8i{_mm256_min_epi8(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec32x8i, 2> minmax(vec32x8i a, vec32x8i b) {
        return {
            vec32x8i{_mm256_min_epi8(decay(a), decay(b))},
            vec32x8i{_mm256_max_epi8(decay(a), decay(b))}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i clamp(vec32x8i x, vec32x8i lo, vec32x8i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i average(vec32x8i a, vec32x8i b) {
        auto tmp = (a ^ b);
        auto avg = (a & b) + (tmp >> 1);
        auto c = broadcast_mask((a < -b) | (b == vec32x8i(0x80))) & tmp & vec32x8i{0x01};
        return avg + c;
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i midpoint(vec32x8i a, vec32x8i b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto avg = _mm256_avg_epu8(decay(a), decay(b));
        auto sign_correction = _mm256_ternarylogic_epi32(decay(a), decay(b), _mm256_set1_epi8(0x80), 0x28);
        auto average = _mm256_xor_si256(avg, sign_correction);

        auto bias = _mm256_ternarylogic_epi32(decay(a), decay(b), _mm256_set1_epi8(0x1), 0x28);
        auto mask = _mm256_cmplt_epi8_mask(decay(a), decay(b));
        auto ret = _mm256_mask_sub_epi8(average, mask, average, bias);

        return vec32x8i{ret};

        #elif defined(AVEL_AVX2)
        auto offset = _mm256_set1_epi8(0x80);

        auto a_offset = _mm256_xor_si256(decay(a), offset);
        auto b_offset = _mm256_xor_si256(decay(b), offset);

        auto average_offset = _mm256_avg_epu8(a_offset, b_offset);
        auto average = _mm256_xor_si256(average_offset, offset);

        auto m = _mm256_cmpgt_epi8(decay(b), decay(a));
        auto bias = _mm256_and_si256(_mm256_xor_si256(decay(a), decay(b)), _mm256_and_si256(m, _mm256_set1_epi8(0x01)));

        return vec32x8i{_mm256_sub_epi8(average, bias)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i negate(mask32x8i m, vec32x8i x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec32x8i{_mm256_mask_sub_epi8(decay(x), decay(m), _mm256_setzero_si256(), decay(x))};

        #elif defined(AVEL_SSSE3)
        return vec32x8i{_mm256_sign_epi8(decay(x), _mm256_or_si256(decay(m), _mm256_set1_epi8(0x01)))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i abs(vec32x8i v) {
        return vec32x8i{_mm256_abs_epi8(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i neg_abs(vec32x8i v) {
        return -vec32x8i{_mm256_abs_epi8(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i neg_abs(vec32x8u v) {
        return neg_abs(vec32x8i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8i load<vec32x8i>(const std::int8_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = (n >= 32) ? std::uint32_t(-1) : (1 << n) - 1;
        return vec32x8i{_mm256_maskz_loadu_epi8(mask, ptr)};

        #elif defined(AVEL_AVX2)

        if (n == 32) {
            return vec32x8i{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
        }

        if (n <= 16) {
            auto low_half = decay(load<vec16x8i>(ptr, n));
            // _mm256_zextsi128_si256 isn't recognized by GCC 9
            return vec32x8i{_mm256_set_m128i(_mm_setzero_si128(), low_half)};
        } else {
            auto low_half = _mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr));
            auto high_half = decay(load<vec16x8i>(ptr + 16, n - 16));
            return vec32x8i{_mm256_set_m128i(high_half, low_half)};
        }

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8i load<vec32x8i, vec32x8i::width>(const std::int8_t* ptr) {
        return vec32x8i{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8i aligned_load<vec32x8i>(const std::int8_t* ptr, std::uint32_t n) {
        return load<vec32x8i>(ptr, n);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec32x8i aligned_load<vec32x8i, vec32x8i::width>(const std::int8_t* ptr) {
        return vec32x8i{_mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))};
    }



    AVEL_FINL void store(std::int8_t* ptr, vec32x8i v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = (n >= 32) ? vec32x8i::width : (1 << n) - 1;
        _mm256_mask_storeu_epi8(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        if (n == 32) {
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(v));
        } else if (n <= 16) {
            vec16x8i lo{_mm256_extracti128_si256(decay(v), 0x0)};
            store(ptr, lo, n);
        } else {
            vec16x8i lo{_mm256_extracti128_si256(decay(v), 0x0)};
            vec16x8i hi{_mm256_extracti128_si256(decay(v), 0x1)};

            store(ptr + 0x00, lo);
            store(ptr + 0x10, hi, n - 16);
        }

        #endif
    }

    template<std::uint32_t N = vec32x8i::width>
    AVEL_FINL void store(std::int8_t* ptr, vec32x8i x) {
        static_assert(N < vec32x8i::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N < vec32x8i::width, int>::type dummy_variable = 0;

        store(ptr, x, N);
    }

    template<>
    AVEL_FINL void store<vec32x8i::width>(std::int8_t* ptr, vec32x8i x) {
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), decay(x));
    }


    AVEL_FINL void aligned_store(std::int8_t* ptr, vec32x8i x, std::uint32_t n) {
        store(ptr, x, n);
    }

    template<std::uint32_t N = vec32x8i::width>
    AVEL_FINL void aligned_store(std::int8_t* ptr, vec32x8i x) {
        static_assert(N < vec32x8i::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N < vec32x8i::width, int>::type dummy_variable = 0;

        aligned_store(ptr, x, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec32x8i::width>(std::int8_t* ptr, vec32x8i x) {
        _mm256_store_si256(reinterpret_cast<__m256i*>(ptr), decay(x));
    }



    [[nodiscard]]
    AVEL_FINL std::array<std::int8_t, 32> to_array(vec32x8i v) {
        alignas(32) arr32x8i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec32x8i> div(vec32x8i x, vec32x8i y) {
        mask32x8i remainder_sign_mask = (x < vec32x8i{0x00});
        mask32x8i quotient_sign_mask = remainder_sign_mask ^ (y < vec32x8i{0x00});

        vec32x8u numerator{abs(x)};
        vec32x8u denominator{abs(y)};

        auto results = div(numerator, denominator);

        return {
            negate(quotient_sign_mask,  vec32x8i{results.quot}),
            negate(remainder_sign_mask, vec32x8i{results.rem})
        };
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i popcount(vec32x8i v) {
        return vec32x8i{popcount(vec32x8u(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i byteswap(vec32x8i v) {
        return v;
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i countl_zero(vec32x8i v) {
        return vec32x8i{countl_zero(vec32x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i countl_one(vec32x8i v) {
        return vec32x8i{countl_one(vec32x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i countr_zero(vec32x8i v) {
        return vec32x8i{countr_zero(vec32x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec32x8i countr_one(vec32x8i v) {
        return vec32x8i{countr_one(vec32x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL mask32x8i has_single_bit(vec32x8i v) {
        return mask32x8i{has_single_bit(vec32x8u{v})};
    }

}

#endif //AVEL_VEC32x8I_HPP
