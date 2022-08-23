//
// Created by avereniect on 2/22/22.
//

#ifndef AVEL_SCALAR32I_HPP
#define AVEL_SCALAR32I_HPP

namespace avel {

    [[nodiscard]]
    AVEL_FINL std::int32_t abs(std::int32_t x) {
        if (x < 0) {
            return -x;
        } else {
            return x;
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
    AVEL_FINL std::int32_t popcount(std::int32_t v) {
        #if defined(AVEL_POPCNT)
        return _popcnt32(v);
        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & std::uint32_t{0x55555555});                    // reuse input as temporary
        v = (v & std::uint32_t{0x33333333}) + ((v >> 2) & std::uint32_t{0x33333333});     // temp
        v = ((v + (v >> 4) & std::uint32_t{0xF0F0F0F}) * std::uint32_t{0x1010101}) >> 24; // count
        return v;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t countl_zero(std::int32_t x) {
        return std::int32_t(countl_zero(std::uint32_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL bool is_pow2(const std::int32_t x) {
        #if defined(AVEL_POPCNT)
        return popcount(x) == 1;
        #else
        return (x > 0) && !(x & (x - 1));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t blend(std::int32_t a, std::int32_t b, bool m) {
        std::uint32_t mask = -m;
        return (a & ~mask) | (b & mask);
    }

}

#endif //AVEL_SCALAR32I_HPP
