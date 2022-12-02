#ifndef AVEL_VEC1X8U_HPP
#define AVEL_VEC1X8U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x8u = Vector<std::uint8_t, 1>;

    using mask1x8u = Vector_mask<std::uint8_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x8u> div(vec1x8u numerator, vec1x8u denominator);





    template<>
    class Vector_mask<std::uint8_t, 1> : public avel_impl::Vector_mask1xT {
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

        AVEL_FINL Vector_mask& operator&=(Vector_mask1xT rhs) {
            base::operator&=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask1xT rhs) {
            base::operator|=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask1xT rhs) {
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
    AVEL_FINL std::uint32_t count(mask1x8u m) {
        return std::uint32_t(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x8u m) {
        return bool(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x8u m) {
        return bool(decay(m) & 0x1);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x8u m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 1> convert<mask1x8u, mask1x8u>(mask1x8u x) {
        return {x};
    }




    template<>
    class alignas(1) Vector<std::uint8_t, 1> {
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

        //Definition of operator-() deferred until after definition of vec1x8i

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
            if (rhs > 7) {
                content = 0;
            } else {
                content <<= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            if (rhs > 7) {
                content = 0;
            } else {
                content >>= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            if (rhs.content > 7) {
                content = 0;
            } else {
                content <<= rhs.content;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            if (rhs.content > 7) {
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
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x8u broadcast_bits(mask1x8u m) {
        return vec1x8u{std::uint8_t(-decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u blend(vec1x8u a, vec1x8u b, mask1x8u m) {
        return vec1x8u{blend(decay(a), decay(b), decay(m))};
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
        if (decay(a) < decay(b)) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u clamp(vec1x8u x, vec1x8u lo, vec1x8u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u midpoint(vec1x8u a, vec1x8u b) {
        vec1x8u t0 = a & b & vec1x8u{0x1};
        vec1x8u t1 = (a | b) & vec1x8u{0x1} & broadcast_bits(a > b);
        vec1x8u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u average(vec1x8u a, vec1x8u b) {
        return (a >> 1) + (b >> 1) + (a & b & vec1x8u{0x1});
    }

    //Definition of neg_abs delayed until vec1x8i is defined

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x8u load<vec1x8u>(const std::uint8_t* ptr) {
        return vec1x8u{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x8u aligned_load<vec1x8u>(const std::uint8_t* ptr) {
        return vec1x8u{*ptr};
    }

    AVEL_FINL void store(std::uint8_t* ptr, vec1x8u v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::uint8_t* ptr, vec1x8u v) {
        *ptr = decay(v);
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x8u> div(vec1x8u numerator, vec1x8u denominator) {
        div_type<vec1x8u> ret;
        ret.quot = decay(numerator) / decay(denominator);
        ret.rem  = decay(numerator) % decay(denominator);
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

    [[nodiscard]]
    AVEL_FINL vec1x8u rotl(vec1x8u v, long long s) {
        return vec1x8u{rotl(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x8u rotl(vec1x8u v, vec1x8u s) {
        return vec1x8u{rotl(decay(v), decay(s))};
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
    AVEL_FINL std::array<std::uint8_t, 8> to_array(vec1x8u x) {
        alignas(1) std::array<std::uint8_t, 8> ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x8u>(vec1x8u x) {
        return {x};
    }

}

#endif //AVEL_VEC1x8U_HPP
