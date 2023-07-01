namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x32f = Vector<float, 8>;
    using arr8x32f = std::array<float, 8>;
    using mask8x32f = Vector_mask<float, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec8x32f trunc(vec8x32f v);
    AVEL_FINL mask8x32f isnan(vec8x32f v);





    template<>
    class Vector_mask<float, 8> {
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
        #elif defined(AVEL_AVX)
        using primitive = __m256;
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

        AVEL_FINL explicit Vector_mask(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL)
            content(b ? 0xff : 0x00) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr8xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si64(arr.data());
            content = static_cast<primitive>(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            auto expanded = _mm256_cvtepi8_epi32(array_data);
            content = _mm256_cmplt_epu32_mask(_mm256_setzero_si256(), expanded);
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

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return (_cvtmask16_u32(primitive(lhs)) == _cvtmask16_u32(primitive(rhs)));
            #elif defined(AVEL_AVX)
            return 0xFF == _mm256_movemask_ps(_mm256_cmp_ps(lhs, rhs, _CMP_EQ_OQ));
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return (_cvtmask16_u32(primitive(lhs)) != _cvtmask16_u32(primitive(rhs)));
            #elif defined(AVEL_AVX)
            return 0xFF != _mm256_movemask_ps(_mm256_cmp_ps(lhs, rhs, _CMP_EQ_OQ));
            #endif
        }

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

        [[nodiscard]]
        Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_knot_mask8(content)};
            #else
            return Vector_mask{_mm256_sub_ps(_mm256_setzero_ps(), content)};
            #endif
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

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask8x32f m) {
        #if defined(AVEL_AVX512VL)
        return popcount(decay(m));

        #elif defined(AVEL_SSE2)
        return popcount(_mm256_movemask_ps(m));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x32f m) {
        #if defined(AVEL_AVX512VL)
        return decay(m);

        #elif defined(AVEL_SSE2)
        return _mm256_movemask_ps(m);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask8x32f m) {
        #if defined(AVEL_AVX512VL)
        return 0xFF == decay(m);

        #elif defined(AVEL_SSE2)
        return 0xFF == _mm256_movemask_ps(m);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x32f m) {
        return !any(m);
    }




    template<>
    class Vector<float, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = float;

        using primitive = __m256;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm256_mask_blend_ps(decay(m), _mm256_setzero_ps(), _mm256_set1_ps(1.0f))) {}
        #else
            content(_mm256_blendv_ps(_mm256_setzero_ps(), _mm256_set1_ps(1.0f), m)) {}
        #endif

        /*
        AVEL_FINL explicit Vector(
            float a, float b, float c, float d,
            float e, float f, float g, float h):
            content(_mm256_setr_ps(a, b, c, d,e, f, g, h)) {}
        */

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm256_set1_ps(x)) {}

        AVEL_FINL explicit Vector(const scalar* x):
            content(_mm256_loadu_ps(x)) {}

        AVEL_FINL explicit Vector(const arr8x32f& a):
            content(_mm256_loadu_ps(a.data())) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(scalar x) {
            content = _mm256_set1_ps(x);
            return *this;
        }

        AVEL_FINL Vector& operator=(primitive p) {
            *this = Vector{p};
            return *this;
        }

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

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

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content = _mm256_add_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm256_sub_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm256_mul_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            content = _mm256_div_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
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
            #if defined(AVEL_AVX512VL)
            auto t = _mm256_castps_si256(content);
            return mask{_mm256_test_epi32_mask(t, t)};

            #elif defined(AVEL_AVX)
            return mask{_mm256_cmp_ps(content, _mm256_setzero_ps(), _CMP_NEQ_UQ)};

            #endif
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    static_assert(
        8 * sizeof(float) == sizeof(vec8x32f),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL float extract(vec8x32f v) {
        static_assert(N < vec8x32f::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x32f::width, int>::type dummy_variable = 0;

        auto quarter = _mm256_extractf128_ps(decay(v), N / 4);
        auto element = avel::bit_cast<float>(_mm_extract_ps(quarter, N % 4));

        return element;
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL vec8x32f insert(vec8x32f v, float x) {
        static_assert(N < vec8x32f::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x32f::width, int>::type dummy_variable = 0;

        auto broadcasted = _mm256_set1_ps(x);
        constexpr auto blend_mask = std::uint8_t(~(1 << N));
        auto ret = _mm256_mask_blend_ps(blend_mask, broadcasted, decay(v));
        return vec8x32f{ret};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec8x32f x) {
        return count(mask8x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec8x32f x) {
        return any(mask8x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec8x32f x) {
        return all(mask8x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec8x32f x) {
        return none(mask8x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f keep(mask8x32f m, vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32f{_mm256_maskz_mov_ps(decay(m), decay(v))};

        #elif defined(AVEL_AVX)
        return vec8x32f{_mm256_and_ps(decay(m), decay(v))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f clear(mask8x32f m, vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32f{_mm256_maskz_mov_ps(decay(!m), decay(v))};

        #elif defined(AVEL_AVX)
        return vec8x32f{_mm256_andnot_ps(decay(m), decay(v))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f blend(mask8x32f m, vec8x32f a, vec8x32f b) {
        #if defined(AVEL_AVX512VL)
        return vec8x32f{_mm256_mask_blend_ps(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_AVX)
        return vec8x32f{_mm256_blendv_ps(decay(a), decay(b), decay(m))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f max(vec8x32f a, vec8x32f b) {
        return vec8x32f{_mm256_max_ps(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f min(vec8x32f a, vec8x32f b) {
        return vec8x32f{_mm256_min_ps(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x32f, 2> minmax(vec8x32f a, vec8x32f b) {
        #if defined(AVEL_AVX)
        return std::array<vec8x32f, 2>{
            vec8x32f{_mm256_min_ps(decay(a), decay(b))},
            vec8x32f{_mm256_max_ps(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f clamp(vec8x32f x, vec8x32f lo, vec8x32f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f negate(mask8x32f m, vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32f{_mm256_mask_sub_ps(decay(v), decay(m), _mm256_setzero_ps(), decay(v))};

        #elif defined(AVEL_AVX)
        auto negation_mask = _mm256_and_ps(decay(m), _mm256_castsi256_ps(_mm256_set1_ps(float_sign_bit_mask)));
        return vec8x32f{_mm256_xor_ps(decay(v), negation_mask)};

        #endif
    }

    AVEL_FINL vec8x32f abs(vec8x32f v) {
        #if defined(AVEL_AVX)
        return vec8x32f{_mm256_andnot_ps(_mm256_set1_ps(float_sign_bit_mask), decay(v))};
        #endif
    }

    AVEL_FINL vec8x32f neg_abs(vec8x32f v) {
        #if defined(AVEL_AVX)
        return vec8x32f{_mm256_or_ps(_mm256_set1_ps(float_sign_bit_mask), decay(v))};
        #endif
    }

    //=====================================================
    // Load/Store Operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f load<vec8x32f>(const float* ptr, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32f{_mm256_maskz_loadu_ps(mask, ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f load<vec8x32f, vec8x32f::width>(const float* ptr) {
        return vec8x32f{_mm256_loadu_ps(ptr)};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f aligned_load<vec8x32f>(const float* ptr, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32f{_mm256_maskz_load_ps(mask, ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f aligned_load<vec8x32f, vec8x32f::width>(const float* ptr) {
        return vec8x32f{_mm256_load_ps(ptr)};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f gather<vec8x32f>(const float* ptr, vec8x32i indices, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32f{_mm256_mmask_i32gather_ps(_mm256_setzero_ps(), mask, decay(indices), ptr, sizeof(float))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f gather<vec8x32f>(const float* ptr, vec8x32i indices) {
        return vec8x32f{_mm256_i32gather_ps(ptr, decay(indices), sizeof(float))};
    }



    AVEL_FINL void store(float* ptr, vec8x32f v, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_storeu_ps(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec8x32f::width>
    AVEL_FINL void store(float* ptr, vec8x32f v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec8x32f::width>(float* ptr, vec8x32f v) {
        #if defined(AVEL_AVX512F)
        _mm256_storeu_ps(ptr, decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(float* ptr, vec8x32f v, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_store_ps(ptr, mask, decay(v));
    }

    template<std::uint32_t N = vec8x32f::width>
    AVEL_FINL void aligned_store(float* ptr, vec8x32f v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec8x32f::width>(float* ptr, vec8x32f v) {
        _mm256_store_ps(ptr, decay(v));
    }



    AVEL_FINL void scatter(float* ptr, vec8x32f v, vec8x32i indices, std::uint32_t n) {
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_i32scatter_ps(ptr, mask, decay(indices), decay(v), sizeof(float));
    }

    template<std::uint32_t N = vec8x32f::width>
    AVEL_FINL void scatter(float* ptr, vec8x32f v, vec8x32i indices) {
        scatter(ptr, v, indices, N);
    }

    template<>
    AVEL_FINL void scatter<0>(float* ptr, vec8x32f v, vec8x32i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec8x32f::width>(float* ptr, vec8x32f v, vec8x32i indices) {
        _mm256_i32scatter_ps(ptr, decay(indices), decay(v), sizeof(float));
    }



    [[nodiscard]]
    AVEL_FINL arr8x32f to_array(vec8x32f v) {
        alignas(32) std::array<float, 8> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // cmath basic operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x32f fmax(vec8x32f a, vec8x32f b) {
        return blend(avel::isnan(b), a, avel::max(a, b));
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fmin(vec8x32f a, vec8x32f b) {
        return blend(avel::isnan(b), a, avel::min(a, b));
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x32f sqrt(vec8x32f x) {
        return vec8x32f{_mm256_sqrt_ps(decay(x))};
    }

    //=====================================================
    // Nearest Integer Floating-point operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x32f ceil(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f floor(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f trunc(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(v, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f round(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f nearbyint(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(v, _MM_FROUND_CUR_DIRECTION)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f rint(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(v, _MM_FROUND_CUR_DIRECTION)};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x32f frexp(vec8x32f v, vec8x32i* exp) {
        #if defined(AVEL_AVX512VL)
        auto is_infinity = _mm256_fpclass_ps_mask(decay(v), 0x10 | 0x08);
        auto is_non_zero = _mm256_cmp_ps_mask(decay(v), _mm256_setzero_ps(), _CMP_NEQ_UQ);

        auto exponents = _mm256_getexp_ps(decay(v));
        exponents = _mm256_add_ps(exponents, _mm256_set1_ps(1.0f));
        *exp = _mm256_maskz_cvttps_epi32(is_non_zero, exponents);

        auto ret = _mm256_getmant_ps(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        ret = _mm256_maskz_mov_ps(is_non_zero, ret);
        ret = _mm256_mask_blend_ps(is_infinity, ret, decay(v));
        return vec8x32f{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f ldexp(vec8x32f x, vec8x32i exp) {
        return vec8x32f{_mm256_scalef_ps(decay(x), _mm256_cvtepi32_ps(decay(exp)))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f scalbn(vec8x32f x, vec8x32i exp) {
        return avel::ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i ilogb(vec8x32f x) {
        #if defined(AVEL_AVX512VL)
        auto exp_fp = _mm256_getexp_ps(decay(x));

        vec8x32f zero_ret{_mm256_castsi256_ps(_mm256_set1_epi32(FP_ILOGB0))};
        vec8x32f inf_ret {_mm256_castsi256_ps(_mm256_set1_epi32(INT_MAX))};
        vec8x32f nan_ret {_mm256_castsi256_ps(_mm256_set1_epi32(FP_ILOGBNAN))};

        auto misc_ret_i = _mm256_cvtps_epi32(exp_fp);
        misc_ret_i = _mm256_maskz_mov_epi32(_mm256_cmpneq_epi32_mask(misc_ret_i, _mm256_set1_epi32(0x80000000)), misc_ret_i);

        vec8x32i zero_ret_i{_mm256_castps_si256(_mm256_fixupimm_ps(zero_ret, exp_fp, _mm256_set1_epi32(0x88808888), 0x00))};
        vec8x32i inf_ret_i {_mm256_castps_si256(_mm256_fixupimm_ps(inf_ret,  exp_fp, _mm256_set1_epi32(0x88088888), 0x00))};
        vec8x32i nan_ret_i {_mm256_castps_si256(_mm256_fixupimm_ps(nan_ret,  exp_fp, _mm256_set1_epi32(0x88888800), 0x00))};

        return (vec8x32i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f logb(vec8x32f x) {
        #if defined(AVEL_AVX512VL)
        return vec8x32f{_mm256_getexp_ps(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f copysign(vec8x32f mag, vec8x32f sign) {
        #if defined(AVEL_AVX2)
        auto mask = _mm256_set1_ps(float_sign_bit_mask);
        auto ret = _mm256_or_ps(_mm256_and_ps(mask, decay(sign)), _mm256_andnot_ps(mask, decay(mag)));

        return vec8x32f{ret};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x32i fpclassify(vec8x32f v) {
        const vec8x32i fp_infinite{int(FP_INFINITE)};
        const vec8x32i fp_nan{int(FP_NAN)};
        const vec8x32i fp_normal{int(FP_NORMAL)};
        const vec8x32i fp_subnormal{int(FP_SUBNORMAL)};
        const vec8x32i fp_zero{int(FP_ZERO)};

        #if defined(AVEL_AVX512VL)
        mask8x32i infinite_mask {_mm256_fpclass_ps_mask(decay(v), 0x08 | 0x10)};
        mask8x32i nan_mask      {_mm256_fpclass_ps_mask(decay(v), 0x01 | 0x80)};
        mask8x32i subnormal_mask{_mm256_fpclass_ps_mask(decay(v), 0x20)};
        mask8x32i zero_mask     {_mm256_fpclass_ps_mask(decay(v), 0x02 | 0x04)};
        mask8x32i normal_mask   {!(infinite_mask | nan_mask | subnormal_mask | zero_mask)};

        return
            keep(infinite_mask, fp_infinite) |
            keep(nan_mask, fp_nan) |
            keep(normal_mask, fp_normal) |
            keep(subnormal_mask, fp_subnormal) |
            keep(zero_mask, fp_zero);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isinf(vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return mask8x32f{_mm256_fpclass_ps_mask(decay(v), 0x08 | 0x10)};

        #elif defined(AVEL_AVX2)
        return abs(v) == vec8x32f{INFINITY};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isnan(vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return mask8x32f{_mm256_fpclass_ps_mask(decay(v), 0x80 | 0x01)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isfinite(vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return vec8x32f{_mm256_getexp_ps(decay(v))} < vec8x32f{128.0f};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isnormal(vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return !mask8x32f{_mm256_fpclass_ps_mask(decay(v), 0xBF)};

        #elif defined(AVEL_AVX2)
        return mask8x32f{}; //TODO: Implement

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f signbit(vec8x32f arg) {
        #if defined(AVEL_AVX512VL)
        return mask8x32f{_mm256_fpclass_ps_mask(decay(arg), 0x40 | 0x04 | 0x10)};
        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask8x32f isgreater(vec8x32f x, vec8x32f y) {
        return x > y;
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isgreaterequal(vec8x32f x, vec8x32f y) {
        return x >= y;
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isless(vec8x32f x, vec8x32f y) {
        return x < y;
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f islessequal(vec8x32f x, vec8x32f y) {
        return x <= y;
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f islessgreater(vec8x32f x, vec8x32f y) {
        return x != y;
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isunordered(vec8x32f x, vec8x32f y) {
        #if defined(AVEL_AVX512VL)
        return mask8x32f{_mm256_cmp_ps_mask(decay(x), decay(y), _CMP_UNORD_Q)};

        #elif defined(AVEL_AVX2)
        return mask8x32f{_mm256_cmp_ps(decay(x), decay(y), _CMP_UNORD_Q)};
        #endif
    }

}
