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

    template<>
    class Vector_mask<double, 1> {
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
    AVEL_FINL std::uint32_t count(mask1x64f m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x64f m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x64f m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x64f m) {
        return !decay(m);
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL bool extract(mask1x64f m) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return decay(m);
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL mask1x64f insert(mask1x64f m, bool b) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return mask1x64f{b};
    }




    template<>
    class Vector<double, 1> {
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

        AVEL_FINL explicit Vector(const arr1x64f& arr) {
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
        1 * sizeof(double) == sizeof(vec1x64f),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL double extract(vec1x64f v) {
        static_assert(N < vec1x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec1x64f::width, int>::type dummy_variable = 0;

        return decay(v);
    }

    template<std::uint32_t N>
    AVEL_FINL vec1x64f insert(vec1x64f v, double x) {
        static_assert(N < vec1x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec1x64f::width, int>::type dummy_variable = 0;

        return vec1x64f{x};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec1x64f x) {
        return bool(decay(x));
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec1x64f x) {
        return bool(decay(x));
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec1x64f x) {
        return bool(decay(x));
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec1x64f x) {
        return !bool(decay(x));
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f keep(mask1x64f m, vec1x64f v) {
        return vec1x64f{avel::keep(decay(m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f clear(mask1x64f m, vec1x64f v) {
        return vec1x64f{avel::clear(decay(m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f blend(mask1x64f m, vec1x64f a, vec1x64f b) {
        return vec1x64f{avel::blend(decay(m), decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f byteswap(vec1x64f v) {
        return bit_cast<vec1x64f>(avel::byteswap(bit_cast<vec1x64u>(v)));
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f max(vec1x64f a, vec1x64f b) {
        return vec1x64f{avel::max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f min(vec1x64f a, vec1x64f b) {
        return vec1x64f{avel::min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x64f, 2> minmax(vec1x64f a, vec1x64f b) {
        return {min(a, b), max(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f clamp(vec1x64f x, vec1x64f lo, vec1x64f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f negate(mask1x64f m, vec1x64f v) {
        return vec1x64f{avel::negate(decay(m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f abs(vec1x64f v) {
        return vec1x64f{avel::abs(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f neg_abs(vec1x64f v) {
        return vec1x64f{avel::neg_abs(decay(v))};
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64f load<vec1x64f>(const double* ptr, std::uint32_t n) {
        if (n) {
            return vec1x64f{*ptr};
        } else {
            return vec1x64f{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64f load<vec1x64f, vec1x64f::width>(const double* ptr) {
        return vec1x64f{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64f aligned_load<vec1x64f>(const double* ptr, std::uint32_t n) {
        if (n) {
            return vec1x64f{*ptr};
        } else {
            return vec1x64f{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64f aligned_load<vec1x64f, vec1x64f::width>(const double* ptr) {
        return vec1x64f{*ptr};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64f gather<vec1x64f>(const double* ptr, vec1x64i indices, std::uint32_t n) {
        if (n) {
            return vec1x64f{ptr[decay(indices)]};
        } else {
            return vec1x64f{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64f gather<vec1x64f>(const double* ptr, vec1x64i indices) {
        return vec1x64f{ptr[decay(indices)]};
    }



    template<std::uint32_t N = vec1x64f::width>
    AVEL_FINL void store(double* ptr, vec1x64f v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void store<vec1x64f::width>(double* ptr, vec1x64f v) {
        *ptr = decay(v);
    }

    AVEL_FINL void store(double* ptr, vec1x64f v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    template<std::uint32_t N = vec1x64f::width>
    AVEL_FINL void aligned_store(double* ptr, vec1x64f v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void aligned_store<vec1x64f::width>(double* ptr, vec1x64f v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(double* ptr, vec1x64f v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    AVEL_FINL void scatter(double* ptr, vec1x64f v, vec1x64i indices, std::uint32_t n) {
        if (n) {
            ptr[decay(indices)] = decay(v);
        }
    }

    template<std::uint32_t N = vec1x64f::width>
    AVEL_FINL void scatter(double* ptr, vec1x64f v, vec1x64i indices) {
        ptr[decay(indices)] = decay(v);
    }

    template<>
    AVEL_FINL void scatter<0>(double* ptr, vec1x64f v, vec1x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec1x64f::width>(double* ptr, vec1x64f v, vec1x64i indices) {
        ptr[decay(indices)] = decay(v);
    }



    [[nodiscard]]
    AVEL_FINL arr1x64f to_array(vec1x64f v) {
        alignas(8) arr1x64f ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Cmath functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f fmax(vec1x64f a, vec1x64f b) {
        return vec1x64f{avel::fmax(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f fmin(vec1x64f a, vec1x64f b) {
        return vec1x64f{avel::fmin(decay(a), decay(b))};
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f sqrt(vec1x64f x) {
        return vec1x64f{avel::sqrt(decay(x))};
    }

    //=====================================================
    // Nearest integer function
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f ceil(vec1x64f x) {
        return vec1x64f{avel::ceil(decay(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f floor(vec1x64f x) {
        return vec1x64f{avel::floor(decay(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f trunc(vec1x64f x) {
        return vec1x64f{avel::trunc(decay(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f round(vec1x64f x) {
        return vec1x64f{avel::round(decay(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f nearbyint(vec1x64f v) {
        return vec1x64f{avel::nearbyint(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f rint(vec1x64f v) {
        return vec1x64f{avel::rint(decay(v))};
    }

    //=====================================================
    // floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64f frexp(vec1x64f v, vec1x64i* exp) {
        std::int64_t e;
        auto ret = avel::frexp(decay(v), &e);
        *exp = e;
        return vec1x64f{ret};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f ldexp(vec1x64f arg, vec1x64i exp) {
        return vec1x64f{avel::ldexp(decay(arg), decay(exp))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f scalbn(vec1x64f arg, vec1x64i exp) {
        return vec1x64f{avel::scalbn(decay(arg), decay(exp))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i ilogb(vec1x64f x) {
        return vec1x64i{avel::ilogb(decay(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f logb(vec1x64f x) {
        return vec1x64f{avel::logb(decay(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64f copysign(vec1x64f mag, vec1x64f sign) {
        return vec1x64f{avel::copysign(decay(mag), decay(sign))};
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64i fpclassify(vec1x64f v) {
        return vec1x64i{avel::fpclassify(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isfinite(vec1x64f v) {
        return mask1x64f{avel::isfinite(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isinf(vec1x64f v) {
        return mask1x64f{avel::isinf(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isnan(vec1x64f v) {
        return mask1x64f{avel::isnan(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isnormal(vec1x64f v) {
        return mask1x64f{avel::isnormal(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f signbit(vec1x64f v) {
        return mask1x64f{avel::signbit(decay(v))};
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask1x64f isgreater(vec1x64f x, vec1x64f y) {
        return mask1x64f{avel::isgreater(decay(x), decay(y))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isgreaterequal(vec1x64f x, vec1x64f y) {
        return mask1x64f{avel::isgreaterequal(decay(x), decay(y))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isless(vec1x64f x, vec1x64f y) {
        return mask1x64f{avel::isless(decay(x), decay(y))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f islessequal(vec1x64f x, vec1x64f y) {
        return mask1x64f{avel::islessequal(decay(x), decay(y))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f islessgreater(vec1x64f x, vec1x64f y) {
        return mask1x64f{avel::islessgreater(decay(x), decay(y))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64f isunordered(vec1x64f x, vec1x64f y) {
        return mask1x64f{avel::isunordered(decay(x), decay(y))};
    }

}

#endif //AVEL_VEC1X64F_HPP
