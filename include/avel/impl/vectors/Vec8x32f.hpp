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

        explicit Vector_mask(bool x):
            content(from_bool(x)) {}

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector_mask& operator=(bool x) {
            content = from_bool(x);
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Bitwise assignment operators
        //=================================================

        Vector_mask& operator&=(const Vector_mask& rhs) {
            content = _mm256_and_ps(content, rhs.content);
            return *this;
        }

        Vector_mask& operator|=(const Vector_mask& rhs) {
            content = _mm256_or_ps(content, rhs.content);
            return *this;
        }

        Vector_mask& operator^=(const Vector_mask& rhs) {
            content = _mm256_xor_ps(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector_mask operator~() const {
            return Vector_mask{_mm256_sub_ps(_mm256_setzero_ps(), content)};
        }

        Vector_mask operator&(Vector_mask rhs) const {
            return Vector_mask{_mm256_and_ps(content, rhs.content)};
        }

        Vector_mask operator|(Vector_mask rhs) const {
            return Vector_mask{_mm256_or_ps(content, rhs.content)};
        }

        Vector_mask operator^(Vector_mask rhs) const {
            return Vector_mask{_mm256_xor_ps(content, rhs.content)};
        }

        //=================================================
        // Accessor
        //=================================================

        bool operator[](int i) {
            int mask = _mm256_movemask_ps(content);
            return mask & (1 << i);
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

        //=================================================
        // Helper functions
        //=================================================

        static primitive from_bool(bool x) {
            static const primitive full_masks[2] {
                {0, 0},
                {-1, -1}
            };

            return full_masks[x];
        }

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

        Vector& operator=(primitive p) {
            this->content = p;
            return *this;
        }

        Vector& operator=(scalar_type x) {
            content = _mm256_set1_ps(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        mask operator==(Vector vec) const {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_EQ_OQ)};
        }

        mask operator!=(Vector vec) const {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_NEQ_OQ)};
        }

        mask operator<(Vector vec) const {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_LT_OQ)};
        }

        mask operator<=(Vector vec) const {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_LE_OQ)};
        }

        mask operator>(Vector vec) const {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_GT_OQ)};
        }

        mask operator>=(Vector vec) const {
            return mask{_mm256_cmp_ps(content, vec.content, _CMP_GE_OQ)};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        Vector operator+() {
            return *this;
        }

        Vector operator-() const {
            return zeros() - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        Vector& operator+=(Vector vec) {
            content = _mm256_add_ps(content, vec.content);
            return *this;
        }

        Vector& operator-=(Vector vec) {
            content = _mm256_sub_ps (content, vec.content);
            return *this;
        }

        Vector& operator*=(Vector vec) {
            content = _mm256_mul_ps (content, vec.content);
            return *this;
        }

        Vector& operator/=(Vector vec) {
            content = _mm256_div_ps (content, vec.content);
            return *this;
        }

        Vector& operator%=(Vector vec);

        //=================================================
        // Arithmetic operators
        //=================================================

        Vector operator+(Vector vec) {
            return Vector{_mm256_add_ps(content, vec.content)};
        }

        Vector operator-(Vector vec) {
            return Vector{_mm256_sub_ps(content, vec.content)};
        }

        Vector operator*(Vector vec) {
            return Vector{_mm256_mul_ps(content, vec.content)};
        }

        Vector operator/(Vector vec) {
            return Vector{_mm256_div_ps(content, vec.content)};
        }

        Vector operator%(Vector vec);

        //=================================================
        // Increment/Decrement operators
        //=================================================

        Vector& operator++() {
            *this += Vector{1.0f};
            return *this;
        }

        Vector operator++(int) {
            auto temp = *this;
            *this += Vector{1.0f};
            return temp;
        }

        Vector& operator--() {
            *this -= Vector{1.0f};
            return *this;
        }

        Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{1.0f};
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

        Vector operator~() const {
            return Vector{_mm256_andnot_ps(ones().content, content)};
        }

        Vector operator&(const Vector vec) const {
            return Vector{_mm256_and_ps(content, vec.content)};
        }

        Vector operator|(const Vector vec) const {
            return Vector{_mm256_or_ps(content, vec.content)};
        }

        Vector operator^(const Vector vec) const {
            return Vector{_mm256_xor_ps(content, vec.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        std::array<scalar_type, width> as_array() const {
            alignas(16) std::array<scalar_type, width> array{};

            _mm256_store_ps(array.data(), content);

            return array;
        }

        operator primitive() const {
            return content;
        }

        explicit operator mask() const {
            return *this == zeros();
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    /*
    vec8x32f fmod(vec8x32f x, vec8x32f y) {
        return x % y;
    }
    */

    vec8x32f sqrt(vec8x32f v) {
        return vec8x32f{_mm256_sqrt_ps(v)};
    }

}
