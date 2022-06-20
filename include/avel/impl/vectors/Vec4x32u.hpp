namespace avel {

    using vec4x32u = Vector<std::uint32_t, 4>;

    using mask4x32u = Vector_mask<std::uint32_t, 4>;

    div_type<vec4x32u> div(vec4x32u numerator, vec4x32u denominator);

    template<>
    class Vector_mask<std::uint32_t, 4> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<std::uint32_t, 4>::type;

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
        // Comparison operators
        //=================================================

        AVEL_FINL bool operator==(Vector_mask rhs) const noexcept {
            #if defined(AVEL_AVX512VL)
            return (_mm512_mask2int(content) == _mm512_mask2int(rhs.content));
            #elif defined(AVEL_SSE2)
            return (0xFFFF == _mm_movemask_epi8(_mm_cmpeq_epi32(content, rhs)));
            #endif
        }

        AVEL_FINL bool operator!=(Vector_mask rhs) const noexcept {
            #if defined(AVEL_AVX512VL)
            return (_mm512_mask2int(content) != _mm512_mask2int(rhs.content));
            #elif defined(AVEL_SSE2)
            return (0xFFFF != _mm_movemask_epi8(_mm_cmpeq_epi32(content, rhs)));
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
            #elif defined(AVEL_NEON)
            content = vandq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = vorrq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs.content);
            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = veorq_u32(content, rhs.content);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_knot_mask8(content)};
            #elif defined(AVEL_SSE2)
            primitive tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};
            #elif defined(AVEL_NEON)
            return Vector_mask{vmvnq_u32(content)};
            #endif
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_and_si128(content, rhs.content)};
            #elif defined(AVEL_NEON)
            return Vector_mask{vandq_u32(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_or_si128(content, rhs.content)};
            #elif defined(AVEL_NEON)
            return Vector_mask{vorrq_u32(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_xor_si128(content, rhs.content)};
            #elif defined(AVEL_NEON)
            return Vector_mask{veorq_u32(content, rhs.content)};
            #endif
        }

        //=================================================
        // Accessors
        //=================================================

        AVEL_FINL bool operator[](int i) const {
            #if defined(AVEL_AVX512VL)
            unsigned mask = _mm512_mask2int(content);
            return mask & (1 << i);
            #elif defined(AVEL_SSE2)
            int mask = _mm_movemask_epi8(content);
            //TODO: Check correctness
            return mask & (1 << (4 * i));
            #elif defined(AVEL_NEON)
            //TODO: Optimize
            switch (i) {
            case 0: return vgetq_lane_u32(content, 0);
            case 1: return vgetq_lane_u32(content, 1);
            case 2: return vgetq_lane_u32(content, 2);
            case 3: return vgetq_lane_u32(content, 3);
            }
            #endif
        }

        //=================================================
        // Conversion operators
        //=================================================

        AVEL_FINL operator primitive() const {
            return content;
        }

        AVEL_FINL operator bool() const {
            #if defined(AVEL_AVX512VL)
            return _mm512_mask2int(content);
            #elif defined(AVEL_SSE2)
            return (0xFFFF == _mm_movemask_epi8(content));
            #endif
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
            #elif defined(AVEL_NEON)
            //TODO: Consider alternative implementations
            std::uint32_t y{x};
            primitive z{};
            return vsubq_u32(veorq_u32(z, z), vdupq_n_u32(y));
            #endif
        }

    };

    template<>
    class Vector<std::uint32_t, 4> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        constexpr static unsigned width = 4;

        using scalar_type = std::uint32_t;

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
            content(_mm_mask_blend_epi32(m, _mm_setzero_si128(), _mm_set1_epi32(1))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi32(_mm_setzero_si128(), m)) {}
        #elif defined(AVEL_NEON)
            content(vsubq_u32(zeros(), m)) {}
        #endif

        /*
        AVEL_FINL explicit Vector(std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d):
            #if defined(AVEL_SSE2)
            content(_mm_set_epi32(d, c, b, a)) {}
            #elif defined(AVEL_NEON)
            content() {}
            #endif
        */

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar_type x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi32(x)) {}
        #elif defined(AVEL_NEON)
            content(vdupq_n_u32(x)) {}
        #endif

        AVEL_FINL explicit Vector(const scalar_type* x):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(x))) {}
        #elif defined(AVEL_NEON)
            //TODO: Implement
            content() {}
        #endif

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& array):
            Vector(array.data()) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            #if defined(AVEL_SSE2)
            return Vector{_mm_setzero_si128()};
            #elif defined(AVEL_NEON)
            primitive x{};
            return Vector{veorq_u32(x, x)};
            #endif
        }

        AVEL_FINL static Vector ones() {
            #if defined(AVEL_SSE2)
            const primitive zeroes = _mm_setzero_si128();
            return Vector{_mm_cmpeq_epi32(zeroes, zeroes)};
            #elif defined(AVEL_NEON)
            primitive x{};
            return Vector{vceqq_u32(x, x)};
            #endif
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
            #if defined(AVEL_SSE2)
            content = _mm_set1_epi32(x);
            #elif defined(AVEL_NEON)
            content = vdupq_n_u32(x);
            #endif
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi32_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi32(content, vec.content)};
            #elif defined(AVEL_NEON)
            return mask{vceqq_u32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epu32_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            return ~(*this == vec);
            #elif defined(AVEL_NEON)
            return mask{vtstq_u32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmplt_epu32_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            auto tmp = (*this - vec);
            return mask{_mm_srai_epi32(tmp, 31)};
            #elif defined(AVEL_NEON)
            return mask{vcleq_u32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epu32_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            return ~mask{*this > vec};
            #elif defined(AVEL_NEON)
            return mask{vcleq_u32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epu32_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            primitive offset = _mm_set1_epi32(-2147483648);
            return mask{_mm_cmpgt_epi32(
                _mm_add_epi32(content, offset),
                _mm_add_epi32(vec.content, offset)
            )};
            #elif defined(AVEL_NEON)
            return mask{vcgtq_u32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epu32_mask(content, vec.content)};
            #elif defined(AVEL_SSE2)
            return ~mask{*this < vec};
            #elif defined(AVEL_NEON)
            return mask{vcgeq_u32(content, vec.content)};
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
            #if defined(AVEL_SSE2)
            content = _mm_add_epi32(content, vec.content);
            #elif defined(AVEL_NEON)
            content = vaddq_u32(content, vec.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi32(content, vec.content);
            #elif defined(AVEL_NEON)
            content = vsubq_u32(content, vec.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_mul_epu32(content, vec.content);
            #elif defined(AVEL_NEON)
            content = vmulq_u32(content, vec.content);
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

        AVEL_FINL Vector operator+(const Vector vec) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_add_epi32(content, vec.content)};
            #elif defined(AVEL_NEON)
            return Vector{vaddq_u32(content, vec.content)};
            #endif
        }

        AVEL_FINL Vector operator-(const Vector vec) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_sub_epi32(content, vec.content)};
            #elif defined(AVEL_NEON)
            return Vector{vsubq_u32(content, vec.content)};
            #endif
        }

       AVEL_FINL  Vector operator*(const Vector vec) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_sub_epi32(content, vec.content)};
            #elif defined(AVEL_NEON)
            return Vector{vmulq_u32(content, vec.content)};
            #endif
        }

        AVEL_FINL Vector operator/(const Vector vec) const {
            auto results = div(*this, vec);
            return results.quot;
        }

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
            #elif defined(AVEL_NEON)
            content = vandq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = vorrq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #elif defined(AVEL_NEON)
            content = veorq_u32(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(std::uint32_t s) {
            #if defined(AVEL_SSE2)
            content = _mm_sll_epi32(content, _mm_loadu_si32(&s));
            #elif defined(AVEL_NEON)
            content = vshlq_u32(content, vdupq_n_s32(static_cast<std::int32_t>(s)));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t s) {
            #if defined(AVEL_SSE2)
            content = _mm_srl_epi32(content, _mm_loadu_si32(&s));
            #elif defined(AVEL_NEON)
            content = vsraq_n_u32(content, vdupq_n_u32(s), 0x00);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector vec) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi32(content, vec.content);
            #elif defined(AVEL_SSE2)
            alignas(alignof(Vector)) auto v = as_array();
            auto s = vec.as_array();

            for (int i = 0; i < width; ++i) {
                if (32 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] << s[i];
                }
            }

            content = _mm_load_si128((const primitive*)v.data());
            #elif defined(AVEL_NEON)
            content = vshlq_u32(content, vreinterpretq_s32_u32(vec.content));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector vec) {
            #if defined(AVEL_AVX2)
            content = _mm_srlv_epi32(content, vec.content);
            #elif defined(AVEL_SSE2)
            auto v = as_array();
            auto s = vec.as_array();

            for (int i = 0; i < width; ++i) {
                if (32 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] >> s[i];
                }
            }

            content = _mm_load_si128((const primitive*)v.data());
            #elif defined(AVEL_NEON)
            content = vsraq_n_u32(content, vec.content, 0x00);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_andnot_si128(content, ones().content)};
            #elif defined(AVEL_NEON)
            return Vector{vmvnq_u32(content)};
            #endif
        }

        AVEL_FINL Vector operator&(Vector rhs) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_and_si128(content, rhs.content)};
            #elif defined(AVEL_NEON)
            return Vector{vandq_u32(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector operator|(Vector rhs) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_or_si128(content, rhs.content)};
            #elif defined(AVEL_NEON)
            return Vector{vorrq_u32(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector operator^(Vector rhs) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_xor_si128(content, rhs.content)};
            #elif defined(AVEL_NEON)
            return Vector{veorq_u32(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector operator<<(std::uint32_t s) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_sll_epi32(content, _mm_loadu_si32(&s))};
            #elif defined(AVEL_NEON)
            return Vector{vshlq_u32(content, vdupq_n_s32(static_cast<std::int32_t>(s)))};
            #endif
        }

        AVEL_FINL Vector operator>>(std::uint32_t s) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_srl_epi32(content, _mm_loadu_si32(&s))};
            #elif defined(AVEL_NEON)
            return Vector{vsraq_n_u32(content, vdupq_n_u32(s), 0x00)};
            #endif
        }

        AVEL_FINL Vector operator<<(Vector vec) const {
            #if defined(AVEL_AVX2)
            return Vector{_mm_sllv_epi32(content, vec.content)};
            #elif defined(AVEL_SSE2)
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
            #elif defined(AVEL_NEON)
            return Vector{vshlq_u32(content, vreinterpretq_s32_u32(vec))};
            #endif
        }

        AVEL_FINL Vector operator>>(Vector vec) const {
            #if defined(AVEL_AVX2)
            return Vector{_mm_srlv_epi32(content, vec.content)};
            #elif defined(AVEL_SSE2)
            auto v = as_array();
            auto s = vec.as_array();

            for (int i = 0; i < width; ++i) {
                if (32 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] >> s[i];
                }
            }

            return Vector{v.data()};
            #elif defined(AVEL_NEON)
            return Vector{vsraq_n_u32(content, vec, 0x00)};
            #endif
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar_type, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar_type, width> array{};

            #if defined(AVEL_SSE2)
            _mm_store_si128(reinterpret_cast<primitive*>(array.data()), content);
            #elif defined(AVEL_NEON)
            vst1q_u32(array.data(), content);
            #endif

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
    // General vector operations
    //=====================================================

    AVEL_FINL vec4x32u blend(vec4x32u a, vec4x32u b, mask4x32u m) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_mask_blend_epi32(
            m,
            static_cast<__m128i>(a),
            static_cast<__m128i>(b)
            )
        };
        #elif defined(AVEL_SSE41)
        return vec4x32u{_mm_blendv_epi8(a, b, m)};
        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(m, a);
        auto y = _mm_and_si128(m, b);
        return vec4x32u{_mm_or_si128(x, y)};
        #elif defined(AVEL_NEON)
        auto x = vandq_u32(b, m);
        auto y = vornq_u32(m, a);
        return vec4x32u{vornq_u32(x, y)};
        #endif
    }

    AVEL_FINL vec4x32u max(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_max_epu32(a, b)};
        #elif defined(AVEL_SSE2)
        return blend(a, b, a < b);
        #elif defined(AVEL_NEON)
        return vec4x32u{vmaxq_u32(a, b)};
        #endif
    }

    AVEL_FINL vec4x32u min(vec4x32u a, vec4x32u b) {
        #if defined(AVEL_SSE41)
        return vec4x32u{_mm_min_epu32(a, b)};
        #elif defined(AVEL_SSE2)
        return blend(a, b, b < a);
        #elif defined(AVEL_NEON)
        return vec4x32u{vminq_u32(a, b)};
        #endif
    }

    AVEL_FINL vec4x32u midpoint(vec4x32u a, vec4x32u b) {
        return (a & b) + ((a ^ b) >> 1);
    }

    template<>
    AVEL_FINL vec4x32u load<vec4x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #elif defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};
        #endif
    }

    template<>
    AVEL_FINL vec4x32u aligned_load<vec4x32u>(const std::uint32_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #elif defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};
        #endif
    }

    template<>
    AVEL_FINL vec4x32u stream_load<vec4x32u>(const std::uint32_t* ptr) {
        #ifdef AVEL_SSE41
        //Casting away of const required by intrinsic
        return vec4x32u{_mm_stream_load_si128((__m128i*)ptr)};
        #elif defined(AVEL_SSE2)
        return aligned_load<vec4x32u>(ptr);
        #elif defined(AVEL_NEON)
        return vec4x32u{vld1q_u32(ptr)};
        #endif
    }

    //Definition of gather deferred until vector of signed integers is defined

    template<>
    AVEL_FINL vec4x32u broadcast<vec4x32u>(std::uint32_t x) {
        #if defined(AVEL_SSE2)
        return vec4x32u{_mm_set1_epi32(x)};
        #endif
    }

    AVEL_FINL void store(std::uint32_t* ptr, vec4x32u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), v);
        #endif
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec4x32u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), v);
        #elif defined(AVEL_NEON)
        vst1q_u32(ptr, v);
        #endif
    }

    AVEL_FINL void stream_store(std::uint32_t* ptr, vec4x32u v) {
        #if defined(AVEL_SSE2)
        _mm_stream_si128(reinterpret_cast<__m128i*>(ptr), v);
        #elif defined(AVEL_NEON)
        vst1q_u32(ptr, v);
        #endif
    }

    //Definition of scatter deferred until vector of signed integers is defined

    //=====================================================
    // Integer arithmetic functions
    //=====================================================

    AVEL_FINL div_type<vec4x32u> div(vec4x32u numerator, vec4x32u denominator) {
        vec4x32u quotient{};

        auto z = vec4x32u::zeros();
        for (std::uint32_t i = 32; (i-- > 0) && (numerator != z);) {
            mask4x32u b = (numerator >> i) >= denominator;

            #if defined(AVEL_AVX512VL)
            numerator -= vec4x32u{_mm_mask_sub_epi32(numerator, b, numerator, (denominator << i))};
            #elif defined(AVEL_SSE2)
            numerator -= (vec4x32u{vec4x32u::primitive(b)} & (denominator << i));
            #endif

            quotient |= (vec4x32u {b} << i);
        }

        return {quotient, numerator};
    }

    //=====================================================
    // Bit operations
    //=====================================================

    AVEL_FINL vec4x32u pop_count(vec4x32u v) {
        #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512VPOPCNTDQ)
        return vec4x32u{_mm_popcnt_epi32(v)};
        #elif defined(AVELAVX512VL) & defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm_popcnt_epi16(v);
        auto tmp1 = _mm_slli_epi32(tmp0, 16);

        auto tmp2 = _mm_add_epi32(tmp0, tmp1);

        return vec4x32u{_mm_srli_epi32(tmp2, 16)};
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

        return vec4x32u{t4};
        #elif defined(AVEL_SSE2)
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec4x32u{0x55555555});                    // reuse input as temporary
        v = (v & vec4x32u{0x33333333}) + ((v >> 2) & vec4x32u{0x33333333});     // temp
        v = ((v + (v >> 4) & vec4x32u{0xF0F0F0F}) * vec4x32u{0x1010101}) >> 24; // count
        return v;
        #elif defined(AVEL_NEON)
        vec4x32u reg_abcd{vreinterpretq_u32_u8(vcntq_u8(vreinterpretq_u8_u32(v)))};
        vec4x32u reg_00ab{vshrq_n_u32(reg_abcd, 16)};
        vec4x32u reg_00xy{vaddq_u32(reg_abcd, reg_00ab)};

        vec4x32u reg_000x{vshrq_n_u32(reg_00xy, 8)};
        vec4x32u reg_000z{vaddq_u32(reg_000x, reg_00xy)};

        return reg_000z;
        #endif
    }

    template<int S>
    AVEL_FINL vec4x32u rotl(vec4x32u v) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rol_epi32(v, S)};
        #elif defined(AVEL_SSE2)
        return (v << S) | (v >> (32 - S));
        #elif defined(AVEL_NEON)
        return (v << S) | (v >> (32 - S));
        #endif
    }

    AVEL_FINL vec4x32u rotl(vec4x32u v, vec4x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rolv_epi32(v, s)};
        #elif defined(AVEL_SSE2)
        return (v << s) | (v >> (vec4x32u{32} - s));
        #elif defined(AVEL_NEON)
        return (v << s) | (v >> (vec4x32u{32} - s));
        #endif
    }

    template<int S>
    AVEL_FINL vec4x32u rotr(vec4x32u v) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_ror_epi32(v, S)};
        #elif defined(AVEL_SSE2)
        return (v >> S) | (v << (32 - S));
        #elif defined(AVEL_NEON)
        return (v >> S) | (v << (32 - S));
        #endif
    }

    AVEL_FINL vec4x32u rotr(vec4x32u v, vec4x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec4x32u{_mm_rorv_epi32(v, s)};
        #elif defined(AVEL_SSE2)
        return (v >> s) | (v << (vec4x32u {32} - s));
        #elif defined(AVEL_NEON)
        return (v >> s) | (v << (vec4x32u {32} - s));
        #endif
    }

}
