//
// Created by avereniect on 6/11/22.
//

#ifndef AVEL_SCALAR8U_HPP
#define AVEL_SCALAR8U_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    std::uint8_t countl_one(std::uint8_t x);

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::uint8_t broadcast_bits<std::uint8_t>(bool x) {
        return -std::uint8_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t popcount(std::uint8_t x) {
        #if defined(AVEL_POPCNT)
        return _popcnt32(std::uint32_t{x});
        #else
        x = x - ((x >> 1) & 0x55);
        x = (x & 0x33) + ((x >> 2) & 0x33);
        x = (x + (x >> 4)) & 0x0F;
        x = x * 0x01u;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t countl_zero(std::uint8_t x) {
        #if defined(AVEL_BMI)
        return static_cast<uint8_t>(_lzcnt_u32(x) - 24);
        #elif defined(AVEL_X86)
        if (x) {
            return 7 - _bit_scan_reverse(x);
        } else {
            return 8;
        }
        #else
        return countl_one(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t countl_one(std::uint8_t x) {
        return countl_zero(std::uint8_t(~x));
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t countr_zero(std::uint8_t x) {
        #if defined(AVEL_BMI)
        return __tzcnt_u32(x | 0xFFFFFF00);
        #elif defined(AVEL_X86)
        if (x) {
            return _bit_scan_forward(x);
        } else {
            return 32;
        }
        #else
        std::uint8_t ret = 0x00;

        if (x == 0x00) {
            return 8;
        }

        x &= -x;

        std::uint8_t b = 0x00;
        b = bool(x & 0xAAu);
        ret |= (b << 0);
        b = bool(x & 0xCCu);
        ret |= (b << 1);
        b = bool(x & 0xF0u);
        ret |= (b << 2);

        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t countr_one(std::uint8_t x) {
        return countr_zero(std::uint8_t(~x));
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t bit_width(std::uint8_t x) {
        #if defined(AVEL_X86)
        return _bit_scan_reverse(x);
        #else
        if (x == 0) {
            return 0;
        }

        std::uint8_t ret = 0x00;

        bool b0 = x & 0xF0;
        ret += b0 * 4;
        x >>= (b0 * 4);

        bool b1 = x & 0xFC;
        ret += b1 * 2;
        x >>= (b1 * 2);

        bool b2 = x & 0xFE;
        ret += b2 * 1;

        return ret + 1;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t bit_floor(std::uint8_t x) {
        #if defined(AVEL_X86)
        if (x == 0) {
            return 0;
        }
        return 1 << bit_width(x);
        #else
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t bit_ceil(std::uint8_t x) {
        #if defined(AVEL_X86)
        auto width = bit_width(x);
        auto tmp = (x != 0) << width;
        return tmp << (tmp != x);
        #else
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        ++x;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::uint8_t x) {
        #if defined(AVEL_POPCNT)
        return popcount(x) == 1;
        #else
        return x && !(x & (x - 1));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t rotl(std::uint8_t x, std::uint8_t y) {
        y &= 0x7;
        if (y == 0) {
            return x;
        }

        return (x << y) | (x >> (32 - y));
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t rotr(std::uint8_t x, std::uint8_t y) {
        y &= 0x7;
        if (y == 0) {
            return x;
        }

        return (x >> y) | (x << (8 - y));
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int8_t neg_abs(std::uint8_t x) {
        return -x;
    }


    [[nodiscard]]
    AVEL_FINL std::uint8_t blend(std::uint8_t a, std::uint8_t b, bool m) {
        std::uint8_t mask = -m;
        return (a & ~mask) | (b & mask);
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t min(std::uint8_t a, std::uint8_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t max(std::uint8_t a, std::uint8_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t clamp(std::uint8_t x, std::uint8_t lo, std::uint8_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t midpoint(std::uint8_t a, std::uint8_t b) {
        std::uint8_t t0 = a & b & std::uint8_t{0x1};
        std::uint8_t t1 = (a | b) & std::uint8_t{0x1} & avel::broadcast_bits<std::uint8_t>(a > b);
        std::uint8_t t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t average(std::uint8_t a, std::uint8_t b) {
        return (a >> 1) + (b >> 1) + (a & b & std::uint8_t{0x1});
    }

}

#endif //AVEL_SCALAR8U_TESTS_HPP
