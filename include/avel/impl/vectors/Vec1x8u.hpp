#ifndef AVEL_VEC1X8U_HPP
#define AVEL_VEC1X8U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x8u = Vector<std::uint8_t, 1>;
    using arr1x8u = std::array<std::uint8_t, 1>;
    using mask1x8u = Vector_mask<std::uint8_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x8u> div(vec1x8u numerator, vec1x8u denominator);
    vec1x8u broadcast_mask(mask1x8u m);
    vec1x8u blend(vec1x8u a, vec1x8u b, mask1x8u m);
    vec1x8u countl_one(vec1x8u x);





    template<>
    class Vector_mask<std::uint8_t, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = std::uint8_t;

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

        AVEL_FINL explicit Vector_mask(primitive p):
            content(p & 0x1) {}

        AVEL_FINL explicit Vector_mask(bool b):
            content(-b) {}

        AVEL_FINL explicit Vector_mask(const arr1xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            content = -arr[0];
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

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask1x8u m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x8u m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x8u m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x8u m) {
        return !decay(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 1> convert<mask1x8u, mask1x8u>(mask1x8u m) {
        return std::array<mask1x8u, 1>{m};
    }





    template<>
    class Vector<std::uint8_t, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint8_t;

        using primitive = std::uint8_t;

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
            content(-decay(m)) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const arr1x8u& arr) {
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
            content = (rhs >= 8) ? 0x00 : (content >> rhs);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            content = (decay(rhs) >= 8) ? 0x00 : (content << decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            content = (decay(rhs) >= 8) ? 0x00 : (content >> decay(rhs));
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            return Vector{static_cast<primitive>(~content)};
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

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, long long rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, long long rhs) {
            lhs >>= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, Vector rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, Vector rhs) {
            lhs >>= rhs;
            return lhs;
        }

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

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x8u broadcast_mask(mask1x8u m) {
        return vec1x8u{decay(m)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u blend(mask1x8u m, vec1x8u a, vec1x8u b) {
        return vec1x8u{blend(decay(m), decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u byteswap(vec1x8u v) {
        return vec1x8u{byteswap(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u max(vec1x8u a, vec1x8u b) {
        return vec1x8u{max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u min(vec1x8u a, vec1x8u b) {
        return vec1x8u{min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 2> minmax(vec1x8u a, vec1x8u b) {
        return {min(a, b), max(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u clamp(vec1x8u x, vec1x8u lo, vec1x8u hi) {
        return vec1x8u{clamp(decay(x), decay(lo), decay(hi))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u midpoint(vec1x8u a, vec1x8u b) {
        return vec1x8u{midpoint(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u average(vec1x8u a, vec1x8u b) {
        return vec1x8u{average(decay(a), decay(b))};
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x8u load<vec1x8u>(const std::uint8_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x8u{*ptr};
        } else {
            return vec1x8u{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x8u load<vec1x8u, vec1x8u::width>(const std::uint8_t* ptr) {
        return vec1x8u{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x8u aligned_load<vec1x8u>(const std::uint8_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x8u{*ptr};
        } else {
            return vec1x8u{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x8u aligned_load<vec1x8u, vec1x8u::width>(const std::uint8_t* ptr) {
        return vec1x8u{*ptr};
    }




    template<std::uint32_t N = vec1x8u::width>
    AVEL_FINL void store(std::uint8_t* ptr, vec1x8u v) {
        *ptr = decay(v);
    }

    template<>
    AVEL_FINL void store<vec1x8u::width>(std::uint8_t* ptr, vec1x8u v) {
        *ptr = decay(v);
    }

    AVEL_FINL void store(std::uint8_t* ptr, vec1x8u v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    template<std::uint32_t N = vec1x8u::width>
    AVEL_FINL void aligned_store(std::uint8_t* ptr, vec1x8u v) {
        *ptr = decay(v);
    }

    template<>
    AVEL_FINL void aligned_store<vec1x8u::width>(std::uint8_t* ptr, vec1x8u v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::uint8_t* ptr, vec1x8u v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint8_t extract(vec1x8u v) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return decay(v);
    }

    template<std::uint32_t N>
    AVEL_FINL vec1x8u insert(vec1x8u v, std::uint8_t x) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return vec1x8u{x};
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    div_type<vec1x8u> div(vec1x8u x, vec1x8u y) {
        div_type<vec1x8u> ret;
        ret.quot = decay(x) / decay(y);
        ret.rem  = decay(x) % decay(y);
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u popcount(vec1x8u v) {
        return vec1x8u{popcount(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u countl_zero(vec1x8u v) {
        return vec1x8u{countl_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u countl_one(vec1x8u v) {
        return vec1x8u{countl_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u countr_zero(vec1x8u v) {
        return vec1x8u{countr_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u countr_one(vec1x8u v) {
        return vec1x8u{countr_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u bit_width(vec1x8u v) {
        return vec1x8u{bit_width(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u bit_floor(vec1x8u v) {
        return vec1x8u{bit_floor(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u bit_ceil(vec1x8u v) {
        return vec1x8u{bit_ceil(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x8u has_single_bit(vec1x8u v) {
        return mask1x8u{has_single_bit(decay(v))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    vec1x8u bit_shift_left(vec1x8u v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        return vec1x8u{decay(v) << S};
    }

    template<>
    [[nodiscard]]
    vec1x8u bit_shift_left<0>(vec1x8u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    vec1x8u bit_shift_left<8>(vec1x8u v) {
        (void)v;
        return vec1x8u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    vec1x8u bit_shift_right(vec1x8u v) {
        static_assert(S <= 8, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 8, int>::type dummy_variable = 0;

        return vec1x8u{decay(v) >> S};
    }

    template<>
    [[nodiscard]]
    vec1x8u bit_shift_right<0>(vec1x8u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    vec1x8u bit_shift_right<8>(vec1x8u v) {
        (void)v;
        return vec1x8u{0x00};
    }




    template<std::uint32_t S, typename std::enable_if<S < 8, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x8u rotl(vec1x8u v) {
        return vec1x8u{rotl(decay(v), S)};
    }

    template<>
    AVEL_FINL vec1x8u rotl<0>(vec1x8u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<8 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x8u rotl(vec1x8u v) {
        return rotl<S % 8>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u rotl(vec1x8u v, long long s) {
        return vec1x8u{rotl(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u rotl(vec1x8u v, vec1x8u s) {
        return vec1x8u{rotl(decay(v), decay(s))};
    }



    template<std::uint32_t S, typename std::enable_if<S < 8, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x8u rotr(vec1x8u v) {
        return vec1x8u{rotr(decay(v), S)};
    }

    template<>
    AVEL_FINL vec1x8u rotr<0>(vec1x8u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<8 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x8u rotr(vec1x8u v) {
        return rotr<S % 8>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u rotr(vec1x8u v, long long s) {
        return vec1x8u{rotr(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u rotr(vec1x8u v, vec1x8u s) {
        return vec1x8u{rotr(decay(v), decay(s))};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL arr1x8u to_array(vec1x8u x) {
        alignas(1) arr1x8u ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x8u>(vec1x8u m) {
        return std::array<vec1x8u, 1>{m};
    }

}

#endif //AVEL_VEC1X8U_HPP
