namespace avel {

    using vec4x32i = Vector<std::int32_t, 4>;

    using mask4x32i = Vector_mask<std::int32_t, 4>;

    template<>
    class Vector_mask<std::int32_t, 4> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<std::int32_t, 4>::type;

        //=================================================
        // Constructor
        //=================================================

        //AVEL_FINL explicit Vector_mask(Vector_mask<std::uint32_t, 4> v);

        //AVEL_FINL explicit Vector_mask(Vector_mask<float, 4> v);

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
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs.content);
            #else
            content = _mm_and_si128(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs.content);
            #else
            content = _mm_or_si128(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs.content);
            #else
            content = _mm_xor_si128(content, rhs.content);
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
            primitive tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};
            #endif
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_and_si128(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_or_si128(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_xor_si128(content, rhs.content)};
            #endif
        }

        //=================================================
        // Accessor
        //=================================================

        AVEL_FINL bool operator[](int i) const {
            #if defined(AVEL_AVX512VL)
            unsigned mask = _cvtmask16_u32(__mmask16(content));
            return mask & (1 << i);
            #else
            int mask = _mm_movemask_epi8(content);
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

        AVEL_FINL static primitive from_bool(bool x) {
            #if defined(AVEL_AVX512VL)
            static const primitive full_masks[2] {
                0x00,
                0xFF
            };

            return full_masks[x];
            #else
            static const primitive full_masks[2] {
                {0,  0},
                {-1, -1}
            };

            return full_masks[x];
            #endif
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

        AVEL_FINL explicit Vector(vec4x32u v):
            content(primitive(v)) {}

        AVEL_FINL explicit Vector(Vector<float, width> v);

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm_mask_blend_epi32(m, _mm_setzero_si128(), _mm_set1_epi32(1))) {}
        #else
            content(_mm_sub_epi32(_mm_setzero_si128(), m)) {}
        #endif

        /*
        AVEL_FINL explicit Vector(std::int32_t a, std::int32_t b, std::int32_t c, std::int32_t d):
            content(_mm_set_epi32(d, c, b, a)) {}
        */

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const scalar_type x):
            content(_mm_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const scalar_type* x):
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(x))) {}

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& array):
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(array.data()))) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            return Vector{_mm_setzero_si128()};
        }

        AVEL_FINL static Vector ones() {
            const primitive zeroes = _mm_setzero_si128();
            return Vector{_mm_cmpeq_epi32(zeroes, zeroes)};
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
            content = _mm_set1_epi32(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi32_mask(content, vec.content)};
            #else
            return mask{_mm_cmpeq_epi32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epi32_mask(content, vec.content)};
            #else
            return ~(*this == vec);
            #endif
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmplt_epi32_mask(content, vec.content)};
            #else
            return mask{_mm_cmplt_epi32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epi32_mask(content, vec.content)};
            #else
            return ~mask{*this > vec};
            #endif
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epi32_mask(content, vec.content)};
            #else
            return mask{_mm_cmpgt_epi32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epi32_mask(content, vec.content)};
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
            content = _mm_add_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm_sub_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            #if defined(AVEL_SSE41)
            content = _mm_mul_epi32(content, vec.content);
            #else

            auto y0 = (vec >> 31);
            primitive abs_a =  (vec ^ y0) - y0;

            auto y1 = (*this >> 31);
            primitive abs_b = (*this ^ y1) - y1;

            //Filled with sign bit of output
            auto sign_mask = y0 ^ y1;
            auto mag = _mm_mul_epu32(abs_a, abs_b);

            //Conditionally negate bits (xor) and add 1 (negation of mask)
            content = _mm_add_epi32(_mm_xor_si128(mag, sign_mask), _mm_sub_epi32(_mm_setzero_si128(), sign_mask));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<primitive*>(array0), content);
            _mm_store_si128(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            content = _mm_load_si128(reinterpret_cast<const primitive*>(array0));

            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<primitive*>(array0), content);
            _mm_store_si128(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            content = _mm_load_si128(reinterpret_cast<const primitive*>(array0));

            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+(const Vector vec) const {
            return Vector{_mm_add_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator-(const Vector vec) const {
            return Vector{_mm_sub_epi32(content, vec.content)};
        }

       AVEL_FINL  Vector operator*(const Vector vec) const {
            return Vector{_mm_mul_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator/(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<primitive*>(array0), content);
            _mm_store_si128(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            return Vector{_mm_load_si128(reinterpret_cast<const primitive*>(array0))};
        }

        AVEL_FINL Vector operator%(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<primitive*>(array0), content);
            _mm_store_si128(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            return Vector{_mm_load_si128(reinterpret_cast<const primitive*>(array0))};
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
            content = _mm_and_si128(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm_or_si128(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm_xor_si128(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(std::uint32_t s) {
            content = _mm_sll_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t s) {
            content = _mm_sra_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector<std::uint32_t, width> s) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi32(content, primitive(s));
            #else
            alignas(alignof(Vector)) auto v = as_array();
            auto t = s.as_array();

            for (int i = 0; i < width; ++i) {
                if (32 <= t[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] << t[i];
                }
            }

            content = _mm_load_si128((const primitive*)v.data());
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector<std::uint32_t, width> s) {
            #if defined(AVEL_AVX2)
            content = _mm_srav_epi32(content, primitive(s));
            #else
            alignas(alignof(Vector)) auto v = as_array();
            auto t = s.as_array();

            #if __cplusplus < 202002L
            for (int i = 0; i < width; ++i) {
                // If the amount shifted is exactly the integer width, the
                // behavior is still undefined. This problem is apparent on
                // Clang so must be explicitly handled.
                bool is_too_big = t[i] >= (CHAR_BIT * sizeof(std::uint32_t));

                // TODO: This branch could be eliminated using some bitwise
                // manipulation

                if (is_too_big) {
                    // Fill with sign bit
                    v[i] = std::int32_t(-(std::uint32_t(v[i]) >> 31));
                    continue;
                }

                // Shifting a negative value pre C++20 is undefined behaviors
                // By shifting as unsigned integer this is avoided
                // Since right shift should shift in ones, and unsigned right
                // shift shifts in zeroes, double negation allows for "shifting
                // in ones".
                // All supported platforms perform conversion between signed
                // and unsigned types as expected
                v[i] = std::int32_t(~(std::uint32_t(~v[i]) >> t[i]));
            }
            #else
            for (int i = 0; i < width; ++i) {
                if (32 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] << s[i];
                }
            }
            #endif
            content = _mm_load_si128((const primitive*)v.data());

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            return Vector{_mm_andnot_si128(content, ones().content)};
        }

        AVEL_FINL Vector operator&(Vector vec) const {
            return Vector{_mm_and_si128(content, vec.content)};
        }

        AVEL_FINL Vector operator|(Vector vec) const {
            return Vector{_mm_or_si128(content, vec.content)};
        }

        AVEL_FINL Vector operator^(Vector vec) const {
            return Vector{_mm_xor_si128(content, vec.content)};
        }

        AVEL_FINL Vector operator<<(std::uint64_t s) const {
            return Vector{_mm_sll_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator>>(std::uint64_t s) const {
            return Vector{_mm_sra_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator<<(Vector<std::uint32_t, width> vec) const {
            #if defined(AVEL_AVX2)
            return Vector{_mm_sllv_epi32(content, primitive(vec))};
            #else
            auto v = as_array();
            auto s = vec.as_array();

            for (int i = 0; i < width; ++i) {
                if (32 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] << s[i];
                }
            }

            return Vector{v.data()};
            #endif
        }

        AVEL_FINL Vector operator>>(Vector<std::uint32_t, width> vec) const {
            #if defined(AVEL_AVX2)
            return Vector{_mm_srav_epi32(content, primitive(vec))};
            #else
            auto v = as_array();
            auto s = vec.as_array();

            #if __cplusplus < 202002L
            for (int i = 0; i < width; ++i) {
                // If the amount shifted is exactly the integer width, the
                // behavior is still undefined. This problem is apparent on
                // Clang so must be explicitly handled.
                bool is_too_big = s[i] >= (CHAR_BIT * sizeof(std::uint32_t));

                // TODO: This branch could be eliminated using some bitwise
                // manipulation

                if (is_too_big) {
                    // Fill with sign bit
                    v[i] = std::int32_t(-(std::uint32_t(v[i]) >> 31));
                    continue;
                }

                // Shifting a negative value pre C++20 is undefined behaviors
                // By shifting as unsigned integer this is avoided
                // Since right shift should shift in ones, and unsigned right
                // shift shifts in zeroes, double negation allows for "shifting
                // in ones".
                // All supported platforms perform conversion between signed
                // and unsigned types as expected
                v[i] = std::int32_t(~(std::uint32_t(~v[i]) >> s[i]));
            }
            #else
            for (int i = 0; i < width; ++i) {
                if (32 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] << s[i];
                }
            }
            #endif
            return Vector{v.data()};

            #endif
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar_type, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar_type, width> array{};

            _mm_store_si128(reinterpret_cast<primitive*>(array.data()), content);

            return array;
        }

        AVEL_FINL operator primitive() const {
            return content;
        }

        AVEL_FINL explicit operator mask() const {
            return *this != zeros();
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

    AVEL_FINL vec4x32u::Vector(vec4x32i c):
        content(c) {}

    //=====================================================
    // General vector operations
    //=====================================================

    AVEL_FINL vec4x32i blend(vec4x32i a, vec4x32i b, mask4x32i m) {
        #if defined(AVEL_AVX512VL)
        return vec4x32i{_mm_mask_blend_epi32(
            m,
            static_cast<__m128i>(a),
            static_cast<__m128i>(b)
            )
        };
        #elif defined(AVEL_SSE41)
        return vec4x32i{_mm_blendv_epi8(a, b, m)};
        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(m, a);
        auto y = _mm_and_si128(m, b);
        return vec4x32i{_mm_or_si128(x, y)};
        #endif
    }

    AVEL_FINL vec4x32i max(vec4x32i a, vec4x32i b) {
        #if defined(AVEL_SSE41)
        return vec4x32i{_mm_max_epi32(a, b)};
        #else
        return blend(a, b, a < b);
        #endif
    }

    AVEL_FINL vec4x32i min(vec4x32i a, vec4x32i b) {
        #if defined(AVEL_SSE41)
        return vec4x32i{_mm_min_epi32(a, b)};
        #else
        return blend(a, b, b < a);
        #endif
    }

    AVEL_FINL vec4x32i midpoint(vec4x32i a, vec4x32i b) {
        const vec4x32u mask{0x80000000};

        auto x = vec4x32u{a} ^ mask;
        auto y = vec4x32u{b} ^ mask;

        return vec4x32i{midpoint(x, y) ^ mask};
    }

    AVEL_FINL vec4x32i abs(vec4x32i v) {
        #ifdef AVEL_SSSE3
        return vec4x32i{_mm_abs_epi32(v)};
        #else
        auto y = v >> 31;
        return (v ^ y) - y;
        #endif
    }

    template<>
    AVEL_FINL vec4x32i load<vec4x32i>(const std::int32_t* ptr) {
        return vec4x32i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
    }

    template<>
    AVEL_FINL vec4x32i aligned_load<vec4x32i>(const std::int32_t* ptr) {
        return vec4x32i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
    }

    template<>
    AVEL_FINL vec4x32i stream_load<vec4x32i>(const std::int32_t* ptr) {
        #if defined(AVEL_SSE41)
        //Casting away const is required by intrinsic
        return vec4x32i{_mm_stream_load_si128((__m128i*)(ptr))};
        #else
        return aligned_load<vec4x32i>(ptr);
        #endif
    }

    template<>
    AVEL_FINL vec4x32i gather<vec4x32i>(const std::int32_t* ptr, vec4x32i indices) {
        #if defined(AVEL_AVX2)
        return vec4x32i{_mm_i32gather_epi32(ptr, indices, sizeof(std::int32_t))};
        #else
        auto i = indices.as_array();

        __m128i a = _mm_loadu_si32(ptr + i[0]);
        __m128i b = _mm_loadu_si32(ptr + i[1]);
        __m128i c = _mm_loadu_si32(ptr + i[2]);
        __m128i d = _mm_loadu_si32(ptr + i[3]);

        __m128i abab = _mm_unpacklo_epi32(a, b);
        __m128i cdcd = _mm_unpacklo_epi32(c, d);

        __m128i abcd = _mm_unpacklo_epi64(abab, cdcd);
        return vec4x32i{abcd};
        #endif
    }

    AVEL_FINL void store(std::int32_t* ptr, vec4x32i v) {
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), v);
    }

    AVEL_FINL void aligned_store(std::int32_t* ptr, vec4x32i v) {
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), v);
    }

    AVEL_FINL void stream_store(std::int32_t* ptr, vec4x32i v) {
        _mm_stream_si128(reinterpret_cast<__m128i*>(ptr), v);
    }

    AVEL_FINL void scatter(std::int32_t* ptr, vec4x32i indices, vec4x32i v) {
        #if defined(AVEL_AVX512VL)
        _mm_i32scatter_epi32(ptr, indices, v, sizeof(std::int32_t));
        #else
        auto i = indices.as_array();

        _mm_storeu_si32(ptr + i[0], v);
        _mm_storeu_si32(ptr + i[1], _mm_bsrli_si128(v, 0x4));
        _mm_storeu_si32(ptr + i[2], _mm_bsrli_si128(v, 0x8));
        _mm_storeu_si32(ptr + i[3], _mm_bsrli_si128(v, 0xC));
        #endif
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    AVEL_FINL vec4x32i popcount(vec4x32i v) {
        #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512VPOPCNTDQ)
        return vec4x32i{_mm_popcnt_epi32(v)};
        #elif defined(AVELAVX512VL) & defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm_popcnt_epi16(v);
        auto tmp1 = _mm_slli_epi32(tmp0, 16);

        auto tmp2 = _mm_add_epi32(tmp0, tmp1);

        return vec4x32i{_mm_srli_epi32(tmp2, 16)};
        #elif defined(AVEL_POPCNT) & defined(AVEL_SSE42)
        int a = _mm_extract_epi32(v, 0);
        int b = _mm_extract_epi32(v, 1);
        int c = _mm_extract_epi32(v, 2);
        int d = _mm_extract_epi32(v, 3);

        auto t0 = _mm_setzero_si128();
        auto t1 = _mm_insert_epi32(t0, _mm_popcnt_u32(a), 0);
        auto t2 = _mm_insert_epi32(t1, _mm_popcnt_u32(b), 1);
        auto t3 = _mm_insert_epi32(t2, _mm_popcnt_u32(c), 2);
        auto t4 = _mm_insert_epi32(t3, _mm_popcnt_u32(d), 3);

        return vec4x32i{t4};
        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec4x32i{0x55555555});                    // reuse input as temporary
        v = (v & vec4x32i{0x33333333}) + ((v >> 2) & vec4x32i{0x33333333});     // temp
        v = ((v + (v >> 4) & vec4x32i{0xF0F0F0F}) * vec4x32i{0x1010101}) >> 24; // count
        return v;

        #endif
    }

    template<int S>
    AVEL_FINL vec4x32i rotl(vec4x32i v) {
        #if defined(AVEL_AVX512VL)
        return vec4x32i{_mm_rol_epi32(v, S)};
        #else
        return (v << S) | (v >> (32 - S));
        #endif
    }

    AVEL_FINL vec4x32i rotl(vec4x32i v, vec4x32i s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32i{_mm_rolv_epi32(v, s)};
        #else
        auto l = vec4x32u{v} << vec4x32u{s};
        auto r = vec4x32u{v} >> (vec4x32u{32} - vec4x32u{v});
        return vec4x32i{l | r};
        #endif
    }

    template<int S>
    AVEL_FINL vec4x32i rotr(vec4x32i v) {
        #if defined(AVEL_AVX512VL)
        return vec4x32i{_mm_ror_epi32(v, S)};
        #else
        return (v >> S) | (v << (32 - S));
        #endif
    }

    AVEL_FINL vec4x32i rotr(vec4x32i v, vec4x32i s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32i{_mm_rorv_epi32(v, s)};
        #else
        auto l = vec4x32u{v} >> vec4x32u{s};
        auto r = vec4x32u{v} << (vec4x32u{32} - vec4x32u{v});
        return vec4x32i{l | r};
        #endif
    }

}
