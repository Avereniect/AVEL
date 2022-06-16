//
// Created by avereniect on 2/22/22.
//

#ifndef AVEL_SCALAR32U_HPP
#define AVEL_SCALAR32U_HPP

namespace avel {

    [[nodiscard]]
    AVEL_FINL bool pop_count(std::uint32_t v) {
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
    AVEL_FINL bool is_pow2(const std::uint32_t x) {
        return x && !(x & (x - 1));
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t divide_ceil(std::uint32_t x, std::uint32_t y) {
        std::uint32_t whole = x / y;
        std::uint32_t partial = x % y;
        return whole + bool(partial);
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t blend(std::uint32_t a, std::uint32_t b, bool m) {
        std::uint32_t mask = -m;
        return (a & ~mask) | (b & mask);
    }

}

#endif //AVEL_SCALAR32U_HPP
