#ifndef AVEL_VEC4X64U_HPP
#define AVEL_VEC4X64U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x64u = Vector<std::uint64_t, 4>;
    using arr4x64u = std::array<std::uint64_t, 4>;
    using mask4x64u = Vector_mask<std::uint64_t, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec4x64u> div(vec4x64u numerator, vec4x64u denominator);
    vec4x64u broadcast_mask(mask4x64u m);
    vec4x64u blend(mask4x64u m, vec4x64u a, vec4x64u b);
    vec4x64u countl_one(vec4x64u x);





    template<>
    class Vector_mask<std::uint64_t, 4> {
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
        #elif defined(AVEL_AVX2)
        using primitive = __m256i;
        #endif

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
        #if defined(AVEL_AVX512VL)
            content(p & 0x0F) {}
        #elif defined(AVEL_AVX2)
            content(p) {}
        #endif

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL)
            content(b ? -1 : 0) {}
        #elif defined(AVEL_AVX2)
            content(b ? _mm256_set1_epi64x(-1) : _mm256_setzero_si256()) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr4xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL)
            //TODO: Implement

            #elif defined(AVEL_AVX2)
            //TODO: Implement

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
            //TODO: Implement
            #elif defined(AVEL_AVX2)
            //TODO: Implement
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL)
            //TODO: Implement
            #elif defined(AVEL_AVX2)
            //TODO: Implement
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content &= rhs.content;
            #elif defined(AVEL_AVX2)
            content = _mm256_and_si256(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content |= rhs.content;
            #elif defined(AVEL_AVX2)
            content = _mm256_or_si256(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content ^= rhs.content;
            #elif defined(AVEL_AVX2)
            content = _mm256_xor_si256(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{primitive(content ^ 0xf)};
            #elif defined(AVEL_AVX2)
            auto ones = _mm256_set1_epi64x(-1);
            return Vector_mask{_mm256_andnot_si256(content, ones)};
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
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask4x64u m) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x64u m) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x64u m) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x64u m) {
        //TODO: Implement
    }





    template<>
    class Vector<std::uint64_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint64_t;

        #if defined(AVEL_AVX2)
        using primitive = __m256i;
        #endif

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
        #if defined(AVEL_AVX512VL)
            content(_mm256_maskz_set1_epi64(decay(m), 0x1)) {}
        #elif defined(AVEL_AVX2)
            content(_mm256_sub_epi64(_mm256_setzero_si256(), decay(m))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            content(_mm256_set1_epi64x(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr4x64u& arr):
        #if defined(AVEL_AVX2)
            content(_mm256_loadu_si256(reinterpret_cast<const __m256i*>(arr.data()))) {}
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
            //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
           //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
           //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            //TODO: Implement
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            //TODO: Implement
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        //Definition of operator-() deferred until later

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            //TODO: Implement
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
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            //TODO: Implement
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            //TODO: Implement
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
            //TODO: Implement
        }

    };

    static_assert(
        4 * sizeof(std::uint64_t) == sizeof(vec4x64u),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint64_t extract(vec4x64u v) {
        static_assert(N < 4, "Specified index does not exist");
        typename std::enable_if<N < 4, int>::type dummy_variable = 0;

        //TODO: Implement
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x64u insert(vec4x64u v, std::uint64_t x) {
        static_assert(N < 4, "Specified index does not exist");
        typename std::enable_if<N < 4, int>::type dummy_variable = 0;

        //TODO: Implement
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_left(vec4x64u v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_left<0>(vec4x64u v) {
        return v;
    }
//$BEGIN<HAS_LEFT_SHIFT_SPECIALIZATIONS>$
//    $<BIT_SHIFT_LEFT_SPECIALIZATIONS>
//$END
    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_left<64>(vec4x64u v) {
        (void)v;
        return vec4x64u{0x00};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_right(vec4x64u v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_right<0>(vec4x64u v) {
        return v;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u bit_shift_right<64>(vec4x64u v) {
        (void)v;
        return vec4x64u{0x00};
    }




    template<std::uint32_t S, typename std::enable_if<S < 64, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x64u rotl(vec4x64u v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL vec4x64u rotl<0>(vec4x64u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<64 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x64u rotl(vec4x64u v) {
        return rotl<S % 64>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u rotl(vec4x64u v, long long s) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u rotl(vec4x64u v, vec4x64u s) {
        //TODO: Implement
    }



    template<std::uint32_t S, typename std::enable_if<S < 64, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x64u rotr(vec4x64u v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL vec4x64u rotr<0>(vec4x64u v) {
        return v;
    }

    template<std::uint32_t S, typename std::enable_if<64 <= S, bool>::type = true>
    [[nodiscard]]
    AVEL_FINL vec4x64u rotr(vec4x64u v) {
        return rotr<S % 64>(v);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u rotr(vec4x64u v, long long s) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u rotr(vec4x64u v, vec4x64u s) {
        //TODO: Implement
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u broadcast_mask(mask4x64u m) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u keep(mask4x64u m, vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u clear(mask4x64u m, vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u blend(mask4x64u m, vec4x64u a, vec4x64u b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u byteswap(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u max(vec4x64u a, vec4x64u b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u min(vec4x64u a, vec4x64u b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x64u, 2> minmax(vec4x64u a, vec4x64u b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u clamp(vec4x64u x, vec4x64u lo, vec4x64u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u average(vec4x64u a, vec4x64u b) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u midpoint(vec4x64u a, vec4x64u b) {
        //TODO: Implement
    }

    //Definition of neg_abs delayed until later

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u load<vec4x64u>(const std::uint64_t* ptr, std::uint32_t n) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u load<vec4x64u, vec4x64u::width>(const std::uint64_t* ptr) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u aligned_load<vec4x64u>(const std::uint64_t* ptr, std::uint32_t n) {
        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64u aligned_load<vec4x64u, vec4x64u::width>(const std::uint64_t* ptr) {
        //TODO: Implement
    }



    AVEL_FINL void store(std::uint64_t* ptr, vec4x64u v, std::uint32_t n) {
        //TODO: Implement
    }

    template<std::uint32_t N = vec4x64u::width>
    AVEL_FINL void store(std::uint64_t* ptr, vec4x64u v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void store<vec4x64u::width>(std::uint64_t* ptr, vec4x64u v) {
        //TODO: Implement
    }



    AVEL_FINL void aligned_store(std::uint64_t* ptr, vec4x64u v, std::uint32_t n) {
        //TODO: Implement
    }

    template<std::uint32_t N = vec4x64u::width>
    AVEL_FINL void aligned_store(std::uint64_t* ptr, vec4x64u v) {
        //TODO: Implement
    }

    template<>
    AVEL_FINL void aligned_store<vec4x64u::width>(std::uint64_t* ptr, vec4x64u v) {
        //TODO: Implement
    }



    [[nodiscard]]
    AVEL_FINL arr4x64u to_array(vec4x64u x) {
        alignas(32) arr4x64u ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec4x64u> div(vec4x64u x, vec4x64u y) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u popcount(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u countl_zero(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u countl_one(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u countr_zero(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u countr_one(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u bit_width(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u bit_floor(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64u bit_ceil(vec4x64u v) {
        //TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL mask4x64u has_single_bit(vec4x64u v) {
        //TODO: Implement
    }

}

#endif //AVEL_VEC4X64U_HPP
