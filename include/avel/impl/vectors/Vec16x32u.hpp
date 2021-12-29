namespace avel {

    using vec16x32u = Vector<std::uint32_t, 16>;

    template<>
    class alignas(64) Vector_mask<std::uint32_t, 16> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<std::uint32_t, 16>::type;

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator=(bool x) {
            content = from_bool(x);
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            content = _kand_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            content = _kor_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            content = _kxor_mask16(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator~() const {
            return Vector_mask{_knot_mask16(content)};
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            return Vector_mask{_kand_mask16(content, rhs.content)};
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            return Vector_mask{_kor_mask16(content, rhs.content)};
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
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
    class alignas(64) Vector<std::uint32_t, 16> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using scalar_type = std::uint32_t;

        using primitive = avel::vector_primitive<std::uint32_t, 16>::type;

        constexpr static unsigned width = 16;

        using mask = Vector_mask<std::uint32_t, 16>;

        template<class U>
        using rebind_type = Vector<U, 16>;

        template<int M>
        using rebind_width = Vector<std::uint32_t, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const scalar_type x):
            content(_mm512_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const scalar_type* x):
            content(_mm512_loadu_si512(reinterpret_cast<const __m128i*>(x))) {}

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& array):
            content(_mm512_loadu_si512(array.data())) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            return Vector{_mm512_setzero_si512()};
        }

        AVEL_FINL static Vector ones() {
            primitive reg = _mm512_undefined_epi32();
            return Vector{_mm512_ternarylogic_epi32(reg, reg, reg, 0xFF)};
        }

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(const Vector&) = default;
        AVEL_FINL Vector& operator=(Vector&&) = default;

        AVEL_FINL Vector& operator=(primitive p) {
            this->content = p;
            return *this;
        }

        AVEL_FINL Vector& operator=(scalar_type x) {
            content = _mm512_set1_epi32(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(const Vector vec) const {
            return mask{_mm512_cmpeq_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            return ~(*this == vec);
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            return mask{_mm512_cmplt_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            return ~mask{*this > vec};
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            return mask{_mm512_cmpgt_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            return ~mask{*this < vec};
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
            content = _mm512_add_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm512_sub_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm512_mul_epu32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
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

        AVEL_FINL Vector& operator%=(const Vector vec) {
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

        AVEL_FINL Vector operator+(const Vector vec) const {
            return Vector{_mm512_add_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator-(const Vector vec) const {
            return Vector{_mm512_sub_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator*(const Vector vec) const {
            return Vector{_mm512_mul_epu32(content, vec.content)};
        }

        AVEL_FINL Vector operator/(const Vector vec) const {
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

        AVEL_FINL Vector operator%(const Vector vec) const {
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

        AVEL_FINL Vector& operator++() {
            *this += Vector{1};
            return *this;
        }

        AVEL_FINL Vector operator++(int) {
            auto temp = *this;
            *this += Vector{1};
            return temp;
        }

        AVEL_FINL Vector& operator--() {
            *this -= Vector{1};
            return *this;
        }

        AVEL_FINL Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{1};
            return temp;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector vec) {
            content = _mm512_and_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm512_or_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm512_xor_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(std::uint32_t s) {
            content = _mm512_sll_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t s) {
            content = _mm512_srl_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector s) {
            content = _mm512_sllv_epi32(content, s.content);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector s) {
            content = _mm512_srlv_epi32(content, s.content);
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

        AVEL_FINL Vector operator<<(std::uint64_t s) const {
            return Vector{_mm512_sll_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator>>(std::uint64_t s) const {
            return Vector{_mm512_srl_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator<<(Vector s) const {
            return Vector{_mm512_sllv_epi32(content, s.content)};
        }

        AVEL_FINL Vector operator>>(Vector s) const {
            return Vector{_mm512_srlv_epi32(content, s.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar_type, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar_type, width> array{};

            _mm512_store_si512(reinterpret_cast<__m128i*>(array.data()), content);

            return array;
        }

        AVEL_FINL operator primitive() const {
            return content;
        }

        AVEL_FINL explicit operator mask() const {
            return *this == zeros();
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

}
