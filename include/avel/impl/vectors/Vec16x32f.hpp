namespace avel {

    using vec16x32f = Vector<float, 16>;

    using mask16x32f = Vector_mask<float, 16>;

    AVEL_FINL vec16x32f trunc(vec16x32f x);

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

        //=================================================
        // Constructor
        //=================================================

        //AVEL_FINL explicit Vector_mask(Vector_mask<std::uint32_t, 16> v);

        //AVEL_FINL explicit Vector_mask(Vector_mask<std::int32_t, 16> v);

        AVEL_FINL explicit Vector_mask(const primitive contents):
            content(contents) {}

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

        AVEL_FINL bool operator==(Vector_mask rhs) {
            return _mm512_mask2int(content) == _mm512_mask2int(rhs.content);
        }

        AVEL_FINL bool operator!=(Vector_mask rhs) {
            return _mm512_mask2int(content) != _mm512_mask2int(rhs.content);
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

        AVEL_FINL Vector_mask operator~() const {
            return Vector_mask{_knot_mask16(content)};
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            return Vector_mask{_kand_mask16(content, rhs.content)};
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            return Vector_mask{_kor_mask16(content, rhs.content)};
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            return Vector_mask{_kxor_mask16(content, rhs.content)};
        }

        //=================================================
        // Accessors
        //=================================================

        AVEL_FINL bool operator[](int i) const {
            int m = _mm512_mask2int(content);
            return m & (1 << i);
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
            static const primitive full_masks[2] {
                _mm512_int2mask(0),
                _mm512_int2mask(-1)
            };

            return full_masks[x];
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    std::uint32_t count(mask16x32f m) {
        return popcount(_mm512_mask2int(m));
    }

    bool any(mask16x32f m) {
        return _mm512_mask2int(m);
    }

    bool all(mask16x32f m) {
        return 0xFFFF == _mm512_mask2int(m);
    }


    template<>
    class alignas(sizeof(float) * 16)Vector<float, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 16;

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

        AVEL_FINL explicit Vector(vec16x32u v);

        AVEL_FINL explicit Vector(vec16x32i v):
            content(_mm512_cvtepi32_ps(v)) {}

        AVEL_FINL explicit Vector(mask m):
            content(_mm512_mask_blend_ps(m, _mm512_setzero_ps(), _mm512_set1_ps(1.0f))) {}

        /*
        AVEL_FINL explicit Vector(
            float a, float b, float c, float d,
            float e, float f, float g, float h,
            float i, float j, float k, float l,
            float m, float n, float o, float p
            ):
            content(_mm512_setr_ps(
                a, b, c, d,
                e, f, g, h,
                i, j, k, l,
                m, n, o, p
            )) {}
        */

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm512_set1_ps(x)) {}

        AVEL_FINL explicit Vector(const scalar* x):
            content(_mm512_loadu_ps(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& a):
            content(_mm512_loadu_ps(a.data())) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            return Vector{_mm512_setzero_ps()};
        }

        AVEL_FINL static Vector ones() {
            __m512i ones = _mm512_set1_epi64(-1);
            return Vector{_mm512_castsi512_ps(ones)};
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

        AVEL_FINL Vector& operator=(scalar x) {
            content = _mm512_set1_ps(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_EQ_OQ)};
        }

        AVEL_FINL mask operator!=(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_NEQ_OQ)};
        }

        AVEL_FINL mask operator<(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LT_OQ)};
        }

        AVEL_FINL mask operator<=(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LE_OQ)};
        }

        AVEL_FINL mask operator>(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GT_OQ)};
        }

        AVEL_FINL mask operator>=(Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GE_OQ)};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            return zeros() - *this;
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

        AVEL_FINL Vector& operator%=(Vector vec);

        //=================================================
        // Arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+(Vector vec) const {
            return Vector{_mm512_add_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator-(Vector vec) const {
            return Vector{_mm512_sub_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator*(Vector vec) const {
            return Vector{_mm512_mul_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator/(Vector vec) const {
            return Vector{_mm512_div_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator%(Vector vec) const {
            return Vector{_mm512_sub_ps(content, _mm512_mul_ps(trunc(*this / vec), vec))};
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

        AVEL_FINL Vector operator&=(Vector vec) {
            #if defined(AVEL_AVX512DQ)
            content = _mm512_and_ps(content, vec.content);
            #else
            auto a = _mm512_castps_si512(content);
            auto b = _mm512_castps_si512(vec.content);
            content = _mm512_castsi512_ps(_mm512_and_si512(a, b));
            #endif
            return *this;
        }

        AVEL_FINL Vector operator|=(Vector vec) {
            #if defined(AVEL_AVX512DQ)
            content = _mm512_or_ps(content, vec.content);
            #else
            auto a = _mm512_castps_si512(content);
            auto b = _mm512_castps_si512(vec.content);
            content = _mm512_castsi512_ps(_mm512_or_si512(a, b));
            #endif
            return *this;
        }

        AVEL_FINL Vector operator^=(Vector vec) {
            #if defined(AVEL_AVX512DQ)
            content = _mm512_xor_ps(content, vec.content);
            #else
            auto a = _mm512_castps_si512(content);
            auto b = _mm512_castps_si512(vec.content);
            content = _mm512_castsi512_ps(_mm512_xor_si512(a, b));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            return Vector{_mm512_andnot_ps(ones().content, content)};
        }

        AVEL_FINL Vector operator&(Vector vec) const {
            #if defined(AVEL_AVX512DQ)
            return Vector{_mm512_and_ps(content, vec.content)};
            #else
            auto a = _mm512_castps_si512(content);
            auto b = _mm512_castps_si512(vec.content);
            return Vector{_mm512_castsi512_ps(_mm512_and_si512(a, b))};
            #endif
        }

        AVEL_FINL Vector operator|(Vector vec) const {
            #if defined(AVEL_AVX512DQ)
            return Vector{_mm512_or_ps(content, vec.content)};
            #else
            auto a = _mm512_castps_si512(content);
            auto b = _mm512_castps_si512(vec.content);
            return Vector{_mm512_castsi512_ps(_mm512_or_si512(a, b))};
            #endif
        }

        AVEL_FINL Vector operator^(Vector vec) const {
            #if defined(AVEL_AVX512DQ)
            return Vector{_mm512_xor_ps(content, vec.content)};
            #else
            auto a = _mm512_castps_si512(content);
            auto b = _mm512_castps_si512(vec.content);
            return Vector{_mm512_castsi512_ps(_mm512_xor_si512(a, b))};
            #endif
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar, width> array{};

            _mm512_store_ps(array.data(), content);

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

    AVEL_FINL vec16x32u::Vector(vec16x32f c):
        content(_mm512_cvtps_epu32(c)) {}

    AVEL_FINL vec16x32i::Vector(vec16x32f c):
        content(_mm512_cvtps_epi32(c)) {}

    template<>
    AVEL_FINL vec16x32f bit_cast<vec16x32f, vec16x32u>(const vec16x32u& v) {
        return vec16x32f{_mm512_castsi512_ps(v)};
    }

    template<>
    AVEL_FINL vec16x32u bit_cast<vec16x32u, vec16x32f>(const vec16x32f& v) {
        return vec16x32u{_mm512_castps_si512(v)};
    }

    template<>
    AVEL_FINL vec16x32f bit_cast<vec16x32f, vec16x32i>(const vec16x32i& v) {
        return vec16x32f{_mm512_castsi512_ps(v)};
    }

    template<>
    AVEL_FINL vec16x32i bit_cast<vec16x32i, vec16x32f>(const vec16x32f& v) {
        return vec16x32i{_mm512_castps_si512(v)};
    }

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec16x32f::mask isnan(vec16x32f v);

    //=====================================================
    // General float vector operations
    //=====================================================

    AVEL_FINL vec16x32f blend(vec16x32f a, vec16x32f b, vec16x32f::mask m) {
        return vec16x32f{_mm512_mask_blend_ps(m, a, b)};
    }

    AVEL_FINL vec16x32f max(vec16x32f a, vec16x32f b) {
        return vec16x32f{_mm512_max_ps(a, b)};
    }

    AVEL_FINL vec16x32f min(vec16x32f a, vec16x32f b) {
        return vec16x32f{_mm512_min_ps(a, b)};
    }

    AVEL_FINL vec16x32f clamp(vec16x32f x, vec16x32f lo, vec16x32f hi) {
        return vec16x32f{min(max(x, lo), hi)};
    }

    AVEL_FINL vec16x32f abs(vec16x32f v) {
        return vec16x32f{_mm512_abs_ps(v)};
    }

    template<>
    AVEL_FINL vec16x32f load<vec16x32f>(const float* ptr) {
        return vec16x32f{_mm512_loadu_ps(ptr)};
    }

    template<>
    AVEL_FINL vec16x32f aligned_load<vec16x32f>(const float* ptr) {
        return vec16x32f{_mm512_load_ps(ptr)};
    }

    template<>
    AVEL_FINL vec16x32f stream_load<vec16x32f>(const float* ptr) {
        return vec16x32f{_mm512_castsi512_ps(_mm512_stream_load_si512((void*) ptr))};
    }

    template<>
    AVEL_FINL vec16x32f gather<vec16x32f>(const float* ptr, vec16x32i o) {
        return vec16x32f{_mm512_i32gather_ps(o, ptr, sizeof(float))};
    }

    template<>
    AVEL_FINL vec16x32f broadcast<vec16x32f>(float x) {
        return vec16x32f{_mm512_set1_ps(x)};
    }

    AVEL_FINL void store(float* ptr, vec16x32f v) {
        _mm512_storeu_ps(ptr, v);
    }

    AVEL_FINL void aligned_store(float* ptr, vec16x32f v) {
        _mm512_store_ps(ptr, v);
    }

    AVEL_FINL void stream_store(float* ptr, vec16x32f v) {
        _mm512_stream_ps(ptr, v);
    }

    AVEL_FINL void scatter(float* ptr, vec16x32i o, vec16x32f v) {
        _mm512_i32scatter_ps(ptr, o, v, sizeof(float));
    }

    //=====================================================
    // Floating-point vector operations
    //=====================================================

    AVEL_FINL vec16x32f average(vec16x32f a, vec16x32f b) {
        static const auto half = vec16x32f{0.5f};
        return (a - a * half) + (b * half);
    }

    AVEL_FINL vec16x32f epsilon_increment(vec16x32f v);

    AVEL_FINL vec16x32f epsilon_decrement(vec16x32f v);

    AVEL_FINL vec16x32f epsilon_offset(vec16x32f v, vec16x32u o);

    //=====================================================
    // cmath basic operations
    //=====================================================

    AVEL_FINL vec16x32f fabs(vec16x32f v) {
        return abs(v);
    }

    AVEL_FINL vec16x32f fabsf(vec16x32f v) {
        return fabs(v);
    }

    AVEL_FINL vec16x32f fmod(vec16x32f x, vec16x32f y) {
        return x % y;
    }

    AVEL_FINL vec16x32f fmodf(vec16x32f x, vec16x32f y) {
        return fmod(x, y);
    }

    AVEL_FINL vec16x32f remainderf(vec16x32f v);

    AVEL_FINL vec16x32f remquof(vec16x32f v);

    AVEL_FINL vec16x32f fmadd(vec16x32f m, vec16x32f x, vec16x32f b) {
        return vec16x32f{_mm512_fmadd_ps(m, x, b)};
    }

    AVEL_FINL vec16x32f fmsub(vec16x32f m, vec16x32f x, vec16x32f b) {
        return vec16x32f{_mm512_fmsub_ps(m, x, b)};
    }

    AVEL_FINL vec16x32f fnmadd(vec16x32f m, vec16x32f x, vec16x32f b) {
        return vec16x32f{_mm512_fnmadd_ps(m, x, b)};
    }

    AVEL_FINL vec16x32f fnmsub(vec16x32f m, vec16x32f x, vec16x32f b) {
        return vec16x32f{_mm512_fnmsub_ps(m, x, b)};
    }

    AVEL_FINL vec16x32f fma(vec16x32f m, vec16x32f x, vec16x32f b) {
        return fmadd(m, x, b);
    }

    AVEL_FINL vec16x32f fmaf(vec16x32f m, vec16x32f x, vec16x32f b) {
        return fma(m, x, b);
    }

    AVEL_FINL vec16x32f fmax(vec16x32f a, vec16x32f b) {
        //TODO: Handle case with two NaNs
        return max(a, max(b, vec16x32f(INFINITY)));
    }

    AVEL_FINL vec16x32f fminf(vec16x32f a, vec16x32f b) {
        //TODO: Handle case with two NaNs
        return min(a, min(b, vec16x32f(INFINITY)));
    }

    AVEL_FINL vec16x32f fmin(vec16x32f a, vec16x32f b) {
        return fminf(a, b);
    }

    AVEL_FINL vec16x32f fdim(vec16x32f a, vec16x32f b) {
        //TODO: Make faster?
        auto tmp = max(a - b, vec16x32f{0.0f});
        auto nan_mask = isnan(a) | isnan(b);
        return blend(tmp, vec16x32f{NAN}, nan_mask);
    }

    AVEL_FINL vec16x32f lerp(vec16x32f a, vec16x32f b, vec16x32f t);

    //=====================================================
    // Exponential functions
    //=====================================================

    AVEL_FINL vec16x32f exp(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::exp(t);
        }

        return aligned_load<vec16x32f>(array.data());

        /*
        alignas(32) static constexpr float data[12] = {
            1.0f / 479001600.0f,
            1.0f / 39916800.0f,
            1.0f / 3628800.0f,
            1.0f / 362880.0f,
            1.0f / 40320.0f,
            1.0f / 5040.0f,
            1.0f / 720.0f,
            1.0f / 120.0f,
            1.0f / 24.0f,
            1.0f / 6.0f,
            1.0f / 2.0f,
            1.0f / 1.0f,
        };

        auto abs_x = abs(x);

        auto t0 = vec16x32f (data[0]);
        auto t1 = fmadd(abs_x, vec16x32f (data[1]), t0);
        auto t2 = fmadd(abs_x, vec16x32f(data[2]), t1);
        auto t3 = fmadd(abs_x, vec16x32f(data[3]), t2);
        auto t4 = fmadd(abs_x, vec16x32f(data[4]), t3);
        auto t5 = fmadd(abs_x, vec16x32f(data[5]), t4);
        auto t6 = fmadd(abs_x, vec16x32f(data[6]), t5);
        auto t7 = fmadd(abs_x, vec16x32f(data[7]), t6);
        auto t8 = fmadd(abs_x, vec16x32f(data[8]), t7);
        auto t9 = fmadd(abs_x, vec16x32f(data[9]), t8);
        auto t10 = fmadd(abs_x, vec16x32f(data[10]), t9);
        auto t11 = fmadd(abs_x, vec16x32f(data[11]), t10);

        auto recip = vec16x32f(data[11]) / t11;

        auto mask = x < vec16x32f{0.0f};
        return blend(recip, t11, mask);
        */
    }

    AVEL_FINL vec16x32f exp2(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::exp2(t);
        }

        return aligned_load<vec16x32f>(array.data());

        /*
        return exp(x * vec16x32f{69314718056.0f});
        */
    }

    AVEL_FINL vec16x32f expm1(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::expm1(t);
        }

        return aligned_load<vec16x32f>(array.data());
    }

    AVEL_FINL vec16x32f log(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log(t);
        }

        return aligned_load<vec16x32f>(array.data());
    }

    AVEL_FINL vec16x32f log10(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log10(t);
        }

        return aligned_load<vec16x32f>(array.data());
    }

    AVEL_FINL vec16x32f log2(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log2(t);
        }

        return aligned_load<vec16x32f>(array.data());

        /*
        __m512 approximation = _mm512_setzero_ps();

        //TODO: Complete implementation

        return vec16x32f{_mm512_add_ps(_mm512_getexp_ps(x), approximation)};
        */
    }

    AVEL_FINL vec16x32f log1p(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log1p(t);
        }

        return aligned_load<vec16x32f>(array.data());
    }

    //=====================================================
    // Power functions
    //=====================================================

    AVEL_FINL vec16x32f pow(vec16x32f base, vec16x32f exp) {
        //TODO: Replace with vectorized implementation
        auto base_array = base.as_array();
        auto exp_array = exp.as_array();

        for (unsigned i = 0; i < vec16x32f::width; ++i) {
            base_array[i] = std::pow(base_array[i], exp_array[i]);
        }

        return aligned_load<vec16x32f>(base_array.data());
    }

    AVEL_FINL vec16x32f sqrt(vec16x32f x) {
        return vec16x32f{_mm512_sqrt_ps(x)};
    }

    AVEL_FINL vec16x32f cbrt(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::cbrt(t);
        }

        return aligned_load<vec16x32f>(array.data());
    }

    AVEL_FINL vec16x32f hypot(vec16x32f x, vec16x32f y) {
        //TODO: Replace with vectorized implementation
        auto x_array = x.as_array();
        auto y_array = y.as_array();

        for (unsigned i = 0; i < vec16x32f::width; ++i) {
            x_array[i] = std::hypot(x_array[i], y_array[i]);
        }

        return aligned_load<vec16x32f>(x_array.data());
    }

    //=====================================================
    // Trigonometric functions
    //=====================================================

    AVEL_FINL std::array<vec16x32f, 2> sincos(vec16x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array0 = angle.as_array();
        auto array1 = angle.as_array();

        for (unsigned i = 0; i < vec16x32f::width; ++i) {
            #if defined(AVEL_GCC)
            sincosf(array0[i], array0.data() + i, array1.data() + i);
            #elif defined(AVEL_CLANG)
            sincosf(array0[i], array0.data() + i, array1.data() + i);
            #else
            static_assert(false, "Default not implemented");
            #endif
        }

        return {
            aligned_load<vec16x32f>(array0.data()),
            aligned_load<vec16x32f>(array1.data())
        };

        /*
        alignas(32) static const float constants0[8] {
            bit_cast<float>(std::uint32_t(0x7FFFFFFF)),
            0.5f, //One half
            6.283185307179586f, // Two pi
            1.0f / (6.283185307179586f), //one over two pi
            0.78539816339744830f, //1 quarter pi
            1.57079632679489661f, //1 half pi
            1.0f, //One
            0.0f //Zero
        };

        __m512 abs_angle = _mm512_abs_ps(angle);

        //Mods input angle to [-pi, pi) range
        const __m512 half       = _mm512_set1_ps(constants0[0x01]);
        const __m512 two_pi     = _mm512_set1_ps(constants0[0x02]);
        const __m512 rcp_two_pi = _mm512_set1_ps(constants0[0x03]);

        __m512 a = _mm512_fnmadd_ps(two_pi, _mm512_roundscale_ps(_mm512_fmadd_ps(abs_angle, rcp_two_pi, half), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), abs_angle);

        __m512 abs_a = _mm512_abs_ps(a);

        __m512 quart_pi = _mm512_set1_ps(constants0[0x04]);
        __m512 half_pi  = _mm512_set1_ps(constants0[0x05]);
         __m512 three_quart_pi = _mm512_add_ps(half_pi, quart_pi);

        // Contain the sign of the final return value.
        __mmask16 c_signs = _mm512_cmp_ps_mask(half_pi, abs_a, _CMP_LT_OQ);
        __mmask16 s_signs = _kxor_mask16(
            _mm512_cmp_ps_mask(a, _mm512_setzero_ps(), _CMP_LT_OQ),
            _mm512_cmp_ps_mask(angle, _mm512_setzero_ps(), _CMP_LT_OQ)
        );

        // Swap results of Taylor series later if false
        // Checks to see if angle is more than 45 degrees off the x-axis
        auto approx_mask = _mm512_cmp_ps_mask(
            _mm512_sub_ps(_mm512_abs_ps(_mm512_sub_ps(abs_a, half_pi)), quart_pi),
            _mm512_setzero_ps(),
            _CMP_GT_OQ
        );

        //TODO: Use the following constant in deciding which approximation to use
        //0.88394194 Boundary between approximation

        // Compute offset that's added to angle to bring it into the range of
        // [-pi/4, pi/4] which the Taylor series are designed to approximate
        auto add_half0 = _mm512_cmp_ps_mask(quart_pi, abs_a, _CMP_LT_OQ);
        auto add_half1 = _mm512_cmp_ps_mask(three_quart_pi, abs_a, _CMP_LT_OQ);

        __m512 angle_offset = _mm512_setzero_ps();
        angle_offset = _mm512_mask_add_ps(angle_offset, add_half0, angle_offset, half_pi);
        angle_offset = _mm512_mask_add_ps(angle_offset, add_half1, angle_offset, half_pi);

        // Compute exponentiations of adjusted angle
        __m512 x = _mm512_sub_ps(abs_a, angle_offset);
        __m512 xx = _mm512_mul_ps(x, x);
        __m512 xxxx = _mm512_mul_ps(xx, xx);
        __m512 xxxxxx = _mm512_mul_ps(xx, xxxx);
        __m512 xxxxxxxx = _mm512_mul_ps(xxxx, xxxx);

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
        const __m512 sd0 = _mm512_set1_ps(constants1[0x01]);
        const __m512 sd1 = _mm512_set1_ps(constants1[0x03]);
        const __m512 sd2 = _mm512_set1_ps(constants1[0x05]);
        const __m512 sd3 = _mm512_set1_ps(constants1[0x07]);

        // Sum sine terms
        __m512 st1 = _mm512_set1_ps(constants0[0x06]);
        __m512 st2 = _mm512_fmadd_ps(sd0, xx, st1);
        __m512 st3 = _mm512_fmadd_ps(sd1, xxxx, st2);
        __m512 st4 = _mm512_fmadd_ps(sd2, xxxxxx, st3);
        __m512 st5 = _mm512_fmadd_ps(sd3, xxxxxxxx, st4);

        __m512 sin = _mm512_mul_ps(x, st5);

        // Denominators for cosine terms
        const __m512 cd0 = _mm512_set1_ps(constants1[0x00]);
        const __m512 cd1 = _mm512_set1_ps(constants1[0x02]);
        const __m512 cd2 = _mm512_set1_ps(constants1[0x04]);
        const __m512 cd3 = _mm512_set1_ps(constants1[0x06]);

        // Sum cosine terms
        __m512 ct1 = _mm512_set1_ps(constants0[0x06]);
        __m512 ct2 = _mm512_fmadd_ps(cd0, xx, ct1);
        __m512 ct3 = _mm512_fmadd_ps(cd1, xxxx, ct2);
        __m512 ct4 = _mm512_fmadd_ps(cd2, xxxxxx, ct3);
        __m512 ct5 = _mm512_fmadd_ps(cd3, xxxxxxxx, ct4);

        __m512 cos = ct5;

        //Swap bits depending on approx_mask
        __m512 ret_sin = _mm512_mask_blend_ps(approx_mask, cos, sin);
        __m512 ret_cos = _mm512_mask_blend_ps(approx_mask, sin, cos);

        // Set sine sign bits
        ret_sin = _mm512_abs_ps(ret_sin);
        ret_sin = _mm512_mask_sub_ps(ret_sin, s_signs, _mm512_setzero_ps(), ret_sin);

        // Set cosine sign bits
        ret_cos = _mm512_abs_ps(ret_cos);
        ret_cos = _mm512_mask_sub_ps(ret_cos, c_signs, _mm512_setzero_ps(), ret_cos);

        return {vec16x32f{ret_sin}, vec16x32f{ret_cos}};
        */
    }

    AVEL_FINL vec16x32f sin(vec16x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array = angle.as_array();

        for (auto& t : array) {
            t = std::sin(t);
        }

        return aligned_load<vec16x32f>(array.data());

        /*
        alignas(64) static const float constants[16] {
            bit_cast<float>(std::uint32_t(0x7FFFFFFF)),
            0.5f, //One half
            6.283185307179586f, // Two pi
            1.0f / (6.283185307179586f), //one over two pi
            0.78539816339744830f, //1 quarter pi
            1.57079632679489661f, //1 half pi
            0.8839419395, // Threshold value
            1.0f / 1.0f, // 1/1!
            1.0f / 6.0f, // 1/3!
            1.0f / 120.0f, // 1/5!
            1.0f / 5040.0f, // 1/7!
            1.0f / 362880.0f, // 1/9!
            1.0f / 39916800.0f, // 1/11!
            1.0f / 6227020800.0f // 1/13!
        };

        auto abs_angle = abs(angle);

        //Remaps input angle to [-pi, pi) range
        vec16x32f half{constants[0x01]};
        vec16x32f two_pi{constants[0x02]};
        vec16x32f rcp_two_pi{constants[0x03]};
        vec16x32f x = fmadd(two_pi, trunc(fmadd(abs_angle, rcp_two_pi, half)), abs_angle);

        vec16x32f u = x * x;

        vec16x32f ret = vec16x32f{constants[13]};
        ret = fnmadd(u, ret, vec16x32f{constants[12]});
        ret = fnmadd(u, ret, vec16x32f{constants[11]});
        ret = fnmadd(u, ret, vec16x32f{constants[10]});
        ret = fnmadd(u, ret, vec16x32f{constants[9]});
        ret = fnmadd(u, ret, vec16x32f{constants[8]});
        ret = fnmadd(u, ret, vec16x32f{constants[7]});

        return ret * x;
        */
    }

    AVEL_FINL vec16x32f cos(vec16x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array = angle.as_array();

        for (auto& t : array) {
            t = std::cos(t);
        }

        return aligned_load<vec16x32f>(array.data());
    }

    AVEL_FINL vec16x32f tan(vec16x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array = angle.as_array();

        for (auto& t : array) {
            t = std::tan(t);
        }

        return aligned_load<vec16x32f>(array.data());
    }

    AVEL_FINL vec16x32f asin(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::asin(t);
        }

        return aligned_load<vec16x32f>(array.data());

        /*
        //TODO: More accurate implementation
        // https://wrf.ecse.rpi.edu/Research/Short_Notes/arcsin/fastsqrt.html
        alignas(32) static constexpr float constants[8] {
            -0.5860008050f,
            +1.5719451050f,
            -1.6696689770f,
            +0.8999841642f,
            -0.6575341673f,
            +1.0123866490f,
            +0.9998421793f,
            0.0f
        };

        vec16x32f offset = sqrt(vec16x32f{1.0f} - (x * x));

        x += offset;

        vec16x32f t0 = fmadd(x, vec16x32f{constants[0]}, vec16x32f{constants[1]});
        vec16x32f t1 = fmadd(x, t0, vec16x32f{constants[2]});
        vec16x32f t2 = fmadd(x, t1, vec16x32f{constants[3]});
        vec16x32f t3 = fmadd(x, t2, vec16x32f{constants[4]});
        vec16x32f t4 = fmadd(x, t3, vec16x32f{constants[5]});
        vec16x32f t5 = fmadd(x, t4, vec16x32f{constants[6]});

        return (t5 - offset);
        */
    }

    AVEL_FINL vec16x32f acos(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::acos(t);
        }

        return aligned_load<vec16x32f>(array.data());

        /*
        //TODO: More accurate approximation
        //TODO: Cheaper implementation
        return asin(-x) - vec16x32f{1.57079632679f};
        */
    }

    AVEL_FINL vec16x32f atan(vec16x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::atan(t);
        }

        return aligned_load<vec16x32f>(array.data());
    }

    AVEL_FINL vec16x32f atan2(vec16x32f x, vec16x32f y);

    //=====================================================
    // Nearest Integer Floating-point operators
    //=====================================================

    AVEL_FINL vec16x32f ceil(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(v, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec16x32f floor(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(v, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec16x32f trunc(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(v, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec16x32f round(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(v, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec16x32f nearbyint(vec16x32f v) {
        return vec16x32f{_mm512_roundscale_ps(v, _MM_FROUND_CUR_DIRECTION)};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    AVEL_FINL vec16x32f frexp(vec16x32f x, vec16x32i* exp);

    AVEL_FINL vec16x32f ldexp(vec16x32f x, vec16x32f exp);

    AVEL_FINL vec16x32f modf(vec16x32f x, vec16x32f* iptr);

    AVEL_FINL vec16x32f scalebn(vec16x32f x, vec16x32f exp);

    AVEL_FINL vec16x32f ilog(vec16x32f x);

    AVEL_FINL vec16x32f logb(vec16x32f x);

    AVEL_FINL vec16x32f copysign(vec16x32f mag, vec16x32f sign) {
        alignas(16) std::uint32_t mask_data[1] = {
            0x80000000
        };

        auto mask = vec16x32f{bit_cast<float>(mask_data[0])};

        //TODO: Pure AVX512F implementation
        return mask & sign | vec16x32f{_mm512_andnot_ps(mask, mag)};
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    AVEL_FINL vec16x32u fpclassify(vec16x32f v);

    AVEL_FINL vec16x32f::mask isnan(vec16x32f v) {
        return (v != v);
    }

    AVEL_FINL vec16x32f::mask isfininte(vec16x32f v) {
        return  vec16x32f{_mm512_getexp_ps(v)} != vec16x32f{255.0f};
    }

    AVEL_FINL vec16x32f::mask isnormal(vec16x32f v) {
        vec16x32f tmp = vec16x32f{_mm512_getexp_ps(v)};
        return  (tmp != vec16x32f::zeros()) & (tmp != vec16x32f{255.0f});
    }

    AVEL_FINL vec16x32f::mask isunordered(vec16x32f x, vec16x32f y) {
        return isnan(x) | isnan(y);
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    AVEL_FINL vec16x32f::mask isgreater(vec16x32f x, vec16x32f y) {
        return x > y;
    }

    AVEL_FINL vec16x32f::mask isgreaterequal(vec16x32f x, vec16x32f y) {
        return x >= y;
    }

    AVEL_FINL vec16x32f::mask isless(vec16x32f x, vec16x32f y) {
        return x < y;
    }

    AVEL_FINL vec16x32f::mask islessequal(vec16x32f x, vec16x32f y) {
        return x <= y;
    }

    AVEL_FINL vec16x32f::mask islessgreater(vec16x32f x, vec16x32f y) {
        return x != y;
    }

}
