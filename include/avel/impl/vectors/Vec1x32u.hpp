#ifndef AVEL_VEC1X32U_HPP
#define AVEL_VEC1X32U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x32u = Vector<std::uint32_t, 1>;
    using arr1x32u = std::array<std::uint32_t, 1>;
    using mask1x32u = Vector_mask<std::uint32_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x32u> div(vec1x32u numerator, vec1x32u denominator);
    vec1x32u set_bits(mask1x32u m);
    vec1x32u blend(mask1x32u m, vec1x32u a, vec1x32u b);
    vec1x32u countl_one(vec1x32u x);





    template<>
    class Vector_mask<std::uint32_t, 1> {
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
    AVEL_FINL std::uint32_t count(mask1x32u m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x32u m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x32u m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x32u m) {
        return !decay(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 1> convert<mask1x8u, mask1x32u>(mask1x32u m) {
        return std::array<mask1x8u, 1>{mask1x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 1> convert<mask1x8i, mask1x32u>(mask1x32u m) {
        return std::array<mask1x8i, 1>{mask1x8i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert<mask1x16u, mask1x32u>(mask1x32u m) {
        return std::array<mask1x16u, 1>{mask1x16u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 1> convert<mask1x16i, mask1x32u>(mask1x32u m) {
        return std::array<mask1x16i, 1>{mask1x16i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x32u>(mask1x32u m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x8u>(mask1x8u m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x8i>(mask1x8i m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x16u>(mask1x16u m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x16i>(mask1x16i m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }





    template<>
    class Vector<std::uint32_t, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint32_t;

        using primitive = std::uint32_t;

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

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const arr1x32u& arr) {
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

        //Definition of operator-() deferred until later

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
            content = (rhs >= 32) ? 0x00 : (content << rhs);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            content = (rhs >= 32) ? 0x00 : (content >> rhs);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            content = (decay(rhs) >= 32) ? 0x00 : (content << decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            content = (decay(rhs) >= 32) ? 0x00 : (content >> decay(rhs));
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
            return Vector{0x00} != *this;
        }

    };

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec1x32u v) {
        return decay(v) != 0;
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t any(vec1x32u v) {
        return decay(v) != 0;
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t all(vec1x32u v) {
        return decay(v) != 0;
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t none(vec1x32u v) {
        return decay(v) == 0;
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u set_bits(mask1x32u m) {
        return vec1x32u{static_cast<vec1x32u::scalar>(-decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u keep(mask1x32u m, vec1x32u v) {
        if (decay(m)) {
            return v;
        } else {
            return vec1x32u{0x0};
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u clear(mask1x32u m, vec1x32u v) {
        if (decay(m)) {
            return vec1x32u{0x0};
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u blend(mask1x32u m, vec1x32u a, vec1x32u b) {
        return vec1x32u{blend(decay(m), decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u byteswap(vec1x32u v) {
        return vec1x32u{byteswap(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u max(vec1x32u a, vec1x32u b) {
        return vec1x32u{max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u min(vec1x32u a, vec1x32u b) {
        return vec1x32u{min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 2> minmax(vec1x32u a, vec1x32u b) {
        return {min(a, b), max(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u clamp(vec1x32u x, vec1x32u lo, vec1x32u hi) {
        return vec1x32u{clamp(decay(x), decay(lo), decay(hi))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u midpoint(vec1x32u a, vec1x32u b) {
        return vec1x32u{midpoint(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u average(vec1x32u a, vec1x32u b) {
        return vec1x32u{average(decay(a), decay(b))};
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u load<vec1x32u>(const std::uint32_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x32u{*ptr};
        } else {
            return vec1x32u{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u load<vec1x32u, vec1x32u::width>(const std::uint32_t* ptr) {
        return vec1x32u{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u aligned_load<vec1x32u>(const std::uint32_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x32u{*ptr};
        } else {
            return vec1x32u{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u aligned_load<vec1x32u, vec1x32u::width>(const std::uint32_t* ptr) {
        return vec1x32u{*ptr};
    }



    template<std::uint32_t N = vec1x32u::width>
    AVEL_FINL void store(std::uint32_t* ptr, vec1x32u v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void store<vec1x32u::width>(std::uint32_t* ptr, vec1x32u v) {
        *ptr = decay(v);
    }

    AVEL_FINL void store(std::uint32_t* ptr, vec1x32u v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    template<std::uint32_t N = vec1x32u::width>
    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec1x32u v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void aligned_store<vec1x32u::width>(std::uint32_t* ptr, vec1x32u v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec1x32u v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }







    [[nodiscard]]
    AVEL_FINL arr1x32u to_array(vec1x32u x) {
        alignas(4) arr1x32u ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint32_t extract(vec1x32u v) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return decay(v);
    }

    template<std::uint32_t N>
    AVEL_FINL vec1x32u insert(vec1x32u v, std::uint32_t x) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return vec1x32u{x};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x32u bit_shift_left(vec1x32u v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        return vec1x32u{decay(v) << S};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u bit_shift_left<0>(vec1x32u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u bit_shift_left<32>(vec1x32u v) {
        (void)v;
        return vec1x32u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x32u bit_shift_right(vec1x32u v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        return vec1x32u{decay(v) >> S};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u bit_shift_right<0>(vec1x32u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u bit_shift_right<32>(vec1x32u v) {
        (void)v;
        return vec1x32u{0x00};
    }




    template<std::uint32_t S, typename std::enable_if<S < 32, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x32u rotl(vec1x32u v) {
        return vec1x32u{rotl(decay(v), S)};
    }

    template<>
    AVEL_FINL vec1x32u rotl<0>(vec1x32u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<32 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x32u rotl(vec1x32u v) {
        return rotl<S % 32>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u rotl(vec1x32u v, long long s) {
        return vec1x32u{rotl(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u rotl(vec1x32u v, vec1x32u s) {
        return vec1x32u{rotl(decay(v), decay(s))};
    }



    template<std::uint32_t S, typename std::enable_if<S < 32, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x32u rotr(vec1x32u v) {
        return vec1x32u{rotr(decay(v), S)};
    }

    template<>
    AVEL_FINL vec1x32u rotr<0>(vec1x32u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<32 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x32u rotr(vec1x32u v) {
        return rotr<S % 32>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u rotr(vec1x32u v, long long s) {
        return vec1x32u{rotr(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u rotr(vec1x32u v, vec1x32u s) {
        return vec1x32u{rotr(decay(v), decay(s))};
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x32u> div(vec1x32u x, vec1x32u y) {
        div_type<vec1x32u> ret;
        ret.quot = decay(x) / decay(y);
        ret.rem  = decay(x) % decay(y);
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u popcount(vec1x32u v) {
        return vec1x32u{popcount(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u countl_zero(vec1x32u v) {
        return vec1x32u{countl_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u countl_one(vec1x32u v) {
        return vec1x32u{countl_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u countr_zero(vec1x32u v) {
        return vec1x32u{countr_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u countr_one(vec1x32u v) {
        return vec1x32u{countr_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u bit_width(vec1x32u v) {
        return vec1x32u{bit_width(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u bit_floor(vec1x32u v) {
        return vec1x32u{bit_floor(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u bit_ceil(vec1x32u v) {
        return vec1x32u{bit_ceil(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32u has_single_bit(vec1x32u v) {
        return mask1x32u{has_single_bit(decay(v))};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x32u>(vec1x32u m) {
        return std::array<vec1x8u, 1>{vec1x8u{static_cast<vec1x8u::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 1> convert<vec1x8i, vec1x32u>(vec1x32u m) {
        return std::array<vec1x8i, 1>{vec1x8i{static_cast<vec1x8i::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x32u>(vec1x32u m) {
        return std::array<vec1x16u, 1>{vec1x16u{static_cast<vec1x16u::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 1> convert<vec1x16i, vec1x32u>(vec1x32u m) {
        return std::array<vec1x16i, 1>{vec1x16i{static_cast<vec1x16i::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x32u>(vec1x32u m) {
        return std::array<vec1x32u, 1>{vec1x32u{static_cast<vec1x32u::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x8u>(vec1x8u m) {
        return std::array<vec1x32u, 1>{vec1x32u{static_cast<vec1x32u::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x8i>(vec1x8i m) {
        return std::array<vec1x32u, 1>{vec1x32u{static_cast<vec1x32u::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x16u>(vec1x16u m) {
        return std::array<vec1x32u, 1>{vec1x32u{static_cast<vec1x32u::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x16i>(vec1x16i m) {
        return std::array<vec1x32u, 1>{vec1x32u{static_cast<vec1x32u::scalar>(decay(m))}};
    }

}

#endif //AVEL_VEC1X32U_HPP
