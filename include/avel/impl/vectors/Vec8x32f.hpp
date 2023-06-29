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

        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return (_cvtmask16_u32(primitive(lhs)) == _cvtmask16_u32(primitive(rhs)));
            #elif defined(AVEL_AVX)
            return 0xFF == _mm256_movemask_ps(_mm256_cmp_ps(lhs, rhs, _CMP_EQ_OQ));
            #endif
        }

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

    AVEL_FINL std::uint32_t count(mask8x32f m) {
        #if defined(AVEL_AVX512VL)
        return popcount(decay(m));
        #elif defined(AVEL_SSE2)
        return popcount(_mm256_movemask_ps(m));
        #endif
    }

    AVEL_FINL bool any(mask8x32f m) {
        #if defined(AVEL_AVX512VL)
        return decay(m);
        #elif defined(AVEL_SSE2)
        return _mm256_movemask_ps(m);
        #endif
    }

    AVEL_FINL bool all(mask8x32f m) {
        #if defined(AVEL_AVX512VL)
        return 0xFF == decay(m);
        #elif defined(AVEL_SSE2)
        return 0xFF == _mm256_movemask_ps(m);
        #endif
    }

    AVEL_FINL bool none(mask8x32f m) {
        return !any(m);
    }




    template<>
    class alignas(sizeof(float) * 8) Vector<float, 8> {
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

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            return Vector(0.0) - *this;
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
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector operator&=(Vector rhs) {
            content = _mm256_and_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector operator|=(Vector rhs) {
            content = _mm256_or_ps(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector operator^=(Vector rhs) {
            content = _mm256_xor_ps(content, rhs.content);
            return *this;
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
            return mask{_mm256_test_epi64_mask(t, t)};

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
    AVEL_FINL float extract(vec8x32f v) {
        static_assert(N < vec8x32f::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x32f::width, int>::type dummy_variable = 0;

        return {};
    }

    template<std::uint32_t N>
    AVEL_FINL vec8x32f insert(vec8x32f v, float x) {
        static_assert(N < vec8x32f::width, "Specified index does not exist");
        typename std::enable_if<N < vec8x32f::width, int>::type dummy_variable = 0;

        return {};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec8x32f x) {
        return 0; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec8x32f x) {
        return false; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec8x32f x) {
        return false; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec8x32f x) {
        return false; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f keep(mask8x32f m, vec8x32f v) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f clear(mask8x32f m, vec8x32f v) {
        return {}; //TODO: Implement
    }

    AVEL_FINL vec8x32f blend(mask8x32f m, vec8x32f a, vec8x32f b) {
        #if defined(AVEL_AVX512VL)
        return vec8x32f{_mm256_mask_blend_ps(decay(m), decay(a), decay(b))};
        #else
        return vec8x32f{_mm256_blendv_ps(a, b, m)};
        #endif
    }

    AVEL_FINL vec8x32f max(vec8x32f a, vec8x32f b) {
        return vec8x32f{_mm256_max_ps(decay(a), decay(b))};
    }

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

    AVEL_FINL vec8x32f clamp(vec8x32f x, vec8x32f lo, vec8x32f hi) {
        return vec8x32f{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f negate(mask8x32f m, vec8x32f v) {
        return{};
    }

    AVEL_FINL vec8x32f abs(vec8x32f v) {
        return vec8x32f{}; //TODO: Implement
    }

    AVEL_FINL vec8x32f neg_abs(vec8x32f v) {

    }

    //=====================================================
    // Load/Store Operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f load<vec8x32f>(const float* ptr, std::uint32_t n) {
        return {}; //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f load<vec8x32f, vec8x32f::width>(const float* ptr) {
        return {}; //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f aligned_load<vec8x32f>(const float* ptr, std::uint32_t n) {
        return {}; //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f aligned_load<vec8x32f, vec8x32f::width>(const float* ptr) {
        return {}; //TODO: Implement
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f gather<vec8x32f>(const float* ptr, vec8x32i indices, std::uint32_t n) {
        return {}; //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f gather<vec8x32f>(const float* ptr, vec8x32i indices) {
        //TODO: Implement
    }



    template<std::uint32_t N = vec8x32f::width>
    AVEL_FINL void store(float* ptr, vec8x32f v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void store<vec8x32f::width>(float* ptr, vec8x32f v) {
        //TODO: Implement
    }

    AVEL_FINL void store(float* ptr, vec8x32f v, std::uint32_t n) {
        //TODO: Implement
    }



    template<std::uint32_t N = vec8x32f::width>
    AVEL_FINL void aligned_store(float* ptr, vec8x32f v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void aligned_store<vec8x32f::width>(float* ptr, vec8x32f v) {

    }

    AVEL_FINL void aligned_store(float* ptr, vec8x32f v, std::uint32_t n) {
        //TODO: Implement
    }



    AVEL_FINL void scatter(float* ptr, vec8x32f v, vec8x32i indices, std::uint32_t n) {
        //TODO: Implement
    }

    template<std::uint32_t N = vec8x32f::width>
    AVEL_FINL void scatter(float* ptr, vec8x32f v, vec8x32i indices) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void scatter<0>(float* ptr, vec8x32f v, vec8x32i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec8x32f::width>(float* ptr, vec8x32f v, vec8x32i indices) {
        //TODO: Implement
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
    AVEL_FINL vec8x32f fabs(vec8x32f v) {
        return abs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fabsf(vec8x32f v) {
        return fabs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fmadd(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fmadd_ps(decay(m), decay(x), decay(b))};
        #else
        return vec8x32f{m * x + b};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fmsubb(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fmsub_ps(decay(m), decay(x), decay(b))};
        #else
        return vec8x32f{m * x - b};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fnmadd(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fnmadd_ps(decay(m), decay(x), decay(b))};
        #else
        return vec8x32f{-m * x + b};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fnmsub(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fnmsub_ps(decay(m), decay(x), decay(b))};
        #else
        return vec8x32f{-m * x - b};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fma(vec8x32f m, vec8x32f x, vec8x32f b) {
        return fmadd(m, x, b);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fmaf(vec8x32f m, vec8x32f x, vec8x32f b) {
        return fma(m, x, b);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fmax(vec8x32f a, vec8x32f b) {
        //TODO: Handle case with two NaNs
        return max(a, max(b, vec8x32f(INFINITY)));
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fminf(vec8x32f a, vec8x32f b) {
        //TODO: Handle case with two NaNs
        return min(a, min(b, vec8x32f(INFINITY)));
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fmin(vec8x32f a, vec8x32f b) {
        return fminf(a, b);
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
    AVEL_FINL vec8x32f ceil(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f floor(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f trunc(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f round(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f nearbyint(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(v, _MM_FROUND_CUR_DIRECTION)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f rint(vec8x32f v) {

    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x32f frexp(vec8x32f v, vec8x32i* exp) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f ldexp(vec8x32f x, vec8x32i exp) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f scalbn(vec8x32f x, vec8x32i exp) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i ilogb(vec8x32f x) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f logb(vec8x32f x) {
        return {}; //TODO: Implement
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
        return vec8x32i{};
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isinf(vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return abs(v) == vec8x32f{INFINITY};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isnan(vec8x32f v) {
        return (v != v);
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isfinite(vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return  vec8x32f{_mm256_getexp_ps(decay(v))} != vec8x32f{255.0f};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isnormal(vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return !mask8x32f{_mm256_fpclass_ps_mask(decay(v), 0xBF)};

        #elif defined(AVEL_AVX2)

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f signbit(vec8x32f arg) {
        #if defined(AVEL_AVX512VL)
        return mask8x32f{_mm256_fpclass_ps_mask(decay(arg), 0x40)};
        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask8x32f isunordered(vec8x32f x, vec8x32f y) {
        return {}; //TODO: Implement
    }

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

}
