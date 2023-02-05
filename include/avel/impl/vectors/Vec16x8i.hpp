#ifndef AVEL_VEC16X8I_HPP
#define AVEL_VEC16X8I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x8i = Vector<std::int8_t, 16>;
    using arr16x8i = std::array<std::int8_t, 16>;
    using mask16x8i = Vector_mask<std::int8_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x8i> div(vec16x8i numerator, vec16x8i denominator);
    vec16x8i broadcast_mask(mask16x8i m);
    vec16x8i blend(mask16x8i m, vec16x8i a, vec16x8i b);
    vec16x8i countl_one(vec16x8i x);





    template<>
    class Vector_mask<std::int8_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        static constexpr std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        using primitive = __mmask16;
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint8x16_t;
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
        #elif defined(AVEL_SSE2)
            content(b ? _mm_set1_epi8(-1) : _mm_setzero_si128()) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u8(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr16xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = primitive(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_SSE2)
            primitive array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = _mm_cmplt_epi8(_mm_setzero_si128(), array_data);

            #endif

            #if defined(AVEL_NEON)
            auto array_data = vld1q_u8(bit_cast<const std::uint8_t*>(arr.data()));
            content = vcltq_u8(vdupq_n_u8(0x00), array_data);
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

            #elif defined(AVEL_SSE2)
            content = b ? _mm_set1_epi8(-1) : _mm_setzero_si128();

            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_u8(b ? -1 : 0);
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
            return decay(lhs) == decay(rhs);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u8(vceqq_u8(decay(lhs), decay(rhs)));
            return min == 0xFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return decay(lhs) != decay(rhs);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u8(vceqq_u8(decay(lhs), decay(rhs)));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(vreinterpretq_u64_u8(decay(lhs)), vreinterpretq_u64_u8(decay(rhs)));
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
            content = _mm_and_si128(content, decay(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = vandq_u8(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content |= decay(rhs);

            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, decay(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u8(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content ^= decay(rhs);

            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, decay(rhs));

            #endif

            #if defined(AVEL_NEON)
            content = veorq_u8(content, decay(rhs));
            #endif

            return *this;
        }

        //=================================================
        // Bitwise operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector_mask{static_cast<primitive>(~content)};

            #elif defined(AVEL_AVX512VL)
            return Vector_mask{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            auto full = _mm_cmpeq_epi8(undef, undef);
            return Vector_mask{_mm_andnot_si128(content, full)};

            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vmvnq_u8(content)};
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
    AVEL_FINL std::uint32_t count(mask16x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(_mm512_mask2int(decay(m)));

        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m)));

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vsubq_u8(vdupq_n_u8(0x00), decay(m));
        return static_cast<std::uint32_t>(vaddvq_u8(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vsubq_u8(vdupq_n_u8(0x00), decay(m));
        auto t1 = vpadd_u8(vget_low_u8(t0), vget_high_u8(t0));
        auto t2 = vpadd_u8(t1, t1);
        auto t3 = vpadd_u8(t2, t2);
        auto t4 = vpadd_u8(t3, t3);

        return static_cast<std::uint32_t>(vget_lane_u8(t4, 0));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask16x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vmaxvq_u8(decay(m)) != 0x00;

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u8(decay(m));
        auto t1 = vpmax_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmax_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) != 0x00;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask16x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm512_mask2int(decay(m));
        return 0xFFFF == t0;
        #elif defined(AVEL_SSE41)
        return _mm_test_all_ones(decay(m));
        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vminvq_u8(decay(m)) == 0xFF;

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u8(decay(m));
        auto t1 = vpmin_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmin_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) == 0xFFFFFFFF;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x8i m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask16x8i>(mask16x8i m) {
        return {m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask16x8i>(mask16x8i m) {
        return {mask16x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask16x8u>(mask16x8u m) {
        return {mask16x8i(decay(m))};
    }






    template<>
    class alignas(16) Vector<std::int8_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int8_t;

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int8x16_t;
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
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content(_mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi8(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vreinterpretq_s8_u8(vsubq_u8(vdupq_n_u8(0x00), decay(m)))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            content(_mm_broadcastb_epi8(_mm_cvtsi32_si128(x))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_set1_epi8(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_s8(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr16x8i& array):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_s8(array.data())) {}
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
            content = _mm_set1_epi8(x);
            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_s8(x);
            #endif
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
            return mask{_mm_cmpeq_epi8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi8(decay(lhs), decay(rhs))};

            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_s8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpneq_epi8_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpeq_epi8(decay(lhs), decay(rhs))};

            #endif

            #if defined(AVEL_NEON)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmplt_epi8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_epi8(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_s8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmple_epi8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpgt_epi8(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_s8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpgt_epi8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_epi8(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_s8(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpge_epi8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmplt_epi8(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_s8(decay(lhs), decay(rhs))};
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
            #if defined(AVEL_SSE2)
            return Vector{0x00} - *this;
            #endif

            #if defined(AVEL_NEON)
            return Vector{vnegq_s8(content)};
            #endif
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi8(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_s8(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi8(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_s8(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto a = _mm256_cvtepu8_epi16(content);
            auto b = _mm256_cvtepu8_epi16(rhs.content);

            auto c = _mm256_mullo_epi16(a, b);
            content = _mm256_cvtepi16_epi8(c);

            #elif defined(AVEL_AVX2)
            auto lhs_whole = _mm256_cvtepu8_epi16(content);
            auto rhs_whole = _mm256_cvtepu8_epi16(rhs.content);

            auto mask = _mm256_set1_epi16(0x00FF);
            auto product = _mm256_mullo_epi16(lhs_whole, rhs_whole);
            auto masked = _mm256_and_si256(product, mask);
            auto packed = _mm256_packus_epi16(masked, _mm256_set_m128i(_mm_setzero_si128(), (_mm256_extractf128_si256(masked, 0x1))));

            content = _mm256_castsi256_si128(packed);

            #elif defined(AVEL_SSE2)
            primitive zeros = _mm_setzero_si128();
            primitive byte_mask = _mm_set1_epi16(0x00FF);
            primitive lhs_whole = content;
            primitive rhs_whole = rhs.content;

            primitive lhs_lo = _mm_unpacklo_epi8(lhs_whole, zeros);
            primitive lhs_hi = _mm_unpackhi_epi8(lhs_whole, zeros);

            primitive rhs_lo = _mm_unpacklo_epi8(rhs_whole, zeros);
            primitive rhs_hi = _mm_unpackhi_epi8(rhs_whole, zeros);

            primitive out_lo = _mm_mullo_epi16(lhs_lo, rhs_lo);
            primitive out_hi = _mm_mullo_epi16(lhs_hi, rhs_hi);

            out_lo = _mm_and_si128(byte_mask, out_lo);
            out_hi = _mm_and_si128(byte_mask, out_hi);

            content = _mm_packus_epi16(out_lo, out_hi);

            #endif

            #if defined(AVEL_NEON)
            content = vmulq_s8(content, decay(rhs));

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

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_s8(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_s8(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_s8(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto tmp = _mm256_cvtepi8_epi16(content);
            tmp = _mm256_sll_epi16(tmp, _mm_cvtsi32_si128(rhs));
            content = _mm256_cvtepi16_epi8(tmp);

            #elif defined(AVEL_AVX2)
            auto whole = _mm256_cvtepu8_epi16(content);
            auto shifted = _mm256_sll_epi16(whole, _mm_cvtsi32_si128(rhs));
            auto masked = _mm256_and_si256(shifted, _mm256_set1_epi16(0x00FF));

            auto lo = _mm256_extracti128_si256(masked, 0x0);
            auto hi = _mm256_extracti128_si256(masked, 0x1);
            content = _mm_packus_epi16(lo, hi);

            #elif defined(AVEL_SSE2)
            auto s = _mm_cvtsi32_si128(rhs);

            auto shifted = _mm_sll_epi16(content, s);
            auto mask = _mm_set1_epi8(std::uint8_t(0xFF << rhs));
            content = _mm_and_si128(mask, shifted);

            #endif

            #if defined(AVEL_NEON)
            auto s = (std::uint64_t(rhs) > 0xFF) ? 0xFF : static_cast<std::uint8_t>(rhs);
            *this = vshlq_s8(decay(*this), vdupq_n_s8(s));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto tmp = _mm256_cvtepi8_epi16(content);
            tmp = _mm256_sra_epi16(tmp, _mm_cvtsi32_si128(rhs));
            content = _mm256_cvtepi16_epi8(tmp);

            #elif defined(AVEL_AVX2)
            auto tmp = _mm256_cvtepi8_epi16(content);
            tmp = _mm256_sra_epi16(tmp, _mm_cvtsi32_si128(rhs));

            auto lo = _mm256_castsi256_si128(tmp);
            auto hi = _mm256_extractf128_si256(tmp, 0x1);

            content = _mm_packs_epi16(lo, hi);

            #elif defined(AVEL_SSE2)
            primitive sign_bits = _mm_cmplt_epi8(content, _mm_setzero_si128());
            primitive lo = _mm_unpacklo_epi8(content, sign_bits);
            primitive hi = _mm_unpackhi_epi8(content, sign_bits);

            primitive s = _mm_cvtsi32_si128(rhs);
            primitive lo_shifted = _mm_sra_epi16(lo, s);
            primitive hi_shifted = _mm_sra_epi16(hi, s);

            primitive result = _mm_packs_epi16(lo_shifted, hi_shifted);

            content = result;

            #endif

            #if defined(AVEL_NEON)
            content =  vshlq_s8(content, vdupq_n_s8(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto whole = _mm256_cvtepu8_epi16(content);
            auto shifts = _mm256_cvtepu8_epi16(rhs.content);
            content = _mm256_cvtepi16_epi8(_mm256_sllv_epi16(whole, shifts));

            #elif defined(AVEL_AVX2)
            auto lhs_lo = _mm256_cvtepu8_epi32(content);
            auto lhs_hi = _mm256_cvtepu8_epi32(_mm_srli_si128(content, 0x8));

            auto rhs_lo = _mm256_cvtepu8_epi32(decay(rhs));
            auto rhs_hi = _mm256_cvtepu8_epi32(_mm_srli_si128(decay(rhs), 0x8));

            auto lo_shifted = _mm256_sllv_epi32(lhs_lo, rhs_lo);
            auto hi_shifted = _mm256_sllv_epi32(lhs_hi, rhs_hi);

            auto byte_mask = _mm256_set1_epi32(0x000000FF);
            auto lo_results = _mm256_and_si256(lo_shifted, byte_mask);
            auto hi_results = _mm256_and_si256(hi_shifted, byte_mask);

            auto results16 = _mm256_packus_epi32(lo_results, hi_results);

            auto lo = _mm256_extracti128_si256(results16, 0x0);
            auto hi = _mm256_extracti128_si256(results16, 0x1);
            auto results_out_of_order = _mm_packus_epi16(lo, hi);

            content = _mm_shuffle_epi32(results_out_of_order, 0xD8);

            #elif defined(AVEL_SSSE3)
            alignas(16) static constexpr std::uint8_t table_data[16] {
                0x01, 0x02, 0x04, 0x08,
                0x10, 0x20, 0x40, 0x80,
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00
            };

            auto zeros = _mm_setzero_si128();
            auto table = _mm_load_si128(reinterpret_cast<const primitive*>(table_data));

            auto lhs_lo = _mm_unpacklo_epi8(content, zeros);
            auto lhs_hi = _mm_unpackhi_epi8(content, zeros);

            auto valid_mask = _mm_cmplt_epi8(_mm_set1_epi8(0x7), decay(rhs));
            auto indices = _mm_or_si128(decay(rhs), valid_mask);
            auto shifts = _mm_shuffle_epi8(table, indices);

            auto rhs_lo = _mm_unpacklo_epi8(shifts, zeros);
            auto rhs_hi = _mm_unpackhi_epi8(shifts, zeros);

            lhs_lo = _mm_mullo_epi16(lhs_lo, rhs_lo);
            lhs_hi = _mm_mullo_epi16(lhs_hi, rhs_hi);

            auto byte_mask = _mm_set1_epi16(0xFF);
            lhs_lo = _mm_and_si128(lhs_lo, byte_mask);
            lhs_hi = _mm_and_si128(lhs_hi, byte_mask);

            content = _mm_packus_epi16(lhs_lo, lhs_hi);

            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto a = _mm_cmpeq_epi8(_mm_and_si128(decay(rhs), _mm_set1_epi8(0x01)), zeros);
            content = _mm_add_epi8(content, _mm_andnot_si128(a, content));

            auto b = _mm_cmpeq_epi8(_mm_and_si128(decay(rhs), _mm_set1_epi8(0x02)), zeros);
            content = _mm_add_epi8(content, _mm_andnot_si128(b, content));
            content = _mm_add_epi8(content, _mm_andnot_si128(b, content));

            auto c = _mm_cmpeq_epi8(_mm_and_si128(decay(rhs), _mm_set1_epi8(0x04)), zeros);
            auto tmp = _mm_and_si128(_mm_slli_epi16(content, 4), _mm_set1_epi16(0xF0FF));
            content = _mm_or_si128(_mm_andnot_si128(c, tmp), _mm_and_si128(c, content));

            content = _mm_andnot_si128(_mm_cmpgt_epi8(decay(rhs), _mm_set1_epi8(0x7)), content);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s8(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto whole = _mm256_cvtepi8_epi16(content);
            auto shifts = _mm256_cvtepi8_epi16(rhs.content);
            auto shifted = _mm256_srav_epi16(whole, shifts);
            content = _mm256_cvtepi16_epi8(shifted);

            //TODO: Offer AVX2 version?

            #elif defined(AVEL_SSE41)
            auto zeros = _mm_setzero_si128();
            auto non_zero_mask = _mm_cmplt_epi8(decay(rhs), _mm_set1_epi8(8));
            auto neg_mask = _mm_cmplt_epi8(content, zeros);
            auto preserve_mask = _mm_set1_epi16(0x00FF);

            for (unsigned i = 0; i < 3; ++i) {
                auto t0 = _mm_and_si128(decay(rhs), _mm_set1_epi8(1u << i));
                auto m = _mm_cmplt_epi8(zeros, t0);

                auto s = _mm_cvtsi64_si128(1u << i);
                preserve_mask = _mm_blendv_epi8(preserve_mask, _mm_srl_epi16(preserve_mask, s), m);
                content = _mm_blendv_epi8(content, _mm_sra_epi16(content, s), m);
            }

            preserve_mask = _mm_or_si128(preserve_mask, _mm_set1_epi16(0xFF00));
            preserve_mask = _mm_and_si128(non_zero_mask, preserve_mask);

            auto invert_mask = _mm_xor_si128(content, neg_mask);
            auto flip_mask = _mm_andnot_si128(preserve_mask, invert_mask);

            content = _mm_xor_si128(content, flip_mask);
            content = _mm_and_si128(content, _mm_or_si128(non_zero_mask, neg_mask));

            #elif defined(AVEL_SSSE3)
            alignas(16) static constexpr std::uint8_t table_data[16] {
                0x80, 0x40, 0x20, 0x10,
                0x08, 0x04, 0x02, 0x01,
                0x01, 0x01, 0x01, 0x01,
                0x01, 0x01, 0x01, 0x01
            };

            auto lo = _mm_unpacklo_epi8(content, content);
            auto hi = _mm_unpackhi_epi8(content, content);

            lo = _mm_srai_epi16(lo, 8);
            hi = _mm_srai_epi16(hi, 8);

            auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
            auto shift_factors = _mm_shuffle_epi8(table, decay(rhs));

            auto zeros = _mm_setzero_si128();
            auto shift_factors_lo = _mm_unpacklo_epi8(shift_factors, zeros);
            auto shift_factors_hi = _mm_unpackhi_epi8(shift_factors, zeros);

            lo = _mm_mullo_epi16(lo, shift_factors_lo);
            hi = _mm_mullo_epi16(hi, shift_factors_hi);

            lo = _mm_srai_epi16(lo, 0x7);
            hi = _mm_srai_epi16(hi, 0x7);

            content = _mm_packs_epi16(lo, hi);

            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();


            auto negative_mask = _mm_cmplt_epi8(content, zeros);
            content = _mm_xor_si128(content, negative_mask);

            auto non_zero_mask = _mm_cmplt_epi8(decay(rhs), _mm_set1_epi8(8));
            content = _mm_and_si128(content, non_zero_mask);


            auto m0 = _mm_set1_epi16(0xFF0F);
            auto c0 = _mm_cmplt_epi8(_mm_slli_epi16(decay(rhs), 5), zeros);
            content = _mm_max_epu8(_mm_andnot_si128(c0, content), _mm_and_si128(_mm_srli_epi16(content, 4), m0));

            auto m1 = _mm_set1_epi16(0xFF3F);
            auto c1 = _mm_cmplt_epi8(_mm_slli_epi16(decay(rhs), 6), zeros);
            content = _mm_max_epu8(_mm_andnot_si128(c1, content), _mm_and_si128(_mm_srli_epi16(content, 2), m1));

            auto m2 = _mm_set1_epi16(0xFF7F);
            auto c2 = _mm_cmplt_epi8(_mm_slli_epi16(decay(rhs), 7), zeros);
            content = _mm_max_epu8(_mm_andnot_si128(c2, content), _mm_and_si128(_mm_srli_epi16(content, 1), m2));

            content = _mm_xor_si128(content, negative_mask);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s8(content, vnegq_s8(decay(rhs)));

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
            return Vector{vmvnq_s8(content)};
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
            return Vector{} != *this;
        }

    };

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec16x8i operator-(vec16x8u v) {
        return vec16x8i{0} - vec16x8i{v};
    }

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int8_t extract(vec16x8i v) {
        static_assert(N <= vec16x8i::width, "Specified index does not exist");
        typename std::enable_if<N <= vec16x8i::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SS41)
        return _mm_extract_epi8(decay(v), N);

        #elif defined(AVEL_SSE2)
        return _mm_cvtsi128_si32(_mm_srli_si128(decay(v), N)) & 0xFF;

        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec16x8i x) {
        return count(vec16x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec16x8i x) {
        return any(vec16x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec16x8i x) {
        return all(vec16x8u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec16x8i x) {
        return none(vec16x8u{x});
    }
    */

    [[nodiscard]]
    AVEL_FINL vec16x8i broadcast_mask(mask16x8i m) {
        return vec16x8i{broadcast_mask(mask16x8u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i blend(mask16x8i m, vec16x8i a, vec16x8i b) {
        return vec16x8i{blend(mask16x8u{m}, vec16x8u{a}, vec16x8u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i max(vec16x8i a, vec16x8i b) {
        #if defined(AVEL_SSE41)
        return vec16x8i{_mm_max_epi8(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec16x8i{vmaxq_s8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i min(vec16x8i a, vec16x8i b) {
        #if defined(AVEL_SSE41)
        return vec16x8i{_mm_min_epi8(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, a, b);

        #endif

        #if defined(AVEL_NEON)
        return vec16x8i{vminq_s8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 2> minmax(vec16x8i a, vec16x8i b) {
        #if defined(AVEL_SSE41)
        return {
            vec16x8i{_mm_min_epi8(decay(a), decay(b))},
            vec16x8i{_mm_max_epi8(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto mask = a < b;

        return {
            blend(mask, a, b),
            blend(mask, b, a)
        };

        #endif

        #if defined(AVEL_NEON)
        return {
            vec16x8i{vminq_s8(decay(a), decay(b))},
            vec16x8i{vmaxq_s8(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i clamp(vec16x8i x, vec16x8i lo, vec16x8i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i average(vec16x8i x, vec16x8i y) {
        #if defined(AVEL_SSE2)
        auto avg = (x & y) + ((x ^ y) >> 1);
        auto c = broadcast_mask((x < -y) | (y == vec16x8i{std::int8_t(1 << 7)})) & (x ^ y) & vec16x8i{1};

        return avg + c;

        #endif

        #if defined(AVEL_NEON)
        auto avg = vec16x8i{vhaddq_s8(decay(x), decay(y))};
        auto c = broadcast_mask((x < -y) | (y == vec16x8i{std::int8_t(1 << 7)})) & (x ^ y) & vec16x8i{1};

        return avg + c;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i midpoint(vec16x8i a, vec16x8i b) {
        //TODO: Leverage newer instruction sets
        #if defined(AVEL_SSE2)
        auto offset = _mm_set1_epi8(0x80);

        auto a_offset = _mm_xor_si128(decay(a), offset);
        auto b_offset = _mm_xor_si128(decay(b), offset);

        auto average_offset = _mm_avg_epu8(a_offset, b_offset);
        auto average = _mm_xor_si128(average_offset, offset);

        auto m = _mm_cmplt_epi8(decay(a), decay(b));
        auto bias = _mm_and_si128(_mm_xor_si128(decay(a), decay(b)), _mm_and_si128(m, _mm_set1_epi8(0x01)));

        return vec16x8i{_mm_sub_epi8(average, bias)};
        #endif

        #if defined(AVEL_NEON)
        vec16x8i t0 = vec16x8i{vhaddq_s8(decay(a), decay(b))};
        vec16x8i t1 = (a ^ b) & vec16x8i{0x1} & broadcast_mask(b < a);
        return t0 + t1;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i negate(mask16x8i m, vec16x8i x) {
        auto mask = broadcast_mask(m);
        return (x ^ mask) - mask;
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i abs(vec16x8i x) {
        #if defined(AVEL_SSSE3)
        return vec16x8i{_mm_abs_epi8(decay(x))};
        #elif defined(AVEL_SSE2)
        auto y = x >> 7;
        return (x ^ y) - y;
        #endif

        #if defined(AVEL_NEON)
        auto zeros = vdupq_n_s8(0x00);
        return vec16x8i{vabdq_s8(decay(x), zeros)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i neg_abs(vec16x8i x) {
        #if defined(AVEL_SSE2)
        auto y = ~x >> 7;
        return (x ^ y) - y;
        #endif

        #if defined(AVEL_NEON)
        auto y = vmvnq_s8(vshrq_n_s8(decay(x), 7));
        return vec16x8i{vsubq_s8(veorq_s8(decay(x), y), y)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i neg_abs(vec16x8u x) {
        return neg_abs(vec16x8i{x});
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i load<vec16x8i>(const std::int8_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        n = std::min(16u, n);
        auto mask = 0xFFFF >> (16 - n);
        return vec16x8i{_mm_maskz_loadu_epi8(mask, ptr)};

        #elif defined(AVEL_SSE2)
        // GCC 9 doesn't have _mm_loadu_si16(), _mm_loadu_si32(), or
        // _mm_loadu_si64() but the following code is optimized down to
        // reasonable machine code.
        switch (n) {
            case 0: {
                return vec16x8i{_mm_setzero_si128()};
            }
            case 1: {
                return vec16x8i{_mm_cvtsi32_si128(ptr[0])};
            }
            case 2: {
                std::int32_t two = 0x00;
                two |= ptr[0] << 0x0;
                two |= ptr[1] << 0x8;
                return vec16x8i{_mm_cvtsi32_si128(two)};
            }
            case 3: {
                std::int32_t two_s = 0x00;
                two_s |= ptr[0] << 0x00;
                two_s |= ptr[1] << 0x08;

                std::int32_t one = 0x00;
                one |= ptr[2] << 0x00;

                auto lo = _mm_cvtsi32_si128(two_s);
                auto hi = _mm_cvtsi32_si128(one);

                return vec16x8i{_mm_unpacklo_epi16(lo, hi)};
            }

            case 4: {
                std::int32_t four_s = 0x00;
                four_s |= ptr[0] << 0x00;
                four_s |= ptr[1] << 0x08;
                four_s |= ptr[2] << 0x10;
                four_s |= ptr[3] << 0x18;
                return vec16x8i{_mm_cvtsi32_si128(four_s)};
            }
            case 5: {
                std::int32_t four_s = 0x00;
                four_s |= ptr[0] << 0x00;
                four_s |= ptr[1] << 0x08;
                four_s |= ptr[2] << 0x10;
                four_s |= ptr[3] << 0x18;

                std::int32_t one_s = 0x00;
                one_s |= ptr[4] << 0x00;

                auto four_v = _mm_cvtsi32_si128(four_s);
                auto one_v  = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi32(four_v, one_v)};
            }
            case 6: {
                std::int32_t four_s = 0x00;
                four_s |= ptr[0] << 0x00;
                four_s |= ptr[1] << 0x08;
                four_s |= ptr[2] << 0x10;
                four_s |= ptr[3] << 0x18;

                std::int32_t two_s = 0x00;
                two_s |= ptr[4] << 0x00;
                two_s |= ptr[5] << 0x08;

                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v  = _mm_cvtsi32_si128(two_s);

                return vec16x8i{_mm_unpacklo_epi32(four_v, two_v)};
            }
            case 7:{
                std::int32_t four_s = 0x00;
                four_s |= ptr[0] << 0x00;
                four_s |= ptr[1] << 0x08;
                four_s |= ptr[2] << 0x10;
                four_s |= ptr[3] << 0x18;

                std::int32_t two_s = 0x00;
                two_s |= ptr[4] << 0x00;
                two_s |= ptr[5] << 0x08;

                std::int32_t one_s = 0x00;
                one_s |= ptr[6] << 0x00;

                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v  = _mm_cvtsi32_si128(two_s);
                auto one_v  = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi32(four_v, _mm_unpacklo_epi16(two_v, one_v))};
            }

            case 8: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                return vec16x8i{_mm_cvtsi64_si128(eight_s)};
            }
            case 9: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t one_s = 0x00;
                one_s |= ptr[8] << 0x00;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, one_v)};
            }
            case 10: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t two_s = 0x00;
                two_s |= ptr[8] << 0x00;
                two_s |= ptr[9] << 0x08;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto two_v = _mm_cvtsi32_si128(two_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, two_v)};
            }
            case 11: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t two_s = 0x00;
                two_s |= ptr[8] << 0x00;
                two_s |= ptr[9] << 0x08;

                std::int32_t one_s = 0x00;
                one_s |= ptr[10] << 0x00;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto two_v = _mm_cvtsi32_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi16(two_v, one_v))};
            }

            case 12: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t four_s = 0x00;
                four_s |= ptr[8]  << 0x00;
                four_s |= ptr[9]  << 0x08;
                four_s |= ptr[10] << 0x10;
                four_s |= ptr[11] << 0x18;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, four_v)};
            }
            case 13: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t four_s = 0x00;
                four_s |= ptr[8]  << 0x00;
                four_s |= ptr[9]  << 0x08;
                four_s |= ptr[10] << 0x10;
                four_s |= ptr[11] << 0x18;

                std::int32_t one_s = 0x00;
                one_s |= ptr[12] << 0x00;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi32(four_v, one_v))};
            }
            case 14: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t four_s = 0x00;
                four_s |= ptr[8]  << 0x00;
                four_s |= ptr[9]  << 0x08;
                four_s |= ptr[10] << 0x10;
                four_s |= ptr[11] << 0x18;

                std::int32_t two_s = 0x00;
                two_s |= ptr[12] << 0x00;
                two_s |= ptr[13] << 0x08;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v = _mm_cvtsi32_si128(two_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi32(four_v, two_v))};
            }
            case 15: {
                std::int64_t eight_s = 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[0]) << 0x00;
                eight_s |= static_cast<std::int64_t>(ptr[1]) << 0x08;
                eight_s |= static_cast<std::int64_t>(ptr[2]) << 0x10;
                eight_s |= static_cast<std::int64_t>(ptr[3]) << 0x18;
                eight_s |= static_cast<std::int64_t>(ptr[4]) << 0x20;
                eight_s |= static_cast<std::int64_t>(ptr[5]) << 0x28;
                eight_s |= static_cast<std::int64_t>(ptr[6]) << 0x30;
                eight_s |= static_cast<std::int64_t>(ptr[7]) << 0x38;

                std::int32_t four_s = 0x00;
                four_s |= ptr[8]  << 0x00;
                four_s |= ptr[9]  << 0x08;
                four_s |= ptr[10] << 0x10;
                four_s |= ptr[11] << 0x18;

                std::int32_t two_s = 0x00;
                two_s |= ptr[12] << 0x00;
                two_s |= ptr[13] << 0x08;

                std::int32_t one_s = 0x00;
                one_s |= ptr[14] << 0x00;

                auto eight_v = _mm_cvtsi64_si128(eight_s);
                auto four_v = _mm_cvtsi32_si128(four_s);
                auto two_v = _mm_cvtsi32_si128(two_s);
                auto one_v = _mm_cvtsi32_si128(one_s);

                return vec16x8i{_mm_unpacklo_epi64(eight_v, _mm_unpacklo_epi32(four_v, _mm_unpacklo_epi16(two_v, one_v)))};
            }

            default: {
                return vec16x8i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
            }
        }
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i load<vec16x8i, vec16x8i::width>(const std::int8_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec16x8i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec16x8i{vld1q_s8(ptr)};
        #endif
    }


    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i aligned_load<vec16x8i>(const std::int8_t* ptr, std::uint32_t n) {
        #if defined(AVEL_SSE2)
        return vec16x8i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i aligned_load<vec16x8i, vec16x8u::width>(const std::int8_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec16x8i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec16x8i{vld1q_s8(ptr)};
        #endif
    }




    template<std::uint32_t N = vec16x8i::width>
    AVEL_FINL void store(std::int8_t* ptr, vec16x8i x) {
        static_assert(N <= vec16x8i::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= vec16x8i::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = 0xFFFF >> N;
        _mm_mask_storeu_epi8(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto mask = _mm_srli_si128(full, vec16x8i::width - N);
        _mm_maskmoveu_si128(decay(x), mask, reinterpret_cast<char *>(ptr));
        #endif
    }

    template<>
    AVEL_FINL void store<vec16x8u::width>(std::int8_t* ptr, vec16x8i x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s8(ptr, decay(x));
        #endif
    }

    AVEL_FINL void store(std::int8_t* ptr, vec16x8i x, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto mask = 0xFFFF >> n; //TODO: Clamp
        _mm_mask_storeu_epi8(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec16x8u::width;
        auto h = vec16x8u::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(w - std::min(w, n)));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(h - std::min(h, n)));
        auto mask = _mm_unpacklo_epi64(lo, hi);
        _mm_maskmoveu_si128(decay(x), mask, reinterpret_cast<char *>(ptr));
        #endif
    }


    template<std::uint32_t N = vec16x8i::width>
    AVEL_FINL void aligned_store(std::int8_t* ptr, vec16x8i x) {
        static_assert(N <= vec16x8u::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= vec16x8u::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s8(ptr, decay(x));
        #endif
    }

    template<>
    AVEL_FINL void aligned_store<vec16x8i::width>(std::int8_t* ptr, vec16x8i x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s8(ptr, decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(std::int8_t* ptr, vec16x8i x, std::uint32_t n) {
        #if defined(AVEL_SSE2)
        store(ptr, x, n);
        #endif

        #if defined(AVEL_NEON)
        store(ptr, x, n);
        #endif
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec16x8i> div(vec16x8i x, vec16x8i y) {
        vec16x8i quotient{0x00};

        mask16x8i sign_mask0 = (x < vec16x8i{});
        mask16x8i sign_mask1 = (y < vec16x8i{});

        mask16x8i sign_mask2 = sign_mask0 ^ sign_mask1;

        vec16x8u numerator{abs(x)};
        vec16x8u denominator{abs(y)};

        //TODO: Compute i more appropriately?
        std::int32_t i = 8;

        //TODO: Consider performing division with abs_neg values
        for (; (i-- > 0) && any(mask16x8u(numerator));) {
            mask16x8u b = ((numerator >> i) >= denominator);
            numerator -= (broadcast_mask(b) & (denominator << i));
            quotient |= vec16x8i(vec16x8u{b} << i);
        }

        //Adjust quotient's sign. Should be xor of operands' signs
        quotient = blend(sign_mask2, -quotient, quotient);

        //Adjust numerator's sign. Should be same sign as it was originally
        x = blend(sign_mask0, -vec16x8i{numerator}, vec16x8i{numerator});

        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i popcount(vec16x8i x) {
        return vec16x8i{popcount(vec16x8u(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i byteswap(vec16x8i x) {
        return x;
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i countl_zero(vec16x8i x) {
        return vec16x8i{countl_zero(vec16x8u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i countl_one(vec16x8i x) {
        return vec16x8i{countl_one(vec16x8u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i countr_zero(vec16x8i x) {
        return vec16x8i{countr_zero(vec16x8u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i countr_one(vec16x8i x) {
        return vec16x8i{countr_one(vec16x8u{x})};
    }

    [[nodiscard]]
    AVEL_FINL mask16x8i has_single_bit(vec16x8i x) {
        return mask16x8i{has_single_bit(vec16x8u{x})};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    vec16x8i bit_shift_left(vec16x8i v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        return vec16x8i{bit_shift_left<S>(vec16x8u{v})};
    }

    template<std::uint32_t S>
    vec16x8i bit_shift_right(vec16x8i v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512VL)
        auto widened = _mm256_cvtepi8_epi16(decay(v));
        widened = _mm256_srai_epi16(widened, S);

        return vec16x8i{_mm256_cvtepi16_epi8(widened)};

        #elif defined(AVEL_SSE2)
        auto lo = _mm_unpacklo_epi8(decay(v), decay(v));
        auto hi = _mm_unpackhi_epi8(decay(v), decay(v));

        lo = _mm_srai_epi16(lo, S + 8);
        hi = _mm_srai_epi16(hi, S + 8);

        return vec16x8i{_mm_packs_epi16(lo, hi)};

        #endif

        #if defined(AVEL_NEON)
        return vec16x8i{vshrq_n_s8(decay(v), S)};

        #endif
    }

    template<>
    [[nodiscard]]
    vec16x8i bit_shift_right<0>(vec16x8i v) {
        return v;
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotl(vec16x8i x, long long s) {
        return vec16x8i{rotl(vec16x8u{x}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotl(vec16x8i x, vec16x8i s) {
        return vec16x8i{rotl(vec16x8u{x}, vec16x8u{s})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotr(vec16x8i x, long long s) {
        return vec16x8i{rotr(vec16x8u{x}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotr(vec16x8i x, vec16x8i s) {
        return vec16x8i{rotr(vec16x8u{x}, vec16x8u{s})};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL arr16x8i to_array(vec16x8i v) {
        alignas(16) arr16x8i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec16x8i>(vec16x8i x) {
        return std::array<vec16x8i, 1>{x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec16x8i>(vec16x8i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{decay(x)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec16x8u{vreinterpretq_u8_s8(decay(x))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec16x8u>(vec16x8u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{vec16x8i::primitive(decay(x))}};
        #endif

        #if defined(AVEL_NEON)
        return {vec16x8u{vreinterpretq_s8_u8(decay(x))};
        #endif
    }

}

#endif //AVEL_VEC16X8I_HPP
