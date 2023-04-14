#ifndef AVEL_VEC4X64I_HPP
#define AVEL_VEC4X64I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x64i = Vector<std::int64_t, 4>;
    using arr4x64i = std::array<std::int64_t, 4>;
    using mask4x64i = Vector_mask<std::int64_t, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec4x64i> div(vec4x64i numerator, vec4x64i denominator);
    vec4x64i broadcast_mask(mask4x64i m);
    vec4x64i blend(mask4x64i m, vec4x64i a, vec4x64i b);
    vec4x64i negate(vec4x64i m, vec4x64i x);





    template<>
    class Vector_mask<std::int64_t, 4> {
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
                "Implementation assumes bool occupy a single byte"
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
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x64u, 1> convert<mask4x64u, mask4x64i>(mask4x64i m) {
        return {mask4x64u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask4x64i, 1> convert<mask4x64i, mask4x64u>(mask4x64u m) {
        return {mask4x64i(decay(m))};
    }

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask4x64i m) {
        return count(mask4x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x64i m) {
        return any(mask4x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x64i m) {
        return all(mask4x64u{m});
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x64i m) {
        return none(mask4x64u{m});
    }





    template<>
    class Vector<std::int64_t, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int64_t;

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

        AVEL_FINL Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            content(_mm256_set1_epi64x(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr4x64i& arr):
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
            // $<vector::operator==(vec, vec)>
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            // $<vector::operator!=(vec, vec)>
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            // $<vector::operator<(vec, vec)>
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            // $<vector::operator<=(vec, vec)>
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            // $<vector::operator>(vec, vec)>
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            // $<vector::operator>=(vec, vec)>
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
            // $<vector::operator+=(vec)>
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            // $<vector::operator-=(vec)>
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            // $<vector::operator*=(vec)>
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
            // $<vector::operator&=(vec)>
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            // $<vector::operator|=(vec)>
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            // $<vector::operator^=(vec)>
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            // $<vector::operator<<=(long long)>
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            // $<vector::operator>>=(long long)>
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            // $<vector::operator<<=(vec)>
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            // $<vector::operator>>=(vec)>
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            // $<vector::operator~()>
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
            // $<vector::operator mask()>
        }

    };

    static_assert(
        4 * sizeof(std::int64_t) == sizeof(vec4x64i),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Vector conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x64u, 1> convert<vec4x64u, vec4x64i>(vec4x64i x) {
        return {vec4x64u{decay(x)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec4x64i, 1> convert<vec4x64i, vec4x64u>(vec4x64u x) {
        return {vec4x64i{decay(x)}};
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec4x64i operator-(vec4x64u v) {
        // $<operator-(uvec)>
    }

    //=====================================================
    // Arrangement Instructions
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int64_t extract(vec4x64i v) {
        static_assert(N < 4, "Specified index does not exist");
        typename std::enable_if<N < 4, int>::type dummy_variable = 0;

        // $<extract(vec)>
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x64i insert(vec4x64i v, std::int64_t x) {
        static_assert(N < 4, "Specified index does not exist");
        typename std::enable_if<N < 4, int>::type dummy_variable = 0;

        // $<insert(vec, scalar)>
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL  vec4x64i bit_shift_left(vec4x64i v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        return vec4x64i{bit_shift_left<S>(vec4x64u{v})};
    }

    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL  vec4x64i bit_shift_right(vec4x64i v) {
        static_assert(S <= 64, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 64, int>::type dummy_variable = 0;

        return vec4x64i{bit_shift_right<S>(vec4x64u{v})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x64i rotl(vec4x64i v) {
        return vec4x64i{rotl<S>(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i rotl(vec4x64i v, long long s) {
        return vec4x64i{rotl(vec4x64u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i rotl(vec4x64i v, vec4x64i s) {
        return vec4x64i{rotl(vec4x64u{v}, vec4x64u{s})};
    }



    template<std::uint32_t S>
    [[nodiscard]]
    AVEL_FINL vec4x64i rotr(vec4x64i v) {
        return vec4x64i{rotr<S>(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i rotr(vec4x64i v, long long s) {
        return vec4x64i{rotr(vec4x64u{v}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i rotr(vec4x64i v, vec4x64i s) {
        return vec4x64i{rotr(vec4x64u{v}, vec4x64u{s})};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x64i broadcast_mask(mask4x64i m) {
        return vec4x64i{broadcast_mask(mask4x64u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i blend(mask4x64i m, vec4x64i a, vec4x64i b) {
        return vec4x64i{blend(mask4x64u{m}, vec4x64u{a}, vec4x64u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i byteswap(vec4x64i v) {
        return vec4x64i{byteswap(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i max(vec4x64i a, vec4x64i b) {
        // $<max(vec, vec)>
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i min(vec4x64i a, vec4x64i b) {
        // $<min(vec, vec)>
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x64i, 2> minmax(vec4x64i a, vec4x64i b) {
        // $<minmax(vec, vec)>
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i clamp(vec4x64i x, vec4x64i lo, vec4x64i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i average(vec4x64i a, vec4x64i b) {
        // $<average(vec, vec)>
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i midpoint(vec4x64i a, vec4x64i b) {
        // $<midpoint(vec, vec)>
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i negate(mask4x64i m, vec4x64i v) {
        // $<negate(mask, vec)>
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i abs(vec4x64i v) {
        // $<abs(vec)>
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i neg_abs(vec4x64i v) {
        // $<neg_abs(vec)>
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i neg_abs(vec4x64u v) {
        // $<neg_abs(uvec)>
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64i load<vec4x64i>(const std::int64_t* ptr, std::uint32_t n) {
        // $<load(const scalar*, std::uint32_t)>
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64i load<vec4x64i, vec4x64i::width>(const std::int64_t* ptr) {
        // $<load<width>(const scalar*)>
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64i aligned_load<vec4x64i>(const std::int64_t* ptr, std::uint32_t n) {
        // $<aligned_load(const scalar*, std::uint32_t)>
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x64i aligned_load<vec4x64i, vec4x64i::width>(const std::int64_t* ptr) {
        // $<aligned_load<width>(const scalar*)>
    }


    template<std::uint32_t N = vec4x64i::width>
    AVEL_FINL vec4x64i gather(std::int64_t* ptr, vec4x64i indices) {
        // $<gather(scalar*, vec)>
    }

    template<>
    AVEL_FINL vec4x64i gather<0>(std::int64_t* ptr, vec4x64i indices) {
        return vec4x64i{0x00};
    }

    template<>
    AVEL_FINL vec4x64i gather<vec4x64i::width>(std::int64_t* ptr, vec4x64i indices) {
        // $<gather<width>(scalar*, vec)>
    }

    AVEL_FINL vec4x64i gather(std::int64_t* ptr, vec4x64i indices, std::uint32_t n) {
        // $<gather(scalar*, vec, std::uint32_t n)>
    }



    template<std::uint32_t N = vec4x64u::width>
    AVEL_FINL vec4x64u gather(std::int64_t* ptr, vec4x64i indices) {
        // $<gather(uscalar*, uvec)>
    }

    template<>
    AVEL_FINL vec4x64u gather<0>(std::int64_t* ptr, vec4x64i indices) {
        return vec4x64u{0x00};
    }

    template<>
    AVEL_FINL vec4x64u gather<vec4x64u::width>(std::int64_t* ptr, vec4x64i indices) {
        // $<gather<width>(uscalar*, uvec)>
    }

    AVEL_FINL vec4x64u gather(std::uint64_t* ptr, vec4x64i indices, std::uint32_t n) {
        // $<gather(uscalar*, uvec, std::uint32_t n)>
    }



    AVEL_FINL void store(std::int64_t* ptr, vec4x64i v, std::uint32_t n) {
        // $<store(scalar*, vec, std::uint32_t)>
    }

    template<std::uint32_t N = vec4x64i::width>
    AVEL_FINL void store(std::int64_t* ptr, vec4x64i v) {
        // $<store<std::uint32_t>(scalar*, vec)>
    }

    template<>
    AVEL_FINL void store<vec4x64i::width>(std::int64_t* ptr, vec4x64i v) {
        // $<store<VECTOR_WIDTH>(scalar*, vec)>
    }



    AVEL_FINL void aligned_store(std::int64_t* ptr, vec4x64i v, std::uint32_t n) {
        // $<aligned_store(scalar*, vec, std::uint32_t)>
    }

    template<std::uint32_t N = vec4x64i::width>
    AVEL_FINL void aligned_store(std::int64_t* ptr, vec4x64i v) {
        // $<aligned_store<std::uint32_t>(scalar*, vec)>
    }

    template<>
    AVEL_FINL void aligned_store<vec4x64i::width>(std::int64_t* ptr, vec4x64i v) {
        // $<aligned_store<VECTOR_WIDTH>(scalar*, vec)>
    }


    
    AVEL_FINL void scatter(const std::int64_t* ptr, vec4x64i v, vec4x64i indices, std::uint32_t n) {
        // $<scatter(const scalar*, vec, vec, std::uint32_t)>
    }

    template<std::uint32_t N = vec4x64i::width>
    AVEL_FINL void scatter(const std::int64_t* ptr, vec4x64i v, vec4x64i indices) {
        // $<scatter(const scalar*, vec, vec)>
    }

    template<>
    AVEL_FINL void scatter<0>(const std::int64_t* ptr, vec4x64i v, vec4x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec4x64i::width>(const std::int64_t* ptr, vec4x64i v, vec4x64i indices) {
        // $<scatter<width>(const scalar*, vec, vec)>
    }



    AVEL_FINL void scatter(const std::uint64_t* ptr, vec4x64u v, vec4x64i indices, std::uint32_t n) {
        // $<scatter(const uscalar*, uvec, vec, std::uint32_t)>
    }

    template<std::uint32_t N = vec4x64i::width>
    AVEL_FINL void scatter(const std::uint64_t* ptr, vec4x64u v, vec4x64i indices) {
        // $<scatter(const uscalar*, uvec, vec)>
    }

    template<>
    AVEL_FINL void scatter<0>(const std::uint64_t* ptr, vec4x64u v, vec4x64i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec4x64i::width>(const std::uint64_t* ptr, vec4x64u v, vec4x64i indices) {
        //$<scatter<width>(const uscalar*, uvec, vec)>
    }




    [[nodiscard]]
    AVEL_FINL arr4x64i to_array(vec4x64i v) {
        alignas(32) arr4x64i ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec4x64i> div(vec4x64i x, vec4x64i y) {
        // TODO: Implement
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i popcount(vec4x64i v) {
        return vec4x64i{popcount(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i countl_zero(vec4x64i v) {
        return vec4x64i{countl_zero(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i countl_one(vec4x64i v) {
        return vec4x64i{countl_one(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i countr_zero(vec4x64i v) {
        return vec4x64i{countr_zero(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL vec4x64i countr_one(vec4x64i v) {
        return vec4x64i{countr_one(vec4x64u{v})};
    }

    [[nodiscard]]
    AVEL_FINL mask4x64i has_single_bit(vec4x64i v) {
        return mask4x64i{has_single_bit(vec4x64u{v})};
    }

}

#endif //AVEL_VEC4X64I_HPP
