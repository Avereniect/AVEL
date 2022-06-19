//
// Created by avereniect on 2/22/22.
//

#ifndef AVEL_SCALAR32U_HPP
#define AVEL_SCALAR32U_HPP

namespace avel {

    [[nodiscard]]
    AVEL_FINL std::uint32_t blend(std::uint32_t a, std::uint32_t b, bool m) {
        std::uint32_t mask = -m;
        return (a & ~mask) | (b & mask);
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t neg_abs(std::uint32_t x) {
        auto y = ~x >> 31;
        return (x ^ y) - y;
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t pop_count(std::uint32_t v) {
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
    AVEL_FINL std::uint32_t leading_zero_count(std::uint32_t x) {
        #if defined(AVEL_BMI)
        return _lzcnt_u32(x);
        #else

        std::uint32_t sum = (x == 0);
        bool b = false;
        x = ~x;

        b = (leading_zero_count_32_masks[4] & x) == leading_zero_count_32_masks[4];
        sum += (b * 16);
        x <<= (b * 16);

        b = (leading_zero_count_32_masks[3] & x) == leading_zero_count_32_masks[3];
        sum += (b * 8);
        x <<= (b * 8);

        b = (leading_zero_count_32_masks[2] & x) == leading_zero_count_32_masks[2];
        sum += (b * 4);
        x <<= (b * 4);

        b = (leading_zero_count_32_masks[1] & x) == leading_zero_count_32_masks[1];
        sum += (b * 2);
        x <<= (b * 2);

        b = (leading_zero_count_32_masks[0] & x) == leading_zero_count_32_masks[0];
        sum += (b * 1);
        x <<= (b * 1);

        return sum;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t leading_one_count(std::uint32_t x) {
        return leading_zero_count(~x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t trailing_zero_count(std::uint32_t x) {
        #if defined(AVEL_BMI)
        return __tzcnt_u32(x);
        #else
        std::uint32_t ret = 0x00;
        std::uint32_t b = 0x00;

        if (x == 0x00) {
            return 32;
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
    AVEL_FINL std::uint32_t trailing_one_count(std::uint32_t x) {
        return trailing_zero_count(~x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t floor2(const std::uint32_t x) {
        std::uint32_t leading_zeros = leading_zero_count(x);
        if (leading_zeros == 32) {
            return 0x00;
        }

        return (1 << (31 - leading_zeros));
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t ceil2(const std::uint32_t x) {
        std::uint32_t leading_zeros = (32 - _lzcnt_u32(x));
        if (leading_zeros == 32) {
            return 0x00;
        }

        std::uint32_t tmp = (1 << leading_zeros);

        if (tmp != x) {
            tmp <<= 1;
        }

        return tmp;
    }

    [[nodiscard]]
    AVEL_FINL bool is_pow2(const std::uint32_t x) {
        #if defined(AVEL_POPCNT)
        return pop_count(x) == 1;
        #else
        return x && !(x & (x - 1));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool rotl(std::uint32_t x, std::uint32_t y) {
        #if defined(AVEL_X86)
        return _rotl(x, y);
        #else
        if (y == 0) {
            return x;
        }
        return (x << y) | (x >> (32 - y));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t divide_ceil(std::uint32_t x, std::uint32_t y) {
        std::uint32_t whole = x / y;
        std::uint32_t partial = x % y;
        return whole + bool(partial);
    }

}

#endif //AVEL_SCALAR32U_HPP
