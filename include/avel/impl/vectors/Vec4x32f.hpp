namespace avel {

    using vec4x32f = Vector<float, 4>;

    AVEL_FINL vec4x32f trunc(vec4x32f x);

    template<>
    class Vector_mask<float, 4> {
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
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs.content);
            #else
            content = _mm_and_ps(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs.content);
            #else
            content = _mm_or_ps(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs.content);
            #else
            content = _mm_xor_ps(content, rhs.content);
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
            return Vector_mask{_mm_sub_ps(_mm_setzero_ps(), content)};
            #endif
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_and_ps(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_or_ps(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_xor_ps(content, rhs.content)};
            #endif
        }

        //=================================================
        // Accessors
        //=================================================

        AVEL_FINL bool operator[](int i) const {
            #if defined(AVEL_AVX512VL)
            unsigned mask = _cvtmask16_u32(__mmask16(content));
            return mask & (1 << i);
            #else
            int mask = _mm_movemask_ps(content);
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
    class Vector<float, 4> {
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

        AVEL_FINL explicit Vector(float a, float b, float c, float d):
            content(_mm_setr_ps(a, b, c, d)) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar_type x):
            content(_mm_set1_ps(x)) {}

        AVEL_FINL explicit Vector(const scalar_type* x):
            content(_mm_loadu_ps(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& a):
            content(_mm_loadu_ps(a.data())) {}

        AVEL_FINL explicit Vector(Vector<std::int32_t, width> v):
            content(_mm_cvtepi32_ps(v)) {}

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
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_EQ_OQ)};
            #else
            return mask{_mm_cmpeq_ps(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_NEQ_OQ)};
            #else
            return mask{_mm_cmpneq_ps(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_LT_OQ)};
            #else
            return mask{_mm_cmplt_ps(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_LE_OQ)};
            #else
            return mask{_mm_cmple_ps(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_GT_OQ)};
            #else
            return mask{_mm_cmpgt_ps(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_GE_OQ)};
            #else
            return mask{_mm_cmpge_ps(content, vec.content)};
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

        AVEL_FINL Vector& operator%=(const Vector vec) {
            content = _mm_sub_ps(content, _mm_mul_ps(trunc(*this / vec), vec));
            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+(Vector vec) const {
            return Vector{_mm_add_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator-(Vector vec) const {
            return Vector{_mm_sub_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator*(Vector vec) const {
            return Vector{_mm_mul_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator/(Vector vec) const {
            return Vector{_mm_div_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator%(Vector vec) const {
            return Vector{_mm_sub_ps(content, _mm_mul_ps(trunc(*this / vec), vec))};
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
            alignas(alignof(Vector)) std::array<scalar_type, width> array{};

            _mm_store_ps(array.data(), content);

            return array;
        }

        AVEL_FINL operator primitive() const {
            return content;
        }

        AVEL_FINL explicit operator mask() const {
            return *this == zeros();
        }

        AVEL_FINL explicit operator Vector<std::int32_t, width>() const {
            return Vector<std::int32_t, width>{_mm_cvtps_epi32(content)};
        }

        /*
        AVEL_FINL explicit operator Vector<std::uint32_t, width>() const {
            #if defined(AVEL_AVX512VL)
            return Vector<std::uint32_t, width>(_mm_cvtps_epu32(content));

            #elif defined(AVEL_AVX2)
            __m128i tmp = _mm_castps_si128(content);

            __m128i exponents = _mm_srli_epi32(tmp, 23);
            __m128i shift_amounts = _mm_sub_epi32(_mm_set1_epi32(150), exponents);

            const __m128i mantissa_mask = _mm_set1_epi32(0x007FFFFF);
            __m128i mantissa = _mm_and_si128(tmp, mantissa_mask);

            __m128i l_shifted = _mm_sllv_epi32(mantissa, shift_amounts);
            __m128i r_shifted = _mm_srlv_epi32(mantissa, _mm_sub_epi32(_mm_setzero_si128(), shift_amounts));

            __m128i blend_mask = _mm_cmplt_epi32(_mm_setzero_si128(), shift_amounts);

            __m128i shifted = _mm_blendv_epi8(l_shifted, r_shifted, blend_mask);

            return Vector<std::uint32_t, width>(shifted);

            #else
            Vector<std::uint32_t, width> max{150};

            switch (fegetround()) {
            case FE_TOWARDZERO:
            case FE_TONEAREST:
            case FE_UPWARD:
            case FE_DOWNWARD:
            }

            return {};
            #endif
        }
        */

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    AVEL_FINL vec4x32f blend(vec4x32f a, vec4x32f b, vec4x32f::mask m) {
        #if defined(AVEL_AVX512VL)
            return vec4x32f{_mm_mask_blend_ps(m, a, b)};
        #elif defined(AVEL_SSE41)
        return vec4x32f{_mm_blendv_ps(a, b, m)};
        #else
        auto x = _mm_andnot_ps(m, b);
        auto y = _mm_and_ps(m, a);
        return vec4x32f{_mm_or_ps(x, y)};
        #endif
    }

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

        const __m128i full = vec4x32i::ones();
        const __m128i data = _mm_castps_si128(x);

        //Extract exponent value to low 8 bits in each element
        __m128i exponents = _mm_srli_epi32(_mm_slli_epi32(data, 1), 24);

        // Generate mask for which bits should be masked out to perform truncation
        __m128i mantissa_mask;
        {
            // Number of mantissa bits that should be masked out, strictly positive.
            // May be greater than 23, the number of explicit mantissa bits
            __m128i shift_amounts = _mm_subs_epu16(exponents, _mm_set1_epi32(118));

            // Evaluates to (-1, 0, 0, 0).
            __m128i mask0001 = _mm_andnot_si128(_mm_slli_si128(full, 4), full);


            //Extract individual shift amounts to lower 64-bit integer and shift individually
            __m128i s0 = _mm_and_si128(mask0001, shift_amounts);
            __m128i m0 = _mm_srl_epi32(full, s0);

            __m128i s1 = _mm_and_si128(mask0001, _mm_srli_si128(shift_amounts, 0x4));
            __m128i m1 = _mm_srl_epi32(full, s1);

            __m128i s2 = _mm_and_si128(mask0001, _mm_srli_si128(shift_amounts, 0x8));
            __m128i m2 = _mm_srl_epi32(full, s2);

            __m128i s3 = _mm_srli_si128(shift_amounts, 0xC);
            __m128i m3 = _mm_srl_epi32(full, s3);


            __m128i tmp0 = _mm_unpacklo_epi32(m0, m2);
            __m128i tmp1 = _mm_unpacklo_epi32(m1, m3);
            mantissa_mask = _mm_unpacklo_epi32(tmp0, tmp1);
        }

        //Fills vector elements with 126
        __m128i lo = _mm_slli_epi32(_mm_srli_epi32(full, 26), 1);

        //Create mask for values that should not be zeroed out because they have exponents greater than 126
        __m128i zmask = _mm_cmplt_epi32(lo, exponents);

        // Mask indicating which bits should be copied in returned values
        __m128i copy_mask = _mm_andnot_si128(mantissa_mask, zmask);

        // Copy only bits indicated by mask
        __m128i ret = _mm_and_si128(copy_mask, data);

        return vec4x32f{_mm_castsi128_ps(ret)};
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

    AVEL_FINL vec4x32f fmod(vec4x32f x, vec4x32f y) {
        return x % y;
    }

    AVEL_FINL vec4x32f sqrt(vec4x32f v) {
        return vec4x32f{_mm_sqrt_ps(v)};
    }

    AVEL_FINL vec4x32f fmadd(vec4x32f m, vec4x32f x, vec4x32f b) {
        #if defined(AVEL_FMA)
        return vec4x32f{_mm_fmadd_ps(m, x, b)};
        #else
        return m * x + b;
        #endif
    }

    AVEL_FINL vec4x32f fmsub(vec4x32f m, vec4x32f x, vec4x32f b) {
        #if defined(AVEL_FMA)
        return vec4x32f{_mm_fmsub_ps(m, x, b)};
        #else
        return m * x - b;
        #endif
    }

    AVEL_FINL vec4x32f fnmadd(vec4x32f m, vec4x32f x, vec4x32f b) {
        #if defined(AVEL_FMA)
        return vec4x32f{_mm_fnmadd_ps(m, x, b)};
        #else
        return -m * x + b;
        #endif
    }

    AVEL_FINL vec4x32f fnmsub(vec4x32f m, vec4x32f x, vec4x32f b) {
        #if defined(AVEL_FMA)
        return vec4x32f{_mm_fnmsub_ps(m, x, b)};
        #else
        return -m * x - b;
        #endif
    }

    AVEL_FINL vec4x32f abs(vec4x32f v) {
        #if defined(AVEL_AVX)
        static const std::uint32_t mask{0x7FFFFFFF};
        return vec4x32f{_mm_and_ps(v, _mm_broadcast_ss(reinterpret_cast<const float*>(mask)))};
        #else
        static const std::uint32_t masks[4] {
            0x7FFFFFFF,
            0x7FFFFFFF,
            0x7FFFFFFF,
            0x7FFFFFFF
        };

        return vec4x32f{_mm_and_ps(v, _mm_load_ps(reinterpret_cast<const float*>(masks)))};
        #endif
    }

    AVEL_FINL vec4x32f sin(vec4x32f angle) {
        return {};
    }

    AVEL_FINL vec4x32f cos(vec4x32f angle) {
        return {};
    }

    AVEL_FINL std::array<vec4x32f, 2> sincos(vec4x32f angle) {
        using mask = vec4x32f::mask;
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

        const mask sign_bit_mask{_mm_set1_ps(constants0[0])};
        vec4x32f abs_angle{_mm_and_ps(angle, sign_bit_mask)};

        //Mods input angle to [-pi, pi) range
        const vec4x32f half      {constants0[0x01]};
        const vec4x32f two_pi    {constants0[0x02]};
        const vec4x32f rcp_two_pi{constants0[0x03]};

        vec4x32f a = abs_angle - (two_pi * trunc(fmadd(abs_angle, rcp_two_pi, half)));

        vec4x32f abs_a = abs(a);

        vec4x32f quart_pi{constants0[0x04]};
        vec4x32f half_pi{constants0[0x05]};

        // Contain the sign of the final return value.
        mask c_signs = (half_pi < abs_a);
        mask s_signs = (a < vec4x32f{0.0f}) ^ (angle < vec4x32f{0.0f});

        // Swap results of Taylor series later if false
        // Checks to see if angle is more than 45 degrees off the x-axis
        #if defined(AVEL_AVX512VL)
        __mmask16 approx_mask = _mm_cmp_ps_mask(
            _mm_sub_ps(_mm_and_ps(_mm_sub_ps(abs_a, half_pi), sign_bit_mask), quart_pi),
            _mm_setzero_ps(),
            _CMP_GT_OQ
        );
        #else
        mask approx_mask = vec4x32f{0.0f} < abs(abs_a - half_pi) - quart_pi;
        #endif

        // Compute offset that's added to angle to bring it into [-pi/4, pi/4]
        // range where error of Taylor series is minimized
        vec4x32f angle_offset{0.0f};
        #if defined(AVEL_AVX512VL)
        __mmask16 add_half0 = _mm_cmp_ps_mask(quart_pi, abs_a, _CMP_LT_OQ);
        __mmask16 add_half1 = _mm_cmp_ps_mask(three_quart_pi, abs_a, _CMP_LT_OQ);
        angle_offset = _mm_mask_add_ps(angle_offset, add_half0, angle_offset, half_pi);
        angle_offset = _mm_mask_add_ps(angle_offset, add_half1, angle_offset, half_pi);
        #else
        mask add_half0 = (quart_pi < abs_a);
        mask add_half1 = (quart_pi < (abs_a - half_pi));
        angle_offset += blend(vec4x32f{0.0f}, half_pi, add_half0);
        angle_offset += blend(vec4x32f{0.0f}, half_pi, add_half1);
        #endif

        // Compute exponentiations of adjusted angle
        vec4x32f x = abs_a - angle_offset;
        vec4x32f xx = x * x;
        vec4x32f xxxx = xx * xx;
        vec4x32f xxxxxx = xx * xxxx;
        vec4x32f xxxxxxxx = xxxx * xxxx;

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

        // Sum sine terms
        vec4x32f st1{constants0[0x06]};
        vec4x32f st2 = fmadd(vec4x32f{constants1[0x01]}, xx, st1);
        vec4x32f st3 = fmadd(vec4x32f{constants1[0x03]}, xxxx, st2);
        vec4x32f st4 = fmadd(vec4x32f{constants1[0x05]}, xxxxxx, st3);
        vec4x32f st5 = fmadd(vec4x32f{constants1[0x07]}, xxxxxxxx, st4);

        vec4x32f sin = x * st5;

        // Sum cosine terms
        vec4x32f ct1{constants0[0x06]};
        vec4x32f ct2 = fmadd(vec4x32f{constants1[0x00]}, xx, ct1);
        vec4x32f ct3 = fmadd(vec4x32f{constants1[0x02]}, xxxx, ct2);
        vec4x32f ct4 = fmadd(vec4x32f{constants1[0x04]}, xxxxxx, ct3);
        vec4x32f ct5 = fmadd(vec4x32f{constants1[0x06]}, xxxxxxxx, ct4);

        vec4x32f cos{ct5};

        //Swap bits depending on approx_mask
        #if defined(AVEL_AVX512VL)
        __m128 ret_sin = _mm_mask_blend_ps(approx_mask, cos, sin);
        __m128 ret_cos = _mm_mask_blend_ps(approx_mask, sin, cos);
        #else
        vec4x32f ret_sin = blend(cos, sin, approx_mask);
        vec4x32f ret_cos = blend(sin, cos, approx_mask);
        #endif

        // Set sine sign bits
        #if defined(AVEL_AVX512VL)
        ret_sin = _mm_and_ps(ret_sin, sign_bit_mask);
        ret_sin = _mm_mask_sub_ps(ret_sin, s_signs, _mm_setzero_ps(), ret_sin);
        #else
        ret_sin = abs(ret_sin);
        ret_sin = blend(ret_sin, -ret_sin, s_signs);
        #endif

        // Set cosine sign bits
        #if defined(AVEL_AVX512VL)
        ret_cos = _mm_and_ps(ret_cos, sign_bit_mask);
        ret_cos = _mm_mask_sub_ps(ret_cos, c_signs, _mm_setzero_ps(), ret_cos);
        #else
        ret_cos = _mm_and_ps(ret_cos, sign_bit_mask);
        ret_cos = blend(ret_cos, -ret_cos, c_signs);
        #endif

        return {vec4x32f{ret_sin}, vec4x32f{ret_cos}};
    }

    vec4x32f log(vec4x32f v) {
        return {};
    }



}
