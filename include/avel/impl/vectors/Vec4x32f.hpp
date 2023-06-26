namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x32f = Vector<float, 4>;
    using arr4x32f = std::array<float, 4>;
    using mask4x32f = Vector_mask<float, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec4x32f trunc(vec4x32f x);
    AVEL_FINL vec4x32f abs(vec4x32f v);
    AVEL_FINL vec4x32f blend(mask4x32f, vec4x32f, vec4x32f);
    AVEL_FINL vec4x32f keep(mask4x32f, vec4x32f);
    AVEL_FINL vec4x32f round(vec4x32f);
    AVEL_FINL mask4x32f isunordered(vec4x32f, vec4x32f);
    AVEL_FINL mask4x32f signbit(vec4x32f x);
    AVEL_FINL mask4x32f isnan(vec4x32f v);

    template<>
    class Vector_mask<float, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE)
        using primitive = __m128;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint32x4_t;
        #endif

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // Constructor
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            Vector_mask(convert<Vector_mask>(v)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL)
            content(b ? 0xf : 0x0) {}
        #elif defined(AVEL_SSE)
            content(_mm_castsi128_ps(_mm_set1_epi32(b ? -1 : 0))) {} //TODO: Implement
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u32(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const std::array<bool, 4>& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if defined(AVEL_AVX512VL)
            __m128i array_data = _mm_set1_epi32(bit_cast<std::uint32_t>(arr));
            content = __mmask8(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_SSE)
            __m128i array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            array_data = _mm_unpacklo_epi8(array_data, array_data);
            array_data = _mm_unpacklo_epi16(array_data, array_data);
            content = _mm_castsi128_ps(_mm_cmplt_epi32(_mm_setzero_si128(), array_data));

            #endif

            #if defined(AVEL_NEON)
            auto array_data = vsetq_lane_u32(
                bit_cast<std::uint32_t>(arr),
                vdupq_n_u32(0x00),
                0x00
            );

            auto zeros = vdupq_n_u8(0x00);
            auto t0 = vreinterpretq_u8_u32(array_data);
            auto mask0 = vcltq_u8(zeros, t0);
            auto mask1 = vzipq_u8(mask0, mask0).val[0];
            auto mask2 = vzipq_u8(mask1, mask1).val[0];

            content = vreinterpretq_u32_u8(mask2);
            #endif
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator=(bool b) {
            *this = Vector_mask{b};
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return lhs.content == rhs.content;

            #elif defined(AVEL_SSE)
            return 0xF == _mm_movemask_ps(_mm_cmpeq_ps(lhs.content, rhs.content));
            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u32(vceqq_u32(decay(lhs), decay(rhs)));
            return min == 0xFFFFFFFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return lhs.content != rhs.content;

            #elif defined(AVEL_SSE)
            return 0xF != _mm_movemask_ps(_mm_cmpeq_ps(lhs.content, rhs.content));
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u32(vceqq_u32(decay(lhs), decay(rhs)));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(vreinterpretq_u64_u32(decay(lhs)), vreinterpretq_u64_u32(decay(rhs)));
            auto shifted = vqshlq_n_u64(diff, 63);
            auto shiftedx4 = vreinterpretq_u32_u64(shifted);
            auto x = vgetq_lane_u32(shiftedx4, 0x1);
            auto y = vgetq_lane_u32(shiftedx4, 0x3);
            return (x | y) >> 31;

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content &= rhs.content;

            #elif defined(AVEL_SSE)
            content = _mm_and_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content |= rhs.content;

            #elif defined(AVEL_SSE)
            content = _mm_or_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content ^= rhs.content;

            #elif defined(AVEL_SSE)
            content = _mm_xor_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{primitive(content ^ 0xf)};

            #elif defined(AVEL_SSE)
            return Vector_mask{_mm_sub_ps(_mm_setzero_ps(), content)};
            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vmvnq_u32(content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {
            return lhs & rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {
            return lhs | rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {
            lhs ^= rhs;
            return lhs;
        }

        //=================================================
        // Conversion operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask4x32f m) {
        #if defined(AVEL_AVX512VL)
        return popcount(decay(m));

        #elif defined(AVEL_SSE)
        return popcount(_mm_movemask_ps(decay(m)));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vnegq_s32(vreinterpretq_s32_u32(decay(m)));
        return static_cast<std::uint32_t>(vaddvq_s32(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(decay(m))));
        auto t1 = vpadd_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpadd_u32(t1, t1);

        return static_cast<std::uint32_t>(vget_lane_u32(t2, 0));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x32f m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE)
        return _mm_movemask_ps(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vmaxvq_u32(decay(m)) != 0x00;

        #elif defined(AVEL_NEON)
        auto t0 = decay(m);
        auto t1 = vpmax_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmax_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) != 0x00;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x32f m) {
        #if defined(AVEL_AVX512VL)
        return 0x0F == _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE)
        return 0x0F == _mm_movemask_ps(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vminvq_u8(vreinterpretq_u8_u32(decay(m))) == 0xFF;

        #elif defined(AVEL_NEON)
        auto t0 = decay(m);
        auto t1 = vpmin_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmin_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) == 0xFFFFFFFF;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x32f m) {
        return !any(m);
    }





    template<>
    class alignas(16) Vector<float, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = float;

        #if defined(AVEL_SSE)
        using primitive = __m128;
        #endif

        #if defined(AVEL_NEON)
        using primitive = float32x4_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
        using rebind_width = Vector<scalar, M>;

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // Constructors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            Vector(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
            content(keep(m, Vector{1.0f})) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE)
            content(_mm_set1_ps(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_f32(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr4x32f& arr):
        #if defined(AVEL_SSE)
            content(_mm_loadu_ps(arr.data())) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_f32(arr.data())) {}
        #endif

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_SSE)
            content = _mm_set1_ps(x);
            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_f32(x);
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_EQ_OQ)};

            #elif defined(AVEL_SSE)
            return mask{_mm_cmpeq_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_f32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_NEQ_OQ)};

            #elif defined(AVEL_SSE)
            return mask{_mm_cmpneq_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_LT_OQ)};

            #elif defined(AVEL_SSE)
            return mask{_mm_cmplt_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_f32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_LE_OQ)};

            #elif defined(AVEL_SSE)
            return mask{_mm_cmple_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_f32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_GT_OQ)};

            #elif defined(AVEL_SSE)
            return mask{_mm_cmpgt_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_f32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_GE_OQ)};

            #elif defined(AVEL_SSE)
            return mask{_mm_cmpge_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_f32(decay(lhs), decay(rhs))};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            //TODO: Consider flipping sign bit manually?
            #if defined(AVEL_SSE)
            return Vector{0.0f} - *this;
            #endif

            #if defined(AVEL_NEON)
            return vector{vnegq_f32(content)};
            #endif
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE)
            content = _mm_add_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_f32(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE)
            content = _mm_sub_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_f32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_SSE)
            content = _mm_mul_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vmulq_f32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            #if defined(AVEL_SSE)
            content = _mm_div_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            content = vdivq_f32(content, decay(rhs));

            #elif defined(AVEL_NEON)
            //TODO: Implement a better emulation
            auto recip = vrecpeq_f32(decay(rhs));
            content = vmul_f32(content, recip);

            #endif
            return *this;
        }

        /*
        AVEL_FINL Vector& operator%=(const Vector rhs) {
            content = decay(fmod(*this, rhs));
            return *this;
        }
        */

        //=================================================
        // Arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend Vector operator+(Vector lhs, Vector rhs) {
            lhs += rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator-(Vector lhs, Vector rhs) {
            lhs -= rhs;
            return lhs;
        }

        [[nodiscard]]
       AVEL_FINL friend Vector operator*(Vector lhs, Vector rhs) {
            lhs *= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator/(Vector lhs, Vector rhs) {
            lhs /= rhs;
            return lhs;
        }

        /*
        [[nodiscard]]
        AVEL_FINL friend Vector operator%(Vector lhs, Vector rhs) {
            lhs %= rhs;
            return lhs;
        }
        */

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
        // Conversions
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return *this != Vector{0.0f};
        }

    };

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL float extract(vec4x32f v) {
        static_assert(N <= vec4x32f::width, "Specified index does not exist");
        typename std::enable_if<N <= vec4x32f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE41)
        return avel::bit_cast<float>(_mm_extract_ps(decay(v), N));

        #elif defined(AVEL_SSE)
        switch (N) {
            case 3: return _mm_cvtss_f32(_mm_shuffle_ps(v, v, 0x03));
            case 2: return _mm_cvtss_f32(_mm_shuffle_ps(v, v, 0x02));
            case 1: return _mm_cvtss_f32(_mm_shuffle_ps(v, v, 0x01));
            case 0: return _mm_cvtss_f32(decay(v));
        }

        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_f32(decay(v), N);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x32f insert(vec4x32f v, float x) {
        static_assert(N < vec4x32f::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x32f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE)
        //TODO: Don't rely on compiler-specific subscripting
        auto tmp = decay(v);
        tmp[N] = x;
        return vec4x32f{tmp};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vsetq_lane_f32(x, decay(v), N)};
        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32f keep(mask4x32f m, vec4x32f v) {
        #if defined(AVEL_AVX512VL)
        return vec4x32f{_mm_maskz_mov_ps(decay(m), decay(v))};

        #elif defined(AVEL_SSE)
        return vec4x32f{_mm_and_ps(decay(m), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vbslq_f32(decay(m), decay(v), vdupq_n_f32(0.0f))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f clear(mask4x32f m, vec4x32f v) {
        #if defined(AVEL_AVX512VL)
        return vec4x32f{_mm_maskz_mov_ps(decay(!m), decay(v))};

        #elif defined(AVEL_SSE)
        return vec4x32f{_mm_andnot_ps(decay(m), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vbslq_f32(decay(m), vdupq_n_f32(0.0f), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f blend(mask4x32f m, vec4x32f a, vec4x32f b) {
        #if defined(AVEL_AVX512VL)
        return vec4x32f{_mm_mask_blend_ps(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_SSE41)
        return vec4x32f{_mm_blendv_ps(decay(b), decay(a), decay(m))};

        #elif defined(AVEL_SSE)
        auto x = _mm_andnot_ps(decay(m), decay(b));
        auto y = _mm_and_ps(decay(m), decay(a));
        return vec4x32f{_mm_or_ps(x, y)};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vbslq_f32(decay(m), decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f byteswap(vec4x32f v) {
        return bit_cast<vec4x32f>(byteswap(bit_cast<vec4x32u>(v)));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f max(vec4x32f a, vec4x32f b) {
        #if defined(AVEL_SSE)
        return vec4x32f{_mm_max_ps(decay(a), decay(b))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vmaxq_f32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f min(vec4x32f a, vec4x32f b) {
        #if defined(AVEL_SSE)
        return vec4x32f{_mm_min_ps(decay(a), decay(b))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vminq_f32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x32f, 2> minmax(vec4x32f a, vec4x32f b) {
        #if defined(AVEL_SSE)
        return std::array<vec4x32f, 2>{
            vec4x32f{_mm_min_ps(decay(a), decay(b))},
            vec4x32f{_mm_max_ps(decay(a), decay(b))}
        };
        #endif

        #if defined(AVEL_NEON)
        return std::array<vec4x32f, 2>{
            vec4x32f{vminq_f32(decay(a), decay(b))},
            vec4x32f{vmaxq_f32(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f clamp(vec4x32f x, vec4x32f lo, vec4x32f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f abs(vec4x32f v) {
        #if defined(AVEL_SSE)
        return vec4x32f{_mm_andnot_ps(_mm_set1_ps(float_sign_bit_mask), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vabsq_f32(decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f neg_abs(vec4x32f v) {
        #if defined(AVEL_SSE)
        return vec4x32f{_mm_and_ps(_mm_set1_ps(float_sign_bit_mask), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vsubq_f32(vdupq_n_f32(0.0f), vabsq_f32(decay(v)))};
        #endif
    }

    //=====================================================
    // Load/Store Operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32f load<vec4x32f>(const float* ptr) {
        #if defined(AVEL_SSE)
        return vec4x32f{_mm_loadu_ps(ptr)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vld1q_f32(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32f aligned_load<vec4x32f>(const float* ptr) {
        #if defined(AVEL_SSE)
        return vec4x32f{_mm_load_ps(ptr)};
        #endif

        #if defined(AVEL_NEON) && __cplusplus >= 202002
            return vec4x32f{vld1q_f32(std::assume_aligned<alignof(vec4x32f)>(ptr))};
        #elif defined(AVEL_NEON) && (defined(AVEL_GCC) || defined(AVEL_CLANG))
            auto* p = reinterpret_cast<const float*>(__builtin_assume_aligned(ptr, alignof(vec4x32f)));
            return vec4x32f{vld1q_f32(p)};
        #elif defined(AVEL_NEON)
            return vec4x32f{vld1q_f32(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32f gather<vec4x32f>(const float* ptr, vec4x32i o) {
        #if defined(AVEL_AVX2)
        return vec4x32f{_mm_i32gather_ps(ptr, decay(o), sizeof(float))};

        #elif defined(AVEL_SSE)
        auto offset_array = to_array(o);

        auto a = _mm_load_ss(ptr + offset_array[0]);
        auto b = _mm_load_ss(ptr + offset_array[1]);
        auto c = _mm_load_ss(ptr + offset_array[2]);
        auto d = _mm_load_ss(ptr + offset_array[3]);

        auto abab = _mm_unpacklo_ps(a, b);
        auto cdcd = _mm_unpacklo_ps(c, d);

        auto abcd = _mm_castpd_ps(_mm_unpacklo_pd(_mm_castps_pd(abab), _mm_castps_pd(cdcd)));

        return vec4x32f{abcd};

        #endif

        #if defined(AVEL_NEON)
        auto ret = vdupq_n_f32(0.0f);
        switch (n) {
            default: ret = vsetq_lane_f32(ptr[extract<3>(indices)], ret, 0x3);
            case 3:  ret = vsetq_lane_f32(ptr[extract<2>(indices)], ret, 0x2);
            case 2:  ret = vsetq_lane_f32(ptr[extract<1>(indices)], ret, 0x1);
            case 1:  ret = vsetq_lane_f32(ptr[extract<0>(indices)], ret, 0x0);
            case 0:  ; //Do nothing
        }

        return vec4x32u{ret};
        #endif
    }

    AVEL_FINL void store(float* ptr, vec4x32f v) {
        #if defined(AVEL_SSE)
        _mm_storeu_ps(ptr, decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_f32(ptr, decay(v));
        #endif
    }

    AVEL_FINL void aligned_store(float* ptr, vec4x32f v) {
        #if defined(AVEL_SSE)
        _mm_store_ps(ptr, decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_f32(ptr, decay(v));
        #endif
    }

    AVEL_FINL void scatter(float* ptr, vec4x32i indices, vec4x32f v) {
        #if defined(AVEL_AVX512VL)
        _mm_i32scatter_ps(ptr, decay(indices), decay(v), sizeof(float));

        #elif defined(AVEL_AVX)
        auto i = to_array(indices);

        _mm_store_ss(ptr + i[0], decay(v));
        _mm_store_ss(ptr + i[1], _mm_permute_ps(decay(v), 0x01));
        _mm_store_ss(ptr + i[2], _mm_permute_ps(decay(v), 0x02));
        _mm_store_ss(ptr + i[3], _mm_permute_ps(decay(v), 0x03));

        #elif defined(AVEL_SSE)
        auto i = to_array(indices);

        _mm_store_ss(ptr + i[0], decay(v));
        _mm_store_ss(ptr + i[1], _mm_shuffle_ps(decay(v), decay(v), 0x01));
        _mm_store_ss(ptr + i[2], _mm_shuffle_ps(decay(v), decay(v), 0x02));
        _mm_store_ss(ptr + i[3], _mm_shuffle_ps(decay(v), decay(v), 0x03));

        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            default: ptr[extract<3>(indices)] = vgetq_lane_f32(decay(x), 0x3);
            case 3:  ptr[extract<2>(indices)] = vgetq_lane_f32(decay(x), 0x2);
            case 2:  ptr[extract<1>(indices)] = vgetq_lane_f32(decay(x), 0x1);
            case 1:  ptr[extract<0>(indices)] = vgetq_lane_f32(decay(x), 0x0);
            case 0: ; //Do nothing
        }

        #endif
    }

    [[nodiscard]]
    AVEL_FINL arr4x32f to_array(vec4x32f v) {
        alignas(16) std::array<float, 4> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Cmath functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32f fmax(vec4x32f a, vec4x32f b) {
        #if defined(AVEL_AVX512VL)
        //TODO: Utilize vfixupimm

        #elif defined(AVEL_SSE)
        const auto neg_inf = vec4x32f(-INFINITY);
        auto result = max(a, max(b, neg_inf));
        auto mask = _mm_cmpeq_ps(decay(result), decay(neg_inf));
        return vec4x32f{_mm_or_ps(decay(result), mask)};
        #endif

        #if defined(AVEL_NEON)
        //TODO:Verify correctness under edge cases
        return vec4x32f{vmax_f32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f fmin(vec4x32f a, vec4x32f b) {
        #if defined(AVEL_AVX512VL)
        //TODO: Utilize vfixupimm

        #elif defined(AVEL_SSE)
        const auto inf = vec4x32f(INFINITY);
        auto result = min(a, min(b, inf));
        auto mask = _mm_cmpeq_ps(decay(result), decay(inf));
        return vec4x32f{_mm_or_ps(decay(result), mask)};
        #endif

        #if defined(AVEL_NEON)
        //TODO:Verify correctness under edge cases
        return vec4x32f{vmin_f32(decay(a), decay(b))};
        #endif
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32f sqrt(vec4x32f x) {
        #if defined(AVEL_SSE)
        return vec4x32f{_mm_sqrt_ps(decay(x))};
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vec4x32f{vsqrtq_f32(decay(x))};

        #elif defined(AVEL_NEON)
        //TODO: Utilize software emulation
        #endif
    }

    //=====================================================
    // Nearest integer function
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32f ceil(vec4x32f x) {
        #if defined(AVELSSE41)
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};

        #elif defined(AVEL_SSE)
        //TODO: Create vectorized implementation
        alignas(alignof(vec4x32f)) auto arr = to_array(x);

        for (int i = 0; i < vec4x32f::width; ++i) {
            arr[0] = std::ceil(arr[1]);
        }

        return aligned_load<vec4x32f>(arr.data());

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH32)
        return vec4x32f{vrndpq_f32(decay(x))};

        #elif defined(AVEL_NEON)
        //TODO: Perform software emulation
        return vec4x32f{};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f floor(vec4x32f x) {
        #if defined(AVELSSE41)
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};

        #elif defined(AVEL_SSE2)
        //TODO: Create vectorized implementation
        alignas(alignof(vec4x32f)) auto arr = to_array(x);

        for (int i = 0; i < vec4x32f::width; ++i) {
            arr[0] = std::floor(arr[1]);
        }

        return aligned_load<vec4x32f>(arr.data());

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH32)
        return vec4x32f{vrndmq_f32(decay(x))};

        #elif defined(AVEL_NEON)
        //TODO: Perform software emulation
        return vec4x32f{};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f trunc(vec4x32f x) {
        #if defined(AVEL_SSE41)
        return vec4x32f{_mm_round_ps(decay(x), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};

        #elif defined(AVEL_SSE)
        vec4x32u x_bits = bit_cast<vec4x32u>(x);
        vec4x32u x_exp = (x_bits >> 23) & vec4x32u{0xff};

        vec4x32u y_exp =  vec4x32u{254} - x_exp;
        vec4x32f y = bit_cast<vec4x32f>(y_exp);

        vec4x32f z = vec4x32f{1.0f} + y;

        vec4x32u m = ~(bit_cast<vec4x32u>(z) - vec4x32u{1}) & vec4x32u{0x007fffff};

        vec4x32f r = bit_cast<vec4x32f>(bit_cast<vec4x32u>(x) & m);

        return r;

        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH32)
        return vec4x32f{vrndq_f32(decay(x))};

        #elif defined(AVEL_NEON)
        //TODO: Perform software emulation
        return vec4x32f{};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f round(vec4x32f v) {
        #if defined(AVELSSE41)
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};

        #elif defined(AVEL_SSE)
        //TODO: Create vectorized implementation
        alignas(alignof(vec4x32f)) auto arr = to_array(v);

        for (int i = 0; i < vec4x32f::width; ++i) {
            arr[0] = std::round(arr[1]);
        }

        return aligned_load<vec4x32f>(arr.data());
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH32)
        return vec4x32f{vrndaq_f32(decay(x))};

        #elif defined(AVEL_NEON)
        //TODO: Perform software emulation
        return vec4x32f{};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f nearbyint(vec4x32f v) {
        #if defined(AVEL_SSE41)
        return vec4x32f{_mm_round_ps(decay(v), _MM_FROUND_CUR_DIRECTION)};

        #elif defined(AVEL_SSE)
        int mode = std::fegetround();
        switch (mode) {
            case FE_DOWNWARD:   return avel::floor(v);
            case FE_TONEAREST:  return avel::round(v);
            case FE_TOWARDZERO: return avel::trunc(v);
            case FE_UPWARD:     return avel::ceil(v);
        default:
            return vec4x32f{0.0f};
            //TODO: Find proper solution. Not sure how to handle such a case
        }
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH32)
        return vec4x32f{vrndiq_f32(decay(x))};

        #elif defined(AVEL_NEON)
        //TODO: Perform software emulation
        return vec4x32f{};
        #endif
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32f frexp(vec4x32f v, vec4x32i* exp) {
        #if defined(AVEL_AVX512VL)
        auto exponents = _mm_getexp_ps(decay(v));
        *exp = _mm_cvttps_epu32(exponents);

        auto ret = _mm_getmant_ps(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        return vec4x32f{ret};

        #elif defined(AVEL_SSE2)
        //TODO: Implement

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f ldexp(vec4x32f arg, vec4x32i exp) {
        //TODO: Implement
        #if defined(AVEL_SSE2)
        auto bits = _mm_castps_si128(decay(arg));
        auto cleared_exponents = _mm_andnot_si128(_mm_set1_epi32(float_exponent_mask_bits), bits);

        //TODO: Handle case where exp is too large
        auto exponents = _mm_srli_epi32(bits, 23);
        auto modified_exponents = _mm_adds_epu8(exponents, decay(exp));

        //TODO: Handle edge cases
        auto reconstructed_exponents = _mm_slli_epi32(modified_exponents, 23);
        auto reconstructed_floats = _mm_or_si128(reconstructed_exponents, cleared_exponents);
        
        return vec4x32f{_mm_castsi128_ps(reconstructed_floats)};
        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f scalebn(vec4x32f x, vec4x32f exp) {
        return ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f ilog(vec4x32f x) {
        #if defined(AVEL_AVX512VL)
        //TODO: Handle edge cases
        return vec4x32f{_mm_cvtepi32_ps(_mm_getexp_ps(decay(x)))};

        #elif defined(AVEL_SSE2)
        auto subnormal_threshold = vec4x32f{1.17549435082228750797e-38f};
        auto is_subnormal = x < subnormal_threshold;

        // Multiply subnormal value so that desired info can be extracted from
        // exponent bits
        auto useful_x = blend(
            is_subnormal,
            x * vec4x32f{8388608.0f},
            x
        );

        auto bits = _mm_castps_si128(decay(useful_x));

        auto byte_mask = _mm_set1_epi32(0xff);
        auto exponent_bits = _mm_and_si128(_mm_srli_epi32(bits, 23), byte_mask);

        auto is_non_normal = _mm_cmpeq_epi32(exponent_bits, byte_mask);
        //TODO: Use this to handle infinities and NaNs

        auto bias = vec4x32i{127};
        bias += keep(mask4x32i{is_subnormal}, vec4x32i{22});

        auto ret = _mm_sub_epi32(exponent_bits, bias);
        return vec4x32f{ret};

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f logb(vec4x32f x) {
        #if defined(AVEL_AVX512VL)
        //TODO: Handle edge cases
        return vec4x32f{_mm_getexp_ps(decay(x))};

        #elif defined(AVEL_SSE2)
        auto subnormal_threshold = vec4x32f{1.17549435082228750797e-38f};
        auto is_subnormal = x < subnormal_threshold;

        // Multiply subnormal value so that desired info can be extracted from
        // exponent bits
        auto useful_x = blend(
            is_subnormal,
            x * vec4x32f{8388608.0f},
            x
        );

        auto bits = _mm_castps_si128(decay(useful_x));

        auto byte_mask = _mm_set1_epi32(0xff);
        auto exponent_bits = _mm_and_si128(_mm_srli_epi32(bits, 23), byte_mask);

        auto is_non_normal = _mm_cmpeq_epi32(exponent_bits, byte_mask);
        //TODO: Use this to handle infinities and NaNs

        auto bias = vec4x32i{127};
        bias += keep(mask4x32i{is_subnormal}, vec4x32i{22});

        auto unbiased_exponents = _mm_sub_epi32(exponent_bits, bias);
        auto ret = _mm_cvtepi32_ps(unbiased_exponents);
        return vec4x32f{ret};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f copysign(vec4x32f mag, vec4x32f sign) {
        #if defined(AVEL_SSE)
        auto mask = _mm_set1_ps(float_sign_bit_mask);
        auto ret = _mm_or_ps(_mm_and_ps(mask, decay(sign)), _mm_andnot_ps(mask, decay(mag)));

        return vec4x32f{ret};
        #endif

        #if defined(AVEL_NEON)
        auto mask = vdupq_n_f32(float_sign_bit_mask);
        auto ret = 	vbslq_f32(mask, decay(mag), decay(sign));

        return vec4x32f{ret};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32u fpclassify(vec4x32f v) {
        const vec4x32u fp_infinite{int(FP_INFINITE)};
        const vec4x32u fp_nan{int(FP_NAN)};
        const vec4x32u fp_normal{int(FP_NORMAL)};
        const vec4x32u fp_subnormal{int(FP_SUBNORMAL)};
        const vec4x32u fp_zero{int(FP_ZERO)};

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        mask4x32u infinite_mask {_mm_fpclass_ps_mask(decay(v), 0x08 | 0x10)};
        mask4x32u nan_mask      {_mm_fpclass_ps_mask(decay(v), 0x01 | 0x80)};
        mask4x32u subnormal_mask{_mm_fpclass_ps_mask(decay(v), 0x20)};
        mask4x32u zero_mask     {_mm_fpclass_ps_mask(decay(v), 0x02 | 0x04)};
        mask4x32u normal_mask   {!(infinite_mask | nan_mask | subnormal_mask | zero_mask)};

        //TODO: Optimize blends
        return
            keep(infinite_mask, fp_infinite) |
            keep(infinite_mask, fp_nan) |
            keep(infinite_mask, fp_normal) |
            keep(infinite_mask, fp_subnormal) |
            keep(infinite_mask, fp_zero);

        #elif defined(AVEL_SSE)
        auto bits = _mm_castps_si128(decay(v));
        auto mantissa = _mm_and_si128(bits, _mm_set1_epi32(0x007fffff));
        auto exponent = _mm_and_si128(_mm_srli_epi32(bits, 23), _mm_set1_epi32(0x000000ff));

        auto has_max_exponent = _mm_cmpeq_epi32(exponent, _mm_set1_epi32(0x000000ff));
        auto has_min_exponent = _mm_cmpeq_epi32(exponent, _mm_set1_epi32(0x00000000));
        auto has_min_mantissa = _mm_cmpeq_epi32(mantissa, _mm_set1_epi32(0x00000000));

        //TODO: Implement and optimize approach
        mask4x32u infinite_mask{};
        mask4x32u nan_mask{};
        mask4x32u normal_mask{};
        mask4x32u subnormal_mask{};
        mask4x32u zero_mask{};

        return
            keep(infinite_mask, fp_infinite) |
            keep(nan_mask, fp_nan) |
            keep(normal_mask, fp_normal) |
            keep(subnormal_mask, fp_subnormal) |
            keep(zero_mask, fp_zero);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isfininte(vec4x32f v) {
        #if defined(AVEL_AVX512VL)
        return  vec4x32f{_mm_getexp_ps(decay(v))} != vec4x32f{255.0f};

        #elif defined(AVEL_SSE)
        auto m = _mm_set1_ps(float_exponent_mask);
        return mask4x32f{_mm_cmpeq_ps(_mm_and_ps(decay(v), m), m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isinf(vec4x32f v) {
        #if defined(AVEL_AVX512VL)
        vec4x32f inf{INFINITY};
        return abs(v) == inf;

        #elif defined(AVEL_SSE)
        vec4x32f inf{INFINITY};
        return abs(v) == inf;

        #endif

        #if defined(AVEL_NEON)
        auto infinity = vdupq_n_f32(INFINITY);
        auto ret = 	vcageq_f32(decay(v), INFINITY);
        return mask4x32f{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isnan(vec4x32f v) {
        #if defined(AVEL_AVX512VL)
        auto tmp = _mm_cmp_ps_mask(decay(v), decay(v), _CMP_UNORD_Q);
        return mask4x32f{tmp};

        #elif defined(AVEL_SSE)
        auto tmp = _mm_cmpunord_ps(decay(v), decay(v));
        return mask4x32f{tmp};
        #endif

        #if defined(AVEL_NEON)
        return v != v;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isnormal(vec4x32f v) {
        #if defined(AVEL_AVX512VL)
        std::uint8_t is_not_normal = _mm_fpclass_ps_mask(decay(v), 0xBF);
        std::uint8_t is_normal = is_not_normal ^ 0xF;
        return mask4x32f{is_normal};

        #elif defined(AVEL_SSE)
        vec4x32f tmp{_mm_and_ps(decay(v), _mm_set1_ps(float_sign_bit_mask))};
        return  (tmp != vec4x32f{0.0f}) & (tmp != vec4x32f{float_exponent_mask});
        #endif

        #if defined(AVEL_NEON)
        auto infinity = vdupq_n_f32(INFINITY);
        auto ret = vcaltq_f32(decay(v), infinty);
        return mask4x32f{ret};

        // Older implementation based on extracting exponent
        auto bits = vreinterpretq_u32_f32(decay(v));
        auto exponents = vandq_u32(vshrq_n_u32(bits, 23), vdupq_n_u32(0x000000ff));
        auto ret = 	vmvnq_u32(vceqq_u32(exponents));
        return mask4x32f{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f signbit(vec4x32f arg) {
        #if defined(AVEL_AVX512VL)
        return mask4x32f{_mm_fpclass_ps_mask(decay(arg), 0x40)};

        #elif defined(AVEL_SSE2)
        auto bits = _mm_castps_si128(decay(arg));
        auto mask_bits = _mm_srai_epi32(bits, 31);
        return mask4x32f{_mm_castsi128_ps(mask_bits)};

        #elif defined(AVEL_SSE)
        auto t0 = bit_cast<vec4x32u>(arg);
        auto t1 = (t0 != vec4x32u{0x00});
        auto t2 = bit_cast<vec4x32f>(t1);
        return mask4x32f{decay(t2)};
        #endif

        #if defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_s32(decay(arg));
        auto t1 = vshrq_n_s32(t0);
        return mask4x32f{vreinterpretq_u32_s32(t1)};
        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask4x32f isgreater(vec4x32f x, vec4x32f y) {
        return x > y;
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isgreaterequal(vec4x32f x, vec4x32f y) {
        return x >= y;
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isless(vec4x32f x, vec4x32f y) {
        return x < y;
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f islessequal(vec4x32f x, vec4x32f y) {
        return x <= y;
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f islessgreater(vec4x32f x, vec4x32f y) {
        return x != y;
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isunordered(vec4x32f x, vec4x32f y) {
        #if defined(AVEL_AVX512VL)
        return mask4x32f{_mm_cmp_ps_mask(decay(x), decay(y), _CMP_ORD_Q)};

        #elif defined(AVEL_SSE)
        return mask4x32f{_mm_cmpunord_ps(decay(x), decay(y))};
        #endif

        #if defined(AVEL_NEON)
        auto a = vceqq_f32(decay(x), decay(x));
        auto b = vceqq_f32(decay(y), decay(y));
        return mask4x32f{vtstq_u32(a, b)};
        #endif
    }

}
