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

            #if defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si64(arr.data());
            content = static_cast<primitive>(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX512F)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            auto expanded = _mm256_cvtepi8_epi32(array_data);
            content = _mm256_cmplt_epu32_mask(_mm256_setzero_si256(), expanded);
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
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_EQ_OS)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_NEQ_OS)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_LT_OS)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_LE_OS)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_GT_OS)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmp_pd_mask(lhs.content, rhs.content, _CMP_GE_OS)};
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
            #if defined(AVEL_AVX512F)
            auto t = _mm512_castpd_si512(content);
            return mask{_mm512_test_epi64_mask(t, t)};
            #endif
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

        return {};
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL vec8x64f insert(vec8x64f v, double x) {
        static_assert(N < vec8x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x64f::width, int>::type dummy_variable = 0;

        return {};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec8x64f x) {
        return 0;
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec8x64f x) {
        return false;
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec8x64f x) {
        return false;
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec8x64f x) {
        return false;
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f keep(mask8x64f m, vec8x64f v) {
        return vec8x64f{};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f clear(mask8x64f m, vec8x64f v) {
        return vec8x64f{};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f blend(mask8x64f m, vec8x64f a, vec8x64f b) {
        return vec8x64f{};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f byteswap(vec8x64f v) {
        return bit_cast<vec8x64f>(byteswap(bit_cast<vec8x64f>(v)));
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f max(vec8x64f a, vec8x64f b) {
        return vec8x64f{};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f min(vec8x64f a, vec8x64f b) {
        return vec8x64f{};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x64f, 2> minmax(vec8x64f a, vec8x64f b) {
        return {};
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
        return -abs(v);
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f load<vec8x64f>(const double* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x64f{_mm512_maskz_loadu_pd(mask, ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f load<vec8x64f, vec8x64f::width>(const double* ptr) {
        #if defined(AVEL_AVX512F)
        return vec8x64f{_mm512_loadu_pd(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f aligned_load<vec8x64f>(const double* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x64f{_mm512_maskz_load_pd(mask, ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f aligned_load<vec8x64f, vec8x64f::width>(const double* ptr) {
        #if defined(AVEL_AVX512F)
        return vec8x64f{_mm512_load_pd(ptr)};
        #endif
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f gather<vec8x64f>(const double* ptr, vec8x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512F)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x64f{_mm512_mask_i64gather_pd(_mm512_setzero_pd(), mask, decay(indices), ptr, sizeof(double))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64f gather<vec8x64f>(const double* ptr, vec8x64i indices) {
        #if defined(AVEL_AVX512F)
        return vec8x64f{_mm512_i64gather_pd(decay(indices), ptr, sizeof(double))};
        #endif
    }



    template<std::uint32_t N = vec8x64f::width>
    AVEL_FINL void store(double* ptr, vec8x64f v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void store<vec8x64f::width>(double* ptr, vec8x64f v) {
        //TODO: Implement
    }

    AVEL_FINL void store(double* ptr, vec8x64f v, std::uint32_t n) {
        //TODO: Implement
    }



    template<std::uint32_t N = vec8x64f::width>
    AVEL_FINL void aligned_store(double* ptr, vec8x64f v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void aligned_store<vec8x64f::width>(double* ptr, vec8x64f v) {
        //TODO: Implement
    }

    AVEL_FINL void aligned_store(double* ptr, vec8x64f v, std::uint32_t n) {
        //TODO: Implement
    }



    AVEL_FINL void scatter(double* ptr, vec8x64f v, vec8x64i indices, std::uint32_t n) {
        //TODO: Implement
    }

    template<std::uint32_t N = vec8x64f::width>
    AVEL_FINL void scatter(double* ptr, vec8x64f v, vec8x64i indices) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void scatter<0>(double* ptr, vec8x64f v, vec8x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec8x64f::width>(double* ptr, vec8x64f v, vec8x64i indices) {
        //TODO: Implement
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
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f fmin(vec8x64f a, vec8x64f b) {
        return {};
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x64f sqrt(vec8x64f x) {
        return {};
    }

    //=====================================================
    // Nearest integer function
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x64f ceil(vec8x64f x) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f floor(vec8x64f x) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f trunc(vec8x64f x) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f round(vec8x64f v) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f nearbyint(vec8x64f v) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f rint(vec8x64f v) {
        return {};
    }

    //=====================================================
    // doubleing-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x64f frexp(vec8x64f v, vec8x64i* exp) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f ldexp(vec8x64f arg, vec8x64i exp) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f scalbn(vec8x64f x, vec8x64i exp) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i ilogb(vec8x64f x) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f logb(vec8x64f x) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64f copysign(vec8x64f mag, vec8x64f sign) {
        #if defined(AVEL_SSE2)
        auto mask = _mm512_set1_pd(double_sign_bit_mask);
        auto ret = _mm512_or_pd(_mm512_and_pd(mask, decay(sign)), _mm512_andnot_pd(mask, decay(mag)));
        return vec8x64f{ret};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x64i fpclassify(vec8x64f v) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isfinite(vec8x64f v) {
        #if defined(AVEL_AVX512F)
        return  vec8x64f{_mm512_getexp_pd(decay(v))} != vec8x64f{1023.0f};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isinf(vec8x64f arg) {
        return abs(arg) == vec8x64f{INFINITY};
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isnan(vec8x64f v) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isnormal(vec8x64f v) {
        return {};
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f signbit(vec8x64f arg) {
        #if defined(AVEL_AVX512VL)
        return mask8x64f{_mm512_fpclass_pd_mask(decay(arg), 0x40)};
        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask8x64f isgreater(vec8x64f x, vec8x64f y) {
        return x > y;
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isgreaterequal(vec8x64f x, vec8x64f y) {
        return x >= y;
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isless(vec8x64f x, vec8x64f y) {
        return x < y;
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f islessequal(vec8x64f x, vec8x64f y) {
        return x <= y;
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f islessgreater(vec8x64f x, vec8x64f y) {
        return x != y;
    }

    [[nodiscard]]
    AVEL_FINL mask8x64f isunordered(vec8x64f x, vec8x64f y) {
        return isnan(x) || isnan(y);
    }

}

#endif //AVEL_VEC8X64F_HPP
