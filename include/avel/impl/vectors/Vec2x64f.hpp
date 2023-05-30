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

        #if defined(AVEL_SSE2)
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

        AVEL_FINL explicit Vector_mask(bool x):
            #if defined(AVEL_SSE2)
            content(_mm_castsi128_pd(x ? _mm_set1_epi64x(-1ul) : _mm_setzero_si128())) {}
            #endif
            #if defined(AVEL_NEON)
            content() {} //TODO: Implement
            #endif

        AVEL_FINL explicit Vector_mask(const std::array<bool, 2>& arr) {
            #if defined(AVEL_SSE2)
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

        AVEL_FINL Vector_mask& operator=(bool x) {
            //TODO: Implement
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_SSE2)
            return _mm_movemask_pd(_mm_xor_pd(decay(lhs), decay(rhs))) == 0x00;
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_SSE2)
            return _mm_movemask_pd(_mm_xor_pd(decay(lhs), decay(rhs))) != 0x00;
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_and_pd(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_pd(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_pd(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_SSE2)
            auto ones = _mm_castsi128_pd(_mm_set1_epi64x(-1));
            return Vector_mask{_mm_andnot_pd(content, ones)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {
            return lhs & rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {
            return lhs | rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {
            lhs ^= rhs;
            return lhs;
        }

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
        #if defined(AVEL_SSE2)
            #if defined(AVEL_POPCNT)
            return popcount(_mm_movemask_pd(decay(m)));

            #else
            alignas(16) static constexpr std::uint32_t table[4]{
                0, 1, 1, 2
            };

            auto bit_mask = _mm_movemask_pd(decay(m));
            return table[bit_mask];
            #endif

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask2x64f m) {
        #if defined(AVEL_SSE2)
        return _mm_movemask_pd(decay(m)) != 0x0;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask2x64f m) {
        #if defined(AVEL_SSE2)
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

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            content(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
            content(/*TODO: Implement*/) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(/*TODO: Implement*/) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array) {
            //TODO: Implement
        }

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        AVEL_FINL Vector& operator=(scalar x) {
            //TODO: Implement
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_SSE)
            return mask{_mm_cmpeq_pd(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_SSE)
            return mask{_mm_cmpneq_pd(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_SSE)
            return mask{_mm_cmplt_pd(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_SSE)
            return mask{_mm_cmple_pd(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_SSE)
            return mask{_mm_cmpgt_pd(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_SSE)
            return mask{_mm_cmpge_pd(decay(lhs), decay(rhs))};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            return Vector{} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector vec) {
            #if defined(AVEL_SSE)
            content = _mm_add_pd(content, decay(vec));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            #if defined(AVEL_SSE)
            content = _mm_sub_pd(content, decay(vec));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            #if defined(AVEL_SSE)
            content = _mm_mul_pd(content, decay(vec));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            #if defined(AVEL_SSE)
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
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector vec) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            //TODO: Implement
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            //TODO: Implement
            return Vector{};
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
            return *this != Vector{};
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    //=====================================================
    // General vector operations
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL vec2x64f set_bits(mask2x64f m) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f blend(mask2x64f m, vec2x64f a, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f max(vec2x64f a, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f min(vec2x64f a, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL std::pair<vec2x64f, vec2x64f> minmax(vec2x64f a, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f clamp(vec2x64f x, vec2x64f lo, vec2x64f hi) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f midpoint(vec2x64f x, vec2x64f y) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f average(vec2x64f a, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f abs(vec2x64f v) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f load<vec2x64f>(const double* ptr) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec2x64f aligned_load<vec2x64f>(const double* ptr) {
        //TODO: Implement
    }
    */

    AVEL_FINL void store(double* ptr, vec2x64f x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_pd(ptr, decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(double* ptr, vec2x64f x) {
        #if defined(AVEL_SSE2)
        _mm_store_pd(ptr, decay(x));
        #endif
    }

    //=====================================================
    // floating-point vector operations
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL vec2x64f epsilon_increment(vec2x64f v);

    [[nodiscard]]
    AVEL_FINL vec2x64f epsilon_decrement(vec2x64f v);

    [[nodiscard]]
    AVEL_FINL vec2x64f epsilon_offset(vec2x64f v, vec2x64f o);
    */

    //=====================================================
    // cmath basic operations
    //=====================================================

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
    AVEL_FINL vec2x64f fmax(vec2x64f a, vec2x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f fminf(vec2x64f a, vec2x64f b) {
        //TODO: Implement
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

    /*
    [[nodiscard]]
    AVEL_FINL vec2x64f pow(vec2x64f base, vec2x64f exp) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f sqrt(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f cbrt(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f hypot(vec2x64f x, vec2x64f y) {
        //TODO: Implement
    }
    */

    //=====================================================
    // Trigonometric functions
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL std::array<vec2x64f, 2> sincos(vec2x64f angle) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f sin(vec2x64f angle) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f cos(vec2x64f angle) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f tan(vec2x64f angle) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f asin(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f acos(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f atan(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f atan2(vec2x64f x, vec2x64f y) {
        //TODO: Implement
    }
    */

    //=====================================================
    // Nearest Integer floating-point operators
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL vec2x64f ceil(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f floor(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f trunc(vec2x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f round(vec2x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec2x64f nearbyint(vec2x64f v) {
        //TODO: Implement
    }
    */

    //=====================================================
    // floating-point manipulation
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL vec2x64f frexp(vec2x64f v, vec2x64i* exp);

    [[nodiscard]]
    AVEL_FINL vec2x64f ldexp(vec2x64f x, vec2x64i exp);

    [[nodiscard]]
    AVEL_FINL vec2x64f modf(vec2x64f x, vec2x64f* iptr);

    [[nodiscard]]
    AVEL_FINL vec2x64f scalebn(vec2x64f x, vec2x64i exp);

    [[nodiscard]]
    AVEL_FINL vec2x64f logb(vec2x64f x);

    [[nodiscard]]
    AVEL_FINL vec2x64f nextafter(vec2x64f from, vec2x64f to) {
        //TODO: Implement
    }
    */

    //=====================================================
    // Classification subroutines
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL vec2x64i fpclassify(vec2x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isnan(vec2x64f v) {
        return (v != v);
    }

    [[nodiscard]]
    AVEL_FINL mask2x64f isfininte(vec2x64f v) {
        //TODO: Implement
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
    AVEL_FINL mask2x64f signbit(vec2x64f x) {
        //TODO: Implement
    }
    */

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

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<double, 2> to_array(vec2x64f v) {
        alignas(16) std::array<double, 2> array{};
        aligned_store(array.data(), v);
        return array;
    }

    /*
    template<>
    [[nodiscard]]
    std::array<vec1x8u, 2> convert<vec1x8u, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x8i, 2> convert<vec1x8i, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x16u, 2> convert<vec1x16u, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x16i, 2> convert<vec1x16i, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x32u, 2> convert<vec1x32u, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x32i, 2> convert<vec1x32i, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64u, 2> convert<vec1x64u, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64i, 2> convert<vec1x64i, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x32f, 2> convert<vec1x32f, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 2> convert<vec1x64f, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec16x8u, 1> convert<vec16x8u, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec16x8i, 1> convert<vec16x8i, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec8x16u, 1> convert<vec8x16u, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec8x16i, 1> convert<vec8x16i, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec4x32u, 1> convert<vec4x32u, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec4x32i, 1> convert<vec4x32i, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64u, 1> convert<vec2x64u, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64i, 1> convert<vec2x64i, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec4x32f, 1> convert<vec4x32f, vec2x64f>(vec2x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 1> convert<vec2x64f, vec1x8u>(vec1x8u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 1> convert<vec2x64f, vec1x8i>(vec1x8i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 1> convert<vec2x64f, vec1x16u>(vec1x16u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 1> convert<vec2x64f, vec1x16i>(vec1x16i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 1> convert<vec2x64f, vec1x32u>(vec1x32u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 1> convert<vec2x64f, vec1x32i>(vec1x32i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 1> convert<vec2x64f, vec1x64u>(vec1x64u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 1> convert<vec2x64f, vec1x64i>(vec1x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 1> convert<vec2x64f, vec1x32f>(vec1x32f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 1> convert<vec2x64f, vec1x64f>(vec1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 8> convert<vec2x64f, vec16x8u>(vec16x8u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec2x64f, 8> convert<vec2x64f, vec16x8i>(vec16x8i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec2x64f, 4> convert<vec2x64f, vec8x16u>(vec8x16u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec2x64f, 4> convert<vec2x64f, vec8x16i>(vec8x16i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec2x64f, 2> convert<vec2x64f, vec4x32u>(vec4x32u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec2x64f, 2> convert<vec2x64f, vec4x32i>(vec4x32i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec2x64f, 1> convert<vec2x64f, vec2x64u>(vec2x64u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec2x64f, 1> convert<vec2x64f, vec2x64i>(vec2x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec2x64f, 2> convert<vec2x64f, vec4x32f>(vec4x32f m) {
        //TODO: Implement

    }
    */

}

#endif //AVEL_VEC2x64F_HPP
