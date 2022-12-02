#ifndef AVEL_VEC1X64I_HPP
#define AVEL_VEC1X64I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x64i = Vector<std::int64_t, 1>;

    using mask1x64i = Vector_mask<std::int64_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x64i> div(vec1x64i numerator, vec1x64i denominator);





    template<>
    class Vector_mask<std::int64_t, 1> : public avel_impl::Vector_mask1xT {
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
    AVEL_FINL std::uint32_t count(mask1x64i m) {
        return std::uint32_t(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x64i m) {
        return bool(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x64i m) {
        return bool(decay(m) & 0x1);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x64i m) {
        return !all(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint64_t to_integer(mask1x64i m) {
        return std::uint64_t(decay(m));
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 1> convert<mask1x64i, mask1x64i>(mask1x64i m) {
        return std::array<mask1x64i, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 1> convert<mask1x8u, mask1x64i>(mask1x64i m) {
        return std::array<mask1x8u, 1>{mask1x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 1> convert<mask1x8i, mask1x64i>(mask1x64i m) {
        return std::array<mask1x8i, 1>{mask1x8i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert<mask1x16u, mask1x64i>(mask1x64i m) {
        return std::array<mask1x16u, 1>{mask1x16u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 1> convert<mask1x16i, mask1x64i>(mask1x64i m) {
        return std::array<mask1x16i, 1>{mask1x16i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x64i>(mask1x64i m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 1> convert<mask1x32i, mask1x64i>(mask1x64i m) {
        return std::array<mask1x32i, 1>{mask1x32i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64u, 1> convert<mask1x64u, mask1x64i>(mask1x64i m) {
        return std::array<mask1x64u, 1>{mask1x64u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 1> convert<mask1x64i, mask1x8u>(mask1x8u m) {
        return std::array<mask1x64i, 1>{mask1x64i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 1> convert<mask1x64i, mask1x8i>(mask1x8i m) {
        return std::array<mask1x64i, 1>{mask1x64i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 1> convert<mask1x64i, mask1x16u>(mask1x16u m) {
        return std::array<mask1x64i, 1>{mask1x64i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 1> convert<mask1x64i, mask1x16i>(mask1x16i m) {
        return std::array<mask1x64i, 1>{mask1x64i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 1> convert<mask1x64i, mask1x32u>(mask1x32u m) {
        return std::array<mask1x64i, 1>{mask1x64i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 1> convert<mask1x64i, mask1x32i>(mask1x32i m) {
        return std::array<mask1x64i, 1>{mask1x64i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 1> convert<mask1x64i, mask1x64u>(mask1x64u m) {
        return std::array<mask1x64i, 1>{mask1x64i{decay(m)}};
    }






    template<>
    class alignas(8) Vector<std::int64_t, 1> {
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
            content = primitive(std::uint64_t(content) + std::uint64_t(rhs.content));
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = primitive(std::uint64_t(content) - std::uint64_t(rhs.content));
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = primitive(std::uint64_t(content) * std::uint64_t(rhs.content));
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
            if (rhs > 63) {
                content = 0;
            } else {
                content <<= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            if (rhs > 63) {
                content = 0;
            } else {
                content >>= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            if (rhs.content > 63) {
                content = 0;
            } else {
                content <<= rhs.content;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            if (rhs.content > 63) {
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
            return Vector{~content};
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

    AVEL_FINL vec1x64i operator-(vec1x64u v) {
        return vec1x64i{} - vec1x64i{v};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x64i broadcast_bits(mask1x64i m) {
        return vec1x64i{broadcast_bits<std::int64_t>(decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i blend(vec1x64i a, vec1x64i b, mask1x64i m) {
        return vec1x64i{blend(decay(a), decay(b), decay(m))};
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
        if (decay(a) < decay(b)) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i clamp(vec1x64i x, vec1x64i lo, vec1x64i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i midpoint(vec1x64i a, vec1x64i b) {
        vec1x64i addition_mask(0x8000000000000000ull);
        return vec1x64i{midpoint(vec1x64u{a ^ addition_mask}, vec1x64u{b ^ addition_mask})} ^ addition_mask;
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i average(vec1x64i a, vec1x64i b) {
        vec1x64i addition_mask(0x8000000000000000ull);
        return vec1x64i{average(vec1x64u{a ^ addition_mask}, vec1x64u{b ^ addition_mask})} ^ addition_mask;
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i abs(vec1x64i v) {
        return vec1x64i{abs(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i neg_abs(vec1x64i v) {
        return vec1x64i{neg_abs(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i neg_abs(vec1x64u v) {
        return vec1x64i{neg_abs(decay(v))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64i load<vec1x64i>(const std::int64_t* ptr) {
        return vec1x64i{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x64i aligned_load<vec1x64i>(const std::int64_t* ptr) {
        return vec1x64i{*ptr};
    }

    AVEL_FINL void store(std::int64_t* ptr, vec1x64i v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::int64_t* ptr, vec1x64i v) {
        *ptr = decay(v);
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x64i> div(vec1x64i numerator, vec1x64i denominator) {
        auto tmp = std::div(decay(numerator), decay(denominator));

        div_type<vec1x64i> ret;
        ret.quot = tmp.quot;
        ret.rem  = tmp.rem;
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i popcount(vec1x64i v) {
        return vec1x64i{popcount(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i countl_zero(vec1x64i v) {
        return vec1x64i{countl_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i countl_one(vec1x64i v) {
        return vec1x64i{countl_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i countr_zero(vec1x64i v) {
        return vec1x64i{countr_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i countr_one(vec1x64i v) {
        return vec1x64i{countr_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x64i has_single_bit(vec1x64i v) {
        return mask1x64i{has_single_bit(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i rotl(vec1x64i v, long long s) {
        return vec1x64i{rotl(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i rotl(vec1x64i v, vec1x64i s) {
        return vec1x64i{rotl(decay(v), decay(s))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i rotr(vec1x64i v, long long s) {
        return vec1x64i{rotr(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x64i rotr(vec1x64i v, vec1x64i s) {
        return vec1x64i{rotr(decay(v), decay(s))};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<std::int64_t, 1> to_array(vec1x64i x) {
        alignas(8) std::array<std::int64_t, 1> ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 1> convert<vec1x64i, vec1x64i>(vec1x64i x) {
        return std::array<vec1x64i, 1>{x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x64i>(vec1x64i x) {
        return std::array<vec1x8u, 1>{vec1x8u{std::uint8_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 1> convert<vec1x8i, vec1x64i>(vec1x64i x) {
        return std::array<vec1x8i, 1>{vec1x8i{std::int8_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x64i>(vec1x64i x) {
        return std::array<vec1x16u, 1>{vec1x16u{std::uint16_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 1> convert<vec1x16i, vec1x64i>(vec1x64i x) {
        return std::array<vec1x16i, 1>{vec1x16i{std::int16_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x64i>(vec1x64i x) {
        return std::array<vec1x32u, 1>{vec1x32u{std::uint32_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 1> convert<vec1x32i, vec1x64i>(vec1x64i x) {
        return std::array<vec1x32i, 1>{vec1x32i{std::int32_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64u, 1> convert<vec1x64u, vec1x64i>(vec1x64i x) {
        return std::array<vec1x64u, 1>{vec1x64u{std::uint64_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 1> convert<vec1x64i, vec1x8u>(vec1x8u x) {
        return std::array<vec1x64i, 1>{vec1x64i{std::int64_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 1> convert<vec1x64i, vec1x8i>(vec1x8i x) {
        return std::array<vec1x64i, 1>{vec1x64i{std::int64_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 1> convert<vec1x64i, vec1x16u>(vec1x16u x) {
        return std::array<vec1x64i, 1>{vec1x64i{std::int64_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 1> convert<vec1x64i, vec1x16i>(vec1x16i x) {
        return std::array<vec1x64i, 1>{vec1x64i{std::int64_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 1> convert<vec1x64i, vec1x32u>(vec1x32u x) {
        return std::array<vec1x64i, 1>{vec1x64i{std::int64_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 1> convert<vec1x64i, vec1x32i>(vec1x32i x) {
        return std::array<vec1x64i, 1>{vec1x64i{std::int64_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 1> convert<vec1x64i, vec1x64u>(vec1x64u x) {
        return std::array<vec1x64i, 1>{vec1x64i{std::int64_t(decay(x))}};
    }

}

#endif //AVEL_VEC1x64I_HPP
