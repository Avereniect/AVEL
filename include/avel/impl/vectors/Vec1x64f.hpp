#ifndef AVEL_VEC1X64F_HPP
#define AVEL_VEC1X64F_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x64f = Vector<double, 1>;

    using arr1x64f = std::array<double, 1>;

    using mask1x64f = Vector_mask<double, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec1x64f trunc(vec1x64f x);
    AVEL_FINL vec1x64f abs(vec1x64f v);
    AVEL_FINL vec1x64f fmod(vec1x64f a, vec1x64f b);
    AVEL_FINL vec1x64f blend(mask1x64f, vec1x64f, vec1x64f);
    AVEL_FINL vec1x64f round(vec1x64f);
    AVEL_FINL mask1x64f isunordered(vec1x64f, vec1x64f);
    AVEL_FINL mask1x64f signbit(vec1x64f x);
    AVEL_FINL mask1x64f isnan(vec1x64f v);





    template<>
    class Vector_mask<double, 1> : public avel_impl::Vector_mask1xT {
        using base = avel_impl::Vector_mask1xT;
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = base::primitive;

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            base(decay(convert<Vector_mask>(v)[0])) {}

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask1xT rhs) {
            base::operator&=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask1xT rhs) {
            base::operator|=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask1xT rhs) {
            base::operator^=(rhs);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            return Vector_mask{base::operator!()};
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

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask1x64f m) {
        return std::uint32_t(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x64f m) {
        return bool(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x64f m) {
        return bool(decay(m) & 0x1);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x64f m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    /*
    template<>
    [[nodiscard]]
    std::array<mask1x64f, 1> convert<mask1x64f, mask1x64f>(mask1x64f m) {
        return std::array<mask1x64f, 1>{m};
    }

    template<>
    [[nodiscard]]
    std::array<mask1x8u, 1> convert<mask1x8u, mask1x64f>(mask1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x8i, 1> convert<mask1x8i, mask1x64f>(mask1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x16u, 1> convert<mask1x16u, mask1x64f>(mask1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x16i, 1> convert<mask1x16i, mask1x64f>(mask1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x32u, 1> convert<mask1x32u, mask1x64f>(mask1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x32i, 1> convert<mask1x32i, mask1x64f>(mask1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64u, 1> convert<mask1x64u, mask1x64f>(mask1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64i, 1> convert<mask1x64i, mask1x64f>(mask1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x32f, 1> convert<mask1x32f, mask1x64f>(mask1x64f m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64f, 1> convert<mask1x64f, mask1x8u>(mask1x8u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64f, 1> convert<mask1x64f, mask1x8i>(mask1x8i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64f, 1> convert<mask1x64f, mask1x16u>(mask1x16u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64f, 1> convert<mask1x64f, mask1x16i>(mask1x16i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64f, 1> convert<mask1x64f, mask1x32u>(mask1x32u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64f, 1> convert<mask1x64f, mask1x32i>(mask1x32i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64f, 1> convert<mask1x64f, mask1x64u>(mask1x64u m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64f, 1> convert<mask1x64f, mask1x64i>(mask1x64i m) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<mask1x64f, 1> convert<mask1x64f, mask1x32f>(mask1x32f m) {
        //TODO: Implement
    }
    */





    template<>
    class alignas(8) Vector<double, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = double;

        using primitive = double;

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
            content(decay(m) ? 0.0 : 1.0) {}

        //AVEL_FINL explicit Vector(primitive content):
        //    content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(x) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            content(array[0]) {}

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
            content = x;
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            return mask{lhs.content == rhs.content};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            return mask{lhs.content != rhs.content};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            return mask{lhs.content < rhs.content};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            return mask{lhs.content <= rhs.content};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            return mask{lhs.content > rhs.content};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            return mask{lhs.content >= rhs.content};
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
            content += rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content -= rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content *= rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            content /= rhs.content;
            return *this;
        }

        /*
        AVEL_FINL Vector& operator%=(Vector vec) {
            content = fmod(*this, vec);
            return *this; //TODO: Uncomment
        }
        */

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

        /*
        [[nodiscard]]
        AVEL_FINL friend Vector operator%(Vector lhs, Vector rhs) {
            lhs %= rhs; //TODO: Uncomment
            return lhs;
        }
        */

        //=================================================
        // Increment/Decrement operators
        //=================================================

        AVEL_FINL Vector& operator++() {
            *this += Vector{primitive(1)};
            return *this;
        }

        AVEL_FINL Vector operator++(int) {
            auto temp = *this;
            *this += Vector{primitive(1)};
            return temp;
        }

        AVEL_FINL Vector& operator--() {
            *this -= Vector{primitive(1)};
            return *this;
        }

        AVEL_FINL Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{primitive(1)};
            return temp;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector rhs) {
            content = bit_cast<double>(bit_cast<std::uint64_t>(content) & bit_cast<std::uint64_t>(rhs.content));
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            content = bit_cast<double>(bit_cast<std::uint64_t>(content) | bit_cast<std::uint64_t>(rhs.content));
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            content = bit_cast<double>(bit_cast<std::uint64_t>(content) ^ bit_cast<std::uint64_t>(rhs.content));
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            return Vector{bit_cast<double>(~bit_cast<std::uint64_t>(content))};
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

    /*
    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f broadcast_mask(mask1x64f m) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f blend(mask1x64f m, vec1x64f a, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f max(vec1x64f a, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f min(vec1x64f a, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL std::pair<vec1x64f, vec1x64f> minmax(vec1x64f a, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f clamp(vec1x64f x, vec1x64f lo, vec1x64f hi) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f midpoint(vec1x64f x, vec1x64f y) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f average(vec1x64f a, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f abs(vec1x64f v) {
        //TODO: Implement
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64f load<vec1x64f>(const double* ptr) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64f aligned_load<vec1x64f>(const double* ptr) {
        //TODO: Implement
    }

    AVEL_FINL void store(double* ptr, vec1x64f v) {
        //TODO: Implement
    }

    AVEL_FINL void aligned_store(double* ptr, vec1x64f v) {
        //TODO: Implement
    }

    //=====================================================
    // doubleing-point vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f epsilon_increment(vec1x64f v);

    [[nodiscard]]
    AVEL_FINL vec1x64f epsilon_decrement(vec1x64f v);

    [[nodiscard]]
    AVEL_FINL vec1x64f epsilon_offset(vec1x64f v, vec1x64f o);

    //=====================================================
    // cmath basic operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f fabs(vec1x64f v) {
        return abs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fabsf(vec1x64f v) {
        return fabs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fmod(vec1x64f x, vec1x64f y) {
        return x % y;
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fmodf(vec1x64f x, vec1x64f y) {
        return x % y;
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f remainderf(vec1x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f remquof(vec1x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fmadd(vec1x64f m, vec1x64f x, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fmsubb(vec1x64f m, vec1x64f x, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fnmadd(vec1x64f m, vec1x64f x, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fnmsub(vec1x64f m, vec1x64f x, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fma(vec1x64f m, vec1x64f x, vec1x64f b) {
        return fmadd(m, x, b);
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fmaf(vec1x64f m, vec1x64f x, vec1x64f b) {
        return fmadd(m, x, b);
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fmax(vec1x64f a, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fminf(vec1x64f a, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fmin(vec1x64f a, vec1x64f b) {
        return fminf(a, b);
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fdim(vec1x64f a, vec1x64f b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f lerp(vec1x64f a, vec1x64f b, vec1x64f t);

    //=====================================================
    // Exponential functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f exp(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f exp2(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f expm1(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f log(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f log10(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f log2(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f log1p(vec1x64f x) {
        //TODO: Implement
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f pow(vec1x64f base, vec1x64f exp) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f sqrt(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f cbrt(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f hypot(vec1x64f x, vec1x64f y) {
        //TODO: Implement
    }

    //=====================================================
    // Trigonometric functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<vec1x64f, 2> sincos(vec1x64f angle) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f sin(vec1x64f angle) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f cos(vec1x64f angle) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f tan(vec1x64f angle) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f asin(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f acos(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f atan(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f atan2(vec1x64f x, vec1x64f y) {
        //TODO: Implement
    }

    //=====================================================
    // Nearest Integer doubleing-point operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f ceil(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f floor(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f trunc(vec1x64f x) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f round(vec1x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f nearbyint(vec1x64f v) {
        //TODO: Implement
    }

    //=====================================================
    // doubleing-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f frexp(vec1x64f v, vec1x64i* exp);

    [[nodiscard]]
    AVEL_FINL vec1x64f ldexp(vec1x64f x, vec1x64i exp);

    [[nodiscard]]
    AVEL_FINL vec1x64f modf(vec1x64f x, vec1x64f* iptr);

    [[nodiscard]]
    AVEL_FINL vec1x64f scalebn(vec1x64f x, vec1x64i exp);

    [[nodiscard]]
    AVEL_FINL vec1x64f logb(vec1x64f x);

    [[nodiscard]]
    AVEL_FINL vec1x64f nextafter(vec1x64f from, vec1x64f to) {
        //TODO: Implement
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64i fpclassify(vec1x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isnan(vec1x64f v) {
        return (v != v);
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isfininte(vec1x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isnormal(vec1x64f v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isunordered(vec1x64f x, vec1x64f y) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f signbit(vec1x64f x) {
        //TODO: Implement
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask1x64f isgreater(vec1x64f x, vec1x64f y) {
        return x > y;
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isgreaterequal(vec1x64f x, vec1x64f y) {
        return x >= y;
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isless(vec1x64f x, vec1x64f y) {
        return x < y;
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f islessequal(vec1x64f x, vec1x64f y) {
        return x <= y;
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f islessgreater(vec1x64f x, vec1x64f y) {
        return x != y;
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<double, 1> to_array(vec1x64f x) {
        alignas(8) std::array<double, 1> array{};
        aligned_store(array.data(), x);
        return array;
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 1> convert<vec1x64f, vec1x64f>(vec1x64f x) {
        return std::array<vec1x64f, 1>{x};
    }

    template<>
    [[nodiscard]]
    std::array<vec1x8u, 1> convert<vec1x8u, vec1x64f>(vec1x64f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x8i, 1> convert<vec1x8i, vec1x64f>(vec1x64f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x16u, 1> convert<vec1x16u, vec1x64f>(vec1x64f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x16i, 1> convert<vec1x16i, vec1x64f>(vec1x64f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x32u, 1> convert<vec1x32u, vec1x64f>(vec1x64f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x32i, 1> convert<vec1x32i, vec1x64f>(vec1x64f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64u, 1> convert<vec1x64u, vec1x64f>(vec1x64f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64i, 1> convert<vec1x64i, vec1x64f>(vec1x64f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x32f, 1> convert<vec1x32f, vec1x64f>(vec1x64f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 1> convert<vec1x64f, vec1x8u>(vec1x8u x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 1> convert<vec1x64f, vec1x8i>(vec1x8i x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 1> convert<vec1x64f, vec1x16u>(vec1x16u x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 1> convert<vec1x64f, vec1x16i>(vec1x16i x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 1> convert<vec1x64f, vec1x32u>(vec1x32u x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 1> convert<vec1x64f, vec1x32i>(vec1x32i x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 1> convert<vec1x64f, vec1x64u>(vec1x64u x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 1> convert<vec1x64f, vec1x64i>(vec1x64i x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    std::array<vec1x64f, 1> convert<vec1x64f, vec1x32f>(vec1x32f x) {
        //TODO: Implement
    }
    */

}

#endif //AVEL_VEC1x64F_HPP
