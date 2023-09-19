#ifndef AVEL_VEC8X64F_HPP
#define AVEL_VEC8X64F_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x64f = Vector<double, 8>;
    using arr8x64f = std::array<double, 8>;
    using mask8x64f = Vector_mask<double, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL mask8x64f isnan(vec8x64f v);
    AVEL_FINL vec8x64f copysign(vec8x64f mag, vec8x64f sign);





    template<>
    class Vector_mask<double, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512F)
        using primitive = __mmask8;
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

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512F)
            content(b ? 0xff : 0x00) {}
        #endif

        AVEL_FINL explicit Vector_mask(primitive p):
            content(p) {}

        AVEL_FINL explicit Vector_mask(const arr8xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si64(arr.data());
            content = static_cast<primitive>(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            auto expanded = _mm256_cvtepi8_epi32(array_data);
            content = _mm256_cmplt_epu32_mask(_mm256_setzero_si256(), expanded);

            #elif defined(AVEL_AVX512F)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            auto expanded = _mm512_castsi256_si512(_mm256_cvtepi8_epi32(array_data));
            content = _mm512_cmplt_epu32_mask(_mm512_setzero_si512(), expanded);

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
            return Vector_mask{primitive(~content)};
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
    AVEL_FINL std::uint32_t count(mask8x64f m) {
        return popcount(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x64f m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask8x64f m) {
        return 0xff == decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x64f m) {
        return !decay(m);
    }




    template<>
    class Vector<double, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = double;

        #if defined(AVEL_AVX512F)
        using primitive = __m512d;
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
        AVEL_FINL explicit Vector(Vector<U, width> x):
            Vector(convert<Vector>(x)[0]) {}

        AVEL_FINL explicit Vector(mask m):
            content(_mm512_mask_blend_pd(decay(m), _mm512_set1_pd(0.0), _mm512_set1_pd(1.0))) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm512_set1_pd(x)) {}

        AVEL_FINL explicit Vector(const arr8x64f& arr):
            content(_mm512_loadu_pd(arr.data())) {}

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
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_EQ_OS)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_NEQ_OS)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_LT_OS)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_LE_OS)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_GT_OS)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_GE_OS)};
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

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content = _mm512_add_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm512_sub_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm512_mul_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            content = _mm512_div_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
            //TODO: Implement
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
        // Conversion operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            auto t = _mm512_castpd_si512(content);
            return mask{_mm512_test_epi64_mask(t, t)};
        }

    };

    static_assert(
        8 * sizeof(double) == sizeof(vec8x64f),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL double extract(vec8x64f v) {
        static_assert(N < vec8x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x64f::width, int>::type dummy_variable = 0;

        auto quarter = _mm_castps_pd(_mm512_extractf32x4_ps(_mm512_castpd_ps(decay(v)), N / 2));

        // Rely on const-folding to eliminate branch
        if (N % 2 == 1) {
            quarter = _mm_unpackhi_pd(quarter, quarter);
        }

        return _mm_cvtsd_f64(quarter);
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL vec8x64f insert(vec8x64f v, double x) {
        static_assert(N < vec8x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x64f::width, int>::type dummy_variable = 0;

        auto broadcasted = _mm512_set1_pd(x);
        constexpr auto blend_mask = std::uint8_t(~(1 << N));
        auto ret = _mm512_mask_blend_pd(blend_mask, broadcasted, decay(v));
        return vec8x64f{ret};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec8x64f x) {
        return count(mask8x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec8x64f x) {
        return any(mask8x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec8x64f x) {
        return all(mask8x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec8x64f x) {
        return none(mask8x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f keep(mask8x64f m, vec8x64f v) {
        return vec8x64f{_mm512_maskz_mov_pd(decay(m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f clear(mask8x64f m, vec8x64f v) {
        return vec8x64f{_mm512_maskz_mov_pd(decay(!m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f blend(mask8x64f m, vec8x64f a, vec8x64f b) {
        return vec8x64f{_mm512_mask_blend_pd(decay(m), decay(b), decay(a))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f byteswap(vec8x64f v) {
        return bit_cast<vec8x64f>(byteswap(bit_cast<vec8x64u>(v)));
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f max(vec8x64f a, vec8x64f b) {
        return vec8x64f{_mm512_max_pd(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f min(vec8x64f a, vec8x64f b) {
        return vec8x64f{_mm512_min_pd(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x64f, 2> minmax(vec8x64f a, vec8x64f b) {
        return {
            vec8x64f{_mm512_min_pd(decay(a), decay(b))},
            vec8x64f{_mm512_max_pd(decay(a), decay(b))}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f clamp(vec8x64f x, vec8x64f lo, vec8x64f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f negate(mask8x64f m, vec8x64f v) {
        return vec8x64f{_mm512_mask_sub_pd(decay(v), decay(m), _mm512_setzero_pd(), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f abs(vec8x64f v) {
        return vec8x64f{_mm512_abs_pd(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f neg_abs(vec8x64f v) {
        #if defined(AVEL_AVX512DQ)
        return vec8x64f{_mm512_or_pd(_mm512_set1_pd(double_sign_bit_mask), decay(v))};

        #elif defined(AVEL_AVX512F)
        auto bits = _mm512_castpd_si512(decay(v));
        auto mask = _mm512_set1_epi64(double_sign_bit_mask_bits);
        auto result = _mm512_or_si512(bits, mask);
        return vec8x64f{_mm512_castsi512_pd(result)};

        #endif
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f load<vec8x64f>(const double* ptr, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x64f{_mm512_maskz_loadu_pd(mask, ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f load<vec8x64f, vec8x64f::width>(const double* ptr) {
        return vec8x64f{_mm512_loadu_pd(ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f aligned_load<vec8x64f>(const double* ptr, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x64f{_mm512_maskz_load_pd(mask, ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f aligned_load<vec8x64f, vec8x64f::width>(const double* ptr) {
        return vec8x64f{_mm512_load_pd(ptr)};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f gather<vec8x64f>(const double* ptr, vec8x64i indices, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x64f{_mm512_mask_i64gather_pd(_mm512_setzero_pd(), mask, decay(indices), ptr, sizeof(double))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f gather<vec8x64f>(const double* ptr, vec8x64i indices) {
        return vec8x64f{_mm512_i64gather_pd(decay(indices), ptr, sizeof(double))};
    }



    AVEL_FINL void store(double* ptr, vec8x64f v, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm512_mask_storeu_pd(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec8x64f::width>
    AVEL_FINL void store(double* ptr, vec8x64f v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec8x64f::width>(double* ptr, vec8x64f v) {
        _mm512_storeu_pd(ptr, decay(v));
    }



    AVEL_FINL void aligned_store(double* ptr, vec8x64f v, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm512_mask_store_pd(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec8x64f::width>
    AVEL_FINL void aligned_store(double* ptr, vec8x64f v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec8x64f::width>(double* ptr, vec8x64f v) {
        _mm512_store_pd(ptr, decay(v));
    }



    AVEL_FINL void scatter(double* ptr, vec8x64f v, vec8x64i indices, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm512_mask_i64scatter_pd(ptr, mask, decay(indices), decay(v), sizeof(double));
    }

    template<std::uint32_t N = vec8x64f::width>
    AVEL_FINL void scatter(double* ptr, vec8x64f v, vec8x64i indices) {
        scatter(ptr, v, indices, N);
    }

    template<>
    AVEL_FINL void scatter<0>(double* ptr, vec8x64f v, vec8x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec8x64f::width>(double* ptr, vec8x64f v, vec8x64i indices) {
        _mm512_i64scatter_pd(ptr, decay(indices), decay(v), sizeof(double));
    }



    [[nodiscard]]
    AVEL_FINL arr8x64f to_array(vec8x64f v) {
        alignas(64) arr8x64f ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Cmath functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x64f fmax(vec8x64f a, vec8x64f b) {
        return blend(avel::isnan(b), a, avel::max(a, b));
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f fmin(vec8x64f a, vec8x64f b) {
        return blend(avel::isnan(b), a, avel::min(a, b));
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x64f sqrt(vec8x64f x) {
        return vec8x64f{_mm512_sqrt_pd(decay(x))};
    }

    //=====================================================
    // Nearest integer function
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x64f ceil(vec8x64f v) {
        return vec8x64f{_mm512_roundscale_pd(decay(v), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f floor(vec8x64f v) {
        return vec8x64f{_mm512_roundscale_pd(decay(v), _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f trunc(vec8x64f v) {
        return vec8x64f{_mm512_roundscale_pd(decay(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f round(vec8x64f v) {
        auto whole = trunc(v);
        auto frac = v - whole;

        auto offset = copysign(vec8x64f{1.0}, v);
        auto should_offset = abs(frac) >= vec8x64f{0.5};
        auto ret = whole + keep(should_offset, offset);

        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f nearbyint(vec8x64f v) {
        return vec8x64f{_mm512_roundscale_pd(decay(v), _MM_FROUND_CUR_DIRECTION)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f rint(vec8x64f v) {
        return vec8x64f{_mm512_roundscale_pd(decay(v), _MM_FROUND_CUR_DIRECTION)};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x64f frexp(vec8x64f v, vec8x64i* exp) {
        #if defined(AVEL_AVX512DQ)
        auto is_infinity = _mm512_fpclass_pd_mask(decay(v), 0x10 | 0x08);
        auto is_non_zero = _mm512_cmp_pd_mask(decay(v), _mm512_setzero_pd(), _CMP_NEQ_UQ);

        auto exponents = _mm512_getexp_pd(decay(v));
        exponents = _mm512_add_pd(exponents, _mm512_set1_pd(1.0f));
        *exp = _mm512_maskz_cvttpd_epi64(is_non_zero, exponents);

        auto ret = _mm512_getmant_pd(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        // Note: Returns -1 or 1 for -infinity and +infinity respectively

        ret = _mm512_maskz_mov_pd(is_non_zero, ret);
        ret = _mm512_mask_blend_pd(is_infinity, ret, decay(v));
        return vec8x64f{ret};

        #elif defined(AVEL_AVX512F)
        auto is_infinity = decay(abs(v) == vec8x64f{INFINITY});
        auto is_non_zero = _mm512_cmp_pd_mask(decay(v), _mm512_setzero_pd(), _CMP_NEQ_UQ);

        auto exponents = _mm512_getexp_pd(decay(v));
        exponents = _mm512_add_pd(exponents, _mm512_set1_pd(1.0f));
        auto exp32 = _mm512_maskz_cvttpd_epi32(is_non_zero, exponents);

        *exp = _mm512_cvtepi32_epi64(exp32);

        auto ret = _mm512_getmant_pd(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        // Note: Returns -1 or 1 for -infinity and +infinity respectively

        ret = _mm512_maskz_mov_pd(is_non_zero, ret);
        ret = _mm512_mask_blend_pd(is_infinity, ret, decay(v));
        return vec8x64f{ret};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f ldexp(vec8x64f arg, vec8x64i exp) {
        #if defined(AVEL_AVX512DQ)
        return vec8x64f{_mm512_scalef_pd(decay(arg), _mm512_cvtepi64_pd(decay(exp)))};

        #elif defined(AVEL_AVX512F)
        auto exp32 = _mm512_cvtsepi64_epi32(decay(exp));
        auto exp_as_f64 = _mm512_cvtepi32_pd(exp32);
        return vec8x64f{_mm512_scalef_pd(decay(arg), exp_as_f64)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f scalbn(vec8x64f x, vec8x64i exp) {
        return avel::ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i ilogb(vec8x64f x) {
        #if defined(AVEL_AVX512DQ)
        auto exp_fp = _mm512_getexp_pd(decay(x));

        vec8x64f zero_ret{_mm512_castsi512_pd(_mm512_set1_epi64(FP_ILOGB0))};
        vec8x64f inf_ret {_mm512_castsi512_pd(_mm512_set1_epi64(INT_MAX))};
        vec8x64f nan_ret {_mm512_castsi512_pd(_mm512_set1_epi64(FP_ILOGBNAN))};

        // Return value when input is not edge case
        auto misc_ret_i = _mm512_cvtpd_epi64(exp_fp);
        misc_ret_i = _mm512_maskz_mov_epi64(_mm512_cmpneq_epi64_mask(misc_ret_i, _mm512_set1_epi64(0x8000000000000000ll)), misc_ret_i);

        vec8x64i zero_ret_i{_mm512_castpd_si512(_mm512_fixupimm_pd(decay(zero_ret), exp_fp, _mm512_set1_epi64(0x88808888), 0x00))};
        vec8x64i inf_ret_i {_mm512_castpd_si512(_mm512_fixupimm_pd(decay(inf_ret),  exp_fp, _mm512_set1_epi64(0x88088888), 0x00))};
        vec8x64i nan_ret_i {_mm512_castpd_si512(_mm512_fixupimm_pd(decay(nan_ret),  exp_fp, _mm512_set1_epi64(0x88888800), 0x00))};

        return (vec8x64i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);

        #elif defined(AVEL_AVX512F)
        auto exp_fp = _mm512_getexp_pd(decay(x));

        // Constants to return when inputs are edge cases
        vec8x64f zero_ret{_mm512_castsi512_pd(_mm512_set1_epi64(FP_ILOGB0))};
        vec8x64f inf_ret {_mm512_castsi512_pd(_mm512_set1_epi64(INT_MAX))};
        vec8x64f nan_ret {_mm512_castsi512_pd(_mm512_set1_epi64(FP_ILOGBNAN))};

        // Return value when input is not edge case
        auto misc_ret_i_32 = _mm512_cvtpd_epi32(exp_fp);
        auto misc_ret_i = _mm512_cvtepi32_epi64(misc_ret_i_32);
        misc_ret_i = _mm512_maskz_mov_epi64(_mm512_cmpneq_epi64_mask(misc_ret_i, _mm512_set1_epi64(0xffffffff80000000ll)), misc_ret_i);

        vec8x64i zero_ret_i{_mm512_castpd_si512(_mm512_fixupimm_pd(decay(zero_ret), exp_fp, _mm512_set1_epi64(0x88808888), 0x00))};
        vec8x64i inf_ret_i {_mm512_castpd_si512(_mm512_fixupimm_pd(decay(inf_ret),  exp_fp, _mm512_set1_epi64(0x88088888), 0x00))};
        vec8x64i nan_ret_i {_mm512_castpd_si512(_mm512_fixupimm_pd(decay(nan_ret),  exp_fp, _mm512_set1_epi64(0x88888800), 0x00))};

        return (vec8x64i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f logb(vec8x64f x) {
        return vec8x64f{_mm512_getexp_pd(decay(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f copysign(vec8x64f mag, vec8x64f sign) {
        #if defined(AVEL_AVX512F)
        auto mask = _mm512_set1_epi64(double_sign_bit_mask_bits);
        auto ret = _mm512_ternarylogic_epi32(_mm512_castpd_si512(decay(sign)), _mm512_castpd_si512(decay(mag)), mask, 0xe4);
        return vec8x64f{_mm512_castsi512_pd(ret)};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x64i fpclassify(vec8x64f v) {
        #if defined(AVEL_AVX512DQ)
        const vec8x64i fp_infinite{int(FP_INFINITE)};
        const vec8x64i fp_nan{int(FP_NAN)};
        const vec8x64i fp_normal{int(FP_NORMAL)};
        const vec8x64i fp_subnormal{int(FP_SUBNORMAL)};
        const vec8x64i fp_zero{int(FP_ZERO)};

        mask8x64i infinite_mask {_mm512_fpclass_pd_mask(decay(v), 0x08 | 0x10)};
        mask8x64i nan_mask      {_mm512_fpclass_pd_mask(decay(v), 0x01 | 0x80)};
        mask8x64i subnormal_mask{_mm512_fpclass_pd_mask(decay(v), 0x20)};
        mask8x64i zero_mask     {_mm512_fpclass_pd_mask(decay(v), 0x02 | 0x04)};
        mask8x64i normal_mask   {!(infinite_mask | nan_mask | subnormal_mask | zero_mask)};

        return
            keep(infinite_mask, fp_infinite) |
            keep(nan_mask, fp_nan) |
            keep(normal_mask, fp_normal) |
            keep(subnormal_mask, fp_subnormal) |
            keep(zero_mask, fp_zero);

        #elif defined(AVEL_AVX512F)
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm512_castpd_si512(decay(v));

        // Take absolute value
        v_bits = _mm512_and_si512(v_bits, _mm512_set1_epi64(0x7fffffffffffffffull));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm512_set1_epi64(0x10000000000000ull);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm512_set1_epi64(0x7ff0000000000000ull);

        // Masks for individual categories
        auto m0 = _mm512_testn_epi64_mask(v_bits, v_bits);
        auto m1 = ~m0 & _mm512_cmplt_epi64_mask(v_bits, min_bits);
        auto m2 = ~_mm512_cmplt_epi64_mask(v_bits, min_bits) & _mm512_cmplt_epi64_mask(v_bits, inf_bits);
        auto m3 = _mm512_cmpeq_epi64_mask(v_bits, inf_bits);
        auto m4 = _mm512_cmpgt_epi64_mask(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm512_setzero_si512();
        ret = _mm512_maskz_mov_epi64(m0, _mm512_set1_epi64(FP_ZERO));
        ret = _mm512_mask_blend_epi64(m1, ret, _mm512_set1_epi64(FP_SUBNORMAL));
        ret = _mm512_mask_blend_epi64(m2, ret, _mm512_set1_epi64(FP_NORMAL)   );
        ret = _mm512_mask_blend_epi64(m3, ret, _mm512_set1_epi64(FP_INFINITE) );
        ret = _mm512_mask_blend_epi64(m4, ret, _mm512_set1_epi64(FP_NAN)      );

        return vec8x64i{ret};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isfinite(vec8x64f v) {
        #if defined(AVEL_AVX512DQ)
        return !mask8x64f{_mm512_fpclass_pd_mask(decay(v), 0x01 | 0x08 | 0x10 | 0x80)};

        #elif defined(AVEL_AVX512F)
        return (vec8x64f{-INFINITY} < v) && (v < vec8x64f{+INFINITY});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isinf(vec8x64f v) {
        #if defined(AVEL_AVX512DQ)
        return mask8x64f{_mm512_fpclass_pd_mask(decay(v), 0x08 | 0x10)};

        #elif defined(AVEL_AVX512F)
        return avel::abs(v) == vec8x64f{INFINITY};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isnan(vec8x64f v) {
        #if defined(AVEL_AVX512DQ)
        return mask8x64f{_mm512_fpclass_pd_mask(decay(v), 0x80 | 0x01)};

        #elif defined(AVEL_AVX512F)
        return mask8x64f{_mm512_cmp_pd_mask(decay(v), decay(v), _CMP_UNORD_Q)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isnormal(vec8x64f v) {
        #if defined(AVEL_AVX512DQ)
        return !mask8x64f{_mm512_fpclass_pd_mask(decay(v), 0xBF)};

        #elif defined(AVEL_AVX512F)
        auto abs_v = avel::abs(v);
        return (vec8x64f{DBL_MIN} <= abs_v) && (abs_v <= vec8x64f{DBL_MAX});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f signbit(vec8x64f arg) {
        #if defined(AVEL_AVX512DQ)
        return mask8x64f{_mm512_fpclass_pd_mask(decay(arg), 0x40 | 0x04 | 0x10)};

        #elif defined(AVEL_AVX512F)
        return mask8x64f{_mm512_cmplt_epi64_mask(_mm512_castpd_si512(decay(arg)), _mm512_setzero_si512())};
        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask8x64f isgreater(vec8x64f x, vec8x64f y) {
        return mask8x64f{_mm512_cmp_pd_mask(decay(x), decay(y), _CMP_GT_OQ)};
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isgreaterequal(vec8x64f x, vec8x64f y) {
        return mask8x64f{_mm512_cmp_pd_mask(decay(x), decay(y), _CMP_GE_OQ)};
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isless(vec8x64f x, vec8x64f y) {
        return mask8x64f{_mm512_cmp_pd_mask(decay(x), decay(y), _CMP_LT_OQ)};
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f islessequal(vec8x64f x, vec8x64f y) {
        return mask8x64f{_mm512_cmp_pd_mask(decay(x), decay(y), _CMP_LE_OQ)};
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f islessgreater(vec8x64f x, vec8x64f y) {
        return mask8x64f{_mm512_cmp_pd_mask(decay(x), decay(y), _CMP_NEQ_OQ)};
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isunordered(vec8x64f x, vec8x64f y) {
        return mask8x64f{_mm512_cmp_pd_mask(decay(x), decay(y), _CMP_UNORD_Q)};
    }

}

#endif //AVEL_VEC8X64F_HPP
