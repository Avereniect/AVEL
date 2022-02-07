namespace avel {

    using vec8x32i = Vector<std::int32_t, 8>;

    using mask8x32i = Vector_mask<std::int32_t, 8>;

    template<>
    class Vector_mask<std::int32_t, 8> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<std::int32_t, 8>::type;

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask(Vector<std::int32_t, 8> v);

        AVEL_FINL explicit Vector_mask(Vector<float, 8> v);

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

        AVEL_FINL Vector_mask& operator=(const Vector_mask&) = default;
        AVEL_FINL Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs.content);
            #else
            content = _mm256_and_si256(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs.content);
            #else
            content = _mm256_or_si256(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs.content);
            #else
            content = _mm256_xor_si256(content, rhs.content);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_knot_mask8(content)};
            #else
            primitive tmp = _mm256_undefined_si256();
            return Vector_mask{_mm256_andnot_si256(content, _mm256_cmpeq_epi32(tmp, tmp))};
            #endif
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm256_and_si256(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm256_or_si256(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm256_xor_si256(content, rhs.content)};
            #endif
        }

        //=================================================
        // Accessor
        //=================================================

        bool operator[](int i) const {
            #if defined(AVEL_AVX512VL)
            unsigned mask = _cvtmask16_u32(__mmask16(content));
            return mask & (1 << i);
            #else
            int mask = _mm256_movemask_epi8(content);
            //TODO: Check correctness
            return mask & (1 << (4 * i));
            #endif
        }

        AVEL_FINL operator primitive() const {
            return content;
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
            #if defined(AVEL_AVX512VL)
            static const primitive full_masks[2] {
                0x00,
                0xFF
            };

            return full_masks[x];
            #else
            static const primitive full_masks[2] {
                {0,  0, 0, 0},
                {-1, -1, -1, -1}
            };

            return full_masks[x];
            #endif
        }

    };


    template<>
    class Vector<std::int32_t, 8> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using scalar_type = std::int32_t;

        using primitive = avel::vector_primitive<std::int32_t, 8>::type;

        constexpr static unsigned width = 8;

        using mask = Vector_mask<std::int32_t, 8>;

        template<class U>
        using rebind_type = Vector<U, 8>;

        template<int M>
        using rebind_width = Vector<std::int32_t, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(Vector<std::uint32_t, 8> v);

        AVEL_FINL explicit Vector(Vector<float, 8> v);

        AVEL_FINL explicit Vector(
            std::int32_t a, std::int32_t b, std::int32_t c, std::int32_t d,
            std::int32_t e, std::int32_t f, std::int32_t g, std::int32_t h):
            content(_mm256_set_epi32(h, g, f, e, d, c, b, a)) {}

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const scalar_type x):
            content(_mm256_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const scalar_type* x):
            content(_mm256_loadu_si256(reinterpret_cast<const primitive*>(x))) {}

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& array):
            content(_mm256_loadu_si256(reinterpret_cast<const primitive*>(array.data()))) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            return Vector{_mm256_setzero_si256()};
        }

        AVEL_FINL static Vector ones() {
            const primitive zeroes = _mm256_setzero_si256();
            return Vector{_mm256_cmpeq_epi32(zeroes, zeroes)};
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
            content = _mm256_set1_epi32(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpeq_epi32_mask(content, vec.content)};
            #else
            return mask{_mm256_cmpeq_epi32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpneq_epi32_mask(content, vec.content)};
            #else
            return ~(*this == vec);
            #endif
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmplt_epi32_mask(content, vec.content)};
            #else
            return mask{_mm256_cmpgt_epi32(vec.content, content)};
            #endif
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmple_epi32_mask(content, vec.content)};
            #else
            return ~mask{*this > vec};
            #endif
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpgt_epi32_mask(content, vec.content)};
            #else
            return mask{_mm256_cmpgt_epi32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpge_epi32_mask(content, vec.content)};
            #else
            return ~mask{*this < vec};
            #endif
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
            content = _mm256_add_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm256_sub_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm256_mul_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm256_store_si256(reinterpret_cast<primitive*>(array0), content);
            _mm256_store_si256(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            content = _mm256_load_si256(reinterpret_cast<const primitive*>(array0));

            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm256_store_si256(reinterpret_cast<primitive*>(array0), content);
            _mm256_store_si256(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            content = _mm256_load_si256(reinterpret_cast<const primitive*>(array0));

            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+(const Vector vec) const {
            return Vector{_mm256_add_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator-(const Vector vec) const {
            return Vector{_mm256_sub_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator*(const Vector vec) const {
            return Vector{_mm256_mul_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator/(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm256_store_si256(reinterpret_cast<primitive*>(array0), content);
            _mm256_store_si256(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            return Vector{_mm256_load_si256(reinterpret_cast<const primitive*>(array0))};
        }

        AVEL_FINL Vector operator%(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm256_store_si256(reinterpret_cast<primitive*>(array0), content);
            _mm256_store_si256(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            return Vector{_mm256_load_si256(reinterpret_cast<const primitive*>(array0))};
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
            content = _mm256_and_si256(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm256_or_si256(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm256_xor_si256(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector operator<<=(std::uint32_t s) {
            content = _mm256_sll_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector operator>>=(std::uint32_t s) {
            content = _mm256_sra_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector operator<<=(Vector<std::uint32_t, width> s) {
            content = _mm256_sllv_epi32(content, primitive(s));
            return *this;
        }

        AVEL_FINL Vector operator>>=(Vector<std::uint32_t, width> s) {
            content = _mm256_srav_epi32(content, primitive(s));
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            return Vector{_mm256_andnot_si256(content, ones().content)};
        }

        AVEL_FINL Vector operator&(Vector vec) const {
            return Vector{_mm256_and_si256(content, vec.content)};
        }

        AVEL_FINL Vector operator|(Vector vec) const {
            return Vector{_mm256_or_si256(content, vec.content)};
        }

        AVEL_FINL Vector operator^(Vector vec) const {
            return Vector{_mm256_xor_si256(content, vec.content)};
        }

        AVEL_FINL Vector operator<<(std::uint64_t s) const {
            return Vector{_mm256_sll_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator>>(std::uint64_t s) const {
            return Vector{_mm256_sra_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator<<(Vector<std::uint32_t, width> s) const {
            return Vector{_mm256_sllv_epi32(content, primitive(s))};
        }

        AVEL_FINL Vector operator>>(Vector<std::uint32_t, width> s) const {
            return Vector{_mm256_srav_epi32(content, primitive(s))};
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar_type, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar_type, width> array{};

            _mm256_store_si256(reinterpret_cast<primitive*>(array.data()), content);

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

    //=====================================================
    // Delayed definitions
    //=====================================================

    Vector<std::uint32_t, 8>::Vector(vec8x32i v):
        content(v) {}

    //=====================================================
    // General vector operations
    //=====================================================

    AVEL_FINL vec8x32i blend(vec8x32i a, vec8x32i b, mask8x32i m) {
        #if defined(AVEL_AVX512VL)
        return vec8x32i{_mm256_mask_blend_epi32(m, a, b)};
        #elif defined(AVEL_AVX2)
        return vec8x32i{_mm256_blendv_epi8(a, b, m)};
        #endif
    }

    AVEL_FINL vec8x32i max(vec8x32i a, vec8x32i b) {
        return vec8x32i{_mm256_max_epi32(a, b)};
    }

    AVEL_FINL vec8x32i min(vec8x32i a, vec8x32i b) {
        return vec8x32i{_mm256_min_epi32(a, b)};
    }

    AVEL_FINL vec8x32i abs(vec8x32i v) {
        return vec8x32i{_mm256_abs_epi32(v)};
    }

    template<>
    AVEL_FINL vec8x32i load<vec8x32i>(const std::int32_t* ptr) {
        return vec8x32i{_mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    template<>
    AVEL_FINL vec8x32i aligned_load<vec8x32i>(const std::int32_t* ptr) {
        return vec8x32i{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    template<>
    AVEL_FINL vec8x32i stream_load<vec8x32i>(const std::int32_t* ptr) {
        return vec8x32i{_mm256_stream_load_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    template<>
    AVEL_FINL vec8x32i gather<vec8x32i>(const std::int32_t* ptr, vec8x32i indices) {
        return vec8x32i{_mm256_i32gather_epi32(ptr, indices, sizeof(std::int32_t))};
    }

    AVEL_FINL void store(std::int32_t* ptr, vec8x32i v) {
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), v);
    }

    AVEL_FINL void aligned_store(std::int32_t* ptr, vec8x32i v) {
        _mm256_store_si256(reinterpret_cast<__m256i*>(ptr), v);
    }

    AVEL_FINL void stream_store(std::int32_t* ptr, vec8x32i v) {
        _mm256_stream_si256(reinterpret_cast<__m256i*>(ptr), v);
    }

    AVEL_FINL void scatter(std::int32_t* ptr, vec8x32i indices, vec8x32i v) {
        #if defined(AVEL_AVX512VL)
        _mm256_i32scatter_epi32(ptr, indices, v, sizeof(std::int32_t));
        #else
        auto i = indices.as_array();

        auto lo = _mm256_castsi256_si128(v);
        auto hi = _mm256_extracti128_si256(v, 1);

        _mm_storeu_si32(ptr + i[0], lo);
        _mm_storeu_si32(ptr + i[1], _mm_bsrli_si128(lo, 0x4));
        _mm_storeu_si32(ptr + i[2], _mm_bsrli_si128(lo, 0x8));
        _mm_storeu_si32(ptr + i[3], _mm_bsrli_si128(lo, 0xC));
        _mm_storeu_si32(ptr + i[4], hi);
        _mm_storeu_si32(ptr + i[5], _mm_bsrli_si128(hi, 0x4));
        _mm_storeu_si32(ptr + i[6], _mm_bsrli_si128(hi, 0x8));
        _mm_storeu_si32(ptr + i[7], _mm_bsrli_si128(hi, 0xC));
        #endif
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    AVEL_FINL vec8x32i popcount(vec8x32i v) {
        #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512VPOPCNTDQ)
        return vec8x32i{_mm_popcnt_epi32(x)};
        #elif defined(AVELAVX512VL) & defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm_popcnt_epi16(x);
        auto tmp1 = _mm_slli_epi32(tmp0, 16);

        auto tmp2 = _mm_add_epi32(tmp0, tmp1);

        return vec4x32u{_mm_srli_epi32(tmp2, 16)};
        #elif defined(AVEL_POPCNT) & defined(AVEL_SSE42)
        int a = _mm_extract_epi32(x, 0);
        int b = _mm_extract_epi32(x, 1);
        int c = _mm_extract_epi32(x, 2);
        int d = _mm_extract_epi32(x, 3);

        auto t0 = _mm_setzero_si128();
        auto t1 = _mm_insert_epi32(t0, _mm_popcnt_u32(a), 0);
        auto t2 = _mm_insert_epi32(t1, _mm_popcnt_u32(b), 1);
        auto t3 = _mm_insert_epi32(t2, _mm_popcnt_u32(c), 2);
        auto t4 = _mm_insert_epi32(t3, _mm_popcnt_u32(d), 3);

        return vec4x32u{t4};
        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec8x32i{0x55555555});                    // reuse input as temporary
        v = (v & vec8x32i{0x33333333}) + ((v >> 2) & vec8x32i{0x33333333});     // temp
        v = ((v + (v >> 4) & vec8x32i{0xF0F0F0F}) * vec8x32i{0x1010101}) >> 24; // count
        return v;

        #endif
    }

    template<int S>
    AVEL_FINL vec8x32i rotl(vec8x32i v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32i{_mm256_rol_epi32(v, S)};
        #else
        return (v << S) | (v >> (32 - S));
        #endif
    }

    AVEL_FINL vec8x32i rotl(vec8x32i v, vec8x32i s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32i{_mm256_rolv_epi32(v, s)};
        #else
        auto l = vec8x32u{v} << vec8x32u{s};
        auto r = vec8x32u{v} >> (vec8x32u{32} - vec8x32u{v});
        return vec8x32i{l | r};
        #endif
    }

    template<int S>
    AVEL_FINL vec8x32i rotr(vec8x32i v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32i{_mm256_ror_epi32(v, S)};
        #else
        return (v >> S) | (v << (32 - S));
        #endif
    }

    AVEL_FINL vec8x32i rotr(vec8x32i v, vec8x32i s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32i{_mm256_rorv_epi32(v, s)};
        #else
        auto l = vec8x32u{v} >> vec8x32u{s};
        auto r = vec8x32u{v} << (vec8x32u{32} - vec8x32u{v});
        return vec8x32i{l | r};
        #endif
    }

}