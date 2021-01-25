#ifndef AVEL_AVX_VEC8X32F_HPP
#define AVEL_AVX_VEC8X32F_HPP

#include <avel/Vector.hpp>
#include <avel/impl/vector_primitives/Vector_primitives.hpp>

namespace avel {

    using vec8x32f = Vector<float, 8>;

    template<>
    class alignas(32) Vector_mask<float, 8> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<float, 8>::type;

        //=================================================
        // Constructor
        //=================================================

        explicit Vector_mask(const primitive content):
            content(content) {}

        //=================================================
        // Instance members
        //=================================================

    private:

        primitive content;
    };



    template<>
    class Vector<float, 8> {
    public:

        using scalar_type = float;

        using primitive = avel::vector_primitive<float, 8>::type;

        constexpr static unsigned width = 8;

        using mask = Vector_mask<float, 8>;

        template<class U>
        using rebind_type = Vector<U, 8>;

        template<int M>
        using rebind_width = Vector<float, M>;

        //=================================================
        // Constructors
        //=================================================

        explicit Vector(const primitive content):
            content(content) {}

        explicit Vector(const scalar_type x):
            content(_mm256_set1_ps(x)) {}

        explicit Vector(const scalar_type* x):
            content(_mm256_loadu_ps(x)) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        static Vector zeros() {
            return Vector{_mm256_setzero_ps()};
        }

        static Vector ones() {
            const primitive zeroes = _mm256_setzero_ps();
            return Vector{_mm256_cmp_ps(zeroes, zeroes, _CMP_EQ_OQ)};
        }

        //=================================================
        // Assignment operators
        //=================================================

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        Vector& operator=(const primitive vec) {
            this->content = vec;
            return *this;
        }

        Vector& operator=(const scalar_type x) {
            content = _mm256_set1_ps(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        mask operator==(const Vector vec) {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_EQ_OQ)};
        }

        mask operator!=(const Vector vec) {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_NEQ_OQ)};
        }

        mask operator<(const Vector vec) {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_LT_OQ)};
        }

        mask operator<=(const Vector vec) {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_LE_OQ)};
        }

        mask operator>(const Vector vec) {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_GT_OQ)};
        }

        mask operator>=(const Vector vec) {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_GE_OQ)};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        Vector operator+() {
            return *this;
        }

        Vector operator-() {
            return zeros() - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        Vector operator+=(const Vector vec) {
            content = _mm256_add_ps(content, vec.content);
            return *this;
        }

        Vector operator+=(const scalar_type x) {
            *this += Vector{x};
            return *this;
        }

        Vector operator-=(const Vector vec) {
            content = _mm256_sub_ps (content, vec.content);
            return *this;
        }

        Vector operator-=(const scalar_type x) {
            *this -= Vector{x};
            return *this;
        }


        Vector operator*=(const Vector vec) {
            content = _mm256_mul_ps (content, vec.content);
            return *this;
        }

        Vector operator*=(const scalar_type x) {
            *this *= Vector{x};
            return *this;
        }

        Vector operator/=(const Vector vec) {
            content = _mm256_div_ps (content, vec.content);
            return *this;
        }

        Vector operator/=(const scalar_type x) {
            *this /= Vector{x};
            return *this;
        }

        //TODO: Implement modulus operator
        //Vector operator%=(const Vector vec) {}

        //Vector operator%=(const scalar_type x) {}

        //=================================================
        // Arithmetic operators
        //=================================================

        Vector operator+(const Vector vec) {
            return Vector{_mm256_add_ps(content, vec.content)};
        }

        Vector operator+(const scalar_type x) {
            return *this + Vector{x};
        }

        friend Vector operator+(const scalar_type x, const Vector vec) {
            return Vector{x} + vec;
        }



        Vector operator-(const Vector vec) {
            return Vector{_mm256_sub_ps(content, vec.content)};
        }

        Vector operator-(const scalar_type x) {
            return *this - Vector{x};
        }

        friend Vector operator-(const scalar_type x, const Vector vec) {
            return Vector{x} - vec;
        }



        Vector operator*(const Vector vec) {
            return Vector{_mm256_mul_ps(content, vec.content)};
        }

        Vector operator*(const scalar_type x) {
            return *this * Vector{x};
        }

        friend Vector operator*(const scalar_type x, const Vector vec) {
            return Vector{x} * vec;
        }



        Vector operator/(const Vector vec) {
            return Vector{_mm256_div_ps(content, vec.content)};
        }

        Vector operator/(const scalar_type x) {
            return *this / Vector{x};
        }

        friend Vector operator/(const scalar_type x, const Vector vec) {
            return Vector{x} / vec;
        }

        //=================================================
        // Increment/Decrement operators
        //=================================================

        Vector& operator++() {
            *this += 1;
            return *this;
        }

        Vector operator++(int) {
            auto temp = *this;
            *this += 1;
            return temp;
        }

        Vector& operator--() {
            *this -= 1;
            return *this;
        }

        Vector operator--(int) {
            auto temp = *this;
            *this -= 1;
            return temp;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        Vector operator&=(const Vector vec) {
            content = _mm256_and_ps(content, vec.content);
            return *this;
        }

        Vector operator|=(const Vector vec) {
            content = _mm256_or_ps(content, vec.content);
            return *this;
        }

        Vector operator^=(const Vector vec) {
            content = _mm256_xor_ps(content, vec.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector operator~() {
            return Vector{_mm256_andnot_ps(ones().content, content)};
        }

        Vector operator&(const Vector vec) {
            return Vector{_mm256_and_ps(content, vec.content)};
        }

        Vector operator|(const Vector vec) {
            return Vector{_mm256_or_ps(content, vec.content)};
        }

        Vector operator^(const Vector vec) {
            return Vector{_mm256_xor_ps(content, vec.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        std::array<float, 4> as_array() const {
            alignas(16) std::array<float, 4> array{};

            _mm256_store_ps(array.data(), content);

            return array;
        }

        operator primitive() {
            return content;
        }

        explicit operator mask() {
            return *this == zeros();
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

}

#endif //AVEL_AVX_VEC8X32F_HPP
