#ifndef AVEL_SCALAR16I_HPP
#define AVEL_SCALAR16I_HPP

namespace avel {

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::int16_t broadcast_mask<std::int16_t>(bool x) {
        return -std::int16_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t popcount(std::int16_t x) {
        return popcount(std::uint16_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t byteswap(std::int16_t x) {
        return byteswap(std::uint16_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t countl_zero(std::int16_t x) {
        return countl_zero(std::uint16_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t countl_one(std::int16_t x) {
        return countl_one(std::uint16_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t countr_zero(std::int16_t x) {
        return countr_zero(std::uint16_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t countr_one(std::int16_t x) {
        return countr_one(std::uint16_t(x));
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::int16_t x) {
        return has_single_bit(std::uint16_t(x));
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t rotl(std::int16_t x, long long s) {
        return rotl(std::uint16_t(x), s);
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t rotr(std::int16_t x, long long s) {
        return rotr(std::uint16_t(x), s);
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int16_t negate(bool m, std::int16_t x) {
        std::int16_t mask = -m;
        return (x ^ mask) - mask;
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t abs(std::int16_t x) {
        if (x < 0) {
            return -x;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t neg_abs(std::int16_t x) {
        if (x < 0) {
            return x;
        } else {
            return -x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t blend(bool m, std::int16_t a, std::int16_t b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t min(std::int16_t a, std::int16_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t max(std::int16_t a, std::int16_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<std::int16_t, 2> minmax(std::int16_t a, std::int16_t b) {
        if (a < b) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t clamp(std::int16_t x, std::int16_t lo, std::int16_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t midpoint(std::int16_t a, std::int16_t b) {
        std::int32_t average = (static_cast<std::int32_t>(a) + static_cast<std::int32_t>(b)) >> 1;
        std::int32_t bias = (b < a) & (a ^ b) & 0x1;
        return static_cast<std::int16_t>(average + bias);
    }

    [[nodiscard]]
    AVEL_FINL std::int16_t average(std::int16_t a, std::int16_t b) {
        return (std::int32_t(a) + std::int32_t(b)) / 2;
    }

}

#endif //AVEL_SCALAR16I_HPP
