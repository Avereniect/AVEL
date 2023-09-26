#ifndef AVEL_VEC2X64I_HPP
#define AVEL_VEC2X64I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec2x64i = Vector<std::int64_t, 2>;
    using arr2x64i = std::array<std::int64_t, 2>;
    using mask2x64i = Vector_mask<std::int64_t, 2>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec2x64i> div(vec2x64i x, vec2x64i y);
    vec2x64i set_bits(mask2x64i m);
    vec2x64i blend(mask2x64i m, vec2x64i a, vec2x64i b);
    vec2x64i negate(mask2x64i m, vec2x64i x);





    template<>
    class Vector_mask<std::int64_t, 2> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 2;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint64x2_t;
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
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content(b ? 0x3 : 0x0) {}
        #elif defined(AVEL_SSE2)
            content(b ? _mm_set1_epi64x(-1) : _mm_setzero_si128()) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u64(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr2xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::int16_t>(arr));
            auto zeros = _mm_setzero_si128();
            content = _mm_cmplt_epu8_mask(zeros, array_data);

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::int16_t>(arr));
            array_data = _mm_cvtepu8_epi64(array_data);
            auto zeros = _mm_setzero_si128();
            content = _mm_cmplt_epu64_mask(zeros, array_data);

            #elif defined(AVEL_SSE)
            auto lo = _mm_cvtsi64_si128(arr[0]);
            auto hi = _mm_cvtsi64_si128(arr[1]);

            auto zeros = _mm_setzero_si128();
            content = _mm_sub_epi64(zeros, _mm_unpacklo_epi64(lo, hi));

            #endif

            #if defined(AVEL_NEON)
            auto lo = vcreate_u64(arr[0]);
            auto hi = vcreate_u64(arr[1]);
            auto whole = vcombine_u64(lo, hi);
            whole = vqshlq_n_u64(vqshlq_n_u64(whole, 32), 32);

            content = whole;

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
            *this = Vector_mask{x};
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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u8(vreinterpretq_u8_u64(vceqq_u64(decay(lhs), decay(rhs))));
            return min == 0xFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u32(vreinterpretq_u32_u64(vceqq_u64(decay(lhs), decay(rhs))));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(decay(lhs), decay(rhs));
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
            content = _mm_and_si128(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content |= rhs.content;
            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content ^= rhs.content;
            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u64(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return Vector_mask{static_cast<primitive>(content ^ 0x3)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            auto ones = _mm_cmpeq_epi32(undef, undef);
            return Vector_mask{_mm_andnot_si128(content, ones)};

            #endif

            #if defined(AVEL_NEON)
            auto tmp = vreinterpretq_u32_u64(content);
            tmp = vmvnq_u32(tmp);
            return Vector_mask{vreinterpretq_u64_u32(tmp)};

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
    AVEL_FINL std::array<mask2x64u, 1> convert<mask2x64u, mask2x64i>(mask2x64i m) {
        return {mask2x64u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask2x64i, 1> convert<mask2x64i, mask2x64u>(mask2x64u m) {
        return {mask2x64i{decay(m)}};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask2x64i m) {
        return count(mask2x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask2x64i m) {
        return any(mask2x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask2x64i m) {
        return all(mask2x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask2x64i m) {
        return none(mask2x64u{m});
    }





    template<>
    class alignas(16) Vector<std::int64_t, 2> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 2;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int64_t;

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int64x2_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
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

        AVEL_FINL explicit Vector(primitive p):
            content(p) {}

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            content(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content(_mm_maskz_set1_epi64(decay(m), 0x1)) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi64(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vnegq_s64(vreinterpretq_s64_u64(decay(m)))) {}
        #endif

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi64x(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_s64(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr2x64i& array):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_s64(array.data())) {}
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
            return mask{_mm_cmpeq_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE41)
            return mask{_mm_cmpeq_epi64(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            auto t0 = _mm_cmpeq_epi32(lhs.content, rhs.content);
            auto t1 = _mm_shuffle_epi32(t0, 0xB1);
            return mask{_mm_and_si128(t0, t1)};

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            return mask{vceqq_s64(decay(lhs), decay(rhs))};

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmpneq_epi64_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE41)
            return !mask{_mm_cmpeq_epi64(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            auto t0 = _mm_cmpeq_epi32(lhs.content, rhs.content);
            auto t1 = _mm_shuffle_epi32(t0, 0xB1);
            return !mask{_mm_and_si128(t0, t1)};

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            return !(lhs == rhs);

            #elif defined(AVEL_NEON)
            auto diff = vreinterpretq_u64_s64(vsubq_s64(decay(lhs), decay(rhs)));
            auto saturated = vqshlq_n_u64(vqshlq_n_u64(diff, 32), 32);
            return mask{saturated};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmplt_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE42)
            return mask{_mm_cmpgt_epi64(decay(rhs), decay(lhs))};

            #elif defined(AVEL_SSE41)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_extract_epi64(decay(lhs), 0x1);

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            auto out_lo = lhs_lo < rhs_lo;
            auto out_hi = lhs_hi < rhs_hi;

            auto out = _mm_cvtsi64_si128(out_lo);
            out = _mm_insert_epi64(out, out_hi, 0x1);

            auto zeros = _mm_setzero_si128();
            return mask{_mm_sub_epi64(zeros, out)};

            #elif defined(AVEL_SSE2)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(lhs), 0x8));

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            auto out_lo = _mm_cvtsi64_si128(lhs_lo < rhs_lo);
            auto out_hi = _mm_cvtsi64_si128(lhs_hi < rhs_hi);

            auto zeros = _mm_setzero_si128();
            auto ret = _mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8));
            return mask{_mm_sub_epi64(zeros, ret)};

            #endif

            #if defined(AVEL_NEON)
            auto diff = vqsubq_s64(decay(lhs), decay(rhs));
            auto sign_bits = vshrq_n_s64(diff, 63);
            return mask{vreinterpretq_u64_s64(sign_bits)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmple_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE42)
            return !mask{_mm_cmpgt_epi64(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE41)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_extract_epi64(decay(lhs), 0x1);

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            auto out_lo = lhs_lo <= rhs_lo;
            auto out_hi = lhs_hi <= rhs_hi;

            auto out = _mm_cvtsi64_si128(out_lo);
            out = _mm_insert_epi64(out, out_hi, 0x1);

            auto zeros = _mm_setzero_si128();
            return mask{_mm_sub_epi64(zeros, out)};

            #elif defined(AVEL_SSE2)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(lhs), 0x8));

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            auto out_lo = _mm_cvtsi64_si128(lhs_lo <= rhs_lo);
            auto out_hi = _mm_cvtsi64_si128(lhs_hi <= rhs_hi);

            auto zeros = _mm_setzero_si128();
            auto ret = _mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8));
            return mask{_mm_sub_epi64(zeros, ret)};

            #endif

            #if defined(AVEL_NEON)
            return !(lhs > rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmpgt_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE42)
            return mask{_mm_cmpgt_epi64(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE41)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_extract_epi64(decay(lhs), 0x1);

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            auto out_lo = lhs_lo > rhs_lo;
            auto out_hi = lhs_hi > rhs_hi;

            auto out = _mm_cvtsi64_si128(out_lo);
            out = _mm_insert_epi64(out, out_hi, 0x1);

            auto zeros = _mm_setzero_si128();
            return mask{_mm_sub_epi64(zeros, out)};

            #elif defined(AVEL_SSE2)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(lhs), 0x8));

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            auto out_lo = _mm_cvtsi64_si128(lhs_lo > rhs_lo);
            auto out_hi = _mm_cvtsi64_si128(lhs_hi > rhs_hi);

            auto zeros = _mm_setzero_si128();
            auto ret = _mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8));
            return mask{_mm_sub_epi64(zeros, ret)};

            #endif

            #if defined(AVEL_NEON)
            auto diff = vqsubq_s64(decay(rhs), decay(lhs));
            auto sign_bits = vshrq_n_s64(diff, 63);
            return mask{vreinterpretq_u64_s64(sign_bits)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmpge_epi64_mask(decay(lhs), decay(rhs))};

            #elif defined(AVEL_SSE42)
            return !mask{_mm_cmpgt_epi64(decay(rhs), decay(lhs))};

            #elif defined(AVEL_SSE41)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_extract_epi64(decay(lhs), 0x1);

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_extract_epi64(decay(rhs), 0x1);

            auto out_lo = lhs_lo >= rhs_lo;
            auto out_hi = lhs_hi >= rhs_hi;

            auto out = _mm_cvtsi64_si128(out_lo);
            out = _mm_insert_epi64(out, out_hi, 0x1);

            auto zeros = _mm_setzero_si128();
            return mask{_mm_sub_epi64(zeros, out)};

            #elif defined(AVEL_SSE2)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(decay(lhs));
            std::int64_t lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(lhs), 0x8));

            std::int64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::int64_t rhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            auto out_lo = _mm_cvtsi64_si128(lhs_lo >= rhs_lo);
            auto out_hi = _mm_cvtsi64_si128(lhs_hi >= rhs_hi);

            auto zeros = _mm_setzero_si128();
            auto ret = _mm_or_si128(out_lo, _mm_slli_si128(out_hi, 0x8));
            return mask{_mm_sub_epi64(zeros, ret)};

            #endif

            #if defined(AVEL_NEON)
            return !(lhs < rhs);

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
            #if defined(AVEL_SSE2)
            content = _mm_add_epi64(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_s64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi64(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_s64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
            content = _mm_mullo_epi64(content, decay(rhs));

            #elif defined(AVEL_SSE41)
            //TODO: Make consistent with vec2x64u counterpart
            std::uint64_t lhs0 = _mm_extract_epi64(content, 0x0);
            std::uint64_t lhs1 = _mm_extract_epi64(content, 0x1);

            std::uint64_t rhs0 = _mm_extract_epi64(decay(rhs), 0x0);
            std::uint64_t rhs1 = _mm_extract_epi64(decay(rhs), 0x1);

            content = _mm_insert_epi64(content, std::uint64_t(lhs0) * std::uint64_t(rhs0), 0x0);
            content = _mm_insert_epi64(content, std::uint64_t(lhs1) * std::uint64_t(rhs1), 0x1);

            #elif defined(AVEL_SSE2)
            alignas(16) std::uint64_t lhs_array[2];
            alignas(16) std::uint64_t rhs_array[2];

            _mm_store_si128(reinterpret_cast<__m128i*>(lhs_array), content);
            _mm_store_si128(reinterpret_cast<__m128i*>(rhs_array), decay(rhs));

            lhs_array[0] *= rhs_array[0];
            lhs_array[1] *= rhs_array[1];

            content = _mm_load_si128(reinterpret_cast<const __m128i*>(lhs_array));

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto a_lo = vgetq_lane_s64(content, 0x0);
            auto a_hi = vgetq_lane_s64(content, 0x1);
            auto b_lo = vgetq_lane_s64(decay(rhs), 0x0);
            auto b_hi = vgetq_lane_s64(decay(rhs), 0x1);

            auto c_lo = a_lo * b_lo;
            auto c_hi = a_hi * b_hi;

            int64x2_t c;
            c = vsetq_lane_s64(c_lo, c, 0x0);
            c = vsetq_lane_s64(c_hi, c, 0x1);
            content = c;

            #elif defined(AVEL_NEON)
            auto l0 = vreinterpretq_u64_s64(content);
            auto l1 = vreinterpretq_u64_s64(decay(rhs));

            auto l2 = vzip_u32(vreinterpret_u32_u64(vget_low_u64(l0)), vreinterpret_u32_u64(vget_high_u64(l0))).val[0];
            auto l3 = vzip_u32(vreinterpret_u32_u64(vget_low_u64(l1)), vreinterpret_u32_u64(vget_high_u64(l1))).val[0];

            auto l4 = vmull_u32(l2, l3);

            auto l5 = vshlq_n_u64(l0, 32);
            auto l6 = vshlq_n_u64(l1, 32);

            auto l7 = vmulq_u32(vreinterpretq_u32_u64(l1), vreinterpretq_u32_u64(l5));

            auto l8 = vmlaq_u32(l7, vreinterpretq_u32_u64(l6), vreinterpretq_u32_u64(l0));

            auto l9 = vaddq_u64(vreinterpretq_u64_u32(l8), l4);

            content = vreinterpretq_s64_u64(l9);

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
            content = vandq_s64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_s64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_s64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi64(content, _mm_cvtsi64_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s64(content, vdupq_n_s64(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content = _mm_sra_epi64(content, _mm_cvtsi64_si128(rhs));

            #elif defined(AVEL_SSE2)
            std::int64_t lo = _mm_cvtsi128_si64(content);
            std::int64_t hi = _mm_cvtsi128_si64(_mm_srli_si128(content, 8));

            rhs = min(rhs, 63ul);

            lo >>= rhs;
            hi >>= rhs;

            content = _mm_set_epi64x(hi, lo);
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s64(content, vdupq_n_s64(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi64(content, rhs.content);

            #elif defined(AVEL_SSE2)
            auto lhs = content;

            auto lo = _mm_sll_epi64(lhs, decay(rhs));
            auto hi = _mm_sll_epi64(_mm_bsrli_si128(lhs, 0x8), _mm_bsrli_si128(decay(rhs), 0x8));

            content = _mm_unpacklo_epi64(lo, hi);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s64(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content = _mm_srav_epi64(content, decay(rhs));

            #elif defined(AVEL_SSE2)
            std::int64_t lhs_lo = _mm_cvtsi128_si64(content);
            std::int64_t lhs_hi = _mm_cvtsi128_si64(_mm_srli_si128(content, 0x8));

            std::uint64_t rhs_lo = _mm_cvtsi128_si64(decay(rhs));
            std::uint64_t rhs_hi =_mm_cvtsi128_si64(_mm_srli_si128(decay(rhs), 0x8));

            lhs_lo >>= min(rhs_lo, 63ul);
            lhs_hi >>= min(rhs_hi, 63ul);

            content = _mm_set_epi64x(lhs_hi, lhs_lo);
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s64(content, vnegq_s64(decay(rhs)));

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            return Vector{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            primitive t = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi32(t, t))};

            #endif

            #if defined(AVEL_NEON)
            auto tmp = vreinterpretq_s32_s64(content);
            tmp = vmvnq_s32(tmp);
            return Vector{vreinterpretq_s64_s32(tmp)};
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
            #if defined(AVEL_AVX512VL)
            return mask{_mm_test_epi64_mask(content, content)};

            #else
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        2 * sizeof(std::int64_t) == sizeof(vec2x64i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec2x64u, 1> convert<vec2x64u, vec2x64i>(vec2x64i m) {
        #if defined(AVEL_SSE2)
        return {vec2x64u{decay(m)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec2x64u{vreinterpretq_u64_s64(decay(m))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec2x64i, 1> convert<vec2x64i, vec2x64u>(vec2x64u m) {
        #if defined(AVEL_SSE2)
        return {vec2x64i{decay(m)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec2x64i{vreinterpretq_s64_u64(decay(m))}};
        #endif
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec2x64i operator-(vec2x64u v) {
        return vec2x64i{0x00} - vec2x64i{v};
    }

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL std::int64_t extract(vec2x64i v) {
        static_assert(N < vec2x64i::width, "Specified index does not exist");
        typename std::enable_if<N < vec2x64i::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SS41)
        return _mm_extract_epi64(decay(v), N);

        #elif defined(AVEL_SSE2)
        return _mm_cvtsi128_si64(_mm_srli_si128(decay(v), 8 * N));

        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_s64(decay(v), N);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec2x64i insert(vec2x64i v, std::int64_t x) {
        static_assert(N < vec2x64i::width, "Specified index does not exist");
        typename std::enable_if<N < vec2x64i::width, int>::type dummy_variable = 0;

        return vec2x64i{insert<N>(vec2x64u{v}, static_cast<std::uint64_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    vec2x64i bit_shift_left(vec2x64i v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        return vec2x64i{bit_shift_left<S>(vec2x64u{v})};
    }

    template<std::uint32_t S>
    [[nodiscard]]
    vec2x64i bit_shift_right(vec2x64i v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec2x64i{_mm_srai_epi64(decay(v), S)};

        #elif defined(AVEL_SSE4_1)
        auto lo = _mm_extract_epi64(decay(v), 0x0);
        auto hi = _mm_extract_epi64(decay(v), 0x1);

        lo >>= S;
        hi >>= S;

        auto ret = _mm_undefined_si128();
        ret = _mm_insert_epi64(ret, 0x0, lo);
        ret = _mm_insert_epi64(ret, 0x0, hi);
        return vec2x64i{ret};

        #elif defined(AVEL_SSE2)
        auto lo = _mm_cvtsi128_si64(decay(v));
        auto hi = _mm_cvtsi128_si64(_mm_srli_si128(decay(v), 8));

        lo >>= S;
        hi >>= S;

        return vec2x64i{_mm_set_epi64x(hi, lo)};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64i{vshrq_n_s64(decay(v), S)};
        #endif
    }

    #if defined(AVEL_SSE2)
    template<>
    AVEL_FINL vec2x64i bit_shift_right<64>(vec2x64i v) {
        auto zeros = _mm_setzero_si128();
        auto shifted = _mm_srli_epi64(decay(v), 63);
        auto ret = _mm_sub_epi64(zeros, shifted);
        return vec2x64i{ret};
    }
    #endif

    template<>
    AVEL_FINL vec2x64i bit_shift_right<0>(vec2x64i v) {
        return v;
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec2x64i rotl(vec2x64i v) {
        return vec2x64i{rotl<S>(vec2x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i rotl(vec2x64i v, long long s) {
        return vec2x64i{rotl(vec2x64u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i rotl(vec2x64i v, vec2x64i s) {
        return vec2x64i{rotl(vec2x64u{v}, vec2x64u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec2x64i rotr(vec2x64i v) {
        return vec2x64i{rotr<S>(vec2x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i rotr(vec2x64i v, long long s) {
        return vec2x64i{rotr(vec2x64u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i rotr(vec2x64i v, vec2x64i s) {
        return vec2x64i{rotr(vec2x64u{v}, vec2x64u{s})};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec2x64i x) {
        return count(vec2x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec2x64i x) {
        return any(vec2x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec2x64i x) {
        return all(vec2x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec2x64i x) {
        return none(vec2x64u{x});
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i set_bits(mask2x64i m) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return vec2x64i{_mm_movm_epi64(decay(m))};

        #elif defined(AVEL_AVX512VL)
        return vec2x64i{_mm_maskz_set1_epi64(decay(m), -1)};

        #elif defined(AVEL_SSE2)
        return vec2x64i{decay(m)};

        #endif

        #if defined(AVEL_NEON)
        return vec2x64i{vreinterpretq_s64_u64(decay(m))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i keep(mask2x64i m, vec2x64i v) {
        return vec2x64i{keep(mask2x64u{m}, vec2x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i clear(mask2x64i m, vec2x64i v) {
        return vec2x64i{clear(mask2x64u{m}, vec2x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i blend(mask2x64i m, vec2x64i a, vec2x64i b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec2x64i{_mm_mask_blend_epi64(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_SSE41)
        return vec2x64i{_mm_blendv_epi8(decay(b), decay(a), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(decay(m), decay(b));
        auto y = _mm_and_si128(decay(m), decay(a));
        return vec2x64i{_mm_or_si128(x, y)};

        #endif

        #if defined(AVEL_NEON)
        auto x = vreinterpretq_u32_u64(decay(m));
        auto y = vreinterpretq_u32_s64(decay(a));
        auto z = vreinterpretq_u32_s64(decay(b));

        auto w = vbslq_u32(x, y, z);

        return vec2x64i{vreinterpretq_s64_u32(w)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i byteswap(vec2x64i x) {
        return vec2x64i{byteswap(vec2x64u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i max(vec2x64i a, vec2x64i b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec2x64i{_mm_max_epi64(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, b, a);

        #endif

        #if defined(AVEL_NEON)
        return blend(a < b, b, a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i min(vec2x64i a, vec2x64i b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec2x64i{_mm_min_epi64(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(b < a, b, a);

        #endif

        #if defined(AVEL_NEON)
        return blend(b < a, b, a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec2x64i, 2> minmax(vec2x64i a, vec2x64i b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return {
            vec2x64i{_mm_min_epi64(decay(a), decay(b))},
            vec2x64i{_mm_max_epi64(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto mask = a < b;
        return {blend(mask, a, b), blend(mask, b, a)};

        #endif

        #if defined(AVEL_NEON)
        auto mask = b < a;
        return {blend(mask, b, a), blend(mask, a, b)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i clamp(vec2x64i x, vec2x64i lo, vec2x64i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i average(vec2x64i x, vec2x64i y) {
        #if defined(AVEL_SSE2)
        auto avg = (x & y) + ((x ^ y) >> 1);
        auto c = set_bits((x < -y) | (y == vec2x64i{std::int64_t(1) << 63})) & (x ^ y) & vec2x64i{1};

        return avg + c;

        #endif

        #if defined(AVEL_NEON)
        auto avg = (x & y) + ((x ^ y) >> 1);
        auto c = set_bits((x < -y) | (y == vec2x64i{std::int64_t(1) << 63})) & (x ^ y) & vec2x64i{1};

        return avg + c;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i midpoint(vec2x64i a, vec2x64i b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto avg = decay(((a ^ b) >> 1) + (a & b));

        auto bias = _mm_ternarylogic_epi32(decay(a), decay(b), _mm_set1_epi64x(0x1), 0x28);
        auto mask = _mm_cmplt_epi64_mask(decay(b), decay(a));
        auto ret = _mm_mask_add_epi64(avg, mask, avg, bias);

        return vec2x64i{ret};

        #elif defined(AVEL_SSE2)
        auto average = ((a ^ b) >> 1) + (a & b);
        auto bias = (set_bits(b < a) & (a ^ b) & vec2x64i{0x1});
        return average + bias;

        #endif

        #if defined(AVEL_NEON)
        vec2x64i avg{vsraq_n_s64(vandq_s64(decay(a), decay(b)), veorq_s64(decay(a), decay(b)), 1)};
        auto bias = (set_bits(b < a) & (a ^ b) & vec2x64i{0x1});
        return avg + bias;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i negate(mask2x64i m, vec2x64i x) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec2x64i{_mm_mask_sub_epi64(decay(x), decay(m), _mm_setzero_si128(), decay(x))};

        #elif defined(AVEL_SSE2)

        auto mask = set_bits(m);
        return (x ^ mask) - mask;
        #endif

        #if defined(AVEL_NEON)
        auto mask = set_bits(m);
        return (x ^ mask) - mask;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i abs(vec2x64i x) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec2x64i{_mm_abs_epi64(decay(x))};

        #elif defined(AVEL_SSE2)
        auto y = x >> 63;
        return (x ^ y) - y;

        #endif

        #if defined(AVEL_NEON)
        auto y = vshrq_n_s64(decay(x), 63);
        auto z = vsubq_s64(veorq_s64(decay(x), y), y);
        return vec2x64i{z};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i neg_abs(vec2x64i x) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return -abs(x);

        #elif defined(AVEL_SSE2)
        auto y = ~(x >> 63);
        return (x ^ y) - y;
        #endif

        #if defined(AVEL_NEON)
        auto y = vshrq_n_s64(decay(x), 63);
        y = vreinterpretq_s64_s32(vmvnq_s32(vreinterpretq_s32_s64(y)));
        auto z = vsubq_s64(veorq_s64(decay(x), y), y);
        return vec2x64i{z};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i neg_abs(vec2x64u x) {
        return neg_abs(vec2x64i{x});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64i load<vec2x64i>(const std::int64_t* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (1 << n) - 1;
        return vec2x64i{_mm_maskz_loadu_epi64(mask, ptr)};

        #elif defined(AVEL_SSE2)
        switch (n) {
            case 0: {
                return vec2x64i{_mm_setzero_si128()};
            }
            case 1: {
                return vec2x64i{_mm_cvtsi64_si128(*ptr)};
            }
            default: {
                return vec2x64i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
            }
        }
        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            case 0: {
                return vec2x64i{vdupq_n_s64(0x00)};
            }
            case 1: {
                return vec2x64i{vsetq_lane_s64(ptr[0], vdupq_n_s64(0x00), 0)};
            }
            default: {
                return vec2x64i{vld1q_s64(ptr)};
            }
        }
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64i load<vec2x64i>(const std::int64_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec2x64i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64i{vld1q_s64(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64i aligned_load<vec2x64i>(const std::int64_t* ptr, std::uint32_t n) {
        return load<vec2x64i>(ptr, n);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64i aligned_load<vec2x64i, vec2x64u::width>(const std::int64_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec2x64i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Utilize __builtin_assume_aligned on GCC and Clang
        //TODO: Utilize assume_aligned if C++ 20 is available
        return vec2x64i{vld1q_s64(ptr)};
        #endif
    }



    template<>
    AVEL_FINL vec2x64u gather<vec2x64u>(const std::uint64_t* ptr, vec2x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 2) ? 0x3 : (1 << n) - 1;

        return vec2x64u{_mm_mmask_i64gather_epi64(_mm_setzero_si128(), mask, decay(indices), ptr, sizeof(std::uint64_t))};

        #elif defined(AVEL_AVX2)
        n = min(n, vec4x32u::width);

        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec2x64u::width;
        auto h = vec2x64u::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(64 * (h - min(h, n))));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(64 * (w - min(w, n))));
        auto mask = _mm_unpacklo_epi64(lo, hi);

        auto ret = _mm_mask_i64gather_epi64(
            _mm_setzero_si128(),
            bit_cast<const long long*>(ptr),
            decay(indices),
            mask,
            sizeof(std::int64_t)
        );

        return vec2x64u{ret};

        #elif defined(AVEL_SSE)
        auto a = _mm_setzero_si128();
        auto b = _mm_setzero_si128();

        if (n > 0) {
            a = _mm_cvtsi64_si128(ptr[extract<0>(indices)]);
        }

        if (n > 1) {
            b = _mm_cvtsi64_si128(ptr[extract<1>(indices)]);
        }

        auto ab = _mm_unpacklo_epi64(a, b);

        return vec2x64u{ab};
        #endif

        #if defined(AVEL_NEON)
        auto ret = vdupq_n_u64(0x00);
        switch (n) {
            default: ret = vsetq_lane_u64(ptr[extract<1>(indices)], ret, 0x1);
            case 1:  ret = vsetq_lane_u64(ptr[extract<0>(indices)], ret, 0x0);
            case 0:  ; //Do nothing
        }

        return vec2x64u{ret};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64u gather<vec2x64u>(const std::uint64_t* ptr, vec2x64i indices) {
        #if defined(AVEL_AVX2)
        return vec2x64u{_mm_i64gather_epi64(
            bit_cast<const long long*>(ptr),
            decay(indices),
            sizeof(std::uint64_t)
        )};

        #elif defined(AVEL_SSE2)
        auto a = _mm_cvtsi64_si128(ptr[extract<0>(indices)]);
        auto b = _mm_cvtsi64_si128(ptr[extract<1>(indices)]);

        auto abcd = _mm_unpacklo_epi64(a, b);
        return vec2x64u{abcd};
        #endif

        #if defined(AVEL_NEON)
        vec2x64u::primitive read_data;
        read_data = vsetq_lane_u64(ptr[extract<0>(indices)], read_data, 0x0);
        read_data = vsetq_lane_u64(ptr[extract<1>(indices)], read_data, 0x1);

        return vec2x64u{read_data};

        #endif
    }


    template<>
    AVEL_FINL vec2x64i gather<vec2x64i>(const std::int64_t* ptr, vec2x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 2) ? 0x3 : (1 << n) - 1;

        return vec2x64i{_mm_mmask_i64gather_epi64(_mm_setzero_si128(), mask, decay(indices), ptr, sizeof(std::int64_t))};

        #elif defined(AVEL_SSE2)
        auto a = _mm_setzero_si128();
        auto b = _mm_setzero_si128();

        switch (n) {
            default: b = _mm_cvtsi64_si128(ptr[extract<1>(indices)]);
            case 1:  a = _mm_cvtsi64_si128(ptr[extract<0>(indices)]);
            case 0: ; //Do nothing
        }

        auto ab = _mm_unpacklo_epi64(a, b);

        return vec2x64i{ab};
        #endif

        #if defined(AVEL_NEON)
        auto ret = vdupq_n_s64(0x00);
        switch (n) {
            default: ret = vsetq_lane_s64(ptr[extract<1>(indices)], ret, 0x1);
            case 1:  ret = vsetq_lane_s64(ptr[extract<0>(indices)], ret, 0x0);
            case 0:  ; //Do nothing
        }

        return vec2x64i{ret};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64i gather<vec2x64i>(const std::int64_t* ptr, vec2x64i indices) {
        #if defined(AVEL_AVX2)
        return vec2x64i{
            _mm_i64gather_epi64(
                bit_cast<const long long*>(ptr),
                decay(indices),
                sizeof(std::int64_t)
            )
        };

        #elif defined(AVEL_SSE2)
        auto a = _mm_cvtsi64_si128(ptr[extract<0>(indices)]);
        auto b = _mm_cvtsi64_si128(ptr[extract<1>(indices)]);

        auto abcd = _mm_unpacklo_epi64(a, b);
        return vec2x64i{abcd};

        #endif

        #if defined(AVEL_NEON)
        vec2x64i::primitive read_data;
        read_data = vsetq_lane_s64(ptr[extract<0x0>(indices)], read_data, 0x0);
        read_data = vsetq_lane_s64(ptr[extract<0x1>(indices)], read_data, 0x1);

        return vec2x64i{read_data};
        #endif
    }



    AVEL_FINL void store(std::int64_t* ptr, vec2x64i x, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (1 << n) - 1;
        _mm_mask_storeu_epi64(ptr, mask, decay(x));

        #elif defined(AVEL_SSE2)
        switch (n) {
            case 0: {
            } break;
            case 1: {
                ptr[0] = _mm_cvtsi128_si64(decay(x));
            } break;
            default: {
                _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
            }
        }
        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            case 0: {

            } break;
            case 1: {
                vst1_s64(ptr, vget_low_s64(decay(x)));
            } break;
            default: {
                vst1q_s64(ptr, decay(x));
            }
        }
        #endif
    }

    template<std::uint32_t N = vec2x64u::width>
    AVEL_FINL void store(std::int64_t* ptr, vec2x64i x) {
        store(ptr, x, N);
    }

    template<>
    AVEL_FINL void store<vec2x64i::width>(std::int64_t* ptr, vec2x64i x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s64(ptr, decay(x));
        #endif
    }




    AVEL_FINL void aligned_store(std::int64_t* ptr, vec2x64i x, std::uint32_t n) {
        store(ptr, x, n);
    }

    template<std::uint32_t N = vec2x64i::width>
    AVEL_FINL void aligned_store(std::int64_t* ptr, vec2x64i x) {
        aligned_store(ptr, x, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec2x64i::width>(std::int64_t* ptr, vec2x64i x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s64(ptr, decay(x));
        #endif
    }



    AVEL_FINL void scatter(std::uint64_t* ptr, vec2x64u x, vec2x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        n = min(n, vec2x64u::width);
        auto mask = (1 << n) - 1;
        _mm_mask_i64scatter_epi64(ptr, mask, decay(indices), decay(x), sizeof(std::int64_t));

        #elif defined(AVEL_SSE2)
        switch (n) {
            default: ptr[extract<1>(indices)] = _mm_cvtsi128_si64(_mm_bsrli_si128(decay(x), 0x8));
            case 1:  ptr[extract<0>(indices)] = _mm_cvtsi128_si64(decay(x));
            case 0: ; //Do nothing
        }

        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            default: ptr[extract<1>(indices)] = vgetq_lane_u64(decay(x), 0x1);
            case 1:  ptr[extract<0>(indices)] = vgetq_lane_u64(decay(x), 0x0);
            case 0: ; //Do nothing
        }

        #endif
    }

    template<std::uint32_t N = vec2x64u::width>
    AVEL_FINL void scatter(std::uint64_t* ptr, vec2x64u x, vec2x64i indices) {
        scatter(ptr, x, indices, N);
    }

    template<>
    AVEL_FINL void scatter<vec2x64u::width>(std::uint64_t* ptr, vec2x64u x, vec2x64i indices) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        _mm_i64scatter_epi64(
            ptr,
            decay(indices),
            decay(x),
            sizeof(std::uint64_t)
        );

        #elif defined(AVEL_SSE2)
        ptr[extract<0>(indices)] = _mm_cvtsi128_si64(decay(x));
        ptr[extract<1>(indices)] = _mm_cvtsi128_si64(_mm_bsrli_si128(decay(x), 0x8));

        #endif

        #if defined(AVEL_NEON)
        ptr[extract<0>(indices)] = vgetq_lane_u64(decay(x), 0x0);
        ptr[extract<1>(indices)] = vgetq_lane_u64(decay(x), 0x1);

        #endif
    }



    AVEL_FINL void scatter(std::int64_t* ptr, vec2x64i x, vec2x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        n = min(n, vec2x64i::width);
        auto mask = (1 << n) - 1;
        _mm_mask_i64scatter_epi64(ptr, mask, decay(indices), decay(x), sizeof(std::int64_t));

        #elif defined(AVEL_SSE2)
        switch (n) {
            default: ptr[extract<1>(indices)] = _mm_cvtsi128_si64(_mm_bsrli_si128(decay(x), 0x8));
            case 1:  ptr[extract<0>(indices)] = _mm_cvtsi128_si64(decay(x));
            case 0: ; //Do nothing
        }

        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            default: ptr[extract<1>(indices)] = vgetq_lane_s64(decay(x), 0x1);
            case 1:  ptr[extract<0>(indices)] = vgetq_lane_s64(decay(x), 0x0);
            case 0: ; //Do nothing
        }

        #endif
    }

    template<std::uint32_t N = vec2x64i::width>
    AVEL_FINL void scatter(std::int64_t* ptr, vec2x64i x, vec2x64i indices) {
        scatter(ptr, x, indices, N);
    }

    template<>
    AVEL_FINL void scatter<vec2x64i::width>(std::int64_t* ptr, vec2x64i x, vec2x64i indices) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        _mm_i64scatter_epi64(ptr, decay(indices), decay(x), sizeof(std::int64_t));

        #elif defined(AVEL_SSE2)
        ptr[extract<0>(indices)] = _mm_cvtsi128_si64(decay(x));
        ptr[extract<1>(indices)] = _mm_cvtsi128_si64(_mm_bsrli_si128(decay(x), 0x8));

        #endif

        #if defined(AVEL_NEON)
        ptr[extract<0>(indices)] = vgetq_lane_s64(decay(x), 0x0);
        ptr[extract<1>(indices)] = vgetq_lane_s64(decay(x), 0x1);

        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr2x64i to_array(vec2x64i v) {
        alignas(16) arr2x64i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec2x64i> div(vec2x64i x, vec2x64i y) {
        mask2x64i remainder_sign_mask = (x < vec2x64i{0x00});
        mask2x64i quotient_sign_mask = remainder_sign_mask ^ (y < vec2x64i{0x00});

        vec2x64u numerator{abs(x)};
        vec2x64u denominator{abs(y)};

        auto results = div(numerator, denominator);

        return {
            negate(quotient_sign_mask,  vec2x64i{results.quot}),
            negate(remainder_sign_mask, vec2x64i{results.rem})
        };
    }

    AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(vec2x64i, mask2x64i, vec2x64u)

}

#endif //AVEL_VEC2x64I_HPP
