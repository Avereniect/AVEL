#ifndef AVEL_VEC4X64F_HPP
#define AVEL_VEC4X64F_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x64f = Vector<double, 4>;

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

        AVEL_FINL explicit Vector_mask(bool x):
            content(/*TODO: Implement*/) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 4>& arr) {
            //TODO: Implement
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
            //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            //TODO: Implement
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            //TODO: Implement
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            //TODO: Implement
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
    AVEL_FINL std::uint32_t count(mask4x64f m) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x64f m) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x64f m) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x64f m) {
        //TODO: Implement
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
        using primitive = __m256;
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
            //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            //TODO: Implement
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

        AVEL_FINL Vector& operator+=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
            //TODO: Implement
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

    [[nodiscard]]
    AVEL_FINL vec4x64f set_bits(mask4x64f m) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f blend(vec4x64f a, vec4x64f b, mask4x64f m) {
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
    AVEL_FINL std::pair<vec4x64f, vec4x64f> minmax(vec4x64f a, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f clamp(vec4x64f x, vec4x64f lo, vec4x64f hi) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f midpoint(vec4x64f x, vec4x64f y) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f average(vec4x64f a, vec4x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f abs(vec4x64f v) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f load<vec4x64f>(const double* ptr) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f aligned_load<vec4x64f>(const double* ptr) {
        //TODO: Implement
    }

    AVEL_FINL void store(double* ptr, vec4x64f v) {
        //TODO: Implement
    }

    AVEL_FINL void aligned_store(double* ptr, vec4x64f v) {
        //TODO: Implement
    }

    //=====================================================
    // Floating-point vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f epsilon_increment(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f epsilon_decrement(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f epsilon_offset(vec4x64f arg, vec4x64f offset) {
        //TODO: Implement
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
    AVEL_FINL vec4x64f scalebn(vec4x64f x, vec4x64i exp) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f ilogb(vec4x64f x) {
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
    AVEL_FINL vec4x64f copysign(vec4x64f mag, vec4x64f sgn) {
        //TODO: Implement
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64i fpclassify(vec4x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isfininte(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isinf(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isnan(vec4x64f arg) {
        return (arg != arg);
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isnormal(vec4x64f arg) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f signbit(vec4x64f arg) {
        //TODO: Implement
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

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<double, 4> to_array(vec4x64f x) {
        alignas(32) std::array<double, 4> array{};
        aligned_store(array.data(), x);
        return array;
    }

}

#endif //AVEL_VEC4x64F_HPP

