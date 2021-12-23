namespace avel {

    using vec4x32f = Vector<float, 4>;

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

        AVEL_FINL explicit Vector_mask(const primitive content):
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
            content = _mm_and_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            content = _mm_or_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            content = _mm_xor_ps(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator~() const {
            return Vector_mask{_mm_sub_ps(_mm_setzero_ps(), content)};
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            return Vector_mask{_mm_and_ps(content, rhs.content)};
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            return Vector_mask{_mm_or_ps(content, rhs.content)};
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            return Vector_mask{_mm_xor_ps(content, rhs.content)};
        }

        //=================================================
        // Accessor
        //=================================================

        AVEL_FINL bool operator[](int i) {
            int mask = _mm_movemask_ps(content);
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

        AVEL_FINL static primitive from_bool(bool x) {
            static const primitive full_masks[2] {
                {0,  0},
                {-1, -1}
            };

            return full_masks[x];
        }

    };


    template<>
    class alignas(16) Vector<float, 4> {
    public:

        //=================================================
        // Type aliases
        //=================================================

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

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar_type x):
            content(_mm_set1_ps(x)) {}

        AVEL_FINL explicit Vector(const scalar_type* x):
            content(_mm_loadu_ps(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& a):
            content(_mm_loadu_ps(a.data())) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            return Vector{_mm_setzero_ps()};
        }

        AVEL_FINL static Vector ones() {
            const primitive zeroes = _mm_setzero_ps();
            return Vector{_mm_cmpeq_ps(zeroes, zeroes)};
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
            content = _mm_set1_ps(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(const Vector vec) const {
            return mask{_mm_cmpeq_ps(content, vec.content)};
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            return mask{_mm_cmpneq_ps(content, vec.content)};
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            return mask{_mm_cmplt_ps(content, vec.content)};
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            return mask{_mm_cmple_ps(content, vec.content)};
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            return mask{_mm_cmpgt_ps(content, vec.content)};
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            return mask{_mm_cmpge_ps(content, vec.content)};
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
            content = _mm_add_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm_sub_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm_mul_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            content = _mm_div_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec);

        //=================================================
        // Arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+(const Vector vec) const {
            return Vector{_mm_add_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator-(const Vector vec) const {
            return Vector{_mm_sub_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator*(const Vector vec) const {
            return Vector{_mm_mul_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator/(const Vector vec) const {
            return Vector{_mm_div_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator%(const Vector vec) const;

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

        AVEL_FINL Vector& operator&=(Vector vec) {
            content = _mm_and_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm_or_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm_xor_ps(content, vec.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            return Vector{_mm_andnot_ps(content, ones().content)};
        }

        AVEL_FINL Vector operator&(Vector vec) const {
            return Vector{_mm_and_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator|(Vector vec) const {
            return Vector{_mm_or_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator^(Vector vec) const {
            return Vector{_mm_xor_ps(content, vec.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar_type, width> as_array() const {
            alignas(16) std::array<scalar_type, width> array{};

            _mm_store_ps(array.data(), content);

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

    vec4x32f frexp(vec4x32f x, vec4x32i* y); //TODO: Implement

    vec4x32f floor(vec4x32f x) {
        #if defined(AVELSSE41)
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
        #else

        alignas(alignof(vec4x32f)) auto arr = x.as_array();

        for (int i = 0; i < vec4x32f::width; ++i) {
            arr[0] = std::floor(arr[1]);
        }

        return vec4x32f{arr.data()};

        #endif
    }

    vec4x32f ceil(vec4x32f x) {
        #if defined(AVELSSE41)
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
        #else

        alignas(alignof(vec4x32f)) auto arr = x.as_array();

        for (int i = 0; i < vec4x32f::width; ++i) {
            arr[0] = std::ceil(arr[1]);
        }

        return vec4x32f{arr.data()};

        #endif
    }

    vec4x32f trunc(vec4x32f x) {
        #if defined(AVEL_SSE41)
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
        #else

        alignas(alignof(vec4x32f)) auto arr = x.as_array();

        for (int i = 0; i < vec4x32f::width; ++i) {
            arr[0] = std::trunc(arr[1]);
        }

        return vec4x32f{arr.data()};

        /*
        //TODO: Test implementation

        __m128i exponents = _mm_castps_si128(x);
        exponents = _mm_slli_epi32(exponents, 1);
        exponents = _mm_srai_epi32(exponents, 24);

        static const std::uint8_t special_mask[16] {
            255,000, 000, 000,
            255,000, 000, 000,
            255,000, 000, 000,
            255,000, 000, 000
        };
        __m128i special = _mm_load_si128(reinterpret_cast<const __m128i*>(special_mask));
        __m128i is_special = _mm_cmpeq_epi32(exponents, special);

        static const std::uint8_t hi_data[16] {
            150,000, 000, 000,
            150,000, 000, 000,
            150,000, 000, 000,
            150,000, 000, 000
        };
        __m128i hi = _mm_load_si128(reinterpret_cast<const __m128i*>(hi_data));

        __m128i full = vec4x32i::ones();

        // Number fo mantissa bits that should be masked out
        __m128i shift_amounts = _mm_subs_epu8(hi, exponents);


        auto tmp1 = vec4x32i{hi}.as_array();
        auto tmp2 = vec4x32i{exponents}.as_array();
        auto tmp3 = vec4x32i{shift_amounts}.as_array();

        __m128i mantissa_mask = full;
        mantissa_mask = _mm_sll_epi32(mantissa_mask, shift_amounts); //TODO: This shift instruction does not take per-lane shift counts
        auto tmp4 = vec4x32i{mantissa_mask}.as_array();
        mantissa_mask = _mm_andnot_si128(mantissa_mask, full);
        auto tmp5 = vec4x32i{mantissa_mask}.as_array();

        __m128i ret = _mm_castps_si128(x);
        ret = _mm_and_si128(mantissa_mask, shift_amounts);


        //Create mask for exponent

        static const std::uint8_t lo_data[16] {
            127,000, 000, 000,
            127,000, 000, 000,
            127,000, 000, 000,
            127,000, 000, 000
        };
        __m128i lo = _mm_load_si128(reinterpret_cast<const __m128i*>(lo_data));

        __m128i exponent_mask = full;
        exponent_mask = _mm_cmplt_epi32(exponents, lo);
        exponent_mask = _mm_slli_epi32(exponent_mask, 23);
        exponent_mask = _mm_andnot_si128(exponent_mask, full);

        ret = _mm_and_si128(ret, exponent_mask);

        // Overwrite with original values if exponent was 255
        // Handles NaNs and infinities

        return vec4x32f{
            _mm_castsi128_ps(
            _mm_or_si128(
                    _mm_and_si128(ret, _mm_andnot_si128(is_special, full)),
                    _mm_and_si128(_mm_castps_si128(x), is_special)
                )
            )
        };
        */

        #endif
    }

    vec4x32f round(vec4x32f x) {
        #if defined(AVELSSE41)
        //TODO: Verify behavior matches with std::round for values ending in .5
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
        #else

        alignas(alignof(vec4x32f)) auto arr = x.as_array();

        for (int i = 0; i < vec4x32f::width; ++i) {
            arr[0] = std::round(arr[1]);
        }

        return vec4x32f{arr.data()};

        #endif
    }

    /*
    vec4x32f fmod(vec4x32f x, vec4x32f y) {
        return x % y;
    }
    */

    AVEL_FINL vec4x32f sqrt(vec4x32f v) {
        return vec4x32f{_mm_sqrt_ps(v)};
    }

}
