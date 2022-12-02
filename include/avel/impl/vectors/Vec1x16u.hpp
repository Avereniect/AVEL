#ifndef AVEL_VEC1X16U_HPP
#define AVEL_VEC1X16U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x16u = Vector<std::uint16_t, 1>;

    using mask1x16u = Vector_mask<std::uint16_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x16u> div(vec1x16u numerator, vec1x16u denominator);





    template<>
    class Vector_mask<std::uint16_t, 1> : public avel_impl::Vector_mask1xT {
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
    AVEL_FINL std::uint32_t count(mask1x16u m) {
        return bool(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x16u m) {
        return bool(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x16u m) {
        return bool(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x16u m) {
        return !all(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================
    
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert<mask1x16u, mask1x16u>(mask1x16u m) {
        return {m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 1> convert<mask1x8u, mask1x16u>(mask1x16u m) {
        return {mask1x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 1> convert<mask1x8i, mask1x16u>(mask1x16u m) {
        return {mask1x8i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert<mask1x16u, mask1x8u>(mask1x8u m) {
        return {mask1x16u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert<mask1x16u, mask1x8i>(mask1x8i m) {
        return {mask1x16u{decay(m)}};
    }

    



    template<>
    class alignas(2) Vector<std::uint16_t, 1> {
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
        AVEL_FINL explicit Vector(Vector<U, width> v):
            content(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
            content(decay(m)) {}

        //AVEL_FINL explicit Vector(primitive content):
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

        //Definition of operator-() deferred until after definition of vec1x16i

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
            if (15 < rhs) {
                content  = 0;
            } else {
                content <<= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            if (15 < rhs) {
                content  = 0;
            } else {
                content >>= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            if (15 < rhs.content) {
                content = 0;
            } else {
                content <<= rhs.content;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            if (15 < rhs.content) {
                content  = 0;
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
            return Vector{primitive(~content)};
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
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x16u broadcast_bits(mask1x16u m) {
        return vec1x16u{broadcast_bits<std::uint16_t>(decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u blend(vec1x16u a, vec1x16u b, mask1x16u m) {
        return vec1x16u{blend(decay(a), decay(b), decay(m))};
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
        if (decay(a) < decay(b)) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u clamp(vec1x16u x, vec1x16u lo, vec1x16u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u midpoint(vec1x16u a, vec1x16u b) {
        vec1x16u t0 = a & b & vec1x16u{0x1};
        vec1x16u t1 = (a | b) & vec1x16u{0x1} & broadcast_bits(a > b);
        vec1x16u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u average(vec1x16u a, vec1x16u b) {
        return (a >> 1) + (b >> 1) + (a & b & vec1x16u{0x1});
    }

    //Definition of neg_abs delayed until vec1x16i is defined

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16u load<vec1x16u>(const std::uint16_t* ptr) {
        return vec1x16u{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x16u aligned_load<vec1x16u>(const std::uint16_t* ptr) {
        return vec1x16u{*ptr};
    }

    AVEL_FINL void store(std::uint16_t* ptr, vec1x16u v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec1x16u v) {
        *ptr = decay(v);
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x16u> div(vec1x16u numerator, vec1x16u denominator) {
        div_type<vec1x16u> ret;
        ret.quot = decay(numerator) / decay(denominator);
        ret.rem  = decay(numerator) % decay(denominator);
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
        return vec1x16u{countr_zero(decay(v))};
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

    [[nodiscard]]
    AVEL_FINL vec1x16u rotl(vec1x16u v, long long s) {
        return vec1x16u{rotl(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x16u rotl(vec1x16u v, vec1x16u s) {
        return vec1x16u{rotl(decay(v), decay(s))};
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
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<std::uint16_t, 1> to_array(vec1x16u x) {
        alignas(2) std::array<std::uint16_t, 1> ret;
        aligned_store(ret.data(), x);
        return ret;
    }
    
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x16u>(vec1x16u x) {
        return {x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x16u>(vec1x16u x) {
        return {vec1x8u{std::uint8_t(decay(x) & 0xFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 1> convert<vec1x8i, vec1x16u>(vec1x16u x) {
        return {vec1x8i{std::int8_t(decay(x) & 0xFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x8u>(vec1x8u x) {
        return {vec1x16u{std::uint16_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x8i>(vec1x8i x) {
        return {vec1x16u{std::uint16_t(decay(x))}};
    }
   
}

#endif //AVEL_VEC1x16U_HPP

