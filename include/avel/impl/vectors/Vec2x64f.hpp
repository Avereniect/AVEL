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
            __m128i array_data = _mm_set1_epi32(bit_cast<std::uint32_t>(arr));
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
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return lhs.content != rhs.content;

            #elif defined(AVEL_SSE2)
            return _mm_movemask_pd(_mm_xor_pd(lhs.content, rhs.content)) != 0x00;

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
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content |= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_or_pd(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content ^= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_xor_pd(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{primitive(content ^ 0x3)};

            #elif defined(AVEL_SSE2)
            auto ones = _mm_castsi128_pd(_mm_set1_epi64x(-1));
            return Vector_mask{_mm_andnot_pd(content, ones)};
            #endif
        }

        AVEL_VECTOR_MASK_BINARY_BITWISE_OPERATORS

        //=================================================
        // Conversion operators
        //=================================================

        AVEL_FINL operator primitive() const {
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
    class alignas(16) Vector<double, 2> {
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
            content(_mm_mask_blend_pd(decay(m), _mm_set1_pd(0.0), _mm_set1_pd(1.0))) {}
        #elif defined(AVEL_AVX)
            content(_mm256_blendv_pd(_mm256_set1_pd(0.0), _mm256_set1_pd(1.0), decay(m))) {}
        #elif defined(AVEL_SSE2)
            content() {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm_set1_pd(x)) {}

        AVEL_FINL explicit Vector(const arr2x64f& array):
            content(_mm_loadu_pd(array.data())) {}

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
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_NEQ_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpneq_pd(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_LT_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_pd(decay(lhs), decay(rhs))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_LE_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmple_pd(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_GT_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_pd(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_pd_mask(lhs.content, rhs.content, _CMP_GE_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpge_pd(decay(lhs), decay(rhs))};
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
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_pd(content, decay(vec));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_mul_pd(content, decay(vec));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_div_pd(content, decay(vec));
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

        AVEL_FINL operator primitive() const {
            return content;
        }

        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512VL)
            auto t = _mm_castpd_si128(content);
            return mask{_mm_test_epi64_mask(t, t)};

            #elif defined(AVEL_AVX)
            return mask{_mm_cmp_pd(content, _mm_setzero_pd(), _CMP_NEQ_UQ)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpneq_pd(content, _mm_setzero_pd())};

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
    }

    template<std::uint32_t N>
    AVEL_FINL vec2x64f insert(vec2x64f v, double x) {
        static_assert(N < vec2x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec2x64f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        auto ret = decay(v);
        ret[N] = x;
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
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f clear(mask2x64f m, vec2x64f v) {
        #if defined(AVEL_AVX512VL)
        return vec2x64f{_mm_maskz_mov_pd(decay(!m), decay(v))};

        #elif defined(AVEL_SSE2)
        return vec2x64f{_mm_andnot_pd(decay(m), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f blend(mask2x64f m, vec2x64f a, vec2x64f b) {
        #if defined(AVEL_AVX512VL)
        return vec2x64f{_mm_mask_blend_pd(decay(m), decay(b), decay(a))};
        #elif defined(AVEL_SSE41)
        return vec2x64f{_mm_blendv_pd(decay(b), decay(a), decay(n))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_pd(decay(m), decay(b));
        auto y = _mm_and_pd(decay(m), decay(a));
        return vec2x64f{_mm_or_pd(x, y)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f max(vec2x64f a, vec2x64f b) {
        #if defined(AVEL_SSE2)
        return vec2x64f{_mm_max_pd(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f min(vec2x64f a, vec2x64f b) {
        #if defined(AVEL_SSE2)
        return vec2x64f{_mm_min_pd(decay(a), decay(b))};
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
        auto negation_mask = _mm_and_pd(decay(m), _mm_castsi128_pd(_mm_set1_pd(double_sign_bit_mask)));
        return vec2x64f{_mm_xor_pd(decay(v), negation_mask)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f abs(vec2x64f v) {
        #if defined(AVEL_SSE)
        return vec2x64f{_mm_andnot_pd(_mm_set1_pd(double_sign_bit_mask), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vabsq_f32(decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f neg_abs(vec2x64f v) {
        #if defined(AVEL_SSE)
        return vec2x64f{_mm_or_pd(_mm_set1_pd(double_sign_bit_mask), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec2x64f{vsubq_f32(vdupq_n_f32(0.0f), vabsq_f32(decay(v)))};
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

        #elif defined(AVEL_AVX2)
        _mm_maskload_pd();

        #elif defined(AVEL_SSE2)

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f load<vec2x64f, vec2x64f::width>(const double* ptr) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f aligned_load<vec2x64f>(const double* ptr, std::uint32_t n) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f aligned_load<vec2x64f, vec2x64f::width>(const double* ptr) {
        //TODO: Implement
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f gather<vec2x64f>(const double* ptr, vec2x64i indices, std::uint32_t n) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f gather<vec2x64f>(const double* ptr, vec2x64i indices) {
        //TODO: Implement
    }



    template<std::uint32_t N = vec2x64f::width>
    AVEL_FINL void store(double* ptr, vec2x64f v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void store<vec2x64f::width>(double* ptr, vec2x64f v) {
        //TODO: Implement
    }

    AVEL_FINL void store(double* ptr, vec2x64f v, std::uint32_t n) {
        //TODO: Implement
    }



    template<std::uint32_t N = vec2x64f::width>
    AVEL_FINL void aligned_store(double* ptr, vec2x64f v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void aligned_store<vec2x64f::width>(double* ptr, vec2x64f v) {
        //TODO: Implement
    }

    AVEL_FINL void aligned_store(double* ptr, vec2x64f v, std::uint32_t n) {
        //TODO: Implement
    }



    AVEL_FINL void scatter(double* ptr, vec2x64f v, vec2x64i indices, std::uint32_t n) {
        //TODO: Implement
    }

    template<std::uint32_t N = vec2x64f::width>
    AVEL_FINL void scatter(double* ptr, vec2x64f v, vec2x64i indices) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void scatter<0>(double* ptr, vec2x64f v, vec2x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec2x64f::width>(double* ptr, vec2x64f v, vec2x64i indices) {
        //TODO: Implement
    }



    [[nodiscard]]
    AVEL_FINL arr1x64f to_array(vec2x64f v) {
        alignas(8) arr1x64f ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // cmath basic operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64f fmax(vec2x64f a, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fmin(vec2x64f a, vec2x64f b) {
        //TODO: Implement
    }

    /*
    [[nodiscard]]
    AVEL_FINL vec2x64f fabs(vec2x64f v) {
        return abs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fabsf(vec2x64f v) {
        return fabs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fmod(vec2x64f x, vec2x64f y) {
        return x % y;
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fmodf(vec2x64f x, vec2x64f y) {
        return x % y;
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f remainderf(vec2x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f remquof(vec2x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fmadd(vec2x64f m, vec2x64f x, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fmsubb(vec2x64f m, vec2x64f x, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fnmadd(vec2x64f m, vec2x64f x, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fnmsub(vec2x64f m, vec2x64f x, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fma(vec2x64f m, vec2x64f x, vec2x64f b) {
        return fmadd(m, x, b);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fmaf(vec2x64f m, vec2x64f x, vec2x64f b) {
        return fmadd(m, x, b);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fmin(vec2x64f a, vec2x64f b) {
        return fminf(a, b);
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fdim(vec2x64f a, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f lerp(vec2x64f a, vec2x64f b, vec2x64f t);
    */

    //=====================================================
    // Exponential functions
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL vec2x64f exp(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f exp2(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f expm1(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f log(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f log10(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f log2(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f log1p(vec2x64f x) {
        //TODO: Implement
    }
    */

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64f sqrt(vec2x64f x) {
        return vec2x64f{}; //TODO: Implement
    }

    //=====================================================
    // Nearest Integer floating-point operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64f ceil(vec2x64f x) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f floor(vec2x64f x) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f trunc(vec2x64f x) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f round(vec2x64f v) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f nearbyint(vec2x64f v) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f rint(vec2x64f v) {
        return {}; //TODO: Implement
    }

    //=====================================================
    // floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64f frexp(vec2x64f v, vec2x64i* exp) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f ldexp(vec2x64f x, vec2x64i exp) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f modf(vec2x64f x, vec2x64f* iptr) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f scalbn(vec2x64f x, vec2x64i exp) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64i ilogb(vec2x64f x) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f logb(vec2x64f x) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f copysign(vec2x64f mag, vec2x64f sign) {
        #if defined(AVEL_SSE2)
        auto mask = _mm_set1_pd(double_sign_bit_mask);
        auto ret = _mm_or_pd(_mm_and_pd(mask, decay(sign)), _mm_andnot_pd(mask, decay(mag)));
        return vec2x64f{ret};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec2x64i fpclassify(vec2x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isnan(vec2x64f v) {
        return (v != v);
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isfinite(vec2x64f arg) {
        #if defined(AVEL_AVX512VL)
        return  vec2x64f{_mm_getexp_pd(decay(arg))} != vec2x64f{1023.0f};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isinf(vec2x64f v) {
        return abs(v) == vec2x64f{INFINITY};
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isnormal(vec2x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isunordered(vec2x64f x, vec2x64f y) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f signbit(vec2x64f arg) {
        #if defined(AVEL_AVX512VL)
        return mask2x64f{_mm_fpclass_pd_mask(decay(arg), 0x40)};

        #elif defined(AVEL_AVX2)


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
        return x != y;
    }

}

#endif //AVEL_VEC2x64F_HPP
