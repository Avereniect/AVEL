#ifndef AVEL_VEC1X32I_HPP
#define AVEL_VEC1X32I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x32i = Vector<std::int32_t, 1>;
    using arr1x32i = std::array<std::int32_t, 1>;
    using mask1x32i = Vector_mask<std::int32_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x32i> div(vec1x32i numerator, vec1x32i denominator);
    vec1x32i broadcast_mask(mask1x32i m);
    vec1x32i blend(mask1x32i m, vec1x32i a, vec1x32i b);
    vec1x32i negate(vec1x32i m, vec1x32i x);





    template<>
    class Vector_mask<std::int32_t, 1> {
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
            *this = Vector_mask{b};
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
    AVEL_FINL std::array<mask1x32u, 1> convert(mask1x32i v) {
        return {mask1x32u{decay(v)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 1> convert(mask1x32u v) {
        return {mask1x32i{decay(v)}};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask1x32i m) {
        return count(mask1x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x32i m) {
        return any(mask1x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x32i m) {
        return all(mask1x32u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x32i m) {
        return none(mask1x32u{m});
    }




    template<>
    class Vector<std::int32_t, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int32_t;

        using primitive = std::int32_t;

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

        AVEL_FINL explicit Vector(const arr1x32i& arr) {
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
            content = (rhs >= 32) ? 0x00 : (content << rhs);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            content = (rhs >= 32) ? (content >> (32 - 1)) : (content >> rhs);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            content = (decay(rhs) >= 32) ? 0x00 : (content << decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            content = (decay(rhs) >= 32) ? (content >> (32 - 1)) : (content >> decay(rhs));
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
        1 * sizeof(std::int32_t) == sizeof(vec1x32i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert(vec1x32i v) {
        return {vec1x32u{static_cast<std::uint32_t>(decay(v))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 1> convert(vec1x32u v) {
        return {vec1x32i{static_cast<std::int32_t>(decay(v))}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec1x32i operator-(vec1x32u v) {
        return -vec1x32i{v};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec1x32i v) {
        return count(vec1x32u{v});
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t any(vec1x32i v) {
        return any(vec1x32u{v});
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t all(vec1x32i v) {
        return all(vec1x32u{v});
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t none(vec1x32i v) {
        return none(vec1x32u{v});
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i broadcast_mask(mask1x32i m) {
        return vec1x32i{static_cast<vec1x32i::scalar>(-decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i keep(mask1x32i m, vec1x32i v) {
        return vec1x32i{keep(mask1x32u{m}, vec1x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i clear(mask1x32i m, vec1x32i v) {
        return vec1x32i{clear(mask1x32u{m}, vec1x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i blend(mask1x32i m, vec1x32i a, vec1x32i b) {
        return vec1x32i{blend(decay(m), decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i byteswap(vec1x32i v) {
        return vec1x32i{byteswap(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i max(vec1x32i a, vec1x32i b) {
        return vec1x32i{max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i min(vec1x32i a, vec1x32i b) {
        return vec1x32i{min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 2> minmax(vec1x32i a, vec1x32i b) {
        return {min(a, b), max(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i clamp(vec1x32i x, vec1x32i lo, vec1x32i hi) {
        return vec1x32i{clamp(decay(x), decay(lo), decay(hi))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i average(vec1x32i a, vec1x32i b) {
        return vec1x32i{average(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i midpoint(vec1x32i a, vec1x32i b) {
        return vec1x32i{midpoint(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i negate(vec1x32i m, vec1x32i v) {
        if (decay(m)) {
            return -v;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i abs(vec1x32i v) {
        if (decay(v) < 0) {
            return -v;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i neg_abs(vec1x32i v) {
        if (decay(v) < 0) {
            return v;
        } else {
            return -v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i neg_abs(vec1x32u v) {
        return neg_abs(vec1x32i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32i load<vec1x32i>(const std::int32_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x32i{*ptr};
        } else {
            return vec1x32i{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32i load<vec1x32i, vec1x32i::width>(const std::int32_t* ptr) {
        return vec1x32i{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32i aligned_load<vec1x32i>(const std::int32_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x32i{*ptr};
        } else {
            return vec1x32i{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32i aligned_load<vec1x32i, vec1x32i::width>(const std::int32_t* ptr) {
        return vec1x32i{*ptr};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32i gather<vec1x32i>(const std::int32_t* ptr, vec1x32i indices, std::uint32_t n) {
        if (n) {
            return vec1x32i{ptr[decay(indices)]};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32i gather<vec1x32i>(const std::int32_t* ptr, vec1x32i indices) {
        return vec1x32i{ptr[decay(indices)]};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u gather<vec1x32u>(const std::uint32_t* ptr, vec1x32i indices, std::uint32_t n) {
        if (n) {
            return vec1x32u{ptr[decay(indices)]};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u gather<vec1x32u>(const std::uint32_t* ptr, vec1x32i indices) {
        return vec1x32u{ptr[decay(indices)]};
    }




    template<std::uint32_t N = vec1x32i::width>
    AVEL_FINL void store(std::int32_t* ptr, vec1x32i v) {
        *ptr = decay(v);
    }

    template<>
    AVEL_FINL void store<vec1x32i::width>(std::int32_t* ptr, vec1x32i v) {
        *ptr = decay(v);
    }

    AVEL_FINL void store(std::int32_t* ptr, vec1x32i v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    template<std::uint32_t N = vec1x32i::width>
    AVEL_FINL void aligned_store(std::int32_t* ptr, vec1x32i v) {
        *ptr = decay(v);
    }

    template<>
    AVEL_FINL void aligned_store<vec1x32i::width>(std::int32_t* ptr, vec1x32i v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::int32_t* ptr, vec1x32i v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    AVEL_FINL void scatter(std::int32_t* ptr, vec1x32i v, vec1x32i indices, std::uint32_t n) {
        if (n) {
            ptr[decay(indices)] = decay(v);
        }
    }

    template<std::uint32_t N = vec1x32i::width>
    AVEL_FINL void scatter(std::int32_t* ptr, vec1x32i v, vec1x32i indices) {
        ptr[decay(indices)] = decay(v);
    }

    template<>
    AVEL_FINL void scatter<0>(std::int32_t* ptr, vec1x32i v, vec1x32i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec1x32i::width>(std::int32_t* ptr, vec1x32i v, vec1x32i indices) {
        ptr[decay(indices)] = decay(v);
    }



    AVEL_FINL void scatter(std::uint32_t* ptr, vec1x32u v, vec1x32i indices, std::uint32_t n) {
        if (n) {
            ptr[decay(indices)] = decay(v);
        }
    }

    template<std::uint32_t N = vec1x32i::width>
    AVEL_FINL void scatter(std::uint32_t* ptr, vec1x32u v, vec1x32i indices) {
        ptr[decay(indices)] = decay(v);
    }

    template<>
    AVEL_FINL void scatter<0>(std::uint32_t* ptr, vec1x32u v, vec1x32i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec1x32i::width>(std::uint32_t* ptr, vec1x32u v, vec1x32i indices) {
        ptr[decay(indices)] = decay(v);
    }



    [[nodiscard]]
    AVEL_FINL arr1x32i to_array(vec1x32i v) {
        alignas(4) arr1x32i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int32_t extract(vec1x32i v) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return decay(v);
    }

    template<std::uint32_t N>
    AVEL_FINL vec1x32i insert(vec1x32i v, std::int32_t x) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return vec1x32i{x};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    vec1x32i bit_shift_left(vec1x32i v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        return vec1x32i{bit_shift_left<S>(vec1x32u{v})};
    }

    template<std::uint32_t S>
    [[nodiscard]]
    vec1x32i bit_shift_right(vec1x32i v) {
        static_assert(S <= 32, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 32, int>::type dummy_variable = 0;

        return vec1x32i{bit_shift_right<S>(vec1x32u{v})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x32i rotl(vec1x32i v) {
        return vec1x32i{rotl<S>(vec1x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i rotl(vec1x32i v, long long s) {
        return vec1x32i{rotl(vec1x32u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i rotl(vec1x32i v, vec1x32i s) {
        return vec1x32i{rotl(vec1x32u{v}, vec1x32u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x32i rotr(vec1x32i v) {
        return vec1x32i{rotr<S>(vec1x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i rotr(vec1x32i v, long long s) {
        return vec1x32i{rotr(vec1x32u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i rotr(vec1x32i v, vec1x32i s) {
        return vec1x32i{rotr(vec1x32u{v}, vec1x32u{s})};
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x32i> div(vec1x32i x, vec1x32i y) {
        div_type<vec1x32i> ret;
        ret.quot = decay(x) / decay(y);
        ret.rem  = decay(x) % decay(y);
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i popcount(vec1x32i v) {
        return vec1x32i{popcount(vec1x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i countl_zero(vec1x32i v) {
        return vec1x32i{countl_zero(vec1x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i countl_one(vec1x32i v) {
        return vec1x32i{countl_one(vec1x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i countr_zero(vec1x32i v) {
        return vec1x32i{countr_zero(vec1x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i countr_one(vec1x32i v) {
        return vec1x32i{countr_one(vec1x32u{v})};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32i has_single_bit(vec1x32i v) {
        return mask1x32i{has_single_bit(vec1x32u{v})};
    }

}

#endif //AVEL_VEC1X32I_HPP
