#ifndef AVEL_SCALAR64F_HPP
#define AVEL_SCALAR64F_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL double trunc(double x);

    [[nodiscard]]
    AVEL_FINL double round(double x);

    [[nodiscard]]
    AVEL_FINL double copysign(double mag, double sgn);

    [[nodiscard]]
    AVEL_FINL bool isnan(double v);

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL double keep(bool m, double v) {
        if (m) {
            return v;
        } else {
            return 0.0f;
        }
    }

    [[nodiscard]]
    AVEL_FINL double clear(bool m, double v) {
        if (m) {
            return 0.0f;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL double blend(bool m, double a, double b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL double byteswap(double x) {
        return bit_cast<double>(byteswap(bit_cast<std::uint64_t>(x)));
    }

    [[nodiscard]]
    AVEL_FINL double max(double x, double y) {
        if (x < y) {
            return y;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL double min(double x, double y) {
        if (x < y) {
            return x;
        } else {
            return y;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<double, 2> minmax(double x, double y) {
        if (x < y) {
            return {x, y};
        } else {
            return {y, x};
        }
    }

    [[nodiscard]]
    AVEL_FINL double clamp(double x, double lo, double hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL double negate(bool m, double x) {
        if (m) {
            return -x;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL double abs(double x) {
        return bit_cast<double>(bit_cast<std::int64_t>(x) & 0x7fffffffffffffffull);
    }

    [[nodiscard]]
    AVEL_FINL double neg_abs(double x) {
        return bit_cast<double>(bit_cast<std::int64_t>(x) | 0x8000000000000000ull);
    }

    //=====================================================
    // Cmath functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL double fmax(double a, double b) {
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
    AVEL_FINL double fmin(double a, double b) {
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
    AVEL_FINL double sqrt(double v) {
        return std::sqrt(v);
    }

    //=====================================================
    // Nearest Integer Functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL double ceil(double arg) {
        return std::ceil(arg);
    }

    [[nodiscard]]
    AVEL_FINL double floor(double arg) {
        return std::floor(arg);
    }

    [[nodiscard]]
    AVEL_FINL double trunc(double arg) {
        return std::trunc(arg);
    }

    [[nodiscard]]
    AVEL_FINL double round(double arg) {
        return std::round(arg);
    }

    [[nodiscard]]
    AVEL_FINL double nearbyint(double arg) {
        return std::nearbyint(arg);
    }

    [[nodiscard]]
    AVEL_FINL double rint(double arg) {
        return std::rint(arg);
    }

    //=====================================================
    // floating-point manipulation functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL double frexp(double arg, std::int64_t* exp) {
        std::int32_t e;
        auto ret = std::frexp(arg, &e);
        *exp = e;
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL double ldexp(double arg, std::int64_t exp) {
        exp = avel::clamp(exp, std::int64_t(INT_MIN), std::int64_t(INT_MAX));
        return std::ldexp(arg, exp);
    }

    [[nodiscard]]
    AVEL_FINL double scalbn(double x, std::int64_t exp) {
        return avel::ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t ilogb(double arg) {
        return static_cast<std::int64_t>(std::ilogb(arg));
    }

    [[nodiscard]]
    AVEL_FINL double logb(double arg) {
        return std::logb(arg);
    }

    [[nodiscard]]
    AVEL_FINL double copysign(double mag, double sgn) {
        return std::copysign(mag, sgn);
    }

    //=====================================================
    // doubleing-point classification
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int64_t fpclassify(double arg) {
        return std::fpclassify(arg);

        std::int64_t bits = bit_cast<std::int64_t>(arg);

        if (0x00000000 == bits || 0x8000000000000000ull == bits) {
            return FP_ZERO;
        }

        std::int64_t exp = bits >> 52 & 0x7FF;
        if (exp == 0x00) {
            return FP_SUBNORMAL;
        }

        if (exp == 0xFF) {
            std::int64_t mantissa = 0x0000ffffffffffffful;
            if (bits & mantissa) {
                return FP_NAN;
            } else {
                return FP_INFINITE;
            }
        }

        return FP_NORMAL;
    }

    [[nodiscard]]
    AVEL_FINL bool isfinite(double arg) {
        std::int64_t tmp = bit_cast<std::int64_t>(arg);
        std::int64_t exp = tmp >> 52 & 0x7FF;
        return (exp != 2047);
    }

    [[nodiscard]]
    AVEL_FINL bool isinf(double arg) {
        std::int64_t tmp = bit_cast<std::int64_t>(arg);
        std::int64_t exp = tmp >> 52 & 0x7FF;
        return (exp == 2047) && !(tmp & 0x0000ffffffffffffful);
    }

    [[nodiscard]]
    AVEL_FINL bool isnan(double arg) {
        std::int64_t tmp = bit_cast<std::int64_t>(arg);
        std::int64_t exp = tmp >> 52 & 0x7FF;
        return (exp == 2047) && (tmp & 0x0000ffffffffffffful);
    }


    [[nodiscard]]
    AVEL_FINL bool isnormal(double arg) {
        std::int64_t tmp = bit_cast<std::int64_t>(arg);
        std::int64_t exp = tmp >> 52 & 0x7FF;

        return (exp != 0x00) && (exp != 2047);
    }

    [[nodiscard]]
    AVEL_FINL bool signbit(double arg) {
        return bit_cast<std::int64_t>(arg) & 0x8000000000000000ull;
    }

    //=====================================================
    // doubleing-point comparisons
    //=====================================================

    [[nodiscard]]
    AVEL_FINL bool isgreater(double x, double y) {
        return std::isgreater(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isgreaterequal(double x, double y) {
        return std::isgreaterequal(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isless(double x, double y) {
        return std::isless(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool islessequal(double x, double y) {
        return std::islessequal(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool islessgreater(double x, double y) {
        return std::islessgreater(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isunordered(double x, double y) {
        return std::isunordered(x, y);
    }

}

#endif //AVEL_SCALAR64F_HPP
