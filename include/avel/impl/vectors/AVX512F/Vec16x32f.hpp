#ifndef AVEL_AVX512F_VEC16X32F_HPP
#define AVEL_AVX512F_VEC16X32F_HPP

#include <avel/Vector.hpp>
#include <avel/impl/vector_primitives/Vector_primitives.hpp>

namespace avel {

    using vec16x32f = Vector<float, 16>;

    template<>
    class alignas(64) Vector_mask<float, 16> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<float, 16>::type;

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
    class Vector<float, 16> {
    public:

        using scalar_type = float;

        using primitive = avel::vector_primitive<float, 16>::type;

        constexpr static unsigned width = 16;

        using mask = Vector_mask<float, 16>;

        template<class U>
        using rebind_type = Vector<U, 16>;

        template<int M>
        using rebind_width = Vector<float, M>;

        //=================================================
        // Constructors
        //=================================================

        explicit Vector(const primitive content):
            content(content) {}

        explicit Vector(const scalar_type x):
            content(_mm512_set1_ps(x)) {}

        explicit Vector(const scalar_type* x):
            content(_mm512_loadu_ps(x)) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        static Vector zeros() {
            return Vector{_mm512_setzero_ps()};
        }

        static Vector ones() {
            __m512i ones = _mm512_set1_epi64(-1);
            return Vector{_mm512_castsi512_ps(ones)};
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
            content = _mm512_set1_ps(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        mask operator==(const Vector vec) {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_EQ_OQ)};
        }

        mask operator!=(const Vector vec) {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_NEQ_OQ)};
        }

        mask operator<(const Vector vec) {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LT_OQ)};
        }

        mask operator<=(const Vector vec) {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LE_OQ)};
        }

        mask operator>(const Vector vec) {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GT_OQ)};
        }

        mask operator>=(const Vector vec) {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GE_OQ)};
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
            content = _mm512_add_ps(content, vec.content);
            return *this;
        }

        Vector operator+=(const scalar_type x) {
            *this += Vector{x};
            return *this;
        }

        Vector operator-=(const Vector vec) {
            content = _mm512_sub_ps (content, vec.content);
            return *this;
        }

        Vector operator-=(const scalar_type x) {
            *this -= Vector{x};
            return *this;
        }


        Vector operator*=(const Vector vec) {
            content = _mm512_mul_ps (content, vec.content);
            return *this;
        }

        Vector operator*=(const scalar_type x) {
            *this *= Vector{x};
            return *this;
        }

        Vector operator/=(const Vector vec) {
            content = _mm512_div_ps (content, vec.content);
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
            return Vector{_mm512_add_ps(content, vec.content)};
        }

        Vector operator+(const scalar_type x) {
            return *this + Vector{x};
        }

        friend Vector operator+(const scalar_type x, const Vector vec) {
            return Vector{x} + vec;
        }



        Vector operator-(const Vector vec) {
            return Vector{_mm512_sub_ps(content, vec.content)};
        }

        Vector operator-(const scalar_type x) {
            return *this - Vector{x};
        }

        friend Vector operator-(const scalar_type x, const Vector vec) {
            return Vector{x} - vec;
        }



        Vector operator*(const Vector vec) {
            return Vector{_mm512_mul_ps(content, vec.content)};
        }

        Vector operator*(const scalar_type x) {
            return *this * Vector{x};
        }

        friend Vector operator*(const scalar_type x, const Vector vec) {
            return Vector{x} * vec;
        }



        Vector operator/(const Vector vec) {
            return Vector{_mm512_div_ps(content, vec.content)};
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
            content = _mm512_and_ps(content, vec.content);
            return *this;
        }

        Vector operator|=(const Vector vec) {
            content = _mm512_or_ps(content, vec.content);
            return *this;
        }

        Vector operator^=(const Vector vec) {
            content = _mm512_xor_ps(content, vec.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector operator~() {
            return Vector{_mm512_andnot_ps(ones().content, content)};
        }

        Vector operator&(const Vector vec) {
            return Vector{_mm512_and_ps(content, vec.content)};
        }

        Vector operator|(const Vector vec) {
            return Vector{_mm512_or_ps(content, vec.content)};
        }

        Vector operator^(const Vector vec) {
            return Vector{_mm512_xor_ps(content, vec.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        std::array<float, 4> as_array() const {
            alignas(16) std::array<float, 4> array{};

            _mm512_store_ps(array.data(), content);

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

#endif //AVEL_AVX512F_VEC16X32F_HPP
