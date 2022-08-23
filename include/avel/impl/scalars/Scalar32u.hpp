//
// Created by avereniect on 2/22/22.
//

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    std::uint32_t countl_one(std::uint32_t x);

    std::uint32_t min(std::uint32_t a, std::uint32_t b);

    std::uint32_t max(std::uint32_t a, std::uint32_t b);

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::uint32_t broadcast_bits<std::uint32_t>(bool x) {
        return -std::uint32_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t popcount(std::uint32_t x) {
        #if defined(AVEL_POPCNT)
        return _popcnt32(x);
        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & 0x55555555u);
        x = (x & 0x33333333u) + ((x >> 2) & 0x33333333u);
        x = ((x + (x >> 4) & 0x0F0F0F0Fu) * 0x01010101u) >> 24;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t byteswap(std::uint32_t x) {
        #if defined(AVEL_X86)
        return _bswap(x);
        #else
        std::uint32_t ret = 0x00;
        ret |= x >> 24;
        ret |= (x >> 8) & (0x0000FF00);
        ret |= (x << 8) & (0x00FF0000);
        ret |= x << 24;
        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t countl_zero(std::uint32_t x) {
        #if defined(AVEL_BMI)
        return _lzcnt_u32(x);
        #elif defined(AVEL_X86)
        if (x) {
            return 31 - _bit_scan_reverse(x);
        } else {
            return 32;
        }
        #else
        return countl_one(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t countl_one(std::uint32_t x) {
        #if defined(AVEL_X86)
        return countl_zero(~x);
        #else

        // Divide and conquer implementation
        std::uint32_t sum = (x == 0xFFFFFFFF);

        bool b0 = (0xFFFF0000u & x) == 0xFFFF0000u;
        sum += (b0 * 16);
        x <<= (b0 * 16);

        bool b1 = (0xFF000000u & x) == 0xFF000000u;
        sum += (b1 * 8);
        x <<= (b1 * 8);

        bool b2 = (0xF0000000u & x) == 0xF0000000u;
        sum += (b2 * 4);
        x <<= (b2 * 4);

        bool b3 = (0xC0000000u & x) == 0xC0000000u;
        sum += (b3 * 2);
        x <<= (b3 * 2);

        bool b4 = (0x80000000u & x) == 0x80000000u;
        sum += (b4 * 1);

        return sum;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t countr_zero(std::uint32_t x) {
        #if defined(AVEL_BMI)
        return __tzcnt_u32(x);
        #elif defined(AVEL_X86)
        if (x) {
            return _bit_scan_forward(x);
        } else {
            return 32;
        }

        #else
        //Divide and conquer implementation
        std::uint32_t ret = 0x00;

        if (x == 0x00) {
            return 32;
        }

        x &= -x;

        std::uint32_t b = 0x00;
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
    AVEL_FINL std::uint32_t countr_one(std::uint32_t x) {
        #if defined(AVEL_X86)
        return countr_zero(~x);
        #else
        return countr_zero(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t bit_width(std::uint32_t x) {
        #if defined(AVEL_X86)
        return _bit_scan_reverse(x);
        #else
        if (x == 0) {
            return 0;
        }

        std::uint32_t ret = 0x00;

        bool b0 = x & 0xFFFF'0000;
        ret += b0 * 16;
        x >>= (b0 * 16);

        bool b1 = x & 0xFFFF'FF00;
        ret += b1 * 8;
        x >>= (b1 * 8);

        bool b2 = x & 0xFFFF'FFF0;
        ret += b2 * 4;
        x >>= (b2 * 4);

        bool b3 = x & 0xFFFF'FFFC;
        ret += b3 * 2;
        x >>= (b3 * 2);

        bool b4 = x & 0xFFFF'FFFE;
        ret += b4 * 1;

        return ret + 1;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t bit_floor(std::uint32_t x) {
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
        x = x | (x >> 16);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t bit_ceil(std::uint32_t x) {
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
        x |= x >> 16;
        ++x;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::uint32_t x) {
        #if defined(AVEL_POPCNT)
        return popcount(x) == 1;
        #else
        return x && !(x & (x - 1));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t rotl(std::uint32_t x, std::uint32_t y) {
        #if defined(AVEL_X86)
        return _rotl(x, y);
        #else
        y &= 0x1F;
        if (y == 0) {
            return x;
        }

        return (x << y) | (x >> (32 - y));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t rotr(std::uint32_t x, std::uint32_t y) {
        #if defined(AVEL_X86)
        return _rotr(x, y);
        #else
        y &= 0x1F;
        if (y == 0) {
            return x;
        }

        return (x >> y) | (x << (32 - y));
        #endif
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int32_t neg_abs(std::uint32_t x) {
        return -x;
        //auto y = ~x >> 31;
        //return (x ^ y) - y;
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t blend(std::uint32_t a, std::uint32_t b, bool m) {
        std::uint32_t mask = -m;
        return (a & ~mask) | (b & mask);
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t min(std::uint32_t a, std::uint32_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t max(std::uint32_t a, std::uint32_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t clamp(std::uint32_t x, std::uint32_t lo, std::uint32_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t midpoint(std::uint32_t a, std::uint32_t b) {
        std::uint32_t t0 = a & b & std::uint32_t{0x1};
        std::uint32_t t1 = (a | b) & std::uint32_t{0x1} & avel::broadcast_bits<std::uint32_t>(a > b);
        std::uint32_t t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t average(std::uint32_t a, std::uint32_t b) {
        return (a >> 1) + (b >> 1) + (a & b & std::uint32_t{0x1});
    }

}
