#ifndef AVEL_VEC1X32F_HPP
#define AVEL_VEC1X32F_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x32f = Vector<float, 1>;
    using arr1x32f = std::array<float, 1>;
    using mask1x32f = Vector_mask<float, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    template<>
    class Vector_mask<float, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = bool;

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
            content(b) {}

        AVEL_FINL explicit Vector_mask(const arr1xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            content = arr[0];
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator=(bool b) {
            content = -b;
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
            return Vector_mask{!content};
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
    AVEL_FINL std::uint32_t count(mask1x32f m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x32f m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x32f m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x32f m) {
        return !decay(m);
    }




    template<>
    class Vector<float, 1> {
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
            content(decay(m) ? 1.0f : 0.0f) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const arr1x32f& arr) {
            content = arr[0];
        }

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

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
            return mask{decay(lhs) == decay(rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            return mask{decay(lhs) != decay(rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            return mask{decay(lhs) < decay(rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            return mask{decay(lhs) <= decay(rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            return mask{decay(lhs) > decay(rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            return mask{decay(lhs) >= decay(rhs)};
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
            return Vector{-content};
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content += decay(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content -= decay(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content *= decay(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            return *this;
            /*
            auto results = div(*this, rhs);
            content = results.quot.content;
            return *this;
            */
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
            /*
            auto results = div(*this, rhs);
            content = results.rem.content;
            */
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
            return Vector{0.0f} != *this;
        }

    };

    static_assert(
        1 * sizeof(float) == sizeof(vec1x32f),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL float extract(vec1x32f v) {
        static_assert(N <= vec1x32f::width, "Specified index does not exist");
        typename std::enable_if<N <= vec1x32f::width, int>::type dummy_variable = 0;

        return decay(v);
    }

    template<std::uint32_t N>
    AVEL_FINL vec1x32f insert(vec1x32f v, float x) {
        static_assert(N < vec1x32f::width, "Specified index does not exist");
        typename std::enable_if<N < vec1x32f::width, int>::type dummy_variable = 0;

        return vec1x32f{x};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec1x32f x) {
        return bool(decay(x));
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec1x32f x) {
        return bool(decay(x));
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec1x32f x) {
        return bool(decay(x));
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec1x32f x) {
        return !bool(decay(x));
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f keep(mask1x32f m, vec1x32f v) {
        return vec1x32f{avel::keep(decay(m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f clear(mask1x32f m, vec1x32f v) {
        return vec1x32f{avel::clear(decay(m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f blend(mask1x32f m, vec1x32f a, vec1x32f b) {
        return vec1x32f{avel::blend(decay(m), decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f byteswap(vec1x32f v) {
        return bit_cast<vec1x32f>(avel::byteswap(bit_cast<vec1x32u>(v)));
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f max(vec1x32f a, vec1x32f b) {
        return vec1x32f{avel::max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f min(vec1x32f a, vec1x32f b) {
        return vec1x32f{avel::min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 2> minmax(vec1x32f a, vec1x32f b) {
        return {min(a, b), max(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f clamp(vec1x32f x, vec1x32f lo, vec1x32f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f negate(mask1x32f m, vec1x32f v) {
        return vec1x32f{avel::negate(decay(m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f abs(vec1x32f v) {
        return vec1x32f{avel::abs(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f neg_abs(vec1x32f v) {
        return vec1x32f{avel::neg_abs(decay(v))};
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32f load<vec1x32f>(const float* ptr, std::uint32_t n) {
        if (n) {
            return vec1x32f{*ptr};
        } else {
            return vec1x32f{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32f load<vec1x32f, vec1x32f::width>(const float* ptr) {
        return vec1x32f{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32f aligned_load<vec1x32f>(const float* ptr, std::uint32_t n) {
        if (n) {
            return vec1x32f{*ptr};
        } else {
            return vec1x32f{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32f aligned_load<vec1x32f, vec1x32f::width>(const float* ptr) {
        return vec1x32f{*ptr};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32f gather<vec1x32f>(const float* ptr, vec1x32i indices, std::uint32_t n) {
        if (n) {
            return vec1x32f{ptr[decay(indices)]};
        } else {
            return vec1x32f{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32f gather<vec1x32f>(const float* ptr, vec1x32i indices) {
        return vec1x32f{ptr[decay(indices)]};
    }



    template<std::uint32_t N = vec1x32f::width>
    AVEL_FINL void store(float* ptr, vec1x32f v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void store<vec1x32f::width>(float* ptr, vec1x32f v) {
        *ptr = decay(v);
    }

    AVEL_FINL void store(float* ptr, vec1x32f v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    template<std::uint32_t N = vec1x32f::width>
    AVEL_FINL void aligned_store(float* ptr, vec1x32f v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void aligned_store<vec1x32f::width>(float* ptr, vec1x32f v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(float* ptr, vec1x32f v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    AVEL_FINL void scatter(float* ptr, vec1x32f v, vec1x32i indices, std::uint32_t n) {
        if (n) {
            ptr[decay(indices)] = decay(v);
        }
    }

    template<std::uint32_t N = vec1x32f::width>
    AVEL_FINL void scatter(float* ptr, vec1x32f v, vec1x32i indices) {
        ptr[decay(indices)] = decay(v);
    }

    template<>
    AVEL_FINL void scatter<0>(float* ptr, vec1x32f v, vec1x32i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec1x32f::width>(float* ptr, vec1x32f v, vec1x32i indices) {
        ptr[decay(indices)] = decay(v);
    }



    [[nodiscard]]
    AVEL_FINL arr1x32f to_array(vec1x32f v) {
        alignas(4) arr1x32f ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Cmath functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x32f fmax(vec1x32f a, vec1x32f b) {
        return vec1x32f{avel::fmax(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f fmin(vec1x32f a, vec1x32f b) {
        return vec1x32f{avel::fmin(decay(a), decay(b))};
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x32f sqrt(vec1x32f x) {
        return vec1x32f{avel::sqrt(decay(x))};
    }

    //=====================================================
    // Nearest integer function
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x32f ceil(vec1x32f v) {
        return vec1x32f{avel::ceil(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f floor(vec1x32f v) {
        return vec1x32f{avel::floor(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f trunc(vec1x32f v) {
        return vec1x32f{avel::trunc(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f round(vec1x32f v) {
        return vec1x32f{avel::round(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f nearbyint(vec1x32f v) {
        return vec1x32f{avel::nearbyint(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f rint(vec1x32f v) {
        return vec1x32f{avel::rint(decay(v))};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x32f frexp(vec1x32f v, vec1x32i* exp) {
        std::int32_t tmp;
        auto ret = avel::frexp(decay(v), &tmp);
        *exp = tmp;
        return vec1x32f{ret};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f ldexp(vec1x32f arg, vec1x32i exp) {
        return vec1x32f{avel::ldexp(decay(arg), decay(exp))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f scalbn(vec1x32f x, vec1x32i exp) {
        return vec1x32f{avel::scalbn(decay(x), decay(exp))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i ilogb(vec1x32f x) {
        return vec1x32i{avel::ilogb(decay(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f logb(vec1x32f x) {
        return vec1x32f{avel::logb(decay(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32f copysign(vec1x32f mag, vec1x32f sign) {
        return vec1x32f{avel::copysign(decay(mag), decay(sign))};
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x32i fpclassify(vec1x32f v) {
        return vec1x32i{avel::fpclassify(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f isfinite(vec1x32f v) {
        return mask1x32f{avel::isfinite(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f isinf(vec1x32f v) {
        return mask1x32f{avel::isinf(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f isnan(vec1x32f v) {
        return mask1x32f{avel::isnan(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f isnormal(vec1x32f v) {
        return mask1x32f{avel::isnormal(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f signbit(vec1x32f arg) {
        return mask1x32f{avel::signbit(decay(arg))};
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask1x32f isgreater(vec1x32f x, vec1x32f y) {
        return mask1x32f{avel::isgreater(decay(x), decay(y))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f isgreaterequal(vec1x32f x, vec1x32f y) {
        return mask1x32f{avel::isgreaterequal(decay(x), decay(y))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f isless(vec1x32f x, vec1x32f y) {
        return mask1x32f{avel::isless(decay(x), decay(y))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f islessequal(vec1x32f x, vec1x32f y) {
        return mask1x32f{avel::islessequal(decay(x), decay(y))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f islessgreater(vec1x32f x, vec1x32f y) {
        return mask1x32f{avel::islessgreater(decay(x), decay(y))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32f isunordered(vec1x32f x, vec1x32f y) {
        return mask1x32f{avel::isunordered(decay(x), decay(y))};
    }

}

#endif //AVEL_VEC1X32F_HPP
