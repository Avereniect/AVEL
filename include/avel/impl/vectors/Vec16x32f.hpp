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
                "Implementation assumes bool occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = _mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data);

            #elif defined(AVEL_AVX512F)
            auto array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            auto widened_data = _mm512_cvtepi8_epi32(array_data);
            content = _mm512_cmplt_epu32_mask(_mm512_setzero_si512(), widened_data);

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
            content = _kand_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(const Vector_mask& rhs) {
            content = _kor_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(const Vector_mask& rhs) {
            content = _kxor_mask16(content, rhs.content);
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
        return popcount(decay(m));
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
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_EQ_OQ)};
        }

        [[nodiscard]]
        AVEL_FINL mask operator!=(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_NEQ_OQ)};
        }

        [[nodiscard]]
        AVEL_FINL mask operator<(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LT_OQ)};
        }

        [[nodiscard]]
        AVEL_FINL mask operator<=(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LE_OQ)};
        }

        [[nodiscard]]
        AVEL_FINL mask operator>(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GT_OQ)};
        }

        [[nodiscard]]
        AVEL_FINL mask operator>=(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GE_OQ)};
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
            return Vector{} - *this;
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
            content = fmod(*this, vec);
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
        AVEL_FINL operator primitive() const {
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
        return 0; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec16x32f x) {
        return false; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec16x32f x) {
        return false; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec16x32f x) {
        return false; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f keep(mask16x32f m, vec16x32f v) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f clear(mask16x32f m, vec16x32f v) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f blend(mask16x32f m, vec16x32f a, vec16x32f b) {
        return vec16x32f{_mm512_mask_blend_ps(decay(m), decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f max(vec16x32f a, vec16x32f b) {
        return vec16x32f{_mm512_max_ps(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f min(vec16x32f a, vec16x32f b) {
        return vec16x32f{_mm512_min_ps(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec16x32f, 2> minmax(vec16x32f a, vec16x32f b) {
        #if defined(AVEL_AVX)
        return std::array<vec16x32f, 2>{
            vec16x32f{_mm512_min_ps(decay(a), decay(b))},
            vec16x32f{_mm512_max_ps(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f clamp(vec16x32f x, vec16x32f lo, vec16x32f hi) {
        return vec16x32f{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f negate(mask16x32f m, vec16x32f v) {
        return{};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f abs(vec16x32f v) {
        #if defined(AVEL_AVX512F)
        return vec16x32f{_mm512_abs_ps(v)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f neg_abs(vec16x32f v) {
        #if defined(AVEL_AVX512F)
        return -abs(v);
        #endif
    }

    //=====================================================
    // Load/Store Operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f load<vec16x32f>(const float* ptr, std::uint32_t n) {
        return {}; //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f load<vec16x32f, vec16x32f::width>(const float* ptr) {
        return {}; //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f aligned_load<vec16x32f>(const float* ptr, std::uint32_t n) {
        return {}; //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f aligned_load<vec16x32f, vec16x32f::width>(const float* ptr) {
        return {}; //TODO: Implement
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f gather<vec16x32f>(const float* ptr, vec16x32i indices, std::uint32_t n) {
        return {}; //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32f gather<vec16x32f>(const float* ptr, vec16x32i indices) {
        #if defined(AVEL_AVX512F)
        return vec16x32f{_mm512_i32gather_ps(decay(indices), ptr, sizeof(float))};
        #endif
    }



    template<std::uint32_t N = vec16x32f::width>
    AVEL_FINL void store(float* ptr, vec16x32f v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void store<vec16x32f::width>(float* ptr, vec16x32f v) {
        //TODO: Implement
    }

    AVEL_FINL void store(float* ptr, vec16x32f v, std::uint32_t n) {
        //TODO: Implement
    }



    template<std::uint32_t N = vec16x32f::width>
    AVEL_FINL void aligned_store(float* ptr, vec16x32f v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void aligned_store<vec16x32f::width>(float* ptr, vec16x32f v) {
        
    }

    AVEL_FINL void aligned_store(float* ptr, vec16x32f v, std::uint32_t n) {
        //TODO: Implement
    }



    AVEL_FINL void scatter(float* ptr, vec16x32f v, vec16x32i indices, std::uint32_t n) {
        //TODO: Implement
    }

    template<std::uint32_t N = vec16x32f::width>
    AVEL_FINL void scatter(float* ptr, vec16x32f v, vec16x32i indices) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void scatter<0>(float* ptr, vec16x32f v, vec16x32i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec16x32f::width>(float* ptr, vec16x32f v, vec16x32i indices) {
        //TODO: Implement
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
    AVEL_FINL vec16x32f fabs(vec16x32f v) {
        return abs(v);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f fmadd(vec16x32f m, vec16x32f x, vec16x32f b) {
        return vec16x32f{_mm512_fmadd_ps(m, x, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f fmsub(vec16x32f m, vec16x32f x, vec16x32f b) {
        return vec16x32f{_mm512_fmsub_ps(m, x, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f fnmadd(vec16x32f m, vec16x32f x, vec16x32f b) {
        return vec16x32f{_mm512_fnmadd_ps(m, x, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f fnmsub(vec16x32f m, vec16x32f x, vec16x32f b) {
        return vec16x32f{_mm512_fnmsub_ps(m, x, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f fma(vec16x32f m, vec16x32f x, vec16x32f b) {
        return fmadd(m, x, b);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f fmaf(vec16x32f m, vec16x32f x, vec16x32f b) {
        return fma(m, x, b);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f fmax(vec16x32f a, vec16x32f b) {
        //TODO: Handle case with two NaNs
        return max(a, max(b, vec16x32f(INFINITY)));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f fminf(vec16x32f a, vec16x32f b) {
        //TODO: Handle case with two NaNs
        return min(a, min(b, vec16x32f(INFINITY)));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f fmin(vec16x32f a, vec16x32f b) {
        return fminf(a, b);
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32f sqrt(vec16x32f x) {
        return vec16x32f{_mm512_sqrt_ps(x)};
    }

    //=====================================================
    // Nearest Integer Floating-point operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32f ceil(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f floor(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f trunc(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(v, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f round(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f nearbyint(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(v, _MM_FROUND_CUR_DIRECTION)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f rint(vec16x32f v) {
        return {};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32f frexp(vec16x32f x, vec16x32i* exp)  {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f ldexp(vec16x32f x, vec16x32i exp)  {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f scalbn(vec16x32f x, vec16x32i exp)  {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i ilogb(vec16x32f x) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f logb(vec16x32f x) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec16x32f copysign(vec16x32f mag, vec16x32f sign) {
        #if defined(AVEL_AVX512F)
        auto mask = _mm512_set1_ps(float_sign_bit_mask);
        auto ret = _mm512_or_ps(_mm512_and_ps(mask, decay(sign)), _mm512_andnot_ps(mask, decay(mag)));
        return vec16x32f{ret};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32i fpclassify(vec16x32f v) {
        return {}; //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isinf(vec16x32f v) {
        #if defined(AVEL_AVX512F)
        return abs(v) == vec16x32f{INFINITY};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isnan(vec16x32f v) {
        return (v != v);
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isfinite(vec16x32f v) {
        return  vec16x32f{_mm512_getexp_ps(v)} != vec16x32f{255.0f};
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isnormal(vec16x32f v) {
        #if defined(AVEL_AVX512F)
        return !mask16x32f{_mm512_fpclass_ps_mask(decay(v), 0xBF)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f signbit(vec16x32f arg) {
        #if defined(AVEL_AVX512F)
        return mask16x32f{_mm512_fpclass_ps_mask(decay(arg), 0x40)};
        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask16x32f isgreater(vec16x32f x, vec16x32f y) {
        return x > y;
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isgreaterequal(vec16x32f x, vec16x32f y) {
        return x >= y;
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isless(vec16x32f x, vec16x32f y) {
        return x < y;
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f islessequal(vec16x32f x, vec16x32f y) {
        return x <= y;
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f islessgreater(vec16x32f x, vec16x32f y) {
        return x != y;
    }

    [[nodiscard]]
    AVEL_FINL mask16x32f isunordered(vec16x32f x, vec16x32f y) {
        return isnan(x) | isnan(y);
    }

}
