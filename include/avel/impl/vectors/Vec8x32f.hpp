namespace avel {

    using vec8x32f = Vector<float, 8>;

    AVEL_FINL vec8x32f trunc(vec8x32f x);

    template<>
    class Vector_mask<float, 8> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<float, 8>::type;

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask(Vector_mask<std::uint32_t, 8> v);

        AVEL_FINL explicit Vector_mask(Vector_mask<std::int32_t, 8> v);

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

        Vector_mask& operator=(bool x) {
            content = from_bool(x);
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Bitwise assignment operators
        //=================================================

        Vector_mask& operator&=(const Vector_mask& rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs.content);
            #else
            content = _mm256_and_ps(content, rhs.content);
            #endif
            return *this;
        }

        Vector_mask& operator|=(const Vector_mask& rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs.content);
            #else
            content = _mm256_or_ps(content, rhs.content);
            #endif
            return *this;
        }

        Vector_mask& operator^=(const Vector_mask& rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs.content);
            #else
            content = _mm256_xor_ps(content, rhs.content);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector_mask operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_knot_mask8(content)};
            #else
            return Vector_mask{_mm256_sub_ps(_mm256_setzero_ps(), content)};
            #endif
        }

        Vector_mask operator&(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm256_and_ps(content, rhs.content)};
            #endif
        }

        Vector_mask operator|(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm256_or_ps(content, rhs.content)};
            #endif
        }

        Vector_mask operator^(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm256_xor_ps(content, rhs.content)};
            #endif
        }

        //=================================================
        // Accessors
        //=================================================

        bool operator[](int i) const {
            #if defined(AVEL_AVX512VL)
            unsigned mask = _cvtmask16_u32(__mmask16(content));
            return mask & (1 << i);
            #else
            int mask = _mm256_movemask_ps(content);
            return mask & (1 << i);
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
                {0, 0},
                {-1, -1}
            };

            return full_masks[x];
            #endif
        }

    };



    template<>
    class Vector<float, 8> {
    public:

        using scalar_type = float;

        using primitive = avel::vector_primitive<float, 8>::type;

        constexpr static unsigned width = 8;

        using mask = Vector_mask<float, 8>;

        template<class U>
        using rebind_type = Vector<U, 8>;

        template<int M>
        using rebind_width = Vector<float, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(Vector<std::uint32_t, 8> v);

        AVEL_FINL explicit Vector(Vector<std::int32_t, 8> v):
            content(_mm256_cvtepi32_ps(v)) {}

        AVEL_FINL explicit Vector(
            float a, float b, float c, float d,
            float e, float f, float g, float h):
            content(_mm256_setr_ps(a, b, c, d,e, f, g, h)) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar_type x):
            content(_mm256_set1_ps(x)) {}

        AVEL_FINL explicit Vector(const scalar_type* x):
            content(_mm256_loadu_ps(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& a):
            content(_mm256_loadu_ps(a.data())) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            return Vector{_mm256_setzero_ps()};
        }

        AVEL_FINL static Vector ones() {
            const primitive zeroes = _mm256_setzero_ps();
            return Vector{_mm256_cmp_ps(zeroes, zeroes, _CMP_EQ_OQ)};
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
            content = _mm256_set1_ps(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(Vector rhs) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_EQ_OQ)};
            #else
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_EQ_OQ)};
            #endif
        }

        AVEL_FINL mask operator!=(Vector rhs) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_NEQ_OQ)};
            #else
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_NEQ_OQ)};
            #endif
        }

        AVEL_FINL mask operator<(Vector rhs) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_LT_OQ)};
            #else
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_LT_OQ)};
            #endif
        }

        AVEL_FINL mask operator<=(Vector rhs) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_LE_OQ)};
            #else
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_LE_OQ)};
            #endif
        }

        AVEL_FINL mask operator>(Vector rhs) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_GT_OQ)};
            #else
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_GT_OQ)};
            #endif
        }

        AVEL_FINL mask operator>=(Vector rhs) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_GE_OQ)};
            #else
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_GE_OQ)};
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

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content = _mm256_add_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm256_sub_ps (content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm256_mul_ps (content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            content = _mm256_div_ps (content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
            content = _mm256_sub_ps(content, _mm256_mul_ps(trunc(*this / vec), vec));
            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+(Vector rhs) const {
            return Vector{_mm256_add_ps(content, rhs.content)};
        }

        AVEL_FINL Vector operator-(Vector rhs) const {
            return Vector{_mm256_sub_ps(content, rhs.content)};
        }

        AVEL_FINL Vector operator*(Vector rhs) const {
            return Vector{_mm256_mul_ps(content, rhs.content)};
        }

        AVEL_FINL Vector operator/(Vector rhs) const {
            return Vector{_mm256_div_ps(content, rhs.content)};
        }

        AVEL_FINL Vector operator%(Vector vec) const {
            return Vector{_mm256_sub_ps(content, _mm256_mul_ps(trunc(*this / vec), vec))};
        }

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

        AVEL_FINL Vector operator&=(const Vector rhs) {
            content = _mm256_and_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector operator|=(const Vector rhs) {
            content = _mm256_or_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector operator^=(const Vector rhs) {
            content = _mm256_xor_ps(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            return Vector{_mm256_andnot_ps(ones().content, content)};
        }

        AVEL_FINL Vector operator&(const Vector rhs) const {
            return Vector{_mm256_and_ps(content, rhs.content)};
        }

        AVEL_FINL Vector operator|(const Vector rhs) const {
            return Vector{_mm256_or_ps(content, rhs.content)};
        }

        AVEL_FINL Vector operator^(const Vector rhs) const {
            return Vector{_mm256_xor_ps(content, rhs.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar_type, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar_type, width> array{};

            _mm256_store_ps(array.data(), content);

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

    Vector<std::uint32_t, 8>::Vector(vec8x32f v):
        #if defined(AVEL_AVX512VL)
        content()
        #else
        content()
        #endif
        {}

    //=====================================================
    // General vector operations
    //=====================================================

    AVEL_FINL vec8x32f blend(vec8x32f a, vec8x32f b, vec8x32f::mask m) {
        #if defined(AVEL_AVX512VL)
        return vec8x32f{_mm256_mask_blend_ps(m, a, b)};
        #else
        return vec8x32f{_mm256_blendv_ps(a, b, m)};
        #endif
    }

    AVEL_FINL vec8x32f frexp(vec8x32f x, vec8x32f* y); //TODO: Implement

    AVEL_FINL vec8x32f floor(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec8x32f ceil(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec8x32f trunc(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec8x32f round(vec8x32f x) {
        //TODO: Verify behavior matches with std::round for values ending in .5
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec8x32f fmod(vec8x32f x, vec8x32f y) {
        return x % y;
    }

    AVEL_FINL vec8x32f sqrt(vec8x32f v) {
        return vec8x32f{_mm256_sqrt_ps(v)};
    }

    AVEL_FINL vec8x32f fmadd(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fmadd_ps(m, x, b)};
        #else
        return m * x + b;
        #endif
    }

    AVEL_FINL vec8x32f fmsub(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fmsub_ps(m, x, b)};
        #else
        return m * x - b;
        #endif
    }

    AVEL_FINL vec8x32f fnmadd(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fnmadd_ps(m, x, b)};
        #else
        return -m * x + b;
        #endif
    }

    AVEL_FINL vec8x32f fnmsub(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fnmsub_ps(m, x, b)};
        #else
        return -m * x - b;
        #endif
    }

    AVEL_FINL vec8x32f abs(vec8x32f v) {
        static const std::uint32_t mask{0x7FFFFFFF};
        return vec8x32f{_mm256_and_ps(v, _mm256_broadcast_ss(reinterpret_cast<const float*>(mask)))};
    }

    AVEL_FINL vec8x32f sin(vec8x32f angle) {
        return {};
    }

    AVEL_FINL vec8x32f cos(vec8x32f angle) {
        return {};
    }

    AVEL_FINL std::array<vec8x32f, 2> sincos(vec8x32f angle) {
        alignas(32) static const float constants0[8] {
            reinterpret_bits<float>(std::uint32_t(0x7FFFFFFF)),
            0.5f,
            6.283185307179586f,
            1.0f / (6.283185307179586f),
            0.78539816339744830f,
            1.57079632679489661f,
            1.0f,
            0.0f
        };

        const __m256 sign_bit_mask = _mm256_set1_ps(constants0[0]);
        __m256 abs_angle = _mm256_and_ps(angle, sign_bit_mask);

        //Mods input angle to [-pi, pi) range
        const __m256 half       = _mm256_set1_ps(constants0[0x01]);
        const __m256 two_pi     = _mm256_set1_ps(constants0[0x02]);
        const __m256 rcp_two_pi = _mm256_set1_ps(constants0[0x03]);
        #if defined(AVEL_FMA)
        __m256 a = _mm256_fnmadd_ps(two_pi, _mm256_round_ps(_mm256_fmadd_ps(abs_angle, rcp_two_pi, half), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), abs_angle);
        #else
        __m256 a0 = _mm256_add_ps(_mm256_mul_ps(abs_angle, rcp_two_pi), half);
        __m256 a1 = _mm256_round_ps(a0, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        __m256 a = _mm256_sub_ps(abs_angle, _mm256_mul_ps(two_pi, a1));
        #endif

        // Taylor series approximation will always return positive sign
        __m256 abs_a = _mm256_and_ps(a, sign_bit_mask);

        __m256 quart_pi = _mm256_set1_ps(constants0[0x04]);
        __m256 half_pi  = _mm256_set1_ps(constants0[0x05]);
         __m256 three_quart_pi = _mm256_add_ps(half_pi, quart_pi);

        // Contain the sign of the final return value.
        #if defined(AVEL_AVX512VL)
        __mmask16 c_signs = _mm256_cmp_ps_mask(half_pi, abs_a, _CMP_LT_OQ);
        __mmask16 s_signs = _kxor_mask16(
            _mm256_cmp_ps_mask(a, _mm256_setzero_ps(), _CMP_LT_OQ),
            _mm256_cmp_ps_mask(angle, _mm256_setzero_ps(), _CMP_LT_OQ)
        );
        #else
        __m256 c_signs = _mm256_cmp_ps(half_pi, abs_a, _CMP_LT_OQ);
        __m256 s_signs = _mm256_xor_ps(
            _mm256_cmp_ps(a, _mm256_setzero_ps(), _CMP_LT_OQ),
            _mm256_cmp_ps(angle, _mm256_setzero_ps(), _CMP_LT_OQ)
        );
        #endif

        // Swap results of Taylor series later if false
        // Checks to see if angle is more than 45 degrees off the x-axis
        #if defined(AVEL_AVX512VL)
        __mmask16 approx_mask = _mm256_cmp_ps_mask(
            _mm256_sub_ps(_mm256_and_ps(_mm256_sub_ps(abs_a, half_pi), sign_bit_mask), quart_pi),
            _mm256_setzero_ps(),
            _CMP_GT_OQ
        );
        #else
        __m256 approx_mask = _mm256_cmp_ps(
            _mm256_sub_ps(_mm256_and_ps(_mm256_sub_ps(abs_a, half_pi), sign_bit_mask), quart_pi),
            _mm256_setzero_ps(),
            _CMP_GT_OQ
        );
        #endif

        // Compute offset that's added to angle to bring it into [-pi/4, pi/4]
        // range where error of Taylor series is minimized
        __m256 angle_offset = _mm256_setzero_ps();
        #if defined(AVEL_AVX512VL)
        __mmask16 add_half0 = _mm256_cmp_ps_mask(quart_pi, abs_a, _CMP_LT_OQ);
        __mmask16 add_half1 = _mm256_cmp_ps_mask(three_quart_pi, abs_a, _CMP_LT_OQ);
        angle_offset = _mm256_mask_add_ps(angle_offset, add_half0, angle_offset, half_pi);
        angle_offset = _mm256_mask_add_ps(angle_offset, add_half1, angle_offset, half_pi);
        #else
        __m256 add_half0 = _mm256_cmp_ps(quart_pi, abs_a, _CMP_LT_OQ);
        __m256 add_half1 = _mm256_cmp_ps(three_quart_pi, abs_a, _CMP_LT_OQ);
        angle_offset = _mm256_add_ps(angle_offset, _mm256_blendv_ps(_mm256_setzero_ps(), half_pi, add_half0));
        angle_offset = _mm256_add_ps(angle_offset, _mm256_blendv_ps(_mm256_setzero_ps(), half_pi, add_half1));
        #endif

        // Compute exponentiations of adjusted angle
        __m256 x = _mm256_sub_ps(abs_a, angle_offset);
        __m256 xx = _mm256_mul_ps(x, x);
        __m256 xxxx = _mm256_mul_ps(xx, xx);
        __m256 xxxxxx = _mm256_mul_ps(xx, xxxx);
        __m256 xxxxxxxx = _mm256_mul_ps(xxxx, xxxx);

        alignas(32) static constexpr float constants1[8] {
            -1.0f / 2.0f,
            -1.0f / 6.0f,
            +1.0f / 24.0f,
            +1.0f / 120.0f,
            -1.0f / 720.0f,
            -1.0f / 5040.0f,
            +1.0f / 40320.0f,
            +1.0f / 362880.0f
        };

        // Denominators for sine terms
        const __m256 sd0 = _mm256_set1_ps(constants1[0x01]);
        const __m256 sd1 = _mm256_set1_ps(constants1[0x03]);
        const __m256 sd2 = _mm256_set1_ps(constants1[0x05]);
        const __m256 sd3 = _mm256_set1_ps(constants1[0x07]);

        // Sum sine terms
        #if defined(AVEL_FMA)
        __m256 st1 = _mm256_set1_ps(constants0[0x06]);
        __m256 st2 = _mm256_fmadd_ps(sd0, xx, st1);
        __m256 st3 = _mm256_fmadd_ps(sd1, xxxx, st2);
        __m256 st4 = _mm256_fmadd_ps(sd2, xxxxxx, st3);
        __m256 st5 = _mm256_fmadd_ps(sd3, xxxxxxxx, st4);
        #else
        __m256 st1 = _mm256_set1_ps(constants0[0x06]);
        __m256 st2 = _mm256_add_ps(_mm256_mul_ps(sd0, xx), st1);
        __m256 st3 = _mm256_add_ps(_mm256_mul_ps(sd1, xxxx), st2);
        __m256 st4 = _mm256_add_ps(_mm256_mul_ps(sd2, xxxxxx), st3);
        __m256 st5 = _mm256_add_ps(_mm256_mul_ps(sd3, xxxxxxxx), st4);
        #endif

        __m256 sin = _mm256_mul_ps(x, st5);

        // Denominators for cosine terms
        const __m256 cd0 = _mm256_set1_ps(constants1[0x00]);
        const __m256 cd1 = _mm256_set1_ps(constants1[0x02]);
        const __m256 cd2 = _mm256_set1_ps(constants1[0x04]);
        const __m256 cd3 = _mm256_set1_ps(constants1[0x06]);

        // Sum cosine terms
        #if defined(AVEL_FMA)
        __m256 ct1 = _mm256_set1_ps(constants0[0x06]);
        __m256 ct2 = _mm256_fmadd_ps(cd0, xx, ct1);
        __m256 ct3 = _mm256_fmadd_ps(cd1, xxxx, ct2);
        __m256 ct4 = _mm256_fmadd_ps(cd2, xxxxxx, ct3);
        __m256 ct5 = _mm256_fmadd_ps(cd3, xxxxxxxx, ct4);
        #else
        __m256 ct1 = _mm256_set1_ps(constants0[0x06]);
        __m256 ct2 = _mm256_add_ps(_mm256_mul_ps(cd0, xx), ct1);
        __m256 ct3 = _mm256_add_ps(_mm256_mul_ps(cd1, xxxx), ct2);
        __m256 ct4 = _mm256_add_ps(_mm256_mul_ps(cd2, xxxxxx), ct3);
        __m256 ct5 = _mm256_add_ps(_mm256_mul_ps(cd3, xxxxxxxx), ct4);
        #endif

        __m256 cos = ct5;

        //Swap bits depending on approx_mask
        #if defined(AVEL_AVX512VL)
        __m256 ret_sin = _mm256_mask_blend_ps(approx_mask, cos, sin);
        __m256 ret_cos = _mm256_mask_blend_ps(approx_mask, sin, cos);
        #else
        __m256 ret_sin = _mm256_blendv_ps(cos, sin, approx_mask);
        __m256 ret_cos = _mm256_blendv_ps(sin, cos, approx_mask);
        #endif

        // Set sine sign bits
        #if defined(AVEL_AVX512VL)
        ret_sin = _mm256_and_ps(ret_sin, sign_bit_mask);
        ret_sin = _mm256_mask_sub_ps(ret_sin, s_signs, _mm256_setzero_ps(), ret_sin);
        #else
        ret_sin = _mm256_and_ps(ret_sin, sign_bit_mask);
        ret_sin = _mm256_blendv_ps(ret_sin, _mm256_sub_ps(_mm256_setzero_ps(), ret_sin), s_signs);
        #endif

        // Set cosine sign bits
        #if defined(AVEL_AVX512VL)
        ret_cos = _mm256_and_ps(ret_cos, sign_bit_mask);
        ret_cos = _mm256_mask_sub_ps(ret_cos, c_signs, _mm256_setzero_ps(), ret_cos);
        #else
        ret_cos = _mm256_and_ps(ret_cos, sign_bit_mask);
        ret_cos = _mm256_blendv_ps(ret_cos, _mm256_sub_ps(_mm256_setzero_ps(), ret_cos), c_signs);
        #endif

        return {vec8x32f{ret_sin}, vec8x32f{ret_cos}};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    AVEL_FINL vec8x32f copysign(vec8x32f mag, vec8x32f sign) {
        alignas(16) std::uint32_t mask_data[1] = {
            0x80000000
        };

        auto mask = vec8x32f{_mm256_set1_ps(mask_data[0])};

        return mask & sign | vec8x32f{_mm256_andnot_ps(mask, mag)};
    }

}
