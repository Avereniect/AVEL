namespace avel {

    using vec4x32i = Vector<std::int32_t, 4>;

    template<>
    class alignas(16) Vector_mask<std::int32_t, 4> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<std::int32_t, 4>::type;

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

        Vector_mask& operator&=(Vector_mask rhs) {
            content = _mm_and_si128(content, rhs.content);
            return *this;
        }

        Vector_mask& operator|=(Vector_mask rhs) {
            content = _mm_or_si128(content, rhs.content);
            return *this;
        }

        Vector_mask& operator^=(Vector_mask rhs) {
            content = _mm_xor_si128(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector_mask operator~() const {
            __m128i tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};
        }

        Vector_mask operator&(Vector_mask rhs) const {
            return Vector_mask{_mm_and_si128(content, rhs.content)};
        }

        Vector_mask operator|(Vector_mask rhs) const {
            return Vector_mask{_mm_or_si128(content, rhs.content)};
        }

        Vector_mask operator^(Vector_mask rhs) const {
            return Vector_mask{_mm_xor_si128(content, rhs.content)};
        }

        //=================================================
        // Accessor
        //=================================================

        bool operator[](int i) {
            int mask = _mm_movemask_epi8(content);
            //TODO: Check correctness
            return mask & (1 << (4 * i));
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
                {0,  0},
                {-1, -1}
            };

            return full_masks[x];
        }

    };


    template<>
    class Vector<std::int32_t, 4> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using scalar_type = std::int32_t;

        using primitive = avel::vector_primitive<std::int32_t, 4>::type;

        constexpr static unsigned width = 4;

        using mask = Vector_mask<std::int32_t, 4>;

        template<class U>
        using rebind_type = Vector<U, 4>;

        template<int M>
        using rebind_width = Vector<std::int32_t, M>;

        //=================================================
        // Constructors
        //=================================================

        explicit Vector(const primitive content):
            content(content) {}

        explicit Vector(const scalar_type x):
            content(_mm_set1_epi32(x)) {}

        explicit Vector(const scalar_type* x):
            content(_mm_loadu_si128(reinterpret_cast<const __m128i*>(x))) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        static Vector zeros() {
            return Vector{_mm_setzero_si128()};
        }

        static Vector ones() {
            const primitive zeroes = _mm_setzero_si128();
            return Vector{_mm_cmpeq_epi32(zeroes, zeroes)};
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
            content = _mm_set1_epi32(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        mask operator==(const Vector vec) const {
            return mask{_mm_cmpeq_epi32(content, vec.content)};
        }

        mask operator!=(const Vector vec) const {
            return ~(*this == vec);
        }

        mask operator<(const Vector vec) const {
            return mask{_mm_cmplt_epi32(content, vec.content)};
        }

        mask operator<=(const Vector vec) const {
            return ~mask{*this > vec};
        }

        mask operator>(const Vector vec) const {
            return mask{_mm_cmpgt_epi32(content, vec.content)};
        }

        mask operator>=(const Vector vec) const {
            return ~mask{*this < vec};
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
            content = _mm_add_epi32(content, vec.content);
            return *this;
        }

        Vector& operator-=(Vector vec) {
            content = _mm_sub_epi32(content, vec.content);
            return *this;
        }

        Vector& operator*=(Vector vec) {
            content = _mm_mul_epi32(content, vec.content);
            return *this;
        }

        Vector& operator/=(Vector vec) {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<__m128i*>(array0), content);
            _mm_store_si128(reinterpret_cast<__m128i*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            content = _mm_load_si128(reinterpret_cast<const __m128i*>(array0));

            return *this;
        }

        Vector& operator%=(const Vector vec) {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<__m128i*>(array0), content);
            _mm_store_si128(reinterpret_cast<__m128i*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            content = _mm_load_si128(reinterpret_cast<const __m128i*>(array0));

            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        Vector operator+(const Vector vec) const {
            return Vector{_mm_add_epi32(content, vec.content)};
        }

        Vector operator-(const Vector vec) const {
            return Vector{_mm_sub_epi32(content, vec.content)};
        }

        Vector operator*(const Vector vec) const {
            return Vector{_mm_mul_epi32(content, vec.content)};
        }

        Vector operator/(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<__m128i*>(array0), content);
            _mm_store_si128(reinterpret_cast<__m128i*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            return Vector{_mm_load_si128(reinterpret_cast<const __m128i*>(array0))};
        }

        Vector operator%(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<__m128i*>(array0), content);
            _mm_store_si128(reinterpret_cast<__m128i*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            return Vector{_mm_load_si128(reinterpret_cast<const __m128i*>(array0))};
        }

        //=================================================
        // Increment/Decrement operators
        //=================================================

        Vector& operator++() {
            *this += Vector{1};
            return *this;
        }

        Vector operator++(int) {
            auto temp = *this;
            *this += Vector{1};
            return temp;
        }

        Vector& operator--() {
            *this -= Vector{1};
            return *this;
        }

        Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{1};
            return temp;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        Vector& operator&=(Vector vec) {
            content = _mm_and_si128(content, vec.content);
            return *this;
        }

        Vector& operator|=(Vector vec) {
            content = _mm_or_si128(content, vec.content);
            return *this;
        }

        Vector& operator^=(Vector vec) {
            content = _mm_xor_si128(content, vec.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector operator~() const {
            return Vector{_mm_andnot_si128(content, ones().content)};
        }

        Vector operator&(Vector vec) const {
            return Vector{_mm_and_si128(content, vec.content)};
        }

        Vector operator|(Vector vec) const {
            return Vector{_mm_or_si128(content, vec.content)};
        }

        Vector operator^(Vector vec) const {
            return Vector{_mm_xor_si128(content, vec.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        std::array<scalar_type, width> as_array() const {
            alignas(16) std::array<scalar_type, width> array{};

            _mm_store_si128(reinterpret_cast<__m128i*>(array.data()), content);

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

}
