//
// Created by avereniect on 2/27/22.
//

#ifndef AVEL_SCALAR64I_HPP
#define AVEL_SCALAR64I_HPP

namespace avel {

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::int64_t broadcast_bits<std::int64_t>(bool x) {
        return -std::int64_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t popcount(std::int64_t x) {
        return std::int64_t(popcount(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t byteswap(std::int64_t x) {
        return std::int64_t(byteswap(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t countl_zero(std::int64_t x) {
        return std::int64_t(countl_zero(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t countl_one(std::int64_t x) {
        return std::int64_t(countl_zero(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t countr_zero(std::int64_t x) {
        return std::int64_t(countr_zero(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t countr_one(std::int64_t x) {
        return std::int64_t(countr_one(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t rotl(std::int64_t x, std::int64_t y) {
        return std::int64_t(rotl(std::uint64_t(x), std::uint64_t(y)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t rotr(std::int64_t x, std::int64_t y) {
        return std::int64_t(rotr(std::uint64_t(x), std::uint64_t(y)));
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int64_t neg_abs(std::int64_t x) {
        auto y = ~x >> 63;
        return (x ^ y) - y;
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t blend(std::int64_t a, std::int64_t b, bool m) {
        std::int64_t mask = -m;
        return (a & ~mask) | (b & mask);
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t min(std::int64_t a, std::int64_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t max(std::int64_t a, std::int64_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t clamp(std::int64_t x, std::int64_t lo, std::int64_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t midpoint(std::int64_t a, std::int64_t b) {
        std::int64_t mask = 0x8000000000000000ull;
        a ^= mask;
        b ^= mask;

        return std::int64_t(midpoint(std::uint64_t(a), std::uint64_t(b)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t average(std::int64_t a, std::int64_t b) {
        std::int64_t mask = 0x8000000000000000ull;
        a ^= mask;
        b ^= mask;

        return std::int64_t(average(std::uint64_t(a), std::uint64_t(b)));
    }

}

#endif //AVEL_SCALAR64I_HPP
