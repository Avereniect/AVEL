namespace avel {

    using vec16x32i = Vector<std::int32_t, 16>;

    template<>
    class alignas(64) Vector_mask<std::int32_t, 16> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<std::int32_t, 16>::type;

        //=================================================
        // Constructor
        //=================================================

        explicit Vector_mask(primitive content):
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
            content = _kand_mask16(content, rhs.content);
            return *this;
        }

        Vector_mask& operator|=(Vector_mask rhs) {
            content = _kor_mask16(content, rhs.content);
            return *this;
        }

        Vector_mask& operator^=(Vector_mask rhs) {
            content = _kxor_mask16(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector_mask operator~() const {
            return Vector_mask{_knot_mask16(content)};
        }

        Vector_mask operator&(Vector_mask rhs) const {
            return Vector_mask{_kand_mask16(content, rhs.content)};
        }

        Vector_mask operator|(Vector_mask rhs) const {
            return Vector_mask{_kor_mask16(content, rhs.content)};
        }

        Vector_mask operator^(Vector_mask rhs) const {
            return Vector_mask{_kxor_mask16(content, rhs.content)};
        }

        //=================================================
        // Accessor
        //=================================================

        bool operator[](int i) const {
            return _cvtmask16_u32(content) & (1 << i);
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
                _cvtu32_mask16(0),
                _cvtu32_mask16(-1)
            };

            return full_masks[x];
        }

    };


    template<>
    class alignas(64) Vector<std::int32_t, 16> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using scalar_type = std::int32_t;

        using primitive = avel::vector_primitive<std::int32_t, 16>::type;

        constexpr static unsigned width = 16;

        using mask = Vector_mask<std::int32_t, 16>;

        template<class U>
        using rebind_type = Vector<U, 16>;

        template<int M>
        using rebind_width = Vector<std::int32_t, M>;

        //=================================================
        // Constructors
        //=================================================

        explicit Vector(const primitive content):
            content(content) {}

        explicit Vector(const scalar_type x):
            content(_mm512_set1_epi32(x)) {}

        explicit Vector(const scalar_type* x):
            content(_mm512_loadu_si512(reinterpret_cast<const __m128i*>(x))) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        static Vector zeros() {
            return Vector{_mm512_setzero_si512()};
        }

        static Vector ones() {
            const primitive zeroes = _mm512_setzero_si512();
            primitive reg = _mm512_undefined_si512();
            return Vector{_mm512_ternarylogic_epi32(reg, reg, reg, 0xFF)};
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
            content = _mm512_set1_epi32(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        mask operator==(const Vector vec) const {
            return mask{_mm512_cmpeq_epi32_mask(content, vec.content)};
        }

        mask operator!=(const Vector vec) const {
            return ~(*this == vec);
        }

        mask operator<(const Vector vec) const {
            return mask{_mm512_cmplt_epi32_mask(content, vec.content)};
        }

        mask operator<=(const Vector vec) const {
            return ~mask{*this > vec};
        }

        mask operator>(const Vector vec) const {
            return mask{_mm512_cmpgt_epi32_mask(content, vec.content)};
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
            content = _mm512_add_epi32(content, vec.content);
            return *this;
        }

        Vector& operator-=(Vector vec) {
            content = _mm512_sub_epi32(content, vec.content);
            return *this;
        }

        Vector& operator*=(Vector vec) {
            content = _mm512_mul_epi32(content, vec.content);
            return *this;
        }

        Vector& operator/=(Vector vec) {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm512_store_si512(reinterpret_cast<primitive*>(array0), content);
            _mm512_store_si512(reinterpret_cast<__m512*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            content = _mm512_load_si512(reinterpret_cast<const __m512*>(array0));

            return *this;
        }

        Vector& operator%=(const Vector vec) {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm512_store_si512(reinterpret_cast<primitive*>(array0), content);
            _mm512_store_si512(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            content = _mm512_load_si512(reinterpret_cast<const primitive*>(array0));

            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        Vector operator+(const Vector vec) const {
            return Vector{_mm512_add_epi32(content, vec.content)};
        }

        Vector operator-(const Vector vec) const {
            return Vector{_mm512_sub_epi32(content, vec.content)};
        }

        Vector operator*(const Vector vec) const {
            return Vector{_mm512_mul_epi32(content, vec.content)};
        }

        Vector operator/(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm512_store_si512(reinterpret_cast<primitive*>(array0), content);
            _mm512_store_si512(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            return Vector{_mm512_load_si512(reinterpret_cast<const primitive*>(array0))};
        }

        Vector operator%(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm512_store_si512(reinterpret_cast<primitive*>(array0), content);
            _mm512_store_si512(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            return Vector{_mm512_load_si512(reinterpret_cast<const primitive*>(array0))};
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
            content = _mm512_and_si512(content, vec.content);
            return *this;
        }

        Vector& operator|=(Vector vec) {
            content = _mm512_or_si512(content, vec.content);
            return *this;
        }

        Vector& operator^=(Vector vec) {
            content = _mm512_xor_si512(content, vec.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector operator~() const {
            return Vector{_mm512_andnot_si512(content, ones().content)};
        }

        Vector operator&(Vector vec) const {
            return Vector{_mm512_and_si512(content, vec.content)};
        }

        Vector operator|(Vector vec) const {
            return Vector{_mm512_or_si512(content, vec.content)};
        }

        Vector operator^(Vector vec) const {
            return Vector{_mm512_xor_si512(content, vec.content)};
        }

        AVEL_FINL Vector operator<<(std::int64_t s) const {
            return Vector{_mm512_sll_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator>>(std::int64_t s) const {
            return Vector{_mm512_sra_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator<<(Vector vec) const {
            return Vector{_mm512_sllv_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator>>(Vector vec) const {
            return Vector{_mm512_srav_epi32(content, vec.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        std::array<scalar_type, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar_type, width> array{};

            _mm512_store_si512(reinterpret_cast<__m128i*>(array.data()), content);

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
