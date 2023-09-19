#ifndef AVEL_VEC4X64F_HPP
#define AVEL_VEC4X64F_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x64f = Vector<double, 4>;
    using arr4x64f = std::array<double, 4>;
    using mask4x64f = Vector_mask<double, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec4x64f trunc(vec4x64f x);
    AVEL_FINL vec4x64f abs(vec4x64f v);
    AVEL_FINL vec4x64f fmod(vec4x64f a, vec4x64f b);
    AVEL_FINL vec4x64f blend(vec4x64f, vec4x64f, mask4x64f);
    AVEL_FINL vec4x64f round(vec4x64f);
    AVEL_FINL mask4x64f isunordered(vec4x64f, vec4x64f);
    AVEL_FINL mask4x64f signbit(vec4x64f x);
    AVEL_FINL mask4x64f isnan(vec4x64f v);
    AVEL_FINL mask4x64f isinf(vec4x64f v);
    AVEL_FINL vec4x64f copysign(vec4x64f mag, vec4x64f sign);





    template<>
    class Vector_mask<double, 4> {
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

        #elif defined(AVEL_AVX)
        using primitive = __m256d;

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
            content(convert<Vector_mask>(v)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL)
            content(b ? 0x0f : 0x00) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_castsi256_pd(_mm256_set1_epi64x(b ? -1 : 0))) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr4xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );


            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m128i array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            content = __mmask8(_mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            auto expanded = _mm_cvtepi8_epi32(array_data);
            content = _mm_cmplt_epu32_mask(_mm_setzero_si128(), expanded);

            #elif defined(AVEL_AVX2)
            auto array_data = _mm_cvtsi32_si128(bit_cast<std::uint32_t>(arr));
            auto widened = _mm256_cvtepi8_epi64(array_data);
            content = _mm256_castsi256_pd(_mm256_cmpgt_epi64(widened, _mm256_setzero_si256()));

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
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return lhs.content == rhs.content;

            #elif defined(AVEL_AVX2)
            return 0x00 == _mm256_movemask_pd(_mm256_xor_pd(lhs.content, rhs.content));

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return lhs.content != rhs.content;

            #elif defined(AVEL_AVX2)
            return 0x00 != _mm256_movemask_pd(_mm256_xor_pd(lhs.content, rhs.content));

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content &= rhs.content;

            #elif defined(AVEL_AVX2)
            content = _mm256_and_pd(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content |= rhs.content;

            #elif defined(AVEL_AVX2)
            content = _mm256_or_pd(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content ^= rhs.content;

            #elif defined(AVEL_AVX2)
            content = _mm256_xor_pd(content, rhs.content);

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{primitive(content ^ 0x0f)};

            #elif defined(AVEL_AVX2)
            return Vector_mask{_mm256_andnot_pd(content, _mm256_castsi256_pd(_mm256_set1_epi64x(-1)))};

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
    AVEL_FINL std::uint32_t count(mask4x64f m) {
        #if defined(AVEL_AVX512VL)
        return avel::popcount(decay(m));

        #elif defined(AVEL_AVX2)
        return popcount(_mm256_movemask_pd(decay(m)));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x64f m) {
        #if defined(AVEL_AVX512VL)
        return decay(m);

        #elif defined(AVEL_AVX2)
        return _mm256_movemask_pd(decay(m));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x64f m) {
        #if defined(AVEL_AVX512VL)
        return 0x0f == decay(m);

        #elif defined(AVEL_AVX2)
        return 0x0f == _mm256_movemask_pd(decay(m));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x64f m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================






    template<>
    class Vector<double, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = double;

        #if defined(AVEL_AVX)
        using primitive = __m256d;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            content(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm256_mask_blend_pd(decay(m), _mm256_set1_pd(0.0), _mm256_set1_pd(1.0))) {}
        #elif defined(AVEL_AVX)
            content(_mm256_blendv_pd(_mm256_set1_pd(0.0), _mm256_set1_pd(1.0), decay(m))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm256_set1_pd(x)) {}

        AVEL_FINL explicit Vector(const arr4x64f& array):
            content(_mm256_loadu_pd(array.data())) {}

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
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_EQ_OS)};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmp_pd(lhs.content, rhs.content, _CMP_EQ_OS)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_NEQ_OS)};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmp_pd(lhs.content, rhs.content, _CMP_NEQ_OS)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_LT_OS)};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmp_pd(lhs.content, rhs.content, _CMP_LT_OS)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_LE_OS)};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmp_pd(lhs.content, rhs.content, _CMP_LE_OS)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_GT_OS)};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmp_pd(lhs.content, rhs.content, _CMP_GT_OS)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmp_pd_mask(lhs.content, rhs.content, _CMP_GE_OS)};

            #elif defined(AVEL_AVX2)
            return mask{_mm256_cmp_pd(lhs.content, rhs.content, _CMP_GE_OS)};

            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            return Vector{0.0} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content = _mm256_add_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm256_sub_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm256_mul_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            content = _mm256_div_pd(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
            //TODO: Call fmod
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

        [[nodiscard]]
        AVEL_FINL friend Vector operator%(Vector lhs, Vector rhs) {
            lhs %= rhs;
            return lhs;
        }

        //=================================================
        // Increment/Decrement operators
        //=================================================

        AVEL_FINL Vector& operator++() {
            *this += Vector{1};
            return *this;
        }

        AVEL_FINL Vector operator++(int) {
            auto temp = *this;
            *this += Vector{1};
            return temp;
        }

        AVEL_FINL Vector& operator--() {
            *this -= Vector{1};
            return *this;
        }

        AVEL_FINL Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{1};
            return temp;
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512VL)
            auto t = _mm256_castpd_si256(content);
            return mask{_mm256_test_epi64_mask(t, t)};

            #elif defined(AVEL_AVX)
            return mask{_mm256_cmp_pd(content, _mm256_setzero_pd(), _CMP_NEQ_UQ)};

            #endif
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    static_assert(
        4 * sizeof(double) == sizeof(vec4x64f),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL double extract(vec4x64f v) {
        static_assert(N < vec4x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x64f::width, int>::type dummy_variable = 0;

        auto quarter = _mm_castps_pd(_mm256_extractf128_ps(_mm256_castpd_ps(decay(v)), N / 2));

        // Rely on const-folding to optimize this branch away
        if (N % 2 == 1) {
            quarter = _mm_unpackhi_pd(quarter, quarter);
        }

        return _mm_cvtsd_f64(quarter);
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x64f insert(vec4x64f v, double x) {
        static_assert(N < vec4x64f::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x64f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX)
        auto ret = _mm256_blend_pd(decay(v), _mm256_set1_pd(x), 1 << N);
        return vec4x64f{ret};

        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec4x64f x) {
        return count(mask4x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec4x64f x) {
        return any(mask4x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec4x64f x) {
        return all(mask4x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec4x64f x) {
        return none(mask4x64f{x});
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f keep(mask4x64f m, vec4x64f v) {
        #if defined(AVEL_AVX512VL)
        return vec4x64f{_mm256_maskz_mov_pd(decay(m), decay(v))};

        #elif defined(AVEL_AVX)
        return vec4x64f{_mm256_and_pd(decay(m), decay(v))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f clear(mask4x64f m, vec4x64f v) {
        #if defined(AVEL_AVX512VL)
        return vec4x64f{_mm256_maskz_mov_pd(decay(!m), decay(v))};

        #elif defined(AVEL_AVX)
        return vec4x64f{_mm256_andnot_pd(decay(m), decay(v))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f blend(mask4x64f m, vec4x64f a, vec4x64f b) {
        #if defined(AVEL_AVX512VL)
        return vec4x64f{_mm256_mask_blend_pd(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_AVX)
        return vec4x64f{_mm256_blendv_pd(decay(b), decay(a), decay(m))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f byteswap(vec4x64f v) {
        return bit_cast<vec4x64f>(byteswap(bit_cast<vec4x64u>(v)));
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f max(vec4x64f a, vec4x64f b) {
        #if defined(AVEL_AVX)
        return vec4x64f{_mm256_max_pd(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f min(vec4x64f a, vec4x64f b) {
        #if defined(AVEL_AVX)
        return vec4x64f{_mm256_min_pd(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x64f, 2> minmax(vec4x64f a, vec4x64f b) {
        #if defined(AVEL_AVX)
        return {
            vec4x64f{_mm256_min_pd(decay(a), decay(b))},
            vec4x64f{_mm256_max_pd(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f clamp(vec4x64f x, vec4x64f lo, vec4x64f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f negate(mask4x64f m, vec4x64f v) {
        #if defined(AVEL_AVX512VL)
        return vec4x64f{_mm256_mask_sub_pd(decay(v), decay(m), _mm256_setzero_pd(), decay(v))};

        #elif defined(AVEL_AVX)
        auto negation_mask = _mm256_and_pd(decay(m), _mm256_set1_pd(double_sign_bit_mask));
        return vec4x64f{_mm256_xor_pd(decay(v), negation_mask)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f abs(vec4x64f v) {
        #if defined(AVEL_AVX)
        return vec4x64f{_mm256_andnot_pd(_mm256_set1_pd(double_sign_bit_mask), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f neg_abs(vec4x64f v) {
        #if defined(AVEL_AVX)
        return vec4x64f{_mm256_or_pd(_mm256_set1_pd(double_sign_bit_mask), decay(v))};
        #endif
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f load<vec4x64f>(const double* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        return vec4x64f{_mm256_maskz_loadu_pd(mask, ptr)};

        #elif defined(AVEL_AVX2)
        auto table_offset = masks256_table.size() / 2  - avel::min(vec4x64f ::width, n) * sizeof(double);
        auto mask = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(masks256_table.data() + table_offset));
        return vec4x64f{_mm256_castsi256_pd(_mm256_maskload_epi64(reinterpret_cast<const long long*>(ptr), mask))};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f load<vec4x64f, vec4x64f::width>(const double* ptr) {
        return vec4x64f{_mm256_loadu_pd(ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f aligned_load<vec4x64f>(const double* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        return vec4x64f{_mm256_maskz_load_pd(mask, ptr)};

        #elif defined(AVEL_AVX2)
        return load<vec4x64f>(ptr, n);

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f aligned_load<vec4x64f, vec4x64f::width>(const double* ptr) {
        #if defined(AVEL_AVX)
        return vec4x64f{_mm256_load_pd(ptr)};
        #endif
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f gather<vec4x64f>(const double* ptr, vec4x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        return vec4x64f{_mm256_mmask_i64gather_pd(_mm256_setzero_pd(), mask, decay(indices), ptr, sizeof(double))};

        #elif defined(AVEL_AVX2)
        auto table_offset = masks256_table.size() / 2 - avel::min(vec4x64f::width, n) * sizeof(double);
        auto mask = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(masks256_table.data() + table_offset));

        return vec4x64f{_mm256_mask_i64gather_pd(
            _mm256_setzero_pd(),
            ptr,
            decay(indices),
            _mm256_castsi256_pd(mask),
            sizeof(double)
        )};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64f gather<vec4x64f>(const double* ptr, vec4x64i indices) {
        #if defined(AVEL_AVX2)
        return vec4x64f{_mm256_i64gather_pd(ptr, decay(indices), sizeof(double))};
        #endif
    }



    AVEL_FINL void store(double* ptr, vec4x64f v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        _mm256_mask_storeu_pd(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        auto table_offset = masks256_table.size() / 2 - avel::min(vec4x64f::width, n) * sizeof(double);
        auto mask = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(masks256_table.data() + table_offset));
        _mm256_maskstore_pd(ptr, mask, decay(v));

        #endif
    }

    template<std::uint32_t N = vec4x64f::width>
    AVEL_FINL void store(double* ptr, vec4x64f v) {
        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec4x64f::width>(double* ptr, vec4x64f v) {
        #if defined(AVEL_AVX)
        _mm256_storeu_pd(ptr, decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(double* ptr, vec4x64f v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        _mm256_mask_store_pd(ptr, mask, decay(v));

        #elif defined(AVEL_AVX2)
        store(ptr, v, n);

        #endif
    }

    template<std::uint32_t N = vec4x64f::width>
    AVEL_FINL void aligned_store(double* ptr, vec4x64f v) {
        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec4x64f::width>(double* ptr, vec4x64f v) {
        #if defined(AVEL_AVX2)
        _mm256_store_pd(ptr, decay(v));
        #endif
    }



    AVEL_FINL void scatter(double* ptr, vec4x64f v, vec4x64i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        _mm256_mask_i64scatter_pd(ptr, mask, decay(indices), decay(v), sizeof(double));

        #elif defined(AVEL_AVX2)
        switch (n) {
            default: ptr[extract<3>(indices)] = extract<3>(v);
            case 3:  ptr[extract<2>(indices)] = extract<2>(v);
            case 2:  ptr[extract<1>(indices)] = extract<1>(v);
            case 1:  ptr[extract<0>(indices)] = extract<0>(v);
            case 0:  ; //Do nothing
        }

        #endif
    }

    template<std::uint32_t N = vec4x64f::width>
    AVEL_FINL void scatter(double* ptr, vec4x64f v, vec4x64i indices) {
        scatter(ptr, v, indices, N);
    }

    template<>
    AVEL_FINL void scatter<0>(double* ptr, vec4x64f v, vec4x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec4x64f::width>(double* ptr, vec4x64f v, vec4x64i indices) {
        #if defined(AVEL_AVX512VL)
        _mm256_i64scatter_pd(ptr, decay(indices), decay(v), sizeof(double));

        #elif defined(AVEL_AVX2)
        ptr[extract<0>(indices)] = extract<0>(v);
        ptr[extract<1>(indices)] = extract<1>(v);
        ptr[extract<2>(indices)] = extract<2>(v);
        ptr[extract<3>(indices)] = extract<3>(v);

        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr4x64f to_array(vec4x64f v) {
        alignas(32) arr4x64f ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // cmath basic operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f fmax(vec4x64f a, vec4x64f b) {
        return blend(avel::isnan(b), a, avel::max(a, b));
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f fmin(vec4x64f a, vec4x64f b) {
        return blend(avel::isnan(b), a, avel::min(a, b));
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f sqrt(vec4x64f arg) {
        return vec4x64f{_mm256_sqrt_pd(decay(arg))};
    }

    //=====================================================
    // Nearest integer floating-point operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f ceil(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f floor(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f trunc(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f round(vec4x64f v) {
        auto whole = trunc(v);
        auto frac = v - whole;

        auto offset = copysign(vec4x64f{1.0}, v);
        auto should_offset = abs(frac) >= vec4x64f{0.5};
        auto ret = whole + keep(should_offset, offset);

        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f nearbyint(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f rint(vec4x64f v) {
        return vec4x64f{_mm256_round_pd(decay(v), _MM_FROUND_CUR_DIRECTION)};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64f frexp(vec4x64f v, vec4x64i* exp) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto is_infinity = _mm256_fpclass_pd_mask(decay(v), 0x10 | 0x08);
        auto is_non_zero = _mm256_cmp_pd_mask(decay(v), _mm256_setzero_pd(), _CMP_NEQ_UQ);

        auto exponents = _mm256_getexp_pd(decay(v));
        exponents = _mm256_add_pd(exponents, _mm256_set1_pd(1.0f));
        *exp = _mm256_maskz_cvttpd_epi64(is_non_zero, exponents);

        auto ret = _mm256_getmant_pd(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        // Note: Returns -1 or 1 for -infinity and +infinity respectively

        ret = _mm256_maskz_mov_pd(is_non_zero, ret);
        ret = _mm256_mask_blend_pd(is_infinity, ret, decay(v));
        return vec4x64f{ret};

        #elif defined(AVEL_AVX512VL)
        auto is_infinity = decay(abs(v) == vec4x64f{INFINITY});
        auto is_non_zero = _mm256_cmp_pd_mask(decay(v), _mm256_setzero_pd(), _CMP_NEQ_UQ);

        auto exponents = _mm256_getexp_pd(decay(v));
        exponents = _mm256_add_pd(exponents, _mm256_set1_pd(1.0f));
        auto exp32 = _mm256_maskz_cvttpd_epi32(is_non_zero, exponents);

        *exp = _mm256_cvtepi32_epi64(exp32);

        auto ret = _mm256_getmant_pd(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        // Note: Returns -1 or 1 for -infinity and +infinity respectively

        ret = _mm256_maskz_mov_pd(is_non_zero, ret);
        ret = _mm256_mask_blend_pd(is_infinity, ret, decay(v));
        return vec4x64f{ret};

        #elif defined(AVEL_AVX2)
        auto is_subnormal = avel::abs(v) < vec4x64f{DBL_MIN};
        auto multiplier = blend(is_subnormal, vec4x64f{9007199254740992.0f}, vec4x64f{1.0f});

        auto x_corrected = v * multiplier;

        auto bits = avel::bit_cast<vec4x64i>(x_corrected);

        vec4x64i exponents = avel::bit_shift_right<52>(bits & vec4x64i{double_exponent_mask_bits});
        auto is_output_self = _mm256_cmpeq_epi64(decay(exponents), _mm256_set1_epi64x(0x7ff));
        vec4x64i bias{_mm256_blendv_epi8(_mm256_set1_epi64x(1022), _mm256_set1_epi64x(1022 + 53), _mm256_castpd_si256(decay(is_subnormal)))};

        auto is_zero = decay(v == vec4x64f{0.0f});
        *exp = clear(mask4x64i{_mm256_castpd_si256(is_zero)}, exponents - bias);

        auto normal_exponent = _mm256_castsi256_pd(_mm256_set1_epi64x(1022ull << 52));
        auto new_exponent = _mm256_andnot_pd(is_zero, normal_exponent);

        auto cleared_exponent = _mm256_andnot_pd(_mm256_set1_pd(double_exponent_mask), decay(x_corrected));
        auto ret = _mm256_or_pd(cleared_exponent, new_exponent);
        return blend(mask4x64f{_mm256_castsi256_pd(is_output_self)}, v, vec4x64f{ret});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f ldexp(vec4x64f arg, vec4x64i exp) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec4x64f{_mm256_scalef_pd(decay(arg), _mm256_cvtepi64_pd(decay(exp)))};

        #elif defined(AVEL_AVX512VL)
        auto exp32 = _mm256_cvtsepi64_epi32(decay(exp));
        auto exp_as_f64 = _mm256_cvtepi32_pd(exp32);
        return vec4x64f{_mm256_scalef_pd(decay(arg), exp_as_f64)};

        #elif defined(AVEL_AVX2)
        //TODO: Optimize. 32-bit integer operations would be faster and suffice
        // Approach based on repeated multiplication by powers of two.
        auto bias = _mm256_set1_epi32(1023);

        auto bits = _mm256_castpd_si256(decay(arg));
        auto exponent_field = _mm256_and_si256(_mm256_set1_epi64x(double_exponent_mask_bits), bits);
        vec4x64i arg_exponent = bit_shift_right<52>(vec4x64i{exponent_field});

        // Perform two multiplications such that they should never lead to lossy rounding
        vec4x64i lower_bound0{vec4x64i{1} - arg_exponent};
        vec4x64i upper_bound0{vec4x64i{1046} - arg_exponent};

        vec4x64i extracted_magnitude = clamp(exp, lower_bound0, upper_bound0);
        exp -= extracted_magnitude;

        auto exponent0 = bit_shift_right<1>(extracted_magnitude);
        auto exponent_field0 = bit_shift_left<52>(exponent0 + vec4x64i{bias});
        auto multiplicand0 = _mm256_castsi256_pd(decay(exponent_field0));

        auto exponent1 = extracted_magnitude - exponent0;
        auto exponent_field1 = bit_shift_left<52>(exponent1 + vec4x64i{bias});
        auto multiplicand1 = _mm256_castsi256_pd(decay(exponent_field1));

        // Perform one more multiplication in case the previous two weren't enough
        // If the number isn't enough then the program

        vec4x64i lower_bound1{-1022};
        vec4x64i upper_bound1{+1022};

        auto exponent2 = avel::clamp(exp, lower_bound1, upper_bound1);
        auto exponent_field2 = bit_shift_left<52>(exponent2 + vec4x64i{bias});
        auto multiplicand2 = _mm256_castsi256_pd(decay(exponent_field2));
        //exp -= exponent2;

        auto t0 = _mm256_mul_pd(decay(arg), multiplicand0);
        auto t1 = _mm256_mul_pd(t0, multiplicand1);
        auto ret = _mm256_mul_pd(t1, multiplicand2);

        return vec4x64f{ret};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f scalbn(vec4x64f x, vec4x64i exp) {
        return avel::ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i ilogb(vec4x64f x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        auto exp_fp = _mm256_getexp_pd(decay(x));

        vec4x64f zero_ret{_mm256_castsi256_pd(_mm256_set1_epi64x(FP_ILOGB0))};
        vec4x64f inf_ret {_mm256_castsi256_pd(_mm256_set1_epi64x(INT_MAX))};
        vec4x64f nan_ret {_mm256_castsi256_pd(_mm256_set1_epi64x(FP_ILOGBNAN))};

        // Return value when input is not edge case
        auto misc_ret_i = _mm256_cvtpd_epi64(exp_fp);
        misc_ret_i = _mm256_maskz_mov_epi64(_mm256_cmpneq_epi64_mask(misc_ret_i, _mm256_set1_epi64x(0x8000000000000000ll)), misc_ret_i);

        vec4x64i zero_ret_i{_mm256_castpd_si256(_mm256_fixupimm_pd(decay(zero_ret), exp_fp, _mm256_set1_epi64x(0x88808888), 0x00))};
        vec4x64i inf_ret_i {_mm256_castpd_si256(_mm256_fixupimm_pd(decay(inf_ret),  exp_fp, _mm256_set1_epi64x(0x88088888), 0x00))};
        vec4x64i nan_ret_i {_mm256_castpd_si256(_mm256_fixupimm_pd(decay(nan_ret),  exp_fp, _mm256_set1_epi64x(0x88888800), 0x00))};

        return (vec4x64i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);

        #elif defined(AVEL_AVX512VL)
        auto exp_fp = _mm256_getexp_pd(decay(x));

        vec4x64f zero_ret{_mm256_castsi256_pd(_mm256_set1_epi64x(FP_ILOGB0))};
        vec4x64f inf_ret {_mm256_castsi256_pd(_mm256_set1_epi64x(INT_MAX))};
        vec4x64f nan_ret {_mm256_castsi256_pd(_mm256_set1_epi64x(FP_ILOGBNAN))};

        // Return value when input is not edge case
        auto misc_ret_i_32 = _mm256_cvtpd_epi32(exp_fp);
        auto misc_ret_i = _mm256_cvtepi32_epi64(misc_ret_i_32);
        misc_ret_i = _mm256_maskz_mov_epi64(_mm256_cmpneq_epi64_mask(misc_ret_i, _mm256_set1_epi64x(0xffffffff80000000ll)), misc_ret_i);

        vec4x64i zero_ret_i{_mm256_castpd_si256(_mm256_fixupimm_pd(decay(zero_ret), exp_fp, _mm256_set1_epi64x(0x88808888), 0x00))};
        vec4x64i inf_ret_i {_mm256_castpd_si256(_mm256_fixupimm_pd(decay(inf_ret),  exp_fp, _mm256_set1_epi64x(0x88088888), 0x00))};
        vec4x64i nan_ret_i {_mm256_castpd_si256(_mm256_fixupimm_pd(decay(nan_ret),  exp_fp, _mm256_set1_epi64x(0x88888800), 0x00))};

        return (vec4x64i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);

        #elif defined(AVEL_AVX2)
        auto is_subnormal = avel::abs(x) < vec4x64f{DBL_MIN};
        auto multiplier = blend(is_subnormal, vec4x64f{9007199254740992.0f}, vec4x64f{1.0f});

        auto x_corrected = x * multiplier;

        auto bits = avel::bit_cast<vec4x64i>(x_corrected);

        vec4x64i exponents = avel::bit_shift_right<52>(bits & vec4x64i{double_exponent_mask_bits});
        vec4x64i bias{_mm256_blendv_epi8(_mm256_set1_epi64x(1023), _mm256_set1_epi64x(1023 + 53), _mm256_castpd_si256(decay(is_subnormal)))};

        auto ret = exponents - bias;

        ret = blend(mask4x64i{_mm256_castpd_si256(decay(x == vec4x64f{0.0}))}, vec4x64i{FP_ILOGB0}, ret);
        ret = blend(mask4x64i{_mm256_castpd_si256(decay(avel::isinf(x)))}, vec4x64i{INT_MAX}, ret);
        ret = blend(mask4x64i{_mm256_castpd_si256(decay(avel::isnan(x)))}, vec4x64i{FP_ILOGBNAN}, ret);

        return ret;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f logb(vec4x64f x) {
        #if defined(AVEL_AVX512VL)
        return vec4x64f{_mm256_getexp_pd(decay(x))};

        #elif defined(AVEL_AVX2)
        auto is_subnormal = avel::abs(x) < vec4x64f{DBL_MIN};
        auto multiplier = blend(is_subnormal, vec4x64f{9007199254740992.0}, vec4x64f{1.0f});

        auto x_corrected = x * multiplier;

        auto bits = avel::bit_cast<vec4x64i>(x_corrected);

        auto exponents_i = avel::bit_shift_right<52>(bits & vec4x64i{double_exponent_mask_bits});

        auto narrowed_exponents = _mm_hadd_epi32(
            _mm256_extracti128_si256(decay(exponents_i), 0x00),
            _mm256_extracti128_si256(decay(exponents_i), 0x01)
        );

        vec4x64f exponents{_mm256_cvtepi32_pd(narrowed_exponents)};
        auto bias = blend(is_subnormal, vec4x64f{1023 + 53}, vec4x64f{1023});

        auto ret = exponents - bias;

        ret = blend(x == vec4x64f{0.0f}, vec4x64f{-INFINITY}, ret);
        ret = blend(avel::isinf(x), vec4x64f{INFINITY}, ret);
        ret = blend(avel::isnan(x), vec4x64f{NAN}, ret);

        return ret;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x64f copysign(vec4x64f mag, vec4x64f sign) {
        #if defined(AVEL_AVX512VL)
        auto mask = _mm256_set1_epi64x(double_sign_bit_mask_bits);
        auto ret = _mm256_ternarylogic_epi32(_mm256_castpd_si256(decay(sign)), _mm256_castpd_si256(decay(mag)), mask, 0xe4);
        return vec4x64f{_mm256_castsi256_pd(ret)};

        #elif defined(AVEL_AVX)
        auto mask = _mm256_set1_pd(double_sign_bit_mask);
        auto ret = _mm256_or_pd(_mm256_and_pd(mask, decay(sign)), _mm256_andnot_pd(mask, decay(mag)));
        return vec4x64f{ret};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64i fpclassify(vec4x64f v) {
        #if defined(AVEL_AVX512VL)
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm256_castpd_si256(decay(v));

        // Take absolute value
        v_bits = _mm256_and_si256(v_bits, _mm256_set1_epi64x(0x7fffffffffffffffull));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm256_set1_epi64x(0x10000000000000ull);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm256_set1_epi64x(0x7ff0000000000000ull);

        // Masks for individual categories
        auto m0 = _mm256_cmpeq_epi64(v_bits, _mm256_setzero_si256());
        auto m1 = _mm256_andnot_si256(m0, _mm256_cmpgt_epi64(min_bits, v_bits));
        auto m2 = _mm256_andnot_si256(_mm256_cmpgt_epi64(min_bits, v_bits), _mm256_cmpgt_epi64(inf_bits, v_bits));
        auto m3 = _mm256_cmpeq_epi64(v_bits, inf_bits);
        auto m4 = _mm256_cmpgt_epi64(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm256_setzero_si256();
        ret = _mm256_and_si256(m0, _mm256_set1_epi64x(FP_ZERO));
        ret = _mm256_ternarylogic_epi64(ret, m1, _mm256_set1_epi64x(FP_SUBNORMAL), 0xf8);
        ret = _mm256_ternarylogic_epi64(ret, m2, _mm256_set1_epi64x(FP_NORMAL),    0xf8);
        ret = _mm256_ternarylogic_epi64(ret, m3, _mm256_set1_epi64x(FP_INFINITE),  0xf8);
        ret = _mm256_ternarylogic_epi64(ret, m4, _mm256_set1_epi64x(FP_NAN),       0xf8);

        return vec4x64i{ret};

        #elif defined(AVEL_AVX2)
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm256_castpd_si256(decay(v));

        // Take absolute value
        v_bits = _mm256_and_si256(v_bits, _mm256_set1_epi64x(0x7fffffffffffffffull));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm256_set1_epi64x(0x10000000000000ull);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm256_set1_epi64x(0x7ff0000000000000ull);

        // Masks for individual categories
        auto m0 = _mm256_cmpeq_epi64(v_bits, _mm256_setzero_si256());
        auto m1 = _mm256_andnot_si256(m0, _mm256_cmpgt_epi64(min_bits, v_bits));
        auto m2 = _mm256_andnot_si256(_mm256_cmpgt_epi64(min_bits, v_bits), _mm256_cmpgt_epi64(inf_bits, v_bits));
        auto m3 = _mm256_cmpeq_epi64(v_bits, inf_bits);
        auto m4 = _mm256_cmpgt_epi64(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm256_setzero_si256();
        ret = _mm256_and_si256(m0, _mm256_set1_epi64x(FP_ZERO));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m1, _mm256_set1_epi64x(FP_SUBNORMAL)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m2, _mm256_set1_epi64x(FP_NORMAL)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m3, _mm256_set1_epi64x(FP_INFINITE)));
        ret = _mm256_or_si256(ret, _mm256_and_si256(m4, _mm256_set1_epi64x(FP_NAN)));

        return vec4x64i{ret};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isfinite(vec4x64f v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return !mask4x64f{_mm256_fpclass_pd_mask(decay(v), 0x01 | 0x08 | 0x10 | 0x80)};

        #elif defined(AVEL_SSE2)
        return (vec4x64f{-INFINITY} < v) && (v < vec4x64f{+INFINITY});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isinf(vec4x64f v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return mask4x64f{_mm256_fpclass_pd_mask(decay(v), 0x08 | 0x10)};

        #elif defined(AVEL_AVX2)
        return avel::abs(v) == vec4x64f{INFINITY};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isnan(vec4x64f v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return mask4x64f{_mm256_fpclass_pd_mask(decay(v), 0x80 | 0x01)};

        #elif defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_cmp_pd_mask(decay(v), decay(v), _CMP_UNORD_Q)};

        #elif defined(AVEL_AVX)
        return mask4x64f{_mm256_cmp_pd(decay(v), decay(v), _CMP_UNORD_Q)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isnormal(vec4x64f v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return !mask4x64f{_mm256_fpclass_pd_mask(decay(v), 0xBF)};

        #elif defined(AVEL_AVX2)
        auto abs_v = avel::abs(v);
        return (vec4x64f{DBL_MIN} <= abs_v) && (abs_v <= vec4x64f{DBL_MAX});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f signbit(vec4x64f arg) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return mask4x64f{_mm256_fpclass_pd_mask(decay(arg), 0x40 | 0x04 | 0x10)};

        #elif defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_cmplt_epi64_mask(_mm256_castpd_si256(decay(arg)), _mm256_setzero_si256())};

        #elif defined(AVEL_AVX2)
        auto ret_32 = _mm256_srai_epi32(_mm256_castpd_si256(decay(arg)), 31);
        auto ret = _mm256_shuffle_epi32(ret_32, 0xf5);
        return mask4x64f{_mm256_castsi256_pd(ret)};

        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask4x64f isgreater(vec4x64f x, vec4x64f y) {
        #if defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_cmp_pd_mask(decay(x), decay(y), _CMP_GT_OQ)};

        #elif defined(AVEL_AVX2)
        return mask4x64f{_mm256_cmp_pd(decay(x), decay(y), _CMP_GT_OQ)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isgreaterequal(vec4x64f x, vec4x64f y) {
        #if defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_cmp_pd_mask(decay(x), decay(y), _CMP_GE_OQ)};

        #elif defined(AVEL_AVX2)
        return mask4x64f{_mm256_cmp_pd(decay(x), decay(y), _CMP_GE_OQ)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isless(vec4x64f x, vec4x64f y) {
        #if defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_cmp_pd_mask(decay(x), decay(y), _CMP_LT_OQ)};

        #elif defined(AVEL_AVX2)
        return mask4x64f{_mm256_cmp_pd(decay(x), decay(y), _CMP_LT_OQ)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f islessequal(vec4x64f x, vec4x64f y) {
        #if defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_cmp_pd_mask(decay(x), decay(y), _CMP_LE_OQ)};

        #elif defined(AVEL_AVX2)
        return mask4x64f{_mm256_cmp_pd(decay(x), decay(y), _CMP_LE_OQ)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f islessgreater(vec4x64f x, vec4x64f y) {
        #if defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_cmp_pd_mask(decay(x), decay(y), _CMP_NEQ_OQ)};

        #elif defined(AVEL_AVX2)
        return mask4x64f{_mm256_cmp_pd(decay(x), decay(y), _CMP_NEQ_OQ)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x64f isunordered(vec4x64f x, vec4x64f y) {
        #if defined(AVEL_AVX512VL)
        return mask4x64f{_mm256_cmp_pd_mask(decay(x), decay(y), _CMP_UNORD_Q)};

        #elif defined(AVEL_AVX2)
        return mask4x64f{_mm256_cmp_pd(decay(x), decay(y), _CMP_UNORD_Q)};

        #endif
    }

}

#endif //AVEL_VEC4x64F_HPP

