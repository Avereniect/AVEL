namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x32f = Vector<float, 16>;
    using arr16x32f = std::array<float, 16>;
    using mask16x32f = Vector_mask<float, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec16x32f trunc(vec16x32f x);
    AVEL_FINL vec16x32f abs(vec16x32f v);
    AVEL_FINL vec16x32f fmod(vec16x32f a, vec16x32f b);
    AVEL_FINL vec16x32f blend(vec16x32f, vec16x32f, mask16x32f);
    AVEL_FINL vec16x32f round(vec16x32f);
    AVEL_FINL mask16x32f isunordered(vec16x32f, vec16x32f);
    AVEL_FINL mask16x32f signbit(vec16x32f x);
    AVEL_FINL mask16x32f isnan(vec16x32f v);
    AVEL_FINL vec16x32f copysign(vec16x32f mag, vec16x32f sign);





    template<>
    class Vector_mask<float, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = __mmask16;

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

        AVEL_FINL explicit Vector_mask(const primitive contents):
            content(contents) {}

        AVEL_FINL explicit Vector_mask(bool b):
            content(b ? 0xffff : 0x00) {}

        AVEL_FINL explicit Vector_mask(const arr16xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = _mm_test_epi8_mask(array_data, array_data);

            #elif defined(AVEL_AVX512F)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            auto widened_data = _mm512_cvtepi8_epi32(array_data);
            content = _mm512_test_epi32_mask(widened_data, widened_data);

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

        AVEL_FINL Vector_mask& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL bool operator==(Vector_mask rhs) const {
            return content == rhs.content;
        }

        AVEL_FINL bool operator!=(Vector_mask rhs) const {
            return content != rhs.content;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(const Vector_mask& rhs) {
            content &= rhs.content;
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(const Vector_mask& rhs) {
            content |= rhs.content;
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(const Vector_mask& rhs) {
            content ^= rhs.content;
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            return Vector_mask{_knot_mask16(content)};
        }

        AVEL_VECTOR_MASK_BINARY_BITWISE_OPERATORS

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

    AVEL_FINL std::uint32_t count(mask16x32f m) {
        return avel::popcount(decay(m));
    }

    AVEL_FINL bool any(mask16x32f m) {
        return decay(m);
    }

    AVEL_FINL bool all(mask16x32f m) {
        return 0xFFFF == decay(m);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x32f m) {
        return !decay(m);
    }




    template<>
    class Vector<float, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = float;

        using primitive = __m512;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(mask m):
            content(_mm512_mask_blend_ps(decay(m), _mm512_setzero_ps(), _mm512_set1_ps(1.0f))) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm512_set1_ps(x)) {}

        AVEL_FINL explicit Vector(const scalar* x):
            content(_mm512_loadu_ps(x)) {}

        AVEL_FINL explicit Vector(const arr16x32f& a):
            content(_mm512_loadu_ps(a.data())) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(scalar x) {
            *this = Vector{x};
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
        AVEL_FINL mask operator==(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_EQ_OS)};
        }

        [[nodiscard]]
        AVEL_FINL mask operator!=(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_NEQ_OS)};
        }

        [[nodiscard]]
        AVEL_FINL mask operator<(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LT_OS)};
        }

        [[nodiscard]]
        AVEL_FINL mask operator<=(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LE_OS)};
        }

        [[nodiscard]]
        AVEL_FINL mask operator>(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GT_OS)};
        }

        [[nodiscard]]
        AVEL_FINL mask operator>=(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GE_OS)};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Vector operator-() const {
            return Vector{0.0f} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector vec) {
            content = _mm512_add_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm512_sub_ps (content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm512_mul_ps (content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            content = _mm512_div_ps (content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector vec) {
            //TODO: Implement
            return *this;
        }

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
            #if defined(AVEL_AVX512F)
            auto t = _mm512_castps_si512(content);
            return mask{_mm512_test_epi32_mask(t, t)};
            #endif
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    static_assert(
        16 * sizeof(float) == sizeof(vec16x32f),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL float extract(vec16x32f v) {
        static_assert(N < vec16x32f::width, "Specified index does not exist");
        typename std::enable_if<N < vec16x32f::width, int>::type dummy_variable = 0;

        auto quarter = _mm512_extractf32x4_ps(decay(v), N / 4);
        auto element = avel::bit_cast<float>(_mm_extract_ps(quarter, N % 4));

        return element;
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL vec16x32f insert(vec16x32f v, float x) {
        static_assert(N < vec16x32f::width, "Specified index does not exist");
        typename std::enable_if<N < vec16x32f::width, int>::type dummy_variable = 0;

        auto broadcasted = _mm512_set1_ps(x);
        constexpr auto blend_mask = std::uint16_t(~(1 << N));
        auto ret = _mm512_mask_blend_ps(blend_mask, broadcasted, decay(v));
        return vec16x32f{ret};
    }

    //=====================================================
    // General float vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec16x32f x) {
        return count(mask16x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec16x32f x) {
        return any(mask16x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec16x32f x) {
        return all(mask16x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec16x32f x) {
        return none(mask16x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f keep(mask16x32f m, vec16x32f v) {
        return vec16x32f{_mm512_maskz_mov_ps(decay(m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f clear(mask16x32f m, vec16x32f v) {
        return vec16x32f{_mm512_maskz_mov_ps(decay(!m), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f blend(mask16x32f m, vec16x32f a, vec16x32f b) {
        return vec16x32f{_mm512_mask_blend_ps(decay(m), decay(b), decay(a))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f max(vec16x32f a, vec16x32f b) {
        return vec16x32f{_mm512_max_ps(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f min(vec16x32f a, vec16x32f b) {
        return vec16x32f{_mm512_min_ps(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec16x32f, 2> minmax(vec16x32f a, vec16x32f b) {
        return std::array<vec16x32f, 2>{
            vec16x32f{_mm512_min_ps(decay(a), decay(b))},
            vec16x32f{_mm512_max_ps(decay(a), decay(b))}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f clamp(vec16x32f x, vec16x32f lo, vec16x32f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f negate(mask16x32f m, vec16x32f v) {
        return vec16x32f{_mm512_mask_sub_ps(decay(v), decay(m), _mm512_setzero_ps(), decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f abs(vec16x32f v) {
        return vec16x32f{_mm512_abs_ps(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f neg_abs(vec16x32f v) {
        #if defined(AVEL_AVX512DQ)
        return vec16x32f{_mm512_or_ps(_mm512_set1_ps(float_sign_bit_mask), decay(v))};

        #elif defined(AVEL_AVX512F)
        auto bits = _mm512_castps_si512(decay(v));
        auto mask = _mm512_set1_epi32(float_sign_bit_mask_bits);
        auto result = _mm512_or_si512(bits, mask);
        return vec16x32f{_mm512_castsi512_ps(result)};

        #endif
    }

    //=====================================================
    // Load/Store Operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f load<vec16x32f>(const float* ptr, std::uint32_t n) {
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        return vec16x32f{_mm512_maskz_loadu_ps(mask, ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f load<vec16x32f, vec16x32f::width>(const float* ptr) {
        return vec16x32f{_mm512_loadu_ps(ptr)};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f aligned_load<vec16x32f>(const float* ptr, std::uint32_t n) {
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        return vec16x32f{_mm512_maskz_load_ps(mask, ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f aligned_load<vec16x32f, vec16x32f::width>(const float* ptr) {
        return vec16x32f{_mm512_load_ps(ptr)};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f gather<vec16x32f>(const float* ptr, vec16x32i indices, std::uint32_t n) {
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        return vec16x32f{_mm512_mask_i32gather_ps(_mm512_setzero_ps(), mask, decay(indices), ptr, sizeof(float))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f gather<vec16x32f>(const float* ptr, vec16x32i indices) {
        #if defined(AVEL_AVX512F)
        return vec16x32f{_mm512_i32gather_ps(decay(indices), ptr, sizeof(float))};
        #endif
    }



    AVEL_FINL void store(float* ptr, vec16x32f v, std::uint32_t n) {
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm512_mask_storeu_ps(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec16x32f::width>
    AVEL_FINL void store(float* ptr, vec16x32f v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec16x32f::width>(float* ptr, vec16x32f v) {
        _mm512_storeu_ps(ptr, decay(v));
    }



    AVEL_FINL void aligned_store(float* ptr, vec16x32f v, std::uint32_t n) {
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm512_mask_store_ps(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec16x32f::width>
    AVEL_FINL void aligned_store(float* ptr, vec16x32f v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec16x32f::width>(float* ptr, vec16x32f v) {
        _mm512_store_ps(ptr, decay(v));
    }



    AVEL_FINL void scatter(float* ptr, vec16x32f v, vec16x32i indices, std::uint32_t n) {
        auto mask = (n >= 16) ? -1 : (1 << n) - 1;
        _mm512_mask_i32scatter_ps(ptr, mask, decay(indices), decay(v), sizeof(float));
    }

    template<std::uint32_t N = vec16x32f::width>
    AVEL_FINL void scatter(float* ptr, vec16x32f v, vec16x32i indices) {
        scatter(ptr, v, indices, N);
    }

    template<>
    AVEL_FINL void scatter<0>(float* ptr, vec16x32f v, vec16x32i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec16x32f::width>(float* ptr, vec16x32f v, vec16x32i indices) {
        _mm512_i32scatter_ps(ptr, decay(indices), decay(v), sizeof(float));
    }



    [[nodiscard]]
    AVEL_FINL arr16x32f to_array(vec16x32f v) {
        alignas(64) std::array<float, 16> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // cmath basic operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32f fmax(vec16x32f a, vec16x32f b) {
        return blend(avel::isnan(b), a, avel::max(a, b));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f fmin(vec16x32f a, vec16x32f b) {
        return blend(avel::isnan(b), a, avel::min(a, b));
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32f sqrt(vec16x32f x) {
        return vec16x32f{_mm512_sqrt_ps(decay(x))};
    }

    //=====================================================
    // Nearest Integer Floating-point operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32f ceil(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(decay(v), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f floor(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(decay(v), _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f trunc(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(decay(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f round(vec16x32f v) {
        auto whole = trunc(v);
        auto frac = v - whole;

        auto offset = copysign(vec16x32f{1.0f}, v);
        auto should_offset = abs(frac) >= vec16x32f{0.5f};
        auto ret = whole + keep(should_offset, offset);

        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f nearbyint(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(decay(v), _MM_FROUND_CUR_DIRECTION)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f rint(vec16x32f v) {
        return nearbyint(v);
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32f frexp(vec16x32f v, vec16x32i* exp)  {
        #if defined(AVEL_AVX512DQ)
        auto is_infinity = _mm512_fpclass_ps_mask(decay(v), 0x10 | 0x08);
        auto is_non_zero = _mm512_cmp_ps_mask(decay(v), _mm512_setzero_ps(), _CMP_NEQ_UQ);

        auto exponents = _mm512_getexp_ps(decay(v));;
        exponents = _mm512_add_ps(exponents, _mm512_set1_ps(1.0f));
        *exp = _mm512_maskz_cvttps_epi32(is_non_zero, exponents);

        auto ret = _mm512_getmant_ps(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        ret = _mm512_maskz_mov_ps(is_non_zero, ret);
        ret = _mm512_mask_blend_ps(is_infinity, ret, decay(v));
        return vec16x32f{ret};

        #elif defined(AVEL_AVX512F)
        auto is_infinity = _mm512_cmp_ps_mask(_mm512_abs_ps(decay(v)), _mm512_set1_ps(INFINITY), _CMP_EQ_OQ);
        auto is_non_zero = _mm512_cmp_ps_mask(decay(v), _mm512_setzero_ps(), _CMP_NEQ_UQ);

        auto exponents = _mm512_getexp_ps(decay(v));
        exponents = _mm512_add_ps(exponents, _mm512_set1_ps(1.0f));
        *exp = _mm512_maskz_cvttps_epi32(is_non_zero, exponents);

        auto ret = _mm512_getmant_ps(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        ret = _mm512_maskz_mov_ps(is_non_zero, ret);
        ret = _mm512_mask_blend_ps(is_infinity, ret, decay(v));
        return vec16x32f{ret};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f ldexp(vec16x32f x, vec16x32i exp)  {
        return vec16x32f{_mm512_scalef_ps(decay(x), _mm512_cvtepi32_ps(decay(exp)))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f scalbn(vec16x32f x, vec16x32i exp)  {
        return avel::ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i ilogb(vec16x32f x) {
        auto exp_fp = _mm512_getexp_ps(decay(x));

        vec16x32f zero_ret{_mm512_castsi512_ps(_mm512_set1_epi32(FP_ILOGB0))};
        vec16x32f inf_ret {_mm512_castsi512_ps(_mm512_set1_epi32(INT_MAX))};
        vec16x32f nan_ret {_mm512_castsi512_ps(_mm512_set1_epi32(FP_ILOGBNAN))};

        // Return value when input is not edge case
        auto misc_ret_i = _mm512_cvtps_epi32(exp_fp);
        misc_ret_i = _mm512_maskz_mov_epi32(_mm512_cmpneq_epi32_mask(misc_ret_i, _mm512_set1_epi32(0x80000000)), misc_ret_i);

        vec16x32i zero_ret_i{_mm512_castps_si512(_mm512_fixupimm_ps(decay(zero_ret), exp_fp, _mm512_set1_epi32(0x88808888), 0x00))};
        vec16x32i inf_ret_i {_mm512_castps_si512(_mm512_fixupimm_ps(decay(inf_ret),  exp_fp, _mm512_set1_epi32(0x88088888), 0x00))};
        vec16x32i nan_ret_i {_mm512_castps_si512(_mm512_fixupimm_ps(decay(nan_ret),  exp_fp, _mm512_set1_epi32(0x88888800), 0x00))};

        return (vec16x32i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f logb(vec16x32f x) {
        return vec16x32f{_mm512_getexp_ps(decay(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f copysign(vec16x32f mag, vec16x32f sign) {
        auto mask = _mm512_set1_epi32(float_sign_bit_mask_bits);
        auto ret = _mm512_ternarylogic_epi32(_mm512_castps_si512(decay(sign)), _mm512_castps_si512(decay(mag)), mask, 0xe4);
        return vec16x32f{_mm512_castsi512_ps(ret)};
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32i fpclassify(vec16x32f v) {
        #if defined(AVEL_AVX512DQ)
        const vec16x32i fp_infinite{int(FP_INFINITE)};
        const vec16x32i fp_nan{int(FP_NAN)};
        const vec16x32i fp_normal{int(FP_NORMAL)};
        const vec16x32i fp_subnormal{int(FP_SUBNORMAL)};
        const vec16x32i fp_zero{int(FP_ZERO)};

        mask16x32i infinite_mask {_mm512_fpclass_ps_mask(decay(v), 0x08 | 0x10)};
        mask16x32i nan_mask      {_mm512_fpclass_ps_mask(decay(v), 0x01 | 0x80)};
        mask16x32i subnormal_mask{_mm512_fpclass_ps_mask(decay(v), 0x20)};
        mask16x32i zero_mask     {_mm512_fpclass_ps_mask(decay(v), 0x02 | 0x04)};
        mask16x32i normal_mask   {!(infinite_mask | nan_mask | subnormal_mask | zero_mask)};

        return
            keep(infinite_mask, fp_infinite) |
            keep(nan_mask, fp_nan) |
            keep(normal_mask, fp_normal) |
            keep(subnormal_mask, fp_subnormal) |
            keep(zero_mask, fp_zero);

        #elif defined(AVEL_AVX512F)
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm512_castps_si512(decay(v));

        // Take absolute value
        v_bits = _mm512_and_si512(v_bits, _mm512_set1_epi32(0x7fffffff));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm512_set1_epi32(0x00800000);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm512_set1_epi32(0x7f800000);

        // Masks for individual categories
        auto m0 = _mm512_testn_epi32_mask(v_bits, v_bits);
        auto m1 = ~m0 & _mm512_cmplt_epi32_mask(v_bits, min_bits);
        auto m2 = ~_mm512_cmplt_epi32_mask(v_bits, min_bits) & _mm512_cmplt_epi32_mask(v_bits, inf_bits);
        auto m3 = _mm512_cmpeq_epi32_mask(v_bits, inf_bits);
        auto m4 = _mm512_cmpgt_epi32_mask(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm512_setzero_si512();
        ret = _mm512_maskz_mov_epi32(m0, _mm512_set1_epi32(FP_ZERO));
        ret = _mm512_mask_blend_epi32(m1, ret, _mm512_set1_epi32(FP_SUBNORMAL));
        ret = _mm512_mask_blend_epi32(m2, ret, _mm512_set1_epi32(FP_NORMAL)   );
        ret = _mm512_mask_blend_epi32(m3, ret, _mm512_set1_epi32(FP_INFINITE) );
        ret = _mm512_mask_blend_epi32(m4, ret, _mm512_set1_epi32(FP_NAN)      );

        return vec16x32i{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isfinite(vec16x32f v) {
        #if defined(AVEL_AVX512DQ)
        return !mask16x32f{_mm512_fpclass_ps_mask(decay(v), 0x01 | 0x08 | 0x10 | 0x80)};

        #elif defined(AVEL_AVX512F)
        return (vec16x32f{-INFINITY} < v) && (v < vec16x32f{+INFINITY});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isinf(vec16x32f v) {
        #if defined(AVEL_AVX512DQ)
        return mask16x32f{_mm512_fpclass_ps_mask(decay(v), 0x08 | 0x10)};

        #elif defined(AVEL_AVX512F)
        return avel::abs(v) == vec16x32f{INFINITY};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isnan(vec16x32f v) {
        #if defined(AVEL_AVX512DQ)
        return mask16x32f{_mm512_fpclass_ps_mask(decay(v), 0x80 | 0x01)};

        #elif defined(AVEL_AVX512F)
        return mask16x32f{_mm512_cmp_ps_mask(decay(v), decay(v), _CMP_UNORD_Q)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isnormal(vec16x32f v) {
        #if defined(AVEL_AVX512DQ)
        return !mask16x32f{_mm512_fpclass_ps_mask(decay(v), 0xBF)};

        #elif defined(AVEL_AVX512F)
        auto abs_v = avel::abs(v);
        return (vec16x32f{FLT_MIN} <= abs_v) && (abs_v <= vec16x32f{FLT_MAX});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f signbit(vec16x32f v) {
        #if defined(AVEL_AVX512DQ)
        return mask16x32f{_mm512_fpclass_ps_mask(decay(v), 0x40 | 0x04 | 0x10)};

        #elif defined(AVEL_AVX512F)
        return mask16x32f{_mm512_cmplt_epi32_mask(_mm512_castps_si512(decay(v)), _mm512_setzero_si512())};

        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask16x32f isgreater(vec16x32f x, vec16x32f y) {
        return mask16x32f{_mm512_cmp_ps_mask(decay(x), decay(y), _CMP_GT_OQ)};
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isgreaterequal(vec16x32f x, vec16x32f y) {
        return mask16x32f{_mm512_cmp_ps_mask(decay(x), decay(y), _CMP_GE_OQ)};
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isless(vec16x32f x, vec16x32f y) {
        return mask16x32f{_mm512_cmp_ps_mask(decay(x), decay(y), _CMP_LT_OQ)};
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f islessequal(vec16x32f x, vec16x32f y) {
        return mask16x32f{_mm512_cmp_ps_mask(decay(x), decay(y), _CMP_LE_OQ)};
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f islessgreater(vec16x32f x, vec16x32f y) {
        return mask16x32f{_mm512_cmp_ps_mask(decay(x), decay(y), _CMP_NEQ_OQ)};
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isunordered(vec16x32f x, vec16x32f y) {
        return mask16x32f{_mm512_cmp_ps_mask(decay(x), decay(y), _CMP_UNORD_Q)};
    }

}
