#include "avel/Vector.hpp"
#include "avel/impl/vector_primitives/Vector_primitives.hpp"

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

        AVEL_FINL explicit Vector_mask(const primitive contents):
            contents(contents) {}

        AVEL_FINL explicit Vector_mask(bool x):
            contents(from_bool(x)) {}

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator=(bool x) {
            contents = from_bool(x);
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(const Vector_mask& rhs) {
            contents = _kand_mask16(contents, rhs.contents);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(const Vector_mask& rhs) {
            contents = _kor_mask16(contents, rhs.contents);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(const Vector_mask& rhs) {
            contents = _kxor_mask16(contents, rhs.contents);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator~() const {
            return Vector_mask{_knot_mask16(contents)};
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            return Vector_mask{_kand_mask16(contents, rhs.contents)};
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            return Vector_mask{_kor_mask16(contents, rhs.contents)};
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            return Vector_mask{_kxor_mask16(contents, rhs.contents)};
        }

        //=================================================
        // Accessor
        //=================================================

        AVEL_FINL bool operator[](int i) const {
            int m = _mm512_mask2int(contents);
            return m & (1 << i);
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive contents;

        //=================================================
        // Helper functions
        //=================================================

        AVEL_FINL static primitive from_bool(bool x) {
            static const primitive full_masks[2] {
                _mm512_int2mask(0),
                _mm512_int2mask(-1)
            };

            return full_masks[x];
        }
    };



    template<>
    class alignas(64) Vector<float, 16> {
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

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const scalar_type x):
            content(_mm512_set1_ps(x)) {}

        AVEL_FINL explicit Vector(const scalar_type* x):
            content(_mm512_loadu_ps(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& a):
            content(_mm512_loadu_ps(a.data())) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            return Vector{_mm512_setzero_ps()};
        }

        AVEL_FINL static Vector ones() {
            __m512i ones = _mm512_set1_epi64(-1);
            return Vector{_mm512_castsi512_ps(ones)};
        }

        //=================================================
        // Assignment operators
        //=================================================

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        AVEL_FINL Vector& operator=(primitive p) {
            this->content = p;
            return *this;
        }

        AVEL_FINL Vector& operator=(scalar_type x) {
            content = _mm512_set1_ps(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_EQ_OQ)};
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_NEQ_OQ)};
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LT_OQ)};
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LE_OQ)};
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GT_OQ)};
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GE_OQ)};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            return zeros() - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector vec) {
            content = _mm512_add_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm512_sub_ps (content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm512_mul_ps (content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            content = _mm512_div_ps (content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector vec);

        //=================================================
        // Arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+(Vector vec) {
            return Vector{_mm512_add_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator-(Vector vec) {
            return Vector{_mm512_sub_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator*(Vector vec) {
            return Vector{_mm512_mul_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator/(Vector vec) {
            return Vector{_mm512_div_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator%(Vector vec);

        //=================================================
        // Increment/Decrement operators
        //=================================================

        AVEL_FINL Vector& operator++() {
            *this += Vector{1.0f};
            return *this;
        }

        AVEL_FINL Vector operator++(int) {
            auto temp = *this;
            *this += Vector{1.0f};
            return temp;
        }

        AVEL_FINL Vector& operator--() {
            *this -= Vector{1.0f};
            return *this;
        }

        AVEL_FINL Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{1.0f};
            return temp;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector operator&=(const Vector vec) {
            content = _mm512_and_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector operator|=(const Vector vec) {
            content = _mm512_or_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector operator^=(const Vector vec) {
            content = _mm512_xor_ps(content, vec.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() {
            return Vector{_mm512_andnot_ps(ones().content, content)};
        }

        AVEL_FINL Vector operator&(const Vector vec) {
            return Vector{_mm512_and_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator|(const Vector vec) {
            return Vector{_mm512_or_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator^(const Vector vec) {
            return Vector{_mm512_xor_ps(content, vec.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar_type, width> as_array() const {
            alignas(16) std::array<scalar_type, width> array{};

            _mm512_store_ps(array.data(), content);

            return array;
        }

        AVEL_FINL operator primitive() {
            return content;
        }

        AVEL_FINL explicit operator mask() {
            return *this == zeros();
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    AVEL_FINL vec16x32f sqrt(vec16x32f v) {
        return vec16x32f{_mm512_sqrt_ps(v)};
    }

}
