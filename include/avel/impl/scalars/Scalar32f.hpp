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
    // Cmath functions
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
        return static_cast<float>(ilogb(arg));
    }

    [[nodiscard]]
    AVEL_FINL float copysign(float mag, float sgn) {
        return std::copysign(mag, sgn);

        /*
        auto mag_bits = bit_cast<std::uint32_t>(mag);
        auto sgn_bits = bit_cast<std::uint32_t>(sgn);

        mag_bits ^= (sgn_bits ^ mag_bits) & 0x80000000;
        return bit_cast<float>(mag_bits);
        */
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
        std::int32_t tmp = bit_cast<std::int32_t>(arg);
        std::int32_t exp = tmp >> 23 & 0xFF;
        return (exp == 255) && (tmp & 0x007fffff);
    }

    [[nodiscard]]
    AVEL_FINL bool isnormal(float arg) {
        std::int32_t tmp = bit_cast<std::int32_t>(arg);
        std::int32_t exp = tmp >> 23 & 0xFF;

        return (exp != 0x00) && (exp != 255);
    }

    [[nodiscard]]
    AVEL_FINL bool signbit(float arg) {
        return bit_cast<std::int32_t>(arg) & 0x80000000;
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
