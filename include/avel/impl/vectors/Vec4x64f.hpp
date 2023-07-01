#ifndef AVEL_VEC4X64F_HPP
#define AVEL_VEC4X64F_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x64f = Vector<double, 4>;
    using arr4x64f = std::array<double, 4>;
    using mask4x64f = Vector_mask<double, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec4x64f trunc(vec4x64f x);
    AVEL_FINL vec4x64f abs(vec4x64f v);
    AVEL_FINL vec4x64f fmod(vec4x64f a, vec4x64f b);
    AVEL_FINL vec4x64f blend(vec4x64f, vec4x64f, mask4x64f);
    AVEL_FINL vec4x64f round(vec4x64f);
    AVEL_FINL mask4x64f isunordered(vec4x64f, vec4x64f);
    AVEL_FINL mask4x64f signbit(vec4x64f x);
    AVEL_FINL mask4x64f isnan(vec4x64f v);

    template<>
    class Vector_mask<double, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512F)
        using primitive = __mmask8;
        #elif defined(AVEL_AVX)
        using primitive = __m256;
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
            content(b ? 0x0f : 0x00) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr4xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );


            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m128i array_data = _mm_set1_epi32(bit_cast<std::uint32_t>(arr));
            content = __mmask8(_mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            auto expanded = _mm_cvtepi8_epi32(array_data);
            content = _mm_cmplt_epu32_mask(_mm_setzero_si128(), expanded);

            #elif defined(AVEL_AVX2)
            __m128i array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            array_data = _mm_unpacklo_epi8(array_data, array_data);
            array_data = _mm_unpacklo_epi16(array_data, array_data);
            content = _mm_castsi128_ps(_mm_cmplt_epi32(_mm_setzero_si128(), array_data));

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

            #elif defined(AVEL_AVX2)


            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return lhs.content != rhs.content;

            #elif defined(AVEL_AVX2)


            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content &= rhs.content;

            #elif defined(AVEL_AVX2)


            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content |= rhs.content;

            #elif defined(AVEL_AVX2)


            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content ^= rhs.content;

            #elif defined(AVEL_AVX2)


            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{primitive(content ^ 0x0f)};

            #elif defined(AVEL_AVX2)
            return {};

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
    AVEL_FINL std::uint32_t count(mask4x64f m) {
        #if defined(AVEL_AVX512VL)
        return avel::popcount(decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x64f m) {
        #if defined(AVEL_AVX512VL)
        return decay(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x64f m) {
        #if defined(AVEL_AVX512VL)
        return 0x0f == decay(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x64f m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================






    template<>
    class Vector<double, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = double;

        #if defined(AVEL_AVX)
        using primitive = __m256d;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            content(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm256_mask_blend_pd(decay(m), _mm256_set1_pd(0.0), _mm256_set1_pd(1.0))) {}
        #elif defined(AVEL_AVX)
            content(_mm256_blendv_pd(_mm256_set1_pd(0.0), _mm256_set1_pd(1.0), decay(m))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm256_set1_pd(x)) {}

        AVEL_FINL explicit Vector(const arr4x64f& array):
            content(_mm256_loadu_pd(array.data())) {}

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

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_EQ_OS)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_NEQ_OS)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_LT_OS)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_LE_OS)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_GT_OS)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_GE_OS)};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            return Vector{0.0} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content = _mm256_add_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm256_sub_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm256_mul_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            content = _mm256_div_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
            //TODO: Call fmod
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

        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512VL)
            auto t = _mm256_castpd_si256(content);
            return mask{_mm256_test_epi64_mask(t, t)};

            #elif defined(AVEL_AVX)
            return mask{_mm256_cmp_pd(content, _mm256_setzero_pd(), _CMP_NEQ_UQ)};

            #endif
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    static_assert(
        4 * sizeof(double) == sizeof(vec4x64f),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL double extract(vec4x64f v) {
        static_assert(N < vec4x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x64f::width, int>::type dummy_variable = 0;

        auto quarter = _mm_castps_pd(_mm256_extractf128_ps(_mm256_castpd_ps(decay(v)), N / 2));
        return quarter[N % 2];
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x64f insert(vec4x64f v, double x) {
        static_assert(N < vec4x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x64f::width, int>::type dummy_variable = 0;

        auto broadcasted = _mm256_set1_pd(x);
        constexpr auto blend_mask = std::uint8_t(~(1 << N));
        auto ret = _mm256_mask_blend_pd(blend_mask, broadcasted, decay(v));
        return vec4x64f{ret};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec4x64f x) {
        return count(mask4x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec4x64f x) {
        return any(mask4x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec4x64f x) {
        return all(mask4x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec4x64f x) {
        return none(mask4x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f keep(mask4x64f m, vec4x64f v) {
        #if defined(AVEL_AVX512VL)
        return vec4x64f{_mm256_maskz_mov_pd(decay(m), decay(v))};

        #elif defined(AVEL_AVX)
        return vec4x64f{_mm256_and_pd(decay(m), decay(v))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f clear(mask4x64f m, vec4x64f v) {
        #if defined(AVEL_AVX512VL)
        return vec4x64f{_mm256_maskz_mov_pd(decay(!m), decay(v))};

        #elif defined(AVEL_AVX)
        return vec4x64f{_mm256_andnot_pd(decay(m), decay(v))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f blend(mask4x64f m, vec4x64f a, vec4x64f b) {
        #if defined(AVEL_AVX512VL)
        return vec4x64f{_mm256_mask_blend_pd(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_AVX)
        return vec4x64f{_mm256_blendv_pd(decay(b), decay(a), decay(n))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f byteswap(vec4x64f v) {
        return bit_cast<vec4x64f>(byteswap(bit_cast<vec4x64u>(v)));
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f max(vec4x64f a, vec4x64f b) {
        #if defined(AVEL_AVX)
        return vec4x64f{_mm256_max_pd(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f min(vec4x64f a, vec4x64f b) {
        #if defined(AVEL_AVX)
        return vec4x64f{_mm256_min_pd(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x64f, 2> minmax(vec4x64f a, vec4x64f b) {
        #if defined(AVEL_AVX)
        return {
            vec4x64f{_mm256_min_pd(decay(a), decay(b))},
            vec4x64f{_mm256_max_pd(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f clamp(vec4x64f x, vec4x64f lo, vec4x64f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f negate(mask4x64f m, vec4x64f v) {
        #if defined(AVEL_AVX512VL)
        return vec4x64f{_mm256_mask_sub_pd(decay(v), decay(m), _mm256_setzero_pd(), decay(v))};

        #elif defined(AVEL_AVX)
        auto negation_mask = _mm256_and_pd(decay(m), _mm256_castsi256_pd(_mm256_set1_pd(double_sign_bit_mask)));
        return vec4x64f{_mm256_xor_pd(decay(v), negation_mask)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f abs(vec4x64f v) {
        #if defined(AVEL_AVX)
        return vec4x64f{_mm256_andnot_pd(_mm256_set1_pd(double_sign_bit_mask), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f neg_abs(vec4x64f v) {
        #if defined(AVEL_AVX)
        return vec4x64f{_mm256_or_pd(_mm256_set1_pd(double_sign_bit_mask), decay(v))};
        #endif
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f load<vec4x64f>(const double* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        return vec4x64f{_mm256_maskz_loadu_pd(mask, ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f load<vec4x64f, vec4x64f::width>(const double* ptr) {
        return vec4x64f{_mm256_loadu_pd(ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f aligned_load<vec4x64f>(const double* ptr, std::uint32_t n) {
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        return vec4x64f{_mm256_maskz_load_pd(mask, ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f aligned_load<vec4x64f, vec4x64f::width>(const double* ptr) {
        return vec4x64f{_mm256_load_pd(ptr)};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f gather<vec4x64f>(const double* ptr, vec4x64i indices, std::uint32_t n) {
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        return vec4x64f{_mm256_mmask_i64gather_pd(_mm256_setzero_pd(), mask, decay(indices), ptr, sizeof(double))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f gather<vec4x64f>(const double* ptr, vec4x64i indices) {
        return vec4x64f{_mm256_i64gather_pd(ptr, decay(indices), sizeof(double))};
    }



    AVEL_FINL void store(double* ptr, vec4x64f v, std::uint32_t n) {
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        _mm256_mask_storeu_pd(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec4x64f::width>
    AVEL_FINL void store(double* ptr, vec4x64f v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec4x64f::width>(double* ptr, vec4x64f v) {
        _mm256_storeu_pd(ptr, decay(v));
    }



    AVEL_FINL void aligned_store(double* ptr, vec4x64f v, std::uint32_t n) {
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        _mm256_mask_store_pd(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec4x64f::width>
    AVEL_FINL void aligned_store(double* ptr, vec4x64f v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec4x64f::width>(double* ptr, vec4x64f v) {
        #if defined(AVEL_AVX2)
        _mm256_store_pd(ptr, decay(v));
        #endif
    }



    AVEL_FINL void scatter(double* ptr, vec4x64f v, vec4x64i indices, std::uint32_t n) {
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        _mm256_mask_i64scatter_pd(ptr, mask, decay(indices), decay(v), sizeof(double));
    }

    template<std::uint32_t N = vec4x64f::width>
    AVEL_FINL void scatter(double* ptr, vec4x64f v, vec4x64i indices) {
        scatter(ptr, v, indices, N);
    }

    template<>
    AVEL_FINL void scatter<0>(double* ptr, vec4x64f v, vec4x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec4x64f::width>(double* ptr, vec4x64f v, vec4x64i indices) {
        _mm256_i64scatter_pd(ptr, decay(indices), decay(v), sizeof(double));
    }



    [[nodiscard]]
    AVEL_FINL arr4x64f to_array(vec4x64f v) {
        alignas(32) arr4x64f ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // cmath basic operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f fmax(vec4x64f a, vec4x64f b) {
        return blend(avel::isnan(b), a, avel::max(a, b));
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fmin(vec4x64f a, vec4x64f b) {
        return blend(avel::isnan(b), a, avel::min(a, b));
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f sqrt(vec4x64f arg) {
        return vec4x64f{_mm256_sqrt_pd(decay(arg))};
    }

    //=====================================================
    // Nearest integer floating-point operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f ceil(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f floor(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f trunc(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f round(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_CUR_DIRECTION)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f nearbyint(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f rint(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_CUR_DIRECTION)};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f frexp(vec4x64f v, vec4x64i* exp) {
        #if defined(AVEL_AVX512VL)
        auto is_infinity = _mm256_fpclass_pd_mask(decay(v), 0x10 | 0x08);
        auto is_non_zero = _mm256_cmp_pd_mask(decay(v), _mm256_setzero_pd(), _CMP_NEQ_UQ);

        auto exponents = _mm256_getexp_pd(decay(v));
        exponents = _mm256_add_pd(exponents, _mm256_set1_pd(1.0f));
        *exp = _mm256_maskz_cvttpd_epi64(is_non_zero, exponents);

        auto ret = _mm256_getmant_pd(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        // Note: Returns -1 or 1 for -infinity and +infinity respectively

        ret = _mm256_maskz_mov_pd(is_non_zero, ret);
        ret = _mm256_mask_blend_pd(is_infinity, ret, decay(v));
        return vec4x64f{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f ldexp(vec4x64f x, vec4x64i exp) {
        return vec4x64f{_mm256_scalef_pd(decay(x), _mm256_cvtepi64_pd(decay(exp)))};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f scalbn(vec4x64f x, vec4x64i exp) {
        return avel::ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i ilogb(vec4x64f x) {
        #if defined(AVEL_AVX512VL)
        auto exp_fp = _mm256_getexp_pd(decay(x));

        vec4x64f zero_ret{_mm256_castsi256_pd(_mm256_set1_epi64x(FP_ILOGB0))};
        vec4x64f inf_ret {_mm256_castsi256_pd(_mm256_set1_epi64x(INT_MAX))};
        vec4x64f nan_ret {_mm256_castsi256_pd(_mm256_set1_epi64x(FP_ILOGBNAN))};

        auto misc_ret_i = _mm256_cvtpd_epi64(exp_fp);
        misc_ret_i = _mm256_maskz_mov_epi64(_mm256_cmpneq_epi64_mask(misc_ret_i, _mm256_set1_epi64x(0x8000000000000000ll)), misc_ret_i);

        vec4x64i zero_ret_i{_mm256_castpd_si256(_mm256_fixupimm_pd(zero_ret, exp_fp, _mm256_set1_epi64x(0x88808888), 0x00))};
        vec4x64i inf_ret_i {_mm256_castpd_si256(_mm256_fixupimm_pd(inf_ret,  exp_fp, _mm256_set1_epi64x(0x88088888), 0x00))};
        vec4x64i nan_ret_i {_mm256_castpd_si256(_mm256_fixupimm_pd(nan_ret,  exp_fp, _mm256_set1_epi64x(0x88888800), 0x00))};

        return (vec4x64i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f logb(vec4x64f x) {
        #if defined(AVEL_AVX512VL)
        return vec4x64f{_mm256_getexp_pd(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f copysign(vec4x64f mag, vec4x64f sign) {
        #if defined(AVEL_AVX)
        auto mask = _mm256_set1_pd(double_sign_bit_mask);
        auto ret = _mm256_or_pd(_mm256_and_pd(mask, decay(sign)), _mm256_andnot_pd(mask, decay(mag)));
        return vec4x64f{ret};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64i fpclassify(vec4x64f v) {
        const vec4x64i fp_infinite{int(FP_INFINITE)};
        const vec4x64i fp_nan{int(FP_NAN)};
        const vec4x64i fp_normal{int(FP_NORMAL)};
        const vec4x64i fp_subnormal{int(FP_SUBNORMAL)};
        const vec4x64i fp_zero{int(FP_ZERO)};

        #if defined(AVEL_AVX512VL)
        mask4x64i infinite_mask {_mm256_fpclass_pd_mask(decay(v), 0x08 | 0x10)};
        mask4x64i nan_mask      {_mm256_fpclass_pd_mask(decay(v), 0x01 | 0x80)};
        mask4x64i subnormal_mask{_mm256_fpclass_pd_mask(decay(v), 0x20)};
        mask4x64i zero_mask     {_mm256_fpclass_pd_mask(decay(v), 0x02 | 0x04)};
        mask4x64i normal_mask   {!(infinite_mask | nan_mask | subnormal_mask | zero_mask)};

        return
            keep(infinite_mask, fp_infinite) |
            keep(nan_mask, fp_nan) |
            keep(normal_mask, fp_normal) |
            keep(subnormal_mask, fp_subnormal) |
            keep(zero_mask, fp_zero);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isfinite(vec4x64f arg) {
        #if defined(AVEL_AVX512VL)
        return  vec4x64f{_mm256_getexp_pd(decay(arg))} < vec4x64f{1024.0f};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isinf(vec4x64f v) {
        #if defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_fpclass_pd_mask(decay(v), 0x08 | 0x10)};

        #elif defined(AVEL_SSE2)
        return abs(v) == vec4x64f{INFINITY};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isnan(vec4x64f arg) {
        return mask4x64f{_mm256_fpclass_pd_mask(decay(arg), 0x80 | 0x01)};
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isnormal(vec4x64f arg) {
        #if defined(AVEL_AVX512VL)
        return !mask4x64f{_mm256_fpclass_pd_mask(decay(arg), 0xBF)};

        #elif defined(AVEL_AVX2)
        return {}; //TODO: Implement

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f signbit(vec4x64f arg) {
        #if defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_fpclass_pd_mask(decay(arg), 0x40 | 0x04 | 0x10)};

        #elif defined(AVEL_AVX2)
        return {};

        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask4x64f isgreater(vec4x64f x, vec4x64f y) {
        return x > y;
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isgreaterequal(vec4x64f x, vec4x64f y) {
        return x >= y;
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isless(vec4x64f x, vec4x64f y) {
        return x < y;
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f islessequal(vec4x64f x, vec4x64f y) {
        return x <= y;
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f islessgreater(vec4x64f x, vec4x64f y) {
        return x != y;
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isunordered(vec4x64f x, vec4x64f y) {
        #if defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_cmp_pd_mask(decay(x), decay(y), _CMP_UNORD_Q)};

        #elif defined(AVEL_AVX2)
        return mask4x64f{_mm256_cmp_pd(decay(x), decay(y), _CMP_UNORD_Q)};

        #endif
    }

}

#endif //AVEL_VEC4x64F_HPP

