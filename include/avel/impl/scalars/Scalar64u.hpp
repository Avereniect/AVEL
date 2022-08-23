//
// Created by avereniect on 2/27/22.
//

#ifndef AVEL_SCALAR64U_HPP
#define AVEL_SCALAR64U_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    std::uint64_t countl_one(std::uint64_t x);

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::uint64_t broadcast_bits<std::uint64_t>(bool x) {
        return -std::uint64_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t popcount(std::uint64_t x) {
        #if defined(AVEL_POPCNT)
        return _mm_popcnt_u64(x);
        #else
        x = x - ((x >> 1) & 0x5555555555555555ul);
        x = (x & 0x3333333333333333u) + ((x >> 2) & 0x3333333333333333u);
        x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0Ful;
        x = (x * 0x0101010101010101u) >> 54;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t byteswap(std::uint64_t x) {
        #if defined(AVEL_X86)
        return _bswap64(x);
        #else
        std::uint64_t ret = 0x00;
        ret |= (x >> 56);
        ret |= (x >> 40) & (0x000000000000FF00ul);
        ret |= (x >> 32) & (0x0000000000FF0000ul);
        ret |= (x >> 24) & (0x00000000FF000000ul);
        ret |= (x << 24) & (0x000000FF00000000ul);
        ret |= (x << 32) & (0x0000FF0000000000ul);
        ret |= (x << 40) & (0x00FF000000000000ul);
        ret |= (x << 56);
        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t countl_zero(std::uint64_t x) {
        #if defined(AVEL_BMI)
        return _lzcnt_u64(x);
        #elif defined(AVEL_GCC) | defined(AVEL_CLANG)
        if (x) {
            return __builtin_clzl(x);
        } else {
            return 64;
        }
        #else
        return countl_one(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t countl_one(std::uint64_t x) {
        #if defined(AVEL_X86)
        return countl_zero(~x);
        #else
        std::uint64_t sum = (x == 0xFFFFFFFFFFFFFFFFul);

        bool b0 = (0xFFFFFFFF00000000u & x) == 0xFFFFFFFF00000000u;
        sum += (b0 * 16);
        x <<= (b0 * 16);

        bool b1 = (0xFFFF000000000000u & x) == 0xFFFF000000000000u;
        sum += (b1 * 16);
        x <<= (b1 * 16);

        bool b2 = (0xFF00000000000000u & x) == 0xFF00000000000000u;
        sum += (b2 * 8);
        x <<= (b2 * 8);

        bool b3 = (0xF000000000000000u & x) == 0xF000000000000000u;
        sum += (b3 * 4);
        x <<= (b3 * 4);

        bool b4 = (0xC000000000000000u & x) == 0xC000000000000000u;
        sum += (b4 * 2);
        x <<= (b4 * 2);

        bool b5 = (0x8000000000000000u & x) == 0x8000000000000000u;
        sum += (b5 * 1);

        return sum;
        #endif
    }
    [[nodiscard]]
    AVEL_FINL std::uint64_t countr_zero(std::uint64_t x) {
        #if defined(AVEL_BMI)
        return __tzcnt_u64(x);
        #elif defined(AVEL_X86)
        //TODO: Use bit scan instruction
        return {};
        #else
        std::uint64_t ret = 0x00;
        std::uint64_t b = 0x00;

        if (x == 0x00) {
            return 64;
        }

        x &= -x;

        b = bool(x & 0xAAAAAAAAu);
        ret |= b;
        b = bool(x & 0xCCCCCCCCu);
        ret |= (b << 1);
        b = bool(x & 0xF0F0F0F0u);
        ret |= (b << 2);
        b = bool(x & 0xFF00FF00u);
        ret |= (b << 3);
        b = bool(x & 0xFFFF0000u);
        ret |= (b << 4);

        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t countr_one(std::uint64_t x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t bit_floor(std::uint64_t x) {
        #if defined(AVEL_BMI) | (defined(AVEL_X86) & (defined(AVEL_GCC) | defined(AVEL_CLANG)))
        //TODO: Implement alternative implementation when leading zero count
        // isn't available
        std::uint64_t leading_zeros = countl_zero(x);
        if (leading_zeros == 64) {
            return 0x00;
        }

        return (1 << (63 - leading_zeros));
        #else
        if (x == 0) {
            return 0;
        }

        std::int32_t ret = 0x00;

        bool b0 = x & 0xFFFF'0000;
        ret += b0 * 16;
        x >>= (b0 * 16);

        bool b1 = x & 0xFFFF'FF00;
        ret += b1 * 8;
        x >>= (b1 * 8);

        bool b2 = x & 0xFFFF'FFF0;
        ret += b2 * 4;
        x >>= (b2 * 4);

        bool b3 = x & 0xFFFF'FFF0;
        ret += b3 * 2;
        x >>= (b3 * 2);

        bool b4 = x & 0xFFFF'FFFC;
        ret += b4 * 1;

        return ret + 1;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t bit_ceil(std::uint64_t x) {
        #if defined(AVEL_BMI) | (defined(AVEL_X86) & (defined(AVEL_GCC) | defined(AVEL_CLANG)))
        std::uint64_t leading_zeros = (64 - countl_zero(x));
        if (leading_zeros == 64) {
            return 0x00;
        }

        std::uint64_t tmp = (1 << leading_zeros);

        if (tmp != x) {
            tmp <<= 1;
        }

        return tmp;
        #else
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        ++x;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::uint64_t x) {
        #if defined(AVEL_POPCNT)
        return popcount(x) == 1;
        #else
        return x && !(x & (x - 1));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t rotl(std::uint64_t x, std::uint64_t y) {
        #if defined(AVEL_X86)
        return _lrotl(x, y);
        #else
        y &= 0x3F;
        if (y == 0) {
            return x;
        }

        return (x << y) | (x >> (64 - y));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t rotr(std::uint64_t x, std::uint64_t y) {
        #if defined(AVEL_X86)
        return _lrotr(x, y);
        #else
        y &= 0x3F;
        if (y == 0) {
            return x;
        }

        return (x >> y) | (x << (64 - y));
        #endif
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int64_t neg_abs(std::uint64_t x) {
        return -x;
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t blend(std::uint64_t a, std::uint64_t b, bool m) {
        std::uint64_t mask = -m;
        return (a & ~mask) | (b & mask);
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t min(std::uint64_t a, std::uint64_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t max(std::uint64_t a, std::uint64_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t clamp(std::uint64_t x, std::uint64_t lo, std::uint64_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t midpoint(std::uint64_t a, std::uint64_t b) {
        std::uint64_t t0 = a & b & std::uint64_t{0x1};
        std::uint64_t t1 = (a | b) & std::uint64_t{0x1} & avel::broadcast_bits<std::uint64_t>(a > b);
        std::uint64_t t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t average(std::uint64_t a, std::uint64_t b) {
        return (a >> 1) + (b >> 1) + (a & b & std::uint64_t{0x1});
    }

}

#endif //AVEL_SCALAR64U_HPP
