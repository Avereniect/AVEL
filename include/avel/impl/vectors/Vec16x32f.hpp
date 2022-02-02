#include "avel/Vector.hpp"
#include "avel/impl/vector_primitives/Vector_primitives.hpp"

namespace avel {

    using vec16x32f = Vector<float, 16>;

    AVEL_FINL vec16x32f trunc(vec16x32f x);

    template<>
    class Vector_mask<float, 16> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<float, 16>::type;

        //=================================================
        // Constructor
        //=================================================

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


    template<>
    class Vector<float, 16> {
    public:

        using scalar_type = float;

        using primitive = avel::vector_primitive<float, 16>::type;

        constexpr static unsigned width = 16;

        using mask = Vector_mask<float, 16>;

        template<class U>
        using rebind_type = Vector<U, 16>;

        template<int M>
        using rebind_width = Vector<float, M>;

        //=================================================
        // Constructors
        //=================================================

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

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar_type x):
            content(_mm512_set1_ps(x)) {}

        AVEL_FINL explicit Vector(const scalar_type* x):
            content(_mm512_loadu_ps(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& a):
            content(_mm512_loadu_ps(a.data())) {}

        AVEL_FINL explicit Vector(Vector<std::int32_t, width> v):
            content(_mm512_cvtepi32_ps(v)) {}

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

        AVEL_FINL Vector& operator=(scalar_type x) {
            content = _mm512_set1_ps(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_EQ_OQ)};
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_NEQ_OQ)};
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LT_OQ)};
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_LE_OQ)};
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GT_OQ)};
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            return mask{_mm512_cmp_ps_mask(content, vec.content, _CMP_GE_OQ)};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() {
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

        AVEL_FINL Vector operator&=(const Vector vec) {
            content = _mm512_and_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector operator|=(const Vector vec) {
            content = _mm512_or_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector operator^=(const Vector vec) {
            content = _mm512_xor_ps(content, vec.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() {
            return Vector{_mm512_andnot_ps(ones().content, content)};
        }

        AVEL_FINL Vector operator&(const Vector vec) {
            return Vector{_mm512_and_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator|(const Vector vec) {
            return Vector{_mm512_or_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator^(const Vector vec) {
            return Vector{_mm512_xor_ps(content, vec.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar_type, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar_type, width> array{};

            _mm512_store_ps(array.data(), content);

            return array;
        }

        AVEL_FINL operator primitive() const {
            return content;
        }

        AVEL_FINL explicit operator mask() const {
            return *this == zeros();
        }

        AVEL_FINL explicit operator Vector<std::int32_t, width>() const {
            return Vector<std::int32_t, width>{_mm512_cvtps_epi32(content)};
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec16x32f::mask isnan(vec16x32f v);

    //=====================================================
    // General vector operations
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

    AVEL_FINL vec16x32f abs(vec16x32f v) {
        return vec16x32f{_mm512_abs_ps(v)};
    }

    AVEL_FINL vec16x32f load(const float* ptr) {
        return vec16x32f{_mm512_loadu_ps(ptr)};
    }

    AVEL_FINL vec16x32f aligned_load(const float* ptr) {
        return vec16x32f{_mm512_load_ps(ptr)};
    }

    AVEL_FINL vec16x32f stream_load(const float* ptr) {
        return vec16x32f{_mm512_castsi512_ps(_mm512_stream_load_si512((void*) ptr))};
    }

    AVEL_FINL vec16x32f gather(const float* ptr, vec16x32i o) {
        return vec16x32f{_mm512_i32gather_ps(o, ptr, sizeof(float))};
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
        auto half = vec16x32f{0.5f};
        return (a - a * half) + (b * half);
    }

    AVEL_FINL vec16x32f epsilon_increment(vec16x32f v) {
        const vec16x32u low_bit{0x01};
        const vec16x32u exponent_mask{0x7f800000};

        auto offset = vec16x32u{_mm512_castps_si512(v)} & exponent_mask | low_bit;

        return vec16x32f{_mm512_add_ps(v, _mm512_castsi512_ps(offset))};
    }

    AVEL_FINL vec16x32f epsilon_decrement(vec16x32f v) {
        const vec16x32u low_bit{0x01};
        const vec16x32u exponent_mask{0x7f800000};

        auto offset = vec16x32u{_mm512_castps_si512(v)} & exponent_mask | low_bit;

        return vec16x32f{_mm512_sub_ps(v, _mm512_castsi512_ps(offset))};
    }

    AVEL_FINL vec16x32f epsilon_offset(vec16x32f, vec16x32i);

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
    }

    AVEL_FINL vec16x32f exp2(vec16x32f x) {
        return exp(x * vec16x32f{69314718056.0f});
    }

    AVEL_FINL vec16x32f expm1(vec16x32f x);

    AVEL_FINL vec16x32f log(vec16x32f x);

    AVEL_FINL vec16x32f log10(vec16x32f x);

    AVEL_FINL vec16x32f log2(vec16x32f x) {
        __m512 approximation = _mm512_setzero_ps();

        //TODO: Complete implementation

        return vec16x32f{_mm512_add_ps(_mm512_getexp_ps(x), approximation)};
    }

    AVEL_FINL vec16x32f log1p(vec16x32f x);

    //=====================================================
    // Power functions
    //=====================================================

    AVEL_FINL vec16x32f pow(vec16x32f x);

    AVEL_FINL vec16x32f sqrt(vec16x32f x) {
        return vec16x32f{_mm512_sqrt_ps(x)};
    }

    AVEL_FINL vec16x32f cbrt(vec16x32f x);

    AVEL_FINL vec16x32f hypot(vec16x32f x);

    //=====================================================
    // Trigonometric functions
    //=====================================================

    AVEL_FINL std::array<vec16x32f, 2> sincos(vec16x32f angle) {
        alignas(32) static const float constants0[8] {
            reinterpret_bits<float>(std::uint32_t(0x7FFFFFFF)),
            0.5f,
            6.283185307179586f,
            1.0f / (6.283185307179586f),
            0.78539816339744830f,
            1.57079632679489661f,
            1.0f,
            0.0f
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

        // Compute offset that's added to angle to bring it into [-pi/4, pi/4]
        // range where error of Taylor series is minimized
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
    }

    AVEL_FINL vec16x32f sin(vec16x32f angle);

    AVEL_FINL vec16x32f cos(vec16x32f angle);

    AVEL_FINL vec16x32f tan(vec16x32f angle);

    AVEL_FINL vec16x32f asin(vec16x32f x) {
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
    }

    AVEL_FINL vec16x32f acos(vec16x32f x) {
        alignas(32) static constexpr float constants[8] {

        };

        vec16x32f offset = sqrt(vec16x32f{1.0f} - (x * x));

        x +=offset;

        vec16x32f t0 = fmadd(x, vec16x32f{constants[0]}, vec16x32f{constants[1]});
        vec16x32f t1 = fmadd(x, t0, vec16x32f{constants[2]});
        vec16x32f t2 = fmadd(x, t1, vec16x32f{constants[3]});
        vec16x32f t3 = fmadd(x, t2, vec16x32f{constants[4]});
        vec16x32f t4 = fmadd(x, t3, vec16x32f{constants[5]});
        vec16x32f t5 = fmadd(x, t4, vec16x32f{constants[6]});

        return (t5 - offset);
    }

    AVEL_FINL vec16x32f atan(vec16x32f v);

    AVEL_FINL vec16x32f atan2(vec16x32f x, vec16x32f y);

    //=====================================================
    // Nearest Integer Floating-point operators
    //=====================================================

    AVEL_FINL vec16x32f ceil(vec16x32f x) {
        return vec16x32f{_mm512_roundscale_ps(x, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec16x32f floor(vec16x32f x) {
        return vec16x32f{_mm512_roundscale_ps(x, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec16x32f trunc(vec16x32f x) {
        return vec16x32f{_mm512_roundscale_ps(x, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
    }

    AVEL_FINL vec16x32f round(vec16x32f x) {
        //TODO: Verify behavior matches with std::round for values ending in .5
        return vec16x32f{_mm512_roundscale_ps(x, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    AVEL_FINL vec16x32f frexp(vec16x32f v, vec16x32i* exp);

    AVEL_FINL vec16x32f ldexp(vec16x32f x, vec16x32f exp);

    AVEL_FINL vec16x32f modf(vec16x32f x, vec16x32f* iptr);

    AVEL_FINL vec16x32f scalebn(vec16x32f x, vec16x32f exp);

    AVEL_FINL vec16x32f ilog(vec16x32f x);

    AVEL_FINL vec16x32f logb(vec16x32f x);

    AVEL_FINL vec16x32f copysign(vec16x32f mag, vec16x32f sign) {
        alignas(16) std::uint32_t mask_data[1] = {
            0x80000000
        };

        auto mask = vec16x32f{reinterpret_bits<float>(mask_data[0])};

        //TODO: AVX512F implementation
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
