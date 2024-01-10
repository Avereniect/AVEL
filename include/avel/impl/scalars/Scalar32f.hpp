#ifndef AVEL_SCALAR32f_HPP
#define AVEL_SCALAR32f_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float trunc(float x);

    [[nodiscard]]
    AVEL_FINL float round(float x);

    [[nodiscard]]
    AVEL_FINL float copysign(float mag, float sgn);

    [[nodiscard]]
    AVEL_FINL bool isnan(float v);

    [[nodiscard]]
    AVEL_FINL bool isinf(float arg);

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float keep(bool m, float v) {
        if (m) {
            return v;
        } else {
            return 0.0f;
        }
    }

    [[nodiscard]]
    AVEL_FINL float clear(bool m, float v) {
        if (m) {
            return 0.0f;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL float blend(bool m, float a, float b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL float byteswap(float v) {
        return bit_cast<float>(byteswap(bit_cast<std::uint32_t>(v)));
    }

    [[nodiscard]]
    AVEL_FINL float max(float x, float y) {
        if (x < y) {
            return y;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL float min(float x, float y) {
        if (x < y) {
            return x;
        } else {
            return y;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<float, 2> minmax(float x, float y) {
        if (x < y) {
            return {x, y};
        } else {
            return {y, x};
        }
    }

    [[nodiscard]]
    AVEL_FINL float clamp(float x, float lo, float hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL float negate(bool m, float x) {
        if (m) {
            return -x;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL float abs(float x) {
        return bit_cast<float>(bit_cast<std::int32_t>(x) & 0x7fffffff);
    }

    [[nodiscard]]
    AVEL_FINL float neg_abs(float x) {
        return bit_cast<float>(bit_cast<std::int32_t>(x) | 0x80000000);
    }

    //=====================================================
    // General Floating-Point Operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float fmax(float a, float b) {
        if (avel::isnan(a)) {
            return b;
        }

        if (avel::isnan(b)) {
            return a;
        }

        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL float fmin(float a, float b) {
        if (avel::isnan(a)) {
            return b;
        }

        if (avel::isnan(b)) {
            return a;
        }

        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    /*
    [[nodiscard]]
    AVEL_FINL float fmod(float x, float y) {
        #if defined(AVEL_AVX512DQ)
        const __m128 abs_mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));

        __m128 x_reg = _mm_set_ss(x);
        __m128 y_reg = _mm_set_ss(y);

        if (_mm_fpclass_ss_mask(x_reg, 0x99) | _mm_fpclass_ss_mask(y_reg, 0x87)) {
            return NAN;
        }

        __m128 abs_x_reg = _mm_and_ps(x_reg, abs_mask);
        __m128 abs_y_reg = _mm_and_ps(y_reg, abs_mask);

        if (_mm_fpclass_ss_mask(y_reg, 0x18) | (_mm_cmp_ss_mask(abs_x_reg, abs_y_reg, _CMP_LT_OS))) {
            return x;
        }

        __m128d xd_reg = _mm_cvtss_sd(_mm_undefined_pd(), abs_x_reg);
        __m128d yd_reg = _mm_cvtss_sd(_mm_undefined_pd(), abs_y_reg);

        __m128d recip_yd = _mm_div_pd(_mm_set_sd(1.0f), yd_reg);
        recip_yd = _mm_castsi128_pd(_mm_add_epi64(_mm_castpd_si128(recip_yd), _mm_set1_epi64x(-1)));

        // Mask used to reduce number of significant digits in subtrahend
        // to prevent an inexact operation. This leaves 29 significant bits
        // which when multiplied again yd's max of 24 significant digits,
        // produces a result with at most 52 significant digits
        const __m128d mask = _mm_castsi128_pd(_mm_set1_epi64x(0xffffffffff000000ll));

        __m128d yd2_reg = _mm_add_sd(yd_reg, yd_reg);

        // Loop runs for at most 10 iterations
        while (_mm_comige_sd(xd_reg, yd2_reg)) {
            __m128d quotient = _mm_mul_sd(xd_reg, recip_yd);
            __m128d masked_quotient = _mm_and_pd(mask, quotient);

            __m128d trunc_q = _mm_round_sd(_mm_undefined_pd(), masked_quotient, _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC);
            xd_reg = _mm_fnmadd_sd(trunc_q, yd_reg, xd_reg);
        }

        xd_reg = _mm_sub_sd(xd_reg, _mm_and_pd(_mm_cmpge_pd(xd_reg, yd_reg), yd_reg));

        auto ret_reg = _mm_cvtsd_ss(_mm_undefined_ps(), xd_reg);

        //Copy sign from x to return value
        ret_reg = _mm_or_ps(ret_reg, _mm_andnot_ps(abs_mask, _mm_set_ss(x)));

        return _mm_cvtss_f32(ret_reg);

        #elif defined(AVEL_AVX512F)
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        __m128 param_reg = _mm_set_ps(0.0f, 0.0f, y, x);
        __m128 abs_mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));
        __m128 abs_param_reg = _mm_and_ps(abs_mask, param_reg);

        __m128d widened_params = _mm_cvtps_pd(abs_param_reg);

        __m128d x_reg = widened_params;
        __m128d y_reg = _mm_unpackhi_pd(widened_params, widened_params);

        __m128d recip_yd = _mm_div_pd(_mm_set_sd(1.0f), y_reg);
        recip_yd = _mm_castsi128_pd(_mm_add_epi64(_mm_castpd_si128(recip_yd), _mm_set1_epi64x(-1)));

        // Mask used to reduce number of significant digits in subtrahend
        // to prevent an inexact operation. This leaves 29 significant bits
        // which when multiplied again yd's max of 24 significant digits,
        // produces a result with at most 52 significant digits
        const __m128d mask = _mm_castsi128_pd(_mm_set1_epi64x(0xffffffffff000000ll));

        __m128d y2_reg = _mm_add_sd(y_reg, y_reg);

        // Loop runs for at most 10 iterations
        while (_mm_comige_sd(x_reg, y2_reg)) {
            __m128d quotient = _mm_mul_sd(x_reg, recip_yd);
            __m128d masked_quotient = _mm_and_pd(mask, quotient);

            __m128d trunc_q = _mm_round_sd(_mm_undefined_pd(), masked_quotient, _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC);
            x_reg = _mm_fnmadd_sd(trunc_q, y_reg, x_reg);
        }

        x_reg = _mm_sub_sd(x_reg, _mm_and_pd(_mm_cmpge_pd(x_reg, y_reg), y_reg));

        auto ret_reg = _mm_cvtsd_ss(_mm_undefined_ps(), x_reg);

        //Copy sign from x to return value
        ret_reg = _mm_or_ps(ret_reg, _mm_andnot_ps(abs_mask, _mm_set_ss(x)));

        return _mm_cvtss_f32(ret_reg);

        #elif defined(AVEL_FMA)
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        __m128 param_reg = _mm_set_ps(0.0f, 0.0f, y, x);
        __m128 abs_mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));
        __m128 abs_param_reg = _mm_and_ps(abs_mask, param_reg);

        __m128d widened_params = _mm_cvtps_pd(abs_param_reg);

        __m128d x_reg = widened_params;
        __m128d y_reg = _mm_unpackhi_pd(widened_params, widened_params);

        __m128d recip_yd = _mm_div_pd(_mm_set_sd(1.0f), y_reg);
        recip_yd = _mm_castsi128_pd(_mm_add_epi64(_mm_castpd_si128(recip_yd), _mm_set1_epi64x(-1)));

        // Mask used to reduce number of significant digits in subtrahend
        // to prevent an inexact operation. This leaves 29 significant bits
        // which when multiplied again yd's max of 24 significant digits,
        // produces a result with at most 52 significant digits
        const __m128d mask = _mm_castsi128_pd(_mm_set1_epi64x(0xffffffffff000000ll));

        __m128d y2_reg = _mm_add_sd(y_reg, y_reg);

        // Loop runs for at most 10 iterations
        while (_mm_comige_sd(x_reg, y2_reg)) {
            __m128d quotient = _mm_mul_sd(x_reg, recip_yd);
            __m128d masked_quotient = _mm_and_pd(mask, quotient);

            __m128d trunc_q = _mm_round_sd(_mm_undefined_pd(), masked_quotient, _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC);
            x_reg = _mm_fnmadd_sd(trunc_q, y_reg, x_reg);
        }

        x_reg = _mm_sub_sd(x_reg, _mm_and_pd(_mm_cmpge_pd(x_reg, y_reg), y_reg));

        auto ret_reg = _mm_cvtsd_ss(_mm_undefined_ps(), x_reg);

        //Copy sign from x to return value
        ret_reg = _mm_or_ps(ret_reg, _mm_andnot_ps(abs_mask, _mm_set_ss(x)));

        return _mm_cvtss_f32(ret_reg);

        #elif defined(AVEL_SSE41)
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        __m128 param_reg = _mm_set_ps(0.0f, 0.0f, y, x);
        __m128 abs_mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));
        __m128 abs_param_reg = _mm_and_ps(abs_mask, param_reg);

        __m128d widened_params = _mm_cvtps_pd(abs_param_reg);

        __m128d x_reg = widened_params;
        __m128d y_reg = _mm_unpackhi_pd(widened_params, widened_params);

        __m128d recip_yd = _mm_div_pd(_mm_set_sd(1.0f), y_reg);
        recip_yd = _mm_castsi128_pd(_mm_add_epi64(_mm_castpd_si128(recip_yd), _mm_set1_epi64x(-1)));

        // Mask used to reduce number of significant digits in subtrahend
        // to prevent an inexact operation. This leaves 29 significant bits
        // which when multiplied again yd's max of 24 significant digits,
        // produces a result with at most 52 significant digits
        const __m128d mask = _mm_castsi128_pd(_mm_set1_epi64x(0xffffffffff000000ll));

        __m128d y2_reg = _mm_add_sd(y_reg, y_reg);

        // Loop runs for at most 10 iterations
        while (_mm_comige_sd(x_reg, y2_reg)) {
            __m128d quotient = _mm_mul_sd(x_reg, recip_yd);
            __m128d masked_quotient = _mm_and_pd(mask, quotient);

            __m128d trunc_q = _mm_round_sd(_mm_undefined_pd(), masked_quotient, _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC);
            __m128d subtrahend = _mm_mul_sd(trunc_q, y_reg);
            x_reg = _mm_sub_sd(x_reg, subtrahend);
        }

        x_reg = _mm_sub_sd(x_reg, _mm_and_pd(_mm_cmpge_pd(x_reg, y_reg), y_reg));

        auto ret_reg = _mm_cvtsd_ss(_mm_undefined_ps(), x_reg);

        //Copy sign from x to return value
        ret_reg = _mm_or_ps(ret_reg, _mm_andnot_ps(abs_mask, _mm_set_ss(x)));

        return _mm_cvtss_f32(ret_reg);

        #else
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        auto decrement = [] (double x) -> double {
            std::uint64_t y = avel::bit_cast<std::uint64_t>(x);
            y -= 1;

            return avel::bit_cast<double>(y);
        };

        double xd = double(avel::abs(x));
        const double yd = double(avel::abs(y));
        const double recip_yd = decrement(1.0 / yd);


        // Loop runs for at most 10 iterations
        while (xd >= (yd + yd)) {
            double quotient = xd * recip_yd;

            // Mask used to reduce number of significant digits in subtrahend
            // to prevent an inexact operation. This leaves 29 significant bits
            // which when multiplied again yd's max of 24 significant digits,
            // produces a result with at most 52 significant digits
            constexpr std::uint64_t mask = 0xffffffffff000000;

            auto quotient_bits = avel::bit_cast<std::uint64_t>(quotient);
            auto masked_quotient_bits = quotient_bits & mask;
            double masked_quotient = avel::bit_cast<double>(masked_quotient_bits);

            double trunc_q = std::trunc(masked_quotient);

            double subtrahend = trunc_q * yd;
            xd -= subtrahend;
        }

        if (xd >= yd) {
            xd -= yd;
        }

        float ret = xd;
        ret = avel::copysign(ret, x);
        return ret;
        #endif
    }
    */

    [[nodiscard]]
    AVEL_FINL float frac(float x) {
        if ((x == 0.0) || (x != x)) {
            return x;
        }

        if (avel::abs(x) == INFINITY) {
            return NAN;
        }

        return x - avel::trunc(x);
    }

    //=====================================================
    // Power Functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float sqrt(float v) {
        return std::sqrt(v);
    }

    //=====================================================
    // Nearest Integer Functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float ceil(float arg) {
        return std::ceil(arg);
    }

    [[nodiscard]]
    AVEL_FINL float floor(float arg) {
        return std::floor(arg);
    }

    [[nodiscard]]
    AVEL_FINL float trunc(float arg) {
        return std::trunc(arg);
    }

    [[nodiscard]]
    AVEL_FINL float round(float arg) {
        return std::round(arg);
    }

    [[nodiscard]]
    AVEL_FINL float nearbyint(float arg) {
        return std::nearbyint(arg);
    }

    [[nodiscard]]
    AVEL_FINL float rint(float arg) {
        return std::rint(arg);
    }

    //=====================================================
    // Floating-point manipulation functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float frexp(float arg, std::int32_t* exp) {
        return std::frexp(arg, exp);
        /*
        if (arg == 0.0) {
            *exp = 0;
            return arg;
        }

        auto bits = bit_cast<std::uint32_t>(arg);
        auto exponent_bits = (bits >> 23) & 0xFF;

        if (exponent_bits == 255) {
            return arg;
        }

        auto target_exponent = (126 << 23);
        auto ret = bits & (0x807FFFFF) | target_exponent;

        if (exponent_bits != 0) {
            *exp = exponent_bits - 126;
            return bit_cast<float>(ret);
        } else {
            auto mantissa_mask = 0x007FFFFF;
            auto significand = bits & mantissa_mask;
            auto offset = countl_zero(significand) - 8;
            ret &= 0xFF800000;
            ret |= mantissa_mask & (significand << offset);
            *exp = -125 - offset;
            return bit_cast<float>(ret);
        }
        */
    }

    [[nodiscard]]
    AVEL_FINL float ldexp(float arg, std::int32_t exp) {
        return std::ldexp(arg, exp);

        /*
        if (arg == 0.0f || exp == 0) {
            return arg;
        }

        auto bits = bit_cast<std::uint32_t>(arg);
        auto exponent = bits >> 23 & 0xFF;
        if (exponent == 255) {
            return arg;
        } else if (exponent == 0) {
            //Subnormal number
        } else {
            //Normal number
        }
        */
    }

    [[nodiscard]]
    AVEL_FINL float scalbn(float x, std::int32_t exp) {
        return ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t ilogb(float arg) {
        return std::ilogb(arg);

        /*
        if (arg == 0.0f) {
            return FP_ILOGB0;
        }

        if (std::isinf(arg)) {
            return 0x7FFFFFFF;
        }

        if (isnan(arg)) {
            return FP_ILOGBNAN;
        }

        auto bits = bit_cast<std::int32_t>(arg);
        bits >>= 23;
        bits &= 0xFF;

        if (bits > 0) {
            return bits - 127;
        } else {
            auto offset = countl_zero(bits << 9);
            return -127 - offset;
        }
        */
    }

    [[nodiscard]]
    AVEL_FINL float logb(float arg) {
        return std::logb(arg);
    }

    [[nodiscard]]
    AVEL_FINL float copysign(float mag, float sgn) {
        auto mag_bits = avel::bit_cast<std::uint32_t>(mag);
        auto sgn_bits = avel::bit_cast<std::uint32_t>(sgn);

        mag_bits ^= (sgn_bits ^ mag_bits) & 0x80000000;
        return avel::bit_cast<float>(mag_bits);
    }

    //=====================================================
    // Floating-point classification
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int32_t fpclassify(float arg) {
        std::int32_t bits = bit_cast<std::int32_t>(arg);

        if (0x00000000 == bits || 0x80000000 == bits) {
            return FP_ZERO;
        }

        std::int32_t exp = bits >> 23 & 0xFF;
        if (exp == 0x00) {
            return FP_SUBNORMAL;
        }

        if (exp == 0xFF) {
            std::int32_t mantissa = 0x007fffff;
            if (bits & mantissa) {
                return FP_NAN;
            } else {
                return FP_INFINITE;
            }
        }

        return FP_NORMAL;
    }

    [[nodiscard]]
    AVEL_FINL bool isfinite(float arg) {
        std::int32_t tmp = bit_cast<std::int32_t>(arg);
        std::int32_t exp = tmp >> 23 & 0xFF;
        return (exp != 255);
    }

    [[nodiscard]]
    AVEL_FINL bool isinf(float arg) {
        std::int32_t tmp = bit_cast<std::int32_t>(arg);
        std::int32_t exp = tmp >> 23 & 0xFF;
        return (exp == 255) && !(tmp & 0x007fffff);
    }

    [[nodiscard]]
    AVEL_FINL bool isnan(float arg) {
        return arg != arg;

        //std::int32_t tmp = bit_cast<std::int32_t>(arg);
        //std::int32_t exp = tmp >> 23 & 0xFF;
        //return (exp == 255) && (tmp & 0x007fffff);
    }

    [[nodiscard]]
    AVEL_FINL bool isnormal(float arg) {
        return std::isnormal(arg);
    }

    [[nodiscard]]
    AVEL_FINL bool signbit(float arg) {
        return std::signbit(arg);
    }

    //=====================================================
    // Floating-point comparisons
    //=====================================================

    [[nodiscard]]
    AVEL_FINL bool isgreater(float x, float y) {
        return std::isgreater(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isgreaterequal(float x, float y) {
        return std::isgreaterequal(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isless(float x, float y) {
        return std::isless(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool islessequal(float x, float y) {
        return std::islessequal(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool islessgreater(float x, float y) {
        return std::islessgreater(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isunordered(float x, float y) {
        return std::isunordered(x, y);
    }

}

#endif //AVEL_IMPL_SCALAR32f_HPP
