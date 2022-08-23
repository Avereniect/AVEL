//
// Created by avereniect on 6/11/22.
//

#ifndef AVEL_SCALAR16U_HPP
#define AVEL_SCALAR16U_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    std::uint16_t countl_one(std::uint16_t x);

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::uint16_t broadcast_bits<std::uint16_t>(bool x) {
        return -std::uint16_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t popcount(std::uint16_t x) {
        #if defined(AVEL_POPCNT)
        return _popcnt32(x);
        #else
        x = x - ((x >> 1) & 0x5555);
        x = (x & 0x3333u) + ((x >> 2) & 0x3333u);
        x = (x + (x >> 4)) & 0x0F0Fu;
        x = (x *  0x0101u) >> 16;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t byteswap(std::uint16_t x) {
        std::uint16_t ret = 0x00;

        ret |= (x << 8);
        ret |= (x >> 8);

        return ret;
    }


    [[nodiscard]]
    AVEL_FINL std::uint16_t countl_zero(std::uint16_t x) {
        #if defined(AVEL_BMI)
        return static_cast<std::uint16_t>(_lzcnt_u32(x) - 16);
        #elif defined(AVEL_X86)
        if (x) {
            return 15 - _bit_scan_reverse(x);
        } else {
            return 16;
        }
        #else
        return countl_one(std::uint16_t(~x));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t countl_one(std::uint16_t x) {
        #if defined(AVEL_X86)
        return countl_zero(std::uint16_t(~x));
        #else
        std::uint32_t sum = (x == 0xFFFFFFFF);

        bool b0 = (0xFF00u & x) == 0xFF00u;
        sum += (b0 * 8);
        x <<= (b0 * 8);

        bool b1 = (0xF000u & x) == 0xF000u;
        sum += (b1 * 4);
        x <<= (b1 * 4);

        bool b2 = (0xC000u & x) == 0xC000u;
        sum += (b2 * 2);
        x <<= (b2 * 2);

        bool b3 = (0x8000u & x) == 0x8000u;
        sum += (b3 * 1);

        return sum;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t countr_zero(std::uint16_t x) {
        #if defined(AVEL_BMI)
        return __tzcnt_u32(x | 0xFFFF0000);
        if (x) {
            return _bit_scan_forward(x | 0xFFFF0000);
        } else {
            return 16;
        }
        #else
        std::uint16_t ret = 0x00;

        if (x == 0x00) {
            return 32;
        }

        x &= -x;

        std::uint16_t b = 0x00;
        b = bool(x & 0xAAAAu);
        ret |= b;
        b = bool(x & 0xCCCCu);
        ret |= (b << 1);
        b = bool(x & 0xF0F0u);
        ret |= (b << 2);
        b = bool(x & 0xFF00u);
        ret |= (b << 3);

        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t countr_one(std::uint16_t x) {
        return countr_zero(std::uint16_t(~x));
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t bit_width(std::uint16_t x) {
        #if defined(AVEL_X86)
        return _bit_scan_reverse(x);
        #else
        if (x == 0) {
            return 0;
        }

        std::uint16_t ret = 0x00;

        bool b0 = x & 0xFF00;
        ret += b0 * 8;
        x >>= (b0 * 8);

        bool b1 = x & 0xFFF0;
        ret += b1 * 4;
        x >>= (b1 * 4);

        bool b2 = x & 0xFFFC;
        ret += b2 * 2;
        x >>= (b2 * 2);

        bool b3 = x & 0xFFFE;
        ret += b3 * 1;

        return ret + 1;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t bit_floor(std::uint16_t x) {
        #if defined(AVEL_X86)
        if (x == 0) {
            return 0;
        }
        return 1 << bit_width(x);
        #else
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t bit_ceil(std::uint16_t x) {
        #if defined(AVEL_X86)
        auto width = bit_width(x);
        auto tmp = (x != 0) << width;
        return tmp << (tmp != x);
        #else
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        ++x;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::uint16_t x) {
        #if defined(AVEL_POPCNT)
        return popcount(x) == 1;
        #else
        return x && !(x & (x - 1));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t rotl(std::uint16_t x, std::uint16_t y) {
        #if defined(AVEL_X86)
        return _rotwl(x, y);
        #else
        y &= 0x0F;
        if (y == 0) {
            return x;
        }

        return (x << y) | (x >> (16 - y));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t rotr(std::uint16_t x, std::uint16_t y) {
        #if defined(AVEL_X86)
        return _rotwr(x, y);
        #else
        y &= 0x0F;
        if (y == 0) {
            return x;
        }

        return (x >> y) | (x << (16 - y));
        #endif
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int16_t neg_abs(std::uint16_t x) {
        return -x;
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t blend(std::uint16_t a, std::uint16_t b, bool m) {
        std::uint16_t mask = -m;
        return (a & ~mask) | (b & mask);
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t min(std::uint16_t a, std::uint16_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t max(std::uint16_t a, std::uint16_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t clamp(std::uint16_t x, std::uint16_t lo, std::uint16_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t midpoint(std::uint16_t a, std::uint16_t b) {
        std::uint16_t t0 = a & b & std::uint16_t{0x1};
        std::uint16_t t1 = (a | b) & std::uint16_t{0x1} & avel::broadcast_bits<std::uint16_t>(a > b);
        std::uint16_t t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t average(std::uint16_t a, std::uint16_t b) {
        return (a >> 1) + (b >> 1) + (a & b & std::uint16_t{0x1});
    }

}

#endif //AVEL_SCALAR16U_HPP
