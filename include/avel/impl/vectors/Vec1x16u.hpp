#ifndef AVEL_VEC1X16U_HPP
#define AVEL_VEC1X16U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x16u = Vector<std::uint16_t, 1>;
    using arr1x16u = std::array<std::uint16_t, 1>;
    using mask1x16u = Vector_mask<std::uint16_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x16u> div(vec1x16u numerator, vec1x16u denominator);
    vec1x16u broadcast_mask(mask1x16u m);
    vec1x16u blend(mask1x16u m, vec1x16u a, vec1x16u b);
    vec1x16u countl_one(vec1x16u x);





    template<>
    class Vector_mask<std::uint16_t, 1> {
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
    AVEL_FINL std::uint32_t count(mask1x16u m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x16u m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x16u m) {
        return decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x16u m) {
        return !decay(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 1> convert<mask1x8u, mask1x16u>(mask1x16u m) {
        return std::array<mask1x8u, 1>{mask1x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 1> convert<mask1x8i, mask1x16u>(mask1x16u m) {
        return std::array<mask1x8i, 1>{mask1x8i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert<mask1x16u, mask1x16u>(mask1x16u m) {
        return std::array<mask1x16u, 1>{mask1x16u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert<mask1x16u, mask1x8u>(mask1x8u m) {
        return std::array<mask1x16u, 1>{mask1x16u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert<mask1x16u, mask1x8i>(mask1x8i m) {
        return std::array<mask1x16u, 1>{mask1x16u{decay(m)}};
    }





    template<>
    class Vector<std::uint16_t, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint16_t;

        using primitive = std::uint16_t;

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

        AVEL_FINL explicit Vector(const arr1x16u& arr) {
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
            content = (rhs >= 16) ? 0x00 : (content << rhs);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            content = (rhs >= 16) ? 0x00 : (content >> rhs);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            content = (decay(rhs) >= 16) ? 0x00 : (content << decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            content = (decay(rhs) >= 16) ? 0x00 : (content >> decay(rhs));
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
            return Vector{0x00} != *this;
        }

    };

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec1x16u v) {
        return decay(v) != 0;
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t any(vec1x16u v) {
        return decay(v) != 0;
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t all(vec1x16u v) {
        return decay(v) != 0;
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t none(vec1x16u v) {
        return decay(v) == 0;
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u broadcast_mask(mask1x16u m) {
        return vec1x16u(decay(m) ? -1 : 0);
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u keep(mask1x16u m, vec1x16u v) {
        if (decay(m)) {
            return v;
        } else {
            return vec1x16u{0x0};
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u clear(mask1x16u m, vec1x16u v) {
        if (decay(m)) {
            return vec1x16u{0x0};
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u blend(mask1x16u m, vec1x16u a, vec1x16u b) {
        return vec1x16u{blend(decay(m), decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u byteswap(vec1x16u v) {
        return vec1x16u{byteswap(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u max(vec1x16u a, vec1x16u b) {
        return vec1x16u{max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u min(vec1x16u a, vec1x16u b) {
        return vec1x16u{min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 2> minmax(vec1x16u a, vec1x16u b) {
        return {min(a, b), max(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u clamp(vec1x16u x, vec1x16u lo, vec1x16u hi) {
        return vec1x16u{clamp(decay(x), decay(lo), decay(hi))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u midpoint(vec1x16u a, vec1x16u b) {
        return vec1x16u{midpoint(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u average(vec1x16u a, vec1x16u b) {
        return vec1x16u{average(decay(a), decay(b))};
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16u load<vec1x16u>(const std::uint16_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x16u{*ptr};
        } else {
            return vec1x16u{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16u load<vec1x16u, vec1x16u::width>(const std::uint16_t* ptr) {
        return vec1x16u{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16u aligned_load<vec1x16u>(const std::uint16_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x16u{*ptr};
        } else {
            return vec1x16u{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16u aligned_load<vec1x16u, vec1x16u::width>(const std::uint16_t* ptr) {
        return vec1x16u{*ptr};
    }



    template<std::uint32_t N = vec1x16u::width>
    AVEL_FINL void store(std::uint16_t* ptr, vec1x16u v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void store<vec1x16u::width>(std::uint16_t* ptr, vec1x16u v) {
        *ptr = decay(v);
    }

    AVEL_FINL void store(std::uint16_t* ptr, vec1x16u v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    template<std::uint32_t N = vec1x16u::width>
    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec1x16u v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void aligned_store<vec1x16u::width>(std::uint16_t* ptr, vec1x16u v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec1x16u v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    [[nodiscard]]
    AVEL_FINL arr1x16u to_array(vec1x16u x) {
        alignas(2) arr1x16u ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint16_t extract(vec1x16u v) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return decay(v);
    }

    template<std::uint32_t N>
    AVEL_FINL vec1x16u insert(vec1x16u v, std::uint16_t x) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return vec1x16u{x};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    vec1x16u bit_shift_left(vec1x16u v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        return v << S;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16u bit_shift_left<0>(vec1x16u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16u bit_shift_left<16>(vec1x16u v) {
        (void)v;
        return vec1x16u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x16u bit_shift_right(vec1x16u v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        return v >> S;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16u bit_shift_right<0>(vec1x16u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16u bit_shift_right<16>(vec1x16u v) {
        (void)v;
        return vec1x16u{0x00};
    }




    template<std::uint32_t S, typename std::enable_if<S < 16, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x16u rotl(vec1x16u v) {
        return vec1x16u{rotl(decay(v), S)};
    }

    template<>
    AVEL_FINL vec1x16u rotl<0>(vec1x16u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<16 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x16u rotl(vec1x16u v) {
        return rotl<S % 16>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u rotl(vec1x16u v, long long s) {
        return vec1x16u{rotl(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u rotl(vec1x16u v, vec1x16u s) {
        return vec1x16u{rotl(decay(v), decay(s))};
    }



    template<std::uint32_t S, typename std::enable_if<S < 16, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x16u rotr(vec1x16u v) {
        return vec1x16u{rotr(decay(v), S)};
    }

    template<>
    AVEL_FINL vec1x16u rotr<0>(vec1x16u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<16 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec1x16u rotr(vec1x16u v) {
        return rotr<S % 16>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u rotr(vec1x16u v, long long s) {
        return vec1x16u{rotr(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u rotr(vec1x16u v, vec1x16u s) {
        return vec1x16u{rotr(decay(v), decay(s))};
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x16u> div(vec1x16u x, vec1x16u y) {
        div_type<vec1x16u> ret;
        ret.quot = decay(x) / decay(y);
        ret.rem  = decay(x) % decay(y);
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u popcount(vec1x16u v) {
        return vec1x16u{popcount(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u countl_zero(vec1x16u v) {
        return vec1x16u{countl_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u countl_one(vec1x16u v) {
        return vec1x16u{countl_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u countr_zero(vec1x16u v) {
        return vec1x16u{countr_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u countr_one(vec1x16u v) {
        return vec1x16u{countr_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u bit_width(vec1x16u v) {
        return vec1x16u{bit_width(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u bit_floor(vec1x16u v) {
        return vec1x16u{bit_floor(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u bit_ceil(vec1x16u v) {
        return vec1x16u{bit_ceil(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x16u has_single_bit(vec1x16u v) {
        return mask1x16u{has_single_bit(decay(v))};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x16u>(vec1x16u m) {
        return std::array<vec1x8u, 1>{vec1x8u{static_cast<vec1x8u::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 1> convert<vec1x8i, vec1x16u>(vec1x16u m) {
        return std::array<vec1x8i, 1>{vec1x8i{static_cast<vec1x8i::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x16u>(vec1x16u m) {
        return std::array<vec1x16u, 1>{vec1x16u{static_cast<vec1x16u::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x8u>(vec1x8u m) {
        return std::array<vec1x16u, 1>{vec1x16u{static_cast<vec1x16u::scalar>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x8i>(vec1x8i m) {
        return std::array<vec1x16u, 1>{vec1x16u{static_cast<vec1x16u::scalar>(decay(m))}};
    }

}

#endif //AVEL_VEC1X16U_HPP
