#ifndef AVEL_VEC1X32I_HPP
#define AVEL_VEC1X32I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x32i = Vector<std::int32_t, 1>;

    using mask1x32i = Vector_mask<std::int32_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x32i> div(vec1x32i numerator, vec1x32i denominator);





    template<>
    class Vector_mask<std::int32_t, 1> : public avel_impl::Vector_mask1xT  {
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
    AVEL_FINL std::uint32_t count(mask1x32i m) {
        return std::uint32_t(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x32i m) {
        return bool(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x32i m) {
        return bool(decay(m) & 0x1);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x32i m) {
        return !all(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 1> convert<mask1x32i, mask1x32i>(mask1x32i m) {
        return std::array<mask1x32i, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL  std::array<mask1x8u, 1> convert<mask1x8u, mask1x32i>(mask1x32i m) {
        return std::array<mask1x8u, 1>{mask1x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 1> convert<mask1x8i, mask1x32i>(mask1x32i m) {
        return std::array<mask1x8i, 1>{mask1x8i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL  std::array<mask1x16u, 1> convert<mask1x16u, mask1x32i>(mask1x32i m) {
        return std::array<mask1x16u, 1>{mask1x16u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 1> convert<mask1x16i, mask1x32i>(mask1x32i m) {
        return std::array<mask1x16i, 1>{mask1x16i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x32i>(mask1x32i m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 1> convert<mask1x32i, mask1x8u>(mask1x8u m) {
        return std::array<mask1x32i, 1>{mask1x32i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 1> convert<mask1x32i, mask1x8i>(mask1x8i m) {
        return std::array<mask1x32i, 1>{mask1x32i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 1> convert<mask1x32i, mask1x16u>(mask1x16u m) {
        return std::array<mask1x32i, 1>{mask1x32i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 1> convert<mask1x32i, mask1x16i>(mask1x16i m) {
        return std::array<mask1x32i, 1>{mask1x32i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 1> convert<mask1x32i, mask1x32u>(mask1x32u m) {
        return std::array<mask1x32i, 1>{mask1x32i{decay(m)}};
    }






    template<>
    class alignas(4) Vector<std::int32_t, 1> {
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
        AVEL_FINL explicit Vector(Vector<U, width> v):
            content(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
            content(decay(m)) {}

        //AVEL_FINL Vector(primitive content):
        //    content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(x) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array) {
            content = array[0];
        }

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
            content = primitive(std::uint32_t(content) + std::uint32_t(rhs.content));
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = primitive(std::uint32_t(content) - std::uint32_t(rhs.content));
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = primitive(std::uint32_t(content) * std::uint32_t(rhs.content));
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
            if (rhs > 31) {
                content = 0;
            } else {
                content <<= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            if (rhs > 31) {
                content = 0;
            } else {
                content >>= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            if (rhs.content > 31) {
                content = 0;
            } else {
                content <<= rhs.content;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            if (rhs.content > 31) {
                content = 0;
            } else {
                content >>= rhs.content;
            }
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

    AVEL_FINL vec1x32i operator-(vec1x32u v) {
        return vec1x32i{} - vec1x32i{v};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x32i broadcast_bits(mask1x32i m) {
        return vec1x32i{broadcast_bits<std::int32_t>(decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i blend(vec1x32i a, vec1x32i b, mask1x32i m) {
        return vec1x32i{blend(decay(a), decay(b), decay(m))};
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
        if (decay(a) < decay(b)) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i clamp(vec1x32i x, vec1x32i lo, vec1x32i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i midpoint(vec1x32i a, vec1x32i b) {
        vec1x32i addition_mask(0x80000000);
        return vec1x32i{midpoint(vec1x32u{a ^ addition_mask}, vec1x32u{b ^ addition_mask})} ^ addition_mask;
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i average(vec1x32i a, vec1x32i b) {
        vec1x32i addition_mask(0x80);
        return vec1x32i{average(vec1x32u{a ^ addition_mask}, vec1x32u{b ^ addition_mask})} ^ addition_mask;
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i abs(vec1x32i v) {
        return vec1x32i{abs(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i neg_abs(vec1x32i v) {
        return vec1x32i{neg_abs(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i neg_abs(vec1x32u v) {
        return vec1x32i{neg_abs(decay(v))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32i load<vec1x32i>(const std::int32_t* ptr) {
        return vec1x32i{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32i aligned_load<vec1x32i>(const std::int32_t* ptr) {
        return vec1x32i{*ptr};
    }

    AVEL_FINL void store(std::int32_t* ptr, vec1x32i v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::int32_t* ptr, vec1x32i v) {
        *ptr = decay(v);
    }
    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x32i> div(vec1x32i numerator, vec1x32i denominator) {
        auto tmp = std::div(decay(numerator), decay(denominator));

        div_type<vec1x32i> ret;
        ret.quot = tmp.quot;
        ret.rem  = tmp.rem;
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i popcount(vec1x32i v) {
        return vec1x32i{popcount(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i countl_zero(vec1x32i v) {
        return vec1x32i{countl_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i countl_one(vec1x32i v) {
        return vec1x32i{countl_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i countr_zero(vec1x32i v) {
        return vec1x32i{countr_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i countr_one(vec1x32i v) {
        return vec1x32i{countr_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32i has_single_bit(vec1x32i v) {
        return mask1x32i{has_single_bit(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i rotl(vec1x32i v, long long s) {
        return vec1x32i{rotl(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i rotl(vec1x32i v, vec1x32i s) {
        return vec1x32i{rotl(decay(v), decay(s))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i rotr(vec1x32i v, long long s) {
        return vec1x32i{rotr(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32i rotr(vec1x32i v, vec1x32i s) {
        return vec1x32i{rotr(decay(v), decay(s))};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<std::int32_t, 1> to_array(vec1x32i x) {
        alignas(4) std::array<std::int32_t, 1> ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 1> convert<vec1x32i, vec1x32i>(vec1x32i m) {
        return std::array<vec1x32i, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x32i>(vec1x32i m) {
        return std::array<vec1x8u, 1>{vec1x8u{std::uint8_t(decay(m) & 0xFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 1> convert<vec1x8i, vec1x32i>(vec1x32i m) {
        return std::array<vec1x8i, 1>{vec1x8i{std::int8_t(decay(m) & 0xFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x32i>(vec1x32i m) {
        return std::array<vec1x16u, 1>{vec1x16u{std::uint16_t(decay(m) & 0xFFFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 1> convert<vec1x16i, vec1x32i>(vec1x32i m) {
        return std::array<vec1x16i, 1>{vec1x16i{std::int16_t(decay(m) & 0xFFFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x32i>(vec1x32i m) {
        return std::array<vec1x32u, 1>{vec1x32u{std::uint32_t(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 1> convert<vec1x32i, vec1x8u>(vec1x8u m) {
        return std::array<vec1x32i, 1>{vec1x32i{std::int32_t(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 1> convert<vec1x32i, vec1x8i>(vec1x8i m) {
        return std::array<vec1x32i, 1>{vec1x32i{std::int32_t(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 1> convert<vec1x32i, vec1x16u>(vec1x16u m) {
        return std::array<vec1x32i, 1>{vec1x32i{std::int32_t(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 1> convert<vec1x32i, vec1x16i>(vec1x16i m) {
        return std::array<vec1x32i, 1>{vec1x32i{std::int32_t(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 1> convert<vec1x32i, vec1x32u>(vec1x32u m) {
        return std::array<vec1x32i, 1>{vec1x32i{std::int32_t(decay(m))}};
    }

}

#endif //AVEL_VEC1x32I_HPP
