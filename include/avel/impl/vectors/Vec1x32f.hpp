#ifndef AVEL_VEC1X32F_HPP
#define AVEL_VEC1X32F_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x32f = Vector<float, 1>;

    using mask1x32f = Vector_mask<float, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec1x32f trunc(vec1x32f x);
    AVEL_FINL vec1x32f abs(vec1x32f v);
    AVEL_FINL vec1x32f fmod(vec1x32f a, vec1x32f b);
    AVEL_FINL vec1x32f blend(vec1x32f, vec1x32f, mask1x32f);
    AVEL_FINL vec1x32f round(vec1x32f);
    AVEL_FINL mask1x32f isunordered(vec1x32f, vec1x32f);
    AVEL_FINL mask1x32f signbit(vec1x32f x);
    AVEL_FINL mask1x32f isnan(vec1x32f v);

    template<>
    class Vector_mask<float, 1> : public avel_impl::Vector_mask1xT {
        using base = Vector_mask1xT;
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = base::primitive;

        //=================================================
        // Constructors
        //=================================================

        using base::base;

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask1x32f m) {
        return std::uint32_t(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x32f m) {
        return bool(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x32f m) {
        return bool(decay(m) & 0x1);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x32f m) {
        return !all(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 1> convert<mask1x32f, mask1x32f>(mask1x32f m) {
        return std::array<mask1x32f, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 1> convert<mask1x8u, mask1x32f>(mask1x32f m) {
        return std::array<mask1x8u, 1>{mask1x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 1> convert<mask1x8i, mask1x32f>(mask1x32f m) {
        return std::array<mask1x8i, 1>{mask1x8i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert<mask1x16u, mask1x32f>(mask1x32f m) {
        return std::array<mask1x16u, 1>{mask1x16u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 1> convert<mask1x16i, mask1x32f>(mask1x32f m) {
        return std::array<mask1x16i, 1>{mask1x16i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x32f>(mask1x32f m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 1> convert<mask1x32i, mask1x32f>(mask1x32f m) {
        return std::array<mask1x32i, 1>{mask1x32i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64u, 1> convert<mask1x64u, mask1x32f>(mask1x32f m) {
        return std::array<mask1x64u, 1>{mask1x64u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 1> convert<mask1x64i, mask1x32f>(mask1x32f m) {
        return std::array<mask1x64i, 1>{mask1x64i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 1> convert<mask1x32f, mask1x8u>(mask1x8u m) {
        return std::array<mask1x32f, 1>{mask1x32f{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 1> convert<mask1x32f, mask1x8i>(mask1x8i m) {
        return std::array<mask1x32f, 1>{mask1x32f{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 1> convert<mask1x32f, mask1x16u>(mask1x16u m) {
        return std::array<mask1x32f, 1>{mask1x32f{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 1> convert<mask1x32f, mask1x16i>(mask1x16i m) {
        return std::array<mask1x32f, 1>{mask1x32f{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 1> convert<mask1x32f, mask1x32u>(mask1x32u m) {
        return std::array<mask1x32f, 1>{mask1x32f{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 1> convert<mask1x32f, mask1x32i>(mask1x32i m) {
        return std::array<mask1x32f, 1>{mask1x32f{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 1> convert<mask1x32f, mask1x64u>(mask1x64u m) {
        return std::array<mask1x32f, 1>{mask1x32f{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 1> convert<mask1x32f, mask1x64i>(mask1x64i m) {
        return std::array<mask1x32f, 1>{mask1x32f{decay(m)}};
    }







    template<>
    class alignas(4) Vector<float, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = float;

        using primitive = float;

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
            content(decay(m) ? 1.0f : 0.0f) {}

        //AVEL_FINL explicit Vector(primitive content):
        //    content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(x) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array) {
            content = array[0];
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
        AVEL_FINL Vector& operator%=(Vector rhs) {
            content = fmod(*this, rhs);
            return *this;
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
            lhs %= rhs;
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
            content = bit_cast<primitive>(bit_cast<std::uint32_t>(content) & bit_cast<std::uint32_t>(rhs.content));
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            content = bit_cast<primitive>(bit_cast<std::uint32_t>(content) | bit_cast<std::uint32_t>(rhs.content));
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            content = bit_cast<primitive>(bit_cast<std::uint32_t>(content) ^ bit_cast<std::uint32_t>(rhs.content));
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            return Vector{bit_cast<primitive>(~bit_cast<std::uint32_t>(content))};
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
    AVEL_FINL vec1x32f broadcast_bits(mask1x32f m) {
        return vec1x32f{decay(m) ? bit_cast<float>(0xFFFFFFFF) : bit_cast<float>(0x00000000)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f blend(vec1x32f a, vec1x32f b, mask1x32f m) {
        return decay(m) ? b : a;
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f max(vec1x32f a, vec1x32f b) {
        return vec1x32f{max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f min(vec1x32f a, vec1x32f b) {
        return vec1x32f{min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 2> minmax(vec1x32f a, vec1x32f b) {
        auto tmp = minmax(decay(a), decay(b));
        return std::array<vec1x32f, 2>{
            vec1x32f{tmp[0]},
            vec1x32f{tmp[1]}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f clamp(vec1x32f x, vec1x32f lo, vec1x32f hi) {
        return vec1x32f{clamp(decay(x), decay(lo), decay(hi))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f midpoint(vec1x32f a, vec1x32f b) {
        return vec1x32f{midpoint(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f average(vec1x32f a, vec1x32f b) {
        return vec1x32f{average(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f abs(vec1x32f v) {
        return vec1x32f{abs(decay(v))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32f load<vec1x32f>(const float* ptr) {
        return vec1x32f{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32f aligned_load<vec1x32f>(const float* ptr) {
        return vec1x32f{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32f stream_load<vec1x32f>(const float* ptr) {
        return vec1x32f{*ptr};
    }

    AVEL_FINL void store(float* ptr, vec1x32f v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(float* ptr, vec1x32f v) {
        *ptr = decay(v);
    }

    AVEL_FINL void stream_store(float* ptr, vec1x32f v) {
        *ptr = decay(v);
    }

    //=====================================================
    // floating-point vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x32f epsilon_increment(vec1x32f v);

    [[nodiscard]]
    AVEL_FINL vec1x32f epsilon_decrement(vec1x32f v);

    [[nodiscard]]
    AVEL_FINL vec1x32f epsilon_offset(vec1x32f v, vec1x32f o);

    //=====================================================
    // cmath basic operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x32f fabs(vec1x32f v) {
        return abs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f fmod(vec1x32f a, vec1x32f b) {
        return a % b;
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f remainder(vec1x32f a, vec1x32f b) {
        return vec1x32f{remainder(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f remquo(vec1x32f a, vec1x32f b, vec1x32u* quo) {
        return vec1x32f{remquo(decay(a), decay(b), reinterpret_cast<std::int32_t*>(quo))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f fma(vec1x32f a, vec1x32f b, vec1x32f c) {
        return vec1x32f{fma(decay(a), decay(b), decay(c))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f fmadd(vec1x32f a, vec1x32f b, vec1x32f c) {
        return vec1x32f{fmadd(decay(a), decay(b), decay(c))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f fmsub(vec1x32f a, vec1x32f b, vec1x32f c) {
        return vec1x32f{fmsub(decay(a), decay(b), decay(c))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f fnmadd(vec1x32f a, vec1x32f b, vec1x32f c) {
        return vec1x32f{fnmadd(decay(a), decay(b), decay(c))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f fnmsub(vec1x32f a, vec1x32f b, vec1x32f c) {
        return vec1x32f{fnmsub(decay(a), decay(b), decay(c))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f fmax(vec1x32f a, vec1x32f b) {
        return vec1x32f{fmax(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f fmin(vec1x32f a, vec1x32f b) {
        return vec1x32f{fmin(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f fdim(vec1x32f a, vec1x32f b) {
        return vec1x32f{fdim(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f lerp(vec1x32f a, vec1x32f b, vec1x32f t) {
        return vec1x32f{fnmsub(decay(a), decay(b), decay(t))};
    }
    */

    //=====================================================
    // Exponential functions
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f exp(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f exp2(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f expm1(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f log(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f log10(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f log2(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f log1p(vec1x32f x) {
        //TODO: Implement
    }
    */

    //=====================================================
    // Power functions
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f pow(vec1x32f base, vec1x32f exp) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f sqrt(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f cbrt(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f hypot(vec1x32f x, vec1x32f y) {
        //TODO: Implement
    }
    */

    //=====================================================
    // Trigonometric functions
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 2> sincos(vec1x32f angle) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f sin(vec1x32f angle) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f cos(vec1x32f angle) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f tan(vec1x32f angle) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f asin(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f acos(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f atan(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f atan2(vec1x32f x, vec1x32f y) {
        //TODO: Implement
    }
    */

    //=====================================================
    // Nearest Integer floating-point operators
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f ceil(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f floor(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f trunc(vec1x32f x) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f round(vec1x32f v) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f nearbyint(vec1x32f v) {
        //TODO: Implement
    }
    */

    //=====================================================
    // floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x32f frexp(vec1x32f v, vec1x32i* exp);

    [[nodiscard]]
    AVEL_FINL vec1x32f ldexp(vec1x32f x, vec1x32i exp);

    [[nodiscard]]
    AVEL_FINL vec1x32f modf(vec1x32f x, vec1x32f* iptr);

    [[nodiscard]]
    AVEL_FINL vec1x32f scalebn(vec1x32f x, vec1x32i exp);

    [[nodiscard]]
    AVEL_FINL vec1x32f logb(vec1x32f x);

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32f nextafter(vec1x32f from, vec1x32f to) {
        //TODO: Implement
    }
    */

    //=====================================================
    // Classification subroutines
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL vec1x32i fpclassify(vec1x32f v) {
        return vec1x32i{fpclassify(decay(v))};
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL mask1x32f isnan(vec1x32f v) {
        return (v != v);
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL mask1x32f isfininte(vec1x32f v) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL mask1x32f isnormal(vec1x32f v) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL mask1x32f isunordered(vec1x32f x, vec1x32f y) {
        //TODO: Implement
    }
    */

    /*
    [[nodiscard]]
    AVEL_FINL mask1x32f signbit(vec1x32f x) {
        //TODO: Implement
    }
    */

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask1x32f isgreater(vec1x32f x, vec1x32f y) {
        return x > y;
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f isgreaterequal(vec1x32f x, vec1x32f y) {
        return x >= y;
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f isless(vec1x32f x, vec1x32f y) {
        return x < y;
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f islessequal(vec1x32f x, vec1x32f y) {
        return x <= y;
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f islessgreater(vec1x32f x, vec1x32f y) {
        return x != y;
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL std::array<float, 1> to_array(vec1x32f x) {
        alignas(4) std::array<float, 1> array{};
        aligned_store(array.data(), x);
        return array;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 1> convert<vec1x32f, vec1x32f>(vec1x32f x) {
        return std::array<vec1x32f, 1>{x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x32f>(vec1x32f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 1> convert<vec1x8i, vec1x32f>(vec1x32f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x32f>(vec1x32f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 1> convert<vec1x16i, vec1x32f>(vec1x32f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x32f>(vec1x32f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 1> convert<vec1x32i, vec1x32f>(vec1x32f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64u, 1> convert<vec1x64u, vec1x32f>(vec1x32f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 1> convert<vec1x64i, vec1x32f>(vec1x32f x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 1> convert<vec1x32f, vec1x8u>(vec1x8u x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 1> convert<vec1x32f, vec1x8i>(vec1x8i x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 1> convert<vec1x32f, vec1x16u>(vec1x16u x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 1> convert<vec1x32f, vec1x16i>(vec1x16i x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 1> convert<vec1x32f, vec1x32u>(vec1x32u x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 1> convert<vec1x32f, vec1x32i>(vec1x32i x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 1> convert<vec1x32f, vec1x64u>(vec1x64u x) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 1> convert<vec1x32f, vec1x64i>(vec1x64i x) {
        //TODO: Implement
    }
    */

}

#endif //AVEL_VEC1x32F_HPP
