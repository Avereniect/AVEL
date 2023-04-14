#ifndef AVEL_VEC16X32U_HPP
#define AVEL_VEC16X32U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x32u = Vector<std::uint32_t, 16>;
    using arr16x32u = std::array<std::uint32_t, 16>;
    using mask16x32u = Vector_mask<std::uint32_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x32u> div(vec16x32u numerator, vec16x32u denominator);
    vec16x32u broadcast_mask(mask16x32u m);
    vec16x32u blend(mask16x32u m, vec16x32u a, vec16x32u b);
    vec16x32u countl_one(vec16x32u x);





    template<>
    class Vector_mask<std::uint32_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = __mmask16;

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask(Vector_mask<std::int32_t, 16>);

        AVEL_FINL explicit Vector_mask(Vector_mask<float, 16>);

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 16>& arr) {
            std::uint64_t lo_half;
            std::memcpy(&lo_half, arr.data() + 0, 8);

            std::uint64_t hi_half;
            std::memcpy(&lo_half, arr.data() + 8, 8);

            //Assumes that the bitwise representation of true has the low bit
            //set, an assumption that holds when targeting the Itanium ABI
            std::uint64_t bit_mask = 0x0101010101010101;

            unsigned t0 = _pext_u64(lo_half, bit_mask);
            unsigned t1 = _pext_u64(hi_half, bit_mask);

            content = _mm512_int2mask((t1 << 8) | t0);
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

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL bool operator==(Vector_mask rhs) const noexcept {
            return _cvtmask16_u32(content) == _cvtmask16_u32(rhs);
        }

        [[nodiscard]]
        AVEL_FINL bool operator!=(Vector_mask rhs) const noexcept {
            return _cvtmask16_u32(content) != _cvtmask16_u32(rhs);
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            content = _kand_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            content = _kor_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            content = _kxor_mask16(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            return Vector_mask{_knot_mask16(content)};
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
            static const primitive full_masks[2] {
                _cvtu32_mask16(0),
                _cvtu32_mask16(-1)
            };

            return full_masks[x];
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask16x32u m) {
        return popcount(_mm512_mask2int(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask16x32u m) {
        return _mm512_mask2int(m);
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask16x32u m) {
        return 0xFFFF == _mm512_mask2int(m);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x32u m) {
        return !all(m);
    }





    template<>
    class alignas(64) Vector<std::uint32_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint32_t;

        using primitive = __m512i;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(Vector<std::int32_t, width> v);

        AVEL_FINL explicit Vector(Vector<float, width> v);

        AVEL_FINL explicit Vector(mask m):
            content(_mm512_mask_blend_epi32(m, _mm512_setzero_epi32(), _mm512_set1_epi32(1))) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm512_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            content(_mm512_loadu_si512(array.data())) {}

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

        AVEL_FINL Vector& operator=(scalar x) {
            content = _mm512_set1_epi32(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpeq_epi32_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpneq_epi32_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            return mask{_mm512_cmplt_epi32_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmple_epi32_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpgt_epi32_mask(decay(lhs), decay(rhs))};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            return mask{_mm512_cmpge_epi32_mask(decay(lhs), decay(rhs))};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() {
            return *this;
        }

        /*
        //TODO: Move definition elsewhere after vec4x32i has been defined
        [[nodiscard]]
        AVEL_FINL friend Vector<std::int32_t, 16> operator-(Vector v) {
            return Vector{} - v;
        }
        */

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content = _mm512_add_epi32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm512_sub_epi32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm512_mullo_epi32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.quot.content;
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector rhs) {
            auto results = div(*this, rhs);
            content = results.rem.content;
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
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector vec) {
            content = _mm512_and_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm512_or_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm512_xor_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(std::uint32_t s) {
            content = _mm512_sll_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t s) {
            content = _mm512_srl_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector s) {
            content = _mm512_sllv_epi32(content, s.content);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector s) {
            content = _mm512_srlv_epi32(content, s.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector operator~() const {
            return Vector{_mm512_ternarylogic_epi32(content, content, content, 0x01)};
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
            return mask{_mm512_test_epi32_mask(content, content)};

            #else
            return *this != Vector{0x00};
            #endif
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    static_assert(
        16 * sizeof(std::uint32_t) == sizeof(vec16x32u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint32_t extract(vec16x32u v) {
        static_assert(N <= vec16x32u::width, "Specified index does not exist");
        typename std::enable_if<N <= vec16x32u::width, int>::type dummy_variable = 0;

        auto quarter = _mm512_extracti32x4_epi32(decay(v), N / 4);
        return _mm_extract_epi32(quarter, N % 4);
    }

    template<std::uint32_t N>
    AVEL_FINL vec16x32u insert(vec16x32u v, std::uint32_t x) {
        static_assert(N <= vec16x32u::width, "Specified index does not exist");
        typename std::enable_if<N <= vec16x32u::width, int>::type dummy_variable = 0;

        //return vec16x32u{_mm512_insert_epi32(decay(v), x, N)};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32u broadcast_mask(mask16x32u m) {
        return vec16x32u{_mm512_movm_epi32(decay(m))};
    }

    AVEL_FINL vec16x32u blend(vec16x32u a, vec16x32u b, mask16x32u m) {
        return vec16x32u{_mm512_mask_blend_epi32(decay(m), decay(a), decay(b))};
    }

    AVEL_FINL vec16x32u max(vec16x32u a, vec16x32u b) {
        return vec16x32u{_mm512_max_epu32(decay(a), decay(b))};
    }

    AVEL_FINL vec16x32u min(vec16x32u a, vec16x32u b) {
        return vec16x32u{_mm512_min_epu32(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::pair<vec16x32u, vec16x32u> minmax(vec16x32u a, vec16x32u b) {
        return {
            vec16x32u{_mm512_min_epu32(decay(a), decay(b))},
            vec16x32u{_mm512_max_epu32(decay(a), decay(b))}
        };
    }

    AVEL_FINL vec16x32u clamp(vec16x32u x, vec16x32u lo, vec16x32u hi) {
        return vec16x32u{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u midpoint(vec16x32u a, vec16x32u b) {
        vec16x32u t0 = a & b & vec16x32u{0x1};
        vec16x32u t1 = (a | b) & vec16x32u{0x1} & broadcast_mask(a > b);
        vec16x32u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u average(vec16x32u a, vec16x32u b) {
        return (a >> 1) + (b >> 1) + (a & b & vec16x32u{0x1});;
    }

    //Definition of neg_abs delayed until later

    template<>
    AVEL_FINL vec16x32u load<vec16x32u>(const std::uint32_t* ptr) {
        return vec16x32u{_mm512_loadu_si512(ptr)};
    }

    template<>
    AVEL_FINL vec16x32u aligned_load<vec16x32u>(const std::uint32_t* ptr) {
        return vec16x32u{_mm512_load_si512(ptr)};
    }

    //Definition of gather deferred until definition of vector of signed integers


    AVEL_FINL void store(std::uint32_t* ptr, vec16x32u v) {
        _mm512_storeu_si512(reinterpret_cast<__m512i*>(ptr), decay(v));
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec16x32u v) {
        _mm512_store_si512(reinterpret_cast<__m512i*>(ptr), decay(v));
    }

    //Definition of scatter deferred until vector of signed integers is defined

    [[nodiscard]]
    AVEL_FINL std::array<std::uint32_t, 16> to_array(vec16x32u v) {
        alignas(64) std::array<std::uint32_t, 16> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer arithmetic functions
    //=====================================================

    AVEL_FINL div_type<vec16x32u> div(vec16x32u numerator, vec16x32u denominator) {
        vec16x32u quotient{};

        #if defined(AVEL_AVX512CD)
        auto t0 = _mm512_lzcnt_epi32(decay(numerator));
        auto t1 = _mm512_sub_epi32(t0, _mm512_set1_epi32(1));
        auto t2 = _mm512_subs_epi16(_mm512_lzcnt_epi32(decay(denominator)), t1);

        std::int32_t i = _mm512_reduce_max_epi32(t2);

        #else

        /*
        //auto d0 = _mm512_extracti32x8_epi32(denominator, 0x0);
        //auto d1 = _mm512_extracti32x8_epi32(denominator, 0x0);

         //TODO: Expand reduction to include all 16 lanes

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

        //TODO: Optimize case were a denominator is zero?
        //std::int32_t i = std::max(t0 - t1 + 1, 0);

        //These reductions likely don't perform well so take an alternative approach
        */

        std::int32_t i = 32;
        for (int j = 0; j < 5; ++j) {
            std::uint32_t shift_amount = (1 << j);
            bool condition = none((numerator >> j) >= denominator);
            if (condition) {
                j -= shift_amount;
            }
        }

        #endif

        for (; (i-- > 0) && any(mask16x32u(numerator));) {
            mask16x32u b = ((numerator >> i) >= denominator);
            numerator -= (broadcast_mask(b) & (denominator << i));
            quotient |= (vec16x32u{b} << i);
        }

        return {quotient, numerator};
    }

    AVEL_FINL vec16x32u popcount(vec16x32u v) {
        #if defined(AVEL_AVX512VPOPCNTDQ)
        return vec16x32u{_mm512_popcnt_epi32(decay(v))};

        #elif defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm512_popcnt_epi16(v);
        auto tmp1 = _mm512_slli_epi32(tmp0, 16);

        auto tmp2 = _mm512_add_epi32(tmp0, tmp1);

        return vec16x32u{_mm512_srli_epi32(tmp2, 16)};
        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec16x32u{0x55555555});                    // reuse input as temporary
        v = (v & vec16x32u{0x33333333}) + ((v >> 2) & vec16x32u{0x33333333});     // temp
        v = ((v + (v >> 4) & vec16x32u{0xF0F0F0F}) * vec16x32u{0x1010101}) >> 24; // count
        return v;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u byteswap(vec16x32u v) {
        alignas(64) static constexpr std::uint8_t index_data[64] {
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12,
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12,
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12,
            3,   2,  1,  0,
            7,   6,  5,  4,
            11, 10,  9,  8,
            15, 14, 13, 12
        };

        auto indices = _mm512_load_si512((const __m512i*)index_data);
        auto ret = vec16x32u{_mm512_shuffle_epi8(decay(v), indices)};
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countl_zero(vec16x32u x) {
        #if defined(AVEL_AVX512CD)
        return vec16x32u{_mm512_lzcnt_epi32(decay(x))};
        #elif defined(AVEL_AVX512F)
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        x = _mm512_andnot_si512(decay(x >> 1), decay(x));
        auto floats = _mm512_add_ps(_mm512_cvtepi32_ps(decay(x)), _mm512_set1_ps(0.5f));
        auto biased_exponents = (vec16x32u(_mm512_castps_si512(floats)) >> 23);
        auto lzcnt = _mm512_subs_epu16(decay(vec16x32u{158}), decay(biased_exponents));
        return vec16x32u{lzcnt};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countl_one(vec16x32u x) {
        #if defined(AVEL_AVX512CD)
        return vec16x32u{_mm512_lzcnt_epi32(decay(~x))};
        #elif defined(AVEL_AVX512F)
        return countl_zero(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countr_zero(vec16x32u x) {
        auto y = (x & (vec16x32u{} - x));
        auto floats = _mm512_cvtepi32_ps(decay(y));
        auto biased_exponents = decay(vec16x32u(_mm512_castps_si512(floats)) >> 23);
        biased_exponents = _mm512_min_epi16(decay(vec16x32u{158}), biased_exponents);
        auto tzcnt = vec16x32u{biased_exponents} - vec16x32u{127};
        tzcnt = blend(tzcnt, vec16x32u{32}, x == vec16x32u{});

        return vec16x32u{tzcnt};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u bit_width(vec16x32u x) {
        #if defined(AVEL_AVX512CD)
        return vec16x32u{32} - vec16x32u{_mm512_lzcnt_epi32(decay(x))};

        #elif defined(AVEL_AVX512)
        auto floats = _mm512_cvtepi32_ps(x);
        auto biased_exponents = (vec16x32u(_mm512_castps_si512(floats)) >> 23 & vec16x32u{0xFF});
        return vec16x32u{_mm512_subs_epu16(vec16x32u{156}, biased_exponents)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u bit_floor(vec16x32u x) {
        #if defined(AVEL_AVX512CD)
        vec16x32u leading_zeros = countl_zero(x);
        mask16x32u zero_mask = (leading_zeros != vec16x32u{32});

        return (vec16x32u{zero_mask} << (vec16x32u{31} - leading_zeros));
        #elif defined(AVEL_AVX512F)
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countr_one(vec16x32u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u bit_ceil(vec16x32u v) {
        #if defined(AVEL_AVX512CD)
        auto tmp = (vec16x32u{32} - countl_zero(v - vec16x32u{1}));
        return vec16x32u{1} << tmp;
        #elif defined(AVEL_AVX512F)
        //TODO: Optimize?
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
    AVEL_FINL mask16x32u has_single_bit(vec16x32u v) {
        #if defined(AVEL_AVX512VPOPCNTDQ)
        return popcount(v) == vec16x32u{1};

        #elif defined(AVEL_AVX512BITALG)
        return popcount(v) == vec16x32u{1};

        #else
        return mask16x32u{v} & !mask16x32u{v & (v - vec16x32u{1})};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u rotl(vec16x32u v, long long s) {
        return vec16x32u{_mm512_rolv_epi32(decay(v), _mm512_set1_epi32(s))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u rotl(vec16x32u v, vec16x32u s) {
        return vec16x32u{_mm512_rolv_epi32(decay(v), decay(s))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u rotr(vec16x32u v, long long s) {
        return vec16x32u{_mm512_rorv_epi32(decay(v), _mm512_set1_epi32(s))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u rotr(vec16x32u v, vec16x32u s) {
        return vec16x32u{_mm512_rorv_epi32(decay(v), decay(s))};
    }

}

#endif //AVEL_VEC16X32U_HPP
