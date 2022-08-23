namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x32u = Vector<std::uint32_t, 8>;

    using mask8x32u = Vector_mask<std::uint32_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x32u> div(vec8x32u numerator, vec8x32u denominator);

    vec8x32u broadcast_bits(mask8x32u m);





    template<>
    class alignas(AVEL_MASK_ALIGNMENT_8X32U) Vector_mask<std::uint32_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL)
        using primitive = __mmask8;
        #elif defined(AVEL_AVX)
        using primitive = __m256i;
        #endif

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask(Vector_mask<std::int32_t, 8>);

        AVEL_FINL explicit Vector_mask(Vector_mask<float, 8>);

        AVEL_FINL explicit Vector_mask(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 8>& arr):
        #if defined(AVEL_AVX512VL)
            content() {

            int t = 0x00;
            t |= int(arr[0]) << 0;
            t |= int(arr[1]) << 1;
            t |= int(arr[2]) << 2;
            t |= int(arr[3]) << 3;
            t |= int(arr[4]) << 4;
            t |= int(arr[5]) << 5;
            t |= int(arr[6]) << 6;
            t |= int(arr[7]) << 7;

            content = _mm512_int2mask(t);
        }
        #elif defined(AVEL_AVX2)
            content() {
            static_assert(sizeof(bool) == 1);

            alignas(32) static const std::uint32_t mask_data[8] {
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000,
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000
            };

            __m128i t0 = _mm_loadu_si32(arr.data() + 0);
            __m128i t1 = _mm_loadu_si32(arr.data() + 4);


            //TODO: Implement
            primitive t2 = _mm256_inserti128_si256(
                _mm256_castsi128_si256(t0),
                t1,
                0x01
            );

            primitive mask = _mm256_load_si256((const __m256i*)mask_data);

            primitive t3 = _mm256_and_si256(t2, mask);
            content = _mm256_cmpgt_epi32(t3, _mm256_setzero_si256());
        }
        #endif

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
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return _mm512_mask2int(primitive(lhs)) == _mm512_mask2int(primitive(rhs));
            #elif defined(AVEL_SSE2)
            return (0xFFFFFFFF == _mm256_movemask_epi8(_mm256_cmpeq_epi32(lhs, rhs)));
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return _mm512_mask2int(primitive(lhs)) != _mm512_mask2int(primitive(rhs));
            #else
            return (0xFFFFFFFF != _mm256_movemask_epi8(_mm256_cmpeq_epi32(lhs, rhs)));
            #endif
        }

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

        #if defined(AVEL_AVX2)
        AVEL_FINL Vector_mask operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_knot_mask8(content)};
            #else
            primitive tmp = _mm256_undefined_si256();
            return Vector_mask{_mm256_andnot_si256(content, _mm256_cmpeq_epi32(tmp, tmp))};
            #endif
        }

        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(lhs, rhs)};
            #else
            return Vector_mask{_mm256_and_si256(lhs, rhs)};
            #endif
        }

        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(lhs, rhs)};
            #else
            return Vector_mask{_mm256_or_si256(lhs, rhs)};
            #endif
        }

        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(lhs, rhs)};
            #elif defined(AVEL_AVX2)
            return Vector_mask{_mm256_xor_si256(lhs, rhs)};
            #endif
        }
        #endif

        //=================================================
        // Accessor
        //=================================================

        #if defined(AVEL_AVX2)
        bool operator[](int i) const {
            #if defined(AVEL_AVX512VL)
            unsigned mask = _mm512_mask2int(__mmask16(content));
            return mask & (1 << i);
            #elif defined(AVEL_AVX2)
            int mask = _mm256_movemask_epi8(content);
            //TODO: Check correctness
            return mask & (1 << (4 * i));
            #endif
        }
        #endif

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

    //=====================================================
    // Mask functions
    //=====================================================

    #if defined(AVEL_AVX2)
    std::uint32_t count(mask8x32u m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(m));
        #elif defined(AVEL_AVX2)
        return pop_count(_mm256_movemask_epi8(m)) / sizeof(std::uint32_t);
        #endif
    }

    bool any(mask8x32u m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(m);
        #elif defined(AVEL_AVX2)
        return _mm256_movemask_epi8(m);
        #endif
    }

    bool all(mask8x32u m) {
        #if defined(AVEL_AVX512VL)
        return 0xFF == _mm512_mask2int(m);
        #elif defined(AVEL_AVX2)
        return 0xFFFFFFFFu == _mm256_movemask_epi8(m);
        #endif
    }
    #endif

    template<>
    class Vector<std::uint32_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint32_t;

        using primitive = __m256i;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

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
        AVEL_FINL explicit Vector(const scalar x):
            content(_mm256_set1_epi32(x)) {}
        #endif

        AVEL_FINL explicit Vector(const scalar* x):
            content(_mm256_loadu_si256(reinterpret_cast<const primitive*>(x))) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
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
        AVEL_FINL Vector& operator=(scalar x) {
            content = _mm256_set1_epi32(x);
            return *this;
        }
        #endif

        //=================================================
        // Comparison operators
        //=================================================

        #if defined(AVEL_AVX2)
        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpeq_epi32_mask(lhs, rhs)};
            #else
            return mask{_mm256_cmpeq_epi32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpneq_epi32_mask(lhs, rhs)};
            #else
            return ~(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmplt_epi32_mask(lhs, rhs)};
            #else
            return mask{_mm256_cmpgt_epi32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmple_epi32_mask(lhs, rhs)};
            #else
            return ~mask{lhs > rhs};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpgt_epi32_mask(lhs, rhs)};
            #else
            return mask{_mm256_cmpgt_epi32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpge_epi32_mask(lhs, rhs)};
            #else
            return ~mask{lhs < rhs};
            #endif
        }
        #endif

        //=================================================
        // Unary arithmetic operators
        //=================================================

        #if defined(AVEL_AVX2)
        [[nodiscard]]
        AVEL_FINL Vector operator+() {
            return *this;
        }

        [[nodiscard]]
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
            content = _mm256_mullo_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            auto results = div(*this, vec);
            *this = results.quot;
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
            auto results = div(*this, vec);
            *this = results.rem;
            return *this;
        }
        #endif

        //=================================================
        // Arithmetic operators
        //=================================================

        #if defined(AVEL_AVX2)
        [[nodiscard]]
        AVEL_FINL friend Vector operator+(Vector lhs, Vector rhs) {
            return Vector{_mm256_add_epi32(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator-(Vector lhs, Vector rhs) {
            return Vector{_mm256_sub_epi32(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator*(Vector lhs, Vector rhs) {
            return Vector{_mm256_mullo_epi32(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator/(Vector lhs, Vector rhs) {
            auto results = div(lhs, rhs);
            return results.quot;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator%(Vector lhs, Vector rhs) {
            auto results = div(lhs, rhs);
            return results.rem;
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
        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            return Vector{_mm256_andnot_si256(content, ones().content)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator&(Vector lhs, Vector rhs) {
            return Vector{_mm256_and_si256(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator|(Vector lhs, Vector rhs) {
            return Vector{_mm256_or_si256(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator^(Vector lhs, Vector rhs) {
            return Vector{_mm256_xor_si256(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL Vector operator<<(std::uint32_t s) const {
            return Vector{_mm256_sll_epi32(content, _mm_loadu_si32(&s))};
        }

        [[nodiscard]]
        AVEL_FINL Vector operator>>(std::uint32_t s) const {
            return Vector{_mm256_srl_epi32(content, _mm_loadu_si32(&s))};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, Vector rhs) {
            return Vector{_mm256_sllv_epi32(lhs, rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, Vector rhs) {
            return Vector{_mm256_srlv_epi32(lhs, rhs)};
        }
        #endif

        //=================================================
        // Conversions
        //=================================================

        [[nodiscard]]
        AVEL_FINL std::array<scalar, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar, width> array{};

            _mm256_store_si256(reinterpret_cast<primitive*>(array.data()), content);

            return array;
        }

        [[nodiscard]]
        AVEL_FINL operator primitive() const {
            return content;
        }

        #if defined(AVEL_AVX2)
        [[nodiscard]]
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
    [[nodiscard]]
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

    [[nodiscard]]
    AVEL_FINL vec8x32u max(vec8x32u a, vec8x32u b) {
        return vec8x32u{_mm256_max_epu32(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u min(vec8x32u a, vec8x32u b) {
        return vec8x32u{_mm256_min_epu32(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u clamp(vec8x32u x, vec8x32u lo, vec8x32u hi) {
        return vec8x32u{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u midpoint(vec8x32u a, vec8x32u b) {
        return (a & b) + ((a ^ b) >> 1);
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u load<vec8x32u>(const std::uint32_t* ptr) {
        return vec8x32u{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u aligned_load<vec8x32u>(const std::uint32_t* ptr) {
        return vec8x32u{_mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u stream_load<vec8x32u>(const std::uint32_t* ptr) {
        return vec8x32u{_mm256_stream_load_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    //Definition of gather deferred until vector of signed integers is defined

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u broadcast<vec8x32u>(std::uint32_t x) {
        return vec8x32u{_mm256_set1_epi32(x)};
    }

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
    // Integer arithmetic functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x32u> div(vec8x32u numerator, vec8x32u denominator) {
        auto quotient = vec8x32u::zeros();

        auto z = vec8x32u::zeros();
        mask8x32u f_mask{false};

        std::uint32_t i = 32;
        for (; (i-- > 0) && ((numerator != z) != f_mask);) {
            mask8x32u b = ((numerator >> i) >= denominator);

            /*
            #if defined(AVEL_AVX512VL)
            numerator -= vec4x32u{_mm_mask_sub_epi32(numerator, b, numerator, (denominator << i))};
            #elif defined(AVEL_SSE2)
            numerator -= (vec4x32u{vec4x32u::primitive(b)} & (denominator << i));
            #endif
            */

            numerator -= (broadcast_bits(b) & (denominator << i));

            quotient |= (vec8x32u{b} << i);
        }

        return {quotient, numerator};
    }

    //=====================================================
    // Bit operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x32u broadcast_bits(mask8x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec8x32u{_mm256_movm_epi32(m)};
        #elif defined(AVEL_AVX512VL)
        const auto x = _mm512_set1_epi32(0);
        const auto y = _mm512_set1_epi32(-1);
        return vec4x32u{_mm512_mask_blend_epi32(m, x, y)};
        #elif defined(AVEL_SSE2)
        return vec8x32u{vec8x32u::primitive(m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u pop_count(vec8x32u v) {
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
    [[nodiscard]]
    AVEL_FINL vec8x32u rotl(vec8x32u v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rol_epi32(v, S)};
        #else
        return (v << S) | (v >> (32 - S));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u rotl(vec8x32u v, vec8x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rolv_epi32(v, s)};
        #else
        return (v << s) | (v >> (vec8x32u{32} - s));
        #endif
    }

    template<int S>
    [[nodiscard]]
    AVEL_FINL vec8x32u rotr(vec8x32u v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_ror_epi32(v, S)};
        #else
        return (v >> S) | (v << (32 - S));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u rotr(vec8x32u v, vec8x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rorv_epi32(v, s)};
        #else
        return (v >> s) | (v << (vec8x32u{32} - s));
        #endif
    }

    #endif
}
