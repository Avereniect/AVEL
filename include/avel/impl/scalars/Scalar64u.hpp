//
// Created by avereniect on 2/27/22.
//

#ifndef AVEL_SCALAR64U_HPP
#define AVEL_SCALAR64U_HPP

namespace avel {

    [[nodiscard]]
    AVEL_FINL bool is_pow2(const std::uint64_t x) {
        return x && !(x & (x - 1));
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t divide_ceil(std::uint64_t x, std::uint64_t y) {
        std::uint64_t whole = x / y;
        std::uint64_t partial = x % y;
        return whole + bool(partial);
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t blend(std::uint64_t a, std::uint64_t b, bool m) {
        std::uint64_t mask = -m;
        return (a & ~mask) | (b & mask);
    }

}

#endif //AVEL_SCALAR64U_HPP
