#ifndef AVEL_SCALAR32I_HPP
#define AVEL_SCALAR32I_HPP

namespace avel {

    template<>
    [[nodiscard]]
    AVEL_FINL std::int32_t set_bits<std::int32_t>(bool x) {
        return -std::int32_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t popcount(std::int32_t v) {
        return std::int32_t(popcount(std::uint32_t(v)));
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t byteswap(std::int32_t v) {
        return std::int32_t(byteswap(std::uint32_t(v)));
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t countl_zero(std::int32_t v) {
        return std::int32_t(countl_zero(std::uint32_t(v)));
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t countl_one(std::int32_t v) {
        return std::int32_t(countl_one(std::uint32_t(v)));
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t countr_zero(std::int32_t v) {
        return std::int32_t(countr_zero(std::uint32_t(v)));
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t countr_one(std::int32_t v) {
        return std::int32_t(countr_one(std::uint32_t(v)));
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t bit_width(std::int32_t x) {
        return bit_width(std::uint32_t(x));
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::int32_t v) {
        return has_single_bit(std::uint32_t(v));
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t rotl(std::int32_t x, long long s) {
        return std::int32_t(rotl(std::uint32_t(x), s));
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t rotr(std::int32_t x, long long s) {
        return std::int32_t(rotr(std::uint32_t(x), s));
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int32_t negate(bool m, std::int32_t x) {
        std::int32_t mask = -m;
        return (x ^ mask) - mask;
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t abs(std::int32_t x) {
        if (x < 0) {
            return -x;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t neg_abs(std::int32_t x) {
        if (x < 0) {
            return x;
        } else {
            return -x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t keep(bool m, std::int32_t x) {
        if (m) {
            return x;
        } else {
            return 0;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t clear(bool m, std::int32_t x) {
        if (m) {
            return 0;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t blend(bool m, std::int32_t a, std::int32_t b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t min(std::int32_t a, std::int32_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t max(std::int32_t a, std::int32_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<std::int32_t, 2> minmax(std::int32_t a, std::int32_t b) {
        if (a < b) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t clamp(std::int32_t x, std::int32_t lo, std::int32_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t midpoint(std::int32_t a, std::int32_t b) {
        std::int64_t average = (static_cast<std::int64_t>(a) + static_cast<std::int64_t>(b)) >> 1;
        std::int64_t bias = (b < a) & (a ^ b) & 0x1;
        return static_cast<std::int32_t>(average + bias);
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t average(std::int32_t a, std::int32_t b) {
        return (std::int64_t(a) + std::int64_t(b)) / 2;
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL bool cmp_equal(std::uint32_t x, std::int32_t y) {
        return std::int64_t(x) == std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_equal(std::int32_t x, std::uint32_t y) {
        return std::int64_t(x) == std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_not_equal(std::uint32_t x, std::int32_t y) {
        return std::int64_t(x) != std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_not_equal(std::int32_t x, std::uint32_t y) {
        return std::int64_t(x) != std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_less(std::uint32_t x, std::int32_t y) {
        return std::int64_t(x) < std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_less(std::int32_t x, std::uint32_t y) {
        return std::int64_t(x) < std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_less_equal(std::uint32_t x, std::int32_t y) {
        return std::int64_t(x) <= std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_less_equal(std::int32_t x, std::uint32_t y) {
        return std::int64_t(x) <= std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_greater(std::uint32_t x, std::int32_t y) {
        return std::int64_t(x) > std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_greater(std::int32_t x, std::uint32_t y) {
        return std::int64_t(x) > std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_greater_equal(std::uint32_t x, std::int32_t y) {
        return std::int64_t(x) >= std::int64_t(y);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_greater_equal(std::int32_t x, std::uint32_t y) {
        return std::int64_t(x) >= std::int64_t(y);
    }

}

#endif //AVEL_SCALAR32I_HPP
