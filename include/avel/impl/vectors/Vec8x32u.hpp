namespace avel {

    using vec8x32u = Vector<std::uint32_t, 8>;

    template<>
    class Vector_mask<std::uint32_t, 8> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<std::uint32_t, 8>::type;

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

        //=================================================
        // Conversion operators
        //=================================================

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
    class Vector<std::uint32_t, 8> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using scalar_type = std::uint32_t;

        constexpr static unsigned width = 8;

        using primitive = avel::vector_primitive<scalar_type, width>::type;

        using mask = Vector_mask<scalar_type, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar_type, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(Vector<std::int32_t, width> v);

        AVEL_FINL explicit Vector(Vector<float, width> v);

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm256_mask_blend_epi32(m, _mm256_setzero_si256(), _mm256_set1_epi32(1))) {}
        #else
            content(_mm256_sub_epi32(_mm256_setzero_si256(), m)) {}
        #endif

        /*
        AVEL_FINL explicit Vector(
            std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
            std::uint32_t e, std::uint32_t f, std::uint32_t g, std::uint32_t h):
            content(_mm256_set_epi32(h, g, f, e, d, c, b, a)) {}
        */

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        #if defined(AVEL_AVX2)
        AVEL_FINL explicit Vector(const scalar_type x):
            content(_mm256_set1_epi32(x)) {}
        #endif

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
            alignas(32) static const std::uint64_t ones[4] {
                0xFFFFFFFFFFFFFFFF,
                0xFFFFFFFFFFFFFFFF,
                0xFFFFFFFFFFFFFFFF,
                0xFFFFFFFFFFFFFFFF
            };

            return Vector{_mm256_load_si256(reinterpret_cast<const __m256i*>(ones))};
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

        #if defined(AVEL_AVX2)
        AVEL_FINL Vector& operator=(scalar_type x) {
            content = _mm256_set1_epi32(x);
            return *this;
        }
        #endif

        //=================================================
        // Comparison operators
        //=================================================

        #if defined(AVEL_AVX2)
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
        #endif

        //=================================================
        // Unary arithmetic operators
        //=================================================

        #if defined(AVEL_AVX2)
        AVEL_FINL Vector operator+() {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            return zeros() - *this;
        }
        #endif

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        #if defined(AVEL_AVX2)
        AVEL_FINL Vector& operator+=(Vector vec) {
            content = _mm256_add_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm256_sub_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm256_mul_epu32(content, vec.content);
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
        #endif

        //=================================================
        // Arithmetic operators
        //=================================================

        #if defined(AVEL_AVX2)
        AVEL_FINL Vector operator+(const Vector vec) const {
            return Vector{_mm256_add_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator-(const Vector vec) const {
            return Vector{_mm256_sub_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator*(const Vector vec) const {
            return Vector{_mm256_mul_epu32(content, vec.content)};
        }

        AVEL_FINL Vector operator/(const Vector vec) const {
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
        #endif

        //=================================================
        // Increment/Decrement operators
        //=================================================

        #if defined(AVEL_AVX2)
        AVEL_FINL Vector& operator++() {
            #if defined(AVEL_AVX2)
            *this += Vector{1};
            return *this;
            #endif
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
        #endif

        //=================================================
        // Bitwise assignment operators
        //=================================================

        #if defined(AVEL_AVX2)
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

        AVEL_FINL Vector& operator<<=(std::uint64_t s) {
            content = _mm256_sll_epi32(content, _mm_loadu_si64(&s));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint64_t s) {
            content = _mm256_srl_epi32(content, _mm_loadu_si64(&s));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector s) {
            content = _mm256_sllv_epi32(content, s.content);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector s) {
            content = _mm256_srlv_epi32(content, s.content);
            return *this;
        }
        #endif

        //=================================================
        // Bitwise operators
        //=================================================

        #if defined(AVEL_AVX2)
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

        AVEL_FINL Vector operator<<(std::uint32_t s) const {
            return Vector{_mm256_sll_epi32(content, _mm_loadu_si32(&s))};
        }

        AVEL_FINL Vector operator>>(std::uint32_t s) const {
            return Vector{_mm256_srl_epi32(content, _mm_loadu_si32(&s))};
        }

        AVEL_FINL Vector operator<<(Vector vec) const {
            return Vector{_mm256_sllv_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator>>(Vector vec) const {
            return Vector{_mm256_srlv_epi32(content, vec.content)};
        }
        #endif

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

        #if defined(AVEL_AVX2)
        AVEL_FINL explicit operator mask() const {
            return *this != zeros();
        }
        #endif

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    //=====================================================
    // General vector operations
    //=====================================================

    #if defined(AVEL_AVX2)
    AVEL_FINL vec8x32u blend(vec8x32u a, vec8x32u b, vec8x32u::mask m) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_mask_blend_epi32(
            m,
            static_cast<__m256i>(a),
            static_cast<__m256i>(b)
            )
        };
        #elif defined(AVEL_AVX2)
        return vec8x32u{_mm256_blendv_epi8(a, b, m)};
        #endif
    }

    AVEL_FINL vec8x32u max(vec8x32u a, vec8x32u b) {
        return vec8x32u{_mm256_max_epu32(a, b)};
    }

    AVEL_FINL vec8x32u min(vec8x32u a, vec8x32u b) {
        return vec8x32u{_mm256_min_epu32(a, b)};
    }

    AVEL_FINL vec8x32u midpoint(vec8x32u a, vec8x32u b) {
        return (a & b) + ((a ^ b) >> 1);
    }

    template<>
    AVEL_FINL vec8x32u load<vec8x32u>(const std::uint32_t* ptr) {
        return vec8x32u{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    template<>
    AVEL_FINL vec8x32u aligned_load<vec8x32u>(const std::uint32_t* ptr) {
        return vec8x32u{_mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    template<>
    AVEL_FINL vec8x32u stream_load<vec8x32u>(const std::uint32_t* ptr) {
        return vec8x32u{_mm256_stream_load_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    //Definition of gather deferred until vector of signed integers is defined

    AVEL_FINL void store(std::uint32_t* ptr, vec8x32u v) {
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), v);
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec8x32u v) {
        _mm256_store_si256(reinterpret_cast<__m256i*>(ptr), v);
    }

    AVEL_FINL void stream_store(std::uint32_t* ptr, vec8x32u v) {
        _mm256_stream_si256(reinterpret_cast<__m256i*>(ptr), v);
    }

    //Definition of scatter deferred until vector of signed integers is defined

    //=====================================================
    // Bit operations
    //=====================================================

    vec8x32u popcount(vec8x32u v) {
        #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512VPOPCNTDQ)
        return vec8x32u{_mm256_popcnt_epi32(v)};
        #elif defined(AVELAVX512VL) & defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm256_popcnt_epi16(v);
        auto tmp1 = _mm256_slli_epi32(tmp0, 16);

        auto tmp2 = _mm256_add_epi32(tmp0, tmp1);

        return vec8x32u{_mm256_srli_epi32(tmp2, 16)};
        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec8x32u{0x55555555});                    // reuse input as temporary
        v = (v & vec8x32u{0x33333333}) + ((v >> 2) & vec8x32u{0x33333333});     // temp
        v = ((v + (v >> 4) & vec8x32u{0xF0F0F0F}) * vec8x32u{0x1010101}) >> 24; // count
        return v;

        #endif
    }

    template<int S>
    AVEL_FINL vec8x32u rotl(vec8x32u v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rol_epi32(v, S)};
        #else
        return (v << S) | (v >> (32 - S));
        #endif
    }

    AVEL_FINL vec8x32u rotl(vec8x32u v, vec8x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rolv_epi32(v, s)};
        #else
        return (v << s) | (v >> (vec8x32u{32} - s));
        #endif
    }

    template<int S>
    AVEL_FINL vec8x32u rotr(vec8x32u v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_ror_epi32(v, S)};
        #else
        return (v >> S) | (v << (32 - S));
        #endif
    }

    AVEL_FINL vec8x32u rotr(vec8x32u v, vec8x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rorv_epi32(v, s)};
        #else
        return (v >> s) | (v << (vec8x32u{32} - s));
        #endif
    }

    #endif
}
