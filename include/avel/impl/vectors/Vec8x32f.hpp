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
    AVEL_FINL vec8x32f fmod(vec8x32f a, vec8x32f b);
    AVEL_FINL mask8x32f isnan(vec8x32f v);
    AVEL_FINL mask8x32f isinf(vec8x32f v);
    AVEL_FINL vec8x32i ilogb(vec8x32f x);
    AVEL_FINL vec8x32f copysign(vec8x32f mag, vec8x32f sign);





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

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
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
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content(b ? 0xff : 0x00) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_castsi256_ps(_mm256_set1_epi32(b ? -1 : 0))) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr8xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto array_data = _mm_loadu_si64(arr.data());
            content = primitive(_mm_test_epi8_mask(array_data, array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_loadu_si64(arr.data());
            auto expanded = _mm256_cvtepi8_epi32(array_data);
            content = _mm256_test_epi32_mask(expanded, expanded);

            #elif defined(AVEL_AVX2)
            auto array_data = _mm_loadu_si64(arr.data());
            auto expanded = _mm256_cvtepi8_epi32(array_data);
            content = _mm256_castsi256_ps(_mm256_cmpgt_epi32(expanded, _mm256_setzero_si256()));

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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return lhs.content == rhs.content;

            #elif defined(AVEL_AVX)
            return 0x00 == _mm256_movemask_ps(_mm256_xor_ps(lhs.content, rhs.content));

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return lhs.content != rhs.content;

            #elif defined(AVEL_AVX)
            return 0x00 != _mm256_movemask_ps(_mm256_xor_ps(lhs.content, rhs.content));

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        Vector_mask& operator&=(const Vector_mask& rhs) {
            #if defined(AVEL_AVX512VL)|| defined(AVEL_AVX10_1)
            content &= rhs.content;

            #elif defined(AVEL_AVX)
            content = _mm256_and_ps(content, rhs.content);

            #endif
            return *this;
        }

        Vector_mask& operator|=(const Vector_mask& rhs) {
            #if defined(AVEL_AVX512VL)|| defined(AVEL_AVX10_1)
            content |= rhs.content;

            #elif defined(AVEL_AVX)
            content = _mm256_or_ps(content, rhs.content);

            #endif
            return *this;
        }

        Vector_mask& operator^=(const Vector_mask& rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content ^= rhs.content;

            #elif defined(AVEL_AVX)
            content = _mm256_xor_ps(content, rhs.content);

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return Vector_mask{primitive(~content)};

            #elif defined(AVEL_AVX)
            return Vector_mask{_mm256_andnot_ps(content, _mm256_castsi256_ps(_mm256_set1_epi32(-1)))};

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
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return avel::popcount(decay(m));

        #elif defined(AVEL_AVX)
        return popcount(_mm256_movemask_ps(decay(m)));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x32f m) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return decay(m);

        #elif defined(AVEL_AVX)
        return _mm256_movemask_ps(decay(m));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask8x32f m) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return 0xFF == decay(m);

        #elif defined(AVEL_AVX)
        return 0xff == _mm256_movemask_ps(decay(m));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x32f m) {
        return !any(m);
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL bool extract(mask8x32f m) {
        static_assert(N < mask8x32f::width, "Specified index does not exist");
        typename std::enable_if<N < mask8x32f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return decay(m) & (1 << N);

        #elif defined(AVEL_AVX2)
        return _mm256_movemask_ps(decay(m)) & (1 << N);

        #endif
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL mask8x32f insert(mask8x32f m, bool b) {
        static_assert(N < mask8x32f::width, "Specified index does not exist");
        typename std::enable_if<N < mask8x32f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = b << N;
        return mask8x32f{__mmask8((decay(m) & ~mask) | mask)};

        #elif defined(AVEL_AVX)
        auto ret = _mm256_blend_ps(decay(m), _mm256_castsi256_ps(_mm256_set1_epi32(b ? -1 : 0)), 1 << N);
        return mask8x32f{ret};

        #endif
    }




    template<>
    class Vector<float, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = float;

        #if defined(AVEL_AVX)
        using primitive = __m256;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content(_mm256_mask_blend_ps(decay(m), _mm256_setzero_ps(), _mm256_set1_ps(1.0f))) {}
        #elif defined(AVEL_AVX)
            content(_mm256_blendv_ps(_mm256_setzero_ps(), _mm256_set1_ps(1.0f), decay(m))) {}
        #endif

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
            *this = Vector{x};
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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_EQ_OS)};

            #elif defined(AVEL_AVX)
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_EQ_OS)};
            #endif
        }

        AVEL_FINL mask operator!=(Vector rhs) const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_NEQ_OS)};

            #elif defined(AVEL_AVX)
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_NEQ_OS)};
            #endif
        }

        AVEL_FINL mask operator<(Vector rhs) const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_LT_OS)};

            #elif defined(AVEL_AVX)
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_LT_OS)};
            #endif
        }

        AVEL_FINL mask operator<=(Vector rhs) const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_LE_OS)};

            #elif defined(AVEL_AVX)
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_LE_OQ)};

            #endif
        }

        AVEL_FINL mask operator>(Vector rhs) const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_GT_OS)};

            #elif defined(AVEL_AVX)
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_GT_OS)};

            #endif
        }

        AVEL_FINL mask operator>=(Vector rhs) const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm256_cmp_ps_mask(content, rhs.content, _CMP_GE_OS)};

            #elif defined(AVEL_AVX)
            return mask{_mm256_cmp_ps(content, rhs.content, _CMP_GE_OS)};
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

        AVEL_FINL Vector& operator%=(Vector rhs) {
            content = avel::fmod(*this, rhs).content;
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
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
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

        #if defined(AVEL_AVX)
        auto ret = _mm256_blend_ps(decay(v), _mm256_set1_ps(x), 1 << N);
        return vec8x32f{ret};

        #endif
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
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec8x32f{_mm256_maskz_mov_ps(decay(m), decay(v))};

        #elif defined(AVEL_AVX)
        return vec8x32f{_mm256_and_ps(decay(m), decay(v))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f clear(mask8x32f m, vec8x32f v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec8x32f{_mm256_maskz_mov_ps(decay(!m), decay(v))};

        #elif defined(AVEL_AVX)
        return vec8x32f{_mm256_andnot_ps(decay(m), decay(v))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f blend(mask8x32f m, vec8x32f a, vec8x32f b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec8x32f{_mm256_mask_blend_ps(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_AVX)
        return vec8x32f{_mm256_blendv_ps(decay(b), decay(a), decay(m))};
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
        return std::array<vec8x32f, 2>{
            vec8x32f{_mm256_min_ps(decay(a), decay(b))},
            vec8x32f{_mm256_max_ps(decay(a), decay(b))}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f clamp(vec8x32f x, vec8x32f lo, vec8x32f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f negate(mask8x32f m, vec8x32f v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec8x32f{_mm256_mask_sub_ps(decay(v), decay(m), _mm256_setzero_ps(), decay(v))};

        #elif defined(AVEL_AVX)
        auto negation_mask = _mm256_and_ps(decay(m), _mm256_set1_ps(float_sign_bit_mask));
        return vec8x32f{_mm256_xor_ps(decay(v), negation_mask)};

        #endif
    }

    AVEL_FINL vec8x32f abs(vec8x32f v) {
        return vec8x32f{_mm256_andnot_ps(_mm256_set1_ps(float_sign_bit_mask), decay(v))};
    }

    AVEL_FINL vec8x32f neg_abs(vec8x32f v) {
        return vec8x32f{_mm256_or_ps(_mm256_set1_ps(float_sign_bit_mask), decay(v))};
    }

    //=====================================================
    // Load/Store Operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f load<vec8x32f>(const float* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32f{_mm256_maskz_loadu_ps(mask, ptr)};

        #elif defined(AVEL_AVX2)
        auto table_offset = masks256_table.size() / 2 - avel::min(8u, n) * sizeof(float);
        auto mask = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(masks256_table.data() + table_offset));
        return vec8x32f{_mm256_maskload_ps(ptr, mask)};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f load<vec8x32f, vec8x32f::width>(const float* ptr) {
        return vec8x32f{_mm256_loadu_ps(ptr)};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f aligned_load<vec8x32f>(const float* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32f{_mm256_maskz_load_ps(mask, ptr)};

        #elif defined(AVEL_AVX2)
        return load<vec8x32f>(ptr, n);
        
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f aligned_load<vec8x32f, vec8x32f::width>(const float* ptr) {
        return vec8x32f{_mm256_load_ps(ptr)};
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f gather<vec8x32f>(const float* ptr, vec8x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        return vec8x32f{_mm256_mmask_i32gather_ps(_mm256_setzero_ps(), mask, decay(indices), ptr, sizeof(float))};

        #elif defined(AVEL_AVX2)
        auto table_offset = masks256_table.size() / 2 - avel::min(vec8x32f::width, n) * sizeof(float);
        auto mask = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(masks256_table.data() + table_offset));
        return vec8x32f{_mm256_mask_i32gather_ps(_mm256_setzero_ps(), ptr, decay(indices), _mm256_castsi256_ps(mask), sizeof(float))};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32f gather<vec8x32f>(const float* ptr, vec8x32i indices) {
        return vec8x32f{_mm256_i32gather_ps(ptr, decay(indices), sizeof(float))};
    }



    AVEL_FINL void store(float* ptr, vec8x32f v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_storeu_ps(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        auto table_offset = masks256_table.size() / 2 - avel::min(vec8x32f::width, n) * sizeof(float);
        auto mask = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(masks256_table.data() + table_offset));
        _mm256_maskstore_ps(ptr, mask, decay(v));

        #endif
    }

    template<std::uint32_t N = vec8x32f::width>
    AVEL_FINL void store(float* ptr, vec8x32f v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec8x32f::width>(float* ptr, vec8x32f v) {
        #if defined(AVEL_AVX2)
        _mm256_storeu_ps(ptr, decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(float* ptr, vec8x32f v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_store_ps(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        auto table_offset = masks256_table.size() / 2 - avel::min(vec8x32f::width, n) * sizeof(float);
        auto mask = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(masks256_table.data() + table_offset));
        _mm256_maskstore_ps(ptr, mask, decay(v));

        #endif
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
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 8) ? -1 : (1 << n) - 1;
        _mm256_mask_i32scatter_ps(ptr, mask, decay(indices), decay(v), sizeof(float));

        #elif defined(AVEL_AVX2)
        switch (n) {
            default: ptr[extract<7>(indices)] = extract<7>(v);
            case 7:  ptr[extract<6>(indices)] = extract<6>(v);
            case 6:  ptr[extract<5>(indices)] = extract<5>(v);
            case 5:  ptr[extract<4>(indices)] = extract<4>(v);
            case 4:  ptr[extract<3>(indices)] = extract<3>(v);
            case 3:  ptr[extract<2>(indices)] = extract<2>(v);
            case 2:  ptr[extract<1>(indices)] = extract<1>(v);
            case 1:  ptr[extract<0>(indices)] = extract<0>(v);
            case 0:  ; //Do nothing
        }

        #endif
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
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        _mm256_i32scatter_ps(ptr, decay(indices), decay(v), sizeof(float));

        #elif defined(AVEL_AVX2)
        ptr[extract<0>(indices)] = extract<0>(v);
        ptr[extract<1>(indices)] = extract<1>(v);
        ptr[extract<2>(indices)] = extract<2>(v);
        ptr[extract<3>(indices)] = extract<3>(v);
        ptr[extract<4>(indices)] = extract<4>(v);
        ptr[extract<5>(indices)] = extract<5>(v);
        ptr[extract<6>(indices)] = extract<6>(v);
        ptr[extract<7>(indices)] = extract<7>(v);

        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr8x32f to_array(vec8x32f v) {
        alignas(32) arr8x32f array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // cmath basic operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x32f fmax(vec8x32f a, vec8x32f b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec8x32f{_mm256_range_ps(decay(a), decay(b), 0x05)};

        #elif defined(AVEL_AVX2)
        return blend(avel::isnan(b), a, avel::max(a, b));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f fmin(vec8x32f a, vec8x32f b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec8x32f{_mm256_range_ps(decay(a), decay(b), 0x04)};

        #elif defined(AVEL_AVX2)
        return blend(avel::isnan(b), a, avel::min(a, b));
        #endif
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
        return vec8x32f{_mm256_round_ps(decay(v), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f floor(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(decay(v), _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f trunc(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(decay(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f round(vec8x32f v) {
        auto whole = trunc(v);
        auto frac = v - whole;

        auto offset = copysign(vec8x32f{1.0f}, v);
        auto should_offset = abs(frac) >= vec8x32f{0.5f};
        auto ret = whole + keep(should_offset, offset);

        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f nearbyint(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(decay(v), _MM_FROUND_CUR_DIRECTION)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f rint(vec8x32f v) {
        return vec8x32f{_mm256_round_ps(decay(v), _MM_FROUND_CUR_DIRECTION)};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x32f frexp(vec8x32f v, vec8x32i* exp) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        auto is_infinity = _mm256_fpclass_ps_mask(decay(v), 0x10 | 0x08);
        auto is_non_zero = _mm256_cmp_ps_mask(decay(v), _mm256_setzero_ps(), _CMP_NEQ_UQ);

        auto exponents = _mm256_getexp_ps(decay(v));
        exponents = _mm256_add_ps(exponents, _mm256_set1_ps(1.0f));
        *exp = _mm256_maskz_cvttps_epi32(is_non_zero, exponents);

        auto ret = _mm256_getmant_ps(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        ret = _mm256_maskz_mov_ps(is_non_zero, ret);
        ret = _mm256_mask_blend_ps(is_infinity, ret, decay(v));
        return vec8x32f{ret};

        #elif defined(AVEL_AVX512VL)
        auto is_infinity = decay(abs(v) == vec8x32f{INFINITY});
        auto is_non_zero = _mm256_cmp_ps_mask(decay(v), _mm256_setzero_ps(), _CMP_NEQ_UQ);

        auto exponents = _mm256_getexp_ps(decay(v));
        exponents = _mm256_add_ps(exponents, _mm256_set1_ps(1.0f));
        *exp = _mm256_maskz_cvttps_epi32(is_non_zero, exponents);

        auto ret = _mm256_getmant_ps(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        ret = _mm256_maskz_mov_ps(is_non_zero, ret);
        ret = _mm256_mask_blend_ps(is_infinity, ret, decay(v));
        return vec8x32f{ret};

        #elif defined(AVEL_AVX2)
        //TODO: Optimize
        auto is_subnormal = avel::abs(v) < vec8x32f{FLT_MIN};
        auto multiplier = blend(is_subnormal, vec8x32f{16777216.0f}, vec8x32f{1.0f});

        auto x_corrected = v * multiplier;

        auto bits = avel::bit_cast<vec8x32i>(x_corrected);

        vec8x32i exponents = avel::bit_shift_right<23>(bits & vec8x32i{float_exponent_mask_bits});
        auto is_output_self = _mm256_cmpeq_epi32(decay(exponents), _mm256_set1_epi32(0xff));
        vec8x32i bias{_mm256_blendv_epi8(_mm256_set1_epi32(126), _mm256_set1_epi32(126 + 24), _mm256_castps_si256(decay(is_subnormal)))};

        auto is_zero = decay(v == vec8x32f{0.0f});
        *exp = clear(mask8x32i{_mm256_castps_si256(is_zero)}, exponents - bias);

        auto normal_exponent = _mm256_castsi256_ps(_mm256_set1_epi32(126 << 23));
        auto new_exponent = _mm256_andnot_ps(is_zero, normal_exponent);

        auto cleared_exponent = _mm256_andnot_ps(_mm256_set1_ps(float_exponent_mask), decay(x_corrected));
        auto ret = _mm256_or_ps(cleared_exponent, new_exponent);
        return blend(mask8x32f{_mm256_castsi256_ps(is_output_self)}, v, vec8x32f{ret});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f ldexp(vec8x32f arg, vec8x32i exp) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec8x32f{_mm256_scalef_ps(decay(arg), _mm256_cvtepi32_ps(decay(exp)))};

        #elif defined(AVEL_AVX2)
        //TODO: Optimize
        // Approach based on repeated multiplication by powers of two.
        auto bias = _mm256_set1_epi32(127);

        auto bits = _mm256_castps_si256(decay(arg));
        auto exponent_field = _mm256_and_si256(_mm256_set1_epi32(float_exponent_mask_bits), bits);
        vec8x32i arg_exponent{_mm256_srli_epi32(exponent_field, 23)};

        // Perform two multiplications such that they should never lead to lossy rounding
        vec8x32i lower_bound0{vec8x32i{1} - arg_exponent};
        vec8x32i upper_bound0{vec8x32i{254} - arg_exponent};

        vec8x32i extracted_magnitude = clamp(exp, lower_bound0, upper_bound0);
        exp -= extracted_magnitude;

        auto exponent0 = bit_shift_right<1>(extracted_magnitude);
        auto exponent_field0 = _mm256_slli_epi32(_mm256_add_epi32(decay(exponent0), bias), 23);
        auto multiplicand0 = _mm256_castsi256_ps(exponent_field0);

        auto exponent1 = extracted_magnitude - exponent0;
        auto exponent_field1 = _mm256_slli_epi32(_mm256_add_epi32(decay(exponent1), bias), 23);
        auto multiplicand1 = _mm256_castsi256_ps(exponent_field1);

        // Perform one more multiplication in case the previous two weren't enough
        // If the number isn't enough then the program

        vec8x32i lower_bound1{-126};
        vec8x32i upper_bound1{+126};

        auto exponent2 = avel::clamp(exp, lower_bound1, upper_bound1);
        auto exponent_field2 = _mm256_slli_epi32(_mm256_add_epi32(decay(exponent2), bias), 23);
        auto multiplicand2 = _mm256_castsi256_ps(exponent_field2);
        //exp -= exponent2;

        auto t0 = _mm256_mul_ps(decay(arg), multiplicand0);
        auto t1 = _mm256_mul_ps(t0, multiplicand1);
        auto ret = _mm256_mul_ps(t1, multiplicand2);

        return vec8x32f{ret};

        /*
        // Approach based on repeated multiplication by powers of two.
        vec8x32i lower_bound{-126};
        vec8x32i upper_bound{+127};
        vec8x32i& bias = upper_bound;

        auto exponent0 = avel::clamp(exp, lower_bound, upper_bound);
        auto exponent_field0 = _mm256_slli_epi32(_mm256_add_epi32(decay(exponent0), decay(bias)), 23);
        auto multiplicand0 = _mm256_castsi256_ps(exponent_field0);
        exp -= exponent0;

        auto exponent1 = avel::clamp(exp, lower_bound, upper_bound);
        auto exponent_field1 = _mm256_slli_epi32(_mm256_add_epi32(decay(exponent1), decay(bias)), 23);
        auto multiplicand1 = _mm256_castsi256_ps(exponent_field1);
        exp -= exponent1;

        auto exponent2 = avel::clamp(exp, lower_bound, upper_bound);
        auto exponent_field2 = _mm256_slli_epi32(_mm256_add_epi32(decay(exponent2), decay(bias)), 23);
        auto multiplicand2 = _mm256_castsi256_ps(exponent_field2);
        exp -= exponent2;

        auto ret = _mm256_mul_ps(_mm256_mul_ps(_mm256_mul_ps(decay(arg), multiplicand0), multiplicand1), multiplicand2);

        return vec8x32f{ret};
        */

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f scalbn(vec8x32f x, vec8x32i exp) {
        return avel::ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32i ilogb(vec8x32f x) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto exp_fp = _mm256_getexp_ps(decay(x));

        vec8x32f zero_ret{_mm256_castsi256_ps(_mm256_set1_epi32(FP_ILOGB0))};
        vec8x32f inf_ret {_mm256_castsi256_ps(_mm256_set1_epi32(INT_MAX))};
        vec8x32f nan_ret {_mm256_castsi256_ps(_mm256_set1_epi32(FP_ILOGBNAN))};

        // Return value when input is not edge case
        auto misc_ret_i = _mm256_cvtps_epi32(exp_fp);
        misc_ret_i = _mm256_maskz_mov_epi32(_mm256_cmpneq_epi32_mask(misc_ret_i, _mm256_set1_epi32(0x80000000)), misc_ret_i);

        vec8x32i zero_ret_i{_mm256_castps_si256(_mm256_fixupimm_ps(decay(zero_ret), exp_fp, _mm256_set1_epi32(0x88808888), 0x00))};
        vec8x32i inf_ret_i {_mm256_castps_si256(_mm256_fixupimm_ps(decay(inf_ret),  exp_fp, _mm256_set1_epi32(0x88088888), 0x00))};
        vec8x32i nan_ret_i {_mm256_castps_si256(_mm256_fixupimm_ps(decay(nan_ret),  exp_fp, _mm256_set1_epi32(0x88888800), 0x00))};

        return (vec8x32i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);

        #elif defined(AVEL_AVX2)
        auto is_subnormal = avel::abs(x) < vec8x32f{FLT_MIN};
        auto multiplier = blend(is_subnormal, vec8x32f{16777216.0f}, vec8x32f{1.0f});

        auto x_corrected = x * multiplier;

        auto bits = avel::bit_cast<vec8x32i>(x_corrected);

        vec8x32i exponents = avel::bit_shift_right<23>(bits & vec8x32i{float_exponent_mask_bits});
        vec8x32i bias{_mm256_blendv_epi8(_mm256_set1_epi32(127), _mm256_set1_epi32(127 + 24), _mm256_castps_si256(decay(is_subnormal)))};

        auto ret = exponents - bias;

        ret = blend(mask8x32i{_mm256_castps_si256(decay(x == vec8x32f{0.0f}))}, vec8x32i{FP_ILOGB0}, ret);
        ret = blend(mask8x32i{_mm256_castps_si256(decay(avel::isinf(x)))}, vec8x32i{INT_MAX}, ret);
        ret = blend(mask8x32i{_mm256_castps_si256(decay(avel::isnan(x)))}, vec8x32i{FP_ILOGBNAN}, ret);

        return ret;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f logb(vec8x32f x) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec8x32f{_mm256_getexp_ps(decay(x))};

        #elif defined(AVEL_AVX2)
        auto is_subnormal = avel::abs(x) < vec8x32f{FLT_MIN};
        auto multiplier = blend(is_subnormal, vec8x32f{16777216.0f}, vec8x32f{1.0f});

        auto x_corrected = x * multiplier;

        auto bits = avel::bit_cast<vec8x32i>(x_corrected);

        auto exponents_i = avel::bit_shift_right<23>(bits & vec8x32i{float_exponent_mask_bits});
        vec8x32f exponents{_mm256_cvtepi32_ps(decay(exponents_i))};
        auto bias = blend(is_subnormal, vec8x32f{127 + 24}, vec8x32f{127});

        auto ret = exponents - bias;

        ret = blend(x == vec8x32f{0.0f}, vec8x32f{-INFINITY}, ret);
        ret = blend(avel::isinf(x), vec8x32f{INFINITY}, ret);
        ret = blend(avel::isnan(x), vec8x32f{NAN}, ret);

        return ret;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32f copysign(vec8x32f mag, vec8x32f sign) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = _mm256_set1_epi32(float_sign_bit_mask_bits);
        auto ret = _mm256_ternarylogic_epi32(_mm256_castps_si256(decay(sign)), _mm256_castps_si256(decay(mag)), mask, 0xe4);
        return vec8x32f{_mm256_castsi256_ps(ret)};

        #elif defined(AVEL_AVX2)
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
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm256_castps_si256(decay(v));

        // Take absolute value
        v_bits = _mm256_and_si256(v_bits, _mm256_set1_epi32(0x7fffffff));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm256_set1_epi32(0x00800000);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm256_set1_epi32(0x7f800000);

        // Masks for individual categories
        auto m0 = _mm256_cmpeq_epi32(v_bits, _mm256_setzero_si256());
        auto m1 = _mm256_andnot_si256(m0, _mm256_cmpgt_epi32(min_bits, v_bits));
        auto m2 = _mm256_andnot_si256(_mm256_cmpgt_epi32(min_bits, v_bits), _mm256_cmpgt_epi32(inf_bits, v_bits));
        auto m3 = _mm256_cmpeq_epi32(v_bits, inf_bits);
        auto m4 = _mm256_cmpgt_epi32(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm256_setzero_si256();
        ret = _mm256_and_si256(m0, _mm256_set1_epi32(FP_ZERO));
        ret = _mm256_ternarylogic_epi32(ret, m1, _mm256_set1_epi32(FP_SUBNORMAL), 0xf8);
        ret = _mm256_ternarylogic_epi32(ret, m2, _mm256_set1_epi32(FP_NORMAL),    0xf8);
        ret = _mm256_ternarylogic_epi32(ret, m3, _mm256_set1_epi32(FP_INFINITE),  0xf8);
        ret = _mm256_ternarylogic_epi32(ret, m4, _mm256_set1_epi32(FP_NAN),       0xf8);

        return vec8x32i{ret};

        #elif defined(AVEL_AVX2)
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm256_castps_si256(decay(v));

        // Take absolute value
        v_bits = _mm256_and_si256(v_bits, _mm256_set1_epi32(0x7fffffff));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm256_set1_epi32(0x00800000);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm256_set1_epi32(0x7f800000);

        // Masks for individual categories
        auto m0 = _mm256_cmpeq_epi32(v_bits, _mm256_setzero_si256());
        auto m1 = _mm256_andnot_si256(m0, _mm256_cmpgt_epi32(min_bits, v_bits));
        auto m2 = _mm256_andnot_si256(_mm256_cmpgt_epi32(min_bits, v_bits), _mm256_cmpgt_epi32(inf_bits, v_bits));
        auto m3 = _mm256_cmpeq_epi32(v_bits, inf_bits);
        auto m4 = _mm256_cmpgt_epi32(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm256_setzero_si256();
        ret = _mm256_and_si256(m0, _mm256_set1_epi32(FP_ZERO));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m1, _mm256_set1_epi32(FP_SUBNORMAL)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m2, _mm256_set1_epi32(FP_NORMAL)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m3, _mm256_set1_epi32(FP_INFINITE)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m4, _mm256_set1_epi32(FP_NAN)));

        return vec8x32i{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isfinite(vec8x32f v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return !mask8x32f{_mm256_fpclass_ps_mask(decay(v), 0x01 | 0x08 | 0x10 | 0x80)};

        #elif defined(AVEL_AVX2)
        return (vec8x32f{-INFINITY} < v) && (v < vec8x32f{+INFINITY});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isinf(vec8x32f v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return mask8x32f{_mm256_fpclass_ps_mask(decay(v), 0x08 | 0x10)};

        #elif defined(AVEL_AVX)
        return avel::abs(v) == vec8x32f{INFINITY};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isnan(vec8x32f v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return mask8x32f{_mm256_fpclass_ps_mask(decay(v), 0x80 | 0x01)};

        #elif defined(AVEL_AVX512VL)
        return mask8x32f{_mm256_cmp_ps_mask(decay(v), decay(v), _CMP_UNORD_Q)};

        #elif defined(AVEL_AVX)
        return mask8x32f{_mm256_cmp_ps(decay(v), decay(v), _CMP_UNORD_Q)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isnormal(vec8x32f v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return !mask8x32f{_mm256_fpclass_ps_mask(decay(v), 0xBF)};

        #elif defined(AVEL_AVX)
        auto abs_v = avel::abs(v);
        return (vec8x32f{FLT_MIN} <= abs_v) && (abs_v <= vec8x32f{FLT_MAX});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f signbit(vec8x32f v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return mask8x32f{_mm256_fpclass_ps_mask(decay(v), 0x40 | 0x04 | 0x10)};

        #elif defined(AVEL_AVX512VL)
        return mask8x32f{_mm256_cmplt_epi32_mask(_mm256_castps_si256(decay(v)), _mm256_setzero_si256())};

        #elif defined(AVEL_AVX2)
        auto bits = _mm256_castps_si256(decay(v));
        auto mask_bits = _mm256_srai_epi32(bits, 31);
        return mask8x32f{_mm256_castsi256_ps(mask_bits)};
        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask8x32f isgreater(vec8x32f x, vec8x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask8x32f{_mm256_cmp_ps_mask(decay(x), decay(y), _CMP_GT_OQ)};

        #elif defined(AVEL_AVX2)
        return mask8x32f{_mm256_cmp_ps(decay(x), decay(y), _CMP_GT_OQ)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isgreaterequal(vec8x32f x, vec8x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask8x32f{_mm256_cmp_ps_mask(decay(x), decay(y), _CMP_GE_OQ)};

        #elif defined(AVEL_AVX2)
        return mask8x32f{_mm256_cmp_ps(decay(x), decay(y), _CMP_GE_OQ)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isless(vec8x32f x, vec8x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask8x32f{_mm256_cmp_ps_mask(decay(x), decay(y), _CMP_LT_OQ)};

        #elif defined(AVEL_AVX2)
        return mask8x32f{_mm256_cmp_ps(decay(x), decay(y), _CMP_LT_OQ)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f islessequal(vec8x32f x, vec8x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask8x32f{_mm256_cmp_ps_mask(decay(x), decay(y), _CMP_LE_OQ)};

        #elif defined(AVEL_AVX2)
        return mask8x32f{_mm256_cmp_ps(decay(x), decay(y), _CMP_LE_OQ)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f islessgreater(vec8x32f x, vec8x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask8x32f{_mm256_cmp_ps_mask(decay(x), decay(y), _CMP_NEQ_OQ)};

        #elif defined(AVEL_AVX2)
        return mask8x32f{_mm256_cmp_ps(decay(x), decay(y), _CMP_NEQ_OQ)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask8x32f isunordered(vec8x32f x, vec8x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask8x32f{_mm256_cmp_ps_mask(decay(x), decay(y), _CMP_UNORD_Q)};

        #elif defined(AVEL_AVX2)
        return mask8x32f{_mm256_cmp_ps(decay(x), decay(y), _CMP_UNORD_Q)};

        #endif
    }

}
