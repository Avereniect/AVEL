#ifndef AVEL_VEC8X32U_HPP
#define AVEL_VEC8X32U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x32u = Vector<std::uint32_t, 8>;

    using mask8x32u = Vector_mask<std::uint32_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x32u> div(vec8x32u numerator, vec8x32u denominator);

    vec8x32u broadcast_bits(mask8x32u m);





    template<>
    class alignas(AVEL_MASK_ALIGNMENT_8X32U) Vector_mask<std::uint32_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL)
        using primitive = __mmask8;
        #elif defined(AVEL_AVX2)
        using primitive = __m256i;
        #elif defined(AVEL_AVX)
        using primitive = __m256;
        #endif

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask(Vector_mask<std::int32_t, 8>);

        AVEL_FINL explicit Vector_mask(Vector_mask<float, 8>);

        AVEL_FINL Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 8>& arr) {
            #if defined(AVEL_AVX512VL)
            static_assert(sizeof(bool) == 1);
            std::uint64_t array_data = avel::bit_cast<std::uint64_t>(arr);

            //Assumes that the bitwise representation of true has the low bit
            //set, an assumption that holds when targeting the Itanium ABI
            std::uint64_t bit_mask = 0x0101010101010101;

            unsigned t = _pext_u64(array_data, bit_mask);

            content = _mm512_int2mask(t);

            #elif defined(AVEL_AVX2)
            static_assert(sizeof(bool) == 1);

            alignas(32) static const std::uint32_t mask_data[8] {
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000,
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000
            };

            __m128i t0 = _mm_loadu_si32(arr.data() + 0);
            __m128i t1 = _mm_loadu_si32(arr.data() + 4);

            primitive t2 = _mm256_inserti128_si256(
                _mm256_zextsi128_si256(t0),
                t1,
                0x01
            );

            primitive mask = _mm256_load_si256((const __m256i*)mask_data);

            primitive t3 = _mm256_and_si256(t2, mask);
            content = _mm256_cmpgt_epi32(t3, _mm256_setzero_si256());
            #else
            static_assert(sizeof(bool) == 1);

            alignas(32) static const std::uint32_t mask_data[8] {
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000,
                0x000000FF,
                0x0000FF00,
                0x00FF0000,
                0xFF000000
            };

            __m128 t0 = _mm_load_ss(reinterpret_cast<const float*>(arr.data() + 0));
            __m128 t1 = _mm_load_ss(reinterpret_cast<const float*>(arr.data() + 4));

            primitive t2 = _mm256_insertf128_ps(
                _mm256_zextps128_ps256(t0),
                t1,
                0x01
            );

            primitive mask = _mm256_castsi256_ps(_mm256_load_si256((const __m256i*)mask_data));

            primitive t3 = _mm256_and_ps(t2, mask);
            content = _mm256_cmp_ps(t3, _mm256_setzero_ps(), _CMP_EQ_OQ);

            #endif
        }

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

        AVEL_FINL Vector_mask& operator=(const Vector_mask&) = default;
        AVEL_FINL Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return _mm512_mask2int(primitive(lhs)) == _mm512_mask2int(primitive(rhs));
            #elif defined(AVEL_AVX2)
            return (0xFFFFFFFF == _mm256_movemask_epi8(_mm256_cmpeq_epi32(lhs, rhs)));
            #elif defined(AVEL_AVX)
            return (0xFF == _mm256_movemask_ps(_mm256_xor_ps(lhs, rhs)));
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            return _mm512_mask2int(primitive(lhs)) != _mm512_mask2int(primitive(rhs));
            #elif defined(AVEL_AVX2)
            return (0xFFFFFFFF != _mm256_movemask_epi8(_mm256_cmpeq_epi32(lhs, rhs)));
            #elif defined(AVEL_AVX)
            return (0xFF != _mm256_movemask_ps(_mm256_xor_ps(lhs, rhs)));
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs);
            #elif defined(AVEL_AVX2)
            content = _mm256_and_si256(content, rhs);
            #elif defined(AVEL_AVX)
            content = _mm256_and_ps(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs);
            #elif defined(AVEL_AVX2)
            content = _mm256_or_si256(content, rhs);
            #elif defined(AVEL_AVX)
            content = _mm256_or_ps(content, rhs);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs);
            #elif defined(AVEL_AVX2)
            content = _mm256_xor_si256(content, rhs);
            #elif defined(AVEL_AVX)
            content = _mm256_xor_ps(content, rhs);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_knot_mask8(content)};
            #elif defined(AVEL_AVX512F)
            return Vector_mask{_mm256_ternarylogic_epi32(content, content, content, 0x01)};
            #elif defined(AVEL_AVX2)
            primitive tmp = _mm256_undefined_si256();
            return Vector_mask{_mm256_andnot_si256(content, _mm256_cmpeq_epi32(tmp, tmp))};
            #elif defined(AVEL_AVX)
            primitive tmp = _mm256_undefined_ps();
            return Vector_mask{_mm256_andnot_ps(content, _mm256_cmp_ps(tmp, tmp, _CMP_TRUE_US))};
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
                {0,  0, 0, 0},
                {-1, -1, -1, -1}
            };

            return full_masks[x];
            #endif
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    std::uint32_t count(mask8x32u m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(m));
        #elif defined(AVEL_AVX2)
        return popcount(_mm256_movemask_epi8(m)) / sizeof(std::uint32_t);
        #elif defined(AVEL_AVX)
        return popcount(_mm256_movemask_ps(m));
        #endif
    }

    [[nodiscard]]
    bool any(mask8x32u m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(m);
        #elif defined(AVEL_AVX2)
        return _mm256_movemask_epi8(m);
        #elif defined(AVEL_AVX)
        return !_mm256_testz_ps(m, m);
        #endif
    }

    [[nodiscard]]
    bool all(mask8x32u m) {
        #if defined(AVEL_AVX512VL)
        return 0xFF == _mm512_mask2int(m);
        #elif defined(AVEL_AVX2)
        return 0xFFFFFFFFu == _mm256_movemask_epi8(m);
        #elif defined(AVEL_AVX)
        return 0xFF == _mm256_movemask_ps(m);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x32u m) {
        #if defined(AVEL_AVX512VL)
        return !all(m);
        #elif defined(AVEL_AVX2)
        return _mm256_testz_si256(m, m);
        #elif defined(AVEL_AVX)
        return _mm256_testz_ps(m, m);
        #endif
    }





    template<>
    class Vector<std::uint32_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint32_t;

        using primitive = __m256i;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<unsigned M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(Vector<std::int32_t, width> v);

        AVEL_FINL explicit Vector(Vector<float, width> v);

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL)
            content(_mm256_mask_blend_epi32(m, _mm256_setzero_si256(), _mm256_set1_epi32(1))) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_sub_epi32(_mm256_setzero_si256(), m)) {}
        #elif defined(AVEL_AVX)
            content(
                _mm256_castps_si256(_mm256_blendv_ps(
                    _mm256_castsi256_ps(_mm256_set1_epi32(0x0)),
                    _mm256_castsi256_ps(_mm256_set1_epi32(0x1)),
                    m
                ))) {}
        #endif

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const scalar x):
            content(_mm256_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            content(_mm256_loadu_si256(reinterpret_cast<const primitive*>(array.data()))) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(const Vector&) = default;
        AVEL_FINL Vector& operator=(Vector&&) = default;

        AVEL_FINL Vector& operator=(primitive p) {
            this->content = p;
            return *this;
        }

        #if defined(AVEL_AVX2)
        AVEL_FINL Vector& operator=(scalar x) {
            content = _mm256_broadcastd_epi32(_mm_cvtsi32_si128(x));
            return *this;
        }
        #endif

        //=================================================
        // Comparison operators
        //=================================================

        #if defined(AVEL_AVX2)

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpeq_epu32_mask(lhs, rhs)};
            #else
            return mask{_mm256_cmpeq_epi32(lhs, rhs)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpneq_epu32_mask(lhs, rhs)};
            #else
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmplt_epu32_mask(lhs, rhs)};
            #else
            Vector addition_mask{0x80000000};
            return mask{_mm256_cmpgt_epi32(rhs ^ addition_mask, lhs ^ addition_mask)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmple_epu32_mask(lhs, rhs)};
            #else
            return !mask{lhs > rhs};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpgt_epu32_mask(lhs, rhs)};
            #else
            Vector addition_mask{0x80000000};
            return mask{_mm256_cmpgt_epi32(lhs ^ addition_mask, rhs ^ addition_mask)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL)
            return mask{_mm256_cmpge_epu32_mask(lhs, rhs)};
            #else
            return !mask{lhs < rhs};
            #endif
        }

        #endif

        //=================================================
        // Unary arithmetic operators
        //=================================================

        #if defined(AVEL_AVX2)

        [[nodiscard]]
        AVEL_FINL Vector operator+() {
            return *this;
        }

        /*
        //TODO: Move definition elsewhere after vec8x32i has been defined
        [[nodiscard]]
        AVEL_FINL friend Vector<std::int32_t, 8> operator-(Vector v) {
            return Vector{} - v;
        }
        */

        #endif

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        #if defined(AVEL_AVX2)
        AVEL_FINL Vector& operator+=(Vector vec) {
            content = _mm256_add_epi32(content, vec);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm256_sub_epi32(content, vec);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm256_mullo_epi32(content, vec);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            auto results = div(*this, vec);
            content = results.quot;
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
            auto results = div(*this, vec);
            content = results.rem;
            return *this;
        }
        #endif

        //=================================================
        // Arithmetic operators
        //=================================================

        #if defined(AVEL_AVX2)

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
        #endif

        //=================================================
        // Increment/Decrement operators
        //=================================================

        #if defined(AVEL_AVX2)
        AVEL_FINL Vector& operator++() {
            #if defined(AVEL_AVX2)
            *this += Vector{1};
            return *this;
            #endif
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
        #endif

        //=================================================
        // Bitwise assignment operators
        //=================================================

        #if defined(AVEL_AVX2)
        AVEL_FINL Vector& operator&=(Vector vec) {
            content = _mm256_and_si256(content, vec);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm256_or_si256(content, vec);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm256_xor_si256(content, vec);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(std::uint64_t s) {
            content = _mm256_sll_epi32(content, _mm_loadu_si64(&s));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint64_t s) {
            content = _mm256_srl_epi32(content, _mm_loadu_si64(&s));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector s) {
            content = _mm256_sllv_epi32(content, s);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector s) {
            content = _mm256_srlv_epi32(content, s);
            return *this;
        }
        #endif

        //=================================================
        // Bitwise operators
        //=================================================

        #if defined(AVEL_AVX2)
        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512F)
            return Vector{_mm256_ternarylogic_epi32(content, content, content, 0x01)};
            #else
            return Vector{_mm256_andnot_si256(content, vec8x32u{0xFFFFFFFF})};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator&(Vector lhs, Vector rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator|(Vector lhs, Vector rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator^(Vector lhs, Vector rhs) {
            lhs ^= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, std::uint32_t rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, std::uint32_t rhs) {
            lhs >>= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, Vector rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, Vector rhs) {
            lhs >>= rhs;
            return lhs;
        }
        #endif

        //=================================================
        // Conversions
        //=================================================

        [[nodiscard]]
        AVEL_FINL operator primitive() const {
            return content;
        }

        #if defined(AVEL_AVX2)
        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return Vector{} < *this;
        }
        #endif

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    //=====================================================
    // General vector operations
    //=====================================================

    #if defined(AVEL_AVX2)
    /*
    template<unsigned O>
    [[nodiscard]]
    AVEL_FINL vec8x32u funnel_shift(vec8x32u a, vec8x32u b) {
        static_assert(O < 8, "");


    }
    */

    [[nodiscard]]
    AVEL_FINL vec8x32u broadcast_bits(mask8x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec8x32u{_mm256_movm_epi32(m)};
        #elif defined(AVEL_AVX512VL)
        const auto x = _mm256_set1_epi32(0);
        const auto y = _mm256_set1_epi32(-1);
        return vec8x32u{_mm256_set1_epi32(m, x, y)};
        #elif defined(AVEL_AVX2)
        return vec8x32u{mask8x32u::primitive(m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u blend(vec8x32u a, vec8x32u b, vec8x32u::mask m) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_mask_blend_epi32(
            m,
            static_cast<__m256i>(a),
            static_cast<__m256i>(b)
            )
        };
        #elif defined(AVEL_AVX2)
        return vec8x32u{_mm256_blendv_epi8(a, b, m)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u max(vec8x32u a, vec8x32u b) {
        return vec8x32u{_mm256_max_epu32(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u min(vec8x32u a, vec8x32u b) {
        return vec8x32u{_mm256_min_epu32(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL std::pair<vec8x32u, vec8x32u> minmax(vec8x32u a, vec8x32u b) {
        return {
            vec8x32u{_mm256_min_epu32(a, b)},
            vec8x32u{_mm256_max_epu32(a, b)}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u clamp(vec8x32u x, vec8x32u lo, vec8x32u hi) {
        return vec8x32u{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u midpoint(vec8x32u a, vec8x32u b) {
        vec8x32u t0 = a & b & vec8x32u{0x1};
        vec8x32u t1 = (a | b) & vec8x32u{0x1} & broadcast_bits(a > b);
        vec8x32u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u average(vec8x32u a, vec8x32u b) {
        return (a >> 1) + (b >> 1) + (a & b & vec8x32u{0x1});;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u load<vec8x32u>(const std::uint32_t* ptr) {
        return vec8x32u{_mm256_loadu_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u aligned_load<vec8x32u>(const std::uint32_t* ptr) {
        return vec8x32u{_mm256_load_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x32u stream_load<vec8x32u>(const std::uint32_t* ptr) {
        return vec8x32u{_mm256_stream_load_si256(reinterpret_cast<const __m256i*>(ptr))};
    }

    //Definition of gather deferred until vector of signed integers is defined

    AVEL_FINL void store(std::uint32_t* ptr, vec8x32u v) {
        _mm256_storeu_si256(reinterpret_cast<__m256i*>(ptr), v);
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec8x32u v) {
        _mm256_store_si256(reinterpret_cast<__m256i*>(ptr), v);
    }

    AVEL_FINL void stream_store(std::uint32_t* ptr, vec8x32u v) {
        _mm256_stream_si256(reinterpret_cast<__m256i*>(ptr), v);
    }

    //Definition of scatter deferred until vector of signed integers is defined

    [[nodiscard]]
    AVEL_FINL std::array<std::uint32_t, 8> to_array(vec8x32u v) {
        alignas(32) std::array<std::uint32_t, 8> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer arithmetic functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x32u> div(vec8x32u numerator, vec8x32u denominator) {
        vec8x32u quotient{};

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        auto t0 = _mm256_sub_epi32(_mm256_lzcnt_epi32(denominator), _mm256_lzcnt_epi32(numerator));
        auto t1 = _mm256_add_epi32(t0, _mm256_set1_epi32(1));
        auto t2 = _mm256_max_epu32(t1, _mm256_set1_epi32(0));

        auto s0 = _mm256_extracti128_si256(t2, 0x0);
        auto s1 = _mm256_extracti128_si256(t2, 0x1);
        auto s2 = _mm_max_epi32(s0, s1);
        auto s3 = _mm_bsrli_si128(s2, 8);
        auto s4 = _mm_max_epi32(s2, s3);
        auto s5 = _mm_bsrli_si128(s2, 4);
        auto s6 = _mm_max_epi32(s4, s5);

        std::int32_t i = _mm_cvtsi128_si32(s6);

        //TODO: Test
        //TODO: Optimize case were a denominator is zero?
        #else

        auto d0 = _mm256_extracti128_si256(denominator, 0x0);
        auto d1 = _mm256_extracti128_si256(denominator, 0x1);
        auto d2 = _mm_min_epi32(d0, d1);
        auto d3 = _mm_bsrli_si128(d2, 8);
        auto d4 = _mm_min_epi32(d2, d3);
        auto d5 = _mm_bsrli_si128(d4, 4);
        auto d6 = _mm_min_epi32(d4, d5);

        auto n0 = _mm256_extracti128_si256(numerator, 0x0);
        auto n1 = _mm256_extracti128_si256(numerator, 0x1);
        auto n2 = _mm_max_epi32(n0, n1);
        auto n3 = _mm_bsrli_si128(n2, 8);
        auto n4 = _mm_max_epi32(n2, n3);
        auto n5 = _mm_bsrli_si128(n4, 4);
        auto n6 = _mm_max_epi32(n4, n5);

        std::int32_t t0 = countl_zero(_mm_cvtsi128_si32(d4));
        std::int32_t t1 = countl_zero(_mm_cvtsi128_si32(n4));

        //TODO: Optimize case were a denominator is zero ?
        std::int32_t i = 32; //std::max(t0 - t1 + 1, 0);
        #endif

        for (; (i-- > 0) && any(mask8x32u(numerator));) {
            mask8x32u b = ((numerator >> i) >= denominator);
            numerator -= (broadcast_bits(b) & (denominator << i));
            quotient |= (vec8x32u{b} << i);
        }

        return {quotient, numerator};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u popcount(vec8x32u v) {
        #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512VPOPCNTDQ)
        return vec8x32u{_mm256_popcnt_epi32(v)};
        #elif defined(AVELAVX512VL) & defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm256_popcnt_epi16(v);
        auto tmp1 = _mm256_slli_epi32(tmp0, 16);

        auto tmp2 = _mm256_add_epi32(tmp0, tmp1);

        return vec8x32u{_mm256_srli_epi32(tmp2, 16)};
        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec8x32u{0x55555555});                    // reuse input as temporary
        v = (v & vec8x32u{0x33333333}) + ((v >> 2) & vec8x32u{0x33333333});     // temp
        v = ((v + (v >> 4) & vec8x32u{0xF0F0F0F}) * vec8x32u{0x1010101}) >> 24; // count
        return v;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u byteswap(vec8x32u v) {
        alignas(32) static constexpr std::uint8_t index_data[32] {
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12,
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12,
        };

        auto indices = _mm256_load_si256((const __m256i*)index_data);
        auto ret = vec8x32u{_mm256_shuffle_epi8(v, indices)};
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u countl_zero(vec8x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec8x32u{_mm256_lzcnt_epi32(x)};
        #elif defined(AVEL_AVX2)
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        x = _mm256_andnot_si256(x >> 1, x);
        auto floats = _mm256_add_ps(_mm256_cvtepi32_ps(x), _mm256_set1_ps(0.5f));
        auto biased_exponents = (vec8x32u(_mm256_castps_si256(floats)) >> 23);
        auto lzcnt = _mm256_subs_epu16(vec8x32u{158}, biased_exponents);
        return vec8x32u{lzcnt};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u countl_one(vec8x32u x) {
        return countl_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u countr_zero(vec8x32u x) {
        auto y = (x & (vec8x32u{} - x));
        auto floats = _mm256_cvtepi32_ps(y);
        auto biased_exponents = (vec8x32u(_mm256_castps_si256(floats)) >> 23);
        biased_exponents = _mm256_min_epi16(vec8x32u{158}, biased_exponents);
        auto tzcnt = biased_exponents - vec8x32u{127};
        tzcnt = blend(tzcnt, vec8x32u{32}, x == vec8x32u{});

        return vec8x32u{tzcnt};
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u countr_one(vec8x32u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u bit_width(vec8x32u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec8x32u{32} - vec8x32u{_mm256_lzcnt_epi32(x)};
        #elif defined(AVEL_AVX2)
        auto floats = _mm256_cvtepi32_ps(x);
        auto biased_exponents = (vec8x32u(_mm256_castps_si256(floats)) >> 23 & vec8x32u{0xFF});
        return vec8x32u{_mm256_subs_epu16(vec8x32u{156}, biased_exponents)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u bit_floor(vec8x32u x) {
        #if defined(AVEL_AVX512CD) && defined(AVEL_AVX512VL)
        vec8x32u leading_zeros = countl_zero(x);
        mask8x32u zero_mask = (leading_zeros != vec8x32u{32});

        return (vec8x32u{zero_mask} << (vec8x32u{31} - leading_zeros));
        #elif defined(AVEL_AVX2)
        //TODO: Optimize ?
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u bit_ceil(vec8x32u v) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)
        return vec8x32u{1} << (vec8x32u{32} << countl_zero(v));
        #elif defined(AVEL_SSE2)
        //TODO: Optimize ?
        --v;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        ++v;
        return v;
        #endif
    };

    [[nodiscard]]
    AVEL_FINL mask8x32u has_single_bit(vec8x32u v) {
        #if (defined(AVEL_AVX512VPOPCNTDQ) || defined(AVEL_AVX512BITALG)) && defined(AVEL_AVX512VL)
        return popcount(v) == vec8x32u{1};
        #else
        return mask8x32u{v} & !mask8x32u{v & (v - vec8x32u{1})};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u rotl(vec8x32u v, std::uint32_t s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rolv_epi32(v, vec8x32u{s})};
        #elif defined(AVEL_AVX2)
        s &= 0x1F;
        return (v << s) | (v >> (32 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u rotl(vec8x32u v, vec8x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rolv_epi32(v, s)};
        #elif defined(AVEL_AVX2)
        s &= vec8x32u(0x1F);
        return (v << s) | (v >> (vec8x32u{32} - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u rotr(vec8x32u v, std::uint32_t s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rorv_epi32(v, vec8x32u{s})};
        #elif defined(AVEL_AVX2)
        s &= 0x1F;
        return (v >> s) | (v << (32 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x32u rotr(vec8x32u v, vec8x32u s) {
        #if defined(AVEL_AVX512VL)
        return vec8x32u{_mm256_rorv_epi32(v, s)};

        #elif defined(AVEL_AVX2)
        s &= vec8x32u(0x1F);
        return (v >> s) | (v << (vec8x32u{32} - s));
        #endif
    }

    #endif
}

#endif //AVEL_VEC8X32U_HPP
