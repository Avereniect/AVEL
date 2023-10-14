#ifndef AVEL_VEC4X32U_HPP
#define AVEL_VEC4X32U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x32u = Vector<std::uint32_t, 4>;
    using arr4x32u = std::array<std::uint32_t, 4>;
    using mask4x32u = Vector_mask<std::uint32_t, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec4x32u> div(vec4x32u x, vec4x32u y);
    vec4x32u set_bits(mask4x32u m);
    vec4x32u blend(mask4x32u m, vec4x32u a, vec4x32u b);
    vec4x32u countl_one(vec4x32u x);





    template<>
    class Vector_mask<std::uint32_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint32x4_t;
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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content(b ? 0xF : 0x00) {}
            #elif defined(AVEL_SSE2)
            content(b ? _mm_set1_epi32(-1) : _mm_setzero_si128()) {}
            #endif
            #if defined(AVEL_NEON)
            content(vmovq_n_u32(b ? -1 : 0)) {}
            #endif

        AVEL_FINL explicit Vector_mask(const arr4xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            content = __mmask8(_mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            auto expanded = _mm_cvtepi8_epi32(array_data);
            content = _mm_cmplt_epu32_mask(_mm_setzero_si128(), expanded);

            #elif defined(AVEL_SSE2)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            array_data = _mm_unpacklo_epi8(array_data, array_data);
            array_data = _mm_unpacklo_epi16(array_data, array_data);
            content = _mm_cmplt_epi32(_mm_setzero_si128(), array_data);

            #endif

            #if defined(AVEL_NEON)
            auto array_data = vsetq_lane_u32(
                bit_cast<std::uint32_t>(arr),
                vdupq_n_u32(0x00),
                0x00
            );

            auto zeros = vdupq_n_u8(0x00);
            auto t0 = vreinterpretq_u8_u32(array_data);
            auto mask0 = vcltq_u8(zeros, t0);
            auto mask1 = vzipq_u8(mask0, mask0).val[0];
            auto mask2 = vzipq_u8(mask1, mask1).val[0];

            content = vreinterpretq_u32_u8(mask2);
            #endif
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector_mask& operator=(bool b) {
            *this = Vector_mask{b};
            return *this;
        }

        Vector_mask& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u32(vceqq_u32(decay(lhs), decay(rhs)));
            return min == 0xFFFFFFFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u32(vceqq_u32(decay(lhs), decay(rhs)));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(vreinterpretq_u64_u32(decay(lhs)), vreinterpretq_u64_u32(decay(rhs)));
            auto shifted = vqshlq_n_u64(diff, 63);
            auto shiftedx4 = vreinterpretq_u32_u64(shifted);
            auto x = vgetq_lane_u32(shiftedx4, 0x1);
            auto y = vgetq_lane_u32(shiftedx4, 0x3);
            return (x | y) >> 31;

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content &= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = vandq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content |= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content = (content ^ rhs.content) & 0xF;

            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = veorq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return Vector_mask{primitive(content ^ 0x0F)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(undef, undef))};
            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vmvnq_u32(content)};
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
    AVEL_FINL std::uint32_t count(mask4x32u m) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return popcount(decay(m));

        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m))) / sizeof(std::uint32_t);

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vnegq_s32(vreinterpretq_s32_u32(decay(m)));
        return static_cast<std::uint32_t>(vaddvq_s32(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(decay(m))));
        auto t1 = vpadd_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpadd_u32(t1, t1);

        return static_cast<std::uint32_t>(vget_lane_u32(t2, 0));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x32u m) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return !_kortestz_mask16_u8(decay(m), decay(m));

        #elif defined(AVEL_SSE41)
        return !_mm_test_all_zeros(decay(m), decay(m));

        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vmaxvq_u32(decay(m)) != 0x00;

        #elif defined(AVEL_NEON)
        auto t0 = decay(m);
        auto t1 = vpmax_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmax_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) != 0x00;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x32u m) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return 0xF == decay(m);

        #elif defined(AVEL_SSE41)
        return _mm_test_all_ones(decay(m));

        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vminvq_u8(vreinterpretq_u8_u32(decay(m))) == 0xFF;

        #elif defined(AVEL_NEON)
        auto t0 = decay(m);
        auto t1 = vpmin_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmin_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) == 0xFFFFFFFF;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x32u m) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return _kortestz_mask8_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX512VL)
        return 0x0 == decay(m);

        #elif defined(AVEL_SSE41)
        return _mm_test_all_zeros(decay(m), decay(m));

        #elif defined(AVEL_SSE2)
        return !any(m);
        #endif

        #if defined(AVEL_NEON)
        return !any(m);
        #endif
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL bool extract(mask4x32u m) {
        static_assert(N < mask4x32u::width, "Specified index does not exist");
        typename std::enable_if<N < mask4x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return decay(m) & (1 << N);

        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m)) & (1 << (N * sizeof(std::uint32_t)));

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement

        #endif
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL mask4x32u insert(mask4x32u m, bool b) {
        static_assert(N < mask4x32u::width, "Specified index does not exist");
        typename std::enable_if<N < mask4x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = b << N;
        return mask4x32u{__mmask8((decay(m) & ~mask) | mask)};

        #elif defined(AVEL_SSE41)
        auto mask = std::uint32_t(b ? -1 : 0);
        return mask4x32u{_mm_insert_epi32(decay(m), mask, N)};

        #elif defined(AVEL_SSE2)
        auto lane = b ? -1 : 0;
        auto ret = decay(m);
        ret = _mm_insert_epi16(ret, lane, N * 2);
        ret = _mm_insert_epi16(ret, lane, N * 2 + 1);
        return mask4x32u{ret};

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement

        #endif
    }





    template<>
    class Vector<std::uint32_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint32_t;

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint32x4_t;
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
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content(_mm_maskz_set1_epi32(decay(m), 0x1)) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi32(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(decay(m))))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi32(x)) {}
        #endif
        #if defined(AVEL_NEON)
            Vector(vdupq_n_u32(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr4x32u& array):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_u32(array.data())) {}
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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmpeq_epi32_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi32(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_u32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmpneq_epi32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpeq_epi32(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmplt_epu32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            Vector add_mask{0x80000000};
            return mask{_mm_cmplt_epi32(decay(lhs ^ add_mask), decay(rhs ^ add_mask))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_u32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmple_epu32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE41)
            auto mins = _mm_min_epu32(decay(lhs), decay(rhs));
            return mask{_mm_cmpeq_epi32(mins, decay(lhs))};

            #elif defined(AVEL_SSE2)
            return !mask{lhs > rhs};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_u32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmpgt_epu32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            Vector add_mask{0x80000000};
            return mask{_mm_cmpgt_epi32(decay(lhs ^ add_mask), decay(rhs ^ add_mask))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_u32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmpge_epu32_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE41)
            auto mins = _mm_min_epu32(decay(lhs), decay(rhs));
            return mask{_mm_cmpeq_epi32(mins, decay(rhs))};

            #elif defined(AVEL_SSE2)
            return !mask{lhs < rhs};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_u32(decay(lhs), decay(rhs))};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        //Definition of operator-() deferred until after definition of vec4x32i

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi32(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_u32(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi32(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_u32(content, rhs.content);
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_SSE41)
            content = _mm_mullo_epi32(content, decay(rhs));

            #elif defined(AVEL_SSE2)
            auto lhs_lo = content;
            auto lhs_hi = _mm_srli_si128(content, 4);

            auto rhs_lo = decay(rhs);
            auto rhs_hi = _mm_srli_si128(decay(rhs), 4);

            auto lo = _mm_mul_epu32(lhs_lo, rhs_lo);
            auto hi = _mm_mul_epu32(lhs_hi, rhs_hi);

            auto half0 = _mm_unpacklo_epi32(lo, hi);
            auto half1 = _mm_unpackhi_epi32(lo, hi);

            content = _mm_unpacklo_epi64(half0, half1);

            #endif

            #if defined(AVEL_NEON)
            content = vmulq_u32(content, decay(rhs));
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
            #if defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi32(content, _mm_cvtsi64_si128(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u32(content, vdupq_n_s32(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_srl_epi32(content, _mm_cvtsi64_si128(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u32(content, vdupq_n_s32(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi32(content, rhs.content);

            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto x_half0 = _mm_unpacklo_epi32(content, zeros);
            auto x_half1 = _mm_unpackhi_epi32(content, zeros);

            auto y_half0 = _mm_unpacklo_epi32(decay(rhs), zeros);
            auto y_half1 = _mm_unpackhi_epi32(decay(rhs), zeros);

            auto X0 = _mm_sll_epi32(x_half0, y_half0);
            auto X1 = _mm_sll_epi32(x_half0, _mm_srli_si128(y_half0, 8));
            auto X2 = _mm_sll_epi32(x_half1, y_half1);
            auto X3 = _mm_sll_epi32(x_half1, _mm_srli_si128(y_half1, 8));

            auto t0 = _mm_unpacklo_epi64(X0, X2);
            auto t1 = _mm_unpackhi_epi64(X1, X3);

            auto t2 = _mm_slli_si128(t1, 0x4);

            content = _mm_or_si128(t0, t2);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u32(content, vreinterpretq_s32_u32(decay(rhs)));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_srlv_epi32(content, rhs.content);

            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto x_half0 = _mm_unpacklo_epi32(content, zeros);
            auto x_half1 = _mm_unpackhi_epi32(content, zeros);

            auto y_half0 = _mm_unpacklo_epi32(decay(rhs), zeros);
            auto y_half1 = _mm_unpackhi_epi32(decay(rhs), zeros);

            auto X0 = _mm_srl_epi32(x_half0, y_half0);
            auto X1 = _mm_srl_epi32(x_half0, _mm_srli_si128(y_half0, 8));
            auto X2 = _mm_srl_epi32(x_half1, y_half1);
            auto X3 = _mm_srl_epi32(x_half1, _mm_srli_si128(y_half1, 8));

            auto t0 = _mm_unpacklo_epi64(X0, X2);
            auto t1 = _mm_unpackhi_epi64(X1, X3);

            auto t2 = _mm_slli_si128(t1, 0x4);

            content = _mm_or_si128(t0, t2);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u32(content, vnegq_s32(vreinterpretq_s32_u32(decay(rhs))));

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return Vector{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            primitive t = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi32(t, t))};

            #endif

            #if defined(AVEL_NEON)
            return Vector{vmvnq_u32(content)};
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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_test_epi32_mask(content, content)};

            #elif defined(AVEL_SSE2)
            return *this != Vector{0x00};
            #endif

            #if defined(AVEL_NEON)
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        4 * sizeof(std::uint32_t) == sizeof(vec4x32u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint32_t extract(vec4x32u v) {
        static_assert(N < vec4x32u::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE41)
        return _mm_extract_epi32(decay(v), N);

        #elif defined(AVEL_SSE2)
        return _mm_cvtsi128_si32(_mm_srli_si128(decay(v), 4 * N));

        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_u32(decay(v), N);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x32u insert(vec4x32u v, std::uint32_t x) {
        static_assert(N < vec4x32u::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x32u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_insert_epi32(decay(v), x, N)};

        #elif defined(AVEL_SSE2)
        auto tmp = decay(v);
        tmp = _mm_insert_epi16(tmp, x & 0xFFFF, N * 2 + 0);
        tmp = _mm_insert_epi16(tmp, x >> 16,    N * 2 + 1);
        return vec4x32u{tmp};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vsetq_lane_u32(x, decay(v), N)};

        #endif
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x32u bit_shift_left(vec4x32u v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_slli_epi32(decay(v), S)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vshlq_n_u32(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u bit_shift_left<0>(vec4x32u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u bit_shift_left<32>(vec4x32u v) {
        return vec4x32u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x32u bit_shift_right(vec4x32u v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_srli_epi32(decay(v), S)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vshrq_n_u32(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u bit_shift_right<0>(vec4x32u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u bit_shift_right<32>(vec4x32u v) {
        (void)v;
        return vec4x32u{0x00};
    }



    template<std::uint32_t S, typename std::enable_if<S < 32, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x32u rotl(vec4x32u v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_rol_epi32(decay(v), S)};

        #elif defined(AVEL_SSE2)
        auto left_shifted  = _mm_slli_epi32(decay(v), S);
        auto right_shifted = _mm_srli_epi32(decay(v), 32 - S);

        auto ret = _mm_or_si128(left_shifted, right_shifted);
        return vec4x32u{ret};
        #endif

        #if defined(AVEL_NEON)
        auto left_shifted  = vshlq_n_u32(decay(v), S);
        auto right_shifted = vshrq_n_u32(decay(v), 32 - S);

        return vec4x32u{vorrq_u32(left_shifted, right_shifted)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u rotl<0>(vec4x32u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<32 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x32u rotl(vec4x32u v) {
        return rotl<S % 32>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotl(vec4x32u v, long long s) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_rolv_epi32(decay(v), _mm_set1_epi32(s))};

        #elif defined(AVEL_SSE2)
        s &= 0x1F;
        return (v << s) | (v >> (32 - s));
        #endif

        #if defined(AVEL_NEON)
        s &= 0x1F;
        return (v << s) | (v >> (32 - s));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotl(vec4x32u v, vec4x32u s) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_rolv_epi32(decay(v), decay(s))};

        #elif defined(AVEL_SSE2)
        s &= vec4x32u(0x1F);
        return (v << s) | (v >> (vec4x32u{32} - s));
        #endif

        #if defined(AVEL_NEON)
        s &= vec4x32u(0x1F);
        return (v << s) | (v >> (vec4x32u{32} - s));

        #endif
    }



    template<std::uint32_t S, typename std::enable_if<S < 32, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x32u rotr(vec4x32u v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_ror_epi32(decay(v), S)};

        #elif defined(AVEL_SSE2)
        auto left_shifted  = _mm_slli_epi32(decay(v), 32 - S);
        auto right_shifted = _mm_srli_epi32(decay(v), S);

        auto ret = _mm_or_si128(left_shifted, right_shifted);
        return vec4x32u{ret};
        #endif

        #if defined(AVEL_NEON)
        auto left_shifted  = vshlq_n_u32(decay(v), 32 - S);
        auto right_shifted = vshrq_n_u32(decay(v), S);

        return vec4x32u{vorrq_u32(left_shifted, right_shifted)};

        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u rotr<0>(vec4x32u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<32 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x32u rotr(vec4x32u v) {
        return rotr<S % 32>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotr(vec4x32u v, long long s) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_rorv_epi32(decay(v), _mm_set1_epi32(s))};

        #elif defined(AVEL_SSE2)
        s &= 0x1F;
        return (v >> s) | (v << (32 - s));

        #endif

        #if defined(AVEL_NEON)
        s &= 0x1F;
        return (v >> s) | (v << (32 - s));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u rotr(vec4x32u v, vec4x32u s) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_rorv_epi32(decay(v), decay(s))};

        #elif defined(AVEL_SSE2)
        s &= vec4x32u(0x1F);
        return (v >> s) | (v << (vec4x32u{32} - s));

        #endif

        #if defined(AVEL_NEON)
        s &= vec4x32u(0x1F);
        return (v >> s) | (v << (vec4x32u {32} - s));
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec4x32u x) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return count(mask4x32u{x});

        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi32(decay(x), _mm_setzero_si128());
        return 4 - (popcount(_mm_movemask_epi8(compared)) / sizeof(vec4x32u::scalar));
        #endif

        #if defined(AVEL_NEON)
        return count(mask4x32u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec4x32u x) {
        #if defined(AVEL_SSE41)
        return !_mm_testz_si128(decay(x), decay(x));

        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi32(decay(x), _mm_setzero_si128());
        return 0xFFFF != _mm_movemask_epi8(compared);
        #endif

        #if defined(AVEL_NEON)
        return any(mask4x32u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec4x32u x) {
        #if defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi32(decay(x), _mm_setzero_si128());
        return 0x00 == _mm_movemask_epi8(compared);
        #endif

        #if defined(AVEL_NEON)
        return all(mask4x32u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec4x32u x) {
        #if defined(AVEL_SSE41)
        return _mm_test_all_zeros(decay(x), decay(x));

        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0xFFFF == _mm_movemask_epi8(compared);
        #endif

        #if defined(AVEL_NEON)
        return none(mask4x32u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u set_bits(mask4x32u m) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_movm_epi32(decay(m))};

        #elif defined(AVEL_AVX512VL)
        return vec4x32u{_mm_maskz_set1_epi32(decay(m), -1)};

        #elif defined(AVEL_SSE2)
        return vec4x32u{decay(m)};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{decay(m)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u keep(mask4x32u m, vec4x32u v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_maskz_mov_epi32(decay(m), decay(v))};

        #elif defined(AVEL_SSE2)
        return set_bits(m) & v;

        #endif

        #if defined(AVEL_NEON)
        return set_bits(m) & v;
        #endif

    }

    [[nodiscard]]
    AVEL_FINL vec4x32u clear(mask4x32u m, vec4x32u v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_maskz_mov_epi32(decay(!m), decay(v))};

        #elif defined(AVEL_SSE2)
        return vec4x32u{_mm_andnot_si128(decay(m), decay(v))};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vbicq_u32(decay(v), decay(m))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u blend(mask4x32u m, vec4x32u a, vec4x32u b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_mask_blend_epi32(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_SSE41)
        return vec4x32u{_mm_blendv_epi8(decay(b), decay(a), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(decay(m), decay(b));
        auto y = _mm_and_si128(decay(m), decay(a));
        return vec4x32u{_mm_or_si128(x, y)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vbslq_u32(decay(m), decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u max(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_max_epu32(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vmaxq_u32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u min( vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_min_epu32(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(b < a, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vminq_u32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x32u, 2> minmax(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return {
            vec4x32u{_mm_min_epu32(decay(a), decay(b))},
            vec4x32u{_mm_max_epu32(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto mask = b < a;
        return {blend(mask, b, a), blend(mask, a, b)};

        #endif

        #if defined(AVEL_NEON)
        return {
            vec4x32u{vminq_u32(decay(a), decay(b))},
            vec4x32u{vmaxq_u32(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u clamp(vec4x32u x, vec4x32u lo, vec4x32u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u average(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE2)
        return ((a ^ b) >> 1) + (a & b);
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vhaddq_u32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u midpoint(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE2)
        vec4x32u avg = ((a ^ b) >> 1) + (a & b);
        vec4x32u c = set_bits(b < a) & (a ^ b) & vec4x32u{0x1};
        return avg + c;
        #endif

        #if defined(AVEL_NEON)
        vec4x32u t0 = vec4x32u{vhaddq_u32(decay(a), decay(b))};
        vec4x32u t1 = (a ^ b) & vec4x32u{0x1} & set_bits(a > b);
        return t0 + t1;

        #endif
    }

    //Definition of neg_abs delayed until vec4x32i is defined

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u load<vec4x32u>(const std::uint32_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        return vec4x32u{_mm_maskz_load_epi32(mask, ptr)};

        #elif defined(AVEL_SSE2)
        switch (n) {
            case 0: {
                return vec4x32u{_mm_setzero_si128()};
            }
            case 1: {
                return vec4x32u{_mm_cvtsi32_si128(ptr[0])};
            }
            case 2: {
                std::int64_t two = 0x00;
                two |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                two |= static_cast<std::int64_t>(ptr[1]) << 0x20;
                return vec4x32u{_mm_cvtsi64_si128(two)};
            }
            case 3: {
                std::int64_t two_s = 0x00;
                two_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                two_s |= static_cast<std::int64_t>(ptr[1]) << 0x20;

                std::int32_t one_s = 0x00;
                one_s |= ptr[2] << 0x00;

                auto two_v = _mm_cvtsi64_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec4x32u{_mm_unpacklo_epi64(two_v, one_v)};
            }
            default: {
                return vec4x32u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
            }
        }

        #elif defined(AVEL_NEON)
        switch (n) {
            case 0: {
                return vec4x32u{vdupq_n_u32(0x00)};
            };
            case 1: {
                std::uint32_t x0;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint32_t));

                auto ret0 = vsetq_lane_u32(x0, vdupq_n_u32(0x00), 0);
                return vec4x32u{ret0};
            }
            case 2: {
                std::uint64_t x0;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));

                auto ret0 = vsetq_lane_u64(x0, vdupq_n_u64(0x00), 0);
                auto ret1 = vreinterpretq_u32_u64(ret0);
                return vec4x32u{ret1};
            }
            case 3: {
                std::uint64_t x0;
                std::uint32_t x1;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));
                std::memcpy(&x1, ptr + 2, sizeof(std::uint32_t));

                auto ret0 = vsetq_lane_u64(x0, vdupq_n_u64(0x00), 0);
                auto ret1 = vsetq_lane_u32(x1, vreinterpretq_u32_u64(ret0), 2);
                return vec4x32u{ret1};
            }
            default: {
                return vec4x32u{vld1q_u32(ptr)};
            }
        }
        #endif
    }

    //TODO: Add specializations that take advantage of AVX2 masked load instructions

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u load<vec4x32u, vec4x32u::width>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};

        #elif defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};

        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u aligned_load<vec4x32u>(const std::uint32_t* ptr, std::uint32_t n) {
        return load<vec4x32u>(ptr, n);
    }

    //TODO: Add specializations that take advantage of AVX2 masked load instructions

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32u aligned_load<vec4x32u, vec4x32u::width>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};

        #endif

        #if defined(AVEL_NEON) && __cplusplus >= 202002
            return vec4x32u{vld1q_u32(assume_aligned<alignof(vec4x32u)>(ptr))};
        #elif defined(AVEL_NEON) && (defined(AVEL_GCC) || defined(AVEL_CLANG))
            auto* p = reinterpret_cast<const std::uint32_t*>(__builtin_assume_aligned(ptr, alignof(vec4x32u)));
            return vec4x32u{vld1q_u32(p)};
        #elif defined(AVEL_NEON)
            return vec4x32u{vld1q_u32(ptr)};
        #endif
    }

    //Definition of gather deferred until vec4x32i is defined



    AVEL_FINL void store(std::uint32_t* ptr, vec4x32u x, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        _mm_mask_storeu_epi32(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        n = min(n, vec4x32u::width);
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec4x32u::width;
        auto h = vec4x32u::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (h - min(h, n))));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (w - min(w, n))));
        auto mask = _mm_unpacklo_epi64(lo, hi);
        _mm_maskmoveu_si128(decay(x), mask, reinterpret_cast<char *>(ptr));

        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            case 0: {

            } break;
            case 1: {
                std::uint32_t x0 = vgetq_lane_u32(decay(x), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::uint32_t));
            } break;
            case 2: {
                std::uint64_t x0 = vgetq_lane_u64(vreinterpretq_u64_u32(decay(x)), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::uint64_t));
            } break;
            case 3: {
                std::uint64_t x0 = vgetq_lane_u64(vreinterpretq_u64_u32(decay(x)), 0);
                std::uint32_t x1 = vgetq_lane_u32(decay(x), 2);

                std::memcpy(ptr + 0, &x0, sizeof(std::uint64_t));
                std::memcpy(ptr + 2, &x1, sizeof(std::uint32_t));
            } break;
            default: {
                vst1q_u32(ptr, decay(x));
            }
        }
        #endif
    }

    template<std::uint32_t N = vec4x32u::width>
    AVEL_FINL void store(std::uint32_t* ptr, vec4x32u x) {
        static_assert(N <= vec4x32u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec4x32u::width, int>::type dummy_variable = 0;

        store(ptr, x, N);
    }

    //TODO: Add specializations that take advantage of AVX2 masked store instructions

    template<>
    AVEL_FINL void store<vec4x32u::width>(std::uint32_t* ptr, vec4x32u x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u32(ptr, decay(x));
        #endif
    }



    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec4x32u x, std::uint32_t n) {
        store(ptr, x, n);
    }

    template<std::uint32_t N = vec4x32u::width>
    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec4x32u x) {
        static_assert(N <= vec4x32u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec4x32u::width, int>::type dummy_variable = 0;

        aligned_store(ptr, x, N);
    }

    //TODO: Add specializations that take advantage of AVX2 masked store instructions

    template<>
    AVEL_FINL void aligned_store<vec4x32u::width>(std::uint32_t* ptr, vec4x32u x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u32(ptr, decay(x));
        #endif
    }

    //Definition of scatter deferred until vec4x32i is defined



    [[nodiscard]]
    AVEL_FINL arr4x32u to_array(vec4x32u v) {
        alignas(16) arr4x32u array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer vector operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec4x32u> div(vec4x32u x, vec4x32u y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto x_widened = _mm256_cvtepu32_pd(decay(x));
        auto y_widened = _mm256_cvtepu32_pd(decay(y));

        auto quotient = _mm256_cvttpd_epu32(_mm256_div_pd(x_widened, y_widened));

        auto offset = _mm_mullo_epi32(decay(y), quotient);
        auto remainder = _mm_sub_epi32(decay(x), offset);

        return {
            vec4x32u{quotient},
            vec4x32u{remainder}
        };

        #elif defined(AVEL_SSE2)
        //TODO: Optimize body with masked instructions
        vec4x32u quotient{0x0};
        std::uint32_t i = 32;
        for (; (i-- > 0) && any(mask4x32u(x));) {
            mask4x32u b = ((x >> i) >= y);
            x -= (set_bits(b) & (y << i));
            quotient |= (vec4x32u{b} << i);
        }

        return {quotient, x};
        #endif

        #if defined(AVEL_NEON)
        //Just to avoid division by zero triggering an exception
        auto denominators = max(y, vec4x32u{1});

        //Falling back to scalar code ends up being faster
        auto x0 = vgetq_lane_u32(decay(x), 0);
        auto x1 = vgetq_lane_u32(decay(x), 1);
        auto x2 = vgetq_lane_u32(decay(x), 2);
        auto x3 = vgetq_lane_u32(decay(x), 3);

        auto y0 = vgetq_lane_u32(decay(y), 0);
        auto y1 = vgetq_lane_u32(decay(y), 1);
        auto y2 = vgetq_lane_u32(decay(y), 2);
        auto y3 = vgetq_lane_u32(decay(y), 3);

        auto quot = vdupq_n_u32(0x00);
        quot = vsetq_lane_u32(x0 / y0, quot, 0);
        quot = vsetq_lane_u32(x1 / y1, quot, 1);
        quot = vsetq_lane_u32(x2 / y2, quot, 2);
        quot = vsetq_lane_u32(x3 / y3, quot, 3);

        auto rem = vdupq_n_u32(0x00);
        rem = vsetq_lane_u32(x0 % y0, rem, 0);
        rem = vsetq_lane_u32(x1 % y1, rem, 1);
        rem = vsetq_lane_u32(x2 % y2, rem, 2);
        rem = vsetq_lane_u32(x3 % y3, rem, 3);

        return {vec4x32u{quot}, vec4x32u{rem}};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u popcount(vec4x32u x) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_popcnt_epi32(decay(x))};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm_popcnt_epi16(decay(x));
        auto tmp1 = _mm_slli_epi32(tmp0, 16);
        auto tmp2 = _mm_add_epi32(tmp0, tmp1);
        return vec4x32u{_mm_srli_epi32(tmp2, 16)};

        #elif defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto nibble_mask = _mm_set1_epi8(0x0F);

        auto index0 = _mm_and_si128(decay(x), nibble_mask);
        auto index1 = _mm_and_si128(_mm_srli_epi16(decay(x), 0x4), nibble_mask);

        auto partial_byte_sum0 = _mm_shuffle_epi8(table, index0);
        auto partial_byte_sum1 = _mm_shuffle_epi8(table, index1);

        auto byte_sum = _mm_add_epi8(partial_byte_sum0, partial_byte_sum1);

        auto partial_sum0 = _mm_add_epi8(byte_sum, _mm_slli_epi32(byte_sum, 16));
        auto partial_sum1 = _mm_add_epi8(partial_sum0, _mm_slli_epi32(partial_sum0, 8));

        auto ret = _mm_srli_epi32(partial_sum1, 24);

        return vec4x32u{ret};

        #elif defined(AVEL_SSE2)
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & vec4x32u{0x55555555u});
        x = (x & vec4x32u{0x33333333u}) + ((x >> 2) & vec4x32u{0x33333333u});
        x = ((x + (x >> 4) & vec4x32u{0x0F0F0F0Fu}) * vec4x32u{0x01010101u}) >> 24;
        return x;

        #endif

        #if defined(AVEL_NEON)
        auto reg_abcd = vreinterpretq_u32_u8(vcntq_u8(vreinterpretq_u8_u32(decay(x))));
        auto reg_00ab = vshlq_n_u32(reg_abcd, 16);
        auto reg_00xy = vaddq_u32(reg_abcd, reg_00ab);

        auto reg_000x = vshlq_n_u32(reg_00xy, 8);
        auto reg_000z = vaddq_u32(reg_000x, reg_00xy);

        return vec4x32u{vshrq_n_u32(reg_000z, 24)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u byteswap(vec4x32u v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)) || defined(AVEL_AVX10_1)
        // Reverse 16-bit halves within 32-bit ints
        auto t0 = _mm_shldi_epi32(decay(v), decay(v), 16);

        // Reverse 8-bit halves within 16-bit ints
        auto t1 = _mm_shldi_epi16(t0, t0, 8);

        return vec4x32u{t1};

        #elif defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t index_data[16] {
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12
        };

        auto indices = _mm_load_si128((const __m128i*)index_data);
        return vec4x32u{_mm_shuffle_epi8(decay(v), indices)};

        #elif defined(AVEL_SSE2)
        auto t0 = _mm_shufflelo_epi16(decay(v), 0xB1);
        auto t1 = _mm_shufflehi_epi16(t0, 0xB1);
        auto t2 = _mm_slli_epi16(t1, 0x8);
        auto t3 = _mm_srli_epi16(t1, 0x8);
        return vec4x32u{_mm_or_si128(t2, t3)};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(decay(v))))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countl_zero(vec4x32u x) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_lzcnt_epi32(decay(x))};

        #elif defined(AVEL_SSE2)
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        x = _mm_andnot_si128(_mm_srli_epi32(decay(x), 1), decay(x));
        auto floats = _mm_add_ps(_mm_cvtepi32_ps(decay(x)), _mm_set1_ps(0.5f));
        auto biased_exponents = _mm_srli_epi32(_mm_castps_si128(floats), 23);
        auto lzcnt = _mm_subs_epu16(_mm_set1_epi32(158), biased_exponents);
        return vec4x32u{lzcnt};

        //#else
        //return countl_one(~x);
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{vclzq_u32(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countl_one(vec4x32u x) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)) || defined(AVEL_AVX10_1)
        return vec4x32u{_mm_lzcnt_epi32(decay(~x))};
        #elif defined(AVEL_SSE2)
        return countl_zero(~x);
        #endif

        /* Pure software implementation
        vec4x32u sum{x == vec4x32u{0xFFFFFFFF}};

        vec4x32u m0{0xFFFF0000u};
        mask4x32u b0 = (m0 & x) == m0;
        sum += set_bits(b0) & vec4x32u{16};
        x <<= set_bits(b0) * vec4x32u{16};

        vec4x32u m1{0xFF000000u};
        mask4x32u b1 = (m1 & x) == m1;
        sum += set_bits(b1) & vec4x32u{8};
        x <<= set_bits(b1) & vec4x32u{8};

        vec4x32u m2{0xF0000000u};
        mask4x32u b2 = (m2 & x) == m2;
        sum += set_bits(b2) & vec4x32u{4};
        x <<= set_bits(b2) & vec4x32u{4};

        vec4x32u m3{0xC0000000u};
        mask4x32u b3 = (m3 & x) == m3;
        sum += set_bits(b3) & vec4x32u{2};
        x <<= set_bits(b3) & vec4x32u{2};

        vec4x32u m4{0x80000000u};
        mask4x32u b4 = (m4 & x) == m4;
        sum += set_bits(b4) & vec4x32u{1};

        return sum;
        */

        #if defined(AVEL_NEON)
        return vec4x32u{vclzq_u32(decay(~x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countr_zero(vec4x32u x) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)) || defined(AVEL_AVX10_1)
        auto undef = _mm_undefined_si128();
        auto neg_one = _mm_cmpeq_epi32(undef, undef);
        auto tz_mask = _mm_andnot_si128(decay(x), _mm_add_epi32(decay(x), neg_one));
        return vec4x32u{_mm_popcnt_epi32(tz_mask)};

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto zero_mask = (x == vec4x32u{0x00});
        auto y = (x & (vec4x32u{0x00} - x));
        auto z = vec4x32u{31} - countl_zero(y);
        return blend(zero_mask, vec4x32u{32}, z);

        #elif defined(AVEL_SSE2)
        auto y = (x & (vec4x32u{0x00} - x));
        auto floats = _mm_cvtepi32_ps(decay(y));
        auto biased_exponents = (vec4x32u(_mm_castps_si128(floats)) >> 23);
        biased_exponents = _mm_min_epi16(decay(vec4x32u{158}), decay(biased_exponents));
        auto tzcnt = biased_exponents - vec4x32u{127};
        tzcnt = blend(x == vec4x32u{0x00}, vec4x32u{32}, tzcnt);

        return tzcnt;
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto reversed_bytes = vrev32q_u8(vreinterpretq_u8_u32(decay(x)));
        auto reversed_bits = vrbitq_u8(reversed_bytes);
        return vec4x32u{vclzq_u32(vreinterpretq_u32_u8(reversed_bits))};

        #elif defined(AVEL_NEON)
        auto zero_mask = (x == vec4x32u{0x00});
        auto y = (x & (vec4x32u{0x00} - x));
        return blend(zero_mask, vec4x32u{32}, vec4x32u{31} - countl_zero(y));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u countr_one(vec4x32u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u bit_width(vec4x32u x) {
        #if defined(AVEL_SSE2)
        return vec4x32u{32} - vec4x32u{countl_zero(x)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32u{32} - vec4x32u{countl_zero(x)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u bit_floor(vec4x32u x) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)) || defined(AVEL_AVX10_1)
        mask4x32u zero_mask = (x != vec4x32u{0x00});
        vec4x32u leading_zeros = countl_zero(x);

        return (vec4x32u{zero_mask} << (vec4x32u{31} - leading_zeros));

        #elif defined(AVEL_SSE2)
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        return x - (x >> 1);

        #endif

        #if defined(AVEL_NEON)
        vec4x32u leading_zeros = countl_zero(x);
        mask4x32u zero_mask = (leading_zeros != vec4x32u{32});

        return (vec4x32u{zero_mask} << (vec4x32u{31} - leading_zeros));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32u bit_ceil(vec4x32u x) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)) || defined(AVEL_AVX10_1)
        auto ones = _mm_set1_epi32(1);
        auto sh = _mm_sub_epi32(_mm_set1_epi32(32), _mm_lzcnt_epi32(_mm_sub_epi32(decay(x), ones)));
        auto result = _mm_sllv_epi32(ones, sh);
        return vec4x32u{_mm_sub_epi32(result, _mm_cmpeq_epi32(decay(x), _mm_setzero_si128()))};

        #elif defined(AVEL_SSE2)
        auto zero_mask = (x == vec4x32u{0x00});

        --x;
        x |= bit_shift_right<1>(x);
        x |= bit_shift_right<2>(x);
        x |= bit_shift_right<4>(x);
        x |= bit_shift_right<8>(x);
        x |= bit_shift_right<16>(x);
        x = _mm_andnot_si128(decay(set_bits(zero_mask)), decay(x));
        ++x;

        return x;

        #endif

        #if defined(AVEL_NEON)
        auto sh = (vec4x32u{32} - countl_zero(x - vec4x32u{1}));
        auto result = vec4x32u{1} << sh;
        return result - set_bits(x == vec4x32u{0x00});
        #endif
    };

    [[nodiscard]]
    AVEL_FINL mask4x32u has_single_bit(vec4x32u v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512VPOPCNTDQ)) || defined(AVEL_AVX10_1)
        return popcount(v) == vec4x32u{1};

        #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
        return mask4x32u{v} & !mask4x32u{v & (v - vec4x32u{1})};
        #endif
    }

}

#endif //AVEL_VEC4X32U_HPP
