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

        //=================================================
        // Constructor
        //=================================================



        AVEL_FINL explicit Vector_mask(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 8>& arr); //TODO: Implement

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

    //=====================================================
    // Mask functions
    //=====================================================

    AVEL_FINL std::uint32_t count(mask8x32f m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(m));
        #elif defined(AVEL_SSE2)
        return popcount(_mm256_movemask_ps(m));
        #endif
    }

    AVEL_FINL bool any(mask8x32f m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return _mm256_movemask_ps(m);
        #endif
    }

    AVEL_FINL bool all(mask8x32f m) {
        #if defined(AVEL_AVX512VL)
        return 0xFF == _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return 0xFF == _mm256_movemask_ps(m);
        #endif
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
            content(_mm256_mask_blend_ps(m, _mm256_setzero_ps(), _mm256_set1_ps(1.0f))) {}
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

        AVEL_FINL explicit Vector(const std::array<scalar, width>& a):
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

        AVEL_FINL Vector& operator=(scalar x) {
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

        AVEL_FINL Vector operator+() const {
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
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            return Vector{_mm256_andnot_ps(ones().content, content)};
        }

        AVEL_FINL Vector operator&(Vector rhs) const {
            return Vector{_mm256_and_ps(content, rhs.content)};
        }

        AVEL_FINL Vector operator|(Vector rhs) const {
            return Vector{_mm256_or_ps(content, rhs.content)};
        }

        AVEL_FINL Vector operator^(Vector rhs) const {
            return Vector{_mm256_xor_ps(content, rhs.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar, width> array{};

            _mm256_store_ps(array.data(), content);

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

    AVEL_FINL vec8x32f max(vec8x32f a, vec8x32f b) {
        return vec8x32f{_mm256_max_ps(a, b)};
    }

    AVEL_FINL vec8x32f min(vec8x32f a, vec8x32f b) {
        return vec8x32f{_mm256_min_ps(a, b)};
    }

    AVEL_FINL vec8x32f clamp(vec8x32f x, vec8x32f lo, vec8x32f hi) {
        return vec8x32f{min(max(x, lo), hi)};
    }

    AVEL_FINL vec8x32f abs(vec8x32f v) {
        return v & ~vec8x32f{float_sign_bit_mask};
    }

    template<>
    AVEL_FINL vec8x32f load<vec8x32f>(const float* ptr) {
        return vec8x32f{_mm256_loadu_ps(ptr)};
    }

    template<>
    AVEL_FINL vec8x32f aligned_load<vec8x32f>(const float* ptr) {
        return vec8x32f{_mm256_load_ps(ptr)};
    }

    template<>
    AVEL_FINL vec8x32f gather<vec8x32f>(const float* ptr, vec8x32i indices) {
        #if defined(AVEL_AVX2)
        return vec8x32f{_mm256_i32gather_ps(ptr, decay(indices), sizeof(float))};

        #else
        auto offset_array = to_array(indices);

        auto a = _mm_load_ss(ptr + offset_array[0]);
        auto b = _mm_load_ss(ptr + offset_array[1]);
        auto c = _mm_load_ss(ptr + offset_array[2]);
        auto d = _mm_load_ss(ptr + offset_array[3]);

        auto e = _mm_load_ss(ptr + offset_array[4]);
        auto f = _mm_load_ss(ptr + offset_array[5]);
        auto g = _mm_load_ss(ptr + offset_array[6]);
        auto h = _mm_load_ss(ptr + offset_array[7]);

        auto abab = _mm_unpacklo_ps(a, b);
        auto cdcd = _mm_unpacklo_ps(c, d);

        auto abcd = _mm_castpd_ps(_mm_unpacklo_pd(_mm_castps_pd(abab), _mm_castps_pd(cdcd)));

        auto efef = _mm_unpacklo_ps(e, f);
        auto ghgh = _mm_unpacklo_ps(g, h);

        auto efgh = _mm_castpd_ps(_mm_unpacklo_pd(_mm_castps_pd(efef), _mm_castps_pd(ghgh)));

        return vec8x32f{_mm256_set_m128(efgh, abcd)};
        #endif
    }

    AVEL_FINL void store(float* ptr, vec8x32f v) {
        _mm256_storeu_ps(ptr, v);
    }

    AVEL_FINL void aligned_store(float* ptr, vec8x32f v) {
        _mm256_store_ps(ptr, v);
    }

    AVEL_FINL void scatter(float* ptr, vec8x32i indices, vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        _mm256_i32scatter_ps(ptr, indices, v, sizeof(float));
        #else
        auto i = to_array(indices);

        __m128 lo = _mm256_castps256_ps128(v);
        __m128 hi = _mm256_extractf128_ps(v, 0x01);

        _mm_store_ss(ptr + i[0], lo);
        _mm_store_ss(ptr + i[1], _mm_permute_ps(lo, 0x01));
        _mm_store_ss(ptr + i[2], _mm_permute_ps(lo, 0x02));
        _mm_store_ss(ptr + i[3], _mm_permute_ps(lo, 0x03));

        _mm_store_ss(ptr + i[4], _mm_permute_ps(hi, 0x00));
        _mm_store_ss(ptr + i[5], _mm_permute_ps(hi, 0x01));
        _mm_store_ss(ptr + i[6], _mm_permute_ps(hi, 0x02));
        _mm_store_ss(ptr + i[7], _mm_permute_ps(hi, 0x03));
        #endif
    }

    //=====================================================
    // Floating-point vector operations
    //=====================================================

    AVEL_FINL vec8x32f average(vec8x32f a, vec8x32f b) {
        static const auto half = vec8x32f{0.5f};
        return (a - a * half) + (b * half);
    }

    AVEL_FINL vec8x32f epsilon_increment(vec8x32f v);

    AVEL_FINL vec8x32f epsilon_decrement(vec8x32f v);

    AVEL_FINL vec8x32f epsilon_offset(vec8x32f v, vec8x32f o);

    //=====================================================
    // cmath basic operations
    //=====================================================

    AVEL_FINL vec8x32f fabs(vec8x32f v) {
        return abs(v);
    }

    AVEL_FINL vec8x32f fabsf(vec8x32f v) {
        return fabs(v);
    }

    AVEL_FINL vec8x32f fmod(vec8x32f x, vec8x32f y) {
        return x % y;
    }

    AVEL_FINL vec8x32f fmodf(vec8x32f x, vec8x32f y) {
        return fmod(x, y);
    }

    AVEL_FINL vec8x32f remainderf(vec8x32f v);

    AVEL_FINL vec8x32f remquof(vec8x32f v);

    AVEL_FINL vec8x32f fmadd(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fmadd_ps(m, x, b)};
        #else
        return vec8x32f{m * x + b};
        #endif
    }

    AVEL_FINL vec8x32f fmsubb(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fmsub_ps(m, x, b)};
        #else
        return vec8x32f{m * x - b};
        #endif
    }

    AVEL_FINL vec8x32f fnmadd(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fnmadd_ps(m, x, b)};
        #else
        return vec8x32f{-m * x + b};
        #endif
    }

    AVEL_FINL vec8x32f fnmsub(vec8x32f m, vec8x32f x, vec8x32f b) {
        #if defined(AVEL_FMA)
        return vec8x32f{_mm256_fnmsub_ps(m, x, b)};
        #else
        return vec8x32f{-m * x - b};
        #endif
    }

    AVEL_FINL vec8x32f fma(vec8x32f m, vec8x32f x, vec8x32f b) {
        return fmadd(m, x, b);
    }

    AVEL_FINL vec8x32f fmaf(vec8x32f m, vec8x32f x, vec8x32f b) {
        return fma(m, x, b);
    }

    AVEL_FINL vec8x32f fmax(vec8x32f a, vec8x32f b) {
        //TODO: Handle case with two NaNs
        return max(a, max(b, vec8x32f(INFINITY)));
    }

    AVEL_FINL vec8x32f fminf(vec8x32f a, vec8x32f b) {
        //TODO: Handle case with two NaNs
        return min(a, min(b, vec8x32f(INFINITY)));
    }

    AVEL_FINL vec8x32f fmin(vec8x32f a, vec8x32f b) {
        return fminf(a, b);
    }

    AVEL_FINL vec8x32f fdim(vec8x32f a, vec8x32f b) {
        //TODO: Make faster?
        auto tmp = max(a - b, vec8x32f{0.0f});
        auto nan_mask = avel::isnan(a) | avel::isnan(b);
        return blend(tmp, vec8x32f{NAN}, nan_mask);
    }

    AVEL_FINL vec8x32f lerp(vec8x32f a, vec8x32f b, vec8x32f t);

    //=====================================================
    // Exponential functions
    //=====================================================

    AVEL_FINL vec8x32f exp(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t: array) {
            t = std::exp(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f exp2(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::exp2(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f expm1(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::expm1(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f log(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f log10(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log10(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f log2(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log2(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f log1p(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log1p(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    //=====================================================
    // Power functions
    //=====================================================

    AVEL_FINL vec8x32f pow(vec8x32f base, vec8x32f exp) {
        //TODO: Replace with vectorized implementation
        auto base_array = base.as_array();
        auto exp_array = exp.as_array();

        for (unsigned i = 0; i < vec8x32f::width; ++i) {
            base_array[i] = std::pow(base_array[i], exp_array[i]);
        }

        return aligned_load<vec8x32f>(base_array.data());
    }

    AVEL_FINL vec8x32f sqrt(vec8x32f x) {
        return vec8x32f{_mm256_sqrt_ps(x)};
    }

    AVEL_FINL vec8x32f cbrt(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::cbrt(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f hypot(vec8x32f x, vec8x32f y) {
        //TODO: Replace with vectorized implementation
        auto x_array = x.as_array();
        auto y_array = y.as_array();

        for (unsigned i = 0; i < vec8x32f::width; ++i) {
            x_array[i] = std::hypot(x_array[i], y_array[i]);
        }

        return aligned_load<vec8x32f>(x_array.data());
    }

    //=====================================================
    // Trigonometric functions
    //=====================================================

    AVEL_FINL std::array<vec8x32f, 2> sincos(vec8x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array0 = angle.as_array();
        auto array1 = angle.as_array();

        for (unsigned i = 0; i < vec8x32f::width; ++i) {
            #if defined(AVEL_GCC)
            sincosf(array0[i], array0.data() + i, array1.data() + i);
            #elif defined(AVEL_CLANG)
            sincosf(array0[i], array0.data() + i, array1.data() + i);
            #elif defined(AVEL_ICX)
            sincosf(array0[i], array0.data() + i, array1.data() + i);
            #else
            static_assert(false, "Default not implemented");
            #endif
        }

        return {
            aligned_load<vec8x32f>(array0.data()),
            aligned_load<vec8x32f>(array1.data())
        };
    }

    AVEL_FINL vec8x32f sin(vec8x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array = angle.as_array();

        for (auto& t : array) {
            t = std::sin(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f cos(vec8x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array = angle.as_array();

        for (auto& t : array) {
            t = std::cos(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f tan(vec8x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array = angle.as_array();

        for (auto& t : array) {
            t = std::tan(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f asin(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::asin(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f acos(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::acos(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f atan(vec8x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::atan(t);
        }

        return aligned_load<vec8x32f>(array.data());
    }

    AVEL_FINL vec8x32f atan2(vec8x32f x, vec8x32f y);

    //=====================================================
    // Nearest Integer Floating-point operators
    //=====================================================

    AVEL_FINL vec8x32f ceil(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec8x32f floor(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec8x32f trunc(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec8x32f round(vec8x32f x) {
        return vec8x32f{_mm256_round_ps(x, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec8x32f nearbyint(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(v, _MM_FROUND_CUR_DIRECTION)};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    AVEL_FINL vec8x32f frexp(vec8x32f v, vec8x32i* exp);

    AVEL_FINL vec8x32f ldexp(vec8x32f x, vec8x32f exp);

    AVEL_FINL vec8x32f modf(vec8x32f x, vec8x32f* iptr);

    AVEL_FINL vec8x32f scalebn(vec8x32f x, vec8x32f exp);

    AVEL_FINL vec8x32f ilog(vec8x32f x);

    AVEL_FINL vec8x32f logb(vec8x32f x);

    AVEL_FINL vec8x32f copysign(vec8x32f mag, vec8x32f sign) {
        alignas(16) std::uint32_t mask_data[1] = {
            0x80000000
        };

        auto mask = vec8x32f{_mm256_set1_ps(mask_data[0])};

        return mask & sign | vec8x32f{_mm256_andnot_ps(mask, mag)};
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    AVEL_FINL vec8x32f fpclassify(vec8x32f v);

    AVEL_FINL vec8x32f::mask isnan(vec8x32f v) {
        return (v != v);
    }

    AVEL_FINL vec8x32f::mask isfininte(vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        return  vec8x32f{_mm256_getexp_ps(v)} != vec8x32f{255.0f};
        #else
        vec8x32f m = vec8x32f{float_exponent_mask};
        return (v & m) == m;
        #endif
    }

    AVEL_FINL vec8x32f::mask isnormal(vec8x32f v) {
        #if defined(AVEL_AVX512VL)
        vec8x32f tmp = vec8x32f{_mm256_getexp_ps(v)};
        return  (tmp != vec8x32f::zeros()) & (tmp != vec8x32f{255.0f});
        #else
        vec8x32f tmp = (v & vec8x32f(float_sign_bit_mask));
        return  (tmp != vec8x32f::zeros()) & (tmp != vec8x32f{float_exponent_mask});
        #endif
    }

    AVEL_FINL vec8x32f::mask isunordered(vec8x32f x, vec8x32f y) {
        return isnan(x) | isnan(y);
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    AVEL_FINL vec8x32f::mask isgreater(vec8x32f x, vec8x32f y) {
        return x > y;
    }

    AVEL_FINL vec8x32f::mask isgreaterequal(vec8x32f x, vec8x32f y) {
        return x >= y;
    }

    AVEL_FINL vec8x32f::mask isless(vec8x32f x, vec8x32f y) {
        return x < y;
    }

    AVEL_FINL vec8x32f::mask islessequal(vec8x32f x, vec8x32f y) {
        return x <= y;
    }

    AVEL_FINL vec8x32f::mask islessgreater(vec8x32f x, vec8x32f y) {
        return x != y;
    }

}