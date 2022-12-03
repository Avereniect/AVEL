#ifndef AVEL_VEC1X16I_HPP
#define AVEL_VEC1X16I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x16i = Vector<std::int16_t, 1>;

    using mask1x16i = Vector_mask<std::int16_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x16i> div(vec1x16i numerator, vec1x16i denominator);





    template<>
    class Vector_mask<std::int16_t, 1> : public avel_impl::Vector_mask1xT {
        using base = avel_impl::Vector_mask1xT;
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = base::primitive;

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            base(decay(convert<Vector_mask>(v)[0])) {}

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            base::operator&=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            base::operator|=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            base::operator^=(rhs);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            return Vector_mask{base::operator!()};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {
            lhs ^= rhs;
            return lhs;
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask1x16i m) {
        return std::uint32_t(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x16i m) {
        return bool(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x16i m) {
        return bool(decay(m) & 0x1);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x16i m) {
        return !all(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    std::array<mask1x16i, 1> convert<>(mask1x16i m) {
        return {m};
    }

    template<>
    [[nodiscard]]
    std::array<mask1x16i, 1> convert(mask1x8u m) {
        return {mask1x16i{static_cast<bool>(decay(m))}};
    }

    template<>
    [[nodiscard]]
    std::array<mask1x8u, 1> convert(mask1x16i m) {
        return {mask1x8u{decay(m)}};
    }


    template<>
    class alignas(2) Vector<std::int16_t, 1> {
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

        primitive content{};

    public:

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            content(decay(convert<Vector>(v))) {}

        AVEL_FINL explicit Vector(mask m):
            content(primitive(decay(m))) {}

        //AVEL_FINL explicit Vector(primitive content):
        //    content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(x) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            Vector(array[0]) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) noexcept = default;

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

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        [[nodiscard]]
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
            auto results = div(*this, rhs);
            content = results.quot.content;
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector rhs) {
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
            content &= rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            content |= rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            content ^= rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            if (rhs > 15) {
                content = 0;
            } else {
                content <<= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            content >>= (rhs > 15) ? 15 : rhs;
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            if (rhs.content > 15) {
                content = 0;
            } else {
                content <<= rhs.content;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            content >>= (decay(rhs) > 15) ? 15 : decay(rhs);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            return Vector{scalar(~content)};
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
            return Vector{} != *this;
        }

    };

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec1x16i operator-(vec1x16u v) {
        return vec1x16i{std::int16_t(decay(v))};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x16i broadcast_bits(mask1x16i m) {
        return vec1x16i{broadcast_bits<std::int16_t>(decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i blend(vec1x16i a, vec1x16i b, mask1x16i m) {
        return vec1x16i{blend(decay(a), decay(b), decay(m))};
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
        if (decay(a) < decay(b)) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i clamp(vec1x16i x, vec1x16i lo, vec1x16i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i midpoint(vec1x16i a, vec1x16i b) {
        vec1x16i t0 = a & b & vec1x16i{0x1};
        vec1x16i t1 = (a | b) & vec1x16i{0x1} & broadcast_bits(a > b);
        vec1x16i t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i average(vec1x16i a, vec1x16i b) {
        return (a >> 1) + (b >> 1) + (a & b & vec1x16i{0x1});
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i abs(vec1x16i v) {
        return vec1x16i{abs(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i neg_abs(vec1x16u v) {
        return -v;
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i neg_abs(vec1x16i v) {
        return vec1x16i{static_cast<std::int16_t>(-abs(decay(v)))};
    }

    template<>
    AVEL_FINL vec1x16i load<vec1x16i>(const std::int16_t* ptr) {
        return vec1x16i{*ptr};
    }

    template<>
    AVEL_FINL vec1x16i aligned_load<vec1x16i>(const std::int16_t* ptr) {
        return vec1x16i{*ptr};
    }

    AVEL_FINL void store(std::int16_t* ptr, vec1x16i v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::int16_t* ptr, vec1x16i v) {
        *ptr = decay(v);
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x16i> div(vec1x16i numerator, vec1x16i denominator) {
        div_type<vec1x16i> ret;
        ret.quot = decay(numerator) / decay(denominator);
        ret.rem = decay(numerator) % decay(denominator);
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i popcount(vec1x16i v) {
        return vec1x16i{popcount(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i countl_zero(vec1x16i v) {
        return vec1x16i{countl_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i countl_one(vec1x16i v) {
        return vec1x16i{countl_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i countr_zero(vec1x16i v) {
        return vec1x16i{countr_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i countr_one(vec1x16i v) {
        return vec1x16i{countr_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x16i has_single_bit(mask1x16i v) {
        return mask1x16i{has_single_bit(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i rotl(vec1x16i v, long long s) {
        return vec1x16i{rotl(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i rotl(vec1x16i v, vec1x16i s) {
        return vec1x16i{rotl(decay(v), decay(s))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i rotr(vec1x16i v, long long s) {
        return vec1x16i{rotr(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16i rotr(vec1x16i v, vec1x16i s) {
        return vec1x16i{rotr(decay(v), decay(s))};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<std::int16_t, 1> to_array(vec1x16i x) {
        alignas(2) std::array<std::int16_t, 1> array{};
        aligned_store(array.data(), x);
        return array;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 1> convert<vec1x16i, vec1x16i>(vec1x16i x) {
        return {x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x16i>(vec1x16i x) {
        return {vec1x8u{std::uint8_t(decay(x) & 0xFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 1> convert<vec1x8i, vec1x16i>(vec1x16i x) {
        return {vec1x8i{std::int8_t(decay(x) & 0xFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x16i>(vec1x16i x) {
        return {vec1x16u{std::uint16_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 1> convert<vec1x16i, vec1x8u>(vec1x8u x) {
        return {vec1x16i{decay(x)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 1> convert<vec1x16i, vec1x8i>(vec1x8i x) {
        return {vec1x16i{decay(x)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 1> convert<vec1x16i, vec1x16u>(vec1x16u x) {
        return {vec1x16i{std::int16_t(decay(x))}};
    }

}

#endif //AVEL_VEC1X16I_HPP
