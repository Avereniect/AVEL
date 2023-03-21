#ifndef AVEL_VEC8X16U_HPP
#define AVEL_VEC8X16U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x16u = Vector<std::uint16_t, 8>;
    using arr8x16u = std::array<std::uint16_t, 8>;
    using mask8x16u = Vector_mask<std::uint16_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x16u> div(vec8x16u numerator, vec8x16u denominator);
    vec8x16u broadcast_mask(mask8x16u m);
    vec8x16u blend(mask8x16u m, vec8x16u a, vec8x16u b);
    vec8x16u countl_one(vec8x16u x);





    template<>
    class Vector_mask<std::uint16_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint16x8_t;
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
            content(b ? 0xFF : 0x00) {}
        #elif defined(AVEL_SSE2)
            content(b ? _mm_set1_epi16(-1) : _mm_setzero_si128()) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u16(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr8xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si64(arr.data());
            content = static_cast<primitive>(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_SSE2)
            auto array_data = _mm_loadu_si64(arr.data());

            array_data = _mm_unpacklo_epi8(array_data, array_data);
            content = _mm_cmplt_epi16(_mm_setzero_si128(), array_data);

            #endif

            #if defined(AVEL_NEON)
            auto array_data = vld1_u8(bit_cast<const std::uint8_t*>(arr.data()));
            auto mask = vclt_u8(vdup_n_u8(0x00), array_data);
            auto widened_mask = vcombine_u8(mask, mask);
            content = vreinterpretq_u16_u8(vzipq_u8(widened_mask, widened_mask).val[0]);
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
            content = b ? 0xFF : 0x00;
            #elif defined(AVEL_SSE2)
            content = b ? _mm_set1_epi16(-1) : _mm_setzero_si128();
            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_u16(b ? -1 : 0);
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u16(vceqq_u16(decay(lhs), decay(rhs)));
            return min == 0xFFFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u8(vreinterpretq_u8_u16(vceqq_u16(decay(lhs), decay(rhs))));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(vreinterpretq_u64_u16(decay(lhs)), vreinterpretq_u64_u16(decay(rhs)));
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content &= decay(rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u16(content, rhs.content);
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content |= decay(rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u16(content, rhs.content);
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content ^= decay(rhs);
            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u16(content, rhs.content);
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

            #elif defined(AVEL_SSE2)
            primitive t = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(t, t))};

            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vmvnq_u16(content)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
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
    AVEL_FINL std::uint32_t count(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(_mm512_mask2int(decay(m)));

        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m))) / sizeof(std::uint16_t);

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vnegq_s16(vreinterpretq_s16_u16(decay(m)));
        return static_cast<std::uint32_t>(vaddvq_s16(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u16_s16(vnegq_s16(vreinterpretq_s16_u16(decay(m))));
        auto t1 = vpadd_u16(vget_low_u16(t0), vget_high_u16(t0));
        auto t2 = vpadd_u16(t1, t1);
        auto t3 = vpadd_u16(t2, t2);

        return static_cast<std::uint32_t>(vget_lane_u16(t3, 0));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512DQ)
        return !_kortestz_mask8_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _mm512_mask2int(decay(m));

        #elif defined(AVEL_SSE41)
        return !_mm_test_all_zeros(decay(m), decay(m));

        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vmaxvq_u16(decay(m)) != 0x00;

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u16(decay(m));
        auto t1 = vpmax_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmax_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) != 0x00;

        #endif
    }


    [[nodiscard]]
    AVEL_FINL bool all(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512DQ)
        return _kortestc_mask8_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return 0xFF == _mm512_mask2int(decay(m));

        #elif defined(AVEL_SSE41)
        return _mm_test_all_ones(decay(m));

        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(decay(m));

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vminvq_u8(vreinterpretq_u8_u16(decay(m))) == 0xFF;

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u16(decay(m));
        auto t1 = vpmin_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmin_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) == 0xFFFFFFFF;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512DQ)
        return _kortestz_mask8_u8(decay(m), decay(m));

        #elif defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return 0x00 == _mm512_mask2int(decay(m));

        #elif defined(AVEL_SSE41)
        return _mm_test_all_zeros(decay(m), decay(m));

        #elif defined(AVEL_SSE2)
        return !any(m);
        #endif

        #if defined(AVEL_NEON)
        return !any(m);
        #endif
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask8x16u>(mask8x16u m) {
        return {m};
    }





    template<>
    class Vector<std::uint16_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint16_t;

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint16x8_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Instance members
        //=================================================

    private:

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
            content(_mm_maskz_set1_epi16(decay(m), 0x1)) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi16(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vreinterpretq_u16_s16(vnegq_s16(vreinterpretq_s16_u16(decay(m))))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi16(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_u16(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr8x16u& array):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_u16(array.data())) {}
        #endif

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_SSE2)
            content = _mm_set1_epi16(x);
            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_u16(x);
            #endif
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpeq_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_u16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpneq_epu16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !(lhs == rhs);
            #endif

            #if defined(AVEL_NEON)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmplt_epu16_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            auto addition_mask = _mm_set1_epi16(0x8000);
            auto lhs_offset = _mm_xor_si128(addition_mask, decay(lhs));
            auto rhs_offset = _mm_xor_si128(addition_mask, decay(rhs));
            return mask{_mm_cmplt_epi16(lhs_offset, rhs_offset)};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_u16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmple_epu16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE41)
            auto mins = _mm_min_epu16(decay(lhs), decay(rhs));
            return mask{_mm_cmpeq_epi16(mins, decay(lhs))};
            #elif defined(AVEL_SSE2)
            return !mask{lhs > rhs};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_u16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpgt_epu16_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            auto addition_mask = _mm_set1_epi16(0x8000);
            auto lhs_offset = _mm_xor_si128(addition_mask, decay(lhs));
            auto rhs_offset = _mm_xor_si128(addition_mask, decay(rhs));
            return mask{_mm_cmpgt_epi16(lhs_offset, rhs_offset)};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_u16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpge_epu16_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE41)
            auto mins = _mm_min_epu16(decay(lhs), decay(rhs));
            return mask{_mm_cmpeq_epi16(mins, decay(rhs))};

            #elif defined(AVEL_SSE2)
            return !mask{lhs < rhs};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_u16(decay(lhs), decay(rhs))};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        //Definition of operator-() deferred until after definition of vec8x16i

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi16(content, vec.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_u16(content, decay(vec));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi16(content, vec.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_u16(content, decay(vec));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_mullo_epi16(content, vec.content);
            #endif

            #if defined(AVEL_NEON)
            content = vmulq_u16(content, decay(vec));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            auto results = div(*this, vec);
            *this = results.quot;
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
            auto results = div(*this, vec);
            *this = results.rem;
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
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_SSE2)
            content  = _mm_sll_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u16(content, vdupq_n_s16(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_srl_epi16(content, _mm_cvtsi32_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u16(content, vdupq_n_s16(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm_sllv_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto t0 = _mm256_cvtepu16_epi32(content);
            auto t1 = _mm256_cvtepu16_epi32(decay(rhs));

            auto t2 = _mm256_and_si256(_mm256_sllv_epi32(t0, t1), _mm256_set1_epi32(0x0000FFFF));

            auto lo = t2;
            auto hi = _mm256_permute2f128_si256(lo, lo, 0x01);
            content = _mm256_castsi256_si128(_mm256_packus_epi32(lo, hi));


            #elif defined(AVEL_SSE41)
            // This implementation is slower than an equivalent scalar approach,
            // but faster than attempting to scalarize the function
            for (unsigned i = 0; i < 4; ++i) {
                auto t0 = _mm_and_si128(rhs.content, _mm_set1_epi16(1u << i));
                auto m = _mm_cmplt_epi16(_mm_setzero_si128(), t0);

                auto shifted =  _mm_sll_epi16(content, _mm_cvtsi64_si128(1u << i));
                content = _mm_blendv_epi8(content, shifted, m);
            }

            auto m = _mm_cmplt_epi16(rhs.content, _mm_set1_epi16(16));
            content = _mm_and_si128(content, m);

            //TODO: Consider SSSE3 implementation

            #elif defined(AVEL_SSE2)
            // This implementation is slower than an equivalent scalar approach,
            // but faster than attempting to scalarize the function
            for (unsigned i = 0; i < 4; ++i) {
                auto t0 = _mm_and_si128(rhs.content, _mm_set1_epi16(1u << i));
                auto m = _mm_cmplt_epi16(_mm_setzero_si128(), t0);

                auto a = _mm_andnot_si128(m, content);
                auto b = _mm_and_si128(m, _mm_sll_epi16(content, _mm_cvtsi64_si128(1u << i)));
                content = _mm_or_si128(a, b);
            }

            auto m = _mm_cmplt_epi16(rhs.content, _mm_set1_epi16(16));
            content = _mm_and_si128(content, m);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u16(content, vreinterpretq_s16_u16(decay(rhs)));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm_srlv_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto t0 = _mm256_cvtepu16_epi32(content);
            auto t1 = _mm256_cvtepu16_epi32(decay(rhs));

            auto t2 = _mm256_and_si256(_mm256_srlv_epi32(t0, t1), _mm256_set1_epi32(0x0000FFFF));

            auto lo = t2;
            auto hi = _mm256_permute2f128_si256(lo, lo, 0x01);
            content = _mm256_castsi256_si128(_mm256_packus_epi32(lo, hi));

            #elif defined(AVEL_SSE41)
            auto zero_mask =  _mm_cmplt_epi16(decay(rhs), _mm_set1_epi16(16));

            for (unsigned i = 0; i < 4; ++i) {
                auto t0 = _mm_and_si128(decay(rhs), _mm_set1_epi16(1u << i));
                auto m = _mm_cmplt_epi16(_mm_setzero_si128(), t0);

                content = _mm_blendv_epi8(content, _mm_srl_epi16(content, _mm_cvtsi64_si128(1u << i)), m);
            }

            content = _mm_and_si128(content, zero_mask);

            //TODO: Consider implementation using pshufb and 32-bit
            // multiplication



            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto non_zero_mask = _mm_cmplt_epi16(decay(rhs), _mm_set1_epi16(16));
            content = _mm_and_si128(content, non_zero_mask);

            auto m0 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 15), zeros);
            auto a0 = _mm_andnot_si128(m0, content);
            auto b0 = _mm_and_si128(m0, _mm_srli_epi16(content, 1));
            content = _mm_or_si128(a0, b0);

            auto m1 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 14), zeros);
            auto a1 = _mm_andnot_si128(m1, content);
            auto b1 = _mm_and_si128(m1, _mm_srli_epi16(content, 2));
            content = _mm_or_si128(a1, b1);

            auto m2 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 13), zeros);
            auto a2 = _mm_andnot_si128(m2, content);
            auto b2 = _mm_and_si128(m2, _mm_srli_epi16(content, 4));
            content = _mm_or_si128(a2, b2);

            auto m3 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 12), zeros);
            auto a3 = _mm_andnot_si128(m3, content);
            auto b3 = _mm_and_si128(m3, _mm_srli_epi16(content, 8));
            content = _mm_or_si128(a3, b3);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_u16(content, vnegq_s16(vreinterpretq_s16_u16(decay(rhs))));

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi32(undef, undef))};

            #endif

            #if defined(AVEL_NEON)
            return Vector{vmvnq_u16(content)};
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
        AVEL_FINL friend Vector operator<<(Vector lhs, long long s) {
            lhs <<= s;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, long long s) {
            lhs >>= s;
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
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX51BW)
            return mask{_mm_test_epi16_mask(content, content)};

            #else
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        8 * sizeof(std::uint16_t) == sizeof(vec8x16u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint16_t extract(vec8x16u v) {
        static_assert(N <= vec8x16u::width, "Specified index does not exist");
        typename std::enable_if<N <= vec8x16u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE41)
        return _mm_extract_epi16(decay(v), N);

        #elif defined(AVEL_SSE2)
        return _mm_cvtsi128_si32(_mm_srli_si128(decay(v), 2 * N)) & 0xFFFF;

        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_u16(decay(v), N);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec8x16u insert(vec8x16u v, std::uint16_t x) {
        static_assert(N <= vec8x16u::width, "Specified index does not exist");
        typename std::enable_if<N <= vec8x16u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_insert_epi16(decay(v), x, N)};

        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vsetq_lane_u16(x, decay(v), N)};
        #endif
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x16u bit_shift_left(vec8x16u v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_slli_epi16(decay(v), S)};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vshlq_n_u16(decay(v), S)};
        #endif
    }

    template<>
    AVEL_FINL vec8x16u bit_shift_left<0>(vec8x16u v) {
        return v;
    }

    template<>
    AVEL_FINL vec8x16u bit_shift_left<16>(vec8x16u v) {
        return vec8x16u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    vec8x16u bit_shift_right(vec8x16u v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_srli_epi16(decay(v), S)};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vshrq_n_u16(decay(v), S)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u bit_shift_right<0>(vec8x16u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u bit_shift_right<16>(vec8x16u v) {
        (void)v;
        return vec8x16u{0x00};
    }



    template<std::uint32_t S, typename std::enable_if<S < 16, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x16u rotl(vec8x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        return vec8x16u{_mm_shldi_epi16(decay(v), decay(v), S)};

        #elif defined(AVEL_SSE2)
        auto left_shifted  = _mm_slli_epi16(decay(v), S);
        auto right_shifted = _mm_srli_epi16(decay(v), 16 - S);

        auto ret = _mm_or_si128(left_shifted, right_shifted);
        return vec8x16u{ret};
        #endif

        #if defined(AVEL_NEON)
        auto left_shifted  = vshlq_n_u16(decay(v), S);
        auto right_shifted = vshrq_n_u16(decay(v), 16 - S);

        return vec8x16u{vorrq_u16(left_shifted, right_shifted)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u rotl<0>(vec8x16u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<16 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x16u rotl(vec8x16u v) {
        return rotl<S % 16>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotl(vec8x16u v, long long s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        s &= 15;
        return vec8x16u{_mm_shldv_epi16(decay(v), decay(v), _mm_set1_epi16(s))};

        #elif defined(AVEL_SSE2)
        s &= 15;
        return (v << s) | (v >> (16 - s));
        #endif

        #if defined(AVEL_NEON)
        s &= 15;
        return (v << s) | (v >> (16 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotl(vec8x16u v, vec8x16u s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        s &= vec8x16u{15};
        return vec8x16u{_mm_shldv_epi16(decay(v), decay(v), decay(s))};

        #elif defined(AVEL_SSE2)
        s &= vec8x16u{15};
        return (v << s) | (v >> (vec8x16u{16} - s));
        #endif

        #if defined(AVEL_NEON)
        s &= vec8x16u{15};
        return (v << s) | (v >> (vec8x16u{16} - s));
        #endif
    }



    template<std::uint32_t S, typename std::enable_if<S < 16, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x16u rotr(vec8x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        return vec8x16u{_mm_shrdi_epi16(decay(v), decay(v), S)};

        #elif defined(AVEL_SSE2)
        auto left_shifted  = _mm_slli_epi16(decay(v), 16 - S);
        auto right_shifted = _mm_srli_epi16(decay(v), S);

        auto ret = _mm_or_si128(left_shifted, right_shifted);
        return vec8x16u{ret};
        #endif

        #if defined(AVEL_NEON)
        auto left_shifted  = vshlq_n_u16(decay(v), 16 - S);
        auto right_shifted = vshrq_n_u16(decay(v), S);

        return vec8x16u{vorrq_u16(left_shifted, right_shifted)};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u rotr<0>(vec8x16u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<16 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec8x16u rotr(vec8x16u v) {
        return rotr<S % 16>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotr(vec8x16u v, long long s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        s &= 15;
        return vec8x16u{_mm_shrdv_epi16(decay(v), decay(v), _mm_set1_epi16(s))};

        #elif defined(AVEL_SSE2)
        s &= 15;
        return (v >> s) | (v << (16 - s));
        #endif

        #if defined(AVEL_NEON)
        s &= 15;
        return (v >> s) | (v << (16 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotr(vec8x16u v, vec8x16u s) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        s &= vec8x16u{15};
        return vec8x16u{_mm_shrdv_epi16(decay(v), decay(v), decay(s))};

        #elif defined(AVEL_SSE2)
        s &= vec8x16u{15};
        return (v >> s) | (v << (vec8x16u{16} - s));
        #endif

        #if defined(AVEL_NEON)
        s &= vec8x16u{15};
        return (v >> s) | (v << (vec8x16u{16} - s));
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec8x16u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return count(mask8x16u{x});

        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi16(decay(x), _mm_setzero_si128());
        return (vec16x8u::width - popcount(_mm_movemask_epi8(compared))) / sizeof(vec8x16u::scalar);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Optimize
        return count(mask8x16u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec8x16u x) {
        #if defined(AVEL_SSE41)
        return !_mm_testz_si128(decay(x), decay(x));

        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0xFFFF != _mm_movemask_epi8(compared);
        #endif

        #if defined(AVEL_NEON)
        //TODO: Optimize
        return any(mask8x16u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec8x16u x) {
        #if defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0x00 == _mm_movemask_epi8(compared);
        #endif

        #if defined(AVEL_NEON)
        //TODO: Optimize
        return all(mask8x16u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec8x16u x) {
        #if defined(AVEL_SSE41)
        return _mm_test_all_zeros(decay(x), decay(x));

        #elif defined(AVEL_SSE2)
        auto compared = _mm_cmpeq_epi8(decay(x), _mm_setzero_si128());
        return 0xFF == _mm_movemask_epi8(compared);
        #endif

        #if defined(AVEL_NEON)
        //TODO: Optimize
        return none(mask8x16u{x});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u broadcast_mask(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec8x16u{_mm_movm_epi16(decay(m))};

        #elif defined(AVEL_SSE2)
        return vec8x16u{decay(m)};

        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{decay(m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u keep(mask8x16u m, vec8x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec8x16u{_mm_maskz_mov_epi16(decay(!m), decay(v))};

        #elif defined(AVEL_SSE2)
        return broadcast_mask(m) & v;

        #endif

        #if defined(AVEL_NEON)
        return broadcast_mask(m) & v;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u clear(mask8x16u m, vec8x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec8x16u{_mm_maskz_mov_epi16(decay(m), decay(v))};

        #elif defined(AVEL_SSE2)
        return vec8x16u{_mm_andnot_si128(decay(m), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vbicq_u16(decay(v), decay(m))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u blend(mask8x16u m, vec8x16u a, vec8x16u b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec8x16u{_mm_mask_blend_epi16(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_SSE41)
        return vec8x16u{_mm_blendv_epi8(decay(b), decay(a), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(decay(m), decay(b));
        auto y = _mm_and_si128(decay(m), decay(a));
        return vec8x16u{_mm_or_si128(x, y)};

        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vbslq_u16(decay(m), decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u byteswap(vec8x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VBMI2)
        return vec8x16u{_mm_shldi_epi16(decay(v), decay(v), 0x8)};

        #elif defined(AVEL_SSE2)
        auto lo = _mm_srli_epi16(decay(v), 8);
        auto hi = _mm_slli_epi16(decay(v), 8);
        return vec8x16u{_mm_or_si128(lo, hi)};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vreinterpretq_u16_u8(vrev16q_u8(vreinterpretq_u8_u16(decay(v))))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u max(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE41)
        return vec8x16u{_mm_max_epu16(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        auto m = a < b;
        return blend(a < b, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vmaxq_u16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u min(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE41)
        return vec8x16u{_mm_min_epu16(decay(a), decay(b))};
        #elif defined(AVEL_SSE2)
        return blend(b < a, b, a);
        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vminq_u16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 2> minmax(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE41)
        return {
            vec8x16u{_mm_min_epu16(decay(a), decay(b))},
            vec8x16u{_mm_max_epu16(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto m = a < b;
        return {blend(m, a, b), blend(m, b, a)};

        #endif

        #if defined(AVEL_NEON)
        return {
            vec8x16u{vminq_u16(decay(a), decay(b))},
            vec8x16u{vmaxq_u16(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u clamp(vec8x16u x, vec8x16u lo, vec8x16u hi) {
        return vec8x16u{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u average(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_avg_epu16(decay(a), decay(b))} - ((a ^ b) & vec8x16u{0x1});
        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vhaddq_u16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u midpoint(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_AVX512VL)
        auto t1 = _mm_avg_epu16(decay(a), decay(b));
        auto t5 = _mm_and_si128(_mm_ternarylogic_epi32(decay(a), decay(b), decay(broadcast_mask(b < a)), 0x14), _mm_set1_epi16(0x1));
        auto t6 = _mm_sub_epi16(t1, t5);
        return vec8x16u{t6};

        #elif defined(AVEL_SSE2)
        auto t1 = _mm_avg_epu16(decay(a), decay(b));
        auto t3 = _mm_andnot_si128(decay(broadcast_mask(b <= a)), _mm_xor_si128(decay(a), decay(b)));
        auto t5 = _mm_and_si128(t3, _mm_set1_epi16(0x1));
        auto t6 = _mm_sub_epi16(t1, t5);
        return vec8x16u{t6};

        #endif

        #if defined(AVEL_NEON)
        vec8x16u t0 = vec8x16u{vhaddq_u16(decay(a), decay(b))};
        vec8x16u t1 = (a ^ b) & vec8x16u{0x1} & broadcast_mask(a > b);
        return t0 + t1;

        #endif
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u load<vec8x16u>(const std::uint16_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        n = std::min(8u, n);
        auto mask = 0xFF >> (8 - n);
        return vec8x16u{_mm_maskz_loadu_epi16(mask, ptr)};

        #elif defined(AVEL_SSE2)
        // GCC 9 doesn't have _mm_loadu_si16(), _mm_loadu_si32(), or
        // _mm_loadu_si64() but the following code is optimized down to
        // reasonable machine code.
        switch (n) {
            case 0: {
                return vec8x16u{_mm_setzero_si128()};
            }
            case 1: {
                return vec8x16u{_mm_cvtsi32_si128(ptr[0])};
            }
            case 2: {
                std::int32_t two_s = 0x00;
                two_s |= ptr[0] << 0x00;
                two_s |= ptr[1] << 0x10;
                return vec8x16u{_mm_cvtsi32_si128(two_s)};
            }
            case 3: {
                std::int32_t two_s = 0x00;
                two_s |= ptr[0] << 0x00;
                two_s |= ptr[1] << 0x10;

                std::int32_t one_s = 0x00;
                one_s |= ptr[2] << 0x00;

                auto two_v = _mm_cvtsi32_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec8x16u{_mm_unpacklo_epi32(two_v, one_v)};
            }

            case 4: {
                std::int64_t four_s = 0x00;
                four_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                four_s |= static_cast<std::int64_t>(ptr[1]) << 0x10;
                four_s |= static_cast<std::int64_t>(ptr[2]) << 0x20;
                four_s |= static_cast<std::int64_t>(ptr[3]) << 0x30;

                auto four_v = _mm_cvtsi64_si128(four_s);
                return vec8x16u{four_v};
            }
            case 5: {
                std::int64_t four_s = 0x00;
                four_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                four_s |= static_cast<std::int64_t>(ptr[1]) << 0x10;
                four_s |= static_cast<std::int64_t>(ptr[2]) << 0x20;
                four_s |= static_cast<std::int64_t>(ptr[3]) << 0x30;

                std::int32_t one_s = 0x00;
                one_s |= ptr[4] << 0x00;

                auto four_v = _mm_cvtsi64_si128(four_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec8x16u{_mm_unpacklo_epi64(four_v, one_v)};
            }
            case 6: {
                std::int64_t four_s = 0x00;
                four_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                four_s |= static_cast<std::int64_t>(ptr[1]) << 0x10;
                four_s |= static_cast<std::int64_t>(ptr[2]) << 0x20;
                four_s |= static_cast<std::int64_t>(ptr[3]) << 0x30;

                std::int32_t two_s = 0x00;
                two_s |= ptr[4] << 0x00;
                two_s |= ptr[5] << 0x10;

                auto four_v = _mm_cvtsi64_si128(four_s);
                auto two_v = _mm_cvtsi32_si128(two_s);

                return vec8x16u{_mm_unpacklo_epi64(four_v, two_v)};
            }
            case 7: {
                std::int64_t four_s = 0x00;
                four_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                four_s |= static_cast<std::int64_t>(ptr[1]) << 0x10;
                four_s |= static_cast<std::int64_t>(ptr[2]) << 0x20;
                four_s |= static_cast<std::int64_t>(ptr[3]) << 0x30;

                std::int32_t two_s = 0x00;
                two_s |= ptr[4] << 0x00;
                two_s |= ptr[5] << 0x10;

                std::int32_t one_s = 0x00;
                one_s |= ptr[6] << 0x00;

                auto four_v = _mm_cvtsi64_si128(four_s);
                auto two_v = _mm_cvtsi32_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec8x16u{_mm_unpacklo_epi64(four_v, _mm_unpacklo_epi32(two_v, one_v))};
            }

            default: {
                return vec8x16u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
            }
        }
        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            case 0: {
                return vec8x16u{vdupq_n_u16(0x00)};
            }
            case 1: {
                std::uint16_t x0;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint16_t));

                auto ret0 = vsetq_lane_u16(x0, vdupq_n_u16(0x00), 0);
                return vec8x16u{ret0};
            }
            case 2: {
                std::uint32_t x0;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint32_t));

                auto ret0 = vsetq_lane_u32(x0, vdupq_n_u32(0x00), 0);
                auto ret1 = vreinterpretq_u16_u32(ret0);
                return vec8x16u{ret1};
            }
            case 3: {
                std::uint32_t x0;
                std::uint16_t x1;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint32_t));
                std::memcpy(&x1, ptr + 2, sizeof(std::uint16_t));

                auto ret0 = vsetq_lane_u32(x0, vdupq_n_u32(0x00), 0);
                auto ret1 = vsetq_lane_u16(x1, vreinterpretq_u16_u32(ret0), 2);
                return vec8x16u{ret1};
            }
            case 4: {
                std::uint64_t x0;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));

                auto ret0 = vsetq_lane_u64(x0, vdupq_n_u64(0x00), 0);
                auto ret1 = vreinterpretq_u16_u64(ret0);
                return vec8x16u{ret1};
            }

            case 5: {
                std::uint64_t x0;
                std::uint16_t x1;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));
                std::memcpy(&x1, ptr + 4, sizeof(std::uint16_t));

                auto ret0 = vsetq_lane_u64(x0, vdupq_n_u64(0x00), 0);
                auto ret1 = vsetq_lane_u16(x1, vreinterpretq_u16_u64(ret0), 4);
                return vec8x16u{ret1};
            }
            case 6: {
                std::uint64_t x0;
                std::uint32_t x1;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));
                std::memcpy(&x1, ptr + 4, sizeof(std::uint32_t));

                auto ret0 = vsetq_lane_u64(x0, vdupq_n_u64(0x00), 0);
                auto ret1 = vsetq_lane_u32(x1, vreinterpretq_u32_u64(ret0), 2);
                auto ret2 = vreinterpretq_u16_u32(ret1);
                return vec8x16u{ret2};
            }
            case 7: {
                std::uint64_t x0;
                std::uint32_t x1;
                std::uint16_t x2;

                std::memcpy(&x0, ptr + 0, sizeof(std::uint64_t));
                std::memcpy(&x1, ptr + 4, sizeof(std::uint32_t));
                std::memcpy(&x2, ptr + 6, sizeof(std::uint16_t));

                auto ret0 = vsetq_lane_u64(x0, vdupq_n_u64(0x00), 0);
                auto ret1 = vsetq_lane_u32(x1, vreinterpretq_u32_u64(ret0), 2);
                auto ret2 = vsetq_lane_u16(x2, vreinterpretq_u16_u32(ret1), 6);
                return vec8x16u{ret2};
            }

            default: {
                return vec8x16u{vld1q_u16(ptr)};
            }
        }
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u load<vec8x16u, vec8x16u::width>(const std::uint16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vld1q_u16(ptr)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u aligned_load<vec8x16u>(const std::uint16_t* ptr, std::uint32_t n) {
        return load<vec8x16u>(ptr, n);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u aligned_load<vec8x16u, vec8x16u::width>(const std::uint16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
            #if __cplusplus >= 202002L
            return vec8x16u{vld1q_u16(assume_aligned<alignof(vec8x16u)>(ptr))};

            #elif defined(AVEL_GCC) || defined(AVEL_CLANG)
            auto* p = reinterpret_cast<const std::uint16_t*>(__builtin_assume_aligned(ptr, alignof(vec8x16u)));
            return vec8x16u{vld1q_u16(p)};

            #else
            return vec8x16u{vld1q_u16(ptr)};

            #endif
        #endif
    }



    AVEL_FINL void store(std::uint16_t* ptr, vec8x16u x, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        n = min(n, vec8x16u::width);
        auto mask = (1 << n) - 1;
        _mm_mask_storeu_epi16(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec8x16u::width;
        auto h = vec8x16u::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(16 * (h - min(h, n))));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(16 * (w - min(w, n))));
        auto mask = _mm_unpacklo_epi64(lo, hi);
        _mm_maskmoveu_si128(decay(x), mask, reinterpret_cast<char *>(ptr));
        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            case 0: {} break;
            case 1: {
                std::uint16_t x0 = vgetq_lane_u16(decay(x), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::uint16_t));
            } break;
            case 2: {
                std::uint32_t x0 = vgetq_lane_u32(vreinterpretq_u32_u16(decay(x)), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::uint32_t));
            } break;
            case 3: {
                std::uint32_t x0 = vgetq_lane_u32(vreinterpretq_u32_u16(decay(x)), 0);
                std::uint16_t x1 = vgetq_lane_u16(decay(x), 2);

                std::memcpy(ptr + 0, &x0, sizeof(std::uint32_t));
                std::memcpy(ptr + 2, &x1, sizeof(std::uint16_t));
            } break;

            case 4: {
                std::uint64_t x0 = vgetq_lane_u64(vreinterpretq_u64_u16(decay(x)), 0);

                std::memcpy(ptr + 0, &x0, sizeof(std::uint64_t));
            } break;
            case 5: {
                std::uint64_t x0 = vgetq_lane_u64(vreinterpretq_u64_u16(decay(x)), 0);
                std::uint16_t x1 = vgetq_lane_u16(decay(x), 4);

                std::memcpy(ptr + 0, &x0, sizeof(std::uint64_t));
                std::memcpy(ptr + 4, &x1, sizeof(std::uint16_t));
            } break;
            case 6: {
                std::uint64_t x0 = vgetq_lane_u64(vreinterpretq_u64_u16(decay(x)), 0);
                std::uint32_t x1 = vgetq_lane_u32(vreinterpretq_u32_u16(decay(x)), 2);

                std::memcpy(ptr + 0, &x0, sizeof(std::uint64_t));
                std::memcpy(ptr + 4, &x1, sizeof(std::uint32_t));
            } break;
            case 7: {
                std::uint64_t x0 = vgetq_lane_u64(vreinterpretq_u64_u16(decay(x)), 0);
                std::uint32_t x1 = vgetq_lane_u32(vreinterpretq_u32_u16(decay(x)), 2);
                std::uint16_t x2 = vgetq_lane_u16(decay(x), 6);

                std::memcpy(ptr + 0, &x0, sizeof(std::uint64_t));
                std::memcpy(ptr + 4, &x1, sizeof(std::uint32_t));
                std::memcpy(ptr + 6, &x2, sizeof(std::uint16_t));
            } break;
            default: {
                vst1q_u16(ptr, decay(x));
            }
        }
        #endif
    }

    template<std::uint32_t N = vec8x16u::width>
    AVEL_FINL void store(std::uint16_t* ptr, vec8x16u x) {
        static_assert(N <= vec8x16u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec8x16u::width, int>::type dummy_variable = 0;

        store(ptr, x, N);
    }

    template<>
    AVEL_FINL void store<vec8x16u::width>(std::uint16_t* ptr, vec8x16u x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u16(ptr, decay(x));
        #endif
    }


    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec8x16u x, std::uint32_t n) {
        store(ptr, x, n);
    }

    template<std::uint32_t N = vec8x16u::width>
    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec8x16u x) {
        static_assert(N <= vec8x16u::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec8x16u::width, int>::type dummy_variable = 0;

        aligned_store(ptr, x, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec8x16u::width>(std::uint16_t* ptr, vec8x16u x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_u16(ptr, decay(x));
        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr8x16u to_array(vec8x16u x) {
        alignas(16) arr8x16u ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x16u> div(vec8x16u x, vec8x16u y) {
        #if defined(AVEL_SSE2)
        /*
        vec8x16u quotient{0x00};
        vec8x16u remainder{0x00};

        quotient = insert<0>(quotient, extract<0>(x) / extract<0>(y));
        quotient = insert<1>(quotient, extract<1>(x) / extract<1>(y));
        quotient = insert<2>(quotient, extract<2>(x) / extract<2>(y));
        quotient = insert<3>(quotient, extract<3>(x) / extract<3>(y));
        quotient = insert<4>(quotient, extract<4>(x) / extract<4>(y));
        quotient = insert<5>(quotient, extract<5>(x) / extract<5>(y));
        quotient = insert<6>(quotient, extract<6>(x) / extract<6>(y));
        quotient = insert<7>(quotient, extract<7>(x) / extract<7>(y));

        remainder = insert<0>(remainder, extract<0>(x) % extract<0>(y));
        remainder = insert<1>(remainder, extract<1>(x) % extract<1>(y));
        remainder = insert<2>(remainder, extract<2>(x) % extract<2>(y));
        remainder = insert<3>(remainder, extract<3>(x) % extract<3>(y));
        remainder = insert<4>(remainder, extract<4>(x) % extract<4>(y));
        remainder = insert<5>(remainder, extract<5>(x) % extract<5>(y));
        remainder = insert<6>(remainder, extract<6>(x) % extract<6>(y));
        remainder = insert<7>(remainder, extract<7>(x) % extract<7>(y));

        return {quotient, remainder};
        */

        /*
        std::int32_t i = 16;
        mask8x16u b;

        b = ((x >> 15) >= y);
        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 14) >= y);
        i = 15;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 13) >= y);
        i = 14;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 12) >= y);
        i = 13;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 11) >= y);
        i = 12;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 10) >= y);
        i = 11;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 9) >= y);
        i = 10;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 8) >= y);
        i = 9;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 7) >= y);
        i = 8;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 6) >= y);
        i = 7;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 5) >= y);
        i = 6;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 4) >= y);
        i = 5;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 3) >= y);
        i = 4;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 2) >= y);
        i = 3;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 1) >= y);
        i = 2;

        if (any(b)) {
            goto avel_vec8x16u_middle;
        }

        b = ((x >> 0) >= y);
        i = 1;

        avel_vec8x16u_middle:
        */

        /*
        vec8x16u quotient{0x00};
        std::int32_t i = 0;
        for (; (i-- > 0) && any(x >= y);) {
             b = ((x >> i) >= y);
            x -= (broadcast_mask(b) & (y << i));
            quotient += quotient;
            quotient -= broadcast_mask(b);
        }

        quotient <<= (i + 1);
        return {quotient, x};
        */

        vec8x16u quotient{0x00};
        std::int32_t i = 16;
        for (; (i-- > 0) && any(x >= y);) {
            mask8x16u b = ((x >> i) >= y);
            x -= (broadcast_mask(b) & (y << i));
            quotient += quotient;
            quotient -= broadcast_mask(b);
        }

        quotient <<= (i + 1);

        return {quotient, x};
        #endif



        #if defined(AVEL_NEON)
        //Just to avoid division by zero triggering an exception
        auto denominators = max(y, vec8x16u{1});

        //Falling back to scalar code ends up being faster
        auto x0 = vgetq_lane_u16(decay(x), 0);
        auto x1 = vgetq_lane_u16(decay(x), 1);
        auto x2 = vgetq_lane_u16(decay(x), 2);
        auto x3 = vgetq_lane_u16(decay(x), 3);
        auto x4 = vgetq_lane_u16(decay(x), 4);
        auto x5 = vgetq_lane_u16(decay(x), 5);
        auto x6 = vgetq_lane_u16(decay(x), 6);
        auto x7 = vgetq_lane_u16(decay(x), 7);

        auto y0 = vgetq_lane_u16(decay(y), 0);
        auto y1 = vgetq_lane_u16(decay(y), 1);
        auto y2 = vgetq_lane_u16(decay(y), 2);
        auto y3 = vgetq_lane_u16(decay(y), 3);
        auto y4 = vgetq_lane_u16(decay(y), 4);
        auto y5 = vgetq_lane_u16(decay(y), 5);
        auto y6 = vgetq_lane_u16(decay(y), 6);
        auto y7 = vgetq_lane_u16(decay(y), 7);

        auto quot = vdupq_n_u16(0x00);
        quot = vsetq_lane_u16(x0 / y0, quot, 0);
        quot = vsetq_lane_u16(x1 / y1, quot, 1);
        quot = vsetq_lane_u16(x2 / y2, quot, 2);
        quot = vsetq_lane_u16(x3 / y3, quot, 3);
        quot = vsetq_lane_u16(x4 / y4, quot, 4);
        quot = vsetq_lane_u16(x5 / y5, quot, 5);
        quot = vsetq_lane_u16(x6 / y6, quot, 6);
        quot = vsetq_lane_u16(x7 / y7, quot, 7);

        auto rem = vdupq_n_u16(0x00);
        rem = vsetq_lane_u16(x0 % y0, rem, 0);
        rem = vsetq_lane_u16(x1 % y1, rem, 1);
        rem = vsetq_lane_u16(x2 % y2, rem, 2);
        rem = vsetq_lane_u16(x3 % y3, rem, 3);
        rem = vsetq_lane_u16(x4 % y4, rem, 4);
        rem = vsetq_lane_u16(x5 % y5, rem, 5);
        rem = vsetq_lane_u16(x6 % y6, rem, 6);
        rem = vsetq_lane_u16(x7 % y7, rem, 7);

        return {vec8x16u{quot}, vec8x16u{rem}};
        #endif
    }

    //=====================================================
    // Bit operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x16u popcount(vec8x16u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        return vec8x16u{_mm_popcnt_epi16(decay(x))};

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

        auto partial_sum0 = _mm_shuffle_epi8(table, index0);
        auto partial_sum1 = _mm_shuffle_epi8(table, index1);

        auto byte_sums = _mm_add_epi8(partial_sum0, partial_sum1);

        auto short_sums = _mm_add_epi16(byte_sums, _mm_slli_epi16(byte_sums, 8));
        auto ret = _mm_srli_epi16(short_sums, 8);

        return vec8x16u{ret};

        #elif defined(AVEL_SSE2)
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & vec8x16u{0x5555});
        x = (x & vec8x16u{0x3333}) + ((x >> 2) & vec8x16u{0x3333});
        x = ((x + (x >> 4) & vec8x16u{0x0F0F}) * vec8x16u{0x0101}) >> 8;
        return x;

        #endif

        #if defined(AVEL_NEON)
        auto t0 = vreinterpretq_u16_u8(vcntq_u8(vreinterpretq_u8_u16(decay(x))));
        auto t1 = vshlq_n_u16(t0, 8);
        auto t2 = vaddq_u16(t0, t1);
        auto t3 = vshrq_n_u16(t2, 8);
        return vec8x16u{t3};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u countl_zero(vec8x16u x) {
        #if defined(AVEL_SSE2)
        return countl_one(~x);
        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vclzq_u16(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u countl_one(vec8x16u x) {
        #if defined(AVEL_SSE2)
        vec8x16u sum{x == vec8x16u{0xFFFF}};

        vec8x16u m0{0xFF00u};
        mask8x16u b0 = (m0 & x) == m0;
        sum += broadcast_mask(b0) & vec8x16u{8};
        x <<= broadcast_mask(b0) & vec8x16u{8};

        vec8x16u m1{0xF000u};
        mask8x16u b1 = (m1 & x) == m1;
        sum += broadcast_mask(b1) & vec8x16u{4};
        x <<= broadcast_mask(b1) & vec8x16u{4};

        vec8x16u m2{0xC000u};
        mask8x16u b2 = (m2 & x) == m2;
        sum += broadcast_mask(b2) & vec8x16u{2};
        x <<= broadcast_mask(b2) & vec8x16u{2};

        vec8x16u m3{0x8000u};
        mask8x16u b3 = (m3 & x) == m3;
        sum += broadcast_mask(b3) & vec8x16u{1};

        return sum;
        #endif

        #if defined(AVEL_NEON)
        return countl_zero(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u countr_zero(vec8x16u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        auto undef = _mm_undefined_si128();
        auto neg_one = _mm_cmpeq_epi16(undef, undef);
        auto tz_mask = _mm_andnot_si128(decay(x), _mm_add_epi16(decay(x), neg_one));
        return vec8x16u{_mm_popcnt_epi16(tz_mask)};

        #elif defined(AVEL_SSE2)
        mask8x16u zero_mask = (x == vec8x16u{0x0000});

        x &= vec8x16u{0x0000} - x;

        vec8x16u ret = vec8x16u{16} & broadcast_mask(x == vec8x16u{0x00});

        mask8x16u b;
        b = mask8x16u(x & vec8x16u{0xAAAAu});
        ret |= (vec8x16u{b} << 0);
        b = mask8x16u(x & vec8x16u{0xCCCCu});
        ret |= (vec8x16u{b} << 1);
        b = mask8x16u(x & vec8x16u{0xF0F0u});
        ret |= (vec8x16u{b} << 2);
        b = mask8x16u(x & vec8x16u{0xFF00u});
        ret |= (vec8x16u{b} << 3);

        return ret;
        #endif


        #if defined(AVEL_NEON)&& defined(AVEL_AARCH64)
        auto reversed_bytes = vrev16q_u8(vreinterpretq_u8_u16(decay(x)));
        auto reversed_bits = vrbitq_u8(reversed_bytes);
        return vec8x16u{vclzq_u16(vreinterpretq_u16_u8(reversed_bits))};

        #elif defined(AVEL_NEON)
        auto zero_mask = (x == vec8x16u{0x00});
        x &= vec8x16u{0x00} - x;

        auto ret = (vec8x16u{15} - countl_zero(x));
        ret = blend(zero_mask, vec8x16u{16}, ret);
        return ret;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u countr_one(vec8x16u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u bit_width(vec8x16u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto widened = _mm256_cvtepu16_epi32(decay(x));
        auto counts32 = _mm256_lzcnt_epi32(widened);
        auto counts16 = _mm256_cvtepi32_epi16(counts32);

        return vec8x16u{32} - vec8x16u{counts16};

        #elif defined(AVEL_SSSE3)
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

        auto nibble_mask = _mm_set1_epi8(0x0F);

        auto nibble0 = _mm_and_si128(nibble_mask, decay(x));
        auto nibble1 = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x4));
        auto nibble2 = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x8));
        auto nibble3 = _mm_srli_epi16(decay(x), 0xC);

        auto table_incrementers = _mm_load_si128(reinterpret_cast<const __m128i*>(table_increments_data));
        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto table1 = _mm_add_epi16(table0, table_incrementers);
        auto table2 = _mm_add_epi16(table1, table_incrementers);
        auto table3 = _mm_add_epi16(table2, table_incrementers);

        auto partial0 = _mm_shuffle_epi8(table0, nibble0);
        auto partial1 = _mm_shuffle_epi8(table1, nibble1);
        auto partial2 = _mm_shuffle_epi8(table2, nibble2);
        auto partial3 = _mm_shuffle_epi8(table3, nibble3);

        auto partial4 = _mm_max_epu8(partial0, partial1);
        auto partial5 = _mm_max_epu8(partial2, partial3);
        auto partial6 = _mm_max_epu8(partial4, partial5);

        auto ret = _mm_and_si128(partial6, _mm_set1_epi16(0x00FF));

        return vec8x16u{ret};

        #elif defined(AVEL_SSE2)
        mask8x16u zero_mask = (x == vec8x16u{0x0000});

        vec8x16u ret{0x0000};

        auto b0 = mask8x16u{x & vec8x16u{0xFF00}};
        auto t0 = broadcast_mask(b0) & vec8x16u{8};
        ret += t0;
        x >>= t0;

        auto b1 = mask8x16u{x & vec8x16u{0xFFF0}};
        auto t1 = broadcast_mask(b1) & vec8x16u{4};
        ret += t1;
        x >>= t1;

        auto b2 = mask8x16u{x & vec8x16u{0xFFFC}};
        auto t2 = broadcast_mask(b2) & vec8x16u{2};
        ret += t2;
        x >>= t2;

        auto b3 = mask8x16u{x & vec8x16u{0xFFFE}};
        auto t3 = broadcast_mask(b3) & vec8x16u{1};
        ret += t3;

        return blend(zero_mask, vec8x16u{0}, ret + vec8x16u{1});
        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{16} - countl_zero(x);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u bit_floor(vec8x16u v) {
        #if defined(AVEL_SSE2)
        v |= (v >> 1);
        v |= (v >> 2);
        v |= (v >> 4);
        v |= (v >> 8);
        v -= (v >> 1);

        return v;
        #endif

        #if defined(AVEL_NEON)
        mask8x16u zero_mask = (v != vec8x16u{0x00});
        vec8x16u leading_zeros = countl_zero(v);

        return (vec8x16u{zero_mask} << (vec8x16u{15} - leading_zeros));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u bit_ceil(vec8x16u v) {
        #if defined(AVEL_SSE2)
        auto zero_mask = (v == vec8x16u{0x00});

        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        ++v;

        return v - broadcast_mask(zero_mask);

        #endif

        #if defined(AVEL_NEON)
        auto sh = (vec8x16u{16} - countl_zero(v - vec8x16u{1}));
        auto result = vec8x16u{1} << sh;
        return result - broadcast_mask(v == vec8x16u{0x00});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x16u has_single_bit(vec8x16u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        return popcount(v) == vec8x16u{1};

        #else
        return !mask8x16u{v & (v - vec8x16u{1})} && mask8x16u{v};

        #endif
    }

}

#endif //AVEL_VEC8X16U_HPP
