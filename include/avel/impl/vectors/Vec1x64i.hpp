#ifndef AVEL_VEC1X64I_HPP
#define AVEL_VEC1X64I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x64i = Vector<std::int64_t, 1>;
    using arr1x64i = std::array<std::int64_t, 1>;
    using mask1x64i = Vector_mask<std::int64_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x64i> div(vec1x64i numerator, vec1x64i denominator);
    vec1x64i broadcast_mask(mask1x64i m);
    vec1x64i blend(mask1x64i m, vec1x64i a, vec1x64i b);
    vec1x64i negate(mask1x64i m, vec1x64i x);





    template<>
    class Vector_mask<std::int64_t, 1> {
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
    AVEL_FINL std::array<mask1x64u, 1> convert(mask1x64i v) {
        return {mask1x64u{decay(v)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 1> convert(mask1x64u v) {
        return {mask1x64i{decay(v)}};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask1x64i m) {
        return count(mask1x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x64i m) {
        return any(mask1x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x64i m) {
        return all(mask1x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x64i m) {
        return none(mask1x64u{m});
    }




    template<>
    class Vector<std::int64_t, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int64_t;

        using primitive = std::int64_t;

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

        AVEL_FINL explicit Vector(const arr1x64i& arr) {
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
            content = (rhs >= 64) ? 0x00 : (content << rhs);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            content = (rhs >= 64) ? (content >> (64 - 1)) : (content >> rhs);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            content = (decay(rhs) >= 64) ? 0x00 : (content << decay(rhs));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            content = (decay(rhs) >= 64) ? (content >> (64 - 1)) : (content >> decay(rhs));
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
        1 * sizeof(std::int64_t) == sizeof(vec1x64i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64u, 1> convert(vec1x64i v) {
        return {vec1x64u{static_cast<std::uint64_t>(decay(v))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 1> convert(vec1x64u v) {
        return {vec1x64i{static_cast<std::int64_t>(decay(v))}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec1x64i operator-(vec1x64u v) {
        return -vec1x64i{v};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec1x64i v) {
        return count(vec1x64u{v});
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t any(vec1x64i v) {
        return any(vec1x64u{v});
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t all(vec1x64i v) {
        return all(vec1x64u{v});
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t none(vec1x64i v) {
        return none(vec1x64u{v});
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i broadcast_mask(mask1x64i m) {
        return vec1x64i{static_cast<vec1x64i::scalar>(-decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i keep(mask1x64i m, vec1x64i v) {
        return vec1x64i{keep(mask1x64u{m}, vec1x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i clear(mask1x64i m, vec1x64i v) {
        return vec1x64i{clear(mask1x64u{m}, vec1x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i blend(mask1x64i m, vec1x64i a, vec1x64i b) {
        return vec1x64i{blend(decay(m), decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i byteswap(vec1x64i v) {
        return vec1x64i{byteswap(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i max(vec1x64i a, vec1x64i b) {
        return vec1x64i{max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i min(vec1x64i a, vec1x64i b) {
        return vec1x64i{min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 2> minmax(vec1x64i a, vec1x64i b) {
        return {min(a, b), max(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i clamp(vec1x64i x, vec1x64i lo, vec1x64i hi) {
        return vec1x64i{clamp(decay(x), decay(lo), decay(hi))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i average(vec1x64i a, vec1x64i b) {
        return vec1x64i{average(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i midpoint(vec1x64i a, vec1x64i b) {
        return vec1x64i{midpoint(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i negate(mask1x64i m, vec1x64i v) {
        if (decay(m)) {
            return -v;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i abs(vec1x64i v) {
        if (decay(v) < 0) {
            return -v;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i neg_abs(vec1x64i v) {
        if (decay(v) < 0) {
            return v;
        } else {
            return -v;
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i neg_abs(vec1x64u v) {
        return neg_abs(vec1x64i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64i load<vec1x64i>(const std::int64_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x64i{*ptr};
        } else {
            return vec1x64i{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64i load<vec1x64i, vec1x64i::width>(const std::int64_t* ptr) {
        return vec1x64i{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64i aligned_load<vec1x64i>(const std::int64_t* ptr, std::uint32_t n) {
        if (n) {
            return vec1x64i{*ptr};
        } else {
            return vec1x64i{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64i aligned_load<vec1x64i, vec1x64i::width>(const std::int64_t* ptr) {
        return vec1x64i{*ptr};
    }







    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64i gather<vec1x64i>(const std::int64_t* ptr, vec1x64i indices, std::uint32_t n) {
        if (n) {
            return vec1x64i{ptr[decay(indices)]};
        } else {
            return vec1x64i{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64i gather<vec1x64i>(const std::int64_t* ptr, vec1x64i indices) {
        return vec1x64i{ptr[decay(indices)]};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64u gather<vec1x64u>(const std::uint64_t* ptr, vec1x64i indices, std::uint32_t n) {
        if (n) {
            return vec1x64u{ptr[decay(indices)]};
        } else {
            return vec1x64u{0x00};
        }
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64u gather<vec1x64u>(const std::uint64_t* ptr, vec1x64i indices) {
        return vec1x64u{ptr[decay(indices)]};
    }





    template<std::uint32_t N = vec1x64i::width>
    AVEL_FINL void store(std::int64_t* ptr, vec1x64i v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void store<vec1x64i::width>(std::int64_t* ptr, vec1x64i v) {
        *ptr = decay(v);
    }

    AVEL_FINL void store(std::int64_t* ptr, vec1x64i v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    template<std::uint32_t N = vec1x64i::width>
    AVEL_FINL void aligned_store(std::int64_t* ptr, vec1x64i v) {
        if (N) {
            *ptr = decay(v);
        }
    }

    template<>
    AVEL_FINL void aligned_store<vec1x64i::width>(std::int64_t* ptr, vec1x64i v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::int64_t* ptr, vec1x64i v, std::uint32_t n) {
        if (n) {
            *ptr = decay(v);
        }
    }



    template<std::uint32_t N = vec1x64i::width>
    AVEL_FINL void scatter(std::int64_t* ptr, vec1x64i v, vec1x64i indices) {
        ptr[decay(indices)] = decay(v);
    }

    template<>
    AVEL_FINL void scatter<0>(std::int64_t* ptr, vec1x64i v, vec1x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec1x64i::width>(std::int64_t* ptr, vec1x64i v, vec1x64i indices) {
        ptr[decay(indices)] = decay(v);
    }

    AVEL_FINL void scatter(std::int64_t* ptr, vec1x64i v, vec1x64i indices, std::uint32_t n) {
        if (n) {
            ptr[decay(indices)] = decay(v);
        }
    }



    template<std::uint32_t N = vec1x64i::width>
    AVEL_FINL void scatter(std::uint64_t* ptr, vec1x64u v, vec1x64i indices) {
        ptr[decay(indices)] = decay(v);
    }

    template<>
    AVEL_FINL void scatter<0>(std::uint64_t* ptr, vec1x64u v, vec1x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec1x64i::width>(std::uint64_t* ptr, vec1x64u v, vec1x64i indices) {
        ptr[decay(indices)] = decay(v);
    }

    AVEL_FINL void scatter(std::uint64_t* ptr, vec1x64u v, vec1x64i indices, std::uint32_t n) {
        if (n) {
            ptr[decay(indices)] = decay(v);
        }
    }



    [[nodiscard]]
    AVEL_FINL arr1x64i to_array(vec1x64i v) {
        alignas(8) arr1x64i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int64_t extract(vec1x64i v) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return decay(v);
    }

    template<std::uint32_t N>
    AVEL_FINL vec1x64i insert(vec1x64i v, std::int64_t x) {
        static_assert(N <= 1, "Specified index does not exist");
        typename std::enable_if<N <= 1, int>::type dummy_variable = 0;

        return vec1x64i{x};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    vec1x64i bit_shift_left(vec1x64i v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        return vec1x64i{bit_shift_left<S>(vec1x64u{v})};
    }

    template<std::uint32_t S>
    [[nodiscard]]
    vec1x64i bit_shift_right(vec1x64i v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        return v >> S;
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x64i rotl(vec1x64i v) {
        return vec1x64i{rotl<S>(vec1x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i rotl(vec1x64i v, long long s) {
        return vec1x64i{rotl(vec1x64u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i rotl(vec1x64i v, vec1x64i s) {
        return vec1x64i{rotl(vec1x64u{v}, vec1x64u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec1x64i rotr(vec1x64i v) {
        return vec1x64i{rotr<S>(vec1x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i rotr(vec1x64i v, long long s) {
        return vec1x64i{rotr(vec1x64u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i rotr(vec1x64i v, vec1x64i s) {
        return vec1x64i{rotr(vec1x64u{v}, vec1x64u{s})};
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x64i> div(vec1x64i x, vec1x64i y) {
        div_type<vec1x64i> ret;
        ret.quot = decay(x) / decay(y);
        ret.rem  = decay(x) % decay(y);
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i popcount(vec1x64i v) {
        return vec1x64i{popcount(vec1x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i countl_zero(vec1x64i v) {
        return vec1x64i{countl_zero(vec1x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i countl_one(vec1x64i v) {
        return vec1x64i{countl_one(vec1x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i countr_zero(vec1x64i v) {
        return vec1x64i{countr_zero(vec1x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i countr_one(vec1x64i v) {
        return vec1x64i{countr_one(vec1x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64i has_single_bit(vec1x64i v) {
        return mask1x64i{has_single_bit(vec1x64u{v})};
    }

}

#endif //AVEL_VEC1X64I_HPP
