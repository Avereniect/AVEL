#ifndef AVEL_SSE2_VEC4X32F_HPP
#define AVEL_SSE2_VEC4X32F_HPP

#include <avel/Vector.hpp>
#include <avel/impl/vector_primitives/Vector_primitives.hpp>

namespace avel {

    template<>
    class alignas(16) Vector_mask<float, 4> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<float, 4>::type;

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
    class Vector<float, 4> {
    public:

        using scalar_type = float;

        using primitive = avel::vector_primitive<float, 4>::type;

        constexpr static unsigned width = 4;

        using mask = Vector_mask<float, 4>;

        template<class U>
        using rebind_type = Vector<U, 4>;

        template<int M>
        using rebind_width = Vector<float, M>;

        //=================================================
        // Constructors
        //=================================================

        explicit Vector(const primitive content):
            content(content) {}

        explicit Vector(const scalar_type x):
            content(_mm_set1_ps(x)) {}

        explicit Vector(const scalar_type* x):
            content(_mm_loadu_ps(x)) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        static Vector zeros() {
            return Vector{_mm_setzero_ps()};
        }

        static Vector ones() {
            const primitive zeroes = _mm_setzero_ps();
            return Vector{_mm_cmpeq_ps(zeroes, zeroes)};
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
            content = _mm_set1_ps(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        mask operator==(const Vector vec) {
            return mask{_mm_cmpeq_ps(content, vec.content)};
        }

        mask operator!=(const Vector vec) {
            return mask{_mm_cmpneq_ps(content, vec.content)};
        }

        mask operator<(const Vector vec) {
            return mask{_mm_cmplt_ps(content, vec.content)};
        }

        mask operator<=(const Vector vec) {
            return mask{_mm_cmple_ps(content, vec.content)};
        }

        mask operator>(const Vector vec) {
            return mask{_mm_cmpgt_ps(content, vec.content)};
        }

        mask operator>=(const Vector vec) {
            return mask{_mm_cmpge_ps(content, vec.content)};
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
            content = _mm_add_ps (content, vec.content);
            return *this;
        }

        Vector operator+=(const scalar_type x) {
            *this += Vector{x};
            return *this;
        }


        Vector operator-=(const Vector vec) {
            content = _mm_sub_ps (content, vec.content);
            return *this;
        }

        Vector operator-=(const scalar_type x) {
            *this -= Vector{x};
            return *this;
        }


        Vector operator*=(const Vector vec) {
            content = _mm_mul_ps (content, vec.content);
            return *this;
        }

        Vector operator*=(const scalar_type x) {
            *this *= Vector{x};
            return *this;
        }

        Vector operator/=(const Vector vec) {
            content = _mm_div_ps (content, vec.content);
            return *this;
        }

        Vector operator/=(const scalar_type x) {
            *this /= Vector{x};
            return *this;
        }

        Vector operator%=(const Vector vec);

        Vector operator%=(const scalar_type x) {
            *this %= Vector{x};
            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        Vector operator+(const Vector vec) {
            return Vector{_mm_add_ps(content, vec.content)};
        }

        Vector operator+(const scalar_type x) {
            return *this + Vector{x};
        }

        friend Vector operator+(const scalar_type x, const Vector vec) {
            return Vector{x} + vec;
        }



        Vector operator-(const Vector vec) {
            return Vector{_mm_sub_ps(content, vec.content)};
        }

        Vector operator-(const scalar_type x) {
            return *this - Vector{x};
        }

        friend Vector operator-(const scalar_type x, const Vector vec) {
            return Vector{x} - vec;
        }



        Vector operator*(const Vector vec) {
            return Vector{_mm_mul_ps(content, vec.content)};
        }

        Vector operator*(const scalar_type x) {
            return *this * Vector{x};
        }

        friend Vector operator*(const scalar_type x, const Vector vec) {
            return Vector{x} * vec;
        }



        Vector operator/(const Vector vec) {
            return Vector{_mm_div_ps(content, vec.content)};
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
            content = _mm_and_ps(content, vec.content);
            return *this;
        }

        Vector operator|=(const Vector vec) {
            content = _mm_or_ps(content, vec.content);
            return *this;
        }

        Vector operator^=(const Vector vec) {
            content = _mm_xor_ps(content, vec.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector operator~() {
            return Vector{_mm_andnot_ps(ones().content, content)};
        }

        Vector operator&(const Vector vec) {
            return Vector{_mm_and_ps(content, vec.content)};
        }

        Vector operator|(const Vector vec) {
            return Vector{_mm_or_ps(content, vec.content)};
        }

        Vector operator^(const Vector vec) {
            return Vector{_mm_xor_ps(content, vec.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        std::array<float, 4> as_array() const {
            alignas(16) std::array<float, 4> array{};

            _mm_store_ps(array.data(), content);

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

    using vec4x32f = Vector<float, 4>;

}

#endif //AVEL_SSE2_VEC4X32F_HPP
