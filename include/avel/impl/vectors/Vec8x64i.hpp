#ifndef AVEL_VEC8X64I_HPP
#define AVEL_VEC8X64I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x64i = Vector<std::int64_t, 8>;
    using arr8x64i = std::array<std::int64_t, 8>;
    using mask8x64i = Vector_mask<std::int64_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x64i> div(vec8x64i numerator, vec8x64i denominator);
    vec8x64i broadcast_mask(mask8x64i m);
    vec8x64i blend(mask8x64i m, vec8x64i a, vec8x64i b);
    vec8x64i negate(vec8x64i m, vec8x64i x);





    template<>
    class Vector_mask<std::int64_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = __mmask8;

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> m):
            Vector_mask(convert<Vector_mask>(m)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive p):
            content(p) {}

        AVEL_FINL explicit Vector_mask(bool b):
            content(b ? -1 : 0) {}

        AVEL_FINL explicit Vector_mask(const arr8xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            content = _mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data);

            #elif defined(AVEL_AVX512F)
            auto array_data = _mm_cvtsi64_si128(bit_cast<std::uint64_t>(arr));
            auto expanded = _mm_cvtepi8_epi64(array_data);
            content = _mm_cmplt_epu64_mask(_mm_setzero_si128(), expanded);

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
            #if defined(AVEL_AVX512DQ)
            auto tmp = _kxor_mask8(decay(lhs), decay(rhs));
            return _kortestz_mask8_u8(tmp, tmp);

            #elif defined(AVEL_AVX512F)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512DQ)
            auto tmp = _kxor_mask8(decay(lhs), decay(rhs));
            return !_kortestz_mask8_u8(tmp, tmp);

            #elif defined(AVEL_AVX512F)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512F)
            content &= rhs.content;
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512F)
            content |= rhs.content;
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512F)
            content ^= rhs.content;
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512DQ)
            return Vector_mask{_knot_mask8(content)};

            #elif defined(AVEL_AVX512F)
            return Vector_mask{static_cast<primitive>(~content)};
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

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x64u, 1> convert<mask8x64u, mask8x64i>(mask8x64i m) {
        return {mask8x64u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x64i, 1> convert<mask8x64i, mask8x64u>(mask8x64u m) {
        return {mask8x64i(decay(m))};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask8x64i m) {
        return count(mask8x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x64i m) {
        return any(mask8x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask8x64i m) {
        return all(mask8x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x64i m) {
        return none(mask8x64u{m});
    }





    template<>
    class Vector<std::int64_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int64_t;

        using primitive = __m512i;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
        using rebind_width = Vector<scalar, M>;

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> x):
            Vector(convert<Vector>(x)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512F)
            content(_mm512_maskz_set1_epi64(decay(m), 0x1)) {}
        #endif

        AVEL_FINL Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX512F)
            content(_mm512_set1_epi64(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr8x64i& arr):
        #if defined(AVEL_AVX512F)
            content(_mm512_loadu_si512(arr.data())) {}
        #endif

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
        Vector& operator=(Vector&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmpeq_epi64_mask(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmpneq_epi64_mask(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmplt_epi64_mask(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmple_epi64_mask(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmpgt_epi64_mask(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_cmpge_epi64_mask(lhs.content, rhs.content)};
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
            return Vector{0x00} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_add_epi64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_sub_epi64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_AVX512DQ)
            content = _mm512_mullo_epi64(content, decay(rhs));
            #elif defined(AVEL_AVX512F)
            content = _mm512_mullox_epi64(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.quot.content;
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
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

        AVEL_FINL Vector& operator&=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_and_si512(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_or_si512(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_xor_si512(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_sll_epi64(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_srl_epi64(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_sllv_epi64(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512F)
            content = _mm512_srav_epi64(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512F)
            return Vector{_mm512_ternarylogic_epi64(content, content, content, 0x01)};
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
        AVEL_FINL friend Vector operator<<(Vector lhs, long long rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, long long rhs) {
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
        // Conversion operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_test_epi64_mask(content, content)};
            #endif
        }

    };

    static_assert(
        8 * sizeof(std::int64_t) == sizeof(vec8x64i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x64u, 1> convert<vec8x64u, vec8x64i>(vec8x64i x) {
        return {vec8x64u{decay(x)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x64i, 1> convert<vec8x64i, vec8x64u>(vec8x64u x) {
        return {vec8x64i{decay(x)}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec8x64i operator-(vec8x64u v) {
        return vec8x64i{0x00} - vec8x64i{v};
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int64_t extract(vec8x64i v) {
        static_assert(N < 8, "Specified index does not exist");
        typename std::enable_if<N < 8, int>::type dummy_variable = 0;

        return static_cast<std::int64_t>(extract<N>(vec8x64i{v}));
    }

    template<std::uint32_t N>
    AVEL_FINL vec8x64i insert(vec8x64i v, std::int64_t x) {
        static_assert(N < 8, "Specified index does not exist");
        typename std::enable_if<N < 8, int>::type dummy_variable = 0;

        return vec8x64i{insert<N>(vec8x64i{v}, static_cast<std::int64_t>(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL  vec8x64i bit_shift_left(vec8x64i v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        return vec8x64i{bit_shift_left<S>(vec8x64u{v})};
    }

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL  vec8x64i bit_shift_right(vec8x64i v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512F)
        return vec8x64i{_mm512_srai_epi64(decay(v), S)};
        #endif
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x64i rotl(vec8x64i v) {
        return vec8x64i{rotl<S>(vec8x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i rotl(vec8x64i v, long long s) {
        return vec8x64i{rotl(vec8x64u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i rotl(vec8x64i v, vec8x64i s) {
        return vec8x64i{rotl(vec8x64u{v}, vec8x64u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec8x64i rotr(vec8x64i v) {
        return vec8x64i{rotr<S>(vec8x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i rotr(vec8x64i v, long long s) {
        return vec8x64i{rotr(vec8x64u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i rotr(vec8x64i v, vec8x64i s) {
        return vec8x64i{rotr(vec8x64u{v}, vec8x64u{s})};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x64i broadcast_mask(mask8x64i m) {
        return vec8x64i{broadcast_mask(mask8x64u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i blend(mask8x64i m, vec8x64i a, vec8x64i b) {
        return vec8x64i{blend(mask8x64u{m}, vec8x64u{a}, vec8x64u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i byteswap(vec8x64i v) {
        return vec8x64i{byteswap(vec8x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i max(vec8x64i a, vec8x64i b) {
        #if defined(AVEL_AVX512F)
        return vec8x64i{_mm512_max_epi64(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i min(vec8x64i a, vec8x64i b) {
        #if defined(AVEL_AVX512F)
        return vec8x64i{_mm512_min_epi64(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x64i, 2> minmax(vec8x64i a, vec8x64i b) {
        #if defined(AVEL_AVX512F)
        return {
            vec8x64i{_mm512_min_epi64(decay(a), decay(b))},
            vec8x64i{_mm512_max_epi64(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i clamp(vec8x64i x, vec8x64i lo, vec8x64i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i average(vec8x64i a, vec8x64i b) {
        #if defined(AVEL_AVX512F)
        auto avg = (a & b) + ((a ^ b) >> 1);
        auto c = broadcast_mask((a < -b) | b == vec8x64i{std::int64_t(1) << 63}) & (a ^ b) & vec8x64i{1};

        return avg + c;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i midpoint(vec8x64i a, vec8x64i b) {
        #if defined(AVEL_AVX512F)
        auto avg = decay(((a ^ b) >> 1) + (a & b));

        auto bias = _mm512_ternarylogic_epi32(decay(a), decay(b), _mm512_set1_epi64(0x1), 0x28);
        auto mask = _mm512_cmplt_epi64_mask(decay(b), decay(a));
        auto ret = _mm512_mask_add_epi64(avg, mask, avg, bias);

        return vec8x64i{ret};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i negate(mask8x64i m, vec8x64i v) {
        #if defined(AVEL_AVX512F)
        return vec8x64i{_mm512_mask_sub_epi64(decay(v), decay(m), _mm512_setzero_si512(), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i abs(vec8x64i v) {
        #if defined(AVEL_AVX512VF)
        return vec8x64i{_mm512_abs_epi64(decay(x))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i neg_abs(vec8x64i v) {
        #if defined(AVEL_AVX512VF)
        return -abs(v);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i neg_abs(vec8x64u v) {
        return neg_abs(vec8x64i{v});
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64i load<vec8x64i>(const std::int64_t* ptr, std::uint32_t n) {
        // $<load(const scalar*, std::uint32_t)>
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64i load<vec8x64i, vec8x64i::width>(const std::int64_t* ptr) {
        // $<load<width>(const scalar*)>
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64i aligned_load<vec8x64i>(const std::int64_t* ptr, std::uint32_t n) {
        //$<aligned_load(const scalar*, std::uint32_t)>
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x64i aligned_load<vec8x64i, vec8x64i::width>(const std::int64_t* ptr) {
        // $<aligned_load<width>(const scalar*)>
    }


    template<std::uint32_t N = vec8x64i::width>
    AVEL_FINL vec8x64i gather(std::int64_t* ptr, vec8x64i indices) {
        // $<gather(scalar*, vec)>
    }

    template<>
    AVEL_FINL vec8x64i gather<0>(std::int64_t* ptr, vec8x64i indices) {
        return vec8x64i{0x00};
    }

    template<>
    AVEL_FINL vec8x64i gather<vec8x64i::width>(std::int64_t* ptr, vec8x64i indices) {
        // $<gather<width>(scalar*, vec)>
    }

    AVEL_FINL vec8x64i gather(std::int64_t* ptr, vec8x64i indices, std::uint32_t n) {
        // $<gather(scalar*, vec, std::uint32_t n)>
    }



    template<std::uint32_t N = vec8x64u::width>
    AVEL_FINL vec8x64u gather(std::int64_t* ptr, vec8x64i indices) {
        // $<gather(uscalar*, uvec)>
    }

    template<>
    AVEL_FINL vec8x64u gather<0>(std::int64_t* ptr, vec8x64i indices) {
        return vec8x64u{0x00};
    }

    template<>
    AVEL_FINL vec8x64u gather<vec8x64u::width>(std::int64_t* ptr, vec8x64i indices) {
        // $<gather<width>(uscalar*, uvec)>
    }

    AVEL_FINL vec8x64u gather(std::uint64_t* ptr, vec8x64i indices, std::uint32_t n) {
        // $<gather(uscalar*, uvec, std::uint32_t n)>
    }



    AVEL_FINL void store(std::int64_t* ptr, vec8x64i v, std::uint32_t n) {
        // $<store(scalar*, vec, std::uint32_t)>
    }

    template<std::uint32_t N = vec8x64i::width>
    AVEL_FINL void store(std::int64_t* ptr, vec8x64i v) {
        // $<store<std::uint32_t>(scalar*, vec)>
    }

    template<>
    AVEL_FINL void store<vec8x64i::width>(std::int64_t* ptr, vec8x64i v) {
        // $<store<VECTOR_WIDTH>(scalar*, vec)>
    }



    AVEL_FINL void aligned_store(std::int64_t* ptr, vec8x64i v, std::uint32_t n) {
        // $<aligned_store(scalar*, vec, std::uint32_t)>
    }

    template<std::uint32_t N = vec8x64i::width>
    AVEL_FINL void aligned_store(std::int64_t* ptr, vec8x64i v) {
        // $<aligned_store<std::uint32_t>(scalar*, vec)>
    }

    template<>
    AVEL_FINL void aligned_store<vec8x64i::width>(std::int64_t* ptr, vec8x64i v) {
        // $<aligned_store<VECTOR_WIDTH>(scalar*, vec)>
    }


    AVEL_FINL void scatter(const std::int64_t* ptr, vec8x64i v, vec8x64i indices, std::uint32_t n) {
        // $<scatter(const scalar*, vec, vec, std::uint32_t)>
    }

    template<std::uint32_t N = vec8x64i::width>
    AVEL_FINL void scatter(const std::int64_t* ptr, vec8x64i v, vec8x64i indices) {
        // $<scatter(const scalar*, vec, vec)>
    }

    template<>
    AVEL_FINL void scatter<0>(const std::int64_t* ptr, vec8x64i v, vec8x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec8x64i::width>(const std::int64_t* ptr, vec8x64i v, vec8x64i indices) {
        // $<scatter<width>(const scalar*, vec, vec)>
    }



    AVEL_FINL void scatter(const std::uint64_t* ptr, vec8x64u v, vec8x64i indices, std::uint32_t n) {
        // $<scatter(const uscalar*, uvec, vec, std::uint32_t)>
    }

    template<std::uint32_t N = vec8x64i::width>
    AVEL_FINL void scatter(const std::uint64_t* ptr, vec8x64u v, vec8x64i indices) {
        // $<scatter(const uscalar*, uvec, vec)>
    }

    template<>
    AVEL_FINL void scatter<0>(const std::uint64_t* ptr, vec8x64u v, vec8x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec8x64i::width>(const std::uint64_t* ptr, vec8x64u v, vec8x64i indices) {
        // $<scatter<width>(const uscalar*, uvec, vec)>
    }



    [[nodiscard]]
    AVEL_FINL arr8x64i to_array(vec8x64i v) {
        alignas(64) arr8x64i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x64i> div(vec8x64i x, vec8x64i y) {
        auto n0 = extract<0>(x);
        auto n1 = extract<1>(x);
        auto n2 = extract<2>(x);
        auto n3 = extract<3>(x);
        auto n4 = extract<4>(x);
        auto n5 = extract<5>(x);
        auto n6 = extract<6>(x);
        auto n7 = extract<7>(x);

        auto d0 = extract<0>(y);
        auto d1 = extract<1>(y);
        auto d2 = extract<2>(y);
        auto d3 = extract<3>(y);
        auto d4 = extract<4>(y);
        auto d5 = extract<5>(y);
        auto d6 = extract<6>(y);
        auto d7 = extract<7>(y);

        vec8x64i quotient{};
        quotient = insert<0>(quotient, n0 / d0);
        quotient = insert<1>(quotient, n1 / d1);
        quotient = insert<0>(quotient, n2 / d2);
        quotient = insert<0>(quotient, n3 / d3);
        quotient = insert<0>(quotient, n4 / d4);
        quotient = insert<0>(quotient, n5 / d5);
        quotient = insert<0>(quotient, n6 / d6);
        quotient = insert<0>(quotient, n7 / d7);

        vec8x64i remainder{};
        remainder = insert<1>(remainder, n0 % d0);
        remainder = insert<1>(remainder, n1 % d1);
        remainder = insert<1>(remainder, n2 % d2);
        remainder = insert<1>(remainder, n3 % d3);
        remainder = insert<1>(remainder, n4 % d4);
        remainder = insert<1>(remainder, n5 % d5);
        remainder = insert<1>(remainder, n6 % d6);
        remainder = insert<1>(remainder, n7 % d7);

        return {quotient, remainder};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i popcount(vec8x64i v) {
        return vec8x64i{popcount(vec8x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i countl_zero(vec8x64i v) {
        return vec8x64i{countl_zero(vec8x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i countl_one(vec8x64i v) {
        return vec8x64i{countl_one(vec8x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i countr_zero(vec8x64i v) {
        return vec8x64i{countr_zero(vec8x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x64i countr_one(vec8x64i v) {
        return vec8x64i{countr_one(vec8x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL mask8x64i has_single_bit(vec8x64i v) {
        return mask8x64i{has_single_bit(vec8x64u{v})};
    }

}

#endif //AVEL_VEC8X64I_HPP
