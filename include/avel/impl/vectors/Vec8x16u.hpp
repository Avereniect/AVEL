namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x16u = Vector<std::uint16_t, 8>;

    using mask8x16u = Vector_mask<std::uint16_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x16u> div(vec8x16u numerator, vec8x16u denominator);

    vec8x16u blend(vec8x16u a, vec8x16u b, mask8x16u m);

    template<>
    class Vector_mask<std::uint16_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 8;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE)
        using primitive = __m128i;
        #endif

        //=================================================
        // -ctors
        //=================================================

        AVEL_FINL explicit Vector_mask(Vector_mask<std::int16_t, 8>);

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}AVEL_FINL explicit Vector_mask(const std::array<bool, 4>& arr):
        #if defined(AVEL_AVX512VL)
            content() {

            //TODO: Implement
        }
        #elif defined(AVEL_SSE2)
            content() {
            static_assert(sizeof(bool) == 1);

            //TODO: Implement
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

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return (_mm512_mask2int(primitive(lhs)) == _mm512_mask2int(primitive(rhs)));
            #elif defined(AVEL_SSE2)
            return (0xFFFF == _mm_movemask_epi8(_mm_cmpeq_epi16(lhs, rhs)));
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return (_mm512_mask2int(primitive(lhs)) != _mm512_mask2int(primitive(rhs)));
            #elif defined(AVEL_SSE2)
            return (0xFFFF != _mm_movemask_epi8(_mm_cmpeq_epi16(lhs, rhs)));
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_knot_mask8(content)};
            #elif defined(AVEL_SSE2)
            primitive tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_and_si128(content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_or_si128(content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_xor_si128(content, rhs.content)};
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

        AVEL_FINL static primitive from_bool(bool x) {
            #if defined(AVEL_AVX512VL)
            static const primitive full_masks[2] {
                0x00,
                0xFF
            };

            return full_masks[x];
            #elif defined(AVEL_SSE2)
            static const primitive full_masks[2] {
                {0,  0},
                {-1, -1}
            };

            return full_masks[x];
            #endif
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask8x16u m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(m));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(m)) / sizeof(std::uint16_t);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x16u m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(m);
        #endif
    }


    [[nodiscard]]
    AVEL_FINL bool all(mask8x16u m) {
        #if defined(AVEL_AVX512VL)
        return 0xFF == _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x16u m) {
        return !all(m);
    }

    template<>
    class Vector<std::uint16_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint16_t;

        using primitive = __m128i;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // -ctors
        //=================================================

        AVEL_FINL explicit Vector(Vector<std::int16_t, 8>);

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm_mask_blend_epi16(m, _mm_setzero_si128(), _mm_set1_epi16(1))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi16(_mm_setzero_si128(), static_cast<primitive>(m))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi16(x)) {}
        #endif

        AVEL_FINL explicit Vector(const scalar* x):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(x))) {}
        #endif

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            Vector(array.data()) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        AVEL_FINL Vector& operator=(primitive p) {
            this->content = p;
            return *this;
        }

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_SSE2)
            content = _mm_set1_epi16(x);
            #endif
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL mask operator==(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi16_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi16(content, vec.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL mask operator!=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epu16_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            return ~(*this == vec);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL mask operator<(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmplt_epu16_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            auto tmp = (*this - vec);
            return mask{_mm_srai_epi16(tmp, 15)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL mask operator<=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epu16_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            return ~mask{*this > vec};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL mask operator>(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epu16_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            primitive offset = _mm_set1_epi32(-2147483648);
            return mask{_mm_cmpgt_epi32(
                _mm_add_epi32(content, offset),
                _mm_add_epi32(vec.content, offset)
            )};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL mask operator>=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epu16_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
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
            return Vector{} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi16(content, vec.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi16(content, vec.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_mullo_epi16(content, vec.content);
            #endif
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

        //=================================================
        // Arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+(const Vector vec) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_add_epi16(content, vec.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector operator-(const Vector vec) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_sub_epi16(content, vec.content)};
            #endif
        }

        [[nodiscard]]
       AVEL_FINL  Vector operator*(const Vector vec) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_mullo_epi16(content, vec.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector operator/(const Vector vec) const {
            auto results = div(*this, vec);
            return results.quot;
        }

        [[nodiscard]]
        AVEL_FINL Vector operator%(const Vector vec) const {
            auto results = div(*this, vec);
            return results.rem;
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

        AVEL_FINL Vector& operator&=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(std::uint32_t s) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi16(content, _mm_loadu_si32(&s));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t s) {
            #if defined(AVEL_SSE2)
            content = _mm_srl_epi16(content, _mm_loadu_si32(&s));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector vec) {
            #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512BW)
            content = _mm_sllv_epi16(content, vec.content);
            #elif defined(AVEL_SSE2)
            alignas(alignof(Vector)) auto v = as_array();
            auto s = vec.as_array();

            for (int i = 0; i < width; ++i) {
                if (16 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] << s[i];
                }
            }

            content = _mm_load_si128((const primitive*)v.data());
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector vec) {
            #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512BW)
            content = _mm_srlv_epi16(content, vec.content);
            #elif defined(AVEL_SSE2)
            auto v = as_array();
            auto s = vec.as_array();

            for (int i = 0; i < width; ++i) {
                if (16 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] >> s[i];
                }
            }

            content = _mm_load_si128((const primitive*)v.data());
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_SSE2)
            auto t = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi16(t, t))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator&(Vector lhs, Vector rhs) {
            #if defined(AVEL_SSE2)
            return Vector{_mm_and_si128(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector operator|(Vector rhs) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_or_si128(content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector operator^(Vector rhs) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_xor_si128(content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector operator<<(std::uint32_t s) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_sll_epi16(content, _mm_loadu_si32(&s))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector operator>>(std::uint32_t s) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_srl_epi16(content, _mm_loadu_si32(&s))};
            #endif
        }


        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX2)
            return Vector{_mm_sllv_epi16(lhs, rhs)};
            #elif defined(AVEL_SSE2)
            for (scalar i = 0; i < 4; ++i) {
                vec8x16u threshold{scalar(1u << i)};
                mask8x16u m = (rhs > threshold);
                lhs = blend(lhs, lhs << i, m);
            }

            return lhs;
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector operator>>(Vector vec) const {
            #if defined(AVEL_AVX2)
            return Vector{_mm_srlv_epi16(content, vec.content)};
            #elif defined(AVEL_SSE2)
            auto v = as_array();
            auto s = vec.as_array();

            for (int i = 0; i < width; ++i) {
                if (16 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] >> s[i];
                }
            }

            return Vector{v.data()};
            #endif
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar, width> array{};

            #if defined(AVEL_SSE2)
            _mm_store_si128(reinterpret_cast<primitive*>(array.data()), content);
            #endif

            return array;
        }

        [[nodiscard]]
        AVEL_FINL operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return *this != Vector{};
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x16u blend(vec8x16u a, vec8x16u b, mask8x16u m) {
        #if defined(AVEL_AVX512VL)
        return vec8x16u{_mm_mask_blend_epi16(
            m,
            static_cast<__m128i>(a),
            static_cast<__m128i>(b)
            )
        };
        #elif defined(AVEL_SSE41)
        return vec8x16u{_mm_blendv_epi8(a, b, m)};
        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(m, a);
        auto y = _mm_and_si128(m, b);
        return vec8x16u{_mm_or_si128(x, y)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u max(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE41)
        return vec8x16u{_mm_max_epu16(a, b)};
        #elif defined(AVEL_SSE2)
        return blend(a, b, a < b);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u min(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE41)
        return vec8x16u{_mm_min_epu16(a, b)};
        #elif defined(AVEL_SSE2)
        return blend(a, b, b < a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u clamp(vec8x16u x, vec8x16u lo, vec8x16u hi) {
        return vec8x16u{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u midpoint(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_avg_epu16(a, b)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u load<vec8x16u>(const std::uint16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u aligned_load<vec8x16u>(const std::uint16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u stream_load<vec8x16u>(const std::uint16_t* ptr) {
        #ifdef AVEL_SSE41
        //Casting away of const required by intrinsic
        return vec8x16u{_mm_stream_load_si128((__m128i*)ptr)};
        #elif defined(AVEL_SSE2)
        return aligned_load<vec8x16u>(ptr);
        #endif
    }

    AVEL_FINL void store(std::uint16_t* ptr, vec8x16u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), v);
        #endif
    }

    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec8x16u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), v);
        #endif
    }

    AVEL_FINL void stream_store(std::uint16_t* ptr, vec8x16u v) {
        #if defined(AVEL_SSE2)
        _mm_stream_si128(reinterpret_cast<__m128i*>(ptr), v);
        #endif
    }

    //=====================================================
    // Integer arithmetic functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x16u> div(vec8x16u numerator, vec8x16u denominator) {
        vec8x16u quotient{};

        auto z = vec8x16u{};
        for (std::uint32_t i = 16; (i-- > 0) && any(numerator != z);) {
            mask8x16u b = (numerator >> i) >= denominator;

            #if defined(AVEL_AVX512VL)
            numerator -= vec8x16u{_mm_mask_sub_epi16(numerator, b, numerator, (denominator << i))};
            #elif defined(AVEL_SSE2)
            numerator -= (vec8x16u{vec8x16u::primitive(b)} & (denominator << i));
            #endif

            quotient |= (vec8x16u {b} << i);
        }

        return {quotient, numerator};
    }

    //=====================================================
    // Bit operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x16u pop_count(vec8x16u v) {
        #if defined(AVELAVX512VL) & defined(AVEL_AVX512BITALG)
        return vec8x16u{_mm_popcnt_epi16v)};
        #elif defined(AVEL_SSE2)
        //TODO: Adjust to work on 16-bit elements
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec8x16u{0x5555});                    // reuse input as temporary
        v = (v & vec8x16u{0x3333}) + ((v >> 2) & vec8x16u{0x3333});     // temp
        v = ((v + (v >> 4) & vec8x16u{0x0F0F}) * vec8x16u{0x0101}) >> 24; // count
        return v;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x16u has_single_bit(vec8x16u x) {
        #if defined(AVELAVX512VL) && defined(AVX512_BITALG)
        return pop_count(x) == 1;
        #else
        return mask8x16u{x} & ~mask8x16u{x & (x - vec8x16u{1})};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotl(vec8x16u v, std::uint16_t s) {
        return (v << s) | (v >> (16 - s));
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotl(vec8x16u v, vec8x16u s) {
        return (v << s) | (v >> (vec8x16u{16} - s));
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotr(vec8x16u v, std::uint16_t s) {
        return (v >> s) | (v << (16 - s));
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotr(vec8x16u v, vec8x16u s) {
        return (v >> s) | (v << (vec8x16u{16} - s));
    }

}
