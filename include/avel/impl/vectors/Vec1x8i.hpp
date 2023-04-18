#ifndef AVEL_VEC1X8I_HPP
#define AVEL_VEC1X8I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x8i = Vector<std::int8_t, 1>;
    using arr1x8i = std::array<std::int8_t, 1>;
    using mask1x8i = Vector_mask<std::int8_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x8i> div(vec1x8i numerator, vec1x8i denominator);
    vec1x8i set_bits(mask1x8i m);
    vec1x8i blend(mask1x8i m, vec1x8i a, vec1x8i b);
    vec1x8i negate(mask1x8i m, vec1x8i x);





    template<>
    class Vector_mask<std::int8_t, 1> {
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
                "Implementation assumes bool occupy a single byte"
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
            return Vector_mask{static_cast<primitive>(content ^ 0x1)};
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
    AVEL_FINL std::uint32_t count(mask1x8i m) {
        return count(mask1x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x8i m) {
        return any(mask1x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x8i m) {
        return all(mask1x8u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x8i m) {
        return none(mask1x8u{m});
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 1> convert<mask1x8u, mask1x8i>(mask1x8i m) {
        return std::array<mask1x8u, 1>{mask1x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 1> convert<mask1x8i, mask1x8u>(mask1x8u m) {
        return std::array<mask1x8i, 1>{mask1x8i{decay(m)}};
    }





    template<>
    class Vector<std::int8_t, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int8_t;

        using primitive = std::int8_t;

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
            content(decay(m) ? 1 : 0) {}

        AVEL_FINL Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const arr1x8i& arr) {
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
            return Vector{0x00} - *this;
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
            auto results = div(*this, rhs);
            content = results.quot.content;
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.rem.content;
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
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector rhs) {
            content &= decay(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            content |= decay(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            content ^= decay(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            content = (rhs >= 8) ? 0x00 : (content << rhs);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            content = (rhs >= 8) ? (content >> (8 - 1)) : (content >> rhs);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            content = (decay(rhs) >= 8) ? 0x00 : (content << decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            content = (decay(rhs) >= 8) ? (content >> (8 - 1)) : (content >> decay(rhs));
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            return Vector{static_cast<primitive>(~content)};
        }

        AVEL_VECTOR_BINARY_BITWISE_OPERATORS

        //=================================================
        // Conversion operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return mask{static_cast<mask::primitive>(bool(content))};
        }

    };

    static_assert(
        1 * sizeof(std::int8_t) == sizeof(vec1x8i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x8i>(vec1x8i m) {
        return std::array<vec1x8u, 1>{vec1x8u{static_cast<vec1x8u::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 1> convert<vec1x8i, vec1x8u>(vec1x8u m) {
        return std::array<vec1x8i, 1>{vec1x8i{static_cast<vec1x8i::scalar>(decay(m))}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec1x8i operator-(vec1x8u v) {
        return -vec1x8i{v};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec1x8i v) {
        return count(vec1x8u{v});
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t any(vec1x8i v) {
        return any(vec1x8u{v});
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t all(vec1x8i v) {
        return all(vec1x8u{v});
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t none(vec1x8i v) {
        return none(vec1x8u{v});
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i set_bits(mask1x8i m) {
        return vec1x8i{static_cast<vec1x8i::scalar>(-decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i keep(mask1x8i m, vec1x8i v) {
        return vec1x8i{keep(mask1x8u{m}, vec1x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i clear(mask1x8i m, vec1x8i v) {
        return vec1x8i{clear(mask1x8u{m}, vec1x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i blend(mask1x8i m, vec1x8i a, vec1x8i b) {
        return vec1x8i{blend(decay(m), decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i byteswap(vec1x8i v) {
        return vec1x8i{byteswap(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i max(vec1x8i a, vec1x8i b) {
        return vec1x8i{max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i min(vec1x8i a, vec1x8i b) {
        return vec1x8i{min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 2> minmax(vec1x8i a, vec1x8i b) {
        return {min(a, b), max(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i clamp(vec1x8i x, vec1x8i lo, vec1x8i hi) {
        return vec1x8i{clamp(decay(x), decay(lo), decay(hi))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i midpoint(vec1x8i a, vec1x8i b) {
        return vec1x8i{midpoint(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i average(vec1x8i a, vec1x8i b) {
        return vec1x8i{average(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i negate(mask1x8i m, vec1x8i v) {
        if (decay(m)) {
            return -v;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i abs(vec1x8i v) {
        if (decay(v) < 0) {
            return -v;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i neg_abs(vec1x8i v) {
        if (decay(v) < 0) {
            return v;
        } else {
            return -v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i neg_abs(vec1x8u v) {
        return neg_abs(vec1x8i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x8i load<vec1x8i>(const std::int8_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x8i{*ptr};
        } else {
            return vec1x8i{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x8i load<vec1x8i, vec1x8i::width>(const std::int8_t* ptr) {
        return vec1x8i{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x8i aligned_load<vec1x8i>(const std::int8_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x8i{*ptr};
        } else {
            return vec1x8i{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x8i aligned_load<vec1x8i, vec1x8i::width>(const std::int8_t* ptr) {
        return vec1x8i{*ptr};
    }



    AVEL_FINL void store(std::int8_t* ptr, vec1x8i v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }

    template<std::uint32_t N = vec1x8i::width>
    AVEL_FINL void store(std::int8_t* ptr, vec1x8i v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void store<vec1x8i::width>(std::int8_t* ptr, vec1x8i v) {
        *ptr = decay(v);
    }



    AVEL_FINL void aligned_store(std::int8_t* ptr, vec1x8i v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }

    template<std::uint32_t N = vec1x8i::width>
    AVEL_FINL void aligned_store(std::int8_t* ptr, vec1x8i v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void aligned_store<vec1x8i::width>(std::int8_t* ptr, vec1x8i v) {
        *ptr = decay(v);
    }



    [[nodiscard]]
    AVEL_FINL arr1x8i to_array(vec1x8i v) {
        alignas(1) arr1x8i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int8_t extract(vec1x8i v) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return decay(v);
    }

    template<std::uint32_t N>
    AVEL_FINL vec1x8i insert(vec1x8i v, std::int8_t x) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return vec1x8i{x};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    vec1x8i bit_shift_left(vec1x8i v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        return vec1x8i{bit_shift_left<S>(vec1x8u{v})};
    }

    template<std::uint32_t S>
    [[nodiscard]]
    vec1x8i bit_shift_right(vec1x8i v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        return v >> S;
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x8i rotl(vec1x8i v) {
        return vec1x8i{rotl<S>(vec1x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i rotl(vec1x8i v, long long s) {
        return vec1x8i{rotl(vec1x8u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i rotl(vec1x8i v, vec1x8i s) {
        return vec1x8i{rotl(vec1x8u{v}, vec1x8u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x8i rotr(vec1x8i v) {
        return vec1x8i{rotr<S>(vec1x8u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i rotr(vec1x8i v, long long s) {
        return vec1x8i{rotr(vec1x8u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8i rotr(vec1x8i v, vec1x8i s) {
        return vec1x8i{rotr(vec1x8u{v}, vec1x8u{s})};
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x8i> div(vec1x8i x, vec1x8i y) {
        div_type<vec1x8i> ret;
        ret.quot = decay(x) / decay(y);
        ret.rem  = decay(x) % decay(y);
        return ret;
    }

    AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(vec1x8i, mask1x8i, vec1x8u)

}

#endif //AVEL_VEC1X8I_HPP
