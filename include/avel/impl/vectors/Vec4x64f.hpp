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
            Vector_mask{primitive(content ^ 0x0f)};

            #elif defined(AVEL_AVX2)


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
    class alignas(32) Vector<double, 4> {
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
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            //TODO: Implement
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

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL operator primitive() const {
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

        return {};
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x64f insert(vec4x64f v, double x) {
        static_assert(N < vec4x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x64f::width, int>::type dummy_variable = 0;

        return {};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec4x64f x) {
        return 0;
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec4x64f x) {
        return false;
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec4x64f x) {
        return false;
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec4x64f x) {
        return false;
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f keep(mask4x64f m, vec4x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f clear(mask4x64f m, vec4x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f blend(mask4x64f m, vec4x64f a, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f byteswap(vec4x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f max(vec4x64f a, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f min(vec4x64f a, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x64f, 2> minmax(vec4x64f a, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f clamp(vec4x64f x, vec4x64f lo, vec4x64f hi) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f negate(mask4x64f m, vec4x64f v) {
        return{};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f abs(vec4x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f neg_abs(vec4x64f v) {
        //TODO: Implement
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f load<vec4x64f>(const double* ptr, std::uint32_t n) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f load<vec4x64f, vec4x64f::width>(const double* ptr) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f aligned_load<vec4x64f>(const double* ptr, std::uint32_t n) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f aligned_load<vec4x64f, vec4x64f::width>(const double* ptr) {
        //TODO: Implement
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f gather<vec4x64f>(const double* ptr, vec4x64i indices, std::uint32_t n) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f gather<vec4x64f>(const double* ptr, vec4x64i indices) {
        //TODO: Implement
    }



    template<std::uint32_t N = vec4x64f::width>
    AVEL_FINL void store(double* ptr, vec4x64f v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void store<vec4x64f::width>(double* ptr, vec4x64f v) {
        //TODO: Implement
    }

    AVEL_FINL void store(double* ptr, vec4x64f v, std::uint32_t n) {
        //TODO: Implement
    }



    template<std::uint32_t N = vec4x64f::width>
    AVEL_FINL void aligned_store(double* ptr, vec4x64f v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void aligned_store<vec4x64f::width>(double* ptr, vec4x64f v) {
        //TODO: Implement
    }

    AVEL_FINL void aligned_store(double* ptr, vec4x64f v, std::uint32_t n) {
        //TODO: Implement
    }



    AVEL_FINL void scatter(double* ptr, vec4x64f v, vec4x64i indices, std::uint32_t n) {
        //TODO: Implement
    }

    template<std::uint32_t N = vec4x64f::width>
    AVEL_FINL void scatter(double* ptr, vec4x64f v, vec4x64i indices) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void scatter<0>(double* ptr, vec4x64f v, vec4x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec4x64f::width>(double* ptr, vec4x64f v, vec4x64i indices) {
        //TODO: Implement
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
    AVEL_FINL vec4x64f fabs(vec4x64f v) {
        return abs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fmod(vec4x64f x, vec4x64f y) {
        return x % y;
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f remainder(vec4x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f remquo(vec4x64f a, vec4x64f b, vec4x64i* quo) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fma(vec4x64f m, vec4x64f x, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fmadd(vec4x64f a, vec4x64f x, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fmsubb(vec4x64f a, vec4x64f b, vec4x64f c) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fnmadd(vec4x64f a, vec4x64f b, vec4x64f c) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fnmsub(vec4x64f m, vec4x64f x, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fmax(vec4x64f a, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fmin(vec4x64f a, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fdim(vec4x64f a, vec4x64f b) {
        //TODO: Implement
    }

    //=====================================================
    // Linear interpolation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f lerp(vec4x64f a, vec4x64f b, vec4x64f t);

    //=====================================================
    // Exponential functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f exp(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f exp2(vec4x64f n) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f expm1(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f log(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f log10(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f log2(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f log1p(vec4x64f arg) {
        //TODO: Implement
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f pow(vec4x64f base, vec4x64f exp) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f sqrt(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f cbrt(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f hypot(vec4x64f x, vec4x64f y) {
        //TODO: Implement
    }

    //=====================================================
    // Trigonometric functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<vec4x64f, 2> sincos(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f sin(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f cos(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f tan(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f asin(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f acos(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f atan(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f atan2(vec4x64f x, vec4x64f y) {
        //TODO: Implement
    }

    //=====================================================
    // Nearest integer floating-point operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f ceil(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f floor(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f trunc(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f round(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f nearbyint(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f rint(vec4x64f arg) {
        //TODO: Implement
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f frexp(vec4x64f v, vec4x64i* exp) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f ldexp(vec4x64f x, vec4x64i exp) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f modf(vec4x64f x, vec4x64f* iptr) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f scalbn(vec4x64f x, vec4x64i exp) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i ilogb(vec4x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f logb(vec4x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f nexttoward(vec4x64f from, vec4x64f to) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f nextafter(vec4x64f from, vec4x64f to) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f copysign(vec4x64f mag, vec4x64f sign) {
        #if defined(AVEL_SSE2)
        auto mask = _mm256_set1_pd(double_sign_bit_mask);
        auto ret = _mm256_or_pd(_mm256_and_pd(mask, decay(sign)), _mm256_andnot_pd(mask, decay(mag)));
        return vec4x64f{ret};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64i fpclassify(vec4x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isfinite(vec4x64f arg) {
        #if defined(AVEL_AVX512VL)
        return  vec4x64f{_mm256_getexp_pd(decay(arg))} != vec4x64f{1023.0f};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isinf(vec4x64f arg) {
        return abs(arg) == vec4x64f{INFINITY};
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isnan(vec4x64f arg) {
        return (arg != arg);
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isnormal(vec4x64f arg) {
        #if defined(AVEL_AVX512VL)
        return !mask4x64f{_mm256_fpclass_ps_mask(decay(arg), 0xBF)};

        #elif defined(AVEL_AVX2)


        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f signbit(vec4x64f arg) {
        #if defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_fpclass_pd_mask(decay(arg), 0x40)};

        #elif defined(AVEL_AVX2)
        //TODO: Implement

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
        return isnan(x) || isnan(y);
    }

}

#endif //AVEL_VEC4x64F_HPP

