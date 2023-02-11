#ifndef AVEL_SCALAR8I_HPP
#define AVEL_SCALAR8I_HPP

namespace avel {

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::int8_t broadcast_mask<std::int8_t>(bool x) {
        return -std::int8_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t popcount(std::int8_t x) {
        return popcount(std::uint8_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t byteswap(std::int8_t x) {
        return x;
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t countl_zero(std::int8_t x) {
        return countl_zero(std::uint8_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t countl_one(std::int8_t x) {
        return countl_one(std::uint8_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t countr_zero(std::int8_t x) {
        return countr_zero(std::uint8_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t countr_one(std::int8_t x) {
        return countr_one(std::uint8_t(x));
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::int8_t x) {
        return has_single_bit(std::uint8_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t rotl(std::int8_t x, long long s) {
        return rotl(std::uint8_t(x), s);
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t rotr(std::int8_t x, long long s) {
        return rotr(std::uint8_t(x), s);
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int8_t negate(bool m, std::int8_t x) {
        std::int8_t mask = -m;
        return (x ^ mask) - mask;
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t abs(std::int8_t x) {
        if (x < 0) {
            return -x;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t neg_abs(std::int8_t x) {
        if (x < 0) {
            return x;
        } else {
            return -x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t blend(bool m, std::int8_t a, std::int8_t b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t min(std::int8_t a, std::int8_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t max(std::int8_t a, std::int8_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<std::int8_t, 2> minmax(std::int8_t a, std::int8_t b) {
        if (a < b) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t clamp(std::int8_t x, std::int8_t lo, std::int8_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t midpoint(std::int8_t a, std::int8_t b) {
        std::int32_t average = (static_cast<std::int32_t>(a) + static_cast<std::int32_t>(b)) >> 1;
        std::int32_t bias = (b < a) & (a ^ b) & 0x1;
        return static_cast<std::int8_t>(average + bias);
    }

    [[nodiscard]]
    AVEL_FINL std::int8_t average(std::int8_t a, std::int8_t b) {
        return (std::int16_t(a) + std::int16_t(b)) / 2;
    }

}

#endif //AVEL_SCALAR8I_HPP
