#ifndef AVEL_VEC4X32U_HPP
#define AVEL_VEC4X32U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x32u = Vector<std::uint32_t, 4>;

    using mask4x32u = Vector_mask<std::uint32_t, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec4x32u> div(vec4x32u numerator, vec4x32u denominator);

    vec4x32u broadcast_bits(mask4x32u m);

    vec4x32u blend(vec4x32u a, vec4x32u b, mask4x32u m);

    vec4x32u countl_one(vec4x32u x);





    template<>
    class alignas(AVEL_MASK_ALIGNMENT_4X32U) Vector_mask<std::uint32_t, 4> {
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
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        //=================================================
        // -ctors
        //=================================================

        AVEL_FINL explicit Vector_mask(Vector_mask<std::int32_t, 4>);

        AVEL_FINL explicit Vector_mask(Vector_mask<float, 4>);

        AVEL_FINL Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 4>& arr) {
            #if defined(AVEL_AVX512VL)

            std::uint32_t array_data = avel::bit_cast<std::uint32_t>(arr);

            //Assumes that the bitwise representation of true has the low bit
            //set, an assumption that holds when targeting the Itanium ABI
            std::uint32_t bit_mask = 0x01010101;

            unsigned t = _pext_u32(array_data, bit_mask);

            content = _mm512_int2mask(t);

            #elif defined(AVEL_SSE2)
            static_assert(sizeof(bool) == 1);

            alignas(16) static const std::uint32_t mask_data[4] {
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000
            };

            //Issue here with older versions of GCC
            primitive array_data = _mm_loadu_si32(arr.data());
            #if defined(AVEL_AVX2)
            array_data = _mm_broadcastd_epi32(array_data);
            #else
            array_data = _mm_shuffle_epi32(array_data, 0x00);
            #endif

            primitive mask = _mm_load_si128((const __m128i*)mask_data);

            primitive t2 = _mm_and_si128(array_data, mask);

            // The two comparisons are used because it works without
            // assuming a particular representation of how true is
            // represented, and instead only assumes that all zeros indicates
            // false.
            content = _mm_or_si128(
                _mm_cmpgt_epi32(t2, _mm_setzero_si128()),
                _mm_cmplt_epi32(t2, _mm_setzero_si128())
            );
            #endif
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator=(bool x) {
            content = from_bool(x);
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
            return (_mm512_mask2int(lhs) == _mm512_mask2int(rhs));
            #elif defined(AVEL_SSE2)
            return (0xFFFF == _mm_movemask_epi8(_mm_cmpeq_epi32(lhs, rhs)));
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return (_mm512_mask2int(lhs) != _mm512_mask2int(rhs));
            #elif defined(AVEL_SSE2)
            return (0xFFFF != _mm_movemask_epi8(_mm_cmpeq_epi32(lhs, rhs)));
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs);
            #elif defined(AVEL_NEON)
            content = vandq_u32(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs);
            #elif defined(AVEL_NEON)
            content = vorrq_u32(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs);
            #elif defined(AVEL_NEON)
            content = veorq_u32(content, rhs);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, 0x0F)};
            #elif defined(AVEL_AVX512F)
            return Vector_mask{_mm_ternarylogic_epi32(content, content, content, 0x01)};
            #elif defined(AVEL_SSE2)
            primitive tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};
            #elif defined(AVEL_NEON)
            return Vector_mask{vmvnq_u32(content)};
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

        AVEL_FINL static primitive from_bool(bool x) {
            #if defined(AVEL_AVX512VL)
            return x ? 0xF :0x0;

            #elif defined(AVEL_SSE2)
            static const primitive full_masks[2] {
                {0,  0},
                {-1, -1}
            };

            return full_masks[x];

            #elif defined(AVEL_NEON)
            //TODO: Consider alternative implementations
            std::uint32_t y{x};
            primitive z{};

            return vsubq_u32(veorq_u32(z, z), vdupq_n_u32(y));
            #endif
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask4x32u m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(m));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(m)) / sizeof(std::uint32_t);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x32u m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x32u m) {
        #if defined(AVEL_AVX512VL)
        return 0xF == _mm512_mask2int(m);
        #elif defined(AVEL_SSE41)
        return _mm_test_all_ones(m);
        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x32u m) {
        #if defined(AVEL_AVX512VL)
        return !all(m);
        #elif defined(AVEL_SSE41)
        return _mm_test_all_zeros(m, m);
        #elif defined(AVEL_SSE2)
        return !all(m);
        #endif
    }





    template<>
    class alignas(16) Vector<std::uint32_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint32_t;

        using primitive = __m128i;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<unsigned M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // -ctors
        //=================================================

        AVEL_FINL explicit Vector(Vector<std::int32_t, width> v);

        AVEL_FINL explicit Vector(Vector<float, width> v);

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm_mask_blend_epi32(m, _mm_setzero_si128(), _mm_set1_epi32(1))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi32(_mm_setzero_si128(), m)) {}
        #elif defined(AVEL_NEON)
            content(vsubq_u32(zeros(), m)) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            content(_mm_broadcastd_epi32(_mm_cvtsi32_si128(x))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_set1_epi32(x)) {}
        #elif defined(AVEL_NEON)
            content(vdupq_n_u32(x)) {}
        #endif

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            Vector(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        AVEL_FINL Vector& operator=(primitive p) {
            this->content = p;
            return *this;
        }

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_AVX2)
            content = _mm_broadcastd_epi32(_mm_cvtsi32_si128(x));
            #elif defined(AVEL_SSE2)
            content = _mm_shuffle_epi32(_mm_cvtsi32_si128(x), 0x00);
            #elif defined(AVEL_NEON)
            content = vdupq_n_u32(x);
            #endif
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi32(lhs, rhs)};
            #elif defined(AVEL_NEON)
            return mask{vceqq_u32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epu32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return !(lhs == rhs);
            #elif defined(AVEL_NEON)
            return mask{vtstq_u32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmplt_epu32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            Vector add_mask{0x80000000};
            return mask{_mm_cmpgt_epi32(rhs ^ add_mask, lhs ^ add_mask)};
            #elif defined(AVEL_NEON)
            return mask{vcleq_u32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epu32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return !mask{lhs > rhs};
            #elif defined(AVEL_NEON)
            return mask{vcleq_u32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epu32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_epi32(lhs, rhs)};
            #elif defined(AVEL_NEON)
            return mask{vcgtq_u32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epu32_mask(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            return !mask{lhs < rhs};
            #elif defined(AVEL_NEON)
            return mask{vcgeq_u32(lhs, rhs)};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() {
            return *this;
        }

        //Definition of operator-() deferred until after definition of vec4x32i

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi32(content, rhs);
            #elif defined(AVEL_NEON)
            content = vaddq_u32(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi32(content, rhs);
            #elif defined(AVEL_NEON)
            content = vsubq_u32(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_SSE41)
            content = _mm_mullo_epi32(content, rhs);
            #elif defined(AVEL_SSE2)
            primitive hi_lhs = _mm_bsrli_si128(content, 4);
            primitive hi_rhs = _mm_bsrli_si128(rhs, 4);

            primitive lo = _mm_mul_epu32(content, rhs);
            primitive hi = _mm_mul_epu32(hi_lhs, hi_rhs);

            primitive t0 = _mm_unpacklo_epi32(lo, hi);
            primitive t1 = _mm_unpackhi_epi32(lo, hi);
            primitive t2 = _mm_unpacklo_epi64(t0, t1);

            content = t2;
            #elif defined(AVEL_NEON)
            content = vmulq_u32(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.quot;
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector rhs) {
            auto results = div(*this, rhs);
            content = results.rem;
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
            #if defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs);
            #elif defined(AVEL_NEON)
            content = vandq_u32(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs);
            #elif defined(AVEL_NEON)
            content = vorrq_u32(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs);
            #elif defined(AVEL_NEON)
            content = veorq_u32(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(std::uint32_t rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi32(content, _mm_cvtsi32_si128(rhs));

            #elif defined(AVEL_NEON)
            content = vshlq_u32(content, vdupq_n_s32(static_cast<std::int32_t>(rhs)));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_srl_epi32(content, _mm_cvtsi32_si128(rhs));

            #elif defined(AVEL_NEON)
            content = vsraq_n_u32(content, vdupq_n_u32(rhs), 0x00);

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi32(content, rhs);
            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i  < 5; ++i) {
                auto t0 = _mm_and_si128(rhs, _mm_set1_epi32(1u << i));
                auto m = _mm_cmplt_epi32(__m128i{}, t0);
                content = blend(*this, *this << (1u << i), mask(m));
            }

            content = _mm_and_si128(content, _mm_cmplt_epi32(rhs, _mm_set1_epi32(32u)));
            #elif defined(AVEL_NEON)
            content = vshlq_u32(content, vreinterpretq_s32_u32(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_srlv_epi32(content, rhs);
            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i  < 5; ++i) {
                auto t0 = _mm_and_si128(rhs, _mm_set1_epi32(1u << i));
                auto m = _mm_cmplt_epi32(__m128i{}, t0);
                *this = blend(*this, *this >> (1u << i), m);
            }

            content = _mm_and_si128(content, _mm_cmplt_epi32(rhs, _mm_set1_epi32(32u)));

            #elif defined(AVEL_NEON)
            content = vsraq_n_u32(content, rhs, 0x00);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512F)
            return Vector{_mm_ternarylogic_epi32(content, content, content, 0x01)};
            #elif defined(AVEL_SSE2)
            primitive t = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi32(t, t))};
            #elif defined(AVEL_NEON)
            return Vector{vmvnq_u32(content)};
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
        AVEL_FINL operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return Vector{} < *this;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    //=====================================================
    // General vector operations
    //=====================================================

    /*
    template<unsigned O>
    [[nodiscard]]
    AVEL_FINL vec4x32u funnel_shift(vec4x32u a, vec4x32u b) {
        static_assert(O < 4, "");

        #if defined(AVEL_SSSE3)
        return vec4x32u{_mm_alignr_epi8(a, b, 4 * O)};
        #elif defined(AVEL_SSE2)
        return vec4x32u{_mm_or_si128(_mm_bslli_si128(a, 4 * (4 - O)), _mm_bsrli_si128(b, O))};
        #endif
    }
    */

    [[nodiscard]]
    AVEL_FINL vec4x32u broadcast_bits(mask4x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec4x32u{_mm_movm_epi32(m)};
        #elif defined(AVEL_AVX512VL)
        const auto x = _mm512_set1_epi32(0);
        const auto y = _mm512_set1_epi32(-1);
        return vec4x32u{_mm512_mask_blend_epi32(m, x, y)};
        #elif defined(AVEL_SSE2)
        return vec4x32u{vec4x32u::primitive(m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u blend(vec4x32u a, vec4x32u b, mask4x32u m) {
        #if defined(AVEL_AVX512VL)

        #if defined(AVEL_GCC)
        return vec4x32u{
            _mm_mask_blend_epi32(
                m,
                vec4x32u::primitive(a),
                vec4x32u::primitive(b)
            )
        };

        #else
        return vec4x32u{ _mm_mask_blend_epi32(m, a, b)};

        #endif

        #elif defined(AVEL_SSE41)
        return vec4x32u{_mm_blendv_epi8(a, b, m)};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(m, a);
        auto y = _mm_and_si128(m, b);
        return vec4x32u{_mm_or_si128(x, y)};

        #elif defined(AVEL_NEON)
        auto x = vandq_u32(b, m);
        auto y = vornq_u32(m, a);
        return vec4x32u{vornq_u32(x, y)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u max(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_max_epu32(a, b)};
        #elif defined(AVEL_SSE2)
        return blend(a, b, a < b);
        #elif defined(AVEL_NEON)
        return vec4x32u{vmaxq_u32(a, b)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u min( vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_min_epu32(a, b)};
        #elif defined(AVEL_SSE2)
        return blend(a, b, b < a);
        #elif defined(AVEL_NEON)
        return vec4x32u{vminq_u32(a, b)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::pair<vec4x32u, vec4x32u> minmax(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return {
            vec4x32u{_mm_min_epu32(a, b)},
            vec4x32u{_mm_max_epu32(a, b)}
        };
        #elif defined(AVEL_SSE2)
        auto mask = b < a;
        return {blend(a, b, mask), blend(a, b, !mask)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u clamp(vec4x32u x, vec4x32u lo, vec4x32u hi) {
        return vec4x32u{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u midpoint(vec4x32u a, vec4x32u b) {
        vec4x32u t0 = a & b & vec4x32u{0x1};
        vec4x32u t1 = (a | b) & vec4x32u{0x1} & broadcast_bits(a > b);
        vec4x32u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u average(vec4x32u a, vec4x32u b) {
        return (a >> 1) + (b >> 1) + (a & b & vec4x32u{0x1});
    }

    //Definition of neg_abs delayed until vec4x32i is defined

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u load<vec4x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #elif defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u aligned_load<vec4x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #elif defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u stream_load<vec4x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE41)
        //Casting away of const required by intrinsic
        return vec4x32u{_mm_stream_load_si128((__m128i*)ptr)};
        #elif defined(AVEL_SSE2)
        return aligned_load<vec4x32u>(ptr);
        #elif defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};
        #endif
    }

    //Definition of gather deferred until vector of signed integers is defined

    AVEL_FINL void store(std::uint32_t* ptr, vec4x32u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), v);
        #endif
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec4x32u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), v);
        #elif defined(AVEL_NEON)
        vst1q_u32(ptr, v);
        #endif
    }

    AVEL_FINL void stream_store(std::uint32_t* ptr, vec4x32u v) {
        #if defined(AVEL_SSE2)
        _mm_stream_si128(reinterpret_cast<__m128i*>(ptr), v);
        #elif defined(AVEL_NEON)
        vst1q_u32(ptr, v);
        #endif
    }

    //Definition of scatter deferred until vector of signed integers is defined

    [[nodiscard]]
    AVEL_FINL std::array<std::uint32_t, 4> to_array(vec4x32u v) {
        alignas(16) std::array<std::uint32_t, 4> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer vector operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec4x32u> div(vec4x32u numerator, vec4x32u denominator) {
        vec4x32u quotient{};

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto t0 = _mm_lzcnt_epi32(denominator);
        auto t1 = _mm_lzcnt_epi32(numerator);
        auto t2 = _mm_subs_epu16(t0, t1);
        auto t3 = _mm_add_epi32(t2, _mm_set1_epi32(1));

        auto s0 = t3;
        auto s1 = _mm_bsrli_si128(s0, 8);
        auto s2 = _mm_max_epi32(s0, s1);
        auto s3 = _mm_bsrli_si128(s2, 4);
        auto s4 = _mm_max_epi32(s2, s3);

        std::int32_t i = _mm_cvtsi128_si32(s4);

        //TODO: Optimize case were a denominator is zero?
        #else

        //16-bit max/min suffice since all that matters is keeping the leading
        //bit in each 32-bit int set

        auto d0 = denominator;
        auto d1 = _mm_bsrli_si128(d0, 8);
        auto d2 = _mm_min_epi16(d0, d1);
        auto d3 = _mm_bsrli_si128(d2, 4);
        auto d4 = _mm_min_epi16(d2, d3);

        auto n0 = numerator;
        auto n1 = _mm_bsrli_si128(n0, 8);
        auto n2 = _mm_max_epi16(n0, n1);
        auto n3 = _mm_bsrli_si128(n2, 4);
        auto n4 = _mm_max_epi16(n2, n3);

        std::int32_t t0 = countl_zero(_mm_cvtsi128_si32(d4));
        std::int32_t t1 = countl_zero(_mm_cvtsi128_si32(n4));

        //TODO: Optimize case were a denominator is zero?
        std::int32_t i = std::max(t0 - t1 + 1, 0);
        #endif

        for (; (i-- > 0) && any(mask4x32u(numerator));) {
            mask4x32u b = ((numerator >> i) >= denominator);
            numerator -= (broadcast_bits(b) & (denominator << i));
            quotient |= (vec4x32u{b} << i);
        }

        return {quotient, numerator};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u popcount(vec4x32u v) {
        #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512VPOPCNTDQ)
        return vec4x32u{_mm_popcnt_epi32(v)};
        #elif defined(AVELAVX512VL) & defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm_popcnt_epi16(v);
        auto tmp1 = _mm_slli_epi32(tmp0, 16);

        auto tmp2 = _mm_add_epi32(tmp0, tmp1);

        return vec4x32u{_mm_srli_epi32(tmp2, 16)};
        #elif defined(AVEL_SSE2)
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec4x32u{0x55555555u});
        v = (v & vec4x32u{0x33333333u}) + ((v >> 2) & vec4x32u{0x33333333u});
        v = ((v + (v >> 4) & vec4x32u{0x0F0F0F0Fu}) * vec4x32u{0x01010101u}) >> 24;
        return v;
        #elif defined(AVEL_NEON)
        vec4x32u reg_abcd{vreinterpretq_u32_u8(vcntq_u8(vreinterpretq_u8_u32(v)))};
        vec4x32u reg_00ab{vshrq_n_u32(reg_abcd, 16)};
        vec4x32u reg_00xy{vaddq_u32(reg_abcd, reg_00ab)};

        vec4x32u reg_000x{vshrq_n_u32(reg_00xy, 8)};
        vec4x32u reg_000z{vaddq_u32(reg_000x, reg_00xy)};

        return reg_000z;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u byteswap(vec4x32u v) {
        #if defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t index_data[16] {
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12
        };

        auto indices = _mm_load_si128((const __m128i*)index_data);
        auto ret = vec4x32u{_mm_shuffle_epi8(v, indices)};
        return ret;

        #elif defined(AVEL_SSE2)
        alignas(16) static constexpr std::uint32_t mask_data[4]{
            0x00ff00ff,
            0x00ff00ff,
            0x00ff00ff,
            0x00ff00ff
        };

        auto t0 = _mm_shufflelo_epi16(v, 0xB1);
        auto t1 = _mm_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm_bslli_si128(t1, 1);
        auto t3 = _mm_bsrli_si128(t1, 1);
        auto t4 = _mm_load_si128(reinterpret_cast<const __m128i*>(mask_data));
        return blend(vec4x32u{t2}, vec4x32u{t3}, mask4x32u{t4});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countl_zero(vec4x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec4x32u{_mm_lzcnt_epi32(x)};
        #elif defined(AVEL_SSE2)
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        x = _mm_andnot_si128(x >> 1, x);
        auto floats = _mm_add_ps(_mm_cvtepi32_ps(x), _mm_set1_ps(0.5f));
        auto biased_exponents = (vec4x32u(_mm_castps_si128(floats)) >> 23);
        auto lzcnt = _mm_subs_epu16(vec4x32u{158}, biased_exponents);
        return vec4x32u{lzcnt};

        #else
        return countl_one(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countl_one(vec4x32u x) {
        #if defined(AVEl_AVX512VL) & defined(AVEL_AVX512CD)
        return vec4x32u{_mm_lzcnt_epi32(~x)};
        #elif defined(AVEL_SSE2)
        return countl_zero(~x);
        #else
        vec4x32u sum{x == vec4x32u{0xFFFFFFFF}};

        vec4x32u m0{0xFFFF0000u};
        mask4x32u b0 = (m0 & x) == m0;
        sum += broadcast_bits(b0) & vec4x32u{16};
        x <<= broadcast_bits(b0) * vec4x32u{16};

        vec4x32u m1{0xFF000000u};
        mask4x32u b1 = (m1 & x) == m1;
        sum += broadcast_bits(b1) & vec4x32u{8};
        x <<= broadcast_bits(b1) & vec4x32u{8};

        vec4x32u m2{0xF0000000u};
        mask4x32u b2 = (m2 & x) == m2;
        sum += broadcast_bits(b2) & vec4x32u{4};
        x <<= broadcast_bits(b2) & vec4x32u{4};

        vec4x32u m3{0xC0000000u};
        mask4x32u b3 = (m3 & x) == m3;
        sum += broadcast_bits(b3) & vec4x32u{2};
        x <<= broadcast_bits(b3) & vec4x32u{2};

        vec4x32u m4{0x80000000u};
        mask4x32u b4 = (m4 & x) == m4;
        sum += broadcast_bits(b3) & vec4x32u{1};

        return sum;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countr_zero(vec4x32u x) {
        auto y = (x & (vec4x32u{} - x));
        auto floats = _mm_cvtepi32_ps(y);
        auto biased_exponents = (vec4x32u(_mm_castps_si128(floats)) >> 23);
        biased_exponents = _mm_min_epi16(vec4x32u{158}, biased_exponents);
        auto tzcnt = biased_exponents - vec4x32u{127};
        tzcnt = blend(tzcnt, vec4x32u{32}, x == vec4x32u{});

        return vec4x32u{tzcnt};
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countr_one(vec4x32u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u bit_width(vec4x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec4x32u{32} - vec4x32u{_mm_lzcnt_epi32(x)};
        #elif defined(AVEL_SSE2)
        auto floats = _mm_cvtepi32_ps(x);
        auto biased_exponents = (vec4x32u(_mm_castps_si128(floats)) >> 23 & vec4x32u{0xFF});
        return vec4x32u{_mm_subs_epu16(vec4x32u{156}, biased_exponents)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u bit_floor(vec4x32u x) {
        #if defined(AVEL_AVX512CD) && defined(AVEL_AVX512VL)
        vec4x32u leading_zeros = countl_zero(x);
        mask4x32u zero_mask = (leading_zeros != vec4x32u{32});

        return (vec4x32u{zero_mask} << (vec4x32u{31} - leading_zeros));
        #elif defined(AVEL_SSE2)
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
    AVEL_FINL vec4x32u bit_ceil(vec4x32u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto tmp = (vec4x32u{32} - countl_zero(v - vec4x32u{1}));
        return vec4x32u{1} << tmp;
        #elif defined(AVEL_SSE2)
        //TODO: Optimize ?
        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        ++v;
        return v;
        #endif
    };

    [[nodiscard]]
    AVEL_FINL mask4x32u has_single_bit(vec4x32u v) {
        #if (defined(AVEL_AVX512VPOPCNTDQ) || defined(AVEL_AVX512BITALG)) && defined(AVEL_AVX512VL)
        return popcount(v) == vec4x32u{1};
        #else
        return mask4x32u{v} & !mask4x32u{v & (v - vec4x32u{1})};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotl(vec4x32u v, std::uint32_t s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rolv_epi32(v, vec4x32u{s})};
        #elif defined(AVEL_SSE2)
        s &= 0x1F;
        return (v << s) | (v >> (32 - s));
        #elif defined(AVEL_NEON)
        s &= 0x1F;
        return (v << S) | (v >> (32 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotl(vec4x32u v, vec4x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rolv_epi32(v, s)};
        #elif defined(AVEL_SSE2)
        s &= vec4x32u(0x1F);
        return (v << s) | (v >> (vec4x32u{32} - s));
        #elif defined(AVEL_NEON)
        s &= vec4x32u(0x1F);
        return (v << s) | (v >> (vec4x32u{32} - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotr(vec4x32u v, std::uint32_t s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rorv_epi32(v, vec4x32u{s})};
        #elif defined(AVEL_SSE2)
        s &= 0x1F;
        return (v >> s) | (v << (32 - s));
        #elif defined(AVEL_NEON)
        s &= vec4x32u(0x1F);
        return (v >> S) | (v << (32 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotr(vec4x32u v, vec4x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rorv_epi32(v, s)};

        #elif defined(AVEL_SSE2)
        s &= vec4x32u(0x1F);
        return (v >> s) | (v << (vec4x32u{32} - s));

        #elif defined(AVEL_NEON)
        s &= vec4x32u(0x1F);
        return (v >> s) | (v << (vec4x32u {32} - s));
        #endif
    }

}

#endif //AVEL_VEC4X32U_HPP
