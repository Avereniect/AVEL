#ifndef AVEL_VEC1X16I_HPP
#define AVEL_VEC1X16I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x16i = Vector<std::int16_t, 1>;
    using arr1x16i = std::array<std::int16_t, 1>;
    using mask1x16i = Vector_mask<std::int16_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x16i> div(vec1x16i numerator, vec1x16i denominator);
    vec1x16i broadcast_mask(mask1x16i m);
    vec1x16i blend(vec1x16i a, vec1x16i b, mask1x16i m);
    vec1x16i negate(vec1x16i m, vec1x16i x);





    template<>
    class Vector_mask<std::int16_t, 1> {
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
            content(p) {}

        AVEL_FINL explicit Vector_mask(bool b):
            content(-b) {}

        AVEL_FINL explicit Vector_mask(const arr1xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
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
            *this = Vector_mask{b};
            return *this;
        }

        AVEL_FINL Vector_mask& operator=(primitive p) {
            *this = Vector_mask{p};
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
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert(mask1x16i v) {
        return {mask1x16u{decay(v)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 1> convert(mask1x16u v) {
        return {mask1x16i{decay(v)}};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask1x16i m) {
        return count(mask1x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x16i m) {
        return any(mask1x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x16i m) {
        return all(mask1x16u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x16i m) {
        return none(mask1x16u{m});
    }




    template<>
    class Vector<std::int16_t, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int16_t;

        using primitive = std::int16_t;

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

        AVEL_FINL Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const arr1x16i& arr) {
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
            content = (rhs >= 16) ? (content >> (16 - 1)) : (content >> rhs);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            content = (decay(rhs) >= 16) ? 0x00 : (content << decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            content = (decay(rhs) >= 16) ? (content >> (16 - 1)) : (content >> decay(rhs));
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

    static_assert(
        1 * sizeof(std::int16_t) == sizeof(vec1x16i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert(vec1x16i v) {
        return {vec1x16u{static_cast<std::uint16_t>(decay(v))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 1> convert(vec1x16u v) {
        return {vec1x16i{static_cast<std::int16_t>(decay(v))}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec1x16i operator-(vec1x16u v) {
        return -vec1x16i{v};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x16i broadcast_mask(mask1x16i m) {
        return vec1x16i{static_cast<vec1x16i::scalar>(-decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i blend(mask1x16i m, vec1x16i a, vec1x16i b) {
        return vec1x16i{blend(decay(m), decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i byteswap(vec1x16i v) {
        return vec1x16i{byteswap(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i max(vec1x16i a, vec1x16i b) {
        return vec1x16i{max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i min(vec1x16i a, vec1x16i b) {
        return vec1x16i{min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 2> minmax(vec1x16i a, vec1x16i b) {
        return {min(a, b), max(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i clamp(vec1x16i x, vec1x16i lo, vec1x16i hi) {
        return vec1x16i{clamp(decay(x), decay(lo), decay(hi))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i average(vec1x16i a, vec1x16i b) {
        return vec1x16i{average(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i midpoint(vec1x16i a, vec1x16i b) {
        return vec1x16i{midpoint(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i negate(vec1x16i m, vec1x16i v) {
        if (decay(m)) {
            return -v;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i abs(vec1x16i v) {
        if (decay(v) < 0) {
            return -v;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i neg_abs(vec1x16i v) {
        if (decay(v) < 0) {
            return v;
        } else {
            return -v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i neg_abs(vec1x16u v) {
        return neg_abs(vec1x16i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16i load<vec1x16i>(const std::int16_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x16i{*ptr};
        } else {
            return vec1x16i{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16i load<vec1x16i, vec1x16i::width>(const std::int16_t* ptr) {
        return vec1x16i{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16i aligned_load<vec1x16i>(const std::int16_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x16i{*ptr};
        } else {
            return vec1x16i{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16i aligned_load<vec1x16i, vec1x16i::width>(const std::int16_t* ptr) {
        return vec1x16i{*ptr};
    }






    template<std::uint32_t N = vec1x16i::width>
    AVEL_FINL void store(std::int16_t* ptr, vec1x16i v) {
        *ptr = decay(v);
    }

    template<>
    AVEL_FINL void store<vec1x16i::width>(std::int16_t* ptr, vec1x16i v) {
        *ptr = decay(v);
    }

    AVEL_FINL void store(std::int16_t* ptr, vec1x16i v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    template<std::uint32_t N = vec1x16i::width>
    AVEL_FINL void aligned_store(std::int16_t* ptr, vec1x16i v) {
        *ptr = decay(v);
    }

    template<>
    AVEL_FINL void aligned_store<vec1x16i::width>(std::int16_t* ptr, vec1x16i v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::int16_t* ptr, vec1x16i v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    [[nodiscard]]
    AVEL_FINL arr1x16i to_array(vec1x16i v) {
        alignas(2) arr1x16i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int16_t extract(vec1x16i v) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return decay(v);
    }

    template<std::uint32_t N>
    AVEL_FINL vec1x16i insert(vec1x16i v, std::int16_t x) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return vec1x16i{x};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    vec1x16i bit_shift_left(vec1x16i v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        return vec1x16i{bit_shift_left<S>(vec1x16u{v})};
    }

    template<std::uint32_t S>
    [[nodiscard]]
    vec1x16i bit_shift_right(vec1x16i v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        return vec1x16i{bit_shift_right<S>(vec1x16u{v})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x16i rotl(vec1x16i v) {
        return vec1x16i{rotl<S>(vec1x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i rotl(vec1x16i v, long long s) {
        return vec1x16i{rotl(vec1x16u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i rotl(vec1x16i v, vec1x16i s) {
        return vec1x16i{rotl(vec1x16u{v}, vec1x16u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x16i rotr(vec1x16i v) {
        return vec1x16i{rotr<S>(vec1x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i rotr(vec1x16i v, long long s) {
        return vec1x16i{rotr(vec1x16u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i rotr(vec1x16i v, vec1x16i s) {
        return vec1x16i{rotr(vec1x16u{v}, vec1x16u{s})};
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x16i> div(vec1x16i x, vec1x16i y) {
        div_type<vec1x16i> ret;
        ret.quot = decay(x) / decay(y);
        ret.rem  = decay(x) % decay(y);
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i popcount(vec1x16i v) {
        return vec1x16i{popcount(vec1x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i countl_zero(vec1x16i v) {
        return vec1x16i{countl_zero(vec1x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i countl_one(vec1x16i v) {
        return vec1x16i{countl_one(vec1x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i countr_zero(vec1x16i v) {
        return vec1x16i{countr_zero(vec1x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i countr_one(vec1x16i v) {
        return vec1x16i{countr_one(vec1x16u{v})};
    }

    [[nodiscard]]
    AVEL_FINL mask1x16i has_single_bit(vec1x16i v) {
        return mask1x16i{has_single_bit(vec1x16u{v})};
    }

}

#endif //AVEL_VEC1X16I_HPP
