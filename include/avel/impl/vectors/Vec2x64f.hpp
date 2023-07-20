#ifndef AVEL_VEC2X64F_HPP
#define AVEL_VEC2X64F_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec2x64f = Vector<double, 2>;
    using arr2x64f = std::array<double, 2>;
    using mask2x64f = Vector_mask<double, 2>;

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec2x64f trunc(vec2x64f x);
    AVEL_FINL vec2x64f abs(vec2x64f v);
    AVEL_FINL vec2x64f fmod(vec2x64f a, vec2x64f b);
    AVEL_FINL vec2x64f blend(mask2x64f, vec2x64f, vec2x64f);
    AVEL_FINL vec2x64f round(vec2x64f);
    AVEL_FINL mask2x64f isunordered(vec2x64f, vec2x64f);
    AVEL_FINL mask2x64f signbit(vec2x64f x);
    AVEL_FINL mask2x64f isnan(vec2x64f v);
    AVEL_FINL mask2x64f isinf(vec2x64f v);
    AVEL_FINL vec2x64f copysign(vec2x64f mag, vec2x64f sign);





    template<>
    class Vector_mask<double, 2> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 2;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE2)
        using primitive = __m128d;
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
        // Constructor
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            content(convert<Vector_mask>(v)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL)
            content(b ? 0x3 : 0x00) {}
        #elif defined(AVEL_SSE2)
            content(_mm_castsi128_pd(b ? _mm_set1_epi64x(-1ul) : _mm_setzero_si128())) {}
        #endif
        #if defined(AVEL_NEON)
            content() {} //TODO: Implement
        #endif

        AVEL_FINL explicit Vector_mask(const arr2xb& arr) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m128i array_data = _mm_cvtsi32_si128(bit_cast<std::uint16_t>(arr));
            content = __mmask8(_mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint16_t>(arr));
            auto expanded = _mm_cvtepi8_epi32(array_data);
            content = _mm_cmplt_epu32_mask(_mm_setzero_si128(), expanded);

            #elif defined(AVEL_SSE2)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint16_t>(arr));
            array_data = _mm_unpacklo_epi8(array_data, array_data);
            array_data = _mm_unpacklo_epi16(array_data, array_data);
            array_data = _mm_unpacklo_epi32(array_data, array_data);

            content = _mm_cmpneq_pd(_mm_castsi128_pd(array_data), _mm_setzero_pd());

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
            return lhs.content == rhs.content;

            #elif defined(AVEL_SSE2)
            return _mm_movemask_pd(_mm_xor_pd(lhs.content, rhs.content)) == 0x00;

            #endif

            #if defined(AVEL_NEON)
            auto min = vminvq_u32(vreinterpretq_u32_u64(vceqq_u64(decay(lhs), decay(rhs))));
            return min == 0xFFFFFFFF;
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return lhs.content != rhs.content;

            #elif defined(AVEL_SSE2)
            return _mm_movemask_pd(_mm_xor_pd(lhs.content, rhs.content)) != 0x00;

            #endif

            #if defined(AVEL_NEON)
            auto min = vminvq_u32(vreinterpretq_u32_u64(vceqq_u64(decay(lhs), decay(rhs))));
            return min == 0x0;
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content &= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_and_pd(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u64(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content |= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_or_pd(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u64(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content ^= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_xor_pd(content, decay(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u64(content, rhs.content);

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{primitive(content ^ 0x3)};

            #elif defined(AVEL_SSE2)
            auto ones = _mm_castsi128_pd(_mm_set1_epi64x(-1));
            return Vector_mask{_mm_andnot_pd(content, ones)};
            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vreinterpretq_u64_u32(vmvnq_u32(vreinterpretq_u32_u64(content)))};
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
    AVEL_FINL std::uint32_t count(mask2x64f m) {
        #if defined(AVEL_AVX512VL)
        return popcount(decay(m));

        #elif defined(AVEL_SSE2) && defined(AVEL_POPCNT)
        return popcount(_mm_movemask_pd(decay(m)));

        #elif defined(AVEL_SSE2)
        auto mm = _mm_movemask_pd(decay(m));
        return mm - (mm > 1);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask2x64f m) {
        #if defined(AVEL_AVX512VL)
        return decay(m);

        #elif defined(AVEL_SSE2)
        return _mm_movemask_pd(decay(m)) != 0x0;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask2x64f m) {
        #if defined(AVEL_AVX512VL)
        return decay(m) == 0x3;

        #elif defined(AVEL_SSE2)
        return _mm_movemask_pd(decay(m)) == 0x3;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask2x64f m) {
        return !any(m);
    }





    template<>
    class Vector<double, 2> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 2;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = double;

        #if defined(AVEL_SSE2)
        using primitive = __m128d;
        #endif

        #if defined(AVEL_NEON)
        using primitive = float64x2_t;
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
        #if defined(AVEL_AVX512VL)
            content(_mm_maskz_mov_pd(decay(m), _mm_set1_pd(1.0))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_and_pd(decay(m), _mm_set1_pd(1.0f))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vreinterpretq_f64_u64(vandq_u64(decay(m), vdupq_n_u64(0x3ff0000000000000ll)))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_pd(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_f64(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr2x64f& array):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_pd(array.data())) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_f64(array.data())) {}
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
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_EQ_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_pd(decay(lhs), decay(rhs))};

            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_f64(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_NEQ_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpneq_pd(decay(lhs), decay(rhs))};

            #endif

            #if defined(AVEL_NEON)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_LT_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_pd(decay(lhs), decay(rhs))};

            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_f64(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_LE_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmple_pd(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_f64(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_GT_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_pd(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_f64(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_GE_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpge_pd(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_f64(lhs.content, rhs.content)};
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
            return Vector{0.0} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_add_pd(content, decay(vec));
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_f64(content, vec.content);
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_pd(content, decay(vec));
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_f64(content, vec.content);
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_mul_pd(content, decay(vec));
            #endif

            #if defined(AVEL_NEON)
            content = vmulq_f64(content, vec.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_div_pd(content, decay(vec));
            #endif

            #if defined(AVEL_NEON)
            content = vdivq_f64(content, vec.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector vec) {
            content = fmod(*this, vec).content;
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
        // Conversions
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512VL)
            auto t = _mm_castpd_si128(content);
            return mask{_mm_test_epi64_mask(t, t)};

            #elif defined(AVEL_AVX)
            return mask{_mm_cmp_pd(content, _mm_setzero_pd(), _CMP_NEQ_UQ)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpneq_pd(content, _mm_setzero_pd())};

            #endif

            #if defined(AVEL_NEON)
            return Vector{0.0} != *this;
            #endif
        }

    };

    static_assert(
        2 * sizeof(double) == sizeof(vec2x64f),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL double extract(vec2x64f v) {
        static_assert(N < vec2x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec2x64f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        if (N == 1) {
            return _mm_cvtsd_f64(_mm_unpackhi_pd(decay(v), decay(v)));
        } else {
            return _mm_cvtsd_f64(decay(v));
        }
        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_f64(decay(v), N);
        #endif
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL vec2x64f insert(vec2x64f v, double x) {
        static_assert(N < vec2x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec2x64f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        auto ret = decay(v);
        ret[N] = x;
        return vec2x64f{ret};
        #endif

        #if defined(AVEL_NEON)
        auto ret = decay(v);
        ret = vsetq_lane_f64(x, decay(v), N);
        return vec2x64f{ret};
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec2x64f x) {
        return count(mask2x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec2x64f x) {
        return any(mask2x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec2x64f x) {
        return all(mask2x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec2x64f x) {
        return none(mask2x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f keep(mask2x64f m, vec2x64f v) {
        #if defined(AVEL_AVX512VL)
        return vec2x64f{_mm_maskz_mov_pd(decay(m), decay(v))};

        #elif defined(AVEL_SSE2)
        return vec2x64f{_mm_and_pd(decay(m), decay(v))};

        #endif

        #if defined(AVEL_NEON)
        return vec2x64f{vreinterpretq_f64_u64(vandq_u64(vreinterpretq_u64_f64(decay(v)), decay(m)))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f clear(mask2x64f m, vec2x64f v) {
        #if defined(AVEL_AVX512VL)
        return vec2x64f{_mm_maskz_mov_pd(decay(!m), decay(v))};

        #elif defined(AVEL_SSE2)
        return vec2x64f{_mm_andnot_pd(decay(m), decay(v))};

        #endif

        #if defined(AVEL_NEON)
        return vec2x64f{vreinterpretq_f64_u64(vbicq_u64(vreinterpretq_u64_f64(decay(v)), decay(m)))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f blend(mask2x64f m, vec2x64f a, vec2x64f b) {
        #if defined(AVEL_AVX512VL)
        return vec2x64f{_mm_mask_blend_pd(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_SSE41)
        return vec2x64f{_mm_blendv_pd(decay(b), decay(a), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_pd(decay(m), decay(b));
        auto y = _mm_and_pd(decay(m), decay(a));
        return vec2x64f{_mm_or_pd(x, y)};

        #endif

        #if defined(AVEL_NEON)
        auto x = vreinterpretq_u32_u64(decay(m));
        auto y = vreinterpretq_u32_f64(decay(a));
        auto z = vreinterpretq_u32_f64(decay(b));

        auto w = vbslq_u32(x, y, z);

        return vec2x64f{vreinterpretq_f64_u32(w)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f byteswap(vec2x64f v) {
        return bit_cast<vec2x64f>(byteswap(bit_cast<vec2x64u>(v)));
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f max(vec2x64f a, vec2x64f b) {
        #if defined(AVEL_SSE2)
        return vec2x64f{_mm_max_pd(decay(a), decay(b))};
        #endif

        #if defined(AVEL_NEON)
        auto mask = a < b;
        return blend(mask, b, a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f min(vec2x64f a, vec2x64f b) {
        #if defined(AVEL_SSE2)
        return vec2x64f{_mm_min_pd(decay(a), decay(b))};
        #endif

        #if defined(AVEL_NEON)
        auto mask = a < b;
        return blend(mask, a, b);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec2x64f, 2> minmax(vec2x64f a, vec2x64f b) {
        #if defined(AVEL_SSE2)
        return {
            vec2x64f{_mm_min_pd(decay(a), decay(b))},
            vec2x64f{_mm_max_pd(decay(a), decay(b))}
        };
        #endif

        #if defined(AVEL_NEON)
        auto mask = a < b;
        return {
            blend(mask, a, b),
            blend(mask, b, a)
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f clamp(vec2x64f x, vec2x64f lo, vec2x64f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f negate(mask2x64f m, vec2x64f v) {
        #if defined(AVEL_AVX512VL)
        return vec2x64f{_mm_mask_sub_pd(decay(v), decay(m), _mm_setzero_pd(), decay(v))};

        #elif defined(AVEL_SSE2)
        auto negation_mask = _mm_and_pd(decay(m), _mm_set1_pd(double_sign_bit_mask));
        return vec2x64f{_mm_xor_pd(decay(v), negation_mask)};

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f abs(vec2x64f v) {
        #if defined(AVEL_SSE)
        return vec2x64f{_mm_andnot_pd(_mm_set1_pd(double_sign_bit_mask), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64f{vabsq_f64(decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f neg_abs(vec2x64f v) {
        #if defined(AVEL_SSE)
        return vec2x64f{_mm_or_pd(_mm_set1_pd(double_sign_bit_mask), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64f{vsubq_f64(vdupq_n_f64(0.0f), vabsq_f64(decay(v)))};
        #endif
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f load<vec2x64f>(const double* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 2) ? -1 : (1 << n) - 1;
        return vec2x64f{_mm_maskz_loadu_pd(mask, ptr)};

        #elif defined(AVEL_SSE2)
        switch (n) {
            case 0:  return vec2x64f{_mm_setzero_pd()};
            case 1:  return vec2x64f{_mm_set_sd(*ptr)};
            default: return vec2x64f{_mm_loadu_pd(ptr)};
        }
        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f load<vec2x64f, vec2x64f::width>(const double* ptr) {
        #if defined(AVEL_SSE2)
        return vec2x64f{_mm_loadu_pd(ptr)};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64f{vld1q_f64(ptr)};
        #endif
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f aligned_load<vec2x64f>(const double* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 2) ? -1 : (1 << n) - 1;
        return vec2x64f{_mm_maskz_load_pd(mask, ptr)};

        #elif defined(AVEL_AVX)
        auto table_offset = masks128_table.size() / 2 - avel::min(vec2x64f::width, n) * sizeof(double);
        auto mask = _mm_loadu_si128(reinterpret_cast<const __m128i*>(masks128_table.data() + table_offset));
        return vec2x64f{_mm_maskload_pd(ptr, mask)};

        #elif defined(AVEL_SSE2)
        switch (n) {
            default: return vec2x64f{_mm_loadu_pd(ptr)};
            case 1: return vec2x64f{_mm_load_sd(ptr)};
            case 0: return vec2x64f{0.0};
        }

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f aligned_load<vec2x64f, vec2x64f::width>(const double* ptr) {
        #if defined(AVEL_SSE2)
        return vec2x64f{_mm_load_pd(ptr)};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Use assume_aligned and alternatives
        return vec2x64f{vld1q_f64(ptr)};
        #endif
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f gather<vec2x64f>(const double* ptr, vec2x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 2) ? -1 : (1 << n) - 1;
        return vec2x64f{_mm_mmask_i64gather_pd(_mm_setzero_pd(), mask, decay(indices), ptr, sizeof(double))};

        #elif defined(AVEL_AVX2)
        auto table_offset = masks128_table.size() / 2 - avel::min(vec2x64f::width, n) * sizeof(double);
        auto mask = _mm_loadu_si128(reinterpret_cast<const __m128i*>(masks128_table.data() + table_offset));
        return vec2x64f{_mm_mask_i64gather_pd(_mm_setzero_pd(), ptr, decay(indices), mask, sizeof(double))};

        #elif defined(AVEL_SSE2)
        auto a = _mm_setzero_pd();
        auto b = _mm_setzero_pd();

        switch (n) {
            default: b = _mm_load_sd(ptr + extract<1>(indices));
            case 1:  a = _mm_load_sd(ptr + extract<0>(indices));
            case 0:  ;
        }

        return vec2x64f{_mm_unpacklo_pd(a, b)};

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f gather<vec2x64f>(const double* ptr, vec2x64i indices) {
        #if defined(AVEL_AVX2)
        return vec2x64f{_mm_i64gather_pd(ptr, decay(indices), sizeof(double))};

        #elif defined(AVEL_SSE2)
        return vec2x64f{_mm_set_pd(
            ptr[extract<1>(indices)],
            ptr[extract<0>(indices)]
        )};

        #endif
    }



    AVEL_FINL void store(double* ptr, vec2x64f v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 2) ? -1 : (1 << n) - 1;
        _mm_mask_storeu_pd(ptr, mask, decay(v));

        #elif defined(AVEL_AVX)
        auto table_offset = masks128_table.size() / 2 - avel::min(vec2x64f::width, n) * sizeof(double);
        auto mask = _mm_loadu_si128(reinterpret_cast<const __m128i*>(masks128_table.data() + table_offset));
        _mm_maskstore_pd(ptr, mask, decay(v));

        #elif defined(AVEL_SSE2)
        auto table_offset = masks128_table.size() / 2 - avel::min(vec2x64f::width, n) * sizeof(double);
        auto mask = _mm_loadu_si128(reinterpret_cast<const __m128i*>(masks128_table.data() + table_offset));

        _mm_maskmoveu_si128(
            _mm_castpd_si128(decay(v)),
            mask,
            reinterpret_cast<char*>(ptr)
        );

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    template<std::uint32_t N = vec2x64f::width>
    AVEL_FINL void store(double* ptr, vec2x64f v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec2x64f::width>(double* ptr, vec2x64f v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_pd(ptr, decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_f64(ptr, decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(double* ptr, vec2x64f v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 2) ? -1 : (1 << n) - 1;
        _mm_mask_store_pd(ptr, mask, decay(v));

        #elif defined(AVEL_SSE2)
        store(ptr, v, n);
        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    template<std::uint32_t N = vec2x64f::width>
    AVEL_FINL void aligned_store(double* ptr, vec2x64f v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec2x64f::width>(double* ptr, vec2x64f v) {
        #if defined(AVEL_SSE2)
        _mm_store_pd(ptr, decay(v));
        #endif

        #if defined(AVEL_NEON)

        #endif
    }



    AVEL_FINL void scatter(double* ptr, vec2x64f v, vec2x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 2) ? -1 : (1 << n) - 1;
        _mm_mask_i64scatter_pd(ptr, mask, decay(indices), decay(v), sizeof(double));

        #elif defined(AVEL_SSE2)
        switch (n) {
            default: ptr[extract<1>(indices)] = extract<1>(v);
            case 1:  ptr[extract<0>(indices)] = extract<0>(v);
            case 0: ;
        }

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    template<std::uint32_t N = vec2x64f::width>
    AVEL_FINL void scatter(double* ptr, vec2x64f v, vec2x64i indices) {
        scatter(ptr, v, indices, N);
    }

    template<>
    AVEL_FINL void scatter<0>(double* ptr, vec2x64f v, vec2x64i indices) {
        // Don't have to do c   thing
    }

    template<>
    AVEL_FINL void scatter<vec2x64f::width>(double* ptr, vec2x64f v, vec2x64i indices) {
        #if defined(AVEL_AVX512VL)
        _mm_i64scatter_pd(ptr, decay(indices), decay(v), sizeof(double));

        #elif defined(AVEL_SSE2)
        ptr[extract<0>(indices)] = extract<0>(v);
        ptr[extract<1>(indices)] = extract<1>(v);

        #endif

        #if defined(AVEL_NEON)

        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr2x64f to_array(vec2x64f v) {
        alignas(16) arr2x64f ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // cmath basic operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64f fmax(vec2x64f a, vec2x64f b) {
        #if defined(AVEL_SSE2)
        return blend(avel::isnan(b), a, avel::max(a, b));
        #endif

        #if defined(AVEL_NEON)
        return vec2x64f{vmaxnmq_f64(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fmin(vec2x64f a, vec2x64f b) {
        #if defined(AVEL_SSE2)
        return blend(avel::isnan(b), a, avel::min(a, b));
        #endif

        #if defined(AVEL_NEON)
        return vec2x64f{vminnmq_f64(decay(a), decay(b))};
        #endif
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64f sqrt(vec2x64f v) {
        #if defined(AVEL_SSE2)
        return vec2x64f{_mm_sqrt_pd(decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64f{vsqrtq_f64(decay(v))};
        #endif
    }

    //=====================================================
    // Nearest Integer floating-point operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64f ceil(vec2x64f v) {
        #if defined(AVEL_SSE41)
        return vec2x64f{_mm_round_pd(decay(v), _MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)};

        #elif defined(AVEL_SSE2)
        auto abs_v = abs(v);

        auto is_integral = _mm_cmple_pd(_mm_set1_pd(4503599627370496.0), decay(abs_v));
        auto is_nan = _mm_cmpunord_pd(decay(abs_v), decay(abs_v));
        auto is_output_self = _mm_or_pd(is_integral, is_nan);

        auto converted0 = _mm_cvttsd_si64(decay(v));
        auto converted1 = _mm_cvttsd_si64(_mm_shuffle_pd(decay(v), decay(v), 0x3));

        auto reconstructed0 = _mm_cvtsi64_sd(_mm_undefined_pd(), converted0);
        auto reconstructed1 = _mm_cvtsi64_sd(_mm_undefined_pd(), converted1);

        auto reconstructed = _mm_unpacklo_pd(reconstructed0, reconstructed1);

        auto is_reconstruction_smaller = _mm_cmplt_pd(reconstructed, decay(v));
        auto corrected_result = _mm_add_pd(reconstructed, _mm_and_pd(is_reconstruction_smaller, _mm_set1_pd(1.0f)));

        return blend(mask2x64f{is_output_self}, v, vec2x64f{corrected_result});

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f floor(vec2x64f v) {
        #if defined(AVEL_SSE41)
        return vec2x64f{_mm_round_pd(decay(v), _MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)};

        #elif defined(AVEL_SSE2)
        auto abs_v = abs(v);

        auto is_integral = _mm_cmple_pd(_mm_set1_pd(4503599627370496.0), decay(abs_v));
        auto is_nan = _mm_cmpunord_pd(decay(abs_v), decay(abs_v));
        auto is_output_self = _mm_or_pd(is_integral, is_nan);

        auto converted0 = _mm_cvttsd_si64(decay(v));
        auto converted1 = _mm_cvttsd_si64(_mm_shuffle_pd(decay(v), decay(v), 0x3));

        auto reconstructed0 = _mm_cvtsi64_sd(_mm_undefined_pd(), converted0);
        auto reconstructed1 = _mm_cvtsi64_sd(_mm_undefined_pd(), converted1);

        auto reconstructed = _mm_unpacklo_pd(reconstructed0, reconstructed1);

        auto is_reconstruction_smaller = _mm_cmplt_pd(decay(v), reconstructed);
        auto corrected_result = _mm_sub_pd(reconstructed, _mm_and_pd(is_reconstruction_smaller, _mm_set1_pd(1.0f)));

        return blend(mask2x64f{is_output_self}, v, vec2x64f{corrected_result});

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f trunc(vec2x64f v) {
        #if defined(AVEL_SSE41)
        return vec2x64f{_mm_round_pd(decay(v), _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)};

        #elif defined(AVEL_SSE2)
        auto abs_v = abs(v);

        auto is_integral = _mm_cmple_pd(_mm_set1_pd(4503599627370496.0), decay(abs_v));
        auto is_nan = _mm_cmpunord_pd(decay(abs_v), decay(abs_v));
        auto is_output_self = _mm_or_pd(is_integral, is_nan);

        auto converted0 = _mm_cvttsd_si64(decay(v));
        auto converted1 = _mm_cvttsd_si64(_mm_shuffle_pd(decay(v), decay(v), 0x3));

        auto reconstructed0 = _mm_cvtsi64_sd(_mm_undefined_pd(), converted0);
        auto reconstructed1 = _mm_cvtsi64_sd(_mm_undefined_pd(), converted1);

        auto reconstructed = _mm_unpacklo_pd(reconstructed0, reconstructed1);

        return blend(mask2x64f{is_output_self}, v, vec2x64f{reconstructed});

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f round(vec2x64f v) {
        #if defined(AVEL_SSE2)
        auto whole = trunc(v);
        auto frac = v - whole;

        auto offset = copysign(vec2x64f{1.0f}, v);
        auto should_offset = abs(frac) >= vec2x64f{0.5f};
        auto ret = whole + keep(should_offset, offset);

        return ret;

        /* Solution that works if the current rounding mode is set to nearest
        // The following constant is the value prior to 0.5
        auto offset = avel::copysign(vec2x64f{avel::bit_cast<double>(0x3fdfffffffffffff)}, v);
        return avel::trunc(v + offset);
        */

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f nearbyint(vec2x64f v) {
        #if defined(AVEL_SSE41)
        return vec2x64f{_mm_round_pd(decay(v), _MM_FROUND_CUR_DIRECTION)};

        #elif defined(AVEL_SSE2)
        int mode = _MM_GET_ROUNDING_MODE();
        switch (mode) {
            case _MM_ROUND_NEAREST: {
                auto abs_v = abs(v);

                auto is_integral = _mm_cmple_pd(_mm_set1_pd(4503599627370496.0), decay(abs_v));
                auto is_nan = _mm_cmpunord_pd(decay(abs_v), decay(abs_v));
                auto is_output_self = _mm_or_pd(is_integral, is_nan);

                auto converted0 = _mm_cvtsd_si64(decay(v));
                auto converted1 = _mm_cvtsd_si64(_mm_shuffle_pd(decay(v), decay(v), 0x3));

                auto reconstructed0 = _mm_cvtsi64_sd(_mm_undefined_pd(), converted0);
                auto reconstructed1 = _mm_cvtsi64_sd(_mm_undefined_pd(), converted1);

                auto reconstructed = _mm_unpacklo_pd(reconstructed0, reconstructed1);

                return blend(mask2x64f{is_output_self}, v, vec2x64f{reconstructed});
            }
            case _MM_ROUND_DOWN:        return avel::floor(v);
            case _MM_ROUND_TOWARD_ZERO: return avel::trunc(v);
            case _MM_ROUND_UP:          return avel::ceil(v);
        default:
            return vec2x64f{0.0};
            // TODO: Find proper solution.
            // Not sure how to handle such a case.
            // Fall back to scalar code?
        }

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f rint(vec2x64f v) {
        return nearbyint(v);
    }

    //=====================================================
    // floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64f frexp(vec2x64f v, vec2x64i* exp) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto is_infinity = _mm_fpclass_pd_mask(decay(v), 0x10 | 0x08);
        auto is_non_zero = _mm_cmp_pd_mask(decay(v), _mm_setzero_pd(), _CMP_NEQ_UQ);

        auto exponents = _mm_getexp_pd(decay(v));
        exponents = _mm_add_pd(exponents, _mm_set1_pd(1.0f));
        *exp = _mm_maskz_cvttpd_epi64(is_non_zero, exponents);

        auto ret = _mm_getmant_pd(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        // Note: Returns -1 or 1 for -infinity and +infinity respectively

        ret = _mm_maskz_mov_pd(is_non_zero, ret);
        ret = _mm_mask_blend_pd(is_infinity, ret, decay(v));
        return vec2x64f{ret};

        #elif defined(AVEL_AVX512VL)
        auto is_infinity = decay(abs(v) == vec2x64f{INFINITY});
        auto is_non_zero = _mm_cmp_pd_mask(decay(v), _mm_setzero_pd(), _CMP_NEQ_UQ);

        auto exponents = _mm_getexp_pd(decay(v));
        exponents = _mm_add_pd(exponents, _mm_set1_pd(1.0f));
        auto exp32 = _mm_maskz_cvttpd_epi32(is_non_zero, exponents);

        *exp = _mm_cvtepi32_epi64(exp32);

        auto ret = _mm_getmant_pd(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        // Note: Returns -1 or 1 for -infinity and +infinity respectively

        ret = _mm_maskz_mov_pd(is_non_zero, ret);
        ret = _mm_mask_blend_pd(is_infinity, ret, decay(v));
        return vec2x64f{ret};

        #elif defined(AVEL_SSE2)
        auto bits = _mm_castpd_si128(decay(v));

        auto is_v_zero = _mm_castpd_si128(_mm_cmpeq_pd(_mm_setzero_pd(), decay(v)));

        // Check if v is subnormal
        auto is_subnormal = avel::abs(v) < vec2x64f{DBL_MIN};
        auto multiplier = blend(is_subnormal, vec2x64f{9007199254740992.0f}, vec2x64f{1.0f});

        // Make v normal if it's subnormal
        auto v_corrected = v * multiplier;
        auto v_corrected_bits = _mm_castpd_si128(decay(v_corrected));

        // Compute exponent to write out
        auto is_subnormal_raw = _mm_castpd_si128(decay(is_subnormal));
        auto biased_exponent = _mm_srli_epi64(_mm_and_si128(v_corrected_bits, _mm_set1_epi64x(double_exponent_mask_bits)), 52);
        auto bias_correction = _mm_add_epi64(_mm_set1_epi64x(1022), _mm_and_si128(is_subnormal_raw, _mm_set1_epi64x(53)));
        auto unbiased_exponent = _mm_sub_epi64(biased_exponent, bias_correction);

        // Clear out exponent value when the input is zero
        *exp = _mm_andnot_si128(is_v_zero, unbiased_exponent);

        // Change exponent to be -1, thereby remapping to [0.5, 1.0) range
        auto new_exponent = _mm_set1_epi64x(1022ull << 52);
        auto cleared_exponent = _mm_andnot_si128(_mm_set1_epi64x(double_exponent_mask_bits), v_corrected_bits);
        auto remapped_significand = _mm_or_si128(cleared_exponent, new_exponent);

        auto comparison32 = _mm_cmpeq_epi32(biased_exponent, _mm_set1_epi64x(0x7ffll));
        auto comparison64 = _mm_shuffle_epi32(comparison32, 0xa0);
        auto is_output_self = _mm_or_si128(is_v_zero, comparison64);
        auto ret = blend(mask2x64i{is_output_self}, vec2x64i{bits}, vec2x64i{remapped_significand});

        mask2x64f m{_mm_castsi128_pd(is_output_self)};
        return blend(m, v, vec2x64f{_mm_castsi128_pd(decay(ret))});

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f ldexp(vec2x64f arg, vec2x64i exp) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec2x64f{_mm_scalef_pd(decay(arg), _mm_cvtepi64_pd(decay(exp)))};

        #elif defined(AVEL_AVX512VL)
        auto exp32 = _mm_cvtsepi64_epi32(decay(exp));
        auto exp_as_f64 = _mm_cvtepi32_pd(exp32);
        return vec2x64f{_mm_scalef_pd(decay(arg), exp_as_f64)};

        #elif defined(AVEL_SSE2)
        //TODO: Optimize
        // Approach based on repeated multiplication by powers of two.
        auto bias = _mm_set1_epi32(1023);

        auto bits = _mm_castpd_si128(decay(arg));
        auto exponent_field = _mm_and_si128(_mm_set1_epi64x(double_exponent_mask_bits), bits);
        vec2x64i arg_exponent = bit_shift_right<52>(vec2x64i{exponent_field});

        // Perform two multiplications such that they should never lead to lossy rounding
        vec2x64i lower_bound0{vec2x64i{1} - arg_exponent};
        vec2x64i upper_bound0{vec2x64i{1046} - arg_exponent};

        vec2x64i extracted_magnitude = clamp(exp, lower_bound0, upper_bound0);
        exp -= extracted_magnitude;

        auto exponent0 = bit_shift_right<1>(extracted_magnitude);
        auto exponent_field0 = bit_shift_left<52>(exponent0 + vec2x64i{bias});
        auto multiplicand0 = _mm_castsi128_pd(decay(exponent_field0));

        auto exponent1 = extracted_magnitude - exponent0;
        auto exponent_field1 = bit_shift_left<52>(exponent1 + vec2x64i{bias});
        auto multiplicand1 = _mm_castsi128_pd(decay(exponent_field1));

        // Perform one more multiplication in case the previous two weren't enough
        // If the number isn't enough then the program

        vec2x64i lower_bound1{-1022};
        vec2x64i upper_bound1{+1022};

        auto exponent2 = avel::clamp(exp, lower_bound1, upper_bound1);
        auto exponent_field2 = bit_shift_left<52>(exponent2 + vec2x64i{bias});
        auto multiplicand2 = _mm_castsi128_pd(decay(exponent_field2));
        //exp -= exponent2;

        auto t0 = _mm_mul_pd(decay(arg), multiplicand0);
        auto t1 = _mm_mul_pd(t0, multiplicand1);
        auto ret = _mm_mul_pd(t1, multiplicand2);

        return vec2x64f{ret};

        /*
        //TODO: Optimize. 32-bit integer operations would be faster and suffice

        // Approach based on repeated multiplication by powers of two.
        vec2x64i lower_bound{-1022};
        vec2x64i upper_bound{+1023};
        vec2x64i& bias = upper_bound;

        auto exponent0 = avel::clamp(exp, lower_bound, upper_bound);
        auto exponent_field0 = _mm_slli_epi64(_mm_add_epi64(decay(exponent0), decay(bias)), 52);
        auto multiplicand0 = _mm_castsi128_pd(exponent_field0);
        exp -= exponent0;

        auto exponent1 = avel::clamp(exp, lower_bound, upper_bound);
        auto exponent_field1 = _mm_slli_epi64(_mm_add_epi64(decay(exponent1), decay(bias)), 52);
        auto multiplicand1 = _mm_castsi128_pd(exponent_field1);
        exp -= exponent1;

        auto exponent2 = avel::clamp(exp, lower_bound, upper_bound);
        auto exponent_field2 = _mm_slli_epi64(_mm_add_epi64(decay(exponent2), decay(bias)), 52);
        auto multiplicand2 = _mm_castsi128_pd(exponent_field2);
        exp -= exponent2;

        auto ret = _mm_mul_pd(_mm_mul_pd(_mm_mul_pd(decay(arg), multiplicand0), multiplicand1), multiplicand2);

        return vec2x64f{ret};
        */

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f scalbn(vec2x64f x, vec2x64i exp) {
        return avel::ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i ilogb(vec2x64f x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto exp_fp = _mm_getexp_pd(decay(x));

        vec2x64f zero_ret{_mm_castsi128_pd(_mm_set1_epi64x(FP_ILOGB0))};
        vec2x64f inf_ret {_mm_castsi128_pd(_mm_set1_epi64x(INT_MAX))};
        vec2x64f nan_ret {_mm_castsi128_pd(_mm_set1_epi64x(FP_ILOGBNAN))};

        // Return value when input is not edge case
        auto misc_ret_i = _mm_cvtpd_epi64(exp_fp);
        misc_ret_i = _mm_maskz_mov_epi64(_mm_cmpneq_epi64_mask(misc_ret_i, _mm_set1_epi64x(0x8000000000000000ll)), misc_ret_i);

        vec2x64i zero_ret_i{_mm_castpd_si128(_mm_fixupimm_pd(zero_ret, exp_fp, _mm_set1_epi64x(0x88808888), 0x00))};
        vec2x64i inf_ret_i {_mm_castpd_si128(_mm_fixupimm_pd(inf_ret,  exp_fp, _mm_set1_epi64x(0x88088888), 0x00))};
        vec2x64i nan_ret_i {_mm_castpd_si128(_mm_fixupimm_pd(nan_ret,  exp_fp, _mm_set1_epi64x(0x88888800), 0x00))};

        return (vec2x64i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);

        #elif defined(AVEL_AVX512VL)
        auto exp_fp = _mm_getexp_pd(decay(x));

        vec2x64f zero_ret{_mm_castsi128_pd(_mm_set1_epi64x(FP_ILOGB0))};
        vec2x64f inf_ret {_mm_castsi128_pd(_mm_set1_epi64x(INT_MAX))};
        vec2x64f nan_ret {_mm_castsi128_pd(_mm_set1_epi64x(FP_ILOGBNAN))};

        // Return value when input is not edge case
        auto misc_ret_i_32 = _mm_cvtpd_epi32(exp_fp);
        auto misc_ret_i = _mm_cvtepi32_epi64(misc_ret_i_32);
        misc_ret_i = _mm_maskz_mov_epi64(_mm_cmpneq_epi64_mask(misc_ret_i, _mm_set1_epi64x(0xffffffff80000000ll)), misc_ret_i);

        vec2x64i zero_ret_i{_mm_castpd_si128(_mm_fixupimm_pd(zero_ret, exp_fp, _mm_set1_epi64x(0x88808888), 0x00))};
        vec2x64i inf_ret_i {_mm_castpd_si128(_mm_fixupimm_pd(inf_ret,  exp_fp, _mm_set1_epi64x(0x88088888), 0x00))};
        vec2x64i nan_ret_i {_mm_castpd_si128(_mm_fixupimm_pd(nan_ret,  exp_fp, _mm_set1_epi64x(0x88888800), 0x00))};

        return (vec2x64i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);

        #elif defined(AVEL_SSE2)
        auto is_subnormal = avel::abs(x) < vec2x64f{DBL_MIN};
        auto multiplier = blend(is_subnormal, vec2x64f{9007199254740992.0f}, vec2x64f{1.0f});

        auto x_corrected = x * multiplier;

        auto bits = avel::bit_cast<vec2x64i>(x_corrected);

        vec2x64i exponents = avel::bit_shift_right<52>(bits & vec2x64i{double_exponent_mask_bits});
        vec2x64i bias = blend(bit_cast<mask2x64i>(is_subnormal), vec2x64i{1023 + 53}, vec2x64i{1023});

        auto ret = exponents - bias;

        ret = blend(mask2x64i{_mm_castpd_si128(decay(x == vec2x64f{0.0}))}, vec2x64i{FP_ILOGB0}, ret);
        ret = blend(mask2x64i{_mm_castpd_si128(decay(avel::isinf(x)))}, vec2x64i{INT_MAX}, ret);
        ret = blend(mask2x64i{_mm_castpd_si128(decay(avel::isnan(x)))}, vec2x64i{FP_ILOGBNAN}, ret);

        return ret;

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f logb(vec2x64f x) {
        #if defined(AVEL_AVX512VL)
        return vec2x64f{_mm_getexp_pd(decay(x))};

        #elif defined(AVEL_SSE2)
        auto is_subnormal = avel::abs(x) < vec2x64f{DBL_MIN};
        auto multiplier = blend(is_subnormal, vec2x64f{9007199254740992.0}, vec2x64f{1.0f});

        auto x_corrected = x * multiplier;

        auto bits = avel::bit_cast<vec2x64i>(x_corrected);

        auto exponents_i = avel::bit_shift_right<52>(bits & vec2x64i{double_exponent_mask_bits});

        auto narrowed_exponents = _mm_shuffle_epi32(decay(exponents_i), 0xF8);

        vec2x64f exponents{_mm_cvtepi32_pd(narrowed_exponents)};
        auto bias = blend(is_subnormal, vec2x64f{1023 + 53}, vec2x64f{1023});

        auto ret = exponents - bias;

        ret = blend(x == vec2x64f{0.0f}, vec2x64f{-INFINITY}, ret);
        ret = blend(avel::isinf(x), vec2x64f{INFINITY}, ret);
        ret = blend(avel::isnan(x), vec2x64f{NAN}, ret);

        return ret;

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f copysign(vec2x64f mag, vec2x64f sign) {
        #if defined(AVEL_AVX512VL)
        auto mask = _mm_set1_pd(double_sign_bit_mask);
        auto ret = _mm_ternarylogic_epi64(decay(sign), decay(mag), mask, 0xe4);
        return vec2x64f{_mm_castsi128_pd(ret)};

        #elif defined(AVEL_SSE2)
        auto mask = _mm_set1_pd(double_sign_bit_mask);
        auto ret = _mm_or_pd(_mm_and_pd(mask, decay(sign)), _mm_andnot_pd(mask, decay(mag)));
        return vec2x64f{ret};
        #endif

        #if defined(AVEL_NEON)
        auto mask = vdupq_n_u64(double_sign_bit_mask_bits);
        auto ret = 	vbslq_f64(mask, decay(mag), decay(sign));

        return vec2x64f{ret};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64i fpclassify(vec2x64f v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        const vec2x64i fp_infinite{int(FP_INFINITE)};
        const vec2x64i fp_nan{int(FP_NAN)};
        const vec2x64i fp_normal{int(FP_NORMAL)};
        const vec2x64i fp_subnormal{int(FP_SUBNORMAL)};
        const vec2x64i fp_zero{int(FP_ZERO)};

        mask2x64i infinite_mask {_mm_fpclass_pd_mask(decay(v), 0x08 | 0x10)};
        mask2x64i nan_mask      {_mm_fpclass_pd_mask(decay(v), 0x01 | 0x80)};
        mask2x64i subnormal_mask{_mm_fpclass_pd_mask(decay(v), 0x20)};
        mask2x64i zero_mask     {_mm_fpclass_pd_mask(decay(v), 0x02 | 0x04)};
        mask2x64i normal_mask   {!(infinite_mask | nan_mask | subnormal_mask | zero_mask)};

        return
            keep(infinite_mask, fp_infinite) |
            keep(nan_mask, fp_nan) |
            keep(normal_mask, fp_normal) |
            keep(subnormal_mask, fp_subnormal) |
            keep(zero_mask, fp_zero);

        #elif defined(AVEL_SSE2)
        const auto fp_infinite = _mm_set1_epi64x(int(FP_INFINITE));
        const auto fp_nan = _mm_set1_epi64x(int(FP_NAN));
        const auto fp_normal = _mm_set1_epi64x(int(FP_NORMAL));
        const auto fp_subnormal = _mm_set1_epi64x(int(FP_SUBNORMAL));
        const auto fp_zero = _mm_set1_epi64x(int(FP_ZERO));

        auto infinity = _mm_set1_pd(INFINITY);
        auto dbl_min = _mm_set1_pd(DBL_MIN);

        v = avel::abs(v);

        auto zero_mask      = _mm_castpd_si128(_mm_cmpeq_pd(decay(v), _mm_setzero_pd()));
        auto subnormal_mask = _mm_castpd_si128(_mm_andnot_pd(_mm_cmpeq_pd(decay(v), _mm_setzero_pd()), _mm_cmple_pd(decay(v), dbl_min)));
        auto infinite_mask  = _mm_castpd_si128(_mm_cmpeq_pd(decay(v), infinity));
        auto nan_mask       = _mm_castpd_si128(_mm_cmpunord_pd(decay(v), decay(v)));
        auto normal_mask    = _mm_castpd_si128(_mm_and_pd(_mm_cmple_pd(dbl_min, decay(v)), _mm_cmplt_pd(decay(v), infinity)));

        return vec2x64i{
            _mm_or_si128(
                _mm_and_si128(infinite_mask, fp_infinite),
                _mm_or_si128(
                    _mm_or_si128(
                        _mm_and_si128(nan_mask, fp_nan),
                        _mm_and_si128(normal_mask, fp_normal)
                    ),
                    _mm_or_si128(
                        _mm_and_si128(subnormal_mask, fp_subnormal),
                        _mm_and_si128(zero_mask, fp_zero)
                    )
                )
            )
        };

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isfinite(vec2x64f v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return !mask2x64f{_mm_fpclass_pd_mask(decay(v), 0x01 | 0x08 | 0x10 | 0x80)};

        #elif defined(AVEL_SSE2)
        return (vec2x64f{-INFINITY} < v) && (v < vec2x64f{+INFINITY});

        #endif

        #if defined(AVEL_NEON)
        return mask2x64f{vcaleq_f64(decay(v), vdupq_n_f64(INFINITY))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isinf(vec2x64f v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return mask2x64f{_mm_fpclass_pd_mask(decay(v), 0x08 | 0x10)};

        #elif defined(AVEL_SSE2)
        return avel::abs(v) == vec2x64f{INFINITY};

        #endif

        #if defined(AVEL_NEON)
        return mask2x64f{vcaleq_f64(vdupq_n_f64(INFINITY), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isnan(vec2x64f v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return mask2x64f{_mm_fpclass_pd_mask(decay(v), 0x80 | 0x01)};

        #elif defined(AVEL_AVX512VL)
        return mask2x64f{_mm_cmp_pd_mask(decay(v), decay(v), _CMP_UNORD_Q)};

        #elif defined(AVEL_SSE)
        return mask2x64f{_mm_cmpunord_pd(decay(v), decay(v))};

        #endif

        #if defined(AVEL_NEON)
        return v != v;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isnormal(vec2x64f v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return !mask2x64f{_mm_fpclass_pd_mask(decay(v), 0xBF)};

        #elif defined(AVEL_SSE2)
        auto abs_v = avel::abs(v);
        return (vec2x64f{DBL_MIN} <= abs_v) && (abs_v <= vec2x64f{DBL_MAX});

        #endif

        #if defined(AVEL_NEON)
        return mask2x64f{vandq_u64(vcaleq_f64(vdupq_n_f64(DBL_MIN), decay(v)), vcaleq_f64(decay(v), vdupq_n_f64(DBL_MAX)))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f signbit(vec2x64f arg) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return mask2x64f{_mm_fpclass_pd_mask(decay(arg), 0x40 | 0x04 | 0x10)};

        #elif defined(AVEL_AVX512VL)
        return mask2x64f{_mm_cmplt_epi64_mask(_mm_castpd_si128(decay(arg)), _mm_setzero_si128())};

        #elif defined(AVEL_SSE2)
        auto ret_32 = _mm_srai_epi32(_mm_castpd_si128(decay(arg)), 31);
        auto ret = _mm_shuffle_epi32(ret_32, 0xf5);
        return mask2x64f{_mm_castsi128_pd(ret)};

        #endif

        #if defined(AVEL_NEON)
        return mask2x64f{vreinterpretq_u64_s64(vshrq_n_s64(vreinterpretq_s64_f64(decay(arg)), 63))};
        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask2x64f isgreater(vec2x64f x, vec2x64f y) {
        return x > y;
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isgreaterequal(vec2x64f x, vec2x64f y) {
        return x >= y;
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isless(vec2x64f x, vec2x64f y) {
        return x < y;
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f islessequal(vec2x64f x, vec2x64f y) {
        return x <= y;
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f islessgreater(vec2x64f x, vec2x64f y) {
        #if defined(AVEL_AVX512VL)
        return mask2x64f{_mm_cmp_pd_mask(decay(x), decay(y), _CMP_NEQ_OQ)};

        #elif defined(AVEL_AVX)
        return mask2x64f{_mm_cmp_pd(decay(x), decay(y), _CMP_NEQ_OQ)};

        #elif defined(AVEL_SSE2)
        return x < y || x > y;
        #endif

        #if defined(AVEL_NEON)
        return x < y || x > y;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isunordered(vec2x64f x, vec2x64f y) {
        #if defined(AVEL_AVX512VL)
        return mask2x64f{_mm_cmp_pd_mask(decay(x), decay(y), _CMP_UNORD_Q)};

        #elif defined(AVEL_SSE2)
        return mask2x64f{_mm_cmpunord_pd(decay(x), decay(y))};

        #endif

        #if defined(AVEL_NEON)
        auto a = vceqq_f64(decay(x), decay(x));
        auto b = vceqq_f64(decay(y), decay(y));
        return mask2x64f{vtstq_u64(a, b)};
        #endif
    }

}

#endif //AVEL_VEC2x64F_HPP
